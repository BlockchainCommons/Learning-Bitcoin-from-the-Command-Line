# Interlude : Utilisation des variables de la ligne de commande

> :information_source: **NOTE:** Ce texte est encore un brouillon, afin que je puisse obtenir un retour d'information de la part des premiers examinateurs. Il n'est pas encore prêt pour l'apprentissage.

La section précédente a montré un certain nombre de commandes en ligne de commande utilisées sans obfuscation ni interférence. Cependant, ce n'est souvent pas la meilleure façon d'exécuter Bitcoin à partir de la ligne de commande. Parce que vous avez affaire à des variables longues, complexes et illisibles, il est facile de faire une erreur si vous copiez ces variables (ou, _satoshi forfend [NDT ?]_, si vous les tapez à la main). Comme ces variables peuvent faire la différence entre recevoir et perdre de l'argent réel, vous ne voulez pas faire d'erreur. Pour ces raisons, nous vous conseillons vivement d'utiliser des variables de ligne de commande pour enregistrer des adresses, des signatures ou d'autres longues chaînes d'informations chaque fois qu'il est raisonnable de le faire.

Si vous utilisez `bash`, vous pouvez enregistrer des informations dans une variable comme ceci :

```
$ VARIABLE=$(command)
```
C'est une simple substitution de commande, l'équivalent de ``VARIABLE=`commande` ``. La commande à l'intérieur des parenthèses est exécutée, puis affectée à la VARIABLE.

La création d'une nouvelle adresse ressemblerait alors à ceci :

```
$ unset NEW_ADDRESS_1
$ NEW_ADDRESS_1=$(bitcoin-cli getnewaddress)
```
Ces commandes effacent la variable NEW_ADDRESS_1, juste pour être sûr, puis la remplissent avec le résultat de la commande `getnewaddress` de `bitcoin-cli`.

Vous pouvez ensuite utiliser la commande `echo` de votre shell pour consulter votre (nouvelle) adresse :

```
$ echo $NEW_ADDRESS_1
n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf
```
Comme votre adresse figure dans une variable, vous pouvez désormais signer facilement un message pour cette adresse, sans vous soucier de faire une erreur de frappe. Bien entendu, vous enregistrerez également cette signature dans une variable !

```
$ NEW_SIG_1=$(bitcoin-cli signmessage $NEW_ADDRESS_1 "Hello, World")
```
Le reste de ce tutoriel utilisera ce style de sauvegarde des informations dans des variables lorsque cela sera pratique.

_Quand n'est-il pas pratique d'utiliser des variables en ligne de commande ?_ Les variables de ligne de commande ne sont pas pratiques si vous devez utiliser les informations ailleurs que sur la ligne de commande. Par exemple, la sauvegarde de votre signature peut ne pas être utile si vous devez simplement l'envoyer à quelqu'un d'autre dans un courriel. En outre, certaines commandes futures produiront des objets JSON au lieu de simples informations, et les variables ne peuvent pas être utilisées pour saisir ces informations... du moins pas sans _un peu plus_ de travail.

## Résumé : Utilisation des variables de la ligne de commande

Les variables shell peuvent être utilisées pour contenir de longues chaînes Bitcoin, ce qui minimise les risques d'erreur.

## Quelle est la prochaine étape ?

Poursuivez "Comprendre votre configuration Bitcoin" avec le [§3.4 : Réception d'une transaction](03_4_Reception_d_une_transaction.md).