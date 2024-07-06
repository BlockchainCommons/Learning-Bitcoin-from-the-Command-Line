# Capitolo Uno: Introduzione all'apprendimento di Bitcoin Core (& Lightning) dalla linea di comando
## Introduzione

Le modalità di pagamento di beni e servizi sono cambiate radicalmente negli ultimi decenni. Mentre un tempo tutte le transazioni venivano effettuate in contanti o con assegni, oggi diversi metodi di pagamento elettronico sono la norma. Tuttavia, la maggior parte di questi pagamenti elettronici avviene ancora attraverso sistemi centralizzati, in cui le società di carte di credito, le banche o persino le istituzioni finanziarie basate su Internet come Paypal conservano lunghi elenchi di transazioni e individui e hanno il potere di censurare le transazioni che non gradiscono.

Questi rischi di centralizzazione sono stati alcuni dei principali catalizzatori della creazione delle criptovalute, la prima e la più importante delle quali è Bitcoin. Bitcoin offre pseudonimato, rende difficile la correlazione delle transazioni; e rende praticamente impossibile la censura da parte di singole entità. Questi vantaggi l'hanno resa una delle valute con la crescita più rapida al mondo. Questa crescita, a sua volta, ha fatto sì che Bitcoin diventasse d'interesse per imprenditori e sviluppatori, desiderosi di creare nuovi servizi per la comunità Bitcoin.

Se sei uno di questi imprenditori o sviluppatori, allora questo corso fa per te, perché si tratta di imparare a programmare Bitcoin. È un corso introduttivo che spiega tutte le sfumature e le caratteristiche di Bitcoin. Inoltre, adotta un approccio molto specifico, offrendo lezioni su come lavorare _direttamente_ con Bitcoin Core e con il server c-lightning utilizzando le loro interfacce RPC. (Remote Procedure Call)

Perché non utilizzare alcune delle librerie più complete presenti in vari linguaggi di programmazione? Perché non crearne una propria da zero? Perché lavorare con le criptovalute è pericoloso. Non ci sono reti di sicurezza. Se accidentalmente pagate commissioni troppo alte o perdete una chiave di firma o create una transazione non valida o commettete una serie di potenziali errori, perderete le vostre monete per sempre. Gran parte della responsabilità ricade ovviamente su di voi come programmatori di criptovalute, ma può essere minimizzata lavorando con le interfacce per criptovalute più robuste, sicure e protette in circolazione, quelle create dagli stessi team di programmazione delle criptovalute: ``bitcoind`` e ``lightningd``.

Gran parte di questo libro tratta quindi di come eseguire lo script di Bitcoin (e di Lightning) direttamente dalla riga di comando. Alcuni capitoli successivi trattano linguaggi di programmazione più sofisticati, ma continuano a interagire direttamente con i _demoni_ ``bitcoind`` e ``lightningd`` utilizzando RPC o interagendo con i file che creano. In questo modo si può salire sulle spalle dei giganti e utilizzare la loro tecnologia di fiducia per imparare a creare i propri sistemi di fiducia.

## Livello di competenza richiesto

Per la maggior parte di questo corso non è necessario essere particolarmente tecnici . Tutto ciò che serve è confidenza con i comandi di base sulla linea di comando UNIX. Se avete familiarità con comandi come `ssh`, `cd` e `ls`, il corso vi fornirà il resto.

Una parte minoritaria di questo corso richiede conoscenze di programmazione e, se necessario, si consiglia di saltare queste sezioni, come illustrato nella sezione successiva. 

## Panoramica degli argomenti

A grandi linee, questo libro è suddiviso nelle seguenti sezioni:

| Part | Description | Skills |
|-------|---------|---------|
| **Parte Uno: Preparazione per Bitcoin** | Comprendere le basi di Bitcoin e configurare un server per l’uso. | Riga di comando | 
| **Parte Due: Utilizzo di Bitcoin-CLI** | Utilizzo di Bitcoin-CLI per la creazione di transazioni. | Riga di comando |
| **Parte Tre: Bitcoin Scripting** | Espandere il tuo lavoro su Bitcoin utilizando gli script. | Concetti di programmazione |
| **Parte Quattro: Utilizzo di Tor** | Migliorare la sicurezza del tuo nodo con Tor | Riga di comando |
| **Parte Cinque: Programmazione con RPC** | Accesso a RPC da C e altri linguaggi. | Programmazione in C |
| **Parte Sei: Utilizzo di Lightning-CLI** | Utilizzo di Lightning-CLI per la creazione di transazioni. | Riga di comando |
| **Appendici** | Utilizzo di configurazioni Bitcoin meno comuni. | Riga di comando |

## Come usare questo corso

Da dove si comincia? Questo libro è stato pensato per essere letto in sequenza. Basta seguire i link "Cosa c'è dopo?" alla fine di ogni sezione e/o cliccare sui link delle singole sezioni in ogni pagina del capitolo. La migliore comprensione di questo corso si ottiene se si costruisce un server Bitcoin (come da Capitolo 2) e poi si esaminano tutti gli esempi nel corso del libro: provare gli esempi è un'eccellente metodologia di apprendimento.

Se avete diversi livelli di abilità o volete imparare cose diverse, potete passare a parti diverse del libro:

* Se si dispone già di un ambiente Bitcoin pronto per essere utilizzato, passa a [Capitolo Tre: Comprendere le Configurazioni di Bitcoin](03_0_Comprendere_le_Configurazioni_di_Bitcoin.md).
* Se vi interessa solo lo scripting di Bitcoin, passate a [Capitolo Nove: Introduzione script di Bitcoin](09_0_Introduzione_script_di_Bitcoin.md).
* Se volete solo leggere dell'uso dei linguaggi di programmazione, saltate a [Capitolo Sedici: Parlare con Bitcoind](16_0_Parlare_con_Bitcoind.md).
* Se invece non volete fare programmazione, saltate assolutamente i capitoli 15-17 durante la lettura e forse anche i capitoli 9-13. Il resto del corso dovrebbe avere senso anche senza di essi.
* Se siete interessati solo a Lightning, fate un salto su [Capitolo dicianove: Comprendere La Configurazione Lightning](19_0_Comprendere_La_Configurazione_Lightning.md).
* Se volete leggere i nuovi contenuti principali aggiunti per la v2 del corso (2020), dopo la v1 (2017), leggete [§3.5: Comprendere il Descriptor](03_5_Comprendere_il_Descriptor.md), [§4.6: Creare una Transazione Segwit](04_6_Creare_una_Transazione_Segwit.md), [Capitolo 7: Ampliare le Transazioni Bitcoin con PSBTs](07_0_Ampliare_le_Transazioni_Bitcoin_con_PSBTs.md), [§9.5: Programmare una P2WPKH](09_5_Programmare_una_P2WPKH.md), [§10.5: Scripting a SegWit Script](10_5_Scripting_a_Segwit_Script.md), [Chapter 14: Using Tor](14_0_Using_Tor.md), [Chapter 15: Using i2p](15_0_Using_i2p.md), [Chapter 16: Talking to Bitcoind with C](16_0_Talking_to_Bitcoind.md), [Chapter 17: Programming with Libwally](17_0_Programming_with_Libwally.md), [Chapter 18: Talking to Bitcoind with Other Languages](18_0_Talking_to_Bitcoind_Other.md), [Chapter 19: Understanding Your Lighting Setup](19_0_Understanding_Your_Lightning_Setup.md), and [Chapter 20: Using Lightning](20_0_Using_Lightning.md).

## Perché usare questo corso

Ovviamente, state seguendo questo corso perché siete interessati a Bitcoin. Oltre a fornire le conoscenze di base, ha anche aiutato i lettori a partecipare (o a creare) progetti open-source e a trovare lavoro nella programmazione di Bitcoin. Alcuni stagisti di Blockchain Commons hanno imparato a conoscere Bitcoin grazie a questo corso, così come alcuni membri del nostro team di programmazione.

## Come sostenere questo corso

* Si prega di usare [Problemi](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/issues) per qualsiasi domanda. Blockchain Commons non ha un team di supporto attivo e quindi non possiamo rispondere a problemi o domande individuali, ma le esamineremo nel tempo e le utilizzeremo per migliorare le future iterazioni del corso.
* Si prega di utilizzare [PRs](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/pulls) per correggere errori di battitura o comandi errati (o modificati). Per le modifiche tecniche o di riga di comando, è molto utile usare la sezione commenti della PR per spiegare perché si è agito in questo modo, in modo semplificare la comprensione della richiesta senza dover fare ricerche.
* Per favore usa la nostra [Area di discussione della comunità](https://github.com/BlockchainCommons/Community/discussions) per parlare di carriere e competenze. Blockchain Commons offre occasionalmente degli stage, come discusso nel nostro repo della Comunità.
* Per favore [Diventa mecenate](https://github.com/sponsors/BlockchainCommons) se questo corso ti è stato utile o se vuoi contribuire a formare la prossima generazione di programmatori blockchain.

## Cosa c'è dopo?

Se desiderate un'introduzione di base a Bitcoin, alla crittografia a chiave pubblica, all'ECC, alle blockchain e a Lightning, leggete il documento [Introduzione a Bitcoin](01_1_Introduzione_a_Bitcoin.md) interludio. 

Altrimenti, se siete pronti a immergervi nel corso, andate su [Configura una Bitcoin-Core VPS](02_0_Setting_Up_a_Bitcoin-Core_VPS.md).
