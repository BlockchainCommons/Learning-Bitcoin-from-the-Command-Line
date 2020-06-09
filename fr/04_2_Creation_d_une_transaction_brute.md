# 4.2 Création d'une transaction brute

> :information_source: **NOTE:** Ce texte est encore un brouillon, afin que je puisse obtenir un retour d'information de la part des premiers examinateurs. Il n'est pas encore prêt pour l'apprentissage.

Vous êtes maintenant prêt à créer des transactions Bitcoin brutes. Cela vous permet d'envoyer de l'argent mais aussi de créer les transactions aussi précisément que vous le souhaitez. Cette première section se concentre sur une simple transaction à une entrée et une sortie. Ce type de transaction _n'est pas_ vraiment utile, car vous voudrez rarement envoyer tout votre argent à une seule personne (sauf si vous ne faites que le transférer, par exemple si vous passez d'un portefeuille à un autre). C'est pourquoi nous ne considérons pas cette section comme un moyen d'envoyer de l'argent. Il s'agit simplement d'un tremplin pour _réellement_ envoyer de l'argent avec une transaction brute.

## Comprendre ce qu'est une transaction Bitcoin

Avant de vous lancer dans la création de transactions brutes, vous devez vous assurer que vous comprenez comment fonctionne une transaction Bitcoin. Il s'agit des UTXO.

_Qu'est-ce qu'un UTXO ?_ Lorsque vous recevez de l'argent liquide dans votre portefeuille Bitcoin, il apparaît comme une transaction individuelle. Chacune de ces transactions est appelée sortie de transaction non dépensée (_NDT : Unspent Transaction Output, UTXO_). Peu importe si plusieurs paiements ont été effectués à la même adresse ou à des adresses multiples : chaque transaction entrante reste distincte dans votre portefeuille en tant qu'UTXO.

Lorsque vous créez une nouvelle transaction sortante, vous rassemblez une ou plusieurs UTXO, dont chacun représente une masse d'argent que vous avez reçue. Vous les utilisez comme entrée pour une nouvelle transaction. Ensemble, leur montant doit être égal _ou supérieur_ à ce que vous voulez dépenser. Ensuite, vous générez une ou plusieurs sorties, qui donnent l'argent représenté par les entrées à une ou plusieurs personnes. Cela crée de nouvelles UTXO pour les bénéficiaires, qui peuvent ensuite les utiliser pour financer de futures transactions.

Voici l'astuce : _toutes les UTXO que vous rassemblez sont entièrement dépensés !_ Cela signifie que si vous voulez envoyer une partie seulement de l'argent d'une UTXO à quelqu'un d'autre, vous devez également générer une sortie supplémentaire qui vous renvoie le reste ! Pour l'instant, nous ne nous inquiétons pas de cela, mais l'utilisation d'une adresse de change sera essentielle pour passer de la théorie de ce chapitre à des transactions réelles.

## Listez vos transactions non dépensées

Pour créer une nouvelle transaction brute, vous devez savoir quels UTXO vous avez sous la main pour pouvoir les dépenser. Vous pouvez déterminer ces informations avec la commande `bitcoin-cli listunspent` :

```
$ bitcoin-cli listunspent
[
  {
    "txid": "ee9805676271f6244eba94c3d1a48b303a8f8359bf711c630eb6f2ea339d0e72",
    "vout": 0,
    "address": "mrS1ypy2pCweh2nBpkMD7r2T3Zj344wxaY",
    "account": "",
    "scriptPubKey": "76a91477ba616a2778b05a5fd73c7449964050fd1a6fd288ac",
    "amount": 0.08000000,
    "confirmations": 2,
    "spendable": true,
    "solvable": true
  },
  {
    "txid": "c1abb6951e6a9aae7e384412b69b69e59c10daac9397d01d0c52b7bc6278d589",
    "vout": 1,
    "address": "mygxipnJUsBgFvscKAaoxDdE8aCmHhRfTZ",
    "account": "",
    "scriptPubKey": "76a914c756c7bd67bf83d83c04e3dc6fd1ff0c6fe8ea9888ac",
    "amount": 0.07800000,
    "confirmations": 1,
    "spendable": true,
    "solvable": true
  },
  {
    "txid": "ab7ca727055b812df882298f4e6e10ec699fb6250d843c813623171781f896d8",
    "vout": 0,
    "address": "mygxipnJUsBgFvscKAaoxDdE8aCmHhRfTZ",
    "account": "",
    "scriptPubKey": "76a914c756c7bd67bf83d83c04e3dc6fd1ff0c6fe8ea9888ac",
    "amount": 0.07800000,
    "confirmations": 1,
    "spendable": true,
    "solvable": true
  }
]
```
Cette liste montre trois UTXO différents, d'une valeur de 0,08, 0,078 et 0,078 BTC. Notez que chacune a son propre txid distinct et reste distincte dans le portefeuille, même si deux d'entre elles ont été envoyées à la même adresse. (Mauvaise pratique ! Mais utilisé ici comme exemple pour montrer que les transactions restent distinctes, quoi qu'il arrive).

Lorsque vous voulez dépenser un UTXO, il ne suffit pas de connaître l'identifiant de la transaction. En effet, chaque transaction peut avoir plusieurs sorties ! Vous souvenez-vous de la première pièce que le robinet nous a envoyée ? Lors de la transaction, une partie de l'argent nous est revenue et une autre est allée à quelqu'un d'autre. Le `txid` fait référence à l'ensemble de la transaction, tandis que le `vout` indique laquelle des multiples sorties vous avez reçue. Dans cette liste, deux de nos UTXO sont la 0ème vout d'une transaction, et l'autre est la 1ère. Cela fait une différence !

Donc, txid+vout=UTXO. Ce sera la base de toute transaction brute.

## Ecrire une transaction brute à une sortie

Vous êtes maintenant prêt à rédiger un exemple de transaction brute simple qui montre comment envoyer l'intégralité d'un UTXO à une autre partie. Comme indiqué, il ne s'agit pas nécessairement d'un cas réel très réaliste.

> :warning: **AVERSTISSEMENT :** Il est très facile de perdre de l'argent avec une transaction brute. Toutes les instructions relatives à l'envoi de bitcoins via des transactions brutes sont _très, très_ dangereuses. Lorsque vous envoyez de l'argent réel à d'autres personnes, vous devriez plutôt utiliser l'une des autres méthodes expliquées dans ce chapitre. La création de transactions brutes est extrêmement utile si vous écrivez des programmes bitcoin, mais _seulement_ lorsque vous écrivez des programmes bitcoin.

### Préparer la transaction brute

Comme bonne pratique, nous commencerons chaque transaction en enregistrant soigneusement les txids et les vouts que nous allons dépenser.

Dans ce cas, nous allons dépenser la transaction la plus ancienne, d'une valeur de 0,08 BTC, car c'est celle qui a été la plus validée:

```
$ utxo_txid="ee9805676271f6244eba94c3d1a48b303a8f8359bf711c630eb6f2ea339d0e72"
$ utxo_vout="0"
```

> :link: **TESTNET vs MAINNET:** Il est évident que le critère "validé-le-plus-possible" serait beaucoup plus important sur le réseau principal, où de l'argent réel est utilisé.

Vous devez également enregistrer l'adresse de votre destinataire, pour vous assurer que vous l'avez bien. Nous envoyons à nouveau de l'argent au robinet TP:
```
$ recipient="n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi"
```
Comme toujours, vérifiez attentivement vos variables, pour vous assurer qu'elles correspondent à vos attentes !
```
$ echo $utxo_txid
ee9805676271f6244eba94c3d1a48b303a8f8359bf711c630eb6f2ea339d0e72
$ echo $utxo_vout
0
$ echo $recipient
n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi
```
Ce destinataire est particulièrement important, car si vous vous trompez, votre argent disparaît ! Alors, vérifiez bien.

### Rédiger la transaction brute

Vous êtes maintenant prêt à créer la transaction brute. Cela utilise la commande `createrawtransaction`, qui peut sembler un peu intimidante. C'est parce que la commande `createrawtransaction` ne vous protège pas entièrement du RPC JSON que le bitcoin-cli utilise. Au lieu de cela, vous allez entrer un tableau JSON pour lister les UTXOs que vous dépensez et un objet JSON pour lister les sorties.

Voici le format standard:
```
$ bitcoin-cli createrawtransaction
'''[
     {
       "txid": "'$your_txid'",
       "vout": '$your_vout'
      }
]'''
'''{
   "'$your_recipient'": bitcoin_amount
 }'''
```
 Oui, il y a tout plein d'apostrophes, mais croyez bien qu'elles feront ce qu'il faut. Utilisez `'''` pour marquer le début et la fin du tableau JSON et de l'objet JSON. Protégez les mots courants comme "this" et les nombres courants comme 0. S'il s'agit de variables, insérez des guillemets simples, comme "'$this_word'" et '$this_num'. (Ouf. Vous vous y habituerez).

 Voici une commande qui crée une transaction brute pour envoyer votre $utxo à votre $destinataire (_NDT : $recipient_)

 ```
$ rawtxhex=$(bitcoin-cli createrawtransaction '''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' '''{ "'$recipient'": 0.0795 }''')
$ echo $rawtxhex
0100000001720e9d33eaf2b60e631c71bf59838f3a308ba4d1c394ba4e24f67162670598ee0000000000ffffffff01b04e7900000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000
 ```

### Comprendre les frais de transaction

Vous noterez que nous n'avons pas envoyé la totalité du .08 BTC à notre destinataire. Cela s'explique par les frais de transaction, qui sont _implicites_ lorsque vous envoyez une transaction brute : le montant que vous paierez à titre de frais est toujours égal au montant de votre entrée moins le montant de votre sortie. Vous devez donc diminuer un peu votre sortie par rapport à votre entrée pour vous assurer que votre transaction soit envoyée.

> :warning: **AVERTISSEMENT:** C'est la partie la plus dangereuse des transactions brutes ! Parce que vous dépensez automatiquement toute la quantité d'UTXO que vous utilisez, il est essentiel de s'assurer que vous savez : (1) quelles UTXO vous utilisez précisément ; (2) combien d'argent elles contiennent exactement ; (3) combien d'argent vous envoyez exactement ; et (4) quelle est la différence. Si vous vous trompez et que vous utilisez la mauvaise UTXO (avec plus d'argent que vous ne le pensiez) ou si vous envoyez trop peu d'argent, l'excédent est perdu. Pour toujours. Ne faites pas cette erreur ! Connaissez _précisément_ vos entrées et vos sorties. Ou mieux encore, n'utilisez pas de transactions brutes, sauf dans le cadre d'un programme soigneusement étudié et vérifié trois fois.

_Combien devez-vous dépenser en frais de transaction ?_ [Bitcoin Fees](https://bitcoinfees.21.co/) fournit une bonne évaluation en direct. Il indique que "les frais de transaction les plus rapides et les moins chers sont actuellement de 220 satoshis/octet" et que "pour une taille médiane des transactions de 226 octets, cela donne des frais de 49 720 satoshis".

Cette information de base est celle que nous avons utilisée pour construire notre transaction brute. Nous avons juste soustrait 50 000 satoshis, soit 0,0005 BTC, du montant que nous envoyions : 0,0800 BTC - 0,0005 BTC= 0,0795 BTC, ce qui correspond à ce que nous avons envoyé. (Souvent, les transactions n'ont pas besoin d'être les plus "rapides" et peuvent s'en tirer avec des frais de transaction beaucoup moins élevés ; nous avons choisi de ne pas le faire parce que nous ne voulons pas retarder le travail de ce tutoriel).

> :warning: **AVERTISSEMENT:** Plus les frais de transaction que vous fixez sont faibles, plus la durée de la transaction est longue avant qu'elle ne soit ajoutée à un bloc. Le site Bitcoin Fees répertorie les délais prévus, de 0 à 22 blocs prévus. Comme les blocs sont construits en moyenne toutes les 10 minutes, cela représente de quelques minutes à quelques heures ! Choisissez donc un tarif de transaction adapté à ce que vous envoyez. Notez que vous ne devez jamais descendre en dessous du tarif minimum de relais, qui est de 0,0001 BTC.

### Vérifiez votre transaction brute

Vous devez ensuite vérifier votre transaction brute avec `decoderawtransaction` pour vous assurer que tout se passera bien.

```
$ bitcoin-cli decoderawtransaction $rawtxhex
{
  "txid": "6d39d612fe382eaab9897d3ea4d5e44233d8acef40e63fe0f06df785fd7a0c45",
  "hash": "6d39d612fe382eaab9897d3ea4d5e44233d8acef40e63fe0f06df785fd7a0c45",
  "size": 85,
  "vsize": 85,
  "version": 1,
  "locktime": 0,
  "vin": [
    {
      "txid": "ee9805676271f6244eba94c3d1a48b303a8f8359bf711c630eb6f2ea339d0e72",
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
      "value": 0.07950000,
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
```
Vérifiez le `vin`. Dépensez-vous la bonne transaction ? Contient-elle la somme d'argent attendue ? Vérifiez votre `vout`. Envoyez-vous le bon montant ? Est-il envoyé à la bonne adresse ? Enfin, faites le calcul pour vous assurer que la balance est à l'équilibre. La valeur de l'UTXO moins le montant dépensé est-elle égale aux frais de transaction prévus ?

> :information_source:  **NOTE - SEQUENCE:** Vous pouvez noter que chaque entrée a un numéro de séquence, fixé ici à 4294967295, c'est-à-dire 0xFFFFFFFFFF. C'est la dernière frontière des transactions Bitcoin, car il s'agit d'un champ standard dans les transactions qui était à l'origine destiné à un but spécifique, mais qui n'a jamais été entièrement mis en œuvre. Il y a donc maintenant ce nombre entier dans les transactions qui pourrait être réutilisé pour d'autres usages. Et, en fait, c'est ce qui s'est passé. Au moment où nous écrivons ces lignes, il y a trois utilisations différentes de la variable appelée nSequence dans le code de base de Bitcoin : elle rend possible RBF, nLockTime et les timelocks relatifs. Si tout est normal, nSequence sera définie à 4294967295. Une valeur inférieure indique qu'il se passe quelque chose de spécial.

### Signer la transaction brute

Jusqu'à présent, votre transaction brute n'est que quelque chose de théorique : vous _pourriez_ l'envoyer, mais rien n'a été promis. Vous devez faire quelques démarches pour l'envoyer sur le réseau.

Tout d'abord, vous devez signer votre transaction brute :

```

$ bitcoin-cli signrawtransactionwithwallet $rawtxhex
{
  "hex": "0100000001720e9d33eaf2b60e631c71bf59838f3a308ba4d1c394ba4e24f67162670598ee000000006b483045022100d8f17dadc2501596f75f2c90b8279130e588638d4f7a4f7d5ebb10fea15252f702200ceb164e81335c430893780d06cfe194c36acec26886f180408e3ac4a7d2292f0121035de6239e70523c8f392e32f98e65f6ef704c4b6b0df994e407212b839bf51048ffffffff01b04e7900000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000",
  "complete": true
}
$ signedtx="0100000001720e9d33eaf2b60e631c71bf59838f3a308ba4d1c394ba4e24f67162670598ee000000006b483045022100d8f17dadc2501596f75f2c90b8279130e588638d4f7a4f7d5ebb10fea15252f702200ceb164e81335c430893780d06cfe194c36acec26886f180408e3ac4a7d2292f0121035de6239e70523c8f392e32f98e65f6ef704c4b6b0df994e407212b839bf51048ffffffff01b04e7900000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000"
```
Notez que nous avons entré l'entier hexadécimal à la main, plutôt que d'essayer de l'analyser à partir de l'objet JSON. Un logiciel appelé "JQ" pourrait faire mieux, comme nous l'expliquerons dans un prochain interlude.

### Envoyer la transaction brute

Vous avez maintenant une transaction brute prête à l'emploi, mais elle ne compte pas tant que vous ne l'avez pas réellement mise sur le réseau, ce que vous faites avec la commande `sendrawtransaction`. Vous obtiendrez un txid :
```
$ bitcoin-cli sendrawtransaction $signedtx
bf02068e3d2a99542c6a9ad07d915f1664374e911d809afe156dbacc46a5183e
```
Vous verrez immédiatement que l'UTXO et son argent ont été retirés de votre compte :
```
$ bitcoin-cli listunspent
[
  {
    "txid": "c1abb6951e6a9aae7e384412b69b69e59c10daac9397d01d0c52b7bc6278d589",
    "vout": 1,
    "address": "mygxipnJUsBgFvscKAaoxDdE8aCmHhRfTZ",
    "account": "",
    "scriptPubKey": "76a914c756c7bd67bf83d83c04e3dc6fd1ff0c6fe8ea9888ac",
    "amount": 0.07800000,
    "confirmations": 12,
    "spendable": true,
    "solvable": true
  },
  {
    "txid": "ab7ca727055b812df882298f4e6e10ec699fb6250d843c813623171781f896d8",
    "vout": 0,
    "address": "mygxipnJUsBgFvscKAaoxDdE8aCmHhRfTZ",
    "account": "",
    "scriptPubKey": "76a914c756c7bd67bf83d83c04e3dc6fd1ff0c6fe8ea9888ac",
    "amount": 0.07800000,
    "confirmations": 12,
    "spendable": true,
    "solvable": true
  }
]
$ bitcoin-cli getbalance
0.15600000
```
Bientôt, `listtransactions` devrait afficher une transaction confirmée dans la catégorie "send".
```
 {
    "account": "",
    "address": "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi",
    "category": "send",
    "amount": -0.07950000,
    "vout": 0,
    "fee": -0.00050000,
    "confirmations": 1,
    "blockhash": "0000000000000dd6f6f455be7eecaf8055bb61d5d18d142d75bcdf8aa6d81456",
    "blockindex": 3,
    "blocktime": 1488410944,
    "txid": "bf02068e3d2a99542c6a9ad07d915f1664374e911d809afe156dbacc46a5183e",
    "walletconflicts": [
    ],
    "time": 1488410872,
    "timereceived": 1488410872,
    "bip125-replaceable": "no",
    "abandoned": false
  }
```
Vous pouvez voir que le txid correspond, ainsi que l'adresse du destinataire. Non seulement il indique le montant envoyé, mais il indique également les frais de transaction. Et il a déjà reçu une confirmation, car nous avons proposé un tarif qui permet de l'ajouter à un bloc rapidement.

Félicitations ! Vous êtes maintenant un peu plus pauvre de quelques satoshis !

## Résumé : Création d'une transaction brute

Lorsque de l'argent arrive dans votre portefeuille Bitcoin, il reste sous forme de montants distincts, appelés UTXO. Lorsque vous créez une transaction brute pour renvoyer cet argent, vous utilisez un ou plusieurs UTXO pour le financer. Vous pouvez ensuite créer une transaction brute, la signer et l'envoyer sur le réseau Bitcoin. Toutefois, il ne s'agit que d'une base : vous devrez généralement créer une transaction brute avec plusieurs sorties pour envoyer réellement quelque chose sur le réseau Bitcoin !

## Quelle est la prochaine étape ?

Prenez du recul avec : [Interlude : Utilisation de JQ](04_2__Interlude_Utilisation_de_JQ.md).