# Tradurre Impara Bitcoin dalla riga di comando

Grazie per il tuo interesse nella traduzione di Learning Bitcoin dalla riga di comando! Quella che segue è la nostra procedura standard per farlo. In caso di domande, inviare un problema.

1. **Si prega di seguire la nostra normale procedura per i collaboratori.**
 * Leggi [CONTRIBUTING.md](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/CONTRIBUTING.md).
 * Compila un [Contratto di licenza per collaboratore](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/CLA.md), firmalo con GPG e invialo come un PR. Assicurati che ciò venga fatto per tutti coloro che contribuiscono alla traduzione.
1. **Scegli una versione.**
 * Scegli una [Versione](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/releases) come base della tua traduzione. Generalmente suggeriamo l'ultima versione. Ciò garantirà la coerenza di tutti i file nella tua traduzione, ti isolerà da eventuali modifiche apportate e renderà più facile vedere cosa è cambiato quando creiamo una nuova versione.
 * Se sembra che non ci sia una nuova versione da un po', invia un problema dicendo che sei interessato a iniziare una nuova traduzione e chiedendo se avrebbe senso che ci sia una nuova pietra miliare di rilascio prima di farlo . Se c'è stato qualcosa di degno di nota, e non siamo nel mezzo delle cose, probabilmente creeremo una nuova patch o una versione minore. Se siamo nel bel mezzo del lavoro, ti suggeriremo semplicemente di utilizzare la versione precedente.
 * Etichetta il tuo sommario e ogni capitolo o sezione con la versione utilizzata.
1. **Richiedi una filiale.**
 * Presenta un [problema](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/issues) richiedendo un nuovo ramo per la tua traduzione.
 * Questo sarà per noi il luogo principale in cui raccoglieremo il lavoro sulla traduzione nel tempo.
 * Creeremo una directory di primo livello per la tua traduzione completa utilizzando il [codice lingua ISO 639-1](https://en.wikipedia.org/wiki/List_of_ISO_639-1_codes), ad esempio `es` (spagnolo), "fr" (francese) o "pt" (portoghese). Il lavoro dovrebbe essere svolto in quella directory.
     
1. **Biforca il ramo.**
 * Una volta creato un ramo di traduzione, ti consigliamo di inserirlo nel tuo account GitHub.
 * Generalmente, suggeriamo di creare un ramo di lavoro da quel ramo di traduzione per ogni capitolo separato. Ciò ti consentirà di affrontare il processo di scrittura/revisione/revisione per ogni singolo capitolo senza che questo rimanga intrappolato con i tuoi nuovi contenuti per i capitoli futuri e ci consentirà di unire i capitoli man mano che vengono completati, che è la nostra preferenza , e aiuterà tutti a tenere traccia di dove si trovano le cose.
 * Assicurati di [firmare i tuoi commit](https://docs.github.com/en/github/authenticating-to-github/managing-commit-signature-verification/signing-commits).
1. **Invia i PR un capitolo alla volta.**
 * Invia i tuoi PR per la traduzione dal tuo work fork al nostro ramo di traduzione in lotti di non più di un singolo capitolo alla volta.
 * Invia in gruppi più piccoli se ha senso, ad esempio perché persone diverse scrivono sezioni diverse.
 * Ancora una volta, suggeriamo che ci sia un ramo per ogni capitolo, quindi quando invii il tuo PR per un capitolo, di solito creerai un ramo per il capitolo successivo.
1. **Richiedi l'approvazione da parte di un madrelingua.**
 * Nessuno potrà mai apportare una modifica eccezionale al proprio lavoro, quindi richiediamo che ogni sezione sia approvata da qualcuno diverso dal traduttore originale.

 * Ciò significa fondamentalmente che qualsiasi team di traduzione _dovrebbe_ contenere almeno due membri, un traduttore e un redattore oppure due persone che si scambiano i ruoli di traduttore e redattore. Se il tuo team non ha un secondo membro, possiamo chiamare un redattore/approvatore quando invii un PR, ma è possibile che non saremo in grado di trovarne uno e il tuo duro lavoro languirà, quindi è meglio averne uno in anticipo.

 * Per rendere il processo di modifica e revisione più semplice, indipendente e ottimizzato, suggeriamo a te e al revisore di [richiedere l'accesso ai rispettivi fork](https://docs.github.com/en/enterprise-server@3.0/github /configurazione-e-gestione-del-tuo-account-utente-github/gestione-dell'accesso-ai-tui-repository-personali/invito-dei-collaboratori-a-un-repository-personale). Una volta invitato, il _revisore_ può quindi:

 1. Effettua facilmente il checkout del PR nel computer locale con [GitHub CLI](https://cli.github.com/) eseguendo `gh pr checkout <pr-number>`. Ciò creerà automaticamente un ramo con il codice PR su cui il revisore potrà lavorare.

 2. Quindi, esegui le modifiche con commit firmati.

 3. Esegui "git push" dopo aver terminato la revisione per far sì che le modifiche si riflettano nel PR.
1. **Richiedi l'approvazione al Blockchain Commons Team.**
 * Una volta che un capitolo o una sezione sono stati approvati da un madrelingua, richiedi l'approvazione a qualcuno del team Blockchain Commons: attualmente [@shannona](https://github.com/shannona).
1. **Continua!**
 * Continua il processo, non più di un capitolo alla volta, finché non avrai un libro completo.
 * Essere consapevoli della portata del progetto complessivo. A partire dalla versione 2.01, Imparare Bitcoin dalla riga di comando ha una lunghezza di 120.000 parole. Come libro, sarebbero 250-400 pagine, a seconda del formato e del layout. (Circa 90.000 parole di questo sono testo da tradurre, mentre il resto è codice.) Vuoi assicurarti di avere il tempo per quel livello di impegno prima di iniziare.
1. **Facci sapere quando hai finito.**
 * Una volta completata la traduzione, invia un problema per farci sapere che il ramo di traduzione è pronto per essere unito al master.
 * Questo ci permetterà anche di annunciare la traduzione completata e di collegarla al file README principale
1. **Aggiorna la tua traduzione con le nuove versioni**
 * Ci auguriamo che le traduzioni restino aggiornate con le nuove versioni, in particolare quelle maggiori e secondarie, che probabilmente includeranno nuovi contenuti e aggiornamenti. Attualmente, questi si verificano solo ogni pochi anni
 * Se hai deciso di interrompere l'aggiornamento di una traduzione, faccelo sapere in un numero, in modo che possiamo far sapere alla comunità che stiamo cercando un nuovo traduttore per continuare ad aggiornare una traduzione.

### ESEMPIO DI FLUSSO DI LAVORO

Il capitolo 3 ha 7 sottosezioni: 3.0, 3.1, 3.2, 3.3, Interludio 3.3, 3.4 e 3.5.
Supponiamo che tu voglia tradurlo in francese.

Un approccio alla sua traduzione potrebbe essere quello di creare un ramo "francese-3" da
ramo "francese-traduzione" (creato nel passaggio precedente). Quindi crea un file
file di traduzione per la sottosezione 3.0. Quando pensi che il capitolo 3.0 sia
pronto per la revisione, applichi le modifiche e continui con il file successivo,
sottosezione 3.1. Ripeti la stessa procedura per questo file e tutti i successivi.
Successivamente, il tuo ramo "francese-3" divergerà dal ramo base
("traduzione francese" in questo caso) da 7 diversi commit, uno per ciascuno
sottosezione, oltre ad alcuni altri commit se hai dimenticato di apportare alcune modifiche o necessità
per correggere gli altri. A questo punto sei pronto per applicare le modifiche al tuo
fork e aprire una richiesta pull al repository originale.

Nel processo di revisione discuterai alcune decisioni con il tuo revisore e
introdurre nuove modifiche sotto forma di commit se è necessario modificare alcune parti
delle tue modifiche. Non dimenticare di rendere significative queste modifiche
pezzi e spingerli sulla forchetta. Dopo aver perfezionato le modifiche, il tuo revisore
accetterà le tue modifiche e richiederà l'unione di tali modifiche sul file
ramo "traduzione francese". Tieni presente che puoi anche inviare le modifiche e
crea la tua richiesta pull dopo il tuo primo commit. Ciò consentirà al revisore
per iniziare la revisione prima di terminare la traduzione di tutti i capitoli.

Puoi tenere traccia dello stato della traduzione di quel particolare capitolo nel file
corpo della richiesta pull, con il seguente modello:

```
Questa richiesta pull crea la versione <translated_lingual> del capitolo
<numero_capitolo> di Imparare Bitcoin dalla riga di comando.

Cose da controllare:
- Rispettare i nomi dei file e dei capitoli da [README] tradotto (locale/percorso/a/README).
- Traduzione corretta.
- Coerenza dei termini: revisione del file ["memoria di traduzione"](locale/percorso/del/file).
- Buona punteggiatura.
- Collegamento corretto.
- Bella formattazione.

| Sottocapitolo | Tradotto | Recensito | Fatto |
| ------------- | :--------------: | :--------------: | :--------------: |
| _0_ | <ul><li> [ ] </li></ul> | <ul><li> [ ] </li></ul> | <ul><li> [ ] </li></ul> |
| _1_ | <ul><li> [ ] </li></ul> | <ul><li> [ ] </li></ul> | <ul><li> [ ] </li></ul> |
| _2_ | <ul><li> [ ] </li></ul> | <ul><li> [ ] </li></ul> | <ul><li> [ ] </li></ul> |
| _3_ | <ul><li> [ ] </li></ul> | <ul><li> [ ] </li></ul> | <ul><li> [ ] </li></ul> |
| _ecc_ | <ul><li> [ ] </li></ul> | <ul><li> [ ] </li></ul> | <ul><li> [ ] </li></ul> |
```

### SUGGERIMENTI FINALI:
- _Mantenere le righe inferiori a 80 caratteri_. Il parser Markdown li formatterà correttamente
 (ricontrollalo) e il processo di revisione sarà più semplice.
- _Traduci prima il file `README.md`_ e usalo come riferimento per la traduzione
 di tutti i seguenti nomi di capitoli, nomi di file e nomi di collegamenti. Il README
 rappresenta lo stato del ramo master al momento dell'inizio del
 processo di traduzione. Verrà consultato da tutti i traduttori per sapere come fare
 tradurre i nomi di file e capitoli. Cercare di raggiungere un accordo con l'altro
 traduttori su quei nomi prima di scrivere qualsiasi capitolo e proseguire oltre
 modifiche di questo file al minimo.
- _Crea un file di "memoria di traduzione"_ con le parole speciali che potresti avere
 più traduzioni negli stessi contesti e la traduzione scelta per
 loro. I buoni candidati per essere in questo elenco sarebbero parole specifiche del dominio,
 come: multisig, input, output, script, timelock, locktime, hash, ecc.


Grazie ancora!
