# 6.5: Envoyer une transaction contenant des données

> :information_source: **NOTE:** Ce texte est encore un brouillon, afin que je puisse obtenir un retour d'information de la part des premiers examinateurs. Il n'est pas encore prêt pour l'apprentissage.

La troisième façon de varier la façon dont vous envoyez une transaction de base est d'utiliser la transaction pour envoyer des données au lieu de fonds (ou plus exactement, en plus des fonds). Cela vous donne la possibilité d'intégrer des informations dans la chaîne de blocs.

## Créez vos données

La première chose à faire est de créer les 80 octets (ou moins) de données que vous allez enregistrer dans votre OP_RETURN. Cela peut être aussi simple que de préparer un message ou de hacher des données existantes. Par exemple, le `sha256sum` produit 256 bits de données, soit 32 octets, ce qui est bien en dessous des limites :

```
$ sha256sum contract.jpg
fe7f0a3b69f56ef2d055a78823ed3bd1422e46c3183658ea854253033ae0ccef  contract.jpg
$ op_return_data="fe7f0a3b69f56ef2d055a78823ed3bd1422e46c3183658ea854253033ae0ccef"
```
_Qu'est-ce-que OP_RETURN ?_ Toutes les transactions Bitcoin sont basées sur des scripts opcode que nous verrons dans le prochain chapitre. L'OP_RETURN est un simple opcode qui définit une transaction comme non valide. La convention a fait en sorte qu'il est utilisé pour intégrer des données dans la chaîne de blocs.

## Préparez de l'argent

En créant une transaction de données, votre but n'est pas d'envoyer de l'argent à qui que ce soit, mais de mettre des données dans la chaîne de blocs. Cependant, vous devez envoyer de l'argent pour ce faire. Il vous suffit d'utiliser une adresse de change comme seul destinataire. Vous pouvez ensuite identifier un UTXO et l'envoyer à votre adresse de change, moins les frais de transaction, tout en utilisant la même transaction pour créer un OP_RETURN.

Voici la configuration standard :

```
$ bitcoin-cli listunspent
[
  {
    "txid": "4460175e8276d5a1935f6136e36868a0a3561532d44ddffb09b7cb878f76f927",
    "vout": 1,
    "address": "my7UvPbPWDbqCxYpAcUZELDbK1X7w9Whmc",
    "scriptPubKey": "76a914c101d8c34de7b8d83b3f8d75416ffaea871d664988ac",
    "amount": 0.90000000,
    "confirmations": 682,
    "spendable": true,
    "solvable": true
  }
]

$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[0] | .txid')
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
$ changeaddress=$(bitcoin-cli getrawchangeaddress)
```

## Rédigez une transaction brute

Vous pouvez maintenant écrire une nouvelle transaction brute avec deux sorties : l'une est votre adresse de change pour récupérer (la plupart) de votre argent, l'autre est une adresse de données, qui est le terme `bitcoin-cli` pour un OP_RETURN.

```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "data": "'$op_return_data'", "'$changeaddress'": 0.8995 }''')
```

Voici à quoi ressemble réellement cette transaction :
```
$ bitcoin-cli -named decoderawtransaction hexstring=$rawtxhex 
{
  "txid": "531660f9bcb2571b2b790ebe6e7b6ed40618f608f3f6722a511275257942790b",
  "hash": "531660f9bcb2571b2b790ebe6e7b6ed40618f608f3f6722a511275257942790b",
  "size": 128,
  "vsize": 128,
  "version": 2,
  "locktime": 0,
  "vin": [
    {
      "txid": "4460175e8276d5a1935f6136e36868a0a3561532d44ddffb09b7cb878f76f927",
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
      "value": 0.00000000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_RETURN fe7f0a3b69f56ef2d055a78823ed3bd1422e46c3183658ea854253033ae0ccef",
        "hex": "6a20fe7f0a3b69f56ef2d055a78823ed3bd1422e46c3183658ea854253033ae0ccef",
        "type": "nulldata"
      }
    }, 
    {
      "value": 0.89950000,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 166692bda9f25ced145267bb44286e8ee3963d26 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914166692bda9f25ced145267bb44286e8ee3963d2688ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mhZQ3Bih6wi7jP1tpFZrCcyr4NsfCapiZP"
        ]
      }
    }
  ]
}
```
Comme vous pouvez le voir, la majorité de l'argent est ainsi renvoyée directement à l'adresse de changement (mhZQ3Bih6wi7jP1tpFZrCcyr4NsfCapiZP) moins les frais de transaction standard que nous utilisons, à savoir 0,0005 BTC. Plus important encore, la première sortie montre un OP_RETURN avec les données (fe7f0a3b69f56ef2d055a78823ed3bd1422e46c3183658ea854253033ae0ccef) juste après.

## Envoyer une transaction brute

Signez votre transaction brute et envoyez-la, et bientôt cet OP_RETURN sera intégré dans la chaîne de blocs !

## Vérifiez votre OP_RETURN

Encore une fois, n'oubliez pas que vous pouvez consulter cette transaction à l'aide d'un explorateur de blockchain : 

[https://www.blocktrail.com/tBTC/tx/3a62b396afb8d8a59ebe7b9e52d6aa2485f1082a1d3fc6ece61fb8b55373823d](https://www.blocktrail.com/tBTC/tx/3a62b396afb8d8a59ebe7b9e52d6aa2485f1082a1d3fc6ece61fb8b55373823d)

Vous pouvez noter un avertissement sur le fait que les données se trouvent dans un "protocole inconnu". Si vous envisagez une utilisation régulière des données OP_RETURN, vous les marquerez probablement avec un préfixe spécial, pour marquer ce protocole. Ensuite, les données OP_RETURN réelles pourraient être quelque chose comme "CONTRACTS3b110a164aa18d3a5ab064ba93fdce62". Cet exemple n'a pas utilisé de préfixe pour éviter de brouiller l'espace des données.

[Coinsecrets](http://coinsecrets.org/) offre une autre façon intéressante d'examiner les données OP_RETURN. Il fait de son mieux pour se tenir au courant des protocoles, afin de pouvoir vous dire qui fait quoi dans la chaîne de blocs. Voici cette transaction : https://www.blocktrail.com/tBTC/tx/3a62b396afb8d8a59ebe7b9e52d6aa2485f1082a1d3fc6ece61fb8b55373823d

## Résumé : envoyer une transaction avec des données

Vous pouvez utiliser un opcode OP_RETURN pour stocker jusqu'à 80 octets de données sur la chaîne de blocs. Vous faites cela avec le mot de code de données pour un vout. Vous devez également envoyer de l'argent, mais vous le renvoyez simplement à une adresse différente, moins les frais de transaction.

_Quelle est la puissance de OP_RETURN ?_ L'OP_RETURN ouvre de toutes nouvelles possibilités pour la chaîne de blocage, car vous pouvez intégrer des données qui prouvent que certaines choses se sont produites à certains moments. Diverses organisations ont utilisé les OP_RETURN pour prouver leur existence, pour les droits d'auteur, pour les pièces de couleur et [à d'autres fins](https://en.bitcoin.it/wiki/OP_RETURN). Bien que 80 octets ne semblent pas beaucoup, cela peut être très efficace si les OP_RETURNs sont utilisés pour stocker des hachages des données réelles. Vous pouvez alors prouver l'existence de vos données numériques en démontrant que le hachage de ces données correspond à celui de la chaîne de blocs.

Notez qu'il y a une certaine controverse sur l'utilisation de la blockchain Bitcoin de cette manière.

## Quelle est la prochaine étape ?

Passez à "Scripts Bitcoin" avec le [chapitre 7 : Présentation des scripts Bitcoin](07_0_Presentation_des_scripts_bitcoin.md).