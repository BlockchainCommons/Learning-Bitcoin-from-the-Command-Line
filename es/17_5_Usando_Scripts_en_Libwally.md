# 17.5: Usando scripts en Libwally
 
> :information_source: **NOTA:** Esta sección se ha añadido recientemente al curso y es un borrador temprano que todavía puede estar pendiente de revisión. Lectura con advertencias.

En la Parte 3, al introducir Scripts, dijimos que era probable que creara transacciones usando scripts con una API y lo marcara como un tema para el futuro. Bueno, el futuro ha llegado.

## Crear el guión

Crear el script es la cosa _más fácil_ de hacer en Libwally. Tomemos el siguiente ejemplo, un simple [guión de rompecabezas](13_1_Escribiendo_Puzzle_Scripts.md) que hemos devuelto de vez en cuando:

```
OP_ADD 99 OP_EQUAL
```
Usando `btcc`, podemos serializar eso.
```
$ btcc OP_ADD 99 OP_EQUAL
warning: ambiguous input 99 is interpreted as a numeric value; use 0x99 to force into hexadecimal interpretation
93016387
```
Anteriormente construimos el script P2SH estándar a mano, pero Libwally puede hacerlo por usted.

En primer lugar, Libwally tiene que convertir el hex en bytes, ya que los bytes son la mayoría de lo que funciona con:
```
  int script_length = strlen(script)/2;
  unsigned char bscript[script_length];
    
  lw_response = wally_hex_to_bytes(script,bscript,script_length,&written);
```
Luego, ejecuta `wally_scriptpubkey_p2sh_from_bytes` con sus bytes, diciéndole a Libwally que también `HASH160` por usted:
```
  unsigned char p2sh[WALLY_SCRIPTPUBKEY_P2SH_LEN];
  			  
  lw_response = wally_scriptpubkey_p2sh_from_bytes(bscript,sizeof(bscript),WALLY_SCRIPT_HASH160,p2sh,WALLY_SCRIPTPUBKEY_P2SH_LEN,&written);
```
Si usted mirara los resultados de `p2sh`, vería que fue:
```
a9143f58b4f7b14847a9083694b9b3b52a4cea2569ed87
```
Lo que [puede recordar](10_2_Construyendo_la_Estructura_de_P2SH.md) se divide en:
```
a9 / 14 / 3f58b4f7b14847a9083694b9b3b52a4cea2569ed / 87
```
Ese es nuestro viejo amigo `OP_HASH160 3f58b4f7b14847a9083694b9b3b52a4cea2569ed OP_EQUAL`.

Básicamente, Libwally tomó su script de canje serializado, lo hasheó para usted con SHA-256 y RIPEMD-160, y aplicó el framing estándar para convertirlo en un P2SH adecuado; Hizo un trabajo similar en [§10.2](10_2_Construyendo_la_Estructura_de_P2SH.md), pero con un exceso de comandos de shell.

De hecho, puede volver a comprobar su trabajo utilizando los mismos comandos de §10.2:
```
$ redeemScript="93016387"
$ echo -n $redeemScript | xxd -r -p | openssl dgst -sha256 -binary | openssl dgst -rmd160
(stdin)= 3f58b4f7b14847a9083694b9b3b52a4cea2569ed
```
## Crear una transacción

Con el fin de hacer uso de ese `pubScriptKey` que acaba de crear, es necesario crear una transacción e incrustar el `pubScriptKey` dentro (y este es el gran cambio de `bitcoin-cli`: en realidad se puede crear a mano una transacción con un script P2SH).

El proceso de crear una transacción en Libwally es muy intenso, al igual que el proceso para crear un PSBT, y por lo que vamos a esbozarlo, tomando un atajo importante, y luego dejar un método sin atajos para la investigación futura.

Crear una transacción en sí es bastante fácil: sólo tiene que decir `wally_tx_init_alloc` su número de versión, su tiempo de bloqueo, y el número de entradas y salidas:
```
  struct wally_tx *tx;
  lw_response = wally_tx_init_alloc(2,0,1,1,&tx);
```
¡Llenar esas entradas y salidas es donde las cosas se ponen complicadas!

### Crear una salida de transacción

Para crear una salida, le dice a `wally_tx_output_init_alloc` cuántos satoshis está gastando y le das el script de bloqueo:
```
  struct wally_tx_output *tx_output;
  lw_response = wally_tx_output_init_alloc(95000,p2sh,sizeof(p2sh),&tx_output);
```
Esa parte en realidad no era difícil en absoluto y que le permitió a la larga incrustar un P2SH en un `vout`.

Un comando más lo añade a su transacción:
```
  lw_response = wally_tx_add_output(tx,tx_output);
```
### Crear una entrada de transacción

Crear la entrada es mucho más difícil porque usted tiene que acumular información en las rutinas de creación, no todas las cuales son intuitivamente accesibles cuando usa Libwally. Así que, en lugar de ir tan profundo en la maleza, aquí es donde tomamos nuestro atajo. Escribimos nuestro código para que pase el código hexadecimal de una transacción que ya ha sido creada, y luego simplemente reutilizamos la entrada.

La conversión del código hexadecimal se realiza con `wally_tx_from_hex`:
```
  struct wally_tx *utxo;
  lw_response = wally_tx_from_hex(utxo_hex,0,&utxo);
```
A continuación, puede tomar prestadas las entradas de su hexcode para crear una entrada con Libwally:
```
  struct wally_tx_input *tx_input;
  lw_response = wally_tx_input_init_alloc(utxo->inputs[0].txhash,sizeof(utxo->inputs[0].txhash),utxo->inputs[0].index,0,utxo->inputs[0].script,utxo->inputs[0].script_len,utxo->inputs[0].witness,&tx_input);
  assert(lw_response == WALLY_OK);						
```
Como es de esperar, a continuación, agregue esa entrada a su transacción:
```
  lw_response = wally_tx_add_input(tx,tx_input);
```
> **NOTA** Obviamente, querrá poder crear sus propias entradas si está usando Libwally para aplicaciones reales, pero esto es un primer paso. Y en realidad puede ser útil para integrar con `bitcoin-cli`, como veremos en [§17.7](17_7_Integrando_Libwally_y_Bitcoin-CLI.md).

### Imprimir una transacción

Teóricamente podría firmar y enviar esta transacción desde su programa C construido en Libwally, pero siguiendo con la idea de que solo estamos usando un simple programa C para sustituir a un P2SH, vamos a imprimir el nuevo hexadecimal. Esto se hace con la ayuda de `wally_tx_to_hex`:
```
  char *tx_hex;
  lw_response = wally_tx_to_hex(tx,0, &tx_hex);

  printf("%s\n",tx_hex);
```
Vamos a mostrar cómo hacer uso de eso en §17.7.

## Pruebe su script de reemplazo

Puede tomar el código de prueba del [directorio src](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/tree/master/src/17_5_replacewithscript.c) y compilarlo:
```
$  cc replacewithscript.c -lwallycore -o replacewithscript
```
Después, preparar una transacción hexagonal y un script hexadecimal serializado:
```
hex=020000000001019527cebb072524a7961b1ba1e58fc18dd7c6fc58cd6c1c45d7e1d8fc690b006e0000000017160014cc6e8522f0287b87b7d0a83629049c2f2b0e972dfeffffff026f8460000000000017a914ba421212a629a840492acb2324b497ab95da7d1e87306f0100000000001976a914a2a68c5f9b8e25fdd1213c38d952ab2be2e271be88ac02463043021f757054fa61cfb75b64b17230b041b6d73f25ff9c018457cf95c9490d173fb4022075970f786f24502290e8a5ed0f0a85a9a6776d3730287935fb23aa817791c01701210293fef93f52e6ce8be581db62229baf116714fcb24419042ffccc762acc958294e6921b00

script=93016387
```
A continuación, puede ejecutar el programa de reemplazo:
```
$ ./replacewithscript $hex $script
02000000019527cebb072524a7961b1ba1e58fc18dd7c6fc58cd6c1c45d7e1d8fc690b006e0000000017160014cc6e8522f0287b87b7d0a83629049c2f2b0e972d0000000001187301000000000017a9143f58b4f7b14847a9083694b9b3b52a4cea2569ed8700000000
```
A continuación, puede ver los resultados con `bitcoin-cli`:
```
$ bitcoin-cli decoderawtransaction $newhex
{
  "txid": "f4e7dbab45e759a7ac6e2fb0f10720cd29d047efad89fe1b569f5f4ba61fd8e6",
  "hash": "f4e7dbab45e759a7ac6e2fb0f10720cd29d047efad89fe1b569f5f4ba61fd8e6",
  "version": 2,
  "size": 106,
  "vsize": 106,
  "weight": 424,
  "locktime": 0,
  "vin": [
    {
      "txid": "6e000b69fcd8e1d7451c6ccd58fcc6d78dc18fe5a11b1b96a7242507bbce2795",
      "vout": 0,
      "scriptSig": {
        "asm": "0014cc6e8522f0287b87b7d0a83629049c2f2b0e972d",
        "hex": "160014cc6e8522f0287b87b7d0a83629049c2f2b0e972d"
      },
      "sequence": 0
    }
  ],
  "vout": [
    {
      "value": 0.00095000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_HASH160 3f58b4f7b14847a9083694b9b3b52a4cea2569ed OP_EQUAL",
        "hex": "a9143f58b4f7b14847a9083694b9b3b52a4cea2569ed87",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2My2ApqGcoNXYceZC4d7fipBu4GodkbefHD"
        ]
      }
    }
  ]
}
```
El `vin` debe coincidir con la entrada que sustituyó en, pero es el `vout` lo que es emocionante: que ha creado una transacción con un `scripthash`!

## Resumen: Usando scripts en Libwally

Crear transacciones en Libwally es otro tema que podría ocupar todo un capítulo, pero lo bueno es que una vez que da este salto, puede introducir un script P2SH `PubKey`, y esa parte por sí sola es bastante fácil. Aunque la metodología detallada en este capítulo requiere que tenga una cadena hexadecimal de la transacción ya en la mano (probablemente creado con `bitcoin-cli`) si profundiza en Libwally, puede hacerlo todo usted mismo.

> :fire: ***Cuál es el poder de los guiones en Libwally?*** Simplemente, puede hacer algo que no podía hacer antes: crear una transacción bloqueada con un P2SH arbitrario.

## ¿Qué es lo siguiente?

Más información sobre "Programar Bitcoin con Libwally" en [§17.6: Usar otras funciones en Libwally](17_6_Usando_Otras_Funciones_en_Libwally.md).
