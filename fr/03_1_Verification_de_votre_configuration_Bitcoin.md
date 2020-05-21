# 3.1: Vérification de votre configuration Bitcoin

> :information_source: **NOTE:** Il s'agit d'un projet en cours, afin que je puisse obtenir un retour d'information de la part des premiers examinateurs. Il n'est pas encore prêt pour l'apprentissage.

Avant de commencer à jouer avec Bitcoin, vous devez vous assurer que tout est correctement configuré.

## Vérifiez vos alias

Les documents de configuration de Bitcoin suggèrent de créer un ensemble d'alias. Si vous n'avez pas consulté ces documents, vous pouvez les créer pour votre utilisateur principal de Bitcoin avec la commande suivante :

```
cat >> ~/.bash_profile <<EOF
alias btcdir="cd ~/.bitcoin/" #linux default bitcoind path
alias bc="bitcoin-cli"
alias bd="bitcoind"
alias btcinfo='bitcoin-cli getwalletinfo | egrep "\"balance\""; bitcoin-cli getnetworkinfo | egrep "\"version\"|connections"; bitcoin-cli getmininginfo | egrep "\"blocks\"|errors"'
alias btcblock="echo \\\`bitcoin-cli getblockcount 2>&1\\\`/\\\`wget -O - http://blockexplorer.com/testnet/q/getblockcount 2> /dev/null | cut -d : -f2 | rev | cut -c 2- | rev\\\`"
EOF
```

> :warning: **AVERTISSEMENT:** L'alias btcblock ne fonctionnera pas correctement si vous essayez de le placer dans votre .bash_profile à la main, plutôt que d'utiliser la commande "cat" comme cela est suggéré. Si vous voulez le saisir à la main, vous devez ajuster le nombre de barres obliques inverses (généralement de trois à une chacune), alors assurez-vous de savoir ce que vous faites si vous ne saisissez pas les commandes exactement comme suggéré.

Notez que ces alias incluent des raccourcis pour exécuter `bitcoin-cli`, pour exécuter `bitcoind`, et pour aller dans le répertoire Bitcoin. Ces alias sont principalement destinés à vous faciliter la vie. Nous vous suggérons de créer d'autres alias pour faciliter votre utilisation des commandes (et arguments) fréquentes et pour minimiser les erreurs. Les alias de ce type peuvent être encore plus utiles si vous avez une configuration complexe où vous exécutez régulièrement des commandes associées à Mainnet, à Testnet _et_ à Regtest, comme expliqué plus loin.

Cela dit, l'utilisation de ces alias dans _ce_ document pourrait accidentellement obscurcir les leçons essentielles enseignées sur Bitcoin, c'est pourquoi les seuls alias directement utilisés ici sont `btcinfo` et `btcblock`, car ils encapsulent des commandes beaucoup plus longues et plus complexes. Sinon, nous montrons les commandes complètes ; adaptez-les à votre propre usage.

> :link: **TESTNET vs MAINNET:** N'oubliez pas que ce tutoriel suppose généralement que vous utilisez testnet. L'alias btcblock doit être légèrement différent sur le réseau principal, où vous pouvez utiliser le plus simple "wget -O - http://blockchain.info/q/getblockcount 2>/dev/null".

## Exécutez Bitcoind

Vous commencerez votre exploration du réseau Bitcoin avec la commande `bitcoin-cli`. Cependant, pour utiliser bitcoin-cli, il _faut_ que bitcoind soit en cours d'exécution, car bitcoin-cli envoie des commandes JSON-RPC au bitcoind. Si vous avez utilisé notre configuration standard, bitcoind devrait déjà être opérationnel. Vous pouvez le vérifier en consultant le tableau des processus.

```
$ ps auxww | grep bitcoind
user1    29360 11.5 39.6 2676812 1601416 ?     SLsl Feb23 163:42 /usr/local/bin/bitcoind -daemon
```
S'il ne fonctionne pas, vous devrez exécuter "/usr/local/bin/bitcoind -daemon" à la main et le placer également dans votre crontab, comme expliqué au [§2.1 : Installation manuelle d'un VPS Bitcoin-Core](02_1_Setting_Up_a_Bitcoin-Core_VPS_by_Hand_FR.md).

## Vérifiez vos blocs

Vous devez avoir téléchargé toute la chaîne de blocs avant de commencer à jouer. Il suffit de lancer l'alias "btcblock" pour voir si tout est chargé. Vous verrez deux nombres, qui vous indiqueront combien de blocs ont été téléchargés sur le nombre total.

Si les deux chiffres ne sont pas identiques, vous devez attendre :
```
$ btcblock
973212/1090099
```
Le temps total de téléchargement peut prendre entre une heure et plusieurs heures, en fonction de votre configuration.

Si les deux numéros sont les mêmes, vous avez tout chargé :
```
$ btcblock
1090099/1090099
```
Cela signifie que vous êtes prêt !

## Facultatif : Connaissez vos types de serveur

> **TESTNET vs MAINNET:** Lorsque vous configurez votre nœud, vous choisissez de le créer comme un nœud Mainnet, Testnet ou Regtest. Bien que ce document suppose une configuration testnet, il est utile de comprendre comment vous pouvez accéder et utiliser les autres types de configuration - même sur la même machine ! Mais si vous êtes un nouvel utilisateur, passez votre chemin, car ce n'est pas nécessaire pour une configuration de base.

Le type de configuration est principalement contrôlé par le fichier ~/.bitcoin/bitcoin.conf. Si vous utilisez testnet, il contient probablement cette ligne :
```
testnet=1
```
Si vous faites un regtest, il contient probablement cette ligne :
```
regtest=1
```
Toutefois, si vous souhaitez exécuter plusieurs types de nœuds simultanément, vous devez laisser le drapeau testnet (ou regtest) en dehors de votre fichier de configuration. Vous pouvez alors choisir si vous utilisez le réseau principal, le réseau test ou votre regtest chaque fois que vous exécutez bitcoind ou bitcoin-cli.

Voici un ensemble d'alias qui vous facilitera la tâche en créant un alias spécifique pour démarrer et arrêter le bitcoind, pour aller dans le répertoire bitcoin et pour exécuter bitcoin-cli, pour chacun des réseaux principaux (qui n'ont pas de drapeaux supplémentaires), testnet (qui est -testnet) ou votre regtest (qui est -regtest).
```
alias bcstart="bitcoind -daemon"
alias btstart="bitcoind -testnet -daemon"
alias brstart="bitcoind -regtest -daemon"

alias bcstop="bitcoin-cli stop"
alias btstop="bitcoin-cli -testnet stop"
alias brstop="bitcoin-cli -regtest -stop"

alias bcdir="cd ~/.bitcoin/" #linux default bitcoin path
alias btdir="cd ~/.bitcoin/testnet" #linux default bitcoin testnet path
alias brdir="cd ~/.bitcoin/regtest" #linux default bitcoin regtest path

alias bc="bitcoin-cli"
alias bt="bitcoin-cli -testnet"
alias br="bitcoin-cli -regtest"
```
Pour encore plus de complexité, vous pouvez demander à chacun de vos alias de "démarrage" d'utiliser le drapeau -conf pour charger la configuration à partir d'un fichier différent. Cela dépasse largement le cadre de ce tutoriel, mais nous le proposons comme point de départ lorsque vos explorations sur le Bitcoin atteindront le niveau supérieur.

## Résumé : Vérification de la configuration de votre Bitcoin

Avant de commencer à jouer avec des bitcoins, vous devez vous assurer que vos alias sont configurés, que votre bitcoind est en cours d'exécution et que vos blocs sont téléchargés. Si vous êtes un utilisateur avancé, vous pouvez également configurer un accès à d'autres configurations de bitcoin.

## Quelle est la prochaine étape ?

Continuez "Comprendre votre configuration Bitcoin" avec le §3.2 : [Connaître votre configuration Bitcoin](03_2_Connaitre_votre_configuration_Bitcoin.md).

