# 6.1: Enviando una Transacción con una Multifirma

La primer forma de variar cómo envías una transacción básica es utilizar multifirma. Esto da la habilidad de requerir múltiples personas (o al menos múltiples claves privadas) autoricen el uso de los fondos.

## Entender Cómo Funciona una Multifirma

En una transacción típica P2PKH o SegWit, los bitcoins son enviados a una dirección basada en tu clave pública, lo que significa que se requiere la clave privada relacionada para desbloquear la transacción, resolver el rompecabezas criptográfico y permitir la reutilización de los fondos. Pero, ¿qué pasaría si pudieras bloquear una transacción con _múltiples_ claves privadas? Esto haría que efectivamente los fondos puedan ser enviados a un grupo de personas, en la que todas esas personas tengan que acordar la reutilización de los fondos.

> :book: ***¿Qué es una multifirma?*** Una multifirma es una metodología que permite a más de una persona la creación conjunta de una firma digital. Es una técnica general para el uso criptográfico de claves que va más allá de Bitcoin.

Técnicamente, un rompecabezas criptográfico multifirma es creado por Bitcoin usando el comando OP_CHECKMULTISIG, y normalmente está encapsulado en una dirección P2SH. [§10.4: Scripting a Multisig](10_4_Scripting_a_Multisig.md) detallará con más precisión cómo funciona. Por ahora, todo lo que debes saber es que puedes usar el comando `bitcoin-cli` para crear direcciones multifirma; los fondos pueden ser enviados a estas direcciones como a cualquier otra dirección P2PKH o Segwit, pero serán requeridas múltiples claves privadas para la redención de los fondos.

> :book: ***¿Qué es una transacción multiforma?*** Una transacción mutifirma es una transacción de Bitcoin que ha sido enviada a una dirección multifirma, requiriendo así la firma de ciertas personas del grupo multifirma para reutilizar los fondos.

Multifirmas simples requieren que todos en el grupo firmen el UTXO cuando sea gastado. Sin embargo, es posible una mayor complejidad. Las multifirmas son descriptas generalmente como "m de n". Esto significa que la transacción está bloqueada con un grupo de "n" claves, pero sólo "m" de ellas son requeridas para desbloquear la transacción. 

> :book: ***¿Qué es una multifirma m-de-n?*** En una multifirma, "m" firmas de un grupo de "n" son requeridas para formar la firma, donde "m ≤ n".

## Crear una Dirección Multifirma

A fin de bloquear una UTXO con múltiples claves privadas, debes crear primero una dirección multifirma. Los ejemplos utilizados aquí muestran la creación (y el uso) de una multifirma 2-de-2.

### Crear las Direcciones

Para crear una dirección multifirma, primero debes preparar las direcciones que combinará la multifirma. Las mejores prácticas sugieren que siempre crees nuevas direcciones. Esto significa que cada uno de los participantes correrá el comando `getnewaddress` en su propia máquina:
```
machine1$ address1=$(bitcoin-cli getnewaddress)
```
Y:
```
machine2$ address2=$(bitcoin-cli getnewaddress)
```
Luego, uno de los destinatarios (o quizás algún tercero) necesitará combinar las direcciones. 

#### Recoger las Claves Públicas

Sin embargo, no puedes crear una multifirma con las direcciones, pues son los hashes de las claves públicas: en su lugar necesitas las clves públicas en sí.

Esta información está ya disponible con el comando `getaddressinfo`.

En la máquina remota, que asumimos que aquí es `machine2`, puedes sacar la información del listado. 
```
machine2$ bitcoin-cli -named getaddressinfo address=$address2
{
  "address": "tb1qr2tkjh8rs9xn5xaktf5phct0wxqufplawrfd9q",
  "scriptPubKey": "00141a97695ce3814d3a1bb65a681be16f7181c487fd",
  "ismine": true,
  "solvable": true,
  "desc": "wpkh([fe6f2292/0'/0'/1']02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3)#zc64l8dw",
  "iswatchonly": false,
  "isscript": false,
  "iswitness": true,
  "witness_version": 0,
  "witness_program": "1a97695ce3814d3a1bb65a681be16f7181c487fd",
  "pubkey": "02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3",
  "ischange": false,
  "timestamp": 1592957904,
  "hdkeypath": "m/0'/0'/1'",
  "hdseedid": "1dc70547f2b80e9bb5fde5f34fb3d85f8d8d1dab",
  "hdmasterfingerprint": "fe6f2292",
  "labels": [
    ""
  ]
}
```
La dirección `pubkey` (`02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3`) es lo que se requiere. Cópialo en tu máquina local por cualquier medio que te resulte más eficiente y _menos propenso a errores_.

Este proceso necesita ser emprendido para _cada_ dirección desde una máquina diferente de la que se está construyendo la multifirma. Obviamente, si algún tercero está creando la dirección, deberás hacer esto para cada dirección.

> :warning: **AVISO:** El uso en Bitcoin de los hashes de las claves-públicas como direcciones, en lugar de las claves públicas, en realidad representa una capa adicional de seguridad. Por lo tanto, enviar una clave pública aumenta ligeramente la vulnerabilidad de la dirección asociada, por alguna posibilidad en el futuro lejano de un compromiso de la curva elíptica. No debes preocuparte por tener que enviar ocasionalmente una clave pública para un uso como este, pero debes tener en cuenta que los hashes de las claves públicas representan seguridad y, por lo tanto, las claves públicas reales no deben enviarse a la ligera.

Si una de las direcciones se creó en tu máquina local, que asumimos que aquí es `machine1`, puedes simplemente volcar la dirección `pubkey` a una nueva variable.
```
machine1$ pubkey1=$(bitcoin-cli -named getaddressinfo address=$address1 | jq -r '.pubkey')
```

### Crear la dirección

Una multifirma puede crearse ahora con el comando `createmultisig`:
```
machine1$ bitcoin-cli -named createmultisig nrequired=2 keys='''["'$pubkey1'","02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3"]'''
{
  "address": "2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr",
  "redeemScript": "522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae",
  "descriptor": "sh(multi(2,02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191,02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3))#0pazcr4y"
}
```
> :warning: **AVISO DE VERSION:** Algunas versiones de `createmultisig` han permitido el ingreso de claves públicas o direcciones, algunas han requerido sólo claves públicas. Actualmente, cualquiera de las dos parece estar permitida.

Al crear la dirección multifirma, listas cuántas firmas se requieren con el argumento `nrequired` (esto es "m" en una multifirma "m-de-n"), luego listas el conjunto total de posibles firmas con el argumento `keys` (que es "n"). Ten en cuenta que las entradas de las `keys` probablemente provienen de diferentes lugares. En este caso, incluimos `$pubkey1` de la máquina local y `02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3` de una máquina remota. 

> :information_source: **NOTA — M-DE-N VS N-DE-N:** Este ejemplo muestra la creación de una simple multifirma 2-de-2. Si en cambio quieres crear una firma m-de-n donde "m < n", ajustas el campo `nrequired` y/o el número de firmas en el objeto JSON `keys`. Para una multifirma 1-de-2, deberías configurar `nrequired=1` y también listar dos claves, mientras que para una multifirma 2-de-3, deberías dejar `nrequired=2`, pero agregar una clave pública más a la lista `keys`.

Cuando es usado correctamente, `createmultisig` devuelve tres resultados, siendo todos de importancia crítica.

_address_ es lo que darás a la gente que desee enviar fondos. Verás que tiene un nuevo prefijo de `2`, exactamente como aquellas direcciones P2SH-SegWit. Eso es porque, como aquellas, `createmultisig` está creando un tipo totalmente nuevo de dirección llamada P2SH. Funciona exactamente igual que una direcciópn P2PKH estándar para enviar fondos, pero como esta se desarrolló para requerir direcciones múltiples, necesitarás hacer algunas tareas más para gastarlas. 

> :link: **TESTNET vs MAINNET:** En testnet, el prefijo para direcciones P2SH es `2`, mientras que en mainnet, es `3`.

_redeemScript_ es lo que necesitas para redimir los fondos (junto a las claves privadas para "m" de the "n" direcciones). Este script es otra característica especial de las direcciones P2SH y será explicada completamente en [§8.1: Building a Bitcoin Script with P2SH](08_1_Building_a_Bitcoin_Script_with_P2SH.md). Por ahora, sólo debes saber que es algo más de data lo que se requiere para obtener tu dinero.

_descriptor_ es la descripción estandarizada de una dirección que conocimos en [§3.5: Understanding the Descriptor](03_5_Understanding_the_Descriptor.md). Provee una forma en la que podrías importar esta dirección nuevamente a la otra máquina, usando el RPC `importmulti`.

> :book: ***¿Qué es una dirección P2SH?*** P2SH significa Pay-to-script-hash. Es un tipo diferente de destinatario que una dirección P2PKH estándar o incluso una Bech32, se usa para fondos cuya redención está basada en Scripts de Bitcoin más complejos. `bitcoin-cli` usa encapsulamiento P2SH para ayudar a estandarizar y simplificar sus multifirmas como "P2SH multisigs", igual que P2SH-SegWit usaba P2SH para estandarizar sus direcciones SegWit y hacerlas totalmente compatibles con versiones anteriores.

> :warning: **AVISO:** las direcciones multifirma P2SH, como las creadas por `bitcoin-cli`, tienen un límite de "m" y "n" en multifirmas basadas en el tamaño máximo del script de redención, que actualmente es de 520 bytes. Prácticamente, no llegarás a menos que hagas algo excesivo.

### Guarda tu trabajo

Aquí hay una advertencia importante: nada acerca de tu multifirma se guarda en tu billetera usando estas técnicas básicas. A fin de redimir dinero luego enviado a esta dirección multifirma, necesitarás retener dos piezas cruciales de información:

   * Una lista de las direcciones de Bitcoin usadas en la multifirma.
   * La salida `redeemScript` del `createmultsig`.
   
Técnicamente, el `redeemScript` puede ser recreado corriendo nuevamente `createmultisig` con la lista completa de claves públicas _en el mismo orden_ y con la cuenta correcta de m-de-n. Pero, es mejor guardar tu trabajo y evitar estrés y penas luego.

### Observa el Orden

Aquí hay una cosa de la que debes tener mucho cuidado: _el orden es importante_. El orden de las claves utilizadas para crear una multi-firma crea un hash único, es decir, si colocas las claves en un orden diferente, producirán una dirección diferente, como se muestra:
```
$ bitcoin-cli -named createmultisig nrequired=2 keys='''["'$pubkey1'","'$pubkey2'"]'''
{
  "address": "2NFBQvz57UzKWDr2Vx5D667epVZifjGixkm",
  "redeemScript": "52210342b306e410283065ffed38c3139a9bb8805b9f9fa6c16386e7ea96b1ba54da0321039cd6842869c1bfec13cfdbb7d8285bc4c501d413e6633e3ff75d9f13424d99b352ae",
  "descriptor": "sh(multi(2,0342b306e410283065ffed38c3139a9bb8805b9f9fa6c16386e7ea96b1ba54da03,039cd6842869c1bfec13cfdbb7d8285bc4c501d413e6633e3ff75d9f13424d99b3))#8l6hvjsk"
}
standup@btctest20:~$ bitcoin-cli -named createmultisig nrequired=2 keys='''["'$pubkey2'","'$pubkey1'"]'''
{
  "address": "2N5bC4Yc5Pqept1y8nPRqvWmFSejkVeRb1k",
  "redeemScript": "5221039cd6842869c1bfec13cfdbb7d8285bc4c501d413e6633e3ff75d9f13424d99b3210342b306e410283065ffed38c3139a9bb8805b9f9fa6c16386e7ea96b1ba54da0352ae",
  "descriptor": "sh(multi(2,039cd6842869c1bfec13cfdbb7d8285bc4c501d413e6633e3ff75d9f13424d99b3,0342b306e410283065ffed38c3139a9bb8805b9f9fa6c16386e7ea96b1ba54da03))#audl88kg"
}
```
Más notablemente, cada ordenamiento crea un diferente _redeemScript_. Esto significa que si utilizaste estas técnicas básicas y no guardaste el redeemScript como se indicó, tendrás que recorrer un número cada vez mayor de variaciones para encontrar la correcta cuando intentes gastar tus fondos!

[BIP67](https://github.com/bitcoin/bips/blob/master/bip-0067.mediawiki) sugiere una forma de ordenar claves lexicográficamente, de manera que siempre generen las mismas multifirmas. ColdCard y Electrum están entre las billeteras que ya soportan esto. Por supuesto, esto puede causar problemas por sí solo si no sabes si una dirección multifirma se creó con claves ordenadas o no ordenadas. Una vez más, [descriptors](03_5_Understanding_the_Descriptor.md) vienen al rescate. Si una multifirma es no ordenada, está construída con la función `multi` y si es ordenada está construída con la función `sortedmulti`.

Si observas el `descriptor` para la multifirma creada anteriormente, verás que Bitcoin Core no ordena actualmente sus multifirmas:
```
  "descriptor": "sh(multi(2,02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191,02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3))#0pazcr4y"
```
Sin embargo, si importa una dirección ccon el tipo `sortedmulti`, hará lo correcto, que es para lo que están los descriptores!

> :warning: **AVISO DE VERSION:** Bitcoin Core sólo entiende la función descriptor `sortedmulti` comenzando por la v 0.20.0. Intenta acceder al descriptor en una versión anterior de Bitcoin Core y obtendrás un error como `A function is needed within P2WSH` (Una función es necesaria dentro de PSWSH).

## Enviar a una dirección Multifirma

Si tienes una multifirma en un formato P2SH conveniente, como la generada por `bitcoin-cli`, puede ser enviada exactamente como una dirección normal.
```
$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[0] | .txid') 
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
$ recipient="2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr"

$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.000065}''')
$ bitcoin-cli -named decoderawtransaction hexstring=$rawtxhex 
{
  "txid": "b164388854f9701051809eed166d9f6cedba92327e4296bf8a265a5da94f6521",
  "hash": "b164388854f9701051809eed166d9f6cedba92327e4296bf8a265a5da94f6521",
  "version": 2,
  "size": 83,
  "vsize": 83,
  "weight": 332,
  "locktime": 0,
  "vin": [
    {
      "txid": "c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b",
      "vout": 0,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 0.00006500,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_HASH160 a5d106eb8ee51b23cf60d8bd98bc285695f233f3 OP_EQUAL",
        "hex": "a914a5d106eb8ee51b23cf60d8bd98bc285695f233f387",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr"
        ]
      }
    }
  ]
}

$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
b164388854f9701051809eed166d9f6cedba92327e4296bf8a265a5da94f6521
```
Como puedes ver, no hubo nada inusual en la creación de la transacción, y se veía completamente normal, aunque con una dirección con un prefijo diferente al normalthere was nothing unusual in the creation of the transaction, and it looked entirely normal, albeit with an address with a different prefix than normal (`2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr`). No surprise, as we similarly saw no difference when we sent to Bech32 addresses for the first time in [§4.6](04_6_Creating_a_Segwit_Transaction.md).

## Summary: Sending a Transaction with a Multisig

Multisig addresses lock funds to multiple private keys — possibly requiring all of those private keys for redemption, and possibly requiring just some from the set. They're easy enough to create with `bitcoin-cli` and they're entirely normal to send to. This ease is due in large part to the invisible use of P2SH (pay-to-script-hash) addresses, a large topic that we've touched upon twice now, with P2SH-SegWit and multisig addresses, and one that will get more coverage in the future.

> :fire: ***What is the power of multisignatures?*** Multisignatures allow the modeling of a variety of financial arrangements such as corporations, partnerships, committees, and other groups. A 1-of-2 multisig might be a married couple's joint bank account, while a 2-of-2 multisig might be used for large expenditures by a Limited Liability Partnership. Multisignatures also form one of the bases of Smart Contracts. For example, a real estate deal could be closed with a 2-of-3 multisig, where the signatures are submitted by the buyer, the seller, and an escrow agent. Once the escrow agent agrees that all of the conditions have been met, he frees up the funds for the seller; or alternatively, the buyer and seller can jointly free the funds.

## What's Next?

Continue "Expanding Bitcoin Transactions" with [§6.2: Spending a Transaction with a Multisig](06_2_Spending_a_Transaction_to_a_Multisig.md).
