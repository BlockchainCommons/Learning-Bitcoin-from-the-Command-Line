# Interludio: Usare Curl

`bitcoin-cli` è in definitiva solo un wrapper. È un modo per interfacciarsi con `bitcoind` dalla riga di comando, fornendo un accesso semplificato ai suoi numerosi comandi RPC. Naturalmente è possibile accedere direttamente a RPC. Ecco di cosa tratta questo interludio: connettersi direttamente a RPC con il comando `curl`.

Non verrà utilizzato molto nei capitoli futuri, ma è un elemento importante che puoi vedere come un accesso alternativo a `bitcoind` se preferisci.

## Capire CURL

`curl`, abbreviazione di `vedi URL`, è uno strumento da riga di comando che ti consente di accedere direttamente agli URL in modo programmatico. È un modo semplice per interagire con server come `bitcoind` che ascoltano le porte su Internet e che parlano una varietà di protocolli. Curl è disponibile anche come libreria per molti linguaggi di programmazione, come C, Java, PHP e Python. Quindi, una volta che sai come lavorare con Curl, avrai una solida base per utilizzare molte API diverse.

Per utilizzare `curl` con `bitcoind`, devi conoscere tre cose: il formato standard, il nome utente e la password e la porta corretta.

### Capire il format

I comandi `bitcoin-cli` sono tutti collegati ai comandi RPC in `bitcoind`. Ciò rende molto semplice la transizione dall'utilizzo di `bitcoin-cli` all'utilizzo di `curl`. Infatti, se guardi una qualsiasi delle pagine di aiuto di `bitcoin-cli`, vedrai che elencano non solo i comandi `bitcoin-cli`, ma anche i comandi paralleli `curl`. Ad esempio, ecco `bitcoin-cli help getmininginfo`:
```
$ bitcoin-cli aiuta a ottenere informazioni sul mining
getmininginfo

Restituisce un oggetto json contenente informazioni relative al mining.
Risultato:

{                              (json object)
  `blocks` : n,                (numeric) The current block
  `currentblockweight` : n,    (numeric, optional) The block weight of the last assembled block (only present if a block was ever assembled)
  `currentblocktx` : n,        (numeric, optional) The number of block transactions of the last assembled block (only present if a block was ever assembled)
  `difficulty` : n,            (numeric) The current difficulty
  `networkhashps` : n,         (numeric) The network hashes per second
  `pooledtx` : n,              (numeric) The size of the mempool
  `chain` : `str`,             (string) current network name (main, test, regtest)
  `warnings` : `str`           (string) any network and blockchain warnings
}

Esempi:
> bitcoin-cli getmininginfo 
> curl --user myusername --data-binary '{"jsonrpc": "1.0", "id": "curltest", "method": "getmininginfo", "params": []}' -H 'content-type: text/plain;' http://127.0.0.1:8332/
```
Ecco il comando `curl`, alla fine della schermata di aiuto! Questo comando piuttosto lungo è composto da quattro parti principali: (1) il tuo nome utente; (2) un flag `--data-binary`; (3) un oggetto JSON che dice a `bitcoind` cosa fare, incluso un array di parametri JSON; e (4) un'intestazione HTTP che include l'URL `bitcoind`.

Quando lavori con `curl`, la maggior parte degli argomenti di `curl` rimarranno gli stessi da comando a comando; in genere cambieranno solo le voci `method` e `params` nell'array JSON. Tuttavia, devi sapere come inserire il tuo nome utente e il tuo indirizzo URL per farlo funzionare in primo luogo!

_Ogni volta che non sei sicuro su come eseguire il curling di un comando RPC, guarda la guida di bitcoin-cli e vai da lì._

### Conosci il tuo nome utente

Per parlare co'l porto `bitcoind`, hai bisogno di un nome utente e una password. Questi sono stati creati come parte della configurazione iniziale di Bitcoin e possono essere trovati in `~/.bitcoin/bitcoin.conf`.

Ad esempio, ecco la nostra configurazione attuale:
```
$ cat ~/.bitcoin/bitcoin.conf
server=1
dbcache=1536
par=1
maxuploadtarget=137
maxconnections=16
rpcuser=StandUp
rpcpassword=8eaf562eaf45c33c3328bc66008f2dd1
rpcallowip=127.0.0.1
debug=tor
prune=550
testnet=1
mintxfee=0.001
txconfirmtarget=1
[test]
rpcbind=127.0.0.1
rpcport=18332
[main]
rpcbind=127.0.0.1
rpcport=8332
[regtest]
rpcbind=127.0.0.1
rpcport=18443
```
Il nostro nome utente è `StandUp` e la tua password è `8eaf562eaf45c33c3328bc66008f2dd1`.

> **ATTENZIONE:** Chiaramente non è molto sicuro avere queste informazioni in un file di testo. A partire da Bitcoin Core 0.12, puoi invece omettere `rpcpassword` dal tuo file `bitcoin.conf` e fare in modo che `bitcoind` generi un nuovo cookie ogni volta che si avvia. Lo svantaggio è che rende più difficile l'uso dei comandi RPC da parte di altre applicazioni, come quelle descritte in questo capitolo. Quindi per ora ci atterremo alle semplici informazioni `rpcuser` e `rpcpassword`, ma per il software di produzione, considera il passaggio ai cookie.

Il modo sicuro per RPC con `bitcoind` è il seguente:

```
$ curl --user StandUp --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getmininginfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/
Enter host password for user 'bitcoinrpc':
```
Come notato, ti verrà richiesta la password.

> :link: **TESTNET vs MAINNET:** Testnet utilizza un URL con porta 18332 e mainnet utilizza un URL con porta 8332. Dai un'occhiata al tuo `bitcoin.conf`, è tutto disposto lì.

Il modo non sicuro per farlo è il seguente:
```
$ curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getmininginfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/
```
> **ATTENZIONE:** L'immissione della password sulla riga di comando potrebbe inserire la password nella tabella dei processi e/o salvarla in una cronologia. Questo è ancora meno consigliato che inserirlo in un file, tranne che per i test su testnet. Se vuoi farlo altrove, assicurati di sapere cosa stai facendo!

### Conosere comandi e parametri

Con tutto questo in mano, sei pronto per inviare comandi RPC standard con `curl`... ma devi ancora sapere come incorporare i due elementi che tendono a cambiare nel comando `curl`.

Il primo è `metodo`, ovvero il metodo RPC utilizzato. In genere dovrebbe corrispondere ai nomi dei comandi che hai inserito in `bitcoin-cli` per anni.

Il secondo è `params`, che è un array di parametri JSON. Questi sono gli stessi argomenti (o argomenti denominati) che hai utilizzato. Costituiscono anche la parte più confusa di `curl`, in gran parte perché sono un array strutturato anziché un semplice elenco.

Ecco come appariranno alcuni array di parametri:

  * `[]` - Un array vuoto
  * `["000b4430a7a2ba60891b01b718747eaf9665cb93fbc0c619c99419b5b5cf3ad2"]` - Un array con dati
  * `["'$signedhex'"]` - Un array con una variabile
  * `[6, 9999999]` - Un array con due parametri
  * `{}` - Un oggetto vuoto
  * `[''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]'', ''{ "'$recipient'": 0.298, "'$changeaddress'": 1.0}'']` - Un array con un array contenente un oggetto e un oggetto semplice

## Ottenere informazioni

Ora puoi inviare il tuo primo comando `curl` accedendo all'RPC `getmininginfo`:
```
$ curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getmininginfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/
{"result":{"blocks":1772428,"difficulty":10178811.40698772,"networkhashps":91963587385939.06,"pooledtx":61,"chain":"test","warnings":"Warning: unknown new rules activated (versionbit 28)"},"error":null,"id":"curltest"}
```

Tieni presente che abbiamo fornito il metodo, `getmininginfo`, e il parametro, '[]', ma tutto il resto era la riga di comando `curl` standard.

> **ATTENZIONE:** Se ottieni un risultato come "Impossibile connettersi alla porta 127.0.0.1 8332: connessione rifiutata", assicurati che una riga come `rpcallowip=127.0.0.1` sia nel tuo ~/.bitcoin/bitcoin .conf. Se le cose continuano a non funzionare, assicurati di consentire l'accesso alla porta 18332 (o 8332) da localhost. La nostra configurazione standard del [Capitolo due: Configurare Bitcoin-Core VPS](02_0_Configurare_Bitcoin-Core_VPS.md) dovrebbe fare tutto questo.

Il risultato è un altro array JSON, che sfortunatamente è brutto da leggere se usi `curl` manualmente. Fortunatamente, puoi ripulirlo semplicemente collegandolo tramite `jq`:
```
$ curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getmininginfo", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.'
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100   295  100   218  100    77  72666  25666 --:--:-- --:--:-- --:--:-- 98333
{
  "result": {
    "blocks": 1772429,
    "difficulty": 10178811.40698772,
    "networkhashps": 90580030969896.44,
    "pooledtx": 4,
    "chain": "test",
    "warnings": "Warning: unknown new rules activated (versionbit 28)"
  },
  "error": null,
  "id": "curltest"
}
```
Vedrai un po' di report sulla connettività man mano che i dati vengono scaricati, quindi quando i dati raggiungono `jq`, tutto verrà visualizzato in un formato con rientro corretto. (Negli esempi futuri ometteremo le informazioni di download.)

## Maneggiare il tuo portafoglio

Anche se accedi direttamente a `bitcoind`, avrai comunque accesso alle funzionalità del portafoglio, perché sono in gran parte archiviate in `bitcoind` stesso.

### Cercare indirizzi

Utilizza l'RPC `getaddressesbylabel` per elencare tutti i tuoi indirizzi attuali:
```
$ curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getaddressesbylabel", "params": [""] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.'
{
  "result": {
    "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE": {
      "purpose": "receive"
    },
    "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff": {
      "purpose": "receive"
    },
    "moKVV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B": {
      "purpose": "receive"
    },
    "mwJL7cRiW2bUnY81r1thSu3D4jtMmwyU6d": {
      "purpose": "receive"
    },
    "tb1q5gnwrh7ss5mmqt0qfan85jdagmumnatcscwpk6": {
      "purpose": "receive"
    },
    "tb1qmtucvjtga68kgrvkl7q05x4t9lylxhku7kqdpr": {
      "purpose": "receive"
    }
  },
  "error": null,
  "id": "curltest"
}
```
Questo è il nostro primo esempio di parametro reale, `""`. Questo è il parametro `label` richiesto per `getaddressesbylabel`, ma tutti i nostri indirizzi sono sotto l'etichetta predefinita, quindi qui non è stato richiesto nulla di speciale.

Il risultato è un elenco di tutti gli indirizzi che sono stati utilizzati da questo portafoglio... alcuni dei quali presumibilmente contengono fondi.

### Cercare i fondi

Utilizza l'RPC `listunspent` per elencare i fondi che hai a disposizione:
```
$ curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "listunspent", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.'
{
  "result": [
    {
      "txid": "e7071092dee0b2ae584bf6c1ee3c22164304e3a17feea7a32c22db5603cd6a0d",
      "vout": 1,
      "address": "mk9ry5VVy8mrA8SygxSQQUDNSSXyGFot6h",
      "scriptPubKey": "76a91432db726320e4ad170c9c1ee83cd4d8a243c3435988ac",
      "amount": 0.0009,
      "confirmations": 4,
      "spendable": true,
      "solvable": true,
      "desc": "pkh([d6043800/0'/1'/2']02881697d252d8bf181d08c58de1f02aec088cd2d468fc5fd888c6e39909f7fabf)#p6k7dptk",
      "safe": true
    },
    {
      "txid": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
      "vout": 0,
      "address": "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
      "label": "",
      "scriptPubKey": "76a9142d573900aa357a38afd741fbf24b075d263ea6e088ac",
      "amount": 0.00022,
      "confirmations": 19,
      "spendable": true,
      "solvable": true,
      "desc": "pkh([d6043800/0'/0'/3']0278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132)#nhjc3f8y",
      "safe": true
    }
  ],
  "error": null,
  "id": "curltest"
}
```
Questo è quasi esattamente lo stesso output che ricevi quando digiti `bitcoin-cli listunspent`, mostrando quanto siano strettamente legate le due interfacce. Se non è necessaria alcuna pulizia o ulteriore aiuto, `bitcoin-cli` restituisce semplicemente l'RPC. Facile!

### Crea un indirizzo

Dopo aver saputo dove sono i tuoi fondi, il passo successivo nella creazione di una transazione è ottenere un cambio di indirizzo. A questo punto probabilmente hai capito come funziona e sai che per semplici comandi RPC, tutto ciò che devi fare è modificare il `metodo` è il comando `curl`:
```
$ curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getrawchangeaddress", "params": ["legacy"] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.'
{
  "result": "mrSqN37TPs89GcidSZTvXmMzjxoJZ6RKoz",
  "error": null,
  "id": "curltest"
}
```

> **AVVERTENZA:** L'ordine dei parametri è importante quando si inviano comandi RPC utilizzando curl. C'è solo un argomento per `getrawchangeaddress`, ma considera il suo cugino stretto `getnewaddress`. Richiede due argomenti: prima etichetta, poi tipo. Se inviassimo lo stesso `"params": ["legacy"]` invece di `"params": ["", "legacy"]`, otterremmo un indirizzo `bech32` con l'etichetta `"legacy"` invece di un indirizzo `legacy`, quindi presta attenzione all'ordine!

A questo punto, possiamo anche tornare alla nostra pratica standard di salvare i risultati in variabili con l'aiuto aggiuntivo di `jq`:

```
$ newaddress=$(curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getrawchangeaddress", "params": ["legacy"] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.result')
$ echo $newaddress
mqdfnjgWr2r3sCCeuTDfe8fJ1CnycF2e6R
```
Non è necessario preoccuparsi delle informazioni di download. Andrà su `STDERR` e verrà visualizzato sullo schermo, mentre i risultati andranno su `STDOUT` e verranno salvati nella tua variabile.

## Crea una transazione

Ora sei pronto per creare una transazione con `curl`.

### Prepara le tue variabili

Proprio come con `bitcoin-cli`, per creare una transazione eseguendo i comandi RPC, dovresti prima salvare le tue variabili. L'unico cambiamento qui è che 'curl' crea un oggetto JSON che include un valore-chiave `result`, quindi devi sempre reindirizzare il tag `.result` prima di fare qualsiasi altra cosa.

Questo esempio imposta le nostre variabili per utilizzare 1,2985 BTC nei fondi elencati nella prima transazione non spesa sopra:

```
$ utxo_txid=$(curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "listunspent", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.result | .[0] | .txid')
$ utxo_vout=$(curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "listunspent", "params": [] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.result | .[0] | .vout')
$ recipient=mwCwTceJvYV27KXBc3NJZys6CjsgsoeHmf
$ changeaddress=$(curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "getrawchangeaddress", "params": ["legacy"] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.result')

$ echo $utxo_txid
e7071092dee0b2ae584bf6c1ee3c22164304e3a17feea7a32c22db5603cd6a0d
$ echo $utxo_vout
1
$ echo $recipient
mwCwTceJvYV27KXBc3NJZys6CjsgsoeHmf
$ echo $changeaddress
n2jf3MzeFpFGa7wq8rXKVnVuv5FoNSJZ1N
```
### Crea la transazione

La transazione creata con `curl` è molto simile alla transazione creata con `bitcoin-cli`, ma con alcune sottili differenze:
```
$ curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "createrawtransaction", "params": [''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]'', ''{ "'$recipient'": 0.0003, "'$changeaddress'": 0.0005}'']}' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.'
{
  "result": "02000000010d6acd0356db222ca3a7ee7fa1e3044316223ceec1f64b58aeb2e0de921007e70100000000ffffffff0230750000000000001976a914ac19d3fd17710e6b9a331022fe92c693fdf6659588ac50c30000000000001976a9147021efec134057043386decfaa6a6aa4ee5f19eb88ac00000000",
  "error": null,
  "id": "curltest"
}
```

Il cuore della transazione è, ovviamente, l'array JSON `params`, che stiamo sfruttando appieno per la prima volta.

Tieni presente che l'intero `params` è alloggiato in `[]` per contrassegnare l'array dei parametri.

Abbiamo anche variato le citazioni rispetto a come funzionavano le cose in `bitcoin-cli`, per iniziare e terminare ogni array e oggetto all'interno dell'array `params` con `''`  invece del nostro tradizionale `'''`. Questo perché l'intero set di argomenti JSON è già racchiuso da un `'`. Come al solito, basta dare un'occhiata alla bizzarra citazione della shell e abituarsi.

Tuttavia, c'è un'ultima cosa degna di nota in questo esempio, e può essere _esasperante_ se te la perdi. Quando hai eseguito un comando `createrawtransaction` con `bitcoin-cli`, l'array JSON degli input e l'oggetto JSON degli output erano parametri distinti, quindi erano separati da uno spazio. Ora, poiché fanno parte dell'array JSON `params`, sono separati da una virgola (`,`). Perdilo e otterrai un `errore di analisi` senza molte informazioni aggiuntive.

> **ATTENZIONE:** Hai mai avuto problemi con il debug del tuo `curl`? Aggiungi l'argomento `--trace-ascii /tmp/foo`. Le informazioni complete su ciò che viene inviato al server verranno salvate in `/tmp/foo` (o qualunque nome di file fornisca).

Dopo aver verificato che le cose funzionino, probabilmente vorrai salvare il codice esadecimale in una variabile:


```
$ hexcode=$(curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "createrawtransaction", "params": [''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]'', ''{ "'$recipient'": 0.0003, "'$changeaddress'": 0.0005}'']}' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.result')
```

### Firma e invia

Firmare e inviare la transazione utilizzando `curl` è un facile utilizzo dell'RPC `signrawtransactionwithwallet` e `sendrawtransaction`:

```
$ signedhex=$(curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "signrawtransactionwithwallet", "params": ["'$hexcode'"] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.result | .hex')

$ curl --user StandUp:8eaf562eaf45c33c3328bc66008f2dd1 --data-binary '{"jsonrpc": "1.0", "id":"curltest", "method": "sendrawtransaction", "params": ["'$signedhex'"] }' -H 'content-type: text/plain;' http://127.0.0.1:18332/ | jq -r '.'
{
  "result": "eb84c5008038d760805d4d9644ace67849542864220cb2685a1ea2c64176b82d",
  "error": null,
  "id": "curltest"
}
```
## Riepilogo: accesso a Bitcoind con Curl

Dopo aver terminato questa sezione, potresti pensare che accedere a `bitcoind` tramite `curl` sia molto simile ad accedervi tramite `bitcoin-cli`... ma più complicato. E avresti ragione. `bitcoin-cli` ha funzionalità RPC piuttosto complete, quindi tutto ciò che fai tramite `curl` probabilmente puoi farlo tramite `bitcoin-cli`. Ecco perché continueremo a concentrarci su `bitcoin-cli` dopo questa digressione.

Ma ci sono ancora ragioni per cui dovresti usare `curl` invece di `bitcoin-cli`:

_Qual è il potere di curl?_ Ovviamente, `curl` elimina un livello di indiretto. Invece di lavorare con `bitcoin-cli` che invia comandi RPC a `bitcoind`, invii questi comandi RPC direttamente. Ciò consente una programmazione più solida, perché non devi preoccuparti di quali cose inaspettate potrebbe fare `bitcoin-cli` o di come potrebbe cambiare nel tempo. Tuttavia, stai anche muovendo i primi passi verso l'utilizzo di un linguaggio di programmazione più completo rispetto alle scarse opzioni offerte da uno script di shell. Come vedrai negli ultimi capitoli di questo, potresti effettivamente vedere che le librerie curl sono altre funzioni per accedere ai comandi RPC in una varietà di linguaggi di programmazione: ma è ancora molto lontano.

## Qual è il prossimo passo?
Scopri un altro modo per "Inviare transazioni Bitcoin" qui: [Capitolo 4.5: InviareMonete con Transazione Grezza Automatizzata](04_5_Inviare_Monete_con_Transazione_Grezza_Automatizzata.md).
