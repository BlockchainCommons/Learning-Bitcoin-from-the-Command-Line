# 8.1: Invio di una transazione con un Locktime

I capitoli precedenti hanno mostrato due modi diversi per inviare fondi da più macchine e a più destinatari. Ma ci sono altri due modi per modificare radicalmente le transazioni di base. Il primo di questi è variare il tempo scegliendo un tempo di attessa, _locktime_. Ciò ti dà la possibilità di inviare transazioni grezze in un momento futuro.

## Comprendere come funziona Locktime

Quando crei una transazione locktime, la fermi con un numero che rappresenta l'altezza del blocco ( (se è un numero piccolo) o un timestamp UNIX (se è un numero grande). Questo dice alla rete Bitcoin che la transazione non può essere inserita in un blocco finché non è arrivato il momento specificato o finché la blockchain non ha raggiunto l'altezza specificata.

> :book: **_Che cos'è l'altezza del blocco?_** `block height` è il conteggio totale dei blocchi nella catena, risalendo al blocco numero 1 cioè il blocco genesi di Bitcoin.

Quando una transazione locktime è in attesa di entrare in un blocco, può essere annullata. Ciò significa che è tutt’altro che finalizzato. In effetti, lo scopo principale di una transazione locktime è avere la possibilità di annullarla.

> :book: **_Cos'è nLockTime?_** È la stessa cosa di locktime. Più specificamente, è ciò che viene chiamato locktime interno al codice sorgente di Bitcoin Core.

> :book: **_Che cos'è Timelock?_** Locktime è solo un modo per trattenere le transazioni Bitcoin fino a un certo punto nel futuro; collettivamente questi metodi sono chiamati timelock. Locktime è il metodo di blocco temporale più semplice. Blocca un'intera transazione con un tempo assoluto ed è disponibile tramite `bitcoin-cli` (motivo per cui è l'unico blocco temporale trattato in questa sezione). Un metodo parallelo, che blocca una transazione con un tempo relativo, è definito nel [BIP 68](https://github.com/bitcoin/bips/blob/master/bip-0068.mediawiki) e trattato nel [Capitolo 11.3: Utilizzo di CSV negli script](11_3_Using_CSV_in_Scripts.md).

> Bitcoin Script potenzia ulteriormente entrambi i tipi di blocchi temporali, consentendo il blocco di singoli output anziché di intere transazioni. I timelock assoluti (come Locktime) sono collegati al codice operativo dello script OP_CHECKLOCKTIMEVERIFY, che è definito in [BIP 65](https://github.com/bitcoin/bips/blob/master/bip-0065.mediawiki) e trattato in [ §11.2: Using CLTV in Scripts](11_2_Usare_CLTV_negli_Scripts.md), mentre i timelock relativi (come Timelock) sono collegati al codice operativo dello script `OP_CHECKSEQUENCEVERIFY`, che è definito nel [BIP 112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki) e trattato anche nel [Capitolo11.3](11_3_Usare_CSV_negli_Scripts.md).

## Creare una transazione Locktime

Per creare una transazione `locktime`, devi prima determinare su cosa imposterai il locktime.

### Scopri il tuo orario di blocco tramite timestamp UNIX

Molto spesso imposterai il locktime su un timestamp UNIX che rappresenta una data e un'ora specifiche. È possibile calcolare un timestamp UNIX in un sito Web come [UNIX Time Stamp](http://www.unixtimestamp.com/) o [Epoch Converter](https://www.epochconverter.com/). Tuttavia, sarebbe meglio [scrivere il proprio script](https://www.epochconverter.com/#code) sul computer locale, in modo da sapere che il timestamp UNIX ricevuto è accurato. Se non lo fai, almeno ricontrolla su due siti diversi.

> :book: **_Perché dovrei utilizzare un timestamp UNIX?_** L'uso di un timestamp UNIX rende semplice collegare definitivamente una transazione a un'ora specifica, senza preoccuparsi se la velocità di creazione del blocco potrebbe cambiare ad un certo punto. Soprattutto se stai creando un locktime lontano nel futuro, è la cosa più sicura da fare. Ma, oltre a ciò, è semplicemente più intuitivo, poiché crea una correlazione diretta tra una data di calendario e l'ora in cui la transazione può essere estratta.

> :avviso: **ATTENZIONE:** Il locktime con i timestamp UNIX ha un po' di margine di manovra: il rilascio dei blocchi non è regolare e gli orari dei blocchi possono essere due ore avanti rispetto al tempo reale, quindi un locktime in realtà significa "entro pochi ore di questo tempo, più o meno".

### Determina il tempo di chiusura in base all'altezza del blocco

In alternativa, puoi impostare il tempo di blocco su un numero più piccolo che rappresenta l'altezza del blocco. Per calcolare l'altezza futura del blocco, devi prima sapere qual è l'altezza attuale del blocco. `bitcoin-cli getblockcount` ti dirà quale è l'altezza del blocco secondo la tua macchina locale. Potresti voler ricontrollare con un esploratore Bitcoin.

Una volta determinata l'altezza attuale, puoi decidere fino a che punto nel futuro impostare il tempo di blocco. Ricorda che in media verrà creato un nuovo blocco ogni 10 minuti. Quindi, ad esempio, se volessi impostare il tempo di blocco su una settimana nel futuro, sceglieresti un'altezza del blocco pari a 6 x 24 x 7 = 1.008 blocchi dopo rispetto a quella attuale.

> :book: **_Perché dovrei usare un Blockheight?_** A differenza dei timestamp, non c'è confusione per i blockheight. Se imposti un'altezza del blocco di 120.000 per il tuo locktime, non c'è assolutamente alcun modo per farlo entrare nel blocco 119.999. Ciò può rendere più semplice il controllo algoritmico della transazione bloccata. Lo svantaggio è che non puoi essere sicuro di quando sarà esattamente l'ora di blocco.

> :warning: **AVVISO:** Se vuoi impostare un block-height locktime, devi impostare il tempo di blocco su un valore inferiore a 500 milioni. Se lo imposti su 500 milioni o più, il tuo numero verrà invece interpretato come un timestamp. Poiché il timestamp UNIX di 500 milioni era il 5 novembre 1985, ciò probabilmente significa che la tua transazione verrà bloccata alla prima occasione dei minatori.

## Scrivere la transazione

Una volta stabilito il tempo di blocco, tutto ciò che devi fare è scrivere una tipica transazione grezza, con una terza variabile per `locktime`:
```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.001, "'$changeaddress'": 0.00095 }''' locktime=1774650)
```
Tieni presente che questo utilizzo di `locktime` è inferiore a 500 milioni, il che significa che definisce l'altezza del blocco. In questo caso, sono solo pochi blocchi oltre l'altezza del blocco attuale al momento della stesura di questo articolo, pensato per esemplificare come funziona il locktime senza stare a lungo ad aspettare e vedere cosa succede.

Ecco come appare la transazione creata:
```
$ bitcoin-cli -named decoderawtransaction hexstring=$rawtxhex 
{
  "txid": "ba440b1dd87a7ccb6a200f087d2265992588284eed0ae455d0672aeb918cf71e",
  "hash": "ba440b1dd87a7ccb6a200f087d2265992588284eed0ae455d0672aeb918cf71e",
  "version": 2,
  "size": 113,
  "vsize": 113,
  "weight": 452,
  "locktime": 1774650,
  "vin": [
    {
      "txid": "0ad9fb6992dfe4ea90236b69852b3605c0175633b32996a486dcd0b2e739e385",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967294
    }
  ],
  "vout": [
    {
      "value": 0.00100000,
      "n": 0,
      "scriptPubKey": {
        "asm": "0 f333554cc0830d03a9c1f26758e2e7e0f155539f",
        "hex": "0014f333554cc0830d03a9c1f26758e2e7e0f155539f",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1q7ve42nxqsvxs82wp7fn43ch8urc425ul5um4un"
        ]
      }
    },
    {
      "value": 0.00095000,
      "n": 1,
      "scriptPubKey": {
        "asm": "0 a37718a3510958112b6a766e0023ff251b6c2bfb",
        "hex": "0014a37718a3510958112b6a766e0023ff251b6c2bfb",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1q5dm33g63p9vpz2m2wehqqglly5dkc2lmtmr98d"
        ]
      }
    }
  ]
}
```
Tieni presente che il numero di sequenza (`4294967294`) è inferiore a `0xffffffff`. Questa è una segnalazione necessaria per mostrare che la transazione include un locktime. Viene eseguito anche automaticamente da `bitcoin-cli`. Se il numero di sequenza è invece impostato su `"0xffffffff`, il tempo di blocco verrà ignorato.

> :information_source: **NOTA — SEQUENCE:** Questo è il secondo utilizzo del valore `nSequence` in Bitcoin. Come con RBF, `nSequence` viene nuovamente utilizzato come opt-in, questa volta per l'utilizzo di locktime. 0xffffffff-1 (4294967294) è il valore preferito per segnalare locktime perché non consente intenzionalmente l'uso sia di RBF (che richiede `nSequence < 0xffffffff-1`) che di timelock relativo (che richiede `nSequence < 0xf0000000`), gli altri due usano del valore "nSequence". Se imposti "nSequence" su un valore inferiore a "0xf0000000", bloccherai anche la transazione, il che probabilmente non è quello che desideri.

> :avviso: **AVVISO:** Se stai creando una transazione grezza locktime con un mezzo diverso da `bitcoin-cli`, dovrai impostare manualmente la sequenza su un valore inferiore a `0xffffffff`.

## Invia la tua transazione

Ormai probabilmente hai già familiarità con il completamento finale delle transazioni:
```
$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
error code: -26
error message:
non-final
```
Oops! Cos'è quell'errore!?

Dal 2013, generalmente non è possibile inserire la transazione bloccata nel mempool finché il suo blocco non è scaduto. Tuttavia, puoi comunque trattenere la transazione, inviandola occasionalmente alla rete Bitcoin finché non viene accettata nel mempool. In alternativa, potresti inviare la transazione firmata (`$signedtx`) al destinatario, in modo che possa inserirla nel mempool una volta scaduto il locktime.

Una volta trascorso il tempo di blocco, chiunque può inviare la transazione firmata alla rete e il destinatario riceverà il denaro come previsto... a condizione che la transazione non sia stata annullata.

## Annullare una transazione Locktime

Annullare una transazione locktime è _molto_ semplice: invii una nuova transazione utilizzando almeno uno degli stessi UTXO.

## Riepilogo: invio di una transazione con un locktime

Locktime offre un modo per creare una transazione che _dovrebbe_ non essere inoltrabile alla rete e che _non_ sarà_ accettata in un blocco finché non sarà arrivato il momento appropriato. Nel frattempo è possibile annullarlo semplicemente riutilizzando un UTXO.

> :fire: **_Qual è il potere di Locktime?_** Il potere di Locktime potrebbe non essere immediatamente evidente a causa della possibilità di annullarlo così facilmente. Tuttavia, è un'altra delle basi degli Smart Contracts: ha molta utilità in una varietà di applicazioni custodiali o contrattuali. Ad esempio, considera una situazione in cui una terza parte detiene i tuoi bitcoin. Per garantire la restituzione dei tuoi bitcoin nel caso in cui il custode scomparisse, potrebbe produrre una transazione con blocco temporale per restituirti le monete, quindi aggiornarla di tanto in tanto con una nuova, più avanti in futuro. Se mai non riuscissero ad aggiornarsi, le monete ti verranno restituite allo scadere del blocco temporale corrente. Allo stesso modo Locktime potrebbe essere applicato a una rete di pagamento, in cui la rete trattiene monete mentre vengono scambiate dai partecipanti alla rete. Infine, un testamento offre un esempio di contratto più complesso, in cui i pagamenti vengono inviati a un numero di persone. Questi pagamenti verrebbero basati su transazioni bloccate e verrebbero continuamente aggiornati finché il proprietario continua a mostrare segni di vita. (Il fattore unificante di tutte queste applicazioni è, ovviamente, la _fiducia_. Le semplici transazioni locktime funzionano solo se ci si può fidare del detentore delle monete per inviarle alle condizioni appropriate.)

## Qual è il prossimo argomento?

Continua con "Espansione delle transazioni Bitcoin" nel [Capitolo 8.2: Inviare una Transazione con Dati](08_2_Inviare_una_Transazione_con_Dati.md).
