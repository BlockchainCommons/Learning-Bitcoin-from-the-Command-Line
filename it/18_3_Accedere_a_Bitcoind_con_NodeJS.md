# 18.3: Accedere a Bitcoind con NodeJS

> :information_source: **NOTA:** Questa sezione è stata aggiunta di recente al corso ed è una bozza iniziale che potrebbe essere ancora in attesa di revisione. Lettore avvisato.

Questa sezione spiega come interagire con `bitcoind` utilizzando il linguaggio di programmazione NodeJS e il [pacchetto BCRPC](https://github.com/dgarage/bcrpc).

## Set Up Node.js

BCRPC è costruito su node.js. Pertanto, dovrai prima installare i pacchetti `node.js` e `npm` (il gestore di pacchetti node) per il tuo sistema.

Se stai usando una macchina Ubuntu, puoi eseguire i seguenti comandi per ottenere una nuova versione di `node.js` (invece della versione molto datata nel sistema di pacchetti di Ubuntu).

```
$ curl -sL https://deb.nodesource.com/setup_14.x | sudo bash -
$ sudo apt-get install -y nodejs
$ sudo npm install mocha -g
```

### Configurare BCRPC

Ora puoi clonare il pacchetto BCRPC da GitHub e installare le sue dipendenze.

```
$ git clone https://github.com/dgarage/bcrpc.git
$ cd bcrpc
$ npm install
```

Per testare il pacchetto BCRPC, devi prima impostare le variabili ambientali per il tuo rpcuser e rpcpassword. Come al solito, questi provengono da `~/.bitcoin/bitcoin.conf`. Devi anche impostare la porta RPC a 18332, che dovrebbe essere corretta per la configurazione standard del testnet descritta in questi documenti.

```
$ export BITCOIND_USER=StandUp
$ export BITCOIND_PASS=d8340efbcd34e312044c8431c59c792c
$ export BITCOIND_PORT=18332
```

> :warning: **WARNING:** Ovviamente, non metteresti mai la tua password in una variabile ambientale in un ambiente di produzione.

> :link: **MAINNET VS TESTNET:** La porta sarebbe 8332 per una configurazione mainnet.

Ora puoi verificare che tutto funzioni correttamente:

```
$ npm test
> bcrpc@0.2.2 test /home/user1/bcrpc
> mocha tests.js
  BitcoinD
    ✓ is running
  bcrpc
    ✓ can get info
  2 passing (36ms)
```

Congratulazioni, ora hai un wrapper RPC pronto per Bitcoin per Node.js che funziona con la tua configurazione di Bitcoin.

### Creare un Progetto BCRPC 

Ora puoi creare un nuovo progetto Node.js e installare BCRPC tramite npm.

```
$ cd ~
$ mkdir myproject
$ cd myproject
$ npm init
  [continue with default options]
$ npm install bcrpc
```

## Costruire la Tua Connessione

Nella tua directory ```myproject```, crea un file `.js` dove verrà eseguito il tuo codice JavaScript.

Puoi iniziare una connessione RPC creando un `RpcAgent`:

```
const RpcAgent = require('bcrpc');
agent = new RpcAgent({port: 18332, user: 'StandUp', pass: 'd8340efbcd34e312044c8431c59c792c'});
```

Ovviamente, il tuo `user` e `pass` dovrebbero corrispondere nuovamente a quelli presenti nel tuo `~/.bitcoin/bitcoin.conf`, e usi `port 18332` se sei su testnet.

### Effettuare una Chiamata RPC

Utilizzando BCRPC, puoi utilizzare gli stessi comandi RPC che useresti di solito tramite `bitcoin-cli` con il tuo `RpcAgent`, tranne che devono essere in camelCase. Ad esempio, `getblockhash` diventerebbe `getBlockHash`.

Per stampare il numero del blocco più recente, basta chiamare `getBlockCount` tramite il tuo `RpcAgent`:

```
agent.getBlockCount(function (err, blockCount) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(blockCount.result);
});
```

### Effettuare una Chiamata RPC con Argomenti

Le funzioni BCRPC possono accettare argomenti. Ad esempio, `getBlockHash` prende `blockCount.result` come input.

```  
  agent.getBlockHash(blockCount.result, function (err, hash) {
    if (err)
      throw Error(JSON.stringify(err));
    console.log(hash.result);
  })
```

Il risultato delle funzioni BCRPC è un oggetto JSON contenente informazioni su eventuali errori e l'id della richiesta. Quando accedi al tuo risultato, aggiungi `.result` alla fine per specificare che sei interessato al risultato effettivo, non alle informazioni sugli errori.

### Eseguire il Codice

Puoi trovare il codice `getinfo` nella [directory src](src/18_3_getinfo.js).

```
$ node getinfo.js
1831094
00000000000002bf8b522a830180ad3a93b8eed33121f54b3842d8838580a53c
```

Questo è l'aspetto che avrebbe l'output dell'esempio sopra se sostituissi `console.log(blockCount.result);` e `console.log(hash.result);` con
`console.log(blockCount);` e `console.log(hash);`, rispettivamente:

```
{ result: 1774686, error: null, id: null }
{
  result: '00000000000000d980c495a2b7addf09bb0a9c78b5b199c8e965ee54753fa5da',
  error: null,
  id: null
}
```

## Cercare Fondi

È utile, quando si accetta Bitcoin, controllare i Bitcoin ricevuti su un indirizzo specifico nel tuo portafoglio. Ad esempio, se gestissi un negozio online che accetta Bitcoin, per ogni pagamento da un cliente, genereresti un nuovo indirizzo, mostreresti quell'indirizzo al cliente, quindi controlleresti il saldo dell'indirizzo dopo un po' di tempo, per assicurarti che l'importo corretto sia stato ricevuto:

```
agent.getReceivedByAddress('mpGpCMX6SuUimDZKiVViuhd7EGyVxkNnha', function (err, addressInfo) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(addressInfo.result);
});
```

> :information_source: **NOTE:** Ovviamente, dovrai inserire un indirizzo riconosciuto dalla tua macchina.

Per impostazione predefinita, questa funzione controlla le transazioni che sono state confermate una volta, tuttavia puoi aumentare questo numero a un numero più alto come 6:

```
agent.getReceivedByAddress('mpGpCMX6SuUimDZKiVViuhd7EGyVxkNnha', 6, function (err, addressInfo) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(addressInfo.result);
});
```

### Cercare informazione sul Wallet

Puoi anche cercare informazioni aggiuntive sul tuo portafoglio e visualizzare il tuo saldo, il conteggio delle transazioni, eccetera:

```
agent.getWalletInfo(function (err, walletInfo) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(walletInfo.result);
});
```

Il codice sorgente è disponibile come [walletinfo.js](src/18_3_walletinfo.js).

```
$ node walletinfo.js
0.008498
{
  walletname: '',
  walletversion: 169900,
  balance: 0.010438,
  unconfirmed_balance: 0,
  immature_balance: 0,
  txcount: 4,
  keypoololdest: 1596567843,
  keypoolsize: 999,
  hdseedid: 'da5a1b058deb9e51ecffef1b0ddc069a5dfb2c5f',
  keypoolsize_hd_internal: 1000,
  paytxfee: 0,
  private_keys_enabled: true,
  avoid_reuse: false,
  scanning: false
}
```

Invece di stampare tutti i dettagli associati al tuo portafoglio, puoi stampare informazioni specifiche, come il tuo saldo. Poiché viene accesso un oggetto JSON, questo può essere fatto cambiando la riga `console.log(walletInfo.result);` in `console.log(walletInfo.result.balance);`:

## Creare un Indirizzo

Puoi anche passare argomenti aggiuntivi ai comandi RPC. Ad esempio, il seguente genera un nuovo indirizzo legacy, con il flag `-addresstype`.

```
agent.getNewAddress('-addresstype', 'legacy', function (err, newAddress) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(newAddress.result);
});
```

Questo è lo stesso che eseguire il seguente comando dal terminale:

```
$ bitcoin-cli getnewaddress -addresstype legacy
mtGPcBvRPZFEHo2YX8un9qqPBydhG82uuZ
```

In BCRPC, puoi generalmente usare gli stessi flag di `bitcoin-cli` in BCRPC. Sebbene tu usi camelCase (`getNewAddress`) per i metodi, i flag, che sono normalmente separati da spazi sulla riga di comando, sono invece posti in stringhe e separati da virgole.

## Inviare una Transazione

Puoi inviare monete a un indirizzo più facilmente utilizzando la funzione `sendToAddress`:

```
agent.sendToAddress(newAddress.result, 0.00001, function(err, txid) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(txid.result);
});
```

Questo dovrebbe stampare il txid della transazione:

```
1679bee019c61608340b79810377be2798efd4d2ec3ace0f00a1967af70666b9
```

### Cercare una Transazione

Potresti ora voler visualizzare una transazione, come quella che hai appena inviato.

```
agent.getTransaction(txid.result, function (err, transaction) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(transaction.result);
});
```

Dovresti ottenere un output simile a questo:

```
{
  amount: 0.001,
  confirmations: 4776,
  blockhash: '000000006628870b0a8a66abea9cf0d4e815c491f079e3fa9e658a87b5dc863a',
  blockindex: 117,
  blocktime: 1591857418,
  txid: '1661ce322c128e053b8ea8fcc22d17df680d2052983980e2281d692b9b4ab7df',
  walletconflicts: [],
  time: 1591857343,
  timereceived: 1591857343,
  'bip125-replaceable': 'no',
  details: [
    {
      address: 'mpGpCMX6SuUimDZKiVViuhd7EGyVxkNnha',
      category: 'receive',
      amount: 0.001,
      label: '',
      vout: 0
    }
  ],
  hex: '02000000000101e9e8c3bd057d54e73baadc60c166860163b0e7aa60cab33a03e89fb44321f8d5010000001716001435c2aa3fc09ea53c3e23925c5b2e93b9119b2568feffffff02a0860100000000001976a914600c8c6a4abb0a502ea4de01681fe4fa1ca7800688ac65ec1c000000000017a91425b920efb2fde1a0277d3df11d0fd7249e17cf8587024730440220403a863d312946aae3f3ef0a57206197bc67f71536fb5f4b9ca71a7e226b6dc50220329646cf786cfef79d60de3ef54f702ab1073694022f0618731902d926918c3e012103e6feac9d7a8ad1ac6b36fb4c91c1c9f7fff1e7f63f0340e5253a0e4478b7b13f41fd1a00'
}
```

Il codice completo è disponibile come [sendtx.js](src/18_3_sendtx.js).

## Sommario: Accesso a Bitcoind con Node

Con BCRPC puoi accedere a tutti i comandi RPC disponibili tramite `bitcoin-cli`, in JavaScript. Il [README di BCRPC](https://github.com/dgarage/bcrpc) ha alcuni esempi che usano promesse (gli esempi in questo documento usano callback). Il [JavaScript dietro di esso](https://github.com/dgarage/bcrpc/blob/master/index.js) è breve e leggibile.

Sulla base di questi esempi, dovresti essere in grado di incorporare Bitcoin in un progetto Node.js e fare cose come inviare e ricevere monete.

## What's Next?

Learn more about "Talking to Bitcoin in Other Languages" nel [Capitolo 18.4: Accedere a Bitcoind con Python](18_4_Accedere_a_Bitcoind_con_Python.md).
