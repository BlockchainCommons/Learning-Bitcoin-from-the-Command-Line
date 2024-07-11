# Interludio: utilizzo di JQ

La creazione di una transazione raw ha rivelato come i risultati più complessi di bitcoin-cli non possano essere facilmente salvati in variabili della riga di comando. La risposta è JQ, che consente di filtrare singoli elementi da dati JSON più complessi.

## Installa JQ

Per le versioni moderne di Debian, dovresti essere in grado di installare JQ usando `apt-get`:

```
# apt-get install jq
```
> :book: ***Che cos'è JQ?*** cito "`jq` è come `sed` per i dati `JSON`: puoi usarlo per suddividere, filtrare, mappare e trasformare i dati strutturati e giocare con il testo con la stessa facilità di `sed` , `awk`, `grep` e compagnia bella."

Se funziona, il gioco è fatto!

Altrimenti, puoi scaricare JQ da un [repository Github](https://stedolan.github.io/jq/). Basta scaricare un file binario per Linux, OS X o Windows, a seconda dei casi.

Una volta scaricato il binario, puoi installarlo sul tuo sistema. Se stai lavorando su un VPS Debian come suggeriamo, la tua installazione sarà simile a questa:
```
$ mv jq-linux64 jq
$ sudo /usr/bin/install -m 0755 -o root -g root -t /usr/local/bin jq
```
## Usa `JQ` per accedere al valore di un oggetto `JSON` tramite chiave

**Esempio di utilizzo:** _Cattura l'esadecimale da una transazione grezza firmata._

Nella sezione precedente, l'uso di "signrawtransaction" ha offerto un esempio di impossibilità di acquisire facilmente i dati in variabili a causa dell'uso dell'output JSON:
```
$ bitcoin-cli signrawtransactionwithwallet $rawtxhex
{
  "hex": "02000000013a6e4279b799791049e1826602e84d2e36797e2005887b98c3ecf16b01b7f361010000006a4730440220335d15a2a2ca3ce6a302ce041686739d4a38eb0599a5ea08305de71965268d05022015f77a33cf7d613015b2aba5beb03088033625505ad5d4d0624defdbea22262b01210278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132ffffffff01409c0000000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000",
  "complete": true
}
```
Fortunately, JQ can easily capture data of that sort!

Per utilizzare JQ, esegui `jq` nel backend di una pipe `|` e utilizza sempre l'invocazione standard di `jq -r`. La `-r` dice a JQ di produrre un output grezzo, raw, che funzionerà per le variabili della riga di comando, mentre il `.` dice a jq di produrre l'output. Proteggiamo questo argomento in `' '` perché avremo bisogno di quella protezione in seguito quando le nostre chiamate a `'jq` diventeranno più complesse.

Per acquisire un valore specifico da un oggetto JSON, elenca semplicemente la chiave dopo `.`:

```
$ bitcoin-cli signrawtransactionwithwallet $rawtxhex | jq -r '.hex'
02000000013a6e4279b799791049e1826602e84d2e36797e2005887b98c3ecf16b01b7f361010000006a4730440220335d15a2a2ca3ce6a302ce041686739d4a38eb0599a5ea08305de71965268d05022015f77a33cf7d613015b2aba5beb03088033625505ad5d4d0624defdbea22262b01210278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132ffffffff01409c0000000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000
```
Con questo strumento in mano, puoi acquisire informazioni dagli oggetti JSON alle variabili della riga di comando:
```
$ signedtx=$(bitcoin-cli signrawtransactionwithwallet $rawtxhex | jq -r '.hex')
$ echo $signedtx
02000000013a6e4279b799791049e1826602e84d2e36797e2005887b98c3ecf16b01b7f361010000006a4730440220335d15a2a2ca3ce6a302ce041686739d4a38eb0599a5ea08305de71965268d05022015f77a33cf7d613015b2aba5beb03088033625505ad5d4d0624defdbea22262b01210278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132ffffffff01409c0000000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000
```
È quindi possibile utilizzare tali variabili facilmente e senza errori:
```
$ bitcoin-cli sendrawtransaction $signedtx
3f9ccb6e16663e66dc119de1866610cc4f7a83079bfec2abf0598ed3adf10a78
```
## Utilizza JQ per accedere ai valori di singoli oggetti JSON in un array tramite chiave

**Esempio di utilizzo:** _Cattura txid e vout per un UTXO selezionato._

Ottenere dati da un oggetto JSON è semplice, ma cosa succede se l'oggetto JSON si trova in un array JSON? Il comando `listunspent` offre un ottimo esempio, perché solitamente conterrà un numero di transazioni diverse. E se volessi acquisire informazioni specifiche da _uno_ di essi?

Quando lavori con un array JSON, la prima cosa che devi fare è dire a JQ a quale indice accedere. Ad esempio, potresti aver esaminato le tue transazioni in "listunspent" e aver deciso di voler lavorare con la seconda. Si usa `'.[1]'` per accedere al secondo elemento. `[ ]` dice che stiamo facendo riferimento a un array JSON e `1` dice che vogliamo il primo indice.
```
$ bitcoin-cli listunspent | jq -r '.[1]'
{
  "txid": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
  "vout": 0,
  "address": "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
  "label": "",
  "scriptPubKey": "76a9142d573900aa357a38afd741fbf24b075d263ea6e088ac",
  "amount": 0.00022,
  "confirmations": 9,
  "spendable": true,
  "solvable": true,
  "desc": "pkh([d6043800/0'/0'/3']0278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132)#nhjc3f8y",
  "safe": true
}
```
È quindi possibile acquisire un singolo valore dall'array selezionato (1)] utilizzando una pipe _all'interno_ degli argomenti JQ; e poi (2) richiedere successivamente il valore specifico, come nell'esempio precedente. Quanto segue catturerebbe il `txid` dal primo oggetto JSON nell'array JSON prodotto da `listunspent`:
```
$ bitcoin-cli listunspent | jq -r '.[1] | .txid'
91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c
```
Notare attentamente come  le virgolette ` ' ' ` stanno dappertutto nelle espressione JQ _inclusa_ la pipe `|` .

Questo metodo può essere utilizzato per compilare le variabili per un UTXO che desideri utilizzare:
```
$ newtxid=$(bitcoin-cli listunspent | jq -r '.[1] | .txid')
$ newvout=$(bitcoin-cli listunspent | jq -r '.[1] | .vout')
$ echo $newtxid
91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c
$ echo $newvout
0
```
Ecco! Ora potremmo creare una nuova transazione grezza utilizzando il nostro primo UTXO come input, senza dover digitare manualmente nessuna delle informazioni UTXO!

## Utilizza JQ per accedere ai valori degli oggetti JSON corrispondenti in un array in base alla chiave

**Esempio di utilizzo:** _Elenca il valore di tutti gli UTXO non spesi._

Invece di accedere a un singolo valore specifico in un oggetto JSON specifico, potresti invece accedere a tutto un valore specifico in tutti gli oggetti JSON. Questo viene fatto con `.[]`, dove non è specificato alcun indice. Ad esempio, questo elencherebbe tutti i fondi non spesi:
```
$ bitcoin-cli listunspent | jq -r '.[] | .amount'
0.0001
0.00022
```

## Usa JQ per fare calcoli semplici tramite chiave

**Esempio di utilizzo:** _Somma il valore di tutti gli UTXO non spesi._

A questo punto, puoi iniziare a utilizzare l'output JQ per semplici calcoli. Ad esempio, sommando i valori delle transazioni non spese con un semplice script `awk` otterresti l'equivalente di `getbalance`:
```
$ bitcoin-cli listunspent | jq -r '.[] | .amount' | awk '{s+=$1} END {print s}'
0.00032
$ bitcoin-cli getbalance
0.00032000
```

## Utilizza JQ per visualizzare più valori di oggetti JSON in un array tramite più chiavi

**Esempio di utilizzo:** _Elenca le informazioni sull'utilizzo per tutti gli UTXO._

JQ può facilmente acquisire singoli elementi da oggetti e array JSON e inserirli in variabili. Questo sarà il suo utilizzo principale nelle sezioni future. Tuttavia, può anche essere utilizzato per ridurre "enormi quantità" di informazioni prodotte da "bitcoin-cli" in "quantità ragionevoli" di informazioni.

Ad esempio, potresti voler vedere un elenco di tutti i tuoi UTXO (`.[]`) e ottenere un elenco di tutte le loro informazioni più importanti (`.txid, .vout, .amount`):
```
$ bitcoin-cli listunspent | jq -r '.[] | .txid, .vout, .amount'
ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36
0
0.0001
91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c
0
0.00022
```
Ciò rende facile decidere quali UTXO spendere in una transazione grezza, ma non è molto carino.

Fortunatamente, JQ ti permette anche di essere fantasioso. Puoi utilizzare `{}` per creare nuovi oggetti JSON (per analisi aggiuntive o per un output gradevole). Puoi anche definire il nome della nuova chiave per ciascuno dei tuoi valori. L'output risultante dovrebbe essere molto più intuitivo e meno soggetto a errori (anche se ovviamente meno utile per scaricare informazioni direttamente nelle variabili).

L'esempio seguente mostra esattamente la stessa analisi di "listunspent", ma con ogni vecchio oggetto JSON ricostruito come un nuovo oggetto JSON abbreviato, con tutti i nuovi valori denominati con le loro vecchie chiavi:
```
$ bitcoin-cli listunspent | jq -r '.[] | { txid: .txid, vout: .vout, amount: .amount }'
{
  "txid": "ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36",
  "vout": 0,
  "amount": 0.0001
}
{
  "txid": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
  "vout": 0,
  "amount": 0.00022
}
```
Ovviamente potresti rinominare le tue nuove chiavi come ritieni opportuno. Non c'è niente di magico nei nomi originali scelti da Satoshi:
```
$ bitcoin-cli listunspent | jq -r '.[] | { tx: .txid, output: .vout, bitcoins: .amount }'
{
  "tx": "ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36",
  "output": 0,
  "bitcoins": 0.0001
}
{
  "tx": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
  "output": 0,
  "bitcoins": 0.00022
}
```
## Utilizza JQ per accedere agli oggetti JSON in base al valore cercato

**Esempio di utilizzo:** _Cerca automaticamente gli UTXO utilizzati in una transazione._

Le ricerche JQ finora sono state abbastanza semplici: si utilizza una chiave per cercare uno o più valori in un oggetto o array JSON. Ma cosa succede se invece desideri cercare un valore in un oggetto JSON... tramite un altro valore? Questo tipo di ricerca indiretta ha una reale applicabilità quando lavori con transazioni basate su UTXO esistenti. Ad esempio, può consentire di calcolare il valore totale degli UTXO utilizzati in una transazione, cosa di vitale importanza.

Questo esempio utilizza la seguente transazione raw. Si noti che questa è una transazione grezza più complessa con due input e due output. Impareremo a realizzarli in alcune sezioni; per ora è necessario saper offrire esempi robusti. Nota che a differenza dei nostri esempi precedenti, questo ha due oggetti nell'array "vin" e due nell'array "vout".
```
$ bitcoin-cli decoderawtransaction $rawtxhex
{
  "txid": "6f83a0b78c598de01915554688592da1d7a3047eacacc8a9be39f5396bf0a07e",
  "hash": "6f83a0b78c598de01915554688592da1d7a3047eacacc8a9be39f5396bf0a07e",
  "size": 160,
  "vsize": 160,
  "version": 2,
  "locktime": 0,
  "vin": [
    {
      "txid": "d261b9494eb29084f668e1abd75d331fc2d6525dd206b2f5236753b5448ca12c",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967295
    },
    {
      "txid": "c7c7f6371ec19330527325908a544bbf8401191645598301d24b54d37e209e7b",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 1.00000000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 cfc39be7ea3337c450a0c77a839ad0e160739058 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914cfc39be7ea3337c450a0c77a839ad0e16073905888ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mzTWVv2QSgBNqXx7RC56zEhaQPve8C8VS9"
        ]
      }
    },
    {
      "value": 0.04500000,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 166692bda9f25ced145267bb44286e8ee3963d26 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914166692bda9f25ced145267bb44286e8ee3963d2688ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mhZQ3Bih6wi7jP1tpFZrCcyr4NsfCapiZP"
        ]
      }
    }
  ]
}
```

### Recupera i valori

Supponiamo di sapere esattamente come è costruita questa transazione: sappiamo che utilizza due UTXO come input. Per recuperare il txid per i due UTXO, potremmo usare `jq` per cercare il valore .vin della transazione, quindi fare riferimento all'array 0 di .vin, quindi al valore .txid di quell'array. Successivamente, potremmo fare lo stesso con il primo array, quindi lo stesso con i due valori .vout di .vin. Facile:

```
$ usedtxid1=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[0] | .txid')
$ echo $usedtxid1
d261b9494eb29084f668e1abd75d331fc2d6525dd206b2f5236753b5448ca12c
$ usedtxid2=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[1] | .txid')
$ echo $usedtxid2
c7c7f6371ec19330527325908a544bbf8401191645598301d24b54d37e209e7b

$ usedvout1=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[0] | .vout')
$ echo $usedvout1
1
$ usedvout2=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[1] | .vout')
$ echo $usedvout2
1
```
Tuttavia, sarebbe meglio avere un caso generale che salvi _automaticamente_ tutti i txid dei nostri UTXO.

Sappiamo già che possiamo accedere a tutti i file `.txid` utilizzando un valore di array `.[]`. Possiamo usarlo per creare una ricerca generale .txid:
```
$ usedtxid=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .txid'))
$ echo ${usedtxid[0]}
d261b9494eb29084f668e1abd75d331fc2d6525dd206b2f5236753b5448ca12c
$ echo ${usedtxid[1]}
c7c7f6371ec19330527325908a544bbf8401191645598301d24b54d37e209e7b

$ usedvout=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .vout'))
$ echo ${usedvout[0]}
1
$ echo ${usedvout[1]}
1
```
L'unico vero trucco qui è il modo in cui abbiamo salvato le informazioni utilizzando la shell bash. Invece di salvare in una variabile con `$(command)`, abbiamo invece salvato in un array con `($(command))`. Siamo quindi riusciti ad accedere ai singoli elementi dell'array bash con una costruzione `${variable[n]}`. Potremmo invece accedere all'intero array con `${variable[@]}`. (Sì, nessuno ha mai detto che Bash fosse carino.)

> :avviso: **ATTENZIONE:** Ricorda sempre che un UTXO è una transazione _più_ un vout. Abbiamo mancato il vout la prima volta che abbiamo scritto questo esempio JQ e ha smesso di funzionare quando ci siamo ritrovati in una situazione in cui ci erano stati inviati due `vout` dalla stessa transazione.

### Recupera gli oggetti correlati

Ora puoi utilizzare le informazioni "txid" e "vout" salvate per fare riferimento agli UTXO in "listunspent". Per trovare le informazioni sugli UTXO utilizzati dalla transazione grezza, è necessario esaminare l'intero array JSON (`[]`) delle transazioni non spese. Puoi quindi scegliere (`select`) singoli oggetti JSON che includono (`contains`) i txid. Si _poi_ si selezionano (`select`) le transazioni tra quelle che _anche_ contengono (`contain`) il vout corretto.

L'uso di un altro livello di pipe è la metodologia standard di JQ: prendi una serie di dati, poi li riduci a tutte le transazioni rilevanti, quindi li riduci ai vout che sono stati effettivamente utilizzati da quelle transazioni. Tuttavia, gli argomenti `select` e `contains` sono qualcosa di nuovo. Mostrano parte della complessità di JSON che va oltre lo scopo di questo tutorial; per ora sappi solo che questa particolare invocazione funzionerà per catturare oggetti corrispondenti.

Per iniziare in modo semplice, seleziona i due UTXO uno alla volta:
```
$ bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${usedtxid[0]}'")) | select(.vout | contains('${usedvout[0]}'))'
{
  "txid": "d261b9494eb29084f668e1abd75d331fc2d6525dd206b2f5236753b5448ca12c",
  "vout": 1,
  "address": "miSrC3FvkPPZgqqvCiQycq7io7wTSVsAFH",
  "scriptPubKey": "76a91420219e4f3c6bc0f6524d538009e980091b3613e888ac",
  "amount": 0.9,
  "confirmations": 6,
  "spendable": true,
  "solvable": true
}
$ bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${usedtxid[1]}'")) | select(.vout | contains('${usedvout[1]}'))'
{
  "txid": "c7c7f6371ec19330527325908a544bbf8401191645598301d24b54d37e209e7b",
  "vout": 1,
  "address": "mzizSuAy8aL1ytFijds7pm4MuDPx5aYH5Q",
  "scriptPubKey": "76a914d2b12da30320e81f2dfa416c5d9499d08f778f9888ac",
  "amount": 0.4,
  "confirmations": 5,
  "spendable": true,
  "solvable": true
}
```
Un semplice bash for-loop potrebbe invece darti _tutti_ i tuoi UTXO:
```
$ for ((i=0; i<${#usedtxid[*]}; i++)); do txid=${usedtxid[i]}; vout=${usedvout[i]}; bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${txid}'")) | select(.vout | contains('$vout'))'; done;
{
  "txid": "d261b9494eb29084f668e1abd75d331fc2d6525dd206b2f5236753b5448ca12c",
  "vout": 1,
  "address": "miSrC3FvkPPZgqqvCiQycq7io7wTSVsAFH",
  "scriptPubKey": "76a91420219e4f3c6bc0f6524d538009e980091b3613e888ac",
  "amount": 0.9,
  "confirmations": 7,
  "spendable": true,
  "solvable": true
}
{
  "txid": "c7c7f6371ec19330527325908a544bbf8401191645598301d24b54d37e209e7b",
  "vout": 1,
  "address": "mzizSuAy8aL1ytFijds7pm4MuDPx5aYH5Q",
  "scriptPubKey": "76a914d2b12da30320e81f2dfa416c5d9499d08f778f9888ac",
  "amount": 0.4,
  "confirmations": 6,
  "spendable": true,
  "solvable": true
}

```
Nota che abbiamo utilizzato ancora un altro elemento brutto dell'array `${#usedtxid[*]}` per determinare la dimensione dell'array, quindi abbiamo effettuato l'accesso a ciascun valore nell'array `usedtxid` e a ciascun valore nell'array parallelo `usedvout`, inserendo trasformarli in variabili più semplici per un accesso meno brutto.

## Usa JSON per calcoli semplici in base al valore

**Esempio di utilizzo:** _Calcola automaticamente il valore degli UTXO utilizzati in una transazione._

Ora puoi fare un ulteriore passo avanti e richiedere il .amount (o qualsiasi altro valore-chiave JSON) dagli UTXO che stai recuperando.

Questo esempio ripete l'utilizzo degli array `$usedtxid` e `$usedvout` impostati come segue:
```
$ usedtxid=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .txid'))
$ usedvout=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .vout'))
```
Lo stesso script "for" può essere utilizzato per scorrere questi array, ma con una pipe aggiunta nel JQ che restituisce il valore `amount` per ciascuno degli UTXO selezionati.
```
$ for ((i=0; i<${#usedtxid[*]}; i++)); do txid=${usedtxid[i]}; vout=${usedvout[i]}; bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${txid}'")) | select(.vout | contains('$vout')) | .amount'; done;
0.9
0.4
```
A questo punto puoi anche sommare gli .amounts con uno script `awk`, per vedere realmente quanti soldi ci sono negli UTXO che la transazione sta spendendo:
```
$ for ((i=0; i<${#usedtxid[*]}; i++)); do txid=${usedtxid[i]}; vout=${usedvout[i]}; bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${txid}'")) | select(.vout | contains('$vout')) | .amount'; done | awk '{s+=$1} END {print s}'
1.3
```
Wow!

## Usa JQ per calcoli complessi

**Esempio di utilizzo:** _Calcola la commissione per una transazione._

Per calcolare la commissione di transazione completa a questo punto è necessario solo un ulteriore calcolo: determinare quanti soldi passano attraverso il `.vout`. Questo è un semplice utilizzo di JQ in cui devi semplicemente usare `awk` per riassumere il `valore` di tutte le informazioni `vout`:
```
$ bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vout  [] | .value' | awk '{s+=$1} END {print s}'
1.045
```
Per completare il calcolo della commissione di transazione, sottrai l'importo .vout .(1.045) dall'importo .vin (1.3).

Per fare ciò, dovrai installare `bc`:

```
$ sudo apt-get install bc
```

Mettendo tutto insieme si crea una calcolatrice completa in sole cinque righe di script:
```
$ usedtxid=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .txid'))
$ usedvout=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .vout'))
$ btcin=$(for ((i=0; i<${#usedtxid[*]}; i++)); do txid=${usedtxid[i]}; vout=${usedvout[i]}; bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${txid}'")) | select(.vout | contains('$vout')) | .amount'; done | awk '{s+=$1} END {print s}')
$ btcout=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vout  [] | .value' | awk '{s+=$1} END {print s}')
$ echo "$btcin-$btcout"| /usr/bin/bc
.255
```
E questo è anche un buon esempio del motivo per cui ricontrolli le tue commissioni: avevamo intenzione di inviare una commissione di transazione di 5.000 satoshi, ma invece abbiamo inviato 255.000 satoshi. Ops!

> :avviso: **ATTENZIONE:** La prima volta che abbiamo scritto questa lezione, abbiamo effettivamente calcolato male la nostra tariffa e non l'abbiamo vista finché non abbiamo eseguito il nostro calcolatore delle tariffe. È *così* facile sbagliare, poi i tuoi soldi sono finiti. (L'esempio sopra è in realtà tratto dalla nostra seconda iterazione della calcolatrice e quella volta abbiamo commesso l'errore di proposito.)

Per ulteriori magie JSON (e se qualcosa non è chiaro), leggi il [Manuale JSON](https://stedolan.github.io/jq/manual/) e il [Cookbook JSON](https:// github.com/stedolan/jq/wiki/Cookbook). Utilizzeremo regolarmente JQ negli esempi futuri.

## Crea alcuni nuovi alias

Il codice JQ può essere un po' ingombrante, quindi dovresti considerare di aggiungere alcune invocazioni più lunghe e interessanti al tuo ~/.bash_profile.

Ogni volta che stai esaminando una grande massa di informazioni in un oggetto JSON generato da un comando `bitcoin-cli`, valuta la possibilità di scrivere un alias per ridurlo a ciò che vuoi vedere.
```
alias btcunspent="bitcoin-cli listunspent | jq -r '.[] | { txid: .txid, vout: .vout, amount: .amount }'"
```

## Esegui lo script della commissione di transazione

Lo [Script di calcolo della tariffa](src/04_2_i_txfee-calc.sh) è disponibile nella directory src-code. Puoi scaricarlo e salvarlo come `txfee-calc.sh`.

> :avviso: **ATTENZIONE:** Questo script non è stato controllato in modo approfondito. Se hai intenzione di usarlo per verificare le commissioni di transazione reali, dovresti farlo solo come triplo controllo dopo aver già fatto tutti i conti da solo.

Assicurati che le autorizzazioni sullo script siano corrette:
```
$ chmod 755 txfee-calc.sh
```
È quindi possibile eseguire lo script come segue:
```
$ ./txfee-calc.sh $rawtxhex
.255
```
Potresti anche voler creare un alias:
```
alias btctxfee="~/txfee-calc.sh"
```

## Riepilogo: utilizzo di JQ

JQ semplifica l'estrazione di informazioni da array e oggetti JSON. Può anche essere utilizzato negli script di shell per calcoli abbastanza complessi che ti semplificheranno la vita.

## Cosa viene dopo?

Continua a inviare Bitcoin" qui [Capitolo 4.3: Creare una Transazione Grezza con Alias](04_3_Creare_una_Transazione_Grezza_con_Alias.md).

