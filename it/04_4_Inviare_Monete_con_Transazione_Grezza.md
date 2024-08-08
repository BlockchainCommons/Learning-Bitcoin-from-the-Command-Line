
# 4.4: Inviare monete con transazioni grezze

Come notato all'inizio di questo capitolo, l'interfaccia `bitcoin-cli` offre tre modi principali per inviare monete. [Capitolo 4.1](04_1_Inviare_Monete_Modo_Semplice.md) ha parlato dell'invio nel primo modo, utilizzando il comando `sendtoaddress`. Da allora, abbiamo sviluppato dettagli su come inviare monete in un secondo modo, con transazioni grezze. [Capitolo 4.2](04_2_Creare_una_Transazione_Grezza.md) ha insegnato come creare una transazione non elaborata, nel [Interludio](04_2_Intermezzo_Usare_JQ.md) è stato spiegato JQ e nel[Capitolo 4.3](04_3_Creare_una_Transazione_Grezza_con_Alias.md) è stato spiegato come usare alias.

Ora possiamo metterli insieme e inviare effettivamente fondi utilizzando una transazione grezza.

## Crea un indirizzo di resto

La nostra transazione grezza di esempio nella sezione §4.2 era molto semplicistica: abbiamo inviato l'intero UTXO a un nuovo indirizzo. Più frequentemente, vorrai inviare a qualcuno una somma di denaro che non corrisponde a un UTXO. Ma ricorderai che il denaro in eccesso da un UTXO che non viene inviato al destinatario diventa semplicemente una commissione di transazione. Quindi, come puoi inviare a qualcuno solo una parte di un UTXO, mantenendo il _resto_ per te?

La soluzione è _inviare_ il resto dei fondi a un secondo indirizzo, un indirizzo di resto che hai creato nel tuo portafoglio appositamente per riceverli:

```
$ changeaddress=$(bitcoin-cli getrawchangeaddress legacy)
$ echo $changeaddress
mk9ry5VVy8mrA8SygxSQQUDNSSXyGFot6h
```
Tieni presente che utilizza una nuova funzione: `getrawchangeaddress`. È in gran parte uguale a `getnewaddress` ma è ottimizzato per l'uso come cambio di indirizzo in una transazione grezza, quindi non fa cose come inserire voci nella tua rubrica. Abbiamo nuovamente selezionato l'indirizzo "legacy", invece di utilizzare l'indirizzo predefinito "bech32", semplicemente per coerenza. Questa è una situazione in cui sarebbe stato del tutto sicuro generare un indirizzo Bech32 predefinito, semplicemente utilizzando `bitcoin-cli getrawchangeaddress`, perché verrebbe inviato e ricevuto da te sul tuo nodo Bitcoin Core che lo supporta pienamente. Ma sposteremo anche questo su Bech32 nel [Capitolo 4.6](04_6_Creare_una_Transazione_Segwit.md).

Ora hai un indirizzo aggiuntivo nel tuo portafoglio, così puoi ricevere il resto da un UTXO! Per utilizzarlo, dovrai creare una transazione grezza con due output.

## Scegli un numero sufficiente di UTXO

La nostra transazione grezza di esempio era semplice anche per un altro aspetto: presupponeva che ci fosse abbastanza denaro in un singolo UTXO per coprire la transazione. Spesso sarà così, ma a volte vorrai creare transazioni che spendono più soldi di quelli che hai in un singolo UTXO. Per fare ciò, è necessario creare una transazione grezza con due (o più) input.

## Scrivi una vera transazione grezza

Per riassumere: la creazione di una vera transazione grezza per inviare monete a volte richiederà più input e quasi sempre richiederà più output, uno dei quali è un indirizzo di resto. Creeremo questo tipo di transazione più realistica qui, in un nuovo esempio che mostra un esempio di vita reale di invio di fondi tramite la seconda metodologia di Bitcoin, le transazioni grezze.

Utilizzeremo il UTXO numero 0 e il numero 2:

```
$ bitcoin-cli listunspent
[
[
  {
    "txid": "0619fecf6b2668fab1308fbd7b291ac210932602a6ac6b8cc11c7ae22c43701e",
    "vout": 1,
    "address": "mwJL7cRiW2bUnY81r1thSu3D4jtMmwyU6d",
    "label": "",
    "scriptPubKey": "76a914ad1ed1c5971b2308f89c1362d4705d020a40e8e788ac",
    "amount": 0.00899999,
    "confirmations": 1,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/4']03eae28c93035f95a620dd96e1822f2a96e0357263fa1f87606a5254d5b9e6698f)#wwnfx2sp",
    "safe": true
  },
  {
    "txid": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
    "vout": 0,
    "address": "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
    "label": "",
    "scriptPubKey": "76a9142d573900aa357a38afd741fbf24b075d263ea6e088ac",
    "amount": 0.00022000,
    "confirmations": 15,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/3']0278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132)#nhjc3f8y",
    "safe": true
  },  
  {
    "txid": "0df23a9dba49e822bbc558f15516f33021a64a5c2e48962cec541e0bcc79854d",
    "vout": 0,
    "address": "mwJL7cRiW2bUnY81r1thSu3D4jtMmwyU6d",
    "label": "",
    "scriptPubKey": "76a914ad1ed1c5971b2308f89c1362d4705d020a40e8e788ac",
    "amount": 0.00100000,
    "confirmations": 1,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/4']03eae28c93035f95a620dd96e1822f2a96e0357263fa1f87606a5254d5b9e6698f)#wwnfx2sp",
    "safe": true
   }
]

```
Nel nostro esempio, invieremo .009 BTC, che è (appena) più grande di uno dei nostri UTXO. Ciò richiede la loro combinazione, quindi l'utilizzo del nostro indirizzo di resto per recuperare i fondi non spesi.

### Imposta le tue variabili

Abbiamo già le variabili `$changeaddress` e `$recipient` degli esempi precedenti:

```
$ echo $changeaddress
mk9ry5VVy8mrA8SygxSQQUDNSSXyGFot6h
$ echo $recipient
n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi
```
Dobbiamo anche registrare txid e vout per ciascuno dei nostri due UTXO. Dopo aver identificato gli UTXO che vogliamo spendere, possiamo utilizzare le nostre tecniche JQ per assicurarci che l'accesso ad essi sia privo di errori:
```
$ utxo_txid_1=$(bitcoin-cli listunspent | jq -r '.[0] | .txid')
$ utxo_vout_1=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
$ utxo_txid_2=$(bitcoin-cli listunspent | jq -r '.[2] | .txid')
$ utxo_vout_2=$(bitcoin-cli listunspent | jq -r '.[2] | .vout')
```

### Scrivere la transazione 

Scrivere la transazione grezza effettiva è sorprendentemente semplice. Tutto quello che devi fare è includere un oggetto JSON aggiuntivo, separato da virgole nell'array JSON di input e un'ulteriore coppia chiave-valore separata da virgole nell'oggetto JSON di output.

Ecco l'esempio. Nota gli input multipli dopo l'argomento 'inputs' e gli output multipli dopo l'argomento 'outputs'.
```
$ rawtxhex2=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid_1'", "vout": '$utxo_vout_1' }, { "txid": "'$utxo_txid_2'", "vout": '$utxo_vout_2' } ]''' outputs='''{ "'$recipient'": 0.009, "'$changeaddress'": 0.0009 }''')
```
Siamo stati _molto_ attenti fare i nostri calcoli finanziari. Questi due UTXO contengono 0,00999999 BTC. Dopo aver inviato 0,009 BTC, ci rimarranno 0,00099999 BTC. Abbiamo scelto 0,00009999 BTC come commissione di transazione. Per accogliere tale commissione, impostiamo il resto su .0009 BTC. Se avessimo sbagliato i nostri calcoli e invece avessimo impostato la modifica su .00009 BTC, quei BTC aggiuntivi andrebbero dritti ai minatori! Se ci fossimo dimenticati di apportare il resto, tutto l'eccesso sarebbe scomparso. Quindi, ancora una volta, _fai attenzione_.

Fortunatamente, possiamo ricontrollare con l'alias `btctxfee` di JQ Interlude:

```
$ ./txfee-calc.sh $rawtxhex2
.00009999
```

### Finire la transazione

Ora puoi firmare, sigillare e consegnare la tua transazione, che sarà tua (e del faucet):

```
$ signedtx2=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex2 | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx2
e7071092dee0b2ae584bf6c1ee3c22164304e3a17feea7a32c22db5603cd6a0d
```

### Aspettare

Come al solito, i tuoi soldi continueranno a fluire per un po': il resto non sarà disponibile fino a quando la transazione non verrà effettivamente confermata e ti verrà dato un nuovo UTXO.

Ma, in 10 minuti o meno (probabilmente), avrai indietro i tuoi soldi rimanenti e sara di nuovo completamente spendibile. Per ora stiamo ancora aspettando:

```
$ bitcoin-cli listunspent
[
  {
    "txid": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
    "vout": 0,
    "address": "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
    "label": "",
    "scriptPubKey": "76a9142d573900aa357a38afd741fbf24b075d263ea6e088ac",
    "amount": 0.00022000,
    "confirmations": 15,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/3']0278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132)#nhjc3f8y",
    "safe": true
  }
]
```
E il resto prima o poi arriverà:
```
[
  {
    "txid": "e7071092dee0b2ae584bf6c1ee3c22164304e3a17feea7a32c22db5603cd6a0d",
    "vout": 1,
    "address": "mk9ry5VVy8mrA8SygxSQQUDNSSXyGFot6h",
    "scriptPubKey": "76a91432db726320e4ad170c9c1ee83cd4d8a243c3435988ac",
    "amount": 0.00090000,
    "confirmations": 1,
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
    "amount": 0.00022000,
    "confirmations": 16,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/3']0278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132)#nhjc3f8y",
    "safe": true
  }
]
```

Questo potrebbe anche essere un buon momento per controllare con un blockchain explorer, in modo da poter vedere in modo più intuitivo come sono disposti gli input, gli output e le commissioni di transazione:[e7071092dee0b2ae584bf6c1ee3c22164304e3a17feea7a32c22db5603cd6a0d](https://live.blockcypher.com/btc-testnet/tx/e7071092dee0b2ae584bf6c1ee3c22164304e3a17feea7a32c22db5603cd6a0d/).

## Riepilogo: invio di monete con transazioni grezze

Per inviare monete con transazioni grezze, è necessario creare una transazione grezza con uno o più input (per avere fondi sufficienti) e uno o più output (per recuperare il resto). Quindi, puoi seguire la normale procedura di utilizzo di `createrawtransaction` con argomenti con nome e JQ, come illustrato nelle sezioni precedenti.

> :fire: ***Qual è il potere di inviare monete con transazioni grezze?***

> _I vantaggi._ Ti dà il massimo controllo. Se il tuo obiettivo è scrivere uno script o un programma Bitcoin più intricato, probabilmente utilizzerai transazioni grezze in modo da sapere esattamente cosa sta succedendo. Questa è anche la situazione _più sicura_ per utilizzare transazioni grezze, perché puoi assicurarti a livello di programmazione di non commettere errori.

> _Gli svantaggi._ È facile perdere soldi. Non ci sono avvisi, protezioni e stop programmati a meno che tu non li scriva. È anche noioso. La formattazione è odiosa, anche utilizzando l'interfaccia `bitcoin-cli` facile da usare, e devi fare molte ricerche e calcoli a mano.

## Qual è il prossimo?

Vedere un altro modo alternativo per inserire comandi con [Curl](04_4_Intermezzo_Usare_Curl.md).

Oppure, preferisci saltare quella che è francamente una digressione, imparare un altro modo per "Inviare transazioni Bitcoin" con [Capitolo 4.5: Inviare Monete con Transazione Grezza Automatizzata](04_5_Inviare_Monete_con_Transazione_Grezza_Automatizzata.md).
