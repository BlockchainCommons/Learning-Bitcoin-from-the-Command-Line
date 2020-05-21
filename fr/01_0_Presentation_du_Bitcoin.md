# Chapitre 1 : Présentation du Bitcoin

Avant de pouvoir commencer à programmer Bitcoin (et Lightning), vous devez avoir une compréhension de base de ce qu'ils sont et de leur fonctionnement. Le présent chapitre vous donne un aperçu de ces notions. De nombreuses autres définitions apparaîtront dans le document lui-même ; elles ne visent qu'à jeter les bases.

## Objectifs de ce chapitre

Après avoir parcouru ce chapitre, un développeur sera en mesure de faire ceci :

- Évaluer les parties du système Bitcoin


- Décider de l'utilité d'une chaîne de blocs


Les objectifs de ce cours sont de permettre de :

- Comprendre ce qu'est Bitcoin

- Comprendre ce que sont les transactions dans la chaîne de blocs (Blockchain)

- Comprendre ce qu'est une chaîne de blocs

- Comprendre ce qu'est Lightning

## A propos du Bitcoin

Bitcoin est un système programmatique qui permet le transfert de la monnaie bitcoin. Il est activé par un système décentralisé de nœuds pairs-à-pair (ndt : peer-to-peer), qui comprend des nœuds complets, des portefeuilles et des mineurs. En travaillant ensemble, ils s'assurent que les transactions de bitcoin sont rapides et non répudiables. Grâce à la nature décentralisée du système, ces transactions sont également résistantes à la censure et peuvent offrir d'autres avantages tels que le "pseudonymat" et la non-corrélation si elles sont bien utilisées.

Evidemment, Bitcoin est le cœur de ce livre, mais il est également à l'origine de nombreux autres systèmes, notamment les blockchains et Lightning, qui sont tous deux détaillés dans ce tutoriel, et de nombreuses autres crypto-monnaies telles que Ethereum et Litecoins, qui ne le sont pas. 

**_Comment les pièces sont-elles transférées ?_** Les pièces de monnaie ne sont pas des pièces physiques. Il s'agit plutôt d'une série ininterrompue de transferts de propriété. Lorsqu'une personne envoie des pièces à une autre, ce transfert est enregistré comme une transaction. C'est la transaction qui enregistre réellement la propriété de l'argent, et non pas un quelconque jeton local au portefeuille du propriétaire ou à sa machine.

**_À qui pouvez-vous envoyer des pièces ?_** La grande majorité des transactions de bitcoin implique l'envoi de pièces à des personnes individuelles (ou au moins à des adresses de bitcoin individuelles). Cependant, des méthodes plus complexes peuvent être utilisées pour envoyer des bitcoins à des groupes de personnes ou à des scripts. Ces diverses méthodologies portent des noms tels que P2PKH, multisig et P2SH.

**_Comment les transactions sont-elles stockées ?_** Les transactions sont regroupées en blocs de données plus importants, qui sont ensuite inscrits dans le livre de la chaîne de blocs. Un bloc est construit de telle manière qu'il ne peut être remplacé ou réécrit une fois que plusieurs blocs ont été construits après lui. C'est ce qui rend les bitcoins non répudiables : le grand livre global décentralisé où tout est enregistré est en fait une base de données permanente et inaltérable.

Cependant, le processus de construction de ces blocs est stochastique : il est quelque peu aléatoire, de sorte que vous ne pouvez jamais être assuré qu'une transaction sera placée dans un bloc spécifique. Il peut également y avoir des changements dans les blocs s'ils sont très récents, mais seulement s'ils sont très récents. Ainsi, les choses deviennent non répudiables (et permanentes et immuables) après un certain temps.

**_Comment les transactions sont-elles protégées ?_** Les fonds contenus dans une transaction Bitcoin sont verrouillés à l'aide d'un puzzle cryptographique. Ces puzzles sont conçus de manière à pouvoir être facilement résolus par la personne à qui les fonds ont été envoyés. Cela se fait grâce à la puissance de la cryptographie à clé publique. 

Les fonds sont en outre protégés par l'utilisation de hachages. Les clés publiques ne sont pas réellement stockées dans la chaîne de blocs, seuls les hachages de clés publiques le sont. Cela signifie que même si l'ordinateur quantique venait à apparaître, les transactions Bitcoin resteraient protégées par ce deuxième niveau de cryptographie.

**_Comment les transactions sont-elles créées ?_** Le cœur de chaque transaction Bitcoin est un langage de script de type FORTH qui est utilisé pour verrouiller la transaction. Pour redépenser l'argent, le bénéficiaire fournit des informations spécifiques au script qui prouvent qu'il est le destinataire prévu.

Toutefois, ces scripts Bitcoin constituent le niveau le plus bas de fonctionnalité de Bitcoin. Une grande partie du travail de Bitcoin est effectuée par le démon `bitcoind` , qui est contrôlé par des commandes RPC. De nombreuses personnes envoient ces commandes RPC par le biais du programme `bitcoin-cli`, qui fournit une interface encore plus simple. Les non-programmeurs ne se soucient même pas de ces détails, mais utilisent plutôt des portefeuilles programmés avec des interfaces plus simples.

### Bitcoin - En résumé

On peut considérer Bitcoin comme une _séquence de transactions atomiques_. Chaque transaction est authentifiée par un expéditeur avec la solution d'un puzzle cryptographique précédent qui a été stocké sous forme de script. La nouvelle transaction est verrouillée pour le destinataire avec un nouveau puzzle cryptographique qui est stocké sous forme de script. Chaque transaction est enregistrée dans un grand livre global immuable.

## À propos de la cryptographie à clé publique

La cryptographie à clé publique est un système mathématique permettant de protéger les données et de prouver la propriété des données grâce à une paire asymétrique de clés liées : la clé publique et la clé privée.

Elle est importante pour Bitcoin (et pour la plupart des systèmes à chaîne de blocs) car elle est à la base d'une grande partie de la cryptographie qui protège les fonds cryptomonétaires. Une transaction Bitcoin est généralement envoyée à une adresse qui est une clé publique hachée. Le destinataire est alors en mesure de récupérer l'argent en révélant à la fois la clé publique et la clé privée.

**_Qu'est-ce qu'une clé publique_** ? Une clé publique est la clé donnée à d'autres personnes. Dans un système de clé publique typique, un utilisateur génère une clé publique et une clé privée, puis il donne la clé publique à tout le monde. Ces destinataires peuvent crypter les informations avec la clé publique, mais elles ne peuvent pas être décryptées avec la même clé publique en raison de l'asymétrie de la paire de clés.

**_Qu'est-ce qu'une clé privée_** ? Une clé privée est liée à une clé publique dans une paire de clés. Dans un système à clé publique typique, un utilisateur garde sa clé privée en sécurité et l'utilise pour décrypter les messages qui ont été cryptés avec sa clé publique en cours d'envoi...

**_Qu'est-ce qu'une signature_** ? Un message (ou plus communément, un hachage d'un message) peut être signé avec une clé privée, créant ainsi une signature. Toute personne possédant la clé publique correspondante peut alors valider la signature, ce qui permet de vérifier que le signataire possède la clé privée associée à la clé publique en question. 

**_Qu'est-ce qu'une fonction de hachage_** ? Une fonction de hachage est un algorithme fréquemment utilisé en cryptographie. C'est un moyen de faire correspondre une grande quantité arbitraire de données à une petite quantité fixe de données. Les fonctions de hachage utilisées en cryptographie sont unidirectionnelles et résistantes aux collisions, ce qui signifie qu'un hachage peut être lié de manière fiable aux données d'origine, mais que les données d'origine ne peuvent pas être régénérées à partir du hachage. Les hachages permettent donc la transmission de petites quantités de données pour représenter de grandes quantités de données, ce qui peut être important pour l'efficacité et les exigences de stockage. 

Bitcoin tire profit de la capacité d'un hachage à dissimuler les données originales, ce qui permet de dissimuler la clé publique réelle d'un utilisateur, rendant les transactions résistantes à l'informatique quantique.

### Cryptographie à clé publique - En résumé

Une façon de penser à la cryptographie à clé publique est : _un moyen pour quiconque de protéger les données de telle sorte que seule une personne autorisée puisse y accéder, et que la personne autorisée puisse prouver qu'elle aura cet accès._

## A propos de l'ECC

ECC signifie cryptographie à courbe elliptique. Il s'agit d'une branche spécifique de la cryptographie à clé publique qui dépend de calculs mathématiques effectués à l'aide de courbes elliptiques définies sur des champs finis. Elle est plus complexe et plus difficile à expliquer que la cryptographie classique à clé publique (qui utilise des nombres premiers), mais elle présente de beaux avantages.

L'ECC ne reçoit pas beaucoup d'attention dans ce tutoriel. C'est parce que ce tutoriel porte sur l'intégration avec les serveurs Bitcoin Core et Lightning, qui se sont déjà chargés de la cryptographie pour vous. En fait, l'intention de ce tutoriel est que vous n'ayez pas à vous soucier du tout de la cryptographie, parce que c'est quelque chose que vous voulez vraiment que des experts s'occupent.

**_Qu'est-ce qu'une courbe elliptique ?_** Une courbe elliptique est une courbe géométrique qui prend la forme `y`<sup>`2`</sup> = `x`<sup>`3`</sup>` + ax + b`. Une courbe elliptique spécifique est choisie en sélectionnant des valeurs spécifiques de a et b. La courbe doit ensuite être soigneusement examinée pour déterminer si elle fonctionne bien pour la cryptographie. Par exemple, la courbe secp256k1 utilisée par Bitcoin est définie comme `a=0` et `b=7`.

Toute ligne qui croise une courbe elliptique le fera en 1 ou 3 points ... et c'est la base de la cryptopgraphie à courbe elliptique.

**_Que sont les champs finis ?_** Un champ fini est un ensemble fini de nombres, où toutes les additions, soustractions, multiplications et divisions sont définies de manière à ce qu'elles aboutissent à d'autres nombres également dans le même champ fini. Une façon simple de créer un champ fini est d'utiliser une fonction modulo. 

**_Comment définit-on une courbe elliptique sur un champ fini ?_** Une courbe ellipitique définie sur un champ fini a tous les points de sa courbe tirés d'un champ fini spécifique. Cela prend la forme : `y`<sup>`2`</sup> `% field-size = (x`<sup>`3`</sup>` + ax + b) % field-size` Le champ fini utilisé pour secp256k1 est `2`<sup>`256`</sup>` - 2`<sup>`32`</sup>` - 2`<sup>`9`</sup>` - 2`<sup>`8`</sup>` - 2`<sup>`7`</sup>` - 2`<sup>`6`</sup>` - 2`<sup>`4`</sup>

**_Comment les courbes elliptiques sont-elles utilisées en cryptographie ?_**  En cryptographie à courbe elliptique, l'utilisateur sélectionne un très grand nombre (256 bits) comme clé privée. Il ajoute ensuite un point de base fixe sur la courbe à lui-même autant de fois que nécessaire. (Dans la secp256k1, le point de base est `G = 04 79BE667E F9DCBBAC 55A06295 CE870B07 029BFCDB 2DCE28D9 59F2815B 16F81798 483ADA77 26A3C465 5DA4FBFC 0E1108A8 FD17B448 A6855419 9C47D08F FB10D4B8`, qui préfixe les deux parties du tuple avec un 04 pour indiquer que le point de données est sous forme non comprimée. Si vous préférez une définition géométrique droite, c'est le point "0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798,0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8") Le numéro qui en résulte est la clé publique. Diverses formules mathématiques peuvent alors être utilisées pour prouver la propriété de la clé publique, étant donné la clé privée. Comme pour toute fonction cryptographique, celle-ci est une trappe : il est facile de passer de la clé privée à la clé publique et largement impossible de passer de la clé publique à la clé privée.

Cette méthodologie particulière explique également pourquoi les champs finis sont utilisés dans les courbes elliptiques : elle garantit que la clé privée ne deviendra pas trop grande. Notez que le champ fini pour secp256k1 est légèrement inférieur à 256 bits, ce qui signifie que toutes les clés publiques auront une longueur de 256 bits, tout comme les clés privées.

### ECC - En résumé

Une façon de penser la cryptographie à clé publique est : _une façon de permettre la cryptographie à clé publique qui utilise de très petites clés et des mathématiques très obscures._

## À propos des chaînes de blocs (Blockchains)

La chaîne de blocs est la généralisation de la méthodologie utilisée par Bitcoin pour créer un grand livre mondial distribué. Bitcoin est une chaîne de blocs, comme le sont tous les nombres de pièces de monnaie alternatives, dont chacune vit dans ses réseaux et écrit dans sa propre chaîne. Les chaînes alternatives (sidechains) comme Liquid sont également des blockchains. Les blockchains n'ont même pas besoin d'avoir un rapport avec les finances. Par exemple, on a beaucoup discuté de l'utilisation des chaînes de blocs pour protéger les identités souveraines.

Bien que vous deviez comprendre les bases du fonctionnement d'une chaîne de blocs pour comprendre le fonctionnement des transactions dans Bitcoin, vous n'aurez pas besoin d'aller plus loin. Les chaînes de blocs étant devenues une vaste catégorie de technologie, ces concepts de base sont susceptibles de s'appliquer à de nombreux autres projets dans ce secteur technologique en pleine croissance. Les commandes de programmation spécifiques apprises dans ce livre ne le seront cependant pas, car elles sont assez spécifiques à Bitcoin (et Lightning). 

**_Pourquoi appelle-t-on cela une chaîne_** ? Chaque bloc de la chaîne de blocs stocke un hachage du bloc qui le précède. Cela permet de relier le bloc actuel au "bloc de genèse" d'origine par une chaîne ininterrompue. C'est un moyen de créer un ordre absolu entre des données éventuellement contradictoires. Cette méthode assure également la sécurité de la chaîne de blocs, car chaque bloc empilé au sommet d'un ancien bloc rend plus difficile la recréation de ce dernier en raison des algorithmes de preuve de travail utilisés dans la création des blocs. Une fois que plusieurs blocs ont été construits au sommet d'un bloc de la chaîne, c'est essentiellement irréversible. 

**_Qu'est-ce qu'une fourchette (fork) ?_** Il arrive que deux blocs soient créés à peu près en même temps. Cela crée temporairement une fourchette d'un seul bloc, où l'un des blocs en cours pourrait être le "vrai" bloc. De temps en temps, une fourche peut s'étendre pour devenir deux blocs, trois blocs, voire quatre blocs de long, mais assez rapidement, un côté de la fourche est déterminé comme étant le vrai, et l'autre est "orphelin". Cela fait partie du processus stochastique de création de blocs, et démontre pourquoi plusieurs blocs doivent être construits au sommet d'un bloc avant qu'il puisse être considéré comme vraiment digne de confiance et non répudiable.

### Blockchain — En résumé

On peut penser à une chaîne de blocs ainsi : _une série de blocs de données immuables, remontant dans le temps._ Une autre façon est : _une série de blocs liés qui ordonnent absolument les données qui pourraient être conflictuelles._

## Est-ce que la Blockchain me convient ?

Si vous voulez faire des transactions de bitcoins, il est évident que Bitcoin est fait pour vous. Cependant, plus largement, le terme "blockchain" est devenu un mot à la mode, même s'il ne constitue pas une solution miracle à tous les problèmes techniques. Cela dit, il existe de nombreuses situations spécifiques dans lesquelles la chaîne de blocs est une technologie supérieure.

Les chaînes de blocs _seront probablement_ utiles si :

- Les utilisateurs ne se font pas confiance.
  - Ou : les utilisateurs existent à travers diverses frontières.

- Les utilisateurs n'ont pas confiance dans les autorités centrales.
  - Et : Les utilisateurs veulent contrôler leur propre destin.
- Les utilisateurs veulent une technologie transparente.
- Les utilisateurs veulent partager quelque chose.
  - Et : Les utilisateurs veulent que ce qui est partagé soit enregistré de façon permanente.
- Les utilisateurs veulent une finalité rapide des transactions.
  - Mais : Les utilisateurs n'ont pas besoin d'une finalité de transaction instantanée.

Les chaînes de blocs _ne seront probablement pas_ utiles si :

On fait confiance aux utilisateurs :

- par exemple : les transactions ont lieu au sein d'une entreprise ou d'une organisation.
- par exemple : les transactions sont supervisées par une autorité centrale.

Le secret est exigé :

- par exemple : Les informations doivent être secrètes.
- Par exemple : Les transactions doivent être secrètes.
- Par exemple : Les transacteurs doivent être secrêts.
- À moins que : Une méthodologie pour le secret cryptographique soit soigneusement étudiée, analysée et testée.

Les utilisateurs ont besoin d'une finalité instantanée de la transaction.

- Par exemple : en moins de 10 minutes sur un réseau de type Bitcoin, en moins de 2,5 minutes sur un réseau de type Litecoin, en moins de 15 secondes sur un réseau de type Ethereum

Notez qu'il peut encore y avoir des solutions à certaines de ces situations dans l'écosystème Bitcoin. Par exemple, les canaux de paiement abordent rapidement les questions de liquidité et de finalité des paiements. 

## A propos de Lightning

Lightning est un protocole de couche 2 qui interagit avec Bitcoin pour permettre aux utilisateurs d'échanger leurs bitcoins "hors chaîne". Il présente à la fois des avantages et des inconvénients par rapport à l'utilisation de Bitcoin seul.

Lightning est également le sujet secondaire de ce tutoriel. Bien qu'il s'agisse principalement d'interagir directement avec Bitcoin (et `bitcoind`), il accorde une certaine attention à Lightning car il s'agit d'une technologie à venir qui devrait devenir une alternative populaire à Bitcoin dans un avenir proche. Ce livre adopte la même approche de Lightning que de Bitcoin : il enseigne comment interagir directement avec un démon Lightning de confiance depuis la ligne de commande.

Contrairement à Bitcoin, il existe en fait plusieurs variantes de Lightning. Ce tutoriel utilise l'implémentation standard [c-lightning](https://github.com/ElementsProject/lightning) comme serveur Lightning de confiance.

**_Qu'est-ce qu'un protocole de couche 2 ?_** Un protocole de couche 2 Bitcoin fonctionne par-dessus Bitcoin. Dans ce cas, Lightning travaille au dessus de Bitcoin, en interagissant avec lui par le biais de contrats intelligents.

**_Qu'est-ce qu'un canal Lightning ?_** Un canal Lightning est une connexion entre deux utilisateurs de Lightning. Chacun des utilisateurs verrouille un certain nombre de bitcoins sur la chaîne de blocs de Bitcoin à l'aide d'un signe multiple signé par les deux. Les deux utilisateurs peuvent ensuite échanger des bitcoins par le biais de leur canal Lightning sans jamais écrire dans la chaîne de blocs des bitcoins. Ce n'est que lorsqu'ils veulent fermer leur canal qu'ils règlent leurs bitcoins, en fonction de la division finale des pièces.

**_Qu'est-ce qu'un réseau Lightning ?_** Le réseau Lightning est le résultat de la mise en commun de plusieurs canaux Lightning. Celui-ci permet à deux utilisateurs qui n'ont pas créé de canal entre eux d'échanger des bitcoins en utilisant Lightning : le protocole forme une chaîne de canaux entre les deux utilisateurs, puis échange les pièces à travers la chaîne en utilisant des transactions verrouillées dans le temps.

**_Quels sont les avantages de Lightning ?_** Lightning permet des transactions plus rapides avec des frais moins élevés. Cela crée la possibilité réelle de micropaiements financés par bitcoin. Elle offre également une meilleure protection de la vie privée, car elle est hors chaîne, seuls les premier et dernier états de la transaction étant inscrits dans le grand livre immuable de Bitcoin. 

**_Quels sont les inconvénients de la foudre ?_** Lightning est une technologie encore très récente et n'a pas été testée de manière aussi approfondie que Bitcoin. Ce n'est pas seulement une question de mise en œuvre technologique, mais aussi de possibilité de jouer à la conception elle-même de manière inattendue.

### Lightning - En résumé

Une façon de penser à Lightning est : _une façon de transférer des bitcoins en utilisant des canaux hors chaîne entre des paires de personnes, de sorte que seuls le premier et le dernier état doit être écrit dans la chaîne de blocs._

## Résumé : Présentation de Bitcoin

Bitcoin est un système pair à pair qui permet le transfert de fonds par le biais de transactions qui sont verrouillées par des puzzles. Ces puzzles dépendent de la cryptographie à clé publique à courbe elliptique. Lorsque l'on généralise les idées qui sous-tendent Bitcoin, on obtient des chaînes de blocs, une technologie qui est actuellement en pleine expansion et innovante. Lorsque vous étendez les idées derrière Bitcoin, vous obtenez des protocoles de couche 2 tels que Lightning, qui élargissent le potentiel de la monnaie.

## Quelle est la prochaine étape ?

Avancez dans "Préparation à Bitcoin" avec le [Chapitre 2 : Mise en place d'un VPS Bitcoin-Core](02_0_Mise_en_place_d_un_VPS_Bitcoin-Core.md).
