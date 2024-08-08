# 20.4: Expanding the Lightning Network

> :information_source: **NOTE:** Questa sezione è stata recentemente aggiunta al corso ed è una bozza iniziale che potrebbe essere ancora in attesa di revisione. Attenzione lettore.

Questi due capitoli hanno coperto solo alcune delle attività più importanti con Lightning. C'è molto altro che si può fare e molta varietà possibile. Di seguito sono riportati alcuni suggerimenti per andare avanti.

## Usa i Plugin di c-lightning

c-lightning è un'implementazione leggera, altamente personalizzabile e conforme agli standard del protocollo Lightning Network. Estende le sue funzionalità utilizzando i Plugin. Principalmente, questi sono sottoprocessi che vengono avviati dal demone `lightningd` e possono interagire con `lightningd` in vari modi:

* Le opzioni della riga di comando consentono ai plugin di registrare i propri argomenti della riga di comando, che vengono quindi esposti tramite `lightningd`.
* Il passaggio dei comandi JSON-RPC consente ai plugin di aggiungere i propri comandi all'interfaccia JSON-RPC.
* Le sottoscrizioni allo stream di eventi forniscono ai plugin un meccanismo di notifica push per `lightnind`.
* Gli hooks sono un'opzione primitiva che consente ai plugin di essere notificati sugli eventi nel demone `lightningd` e modificare il suo comportamento o trasmettere comportamenti personalizzati.

Un plugin può essere scritto in qualsiasi linguaggio e può comunicare con `lightningd` attraverso stdin e stdout del plugin. JSON-RPCv2 è utilizzato come protocollo sopra i due stream, con il plugin che agisce come server e `lightningd` che agisce come client.

Il repository GitHub di `lightningd` mantiene un elenco aggiornato di [plugin](https://github.com/lightningd/plugins) disponibili.

## Usa Wallet Mobile

Attualmente conosciamo due wallet Lightning mobili che supportano l'implementazione di c-lightning.

Per i dispositivi iOS FullyNoded è un wallet Bitcoin open-source per iOS che si connette tramite Tor V3 authenticated service al tuo nodo completo. La funzionalità di FullyNoded è attualmente in fase di sviluppo attivo e in fase di beta testing iniziale.

* [FullyNoded](https://github.com/Fonta1n3/FullyNoded/blob/master/Docs/Lightning.md)

SparkWallet è un wallet GUI minimalista per c-lightning, accessibile via web o tramite app mobili e desktop per Android.

* [SparkWallet](https://github.com/shesek/spark-wallet)

## Usa Diverse Implementazioni di Lightning

c-lightning non è la tua unica opzione. Oggi ci sono tre implementazioni ampiamente utilizzate del Lightning Network. Tutte seguono i documenti [Basis of Lightning Technology (BOLT)](https://github.com/lightningnetwork/lightning-rfc), che descrivono un protocollo di livello 2 per i trasferimenti di bitcoin off-chain. Le specifiche sono attualmente in corso di stesura.

| Nome | Descrizione | BitcoinStandup | Linguaggio | Repository |
| ------------- | ------------- | :---: | ------------- | ------------- |
| C-lightning | Blockstream | X | C | [Download](https://github.com/ElementsProject/lightning) |
| LND | Lightning Labs | X | Go | [Download](https://github.com/lightningnetwork/lnd) |
| Eclair | ACINQ | - | Scala | [Download](https://github.com/ACINQ/eclair) |

## Mantieni Backup

Il tuo nodo Lightning deve essere online tutto il tempo, altrimenti la tua controparte potrebbe inviare uno stato precedente del canale e rubare i tuoi fondi. Tuttavia, c'è un altro scenario in cui i fondi possono essere persi, ed è quando si verifica un guasto hardware che impedisce al nodo di stabilire una chiusura cooperativa con la controparte. Questo probabilmente significherà che se non hai una copia esatta dello stato del canale prima del guasto, avrai uno stato non valido che potrebbe portare l'altro nodo a considerarlo un tentativo di frode e utilizzare la transazione di penalità. In questo caso, tutti i fondi andranno persi. Per evitare questa situazione indesiderata, esiste una soluzione basata sull'alta disponibilità del database postgresQL [esiste](https://github.com/gabridome/docs/blob/master/c-lightning_with_postgresql_reliability.md).

Non abbiamo testato questa soluzione.

## Sommario: Espandere il Lightning Network

Puoi utilizzare diverse implementazioni, plugin, wallet mobili o backup per espandere la tua esperienza con Lightning.

## Cosa Succede Dopo?

Hai completato Learning Bitcoin from the Command Line, anche se se non hai mai visitato gli [Appendici](A0_Appendices.md) di configurazioni alternative, puoi farlo ora.

Altrimenti, ti incoraggiamo a unirti alle comunità di sviluppatori, a programmare e a mettere in pratica le tue nuove conoscenze.

Puoi anche aiutarci qui a Blockchain Commons con Issues o PR per Learning Bitcoin o per uno qualsiasi dei nostri altri repository, o puoi persino diventare un [Sponsor](https://github.com/sponsors/BlockchainCommons). Puoi anche aiutare spargendo la voce: fai sapere alle persone sui social media del corso e di cosa hai imparato!

Ora vai là fuori e rendi la comunità Blockchain un posto migliore!
