# 16.2: Programmare Bitcoind in C con Librerie RPC

> :information_source: **NOTA:** Questa sezione è stata recentemente aggiunta al corso ed è una bozza iniziale che potrebbe essere ancora in attesa di revisione. Lettore avvisato.

Il [Capitolo 16.1](16_1_Accedere_a_Bitcoind_con_Librerie_RPC.md) ha delineato la metodologia per creare programmi in C utilizzando librerie RPC e JSON. Ora mostreremo il potenziale di queste librerie C delineando una versione semplificata di un vero programma Bitcoin.

## Pianifica il Tuo Codice

Questa sezione creerà una prima versione semplificata di `sendtoaddress`, che permetterà a un utente di inviare denaro a un indirizzo purché abbia un UTXO abbastanza grande. Ecco cosa dobbiamo fare:

  1. Richiedere un indirizzo e un importo
  2. Impostare una commissione arbitraria
  3. Preparare il Tuo RPC
  4. Trovare un UTXO abbastanza grande per l'importo + la commissione
  5. Creare un indirizzo di resto
  6. Creare una transazione grezza che invia dall'UTXO all'indirizzo e all'indirizzo di resto
  7. Firmare la transazione
  8. Inviare la transazione
  
### Pianifica il Tuo Futuro

Dato che questo è il tuo primo programma funzionale in C, cercheremo di mantenerlo semplice (KISS). Se stessimo producendo un vero programma di produzione, vorremmo almeno fare quanto segue:

   1. Testare e/o sanitizzare gli input
   2. Calcolare automaticamente una commissione
   3. Pensare logicamente a quale UTXO valido utilizzare
   4. Combinare più UTXO se necessario
   5. Controllare più errori nei comandi `libbitcoinrpc` o `jansson`
   6. Controllare errori nelle risposte RPC
   
Se vuoi continuare a espandere questo esempio, affrontare le carenze di questo programma esemplare sarebbe un ottimo punto di partenza.

## Scrivi il Tuo Software di Transazione

Ora sei pronto a intraprendere questo piano passo dopo passo.

### Passo 1: Richiedere un Indirizzo e un Importo

Inserire le informazioni è abbastanza facile tramite gli argomenti della riga di comando:
:
```
if (argc != 3) {

  printf("ERROR: Only %i arguments! Correct usage is '%s [recipient] [amount]'\n",argc-1,argv[0]);
  exit(-1);

}

char *tx_recipient = argv[1];
float tx_amount = atof(argv[2]);

printf("Sending %4.8f BTC to %s\n",tx_amount,tx_recipient);
```


> :warning: **AVVERTENZA:** Un vero programma avrebbe bisogno di una sanificazione molto migliore di queste variabili. 

### Passo 2: Impostare una Commissione Arbitraria

Questo esempio imposta una commissione arbitraria di 0,0005 BTC per assicurarsi che le transazioni di test vadano a buon fine rapidamente:

```
float tx_fee = 0.0005;
float tx_total = tx_amount + tx_fee;
```


> :warning: **AVVERTENZA:** Un vero programma calcolerebbe una commissione che minimizza il costo garantendo al contempo che la velocità sia sufficiente per il mittente. 

### Passo 3: Preparare il Tuo RPC 

Ovviamente, dovrai preparare di nuovo tutte le tue variabili, come discusso in [§16.1: Accesso a Bitcoind con C](16_1_Accessing_Bitcoind_with_C.md). Devi anche inizializzare la tua libreria, connettere il tuo client RPC e preparare il tuo oggetto di risposta:

```
bitcoinrpc_global_init();
rpc_client = bitcoinrpc_cl_init_params ("bitcoinrpc", "YOUR-RPC-PASSWD", "127.0.0.1", 18332);
btcresponse = bitcoinrpc_resp_init();
```


### Passo 4: Trovare un UTXO

Per trovare un UTXO devi chiamare l'RPC `listunspent`:
:
```
rpc_method = bitcoinrpc_method_init(BITCOINRPC_METHOD_LISTUNSPENT);
bitcoinrpc_call(rpc_client, rpc_method, btcresponse, &btcerror);
```
Tuttavia, il vero lavoro consiste nel decodificare la risposta. La sezione precedente ha notato che la libreria `jansson` era "un po' ingombrante" e questo è il motivo: devi creare (e cancellare) un gran numero di oggetti `json_t` per scavare fino a ciò che desideri.

Innanzitutto, devi recuperare il campo `result` dal JSON:

```
json_t *lu_response = NULL;
json_t *lu_result = NULL;

lu_response = bitcoinrpc_resp_get (btcresponse);
lu_result = json_object_get(lu_response,"result");
```


> :warning: **AVVERTENZA:** Ottieni un risultato solo se non c'è stato un errore. Ecco un altro punto per un controllo degli errori migliore per il codice di produzione.

Poi, entri in un ciclo, esaminando ogni transazione non spesa, che appare come un elemento nel tuo array di risultati JSON:

```
int i;

const char *tx_id = 0;
int tx_vout = 0;
double tx_value = 0.0;

for (i = 0 ; i < json_array_size(lu_result) ; i++) {

  json_t *lu_data = NULL;
  lu_data = json_array_get(lu_result, i);
  
  json_t *lu_value = NULL;
  lu_value = json_object_get(lu_data,"amount");
  tx_value = json_real_value(lu_value);
```
L'UTXO è abbastanza grande da pagare la tua transazione? Se sì, prendilo!

> :warning: **AVVERTENZA:** Un programma del mondo reale penserebbe più attentamente a quale UTXO prendere, in base alle dimensioni e ad altri fattori. Probabilmente non prenderebbe solo la prima cosa che vede che funziona.


```
  if (tx_value > tx_total) {

    json_t *lu_txid = NULL;
    lu_txid = json_object_get(lu_data,"txid");
    tx_id = strdup(json_string_value(lu_txid));

    json_t *lu_vout = NULL;
    lu_vout = json_object_get(lu_data,"vout");
    tx_vout = json_integer_value(lu_vout);

    json_decref(lu_value);
    json_decref(lu_txid);
    json_decref(lu_vout);
    json_decref(lu_data);
    break;

  } 
```
Dovresti anche cancellare i tuoi elementi JSON principali:
```
}

json_decref(lu_result);
json_decref(lu_response);
```      


> :warning: **AVVERTENZA:** Un programma del mondo reale si assicurerebbe anche che gli UTXO siano `spendibili`.

Se non hai trovato UTXO abbastanza grandi, dovrai riferire questo triste fatto all'utente... e forse suggerire che dovrebbe usare un programma migliore che unirà correttamente gli UTXO.

```
if (!tx_id) {

  printf("Very Sad: You don't have any UTXOs larger than %f\n",tx_total);
  exit(-1);
}
```


> **AVVERTENZA:** Un vero programma utilizzerebbe subroutine per questo tipo di ricerca, in modo da poter chiamare varie RPC da una libreria di funzioni C con fiducia. Stiamo solo ammassando tutto in `main` come parte della nostra filosofia KISS di esempi semplici.

### Passo 5: Creare un Indirizzo di Resto

Ripeti la metodologia standard di ricerca RPC per ottenere un indirizzo di resto:

```
rpc_method = bitcoinrpc_method_init(BITCOINRPC_METHOD_GETRAWCHANGEADDRESS);

if (!rpc_method) {

  printf("ERROR: Unable to initialize listunspent method!\n");
  exit(-1);

}

bitcoinrpc_call(rpc_client, rpc_method, btcresponse, &btcerror);

if (btcerror.code != BITCOINRPCE_OK) {

printf("Error: listunspent error code %d [%s]\n", btcerror.code,btcerror.msg);

  exit(-1);

}

lu_response = bitcoinrpc_resp_get (btcresponse);
lu_result = json_object_get(lu_response,"result");
char *changeaddress = strdup(json_string_value(lu_result));
```
L'unica differenza sta in quale particolare informazione viene estratta dall'oggetto JSON.

> :warning: **AVVERTENZA:** Ecco un punto in cui una subroutine sarebbe molto utile: per astrarre l'intera inizializzazione del metodo RPC e la chiamata.

### Passo 6: Creare una Transazione Grezza

Creare la transazione grezza effettiva è l'altra parte complicata della programmazione del tuo sostituto `sendtoaddress`. Questo perché richiede la creazione di un oggetto JSON complesso come parametro.

Per creare correttamente questi parametri, dovrai rivedere cosa si aspetta l'RPC `createrawtransaction`. Fortunatamente, questo è facile da determinare utilizzando la funzionalità `bitcoin-cli help`:

```
$ bitcoin-cli help createrawtransaction
createrawtransaction [{"txid":"id","vout":n},...] {"address":amount,"data":"hex",...} ( locktime )
```
Per riassumere, i tuoi input saranno un array JSON contenente un oggetto JSON per ciascun UTXO. Poi tutti gli output saranno in un unico oggetto JSON. È più facile creare questi elementi JSON dall'interno verso l'esterno, utilizzando i comandi `jansson`. 

#### Passo 6.1: Creare i Parametri di Input

Per creare l'oggetto input per il tuo UTXO, usa `json_object`, quindi riempilo con coppie chiave-valore utilizzando `json_object_set_new` (per riferimenti di nuova creazione) o `json_object_set` (per riferimenti esistenti):

```
json_t *inputtxid = NULL;
inputtxid = json_object();

json_object_set_new(inputtxid,"txid",json_string(tx_id));
json_object_set_new(inputtxid,"vout",json_integer(tx_vout));
```
Noterai che devi di nuovo tradurre ciascun tipo di variabile C in un tipo di variabile JSON utilizzando la funzione appropriata, come `json_string` o `json_integer`.

Per creare l'array di input generale per tutti i tuoi UTXO, usa `json_array`, quindi riempilo con oggetti utilizzando `json_array_append`:

```
json_t *inputparams = NULL;
inputparams = json_array();
json_array_append(inputparams,inputtxid);
```


#### Passo 6.2: Creare i Parametri di Output

Per creare l'array di output per la tua transazione, segui lo stesso formato, creando un oggetto JSON con `json_object`, quindi riempiendolo con `json_object_set`:

```
json_t *outputparams = NULL;
outputparams = json_object();

char tx_amount_string[32];
sprintf(tx_amount_string,"%.8f",tx_amount);
char tx_change_string[32];
sprintf(tx_change_string,"%.8f",tx_value - tx_total);

json_object_set(outputparams, tx_recipient, json_string(tx_amount_string));
json_object_set(outputparams, changeaddress, json_string(tx_change_string));
```


> :warning: **AVVISO:** Potresti aspettarti di inserire i tuoi valori Bitcoin come numeri, utilizzando `json_real`. Sfortunatamente, questo espone uno dei principali problemi con l'integrazione della libreria `jansson` e Bitcoin. Bitcoin è valido solo fino a otto cifre significative dopo il punto decimale. Ricorderai che .00000001 BTC è un satoshi, e questa è la divisione più piccola possibile di un Bitcoin. I doppi in C offrono più cifre significative di così, anche se spesso sono imprecisi oltre otto decimali. Se provi a convertire direttamente il tuo valore doppio in C (o un valore float, per quella materia) in un valore Bitcoin, l'imprecisione creerà spesso un valore Bitcoin con più di otto cifre significative. Prima di Bitcoin Core 0.12 questo sembra funzionare, e potresti usare `json_real`. Ma a partire da Bitcoin Core 0.12, se provi a dare a `createrawtransaction` un valore Bitcoin con troppe cifre significative, riceverai invece un errore e la transazione non sarà creata. Di conseguenza, se il valore Bitcoin è _mai_ diventato un doppio o un float, devi riformattarlo a otto cifre significative dopo la cifra prima di inserirlo come stringa. Questo è ovviamente un trucco, quindi dovresti assicurarti che continui a funzionare nelle future versioni di Bitcoin Core.

#### Passo 6.3: Creare l'Array dei Parametri

Per finire di creare i tuoi parametri, basta racchiuderli tutti in un array JSON:

```
json_t *params = NULL;
params = json_array();
json_array_append(params,inputparams);
json_array_append(params,outputparams);
```
#### Passo 6.4: Effettuare la Chiamata RPC

Usa il metodo normale per creare la tua chiamata RPC:

```
rpc_method = bitcoinrpc_method_init(BITCOINRPC_METHOD_CREATERAWTRANSACTION);
```
Tuttavia, ora devi fornire i tuoi parametri. Questo viene fatto semplicemente con `bitcoinrpc_method_set_params`:
```
if (bitcoinrpc_method_set_params(rpc_method, params) != BITCOINRPCE_OK) {

  fprintf (stderr, "Error: Could not set params for createrawtransaction");

}
```
Successivamente, esegui l'RPC e ottieni i risultati come al solito:
```
bitcoinrpc_call(rpc_client, rpc_method, btcresponse, &btcerror);

lu_response = bitcoinrpc_resp_get(btcresponse);
lu_result = json_object_get(lu_response,"result");

char *tx_rawhex = strdup(json_string_value(lu_result));
```
### Passo 7: Firmare la Transazione

È molto più facile assegnare un semplice parametro a una funzione. Basta creare un array JSON, quindi assegnare il parametro all'array:

```
params = json_array();
json_array_append_new(params,json_string(tx_rawhex));
```
Firma la transazione seguendo la solita tiritera per creare una chiamata RPC:
```
rpc_method = bitcoinrpc_method_init(BITCOINRPC_METHOD_SIGNRAWTRANSACTION);
if (bitcoinrpc_method_set_params(rpc_method, params) != BITCOINRPCE_OK) {

  fprintf (stderr, "Error: Could not set params for signrawtransaction");

}

json_decref(params);

bitcoinrpc_call(rpc_client, rpc_method, btcresponse, &btcerror);
lu_response = bitcoinrpc_resp_get(btcresponse);
```
Ancora una volta, usare `jansson` per accedere all'output può essere un po' complicato. Qui devi ricordare che `hex` è parte di un oggetto JSON, non un risultato autonomo, come era il caso quando hai creato la transazione grezza. Naturalmente, puoi sempre accedere a queste informazioni dall'aiuto della riga di comando: `bitcoin-cli help signrawtransaction`:

```
lu_result = json_object_get(lu_response,"result");
json_t *lu_signature = json_object_get(lu_result,"hex");
char *tx_signrawhex = strdup(json_string_value(lu_signature));
json_decref(lu_signature);
```


> :warning: **AVVERTENZA:** Un programma del mondo reale ovviamente testerebbe attentamente la risposta di ogni comando RPC per assicurarsi che non ci siano errori. Questo è particolarmente vero per `signrawtransaction`, perché potresti finire con una transazione parzialmente firmata. Peggio ancora, se non controlli gli errori nell'oggetto JSON, vedrai solo l'`hex` e non ti renderai conto che è non firmato o parzialmente firmato. 

### Passo 8: Inviare la Transazione

Ora puoi inviare la tua transazione, usando tutte le tecniche precedenti:

```
params = json_array();
json_array_append_new(params,json_string(tx_signrawhex));

rpc_method = bitcoinrpc_method_init(BITCOINRPC_METHOD_SENDRAWTRANSACTION);

if (bitcoinrpc_method_set_params(rpc_method, params) != BITCOINRPCE_OK) {

  fprintf (stderr, "Error: Could not set params for sendrawtransaction");

}

json_decref(params);

bitcoinrpc_call(rpc_client, rpc_method, btcresponse, &btcerror);
lu_response = bitcoinrpc_resp_get(btcresponse);
lu_result = json_object_get(lu_response,"result");

char *tx_newid = strdup(json_string_value(lu_result));

printf("Txid: %s\n",tx_newid);
```
L'intero codice, con un _po' di_ controllo degli errori in più, appare nell'Appendice.

## Testare il Tuo Codice

Il codice completo può essere trovato nella [directory src](src/16_2_sendtoaddress.c).

Compila questo come al solito:

```
$ cc sendtoaddress.c -lbitcoinrpc -ljansson -o sendtoaddress
```
Puoi quindi usarlo per inviare fondi a un indirizzo:

```
./sendtoaddress tb1qynx7f8ulv4sxj3zw5gqpe56wxleh5dp9kts7ns .001
Txid: b93b19396f8baa37f5f701c7ca59d3128144c943af5294aeb48e3eb4c30fa9d2
```
Puoi vedere le informazioni su questa transazione che abbiamo inviato [qui](https://live.blockcypher.com/btc-testnet/tx/b93b19396f8baa37f5f701c7ca59d3128144c943af5294aeb48e3eb4c30fa9d2/).

## Riepilogo: Programmare Bitcoind con C

Con l'accesso a una libreria C, puoi creare programmi molto più completi di quanto fosse ragionevole fare con script di shell. Ma può richiedere molto lavoro! Anche con 316 linee di codice, `sendtoaddress.c` non copre quasi tutte le complessità necessarie per transare bitcoin in modo sicuro e intelligente.

## Cosa Succede Dopo?

Scopri di più su "Parlare con Bitcoind in C" nel [Capitolo 16.3: Ricevere Notifiche di Bitcoind in C tramite Librerie ZMQ](16_3_Ricevere_Notifiche_di_Bitcoind_in_C_tramite_Librerie_ZMQ.md).
