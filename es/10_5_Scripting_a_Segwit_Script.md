# 10.5: Guioniendo un Guion de Segwit

> :information_source: **NOTA:** Esta sección se ha agregado recientemente al curso y es un borrador inicial que aún puede estar pendiente de revisión. Lector de advertencias.

Segwit introdujo una serie de nuevas opciones para los tipos de direcciones (y, por lo tanto, secuencias de comandos). [§9.5: Scripting a P2WPKH](09_5_Scripting_a_P2WPKH.md) explicó cómo el nuevo tipo de dirección Bech32 variaba los scripts estándar encontrados en la mayoría de las transacciones tradicionales. Este capitulo analiza los otros tres tipos de scripts introducidos por la actualización de Segwit: el P2SH-Segwit (que era la dirección de transición "Segwit anidada", cuando Segwit entró en uso), el P2WSH (que es el equivalente Segwit de la dirección P2SH, al igual que P2WPKH es el equivalente Segwit de la dirección P2PKH), y la dirección P2WSH anidada.

Esta es otra situación en la que realmente no tendrá que preocuparse por estos matices mientras trabaja con `bitcoin-cli`, pero es útil saber cómo funciona todo.

## Entender un Guion de P2SH-Segwit

La dirección P2SH-Segwit es una raza en extinción. Básicamente, fue una medidia provisional mientras Bitcoin estaba en transición a Segwit que permitía a un usuario crear una dirección de Segwit y luego tener a alguien con un intercambio o fondo de biletera no habilitado para Segwit que se dirigiera. 

Si alguna vez necesita usar uno, hay una opción para crear una dirección P2SH-Segwit usando `getnewaddress`:
```
$ bitcoin-cli getnewaddress -addresstype p2sh-segwit
2NEzBvokxh4ME4ahdT18NuSSoYvvhS7EnMU
```
La dirección comienza con un `2` (o un `3`) que la revela como un guión.

> :book: ***¿Por qué los nodos antiguos no pueden enviar a direcciones nativas de Segwit?*** [§10.1](10_1_Understanding_the_Foundation_of_P2SH.md) señaló que había un número determinado de transacciones de Bitcoin "estándar". En realidad, no puede bloquear una transacción con un script que no sea uno de esos tipos estándar. Segwit ahora se reconoce como uno de esos estándares, pero un nodo antiguo no lo sabrá, por lo que se negará a enviar una transacción de este tipo para la protección del remitente. Envolver una dirección Segwit dentro de un hash de script estándar resuelve el problema.

Cuando mira un UTXO enviado a esa dirección, puede ver que el `desc` es diferente, revelando una dirección WPKH envuelta en un script:
```
$ bitcoin-cli listunspent
  {
    "txid": "ed752673bfd4338ccf0995983086da846ad652ae0f28280baf87f9fd44b3c45f",
    "vout": 1,
    "address": "2NEzBvokxh4ME4ahdT18NuSSoYvvhS7EnMU",
    "redeemScript": "001443ab2a09a1a5f2feb6c799b5ab345069a96e1a0a",
    "scriptPubKey": "a914ee7aceea0865a05a29a28d379cf438ac5b6cd9c687",
    "amount": 0.00095000,
    "confirmations": 1,
    "spendable": true,
    "solvable": true,
    "desc": "sh(wpkh([f004311c/0'/0'/3']03bb469e961e9a9cd4c23db8442d640d9b0b11702dc0126462ac9eb88b64a4dd48))#p29e839h",
    "safe": true
  }
```
Más importante aún, hay un `redeemScript`, que decodifica a `OP_0 OP_PUSHDATA (20 bytes) 3ab2a09a1a5f2feb6c799b5ab345069a96e1a0a`. Deberia parecer familiar, porque es un `OP_0` seguido de un código hexadecimal de 20 bytes de un hash de clave pública. En otras palabras, un P2SH-SegWit es solo un SegWit `scriptPubKey` atascado en un script. Eso es todo al respecto. Coincide precisamente con la forma en que las multifirmas modernas son un multifirma atascado en un P2SH, como se explica en [§10.4: Scripting a Multisig](10_4_Scripting_a_Multisig.md).

La transacción sin procesar revela un poco más cuando mira el `vout` `1`:
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
Esto confirma que esto es solo un P2SH normal, bloqueado por `"OP_DUP OP_HASH160 41d83eaffbf80f82dee4c152de59a38ffd0b6021 OP_EQUALVERIFY OP_CHECKSIG"`. Es cuando se ejecuta el script de canje que ocurre la magia. Al igual que con un P2WPKH, un nodo antiguo verá `OP_0 OP_PUSHDATA (20 bytes) 3ab2a09a1a5f2feb6c799b5ab345069a96e1a0a` y lo verificará automáticamente, mientras que un nuevo nodo lo verá, sabrá que es un P2WPKH, y así saldrá con los `testigos`. See [§9.5: Scripting a P2WPKH](09_5_Scripting_a_P2WPKH.md).

> :book: ***¿Cuáles son las desventajas de las transacciones segwit anidadas?*** Son más grandes que las transacciones nativas de Segwit, por lo que obtiene algunas de las ventajas de Segwit, pero no todas.

## Entender un Guion P2WSH

Por el contrario, las transacciones P2WSH deberían tener un uso cada vez mayor, ya que son el reemplazo nativo de Segwit para P2SH, ofreciendo las mismas ventajas de tamaño de bloque que se crearon con transacciones nativas Segwit P2WPKH.

Este es un ejemplo de dirección P2WSH:
[https://blockstream.info/testnet/address/tb1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3q0sl5k7](https://blockstream.info/testnet/address/tb1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3q0sl5k7)

Los detalles muestran que un UTXO enviado a esta dirección está bloqueado con una `scriptPubKey` como esta:
```
OP_0 OP_PUSHDATA (32 bytes) 1863143c14c5166804bd19203356da136c985678cd4d27a1b8c6329604903262
```
Esto funciona como una dirección P2WPKH, la única diferencia es que un lugar de un hash de clave pública de 20 bytes, el UTXO incluye un hash de script de 32 bytes. Al igual que con un P2WPKH, los nodos antiguos solo verifican esto, mientras que los nuevos nodos reconocen que se trata de un P2WSH y, por lo tanto, verifican internamente el script como se describe en las secciones anteriores, pero utilizando los datos del `testigo`, que ahora incluye el script de canje.
También hay una variante más, un script P2WSH incrustado en un script P2SH, que funciona de manera muy similar al P2SH-Segwit descrito anterioremente, pero para scripts P2WSH anidados. (Whew!)

## Resumen: Programación de un Hash de Clave Pública de Pagar Para Precenciar

Hay dos tipos de scripts P2SH que se relacionan con Segwit. 

La dirección P2SH-Segwit es una dirección Segwit anidada que incrusta el simple `scriptPubkey` de Segwit dentro de un script, al igual que las multifirmas están incrustadas en los scripts hoy en día: la clave de estilo Segwit se desenrolla y luego se analiza como de costumbre en una máquina que entiende Segwit. El propósito es la compatibilidad con versiones anteriores de nodos antiguos que, de otro modo, no podrían enviarse a direcciones nativas de Segwit.

La dirección P2WSH es una variante Segwit de P2SH, al igual que P2WPKH es una variante Segwit de P2WSH. Funciona con la misma lógica y se identifica por tener un hash de 32 bytes en lugar de un hash de 20 bytes. El propósito es extender las ventajas de Segwit a otros tipos de scripts.

## Que Sigue?

Continúe "Incrustando Bitcoin Scripts" con [§10.6: Spending a P2SH Transaction](10_6_Spending_a_P2SH_Transaction.md).
