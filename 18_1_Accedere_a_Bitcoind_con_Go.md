# 18.1: Accedere a Bitcoind con Go

> :information_source: **NOTA:** Questa sezione è stata recentemente aggiunta al corso ed è una bozza preliminare che potrebbe essere ancora in attesa di revisione. Lettore avvisato.

Questa sezione spiega come interagire con `bitcoind` utilizzando il linguaggio di programmazione Go e il [btcd rpcclient](https://github.com/btcsuite/btcd/tree/master/rpcclient). Si noti che presenta alcune peculiarità e limitazioni.

## Configura Go

Per preparare l'uso di Go sulla tua macchina UNIX, prima installa `curl` se non lo hai già fatto:



```
$ sudo apt install curl
```


Poi, visita la [pagina di download di Go](https://golang.org/dl/), ottieni il link per l'ultima versione disponibile e scaricalo usando `curl`. Per una configurazione Debian, dovrai usare la versione `linux-amd64`:


```
$ curl -O https://dl.google.com/go/go1.15.1.linux-amd64.tar.gz
```

Una volta terminato il download, confronta l'hash del file scaricato con l'hash sulla [pagina di download di Go](https://golang.org/dl/):

```
$ sha256sum go1.15.1.linux-amd64.tar.gz 
70ac0dbf60a8ee9236f337ed0daa7a4c3b98f6186d4497826f68e97c0c0413f6  go1.15.1.linux-amd64.tar.gz
```

Gli hash dovrebbero corrispondere. Se sì, estrai il tarball e installa Go sul tuo sistema:

```
$ tar xfv go1.15.1.linux-amd64.tar.gz 
$ sudo chown -R root:root ./go
$ sudo mv go /usr/local
```

Ora devi creare un percorso Go per specificare il tuo ambiente. Apri il file `~/.profile` con un editor a tua scelta e aggiungi quanto segue alla fine:

```
export GOPATH=$HOME/work
export PATH=$PATH:/usr/local/go/bin:$GOPATH/bin
```

Poi, aggiorna il tuo profilo:

```
$ source ~/.profile
```

Infine, crea la directory per il tuo spazio di lavoro Go:

```
$ mkdir $HOME/work
```

### Configura `btcd` `rpcclient`

Utilizzerai il `rpcclient` fornito con `btcd`, un'implementazione di Bitcoin scritta in Go. Anche se `rpcclient` è stato originariamente progettato per lavorare con il nodo completo `btcd` di Bitcoin, funziona anche con Bitcoin Core. Ha alcune peculiarità che esploreremo.

Puoi usare ```go get``` per scaricarlo:


```
$ go get github.com/btcsuite/btcd/rpcclient
```

Per testare che funzioni, naviga nella directory con gli esempi di Bitcoin Core:

```
$ cd $GOPATH/src/github.com/btcsuite/btcd/rpcclient/examples/bitcoincorehttp
```

Modifica il file ```main.go``` e inserisci i dettagli associati alla tua configurazione di Bitcoin Core, che puoi trovare in `~/.bitcoin/bitcoin.conf`:

```
		Host:         "localhost:18332",
		User:         "StandUp",
		Pass:         "6305f1b2dbb3bc5a16cd0f4aac7e1eba",
```

> **MAINNET VS TESTNET:** La porta sarà 8332 per una configurazione mainnet.

Ora puoi eseguire un test:

```
$ go run main.go
```

Dovresti vedere il conteggio dei blocchi stampato:

```
2020/09/01 11:41:24 Block count: 1830861
```

### Crea un Progetto `rpcclient`

Tipicamente creerai progetti nella tua directory `~/work/src/myproject/bitcoin`:

```
$ mkdir -p ~/work/src/myproject/bitcoin
$ cd ~/work/src/myproject/bitcoin
```

Ogni progetto dovrebbe avere i seguenti `import`:

```
import (
	"log"
	"fmt"
	"github.com/btcsuite/btcd/rpcclient"
)
```

Questa dichiarazione di ```import``` ti consente di importare le librerie pertinenti. Per ogni esempio qui, dovrai importare ```"log", "fmt"``` e ```"github.com/btcsuite/btcd/rpcclient"```. Potrebbe essere necessario importare librerie aggiuntive per alcuni esempi.
   * ```log``` è usato per stampare messaggi di errore. Dopo ogni chiamata al nodo Bitcoin, un'istruzione `if` controllerà se ci sono errori. Se ci sono errori, ```log``` verrà usato per stamparli. 
   * ```fmt``` è usato per stampare l'output. 
   * ```rpcclient``` è ovviamente la libreria `rpcclient`.

## Costruisci la Tua Connessione

Ogni funzione `bitcoind` in Go inizia con la creazione della connessione RPC, usando la funzione `ConnConfig`:

```
	connCfg := &rpcclient.ConnConfig{
		Host:         "localhost:18332",
		User:         "StandUp",
		Pass:         "431451790e3eee1913115b9dd2fbf0ac",
		HTTPPostMode: true,
		DisableTLS:   true,
	}
	client, err := rpcclient.New(connCfg, nil)
	if err != nil {
		log.Fatal(err)
	}
	defer client.Shutdown()
```
TI parametri ```connCfg``` ti permettono di scegliere la porta RPC di Bitcoin, il nome utente, la password e se sei su testnet o mainnet. 

> **NOTA:** Ancora una volta, assicurati di sostituire `User` e `Pass` con quelli trovati nel tuo `~/.bitcoin/bitcoind.conf`. 

La funzione ```rpcclient.New(connCfg, nil)``` configura ```client``` per connettersi al tuo nodo Bitcoin. 

La linea ```defer client.Shutdown()``` serve per disconnettersi dal nodo Bitcoin una volta terminata l'esecuzione della funzione ```main()```. Dopo la linea ```defer client.Shutdown()``` è dove vanno le cose interessanti — e sarà piuttosto facile da usare. Questo perché `rpcclient` trasforma utilmente i comandi `bitcoin-cli` in funzioni usando PascalCase. Ad esempio, ```bitcoin-cli getblockcount``` sarà ```client.GetBlockCount``` in Go.

### Effettua una Chiamata RPC

Ora tutto ciò che è richiesto è fare una chiamata informativa come `GetBlockCount` o `GetBlockHash` usando il tuo `client`:

```
	blockCount, err := client.GetBlockCount()
	if err != nil {
		log.Fatal(err)
	}
	blockHash, err := client.GetBlockHash(blockCount)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Printf("%d\n", blockCount)
	fmt.Printf("%s\n", blockHash.String())
```

### Effettua una Chiamata RPC con Argomenti

Le funzioni `rpcclient` possono accettare anche input; per esempio ```client.GetBlockHash(blockCount)``` prende il conteggio dei blocchi come input. Il ```client.GetBlockHash(blockCount)``` di cui sopra apparirebbe così come comando ```bitcoin-cli```:

```
$ bitcoin-cli getblockhash 1830868
00000000000002d53b6b9bba4d4e7dc44a79cebd1024d1bcfb9b3cc07d6cad9c
```
Tuttavia, una particolarità con gli hash in `rpcclient` è che di solito vengono stampati in una codifica diversa se li stampi normalmente con ```blockHash```. Per stamparli come stringa, devi usare ```blockHash.String()```.

### Esegui il Tuo Codice

Puoi scaricare il codice completo dalla [directory src](src/18_1_blockinfo.go).

Puoi poi eseguire:

```
$ go run blockinfo.go 
1830868
00000000000002d53b6b9bba4d4e7dc44a79cebd1024d1bcfb9b3cc07d6cad9c
```

Il numero dell'ultimo blocco insieme al suo hash dovrebbe essere stampato.

## Cerca Fondi

A causa delle limitazioni del `btcd` `rpcclient`, non puoi utilizzare la funzione ```getwalletinfo```. Tuttavia, puoi utilizzare il RPC `getbalance`:



```
	wallet, err := client.GetBalance("*")
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println(wallet)
```
```client.GetBalance("*")``` richiede l'input ```"*"```, a causa di una peculiarità di `btcd`. L'asterisco indica che vuoi ottenere il saldo di tutti i tuoi portafogli.

Se esegui [il codice src](src/18_1_getbalance.go), dovresti ottenere un output simile a questo:

```
$ go run getbalance.go 
0.000689 BTC
```

## Recupera Dati

Dopo aver configurato il tuo client e recuperato informazioni generali, puoi usare `client.GetBlock` per accedere ai dati del blocco e `client.GetRawTransaction` per accedere ai dati delle transazioni.

### Ottieni i Dati del Blocco

Per ottenere i dati del blocco, dovrai usare:


## Crea un Indirizzo

Puoi generare indirizzi in Go, ma non puoi specificare il tipo di indirizzo:

Questo richiede l'uso di una funzione speciale `chaincfg`, per specificare per quale rete vengono creati gli indirizzi. Questa specifica è necessaria solo durante la generazione degli indirizzi, motivo per cui è utilizzata solo in questo esempio. Puoi includerla anche in altri esempi, ma non è necessario.

Assicurati di importare `"github.com/btcsuite/btcd/chaincfg"`:

```
import (
	"log"
	"fmt"
	"github.com/btcsuite/btcd/rpcclient"
	"github.com/btcsuite/btcd/chaincfg"
)
```
Quindi chiama `connCfG` con il parametro `chaincfg.TestNet3Params.Name`:

```
	connCfg := &rpcclient.ConnConfig{
		Host:         "localhost:18332",
		User:         "bitcoinrpc",
		Pass:         "431451790e3eee1913115b9dd2fbf0ac",
		HTTPPostMode: true,
		DisableTLS:   true,
		Params: chaincfg.TestNet3Params.Name,
	}
	client, err := rpcclient.New(connCfg, nil)
	if err != nil {
		log.Fatal(err)
	}
	defer client.Shutdown()
```
> **MAINNET VS TESTNET:** `Params: chaincfg.TestNet3Params.Name,` dovrebbe essere `Params: chaincfg.MainNetParams.Name,` su mainnet.

Puoi quindi creare il tuo indirizzo:

```
	address, err := client.GetNewAddress("")
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println(address)
```
Una peculiarità di `client.GetNewAddress("")` è che è necessario includere una stringa vuota affinché funzioni.

Eseguendo [la fonte](17_1_getaddress.go) produce i seguenti risultati:

```
$ go run getaddress.go 
tb1qutkcj34pw0aq7n9wgp3ktmz780szlycwddfmza
```

### Decodifica un Indirizzo

Creare un indirizzo ha richiesto uno sforzo extra, nel specificare la catena appropriata. Utilizzare un indirizzo richiederà anche che tu lo decodifichi prima dell'uso.

Significa che dovrai importare entrambe le librerie `"github.com/btcsuite/btcutil"` e `"github.com/btcsuite/btcd/chaincfg"`:
   * `btcutil` permette di decodificare un indirizzo Bitcoin in un modo che `rpcclient` può comprendere. Questo è necessario quando si lavora con gli indirizzi in `rpcclient`.
   * `chaincfg` è (di nuovo) utilizzato per configurare la tua catena come la catena Testnet. Questo è necessario per la decodifica degli indirizzi poiché gli indirizzi utilizzati su Mainnet e Testnet sono diversi.


```
import (
	"log"
	"fmt"
	"github.com/btcsuite/btcd/rpcclient"
	"github.com/btcsuite/btcutil"
	"github.com/btcsuite/btcd/chaincfg"
)
```

La variabile `defaultNet` è ora utilizzata per specificare se il tuo nodo Bitcoin è su testnet o su mainnet. Queste informazioni (e l'oggetto `btcutil`) sono quindi utilizzate per decodificare l'indirizzo.

> **MAINNET VS TESTNET:** `&chaincfg.TestNet3Params` dovrebbe essere `&chaincfg.MainNetParams` su mainnet.

```
	defaultNet := &chaincfg.TestNet3Params
	addr, err := btcutil.DecodeAddress("mpGpCMX6SuUimDZKiVViuhd7EGyVxkNnha", defaultNet)
	if err != nil {
		log.Fatal(err)
	}
```


> **NOTA:** Cambia l'indirizzo (`mpGpCMX6SuUimDZKiVViuhd7EGyVxkNnha`) con uno effettivamente del tuo portafoglio; puoi usare `bitcoin-cli listunspent` per trovare alcuni indirizzi con fondi per questo test. Se vuoi essere davvero sofisticato, modifica il codice Go per prendere un argomento, poi scrivi uno script che esegue `listunspent`, salva le informazioni in una variabile, e esegue il codice Go su quella.

Solo dopo utilizzi il RPC `getreceivedbyaddress`, sul tuo indirizzo decodificato:

```
	wallet, err := client.GetReceivedByAddress(addr)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println(wallet)
```
Quando esegui [il codice](src/18_1_getamountreceived.go), dovresti ottenere un output simile a:

```
$ go run getamountreceived.go 
0.0085 BTC
```


## Invia una Transazione

Ora hai tutti i pezzi del puzzle per inviare una transazione. Ecco cosa dovrai fare:

1. Importa le librerie corrette, incluse `chaincfg` per specificare una rete e `btcutil` per decodificare un indirizzo.
2. Scegli un indirizzo a cui inviare.
3. Decodifica quell'indirizzo.
4. Esegui `sendtoaddress` per inviare fondi in modo semplice.


```
package main
import (
	"log"
	"fmt"
	"github.com/btcsuite/btcd/rpcclient"
	"github.com/btcsuite/btcutil"
	"github.com/btcsuite/btcd/chaincfg"
)
func main() {
	connCfg := &rpcclient.ConnConfig{
		Host:         "localhost:18332",
		User:         "StandUp",
		Pass:         "431451790e3eee1913115b9dd2fbf0ac",
		HTTPPostMode: true,
		DisableTLS:   true,
	}
	client, err := rpcclient.New(connCfg, nil)
	if err != nil {
		log.Fatal(err)
	}
	defer client.Shutdown()
	defaultNet := &chaincfg.TestNet3Params
	addr, err := btcutil.DecodeAddress("n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi", defaultNet)
	if err != nil {
		log.Fatal(err)
	}
	sent, err := client.SendToAddress(addr, btcutil.Amount(1e4))
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println(sent)
}
```

Quando esegui [il codice](src/18_1_sendtransaction.go), l'ID della transazione viene visualizzato:

```
$ go run sendtransaction.go
9aa4cd6559e0d69059eae142c35bfe78b71a8084e1fcc2c74e2a9675e9e7489d
```

### Cerca una Transazione

Per cercare una transazione, come quella che hai appena inviato, dovrai fare alcune conversioni, questa volta dell'ID della transazione. `"github.com/btcsuite/btcd/chaincfg/chainhash"` è importato per permettere che gli hash siano memorizzati nel codice Go. `chainhash.NewHashFromStr("hash")` converte un hash in una stringa in un formato che funziona con rpcclient.


```
package main

import (
	"log"
	"fmt"
	"github.com/btcsuite/btcd/rpcclient"
	"github.com/btcsuite/btcd/chaincfg/chainhash"
)

func main() {
	connCfg := &rpcclient.ConnConfig{
		Host:         "localhost:18332",
		User:         "StandUp",
		Pass:         "431451790e3eee1913115b9dd2fbf0ac",
		HTTPPostMode: true,
		DisableTLS:   true,
	}
	client, err := rpcclient.New(connCfg, nil)
	if err != nil {
		log.Fatal(err)
	}
	defer client.Shutdown()

	chash, err := chainhash.NewHashFromStr("1661ce322c128e053b8ea8fcc22d17df680d2052983980e2281d692b9b4ab7df")
	if err != nil {
		log.Fatal(err)
	}
	transactions, err := client.GetTransaction(chash)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println(transactions)
}
```
> **NOTA:** Ancora una volta, vorrai cambiare l'ID della transazione con uno effettivamente riconosciuto dal tuo sistema.

Quando esegui [il codice](17_1_lookuptransaction.go), stampa i dettagli associati a una transazione, come l'importo e quante volte è stata confermata:



```
$ go run lookuptransaction.go
{
  "amount": 0.00100000,
  "confirmations": 4817,
  "blockhash": "000000006628870b0a8a66abea9cf0d4e815c491f079e3fa9e658a87b5dc863a",
  "blockindex": 117,
  "blocktime": 1591857418,
  "txid": "1661ce322c128e053b8ea8fcc22d17df680d2052983980e2281d692b9b4ab7df",
  "walletconflicts": [
  ],
  "time": 1591857343,
  "timereceived": 1591857343,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "mpGpCMX6SuUimDZKiVViuhd7EGyVxkNnha",
      "category": "receive",
      "amount": 0.00100000,
      "label": "",
      "vout": 0
    }
  ],
  "hex": "02000000000101e9e8c3bd057d54e73baadc60c166860163b0e7aa60cab33a03e89fb44321f8d5010000001716001435c2aa3fc09ea53c3e23925c5b2e93b9119b2568feffffff02a0860100000000001976a914600c8c6a4abb0a502ea4de01681fe4fa1ca7800688ac65ec1c000000000017a91425b920efb2fde1a0277d3df11d0fd7249e17cf8587024730440220403a863d312946aae3f3ef0a57206197bc67f71536fb5f4b9ca71a7e226b6dc50220329646cf786cfef79d60de3ef54f702ab1073694022f0618731902d926918c3e012103e6feac9d7a8ad1ac6b36fb4c91c1c9f7fff1e7f63f0340e5253a0e4478b7b13f41fd1a00"
}
```
## Riassunto: Accesso a Bitcoind con Go

Anche se il `btcd` `rpcclient` presenta alcuni limiti, puoi comunque eseguire i principali comandi RPC in Go. La documentazione per `rpcclient` è disponibile su [Godoc](https://godoc.org/github.com/btcsuite/btcd/rpcclient). Se la documentazione non offre ciò che cerchi, consulta anche il [repository btcd](https://github.com/btcsuite/btcd). È generalmente ben documentato e facile da leggere. Basandoti su questi esempi dovresti essere in grado di incorporare Bitcoin in un progetto Go e fare cose come inviare e ricevere monete.

## Prossimi passi?

Scopri di più su "Parlare con Bitcoin in Altre Lingue" nel [Capitolo 18.2: Accedere a Bitcoind con Java](18_2_Accedere_a_Bitcoind_con_Java.md).
