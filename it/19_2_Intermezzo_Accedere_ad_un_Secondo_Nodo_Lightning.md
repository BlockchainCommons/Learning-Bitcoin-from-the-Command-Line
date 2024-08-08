# Intermezzo: Accedere ad un Secondo Nodo Lightning

> :information_source: **NOTE:** Questa sezione è stata recentemente aggiunta al corso ed è una bozza iniziale che potrebbe essere ancora in attesa di revisione. Attenzione lettore.

Quando hai giocato con Bitcoin, stavi accedendo a una rete esistente, e questo ha reso relativamente facile lavorarci: basta accendere `bitcoind` e interagisci immediatamente con la rete. Questo non è il modo in cui funziona Lightning: è fondamentalmente una rete peer-to-peer, costruita dalle connessioni tra due nodi individuali. In altre parole, per interagire con la rete Lightning, devi prima trovare un nodo a cui connetterti.

Ci sono quattro modi per farlo (i primi tre dei quali sono possibili per la tua prima connessione):

## Chiedere Informazioni su un Nodo

Se qualcun altro ha già un nodo Lightning sulla rete di tua scelta, chiedi loro il loro ID.

Se stanno eseguendo c-lightning, devono solo usare il comando `getinfo`:



```
$ lightning-cli getinfo
lightning-cli: WARNING: default network changing in 2020: please set network=testnet in config!
   "id": "03240a4878a9a64aea6c3921a434e573845267b86e89ab19003b0c910a86d17687",
   "alias": "VIOLETGLEE",
   "color": "03240a",
   "num_peers": 0,
   "num_pending_channels": 0,
   "num_active_channels": 0,
   "num_inactive_channels": 0,
   "address": [
      {
         "type": "ipv4",
         "address": "74.207.240.32",
         "port": 9735
      }
   ],
   "binding": [
      {
         "type": "ipv6",
         "address": "::",
         "port": 9735
      },
      {
         "type": "ipv4",
         "address": "0.0.0.0",
         "port": 9735
      }
   ],
   "version": "v0.9.1-96-g6f870df",
   "blockheight": 1862854,
   "network": "testnet",
   "msatoshi_fees_collected": 0,
   "fees_collected_msat": "0msat",
   "lightning-dir": "/home/standup/.lightning/testnet"
}
```

Poi possono dirti il loro `id` (`03240a4878a9a64aea6c3921a434e573845267b86e89ab19003b0c910a86d17687`). Dovranno anche dirti il loro indirizzo IP (`74.207.240.32`) e la porta (`9735`).

## Creare un Nuovo Nodo c-lightning

Tuttavia, per scopi di test, probabilmente vorrai avere un secondo nodo sotto il tuo controllo. Il modo più semplice per farlo è creare un secondo nodo c-lightning su una nuova macchina, utilizzando Bitcoin Standup, vedi il [Capitolo 2.1](02_1_Configurare_Bitcoin-Core_VPS_con_StackScript.md) o compilandolo a mano, vedi il [Capitolo 19.1](19_1_Verificare_la_Configurazione_Lightning.md).

Una volta che il tuo nodo è in esecuzione, puoi eseguire `getinfo` per recuperare le tue informazioni, come mostrato sopra.

## Creare un Nuovo Nodo LND

Tuttavia, per i nostri esempi nel prossimo capitolo, creeremo invece un nodo LND. Questo ci permetterà di dimostrare un po' della profondità dell'ecosistema Lightning mostrando come funzionano comandi simili sulle due diverse piattaforme.

Un modo per creare un nodo LND è eseguire di nuovo gli Script Bitcoin Standup su una nuova macchina, ma questa volta scegliere LND, vedi il [Capitolo 2.1](02_1_Configurare_Bitcoin-Core_VPS_con_StackScript.md).

Un altro modo è compilare LND dal codice sorgente su una macchina dove stai già eseguendo un nodo Bitcoin, come segue.

### Compilare il Codice Sorgente di LND

Prima, devi scaricare e installare Go:


```
$ wget --progress=bar:force https://dl.google.com/go/"go1.14.4"."linux"-"amd64".tar.gz -O ~standup/"go1.14.4"."linux"-"amd64".tar.gz
$ /bin/tar xzf ~standup/"go1.14.4"."linux"-"amd64".tar.gz -C ~standup
$ sudo mv ~standup/go /usr/local
```


Assicurati che la versione di Go sia la più aggiornata (è `go1.14.4` al momento attuale), e che la piattaforma e l'architettura siano corrette per la tua macchina. (Quanto sopra funzionerà per Debian.)

Aggiorna il tuo percorso:


```
$ export GOPATH=~standup/gocode
$ export PATH="$PATH":/usr/local/go/bin:"$GOPATH"/bin
```


Poi assicurati che `go` funzioni:


```
$ go version
go version go1.14.4 linux/amd64
```


Avrai anche bisogno di `git` e `make`:


```
$ sudo apt-get install git
$ sudo apt-get install build-essential
```


Ora sei pronto per recuperare LND. Assicurati di ottenere la versione corrente (attualmente `v0.11.0-beta.rc4`).


```
$ go get -d github.com/lightningnetwork/lnd
```


E ora puoi compilare:


```
$ cd "$GOPATH"/src/github.com/lightningnetwork/lnd
$ git checkout v0.11.0-beta.rc4
$ make
$ make install

```
Questo verrà installato in `~/gocode/bin`, che è `$GOPATH/bin`.

Dovresti spostarlo nelle directory globali:



```
$ sudo cp $GOPATH/bin/lnd $GOPATH/bin/lncli /usr/bin
```


### Creare un File di Configurazione LND

A differenza di c-lightning, dovrai creare un file di configurazione predefinito per LND.

Tuttavia, prima, devi abilitare ZMQ sul tuo Bitcoind, se non l'hai già fatto nel [Capitolo 16.3](16_3_Ricevere_Notifiche_di_Bitcoind_in_C_tramite_Librerie_ZMQ.md).

Questo richiede di aggiungere il seguente al tuo file `~/.bitcoin/bitcoin.conf` se non è già lì:



```
zmqpubrawblock=tcp://127.0.0.1:28332
zmqpubrawtx=tcp://127.0.0.1:28333
```


Se stai usando un file di configurazione Bitcoin da Standup o un altro `conf` specializzato, assicurati di mettere i tuoi nuovi comandi nella sezione corretta. Idealmente, dovrebbero andare vicino alla parte superiore del file, altrimenti nella sezione `[test]` (supponendo, come al solito, che stai testando su testnet).

Devi quindi riavviare bitcoin (o semplicemente riavviare la tua macchina). Puoi testare che stia funzionando come segue:


```
$ bitcoin-cli getzmqnotifications
[
  {
    "type": "pubrawblock",
    "address": "tcp://127.0.0.1:28332",
    "hwm": 1000
  },
  {
    "type": "pubrawtx",
    "address": "tcp://127.0.0.1:28333",
    "hwm": 1000
  }
]
```


Ora sei pronto per creare un file di configurazione.

Prima, devi recuperare il tuo rpcuser e rpcpassword. Ecco un modo automatico per farlo:



```
$ BITCOINRPC_USER=$(cat ~standup/.bitcoin/bitcoin.conf | grep rpcuser | awk -F = '{print $2}')
$ BITCOINRPC_PASS=$(cat ~standup/.bitcoin/bitcoin.conf | grep rpcpassword | awk -F = '{print $2}')
```


> :warning: **WARNING:** Ovviamente, non memorizzare mai la tua password RPC in una variabile di shell in un ambiente di produzione.

Poi, puoi scrivere il file:



```
$ mkdir ~/.lnd
$ cat > ~/.lnd/lnd.conf << EOF
[Application Options]
maxlogfiles=3
maxlogfilesize=10
#externalip=1.1.1.1 # change to your public IP address if required.
alias=StandUp
listen=0.0.0.0:9735
debuglevel=debug
[Bitcoin]
bitcoin.active=1
bitcoin.node=bitcoind
bitcoin.testnet=true
[Bitcoind]
bitcoind.rpchost=localhost
bitcoind.rpcuser=$BITCOINRPC_USER
bitcoind.rpcpass=$BITCOINRPC_PASS
bitcoind.zmqpubrawblock=tcp://127.0.0.1:28332
bitcoind.zmqpubrawtx=tcp://127.0.0.1:28333
EOF
```


### Creare un Servizio LND

Infine, puoi creare un servizio LND per eseguire automaticamente `lnd`:



```
$ cat > ~/lnd.service << EOF
# It is not recommended to modify this file in-place, because it will
# be overwritten during package upgrades. If you want to add further
# options or overwrite existing ones then use
# $ systemctl edit lnd.service
# See "man systemd.service" for details.
# Note that almost all daemon options could be specified in
# /etc/lnd/lnd.conf, except for those explicitly specified as arguments
# in ExecStart=
[Unit]
Description=LND Lightning Network Daemon
Requires=bitcoind.service
After=bitcoind.service
[Service]
ExecStart=/usr/bin/lnd
ExecStop=/usr/bin/lncli --lnddir /var/lib/lnd stop
PIDFile=/run/lnd/lnd.pid
User=standup
Type=simple
KillMode=process
TimeoutStartSec=60
TimeoutStopSec=60
Restart=always
RestartSec=60
[Install]
WantedBy=multi-user.target
EOF
```
> (Non è consigliato modificare questo file sul posto, perché verrà
sovrascritto durante gli aggiornamenti dei pacchetti. Se vuoi aggiungere ulteriori
opzioni o sovrascrivere quelle esistenti, utilizza
$ systemctl edit lnd.service
Vedi "man systemd.service" per dettagli.
Nota che quasi tutte le opzioni del demone potrebbero essere specificate in
/etc/lnd/lnd.conf, eccetto quelle esplicitamente specificate come argomenti)


Poi devi installarlo e avviarlo:


```
$ sudo cp ~/lnd.service /etc/systemd/system
$ sudo systemctl enable lnd
$ sudo systemctl start lnd
```
(Prevedi che questo richieda un minuto la prima volta.)

### Abilitare Connessioni Remote

Proprio come con c-lightning, dovrai rendere LND accessibile ad altri nodi. Ecco come farlo se usi `ufw`, come nei setup Bitcoin Standup:



```
$ sudo ufw allow 9735
```


### Creare un Wallet

La prima volta che esegui LND, devi creare un wallet:



```
$ lncli --network=testnet create
```


LND ti chiederà una password e poi se vuoi inserire una mnemonic esistente (basta premere `n` per quest'ultima).

Ora dovresti avere un `lnd` funzionante, che puoi verificare con `getinfo`:



```
$ lncli --network=testnet getinfo
{
    "version": "0.11.0-beta.rc4 commit=v0.11.0-beta.rc4",
    "commit_hash": "fc12656a1a62e5d69430bba6e4feb8cfbaf21542",
    "identity_pubkey": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
    "alias": "StandUp",
    "color": "#3399ff",
    "num_pending_channels": 0,
    "num_active_channels": 0,
    "num_inactive_channels": 0,
    "num_peers": 2,
    "block_height": 1862848,
    "block_hash": "000000000000000ecb6fd95e1f486283d48683aa3111b6c23144a2056f5a1532",
    "best_header_timestamp": "1602632294",
    "synced_to_chain": true,
    "synced_to_graph": false,
    "testnet": true,
    "chains": [
        {
            "chain": "bitcoin",
            "network": "testnet"
        }
    ],
    "uris": [
    ],
    "features": {
        "0": {
            "name": "data-loss-protect",
            "is_required": true,
            "is_known": true
        },
        "5": {
            "name": "upfront-shutdown-script",
            "is_required": false,
            "is_known": true
        },
        "7": {
            "name": "gossip-queries",
            "is_required": false,
            "is_known": true
        },
        "9": {
            "name": "tlv-onion",
            "is_required": false,
            "is_known": true
        },
        "13": {
            "name": "static-remote-key",
            "is_required": false,
            "is_known": true
        },
        "15": {
            "name": "payment-addr",
            "is_required": false,
            "is_known": true
        },
        "17": {
            "name": "multi-path-payments",
            "is_required": false,
            "is_known": true
        }
    }
}
```


L'ID di questo nodo è `032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543`. Sebbene questo comando non mostri l'indirizzo IP e la porta, dovrebbero essere l'indirizzo IP della tua macchina e la porta `9735`.

## Ascoltare i Gossip

Se eri già connesso alla rete Lightning e stavi "spettegolando" con i peer, potresti anche essere in grado di trovare informazioni sui peer automaticamente, tramite il comando `listpeers`:



```       
c$ lightning-cli --network=testnet listpeers
{
   "peers": [
      {
         "id": "0302d48972ba7eef8b40696102ad114090fd4c146e381f18c7932a2a1d73566f84",
         "connected": true,
         "netaddr": [
            "127.0.0.1:9736"
         ],
         "features": "02a2a1",
         "channels": []
      }
   ]
}
```       


Tuttavia, sicuramente non sarà il caso della tua prima interazione con la rete Lightning.

## Sommario: Accesso a un Secondo Nodo Lightning

Hai sempre bisogno di due nodi Lightning per formare un canale. Se non hai qualcun altro che sta testando con te, dovrai creare un secondo nodo, utilizzando c-lightning o (come faremo nei nostri esempi) LND.

## Cosa Succede Dopo?

Anche se hai eventualmente creato un LND, c-lightning rimarrà il cuore dei nostri esempi fino a quando non sarà necessario usare entrambi, in [Capitolo 19](19_0_Understanding_Your_Lightning_Setup.md).

Continua a "Understanding Your Lightning Setup" nel [Capitolo 19.3: Creare un Canale in Lightning](19_3_Creare_un_Canale_in_Lightning.md).

