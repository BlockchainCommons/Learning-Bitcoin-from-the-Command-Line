# 6.4: Envoyer une transaction avec un délai de blocage

> :information_source: **NOTE:** Ce texte est encore un brouillon, afin que je puisse obtenir un retour d'information de la part des premiers examinateurs. Il n'est pas encore prêt pour l'apprentissage.

La deuxième façon de varier la façon dont vous envoyez une transaction de base est de choisir un délai de blocage. Cela vous donne la possibilité d'envoyer des transactions brutes à un moment donné dans le futur.

## Comprendre le fonctionnement du locktime

Lorsque vous créez une transaction verrouillée dans le temps (locktime), vous la verrouillez avec un nombre qui représente soit une hauteur de bloc (s'il s'agit d'un petit nombre), soit un horodatage UNIX (s'il s'agit d'un grand nombre). Cela indique au réseau Bitcoin que la transaction ne peut pas être mise en bloc tant que l'heure spécifiée n'est pas arrivée ou que la chaîne de blocs n'a pas atteint la hauteur spécifiée. 

_Qu'est-ce que la hauteur de bloc ?_ C'est le nombre total de blocs dans la chaîne, en remontant jusqu'au bloc initial de Bitcoin. 

Lorsqu'une opération de type locktime est en attente d'entrer dans un bloc, elle peut être annulée. Cela signifie qu'elle est loin, très loin d'être finalisée. En fait, la possibilité d'annuler est l'objectif même d'une opération de type locktime.

_Qu'est-ce-que le nLockTime ?_ C'est la même chose que le locktime. Plus précisément, c'est ce que l'on appelle le locktime interne au code source de Bitcoin Core.

_Qu'est-ce-que le Timelock ?_ Le verrouillage n'est qu'une façon de verrouiller les transactions Bitcoin jusqu'à un certain point dans le futur ; collectivement, ces méthodes sont appelées "timelocks". Locktime est la méthode de verrouillage du temps la plus élémentaire. Elle verrouille une transaction entière avec une heure absolue, et elle est disponible via `bitcoin-cli` (c'est pourquoi c'est la seule méthode couverte dans cette section). Une méthode parallèle, qui verrouille une transaction avec un temps relatif, est définie dans la [BIP 68](https://github.com/bitcoin/bips/blob/master/bip-0068.mediawiki) et couverte dans le [§9.3 : Utilisation de CSV dans des scripts](09_3_Utilisation_de_CSV_dans_des_scripts.md). Bitcoin Script permet en outre les deux types de verrouillages temporels, permettant de verrouiller des sorties individuelles au lieu de transactions entières. Les timelocks absolus sont liés à OP_CHECKLOCKTIMEVERIFY, qui est défini dans la [BIP 65](https://github.com/bitcoin/bips/blob/master/bip-0065.mediawiki) et couvert au [§9.2 : Utilisation de CLTV dans des scripts](09_2_Utilisation_de_CLTV_dans_des_scripts), tandis que les timelocks relatifs sont liés à OP_CHECKSEQUENCEVERIFY, qui est défini dans la [BIP 112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki) et également couvert au [§9.3](09_3_Utilisation_de_CSV_dans_des_scripts.md).

## Créez une transaction locktime

Pour créer une opération locktime, vous devez d'abord déterminer à quoi vous allez fixer le verrouillage.

### Calculez votre heure en fonction de l'horodatage UNIX

Le plus souvent, vous fixez l'heure à un horodatage UNIX représentant une date et une heure précises. Vous pouvez calculer un horodatage UNIX sur un site web comme [UNIX Time Stamp](http://www.unixtimestamp.com/) ou [Epoch Convertor](https://www.epochconverter.com/). Toutefois, il serait préférable d'[écrire votre propre script](https://www.epochconverter.com/#code) sur votre machine locale, afin que vous sachiez que l'horodatage UNIX que vous recevez est exact. Si vous ne le faites pas, vérifiez au moins sur deux sites différents.

_Pourquoi utiliser un horodatage UNIX ?_ L'utilisation d'un horodatage UNIX permet de lier définitivement une transaction à un moment précis, sans se soucier de savoir si la vitesse de création des blocs pourrait changer à un moment donné. C'est la solution la plus sûre, en particulier si vous créez un bloc horaire qui est loin dans le futur. Mais, au-delà de cela, c'est juste plus intuitif, en créant une corrélation directe entre une date de calendrier et l'heure à laquelle la transaction peut être exploitée.

> :warning: **AVERTISSEMENT :** Le locktime avec les horodatages UNIX a un peu de marge de manœuvre : la libération des blocs n'est pas régulière et les heures des blocs peuvent être en avance de deux heures sur le temps réel, donc un temps de verrouillage signifie en fait "dans les quelques heures suivant cette heure, plus ou moins".

### Calculez votre heure en fonction de la hauteur de bloc

Vous pouvez également régler l'heure sur un nombre plus petit représentant la hauteur d'un bloc. Pour calculer votre future hauteur de bloc, vous devez d'abord connaître la hauteur de bloc actuelle. `bitcoin-cli getblockcount` vous indiquera ce que votre machine locale pense de la hauteur de bloc. Vous pouvez vérifier qu'elle est à jour grâce à l'alias `btcblock`, qui compare la hauteur de bloc de votre bitcoind avec une hauteur de bloc prise sur le réseau.

Une fois que vous avez déterminé la hauteur actuelle, vous pouvez décider à quelle distance dans l'avenir vous devez fixer votre heure. N'oubliez pas qu'en moyenne, un nouveau bloc sera créé toutes les 10 minutes. Par exemple, si vous souhaitez fixer l'heure à une semaine dans le futur, vous devez choisir une hauteur de bloc de 6 x 24 x 7 = 1 008 blocs de plus que le bloc actuel.

_Pourquoi utiliser une hauteur de bloc ?_ Contrairement à l'horodatage, il n'y a pas de flou pour les hauteurs de bloc. Si vous fixez une hauteur de bloc de 120 000 pour votre heure de fermeture, alors il n'y a absolument aucun moyen qu'elle aille dans le bloc 119 999. Cela peut faciliter le contrôle algorithmique de votre transaction locktime. L'inconvénient est que vous ne pouvez pas être aussi sûr du moment précis où le locktime sera effectué.

> :warning: **AVERTISSEMENT :** Si vous voulez fixer une heure de fermeture liée à la hauteur de bloc, vous devez la fixer à moins de 500 millions. Si vous la fixez à 500 millions ou plus, votre numéro sera plutôt interprété comme un horodatage. Comme l'horodatage UNIX de 500 millions était le 5 novembre 1985, cela signifie probablement que votre transaction sera bloquée à la première occasion pour les mineurs.

## Rédigez votre transaction

Une fois que vous avez déterminé votre heure de fermeture, il vous suffit d'écrire une transaction brute typique, avec une troisième variable pour l'heure de fermeture :

```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.8, "'$changeaddress'": 0.0895 }''' locktime=1119160)
```
Notez que cette utilisation de `locktime` est inférieure à 500 millions, ce qui signifie qu'elle définit une hauteur de bloc. Dans le cas présent, il s'agit juste de quelques blocs au-delà de la hauteur du bloc actuel, afin d'illustrer comment fonctionne le temps de verrouillage sans avoir à attendre longtemps pour voir ce qui se passe.

Voici à quoi ressemble la transaction créée :

```
$ bitcoin-cli -named decoderawtransaction hexstring=$rawtxhex 
{
  "txid": "34650e513b9b84bc47bc10f39ab7f66f59915b65c0c07fdcaf786502d88cec4a",
  "hash": "34650e513b9b84bc47bc10f39ab7f66f59915b65c0c07fdcaf786502d88cec4a",
  "size": 119,
  "vsize": 119,
  "version": 2,
  "locktime": 1119160,
  "vin": [
    {
      "txid": "4dcef95a7bb24d907cc0954d75754f8bf1b70cc0542ca071a023abde425a734b",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967294
    }
  ],
  "vout": [
    {
      "value": 0.80000000,
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
      "value": 0.08950000,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 695c79109dc8424573ca6963bda9beeb5d8a6c68 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914695c79109dc8424573ca6963bda9beeb5d8a6c6888ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mq842Ku2f2ySWpapEwxjuTCjR3Btvz88nx"
        ]
      }
    }
  ]
}
```
Notez que le numéro de séquence (4294967294) est inférieur à 0xffffffffffff. Il s'agit d'une signalisation nécessaire pour montrer que la transaction comprend un temps de blocage. Elle est également effectuée automatiquement par `bitcoin-cli`. Si le numéro de séquence est plutôt fixé à 0xffffffffff, votre temps de blocage sera ignoré.

> :information_source: **NOTE — SEQUENCE:** C'est la deuxième utilisation de la valeur nSequence dans Bitcoin. Comme pour RBF, `nSequence` est à nouveau utilisé comme opt-in, cette fois pour l'utilisation de locktime. 0xffffffffff-1 (4294967294) est la valeur préférée pour signaler le temps de verrouillage, car elle interdit délibérément l'utilisation à la fois de RBF (qui nécessite `nSequence < 0xffffffff-1`) et du verrouillage temporel relatif (qui nécessite `nSequence < 0xf0000000`), les deux autres utilisations de la valeur nSequence. Si vous fixez `nSequence` à une valeur inférieure à `0xf0000000`, vous obtiendrez également une synchronisation temporelle relative de votre transaction, ce qui n'est probablement pas ce que vous souhaitez.

> :warning: **AVERTISSEMENT :** Si vous créez une transaction brute locktime par un autre moyen que `bitcoin-cli`, vous devrez régler la séquence à moins de 0xffffffff à la main.

## Envoyez votre transaction

Maintenant, vous êtes probablement familier avec la finition des choses :

```
$ signedtx=$(bitcoin-cli -named signrawtransaction hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
error code: -26
error message:
64: non-final
```
Oula ! Quelle est cette erreur ?

Depuis 2013, vous ne pouvez généralement pas placer la transaction verrouillée dans le mempool avant que son verrouillage n'ait expiré. Cependant, vous pouvez toujours conserver la transaction, en la renvoyant occasionnellement au réseau Bitcoin jusqu'à ce qu'elle soit acceptée dans le mempool. Vous pouvez également envoyer la transaction signée (`$signedtx`) au destinataire, afin qu'il puisse la placer dans le mempool lorsque le délai de blocage a expiré. 

Une fois le délai de blocage écoulé, n'importe qui peut envoyer cette transaction signée au réseau, et le destinataire recevra l'argent comme prévu ... à condition que la transaction n'ait pas été annulée.

## Annuler une opération de blocage

L'annulation d'une opération de blocage est _très_ simple : vous envoyez une nouvelle opération en utilisant au moins un des mêmes UTXO.

## Résumé : Envoyer une transaction avec un délai de blocage

Locktime offre un moyen de créer une transaction qui ne _doit_ pas être relayée au réseau et qui ne sera pas acceptée dans un bloc jusqu'à ce que le moment approprié soit arrivé. En attendant, il peut être annulé simplement en réutilisant un UTXO.

_Quelle est la puissance de locktime ?_ Le pouvoir des transactions de type locktime peut ne pas être immédiatement évident en raison de la possibilité de l'annuler si facilement. Cependant, c'est un autre des fondements des contrats intelligents : il est très utile dans une variété d'applications de garde ou contractuelles. Par exemple, imaginez une situation où un tiers détient vos bitcoins. Afin de garantir la restitution de vos bitcoins si le dépositaire venait à disparaître, il pourrait produire un timelock de transition pour vous rendre l'argent, puis mettre à jour ce timelock de temps en temps avec un nouveau timelock, plus tard. En cas d'échec de la mise à jour, les pièces vous seraient rendues à l'expiration du timelock actuel. Le verrouillage pourrait être appliqué de la même manière à un réseau de paiement, où le réseau conserve les pièces pendant qu'elles sont échangées par les participants au réseau. Enfin, un testament offre un exemple de contrat plus complexe, dans lequel les paiements sont envoyés à un certain nombre de personnes. Ces paiements s'appuieraient sur des transactions à temps de blocage, et seraient continuellement mis à jour tant que le propriétaire continue à montrer des signes de vie. (Le facteur unificateur de toutes ces applications est, bien entendu, _la confiance_. Les transactions simples à temps de blocage ne fonctionnent que si l'on peut faire confiance au détenteur des pièces pour les envoyer dans les conditions appropriées).

## Quelle est la prochaine étape ?

Continuez "Développer les transactions Bitcoin" avec le [§6.5: Envoyer une transaction contenant des données](06_5_Envoyer_une_transaction_contenant_des_donnees.md).