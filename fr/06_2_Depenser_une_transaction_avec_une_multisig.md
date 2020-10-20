# 6.2: Dépenser une transaction avec une multisig

> :information_source: **NOTE:** Ce texte est encore un brouillon, afin que je puisse obtenir un retour d'information de la part des premiers examinateurs. Il n'est pas encore prêt pour l'apprentissage.

La manière classique, et complexe, de dépenser des fonds envoyés à une adresse à signature multiple en utilisant `bitcoin-cli` nécessite un travail de fond important.

## Trouvez vos fonds

Pour commencer, vous devez trouver vos fonds ; votre ordinateur ne sait pas les rechercher, car ils ne sont associés à aucune adresse dans votre portefeuille. Vous pouvez alerter `bitcoind` pour le faire en utilisant la commande `Importaddress` :

```
$ bitcoin-cli -named importaddress address=2NAGfA4nW6nrZkD5je8tSiAcYB9xL2xYMCz
```
Cette commande peut prendre un certain temps à s'exécuter car elle effectue un `rescan` pour trouver toutes les transactions liées. Ensuite, les fonds devraient apparaître si vous utilisez `listunspent`... mais ils ne sont toujours pas faciles à dépenser.

```
$ bitcoin-cli listunspent
[
  {
    "txid": "621be11aac439d6ec58be398058fc33c3e89cf45138a0e73e05b7001f9b6e328",
    "vout": 0,
    "address": "2NAGfA4nW6nrZkD5je8tSiAcYB9xL2xYMCz",
    "account": "",
    "scriptPubKey": "a914babf9063cee8ab6e9334f95f6d4e9148d0e551c287",
    "amount": 1.29950000,
    "confirmations": 62,
    "spendable": false,
    "solvable": false
  }
]
```
(En fait, votre portefeuille prétend qu'ils ne sont pas du tout dépensables !)

## Définissez vos variables

Lorsque vous êtes prêt à dépenser les fonds reçus par une adresse à signature multiple, vous allez devoir collecter _beaucoup_ de données : beaucoup plus que ce dont vous avez besoin lorsque vous dépensez une P2PKH UTXO normale. Cela s'explique en partie par le fait que les informations sur l'adresse multisig ne se trouvent pas dans votre portefeuille, et en partie par le fait que vous dépensez de l'argent qui a été envoyé à une adresse P2SH (pay-to-script-hash), ce qui est beaucoup plus exigeant.

Au total, vous devrez recueillir trois choses : des informations détaillées sur l'UTXO, le redeemScript et toutes les clés privées impliquées. Vous aurez bien entendu besoin d'une nouvelle adresse de destinataire. Les clés privées doivent attendre l'étape de la signature, mais tout le reste peut être fait tout de suite.

### Accéder aux informations de l'UTXO

Pour commencer, prenez le `txid` et la `vout` pour la transaction que vous voulez passer, comme d'habitude. Par exemple, s'il s'agit de votre UTXO n°0 :

```
$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[0] | .txid') 
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
```
Cependant, vous devez également accéder à un troisième élément d'information sur l'UTXO, son `scriptPubKey`, qui est le script qui a verrouillé la transaction. Cela se fait par une simple invocation de `bitcoin-cli/jq` qui reflète les commandes précédentes.

```
$ utxo_spk=$(bitcoin-cli listunspent | jq -r '.[0] | .scriptPubKey')
```

### Enregistrer le Redeem Script

Heureusement, vous avez enregistré le redeemScript. Vous devez maintenant l'enregistrer dans une variable.

```
$ redeem_script="52210307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819210367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a2852ae"
```
### Décidez de votre bénéficiaire

Nous allons simplement renvoyer l'argent à nous-mêmes. C'est utile car cela libère les fonds de la multisig, les convertissant en une transaction P2PKH normale qui peut ensuite être confirmée par une seule clé privée :

```
$ recipient=$(bitcoin-cli getrawchangeaddress)
```
## Créez votre transaction

Vous pouvez maintenant créer votre transaction. Ce n'est pas différent de ce qui se fait habituellement.
```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 1.299}''')
```

## Signez votre transaction

Vous êtes maintenant prêt à signer votre transaction. Il s'agit d'un processus en plusieurs étapes car vous devrez le faire sur plusieurs machines, chacune d'entre elles apportant sa propre clé privée.

### Déposez votre première clé privée

Comme cette transaction n'utilise pas pleinement votre portefeuille, vous allez devoir accéder directement à vos clés privées. Commencez par la `machine1`, où vous devez récupérer toutes les clés privées de l'utilisateur qui ont participé au multisig :

```
machine1$ bitcoin-cli -named dumpprivkey address=$address1
cMgb3KM8hPATCtgMKarKMiFesLft6eEw3DY6BB8d97fkeXeqQagw
```
> :warning: **AVERTISSEMENT:** Accéder directement à vos clés privées depuis le shell est un comportement très dangereux et doit être fait avec une extrême prudence si vous utilisez de l'argent réel. Au moins, ne sauvegardez pas l'information dans une variable accessible depuis votre machine. La suppression de l'historique de votre shell est une autre étape importante. Il vaut mieux éviter tout simplement ce comportement.

### Faites votre première signature

Vous pouvez maintenant faire votre première signature avec la commande `signrawtransaction`. Ici, les choses sont différentes : vous allez devoir apprendre à la commande comment signer. Pour ce faire, vous devez ajouter les nouvelles informations suivantes :

* Incluez un argument `prevtxs` qui comprend le `txid`, la `vout`, le `scriptPubKey` et le `redeemScript` que vous avez enregistrés, chacun d'eux étant une paire clé-valeur individuelle dans l'objet JSON. 
* Incluez un argument `privkeys` qui énumère les clés privées que vous avez déposées sur cette machine.

```
machine1$ bitcoin-cli -named signrawtransaction hexstring=$rawtxhex prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cMgb3KM8hPATCtgMKarKMiFesLft6eEw3DY6BB8d97fkeXeqQagw"]'
{
  "hex": "020000000128e3b6f901705be0730e8a1345cf893e3cc38f0598e38bc56e9d43ac1ae11b62000000009200483045022100a9fe6ed0dbe14c0c4c7c89cee0aef2770f0b2bdcd6b3e8d71fe91e91c4bb765e02200cfba27a59b584a0cc8e70fb4438be94da417ee77eff28deb70449e012b6d6fa014752210307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819210367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a2852aeffffffff01e01dbe07000000001976a914cd1b2ba4fa8ae3e62bc4fc6be467a63228ceeedf88ac00000000",
  "complete": false,
  "errors": [
    {
      "txid": "621be11aac439d6ec58be398058fc33c3e89cf45138a0e73e05b7001f9b6e328",
      "vout": 0,
      "scriptSig": "00483045022100a9fe6ed0dbe14c0c4c7c89cee0aef2770f0b2bdcd6b3e8d71fe91e91c4bb765e02200cfba27a59b584a0cc8e70fb4438be94da417ee77eff28deb70449e012b6d6fa014752210307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819210367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a2852ae",
      "sequence": 4294967295,
      "error": "Operation not valid with the current stack size"
    }
  ]
}
```
Cela produit des erreurs effrayantes et dit que ce n'est pas complet (`complete`: false). Tout cela est correct. Vous pouvez voir que la signature a été partiellement réussie parce que le `hex` s'est allongé. Bien que la transaction ait été partiellement signée, elle n'est pas effectuée parce qu'elle nécessite plus de signatures. 

### Répétez pour les autres signataires

Vous pouvez maintenant transmettre la transaction, pour qu'elle soit à nouveau signée par toute autre personne nécessaire à la multisig. Ils le font en exécutant la même commande de signature que vous mais : (1) avec le `hex` plus long que vous produisez (`bitcoin-cli -named signrawtransaction hexstring=$rawtxhex prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cMgb3KM8hPATCtgMKarKMiFesLft6eEw3DY6BB8d97fkeXeqQagw"]' | jq -r '. | .hex'`) ; et (2) avec leur propre clé privée.

> :information_source: **NOTE — M-DE-N / N-DE-N:** Évidemment, si vous avez une signature n-de-n (comme le multisig 2-de-2 dans cet exemple), alors tout le monde doit signer, mais si vous avez une multisig m-de-n où "m < n", alors la signature sera complète lorsque seulement certains ("m") des signataires ont signé.

Pour ce faire, ils accèdent d'abord à leurs clés privées :

```
$ bitcoin-cli -named dumpprivkey address=$address2
cTi1Muvj24vG159R8orFjtqsPygCxhu8mJt2GLDQv7bNBGYoav4B
```
Ensuite, ils signent le nouveau `hex` en utilisant les mêmes valeurs `prevtxs` :

```
$ bitcoin-cli -named signrawtransaction hexstring=020000000128e3b6f901705be0730e8a1345cf893e3cc38f0598e38bc56e9d43ac1ae11b62000000009200483045022100a9fe6ed0dbe14c0c4c7c89cee0aef2770f0b2bdcd6b3e8d71fe91e91c4bb765e02200cfba27a59b584a0cc8e70fb4438be94da417ee77eff28deb70449e012b6d6fa014752210307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819210367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a2852aeffffffff01e01dbe07000000001976a914cd1b2ba4fa8ae3e62bc4fc6be467a63228ceeedf88ac00000000 prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cTi1Muvj24vG159R8orFjtqsPygCxhu8mJt2GLDQv7bNBGYoav4B"]'
{
  "hex": "020000000128e3b6f901705be0730e8a1345cf893e3cc38f0598e38bc56e9d43ac1ae11b6200000000db00483045022100a9fe6ed0dbe14c0c4c7c89cee0aef2770f0b2bdcd6b3e8d71fe91e91c4bb765e02200cfba27a59b584a0cc8e70fb4438be94da417ee77eff28deb70449e012b6d6fa01483045022100d5190eb824535423f67b15040efaba66953ea39f312540dd38504ed85ba6436402206171883ff28c235030550c36cadb31e40aaa9a74f71579557b74a5684545675c014752210307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819210367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a2852aeffffffff01e01dbe07000000001976a914cd1b2ba4fa8ae3e62bc4fc6be467a63228ceeedf88ac00000000",
  "complete": true
}
```
Troisièmement, ils peuvent avoir besoin d'envoyer la chaîne `hexstring` encore plus longue qu'ils produisent à d'autres signataires.

Dans ce cas, nous voyons maintenant que la signature est complète (`complete`: true) !

## Envoyez votre transaction

Lorsque vous avez terminé, vous devriez vous rabattre sur la méthodologie standard de JQ pour sauver votre `hexstring` et ensuite l'envoyer :

```
$ signedtx = $(bitcoin-cli -named signrawtransaction hexstring=020000000128e3b6f901705be0730e8a1345cf893e3cc38f0598e38bc56e9d43ac1ae11b62000000009200483045022100a9fe6ed0dbe14c0c4c7c89cee0aef2770f0b2bdcd6b3e8d71fe91e91c4bb765e02200cfba27a59b584a0cc8e70fb4438be94da417ee77eff28deb70449e012b6d6fa014752210307fd375ed7cced0f50723e3e1a97bbe7ccff7318c815df4e99a59bc94dbcd819210367c4f666f18279009c941e57fab3e42653c6553e5ca092c104d1db279e328a2852aeffffffff01e01dbe07000000001976a914cd1b2ba4fa8ae3e62bc4fc6be467a63228ceeedf88ac00000000 prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cTi1Muvj24vG159R8orFjtqsPygCxhu8mJt2GLDQv7bNBGYoav4B"]' | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
99d2b5717fed8875a1ed3b2827dd60ae3089f9caa7c7c23d47635f6f5b397c04
```

## Comprendre l'importance de cette méthode de signature élargie

Cela a demandé un certain travail, et comme vous l'apprendrez bientôt, la bêtise des clés privées, du redeem script, et de la scriptpubkey n'est pas vraiment nécessaire pour racheter des adresses à signatures multiples en utilisant les nouvelles versions de Bitcoin Core. Alors, quel était le but ?

Cette méthode de rachat montre la façon standard de signer et de réutiliser les transactions P2SH. En bref, pour racheter des fonds P2SH, une `signrawtransaction` doit :

1. Inclure le scriptPubKey, qui explique le puzzle cryptographique du P2SH.
2. Inclure le redeemScript, qui résout le puzzle cryptographique P2SH, et introduit un nouveau puzzle qui lui est propre.
3. Être exécuté sur chaque machine contenant les signatures requises.
4. Inclure les signatures pertinentes, qui permettent de résoudre le puzzle du redeemScript.

Ici, nous avons vu cette méthodologie utilisée pour racheter des fonds multisig. À l'avenir, vous pourrez également l'utiliser pour racheter des fonds qui ont été bloqués avec d'autres scripts P2SH plus complexes, comme expliqué à partir du chapitre 9.

## Résumé : Dépenser une transaction avec un multisig

Il s'avère que l'envoi d'argent à une adresse multisig peut demander beaucoup de travail. Mais tant que vous avez vos adresses d'origine et votre redeemScript, vous pouvez le faire en signant une transaction brute avec chaque adresse différente, et en fournissant quelques informations supplémentaires en cours de route.

## Quelle est la prochaine étape ?

Continuez "Développer les transactions Bitcoin" avec [§6.3 : Envoyer et dépenser une multisig automatisée](06_3_Envoyer_et_depenser_une_multisig_automatisee).