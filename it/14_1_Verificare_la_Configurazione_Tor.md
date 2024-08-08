# 14.1: Verificare la Configurazione di Tor

> :information_source: **NOTA:** Questa sezione è stata recentemente aggiunta al corso ed è una bozza preliminare che potrebbe essere ancora in attesa di revisione. Lettore avvisato.

Se hai effettuato un'installazione standard con [Bitcoin Standup](https://github.com/BlockchainCommons/Bitcoin-Standup), dovresti avere Tor configurato come parte del tuo nodo Bitcoin: Tor è installato e ha creato servizi nascosti per le porte RPC di Bitcoin; mentre un indirizzo `onion` è stato creato anche per `bitcoind`. Questa sezione parla di cosa sia tutto questo e cosa fare con esso.

> :book: ***Cos'è Tor?*** Tor è una rete di anonimato a bassa latenza e sovrapposizione basata sul routing a cipolla e sulla progettazione di costruzione del percorso per consentire la comunicazione anonima. È un software libero e open-source il cui nome deriva dall'acronimo del nome del progetto software originale: "The Onion Router".

> :book: ***Perché Usare Tor per Bitcoin?*** La rete Bitcoin è una rete peer-to-peer che ascolta le transazioni e le propaga utilizzando un indirizzo IP pubblico. Quando ci si connette alla rete senza usare Tor, si condividerebbe il proprio indirizzo IP, il che potrebbe esporre la propria posizione, il tempo di attività e altri dettagli a terze parti — una pratica di privacy indesiderabile. Per proteggerti online dovresti usare strumenti come Tor per nascondere i dettagli della tua connessione. Tor ti consente di migliorare la tua privacy online poiché i tuoi dati sono crittograficamente codificati e passano attraverso diversi nodi, ciascuno dei quali decodifica un singolo strato (da qui la metafora della cipolla).

## Comprendere Tor

Quindi, come funziona Tor?

Quando un utente vuole connettersi a un server Internet, Tor cerca di costruire un percorso formato da almeno tre nodi di relay Tor, chiamati Guard, Middle e Exit. Durante la costruzione di questo percorso, vengono negoziate chiavi di crittografia simmetriche; quando un messaggio si sposta lungo il percorso, ogni relay rimuove il proprio strato di crittografia. In questo modo, il messaggio arriva alla destinazione finale nella sua forma originale, e ogni parte conosce solo l'hop precedente e successivo e non può determinare l'origine o la destinazione.

Ecco come appare una connessione senza Tor:
```
20:58:03.804787 IP bitcoin.36300 > lb-140-82-114-25-iad.github.com.443: Flags [P.], seq 1:30, ack 25, win 501, options [nop,nop,TS val 3087919981 ecr 802303366], length 29
```
Al contrario, con Tor viene trasmessa molta meno informazione sulle macchine effettive:
```
21:06:52.744602 IP bitcoin.58776 > 195-xxx-xxx-x.rev.pxxxxxm.eu.9999: Flags [P.], seq 264139:265189, ack 3519373, win 3410, options [nop,nop,TS val 209009853 ecr 3018177498], length 1050
21:06:52.776968 IP 195-xxx-xxx-x.rev.pxxxxxm.eu.9999 > bitcoin.58776: Flags [.], ack 265189, win 501, options [nop,nop,TS val 3018177533 ecr 209009853], length 0
```
Conclusione: Tor crittografa i tuoi dati in modo tale da nascondere la tua origine, la tua destinazione e quali servizi stai utilizzando, mentre un protocollo di crittografia standard come TLS *protegge solo* il contenuto dei tuoi dati.

### Comprendere l'Architettura della Rete Tor

L'architettura di base della rete Tor è composta dai seguenti componenti:

* **Tor Client (OP o Onion Proxy).** Un client Tor installa software locale che funge da onion proxy. Esso impacchetta i dati dell'applicazione in celle tutte della stessa dimensione (512 byte), che poi invia alla rete Tor. Una cella è l'unità base di trasmissione di Tor.
* **Onion Node (OR o Onion Router).** Un onion node trasmette celle provenienti dal client Tor e dai server online. Ci sono tre tipi di onion node: input (Guard), intermediate nodes (Middle) e output nodes (Exit).
*  **Directory Server.** Un server di directory memorizza informazioni sugli onion routers e sugli onion servers (servizi nascosti), come le loro chiavi pubbliche.
*  **Onion Server (hidden server).** Un onion server supporta applicazioni TCP come pagine web o IRC come servizi.

### Comprendere le Limitazioni di Tor

Tor non è uno strumento perfetto. Poiché le informazioni dalla rete Tor vengono decrittate ai nodi di uscita prima di essere inviate alle destinazioni finali, teoricamente un osservatore potrebbe raccogliere metadati sufficienti per compromettere l'anonimato e potenzialmente identificare gli utenti.

Ci sono anche studi che suggeriscono che possibili exploit della protezione anti-DoS di Bitcoin potrebbero consentire a un attaccante di costringere altri utenti che usano Tor a connettersi esclusivamente tramite i suoi nodi di uscita Tor o ai suoi peer Bitcoin, isolando il client dal resto della rete Bitcoin ed esponendoli a censura, correlazione e altri attacchi.

Allo stesso modo, gli utenti Bitcoin Tor potrebbero essere attaccati con fingerprint impostando un cookie di indirizzo sui loro nodi. Ciò consentirebbe anche la correlazione e quindi la deanonymization.

Nel frattempo, anche su Tor, Bitcoin è solo un servizio pseudonimo a causa dei molti pericoli di correlazione derivanti dal ledger permanente stesso. Ciò significa che l'uso di Bitcoin su Tor è in realtà più probabile essere _deanonimizzato_ rispetto ad altri servizi (e potrebbe portare alla deanonymization di altre attività).

Detto questo, Tor è generalmente considerato più sicuro dell'alternativa, che è la navigazione non anonima.

## Verificare la Configurazione di Tor

Quindi, come verifichi di aver abilitato Tor? Se hai installato con Bitcoin Standup, quanto segue verificherà che Tor sia in esecuzione sul tuo sistema:

```
$ sudo -u debian-tor tor --verify-config
```


Se Tor è installato correttamente, dovresti ottenere un output simile a questo:
```
Jun 26 21:52:09.230 [notice] Tor 0.4.3.5 running on Linux with Libevent 2.0.21-stable, OpenSSL 1.0.2n, Zlib 1.2.11, Liblzma 5.2.2, and Libzstd N/A.
Jun 26 21:52:09.230 [notice] Tor can't help you if you use it wrong! Learn how to be safe at https://www.torproject.org/download/download#warning
Jun 26 21:52:09.230 [notice] Read configuration file "/etc/tor/torrc".
Configuration was valid
```
> :warning: **ATTENZIONE:** Questo significa solo che Tor è in esecuzione, non che sia utilizzato per tutte (o qualsiasi) connessioni.

### Verificare la Configurazione di Tor per RPC

Lo scopo più importante di Tor, come installato da Bitcoin Standup, è offrire servizi nascosti per le porte RPC che vengono utilizzate per inviare comandi in stile linea di comando a `bitcoind`.

> :book: ***Cos'è un Servizio Nascosto di Tor?*** Un servizio nascosto (alias "onion service") è un servizio accessibile tramite Tor. La connessione effettuata a quel servizio _utilizzando la Rete Onion_ sarà anonima.

Il file di configurazione di Tor si trova in `/etc/tor/torrc`. Se lo guardi, dovresti vedere i seguenti servizi per proteggere le tue porte RPC:

```
HiddenServiceDir /var/lib/tor/standup/
HiddenServiceVersion 3
HiddenServicePort 1309 127.0.0.1:18332
HiddenServicePort 1309 127.0.0.1:18443
HiddenServicePort 1309 127.0.0.1:8332
```
> :link: **TESTNET vs MAINNET:** L'RPC del Mainnet è eseguito sulla porta 8332, quello del testnet sulla porta 18332.

> :information_source: **NOTA:** La `HiddenServiceDir` è dove sono conservati tutti i file per questo particolare servizio. Se hai bisogno di cercare il tuo indirizzo onion, le chiavi di accesso o aggiungere client autorizzati, questo è il posto dove farlo!

Il modo semplice per testare il tuo Servizio Nascosto RPC è utilizzare l'[API QuickConnect](https://github.com/BlockchainCommons/Bitcoin-Standup/blob/master/Docs/Quick-Connect-API.md) integrata in Bitcoin Standup. Basta scaricare il codice QR trovato in `/qrcode.png` e scansionarlo utilizzando un wallet o un nodo che supporti QuickConnect, come [The Gordian Wallet](https://github.com/BlockchainCommons/FullyNoded-2). Quando scansionerai il QR, dovresti vedere il wallet sincronizzarsi con il tuo nodo; sta facendo ciò utilizzando i servizi nascosti RPC.

Il modo difficile per testare il tuo Servizio Nascosto RPC è inviare un comando `bitcoin-cli` con `torify`, che ti consente di tradurre un comando UNIX normale in un comando protetto da Tor. È difficile perché devi raccogliere tre pezzi di informazioni.

1. **La Porta del Servizio Nascosto.** Questo proviene da `/etc/tor/torrc/`. Per impostazione predefinita, è la porta 1309.
2. **Il Tuo Indirizzo Tor.** Questo si trova nel file `hostname` nella directory `HiddenServiceDir` definita in `/etc/tor/torrc`. Per impostazione predefinita, il file è quindi `/var/lib/tor/standup/hostname`. È protetto, quindi dovrai usare `sudo` per accedervi:

```
$ sudo more /var/lib/tor/standup/hostname
mgcym6je63k44b3i5uachhsndayzx7xi4ldmwrm7in7yvc766rykz6yd.onion
```
3. **La Tua Password RPC.** Questa si trova in `~/.bitcoin/bitcoin.conf`

Quando hai tutte queste informazioni, puoi emettere un comando `bitcoin-cli` utilizzando `torify` e specificando il `-rpcconnect` come il tuo indirizzo onion, il `-rpcport` come la tua porta del servizio nascosto e il `-rpcpassword` come la tua password:
```
$ torify bitcoin-cli -rpcconnect=mgcym6je63k44b3i5uachhsndayzx7xi4ldmwrm7in7yvc766rykz6yd.onion -rpcport=1309 -rpcuser=StandUp -rpcpassword=685316cc239c24ba71fd0969fa55634f getblockcount
```


### Verificare la Configurazione di Tor per Bitcoind

Bitcoin Standup garantisce anche che il tuo `bitcoind` sia configurato per comunicare opzionalmente su un indirizzo onion.

Puoi verificare la configurazione iniziale di Tor per `bitcoind` cercando "tor" nel `debug.log` nella tua directory dei dati:

```
$ grep "tor:" ~/.bitcoin/testnet3/debug.log
2021-06-09T14:07:04Z tor: ADD_ONION successful
2021-06-09T14:07:04Z tor: Got service ID vazr3k6bgnfafmdpcmbegoe5ju5kqyz4tk7hhntgaqscam2qupdtk2yd, advertising service vazr3k6bgnfafmdpcmbegoe5ju5kqyz4tk7hhntgaqscam2qupdtk2yd.onion:18333
2021-06-09T14:07:04Z tor: Cached service private key to /home/standup/.bitcoin/testnet3/onion_v3_private_key
```
> :information_source: **NOTA:** Bitcoin Core non supporta più gli indirizzi v2. Il supporto per Tor v2 è stato rimosso in [#22050](https://github.com/bitcoin/bitcoin/pull/22050)

> **TESTNET vs MAINNET:** Il `bitcoind` del Mainnet risponde sulla porta 8333, quello del testnet sulla porta 18333.

Puoi verificare che sia stato creato un servizio nascosto Tor per Bitcoin con la chiamata RPC `getnetworkinfo`:

```
$ bitcoin-cli getnetworkinfo
...
 "localaddresses": [
    {
      "address": "173.255.245.83",
      "port": 18333,
      "score": 1
    },
    {
      "address": "2600:3c01::f03c:92ff:fe86:f26",
      "port": 18333,
      "score": 1
    },
    {
      "address": "vazr3k6bgnfafmdpcmbegoe5ju5kqyz4tk7hhntgaqscam2qupdtk2yd.onion",
      "port": 18333,
      "score": 4
    }
  ],
...
```
Questo mostra tre indirizzi per accedere al tuo server Bitcoin, un indirizzo IPv4 (`173.255.245.83`), un indirizzo IPv6 (`2600:3c01::f03c:92ff:fe86:f26`) e un indirizzo Tor (`vazr3k6bgnfafmdpcmbegoe5ju5kqyz4tk7hhntgaqscam2qupdtk2yd.onion`).

> :warning: **ATTENZIONE:** Ovviamente: non rivelare mai il tuo indirizzo Tor in modo associato al tuo nome o altri dati personali!

Puoi vedere informazioni simili con `getnetworkinfo`.

```
 bitcoin-cli getnetworkinfo
{
  "version": 200000,
  "subversion": "/Satoshi:0.20.0/",
  "protocolversion": 70015,
  "localservices": "0000000000000408",
  "localservicesnames": [
    "WITNESS",
    "NETWORK_LIMITED"
  ],
  "localrelay": true,
  "timeoffset": 0,
  "networkactive": true,
  "connections": 10,
  "networks": [
    {
      "name": "ipv4",
      "limited": false,
      "reachable": true,
      "proxy": "",
      "proxy_randomize_credentials": false
    },
    {
      "name": "ipv6",
      "limited": false,
      "reachable": true,
      "proxy": "",
      "proxy_randomize_credentials": false
    },
    {
      "name": "onion",
      "limited": false,
      "reachable": true,
      "proxy": "127.0.0.1:9050",
      "proxy_randomize_credentials": true
    }
  ],
  "relayfee": 0.00001000,
  "incrementalfee": 0.00001000,
  "localaddresses": [
    {
      "address": "173.255.245.83",
      "port": 18333,
      "score": 1
    },
    {
      "address": "2600:3c01::f03c:92ff:fe86:f26",
      "port": 18333,
      "score": 1
    },
    {
      "address": "vazr3k6bgnfafmdpcmbegoe5ju5kqyz4tk7hhntgaqscam2qupdtk2yd.onion",
      "port": 18333,
      "score": 4
    }
  ],
  "warnings": ""
}
```
Questo servizio nascosto consentirà connessioni anonime al tuo `bitcoind` sulla rete Bitcoin.

> :warning: **ATTENZIONE:** Eseguire Tor e avere un servizio nascosto Tor non obbliga né te né i tuoi peer a usare Tor.

### Verificare la Configurazione di Tor per i Peer

Utilizzando il comando RPC `getpeerinfo`, puoi vedere quali nodi sono connessi al tuo nodo e verificare se sono connessi con Tor.

.

```
$ bitcoin-cli getpeerinfo
```
Alcuni potrebbero essere connessi tramite Tor:

```
...
{
    "id": 9,
    "addr": "nkv.......xxx.onion:8333",
    "addrbind": "127.0.0.1:51716",
    "services": "000000000000040d",
    "servicesnames": [
      "NETWORK",
      "BLOOM",
      "WITNESS",
      "NETWORK_LIMITED"
    ],
    "relaytxes": true,
    "lastsend": 1593981053,
    "lastrecv": 1593981057,
    "bytessent": 1748,
    "bytesrecv": 41376,
    "conntime": 1593980917,
    "timeoffset": -38,
    "pingwait": 81.649295,
    "version": 70015,
    "subver": "/Satoshi:0.20.0/",
    "inbound": false,
    "addnode": false,
    "startingheight": 637875,
    "banscore": 0,
    "synced_headers": -1,
    "synced_blocks": -1,
    "inflight": [
    ],
    "whitelisted": false,
    "permissions": [
    ],
    "minfeefilter": 0.00000000,
    "bytessent_per_msg": {
      "addr": 55,
      "feefilter": 32,
      "getaddr": 24,
      "getheaders": 1053,
      "inv": 280,
      "ping": 32,
      "pong": 32,
      "sendcmpct": 66,
      "sendheaders": 24,
      "verack": 24,
      "version": 126
    },
    "bytesrecv_per_msg": {
      "addr": 30082,
      "feefilter": 32,
      "getdata": 280,
      "getheaders": 1053,
      "headers": 106,
      "inv": 9519,
      "ping": 32,
      "pong": 32,
      "sendcmpct": 66,
      "sendheaders": 24,
      "verack": 24,
      "version": 126
    }
  }
...
```
Alcuni potrebbero non esserlo, come questa connessione IPv6:
```
...
  {
    "id": 17,
    "addr": "[2001:638:a000:4140::ffff:191]:18333",
    "addrlocal": "[2600:3c01::f03c:92ff:fe86:f26]:36344",
    "addrbind": "[2600:3c01::f03c:92ff:fe86:f26]:36344",
    "services": "0000000000000409",
    "servicesnames": [
      "NETWORK",
      "WITNESS",
      "NETWORK_LIMITED"
    ],
    "relaytxes": true,
    "lastsend": 1595447081,
    "lastrecv": 1595447067,
    "bytessent": 12250453,
    "bytesrecv": 2298711417,
    "conntime": 1594836414,
    "timeoffset": -1,
    "pingtime": 0.165518,
    "minping": 0.156638,
    "version": 70015,
    "subver": "/Satoshi:0.20.0/",
    "inbound": false,
    "addnode": false,
    "startingheight": 1780784,
    "banscore": 0,
    "synced_headers": 1781391,
    "synced_blocks": 1781391,
    "inflight": [
    ],
    "whitelisted": false,
    "permissions": [
    ],
    "minfeefilter": 0.00001000,
    "bytessent_per_msg": {
      "addr": 4760,
      "feefilter": 32,
      "getaddr": 24,
      "getdata": 8151183,
      "getheaders": 1085,
      "headers": 62858,
      "inv": 3559475,
      "ping": 162816,
      "pong": 162816,
      "sendcmpct": 132,
      "sendheaders": 24,
      "tx": 145098,
      "verack": 24,
      "version": 126
    },
    "bytesrecv_per_msg": {
      "addr": 33877,
      "block": 2291124374,
      "feefilter": 32,
      "getdata": 9430,
      "getheaders": 1085,
      "headers": 60950,
      "inv": 2019175,
      "ping": 162816,
      "pong": 162816,
      "sendcmpct": 66,
      "sendheaders": 24,
      "tx": 5136622,
      "verack": 24,
      "version": 126
    }
  }
...
```
Avere un indirizzo Tor per il tuo `bitcoind` è probabilmente un po' meno utile che avere un indirizzo Tor per le tue connessioni RPC. Questo in parte perché non è consigliato cercare di inviare tutte le tue connessioni Bitcoin tramite Tor, e in parte perché proteggere i tuoi comandi RPC è davvero ciò che è importante: è molto più probabile che tu stia facendo ciò da remoto, da un wallet software come The Gordian Wallet, mentre il tuo server stesso è più probabilmente seduto nel tuo ufficio, seminterrato o bunker.

Tuttavia, ci sono modi per far usare Tor di più a `bitcoind`, come discusso nella prossima sezione.

## Riepilogo: Verificare la Configurazione di Tor

Tor è un pacchetto software installato come parte di Bitcoin Standup che ti consente di scambiare comunicazioni in modo anonimo. Proteggerà sia le tue porte RPC (8332 o 18332) che le tue porte `bitcoind` (8333 o 18333) — ma devi collegarti attivamente all'indirizzo onion per usarle! Tor è una pietra miliare della privacy e della sicurezza per il tuo setup Bitcoin, e puoi verificare che sia disponibile e collegato a Bitcoin con pochi semplici comandi.

> :fire: ***Qual è il potere di Tor?*** Molti attacchi agli utenti Bitcoin dipendono dal sapere chi è la vittima e che stanno transando Bitcoin. Tor può proteggerti da ciò nascondendo sia dove ti trovi che cosa stai facendo. È particolarmente importante se vuoi connetterti al tuo nodo da remoto tramite un wallet software, e può essere cruciale se lo fai in qualche paese dove potresti non sentirti che il tuo utilizzo di Bitcoin sia apprezzato o protetto. Se devi portare i tuoi servizi Bitcoin in viaggio, assicurati che il tuo wallet supporti pienamente Tor e scambi tutti i comandi RPC con il tuo server utilizzando quel protocollo.

## Cosa c'è Dopo?

Continua "Comprendere Tor" con [Capitolo 14.2: Cambiare Bitcoin Hidden Services](14_2_Cambiare_Bitcoin_Hidden_Services.md).

