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

A multisig can now be created with the `createmultisig` command:
```
machine1$ bitcoin-cli -named createmultisig nrequired=2 keys='''["'$pubkey1'","02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3"]'''
{
  "address": "2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr",
  "redeemScript": "522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae",
  "descriptor": "sh(multi(2,02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191,02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3))#0pazcr4y"
}
```
> :warning: **VERSION WARNING:** Some versions of `createmultisig` have allowed entry of public keys or addresses, some have required public keys only. Currently, either one seems to be allowed.

When creating the multisignature address, you list how many signatures are required with the `nrequired` argument (that's "m" in a "m-of-n" multisignature), then you list the total set of possible signatures with the `keys` argument (that's "n"). Note that the the `keys` entries likely came from different places. In this case, we included `$pubkey1` from the local machine and `02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3` from a remote machine. 

> :information_source: **NOTE — M-OF-N VS N-OF-N:** This example shows the creation of a simple 2-of-2 multisig. If you instead want to create an m-of-n signature where "m < n", you adjust the `nrequired` field and/or the number of signatures in the `keys` JSON object. For a 1-of-2 multisig, you'd set `nrequired=1` and also list two keys, while for a 2-of-3 multisig, you'd leave `nrequired=2`, but add one more public key to the `keys` listing.

When used correctly, `createmultisig` returns three results, all of which are critically important.

The _address_ is what you'll give out to people who want to send funds. You'll notice that it has a new prefix of `2`, exactly like those P2SH-SegWit addresses. That's because, like them, `createmultisig` is actually creating a totally new type of address called a P2SH address. It works exactly like a standard P2PKH address for sending funds, but since this one has been built to require multiple addresses, you'll need to do a little more work to spend them. 

> :link: **TESTNET vs MAINNET:** On testnet, the prefix for P2SH addresses is `2`, while on mainnet, it's `3`.

The _redeemScript_ is what you need to redeem the funds (along with the private keys for "m" of the "n" addresses). This script is another special feature of P2SH addresses and will be fully explained in [§8.1: Building a Bitcoin Script with P2SH](08_1_Building_a_Bitcoin_Script_with_P2SH.md). For now, just be aware that it's a bit of data that's required to get your money.

The _descriptor_ is the standardized description for an address that we met in [§3.5: Understanding the Descriptor](03_5_Understanding_the_Descriptor.md). It provides one way that you could import this address back to the other machine, using the `importmulti` RPC.

> :book: ***What is a P2SH address?*** P2SH stands for Pay-to-script-hash. It's a different type of recipient than a standard P2PKH address or even a Bech32, used for funds whose redemption are based on more complex Bitcoin Scripts. `bitcoin-cli` uses P2SH encapsulation to help standardize and simplify its multisigs as "P2SH multisigs", just like P2SH-SegWit was using P2SH to standardize its SegWit addresses and make them fully backward compatible.

> :warning: **WARNING:** P2SH multisig addresses, like the ones created by `bitcoin-cli`, have a limit for "m" and "n" in multisigs based on the maximum size of the redeem script, which is currently 520 bytes. Practically, you won't hit this unless you're doing something excessive.

### Save Your Work

Here's an important caveat: nothing about your multisig is saved into your wallet using these basic techniques. In order to later redeem money sent to this multisignature address, you're going to need to retain two crucial bits of information:

   * A list of the Bitcoin addresses used in the multisig.
   * The `redeemScript` output by `createmultsig`.
   
Technically, the `redeemScript` can be recreated by rerunning `createmultisig` with the complete list of public keys _in the same order_ and with the right m-of-n count. But, it's better to hold onto it and save yourself stress and grief.

### Watch the Order

Here's one thing to be very wary of: _order matters_. The order of keys used to create a multi-sig creates a unique hash, which is to say if you put the keys in a different order, they'll produce a different address, as shown:
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
More notably, each ordering creates a different _redeemScript_. That means that if you used these basic techniques and failed to save the redeemScript as you were instructed, you'll have to walk through an ever-increasing number of variations to find the right one when you try and spend your funds!

[BIP67](https://github.com/bitcoin/bips/blob/master/bip-0067.mediawiki) suggests a way to lexicographically order keys, so that they always generate the same multisignatures. ColdCard and Electrum are among the wallets that already support this. Of course, this can cause troubles on its own if you don't know if a multisig address was created with sorted or unsorted keys. Once more, [descriptors](03_5_Understanding_the_Descriptor.md) come to the rescue. If a multisig is unsorted, it's built with the function `multi` and if it's sorted it's built with the function `sortedmulti`.

If you look at the `desc`riptor for the multisig that you created above, you'll see that Bitcoin Core doesn't currently sort its multisigs:
```
  "descriptor": "sh(multi(2,02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191,02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3))#0pazcr4y"
```
However, if it imports an address with type `sortedmulti`, it'll do the right thing, which is the whole point of descriptors!

> :warning: **VERSION WARNING:** Bitcoin Core only understands the `sortedmulti` descriptor function beginning with v 0.20.0. Try and access the descriptor on an earlier version of Bitcoin Core and you'll get an error such as `A function is needed within P2WSH`.

## Send to a Multisig Address

If you've got a multisignature in a convenient P2SH format, like the one generated by `bitcoin-cli`, it can be sent to exactly like a normal address.
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
As you can see, there was nothing unusual in the creation of the transaction, and it looked entirely normal, albeit with an address with a different prefix than normal (`2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr`). No surprise, as we similarly saw no difference when we sent to Bech32 addresses for the first time in [§4.6](04_6_Creating_a_Segwit_Transaction.md).

## Summary: Sending a Transaction with a Multisig

Multisig addresses lock funds to multiple private keys — possibly requiring all of those private keys for redemption, and possibly requiring just some from the set. They're easy enough to create with `bitcoin-cli` and they're entirely normal to send to. This ease is due in large part to the invisible use of P2SH (pay-to-script-hash) addresses, a large topic that we've touched upon twice now, with P2SH-SegWit and multisig addresses, and one that will get more coverage in the future.

> :fire: ***What is the power of multisignatures?*** Multisignatures allow the modeling of a variety of financial arrangements such as corporations, partnerships, committees, and other groups. A 1-of-2 multisig might be a married couple's joint bank account, while a 2-of-2 multisig might be used for large expenditures by a Limited Liability Partnership. Multisignatures also form one of the bases of Smart Contracts. For example, a real estate deal could be closed with a 2-of-3 multisig, where the signatures are submitted by the buyer, the seller, and an escrow agent. Once the escrow agent agrees that all of the conditions have been met, he frees up the funds for the seller; or alternatively, the buyer and seller can jointly free the funds.

## What's Next?

Continue "Expanding Bitcoin Transactions" with [§6.2: Spending a Transaction with a Multisig](06_2_Spending_a_Transaction_to_a_Multisig.md).
