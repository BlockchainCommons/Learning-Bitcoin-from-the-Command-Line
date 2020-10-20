# Chapitre 6 : Développer les transactions Bitcoin

Les transactions de base de Bitcoin  : (1) envoi de fonds ; (2) à un seul destinataire P2PKH ; (3) immédiatement. Cependant, les trois parties de cette définition peuvent être élargies en utilisant des transactions Bitcoin plus complexes, qui peuvent alternativement envoyer des données, qui peuvent être envoyées à un groupe de destinataires, ou qui peuvent être envoyées à un moment ultérieur. Ces trois options représentent un renforcement de l'autonomie de Bitcoin et aussi les limites les plus éloignées de ce que vous pouvez faire avec la commande de base `bitcoin-cli`.

## Objectifs de cette section

Après avoir parcouru ce chapitre, un développeur sera en mesure de faire ceci :



   * Créer des adresses Bitcoin multi-signatures
   * Créer des transactions avec des périodes de blocage (Locktimes)
   * Créer des transactions avec des données

Les objectifs de soutien incluent également :

   * Comprendre comment dépenser des fonds envoyés à une multi-signature
   * Plan pour la puissance des multi-signatures
   * Comprendre les différentes sortes de timelocks
   * Plan pour la puissance de Locktime
   * Plan pour la puissance de OP_RETURN

## Table of Contents

   * [Section 1: Envoyer une transaction avec une multisig](06_1_Envoyer_une_transaction_avec_une_multisig.md)
   * [Section 2 : Dépenser une transaction avec une multisig](06_2_Depenser_une_transaction_avec_une_multisig.md)
   * [Section 3 : Envoyer et dépenser une multisig automatisée](06_3_Envoyer_et_depenser_une_multisig_automatisee.md)
   * [Section 4 : Envoyer une transaction avec un délai de blocage](06_4_Envoyer_une_transaction_avec_un_delai_de_blocage.md)
   * [Section 5 : Envoyer une transaction contenant des données](06_5_Envoyer_une_transaction_contenant_des_donnees.md)
