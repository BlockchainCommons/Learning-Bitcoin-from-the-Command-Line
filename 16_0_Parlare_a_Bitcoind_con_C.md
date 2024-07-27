16_0_Parlare_a_Bitcoind_con_C.md
# Capitolo 16: Parlare con Bitcoind in C

Durante il lavoro con gli Script Bitcoin, abbiamo raggiunto i limiti di ciò che è possibile fare con `bitcoin-cli`: attualmente non può essere utilizzato per generare transazioni contenenti script inusuali. Gli script di shell non sono nemmeno ideali per alcune cose, come la creazione di programmi ascoltatori che controllano costantemente. Fortunatamente, ci sono altri modi per accedere alla rete Bitcoin: le API di programmazione.

Questa sezione si concentra su tre diverse librerie che possono essere utilizzate come base per una programmazione sofisticata in C: una libreria RPC e una libreria JSON insieme ti permettono di ricreare molte delle operazioni che hai fatto con gli script di shell, ma ora utilizzando C; mentre una libreria ZMQ ti collega alle notifiche, qualcosa a cui non sei stato in grado di accedere precedentemente. (Il prossimo capitolo coprirà una libreria ancora più sofisticata chiamata Libwally, per completare questa introduzione alla programmazione di Bitcoin con C.)

## Obiettivi per Questo Capitolo

Dopo aver lavorato su questo capitolo, uno sviluppatore sarà in grado di:

   * Creare Programmi in C che utilizzano RPC per Parlare con Bitcoind
   * Creare Programmi in C che utilizzano ZMQ per Parlare con Bitcoind
   
Obiettivi di supporto includono la capacità di:

   * Comprendere come utilizzare una libreria RPC
   * Comprendere come utilizzare una libreria JSON
   * Comprendere le capacità di ZMQ
   * Comprendere come utilizzare una libreria ZMQ
   
## Indice

   
## Table of Contents

  * [Capitolo 16.1: Accedere a Bitcoind con Librerie RPC](16_1_Accedere_a_Bitcoind_con_Librerie_RPC.md)
  * [Capitolo 16.2: Programare Bitcoind in C con Librerie RPC](16_2_Programare_Bitcoind_in_C_con_Librerie_RPC.md)
  * [Capitolo 16.3: Ricevere Notifiche di Bitcoind in C tramite Librerie ZMQ](16_3_Ricevere_Notifiche_di_Bitcoind_in_C_tramite_Librerie_ZMQ.md)
  
