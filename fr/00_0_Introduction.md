# Introduction à la programmation de Bitcoin Core & Lightning

La façon dont nous effectuons les paiements pour les biens et les services a changé de manière spectaculaire au cours des dernières décennies. Alors qu'autrefois, toutes les transactions étaient effectuées en espèces ou par chèque, aujourd'hui, divers modes de paiement électronique sont la norme. Cependant, la plupart de ces paiements électroniques se font encore par l'intermédiaire de systèmes centralisés, où les sociétés de cartes de crédit, les banques, voire les institutions financières basées sur Internet comme Paypal, conservent de longues listes de transactions corrélées individuellement et ont le pouvoir de censurer les transactions qui ne leur plaisent pas.

Ces risques de centralisation ont été parmi les principaux catalyseurs de la création des crypto-monnaies, dont la première et la plus réussie est le Bitcoin. Bitcoin offre un pseudonyme, il rend difficile la corrélation des transactions et rend la censure par des entités individuelles pratiquement impossible. Ces avantages en ont fait l'une des monnaies à la croissance la plus rapide au monde. Cette croissance a à son tour fait de Bitcoin une entreprise florissante pour les entrepreneurs et les développeurs, désireux de créer de nouveaux services pour la communauté Bitcoin.

Si vous êtes l'un de ces entrepreneurs ou développeurs, alors ce livre est pour vous, car il s'agit d'apprendre à programmer Bitcoin. C'est un livre d'introduction, qui explique toutes les nuances et les caractéristiques de Bitcoin au fur et à mesure. Il adopte également une approche très spécifique, en proposant des leçons sur la façon de travailler directement avec Bitcoin Core et avec le serveur c-lightning. 

Pourquoi ne pas utiliser certaines des bibliothèques les plus complètes que l'on trouve dans divers langages de programmation ? Pourquoi ne pas créer votre propre bibliothèque à partir de zéro ? C'est parce qu'il est dangereux de travailler avec des crypto-monnaies. Il n'existe pas de filet de sécurité. Si vous payez accidentellement trop cher, si vous perdez une clé de signature, si vous créez une transaction non valable ou si vous faites un certain nombre d'erreurs potentielles, votre crypto-monnaie disparaîtra à jamais. Une grande partie de cette responsabilité vous incombera, bien sûr, en tant que programmeur de crypto-monnaie, mais elle peut être réduite au minimum en travaillant avec les interfaces de crypto-monnaies les plus robustes, les plus sûres et les plus sécurisées qui existent, celles créées par les équipes de programmation de crypto-monnaies elles-mêmes : ``bitcoind`` and ``lightningd``.

Une grande partie de ce livre traite donc de la manière de scripter Bitcoin (et Lightning) directement depuis la ligne de commande. Certains chapitres ultérieurs traitent de langages de programmation plus sophistiqués, mais là encore, ils continuent à interagir directement avec les démons bitcoin et lightningd en utilisant RPC ou en interagissant avec les fichiers qu'ils créent. Cela vous permet de vous tenir sur les épaules des géants et d'utiliser leur technologie en toute confiance pour apprendre à créer vos propres systèmes de confiance.



## Quelle est la prochaine étape ?

Apprenez les bases du bitcoin dans le premier chapitre : [Chapitre 1 : Présentation du bitcoin](01_0_Presentation_du_Bitcoin.md).
