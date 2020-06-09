# 4.4: Envoi de pièces à l'aide des transactions brutes

> :information_source: **NOTE:** Ce texte est encore un brouillon, afin que je puisse obtenir un retour d'information de la part des premiers examinateurs. Il n'est pas encore prêt pour l'apprentissage.

Comme indiqué au début de ce chapitre, l'interface `bitcoin-cli` propose trois façons d'envoyer des pièces. Le [§4.1](04_1_Envoi_de_pieces_par_la_voie_la_plus_simple.md) parlait de la première façon, à savoir l'utilisation de la commande `sendtoaddress`. Puis nous avons appris de manière détaillée comment envoyer des pièces par un deuxième moyen : les transactions brutes. Le [§4.2](04_2_Creation_d_une_transaction_brute.md) enseignait comment créer une transaction brute, un [Interlude](04_2__Interlude_Utilisation_de_JQ.md) expliquait JQ, et le [§4.3](04_3_Creation_d_une_transaction_brute_avec_des_arguments_nommes.md) montrait l'utilisation des arguments nommés.

Nous pouvons maintenant les rassembler et envoyer des fonds en utilisant une transaction brute.

## Créer une adresse pour la monnaie

Notre exemple de transaction brute de la section 4.2 était très simpliste : nous avons envoyé la totalité d'un UTXO à une nouvelle adresse. Plus fréquemment, vous voudrez envoyer à quelqu'un une somme d'argent qui ne correspond pas à une UTXO. Mais vous vous rappellerez que l'argent excédentaire d'une UTXO qui n'est pas envoyée à votre destinataire devient simplement un frais de transaction. Alors, comment envoyer à quelqu'un une partie seulement d'un UTXO, tout en gardant le reste pour vous ?

La solution consiste à _envoyer_ le reste des fonds à une deuxième adresse, une adresse pour la monnaie (_NDT : change_) que vous avez créée dans votre portefeuille spécialement pour les recevoir :

```
$ changeaddress=$(bitcoin-cli getrawchangeaddress)
$ echo $changeaddress
myrK8U3SE1nWh9y9XPho5aTrKYW6n8qSQv
```
Notez que cette fonction utilise une nouvelle fonction : `getrawchangeaddress`. Elle est en grande partie identique à `getnewaddress` mais est optimisée pour être utilisée comme adresse de retour de la monnaie dans une transaction brute, donc elle ne fait pas des choses comme créer des entrées dans votre carnet d'adresses.

Vous avez maintenant une adresse supplémentaire dans votre portefeuille, de sorte que vous pouvez recevoir de la monnaie d'un UTXO ! Pour l'utiliser, vous devez créer une transaction brute comportant deux sorties.

## Choisir un nombre suffisant d'UTXOs

Notre exemple de transaction brute était simplifié également pour une autre raison : il supposait qu'il y avait suffisamment d'argent dans un seul UTXO pour couvrir la transaction. C'est souvent le cas, mais il arrive parfois que vous souhaitiez créer des transactions qui dépensent plus d'argent que ce que vous avez dans une seule UTXO. Pour ce faire, vous devez créer une transaction brute avec deux (ou plus) entrées.

## Ecrire une véritable transaction brute

Pour résumer : la création d'une véritable transaction brute pour envoyer des pièces nécessitera parfois plusieurs entrées et presque toujours plusieurs sorties, dont une adresse pour la monnaie. Nous allons créer ce type de transaction plus réaliste ici, dans un nouvel exemple qui montre un exemple réel d'envoi de fonds via la deuxième méthodologie de Bitcoin, les transactions brutes.

Voici les UTXOs que nous utiliserons :

```
$ bitcoin-cli listunspent
[
  {
    "txid": "ec0598918f6f5476cb90365651e8a2724ef26f949290bbf196f41ed96092a52f",
    "vout": 0,
    "address": "mjtEqr4Fffd1XtpAkKoDkMBP54mMXJeQ3j",
    "account": "",
    "scriptPubKey": "76a9142fe70d51e886b9ef73b76c1743c5a2bb2894db8588ac",
    "amount": 3.90000000,
    "confirmations": 9551,
    "spendable": true,
    "solvable": true
  },
  {
    "txid": "3470e5fe08633583d136b9cd49bb1a224c9d9313a0b4584fd3b7438dbdf34dbd",
    "vout": 0,
    "address": "msiyutru5TV33Q2UGK2Bbh2ewdrYALBzTb",
    "account": "",
    "scriptPubKey": "76a91485e7d9fe99708d575f3b93be13c0c55c6ffb765088ac",
    "amount": 1.95000000,
    "confirmations": 9542,
    "spendable": true,
    "solvable": true
  }
]

```
Dans notre exemple, nous allons envoyer 4.0 BTC, ce qui est plus grand que l'un ou l'autre de nos UTXO. Pour cela, il faut les combiner, puis utiliser notre adresse pour récupérer les fonds non dépensés.

### Définissez vos variables

Nous avons déjà les variables `$changeaddress` et `$recipient` des exemples précédents :

```
$ echo $changeaddress
myrK8U3SE1nWh9y9XPho5aTrKYW6n8qSQv
~$ echo $recipient
n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi
```
Nous devons également enregistrer le txid et le vout pour chacun de nos deux UTXOs. Après avoir identifié les UTXOs que nous voulons dépenser, nous pouvons utiliser JQ pour nous assurer que l'accès à ces UTXO est sans erreur :
```
$ utxo_txid_1=$(bitcoin-cli listunspent | jq -r '.[0] | .txid')
$ utxo_vout_1=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
$ utxo_txid_2=$(bitcoin-cli listunspent | jq -r '.[1] | .txid')
$ utxo_vout_2=$(bitcoin-cli listunspent | jq -r '.[1] | .vout')
```

### Ecrire la transaction

Ecrire la transaction brute réelle est étonnamment simple. Il suffit d'inclure un objet JSON supplémentaire, séparé par des virgules, dans le tableau des entrées JSON et une paire clé-valeur supplémentaire, séparée par des virgules, dans l'objet JSON des sorties.

Voici l'exemple. Notez les entrées multiples après l'argument `inputs`  et les sorties multiples après l'argument `outputs` .

```
$ rawtxhex2=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid_1'", "vout": '$utxo_vout_1' }, { "txid": "'$utxo_txid_2'", "vout": '$utxo_vout_2' } ]''' outputs='''{ "'$recipient'": 4.0, "'$changeaddress'": 1.8495 }''')
```
Nous avons fait _très_ attention à nos calculs d'argent. Ces deux UTXOs contiennent 5,85 BTC. Après avoir envoyé 4,0 BTC, il nous restera 1,85 BTC. Nous avons de nouveau choisi 0,0005 BTC pour les frais de transaction. Pour tenir compte de ces frais, nous avons fixé notre monnaie à 1,8495 BTC. Si nous avions fait une erreur de calcul en fixant cette valeur à 1,7495 BTC, ce 0,1 BTC supplémentaire aurait été perdu. Ce sont 100 dollars qui ont été versés aux mineurs ! Si nous avions oublié de rendre la monnaie, la totalité des 1,85 BTC (2 000 $ !) aurait disparu. Alors, encore une fois, _soyez prudents_. 

Heureusement, nous pouvons tout bien vérifier grâce à l'alias `btctxfee` de l'interlude JQ :

```
$ btctxfee $rawtxhex2
.0005
```

### Terminer

Vous pouvez maintenant signer, sceller et livrer votre transaction, et elle est à vous (et au robinet) :
```
$ signedtx2=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex2 | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx2
69c9ef4d1adb48596c470146ee9b60593023b6eb870b79ef666a8c9369768469
```

### Attendre

Comme d'habitude, votre argent sera en circulation pendant un certain temps : la monnaie ne sera pas disponible tant que la transaction ne sera pas effectivement confirmée et qu'un nouvel UTXO ne vous aura pas été remis.

Mais, dans 10 minutes ou moins (probablement), vous aurez récupéré votre argent restant et vous pourrez à nouveau le dépenser entièrement :

```
$ bitcoin-cli listunspent
[
  {
    "txid": "69c9ef4d1adb48596c470146ee9b60593023b6eb870b79ef666a8c9369768469",
    "vout": 1,
    "address": "myrK8U3SE1nWh9y9XPho5aTrKYW6n8qSQv",
    "scriptPubKey": "76a914c91b8f2f983aa9f8f0ba552adf6b6491ac01e02888ac",
    "amount": 1.84950000,
    "confirmations": 2,
    "spendable": true,
    "solvable": true
  }
]
```

C'est également le moment de revisiter un explorateur de chaîne de bloc, afin de voir plus intuitivement comment les entrées, les sorties et les frais de transaction sont enregistrés : [69c9ef4d1adb48596c470146ee9b60593023b6eb870b79ef666a8c9369768469](https://live.blockcypher.com/btc-testnet/tx/69c9ef4d1adb48596c470146ee9b60593023b6eb870b79ef666a8c9369768469/).

## Résumé : Envoyer des pièces avec des transactions brutes

Pour envoyer des pièces avec des transactions brutes, vous devez créer une transaction brute avec une ou plusieurs entrées (pour avoir des fonds suffisants) et une ou plusieurs sorties (pour récupérer la monnaie). Ensuite, vous pouvez suivre votre procédure normale d'utilisation de la transaction brute avec arguments nommés et JQ, comme indiqué dans les sections précédentes.

### Pourquoi utiliser les transactions brutes

_Les avantages._ Cela vous donne le meilleur contrôle. Si votre objectif est d'écrire un script ou un programme Bitcoin plus complexe, vous utiliserez probablement des transactions brutes afin de savoir exactement ce qui se passe. C'est également la façon la plus sûre d'utiliser des transactions brutes, car vous pouvez vous assurer par programmation que vous ne faites pas d'erreurs.

_Les inconvénients._ Il est facile de perdre de l'argent. Il n'y a pas d'avertissement, pas de garde-fous, et pas de filet de sécurité à moins que vous ne les programmiez. De plus, c'est obscur. Le formatage est horrible, même en utilisant l'interface conviviale `bitcoin-cli`, et vous devez faire beaucoup de recherches et de calculs à la main.

## What's Next?

Continuer "Envoyer des transactions Bitcoin" avec [§4.5: Envoyer des pièces à l'aide des transactions brutes automatisées](04_5_Envoyer_des_pieces_a_l_aide_des_transactions_brutes_automatisees.md).
