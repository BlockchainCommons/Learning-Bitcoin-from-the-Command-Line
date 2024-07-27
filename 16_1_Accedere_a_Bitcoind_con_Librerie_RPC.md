# 16.1: Accesso a Bitcoind in C con Librerie RPC

> :information_source: **NOTA:** Questa sezione è stata recentemente aggiunta al corso ed è una bozza iniziale che potrebbe essere ancora in attesa di revisione. Lettore avvisato.

Hai già visto un modo alternativo per accedere alle porte RPC di Bitcoind: `curl`, che è stato trattato in un [Interludio del Capitolo 4](04_4__Interlude_Using_Curl.md). Interagire con `bitcoind` tramite una libreria RPC in C non è diverso da questo, hai solo bisogno di alcune buone librerie per aiutarti. Questa sezione introduce un pacchetto chiamato `libbitcoinrpc`, che ti permette di accedere alla porta JSON-RPC di `bitcoind`. Utilizza la libreria `curl` per accedere ai dati e la libreria `jansson` per codificare e decodificare il JSON.

## Configurare libbitcoinrpc

> :warning: **AVVERTENZA** Sembra che `libbitcoinrpc` sia stato completamente abbandonato. Abbiamo registrato l'aggiornamento a una nuova libreria C come un [problema](https://github.com/BlockchainCommons/Community/issues/140). Nel frattempo, la libreria `libbitcoinrpc` attualmente non compila senza interventi. Di conseguenza, le sezioni 16.1 e 16.2 sono principalmente visualizzabili come pseudo-codice che mostra il processo di integrazione di Bitcoin-Core con C.

Per utilizzare `libbitcoinrpc`, devi installare una configurazione di base di C e i pacchetti dipendenti `libcurl`, `libjansson` e `libuuid`. I seguenti comandi lo faranno sul tuo server Bitcoin Standup (o su qualsiasi altro server Ubuntu).
.
```
$ sudo apt-get install make gcc libcurl4-openssl-dev libjansson-dev uuid-dev
Suggested packages:
  libcurl4-doc libidn11-dev libkrb5-dev libldap2-dev librtmp-dev libssh2-1-dev
The following NEW packages will be installed:
  libcurl4-openssl-dev libjansson-dev uuid-dev
0 upgraded, 3 newly installed, 0 to remove and 4 not upgraded.
Need to get 358 kB of archives.
After this operation, 1.696 kB of additional disk space will be used.
Do you want to continue? [Y/n] y
```

Puoi quindi scaricare [libbitcoinrpc da Github](https://github.com/BlockchainCommons/libbitcoinrpc/blob/master/README.md). Clonalo o prendi un file zip, come preferisci.

```
$ sudo apt-get install git
$ git clone https://github.com/BlockchainCommons/libbitcoinrpc.git
```

> :warning: **AVVERTENZA** Una modifica nella RPC "signrawtransaction" ha causato il crash con segmentazione (segfault) con `libbitcoinrpc` per Bitcoin 0.17 o superiore. È stato presentato un [PR](https://github.com/gitmarek/libbitcoinrpc/pull/1) per risolvere il problema, ma se non è ancora stato unito, puoi semplicemente apportare la modifica nel codice sorgente a `src/bitcoinrpc_method.c` prima di compilare.

### Compilare libbitcoinrpc

Prima di poter compilare e installare il pacchetto, probabilmente dovrai regolare il tuo `$PATH`, in modo da poter accedere a `/sbin/ldconfig`:

```
$ PATH="/sbin:$PATH"
```
Per un sistema Ubuntu, vorrai anche regolare il `INSTALL_LIBPATH` nel `Makefile` di `libbitcoinrpc` per installarlo in `/usr/lib` invece di `/usr/local/lib`:
```
$ emacs ~/libbitcoinrpc/Makefile
...
INSTALL_LIBPATH    := $(INSTALL_PREFIX)/usr/lib
```
(Se preferisci non sporcare il tuo `/usr/lib`, l'alternativa è modificare il tuo `etc/ld.so.conf` o i suoi file dipendenti in modo appropriato... ma per una configurazione di test su una macchina di test, questo va probabilmente bene.)

Allo stesso modo, vorrai anche regolare il `INSTALL_HEADERPATH` nel `Makefile` di `libbitcoinrpc` per installarlo in `/usr/include` invece di `/usr/local/include`:
```
...
INSTALL_HEADERPATH    := $(INSTALL_PREFIX)/usr/include
```


Poi puoi compilare:
```
$ cd libbitcoinrpc
~/libbitcoinrpc$ make

gcc -fPIC -O3 -g -Wall -Werror -Wextra -std=c99 -D VERSION=\"0.2\" -o src/bitcoinrpc_err.o -c src/bitcoinrpc_err.c
gcc -fPIC -O3 -g -Wall -Werror -Wextra -std=c99 -D VERSION=\"0.2\" -o src/bitcoinrpc_global.o -c src/bitcoinrpc_global.c
gcc -fPIC -O3 -g -Wall -Werror -Wextra -std=c99 -D VERSION=\"0.2\" -o src/bitcoinrpc.o -c src/bitcoinrpc.c
gcc -fPIC -O3 -g -Wall -Werror -Wextra -std=c99 -D VERSION=\"0.2\" -o src/bitcoinrpc_resp.o -c src/bitcoinrpc_resp.c
gcc -fPIC -O3 -g -Wall -Werror -Wextra -std=c99 -D VERSION=\"0.2\" -o src/bitcoinrpc_cl.o -c src/bitcoinrpc_cl.c
gcc -fPIC -O3 -g -Wall -Werror -Wextra -std=c99 -D VERSION=\"0.2\" -o src/bitcoinrpc_method.o -c src/bitcoinrpc_method.c
gcc -fPIC -O3 -g -Wall -Werror -Wextra -std=c99 -D VERSION=\"0.2\" -shared -Wl,-soname,libbitcoinrpc.so.0 \
src/bitcoinrpc_err.o src/bitcoinrpc_global.o src/bitcoinrpc.o src/bitcoinrpc_resp.o src/bitcoinrpc_cl.o src/bitcoinrpc_method.o \
-o .lib/libbitcoinrpc.so.0.2 \
-Wl,--copy-dt-needed-entries -luuid -ljansson -lcurl
ldconfig -v -n .lib
.lib:
	libbitcoinrpc.so.0 -> libbitcoinrpc.so.0.2 (changed)
ln -fs libbitcoinrpc.so.0 .lib/libbitcoinrpc.so
```
Se funziona, puoi installare il pacchetto:
```
$ sudo make install
Installing to
install .lib/libbitcoinrpc.so.0.2 /usr/local/lib
ldconfig  -n /usr/local/lib
ln -fs libbitcoinrpc.so.0 /usr/local/lib/libbitcoinrpc.so
install -m 644 src/bitcoinrpc.h /usr/local/include
Installing docs to /usr/share/doc/bitcoinrpc
mkdir -p /usr/share/doc/bitcoinrpc
install -m 644 doc/*.md /usr/share/doc/bitcoinrpc
install -m 644 CREDITS /usr/share/doc/bitcoinrpc
install -m 644 LICENSE /usr/share/doc/bitcoinrpc
install -m 644 Changelog.md /usr/share/doc/bitcoinrpc
Installing man pages
install -m 644 doc/man3/bitcoinrpc*.gz /usr/local/man/man3
```


## Preparare il Codice

`libbitcoinrpc` ha metodi ben strutturati e semplici per connettersi al tuo `bitcoind`, eseguire chiamate RPC e decodificare la risposta.

Per utilizzare `libbitcoinrpc`, assicurati che i tuoi file di codice includano le intestazioni appropriate:

```
#include <jansson.h>
#include <bitcoinrpc.h>
```
Avrai anche bisogno di collegare le librerie appropriate ogni volta che compili:
```
$ cc yourcode.c -lbitcoinrpc -ljansson -o yourcode
```


## Costruire la Connessione

Costruire la connessione al tuo server `bitcoind` richiede pochi semplici passaggi.

Innanzitutto, inizializza la libreria:

```
bitcoinrpc_global_init();
```
Poi connettiti al tuo `bitcoind` con `bitcoinrpc_cl_init_params`. I quattro argomenti per `bitcoinrpc_cl_init_params` sono nome utente, password, indirizzo IP e porta. Dovresti già conoscere tutte queste informazioni dal tuo lavoro con [Curl](04_4__Interlude_Using_Curl.md). Come ricorderai, l'indirizzo IP 127.0.0.1 e la porta 18332 dovrebbero essere corretti per la configurazione standard del testnet descritta in questi documenti, mentre puoi estrarre l'utente e la password da `~/.bitcoin/bitcoin.conf`.
```
$ cat bitcoin.conf
server=1
dbcache=1536
par=1
maxuploadtarget=137
maxconnections=16
rpcuser=StandUp
rpcpassword=6305f1b2dbb3bc5a16cd0f4aac7e1eba
rpcallowip=127.0.0.1
debug=tor
prune=550
testnet=1
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
Che poi inserisci in `bitcoinrpc_cl_init_params`:
```
bitcoinrpc_cl_t *rpc_client;
rpc_client = bitcoinrpc_cl_init_params("StandUp", "6305f1b2dbb3bc5a16cd0f4aac7e1eba", "127.0.0.1", 18332);
```


> **MAINNET VS TESTNET:** La porta sarebbe 8332 per una configurazione mainnet.

Se `rpc_client` viene inizializzato correttamente, sarai in grado di inviare comandi RPC.

Successivamente, quando avrai finito con la tua connessione `bitcoind`, dovresti chiuderla:
```
bitcoinrpc_global_cleanup();
```


### Testare il Codice di Test

Il codice di test può essere trovato in [16_1_testbitcoin.c nella directory src](src/16_1_testbitcoin.c). Scaricalo sulla tua macchina testnet, quindi inserisci la password RPC corretta (e cambia l'utente RPC se non hai creato il tuo server con StandUp).

Puoi compilare ed eseguire questo come segue:

```
$ cc testbitcoin.c -lbitcoinrpc -ljansson -o testbitcoin
$ ./testbitcoin
Successfully connected to server!
```


> :warning: **AVVERTENZA:** Se dimentichi di inserire la tua password RPC in questo o in qualsiasi altro esempio di codice che dipende da RPC, riceverai un misterioso `ERROR CODE 5`.

## Effettuare una Chiamata RPC

Per utilizzare un metodo RPC utilizzando `libbitcoinrpc`, devi inizializzare una variabile di tipo `bitcoinrpc_method_t`. Lo fai con il valore appropriato per il metodo che vuoi utilizzare, tutti elencati nel [Reference di bitcoinrpc](https://github.com/BlockchainCommons/libbitcoinrpc/blob/master/doc/reference.md).

```
bitcoinrpc_method_t *getmininginfo  = NULL;
getmininginfo = bitcoinrpc_method_init(BITCOINRPC_METHOD_GETMININGINFO);
```
Di solito imposteresti i parametri successivamente, ma `getmininginfo` non richiede parametri, quindi puoi saltare questo passaggio per ora.

Devi anche creare altri due oggetti, un "oggetto di risposta" e un "oggetto di errore". Possono essere inizializzati come segue:

```
bitcoinrpc_resp_t *btcresponse  = NULL;
btcresponse = bitcoinrpc_resp_init();

bitcoinrpc_err_t btcerror;
```
Usi la variabile `rpc_client` di cui hai già appreso nel test precedente, e aggiungi il tuo metodo `getmininginfo` e i due altri oggetti:
```
bitcoinrpc_call(rpc_client, getmininginfo, btcresponse, &btcerror);
```
### Output della Tua Risposta

Vorrai sapere cosa ha restituito la chiamata RPC. Per farlo, recupera l'output della tua chiamata come oggetto JSON con `bitcoinrpc_resp_get` e salvalo in un oggetto `jansson` standard, di tipo `json_t`:

```
json_t *jsonresponse = NULL;
jsonresponse = bitcoinrpc_resp_get(btcresponse);
```
Se vuoi _outputtare_ i risultati JSON completi della chiamata RPC, puoi farlo con una semplice invocazione di `json_dumps`, anche dalla libreria `jansson`:
```
printf ("%s\n", json_dumps(j, JSON_INDENT(2)));
```

Tuttavia, poiché ora stai scrivendo programmi completi, probabilmente vorrai fare lavori più sottili, come estrarre valori JSON individuali per un uso specifico. Il [Reference di jansson](https://jansson.readthedocs.io/en/2.10/apiref.html) dettaglia come farlo.

Proprio come quando utilizzavi [Curl](04_2_Intermezzo_Usare_JQ.md), scoprirai che le RPC restituiscono un oggetto JSON contenente un `id`, un `error`, e soprattutto un oggetto JSON del `result`.

La funzione `json_object_get` ti permetterà di recuperare un valore (come il `result`) da un oggetto JSON per chiave:

```
json_t *jsonresult = NULL;
jsonresult = json_object_get(jsonresponse,"result");
printf ("%s\n", json_dumps (jsonresult, JSON_INDENT(2)));
```
Tuttavia, probabilmente vorrai approfondire ulteriormente, per ottenere una variabile specifica. Una volta recuperato il valore appropriato, dovrai convertirlo in un oggetto C standard utilizzando la funzione `json_*_value` appropriata. Ad esempio, accedere a un intero utilizza `json_integer_value`:
```
json_t *jsonblocks = NULL;
jsonblocks = json_object_get(jsonresult,"blocks");

int blocks;
blocks = json_integer_value(jsonblocks);
printf("Block Count: %d\n",blocks);
```


> :warning: **AVVERTENZA:** È estremamente facile far crashare il tuo codice C con segmentazione quando lavori con oggetti `jansson` se ti confondi con il tipo di oggetto che stai recuperando. Fai un uso attento di `bitcoin-cli help` per sapere cosa aspettarti, e se sperimenti un errore di segmentazione, prima guarda le tue funzioni di recupero JSON.

### Testare il Codice Info

Recupera il codice di test dalla [directory src](src/16_1_getmininginfo.c).

```
$ cc getmininginfo.c -lbitcoinrpc -ljansson -o getmininginfo
$ ./getmininginfo
Full Response: {
  "result": {
    "blocks": 1804406,
    "difficulty": 4194304,
    "networkhashps": 54842097951591.781,
    "pooledtx": 127,
    "chain": "test",
    "warnings": "Warning: unknown new rules activated (versionbit 28)"
  },
  "error": null,
  "id": "474ccddd-ef8c-4e3f-93f7-fde72fc08154"
}

Just the Result: {
  "blocks": 1804406,
  "difficulty": 4194304,
  "networkhashps": 54842097951591.781,
  "pooledtx": 127,
  "chain": "test",
  "warnings": "Warning: unknown new rules activated (versionbit 28)"
}

Block Count: 1804406
```
## Effettuare una Chiamata RPC con Argomenti

Ma cosa succede se la tua chiamata RPC _ha_ argomenti?

### Creare un Array JSON

Per inviare parametri alla tua chiamata RPC utilizzando `libbitcoinrpc` devi avvolgerli in un array JSON. Poiché un array è semplicemente un elenco ordinato di valori, tutto ciò che devi fare è codificare i parametri come elementi ordinati nell'array.

Crea l'array JSON utilizzando la funzione `json_array` di `jansson`:

```
json_t *params = NULL;
params = json_array();
```
Invertirai quindi la procedura che hai seguito per accedere ai valori JSON: convertirai gli oggetti di tipo C in oggetti di tipo JSON utilizzando le funzioni `json_*`. Successivamente, li aggiungerai all'array:
```
json_array_append_new(params,json_string(tx_rawhex));
```
Nota che ci sono due varianti del comando append: `json_array_append_new`, che aggiunge una variabile appena creata, e `json_array_append`, che aggiunge una variabile esistente.

Questa semplice metodologia `json_array_append_new` servirà per la maggior parte dei comandi RPC con parametri, ma alcuni comandi RPC richiedono input più complessi. In questi casi, potrebbe essere necessario creare oggetti JSON o array JSON sussidiari, che aggiungerai quindi all'array dei parametri come al solito. La sezione successiva contiene un esempio di come farlo utilizzando `createrawtransaction`, che contiene un array JSON di oggetti JSON per gli input, un oggetto JSON per gli output e il parametro `locktime`.

### Assegnare i Parametri

Quando hai creato il tuo array JSON dei parametri, lo assegni semplicemente dopo aver inizializzato il tuo metodo RPC, come segue:

```
bitcoinrpc_method_set_params(rpc_method, params)
```
Questa sezione non include un esempio completo di questa metodologia più complessa, ma la vedremo in azione più volte nel nostro primo programma completo basato su RPC in C, nella prossima sezione.

## Riepilogo: Accesso a Bitcoind con C

Collegandoti alle librerie RPC `bitcoinrpc` e JSON `jansson`, puoi facilmente accedere a `bitcoind` tramite chiamate RPC da una libreria C. Per farlo, crei una connessione RPC, quindi effettui chiamate RPC individuali, alcune con parametri. `jansson` ti permette poi di decodificare le risposte JSON. La prossima sezione dimostrerà come questo possa essere utilizzato per un programma pragmatico e reale.

* :fire: ***Qual è il potere di C?*** C ti permette di fare il passo successivo oltre il scripting shell, permettendo la creazione di programmi più completi e robusti.

## Cosa Succede Dopo?

Scopri di più su "Parlare con Bitcoind in C" nel [Capitolo 16.2: Programare Bitcoind in C con Librerie RPC](16_2_Programare_Bitcoind_in_C_con_Librerie_RPC.md).
