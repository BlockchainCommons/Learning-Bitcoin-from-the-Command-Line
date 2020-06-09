# Interlude : Utilisation de JQ

> :information_source: **NOTE:** Ce texte est encore un brouillon, afin que je puisse obtenir un retour d'information de la part des premiers examinateurs. Il n'est pas encore prêt pour l'apprentissage.

La création d'une transaction brute a révélé que les résultats les plus complexes de bitcoin-cli ne peuvent pas être facilement sauvegardés dans des variables de ligne de commande. La réponse est JQ, qui permet de filtrer les éléments individuels des données JSON les plus complexes.

## Installer JQ

JQ est disponible ici : [Github repository](https://stedolan.github.io/jq/). Il suffit de le télécharger pour Linux, OS X ou Windows, selon le cas.

Une fois que vous avez téléchargé le binaire, vous pouvez l'installer sur votre système. Si vous travaillez sur un VPS Debian comme nous le suggérons, votre installation ressemblera à ceci :

```
$ mv jq-linux64 jq
$ sudo /usr/bin/install -m 0755 -o root -g root -t /usr/local/bin jq
```
_Qu'est-ce que JQ ?_ Le repository l'explique le mieux, en disant que "jq est comme sed pour les données JSON - vous pouvez l'utiliser pour découper, filtrer, cartographier et transformer des données structurées avec la même facilité que sed, awk, grep et leurs amis vous laissent jouer avec du texte".

## Utiliser JQ pour accéder à une valeur d'objet JSON à partir d'une clé

**Exemple d'utilisation:** _Extrayez la valeur hexadécimale à partir d'une transaction brute signée._

Dans la section précédente, l'utilisation de `signrawtransaction` offrait un exemple de l'impossibilité de saisir facilement des données dans des variables en raison de l'utilisation de la sortie JSON :

```
$ bitcoin-cli signrawtransaction $rawtxhex
{
  "hex": "0200000001735dfa1584b930a78ad2c1d6db72dd2a80ae5e5d552ad97e19f1d50d41fdd6d8000000006a47304402202210ce4b2a037da02622c380278cd79fec4e0e016e66f3eb894a2dcbb9ee998f02202cac167e6abdbbf08af139fb7c6b86e9d2e58e5516cd566ae2d54953ead9923b012102111bb978a3c93a00038ae344a1a017d7fee8a9be9d0558b5793ce6f440704a96ffffffff01b0e78604000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000",
  "complete": true
}
```
Heureusement, JQ peut facilement saisir des données de ce type !

Pour utiliser JQ, lancez `jq` derrière le pipe, et utilisez toujours l'invocation standard de `jq -r '.'`. Le `-r` indique à JQ de produire une sortie brute, qui fonctionnera pour les variables de ligne de commande, tandis que le `.` indique à jq de produire une sortie. Nous protégeons cet argument dans `' ' ` parce que nous aurons besoin de cette protection plus tard, lorsque nos invocations de `jq` deviendront plus complexes.

Pour capturer une valeur spécifique d'un objet JSON, il suffit de lister la clé après le `.`:

```
$ bitcoin-cli signrawtransaction $rawtxhex | jq -r '.hex'
0200000001735dfa1584b930a78ad2c1d6db72dd2a80ae5e5d552ad97e19f1d50d41fdd6d8000000006a47304402202210ce4b2a037da02622c380278cd79fec4e0e016e66f3eb894a2dcbb9ee998f02202cac167e6abdbbf08af139fb7c6b86e9d2e58e5516cd566ae2d54953ead9923b012102111bb978a3c93a00038ae344a1a017d7fee8a9be9d0558b5793ce6f440704a96ffffffff01b0e78604000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000
```
Avec cet outil en main, vous pouvez obtenir les informations des objets JSON et les transmettre aux variables de la ligne commande :
```
$ signedtx=$(bitcoin-cli signrawtransaction $rawtxhex | jq -r '.hex')
$ echo $signedtx
0200000001735dfa1584b930a78ad2c1d6db72dd2a80ae5e5d552ad97e19f1d50d41fdd6d8000000006a47304402202210ce4b2a037da02622c380278cd79fec4e0e016e66f3eb894a2dcbb9ee998f02202cac167e6abdbbf08af139fb7c6b86e9d2e58e5516cd566ae2d54953ead9923b012102111bb978a3c93a00038ae344a1a017d7fee8a9be9d0558b5793ce6f440704a96ffffffff01b0e78604000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000
```
Vous pouvez alors utiliser ces variables facilement et sans erreur:
```
$ bitcoin-cli sendrawtransaction $signedtx
3f9ccb6e16663e66dc119de1866610cc4f7a83079bfec2abf0598ed3adf10a78
```
## Utiliser JQ pour accéder aux valeurs d'un objet JSON unique dans un tableau à partir de la clé

**Exemple d'utilisation:** _Extrayez le txid et le vout pour un UTXO donné._

Extraire des données d'un objet JSON est facile, mais que faire si cet objet JSON se trouve dans un tableau JSON ? La commande `listunspent` offre un excellent exemple, car elle contient généralement un certain nombre de transactions différentes. Que faire si vous souhaitez obtenir des informations spécifiques à partir de l'_une_ d'entre elles ?

Lorsque vous travaillez avec un tableau JSON, la première chose à faire est d'indiquer à JQ quel index il faut accéder. Par exemple, vous avez peut-être consulté vos transactions dans `listunspent` et décidé que vous vouliez travailler avec la première d'entre elles. Vous utilisez `.[0]` pour accéder à ce premier élément. Le `[]` dit que nous faisons référence à un tableau JSON et le 0 dit que nous voulons le 0ème index.

```
$ bitcoin-cli listunspent | jq -r '.[0]'
{
  "txid": "2b5f5798359e0e23e02764588166f222d4ce056419dec83c743b72aad171d708",
  "vout": 1,
  "address": "mjtEqr4Fffd1XtpAkKoDkMBP54mMXJeQ3j",
  "account": "",
  "scriptPubKey": "76a9142fe70d51e886b9ef73b76c1743c5a2bb2894db8588ac",
  "amount": 0.76,
  "confirmations": 6578,
  "spendable": true,
  "solvable": true
}
```
Vous pouvez ensuite extraire une valeur individuelle de ce tableau (1) en utilisant un pipe _parmi_ les arguments de JQ ; puis (2) en demandant la valeur spécifique après, comme dans l'exemple précédent. L'exemple suivant permet de capturer la `txid` du 0ème objet JSON dans le tableau JSON produit par `listunspent` :

```
$ bitcoin-cli listunspent | jq -r '.[0] | .txid'
2b5f5798359e0e23e02764588166f222d4ce056419dec83c743b72aad171d708
```
Notez bien comment les ` ' '` entourent toute l'expression JQ _y compris_ la pipe.

Cette méthode peut être utilisée pour remplir les variables d'un UTXO que vous voulez utiliser :

```
$ newtxid=$(bitcoin-cli listunspent | jq -r '.[0] | .txid')
$ newvout=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
$ echo $newtxid
2b5f5798359e0e23e02764588166f222d4ce056419dec83c743b72aad171d708
$ echo $newvout
1
```
Voilà ! Nous pouvons maintenant créer une nouvelle transaction brute en utilisant notre 0ème UTXO comme entrée, sans avoir à taper à la main les informations sur les UTXO !

## Utiliser JQ pour accéder aux valeurs correspondantes des objets JSON dans un tableau par clé

**Exemple d'utilisation:** _Indiquez la valeur de toutes les UTXO non dépensées._

Au lieu d'accéder à une seule valeur spécifique dans un objet JSON spécifique, vous pourriez accéder à l'ensemble des valeurs spécifiques dans tous les objets JSON. Cela se fait avec `.[]`, où aucun index n'est spécifié. Par exemple, cela permet de lister tous les fonds non dépensés :

```
$ bitcoin-cli listunspent | jq -r '.[] | .amount'
0.76
3.9
1.95
```

## Utiliser JQ pour des calculs simples par clé

**Exemple d'utilisation:** _Additionnez la valeur de tous les UTXOs non dépensés._

À ce stade, vous pouvez commencer à utiliser la sortie de JQ pour les mathématiques simples. Par exemple, en additionnant les valeurs de ces transactions non dépensées avec un simple script `awk`, vous obtiendrez l'équivalent de `getbalance` :

```
$ bitcoin-cli listunspent | jq -r '.[] | .amount' | awk '{s+=$1} END {print s}'
6.61
$ bitcoin-cli getbalance
6.61000000
```

## Utiliser JQ pour afficher plusieurs valeurs d'objets JSON dans un tableau par plusieurs clés

**Exemple d'utilisation:** _Liste des informations sur l'utilisation de tous les UTXOs._

JQ peut facilement extraire des éléments individuels à partir d'objets et de tableaux JSON et placer ces éléments dans des variables. Ce sera sa principale utilité dans les sections à venir. Cependant, il peut également être utilisé pour réduire les énormes quantités d'informations produites par `bitcoin-cli` en quantités plus raisonnables.

Par exemple, vous pourriez vouloir voir une liste de toutes vos UTXOs (`.[]`) et obtenir une liste de toutes leurs informations les plus importantes (`.txid, .vout, .amount`) :

```
$ bitcoin-cli listunspent | jq -r '.[] | .txid, .vout, .amount'
2b5f5798359e0e23e02764588166f222d4ce056419dec83c743b72aad171d708
1
0.76
ec0598918f6f5476cb90365651e8a2724ef26f949290bbf196f41ed96092a52f
0
3.9
3470e5fe08633583d136b9cd49bb1a224c9d9313a0b4584fd3b7438dbdf34dbd
0
1.95
```
Il est donc facile de décider quels UTXOs dépenser dans une transaction brute, mais ce n'est pas très joli.

Heureusement, JQ vous permet aussi d'être fantaisiste. Vous pouvez utiliser les `{}` pour créer de nouveaux objets JSON (soit pour une analyse supplémentaire, soit pour une sortie plus adequat). Vous pouvez également définir le nom de la nouvelle clé pour chacune de vos valeurs. La sortie résultante devrait être beaucoup plus intuitive et moins sujette à l'erreur (bien qu'évidemment, elle soit moins utile pour fournir les valeurs directement dans les variables).

L'exemple suivant montre exactement la même analyse de `listunspent`, mais avec chaque ancien objet JSON reconstruit comme un nouvel objet JSON abrégé, avec toutes les nouvelles valeurs nommées avec leurs anciennes clés :

```
$ bitcoin-cli listunspent | jq -r '.[] | { txid: .txid, vout: .vout, amount: .amount }'
{
  "txid": "2b5f5798359e0e23e02764588166f222d4ce056419dec83c743b72aad171d708",
  "vout": 1,
  "amount": 0.76
}
{
  "txid": "ec0598918f6f5476cb90365651e8a2724ef26f949290bbf196f41ed96092a52f",
  "vout": 0,
  "amount": 3.9
}
{
  "txid": "3470e5fe08633583d136b9cd49bb1a224c9d9313a0b4584fd3b7438dbdf34dbd",
  "vout": 0,
  "amount": 1.95
}
```
Vous pouvez bien sûr renommer vos nouvelles clés comme bon vous semble. Il n'y a rien de magique dans les noms originaux :
```
$ bitcoin-cli listunspent | jq -r '.[] | { tx: .txid, output: .vout, bitcoins: .amount }'
{
  "tx": "2b5f5798359e0e23e02764588166f222d4ce056419dec83c743b72aad171d708",
  "output": 1,
  "bitcoins": 0.76
}
{
  "tx": "ec0598918f6f5476cb90365651e8a2724ef26f949290bbf196f41ed96092a52f",
  "output": 0,
  "bitcoins": 3.9
}
{
  "tx": "3470e5fe08633583d136b9cd49bb1a224c9d9313a0b4584fd3b7438dbdf34dbd",
  "output": 0,
  "bitcoins": 1.95
}
```
## Utiliser JQ pour accéder aux objets JSON par recherche de valeur

**Exemple d'utilisation:** _Rechercher automatiquement les UTXOs utilisés dans une transaction._

Jusqu'à présent, les recherches JQ sont assez simples : vous utilisez une clé pour rechercher une ou plusieurs valeurs dans un objet ou un tableau JSON. Mais que faire si vous voulez plutôt rechercher une valeur dans un objet JSON ... à partir d'une autre valeur ? Ce type de recherche indirecte peut s'appliquer à des transactions basées sur des UTXOs existants. Par exemple, elle peut vous permettre de calculer la valeur totale des UTXOs utilisées dans une transaction, ce qui est d'une importance capitale.

Cet exemple utilise la transaction brute suivante. Notez qu'il s'agit d'une transaction brute plus complexe avec deux entrées et deux sorties. Nous apprendrons à les réaliser dans quelques sections ; pour l'instant, il est nécessaire de pouvoir proposer des exemples solides. Notez que contrairement à nos exemples précédents, celui-ci a deux objets dans son tableau `vin` et deux dans son tableau `vout`.

```
$ bitcoin-cli decoderawtransaction $rawtxhex
{
  "txid": "6f83a0b78c598de01915554688592da1d7a3047eacacc8a9be39f5396bf0a07e",
  "hash": "6f83a0b78c598de01915554688592da1d7a3047eacacc8a9be39f5396bf0a07e",
  "size": 160,
  "vsize": 160,
  "version": 2,
  "locktime": 0,
  "vin": [
    {
      "txid": "d261b9494eb29084f668e1abd75d331fc2d6525dd206b2f5236753b5448ca12c",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967295
    },
    {
      "txid": "c7c7f6371ec19330527325908a544bbf8401191645598301d24b54d37e209e7b",
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
      "value": 1.00000000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 cfc39be7ea3337c450a0c77a839ad0e160739058 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914cfc39be7ea3337c450a0c77a839ad0e16073905888ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mzTWVv2QSgBNqXx7RC56zEhaQPve8C8VS9"
        ]
      }
    },
    {
      "value": 0.04500000,
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

### Récupérer la(les) valeur(s)

Supposons que nous sachions exactement comment cette transaction est construite : nous savons qu'elle utilise deux UTXOs comme entrée. Pour récupérer le txid des deux UTXOs, nous pourrions utiliser `jq` pour rechercher la valeur .vin de la transaction, puis faire référence au 0ème tableau de .vin, puis à la valeur .txid de ce tableau. Ensuite, nous pourrions faire la même chose avec le 1er tableau, puis avec les deux valeurs .vout du fichier .vin. C'est facile :

```
$ usedtxid1=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[0] | .txid')
$ echo $usedtxid1
d261b9494eb29084f668e1abd75d331fc2d6525dd206b2f5236753b5448ca12c
$ usedtxid2=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[1] | .txid')
$ echo $usedtxid2
c7c7f6371ec19330527325908a544bbf8401191645598301d24b54d37e209e7b

$ usedvout1=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[0] | .vout')
$ echo $usedvout1
1
$ usedvout2=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[1] | .vout')
$ echo $usedvout2
1
```
Cependant, il serait préférable d'avoir un cas général qui sauve _automatiquement_ tous les txids de nos UTXOs.

Nous savons déjà que nous pouvons accéder à tous les .txids en utilisant une valeur de tableau . Nous pouvons l'utiliser pour construire une recherche générale des .txids :

```
$ usedtxid=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .txid'))
$ echo ${usedtxid[0]}
d261b9494eb29084f668e1abd75d331fc2d6525dd206b2f5236753b5448ca12c
$ echo ${usedtxid[1]}
c7c7f6371ec19330527325908a544bbf8401191645598301d24b54d37e209e7b

$ usedvout=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .vout'))
$ echo ${usedvout[0]}
1
$ echo ${usedvout[1]}
1
```
La seule véritable astuce ici est la façon dont nous avons sauvegardé les informations en utilisant le shell bash. Plutôt que de sauvegarder dans une variable avec `$(command)`, nous avons sauvegardé dans un tableau avec `($(command))`. Nous avons alors pu accéder aux éléments individuels du tableau bash avec la syntaxe `${variable[n]}`. Nous avons pu accéder à l'ensemble du tableau avec `${variable[@]}`. (Oui, personne n'a jamais dit que bash était nécessairement joli).

> :warning: **AVERTISSEMENT:** Rappelez-vous toujours qu'un UTXO est une transaction _plus_ une vout. Nous avons manqué la vout la première fois que nous avons écrit cet exemple de JQ, et cela a cessé de fonctionner lorsque nous nous sommes retrouvés dans une situation où l'on nous avait envoyé deux `vouts` de la même transaction.

### Récupérer le(s) objet(s) lié(s)

Vous pouvez maintenant utiliser vos informations `txid` et `vout` sauvegardées pour référencer les UTXOs dans `listunspent`. Pour trouver les informations sur les UTXOs utilisées par la transaction brute, vous devez consulter l'ensemble du tableau JSON (`[]`) des transactions non dépensées. Vous pouvez ensuite choisir (`select`) les objets JSON individuels qui comprennent (`constains`) les txids. _Ensuite_, vous sélectionnez (`select`) les transactions parmi celles qui contiennent _également_ (`contain`) la vout correcte.

L'utilisation d'un autre niveau de pipe est la méthodologie standard de JQ : vous saisissez un ensemble de données, puis vous le réduisez à toutes les transactions pertinentes, puis vous le réduisez aux vouts qui ont été effectivement utilisés à partir de ces transactions. Cependant, les arguments `select` et `contain` sont quelque chose de nouveau. Ils mettent en évidence une partie de la complexité de JSON qui dépasse le cadre de ce tutoriel ; pour l'instant, sachez simplement que cette invocation particulière fonctionnera pour saisir les objets correspondants.

Pour commencer, il suffit de sélectionner les deux UTXOs un par un :

```
$ bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${usedtxid[0]}'")) | select(.vout | contains('${usedvout[0]}'))'
{
  "txid": "d261b9494eb29084f668e1abd75d331fc2d6525dd206b2f5236753b5448ca12c",
  "vout": 1,
  "address": "miSrC3FvkPPZgqqvCiQycq7io7wTSVsAFH",
  "scriptPubKey": "76a91420219e4f3c6bc0f6524d538009e980091b3613e888ac",
  "amount": 0.9,
  "confirmations": 6,
  "spendable": true,
  "solvable": true
}
$ bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${usedtxid[1]}'")) | select(.vout | contains('${usedvout[1]}'))'
{
  "txid": "c7c7f6371ec19330527325908a544bbf8401191645598301d24b54d37e209e7b",
  "vout": 1,
  "address": "mzizSuAy8aL1ytFijds7pm4MuDPx5aYH5Q",
  "scriptPubKey": "76a914d2b12da30320e81f2dfa416c5d9499d08f778f9888ac",
  "amount": 0.4,
  "confirmations": 5,
  "spendable": true,
  "solvable": true
}
```
Une simple boucle bash `for-loop` pourrait également vous donner _tous_ vos UTXOs :

```
$ for ((i=0; i<${#usedtxid[*]}; i++)); do txid=${usedtxid[i]}; vout=${usedvout[i]}; bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${txid}'")) | select(.vout | contains('$vout'))'; done;
{
  "txid": "d261b9494eb29084f668e1abd75d331fc2d6525dd206b2f5236753b5448ca12c",
  "vout": 1,
  "address": "miSrC3FvkPPZgqqvCiQycq7io7wTSVsAFH",
  "scriptPubKey": "76a91420219e4f3c6bc0f6524d538009e980091b3613e888ac",
  "amount": 0.9,
  "confirmations": 7,
  "spendable": true,
  "solvable": true
}
{
  "txid": "c7c7f6371ec19330527325908a544bbf8401191645598301d24b54d37e209e7b",
  "vout": 1,
  "address": "mzizSuAy8aL1ytFijds7pm4MuDPx5aYH5Q",
  "scriptPubKey": "76a914d2b12da30320e81f2dfa416c5d9499d08f778f9888ac",
  "amount": 0.4,
  "confirmations": 6,
  "spendable": true,
  "solvable": true
}

```
Notez que nous avons encore utilisé une autre forme de la laideur du tableau `${#usedtxid[*]}` pour déterminer la taille du tableau, puis nous avons accédé à chaque valeur du tableau `usedtxid` et à chaque valeur du tableau parallèle `usedvout`, en les plaçant dans des variables plus simples pour un accès moins pénible.

## Utilisez JSON pour un calcul simple par valeur

**Exemple d'utilisation:** _Calculer automatiquement la valeur des UTXOs utilisés dans une transaction._

Vous pouvez maintenant aller plus loin et demander le .amount (ou toute autre valeur de la clé JSON) des UTXOs que vous récupérez.

Cet exemple réutilise les tableaux `$usedtxid` et `$usedvout` qui ont été définis comme suit :

```
$ usedtxid=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .txid'))
$ usedvout=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .vout'))
```
Le même script `for` peut être utilisé pour parcourir ces tableaux, mais avec un pipe supplémentaire dans le JQ qui fournit la valeur `amount` pour chacune des UTXOs sélectionnées.

```
$ for ((i=0; i<${#usedtxid[*]}; i++)); do txid=${usedtxid[i]}; vout=${usedvout[i]}; bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${txid}'")) | select(.vout | contains('$vout')) | .amount'; done;
0.9
0.4
```
À ce stade, vous pouvez également additionner les .amounts avec un script awk, pour voir combien d'argent se trouve réellement dans les UTXOs que la transaction dépense :

```
$ for ((i=0; i<${#usedtxid[*]}; i++)); do txid=${usedtxid[i]}; vout=${usedvout[i]}; bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${txid}'")) | select(.vout | contains('$vout')) | .amount'; done | awk '{s+=$1} END {print s}'
1.3
```
Ouf !

## Utiliser JQ pour les calculs complexes

**Exemple d'utilisation:** _Calculer les frais pour une transaction._

Pour déterminer le montant total des frais de transaction à ce stade, il suffit de faire un autre calcul : déterminer combien d'argent passe par le .vout. Il s'agit d'une utilisation simple de JQ où vous utilisez simplement `awk` pour ajouter la `value` de toutes les informations de `vout` :

```
$ bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vout  [] | .value' | awk '{s+=$1} END {print s}'
1.045
```
Pour calculer les frais de transaction, vous soustrayez le montant .vout (1,045) du montant .vin (1,3).

En rassemblant tous ces éléments, vous obtenez une calculatrice complète en seulement cinq lignes de script :

```
$ usedtxid=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .txid'))
$ usedvout=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .vout'))
$ btcin=$(for ((i=0; i<${#usedtxid[*]}; i++)); do txid=${usedtxid[i]}; vout=${usedvout[i]}; bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${txid}'")) | select(.vout | contains('$vout')) | .amount'; done | awk '{s+=$1} END {print s}')
$ btcout=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vout  [] | .value' | awk '{s+=$1} END {print s}')
$ echo "$btcin-$btcout"| /usr/bin/bc
.255
```
Et c'est aussi un bon exemple de la raison pour laquelle vous deviez vérifier vos frais : nous avions l'intention d'envoyer des frais de transaction de 5 000 satoshis, mais nous avons envoyé 255 000 satoshis à la place. Oups !

> :warning: **AVERTISSEMENT:** La première fois que nous avons rédigé cette leçon, nous avons réellement mal calculé nos frais et nous ne l'avons pas vu avant d'avoir utilisé notre calculateur de frais. C'est aussi _simple_ que cela, et votre argent disparaît. (L'exemple ci-dessus est en fait tiré de notre deuxième itération du calculateur, et cette fois nous avons fait l'erreur exprès).

Pour plus de magie JSON (et si tout cela n'est pas clair), veuillez lire le [JSON Manual](https://stedolan.github.io/jq/manual/) et le [JSON Cookbook](https://github.com/stedolan/jq/wiki/Cookbook). Nous utiliserons régulièrement JQ dans nos futurs exemples.

## Créer de nouveaux alias

Le code JQ peut être un peu lourd, vous devriez donc envisager d'ajouter des commandes plus longues et plus intéressantes à votre ~/.bash_profile. 

Chaque fois que vous recherchez une grande masse d'informations dans un objet JSON produit par une commande `bitcoin-cli`, pensez à écrire un alias pour le réduire à ce que vous voulez voir.

```
alias btcunspent="bitcoin-cli listunspent | jq -r '.[] | { txid: .txid, vout: .vout, amount: .amount }'"
```
## Résumé : Utilisation de JQ

JQ permet d'extraire facilement des informations des tableaux et objets JSON. Il peut également être utilisé dans des scripts shell pour des calculs assez complexes qui vous faciliteront la vie.

## Quelle est la prochaine étape ?

Continuez "Envoyer des transactions Bitcoin" avec le §4.3 [Création d'une transaction brute avec des arguments nommés](04_3_Creation_d_une_transaction_brute_avec_des_arguments_nommes.md).

## Annexe : Le script de calcul des frais de transaction

Le script suivant exécute le "Calcul des frais" à partir de l'exemple ci-dessus.

> :warning: **AVERTISSEMENT:** Ce scénario n'a pas été vérifié de manière approfondie. Si vous comptez l'utiliser pour vérifier les frais de transaction réels, vous ne devriez le faire qu'après une triple vérification et refait tous les calculs vous-même.

```
file: txfee-calc.sh
#!/bin/bash

if [ -z $1 ];
then
    echo "You must include the raw transaction hex as an argument.";
    exit;
fi

usedtxid=($(bitcoin-cli decoderawtransaction $1 | jq -r '.vin | .[] | .txid'))
usedvout=($(bitcoin-cli decoderawtransaction $1 | jq -r '.vin | .[] | .vout'))
btcin=$(for ((i=0; i<${#usedtxid[*]}; i++)); do txid=${usedtxid[i]}; vout=${usedvout[i]}; bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${txid}'")) | select(.vout | contains('$vout')) | .amount'; done | awk '{s+=$1} END {print s}')
btcout=$(bitcoin-cli decoderawtransaction $1 | jq -r '.vout  [] | .value' | awk '{s+=$1} END {print s}')
echo "$btcin-$btcout"| /usr/bin/bc
```
Assurez-vous que les autorisations sur le script sont correctes :
```
$ chmod 755 txfee-calc.sh
```
Vous pouvez ensuite exécuter le script comme suit :
```
$ ./txfee-calc.sh $rawtxhex
.255
```
Vous pouvez également créer un alias :
```
alias btctxfee="~/txfee-calc.sh"
```
