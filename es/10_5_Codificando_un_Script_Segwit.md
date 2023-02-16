# 10.5: Codificando un Script Segwit

> :information_source: **NOTA:** Esta sección se ha agregado recientemente al curso y es un borrador inicial que aún puede estar pendiente de revisión. Lector de advertencias.

Segwit introdujo una serie de nuevas opciones para los tipos de direcciones (y, por lo tanto, secuencias de comandos). [§9.5: Codificando una P2WPKH](09_5_Codificando_una_P2WPKH.md) explicó cómo el nuevo tipo de dirección Bech32 variaba los scripts estándar encontrados en la mayoría de las transacciones tradicionales. Este capitulo analiza los otros tres tipos de scripts introducidos por la actualización de Segwit: el P2SH-Segwit (que era la dirección de transición "Segwit anidada", cuando Segwit entró en uso), el P2WSH (que es el equivalente Segwit de la dirección P2SH, al igual que P2WPKH es el equivalente Segwit de la dirección P2PKH), y la dirección P2WSH anidada.

Esta es otra situación en la que realmente no tendrá que preocuparse por estos matices mientras trabaja con `bitcoin-cli`, pero es útil saber cómo funciona todo.

## Entender un Script de P2SH-Segwit

La dirección P2SH-Segwit es una raza en extinción. Básicamente, fue una medida provisional mientras Bitcoin estaba en transición a Segwit que permitía a un usuario crear una dirección de Segwit y luego tener a alguien con un intercambio o fondo de biletera no habilitado para Segwit que se dirigiera. 

Si alguna vez necesita usar uno, hay una opción para crear una dirección P2SH-Segwit usando `getnewaddress`:
```
$ bitcoin-cli getnewaddress -addresstype p2sh-segwit
2NEzBvokxh4ME4ahdT18NuSSoYvvhS7EnMU
```
La dirección comienza con un `2` (o un `3`) que la revela como un guión.

> :book: ***¿Por qué los nodos antiguos no pueden enviar a direcciones nativas de Segwit?*** [§10.1](10_1_Entendiendo_la_Base_de_P2SH.md) señaló que había un número determinado de transacciones de Bitcoin "estándar". En realidad, no puede bloquear una transacción con un script que no sea uno de esos tipos estándar. Segwit ahora se reconoce como uno de esos estándares, pero un nodo antiguo no lo sabrá, por lo que se negará a enviar una transacción de este tipo para la protección del remitente. Envolver una dirección Segwit dentro de un hash de script estándar resuelve el problema.

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
Más importante aún, hay un `redeemScript`, que decodifica a `OP_0 OP_PUSHDATA (20 bytes) 3ab2a09a1a5f2feb6c799b5ab345069a96e1a0a`. Deberia parecer familiar, porque es un `OP_0` seguido de un código hexadecimal de 20 bytes de un hash de clave pública. En otras palabras, un P2SH-SegWit es solo un SegWit `scriptPubKey` atascado en un script. Eso es todo al respecto. Coincide precisamente con la forma en que las multifirmas modernas son un multifirma puesto dentro  de un P2SH, como se explica en [§10.4: Codificando una Multifirma](10_4_Codificando_una_Multifirma.md).

Por el contrario, cuando gastamos esta transacción, se ve exactamente como un P2SH:
```
$ bitcoin-cli getrawtransaction ed752673bfd4338ccf0995983086da846ad652ae0f28280baf87f9fd44b3c45f 1
{
  "txid": "ed752673bfd4338ccf0995983086da846ad652ae0f28280baf87f9fd44b3c45f",
  "hash": "aa4b1c2bde86ea446c9a9db2f77e27421316f26a8d88869f5b195f03b1ac4f23",
  "version": 2,
  "size": 247,
  "vsize": 166,
  "weight": 661,
  "locktime": 1781316,
  "vin": [
    {
      "txid": "59178b02cfcbdee51742a4b2658df35b63b51115a53cf802bc6674fd94fa593a",
      "vout": 1,
      "scriptSig": {
        "asm": "00149ef51fb1f5adb44e20eff758d34ae64fa781fa4f",
        "hex": "1600149ef51fb1f5adb44e20eff758d34ae64fa781fa4f"
      },
      "txinwitness": [
        "3044022069a23fcfc421b44c622d93b7639a2152f941dbfd031970b8cef69e6f8e97bd46022026cb801f38a1313cf32a8685749546a5825b1c332ee4409db82f9dc85d99086401",
        "030aec1384ae0ef264718b8efc1ef4318c513403d849ea8466ef2e4acb3c5ccce6"
      ],
      "sequence": 4294967294
    }
  ],
  "vout": [
    {
      "value": 8.49029534,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_HASH160 b4b656f4c4b14ee0d098299d1d6eb42d2e22adcd OP_EQUAL",
        "hex": "a914b4b656f4c4b14ee0d098299d1d6eb42d2e22adcd87",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2N9ik3zihJ91VGNF55sZFe9GiCAXh2cVKKW"
        ]
      }
    },
    {
      "value": 0.00095000,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_HASH160 ee7aceea0865a05a29a28d379cf438ac5b6cd9c6 OP_EQUAL",
        "hex": "a914ee7aceea0865a05a29a28d379cf438ac5b6cd9c687",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2NEzBvokxh4ME4ahdT18NuSSoYvvhS7EnMU"
        ]
      }
    }
  ],
  "hex": "020000000001013a59fa94fd7466bc02f83ca51511b5635bf38d65b2a44217e5decbcf028b175901000000171600149ef51fb1f5adb44e20eff758d34ae64fa781fa4ffeffffff029e299b320000000017a914b4b656f4c4b14ee0d098299d1d6eb42d2e22adcd87187301000000000017a914ee7aceea0865a05a29a28d379cf438ac5b6cd9c68702473044022069a23fcfc421b44c622d93b7639a2152f941dbfd031970b8cef69e6f8e97bd46022026cb801f38a1313cf32a8685749546a5825b1c332ee4409db82f9dc85d9908640121030aec1384ae0ef264718b8efc1ef4318c513403d849ea8466ef2e4acb3c5ccce6442e1b00",
  "blockhash": "0000000069cbe44925fab2d472870608c7e1e241a1590fd78be10c63388ed6ee",
  "confirmations": 282952,
  "time": 1595360859,
  "blocktime": 1595360859
}
```
Esto confirma que esto es solo un P2SH normal, bloqueado por `"OP_DUP OP_HASH160 41d83eaffbf80f82dee4c152de59a38ffd0b6021 OP_EQUALVERIFY OP_CHECKSIG"`. Es cuando se ejecuta el script de canje que ocurre la magia. Al igual que con un P2WPKH, un nodo antiguo verá `OP_0 OP_PUSHDATA (20 bytes) 3ab2a09a1a5f2feb6c799b5ab345069a96e1a0a` y lo verificará automáticamente, mientras que un nuevo nodo lo verá, sabrá que es un P2WPKH, y así saldrá con los `testigos`. See [§9.5: Codificando una P2WPKH](09_5_Codificando_una_P2WPKH.md).

Cada `vout` tiene el formato `OP_HASH160 <HASH> OP_EQUAL`. Eso es un P2SH normal según [§10.2](10_2_Construyendo_la_Estructura_de_P2SH.md), lo que significa que es solo cuando se ejecuta el script de canje que ocurre la magia. Al igual que con un P2WPKH, un nodo antiguo verá `OP_0 OP_PUSHDATA (20 bytes) 3ab2a09a1a5f2feb6c799b5ab345069a96e1a0a` en el script de canje y lo verificará automáticamente, mientras que un nuevo nodo lo verá, sabrá que es un P2WPKH, y así irá a los `witnesses`. Consulte [§9.5: Codificando una P2WPKH](09_5_Codificando_una_P2WPKH.md).

> :book: ***¿Cuáles son las desventajas de las transacciones segwit anidadas?*** Son más grandes que las transacciones nativas de Segwit, por lo que obtiene algunas de las ventajas de Segwit, pero no todas.

## Entender un Script P2WSH

Por el contrario, las transacciones P2WSH deberían tener un uso cada vez mayor, ya que son el reemplazo nativo de Segwit para P2SH, ofreciendo las mismas ventajas de tamaño de bloque que se crearon con transacciones nativas Segwit P2WPKH.

Este es un ejemplo de dirección P2WSH:
[https://blockstream.info/testnet/address/tb1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3q0sl5k7](https://blockstream.info/testnet/address/tb1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3q0sl5k7)

Los detalles muestran que un UTXO enviado a esta dirección está bloqueado con una `scriptPubKey` como esta:
```
OP_0 OP_PUSHDATA (32 bytes) 1863143c14c5166804bd19203356da136c985678cd4d27a1b8c6329604903262
```
Esto funciona como una dirección P2WPKH, la única diferencia es que un lugar de un hash de clave pública de 20 bytes, el UTXO incluye un hash de script de 32 bytes. Al igual que con un P2WPKH, los nodos antiguos solo verifican esto, mientras que los nuevos nodos reconocen que se trata de un P2WSH y, por lo tanto, verifican internamente el script como se describe en las secciones anteriores, pero utilizando los datos del `testigo`, que ahora incluye el script de canje.
También hay una variante más, un script P2WSH incrustado en un script P2SH, que funciona de manera muy similar al P2SH-Segwit descrito anterioremente, pero para scripts P2WSH anidados. (Whew!)

## Resumen: Programando un script Segwit

Hay dos tipos de scripts P2SH que se relacionan con Segwit. 

La dirección P2SH-Segwit es una dirección Segwit anidada que incrusta el simple `scriptPubkey` de Segwit dentro de un script, al igual que las multifirmas están incrustadas en los scripts hoy en día: la clave de estilo Segwit se desenrolla y luego se analiza como de costumbre en una máquina que entiende Segwit. El propósito es la compatibilidad con versiones anteriores de nodos antiguos que, de otro modo, no podrían enviarse a direcciones nativas de Segwit.

La dirección P2WSH es una variante Segwit de P2SH, al igual que P2WPKH es una variante Segwit de P2WSH. Funciona con la misma lógica y se identifica por tener un hash de 32 bytes en lugar de un hash de 20 bytes. El propósito es extender las ventajas de Segwit a otros tipos de scripts.

## Que Sigue?

Continúe "Incrustando Bitcoin Scripts" con [§10.6: Gastando una Transacción P2SH](10_6_Gastando_una_Transaccion_P2SH.md).
