# 4.2 Creazione di una transazione grezza

Ora sei pronto per creare transazioni grezze (raw) su Bitcoin. Ciò ti consente di inviare denaro ma di elaborare le transazioni con la precisione che desideri. Questa prima sezione si concentra su una semplice transazione one-input, one-output. Questo tipo di transazione _non_ è in realtà così utile, perché raramente vorrai inviare tutto il tuo denaro a una persona (a meno che tu non lo stia semplicemente inoltrando, ad esempio se stai spostando denaro da un portafoglio a un altro). Pertanto, non etichettiamo questa sezione come un modo per inviare denaro. È solo un trampolino di lancio fondamentale per _inviare effettivamente_ denaro con una transazione grezza.

## Comprendere la transazione Bitcoin

Prima di immergerti nella creazione effettiva di transazioni grezze, dovresti assicurarti di comprendere come funziona una transazione Bitcoin. Riguarda tutto gli UTXO.

> :book: ***Cos'è un UTXO?*** Quando ricevi denaro nel tuo portafoglio Bitcoin, appare come una transazione individuale. Ognuna di queste transazioni è chiamata *Unspent Transaction Output* (UTXO). Non importa se vari pagamenti sono stati effettuati allo stesso indirizzo o a più indirizzi: ogni transazione in entrata rimane distinta nel tuo portafoglio come UTXO.

Quando crei una nuova transazione in uscita, raccogli uno o più UTXO, ognuno dei quali rappresenta una moneta che hai ricevuto. Li usi come input per una nuova transazione. Insieme, il loro importo deve essere uguale a quanto vuoi spendere _o più_. Quindi, generi uno o più output, che danno il denaro rappresentato dagli input a una o più persone. Questo crea nuovi UTXO per i destinatari, che possono quindi utilizzare _quelli_ per finanziare transazioni future.

Ecco il trucco: _tutti gli UTXO che raccogli vengono spesi per intero!_ Ciò significa che se vuoi inviare solo una parte del denaro in un UTXO a qualcun altro, allora devi anche generare un output aggiuntivo che ti rispedisca il resto! Per ora, non ci preoccuperemo di questo, ma l'uso di un indirizzo di cambio sarà fondamentale quando passeremo dalla teoria alla pratica.

## Elenca le tue transazioni non spese

Per creare una nuova transazione raw (grezza), devi sapere quali UTXO hai a disposizione da spendere. Puoi determinare questa informazione con il comando `bitcoin-cli listunspent`:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36",
    "vout": 0,
    "address": "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
    "label": "",
    "scriptPubKey": "76a9141b72503639a13f190bf79acf6d76255d772360b788ac",
    "amount": 0.00010000,
    "confirmations": 20,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/1']02fd5740996d853ea51a6904cf03257fc11204b0179f344c49739ec5b20b39c9ba)#62rud39c",
    "safe": true
  },
  {
    "txid": "61f3b7016bf1ecc3987b8805207e79362e4de8026682e149107999b779426e3a",
    "vout": 1,
    "address": "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
    "label": "",
    "scriptPubKey": "76a9142d573900aa357a38afd741fbf24b075d263ea6e088ac",
    "amount": 0.00050000,
    "confirmations": 3,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/3']0278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132)#nhjc3f8y",
    "safe": true
  },
  {
    "txid": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
    "vout": 0,
    "address": "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
    "label": "",
    "scriptPubKey": "76a9142d573900aa357a38afd741fbf24b075d263ea6e088ac",
    "amount": 0.00022000,
    "confirmations": 3,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/3']0278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132)#nhjc3f8y",
    "safe": true
  }
]

```

Questo elenco mostra tre UTXO diversi, del valore di 10000, 50000 e 22000 satoshi. Nota che ognuno ha il suo `txid` distinto e rimane distinto nel portafoglio, anche gli ultimi due, che sono stati inviati allo stesso indirizzo.

Quando vuoi spendere un UTXO, non è sufficiente conoscere solo l'ID della transazione. Questo perché ogni transazione può avere più output! Ricordi quella prima moneta che il faucet ci ha inviato? Nella transazione, parte del denaro è andata a noi e parte a qualcun altro. Il `txid` si riferisce alla transazione complessiva, mentre un `vout` indica quale dei più output hai ricevuto. In questo elenco, ciascuna di queste transazioni è il `vout` 0 di una transazione precedente, ma _non deve essere necessariamente così_.

Quindi, txid+vout=UTXO. Questa sarà la base di qualsiasi transazione raw.

## Scrivi una transazione raw con un output

Ora sei pronto a scrivere una semplice transazione raw di esempio che mostra come inviare l'intero UTXO a un'altra parte. Come notato, questo non è necessariamente un caso reale molto realistico.

> :warning: **WARNING:** È molto facile perdere soldi con una transazione raw. Considera tutte le istruzioni sull'invio di bitcoin tramite transazioni raw come _molto_, _molto_ pericolose. Ogni volta che invii effettivamente denaro reale ad altre persone, dovresti invece utilizzare uno degli altri metodi spiegati in questo capitolo. Creare transazioni raw è estremamente utile se stai scrivendo programmi bitcoin, ma _solo_ quando stai scrivendo programmi bitcoin. (Ad esempio: scrivendo questo esempio per una versione di questo tutorial, abbiamo speso accidentalmente la transazione sbagliata, anche se aveva circa 10 volte tanto valore. Quasi tutto è andato perso dai minatori.)

### Prepara la transazione grezza

Per le best practice, inizieremo ogni transazione registrando attentamente i txid e i vout che spenderemo.

In questo caso, spenderemo quella dal valore di 50000 satoshio perché è l'unica con un valore decente.
```
$ utxo_txid="61f3b7016bf1ecc3987b8805207e79362e4de8026682e149107999b779426e3a"
$ utxo_vout="1"
```

Dovresti registrare in modo simile l'indirizzo del destinatario, per assicurarti di averlo scritto correttamente. Stiamo di nuovo inviando del denaro al faucet:
```
$ recipient="n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi"
```
Come sempre, controlla attentamente le tue variabili per assicurarti che siano quelle che ti aspetti!

```
$ echo $utxo_txid
61f3b7016bf1ecc3987b8805207e79362e4de8026682e149107999b779426e3a
$ echo $utxo_vout
1
$ echo $recipient
n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi
```
Quel destinatario è particolarmente importante, perché se sbagli, i tuoi soldi sono _andati_! (E come abbiamo già visto, scegliere la transazione sbagliata può comportare la perdita di denaro!) Quindi controlla tutto tre volte.

### Comprendi la commissione di transazione, la fee

Ogni transazione ha una commissione associata. È _implicita_ quando invii una transazione raw: l'importo che pagherai come commissione è sempre uguale all'importo del tuo input meno l'importo del tuo output. Quindi, devi diminuire un po' il tuo output rispetto al tuo input per assicurarti che la tua transazione venga eseguita.

> :warning: **ATTENZIONE:** Questa è la parte molto pericolosa delle transazioni raw!! Poiché spendi automaticamente tutto l'importo negli UTXO che utilizzi, è di fondamentale importanza assicurarti di sapere: (1) esattamente quali UTXO stai utilizzando; (2) esattamente quanti soldi contengono; (3) esattamente quanti soldi stai inviando; e (4) qual è la differenza. Se sbagli e usi l'UTXO sbagliato (con più soldi di quanto pensavi) o se invii troppo poco denaro, l'eccesso è perso. Per sempre. Non commettere questo errore! Conosci i tuoi input e output _precisamente_. O meglio, non usare transazioni raw se non come parte di un programma attentamente ponderato e verificato tre volte.

> :book: ***Quanto dovresti spendere in fees?*** [Bitcoin Fees](https://bitcoinfees.net/) ha una bella valutazione in tempo reale. Dice che "la commissione di transazione più veloce ed economica è attualmente di 42 satoshi/byte" e che "per la dimensione media della transazione di 224 byte, ciò si traduce in una commissione di 9408 satoshi".

Attualmente Bitcoin Fees suggerisce una commissione di transazione di circa 10000 satoshi, (0,0001 BTC). Sì, è per la mainnet, non per la testnet, ma vogliamo testare le cose in modo realistico, quindi è quello che useremo.

In questo caso, significa prendere 50000 BTC nell'UTXO che siamo selezionati, ridurli di 10000 BTC per la fee e inviare i restanti 40000 BTC. (E questo è un esempio del perché i micropagamenti non funzionano sulla rete Bitcoin, perché una commissione di transazione di circa 1$ è piuttosto costosa quando si inviano 4$, figuriamoci se si stava cercando di effettuare un micropagamento di 0,50$. Ma è sempre per questo che abbiamo Lightning.)

> :warning: **ATTENZIONE:** Più bassa è la commissione di transazione impostata, più tempo ci vorrà prima che la transazione venga incorporata in un blocco. Il sito [Bitcoin Fees](https://bitcoinfees.net/) elenca i tempi previsti, da circa 0 blocchi a 22 blocchi. Poiché i blocchi vengono creati in media ogni 10 minuti, questa è la differenza tra pochi minuti e poche ore! Quindi, scegli una commissione di transazione appropriata per ciò che stai inviando. Nota che non dovresti mai scendere sotto la commissione di inoltro minima, che è 10000 BTC.

### Scrivi la transazione raw

Ora sei pronto per creare la transazione raw. Per questo utilizza il comando `createrawtransaction`, che potrebbe sembrare un po' intimidatorio. Questo perché il comando `createrawtransaction` non ti protegge completamente dal `JSON RPC` che usa `bitcoin-cli`. Invece, inserirai un array `JSON` per elencare gli `UTXO` che stai spendendo e un oggetto `JSON` per elencare gli output.

Ecco il formato standard:
```
$ bitcoin-cli createrawtransaction
'''[
     {
       "txid": "'$your_txid'",
       "vout": '$your_vout'
      }
]'''
'''{
   "'$your_recipient'": bitcoin_amount
 }'''
 ```
Eh sì, ci sono tutti i tipi di virgolette folli lì, ma fidati che faranno la cosa giusta. Usa `'''` per contrassegnare l'inizio e la fine dell'array JSON e dell'oggetto JSON. Proteggi parole normali come `"this"`, ma non devi proteggere numeri normali: `0`. Se sono variabili, inserisci virgolette singole, come `"'$this_word'"` e `'$this_num'`. (Uffa. Ti ci abituerai.)

Ecco un comando che crea una transazione raw per inviare il tuo $utxo al tuo $recipient
 ```
$ rawtxhex=$(bitcoin-cli createrawtransaction '''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' '''{ "'$recipient'": 0.0004 }''')
$ echo $rawtxhex
02000000013a6e4279b799791049e1826602e84d2e36797e2005887b98c3ecf16b01b7f3610100000000ffffffff01409c0000000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000
```

### Verifica la tua transazione raw

Dovresti quindi verificare la tua transazione raw con `decoderawtransaction` per assicurarti che faccia la cosa giusta.
```
$ bitcoin-cli decoderawtransaction $rawtxhex
{
  "txid": "dcd2d8f0ec5581b806a1fbe00325e1680c4da67033761b478a26895380cc1298",
  "hash": "dcd2d8f0ec5581b806a1fbe00325e1680c4da67033761b478a26895380cc1298",
  "version": 2,
  "size": 85,
  "vsize": 85,
  "weight": 340,
  "locktime": 0,
  "vin": [
    {
      "txid": "61f3b7016bf1ecc3987b8805207e79362e4de8026682e149107999b779426e3a",
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
      "value": 0.00040000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 e7c1345fc8f87c68170b3aa798a956c2fe6a9eff OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi"
        ]
      }
    }
  ]
}
```

Controlla il `vin`. Stai spendendo la transazione giusta? Contiene l'importo di denaro previsto? (Controlla con `bitcoin-cli gettransaction` e assicurati di guardare il `vout` giusto.) Controlla il tuo `vout`. Stai inviando l'importo giusto? Va all'indirizzo giusto? Infine, fai i calcoli per assicurarti che il denaro sia in pareggio. Il valore dell'UTXO meno l'importo speso è uguale alla commissione di transazione prevista?

> :information_source: **NOTA - SEQUENZA:** Potresti notare che ogni input ha un numero di sequenza, impostato qui su 4294967295, che è 0xFFFFFFFF. Questa è l'ultima frontiera delle transazioni Bitcoin, perché è un campo standard nelle transazioni che era originariamente destinato a uno scopo specifico, ma non è mai stato completamente implementato. Quindi ora c'è questo intero in giro nelle transazioni che potrebbe essere riutilizzato per altri usi. E, in effetti, lo è stato. Al momento in cui scrivo, ci sono tre diversi utilizzi per la variabile chiamata `nSequence` nel codice Bitcoin Core: abilita RBF, `nLockTime` e timelock relativi. Se non succede nulla di strano, `nSequence` verrà impostato su 4294967295. Impostandolo su un valore inferiore, si segnala che sta succedendo qualcosa di speciale.

### Firma la transazione raw

A oggi, la tua transazione raw è solo qualcosa di teorico: potresti inviarla, ma non ti è stato promesso nulla. Devi fare alcune cose per farla uscire sulla rete.

Per prima cosa, devi firmare la tua transazione raw:

```

$ bitcoin-cli signrawtransactionwithwallet $rawtxhex
{
  "hex": "02000000013a6e4279b799791049e1826602e84d2e36797e2005887b98c3ecf16b01b7f361010000006a4730440220335d15a2a2ca3ce6a302ce041686739d4a38eb0599a5ea08305de71965268d05022015f77a33cf7d613015b2aba5beb03088033625505ad5d4d0624defdbea22262b01210278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132ffffffff01409c0000000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000",
  "complete": true
}
$ signedtx="02000000013a6e4279b799791049e1826602e84d2e36797e2005887b98c3ecf16b01b7f361010000006a4730440220335d15a2a2ca3ce6a302ce041686739d4a38eb0599a5ea08305de71965268d05022015f77a33cf7d613015b2aba5beb03088033625505ad5d4d0624defdbea22262b01210278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132ffffffff01409c0000000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000"
```
Nota che abbiamo catturato manualmente l'esadecimale firmato, anziché cercare di analizzarlo dall'oggetto JSON. Un pacchetto software chiamato "JQ" potrebbe fare di meglio, come spiegheremo in un prossimo interludio.

### Invia la transazione raw

Ora hai una transazione raw pronta all'uso, ma non conta finché non la metti effettivamente in rete, cosa che fai con il comando `sendrawtransaction`. Riceverai un txid:

```
$ bitcoin-cli sendrawtransaction $signedtx
a1fd550d1de727eccde6108c90d4ffec11ed83691e96e119d842b3f390e2f19a
```

Vedrai immediatamente che l'UTXO e il suo denaro sono stati rimossi dal tuo portafoglio:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36",
    "vout": 0,
    "address": "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
    "label": "",
    "scriptPubKey": "76a9141b72503639a13f190bf79acf6d76255d772360b788ac",
    "amount": 0.00010000,
    "confirmations": 23,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/1']02fd5740996d853ea51a6904cf03257fc11204b0179f344c49739ec5b20b39c9ba)#62rud39c",
    "safe": true
  },
  {
    "txid": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
    "vout": 0,
    "address": "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
    "label": "",
    "scriptPubKey": "76a9142d573900aa357a38afd741fbf24b075d263ea6e088ac",
    "amount": 0.00022000,
    "confirmations": 6,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/3']0278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132)#nhjc3f8y",
    "safe": true
  }
]

$ bitcoin-cli getbalance
0.00032000
```

Presto `listtransactions` dovrebbe mostrare una transazione confermata della categoria `send`.
```
 {
    "address": "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi",
    "category": "send",
    "amount": -0.00040000,
    "vout": 0,
    "fee": -0.00010000,
    "confirmations": 1,
    "trusted": true,
    "txid": "a1fd550d1de727eccde6108c90d4ffec11ed83691e96e119d842b3f390e2f19a",
    "walletconflicts": [
    ],
    "time": 1592608574,
    "timereceived": 1592608574,
    "bip125-replaceable": "no",
    "abandoned": false
  }
```
Puoi vedere che corrisponde all'indirizzo `txid` e `recipient`. Non solo mostra l'`importo` inviato, ma mostra anche la `commissione` della transazione. E ha già ricevuto una conferma, perché abbiamo offerto una commissione che lo avrebbe rapidamente inserito in un blocco.

Congratulazioni! Ora sei più povero di qualche satoshi!

## Riepilogo: creazione di una transazione raw

Quando le monete entrano nel tuo portafoglio Bitcoin, rimangono come importi distinti, chiamati UTXO. Quando crei una transazione raw per inviare quel denaro, utilizzi uno o più UTXO per pagare (come fondere le monete d'oro e ricomporle a seconda del bisogno). Puoi quindi creare una transazione raw, firmarla e inviarla sulla rete Bitcoin. Tuttavia, questa è solo una base: di solito dovrai creare una transazione raw con più output per inviare effettivamente qualcosa sulla rete Bitcoin!

## Cosa c'è dopo?

Ne vale la pena impiegare qualche minuto per capire meglio come [usare JQ](04_2_Intermezzo_Usare_JQ.md).
