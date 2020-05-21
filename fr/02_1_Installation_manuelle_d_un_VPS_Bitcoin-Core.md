# 2.1: Installation manuelle d'un VPS Bitcoin-Core

> :information_source: **NOTE:** Il s'agit d'un projet en cours, afin que je puisse obtenir un retour d'information de la part des premiers examinateurs. Il n'est pas encore prêt pour l'apprentissage.

Ce document explique comment mettre en place un VPS (Virtual Private Server) à la main pour faire fonctionner un nœud Bitcoin sur un ordinateur dans le cloud. Vous devrez configurer votre ordinateur vous-même, mais ce document vous fournira ensuite les commandes importantes pour sécuriser votre VPS et lui faire exécuter Bitcoin. La configuration doit être effectuée lors de ma session, afin de ne pas perdre les variables `export`ées au départ.

:warning: **AVERTISSEMENT** N'utilisez pas un VPS pour un portefeuille de bitcoin avec des fonds réels importants ; voir http://blog.thestateofme.com/2012/03/03/lessons-to-be-learned-from-the-linode-bitcoin-incident/ . Il est très agréable de pouvoir expérimenter de vraies transactions de bitcoin sur un nœud en direct sans avoir à connecter un serveur auto-hébergé sur un réseau local. J'ai également trouvé utile de pouvoir utiliser un iPhone ou un iPad pour communiquer via SSH avec mon VPS afin d'effectuer quelques tâches bitcoin simples. Mais un niveau de sécurité plus élevé est nécessaire pour les fonds importants.

Si vous souhaitez plutôt qu'un script fasse l'installation pour vous, en particulier sur linode.com, lisez le fichier HOWTO parallèle, [§2.2: Configuration d'un VPS Bitcoin-Core avec StackScript](02_2_Configuration_d_un_VPS_Bitcoin-Core_avec_StackScript.md).

Si vous avez déjà un nœud Bitcoin en cours d'exécution, lisez plutôt le prochain fichier HOWTO, chapitre trois : [Comprendre votre configuration Bitcoin](03_0_Comprendre_votre_configuration_Bitcoin.md).

## Choisissez votre configuration Bitcoin

Avant de commencer, vous devez choisir entre cinq configurations possibles de Bitcoin, comme indiqué dans l'annexe I. Nous allons faire simple : _Choisissez la configuration Testnet non élaguée_. C'est la configuration parfaite pour jouer avec Bitcoin, et nous allons supposer que c'est la configuration par défaut à partir de maintenant, bien que nous aurons quelques encadrés où nous expliquerons les autres options.

## Démarrez chez un fournisseur de services en ligne

Vous devez d'abord choisir un fournisseur de services en ligne. Les commandes de ce document sont dérivées du script que nous utilisons à linode.com ; elles devraient également fonctionner à digitalocean.com.

### Créez un compte

Après avoir sélectionné votre fournisseur de services en ligne, vous devrez créer un compte, si vous n'en avez pas déjà un. Si vous le souhaitez, vous pouvez utiliser les codes de parrainage suivants pour gagner environ un mois de temps gratuit :

```
Linode Referral Code: https://www.linode.com/?r=3c7fa15a78407c9a3d4aefb027539db2557b3765
Digital Ocean: http://www.digitalocean.com/?refcode=a6060686b88a
```

### Facultatif : Envisager une authentification à deux facteurs

Si votre fournisseur de _cloud computing_ propose une authentification à deux facteurs pour ses outils web, nous vous suggérons vivement de l'utiliser, afin de garantir que votre machine reste sécurisée.

### Créez une machine

Vous devez maintenant créer votre VPS Bitcoin. Pour une installation Testnet non élaguée, vous devez disposer d'au moins 2-3G de mémoire et d'au moins 19G de stockage.

:warning: **AVERTISSEMENT:** Il nous est arrivé d'avoir des machines qui manquaient de mémoire après avoir fait tourner `bitcoind` pendant quelques jours alors qu'elles n'avaient que 2G. En revenant, nous constatons que `bitcoind` s'est arrêté, laissant le message "Error : Out of memory. Terminating" dans le fichier debug.log. Cela nécessite simplement un redémarrage de `bitcoind`` et dix ou quinze minutes pour obtenir la resynchronisation de la chaîne de blocs. Soyez généreux avec la mémoire si vous voulez éviter ce désagrément, mais ne vous inquiétez pas trop si vous le rencontrez.

Nous vous suggérons également de choisir une image Debian 9 lors de la création de votre machine. Ces commandes ont toutes été testées sur Debian 9.5 (stretch). Plus vous vous en éloignez, moins il est probable que les choses fonctionnent comme décrit ici. Donc, une autre Debian est probablement correcte et peut-être une Ubuntu différente, mais nous avons testé celles-ci sur un Mac, et nous savons qu'il manque certaines commandes comme "wget". Donc, installateur, attention !

Ensuite, démarrez votre VPS.



> :link: **Comparaison TESTNET / MAINNET:** Les différentes configurations possibles nécessitent 5-19G de stockage et 2-3G de mémoire. Le réseau principal non élagué est la seule configuration qui nécessite beaucoup plus : environ 170G de stockage pour maintenir la chaîne de blocs courante. 
>
> Vous trouverez ci-dessous des suggestions concernant les exigences relatives aux machines :
>
> | Configuration | Mémoire | Stockage |
> |-------|--------|---------|
> | Mainnet | 2-3G | 170G |
> | Pruned Mainnet | 2-3G | ~5G |
> | Testnet | 2-3G | ~19G |
> | Pruned Testnet | 2-3G | ~5G |
> | Regtest | 2-3G | ~ |

## Configurez votre VPS

Vous devrez rechercher l'adresse IP de votre nouvelle machine, puis vous devriez pouvoir vous connecter en SSH :

```
$ ssh root@192.168.1.52
```

Il vous faudra maintenant procéder à une configuration standard, puis travailler à améliorer la sécurité de votre machine.

_Si vous avez déjà vos propres techniques pour configurer une machine, allez-y et suivez-les, puis passez à "Configuration d'un utilisateur", puis "Installation de Bitcoin". Sinon, continuez !_

### Configurez votre nom d'hôte

Choisissez un nom d'hôte (_hostname_) pour votre machine (par exemple "mybtc") et entrez-le dans votre fichier de nom d'hôte ; ensuite, exécutez quelques commandes pour propager cette nouvelle information :

```
$ echo "mybtc" > /etc/hostname
$ /etc/init.d/hostname.sh start
$ /bin/hostname "mybtc"
```

Saisissez les informations relatives à l'adresse IP et au nom d'hôte dans votre fichier /etc/hosts. Notez que vous devez également saisir un nom d'hôte complet, comme indiqué ci-dessous. Si vous n'intégrez pas la machine dans un domaine, il suffit de choisir un suffixe ".local".

```
$ echo "127.0.0.1    localhost" > /etc/hosts
$ echo "127.0.1.1 mybtc.local mybtc" >> /etc/hosts
```

### Mettez à jour votre fuseau horaire

Assurez-vous que votre fuseau horaire est correct.

L'exemple suivant règle votre machine sur le fuseau horaire de la côte ouest américaine :

```
$ echo "America/Los_Angeles" > /etc/timezone
$ cp /usr/share/zoneinfo/America/Los_Angeles /etc/localtime
```

## Protégez votre VPS

Bien que vous n'accordiez pas beaucoup de valeur réelle à ce serveur, vous devez néanmoins vous assurer qu'il est aussi absolument sécurisé que possible.

### Créez des règles de pare-feu

Pour commencer, créez un fichier de règles de pare-feu (_firewall_).

_Pour toutes les instructions qui ressemblent à celle-ci, vous devez simplement pouvoir couper du "cat" jusqu'à l'EOF, et tout sera placé dans le fichier approprié._

```
$ cat > /etc/iptables.firewall.rules <<EOF
*filter

#  Allow all loopback (lo0) traffic and drop all traffic to 127/8 that doesn't use lo0
-A INPUT -i lo -j ACCEPT
-A INPUT -d 127.0.0.0/8 -j REJECT

#  Accept all established inbound connections
-A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT

#  Allow all outbound traffic - you can modify this to only allow certain traffic
-A OUTPUT -j ACCEPT

# If you want HTTP and HTTPS, uncomment these

#  Allow SSH connections
#
#  The -dport number should be the same port number you set in sshd_config
#
-A INPUT -p tcp -m state --state NEW --dport 22 -j ACCEPT

#  Allow ping
-A INPUT -p icmp -j ACCEPT

# Allow Bitcoin connections
-A INPUT -p tcp --dport 22 -m conntrack --ctstate NEW,ESTABLISHED -j ACCEPT
-A INPUT -p tcp --dport 8333 -j ACCEPT
-A INPUT -p tcp --dport 18333 -j ACCEPT
-A INPUT -p tcp --dport 9735 -j ACCEPT
-A INPUT -m conntrack --ctstate ESTABLISHED -j ACCEPT

#  Log iptables denied calls
-A INPUT -m limit --limit 5/min -j LOG --log-prefix "iptables denied: " --log-level 7

#  Drop all other inbound - default deny unless explicitly allowed policy
-A INPUT -j DROP
-A FORWARD -j DROP

COMMIT
EOF
```

> :link: **Comparaison TESTNET / MAINNET:** Notez que 8333 est le port principal Bitcoin, 18333 est le port Testnet Bitcoin, et 9735 est le port Lightning. Si vous voulez être aventureux, vous pouvez supprimer celui que vous n'utilisez pas.

La plupart des travaux sont encore effectués sur les réseaux IPv4, mais les paragraphes suivants établiront les mêmes règles pour IPv6 :

```
$ cat /etc/iptables.firewall.rules | sed 's/127.0.0.0\/8/::1\/128/' > /etc/ip6tables.firewall.rules
```

Ensuite, vous devez créer un fichier qui les exécutera tous les deux au démarrage :

```
$ cat > /etc/network/if-pre-up.d/firewall <<EOF
#!/bin/sh
/sbin/iptables-restore < /etc/iptables.firewall.rules
/sbin/ip6tables-restore < /etc/ip6tables.firewall.rules
EOF

$ chmod a+x /etc/network/if-pre-up.d/firewall
```

Enfin, vous devriez immédiatement l'exécuter :

```
$ /etc/network/if-pre-up.d/firewall
```

#### Facultatif : Ajoutez d'autres règles de pare-feu

Veuillez noter que cela ne permettra l'accès à votre machine que pour les services SSH, Ping et Bitcoin. Si vous souhaitez accepter d'autres types de trafic, vous devrez ouvrir des ports supplémentaires ("dports") en utilisant la même méthodologie que celle indiquée pour la connexion au port 8333 de Bitcoin. Par exemple, pour autoriser les connexions au port 25 (mail), il faudrait ajouter les éléments suivants aux tables iptables et ip6tables :

```
-A INPUT -p tcp --dport 25 -j ACCEPT
-A INPUT -p udp --dport 25 -j ACCEPT
```

Cet exemple ouvre l'accès au port 25 pour les connexions TCP et UDP. Les autres ports populaires sont 80 (HTTP), 443 (HTTPS), 53 (DNS), 110 (POP) et 143 (IMAP).  Assurez-vous que ces modifications sont placées au-dessus des valeurs par défaut qui apparaissent à la fin des fichiers iptables.firewall.rules.

Si vous avez apporté des modifications, vous pouvez les intégrer immédiatement en exécutant à nouveau le script de pare-feu que vous avez créé.

```
$ /etc/network/if-pre-up.d/firewall
```

### Verrouillez votre SSH

Si vous connaissez votre adresse IP fixe pour votre domicile, nous vous suggérons _vivement_ de verrouiller votre SSH, afin que votre serveur ne soit accessible qu'à partir de cette IP. Vous pouvez également saisir plusieurs adresses IP si vous les séparez par une virgule.

Pour ce faire, il suffit de remplacer $YOUR_HOME_IP par votre propre adresse :

```
$ echo "sshd: $YOUR_HOME_IP" >> /etc/hosts.allow
$ echo "sshd: ALL" >> /etc/hosts.deny
```

### Mettez à jour votre distribution Debian

Une Debian à jour est une Debian sûre.

Avant d'installer Bitcoin, vous devez exécuter les commandes suivantes pour obtenir tous les derniers correctifs :

```
$ export DEBIAN_FRONTEND=noninteractive
$ apt-get update
$ apt-get upgrade -y
$ apt-get dist-upgrade -y
```
Ce processus prendra plusieurs minutes. Faites une pause. Prenez un espresso.

Ensuite, vous devez également installer un générateur de nombres aléatoires :

```
$ apt-get install haveged -y
```
Enfin, nous vous suggérons vivement de paramétrer les mises à jour pour qu'elles soient automatiques, afin de vous tenir au courant des mises à jour futures :

```
$ echo "unattended-upgrades unattended-upgrades/enable_auto_updates boolean true" | debconf-set-selections
$ apt-get -y install unattended-upgrades
```
## Mise en place d'un utilisateur

Il est toujours préférable de faire votre travail avec un utilisateur autre que root. Voici comment créer un compte utilisateur pour "user1".

```
$ /usr/sbin/useradd -m -g sudo -s /bin/bash user1
$ /usr/bin/passwd user1
$ /usr/sbin/adduser user1 sudo
```
Un mot de passe pour user1 vous sera demandé après la deuxième commande.

### Facultatif : Établissez une clé SSH

Bien que ce ne soit pas obligatoire, nous vous suggérons de copier votre clé SSH sur votre compte user1, afin de simplifier l'accès et de le rendre plus sûr.

Créez le répertoire SSH :

```
$ mkdir ~user1/.ssh
```

Ensuite, collez votre clé SSH dans ~user1/.ssh/authorized_keys .

Ensuite, donnez à user1 l'accès au répertoire :

```
$ chown -R user1 ~user1/.ssh
```
Si vous n'avez pas encore configuré une clé SSH sur votre ordinateur local, il existe de bonnes instructions à ce sujet sur [Github](https://help.github.com/articles/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent/).

## Installer Bitcoin

Vous êtes maintenant prêt à accéder à la partie de ce tutoriel consacrée aux bitcoins !

### Créer des alias Bitcoin

Nous trouvons qu'un certain nombre d'alias Bash sont utiles pour faciliter l'utilisation de Bitcoin.
```
$ sudo -u user1 cat >> ~user1/.bash_profile <<EOF
alias btcdir="cd ~/.bitcoin/" #linux default bitcoind path
alias bc="bitcoin-cli"
alias bd="bitcoind"
alias btcinfo='bitcoin-cli getwalletinfo | egrep "\"balance\""; bitcoin-cli getnetworkinginfo | egrep "\"version\"|connections"; bitcoin-cli getmininginfo | egrep "\"blocks\"|errors"'
# next alias not working; blockexplorer.com is not redirecting properly
# alias btcblock="echo \\\`bitcoin-cli getblockcount 2>&1\\\`/\\\`wget -O - http://blockexplorer.com/testnet/q/getblockcount 2> /dev/null | cut -d : -f2 | rev | cut -c 2- | rev\\\`"
EOF
```

> :link: **Comparaison TESTNET / MAINNET:** Si vous utilisez un réseau principal ou un réseau principal élagué, nous vous suggérons plutôt ce qui suit. La seule différence entre les deux se trouve dans la ligne "btcblock", qui recherche le nombre de blocs à différents endroits pour mainnet et testnet.
```
$ sudo -u user1 cat >> ~user1/.bash_profile <<EOF
alias btcdir="cd ~/.bitcoin/" #linux default bitcoind path
alias bc="bitcoin-cli"
alias bd="bitcoind"
alias btcinfo='bitcoin-cli getwalletinfo | egrep "\"balance\""; bitcoin-cli getnetworkinginfo | egrep "\"version\"|connections"; bitcoin-cli getmininginfo | egrep "\"blocks\"|errors"'
alias btcblock="echo \\\`bitcoin-cli getblockcount 2>&1\\\`/\\\`wget -O - http://blockchain.info/q/getblockcount 2>/dev/null\\\`"
EOF
```

> :warning: **AVERTISSEMENT:** L'alias btcblock ne fonctionnera pas correctement si vous essayez de le placer dans votre .bash_profile à la main, plutôt que d'utiliser la commande "cat" comme cela est suggéré. Pour le saisir à la main, vous devez ajuster le nombre de barres obliques inverses (généralement de trois à une chacune), donc assurez-vous de savoir ce que vous faites si vous ne saisissez pas les commandes exactement comme indiqué.

Comme d'habitude, donnez votre autorisation d'utilisation :
```
$ /bin/chown user1 ~user1/.bash_profile
```
Et en mettre une copie en .bashrc pour l'utiliser avec des shells interactifs sans connexion :
```
$ cp ~user1/.bash_profile ~user1/.bashrc
$ /bin/chown user1 ~user1/.bashrc
```

### Connectez-vous en tant qu'utilisateur non-privilégié

Vous voulez maintenant passer au compte utilisateur1 pour l'installation proprement dite :
```
$ su user1
$ cd
$ source ~/.bash_profile
```

### Variables de configuration

Nous suggérons de mettre en place deux variables pour rendre cette installation plus automatique.

La première variable, $BITCOIN, devrait être définie sur la version actuelle de Bitcoin. Elle était à 0.16.2 lorsque nous avons écrit ceci. La seconde générera alors automatiquement une forme tronquée utilisée par certains des fichiers.

```
$ export BITCOIN=bitcoin-core-0.16.2
$ export BITCOINPLAIN=`echo $BITCOIN | sed 's/bitcoin-core/bitcoin/'`
```

### Téléchargez les fichiers

Prenez les fichiers pertinents sur bitcoin.org :
```
$ wget https://bitcoin.org/bin/$BITCOIN/$BITCOINPLAIN-x86_64-linux-gnu.tar.gz -O ~user1/$BITCOINPLAIN-x86_64-linux-gnu.tar.gz
$ wget https://bitcoin.org/bin/$BITCOIN/SHA256SUMS.asc -O ~user1/SHA256SUMS.asc
$ wget https://bitcoin.org/laanwj-releases.asc -O ~user1/laanwj-releases.asc
```
C'est l'autre étape du processus de mise en place qui prend environ cinq minutes. Il est peut-être temps d'aller faire une belle promenade au soleil.

### Vérifiez la signature de Bitcoin

Vous voulez vous assurer que votre installation Bitcoin est authentique et n'a pas été altérée. La première façon de le faire est d'examiner la signature :
```
$ /usr/bin/gpg --import ~user1/laanwj-releases.asc
$ /usr/bin/gpg --verify ~user1/SHA256SUMS.asc
```
Parmi les informations que vous recevez en retour de la dernière commande, il devrait y avoir une ligne vous indiquant que vous avez une "bonne signature". (Ne vous inquiétez pas de l'avertissement).

### Vérifiez Bitcoin SHA

Ensuite, vous devez vérifier le hachage du fichier bitcoin tar par rapport au hachage attendu :
```
$ /usr/bin/sha256sum ~user1/$BITCOINPLAIN-x86_64-linux-gnu.tar.gz | awk '{print $1}'
$ cat ~user1/SHA256SUMS.asc | grep $BITCOINPLAIN-x86_64-linux-gnu.tar.gz | awk '{print $1}'
```
Si les deux produisent le même nombre, c'est bon.

### Installez Bitcoin

Si vos deux tests de vérification ont réussi, vous pouvez maintenant installer Bitcoin. (Si ce n'est pas le cas, vous devez commencer à examiner ce qui se passe).
```
$ /bin/tar xzf ~user1/$BITCOINPLAIN-x86_64-linux-gnu.tar.gz -C ~user1
$ sudo /usr/bin/install -m 0755 -o root -g root -t /usr/local/bin ~user1/$BITCOINPLAIN/bin/*
$ /bin/rm -rf ~user1/$BITCOINPLAIN/
```

### Créez une configuration Bitcoin

Enfin, vous devez mettre en place un fichier de configuration bitcoin.

Tout d'abord, créez le répertoire :

```
$ /bin/mkdir ~user1/.bitcoin
```
Il s'agit du fichier bitcoin.conf de base, qui convient à une installation testnet non élaguée :
```
$ cat >> ~user1/.bitcoin/bitcoin.conf << EOF
# Accept command line and JSON-RPC commands
server=1

# Set database cache size in megabytes (4 to 16384, default: 450)
dbcache=1536

# Set the number of script verification threads (-6 to 16, 0 = auto, <0 = leave that many cores free, default: 0)
par=1

# Set to blocksonly mode, sends and receives no lose transactions, instead handles only complete blocks
blocksonly=1

# Tries to keep outbound traffic under the given target (in MiB per 24h), 0 = no limit (default: 0)
maxuploadtarget=137

# Maintain at most <n> connections to peers (default: 125)
maxconnections=16

# Username for JSON-RPC connections
rpcuser=bitcoinrpc

# Password for JSON-RPC connections
rpcpassword=$(xxd -l 16 -p /dev/urandom)

# Allow JSON-RPC connections from, by default only localhost are allowed
rpcallowip=127.0.0.1

# Use the test chain
testnet=1

# Maintain a full transaction index, used by the getrawtransaction rpc call (default: 0)
txindex=1

# Make the wallet broadcast transactions (default: 1)
walletbroadcast=1
EOF
```
> :link: **Comparaison TESTNET / MAINNET:** Si vous voulez utiliser mainnet au lieu de testnet, il suffit d'omettre la ligne "testnet=1" ; facile !

> :link: **Comparaison ELAGUE / NON ELAGUE:** Si vous souhaitez utiliser une copie élaguée de la chaîne de blocs au lieu d'une copie non élaguée, pour minimiser les besoins de stockage et le temps de chargement, n'incluez pas la ligne "txindex=1", mais ajoutez plutôt une ligne "prune=550". txindex donne l'avantage d'un index complet des transactions, mais n'est pas compatible avec l'élagage, donc vous choisissez l'un ou l'autre

Pour terminer, limitez les permissions d'accès à votre fichier de configuration :
```
$ /bin/chmod 600 ~user1/.bitcoin/bitcoin.conf
```

### Lancez le démon !

Après tout cela, démarrer le démon Bitcoin est anticlimatiquement simple :
```
$ /usr/local/bin/bitcoind -daemon
```
Vous devez également ajouter une entrée crontab, afin que le démon bitcoin démarre à chaque fois que votre VPS redémarre :
```
$ ( /usr/bin/crontab -l -u user1 2>/dev/null; echo "@reboot /usr/local/bin/bitcoind -daemon" ) | /usr/bin/crontab -u user1 -
```

## Jouer avec Bitcoin

Alors maintenant, vous voulez probablement jouer avec Bitcoin !

Mais attendez, votre démon Bitcoin est probablement encore en train de télécharger des blocs. Cet alias, issu de votre configuration .bash, vous indiquera comment les choses se passent :

```
$ btcblock
```
La version 0.15.1 est assez rapide pour télécharger des blocs, mais le téléchargement du testnet non élagué peut prendre une heure. Il est peut-être temps de faire quelques expressos supplémentaires.

> :link: **Comparaison TESTNET / MAINNET:** Le réseau principal (MAINNET) non élagué prendra des heures de plus.

Mais, lorsque vous êtes prêt, continuez avec le [Chapitre trois : Comprendre la configuration de votre Bitcoin](03_0_Understanding_Your_Bitcoin_Setup_FR.md), où nous parlerons des fichiers et de la façon dont vous pouvez commencer à expérimenter.

## Résumé : Mise en place d'un VPS Bitcoin-Core à la main

En installant un VPS Bitcoin-Core à la main, vous avez simultanément créé votre jeu pour apprendre Bitcoin tout en vous assurant que vous savez comment tout cela fonctionne.

## Quelle est la prochaine étape ?

Voir l'autre méthode de mise en place d'un VPS au [Section 2 : Mise en place d'un VPS Bitcoin-Core avec StackScript](02_2_Configuration_d_un_VPS_Bitcoin-Core_avec_StackScript.md). Ou bien, passez à "bitcoin-cli" avec le [Chapitre 3 : Comprendre votre configuration Bitcoin](03_0_Comprendre_votre_configuration_Bitcoin.md).

## Annexe : Types d'installation de Bitcoin

**Mainnet.** Cela permettra de télécharger l'intégralité de la chaîne de blocage Bitnet. Cela représente 120G de données (et plus chaque jour).

**Mainnet élagué.** Cela permettra de réduire la chaîne de blocs que vous stockez aux 550 derniers blocs seulement. Si vous n'exploitez pas de mines ou un autre service Bitcoin, cela devrait suffire pour la validation.

**Testnet.** Cela vous permet d'accéder à une chaîne de blocs de bitcoins alternative où les bitcoins n'ont pas réellement de valeur. Elle est destinée à l'expérimentation et aux tests.

**Testnet élagué.** Ce ne sont que les 550 derniers blocs de Testnet ... parce que la chaîne de blocs de Testnet est maintenant assez grande aussi.

**Regtest privé.** Il s'agit du mode de test de régression, qui vous permet de faire fonctionner un serveur Bitcoin totalement local. Il permet d'effectuer des tests encore plus approfondis. Il n'est pas nécessaire d'élaguer ici, car vous repartez de zéro. Il s'agit d'une configuration très différente, qui est donc abordée vers la fin de ce tutoriel.