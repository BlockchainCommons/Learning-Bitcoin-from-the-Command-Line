# 2.1: Configurando Bitcoin-Core VPS con StackScript

Questo documento spiega come configurare un VPS (Virtual Private Sever) per avviare un nodo Bitcoin su Linode.com, installato utilizzando uno StackScript automatizzato del [Progetto Bitcoin Standup](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts). Devi solo inserire alcuni comandi e avviare il tuo VPS. Quasi subito dopo l'avvio, scoprirai che il tuo nuovo nodo Bitcoin scarica i blocchi.

> :warning: **Pericolo:** Non utilizzare un VPS per un portafoglio bitcoin con fondi reali significativi; Vedere [Perdere facilmente Bitcoin](http://blog.thestateofme.com/2012/03/03/lessons-to-be-learned-from-the-linode-bitcoin-incident/ ). È molto bello poter sperimentare con transazioni bitcoin reali su un nodo live senza vincolare un server proprio su una rete locale. È anche utile essere in grado di utilizzare un iPhone o un iPad per comunicare tramite SSH con il tuo VPS per svolgere alcune semplici attività bitcoin. Ma è necessario un livello di sicurezza più elevato per fondi significativi.

* Se vuoi capire l'utilità di questo setup, leggi [Appendice I: Comprendere Bitcoin Standup.md](A1_0_Comprendere_Bitcoin_Standup.md) mentre procedi con l'installazione.
* Se invece vuole eseguire la configurazione su una macchina diversa da un VPS Linode, come una macchina AWS o un Mac, vai a [Capitolo 2.2: Configurare Bitcoin-Core con altri mezzi](02_2_Configurare_Bitcoin_Core_Altri_Mezzi.md)
* Se hai già un nodo Bitcoin funzionando, vai a [Capitolo 3: Capire le configurazioni di Bitcoin](03_0_Comprendere_le_Configurazioni_di_Bitcoin.md).

## Iniziare con Linode

Linode è un servizio di Cloud Hosting che offre server Linux veloci ed economici con archiviazione SSD. Li usiamo per questo tutorial principalmente perché i loro StackScripts basati su BASH offrono un modo semplice per configurare automaticamente un nodo Bitcoin senza problemi.

### Configurando un account Linode

Vai a questo link per creare un account Linode:

```
https://www.linode.com
```

Se preferisci, il seguente codice di riferimento ti darà circa un mese di utilizzo gratuito, ottimo per imparare Bitcoin:
Dovrai fornire un indirizzo email e successivamente precaricare denaro da una carta di credito o PayPal per eventuali costi futuri.

```
https://www.linode.com/?r=23211828bc517e2cb36e0ca81b91cc8c0e1b2d96
```

Quando avrai finito, dovresti arrivare a [https://cloud.linode.com/dashboard](https://cloud.linode.com/dashboard).

### Considera abilitare la autenticazione a 2 fattori (Two-Factor Authentication)

Il tuo server non sara al sicuro se le persone riescono ad accedere al tuo account Linode, quindi valuta la possibilità di impostare l'autenticazione a due fattori per questo. Puoi trovare questa impostazione nella tua pagina [Il mio profilo: password e autenticazione](https://manager.linode.com/profile/auth). Se non lo fai adesso, imposta un promemoria e fallo più tardi.

## Crea una immmagine Linode utilizando StackScript

### Carica lo StackScript

Scarica [Linode Standup Script](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts/blob/master/Scripts/LinodeStandUp.sh) da [Bitcoin Standup Scripts repo](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts). Questo script sostanzialmente automatizza tutte le istruzioni di configurazione di Bitcoin VPS. Se vuoi essere particolarmente prudente, leggilo attentamente. Se sei soddisfatto, puoi copiare StackScript nel tuo account andando alla [pagina StackScripts](https://cloud.linode.com/stackscripts?type=account) nel tuo account Linode account e scegliere [Crea Nuovo Stackscript](https://cloud.linode.com/stackscripts/create). Dategli un nome (noi usiamo `Bitcoin Standup`), poi copia e incolla lo script. Scegli Debian 10 come immagine di destinazione e "Salva"..

### Configurazione iniziale

Ora sei pronto per creare un Nodo basato su Stackscript.

1. Nella pagina [Stackscripts page](https://cloud.linode.com/stackscripts?type=account), clicca i tre puntini "..." nella parte destra del tuo nuovo script e seleziona "Deploy New Linode".
2. Inserisci un nome host breve e attinente
   * **Nome host breve.** Scegli un nome per il tuo VPS. Ad esempio, "mybtctest".
   * **Nome host completo.** Se hai intenzione di includere questo VPS come parte di una rete con record DNS completi, digita il nome host con il suo dominio. Ad esempio, "mybtctest.mydomain.com". Altrimenti, ripeti semplicemente il nome host breve e aggiungi ".local", ad esempio "mybtctest.local".
3. Immettere la password per l'utente "standup".

4. Scegli un tipo di installazione nelle opzioni avanzate.
* **Tipo di installazione.** Probabilmente è "Mainnet" o "Pruned Mainnet" se stai impostando un nodo per l'utilizzo e "Testnet" o "Pruned Testnet" se stai solo giocando. La maggior parte di questo tutorial darà per scontato che tu abbia scelto "Pruned Testnet", ma dovresti comunque essere in grado di seguire altri tipi. Consulta la [Sinossi](#sinossi-tipi-di-installazione-di-bitcoin) per maggiori informazioni su queste opzioni. (Nota che se hai intenzione di provare i capitoli Lightning, probabilmente vorrai usare un nodo Unpruned, non potato, poiché lavorare con nodi Pruned su Lightning è incerto. Consulta [Capitolo 19](19_0_Comprendere_La_Configurazione_Lightning.md) per i dettagli.)



  
* **Chiave pubblica X25519.** Questa è una chiave pubblica da aggiungere all'elenco dei client autorizzati di Tor. Se non la usi, chiunque ottenga il codice QR per il tuo nodo può accedervi. Otterrai questa chiave pubblica da qualsiasi client tu stia utilizzando per connetterti al tuo nodo. Ad esempio, se utilizzi [FullyNoded 2](https://github.com/BlockchainCommons/FullyNoded-2), puoi andare alle sue impostazioni e "Esporta chiave pubblica di autenticazione Tor V3" per utilizzarla qui.
* **Chiave SSH.** Copia qui la chiave SSH del tuo computer locale; questo ti consente di accedere automaticamente tramite SSH all'account standup. Se non hai ancora impostato una chiave SSH sul tuo computer locale, ci sono delle buone istruzioni su [Github](https://help.github.com/articles/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent/). Potresti anche voler aggiungere la tua chiave SSH nel tuo Linode LISH (Linode Interactive Shell) andando su "Linode Home Page / My Preferences / LISH Settings / LISH Keys". L'utilizzo di una chiave SSH ti darà un modo più semplice e sicuro per accedere al tuo server.
* **IP consentiti tramite SSH.** Questo è un elenco separato da virgole di IP a cui sarà consentito l'accesso tramite SSH al VPS. Ad esempio "192.168.1.15,192.168.1.16". Se non inserisci alcun IP, _il tuo VPS non sarà molto sicuro_. Sarà costantemente bombardato da aggressori che cercheranno di entrare e potrebbero anche riuscirci.
  
5. Select an Image
   * **Target Image.** If you followed the instructions, this will only allow you to select "Debian 10" (though "Debian 9" did also work with previous versions of this Stackscript and might still).
6. Choose a region for where the Linode will be located.

*The remaining questions all have to do with the mechanics of the VPS deployment and should be left as they are with one exception: bump the Swap Disk from 256MB to 512MB, to ensure that you have enough memory to download the blockchain._

Finally, you'll need to fill in a root password, which will be the password used for the root account.




5. Seleziona un'immagine
* **Immagine di destinazione.** Se hai seguito le istruzioni, questo ti consentirà di selezionare solo "Debian 10" (anche se "Debian 9" funzionava con le versioni precedenti di questo Stackscript, potrebbe ancora funzionare).
6. Scegli una regione in cui verrà posizionato Linode.

*Le domande rimanenti riguardano tutte la meccanica della distribuzione VPS e dovrebbero essere lasciate così come sono, con un'eccezione: aumenta lo Swap Disk da 256 MB a 512 MB, per assicurarti di avere abbastanza memoria per scaricare la blockchain._

Infine, dovrai inserire una password di root, che sarà la password utilizzata per l'account root.

### Scegli altre opzioni di Standup

Blockchain Commons è attualmente impegnata ad ampliare i suoi Bitcoin Standup Scripts con opzioni per installare Lightning e altre app Bitcoin degne di nota. Dai un'occhiata a tutte le opzioni extra e vedi se sono cose con cui ti piacerebbe giocare. In particolare, se Lightning è un'opzione, ti consigliamo di installarla, perché renderà molto più semplici [Capitolo 19](19_0_Comprendere_La_Configurazione_Lightning.md) e [Capitolo 20](20_0_Usare_Lightning.md).


### Scegli un piano Linode

Dovrai scegliere un piano Linode.

Un Linode da 4 GB sarà sufficiente per la maggior parte delle configurazioni, tra cui: Pruned Mainnet, Pruned Testnet e persino non-Pruned Testnet. Tutte utilizzano meno di 50 GB di spazio di archiviazione e 4 GB sono una quantità di memoria confortevole. Questa è la configurazione che suggeriamo. Costa $ 20 al mese.

Se invece desideri avere una Mainnet non-Pruned in un VPS, dovrai installare un Linode con un disco superiore a 800 G (!), che attualmente è il Linode da 16 GB, che ha 320 G di spazio di archiviazione e 16 G di memoria e costa circa $ 80 al mese. Noi _non_ lo consigliamo.

La seguente tabella mostra i requisiti minimi

| Setup | Memory | Storage | Linnode |
|-------|--------|---------|---------|
| Mainnet | 2G | 280G | Linode 16GB |
| Pruned Mainnet | 2G | ~5G | Linode 4GB |
| Testnet | 2G | ~15G | Linode 4GB |
| Pruned Testnet | 2G | ~5G | Linode 4GB |
| Regtest | 2G | ~ | Linode 4GB |

Nota, potrebbero esserci modi per ridurre entrambi i costi.

* Per le macchine che suggeriamo come **Linode 4 GB**, potresti essere in grado di ridurle a un Linode 2 GB. Alcune versioni di Bitcoin Core hanno funzionato bene a quella dimensione, alcune hanno occasionalmente esaurito la memoria e poi si sono riprese, e alcune hanno esaurito continuamente la memoria. Ricordati di aumentare quello spazio di swap per massimizzare le probabilità che funzioni. Usalo a tuo rischio e pericolo.
* Per la Mainnet non potata, che suggeriamo come **Linode 16 GB**, probabilmente puoi cavartela con un Linode 4 GB, ma aggiungi [Block Storage](https://cloud.linode.com/volumes) sufficiente per archiviare la blockchain. Questa è sicuramente una soluzione migliore a lungo termine poiché i requisiti di archiviazione della blockchain Bitcoin aumentano continuamente se non esegui il pruning, mentre i requisiti della CPU non lo fanno (o non lo fanno nella stessa misura). Uno storage da 320 GibiByte costerebbe 32 $ al mese, che combinato con un Linode da 4 GB è di 52 $ al mese, invece di 80 $, e cosa più importante, puoi continuare a farlo crescere. Non documentiamo completamente questa configurazione per due motivi (1) non suggeriamo la configurazione mainnet non potata, e quindi sospettiamo che sia una configurazione molto meno comune; e (2) non abbiamo testato come i volumi Linode si confrontano con i loro SSD intrinseci per prestazioni e utilizzo. Ma c'è una documentazione completa sulla pagina Block Storage. Dovresti configurare Linode, eseguire il suo stackscript, ma poi interromperlo per spostare eccessivamente lo storage blockchain su un volume appena commissionato prima di continuare.

### Esegui la configurazione finale

L'ultima cosa che devi fare è inserire una password di root. (Se hai dimenticato qualcosa, te lo diremo ora!)

Fai clic su "Deploy" per inizializzare i dischi e preparare il tuo VPS. L'intera coda dovrebbe essere eseguita in meno di un minuto. Quando è terminata, dovresti vedere nella "Host Job Queue" i pulsanti verdi "Success" che indicano "Disk Create from StackScript - Setting password for root… done." e "Create Filesystem - 256MB Swap Image".

Ora potresti voler cambiare il nome del tuo Linode VPS dal predefinito `linodexxxxxxxx`. Vai alla scheda Impostazioni e cambia l'etichetta in modo che sia più utile, come il nome host breve del tuo VPS. Ad esempio, potresti chiamarlo `bitcoin-testnet-pruned` per differenziarlo dagli altri VPS nel tuo account.

## Accedi al tuo VPS

Se guardi il tuo pannello di controllo Linode, dovresti vedere il nuovo computer avviarsi. Quando il lavoro avrà raggiunto il 100%, potrai effettuare l'accesso.

Per prima cosa, ti servirà l'indirizzo IP. Fai clic sulla scheda "Linodes" e dovresti vedere un elenco del tuo VPS, il fatto che sia in esecuzione, il suo "piano", il suo indirizzo IP e altre informazioni.

Vai alla tua console locale e accedi all'account `standup` utilizzando quell'indirizzo:




```
ssh standup@[IP-ADDRESS]
```

Per esempio:

```
ssh standup@192.168.33.11
```

Se hai configurato il tuo VPS per usare una chiave SSH, l'accesso dovrebbe essere automatico (potrebbe richiedere la tua password SSH per sbloccare la chiave). Se non hai configurato una chiave SSH, dovrai digitare la password user1.

### Attendi qualche minuto

Ecco un piccolo trucco: _il tuo StackScript è in esecuzione in questo momento_. Lo script BASH viene eseguito la prima volta che il VPS viene avviato. Ciò significa che il tuo VPS non è ancora pronto.

Il tempo di esecuzione totale è di circa 10 minuti. Quindi, fai una pausa, prenditi un cafferino rilassati per qualche minuto. Ci sono due parti dello script che richiedono un po' di tempo: l'aggiornamento di tutti i pacchetti Debian e il download del codice Bitcoin. Non dovrebbero richiedere più di 5 minuti ciascuna, il che significa che se torni tra 10 minuti, probabilmente sarai pronto per partire.

Se sei impaziente puoi andare avanti e digitare `sudo tail -f ~root/standup.log`, che visualizzerà l'avanzamento attuale dell'installazione, come descritto nella sezione successiva.

## Verifica la tua installazione

Saprai che stackscrpit è stato eseguito quando la `tail` di `standup.log` dice qualcosa del tipo:

```
/root/StackScript - Bitcoin is setup as a service and will automatically start if your VPS reboots and so is Tor
/root/StackScript - You can manually stop Bitcoin with: sudo systemctl stop bitcoind.service
/root/StackScript - You can manually start Bitcoin with: sudo systemctl start bitcoind.service
```

A quel punto, la tua directory home dovrebbe apparire così:

```
$ ls
bitcoin-0.20.0-x86_64-linux-gnu.tar.gz  laanwj-releases.asc  SHA256SUMS.asc
```

Questi sono i vari file che sono stati utilizzati per installare Bitcoin sul tuo VPS. _Nessuno_ di loro è necessario. Li abbiamo lasciati solo nel caso in cui volessi fare delle verifiche aggiuntive. Altrimenti, puoi eliminarli:

```
$ rm *
```

Verifica la configurazione di Bitcoin

Per garantire che la versione di Bitcoin scaricata sia valida, StackScript controlla sia la firma che il checksum SHA. Dovresti verificare che entrambi i test siano tornati corretti:

```
$ sudo grep VERIFICATION ~root/standup.log
```


Se vedi qualcosa di simile a quanto segue, tutto dovrebbe andare bene:

```
/root/StackScript - VERIFICATION SUCCESS / SIG: gpg: Good signature from "Wladimir J. van der Laan (Bitcoin Core binary release signing key) <laanwj@gmail.com>" [unknown]
/root/StackScript - VERIFICATION SUCCESS / SHA: 35ec10f87b6bc1e44fd9cd1157e5dfa4```
```
However, if either of those two checks instead reads "VERIFICATION ERROR", then there's a problem. Since this is all scripted, it's possible that there's just been a minor change that has caused the script's checks not to work right. (This has happened a few times over the existence of the script that became Standup.) But, it's also possible that someone is trying to encourage you to run a fake copy of the Bitcoin daemon. So, _be very sure you know what happened before you make use of Bitcoin!_


Tuttavia, se uno di quei due controlli invece dice "VERIFICATION ERROR", allora c'è un problema. Poiché tutto questo è scritto in uno script, è possibile che ci sia stata solo una piccola modifica che ha causato il malfunzionamento dei controlli dello script. (Questo è successo un paio di volte durante l'esistenza dello script che è diventato Standup.) Ma è anche possibile che qualcuno stia cercando di incoraggiarti a eseguire una copia falsa del Bitcoin daemon. Quindi, _assicurati di sapere cosa è successo prima di usare Bitcoin!_

### Leggi i log

Potresti anche voler leggere tutti i file di log di installazione, per assicurarti che non sia successo nulla di inaspettato durante l'installazione.

È meglio guardare il file di log standard di StackScript, che contiene tutti gli output, inclusi gli errori:

`$ sudo more ~root/standup.log`

Nota che è del tutto normale vedere _alcuni_ errori, in particolare quando si esegue il rumorosissimo software gpg e quando varie cose provano ad accedere al dispositivo inesistente `/dev/tty`.

Se invece vuoi guardare un set di informazioni più piccolo, tutti gli errori dovrebbero essere in:

`$ sudo more ~root/standup.err`

Questo continene ancora una discreta quantità di informazioni che non sono errori, ma è veloce da leggere.

Se tutto sembra a posto, congratulazioni, hai un nodo Bitcoin funzionante usando Linode!

## Cosa abbiamo fatto

Sebbene l'immagine Debian 10 predefinita che stiamo usando per il tuo VPS sia stata modificata da Linode per essere relativamente sicura, il tuo nodo Bitcoin installato tramite Linode StackScript è impostato con un livello di sicurezza ancora più elevato. Potresti trovare questo limitante o non essere in grado di fare le cose che ti aspetti. Ecco alcune note a riguardo:

### Servizi protetti

L'installazione del tuo VPS Bitcoin è minima e non consente quasi nessuna comunicazione. Ciò avviene tramite il firewall semplice (`ufw`), che blocca tutto tranne le connessioni SSH. C'è anche un po' di sicurezza aggiuntiva possibile per le tue porte RFC, grazie ai servizi nascosti installati da Tor.


**Regolazione di UFW.** Probabilmente dovresti lasciare UFW nella sua fase super protetta! Non vuoi usare una macchina Bitcoin per altri servizi, perché tutti aumentano la tua vulnerabilità! Se decidi diversamente, ci sono diverse [guide a UFW](https://www.digitalocean.com/community/tutorials/ufw-essentials-common-firewall-rules-and-commands) che ti permetteranno di aggiungere servizi. Come detto, non è complicato. Ad esempio, aggiungere servizi di posta richiederebbe solo l'apertura della porta di posta: `sudo ufw allow 25`. Ma non farlo.

**Regolazione di Tor.** Potresti voler proteggere meglio servizi come SSH. Consulta [Capitolo 14: Usare Tor](14_0_Usare_Tor.md) per maggiori informazioni su Tor.

### Shell protette

Se hai definito "IP consentiti tramite SSH", l'accesso SSH (e SCP) al server è severamente limitato. `/etc/hosts.deny` non consente a nessuno di effettuare l'accesso. _Non consigliamo di cambiare questo_. `/etc/hosts.allow` consente quindi indirizzi IP specifici. Aggiungi semplicemente altri indirizzi IP in un elenco separato da virgole se hai bisogno di offrire più accesso.

Ad esempio:

```
sshd: 127.0.0.1, 192.128.23.1
```

### Aggiornamenti automatici

Debian è anche impostato per aggiornarsi automaticamente, per garantire che rimanga al passo con le patch di sicurezza più recenti.

Se per qualche motivo volessi cambiare questa impostazione (_non lo consigliamo_), puoi fare così:

```
echo "unattended-upgrades unattended-upgrades/enable_auto_updates boolean false" | debconf-set-selections
```

_Se vuoi saperne di più su cosa fa lo stackscript di Bitcoin Standup, consulta [Appendice I: Comprendere Bitcoin Standup](A1_0_Comprendere_Bitcoin_Standup.md)._

## Giocare con Bitcoin

Quindi ora probabilmente vuoi giocare con Bitcoin!

Ma aspetta, il tuo Bitcoin daemon sta probabilmente ancora scaricando blocchi. `bitcoin-cli getblockcount` ti dirà come stai andando al momento:

```
$ bitcoin-cli getblockcount
1771352
```

Se è diverso ogni volta che digiti il comando, devi aspettare prima di lavorare con Bitcoin. Attualmente ci vogliono da 1 a 6 ore per una configurazione ridotta, a seconda della tua macchina.

Ma una volta che si stabilizza su un numero, sei pronto per continuare!

Tuttavia, potrebbe essere il momento di qualche altro caffè. Ma presto il tuo sistema sarà pronto per partire e sarai pronto per iniziare a sperimentare.





## Riepilogo: Impostazione manuale di un VPS Bitcoin-Core

Creare un VPS Bitcoin-Core con gli script Standup ha reso l'intero processo rapido, semplice e (si spera) indolore.

## Cosa c'è dopo?

Hai alcune opzioni per cosa c'è dopo:

* Leggi [StackScript](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts/blob/master/Scripts/LinodeStandUp.sh) per comprendere la tua configurazione.
* Leggi cosa fa StackScript in [Appendice I](A1_0_Comprendere_Bitcoin_Standup.md).
* Scegli una metodologia completamente alternativa in [Capitolo 2.2: Configurare Bitcoin Core Altri Mezzi](02_2_Configurare_Bitcoin_Core_Altri_Mezzi.md). * Passa a "bitcoin-cli" con [Capitolo 3: Comprendere le Configurazioni di Bitcoin](03_0_Comprendere_le_Configurazioni_di_Bitcoin.md).

## Sinossi: Tipi di installazione di Bitcoin

**Mainnet.** Questo scaricherà l'intera blockchain di Bitnet. Sono 800 G di dati (e aumentano ogni giorno).

**Pruned Mainnet.** Questo taglierà la blockchain che stai memorizzando fino agli ultimi 550 blocchi. Se non stai estraendo o eseguendo un altro servizio Bitcoin, questo dovrebbe essere più che sufficiente per la convalida.

**Testnet.** Questo ti dà accesso a una blockchain Bitcoin alternativa in cui i Bitcoin non hanno effettivamente valore. È pensato per la sperimentazione e il test.

**Pruned Testnet.** Questi sono solo gli ultimi 550 blocchi di Testnet... perché anche la blockchain di Testnet è piuttosto grande ora.

**Private Regtest.** Questa è la modalità di test di regressione, che ti consente di eseguire un server Bitcoin completamente locale. Permette di effettuare test ancora più approfonditi. Non c'è bisogno di potatura qui, perché si partirà da zero. Questa è una configurazione molto diversa, e quindi è trattata in [Appendice 3](A3_0_Usare_Bitcoin_Regtest.md).
