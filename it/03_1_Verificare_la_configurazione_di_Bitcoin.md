# 3.1: Verificare la configurazione di Bitcoin

Prima di iniziare a giocare con Bitcoin, dovresti assicurarti che tutto sia impostato correttamente.

## Crea i tuoi alias

Suggeriamo di creare alcuni alias per facilitare l'utilizzo di Bitcoin.

Puoi farlo inserendoli nel tuo `.bash_profile`, `.bashrc` o `.profile`.
```
cat >> ~/.bash_profile <<EOF
alias btcdir="cd ~/.bitcoin/" #linux percorso bitcoind predefinito
alias bc="bitcoin-cli"
alias bd="bitcoind"
alias btcinfo='bitcoin-cli getwalletinfo | egrep "\"saldo\""; bitcoin-cli getnetworkinfo | egrep "\"versione\"|connessioni"; bitcoin-cli getmininginfo | egrep "\"blocchi\"|errori"'
EOF
```
Dopo aver inserito questi alias puoi utilizzare `source .bash_profile` per inserirli o semplicemente disconnetterti e rientrare.

Tieni presente che questi alias includono scorciatoie per eseguire "bitcoin-cli", per eseguire "bitcoind" e per accedere alla directory Bitcoin. Questi alias hanno principalmente lo scopo di semplificarti la vita. Ti suggeriamo di creare altri alias per facilitare l'uso di comandi (e argomenti) frequenti e per ridurre al minimo gli errori. Alias ​​di questo tipo possono essere ancora più utili se si dispone di una configurazione complessa in cui si eseguono regolarmente comandi associati a Mainnet, a Testnet _e_ a Regtest, come spiegato più avanti.

Detto questo, l'uso di questi alias in _questo_ documento potrebbe oscurare accidentalmente le lezioni fondamentali insegnate su Bitcoin, quindi l'unico alias utilizzato direttamente qui è "btcinfo" perché incapsula un comando molto più lungo e complesso. Altrimenti mostriamo i comandi completi; adattarlo per uso personale, a seconda dei casi.

## Esegui Bitcoind

Inizierai la tua esplorazione della rete Bitcoin con il comando "bitcoin-cli". Tuttavia, bitcoind _deve_ essere in esecuzione per utilizzare bitcoin-cli, poiché bitcoin-cli invia comandi JSON-RPC a bitcoind. Se hai utilizzato la nostra configurazione standard, bitcoind dovrebbe essere già attivo e funzionante. Puoi ricontrollare guardando la tabella dei processi.

```
$ps auxww | grep bitcoind
standup  455 1,3 34,4 3387536 1392904 ? SLsl 16 Jun16 59:30 /usr/local/bin/bitcoind -conf=/home/standup/.bitcoin/bitcoin.conf
```

Se non è in esecuzione, ti consigliamo di eseguire manualmente `/usr/local/bin/bitcoind -daemon` e inserirlo anche nel tuo _crontab_.

## Verifica i tuoi blocchi

Dovresti avere scaricato l'intera blockchain prima di iniziare a giocare. Basta eseguire l'alias `bitcoin-cli getblockcount` per vedere se è tutto caricato.
```
$ bitcoin-cli getblockcount
1772384
```
Questo ti dice cosa è caricato; dovrai quindi confrontarlo con un servizio online che ti indica l'altezza attuale del blocco.

> :book: ***Cos'è l'altezza del blocco?*** L'altezza del blocco è la distanza alla quale un particolare blocco si trova dopo il blocco genesi. L'altezza del blocco corrente è l'altezza del blocco più recente aggiunto alla blockchain.

Puoi farlo esaminando un blocknet explorer, come [Blockcypher Testnet explorer](https://live.blockcypher.com/btc-testnet/). Il suo numero più recente corrisponde al tuo `getblockcount`? Se è così, sei aggiornato.

Se desideri un alias per visualizzare tutto in una volta, quanto segue attualmente funziona per Testnet, ma potrebbe scomparire in futuro:

```
$ echo "alias btcblock='echo \$(bitcoin-cli -testnet getblockcount)/\$(curl -s https://blockstream.info/testnet/api/blocks/tip/height)'" >> .bash_profile
$ source .bash_profile 
$ btcblock
1804372/1804372
```
> :link: **TESTNET vs MAINNET:** Ricorda che questo tutorial generalmente presuppone che tu stia utilizzando testnet. Se invece utilizzi la rete principale, puoi recuperare l'altezza del blocco corrente con: `curl -s https://blockchain.info/q/getblockcount`. Puoi sostituire la seconda metà dell'alias `btcblock` (dopo `/\$(`) con quella.

Se non sei aggiornato, ma il tuo `getblockcount` sta aumentando, nessun problema. Il tempo totale di download può richiedere ancora piu tempo, a seconda della configurazione.

## Opzionale: conosci i tuoi tipi di server

> **TESTNET vs MAINNET:** quando configuri il tuo nodo, scegli di crearlo come nodo Mainnet, Testnet o Regtest. Anche se questo documento presuppone una configurazione di testnet, vale la pena capire come accedere e utilizzare gli altri tipi di configurazione, anche tutti sulla stessa macchina! Ma, se sei un utente alle prime armi, salta oltre, poiché non è necessario per una configurazione di base.

Il tipo di configurazione è controllato principalmente tramite il file ~/.bitcoin/bitcoin.conf. Se stai utilizzando testnet, probabilmente contiene questa riga:
```
testnet=1
```
Se stai eseguendo regtest, probabilmente contiene questa riga:
```
regtest=1
```
Tuttavia, se desideri eseguire diversi tipi di nodi contemporaneamente, dovresti lasciare il flag testnet (o regtest) fuori dal file di configurazione. Puoi quindi scegliere se utilizzare la mainnet, la testnet o il regtest ogni volta che esegui bitcoind o bitcoin-cli.

Ecco una serie di alias che renderebbero tutto più semplice creando un alias specifico per avviare e arrestare bitcoind, per andare alla directory bitcoin e per eseguire bitcoin-cli, per ciascuna rete principale (che non ha flag aggiuntivi), il testnet (che è -testnet) o il tuo regtest (che è -regtest).
```
cat >> ~/.bash_profile <<EOF
alias bcstart="bitcoind -daemon"
alias btstart="bitcoind -testnet -daemon"
alias brstart="bitcoind -regtest -daemon"

alias bcstop="bitcoin-cli stop"
alias btstop="bitcoin-cli -testnet stop"
alias brstop="bitcoin-cli -regtest stop"

alias bcdir="cd ~/.bitcoin/" #linux default bitcoin path
alias btdir="cd ~/.bitcoin/testnet" #linux default bitcoin testnet path
alias brdir="cd ~/.bitcoin/regtest" #linux default bitcoin regtest path

alias bc="bitcoin-cli"
alias bt="bitcoin-cli -testnet"
alias br="bitcoin-cli -regtest"
EOF
```
Per una complessità ancora maggiore, potresti fare in modo che ciascuno dei tuoi alias "iniziali" utilizzi il flag -conf per caricare la configurazione da un file diverso. Questo va ben oltre lo scopo di questo tutorial, ma lo offriamo come punto di partenza per quando le tue esplorazioni di Bitcoin raggiungeranno il livello successivo.

## Riepilogo: verificare la configurazione di Bitcoin

Prima di iniziare a giocare con bitcoin, dovresti assicurarti che i tuoi alias siano impostati, che il tuo bitcoind sia in esecuzione e che i tuoi blocchi siano scaricati. Potresti anche voler impostare l'accesso a configurazioni Bitcoin alternative, se sei un utente avanzato.

## Cosa fare dopo?

Continua "Comprendere la configurazione di Bitcoin" con [ Capitolo 3.2 Capire la configurazione di Bitcoin.md](03_2_Capire_la_configurazione_di_Bitcoin.md).
