# Introduction à la programmation de Bitcoin Core & Lightning

La façon dont nous effectuons les paiements pour les biens et les services a changé de manière spectaculaire au cours des dernières décennies. Alors qu'autrefois, toutes les transactions étaient effectuées en espèces ou par chèque, aujourd'hui, divers modes de paiement électronique sont la norme. Cependant, la plupart de ces paiements électroniques se font encore par l'intermédiaire de systèmes centralisés, où les sociétés de cartes de crédit, les banques, voire les institutions financières basées sur Internet comme Paypal, conservent de longues listes de transactions corrélées individuellement et ont le pouvoir de censurer les transactions qui ne leur plaisent pas.

Ces risques induits par la centralisation ont été parmi les principaux catalyseurs de la création des crypto-monnaies, dont la première et la plus réussie est Bitcoin. Bitcoin permet l'usage de pseudonymes, rend difficile de corréler des transactions et prévient presque totalement la censure par des entités individuelles. Ces avantages en ont fait l'une des monnaies à la croissance la plus rapide au monde. Cette croissance a à son tour fait de Bitcoin une entreprise florissante pour les entrepreneurs et les développeurs, désireux de créer de nouveaux services pour la communauté Bitcoin.

Si vous êtes l'un de ces entrepreneurs ou développeurs ce livre est pour vous, car il est entièrement consacré à l'apprentissage de la programmation dans Bitcoin. Ouvrage d'initiation, il éclaire, au cours de son cheminement, toutes les nuances et caractéristiques de Bitcoin. D'un abord pragmatique, ses leçons vous offrent une approche directe du travail avec Bitcoin Core et le serveur c-lightning. 

Pourquoi ne pas utiliser une des bibliothèques de fonctions, plus complètes et déjà disponibles dans d'autres langages de programmation ? Pourquoi ne pas créer sa propre bibliothèque, à partir de zéro ? Avant tout parce la crypto-monnaie est une activité risquée: Il n'existe pas de filet de sécurité. Si l'on paie accidentellement trop cher, perd sa clé de signature électronique, crée une transaction invalide ou commet quelqu'autre erreur de ce type, la perte des crypto-monnaies s'avère irrévocable. Bien sûr en tant que développeur, la responsabilité de tels dommages vous incombera en grande partie, mais il vous est possible de la réduire au minimum en travaillant avec les interfaces de crypto-monnaies les plus robustes, fiables et sécurisées qui soient, celles programmées par les propres créateurs de ces crypto-monnaies: ``bitcoind`` et ``lightningd``.

Une grande partie de ce livre traite donc de la manière de lancer Bitcoin (et Lightning) directement en ligne de commande. Certains des chapitres ultérieurs traitent de langages de programmation plus sophistiqués, mais là encore en relation directe avec les daemons bitcoind et lightningd, soit par le truchement de RPC, soit en interagissant avec les fichiers qu'ils créent. Vous pourrez ainsi vous hisser sur les épaules de ces géants, mettre à profit la fiabilité de leur technologie pour apprendre à créer, à votre tour, des solutions fiables.



## Quelle est la prochaine étape ?

Apprenez les bases du bitcoin dans le premier chapitre : [Chapitre 1 : Présentation du bitcoin](01_0_Presentation_du_Bitcoin.md).
