# 5.1: Surveiller les transactions bloquées

> :information_source: **NOTE:** Ce texte est encore un brouillon, afin que je puisse obtenir un retour d'information de la part des premiers examinateurs. Il n'est pas encore prêt pour l'apprentissage.

Il arrive qu'une transaction Bitcoin soit bloquée. Généralement, c'est parce que les frais de transaction n'étaient pas suffisants, mais cela peut aussi être dû à un problème ponctuel de réseau ou de logiciel.

## Surveillez vos transactions

Vous devez _toujours_ veiller à ce que vos transactions soient émises. `bitcoin-cli listtransactions` affichera toutes vos transactions entrantes et sortantes, tandis que `bitcoin-cli gettransaction` avec un txid affichera une transaction spécifique. 

Le tableau suivant montre une transaction qui n'a pas été insérée dans un bloc. Vous pouvez le savoir car il n'y a pas de confirmation. 

```
$  bitcoin-cli -named gettransaction txid=0f618e38efe887028a5dd04d0e12241431978b4de32f70308c13a114d7cfcbd2
{
  "amount": -0.84927000,
  "fee": -0.00000400,
  "confirmations": 0,
  "trusted": true,
  "txid": "0f618e38efe887028a5dd04d0e12241431978b4de32f70308c13a114d7cfcbd2",
  "walletconflicts": [
  ],
  "time": 1491588722,
  "timereceived": 1491588722,
  "bip125-replaceable": "no",
  "details": [
    {
      "account": "",
      "address": "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi",
      "category": "send",
      "amount": -0.84927000,
      "vout": 0,
      "fee": -0.00000400,
      "abandoned": false
    }
  ],
  "hex": "0200000001c6cb05c5212bb7ec14add72883a17f0a68aa399354bc2f7cf02df6e7d68a6937000000006a47304402203f21d769cbf3cf1626ef09bb35d0d8e88efe9f14d097f4b493628e96b2e1c90b0220162ff60701525e70942f5090dc48ec2b2f3b87cd40185351ab316991567f61c50121029045eaa55d283526c723e6d5495d9b3f077b545563f86465aafcd9bfdd50359effffffff0118e20f05000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000"
}
```
Une transaction peut être considérée comme bloquée si elle reste dans cet état pendant une période prolongée. Il n'y a pas si longtemps, vous pouviez être sûr que chaque transaction _finirait_ par passer. Mais, ce n'est plus le cas en raison de l'utilisation accrue de Bitcoin. Aujourd'hui, si une transaction est bloquée trop longtemps, elle sera retirée du mempool et sera perdue du réseau Bitcoin.

_Qu'est-ce que le mempool_ ? Mempool (ou Memory Pool) est l'ensemble de toutes les transactions non confirmées sur un nœud de bitcoin. Il s'agit des transactions qu'un nœud a reçues du réseau p2p et qui ne sont pas encore incluses dans un bloc. Chaque nœud de bitcoin peut avoir un ensemble de transactions légèrement différent dans son mempool : différentes transactions peuvent avoir été propogées à un nœud spécifique. Cela dépend de la date à laquelle le nœud a été lancé pour la dernière fois et aussi des limites qu'il est disposé à stocker. Lorsqu'un mineur fait un bloc, il utilise les transactions de son mempool. Ensuite, lorsqu'un bloc est vérifié, tous les mineurs retirent les transactions contenues dans leur propre mempool. Depuis la version 0.12 de Bitcoin, les transactions non confirmées peuvent également expirer des mempools si elles sont trop anciennes, généralement 72 heures, et depuis la version 0.14.0, le délai d'expulsion a été porté à 2 semaines. Les groupes de minage peuvent avoir leurs propres mécanismes de gestion des mempools.

Cette liste de toutes les [transactions non confirmées](https://blockchain.info/unconfirmed-transactions) peut ne pas correspondre au mempool d'une machine individuelle, mais elle devrait (la plupart du temps) en constituer un sur-ensemble.

## Décider de ce qu'il faut faire

Si votre transaction est bloquée plus longtemps que vous ne le souhaitez, vous pouvez généralement faire l'une des quatre choses suivantes :

**1. Attendre qu'elle disparaisse.** Si vous avez envoyé votre transaction avec des frais faibles ou moyens, elle devrait finalement être acceptée. Comme l'indique [Bitcoin Fees](https://bitcoinfees.21.co/), les transactions dont les frais sont inférieurs ou égaux à 100 satoshis par octet ont tendance à être retardées de 25 à 100 blocs (jusqu'à 16 ou 17 heures !), mais elles finissent par apparaître dans un bloc.

**2. Attendre qu'elle expire. ** Si vous avez accidentellement envoyé sans frais de transaction, ou si un numéro ou d'autres conditions sont remplies, il se peut que votre transaction n'aboutisse jamais. Cependant, vos pièces ne sont pas perdues. Tant que vous n'avez pas un portefeuille qui renvoie délibérément des transactions non confirmées, il devrait se dégager du mempool au bout de trois jours environ, et vous pourrez alors réessayer.

**3. Utiliser RBF comme expéditeur. ** Si vous êtes l'expéditeur de la transaction et que vous avez opté pour le système RBF (Replace-By-Fee), vous pouvez réessayer avec des frais plus élevés. Voir le [chapitre 5.2 : Ré-envoyer une transaction avec RBF](05_2_Re_envoyer_une_transaction_avec_RBF.md).

**4. Utiliser le CPFP comme destinataire. ** Autrement, si vous êtes le destinataire de la transaction, vous pouvez utiliser le système CPFP (Child-pays-for-parent) pour utiliser la transaction non confirmée comme entrée dans une nouvelle transaction. Voir [chapitre 5.3 : Financer une transaction avec CPFP](05_3_Financer_une_transaction_avec_CPFP.md).



## Résumé : Surveiller les transactions bloquées

Ceci est une introduction à la puissance des transactions Bitcoin. Si vous savez qu'une transaction est bloquée, vous pouvez alors décider de la libérer avec des fonctionnalités comme RBF ou CPFP.

## Quelle est la prochaine étape ?

Continuez "Contrôler les transactions Bitcoin" avec [chapitre 5.2 : Ré-envoyer une transaction avec RBF](05_2_Re_envoyer_une_transaction_avec_RBF.md).