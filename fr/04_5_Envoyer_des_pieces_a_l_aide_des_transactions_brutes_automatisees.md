# 4.5: Envoyer des pièces à l'aide des transactions brutes automatisées

> :information_source: **NOTE:** Ce texte est encore un brouillon, afin que je puisse obtenir un retour d'information de la part des premiers examinateurs. Il n'est pas encore prêt pour l'apprentissage.

Ce chapitre présente trois façons d'envoyer des fonds via l'interface `bitcoin-cli`. Le [§4.1](04_1_Envoi_de_pieces_par_la_voie_la_plus_simple.md) décrit comment le faire avec une simple commande, et le [§4.4](04_4_Envoi_de_pieces_a_l_aide_des_transactions_brutes.md) détaille comment utiliser une transaction brute, plus dangereuse. Cette dernière section fait la différence en montrant comment rendre les transactions brutes plus simples et plus sûres.

## Laissez Bitcoin calculer pour vous

La méthodologie des transactions brutes automatisées est simple : vous créez une transaction brute, mais vous utilisez la commande `fundrawtransaction` pour demander au `bitcoind` d'effectuer les calculs pour vous.

Pour pouvoir utiliser cette commande, vous devez vous assurer que votre fichier ~/.bitcoin/bitcoin.conf contient des variables raisonnables pour le calcul des frais de transaction. Veuillez vous reporter au [§4.1: Envoi de pièces par la voie la plus simple](04_1_Envoi_de_pieces_par_la_voie_la_plus_simple.md) pour plus d'informations à ce sujet.

Pour des chiffres très conservateurs, nous avons suggéré d'ajouter ce qui suit au fichier bitcoin.conf :
```
mintxfee=0.0001
txconfirmtarget=6
```
Pour pouvoir avancer dans ce tutoriel (et plus généralement pour faire circuler l'argent rapidement), nous avons suggéré ce qui suit :
```
mintxfee=0.001
txconfirmtarget=1
```

## Créer le squelette de votre transaction brut

Pour utiliser `fundrawtransaction`, vous devez d'abord créer une transaction brute qui n'indique _aucune_ entrée ni _aucune_ adresse pour la monnaie. Vous n'aurez qu'à indiquer le nom de votre destinataire et le montant que vous souhaitez lui envoyer :

```
$ unfinishedtx=$(bitcoin-cli -named createrawtransaction inputs='''[]''' outputs='''{ "'$recipient'": 1.0 }''')
```

## Financer cette transaction

Vous dites ensuite à `bitcoin-cli` comment financer cette transaction "toute nue" :
```
$ bitcoin-cli -named fundrawtransaction hexstring=$unfinishedtx
{
  "hex": "020000000169847669938c6a66ef790b87ebb6233059609bee4601476c5948db1a4defc9690100000000feffffff02a8e30f05000000001976a914a6f0ee37c44947f4137d56e4aab12f27ad50369188ac00e1f505000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000",
  "changepos": 0,
  "fee": 0.00022600
}
```
Cela fournit beaucoup d'informations utiles, mais une fois que vous êtes sûr de son fonctionnement, vous utiliserez plutôt JQ pour enregistrer votre `hex` dans une variable, comme d'habitude :

```
$ rawtxhex3=$(bitcoin-cli -named fundrawtransaction hexstring=$unfinishedtx | jq -r '.hex')
```
## Vérifiez votre transaction financée

Cela semble magique, donc les premières fois que vous utiliserez `fundrawtransaction`, vous voudrez probablement le vérifier.

L'exécution de `decoderawtransaction` montrera que la transaction brute est maintenant correctement configurée, utilisant un ou plusieurs de vos UTXOs et renvoyant les fonds excédentaires à une adresse pour la monnaie :

```
$ bitcoin-cli -named decoderawtransaction hexstring=$rawtxhex3
{
  "txid": "2e34603b7449d29412fb7b0d184085d4d839d965f2bba361749c20d9dbae3d0b",
  "hash": "2e34603b7449d29412fb7b0d184085d4d839d965f2bba361749c20d9dbae3d0b",
  "size": 119,
  "vsize": 119,
  "version": 2,
  "locktime": 0,
  "vin": [
    {
      "txid": "69c9ef4d1adb48596c470146ee9b60593023b6eb870b79ef666a8c9369768469",
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
      "value": 0.84927400,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 123cd8796558d195e52137ce3800e5f8120ee46f OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914123cd8796558d195e52137ce3800e5f8120ee46f88ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mhBPM8hU2PHjDTUvwa3SC7pqv8ExkK6mH8"
        ]
      }
    }, 
    {
      "value": 1.00000000,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 e7c1345fc8f87c68170b3aa798a956c2fe6a9eff OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi"
        ]
      }
    }
  ]
}
```
Nous avons vu les frais précédemment, avant de sauver le `hex` dans variable à l'aide de JQ, mais vous pouvez à nouveau vérifier avec l'alias JQ `btctxfee` :
```
$ btctxfee $rawtxhex3
.00023
```
Enfin, vous pouvez utiliser `validateaddress` pour vérifier que l'adresse générée pour la monnaie vous appartient bien :
```
$ bitcoin-cli -named validateaddress address=mhBPM8hU2PHjDTUvwa3SC7pqv8ExkK6mH8
{
  "isvalid": true,
  "address": "mhBPM8hU2PHjDTUvwa3SC7pqv8ExkK6mH8",
  "scriptPubKey": "76a914123cd8796558d195e52137ce3800e5f8120ee46f88ac",
  "ismine": true,
  "iswatchonly": false,
  "isscript": false,
  "pubkey": "029045eaa55d283526c723e6d5495d9b3f077b545563f86465aafcd9bfdd50359e",Y
  "iscompressed": true,
  "timestamp": 1489170694,
  "hdkeypath": "m/0'/0'/11'",
  "hdmasterkeyid": "144a68bde927a1fed7c2b71ad9010b0201819be5"
}
```
Regardez la valeur du champ `ismine`.

## Envoyez votre transaction financée

À ce stade, vous pouvez signer et envoyer la transaction comme d'habitude.
```
$ signedtx3=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex3 | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx3
```
Dans quelques minutes, vous aurez récupéré votre monnaie :
```
$ bitcoin-cli listunspent
[
  {
    "txid": "37698ad6e7f62df07c2fbc549339aa680a7fa18328d7ad14ecb72b21c505cbc6",
    "vout": 0,
    "address": "mhBPM8hU2PHjDTUvwa3SC7pqv8ExkK6mH8",
    "scriptPubKey": "76a914123cd8796558d195e52137ce3800e5f8120ee46f88ac",
    "amount": 0.84927400,
    "confirmations": 1,
    "spendable": true,
    "solvable": true
  }
]
```

## Résumé : Envoyer des pièces avec des transactions brutes automatisées

Si vous devez envoyer des fonds avec des transactions brutes, alors `fundrawtransaction` vous offre une alternative intéressante où les frais, les entrées et les sorties sont calculés pour vous, afin que vous ne perdiez pas accidentellement de l'argent.

### Pourquoi utiliser les transactions brutes automatisées

_Les avantages._ Elles offrent un bon équilibre. Si vous envoyez des fonds en main propre et que `sendtoaddress` n'offre pas un contrôle suffisant pour une raison quelconque, vous pouvez bénéficier de certains des avantages des transactions brutes sans en subir les dangers. Cette méthode doit être utilisée dans la mesure du possible si vous envoyez des transactions brutes à la main.

_Les inconvénients._ C'est un nid à embrouilles. Bien qu'il existe quelques options supplémentaires pour la commande `fundrawtransaction` qui n'ont pas été mentionnées ici, votre contrôle reste limité. Vous ne voudriez probablement jamais utiliser cette méthode si vous écriviez un programme dont le but est de savoir exactement ce qui se passe.

## Quelle est la prochaine étape ?

Avancez dans "bitcoin-cli" avec le [chapitre cinq : Contrôler les transactions bitcoin](05_0_Controler_les_transactions_bitcoin).