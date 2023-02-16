# 4.6: Creando una Transacción Segwit

> :information_source: **NOTA:** Esta sección ha sido añadida recientemente al curso y es un primer borrador que puede estar aún pendiente de revisión. Advertencia al lector.

Érase una vez, los cielos de Bitcoin se agitaron con las guerras de tamaño de bloque. Las tarifas se disparaban y los usuarios estaban preocupados por el escalado. Los desarrolladores de Bitcoin Core eran reacios a aumentar simplemente el tamaño de los bloques, pero llegaron a un compromiso: SegWit, el Testigo Segregado. Testigo Segregado es una forma elegante de decir "Firma Separada". Crea nuevos tipos de transacciones que eliminan las firmas al final de la transacción. Combinando esto con un aumento del tamaño de los bloques que sólo son visibles para los nodos actualizados, SegWit resolvió los problemas de escalado de Bitcoin en ese momento (y también resolvió un desagradable error de maleabilidad que había hecho impracticable un escalado aún mejor con protocolos de capa 2 como Lightning). 

¿El truco? SegWit utiliza diferentes direcciones, algunas de las cuales son compatibles con los nodos más antiguos, y otras no.

> :warning: **ADVERTENCIA DE VERSIÓN:** SegWit fue introducido en BitCoin 0.16.0 con lo que se describió en su momento como "soporte completo". Dicho esto, hubo algunos fallos en su integración con `bitcoin-cli` en ese momento que impidieron que la firma funcionara correctamente en las nuevas direcciones P2SH-SegWit. La dirección Bech32 no compatible con la versión anterior también se introdujo en Bitcoin 0.16.0 y se convirtió en el tipo de dirección por defecto en Bitcoin 0.19.0. Toda esta funcionalidad debería ahora funcionar completamente con respecto a las funciones `bitcoin-cli` (y por lo tanto este tutorial). 

> El problema es la interacción con el resto del mundo. Todo el mundo debería ser capaz de enviar a una dirección P2SH-SegWit porque fue construido a propósito para soportar la compatibilidad hacia atrás envolviendo la funcionalidad SegWit en un Script de Bitcoin. No ocurre lo mismo con las direcciones Bech32: si alguien le dice que no puede enviar a su dirección Bech32, es por eso, y necesita generar una dirección `legacy` o P2SH-SegWit para su uso. (Muchos sitios, particularmente los intercambios, tampoco pueden generar o recibir en direcciones SegWit, particularmente en direcciones Bech32, pero eso es un tema totalmente diferente y no afecta a su uso).

## Entender una transacción SegWit

En las transacciones clásicas, la información de la firma (testigo) se almacenaba hacia la mitad de la transacción, mientras que en las transacciones SegWit, está en la parte inferior. Esto va de la mano con el aumento del tamaño de los bloques que se introdujo en la actualización de SegWit. El tamaño del bloque pasó de 1M a una cantidad variable basada en cuántas transacciones SegWit hay en un bloque, empezando por 1M (sin transacciones SegWit) y llegando hasta 4M (todas las transacciones SegWit). Este tamaño variable se creó para acomodar a los nodos clásicos, para que todo siga siendo compatible con el pasado. Si un nodo clásico ve una transacción SegWit, desecha la información del testigo (dando como resultado un bloque de menor tamaño, por debajo del antiguo límite de 1M), mientras que si un nodo nuevo ve una transacción SegWit, mantiene la información del testigo (dando como resultado un bloque de mayor tamaño, hasta el nuevo límite de 4M).

Así que ese es el qué y el cómo de las transacciones SegWit. No es que necesite saber nada de esto para usarlas. La mayoría de las transacciones en la red Bitcoin son ahora SegWit. Son las que se van a utilizar de forma nativa para más transacciones y recibos de dinero. Los detalles no son más relevantes en este momento que los detalles de cómo funciona la mayor parte de Bitcoin.

## Crear una dirección SegWit

Una dirección SegWit se crea de la misma manera que cualquier otra dirección, con los comandos `getnewaddress` y `getrawchangeaddress`.

Si necesita crear una dirección para alguien que no puede enviar a las nuevas direcciones Bech32, utiliza el tipo de dirección `p2sh-segwit`:
```
$ bitcoin-cli -named getnewaddress address_type=p2sh-segwit
2N5h2r4karVqN7uFtpcn8xnA3t5cbpszgyN
```
Ver una dirección con el prefijo "2" significa que lo ha hecho bien.

> :link: **TESTNET vs MAINNET:** "3" para Mainnet.

Sin embargo, si la persona con la que está interactuando tiene un cliente completamente maduro, podrá enviar a una dirección Bech32, que crea usando los comandos de la manera predeterminada:
```
$ bitcoin-cli getnewaddress
tb1q5gnwrh7ss5mmqt0qfan85jdagmumnatcscwpk6
```
Como ya hemos visto, las direcciones de cambio generadas desde `bitcoin-cli` interactúan bien con las direcciones de Bech32, por lo que tampoco tiene sentido usar la bandera `legacy` allí:
```
$ bitcoin-cli getrawchangeaddress
tb1q05wx5tyadm8qe83exdqdyqvqqzjt3m38vfu8ff
```

En este caso, hay que tener en cuenta que el prefijo único "tb1" denota Bech32.

> :link: **TESTNET vs MAINNET:** "bc1" para mainnet.

A Bitcoin-cli no le importa el tipo de dirección que esté usando. Puede ejecutar un comando como `listaddressgroupings` y mezclará libremente direcciones de los diferentes tipos:
```
$ bitcoin-cli listaddressgroupings
[
  [
    [
      "mfsiRhxbQxcD7HLS4PiAim99oeGyb9QY7m",
      0.01000000,
      ""
    ]
  ],
  [
    [
      "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
      0.00000000,
      ""
    ],
    [
      "tb1q6dak4e9fz77vsulk89t5z92l2e0zm37yvre4gt",
      0.00000000
    ]
  ],
  [
    [
      "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
      0.00022000,
      ""
    ]
  ],
  [
    [
      "mk9ry5VVy8mrA8SygxSQQUDNSSXyGFot6h",
      0.00000000
    ],
    [
      "mqjrdY5raxKzXQf5t2VvVvzhvFAgersu9B",
      0.00000000
    ],
    [
      "mwJL7cRiW2bUnY81r1thSu3D4jtMmwyU6d",
      0.00000000,
      ""
    ],
    [
      "tb1q57p0f3hpuad9kf8n6e6adugmt6lnkg2zzr592r",
      0.00007800
    ]
  ],
  [
    [
      "mpVLL7iqPr4d7BJkEG54mcdm7WmrAhaW6q",
      0.01000000,
      ""
    ]
  ],
  [
    [
      "tb1q5gnwrh7ss5mmqt0qfan85jdagmumnatcscwpk6",
      0.01000000,
      ""
    ]
  ]
]
```

## Enviar una transacción SegWit de forma sencilla

¿Cómo se envía una transacción Segwit? Exactamente igual que cualquier otra transacción. No importa si el UTXO es SegWit, la dirección es SegWit, o alguna combinación de ellos. Puede esperar que `bitcoin-cli` haga lo correcto. Aunque se pueden notar las diferencias a través de las direcciones, no importan para interactuar con cosas a nivel de `bitcoin-cli` o RPC. (Y esta es una de las ventajas de utilizar la línea de comandos y la interfaz RPC, como se sugiere en este tutorial: los expertos ya han hecho el trabajo duro por usted, incluyendo cosas como la forma de enviar tanto a direcciones heredadas como a Bech32. Usted sólo tiene que utilizar esa funcionalidad en su propio beneficio).

Aquí hay un ejemplo de envío a una dirección SegWit, de la manera más fácil:
```
$ bitcoin-cli sendtoaddress tb1qw508d6qejxtdg4y5r3zarvary0c5xw7kxpjzsx 0.005
854a833b667049ac811b4cf1cad40fa7f8dce8b0f4c1018a58b84559b6e05f42
```
Si mira su transacción, puede ver el uso de la dirección Bech32:
```
$ bitcoin-cli -named gettransaction txid="854a833b667049ac811b4cf1cad40fa7f8dce8b0f4c1018a58b84559b6e05f42" verbose=true
{
  "amount": -0.00500000,
  "fee": -0.00036600,
  "confirmations": 0,
  "trusted": true,
  "txid": "854a833b667049ac811b4cf1cad40fa7f8dce8b0f4c1018a58b84559b6e05f42",
  "walletconflicts": [
  ],
  "time": 1592948795,
  "timereceived": 1592948795,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "tb1qw508d6qejxtdg4y5r3zarvary0c5xw7kxpjzsx",
      "category": "send",
      "amount": -0.00500000,
      "vout": 1,
      "fee": -0.00036600,
      "abandoned": false
    }
  ],
  "hex": "0200000002114d5a4c3b847bc796b2dc166ca7120607b874aa6904d4a43dd5f9e0ea79d4ba010000006a47304402200a3cc08b9778e7b616340d4cf7841180321d2fa019e43f25e7f710d9a628b55c02200541fc200a07f2eb073ad8554357777d5f1364c5a96afe5e77c6185d66a40fa7012103ee18c598bafc5fbea72d345329803a40ebfcf34014d0e96aac4f504d54e7042dfeffffffa71321e81ef039af490251379143f7247ad91613c26c8f3e3404184218361733000000006a47304402200dd80206b57beb5fa38a3c3578f4b0e40d56d4079116fd2a6fe28e5b8ece72310220298a8c3a1193ea805b27608ff67a2d8b01e347e33a4222edfba499bb1b64a31601210339c001b00dd607eeafd4c117cfcf86be8efbb0ca0a33700cffc0ae0c6ee69d7efeffffff026854160000000000160014d591091b8074a2375ed9985a9c4b18efecfd416520a1070000000000160014751e76e8199196d454941c45d1b3a323f1433bd6c60e1b00",
  "decoded": {
    "txid": "854a833b667049ac811b4cf1cad40fa7f8dce8b0f4c1018a58b84559b6e05f42",
    "hash": "854a833b667049ac811b4cf1cad40fa7f8dce8b0f4c1018a58b84559b6e05f42",
    "version": 2,
    "size": 366,
    "vsize": 366,
    "weight": 1464,
    "locktime": 1773254,
    "vin": [
      {
        "txid": "bad479eae0f9d53da4d40469aa74b8070612a76c16dcb296c77b843b4c5a4d11",
        "vout": 1,
        "scriptSig": {
          "asm": "304402200a3cc08b9778e7b616340d4cf7841180321d2fa019e43f25e7f710d9a628b55c02200541fc200a07f2eb073ad8554357777d5f1364c5a96afe5e77c6185d66a40fa7[ALL] 03ee18c598bafc5fbea72d345329803a40ebfcf34014d0e96aac4f504d54e7042d",
          "hex": "47304402200a3cc08b9778e7b616340d4cf7841180321d2fa019e43f25e7f710d9a628b55c02200541fc200a07f2eb073ad8554357777d5f1364c5a96afe5e77c6185d66a40fa7012103ee18c598bafc5fbea72d345329803a40ebfcf34014d0e96aac4f504d54e7042d"
        },
        "sequence": 4294967294
      },
      {
        "txid": "33173618421804343e8f6cc21316d97a24f7439137510249af39f01ee82113a7",
        "vout": 0,
        "scriptSig": {
          "asm": "304402200dd80206b57beb5fa38a3c3578f4b0e40d56d4079116fd2a6fe28e5b8ece72310220298a8c3a1193ea805b27608ff67a2d8b01e347e33a4222edfba499bb1b64a316[ALL] 0339c001b00dd607eeafd4c117cfcf86be8efbb0ca0a33700cffc0ae0c6ee69d7e",
          "hex": "47304402200dd80206b57beb5fa38a3c3578f4b0e40d56d4079116fd2a6fe28e5b8ece72310220298a8c3a1193ea805b27608ff67a2d8b01e347e33a4222edfba499bb1b64a31601210339c001b00dd607eeafd4c117cfcf86be8efbb0ca0a33700cffc0ae0c6ee69d7e"
        },
        "sequence": 4294967294
      }
    ],
    "vout": [
      {
        "value": 0.01463400,
        "n": 0,
        "scriptPubKey": {
          "asm": "0 d591091b8074a2375ed9985a9c4b18efecfd4165",
          "hex": "0014d591091b8074a2375ed9985a9c4b18efecfd4165",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1q6kgsjxuqwj3rwhkenpdfcjccalk06st9z0k0kh"
          ]
        }
      },
      {
        "value": 0.00500000,
        "n": 1,
        "scriptPubKey": {
          "asm": "0 751e76e8199196d454941c45d1b3a323f1433bd6",
          "hex": "0014751e76e8199196d454941c45d1b3a323f1433bd6",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1qw508d6qejxtdg4y5r3zarvary0c5xw7kxpjzsx"
          ]
        }
      }
    ]
  }
}
```
De hecho, ambos `vouts` utilizan direcciones Bech32: su destinatario y la dirección de cambio generada automáticamente.

Pero cuando retrocedemos en nuestro `vin`, descubrimos que proviene de una dirección heredada. Porque no importa:
```
$ bitcoin-cli -named gettransaction txid="33173618421804343e8f6cc21316d97a24f7439137510249af39f01ee82113a7"
{
  "amount": 0.01000000,
  "confirmations": 43,
  "blockhash": "00000000000000e2365d2f814d1774b063d9a04356f482010cdfdd537b1a24bb",
  "blockheight": 1773212,
  "blockindex": 103,
  "blocktime": 1592937103,
  "txid": "33173618421804343e8f6cc21316d97a24f7439137510249af39f01ee82113a7",
  "walletconflicts": [
  ],
  "time": 1592936845,
  "timereceived": 1592936845,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "mpVLL7iqPr4d7BJkEG54mcdm7WmrAhaW6q",
      "category": "receive",
      "amount": 0.01000000,
      "label": "",
      "vout": 0
    }
  ],
  "hex": "020000000001016a66efa334f06e2c54963e48d049a35d7a1bda44633b7464621cae302f35174a0100000017160014f17b16c6404e85165af6f123173e0705ba31ec25feffffff0240420f00000000001976a914626ab1ca41d98f597d18d1ff8151e31a40d4967288acd2125d000000000017a914d5e76abfe5362704ff6bbb000db9cdfa43cd2881870247304402203b3ba83f51c1895b5f639e9bfc40124715e2495ef2c79d4e49c0f8f70fbf2feb02203d50710abe3cf37df4d2a73680dadf3cecbe4f2b5d0b276dbe7711d0c2fa971a012102e64f83ee1c6548bcf44cb965ffdb803f30224459bd2e57a5df97cb41ba476b119b0e1b00"
}
```

## Enviar una transacción SegWit de la manera más difícil

De forma similar, puede financiar una transacción con una dirección Bech32 sin que haya ninguna diferencia con las técnicas que has aprendido hasta ahora. Aquí hay un ejemplo de cómo hacerlo con una transacción completa en crudo:
```
$ changeaddress=$(bitcoin-cli getrawchangeaddress)
$ echo $changeaddress
tb1q4xje3mx9xn7f8khv7p69ekfn0q72kfs8x3ay4j
$ bitcoin-cli listunspent
[
...
  {
    "txid": "003bfdca5578c0045a76768281f05d5e6f57774be399a76f387e2a0e99e4e452",
    "vout": 0,
    "address": "tb1q5gnwrh7ss5mmqt0qfan85jdagmumnatcscwpk6",
    "label": "",
    "scriptPubKey": "0014a226e1dfd08537b02de04f667a49bd46f9b9f578",
    "amount": 0.01000000,
    "confirmations": 5,
    "spendable": true,
    "solvable": true,
    "desc": "wpkh([d6043800/0'/0'/5']0327dbe2d58d9ed2dbeca28cd26e18f48aa94c127fa6fb4b60e4188f6360317640)#hd66hknp",
    "safe": true
  }
]
$ recipient=tb1qw508d6qejxtdg4y5r3zarvary0c5xw7kxpjzsx
$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[2] | .txid')
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[2] | .vout')
$ echo $utxo_txid $utxo_vout
003bfdca5578c0045a76768281f05d5e6f57774be399a76f387e2a0e99e4e452 0
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.002, "'$changeaddress'": 0.007 }''')
$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
e02568b706b21bcb56fcf9c4bb7ba63fdbdec1cf2866168c4f50bc0ad693f26c
```
Todo funciona exactamente igual que otros tipos de transacciones.

### Reconocer el nuevo descriptor

Si mira el campo `desc`, notará que la dirección SegWit tiene un estilo de descriptor diferente a los encontrados en [§3.5: Entendiendo el Descriptor](03_5_Entendiendo_El_Descriptor.md). Un descriptor heredado descrito en esa sección tenía el siguiente aspecto `pkh([d6043800/0'/0'/18']03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388)#4ahsl9pk`. En cambio, nuestro nuevo descriptor SegWit tiene el siguiente aspecto `wpkh([d6043800/0'/0'/5']0327dbe2d58d9ed2dbeca28cd26e18f48aa94c127fa6fb4b60e4188f6360317640)#hd66hknp"`.

Lo más importante es que la función ha cambiado. Antes era `pkh`, que es una dirección de clave pública con hash P2PKH estándar. La dirección SegWit es en cambio `wpkh`, lo que significa que es una dirección SegWit nativa de P2WPKH. Esto subraya :fire: ***el poder de los descriptores***. Describen cómo crear una dirección a partir de una clave u otra información, y las funciones definen sin ambigüedad cómo hacer la dirección en función de su tipo.

## Resumen: Creación de una transacción SegWit

Realmente no hay ninguna complejidad en la creación de transacciones SegWit. Internamente, están estructuradas de forma diferente a las transacciones heredadas, pero desde la línea de comandos no hay ninguna diferencia: simplemente se utiliza una dirección con un prefijo diferente. Lo único que hay que tener en cuenta es que algunas personas pueden no ser capaces de enviar a una dirección Bech32 si están utilizando un software obsoleto.

> 🔥 ___¿Qué ventajas tiene el envío de monedas con SegWit?___

> _Las ventajas._ Las transacciones con SegWit son más pequeñas, y por lo tanto serán más baratas de enviar que las transacciones heredadas debido a las menores comisiones. Bech32 duplica esta ventaja, y también crea direcciones que son más difíciles de estropear al transcribir - y eso es bastante importante, dado que el error del usuario es una de las formas más probables de perder sus bitcoins.

> _Las desventajas._ Las direcciones SegWit pueden no ser soportadas por el software obsoleto de Bitcoin. En particular, la gente puede no ser capaz de enviar a su dirección Bech32.

## ¿Qué sigue?

Avanzar a través de "bitcoin-cli" con [Capítulo 5: Controlando Transacciones Bitcoin](05_0_Controlando_Transacciones_Bitcoin.md).
