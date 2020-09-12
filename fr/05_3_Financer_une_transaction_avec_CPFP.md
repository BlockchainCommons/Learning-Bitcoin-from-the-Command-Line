# 5.3: Financer une transaction avec CPFP

> :information_source: **NOTE:** Ce texte est encore un brouillon, afin que je puisse obtenir un retour d'information de la part des premiers examinateurs. Il n'est pas encore prêt pour l'apprentissage.

Si votre transaction Bitcoin est bloquée, et que vous en êtes le _bénéficiaire_, vous pouvez l'annuler en utilisant le système CPFP (child-pays-for-parent). Il s'agit d'une alternative à la possibilité pour l'_expéditeur_ de le faire avec le sytème RBF.

> :warning: **AVERTISSEMENT VERSION:** Il s'agit d'une innovation de Bitcoin Core v 0.13.0, ce qui signifie à nouveau que la plupart des gens devraient l'utiliser maintenant.

## Comprendre le fonctionnement de CPFP

Le RBF était axé sur l'expéditeur. Il s'est trompé et a dû augmenter les frais, ou il voulait être intelligent et combiner les transactions pour diverses raisons. C'était une fonction puissante axée sur l'expéditeur. D'une certaine manière, le CPFP est l'opposé du RBF, car il donne du pouvoir au destinataire qui sait que son argent n'est pas encore arrivé et qui veut l'accélérer. Cependant, c'est aussi une fonction beaucoup plus simple, avec une applicabilité moins large. 

Fondamentalement, l'idée du CPFP est qu'un bénéficiaire a une transaction qui n'a pas été confirmée dans un bloc qu'il veut dépenser. Il inclut donc cette transaction non confirmée dans une nouvelle transaction et paie une redevance suffisamment élevée pour encourager un mineur à inclure dans un bloc à la fois la transaction originale (parent) et la nouvelle transaction (enfant). Ainsi, les transactions du parent et de l'enfant sont effectuée simultanément.

Il convient de noter que le CPFP n'est pas une nouvelle caractéristique du protocole, comme le RBF. Il s'agit simplement d'un nouveau système d'incitation qui peut être utilisé pour la sélection des transactions par les mineurs. Cela signifie également qu'il n'est pas aussi fiable qu'un changement de protocole comme le RBF : il peut y avoir des raisons pour lesquelles l'enfant n'est pas sélectionné pour être mis dans un bloc, et cela empêchera le parent d'être mis dans un bloc.

## Dépenser des UTXOs non confirmées

Le financement d'une transaction avec le CPFP est un processus très simple utilisant les méthodes que vous connaissez déjà :

   1. Trouver le `txid` et le `vout` de la transaction non confirmée. C'est peut-être la partie la plus délicate, car `bitcoin-cli` tente généralement de vous protéger des transactions non confirmées. L'expéditeur peut être en mesure de vous envoyer cette information ; même avec seulement le `txid`, vous devriez être en mesure de trouver le `vout` dans un explorateur de la chaîne de blocs.

Vous avez une autre option : utiliser `bitcoin-cli getrawmempool`, qui peut être utilisé pour lister le contenu de tout votre mempool, où se trouveront les transactions non confirmées. Vous pouvez avoir à en fouiller plusieurs si le mempool est particulièrement occupé. Vous pouvez alors obtenir plus d'informations sur une transaction spécifique avec `bitcoin-cli getrawtransaction`, avec le drapeau `verbose` positionné sur `true` :   

   ```
$ bitcoin-cli getrawmempool
[
  "ed7f68d9e363f710379c83baca1b71360b3a2ceaf62f8414a26b6680f77e132b"
]

$ bitcoin-cli getrawtransaction ed7f68d9e363f710379c83baca1b71360b3a2ceaf62f8414a26b6680f77e132b 1
{
  "hex": "02000000010d5151273464ac8fa74c1b57ff8e4650b8e190c488b1cfadae6561ac3988e83c000000006a473044022059c402379c8e7d5bf60840fc75497ee67694e6dfc9b0391c59b4b202edf4194b0220686c5a27bee1a9214e410cbce3d11e47ca57736d7546ba208adcc068a3f826f5012103fb14f31f5f143463198a3e79c7529cae529a32857ada29bec26f7ea9d8ea67f3ffffffff0130e1be07000000001976a914cbecb861750ee3b00ea845f5c1efa72d1c541cec88ac00000000",
  "txid": "ed7f68d9e363f710379c83baca1b71360b3a2ceaf62f8414a26b6680f77e132b",
  "hash": "ed7f68d9e363f710379c83baca1b71360b3a2ceaf62f8414a26b6680f77e132b",
  "size": 191,
  "vsize": 191,
  "version": 2,
  "locktime": 0,
  "vin": [
    {
      "txid": "3ce88839ac6165aeadcfb188c490e1b850468eff571b4ca78fac64342751510d",
      "vout": 0,
      "scriptSig": {
        "asm": "3044022059c402379c8e7d5bf60840fc75497ee67694e6dfc9b0391c59b4b202edf4194b0220686c5a27bee1a9214e410cbce3d11e47ca57736d7546ba208adcc068a3f826f5[ALL] 03fb14f31f5f143463198a3e79c7529cae529a32857ada29bec26f7ea9d8ea67f3",
        "hex": "473044022059c402379c8e7d5bf60840fc75497ee67694e6dfc9b0391c59b4b202edf4194b0220686c5a27bee1a9214e410cbce3d11e47ca57736d7546ba208adcc068a3f826f5012103fb14f31f5f143463198a3e79c7529cae529a32857ada29bec26f7ea9d8ea67f3"
      },
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 1.29950000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 cbecb861750ee3b00ea845f5c1efa72d1c541cec OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914cbecb861750ee3b00ea845f5c1efa72d1c541cec88ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mz7D3c6tMtNXV6CTsEwDBJ2vhSVTo9bSMN"
        ]
      }
    }
  ]
}
   ```
Regardez dans le tableau `vout`. Trouvez l'objet qui correspond à votre adresse. (Ici, c'est le seul.) La valeur `n` est votre `vout`. Vous avez maintenant tout ce dont vous avez besoin pour créer une nouvelle transaction CPFP.

```
$ utxo_txid=ed7f68d9e363f710379c83baca1b71360b3a2ceaf62f8414a26b6680f77e132b
$ utxo_vout=0
$ recipient2=$(bitcoin-cli getrawchangeaddress)
```

   2. Créez une transaction brute en utilisant votre transaction non confirmée comme entrée.
   3. Doublez les frais de transaction (ou plus).

Lorsque vous prenez ces mesures, tout devrait sembler tout à fait normal, même si vous travaillez avec une transaction non confirmée. Pour vérifier que tout allait bien, nous avons même examiné les résultats de notre signature avant de sauvegarder les informations dans une variable :

```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient2'": 1.2985 }''')

$ bitcoin-cli -named signrawtransaction hexstring=$rawtxhex | jq -r '.hex'
02000000012b137ef780666ba214842ff6ea2c3a0b36711bcaba839c3710f763e3d9687fed000000006a473044022003ca1f6797d781ef121ba7c2d1d41d763a815e9dad52aa8bc5ea61e4d521f68e022036b992e8e6bf2c44748219ca6e0056a88e8250f6fd0794dc69f79a2e8993671601210317b163ab8c8862e09c71767112b828abd3852e315441893fa0f535de4fa39b8dffffffff01905abd07000000001976a91450b1d90a130c4f3f1e5fbfa7320fd36b7265db0488ac00000000

$ signedtx=$(bitcoin-cli -named signrawtransaction hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
6a184a2f07fa30189f4831d6f041d52653a103b3883d2bec2f79187331fd7f0e
```

   4. Il n'est pas nécessaire de croiser les doigts. Vous avez vérifié que vos données sont correctes. À partir de ce moment, vous n'avez plus le contrôle.

Vos transactions peuvent se dérouler rapidement. Elles ne le seront peut-être pas. Tout dépend si les mineurs qui génèrent aléatoirement les blocs actuels ont le patch CPFP ou non. Mais vous avez donné à vos transactions les meilleures chances.

C'est vraiment tout ce qu'il y a à faire.

### Être conscient des nuances

Bien que le CPFP soit généralement décrit comme étant l'utilisation par un bénéficiaire d'une nouvelle transaction pour payer une ancienne qui n'a pas été confirmée, il y a une nuance à cela.

Un _expéditeur_ pourrait utiliser le CPFP pour libérer une transaction s'il en recevait de la monnaie. Il n'aurait plus qu'à utiliser cette modification comme entrée, et l'utilisation du CPFP qui en résulterait libérerait la totalité de la transaction. Il ferait mieux d'utiliser RBF s'il est activé, car les frais totaux seraient alors moins élevés.

Un _bénéficiaire_ pourrait utiliser le CPFP même s'il ne prévoit pas de dépenser immédiatement l'argent, par exemple s'il craint que les fonds ne lui soient pas renvoyés si la transaction arrive à expiration. Dans ce cas, il crée simplement une transaction enfant qui envoie tout l'argent (moins les frais de transaction) à une adresse de change. C'est ce que nous avons fait dans notre exemple, ci-dessus.

## Résumé : Financer une transaction avec CPFP

Vous pouvez profiter des incitations du CPFP pour libérer les fonds qui vous ont été envoyés mais qui n'ont pas été confirmés. Il vous suffit d'utiliser la transaction non confirmée comme UTXO et de payer des frais de transaction plus élevés que la moyenne.

_Quel est le pouvoir du CPFP ?_ Le CPFP est surtout utile pour débloquer des fonds lorsque vous êtes le destinataire et que l'expéditeur ne peut pas aider pour une raison quelconque. Il n'a pas les possibilités, plus puissantes, du RBF, mais c'est un moyen alternatif d'exercer un contrôle sur une transaction après qu'elle ait été placée dans le mempool, mais avant qu'elle ne soit confirmée dans un bloc.

## Quelle est la prochaine étape ?

Avancer dans `bitcoin-cli` avec [Chapitre 6 : Développer les transactions Bitcoin](06_0_Developper_les_transactions_Bitcoin.md).
