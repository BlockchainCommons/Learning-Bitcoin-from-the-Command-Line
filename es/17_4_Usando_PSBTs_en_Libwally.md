# 17.4: Uso de PSBTs en Libwally

> :information_source: **NOTA:** Esta sección se ha añadido recientemente al curso y es un borrador temprano que todavía puede estar pendiente de revisión. Lectura con advertencias.

Aprendió todo sobre las Transacciones de Bitcoin Parcialmente Firmadas (PSBTs) en [§7.1](07_1_Creando_una_Transaccion_Bitcoin_Parcialmente_Firmada.md) y [§7.2](07_2_Usando_una_Transaccion_Bitcoin_Parcialmente_Firmada.md), y como vio en [§7.3: Integración con Wallets de Hardware](07_3_Integrando_con_Hardware_Wallets.md), una de sus principales ventajas es poder integrarse con nodos fuera de línea, como Hardware Wallets. HWI le permitió pasar comandos a una Cartera de Hardware, pero ¿qué utiliza la propia cartera para gestionar los PSBTs? Resulta que puede usar algo como Libwally, como demuestra esta sección.

Básicamente, Libwally tiene toda la funcionalidad de PSBT, así que si hay algo que puede hacer con `bitcoind`, también puede hacerlo usando Libwally, incluso si su dispositivo está desconectado. Lo que sigue es la más simple introducción a lo que es un tema muy complejo.

## Convertir un PSBT

Convertir un PSBT en la estructura interna de Libwally es increíblemente fácil, simplemente ejecuta `wally_psbt_from_base64` con un PSBT base64 - que son las salidas producidas por `bitcoin-cli`, tales como:
`cHNidP8BAJoCAAAAAri6BLjKQZGO9Y1iVIYbxlxBJ2kqsTPWnxGaH4HrSjxbAAAAAAD+////leV0hwJ0fO40RmhuFVIYtO16ktic2J4vJFLAsT5TM8cBAAAAAP7///8CYOMWAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBU+gBwAAAAAAFgAU9Ojd5ds3CJi1fIRWbj92CYhQgX0AAAAAAAEBH0BCDwAAAAAAFgAUABk8i/Je8Fb41FcaHD9lEj5f54giBgMBaNlILisC1wJ/tKie3FStqhrfcJM09kfQobBTOCiuxRiaHVILVAAAgAEAAIAAAACAAAAAADkCAAAAAQEfQEIPAAAAAAAWABQtTxOfqohTBNFWFqFm0tUVdK9KXSIGAqATz5xLX1aJ2SUwNqPkd8+YaJYm94FMlPCScm8Rt0GrGJodUgtUAACAAQAAgAAAAIAAAAAAAAAAAAAAIgID2UK1nupSfXC81nmB65XZ+pYlJp/W6wNk5FLt5ZCSx6kYmh1SC1QAAIABAACAAAAAgAEAAAABAAAAAA==`

Sin embargo, es un poco más difícil lidiar con el resultado, porque Libwally lo convierte en una estructura muy compleja `wally_psbt` .

Así es como se define en `/usr/include/wally_psbt.h`:
```c
struct wally_psbt {
    unsigned char magic[5];
    struct wally_tx *tx;
    struct wally_psbt_input *inputs;
    size_t num_inputs;
    size_t inputs_allocation_len;
    struct wally_psbt_output *outputs;
    size_t num_outputs;
    size_t outputs_allocation_len;
    struct wally_unknowns_map *unknowns;
    uint32_t version;
};

struct wally_psbt_input {
    struct wally_tx *non_witness_utxo;
    struct wally_tx_output *witness_utxo;
    unsigned char *redeem_script;
    size_t redeem_script_len;
    unsigned char *witness_script;
    size_t witness_script_len;
    unsigned char *final_script_sig;
    size_t final_script_sig_len;
    struct wally_tx_witness_stack *final_witness;
    struct wally_keypath_map *keypaths;
    struct wally_partial_sigs_map *partial_sigs;
    struct wally_unknowns_map *unknowns;
    uint32_t sighash_type;
};

struct wally_psbt_output {
    unsigned char *redeem_script;
    size_t redeem_script_len;
    unsigned char *witness_script;
    size_t witness_script_len;
    struct wally_keypath_map *keypaths;
    struct wally_unknowns_map *unknowns;
};
```
Estos a su vez utilizan algunas estructuras de transacción definidas en `/usr/include/wally_transaction.h`:
```c
struct wally_tx {
    uint32_t version;
    uint32_t locktime;
    struct wally_tx_input *inputs;
    size_t num_inputs;
    size_t inputs_allocation_len;
    struct wally_tx_output *outputs;
    size_t num_outputs;
    size_t outputs_allocation_len;
};

struct wally_tx_output {
    uint64_t satoshi;
    unsigned char *script;
    size_t script_len;
    uint8_t features;
};
```
¡Hay mucho allí! Aunque mucho de esto debería ser familiar de los capítulos penetrantes, es un poco abrumador verlo todo expuesto en las estructuras C.

## Leer un PSBT convertido

Obviamente, puede leer cualquier cosa de una estructura PSBT llamando a los elementos individuales de las diversas subestructuras. El siguiente es un breve resumen que muestra cómo tomar algunos de los elementos. 

Aquí hay un ejemplo de la recuperación de los valores y `scriptPubKeys` de las entradas:

```c
  int inputs = psbt->num_inputs;
  printf("TOTAL INPUTS: %i\n",inputs);

  for (int i = 0 ; i < inputs ; i++) {
    printf("\nINPUT #%i: %i satoshis\n",i, psbt->inputs[i].witness_utxo->satoshi);

    char *script_hex;
    wally_hex_from_bytes(psbt->inputs[i].witness_utxo->script,psbt->inputs[i].witness_utxo->script_len,&script_hex);
    printf("scriptPubKey: %s\n",script_hex);
    wally_free_string(script_hex);
    
  }
```
Este patrón de programación se utilizará en muchas partes del PSBT. Se observa el tamaño de la matriz de entradas, luego se pasa a través de él, recuperando lo que desea ver (en este caso, satoshis y scripts).

Aquí hay un ejemplo similar para las salidas:
```c
  int outputs = psbt->num_outputs;
  printf("\nTOTAL OUTPUTS: %i\n",outputs);### Pruebe su lector PSBT

  for (int i = 0 ; i < outputs ; i++) {

    char *pubkey_hex;
    wally_hex_from_bytes(psbt->tx->outputs[i].script,psbt->tx->outputs[i].script_len,&pubkey_hex);
    printf("\nINPUT #%i\n",i);
    printf("scriptPubKey: %s\n",pubkey_hex);
    wally_free_string(pubkey_hex);    
  }
```
Obviamente, hay mucho más que podría ver en los PSBT. De hecho, observar es el punto principal de un PSBT: puede verificar entradas y salidas desde un ordenador sin conexión.

> :warning: **ADVERTENCIA:** Estas funciones de lectura son muy rudimentarias y no funcionan correctamente para situaciones extremadamente normales como una entrada o salida que todavía está vacía o que incluye un `non_witness_utxo`. Esto dará un error segfault si no se entrega un PSBT exactamente esperado. Un lector real tendría que ser considerablemente más robusto, para cubrir todas las situaciones posibles, pero eso queda como un ejercicio para el lector.

### Pruebe su lector PSBT

De nuevo, el código para este lector PSBT (extremadamente rudimentario y específico) está en el [src directory](../src/17_4_examinepsbt.c). 

Puede compilarlo como siempre:
```
$ cc examinepsbt.c -lwallycore -o examinepsbt
```
El siguiente PSBT de [§7.3](07_3_Integrando_con_Hardware_Wallets.md) se puede utilizar para pruebas, ya que coincide con los criterios muy estrechos requeridos por esta implementación limitada:

```
psbt=cHNidP8BAJoCAAAAAri6BLjKQZGO9Y1iVIYbxlxBJ2kqsTPWnxGaH4HrSjxbAAAAAAD+////leV0hwJ0fO40RmhuFVIYtO16ktic2J4vJFLAsT5TM8cBAAAAAP7///8CYOMWAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBU+gBwAAAAAAFgAU9Ojd5ds3CJi1fIRWbj92CYhQgX0AAAAAAAEBH0BCDwAAAAAAFgAUABk8i/Je8Fb41FcaHD9lEj5f54giBgMBaNlILisC1wJ/tKie3FStqhrfcJM09kfQobBTOCiuxRiaHVILVAAAgAEAAIAAAACAAAAAADkCAAAAAQEfQEIPAAAAAAAWABQtTxOfqohTBNFWFqFm0tUVdK9KXSIGAqATz5xLX1aJ2SUwNqPkd8+YaJYm94FMlPCScm8Rt0GrGJodUgtUAACAAQAAgAAAAIAAAAAAAAAAAAAAIgID2UK1nupSfXC81nmB65XZ+pYlJp/W6wNk5FLt5ZCSx6kYmh1SC1QAAIABAACAAAAAgAEAAAABAAAAAA==
```

Ejecute `examinepsbt` con ese PSBT, y debería ver los scripts en las entradas y salidas:
```
$ ./examinepsbt $psbt
TOTAL INPUTS: 2

INPUT #0: 1000000 satoshis
scriptPubKey: 001400193c8bf25ef056f8d4571a1c3f65123e5fe788

INPUT #1: 1000000 satoshis
scriptPubKey: 00142d4f139faa885304d15616a166d2d51574af4a5d

TOTAL OUTPUTS: 2

INPUT #0
scriptPubKey: 0014c772d6f95542e11ef11e8efc7c7a69830ad38a05

INPUT #1
scriptPubKey: 0014f4e8dde5db370898b57c84566e3f76098850817d
```
Y por supuesto, puede comprobar esto con el comando `decodepsbt` RPC para `bitcoin-cli`:
```
$ bitcoin-cli decodepsbt $psbt
{
  "tx": {
    "txid": "45f996d4ff8c9e9ab162f611c5b6ad752479ede9780f9903bdc80cd96619676d",
    "hash": "45f996d4ff8c9e9ab162f611c5b6ad752479ede9780f9903bdc80cd96619676d",
    "version": 2,
    "size": 154,
    "vsize": 154,
    "weight": 616,
    "locktime": 0,
    "vin": [
      {
        "txid": "5b3c4aeb811f9a119fd633b12a6927415cc61b8654628df58e9141cab804bab8",
        "vout": 0,
        "scriptSig": {
          "asm": "",
          "hex": ""
        },
        "sequence": 4294967294
      },
      {
        "txid": "c733533eb1c052242f9ed89cd8927aedb41852156e684634ee7c74028774e595",
        "vout": 1,
        "scriptSig": {
          "asm": "",
          "hex": ""
        },
        "sequence": 4294967294
      }
    ],
    "vout": [
      {
        "value": 0.01500000,
        "n": 0,
        "scriptPubKey": {
          "asm": "0 c772d6f95542e11ef11e8efc7c7a69830ad38a05",
          "hex": "0014c772d6f95542e11ef11e8efc7c7a69830ad38a05",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1qcaedd724gts3aug73m78c7nfsv9d8zs9q6h2kd"
          ]
        }
      },
      {
        "value": 0.00499791,
        "n": 1,
        "scriptPubKey": {
          "asm": "0 f4e8dde5db370898b57c84566e3f76098850817d",
          "hex": "0014f4e8dde5db370898b57c84566e3f76098850817d",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1q7n5dmewmxuyf3dtus3txu0mkpxy9pqtacuprak"
          ]
        }
      }
    ]
  },
  "unknown": {
  },
  "inputs": [
    {
      "witness_utxo": {
        "amount": 0.01000000,
        "scriptPubKey": {
          "asm": "0 00193c8bf25ef056f8d4571a1c3f65123e5fe788",
          "hex": "001400193c8bf25ef056f8d4571a1c3f65123e5fe788",
          "type": "witness_v0_keyhash",
          "address": "tb1qqqvnezljtmc9d7x52udpc0m9zgl9leugd2ur7y"
        }
      },
      "bip32_derivs": [
        {
          "pubkey": "030168d9482e2b02d7027fb4a89edc54adaa1adf709334f647d0a1b0533828aec5",
          "master_fingerprint": "9a1d520b",
          "path": "m/84'/1'/0'/0/569"
        }
      ]
    },
    {
      "witness_utxo": {
        "amount": 0.01000000,
        "scriptPubKey": {
          "asm": "0 2d4f139faa885304d15616a166d2d51574af4a5d",
          "hex": "00142d4f139faa885304d15616a166d2d51574af4a5d",
          "type": "witness_v0_keyhash",
          "address": "tb1q948388a23pfsf52kz6skd5k4z4627jja2evztr"
        }
      },
      "bip32_derivs": [
        {
          "pubkey": "02a013cf9c4b5f5689d9253036a3e477cf98689626f7814c94f092726f11b741ab",
          "master_fingerprint": "9a1d520b",
          "path": "m/84'/1'/0'/0/0"
        }
      ]
    }
  ],
  "outputs": [
    {
    },
    {
      "bip32_derivs": [
        {
          "pubkey": "03d942b59eea527d70bcd67981eb95d9fa9625269fd6eb0364e452ede59092c7a9",
          "master_fingerprint": "9a1d520b",
          "path": "m/84'/1'/0'/1/1"
        }
      ]
    }
  ],
  "fee": 0.00000209
}
```
Puede ver la entrada en satoshis y el `scriptPubKey` claramente listados en las `inputs` y el nuevo `scriptPubKey`s en el `vout` de la `tx`. 

¡Así que todo está ahí para unirlo!

## Crear un PSBT

Como se señaló al principio de esta sección, todas las funciones necesarias para crear y procesar las PSBT están disponibles en Libwally. En realidad, correr a través del proceso de hacerlo es lo suficientemente complejo como para que esté más allá del alcance de esta sección, pero aquí hay una rápida ejecución de las funciones requeridas. Tenga en cuenta que los [documentos](https://wally.readthedocs.io/en/latest/psbt/) están desactualizados para PSBTs, por lo que deberá consultar `/usr/include/wally_psbt.h` para información completa.
Como se discute en [§7.1](07_1_Creando_una_Transaccion_Bitcoin_Parcialmente_Firmada.md) hay varios roles involucrados en la creación de PSBTs

### Tomar el rol de creador

El rol de creador se encarga de crear un PSBT con al menos una entrada.

Un PSBT se crea con un uso simple de `wally_psbt_init_alloc`, indicándole cuántas entradas y salidas añadirá finalmente:
```c
  struct wally_psbt *psbt;
  lw_response = wally_psbt_init_alloc(0,1,1,0,&psbt);
```
Pero lo que usted tiene todavía no es un PSBT legal, debido a la falta de insumos. Puede crearlos creando una transacción y estableciéndola como la transacción global en el PSBT, que actualiza todas las entradas y salidas:
```c
  struct wally_tx *gtx;
  lw_response = wally_tx_init_alloc(0,0,1,1,&gtx);
  lw_response = wally_psbt_set_global_tx(psbt,gtx);
```
## Pruebe su creación de PSBT

En este punto, debería tener un PSBT vacío, pero funcional, que puede ver compilando y ejecutando [el programa](../src/17_4_createemptypsbt.c).
```
$ cc createemptypsbt.c -lwallycore -o createemptypsbt
$ ./createemptypsbt 
cHNidP8BAAoAAAAAAAAAAAAAAA==
```
Incluso usted puede usar `bitcoin-cli` para probar el resultado:
```
$ psbt=$(./createpsbt)
$ bitcoin-cli decodepsbt $psbt
{
  "tx": {
    "txid": "f702453dd03b0f055e5437d76128141803984fb10acb85fc3b2184fae2f3fa78",
    "hash": "f702453dd03b0f055e5437d76128141803984fb10acb85fc3b2184fae2f3fa78",
    "version": 0,
    "size": 10,
    "vsize": 10,
    "weight": 40,
    "locktime": 0,
    "vin": [
    ],
    "vout": [
    ]
  },
  "unknown": {
  },
  "inputs": [
  ],
  "outputs": [
  ],
  "fee": 0.00000000
}
```

## Asumir el resto de los roles

Al igual que con la lectura de PSBT, estamos introduciendo el concepto de creación de PSBT, y dejando el resto como un ejercicio para el lector.

A continuación se presenta una lista aproximada de funciones para cada función; se necesitarán más funciones para crear algunos de los elementos que se añaden a las PSBT.

**Creador:**

* wally_psbt_init_alloc
* wally_psbt_set_global_tx

**Actualizador:**
* wally_psbt_input_set_non_witness_utxo
* wally_psbt_input_set_witness_utxo
* wally_psbt_input_set_redeem_script
* wally_psbt_input_set_witness_script
* wally_psbt_input_set_keypaths
* wally_psbt_input_set_unknowns
* wally_psbt_output_set_redeem_script
* wally_psbt_output_set_witness_script
* wally_psbt_output_set_keypaths
* wally_psbt_output_set_unknowns

**Firmante:**
* wally_psbt_input_set_partial_sigs
* wally_psbt_input_set_sighash_type
* wally_psbt_sign

**Combinador:**
* wally_psbt_combine

**Finalizador:**
* wally_psbt_finalize
* wally_psbt_input_set_final_script_sig
* wally_psbt_input_set_final_witness## Resumen: Usando PSBTs en Libwally

**Extractor:**
* wally_psbt_extract

## Resumen: Usando PSBTs en Libwally

Esta sección podría ser un capítulo completo, ya que trabajar con PSBTs a un nivel bajo es un trabajo muy intenso que requiere una manipulación mucho más intensiva de las entradas y salidas que en [Capítulo 7](07_0_Expandiendo_las_Transacciones_Bitcoin_con_PSBTs.md). En su lugar, esta sección muestra lo básico: cómo extraer información de un PSBT, y cómo empezar a crear uno.
> :fire: ***Cuál es el poder de los PSBTs en Libwally?*** Obviamente, ya se puede hacer todo esto en `bitcoin-cli`, y es más simple porque Bitcoin Core maneja una gran cantidad de trabajo pesado. La ventaja de usar Libwally es que se puede ejecutar sin conexión, por lo que podría ser Libwally que está sentado en el otro lado de un dispositivo de hardware que su `bitcoin-cli` se está comunicando con HWI. Este es, de hecho, uno de los puntos principales de las PSBT: poder manipular transacciones parcialmente firmadas sin necesidad de un nodo completo. Libwally lo hace posible.

## ¿Qué es lo siguiente?

Más información sobre "Programar Bitcoin con Libwally" en [17.5: Usar scripts en Libwally](17_5_Usando_Scripts_en_Libwally.md).
