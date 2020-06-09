# 4.3 Création d'une transaction brute avec des arguments nommés

> :information_source: **NOTE:** Ce texte est encore un brouillon, afin que je puisse obtenir un retour d'information de la part des premiers examinateurs. Il n'est pas encore prêt pour l'apprentissage.

Il peut parfois être décourageant de trouver le bon ordre des arguments d'une commande de bitcoin-cli. Heureusement, vous pouvez utiliser des _arguments nommés_ comme alternative.

> :warning: **AVERTISSEMENT CONCERNANT LA VERSION :** Il s'agit d'une innovation de Bitcoin Core v 0.14.0. Si vous avez utilisé nos scripts d'installation, c'est ce que vous devriez avoir, mais vérifiez votre version si vous avez des problèmes. De plus, il y a un bug dans l'utilisation des arguments nommés dans la commande `createawtransaction` qui sera probablement corrigé dans la version 0.14.1.
>

## Créer un alias pour utiliser les arguments nommés

Pour utiliser un argument nommé, vous devez exécuter `bitcoin-cli` avec l'argument `-named`. Si vous prévoyez de le faire régulièrement, vous voudrez probablement créer un alias :

```
alias bitcoin-cli="bitcoin-cli -named"
```
Comme d'habitude, c'est pour vous faciliter l'utilisation, mais nous continuerons à utiliser l'ensemble des commandes pour maintenir la clarté.

## Test d'un argument nommé

Pour savoir quels sont les noms des arguments d'une commande, consultez `bitcoin-cli help`. Vous y trouverez la liste des arguments dans un ordre aproprié, mais aussi le nom de chacun d'entre eux.

Par exemple, `bitcoin-cli help getbalance` fournit cette liste :

   1. account
   2. minconf
   3. include watchonly

Ce qui suit montre une utilisation traditionnelle et peu intuitive de `getbalance` en utilisant l'argument de confirmation minimale :
```
$ bitcoin-cli getbalance "*" 1
```
Avec des arguments nommés, vous pouvez clarifier ce que vous faites, ce qui minimise également les erreurs :
```
$ bitcoin-cli -named getbalance account="*" minconf=1
```

## Tester une transaction brute

Voici à quoi ressembleraient les commandes pour envoyer une transaction brute avec des arguments nommés :
```
$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[0] | .txid') 
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
$ recipient="n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi"

$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.7595 }''')
$ bitcoin-cli -named decoderawtransaction hexstring=$rawtxhex 
{
  "txid": "f445f121085d98635f7302e641f815d1ca4ce70f0e1b03f144ad1661dc5e10e7",
  "hash": "f445f121085d98635f7302e641f815d1ca4ce70f0e1b03f144ad1661dc5e10e7",
  "size": 85,
  "vsize": 85,
  "version": 2,
  "locktime": 0,
  "vin": [
    {
      "txid": "2b5f5798359e0e23e02764588166f222d4ce056419dec83c743b72aad171d708",
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
      "value": 0.75950000,
      "n": 0,
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

$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
8000dca7b1e7ab70f4056bc4512af6ffff7727d1588436521da3e5d886dbcddf
```
Voilà ! Vous avez envoyé une autre transaction brute, mais cette fois en utilisant des arguments nommés pour plus de clarté et pour réduire les erreurs.

> :warning: **AVERTISSEMENT CONCERNANT LA VERSION:** C'est là que le BUG de Bitcoin Core 0.14 apparaît : l'argument `inputs` pour `createrawtransaction` est mal nommé `transactions`. Donc, si vous êtes sur Bitcoin Core 0.14.0, remplacez l'argument nommé `inputs` par `transactions` pour cet exemple et les suivants. Cependant, à partir de Bitcoin Core 0.14.1, ce code devrait fonctionner comme indiqué.

## Résumé : Créer une transaction brute avec des arguments nommés

En lançant `bitcoin-cli` avec l'option `-named`, vous pouvez utiliser des arguments nommés plutôt que de dépendre d'arguments ordonnés. `bitcoin-cli help` vous montrera toujours le bon nom pour chaque argument. Cela permet d'obtenir un code plus robuste, plus facile à lire et moins sujet aux erreurs.

_Ces documents utiliseront des arguments nommés pour tous les exemples futurs, par souci de clarté et pour établir les meilleures pratiques. Toutefois, ils présenteront également tous les arguments dans le bon ordre. Ainsi, si vous préférez ne pas utiliser d'arguments nommés, il suffit de supprimer le drapeau "-named" et tous les "name="s et les exemples devraient continuer à fonctionner correctement_

## Quelle est la prochaine étape ?

Continuer "Envoyer des transactions Bitcoin" avec [§4.4: Envoi de pièces à l'aide des transactions brutes](04_4_Envoi_de_pieces_a_l_aide_des_transactions_brutes.md).
