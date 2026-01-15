# Capitolo 17: Programmazione con Libwally

Il capitolo precedente ha presentato tre librerie C, per RPC, JSON e ZMQ, tutte destinate a interagire direttamente con `bitcoind`, proprio come hai fatto dall'inizio. Ma a volte potresti voler programmare senza accesso diretto a un `bitcoind`. Questo potrebbe essere dovuto a un client offline, o semplicemente perché vuoi mantenere alcune funzionalità interne al tuo programma C. Potresti anche voler approfondire le funzionalità del portafoglio, come la creazione di parole mnemoniche o la derivazione di indirizzi. È qui che entra in gioco Libwally: è una libreria per portafogli per C, C++, Java, NodeJS o Python, con wrapper disponibili anche per altri linguaggi, come Swift.

Questo capitolo tocca le funzionalità possibili all'interno di Libwally, la maggior parte delle quali completa il lavoro che hai svolto tramite l'accesso RPC a `bitcoind`, ma alcune delle quali lo replicano. Mostra anche come integrare quel lavoro con i client RPC con cui sei più familiare. Tuttavia, nota che questa è solo la minima introduzione a Libwally. Diverse delle sue serie di funzioni più importanti sono evidenziate, ma non facciamo mai più che bagnare i piedi. Se trovi le sue funzioni utili o intriganti, allora dovrai approfondire molto più di quanto questo corso possa coprire.

## Obiettivi di questo Capitolo

Dopo aver lavorato attraverso questo capitolo, uno sviluppatore sarà in grado di:

   * Utilizzare le Funzioni del Portafoglio con Libwally
   * Eseguire Manipolazioni di PSBT e Transazioni con Libwally
   * Implementare Progetti che Mescolano Lavoro con Libwally e RPC
   
Obiettivi di supporto includono la capacità di:

  * Comprendere le Parole Mnemoniche BIP39
  * Comprendere di Più sui Portafogli Gerarchici BIP32
  * Riassumere la Profondità Funzionale di Libwally
   
## Sommario


  * [Capitolo 17.1 Configurare Libwally](17_1_Configurare_Libwally.md)
  * [Capitolo 17.2 Usare BIP39 in Libwally](17_2_Usare_BIP39_in_Libwally.md)
  * [Capitolo 17.3 Usare BIP32 in Libwally](17_3_Usare_BIP32_in_Libwally.md)
  * [Capitolo 17.4 Usare PSBTs in Libwally](17_4_Usare_PSBTs_in_Libwally.md)
  * [Capitolo 17.5 Usare Scripts in Libwally](17_5_Usare_Scripts_in_Libwally.md)
  * [Capitolo 17.6 Usare Altre Funzioni in Libwally](17_6_Usare_Altre_Funzioni_in_Libwally.md)
  * [Capitolo 17.7 Integrare Libwally e Bitcoin-CLI](17_7_Integrare_Libwally_e_Bitcoin-CLI.md)
