# 17.7: Integración de Libwally y Bitcoin-CLI

> :information_source: **NOTA:** Esta sección se ha añadido recientemente al curso y es un borrador temprano que todavía puede estar pendiente de revisión. Lectura con advertencias.

Libwally es muy limitada. Se trata de manipular semillas, llaves, direcciones y otros elementos de billeteras, con algunas funciones adicionales relacionadas con las transacciones y PSBTs que podrían ser útiles para servicios que no están conectados a nodos completos en Internet. En última instancia, sin embargo, va a necesitar servicios de un nodo completo para tomar ventaja de Libwally.
Esta sección final ofrecerá algunos ejemplos de uso de programas de Libwally para complementar un entorno `bitcoin-cli`. Aunque estos ejemplos implican que estos servicios están todos en la misma máquina, pueden volverse aún más poderosos si el servicio `bitcoin-cli` está directamente conectado a Internet y el servicio Libwally no lo está.

## Compartir una transacción

[§17.5: Usar scripts en Libwally](17_5_Usando_Scripts_en_Libwally.md) detalló cómo Libwally podría usarse para reescribir una transacción existente, para hacer algo que `bitcoin-cli` no puede: producir una transacción que contenga un P2SH único. Obviamente, este es un bloque de construcción; si decide profundizar más en Libwally creará transacciones completas por su cuenta. Pero esta metodología abreviada también tiene su propio uso: muestra cómo las transacciones se pueden pasar de ida y vuelta entre `bitcoin-cli` y Libwally, demostrando un primer ejemplo de su uso de una manera complementaria.

Para demostrar completamente esta metodología, creará una transacción con `bitcoin-cli`, usando este UTXO:
```
  {
    "txid": "c0a110a7a84399b98052c6545018873b13ee3128fa74f7a697779174a36ea33a",
    "vout": 1,
    "address": "mvLyH7Rs45c16FG2dfV7uuTKV6pL92kWxo",
    "label": "",
    "scriptPubKey": "76a914a2a68c5f9b8e25fdd1213c38d952ab2be2e271be88ac",
    "amount": 0.00094000,
    "confirmations": 17375,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([ce0c7e14/0'/0'/5']0368d0fffa651783524f8b934d24d03b32bf8ff2c0808943a556b3d74b2e5c7d65)#qldtsl65",
    "safe": true
  }
```
Por ahora, usted sabe cómo configurar una transacción con `bitcoin-cli`:

Aunque puso un destinatario y una cantidad en la salida, es irrelevante porque tendrá que reescribirlas. Un código un poco más elegante podría leer la información `vout` existente antes de reescribir, pero estamos manteniendo las cosas muy cerca de nuestro [código original](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/tree/master/src/17_5_replacewithscript.c).

Aquí está el cambio necesario, para que pueda especificar el monto de satoshis para la salida `vout`, sin tener que ponerlo a fuego como en el original:
```
...
  int satoshis = atoi(argv[3]);
...
  lw_response = wally_tx_output_init_alloc(satoshis,p2sh,sizeof(p2sh),&tx_output);
...
```
Entonces usted ejecuta las cosas como antes:
```
$ newtxhex=$(./replacewithscript $rawtxhex $script 9000)
```
Así era la transacción original:
```
$ bitcoin-cli decoderawtransaction $rawtxhex
{
  "txid": "438d50edd7abeaf656c5abe856a00a20af5ff08939df8fdb9f8bfbfb96234fcb",
  "hash": "438d50edd7abeaf656c5abe856a00a20af5ff08939df8fdb9f8bfbfb96234fcb",
  "version": 2,
  "size": 82,
  "vsize": 82,
  "weight": 328,
  "locktime": 0,
  "vin": [
    {
      "txid": "c0a110a7a84399b98052c6545018873b13ee3128fa74f7a697779174a36ea33a",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 0.00090000,
      "n": 0,
      "scriptPubKey": {
        "asm": "0 2621b0465d851d6ba4e61a667b7ab13e3740e0f7",
        "hex": "00142621b0465d851d6ba4e61a667b7ab13e3740e0f7",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1qycsmq3jas5wkhf8xrfn8k7438cm5pc8h9ae2k0"
        ]
      }
    }
  ]
}
```
Y aquí está la transacción reescrita por Libwally para usar un P2SH:
```
standup@btctest:~/c$ bitcoin-cli decoderawtransaction $newtxhex
{
  "txid": "badb57622ab5fe029fc1a71ace9f7b76c695f933bceb0d38a155c2e5c984f4e9",
  "hash": "badb57622ab5fe029fc1a71ace9f7b76c695f933bceb0d38a155c2e5c984f4e9",
  "version": 2,
  "size": 83,
  "vsize": 83,
  "weight": 332,
  "locktime": 0,
  "vin": [
    {
      "txid": "c0a110a7a84399b98052c6545018873b13ee3128fa74f7a697779174a36ea33a",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 0
    }
  ],
  "vout": [
    {
      "value": 0.00090000,
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
Después puede firmarlo como de costumbre con `bitcoin-cli`:
```
$ signedtx=$(bitcoin-cli signrawtransactionwithwallet $newtxhex | jq -r '.hex')
```
Y como puede ver, el resultado es una transacción legítima lista para salir a la red Bitcoin:
```
$ bitcoin-cli decoderawtransaction $signedtx
{
  "txid": "3061ca8c01d029c0086adbf8b7d4280280c8aee151500bab7c4f783bbc8e75e6",
  "hash": "3061ca8c01d029c0086adbf8b7d4280280c8aee151500bab7c4f783bbc8e75e6",
  "version": 2,
  "size": 189,
  "vsize": 189,
  "weight": 756,
  "locktime": 0,
  "vin": [
    {
      "txid": "c0a110a7a84399b98052c6545018873b13ee3128fa74f7a697779174a36ea33a",
      "vout": 1,
      "scriptSig": {
        "asm": "3044022026c81b6ff4a15135d10c7f4b1ae6e44ac4fdb25c4a3c03161b17b8ab8d04850502200b448d070f418de1ca07e76943d23d447bc95c7c5e0322bcc153cadb5d9befe0[ALL] 0368d0fffa651783524f8b934d24d03b32bf8ff2c0808943a556b3d74b2e5c7d65",
        "hex": "473044022026c81b6ff4a15135d10c7f4b1ae6e44ac4fdb25c4a3c03161b17b8ab8d04850502200b448d070f418de1ca07e76943d23d447bc95c7c5e0322bcc153cadb5d9befe001210368d0fffa651783524f8b934d24d03b32bf8ff2c0808943a556b3d74b2e5c7d65"
      },
      "sequence": 0
    }
  ],
  "vout": [
    {
      "value": 0.00090000,
      
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
¡Voila! Ese es el poder de Libwally con `bitcoin-cli`.

Obviamente, también puede pasar un PSBT usando las funciones descritas en [§17.4](17_4_Usando_PSBTs_en_Libwally.md) y esa es una metodología más actualizada para el uso actual de Bitcoin, pero en cualquier caso, el concepto de pasar transacciones desde `bitcoin-cli` a código Libwally y viceversa debe ser similar.

## Importar y exportar semillas BIP39

Desafortunadamente, no todas las interacciones entre Libwally y `bitcoin-cli` van tan suavemente. Por ejemplo, sería bueno si pudiera exportar una semilla HD desde `bitcoin-cli`, para generar la frase mnemónica con Libwally, o generar una semilla a partir de una frase mneonímica usando Libwally, y luego importarla a `bitcoin-cli`. Desafortunadamente, ninguno de estos casos es posible en este momento. Una frase mneomínica se traduce en una semilla usando HMAC-SHA512, lo que significa que el resultado es de 512 bits. Sin embargo, `bitcoin-cli` exporta semillas HD (usando `dumpwallet`) e importa semillas HD (usando `sethdseed`) con una longitud de 256 bits. Hasta que eso cambie, nunca se encontrarán.

> :book: ***Cuál es la diferencia entre entropía y semilla?*** Libwally dice que crea sus frases mnemónicas a partir de la entropía. Eso es esencialmente lo mismo que una semilla: ambos son grandes números aleatorios. Entonces, si `bitcoin-cli` fuera compatible con semillas de frases mnemónicas de 512 bits, podría usar una para generar las frases mneonímicas y obtener los resultados que esperaba.

> :book: ***Cuál es la diferencia entre entropía y entropía cruda?*** No toda entropía es la misma. Cuando usted ingresa entropía en un comando que crea una semilla mnemónica, tiene una longitud específica y bien comprendida. Cambiar la entropía cruda en entropía requiere masajear la entropía cruda hasta que sea la longitud y el formato correctos, y en ese punto podría reutilizar esa entropía (no cruda) para recrear siempre la misma mnemónica (por lo que la entropía es efectivamente lo mismo que una semilla en ese punto, pero la entropía cruda no lo es).

## Importar claves privadas

Afortunadamente puede hacer lo mismo importando una clave privada generada en Libwally. De un vistazo a [genhd-for-import.c](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/tree/master/src/17_7_genhd_for_import.c), que es una versión simplificada del programa `genhd` de [§17.3](17_3_Usando_BIP32_en_Libwally.md) que también utiliza la biblioteca `jansson` de [§16.1](16_1_Accediendo_a_Bitcoind_en_C_con_las_Bibliotecas_RPC.md) para la salida regularizada.

El código actualizado también contiene un cambio de nota: solicita una huella dactilar de Libwally para que pueda crear correctamente una ruta de derivación:

```
  char account_fingerprint[BIP32_KEY_FINGERPRINT_LEN];
  lw_response = bip32_key_get_fingerprint(key_account,account_fingerprint,BIP32_KEY_FINGERPRINT_LEN);

  char *fp_hex;
  lw_response = wally_hex_from_bytes(account_fingerprint,BIP32_KEY_FINGERPRINT_LEN,&fp_hex);
```
> :warning: **ADVERTENCIA:** Recuerde que la huella dactilar en las rutas de derivación es arbitraria. Dado que Libwally proporciona uno lo estamos usando, pero si no tuviera uno, podría añadir un hexcode arbitrario de 4 bytes como huella digital a su ruta de derivación.

Asegúrese de compilar el nuevo código con la biblioteca `jansson`, después de instalarlo (si es necesario) por [§16.1](16_1_Accediendo_a_Bitcoind_en_C_con_las_Bibliotecas_RPC.md).

```
$ cc genhd-for-import.c -lwallycore -lsodium -ljansson -o genhd-for-import
```

Cuando ejecute el nuevo programa, le dará una buena lista de salida de todo:

```
$ ./genhd-for-import 
{
  "mnemonic": "physical renew say quit enjoy eager topic remind riot concert refuse chair",
  "account-xprv": "tprv8yxn8iFNgsLktEPkWKQpMqb7bcx5ViFQEbJMtqrGi8LEgvy8es6YeJvyJKrbYEPKMw8JbU3RFhNRQ4F2pataAtTNokS1JXBZVg62xfd5HCn",
  "address": "tb1q9lhru6k0ymwrtr5w98w35n3lz22upml23h753n",
  "derivation": "[d1280779/84h/1h/0h]"
}
```
Usted tiene el `mnemonic` del que puede recuperarse un `account-xprv` que puede importar, un `derivation` para usar para la importación, y una muestra de `address` que puede utilizar para probar la importación.

Ahora puede recurrir a las lecciones aprendidas de [§3.5](03_5_Entendiendo_El_Descriptor.md) sobre cómo convertir ese xprv en un descriptor e importarlo.

En primer lugar, es necesario averiguar la suma de comprobación:

```
$ xprv=tprv8yxn8iFNgsLktEPkWKQpMqb7bcx5ViFQEbJMtqrGi8LEgvy8es6YeJvyJKrbYEPKMw8JbU3RFhNRQ4F2pataAtTNokS1JXBZVg62xfd5HCn
$ dp=[d1280779/84h/1h/0h]
$ bitcoin-cli getdescriptorinfo "wpkh($dp$xprv/0/*)"
{
  "descriptor": "wpkh([d1280779/84'/1'/0']tpubDWepH8HcqF2RmhRYPy5QmFFEAeU1f3SJotu9BMta8Q8dXRDuHFv8poYqUUtEiWftBjtKn1aNhi9Qg2P4NdzF66dShYvB92z78WJbYeHTLTz/0/*)#f8rmqc0z",
  "checksum": "46c00dk5",
  "isrange": true,
  "issolvable": true,
  "hasprivatekeys": true
}
```
Aquí hay tres cosas a tener en cuenta:

1. Usamos `wpkh` como función en nuestra ruta de derivación. Eso es porque queremos generar direcciones Segwit modernas, no direcciones heredadas. Eso coincide con nuestro uso en Libwally de la función `wally_bip32_key_to_addr_segwit`. Lo más importante, sin embargo, es tener las mismas expectativas con Libwally y `bitcoin-cli` (y su descriptor) sobre qué tipo de dirección está generando, para que todo coincida!

2. Usamos la ruta `/0/*` porque queríamos las direcciones externas para esta cuenta. Si en cambio queríamos las direcciones de cambio, usaríamos `/1/*`.

3. No vamos a utilizar la línea `descriptor` devuelta, ya que es para una dirección `xpub`. En su lugar aplicaremos `checksum` devuelta a la `xprv` que ya tenemos.

```
$ cs=$(bitcoin-cli getdescriptorinfo "wpkh($dp$xprv/0/*)" | jq -r ' .checksum')
```
A continuación, conecte eso a `importmulti` para importar esta clave en `bitcoin-cli`:
```
$ bitcoin-cli importmulti '''[{ "desc": "wpkh('$dp''$xprv'/0/*)#'$cs'", "timestamp": "now", "range": 10, "watchonly": false, "label": "LibwallyImports", "keypool": false, "rescan": false }]'''
[
  {
    "success": true
  }
]

```
Aquí, usted importó/generó las primeras diez direcciones para la clave privada.

Examinando la nueva `LibwallyImported` la etiqueta los muestra:
```
$ bitcoin-cli getaddressesbylabel "LibwallyImports"
{
  "tb1qzeqrrt77xhvazq5g8sc9th0lzjwstknan8gzq7": {
    "purpose": "receive"
  },
  "tb1q9lhru6k0ymwrtr5w98w35n3lz22upml23h753n": {
    "purpose": "receive"
  },
  "tb1q8fsgxt0z9r9hfl5mst5ylxka2yljjxlxlvaf8j": {
    "purpose": "receive"
  },
  "tb1qg6dayhdk4qc6guutxvdweh6pctc9dpguu6awqc": {
    "purpose": "receive"
  },
  "tb1qdphaj0exvemxhgfpyh4p99wn84e2533u7p96l6": {
    "purpose": "receive"
  },
  "tb1qwv9mdqkpx6trtmvgw3l95npq8gk9pgllucvata": {
    "purpose": "receive"
  },
  "tb1qwh92pkrv6sps62udnmez65vfxe9n5ceuya56xz": {
    "purpose": "receive"
  },
  "tb1q4e98ln8xlym64qjzy3k8zyfyt5q60dgcn39d90": {
    "purpose": "receive"
  },
  "tb1qhzje887fyl65j4mulqv9ysmntwn95zpgmgvtqd": {
    "purpose": "receive"
  },
  "tb1q62xf9ec8zcfkh2qy5qnq4qcxrx8l0jm27dd8ru": {
    "purpose": "receive"
  },
  "tb1qlw85usfk446ssxejm9dmxsfn40kzsqce77aq20": {
    "purpose": "receive"
  }
}

```
El segundo en su lista de hecho coincide con su muestra (`tb1q9lhru6k0ymwrtr5w98w35n3lz22upml23h753n`). La importación de esta clave privada y la derivación de diez direcciones fue un éxito.

Si usted mira hacia atrás en [§7.3](07_3_Integrando_con_Hardware_Wallets.md), verá que esta fue la misma metodología que usamos para importar direcciones desde un Hardware Wallet (aunque esta vez también importamos la clave privada como prueba de concepto). La mayor diferencia es que anteriormente la información fue creada por una caja negra (literalmente: era un dispositivo de Ledger), y esta vez usted mismo creó la información usando Libwally, mostrando cómo puede hacer este tipo de trabajo de modo _airgapped_ u otros dispositivos más remotos y luego llevarlo a `bitcoin-cli`.

# Importar direcciones

Obviamente, si puede importar claves privadas, también puede importar direcciones lo que generalmente significa importar direcciones de sólo lectura _sin_ las claves privadas.

Una forma de hacerlo es utilizar la metodología `importmulti` anterior, pero utilizar la dirección xpub suministrada (`wpkh([d1280779/84'/1'/0']tpubDWepH8HcqF2RmhRYPy5QmFFEAeU1f3SJotu9BMta8Q8dXRDuHv8poYqUUtEiWftBjtKn1aNhi9Qg2P4NdzF66dShYvB92z78WJbYeHTLTz/0/*)#f8rmqc0z`) en lugar de la xprv original. Esa es la mejor manera de importar una secuencia entera de direcciones de sólo lectura.

Alternativamente, puede importar direcciones individuales. Por ejemplo, considere la dirección de ejemplo individual actualmente devuelta por el programa `genhd-for-import` :
```
$ ./genhd-for-import 
{
  "mnemonic": "finish lady crucial walk illegal ride hamster strategy desert file twin nature",
  "account-xprv": "tprv8xRujYeVN7CwBHxLoTHRdmzwdW7dKUzDfruSo56GqqfRW9QXtnxnaRG8ke7j65uNjxmCVfcagz5uuaMi2vVJ8jpiGZvLwahmNB8F3SHUSyv",
  "address": "tb1qtvcchgyklp6cyleth85c7pfr4j72z2vyuwuj3d",
  "derivation": "[6214ecff/84h/1h/0h]"
}
```
Puede importar eso como una dirección de sólo lectura con `importaddress`:
```
$ bitcoin-cli -named importaddress address=tb1qtvcchgyklp6cyleth85c7pfr4j72z2vyuwuj3d label=LibwallyWO rescan=false
$ bitcoin-cli getaddressesbylabel "LibwallyWO"
{
  "tb1qtvcchgyklp6cyleth85c7pfr4j72z2vyuwuj3d": {
    "purpose": "receive"
  }
}
}
```
## Resumen: Integración de Libwally y Bitcoin-CLI

Con un conocimiento fundamental de Libwally, ahora puede complementar todo el trabajo de sus lecciones anteriores. Transferir direcciones, claves, transacciones y PSBTs son sólo algunas de las formas en las que puede hacer uso de estos dos poderosos métodos de programación Bitcoin juntos. También hay mucha más profundidad potencial si quiere profundizar en la extensa biblioteca de funciones de Libwally.

> :fire: ***Cuál es el poder de integrar Libwally y Bitcoin-CLI?*** Una de las mayores ventajas de Libwally es que tiene muchas funciones que se pueden usar fuera de línea. En comparación, Bitcoin Core es un programa en red. Esto puede ayudarle a aumentar la seguridad al tener `bitcoin-cli` para pasar claves, direcciones, transacciones o PSBTs a una fuente fuera de línea (que estaría ejecutando programas Libwally). Además de eso, Libwally puede hacer cosas que Bitcoin Core actualmente no puede, como generar una semilla a partir de un BIP39 mnemonic (e incluso si actualmente no se puede importar la semilla a Bitcoin Core, usted _puede_ todavía importar la clave maestra para la cuenta, como se muestra aquí).

## Que sigue?

Aprenda sobre otro tipo de programación en [18.0: Hablando a Bitcoind con Otros Lenguajes](18_0_Hablando_a_Bitcoind_con_Otros_Lenguajes.md).
