# 3.3: Configurer votre portefeuille

> :information_source: **NOTE:** Ce texte est encore un brouillon, afin que je puisse obtenir un retour d'information de la part des premiers examinateurs. Il n'est pas encore prêt pour l'apprentissage.

Vous êtes maintenant prêt à commencer à travailler avec Bitcoin. Pour commencer, vous devrez créer une adresse pour recevoir des fonds.

## Créer une adresse

La première chose à faire est de créer une adresse pour recevoir les paiements. Cela se fait avec la commande `bitcoin-cli getnewaddress`. N'oubliez pas que si vous souhaitez obtenir plus d'informations sur cette commande, vous pouvez taper `bitcoin-cli help getnewaddress`.

```
$ bitcoin-cli getnewaddress "" legacy
n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf
```
Notez que cette adresse commence par un "n" (ou parfois un "m"). Cela signifie qu'il s'agit d'une adresse testnet.

L'option "legacy" est nécessaire pour générer une adresse traditionnelle, plutôt qu'une adresse p2sh-segwit ou bech32. L'adresse "legacy" est actuellement requise à partir de la ligne de commande pour s'assurer que la signature fonctionne correctement.

> :link: **TESTNET vs MAINNET:** L'adresse équivalente pour le réseau principal commencerait par un 1.

Prenez soigneusement note de l'adresse. Vous devrez la donner à la personne qui vous enverra des fonds.

_Qu'est-ce qu'une adresse Bitcoin?_ Une adresse Bitcoin est littéralement l'endroit où vous recevez de l'argent. C'est comme une adresse électronique, mais pour les fonds. Toutefois, contrairement à une adresse électronique, une adresse Bitcoin doit être considérée comme à usage unique : utilisez-la pour recevoir des fonds _une seule fois_. Lorsque vous souhaitez recevoir des fonds de quelqu'un d'autre ou à un autre moment, générez une nouvelle adresse. Cela est suggéré en grande partie pour améliorer la protection de votre vie privée. Toute la chaîne de blocs est immuable, ce qui signifie que les explorateurs peuvent examiner de longues chaînes de transactions au fil du temps, ce qui permet de déterminer statistiquement qui vous êtes et qui sont vos contacts, même si vous êtes prudent. Si vous réutilisez toujours la même adresse, cela devient encore plus facile.

_Qu'est-ce qu'une adresse P2PKH?_ Une adresse Bitcoin, c'est aussi autre chose : une clé publique (ou plus précisément, le hachage en 160 bits d'une clé publique). C'est pourquoi on appelle cela une adresse "Pay to PubKey Hash" (ou P2PKH). Cette clé publique de votre paire de clés vous permet de recevoir de l'argent, tandis qu'une clé privée associée vous permet de dépenser cet argent. Ceci dit, les bitcoins peuvent être envoyés à d'autres types d'adresses : Les adresses de type "Pay to Script Hash" (P2SH) figurent en bonne place dans la dernière partie de ce tutoriel.

_Qu'est-ce qu'un portefeuille Bitcoin?_ En créant votre première adresse Bitcoin, vous avez également commencé à remplir votre portefeuille Bitcoin (_NDT : wallet_). Plus précisément, vous avez commencé à remplir le fichier `wallet.dat` dans votre répertoire ~/.bitcoin/testnet3. Le fichier `wallet.dat` contient des données sur les préférences et les transactions, mais surtout il contient toutes les paires de clés que vous créez : la clé publique (qui est la source de l'adresse où vous recevez des fonds) et la clé privée (qui est la façon dont vous dépensez ces fonds). Dans la plupart des cas, vous n'aurez pas à vous préoccuper de cette clé privée : bitcoind l'utilisera quand elle sera nécessaire. Cependant, cela rend le fichier wallet.dat extrêmement important : si vous le perdez, vous perdez vos clés privées, et si vous perdez vos clés privées, vous perdez vos fonds !

Avec une seule adresse en main, vous pourriez passer directement à la section suivante et commencer à recevoir des fonds. Cependant, avant d'en arriver là, nous allons parler de quelques autres commandes concernant le portefeuille que vous pourriez vouloir utiliser à l'avenir.

## Facultatif : Signer un message

Parfois, vous devrez prouver que vous contrôlez une adresse Bitcoin (ou plutôt, que vous contrôlez sa clé privée). C'est important car cela permet aux gens de savoir qu'ils envoient des fonds à la bonne personne. Cela peut être fait en créant une signature avec la commande `bitcoin-cli signmessage`, sous la forme `bitcoin-cli signmessage [adresse] [message]`. Par exemple :

```
$ bitcoin-cli signmessage "n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf" "Hello, World"
H3yMBZaFeSmG2HgnH38dImzZAwAQADcOiMKTC1fryoV6Y93BelqzDMTCqNcFoik86E8qHa6o3FCmTsxWD7Wa5YY=
```
Vous obtiendrez la signature en retour.

_Qu'est-ce qu'une signature ?_ Une signature numérique est une combinaison d'un message et d'une clé privée qui peut ensuite être déverrouillée avec une clé publique. Comme il existe une bijection entre les éléments d'une paire de clés, le déverrouillage avec une clé publique prouve que le signataire contrôle la clé privée correspondante.

Une autre personne peut alors utiliser la commande `verifymessage` de `bitcoin-cli` pour vérifier la signature. Elle entre l'adresse en question, la signature et le message :

```
$ bitcoin-cli verifymessage "n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf" "H3yMBZaFeSmG2HgnH38dImzZAwAQADcOiMKTC1fryoV6Y93BelqzDMTCqNcFoik86E8qHa6o3FCmTsxWD7Wa5YY=" "Hello, World"
true
```
Si tout correspond, l'autre personne sait qu'elle peut transférer des fonds en toute sécurité à la personne qui a signé le message envoyé.

Si un chapeau noir inventait des signatures, cela produirait plutôt un résultat négatif :

```
$ bitcoin-cli verifymessage "n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf" "FAKEBZaFeSmG2HgnH38dImzZAwAQADcOiMKTC1fryoV6Y93BelqzDMTCqNcFoik86E8qHa6o3FCmTsxWD7Wa5YY=" "Hello, World"
false
```

## Facultatif : Videz votre portefeuille

Il peut sembler dangereux d'avoir toutes vos clés privées irremplaçables dans un seul fichier. C'est à cela que sert le portefeuille de sauvegarde (`backupwallet`) de `bitcoin-cli`. Il vous permet de faire une copie de votre wallet.dat :

```
$ bitcoin-cli backupwallet backup.dat
```
You can then recover it with `bitcoin-cli importwallet`.
```
$ bitcoin-cli importwallet backup.dat
```

## Facultatif : Voir vos clés privées

Parfois, vous pourriez avoir envie de regarder les clés privées associées à vos adresses Bitcoin. Peut-être souhaitez-vous pouvoir signer un message ou dépenser des bitcoins à partir d'une autre machine. Peut-être souhaitez-vous simplement sauvegarder certaines clés privées importantes.

Pour consulter _toutes_ les clés de votre portefeuille, tapez `bitcoin-cli dumpwallet ~/mywallet.txt`.

```
$ bitcoin-cli dumpwallet ~/mywallet.txt
{
  "filename": "/home/user1/mywallet.txt"
}
```
Cela créera un fichier mywallet.txt dans votre répertoire personnel avec une longue liste de clés privées, d'adresses et d'autres informations. Attention, vous ne devriez jamais mettre ces données dans un fichier en texte brut sur une installation Bitcoin avec de vrais fonds !

Il est plus probable que vous souhaitiez simplement examiner la clé privée associée à une adresse spécifique. Cela peut être fait avec la commande `bitcoin-cli dumpprivkey`.

```
$ bitcoin-cli dumpprivkey "n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf"
cW4s4MdW7BkUmqiKgYzSJdmvnzq8QDrf6gszPMC7eLmfcdoRHtHh
```
Vous pouvez ensuite conserver cette clé dans un endroit sûr, de préférence un endroit non connecté à l'internet.

Vous pouvez importer n'importe quelle clé privée, à partir d'une décharge (_NDT : dump_) de portefeuille ou d'une décharge de clés individuelles, comme suit :

```
$ bitcoin-cli importprivkey cW4s4MdW7BkUmqiKgYzSJdmvnzq8QDrf6gszPMC7eLmfcdoRHtHh
```
Attendez-vous à ce que cela prenne un certain temps, car `bitcoind` doit relire toutes les transactions passées, pour voir s'il y en a de nouvelles auxquelles il doit prêter attention.

> :information_source: **NOTE:** De nombreux portefeuilles modernes préfèrent les [mnémoniques](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki) pour générer les graines nécessaires à la création des clés privées. Cette méthodologie n'est pas utilisée par `bitcoin-cli`, vous ne pourrez donc pas générer de listes de mots pratiques pour vous souvenir de vos clés privées.

_Vous avez tapé souvent cette adresse Bitcoin que vous avez générée_, pendant que vous signiez des messages et que vous déverrouillez des clés. Si vous pensez que c'est pénible, nous sommes d'accord. Elle est également sujette à des erreurs, un sujet que nous aborderons dans la section suivante.

## Résumé : Configurer votre portefeuille

Vous devez créer une adresse pour recevoir des fonds. Votre adresse est stockée dans un portefeuille, que vous pouvez sauvegarder. Vous pouvez également faire bien d'autres choses avec une adresse, par exemple en conservant sa clé privée ou en l'utilisant pour signer des messages. Mais en fait, il vous suffit de créer cette adresse pour recevoir des fonds Bitcoin.

## Quelle est la prochaine étape ?

Prenez du recul par rapport à "Comprendre votre configuration Bitcoin" avec [Interlude : Utilisation des variables de la ligne de commande](03_3__Interlude_Utilisation_des_variables_de_la_ligne_de_commande.md).