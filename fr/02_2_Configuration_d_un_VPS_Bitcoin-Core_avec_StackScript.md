# 2.2: Configuration d'un VPS Bitcoin-Core avec StackScript

> :information_source: **NOTE:** Il s'agit d'un projet en cours, afin que je puisse obtenir un retour d'information de la part des premiers examinateurs. Il n'est pas encore prêt pour l'apprentissage.

Ce document explique comment configurer un VPS (Virtual Private Sever) pour faire fonctionner un nœud Bitcoin sur Linode.com, installé à l'aide d'un script automatisé nommé StackScript. Il vous suffit d'entrer quelques commandes et de démarrer votre VPS. Ensuite, après être allé vous chercher un espresso, vous pourrez revenir et trouver votre nouveau nœud Bitcoin en train de télécharger des blocs avec plaisir.

> :warning: **AVERSTISSEMENT:** N'utilisez pas un VPS pour un portefeuille de bitcoin avec des fonds réels importants; cf http://blog.thestateofme.com/2012/03/03/lessons-to-be-learned-from-the-linode-bitcoin-incident/ . Il est très agréable de pouvoir expérimenter de vraies transactions de bitcoin sur un nœud en direct sans avoir à attacher un serveur auto-hébergé sur un réseau local. J'ai également trouvé utile de pouvoir utiliser un iPhone ou un iPad pour communiquer via SSH avec mon VPS afin d'effectuer quelques tâches bitcoin simples. Mais un niveau de sécurité plus élevé est nécessaire pour les fonds importants.

Si vous souhaitez plutôt faire toute la configuration à la main, veuillez lire le fichier HOWTO parallèle, [§2.1 : Installation manuelle d'un VPS Bitcoin-Core](02_1_Installation_manuelle_d_un_VPS_Bitcoin-Core.md).

Si vous avez déjà un nœud Bitcoin en cours d'exécution, lisez plutôt le prochain fichier HOWTO, [Chapitre 3 : Comprendre votre configuration Bitcoin](03_0_Comprendre_votre_configuration_Bitcoin.md).

## Démarrer avec Linode

Linode est un service d'hébergement en ligne (dans le _cloud_) qui propose des serveurs Linux rapides et bon marché avec un stockage SSD. Nous les utilisons pour ce tutoriel principalement parce que leurs StackScripts en BASH offrent un moyen facile de configurer automatiquement un nœud Bitcoin sans complications.

### Créez un compte Linode

Vous pouvez créer un compte Linode en vous rendant ici :

```
https://www.linode.com
```

Si vous préférez, le code de référence suivant vous donnera environ un mois d'utilisation gratuite, idéal pour apprendre Bitcoin :

```
https://www.linode.com/?r=23211828bc517e2cb36e0ca81b91cc8c0e1b2d96
```

Vous devrez fournir une adresse électronique et précharger plus tard de l'argent à partir d'une carte de crédit ou de PayPal pour les frais futurs.

Quand vous aurez terminé, vous devriez atterrir sur https://manager.linode.com

### Envisagez l'authentification à deux facteurs

La sécurité de votre serveur ne sera pas complète si des personnes peuvent s'introduire dans votre compte Linode, alors pensez à mettre en place une authentification à deux facteurs pour celui-ci. Vous pouvez trouver ce paramètre sur votre [My Profile: Password & Authentication page](https://manager.linode.com/profile/auth). Si vous ne le faites pas maintenant, ajoutez un item AFAIRE pour revenir et le faire plus tard.

## Création de l'image Linode à l'aide d'un StackScript

### Téléchargez le StackScript

Il y a une copie de notre StackScript de configuration VPS Bitcoin dans [le repo ici](02_2__Script_Linode_Setup.stackscript). Ce script automatise essentiellement les instructions de configuration du VPS Bitcoin à partir du [§2.1](02_1_Installation_manuelle_d_un_VPS_Bitcoin-Core.md). Si vous voulez être particulièrement prudent, lisez-le attentivement. Si vous êtes satisfait, vous pouvez copier ce StackScript dans votre propre compte en allant sur Gérer les [StackScripts](https://manager.linode.com/stackscripts/index), ou en cliquant sur le lien sous votre liste de Linodes. Cliquez sur "Ajouter un nouveau StackScript", donnez-lui un bon nom (nous utilisons `Bitcoin VPS Setup`), puis copiez et collez le script.

### Ajoutez un serveur Linode

Vous devez ensuite cliquer sur l'onglet "Linodes" et sélectionner "Add a Linode".

Une Linode 2G suffira pour la plupart des installations, y compris : Mainnet élagué, Testnet élagué, et même Testnet non élagué. Ils utilisent tous moins de 50G de mémoire et ils peuvent à peine s'en sortir avec 2G de mémoire. C'est la configuration que nous suggérons.

Si vous souhaitez plutôt avoir un Mainnet non élagué dans un VPS, vous devrez installer une Linode avec un disque de plus de 120G, qui est actuellement la Linode 12288, qui a 192G de stockage et 12G de mémoire et coûte environ 80$ par mois. Nous ne vous le recommandons pas.

Le tableau suivant indique les exigences minimales

| Configuration | Mémoire | Stockage | Linnode |
|-------|--------|---------|---------|
| Mainnet | 2G | 120G | Linode 8GB |
| Mainnet élagué | 2G | ~5G | Linode 2GB |
| Testnet | 2G | ~15G | Linode 2GB |
| Testnet élagué | 2G | ~5G | Linode 2GB |
| Regtest | 2G | ~ | Linode 2GB |

Choisissez simplement votre type de Linode, choisissez un endroit géographiquement aussi proche que possible de vous, et cliquez sur "Ajouter votre Linode".

:warning: **AVERTISSEMENT** Il nous est arrivé d'avoir des machines qui manquaient de mémoire après avoir fait tourner `bitcoind` pendant quelques jours alors qu'elles n'avaient que 2G. En revenant, nous constatons que `bitcoind` s'est arrêté, laissant le message "Error : Out of memory. Terminating" dans le fichier `debug.log`. Cela nécessite simplement un redémarrage de `bitcoind` et dix ou quinze minutes pour obtenir la resynchronisation de la chaîne de blocage. Ce problème a semblé plus important à partir de notre mise à jour vers la version 0.18 (après avoir disparu en grande partie), il est donc possible que vous deviez passer aux machines 4G uniquement pour des raisons de mémoire.

_Sachez que les exigences peuvent changer avec le temps, car la chaîne de blocs continue à se développer. Surveillez les erreurs "Out of Memory" ou "Disk Space is Low ! L'une ou l'autre indique que vous devez migrer vers la prochaine machine plus grande !_

### Configurez votre serveur Linode

Vous devriez maintenant être de retour sur la page de votre liste de Linodes, avec votre VPS listé avec un statut "Brand New".

Cliquez sur ce nouveau Linode pour accéder à son tableau de bord.

Vous verrez qu'aucun disque n'est encore installé.

Assurez-vous que Lindeo a terminé la configuration initiale, ce qui ne prend qu'une minute ou deux. Cherchez simplement "Linode Initial Configuration" dans la "Host Job Queue" et assurez-vous qu'elle a un petit bouton vert "Success".

### Exécutez le StackScript

Vous êtes maintenant prêt à exécuter votre StackScript !

Cliquez sur "Déployer une image" puis choisissez "Déployer à l'aide de StackScripts".

Si vous avez déjà ajouté le StackScript à votre compte, il devrait se trouver dans la liste de "Vos StackScripts" ; sélectionnez-le.

### Entrez les options de Stackscript

Pour vous assurer que vous pouvez configurer votre VPS selon vos spécifications, le StackScript dispose de quelques options. Voici ce que vous devez saisir.

_Cette configuration peut ne pas fonctionner si vous ne saisissez pas toutes ces informations obligatoires :_

**Type d'Installation.** Voir _l'annexe I_ pour plus d'informations sur ces types d'installation de Bitcoin. Si vous prévoyez d'utiliser le réseau principal Bitcoin, vous choisirez probablement "Mainnet élagué". Si vous souhaitez jouer avec Bitcoin Core et en savoir plus sur son fonctionnement, vous choisirez probablement "Testnet non élagué".

**Nom d'hôte court.** Choisissez un nom pour votre VPS. Par exemple, "mybtctest".

**Nom d'hôte complet.** Si vous comptez inclure ce VPS dans un réseau avec des enregistrements DNS complets, tapez le nom d'hôte avec son domaine. Par exemple, "monstre.mondomaine.com". Sinon, répétez simplement le nom d'hôte court et ajoutez ".local", par exemple "monstre.local".

**Mot de passe utilisateur.** Bitcoin sera installé sur un compte appelé "user1". Il s'agit du mot de passe de ce compte.

_Vous pouvez librement choisir de saisir ces informations facultatives ou de les ignorer :_

**Clé SSH.** Copiez ici la clé SSH de votre ordinateur local ; cela vous permet de vous connecter automatiquement via SSH à votre compte user1. Si vous n'avez pas encore configuré de clé SSH sur votre ordinateur local, vous trouverez de bonnes instructions à ce sujet sur [Github](https://help.github.com/articles/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent/).  Vous pouvez également ajouter votre clé SSH dans votre Linode LISH (Linode Interactive Shell) en allant sur votre "Linode Home Page / My Preferences / LISH Settings / LISH Keys". L'utilisation d'une clé SSH vous donnera un moyen plus simple et plus sûr de vous connecter à votre serveur.

**IPs autorisées pour SSH.** Il s'agit d'une liste d'IPs séparés par des virgules qui seront autorisés à faire des SSH dans le VPS. Par exemple "192.168.1.15,192.168.1.16". Si vous n'entrez aucun IP, _votre VPS ne sera pas très sécurisé_. Il sera constamment bombardé par des attaquants qui tenteront de trouver leur chemin, et ils pourraient très bien réussir.

Les questions restantes concernent tous les mécanismes de déploiement du VPS et doivent être laissées telles quelles, à une exception près : faites passer le disque d'échange de 256 Mo à 512 Mo, pour vous assurer que vous avez suffisamment de mémoire pour télécharger la chaîne de blocs.

Enfin, vous devrez remplir un mot de passe root, qui sera le mot de passe utilisé pour le compte root.

Cliquez sur "Déployer" pour initialiser vos disques et préparer votre VPS. L'ensemble de la file d'attente devrait fonctionner en moins d'une minute. Lorsque cela est fait, vous devriez voir dans la "File d'attente des tâches de l'hôte", des boutons verts "Réussite" indiquant "Disque créé à partir de StackScript - Définition du mot de passe pour le compte root... terminé" et "Créer un système de fichiers - 256 Mo d'échange d'image".

Vous pouvez maintenant changer le nom de votre VPS Linode par rapport au nom par défaut `linodexxxxxxxx`.  Allez dans l'onglet Paramètres, et changez le label pour qu'il soit plus utile, comme par exemple le nom d'hôte court de votre VPS. Par exemple, j'ai renommé le mien `bitcoin-testnet-pruned` pour le différencier des autres VPS de mon compte.

## Démarrez votre VPS

Votre VPS Linode est maintenant prêt à démarrer. Si vous n'êtes pas sur le tableau de bord de votre nouveau VPS, cliquez dessus.

Sélectionnez maintenant le bouton "Boot". Dès que vous voyez le bouton vert "Success : System Boot", vous pouvez vous connecter.

Tout d'abord, vous aurez besoin de l'adresse IP. Cliquez sur l'onglet "Linodes" et vous devriez voir une liste de votre VPS, le fait qu'il fonctionne, son "plan", son adresse IP et quelques autres informations.

Rendez-vous sur votre console locale et connectez-vous au compte user1 en utilisant cette adresse :

```
ssh user1@[IP-ADDRESS]
```

Par exemple :

```
ssh user1@192.168.33.11
```

Si vous avez configuré votre VPS pour utiliser une clé SSH, la connexion devrait être automatique (nécessitant éventuellement votre mot de passe SSH pour déverrouiller votre clé). Si vous n'avez pas configuré de clé SSH, vous devrez alors saisir le mot de passe user1.

### Prenez un espresso

Voici le gros problème : _votre StackScript fonctionne en ce moment même_. Le script BASH est exécuté la première fois que le VPS est démarré. Cela signifie que votre VPS n'est pas encore prêt.

Alors, faites une pause, prenez un espresso ou détendez-vous quelques minutes. Deux parties du script prennent un certain temps : la mise à jour de tous les paquets Debian et le téléchargement du code Bitcoin. Elles ne devraient pas prendre plus de 5 minutes chacune, ce qui signifie que si vous revenez dans 10 minutes, ce sera sûrement prêt.

Si vous êtes impatient, vous pouvez sauter devant et sudo tail -f ~root/stackscript.log qui affichera la progression de l'installation, comme décrit dans la section suivante.

## Vérifiez votre installation

Vous saurez que les StackScripts sont terminés lorsqu'un fichier BITCOIN-IS-READY apparaît dans le répertoire d'accueil du user1. À ce moment-là, votre répertoire home devrait ressembler à ceci :

```
$ ls
bitcoin-0.16.2-x86_64-linux-gnu.tar.gz    laanwj-releases.asc
BITCOIN-IS-READY            SHA256SUMS.asc
```

A côté du fichier BITCOIN-IS-READY se trouvent les différents fichiers qui ont été utilisés pour installer Bitcoin sur votre VPS. Aucun d'entre eux n'est nécessaire. Nous les avons juste laissés au cas où vous voudriez faire une vérification supplémentaire. Sinon, vous pouvez les supprimer :

```
$ rm *
```

### Vérifiez la configuration de Bitcoin

Afin de s'assurer que la version Bitcoin téléchargée est valide, le StackScript vérifie à la fois la signature et la somme de contrôle SHA. Vous devez vérifier que ces deux tests sont corrects :

```
$ sudo grep VERIFICATION ~root/stackscript.log
```

Si vous voyez quelque chose comme ce qui suit, tout devrait aller bien:

```
VERIFICATION SUCCESS / SIG: gpg: Good signature from &quot;Wladimir J. van der Laan (Bitcoin Core binary release signing key) &lt;laanwj@gmail.com&gt;&quot;
VERIFICATION SUCCESS / SHA: 29215a7fe7430224da52fc257686d2d387546eb8acd573a949128696e8761149
```

Cependant, si l'un de ces deux contrôles indique plutôt "ERREUR DE VÉRIFICATION", il y a un problème. Comme tout cela est écrit dans le script, il est possible qu'il y ait eu un changement mineur qui a fait que les contrôles du script ne fonctionnent pas correctement. Mais il est également possible que quelqu'un essaie de vous encourager à lancer une fausse copie du démon Bitcoin. _Donc, soyez sûr de savoir ce qui s'est passé avant d'utiliser Bitcoin !_

### Lisez les fichiers journaux

Vous pouvez également lire tous les fichiers journaux (_logs_) d'installation, pour vous assurer que rien d'inattendu ne s'est produit pendant l'installation.

Il est préférable de consulter le fichier journal standard de StackScript, qui contient tous les résultats, y compris les erreurs:

`$ sudo more ~root/stackscript.log`

Notez qu'il est tout à fait normal de voir quelques erreurs, en particulier lorsque vous utilisez le logiciel gpg très bruyant et lorsque diverses choses essaient d'accéder au périphérique /dev/tty qui n'existe pas.

Si vous voulez plutôt regarder un ensemble d'informations plus petit, toutes les erreurs devraient être dans:

`$ sudo more ~root/stackscript.err`

Il contient encore une bonne quantité d'informations qui ne sont pas des erreurs, mais sa lecture est plus rapide.

Si tout semble bon, félicitations, vous avez un nœud Bitcoin fonctionnel utilisant Linode !

## Ce que nous avons accompli

Bien que l'image par défaut de Debian 8 que nous utilisons pour votre VPS ait été modifiée par Linode pour être relativement sûre, votre nœud Bitcoin tel qu'il est installé par le StackScript de Linode est configuré avec un niveau de sécurité encore plus élevé. Vous pouvez trouver cela limitant, ou être incapable de faire les choses que vous attendez. Voici quelques notes à ce sujet :

### Services protégés

L'installation de votre VPS Bitcoin est minimale et ne permet pratiquement aucune communication. Elle est gérée par la partie 3 du StackScript, qui bloque l'accès à la plupart des ports du VPS.

Deux fichiers contrôlent ces restrictions, un pour IPv4 et un pour IPv6:

```
/etc/iptables.firewall.rules
/etc/ip6tables.firewall.rules
```

Si vous regardez les règles, vous verrez qu'elles autorisent tout le trafic sortant et tout le trafic de bouclage local. Toutefois, pour le trafic entrant, les seules connexions acceptées sont Bitcoin, Ping et SSH. Si vous souhaitez accepter d'autres types de trafic, vous devrez ouvrir des ports supplémentaires ("dports") en utilisant la même méthodologie que celle indiquée pour la connexion Bitcoin port 8333. Par exemple, pour autoriser les connexions au port 25 (mail), il faudrait ajouter les éléments suivants aux tables iptables et ip6tables:

```
-A INPUT -p tcp --dport 25 -j ACCEPT
-A INPUT -p udp --dport 25 -j ACCEPT
```

Cet exemple ouvre l'accès au port 25 pour les connexions TCP et UDP. Les autres ports populaires sont 80 (HTTP), 443 (HTTPS), 53 (DNS), 110 (POP) et 143 (IMAP).  Assurez-vous que ces modifications sont placées au-dessus des fichiers journaux et des valeurs par défaut qui apparaissent à la fin des fichiers iptables.firewall.rules.

Vous pouvez immédiatement intégrer vos modifications en exécutant le script suivant:

```
$ /etc/network/if-pre-up.d/firewall
```

### Shells protégés

Si vous avez fourni un accès IP pour le contrôle SSH, l'accès SSH (et SCP) au serveur est sévèrement limité. Le fichier /etc/hosts.deny interdit à quiconque de se connecter. _Nous ne suggérons pas de changer cela_. /etc/hosts.allow autorise alors des adresses IP spécifiques. Ajoutez simplement d'autres adresses IP dans une liste séparée par des virgules si vous souhaitez offrir un accès plus large.

Par exemple :

```
sshd: 127.0.0.1, 192.128.23.1
```

### Mises à jour automatisées

Debian est également configuré pour se mettre automatiquement à niveau, afin de s'assurer qu'il reste au fait des derniers correctifs de sécurité.

Si, pour une raison quelconque, vous souhaitez changer cela (_nous ne le conseillons pas_), vous pouvez le faire :

```
echo "unattended-upgrades unattended-upgrades/enable_auto_updates boolean false" | debconf-set-selections
```

## Jouer avec Bitcoin

Alors maintenant, vous voulez probablement jouer avec Bitcoin !

Mais attendez, votre démon Bitcoin est probablement encore en train de télécharger des blocs. Cet alias, issu de votre configuration .bash, vous indiquera comment les choses se passent :

```
$ btcblock
```

Si vous choisissez l'une des options élaguées, il vous faudra probablement une journée pour tout télécharger.

Il est donc peut-être temps de faire quelques expressos supplémentaires. Mais bientôt, votre système sera prêt à fonctionner, et vous pourrez commencer à expérimenter.

## Résumé : Mise en place d'un VPS Bitcoin-Core avec StackScript

La création d'un VPS Bitcoin-Core avec un StackScript a rendu l'ensemble du processus rapide, simple et (espérons-le) sans douleur.

## Quelle est la prochaine étape ?

You have a few options for what's next:

   * Lisez le [StackScript](02_2__Script_Linode_Setup.stackscript) pour comprendre votre configuration.
   * Voir l'autre méthode de mise en place d'un SPV au [§2.1 : Installation manuelle d'un VPS Bitcoin-Core](02_1_Installation_manuelle_d_un_VPS_Bitcoin-Core.md).
   * Passez à "bitcoin-cli" avec le [Chapitre 3 : Comprendre votre configuration Bitcoin](03_0_Comprendre_votre_configuration_Bitcoin.md).

## Annexe : Types d'installation de Bitcoin

**Mainnet.** Cela permettra de télécharger l'intégralité de la chaîne de blocage Bitnet. Cela représente 120G de données (et plus chaque jour).

**Mainnet élagué.** Cela permettra de réduire la chaîne de blocs que vous stockez aux 550 derniers blocs seulement. Si vous n'exploitez pas de mines ou un autre service Bitcoin, cela devrait suffire pour la validation.

**Testnet.** Cela vous permet d'accéder à une chaîne de blocs de bitcoins alternative où les bitcoins n'ont pas réellement de valeur. Elle est destinée à l'expérimentation et aux tests.

**Testnet élagué.** Ce ne sont que les 550 derniers blocs de Testnet ... parce que la chaîne de blocs de Testnet est maintenant assez grande aussi.

**Regtest privé.** Il s'agit du mode de test de régression, qui vous permet de faire fonctionner un serveur Bitcoin totalement local. Il permet d'effectuer des tests encore plus approfondis. Il n'est pas nécessaire d'élaguer ici, car vous repartez de zéro. Il s'agit d'une configuration très différente, qui est donc abordée vers la fin de ce tutoriel.