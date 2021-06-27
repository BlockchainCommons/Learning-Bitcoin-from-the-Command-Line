# 9.4: Codificando una P2PKH

Las direcciones P2PKH están perdiendo popularidad rápidamente debido a la llegada de SegWit, pero, no obstante, siguen siendo un gran bloque de construcción para comprender Bitcoin, y especialmente para comprender los scripts de Bitcoin. (Echaremos un vistazo rápido a cómo los scripts P2WPKH nativos de Segwit funcionan de manera diferente en la siguiente sección).

## Comprender el script de desbloqueo

Durante mucho tiempo hemos dicho que cuando los fondos se envían a una dirección de Bitcoin, están bloqueados en la clave privada asociada con esa dirección. Esto se gestiona a través del `scriptPubKey` de una transacción P2PKH, que está diseñada de tal manera que requiere que el destinatario tenga la clave privada asociada a la dirección P2PKH Bitcoin. Para ser precisos, el destinatario debe proporcionar tanto la clave pública vinculada a la clave privada como una firma generada por la clave privada. 

Revise nuevamente la transacción que creó en [§9.1](09_1_Entendiendo_la_base_de_las_Transacciones.md):
```
$ bitcoin-cli -named decoderawtransaction hexstring=$signedtx
{
  "txid": "34151dac704d94a269cd33f80be34c122152edc9bfbb9323852966bf0ce937ed",
  "hash": "34151dac704d94a269cd33f80be34c122152edc9bfbb9323852966bf0ce937ed",
  "version": 2,
  "size": 191,
  "vsize": 191,
  "weight": 764,
  "locktime": 0,
  "vin": [
    {
      "txid": "bb4362dec15e67d366088f5493c789f22fb4a604e767dae1f6a631687e2784aa",
      "vout": 0,
      "scriptSig": {
        "asm": "304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b92cbab7d1022066f273178febc7a37568e2e9f4dec980a2e9a95441abe838c7ef64c39d85849c[ALL] 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b",
        "hex": "47304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b92cbab7d1022066f273178febc7a37568e2e9f4dec980a2e9a95441abe838c7ef64c39d85849c01210315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b"
      },
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 0.00090000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 06b5c6ba5330cdf738a2ce91152bfd0e71f9ec39 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a91406b5c6ba5330cdf738a2ce91152bfd0e71f9ec3988ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mg8S7F1gY3ivV9M9GrWwe6ziWvK2MFquCf"
        ]
      }
    }
  ]
}
```
Puede ver que su `scriptSig` o secuencia de comandos de desbloqueo tiene dos valores. Eso es un `<signature>` (y un `[ALL]`) y un `<pubKey>`:
```
304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b92cbab7d1022066f273178febc7a37568e2e9f4dec980a2e9a95441abe838c7ef64c39d85849c[ALL] 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b
```
Eso es todo lo que es un script de desbloqueo! (Para un P2PKH.)

## Comprender el script de bloqueo

Recuerde que cada script de desbloqueo desbloquea un UTXO anterior. En el ejemplo anterior, `vin` revela que en realidad está desbloqueando el vout `0` de la transacción con txid `bb4362dec15e67d366088f5493c789f22fb4a604e767dae1f6a631687e2784aa`.

Puede examinar ese UTXO con `gettransaction`.

```
$ bitcoin-cli gettransaction "bb4362dec15e67d366088f5493c789f22fb4a604e767dae1f6a631687e2784aa"
{
  "amount": 0.00095000,
  "confirmations": 12,
  "blockhash": "0000000075a4c1519da5e671b15064734c42784eab723530a6ace83ca1e66d3f",
  "blockheight": 1780789,
  "blockindex": 132,
  "blocktime": 1594841768,
  "txid": "bb4362dec15e67d366088f5493c789f22fb4a604e767dae1f6a631687e2784aa",
  "walletconflicts": [
  ],
  "time": 1594841108,
  "timereceived": 1594841108,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "mmX7GUoXq2wVcbnrnFJrGKsGR14fXiGbD9",
      "category": "receive",
      "amount": 0.00095000,
      "label": "",
      "vout": 0
    }
  ],
  "hex": "020000000001011efcc3bf9950ac2ea08c53b43a0f8cc21e4b5564e205f996f7cadb7d13bb79470000000017160014c4ea10874ae77d957e170bd43f2ee828a8e3bc71feffffff0218730100000000001976a91441d83eaffbf80f82dee4c152de59a38ffd0b602188ac713b10000000000017a914b780fc2e945bea71b9ee2d8d2901f00914a25fbd8702473044022025ee4fd38e6865125f7c315406c0b3a8139d482e3be333727d38868baa656d3d02204b35d9b5812cb85894541da611d5cec14c374ae7a7b8ba14bb44495747b571530121033cae26cb3fa063c95e2c55a94bd04ab9cf173104555efe448b1bfc3a68c8f873342c1b00"
}
```
Pero como puede ver no obtuvo el `scriptPubKey` con el comando`gettransaction`. Debe dar un paso adicional para recuperar eso examinando la información de la transacción sin procesar (esa es la `hex`) con `decoderawtransaction`:

```
$ hex=$(bitcoin-cli gettransaction "bb4362dec15e67d366088f5493c789f22fb4a604e767dae1f6a631687e2784aa" | jq -r '.hex')
$ bitcoin-cli decoderawtransaction $hex
{
  "txid": "bb4362dec15e67d366088f5493c789f22fb4a604e767dae1f6a631687e2784aa",
  "hash": "6866490b16a92d68179e1cf04380fd08f16ec80bf66469af8d5e78ae624ff202",
  "version": 2,
  "size": 249,
  "vsize": 168,
  "weight": 669,
  "locktime": 1780788,
  "vin": [
    {
      "txid": "4779bb137ddbcaf796f905e264554b1ec28c0f3ab4538ca02eac5099bfc3fc1e",
      "vout": 0,
      "scriptSig": {
        "asm": "0014c4ea10874ae77d957e170bd43f2ee828a8e3bc71",
        "hex": "160014c4ea10874ae77d957e170bd43f2ee828a8e3bc71"
      },
      "txinwitness": [
        "3044022025ee4fd38e6865125f7c315406c0b3a8139d482e3be333727d38868baa656d3d02204b35d9b5812cb85894541da611d5cec14c374ae7a7b8ba14bb44495747b5715301",
        "033cae26cb3fa063c95e2c55a94bd04ab9cf173104555efe448b1bfc3a68c8f873"
      ],
      "sequence": 4294967294
    }
  ],
  "vout": [
    {
      "value": 0.00095000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 41d83eaffbf80f82dee4c152de59a38ffd0b6021 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a91441d83eaffbf80f82dee4c152de59a38ffd0b602188ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mmX7GUoXq2wVcbnrnFJrGKsGR14fXiGbD9"
        ]
      }
    },
    {
      "value": 0.01063793,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_HASH160 b780fc2e945bea71b9ee2d8d2901f00914a25fbd OP_EQUAL",
        "hex": "a914b780fc2e945bea71b9ee2d8d2901f00914a25fbd87",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2N9yWARt5E3TQsX2RjsauxSZaEZVhinAS4h"
        ]
      }
    }
  ]
}
```
Ahora puede mirar el `vout` `0` y ver que estaba bloqueado con la `scriptPubKey` de `OP_DUP OP_HASH160 41d83eaffbf80f82dee4c152de59a38ffd0b6021 OP_EQUALVERIFY OP_CHECKSIG`. Esa es la metodología de bloqueo estándar utilizada para una dirección P2PKH anterior con el `<pubKeyHash>` bloqueado en el medio.

Ejecutarlo demostrará cómo funciona.

## Ejecute un script P2PKH

Cuando desbloquea un UTXO P2PKH, (efectivamente) concatena los scripts de desbloqueo y bloqueo. Para una dirección P2PKH, como el ejemplo utilizado en este capítulo, que produce:

```
Script: <signature> <pubKey> OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
```
Con eso en conjunto, puede examinar cómo se desbloquea un UTXO P2PKH.

Primero, pone las constantes iniciales en la pila, y luego hace un duplicado de la pubKey con `OP_DUP`:
```
Script: <signature> <pubKey> OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Stack: [ ]

Script: <pubKey> OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Stack: [ <signature> ]

Script: OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Stack: [ <signature> <pubKey> ]

Script: OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Running: <pubKey> OP_DUP
Stack: [ <signature> <pubKey> <pubKey> ]
```
Por qué el duplicado? Porque es necesario comprobar los dos elementos de desbloqueo: la clave pública y la firma.

A continuación, `OP_HASH160` saca el resultado de la pila `<pubKey>`, lo codifica y vuelve a colocar el resultado en la pila.
```
Script: <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Running: <pubKey> OP_HASH160
Stack: [ <signature> <pubKey> <pubKeyHash> ]
```
Luego, coloca el `<pubKeyHash>` que estaba en el script de bloqueo en la pila:
```
Script: OP_EQUALVERIFY OP_CHECKSIG
Stack: [ <signature> <pubKey> <pubKeyHash> <pubKeyHash> ]
```
`OP_EQUALVERIFY` es efectivamente dos códigos de operación: `OP_EQUAL`, que saca dos elementos de la pila y empuja `True` o `False` en función de la comparación y `OP_VERIFY` que muestra ese resultado e inmediatamente marca la transacción como inválida si es `False`. (El capítulo 12 habla más sobre el uso de `OP_VERIFY` como condicional.)

Suponiendo que los dos `<pubKeyHash>es` son iguales, obtendrá el siguiente resultado:
```
Script: OP_CHECKSIG
Running: <pubKeyHash> <pubKeyHash> OP_EQUALVERIFY
Stack: [ <signature> <pubKey> ]
```
En este punto, ha demostrado que el `<pubKey>` provisto en el `scriptSig` hashea a la dirección de Bitcoin en cuestión, por lo que sabe que el receptor conocía la clave pública. Pero también necesita demostrar el conocimiento de la clave privada, lo cual se hace con `OP_CHECKSIG`, lo que confirma que la firma del script de desbloqueo coincide con esa clave pública
```
Script:
Running: <signature> <pubKey> OP_CHECKSIG
Stack: [ True ]
```
