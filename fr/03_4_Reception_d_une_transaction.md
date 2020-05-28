# 3.4: Réception d'une transaction

> :information_source: **NOTE:** Ce texte est encore un brouillon, afin que je puisse obtenir un retour d'information de la part des premiers examinateurs. Il n'est pas encore prêt pour l'apprentissage.

Vous êtes maintenant prêt à recevoir de l'argent à la nouvelle adresse que vous avez établie.

## Obtenir de l'argent

Pour aller plus loin, vous devez obtenir de l'argent. Sur testnet, cela se fait par des robinets (_NDT : faucets_). Comme l'argent est factice, il vous suffit d'aller à un robinet, de demander de l'argent, et il vous sera envoyé. Nous vous suggérons d'utiliser le robinet à l'adresse https://testnet.coinfaucet.eu/en/. S'il n'est pas disponible pour une raison quelconque, cherchez "bitcoin testnet faucet", et vous devriez en trouver d'autres.

Pour utiliser un robinet, vous devez généralement vous rendre à une URL et faire un copier-coller de votre adresse. Notez que c'est l'un des cas où vous ne pourrez pas utiliser les variables de la ligne de commande, hélas. Ensuite, une transaction sera créée qui vous enverra de l'argent en provenance du robinet.

_Qu'est-ce qu'une transaction?_ Une transaction est un échange de bitcoin. Le propriétaire de quelques bitcoins utilise sa clé privée pour accéder à ces pièces, puis verrouille la transaction à l'aide de la clé publique du destinataire.

> :link: **TESTNET vs MAINNET:** Malheureusement, il n'y a pas de robinets dans la vie réelle. Si vous jouez sur le réseau principal, vous devez aller acheter des bitcoins dans un bureau de change ou à un distributeur automatique, ou vous devez demander à quelqu'un de vous les envoyer. La vie sur Testnet est beaucoup plus facile.

## Vérifiez votre argent

Une fois que vous avez demandé votre argent, vous devriez pouvoir le vérifier avec la commande `bitcoin-cli getbalance` :
```
$ bitcoin-cli getbalance
0.00000000
```
Mais attendez, le solde est encore nul !?

Bienvenue dans le monde de la latence Bitcoin. Le problème est que votre transaction n'a pas encore été enregistrée dans un bloc !

_Qu'est-ce qu'un bloc ?_ Les transactions sont transmises sur le réseau et assemblées en blocs par les mineurs. Ces blocs sont sécurisés par une preuve de travail (_NDT : proof-of-work_), qui prouve que la puissance de calcul a été dépensée dans le cadre de la création du bloc. C'est cette preuve de travail (multipliée par de nombreux blocs, chacun étant construit à la suite du dernier) qui permet de sécuriser Bitcoin.

_Qu'est-ce qu'un mineur ?_ Un mineur est un participant du réseau Bitcoin qui travaille à la création de blocs. Il s'agit d'un travail rémunéré : lorsqu'un mineur réussit à créer un bloc, il reçoit une récompense unique, plus les frais des transactions relatifs à son bloc. L'exploitation minière est un gros business. Les mineurs ont tendance à utiliser du matériel spécial, accéléré, de telle sorte qu'ils augmentent leur propre probabilité de pouvoir créer des blocs. Ils ont également tendance à faire partie de regroupements de minage (_NDT : mining pools_), dans lesquels les mineurs acceptent tous de partager les récompenses lorsque l'un d'entre eux crée un bloc avec succès.

Heureusement, `bitcoin-cli getunconfirmedbalance` devrait toujours afficher votre solde actualisé tant que la transaction initiale a été créée :

```
$ bitcoin-cli getunconfirmedbalance
0.47000000
```
Si le montant affiché est toujours zéro, c'est probablement que vous avancez trop vite dans ce tutoriel. Attendez une seconde. Les pièces devraient apparaître non confirmées, puis passer rapidement à confirmées. Notez qu'une pièce peut passer d'un solde non confirmé à un solde confirmé presque immédiatement, alors assurez-vous de vérifier les deux. Cependant, si votre `getbalance` et votre `getunconfirmedbalance` affichent toujours zéro au bout de dix minutes, c'est qu'il y a probablement un problème avec le robinet et que vous devez en choisir un autre.

### Gagnez de la confiance envers votre argent

Vous pouvez utiliser `bitcoin-cli getbalance "*" [n]` pour voir si un solde confirmé a une profondeur de `n` blocs.

_Qu'est-ce que la profondeur de bloc ?_ Après qu'un bloc ait été construit et confirmé, un autre bloc est construit par-dessus, et ainsi de suite ... Comme il s'agit d'un processus stochastique, il y a une possibilité de chamboulement lorsqu'un bloc est encore nouveau. Ainsi, un bloc doit être enfoui sous plusieurs blocs de profondeur dans une chaîne avant que vous puissiez avoir une confiance totale dans vos fonds. Chacun de ces blocs a tendance à être construit en 10 minutes en moyenne... il faut donc généralement une heure environ pour qu'une transaction confirmée soit totalement sûre.

Ce qui suit montre que notre transaction a été confirmée une fois, mais pas deux :
```
$ bitcoin-cli getbalance "*" 1
0.47000000
$ bitcoin-cli getbalance "*" 2
0.00000000
```
Evidemment, toutes les dix minutes environ, cette profondeur augmentera.

Bien sûr, sur le testnet, personne ne s'inquiète à ce point de la fiabilité de ses fonds. Vous pourrez dépenser votre argent dès qu'il sera confirmé.

## Verify Your Wallet

La commande `bitcoin-cli getwalletinfo` vous donne plus d'informations sur le solde de votre portefeuille :
```
$ bitcoin-cli getwalletinfo
{
  "walletversion": 130000,
  "balance": 0.47000000,
  "unconfirmed_balance": 0.00000000,
  "immature_balance": 0.00000000,
  "txcount": 1,
  "keypoololdest": 1488216266,
  "keypoolsize": 100,
  "paytxfee": 0.00000000,
  "hdmasterkeyid": "b91d5ec57d5ae3e90fff50d12e819429b6496b94"
}
```

## Découvrez votre identifiant de transaction

Votre argent est arrivé dans votre portefeuille par le biais d'une transaction. Vous pouvez découvrir cet ID de transaction (txid) avec la commande `bitcoin-cli listtransactions` :
```
$ bitcoin-cli listtransactions
[
  {
    "account": "",
    "address": "n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf",
    "category": "receive",
    "amount": 0.47000000,
    "label": "",
    "vout": 0,
    "confirmations": 2,
    "blockhash": "00000000fa4fdd22a2c33c6200b68239939ad65af3f1a48ecea25f8200f5d66b",
    "blockindex": 45,
    "blocktime": 1488307692,
    "txid": "88e5d5f3077517d76f5a61491fa52e6aaae078c52bc62d849f09507ef0cfada2",
    "walletconflicts": [
    ],
    "time": 1488307692,
    "timereceived": 1488307696,
    "bip125-replaceable": "no"
  }
]
```
Ceci montre une transaction ("88e5d5f3077517d76f5a61491fa52e6aaae078c52bc62d849f09507ef0cfada2") qui a été reçue ("receive") à une adresse spécifique dans mon portefeuille ("n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf") pour un montant spécifique ("0,47000000").

Vous pouvez accéder à des informations similaires avec la commande `bitcoin-cli listunspent`, mais elle n'affiche que les transactions concernant l'argent que vous n'avez pas dépensé. Ces transactions sont appelées UTXO, et seront d'une importance vitale lorsque vous enverrez de l'argent dans le monde des bitcoins :
```
$ bitcoin-cli listunspent
[
  {
    "txid": "88e5d5f3077517d76f5a61491fa52e6aaae078c52bc62d849f09507ef0cfada2",
    "vout": 0,
    "address": "n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf",
    "account": "",
    "scriptPubKey": "76a914fd67e8a7c7813e7a5c376eb71074f373d924d96888ac",
    "amount": 0.47000000,
    "confirmations": 3,
    "spendable": true,
    "solvable": true
  }
]
```
Notez que les bitcoins ne sont pas seulement un amas homogène d'argent liquide dans votre poche. Chaque transaction individuelle que vous recevez ou que vous envoyez est placée en bloc dans le grand livre immuable de la chaîne des blocs. Vous pouvez voir ces transactions individuelles lorsque vous regardez votre argent non dépensé. Cela signifie que les dépenses en bitcoin ne sont pas aussi anonymes que vous le pensez. Bien que les adresses soient assez privées, les transactions peuvent être examinées au fur et à mesure qu'elles entrent et sortent des adresses. Cela rend la vie privée vulnérable à l'analyse statistique. Cela introduit également une certaine non-fongibilité potentielle des bitcoins, car vous pouvez suivre une série de transactions, même si vous ne pouvez pas suivre un "bitcoin" spécifique.

_Pourquoi toutes ces quantités de bitcoin sont-elles exprimées en fractions ?_ Les bitcoins sont produits lentement et sont donc relativement peu nombreux en circulation. Par conséquent, chaque bitcoin sur le réseau principal vaut un peu plus (~ 1 000 $ au moment de la rédaction du présent document). Cela signifie que les gens travaillent généralement par fractions. En fait, 0,47 BTC serait beaucoup dans le monde réel. Vous aurez souvent affaire à des fractions encore plus petites sur le réseau principal. C'est pourquoi des noms sont apparus pour de plus petites quantités de bitcoins, notamment les millibitcoins ou mBTC (un millième de bitcoin), les microbitcoins ou bits ou μBTCs (un millionième de bitcoin) et les satoshis (un cent millionième de bitcoin).

## Examinez votre transaction

Vous pouvez obtenir plus d'informations sur une transaction avec la commande `bitcoin-cli getrawtransaction` :
```
$ bitcoin-cli getrawtransaction "88e5d5f3077517d76f5a61491fa52e6aaae078c52bc62d849f09507ef0cfada2"
010000000133261a25b44689bab2c6a207381ca21d243de9bbf21f0fa40c3a26ba7282a87b000000006b483045022100a2640761810dfc34dabed599928243afe24e13f520f780ceb382843a530a577c022050b92f5d9843d70ddb60a0aa294938862f2b7372818d6149ffd4f6adec5cf6c80121034dcaa515c2fda0f4a50b90a6d798e01c00a870bef0bd97154066fe202d2b5d75feffffff02c029cd02000000001976a914fd67e8a7c7813e7a5c376eb71074f373d924d96888ac17791703000000001976a914e176ee39c642344df2180863e27e2e936307273c88ac07a41000
```
Certes, ce n'est pas très utile, car il s'agit des données de transaction codées en hexadécimal. Heureusement, vous pouvez obtenir une description plus détaillée en ajoutant simplement un "1" à la commande :

```
$ bitcoin-cli getrawtransaction "88e5d5f3077517d76f5a61491fa52e6aaae078c52bc62d849f09507ef0cfada2" 1
{
  "hex": "010000000133261a25b44689bab2c6a207381ca21d243de9bbf21f0fa40c3a26ba7282a87b000000006b483045022100a2640761810dfc34dabed599928243afe24e13f520f780ceb382843a530a577c022050b92f5d9843d70ddb60a0aa294938862f2b7372818d6149ffd4f6adec5cf6c80121034dcaa515c2fda0f4a50b90a6d798e01c00a870bef0bd97154066fe202d2b5d75feffffff02c029cd02000000001976a914fd67e8a7c7813e7a5c376eb71074f373d924d96888ac17791703000000001976a914e176ee39c642344df2180863e27e2e936307273c88ac07a41000",
  "txid": "88e5d5f3077517d76f5a61491fa52e6aaae078c52bc62d849f09507ef0cfada2",
  "hash": "88e5d5f3077517d76f5a61491fa52e6aaae078c52bc62d849f09507ef0cfada2",
  "size": 226,
  "vsize": 226,
  "version": 1,
  "locktime": 1090567,
  "vin": [
    {
      "txid": "7ba88272ba263a0ca40f1ff2bbe93d241da21c3807a2c6b2ba8946b4251a2633",
      "vout": 0,
      "scriptSig": {
        "asm": "3045022100a2640761810dfc34dabed599928243afe24e13f520f780ceb382843a530a577c022050b92f5d9843d70ddb60a0aa294938862f2b7372818d6149ffd4f6adec5cf6c8[ALL] 034dcaa515c2fda0f4a50b90a6d798e01c00a870bef0bd97154066fe202d2b5d75",
        "hex": "483045022100a2640761810dfc34dabed599928243afe24e13f520f780ceb382843a530a577c022050b92f5d9843d70ddb60a0aa294938862f2b7372818d6149ffd4f6adec5cf6c80121034dcaa515c2fda0f4a50b90a6d798e01c00a870bef0bd97154066fe202d2b5d75"
      },
      "sequence": 4294967294
    }
  ],
  "vout": [
    {
      "value": 0.47000000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 fd67e8a7c7813e7a5c376eb71074f373d924d968 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914fd67e8a7c7813e7a5c376eb71074f373d924d96888ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf"
        ]
      }
    },
    {
      "value": 0.51869975,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 e176ee39c642344df2180863e27e2e936307273c OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914e176ee39c642344df2180863e27e2e936307273c88ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "n256of3JH1A6X8AQUU7LYkcaRcmrfGjGKC"
        ]
      }
    }
  ],
  "blockhash": "00000000fa4fdd22a2c33c6200b68239939ad65af3f1a48ecea25f8200f5d66b",
  "confirmations": 3,
  "time": 1488307692,
  "blocktime": 1488307692
}
```
Maintenant vous pouvez voir les informations complètes sur la transaction, y compris toutes les entrées ("vin") et toutes les sorties ("vout"). L'une des choses intéressantes à noter est que, bien que nous ayons reçu 0,47 BTC dans la transaction, un autre 0,51869975 a été envoyé à une autre adresse. Il s'agissait probablement d'un changement d'adresse, un concept qui est examiné dans la section suivante. Il est tout à fait typique qu'une transaction ait plusieurs entrées et/ou plusieurs sorties.

> :warning: **AVERTISSEMENT :** Ces commandes ne fonctionneront pas dans certaines situations. Pour pouvoir visualiser une transaction brute sur un nœud standard, une partie de l'argent doit être non dépensé, ou la transaction doit encore se trouver dans votre mempool - ce qui signifie que cette commande fonctionnera bien pour l'argent que vous venez de recevoir, mais pas pour les vieux trucs. Si vous voulez pouvoir visualiser les anciennes transactions qui ont été dépensées, vous pouvez le faire en conservant un ensemble de toutes les transactions avec la configuration txindex=1, ce que nos scripts suggèrent pour toutes les instances non élaguées. (Vous ne pouvez pas maintenir un index des transactions si votre noeud est élagué).

## Facultatif : Utiliser un explorateur de bloc

Le fait de regarder les informations verbeuses d'une transaction peut être un peu intimidant. L'objectif principal de ce tutoriel est d'enseigner comment traiter les transactions brutes à partir de la ligne de commande, mais nous sommes heureux de parler d'autres outils lorsqu'ils sont applicables. L'un de ces outils est un explorateur de blocs, que vous pouvez utiliser pour examiner les transactions à partir d'un navigateur web dans un format beaucoup plus convivial.

Actuellement, notre explorateur de bloc préféré est [https://live.blockcypher.com/](https://live.blockcypher.com/).

Vous pouvez l'utiliser pour rechercher une adresse dans l'ensemble des transactions :

[https://live.blockcypher.com/btc-testnet/address/n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf/](https://live.blockcypher.com/btc-testnet/address/n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf/)

Vous pouvez également l'utiliser pour examiner des transactions individuelles :

[https://live.blockcypher.com/btc-testnet/tx/88e5d5f3077517d76f5a61491fa52e6aaae078c52bc62d849f09507ef0cfada2/](https://live.blockcypher.com/btc-testnet/tx/88e5d5f3077517d76f5a61491fa52e6aaae078c52bc62d849f09507ef0cfada2/)

Un explorateur de bloc ne fournit généralement pas plus d'informations qu'une ligne de commande qui examine une transaction brute ; il fait simplement un bon travail de mise en évidence des informations importantes et d'assemblage des pièces du puzzle, y compris les frais de transaction à l'origine d'une transaction - un autre concept que nous aborderons dans les prochaines sections.

## Résumé : Réception d'une transaction

Les robinets vous donneront de l'argent sur le testnet. Ils arrivent sous forme de transactions brutes, qui peuvent être examinées avec `getrawtransaction` ou un explorateur de blocs. Une fois que vous avez reçu une transaction, vous pouvez la voir sur votre solde et dans votre portefeuille.

## Quelle est la prochaine étape ?

Avancez dans `bitcoin-cli` avec le [Chapitre quatre : Envoi de transactions Bitcoin](04_0_Envoi_de_transactions_Bitcoin.md).