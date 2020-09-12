# 5.2: Ré-envoyer une transaction avec le RBF

> :information_source: **NOTE:** Ce texte est encore un brouillon, afin que je puisse obtenir un retour d'information de la part des premiers examinateurs. Il n'est pas encore prêt pour l'apprentissage.

Si votre transaction Bitcoin est bloquée et que vous êtes l'expéditeur, vous pouvez la renvoyer en utilisant le système RBF (replace-by-fee). Toutefois, ce n'est pas tout ce que le RBF peut faire : c'est plus généralement une fonction puissante et polyvalente qui permet aux expéditeurs de Bitcoin de recréer des transactions pour des raisons diverses.

:warning: **AVERTISSEMENT VERSION** Il s'agit d'une innovation de Bitcoin Core v 0.12.0, qui a atteint sa pleine maturité dans le portefeuille Bitcoin core avec Bitcoin Core v 0.14.0. Il est évident que la plupart des gens devraient l'utiliser maintenant.

## Optez pour le RBF

Le RBF est une fonctionnalité optionnelle de Bitcoin. Les transactions ne peuvent utiliser le RBF que si elles ont été créées avec un drapeau RBF spécial. Pour ce faire, il suffit de définir un des numéros de séquence UTXO de la transaction (qui sont généralement définis automatiquement), de sorte qu'il soit supérieur à 0 et inférieur à 0xffffffffff-1 (4294967294).

Pour ce faire, il suffit d'ajouter une variable `sequence` à vos entrées UTXO :

```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "sequence": 1 } ]''' outputs='''{ "'$recipient'": 0.1, "'$changeaddress'": 0.9 }''')
```
Vous devez bien sûr signer et envoyer votre transaction comme d'habitude :

```
$ signedtx=$(bitcoin-cli -named signrawtransaction hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
7218b78ad4853eb957b610033b8e1ef48b01d948e0ec5dbf79f12caebc2b17e2
```
Maintenant, lorsque vous regardez votre transaction, vous devriez voir quelque chose de nouveau : la ligne `bip125-replaceable`, qui a toujours été marquée `no` auparavant, est maintenant marquée `yes` :

```
$ bitcoin-cli -named gettransaction txid=7218b78ad4853eb957b610033b8e1ef48b01d948e0ec5dbf79f12caebc2b17e2
{
  "amount": -0.10000000,
  "fee": 0.00000000,
  "confirmations": 0,
  "trusted": true,
  "txid": "7218b78ad4853eb957b610033b8e1ef48b01d948e0ec5dbf79f12caebc2b17e2",
  "walletconflicts": [
  ],
  "time": 1491603320,
  "timereceived": 1491603320,
  "bip125-replaceable": "yes",
  "details": [
    {
      "account": "",
      "address": "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi",
      "category": "send",
      "amount": -0.10000000,
      "vout": 0,
      "fee": 0.00000000,
      "abandoned": false
    }
  ],
  "hex": "02000000014e843e22cb8ee522fbf4d8a0967a733685d2ad92697e63f52ce41bec8f7c8ac0010000006b483045022100834731cd64efcc078d6c3e59cf0963599ffbc44722b7851b0404bb68e4a1fec70220759a0887ea791592c8119bbe61842eb3850a20cdf8433b4ba00d4ead752facfe012103456575f59a127a4c3e79c23f185899fa0a9ccd40162d05617fb112fa31bd14e5010000000280969800000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac804a5d05000000001976a914c101d8c34de7b8d83b3f8d75416ffaea871d664988ac00000000"
}
```
Le drapeau `bip125-replaceable` restera à `yes` jusqu'à ce que la transaction reçoive des confirmations. À ce moment-là, il n'est plus remplaçable.

_Dois-je faire confiance aux transactions sans confirmation ?_ Non, jamais. C'était vrai avant le RBF et ça reste vrai après le RBF. Les transactions doivent recevoir des confirmations avant d'être fiables. C'est particulièrement vrai si une transaction est marquée comme étant `bip125-replaceable`, car elle peut alors être ... remplacée.

:information_source: **NOTE — SEQUENCE:** C'est la première utilisation de la valeur `nSequence` dans Bitcoin. Vous pouvez la régler entre 1 et 0xffffffff-2 (4294967293) et activer le RBF, mais si vous ne faites pas attention, vous pouvez vous heurter à l'utilisation parallèle de `nSequence` pour les timelocks relatifs. Nous vous suggérons de toujours le régler sur "1", ce que fait Bitcoin Core, mais l'autre option est de le régler sur une valeur comprise entre 0xf0000000 (4026531840) et 0xffffffffff-2 (4294967293). Le réglage sur "1" rend en effet les timelocks relatifs irréversibles et le réglage sur 0xf0000000 ou plus les désactive. Tout ceci est expliqué plus en détail au [chapitre 9.3: Utilisation de CSV dans des scripts](09_3_Utilisation_de_CSV_dans_des_scripts.md). Pour l'instant, il suffit de choisir une des valeurs non conflictuelles pour `nSequence`.  

### Optionnel : Toujours opter pour le RBF

Si vous préférez, vous pouvez _toujours_ opter pour le RBF. Pour ce faire, exécutez votre `bitcoind` avec la commande `-walletrbf`. Une fois que vous avez fait cela (et redémarré `bitcoind`), toutes les UTXOs doivent avoir un numéro de séquence inférieur et la transaction doit être marquée comme `bip125-remplaceble`.

Notez l'exemple suivant généré par `bitcoind`, où l'UTXO utilise en effet un numéro de séquence de "1" :

```
{
  "txid": "d261b9494eb29084f668e1abd75d331fc2d6525dd206b2f5236753b5448ca12c",
  "hash": "d261b9494eb29084f668e1abd75d331fc2d6525dd206b2f5236753b5448ca12c",
  "size": 226,
  "vsize": 226,
  "version": 2,
  "locktime": 0,
  "vin": [
    {
      "txid": "4075dbf84303c01adcb0b36cd2c164e2b447192c2d9fbf5fde3b99d0ac7e64b6",
      "vout": 1,
      "scriptSig": {
        "asm": "3045022100b3a0d66abe3429f81a6dc397369d6ac9cb025a2243b68649d95665967fe4365b022038cf037aaab9368268e97203494d1b542e83101e6aaaf97957daf70dee6ee0af[ALL] 022615f4b6417b991530df4bc8c8ee10b8925c741773fead7a5edd89337caeba53",
        "hex": "483045022100b3a0d66abe3429f81a6dc397369d6ac9cb025a2243b68649d95665967fe4365b022038cf037aaab9368268e97203494d1b542e83101e6aaaf97957daf70dee6ee0af0121022615f4b6417b991530df4bc8c8ee10b8925c741773fead7a5edd89337caeba53"
      },
      "sequence": 1
    }
  ],
  "vout": [
    {
      "value": 0.10000000,
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
    }, 
    {
      "value": 0.90000000,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 20219e4f3c6bc0f6524d538009e980091b3613e8 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a91420219e4f3c6bc0f6524d538009e980091b3613e888ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "miSrC3FvkPPZgqqvCiQycq7io7wTSVsAFH"
        ]
      }
    }
  ]
}
```
> :warning: **AVERTISSEMENT VERSION :** Le drapeau walletrbf nécessite Bitcoin Core v.0.14.0.

## Comprendre le fonctionnement du RBF

La fonctionnalité RBF est basée sur le [BIP 125](https://github.com/bitcoin/bips/blob/master/bip-0125.mediawiki), qui utilise les règles suivantes pour l'utilisation de RBF :

> 1. Les transactions initiales signalent la possibilité de remplacement explicitement ou par héritage comme décrit dans la section Résumé ci-dessus.

Cela signifie que le numéro de séquence doit être inférieur à 0xffffffffff-1. (4294967294), ou cela est vrai pour les transactions parentes non confirmées.

> 2. La transaction de remplacement donne lieu à des frais strictement plus élevés que la somme payée par les transactions initiales.
> 3. La transaction de remplacement ne contient pas de nouvelles entrées non confirmées qui n'apparaissaient pas auparavant dans le mempool (les entrées non confirmées sont des entrées dépensant des sorties de transactions pas encore confirmées).
> 4. La transaction de remplacement doit payer pour sa propre bande passante en plus du montant payé par les transactions originales à un taux égal ou supérieur à celui fixé par le tarif minimum des frais de relais du nœud. Par exemple, si la redevance minimale de relais est de 1 satoshi/octet et que la transaction de remplacement porte sur un total de 500 octets, le remplaçant doit payer une redevance supérieure d'au moins 500 satoshis à la somme des originaux.
> 5. Le nombre de transactions initiales à remplacer et leurs descendantes qui seront évincées du mempool ne doit pas dépasser un total de 100 transactions.

_Qu'est-ce qu'une BIP?_  Une BIP est une proposition d'amélioration de Bitcoin (ndlr : _Bitcoin Improvement Proposal_) . Il s'agit d'une suggestion approfondie de modification du code de base de Bitcoin. Souvent, lorsqu'une BIP a été suffisamment discuté et mis à jour, elle devient une partie intégrante du code de base de Bitcoin. Par exemple, la BIP 125 a été implémenté dans le Bitcoin Core 0.12.0.

L'autre chose à comprendre au sujet du RBF est que pour l'utiliser, il faut faire une double-dépense, en réutilisant une ou plusieurs fois la même UTXO. Le simple fait d'envoyer une autre transaction avec un UTXO différent au même destinataire ne fera pas l'affaire (et vous fera probablement perdre de l'argent). Vous devez plutôt créer délibérément un conflit, où la même UTXO est utilisée dans deux transactions différentes. 

Face à ce conflit, les mineurs sauront utiliser celui dont la redevance est la plus élevée, et ils seront incités à le faire par cette redevance plus élevée.

_Qu'est-ce qu'une double-dépense ?_  Il y a double-dépense lorsqu'une personne envoie les mêmes fonds électroniques à deux personnes différentes (ou, à la même personne deux fois, dans deux transactions différentes). Il s'agit d'un problème central pour tout système de monnaie électronique. Il est résolu dans Bitcoin par le grand livre immuable : une fois qu'une transaction est suffisamment confirmée, aucun mineur ne vérifiera les transactions qui réutilisent la même UTXO. Cependant, il est possible de doubler les dépenses avant qu'une transaction n'ait été confirmée - c'est pourquoi vous voudrez toujours une ou plusieurs confirmations avant de finaliser une transaction. Dans le cas du RBF, vous doublez délibérément les dépenses parce qu'une transaction initiale a été bloquée, et les mineurs acceptent votre double dépense si vous répondez aux critères spécifiques établis par la BIP 125.

> :warning: **AVERTISSEMENT:** Certaines des premières discussions sur cette politique ont suggéré que le nombre `nSequence` soit également augmenté. C'était en fait l'utilisation prévue de `nSequence` dans sa forme originale. Ceci _ne fait pas_ partie de la politique publiée dans le BIP 125. En fait, l'augmentation de votre numéro de séquence peut accidentellement bloquer votre transaction avec une temporisation relative, à moins que vous n'utilisiez des numéros de séquence compris entre 0xf0000000 (4026531840) et 0xffffffff-2 (4294967293).  

## Remplacer une transaction à la dure : à la main

Pour créer une transaction RBF à la main, il suffit de créer une transaction brute qui : (1) remplace une transaction brute précédente qui a opté pour le RBF et qui n'est pas confirmée ; (2) réutilise une ou plusieurs UTXO identique ; (3) augmente les frais ; et (4) paie la largeur de bande minimale des deux transactions [qui peut déjà être prise en charge par (3)].

L'exemple suivant réutilise simplement nos variables existantes, mais diminue le montant envoyé à l'adresse de change, pour augmenter les frais accidentels de 0 BTC  de la transaction originale à un trop généreux 0,01 BTC  dans la nouvelle transaction :

```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "sequence": 1 } ]''' outputs='''{ "'$recipient'": 0.1, "'$changeaddress'": 0.89 }''')
```
Nous devons bien sûr le re-signer et le renvoyer :
```
$ signedtx=$(bitcoin-cli -named signrawtransaction hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
959b0b0f4c8350e9038279dfe0f5ae7b165660cc1281e37bea08d0bd084edb39
```
Après la création de plusieurs blocs, la transaction initiale continue de traîner :
```
$ bitcoin-cli -named gettransaction txid=7218b78ad4853eb957b610033b8e1ef48b01d948e0ec5dbf79f12caebc2b17e2
{
  "amount": -0.10000000,
  "fee": 0.00000000,
  "confirmations": -5,
  "trusted": false,
  "txid": "7218b78ad4853eb957b610033b8e1ef48b01d948e0ec5dbf79f12caebc2b17e2",
  "walletconflicts": [
    "959b0b0f4c8350e9038279dfe0f5ae7b165660cc1281e37bea08d0bd084edb39"
  ],
  "time": 1491603320,
  "timereceived": 1491603320,
  "bip125-replaceable": "yes",
  "details": [
    {
      "account": "",
      "address": "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi",
      "category": "send",
      "amount": -0.10000000,
      "vout": 0,
      "fee": 0.00000000,
      "abandoned": false
    }
  ],
  "hex": "02000000014e843e22cb8ee522fbf4d8a0967a733685d2ad92697e63f52ce41bec8f7c8ac0010000006b483045022100834731cd64efcc078d6c3e59cf0963599ffbc44722b7851b0404bb68e4a1fec70220759a0887ea791592c8119bbe61842eb3850a20cdf8433b4ba00d4ead752facfe012103456575f59a127a4c3e79c23f185899fa0a9ccd40162d05617fb112fa31bd14e5010000000280969800000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac804a5d05000000001976a914c101d8c34de7b8d83b3f8d75416ffaea871d664988ac00000000"
}
```
Notez que `bitcoin-cli` reconnaît qu'il y a un conflit avec une autre transaction dans la section `walletconflicts`. Notez également que cette transaction est maintenant répertoriée avec des _confirmations négatives_, ce qui indique depuis combien de temps la double-dépense incluse n'a pas été confirmée.

Entre-temps, la nouvelle transaction a bien fonctionné :

```
$ bitcoin-cli -named gettransaction txid=959b0b0f4c8350e9038279dfe0f5ae7b165660cc1281e37bea08d0bd084edb39
{
  "amount": -0.10000000,
  "fee": -0.01000000,
  "confirmations": 5,
  "blockhash": "00000000000006eeb468791e5ee0d86613c03acd871ef7d89c25fd28474754d5",
  "blockindex": 20,
  "blocktime": 1491603862,
  "txid": "959b0b0f4c8350e9038279dfe0f5ae7b165660cc1281e37bea08d0bd084edb39",
  "walletconflicts": [
    "7218b78ad4853eb957b610033b8e1ef48b01d948e0ec5dbf79f12caebc2b17e2"
  ],
  "time": 1491603673,
  "timereceived": 1491603673,
  "bip125-replaceable": "no",
  "details": [
    {
      "account": "",
      "address": "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi",
      "category": "send",
      "amount": -0.10000000,
      "vout": 0,
      "fee": -0.01000000,
      "abandoned": false
    }
  ],
  "hex": "02000000014e843e22cb8ee522fbf4d8a0967a733685d2ad92697e63f52ce41bec8f7c8ac0010000006a47304402207fea4a11db8576257b9d9e104aa07cb3d3ae6a42e38dd7126111276ca5b45daa0220594a3553cc278c43fd015b35029d5b9596d4ac9f36d3d20fb1a8c9efface5c50012103456575f59a127a4c3e79c23f185899fa0a9ccd40162d05617fb112fa31bd14e5020000000280969800000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac40084e05000000001976a914c101d8c34de7b8d83b3f8d75416ffaea871d664988ac00000000"
}
```
Nos bénéficiaires ont leur argent, et la transaction originale, qui a échoué, finira par tomber hors du mempool.

## Remplacer une transaction de manière plus simple : bumpfee

Les transactions brutes sont très puissantes, et vous pouvez faire beaucoup de choses intéressantes en les combinant avec le RBF. Cependant, parfois, _tout_ ce que vous voulez faire, c'est libérer une transaction qui a été suspendue. Vous pouvez maintenant faire cela avec une simple commande, `bumpfee`.

Par exemple, pour augmenter les frais de la transaction `4460175e8276d5a1935f6136e36868a0a3561532d44ddffb09b7cb878f76f927` vous lanceriez la commande :

```
$ bitcoin-cli -named bumpfee txid=4460175e8276d5a1935f6136e36868a0a3561532d44ddffb09b7cb878f76f927
{
  "txid": "75208c5c8cbd83081a0085cd050fc7a4064d87c7d73176ad9a7e3aee5e70095f",
  "origfee": 0.00000000,
  "fee": 0.00022600,
  "errors": [
  ]
}
```
Le résultat est la génération automatique d'une nouvelle transaction dont les frais sont déterminés par votre fichier bitcoin.conf :
```
$ bitcoin-cli -named gettransaction txid=75208c5c8cbd83081a0085cd050fc7a4064d87c7d73176ad9a7e3aee5e70095f
{
  "amount": -0.10000000,
  "fee": -0.00022600,
  "confirmations": 0,
  "trusted": false,
  "txid": "75208c5c8cbd83081a0085cd050fc7a4064d87c7d73176ad9a7e3aee5e70095f",
  "walletconflicts": [
    "4460175e8276d5a1935f6136e36868a0a3561532d44ddffb09b7cb878f76f927"
  ],
  "time": 1491605676,
  "timereceived": 1491605676,
  "bip125-replaceable": "yes",
  "replaces_txid": "4460175e8276d5a1935f6136e36868a0a3561532d44ddffb09b7cb878f76f927",
  "details": [
    {
      "account": "",
      "address": "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi",
      "category": "send",
      "amount": -0.10000000,
      "vout": 0,
      "fee": -0.00022600,
      "abandoned": false
    }
  ],
  "hex": "02000000014e843e22cb8ee522fbf4d8a0967a733685d2ad92697e63f52ce41bec8f7c8ac0020000006b48304502210094e54afafce093008172768d205d99ee2e9681b498326c077f0b6a845d9bbef702206d90256d5a2edee3cab1017b9b1c30b302530b0dd568e4af6f2d35380bbfaa280121029f39b2a19943fadbceb6697dbc859d4a53fcd3f9a8d2c8d523df2037e7c32a71010000000280969800000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac38f25c05000000001976a914c101d8c34de7b8d83b3f8d75416ffaea871d664988ac00000000"
}
```

> :warning: **AVERSTISSEMENT VERSION:** `bumpfee` requiert Bitcoin Core v.0.14.0.

## Résumé : Renvoyer une transaction avec le RBF

Si une transaction est bloquée, et que vous ne voulez pas attendre qu'elle expire entièrement, si vous avez opté pour le RBF, vous pouvez alors utiliser le RBF pour créer une transaction de remplacement (ou simplement utiliser `bumpfee`).

_Quelle est la puissance du RBF ?_  Il est évident que le RBF est très utile si vous avez créé une transaction avec des frais trop faibles et que vous avez besoin de faire passer ces fonds. Cependant, la possibilité de remplacer les transactions non confirmées par des transactions mises à jour a plus de pouvoir que cela (et c'est pourquoi vous pourriez vouloir continuer à utiliser le RBF pour les transactions brutes, même après l'avènement de `bumpfee`).

Par exemple, vous pouvez envoyer une transaction, puis, avant qu'elle ne soit confirmée, la combiner avec une deuxième transaction. Cela vous permet de comprimer plusieurs transactions en une seule, ce qui réduit les frais globaux. Cela peut également présenter des avantages en termes de respect de la vie privée. Il existe d'autres raisons d'utiliser le système RBF, pour les contrats intelligents ou les transactions fractionnées, comme le décrit la [Opt-in RBF FAQ](https://bitcoincore.org/en/faq/optin_rbf/).

## Quelle est la prochaine étape ?

Continuer avec [chapitre 5.3 : Financer une transaction avec CPFP](05_3_Financer_une_transaction_avec_CPFP.md)  
