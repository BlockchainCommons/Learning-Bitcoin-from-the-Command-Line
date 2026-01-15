# 19.1: Verificare la Configurazione Lightning

> :information_source: **NOTA:** Questa sezione è stata recentemente aggiunta al corso ed è una bozza preliminare che potrebbe essere ancora in attesa di revisione. Lettore avvisato..

In questa sezione, installerai e verificherai c-lightning, il tuo strumento per accedere al Lightning Network.

> :book: ***Cos'è il Lightning Network?*** Il Lightning Network è una rete decentralizzata che utilizza la funzionalità dei contratti intelligenti della blockchain di Bitcoin per abilitare pagamenti istantanei tra una rete di partecipanti. Lightning è costruito come un protocollo di livello-2 che interagisce con Bitcoin per consentire agli utenti di scambiare i loro bitcoin "off-chain".

> :book: ***Cos'è un protocollo di livello-2?*** Il livello 2 si riferisce a un protocollo secondario costruito sopra il sistema della blockchain di Bitcoin. L'obiettivo principale di questi protocolli è risolvere i problemi di velocità delle transazioni e di scalabilità presenti in Bitcoin: Bitcoin non è in grado di elaborare migliaia di transazioni al secondo (TPS), quindi sono stati creati protocolli di livello-2 per risolvere il problema della scalabilità della blockchain. Queste soluzioni sono anche conosciute come soluzioni di scalabilità "off-chain".

## Installare C-Lightning

Se hai utilizzato i [Bitcoin Standup Scripts](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts), potresti aver già installato Lightning all'inizio di questo corso. Puoi verificarlo vedendo se `lightningd` è in esecuzione:


```
$ ps auxww | grep -i lightning
standup  31213  0.0  0.2  24144 10424 pts/0    S    15:38   0:00 lightningd --testnet
standup  31214  0.0  0.1  22716  7444 pts/0    S    15:38   0:00 /usr/local/bin/../libexec/c-lightning/plugins/autoclean
standup  31215  0.0  0.2  22992  8248 pts/0    S    15:38   0:00 /usr/local/bin/../libexec/c-lightning/plugins/bcli
standup  31216  0.0  0.1  22756  7604 pts/0    S    15:38   0:00 /usr/local/bin/../libexec/c-lightning/plugins/keysend
standup  31217  0.0  0.1  22776  7648 pts/0    S    15:38   0:00 /usr/local/bin/../libexec/c-lightning/plugins/pay
standup  31218  0.0  0.1  22720  7652 pts/0    S    15:38   0:00 /usr/local/bin/../libexec/c-lightning/plugins/txprepare
standup  31219  0.0  0.1  22744  7716 pts/0    S    15:38   0:00 /usr/local/bin/../libexec/c-lightning/plugins/spenderp
standup  31227  0.0  0.1  22748  7384 pts/0    SL   15:38   0:00 /usr/local/libexec/c-lightning/lightning_hsmd
standup  31228  0.0  0.2  23044  8192 pts/0    S    15:38   0:00 /usr/local/libexec/c-lightning/lightning_connectd
standup  31229  0.0  0.1  22860  7556 pts/0    S    15:38   0:00 /usr/local/libexec/c-lightning/lightning_gossipd
standup  32072  0.0  0.0   6208   888 pts/0    S+   15:50   0:00 grep -i lightning
```


In caso contrario, dovrai installarlo ora. Sfortunatamente, se stai usando Debian dovrai installarlo a mano, compilando il codice sorgente — ma dovrebbe comunque essere abbastanza semplice se segui queste istruzioni. Se per caso sei su un sistema Ubuntu standard, prova invece a [Installare da Ubuntu ppa](#variant-install-from-ubuntu-ppa), e puoi sempre tentare [Installare Binaries Precompilati](#variant-install-pre-compiled-binaries).

> :book: ***Cos'è c-lightning?*** Attualmente ci sono tre diverse implementazioni di Lightning: c-lightning, LND e Eclair. Dovrebbero essere tutte funzionalmente compatibili, basate sugli stessi [BOLT RFCs](https://github.com/lightningnetwork/lightning-rfc/blob/master/00-introduction.md), ma i dettagli dell'implementazione potrebbero essere diversi. Abbiamo scelto c-lightning come base del nostro corso perché fa anche parte dello stesso [Elements Project](https://github.com/ElementsProject) che contiene anche Libwally.

### Compilare il Codice Sorgente di c-lightning

Installare Lightning dal codice sorgente dovrebbe essere abbastanza semplice se segui queste istruzioni.

Probabilmente _vorrai_ farlo su un nodo non potato, poiché lavorare con nodi potato su Lightning potrebbe causare problemi con l'installazione e l'uso. Se hai impostato il tuo nodo all'inizio di questo corso per essere potato, potresti volerlo sostituire con un nodo non potato ora. (Se stai usando testnet, dovresti essere in grado di usare lo stesso tipo di macchina che hai usato per il tuo nodo potato.)

> :warning: **WARNING:** In realtà puoi eseguire c-lightning su un nodo potato. Tuttavia, come nota il [repo di Lightning](https://github.com/ElementsProject/lightning#pruning), potrebbero esserci problemi. Per farlo funzionare devi assicurarti che il tuo nodo Lightning stia cercando di aggiornare solo le informazioni sui blocchi che il tuo nodo Bitcoin non ha potato. Per farlo devi assicurarti (1) che il tuo nodo Bitcoin sia completamente aggiornato prima di avviare il tuo nodo Lightning per la prima volta; e (2) che il tuo nodo Lightning non rimanga troppo indietro rispetto al tuo nodo Bitcoin (per una potatura standard di 550 blocchi, non può essere spento per 4 o più giorni). Quindi, puoi farlo, ma introduce un certo rischio, il che non è una buona idea se stai eseguendo un servizio di produzione.

Detto ciò, sei pronto per installare Lightning:

Per prima cosa, installa le dipendenze, inclusi i requisiti di sviluppo.



```
$ sudo apt-get install -y \
   autoconf automake build-essential git libtool libgmp-dev \
   libsqlite3-dev python3 python3-mako net-tools zlib1g-dev libsodium-dev \
   gettext
$ sudo apt-get install -y valgrind python3-pip libpq-dev
```


Questi possono richiedere un po' di tempo, perché ce ne sono diversi, e alcuni sono grandi.

In secondo luogo, clona il repository di Lightning:



```
$ cd ~
$ git clone https://github.com/ElementsProject/lightning.git
$ cd lightning
```


Ora puoi usare il `pip3` che hai installato per installare ulteriori requisiti per la compilazione e configurare tutto:


```
$ pip3 install -r requirements.txt
$ ./configure
```


Ora, compila. Anche questo potrebbe richiedere del tempo a seconda della tua macchina.


```
$ make
```


Dopo, tutto quello che devi fare è installare:


```
$ sudo make install
```


## Controlla la Tua Installazione

Puoi confermare di aver installato correttamente lightningd usando il parametro `help`:



```
$ lightningd --help
lightningd: WARNING: default network changing in 2020: please set network=testnet in config!
Usage: lightningd 
A bitcoin lightning daemon (default values shown for network: testnet).
--conf=<file>                        Specify configuration file
--lightning-dir=<dir>                Set base directory: network-specific
                                     subdirectory is under here
                                      (default: "/home/javier/.lightning")
--network <arg>                      Select the network parameters (bitcoin,
                                     testnet, regtest, litecoin or
                                     litecoin-testnet) (default: testnet)
--testnet                            Alias for --network=testnet
--signet                             Alias for --network=signet
--mainnet                            Alias for --network=bitcoin

```


## Esegui lightningd

Inizierai la tua esplorazione della rete Lightning con il comando `lightning-cli`. Tuttavia, `lightningd` _deve_ essere in esecuzione per usare `lightning-cli`, poiché `lightning-cli` invia comandi JSON-RPC al `lightningd` (tutto proprio come con `bitcoin-cli` e `bitcoind`).

Se hai installato `c-lightning` a mano, ora dovrai avviarlo:



```
$ nohup lightningd --testnet &
```


### Esegui lightningd come servizio

Se preferisci, puoi installare `lightningd` come servizio che verrà eseguito ogni volta che riavvii la tua macchina. Il seguente farà così e lo avvierà immediatamente:



```
$ cat > ~/lightningd.service << EOF
# It is not recommended to modify this file in-place, because it will
# be overwritten during package upgrades. If you want to add further
# options or overwrite existing ones then use
# $ systemctl edit bitcoind.service
# See "man systemd.service" for details.
# Note that almost all daemon options could be specified in
# /etc/lightning/config, except for those explicitly specified as arguments
# in ExecStart=
[Unit]
Description=c-lightning daemon
[Service]
ExecStart=/usr/local/bin/lightningd --testnet
# Process management
####################
Type=simple
PIDFile=/run/lightning/lightningd.pid
Restart=on-failure
# Directory creation and permissions
####################################
# Run as standup
User=standup
# /run/lightningd
RuntimeDirectory=lightningd
RuntimeDirectoryMode=0710
# Hardening measures
####################
# Provide a private /tmp and /var/tmp.
PrivateTmp=true
# Mount /usr, /boot/ and /etc read-only for the process.
ProtectSystem=full
# Disallow the process and all of its children to gain
# new privileges through execve().
NoNewPrivileges=true
# Use a new /dev namespace only populated with API pseudo devices
# such as /dev/null, /dev/zero and /dev/random.
PrivateDevices=true
# Deny the creation of writable and executable memory mappings.
MemoryDenyWriteExecute=true
[Install]
WantedBy=multi-user.target
EOF
$ sudo cp ~/lightningd.service /etc/systemd/system
$ sudo systemctl enable lightningd.service
$ sudo systemctl start lightningd.service
```


### Abilita Connessioni Remote

Se hai qualche tipo di firewall, dovrai aprire la porta 9735, per permettere agli altri nodi Lightning di parlarti.

Se usi `ufw` da Bitcoin Standup, questo viene fatto come segue:


```
$ sudo ufw allow 9735
```


## Verifica il tuo Nodo

Puoi controllare se il tuo nodo Lightning è pronto confrontando l'output di `bitcoin-cli getblockcount` con il risultato `blockheight` da `lightning-cli getinfo`.



```
$ bitcoin-cli -testnet getblockcount
1838587
$ lightning-cli --testnet getinfo
{
   "id": "03d4592f1244cd6b5a8bb7fba6a55f8a91591d79d3ea29bf8e3c3a405d15db7bf9",
   "alias": "HOPPINGNET",
   "color": "03d459",
   "num_peers": 0,
   "num_pending_channels": 0,
   "num_active_channels": 0,
   "num_inactive_channels": 0,
   "address": [
      {
         "type": "ipv4",
         "address": "74.207.240.32",
         "port": 9735
      },
      {
         "type": "ipv6",
         "address": "2600:3c01::f03c:92ff:fe48:9ddd",
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
   "blockheight": 1838587,
   "network": "testnet",
   "msatoshi_fees_collected": 0,
   "fees_collected_msat": "0msat",
   "lightning-dir": "/home/standup/.lightning/testnet"
}
```


In questo caso, il `blockheight` è mostrato come `1838587` da entrambi i comandi.

Potresti invece ottenere un errore, a seconda della situazione precisa.

Se il nodo Bitcoin è ancora in sincronizzazione con la rete bitcoin dovresti vedere un messaggio come questo:



```
"warning_bitcoind_sync": "Bitcoind is not up-to-date with network."
```


Se il tuo lightning daemon non è aggiornato, riceverai un messaggio come questo:


```
"warning_lightningd_sync": "Still loading latest blocks from bitcoind."
```


Se hai provato a eseguire su una blockchain potato dove il nodo Bitcoin non era aggiornato quando hai avviato il nodo Lightning, riceverai messaggi di errore nel tuo log come questo:


```
bitcoin-cli -testnet getblock 0000000000000559febee77ab6e0be1b8d0bef0f971c7a4bee9785393ecef451 0 exited with status 1
```


## Crea Alias

Suggeriamo di creare alcuni alias per rendere più facile l'uso di c-lightning.

Puoi farlo inserendoli nel tuo `.bash_profile`.



```
cat >> ~/.bash_profile <<EOF
alias lndir="cd ~/.lightning/" #linux default c-lightning path
alias lnc="lightning-cli"
alias lnd="lightningd"
alias lninfo='lightning-cli getinfo'
EOF
```


Dopo aver inserito questi alias puoi o `source ~/.bash_profile` per inserirli o semplicemente uscire e rientrare.

Nota che questi alias includono scorciatoie per eseguire `lightning-cli`, per eseguire `lightningd` e per andare alla directory di c-lightning. Questi alias sono principalmente pensati per renderti la vita più facile. Suggeriamo di creare altri alias per facilitare l'uso dei comandi frequenti (e degli argomenti) e per minimizzare gli errori. Alias di questo tipo possono essere ancora più utili se hai un setup complesso in cui esegui regolarmente comandi associati a Mainnet, Testnet e Regtest, come spiegato più avanti.

Detto ciò, l'uso di questi alias in _questo_ documento potrebbe accidentalmente oscurare le lezioni principali che vengono insegnate su c-lightning, quindi continueremo a mostrare i comandi completi; adatta per il tuo uso come appropriato.

## Opzionale: Modifica i Tuoi Tipi di Server

> :link: **TESTNET vs MAINNET:** Quando hai impostato il tuo nodo, hai scelto di crearlo come un nodo Mainnet, Testnet o Regtest. Anche se questo documento presume un setup testnet, vale la pena capire come potresti accedere e usare gli altri tipi di setup, anche tutti sulla stessa macchina! Ma, se sei un utente per la prima volta, salta questo passaggio, poiché non è necessario per un setup di base.

Quando lightningd si avvia, solitamente legge un file di configurazione la cui posizione dipende dalla rete che stai usando (predefinito: `~/.lightning/testnet/config`). Questo può essere cambiato con i flag `–conf` e `–lightning-dir`.



```
~/.lightning/testnet$ ls -la config
-rw-rw-r-- 1 user user 267 jul 12 17:08 config
```


C'è anche un file di configurazione generale (predefinito: `~/.lightning/config`). Se vuoi eseguire diversi tipi di nodi contemporaneamente, devi lasciare il flag testnet (o regtest) fuori da questo file di configurazione. Dovresti quindi scegliere se stai usando il mainnet, il testnet o il tuo regtest ogni volta che esegui `lightningd` o `lightning-cli`.

Il tuo setup potrebbe non avere effettivamente file di configurazione: c-lightning funzionerà con un buon setup predefinito senza di essi.

## Sommario: Verificare la Configurazione Lightning

Prima di iniziare a giocare con lightning, dovresti assicurarti che i tuoi alias siano impostati, che il tuo `lightningd` sia in esecuzione e che il tuo nodo sia sincronizzato. Potresti anche voler configurare qualche accesso a setup di lightning alternativi, su altre reti.

## Cosa segue?

Continua "Understanding Your Lightning Setup" col [Capitolo 19.2: Comprendere la ConfigurazioneLightning](19_2_Comprendere_la_Configurazione_Lightning.md).

## Variante: Installare da Ubuntu ppa

Se stai usando una versione di Ubuntu diversa da Debian, puoi installare c-lightning usando [Ubuntu ppa](https://launchpad.net/~lightningnetwork/+archive/ubuntu/ppa):



```
$ sudo apt-get install -y software-properties-common
$ sudo add-apt-repository -u ppa:lightningnetwork/ppa
$ sudo apt-get install lightningd
```


## Variante: Installare Binaries Precompilati

Un altro metodo per installare Lightning è usare i binaries precompilati sul [repo di Github](https://github.com/ElementsProject/lightning/releases). Scegli l'ultima tarball, come `clightning-v0.9.1-Ubuntu-20.04.tar.xz`.

Dopo averlo scaricato, devi spostarti nella directory root e decomprimerlo:



```
$ cd /
$ sudo tar xf ~/clightning-v0.9.1-Ubuntu-20.04.tar.xz 
```


Attenzione: questo richiederà che tu abbia le stesse librerie precise utilizzate per creare il binary. Spesso è più facile ricompilare.
