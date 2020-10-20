 # 6.3: Envoyer et dépenser une multisig automatisée

> :information_source: **NOTE:** Ce texte est encore un brouillon, afin que je puisse obtenir un retour d'information de la part des premiers examinateurs. Il n'est pas encore prêt pour l'apprentissage.

La technique standard pour créer des adresses à plusieurs signatures et pour dépenser leurs fonds est complexe, mais c'est un exercice intéressant pour comprendre un peu plus comment elles fonctionnent, et comment vous pouvez les manipuler à un niveau relativement bas. Cependant, Bitcoin Core a rendu le multisig un peu plus facile dans les nouvelles versions. 

> :warning: **AVERTISSEMENT SUR LA VERSION :** La commande `addmultisigaddress` est disponible dans Bitcoin Core v0.10 ou supérieur.

## Créez une adresse multisig dans votre portefeuille

Afin de faciliter la dépense des fonds envoyés à des adresses multisig, vous devez avoir fait quelques préparatifs en utilisant la commande `addmultisigaddress`. Ce n'est probablement pas ce que vous voudriez faire si vous écriviez des programmes de portefeuille multisig, mais si vous essayez simplement de recevoir des fonds à la main, cela pourrait vous éviter de vous arracher les cheveux.

### Rassemblez les clés

Vous commencez par créer des adresses P2PKH et récupérer les clés publiques comme d'habitude, pour chaque utilisateur qui fera partie du multisig :

```
machine1$ address3=$(bitcoin-cli getnewaddress)
machine1$ echo $address3
mkMkhbUzcSPdEHUoRQkBKHe8otP1SzWWeb
machine1$ bitcoin-cli -named validateaddress address=$address3 | jq -r '. | .pubkey'
02e7356952f4bb1daf475c04b95a2f7e0d9a12cf5b5c48a25b2303783d91849ba4

machine2$ address4=$(bitcoin-cli getnewaddress)
$ echo $address4
mkyeUBPDoeyFrfLE4V5oAQfee99pT2W1E3
$ bitcoin-cli -named validateaddress address=$address4 | jq -r '. | .pubkey'
030186d2b55de166389aefe209f508ce1fbd79966d9ac417adef74b7c1b5e07776
```

### Créez l'adresse multisig partout

Ensuite, vous créez la multisig sur _chaque machine qui contribue aux signatures_ en utilisant une nouvelle commande, `addmultisigaddress`, au lieu de `createmultisig`. Cette nouvelle commande enregistre une partie des informations dans votre portefeuille, ce qui facilite grandement les dépenses ultérieures.

```
machine1$ bitcoin-cli -named addmultisigaddress nrequired=2 keys='''["'$address3'","030186d2b55de166389aefe209f508ce1fbd79966d9ac417adef74b7c1b5e07776"]'''
2Mzw7WBvh9RAQ4ssKqxyNyP7L9NAojLqSW8

machine2$ bitcoin-cli -named addmultisigaddress nrequired=2 keys='''["02e7356952f4bb1daf475c04b95a2f7e0d9a12cf5b5c48a25b2303783d91849ba4","'$address4'"]'''
2Mzw7WBvh9RAQ4ssKqxyNyP7L9NAojLqSW8
```
Comme indiqué dans la section précédente, peu importe que vous utilisiez des adresses ou des clés publiques : vous obtiendrez la même adresse multisig. Cependant, vous devez utiliser le même ordre. Ainsi, il est préférable que les membres du multisigne vérifient entre eux pour s'assurer qu'ils ont tous obtenu le même résultat. 

### Surveillez les fonds

Par la suite, les membres de la multisig devront encore exécuter `importaddress` pour surveiller les fonds reçus sur l'adresse multisig :

```
machine1$ bitcoin-cli -named importaddress address=2Mzw7WBvh9RAQ4ssKqxyNyP7L9NAojLqSW8

machine2$ bitcoin-cli -named importaddress address=2Mzw7WBvh9RAQ4ssKqxyNyP7L9NAojLqSW8
```

## Redéposez avec une transaction automatisée

Par la suite, vous pourrez recevoir des fonds à l'adresse multisig comme d'habitude. L'utilisation de `addmultisigaddress` est simplement une question bureaucratique de la part des bénéficiaires : un peu de comptabilité pour leur faciliter la vie.

Mais cela rend la vie beaucoup plus facile. Comme tout est dans le portefeuille, les signataires pourront dépenser les fonds envoyés à l'adresse à signature multiple exactement de la même manière que pour toute autre adresse ... à part la nécessité de signer sur plusieurs machines.

Vous commencez par collecter vos variables, mais vous n'avez plus à vous soucier de `scriptPubKey` ou de `redeemScript` :

```
machine1$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[1] | .txid') 
machine1$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[1] | .vout') 
machine1$ recipient=$(bitcoin-cli getrawchangeaddress)
```
Vous créez une transaction brute:
```
machine1$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 1.2995}''')
```
Ensuite, vous la signez:
```
machine1$ bitcoin-cli -named signrawtransaction hexstring=$rawtxhex
{
  "hex": "02000000014ecda61c45f488e35c613a7c4ae26335a8d7bfd0a942f026d0fb1050e744a67d000000009100473044022025decef887fe2e3eb1c4b3edaa155e5755102d1570716f1467bb0b518b777ddf022017e97f8853af8acab4853ccf502213b7ff4cc3bd9502941369905371545de28d0147522102e7356952f4bb1daf475c04b95a2f7e0d9a12cf5b5c48a25b2303783d91849ba421030186d2b55de166389aefe209f508ce1fbd79966d9ac417adef74b7c1b5e0777652aeffffffff0130e1be07000000001976a9148dfbf103e48df7d1993448aa387dc31a2ebd522d88ac00000000",
  "complete": false,
  "errors": [
    {
      "txid": "7da644e75010fbd026f042a9d0bfd7a83563e24a7c3a615ce388f4451ca6cd4e",
      "vout": 0,
      "scriptSig": "00473044022025decef887fe2e3eb1c4b3edaa155e5755102d1570716f1467bb0b518b777ddf022017e97f8853af8acab4853ccf502213b7ff4cc3bd9502941369905371545de28d0147522102e7356952f4bb1daf475c04b95a2f7e0d9a12cf5b5c48a25b2303783d91849ba421030186d2b55de166389aefe209f508ce1fbd79966d9ac417adef74b7c1b5e0777652ae",
      "sequence": 4294967295,
      "error": "Operation not valid with the current stack size"
    }
  ]
}
```
Notez que vous n'avez plus besoin de donner à `signrawtransaction` une aide supplémentaire, car toutes ces informations supplémentaires se trouvaient déjà dans votre portefeuille. Plus important encore, vous n'avez pas rendu vos clés privées vulnérables en les manipulant directement. Au lieu de cela, le processus était _exactement_ le même que celui d'une UTXO normale, sauf que la transaction n'était pas entièrement signée à la fin.

### Signez-la sur d'autres machines

La dernière étape consiste à exporter le `hex` partiellement signé vers d'autres machines et à le signer à nouveau :

```
$ signedtx=$(bitcoin-cli -named signrawtransaction hexstring=02000000014ecda61c45f488e35c613a7c4ae26335a8d7bfd0a942f026d0fb1050e744a67d000000009100473044022025decef887fe2e3eb1c4b3edaa155e5755102d1570716f1467bb0b518b777ddf022017e97f8853af8acab4853ccf502213b7ff4cc3bd9502941369905371545de28d0147522102e7356952f4bb1daf475c04b95a2f7e0d9a12cf5b5c48a25b2303783d91849ba421030186d2b55de166389aefe209f508ce1fbd79966d9ac417adef74b7c1b5e0777652aeffffffff0130e1be07000000001976a9148dfbf103e48df7d1993448aa387dc31a2ebd522d88ac00000000 | jq -r '.hex')
```
Lorsque toutes les personnes requises ont signé, vous avez terminé :
```
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
3ce88839ac6165aeadcfb188c490e1b850468eff571b4ca78fac64342751510d
```
Comme pour le raccourci démontré au [§4.5 : Envoyer des pièces à l'aide des transactions brutes automatisées](04_5_Envoyer_des_pieces_a_l_aide_des_transactions_brutes_automatisees.md), le résultat est beaucoup plus facile, mais vous perdez un peu de contrôle dans le processus.

## Résumé : Envoyer et dépenser une multisig automatisée

Il existe un moyen plus simple de reverser les fonds envoyés à des adresses multisig qui nécessite simplement l'utilisation de la commande `addmultisigaddress` lorsque vous créez votre adresse. Cela ne démontre pas les subtilités de la redistribution des fonds P2SH et ne vous donne pas un contrôle étendu, mais si vous voulez simplement récupérer votre argent, c'est la solution.

## Quelle est la prochaine étape ?

Continuez "Développer les transactions Bitcoin" avec le [§6.4 : Envoyer une transaction avec un délai de blocage](06_4_Envoyer_une_transaction_avec_un_delai_de_blocage.md).