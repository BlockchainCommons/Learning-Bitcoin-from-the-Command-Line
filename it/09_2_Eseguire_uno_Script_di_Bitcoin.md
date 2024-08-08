# 9.2: Esecuzione di uno script Bitcoin

Gli script Bitcoin inizialmente potrebbero non sembrare così intuitivi, ma la loro esecuzione è abbastanza semplice, utilizzando la notazione Polacca inversa e uno stack.

## Comprendere il linguaggio di scripting

Uno script Bitcoin è composto da tre parti: ha una riga di input; ha uno stack per la conservazione; e ha comandi specifici per l'esecuzione.

### Comprendere l'ordine

Gli script Bitcoin vengono eseguiti da sinistra a destra. Sembra abbastanza facile, perché è lo stesso modo in cui leggi. Tuttavia, potrebbe in realtà essere l'elemento meno intuitivo di Bitcoin Script, perché significa che le funzioni non appaiono come ti aspetteresti. Invece, _gli operandi vanno prima dell'operatore._

Ad esempio, se dovessi sommare "1" e "2", il tuo script Bitcoin sarebbe `1 2 OP_ADD`, _non_ "1 + 2". Poiché sappiamo che l'operatore OP_ADD accetta due input, sappiamo che i due input prima di esso sono i suoi operandi.

> :warning: **AVVISO:** Tecnicamente, tutto in Bitcoin Script è un codice operativo, quindi sarebbe più appropriato registrare l'esempio precedente come `OP_1 OP_2 OP_ADD`. Nei nostri esempi, non ci preoccupiamo di come verranno valutate le costanti, poiché questo è un argomento di traduzione, come spiegato nel [Capitolo 10.2: Costruire la struttura di P2SH](10_2_Construire_la_Struttura_di_P2SH.md). Alcuni autori preferiscono lasciare anche il prefisso "OP" su tutti gli operatori, ma noi abbiamo deciso di non farlo.

### Comprendere lo stack

In realtà non è del tutto corretto dire che un operatore si applica agli input che lo precedono. In realtà, un operatore si applica agli input principali nello stack di Bitcoin.

> :book: ***Cos'è uno stack?*** Uno stack è una struttura dati LIFO (last-in-first-out). Ha due funzioni di accesso: push e pop. Push posiziona un nuovo oggetto in cima alla pila, spingendo verso il basso tutto ciò che si trova sotto di esso. Pop rimuove l'oggetto in cima alla pila.

Ogni volta che Bitcoin Script incontra una costante, la inserisce nello stack. Quindi l'esempio sopra di "1 2 OP_ADD" sarebbe effettivamente simile a questo mentre veniva elaborato:
```
Script: 1 2 OP_ADD
Stack: [ ]

Script: 2 OP_ADD
Stack: [ 1 ]

Script: OP_ADD
Stack: [ 1 2 ]
```
_Nota che in questo esempio e nei seguenti esempi la parte superiore dello stack è a destra e la parte inferiore a sinistra._

### Comprendere i codici operativi

Quando uno script Bitcoin incontra un operatore, lo valuta. Ogni operatore estrae zero o più elementi dallo stack come input, solitamente uno o due. Quindi li elabora in un modo specifico prima di rimettere zero o più elementi nello stack, solitamente uno o due.

> :book: ***Cos'è un Opcode?*** Opcode sta per "codice operativo". È tipicamente associato al codice in linguaggio macchina ed è una funzione semplice (o "operatore").

OP_ADD estrae due elementi dallo stack (qui: 2 poi 1), li somma e rimette il risultato nello stack (qui: 3).
```
Script:
Running: 1 2 OP_ADD
Stack: [ 3 ]
```

## Aumentiamo la complessità

Gli script più complessi vengono creati eseguendo più comandi in ordine. Devono essere valutati attentamente da sinistra a destra, in modo da poter comprendere lo stato dello stack quando viene eseguito ogni nuovo comando. Cambierà costantemente, come risultato degli operatori precedenti:
```
Script: 3 2 OP_ADD 4 OP_SUB
Stack: [ ]

Script: 2 OP_ADD 4 OP_SUB
Stack: [ 3 ]

Script: OP_ADD 4 OP_SUB
Stack: [ 3 2 ]

Script: 4 OP_SUB
Running: 3 2 OP_ADD
Stack: [ 5 ]

Script: OP_SUB
Stack: [ 5 4 ]

Script: 
Running: 5 4 OP_SUB
Stack: [ 1 ]
```

## Comprendere l'utilizzo dello script Bitcoin

Questo è praticamente Bitcoin Scripting... a parte alcune complessità su come questo linguaggio di scripting interagisce con Bitcoin stesso.

### Comprendere scriptSig e scriptPubKey

Come abbiamo visto, ogni input per una transazione Bitcoin contiene uno "scriptSig" utilizzato per sbloccare lo "scriptPubKey" per l'UTXO associato. Sono _effettivamente_ concatenati insieme, il che significa che `scriptSig` e `scriptPubKey` vengono eseguiti insieme, in quest'ordine.

Quindi, presumi che un UTXO sia stato bloccato con uno `scriptPubKey` che legga `OP_ADD 99 OP_EQUAL`, richiedendo come input due numeri che si sommano a novantanove, e presumi che `scriptSig` di `1 98` sia stato eseguito per sbloccare Esso. I due script verrebbero effettivamente eseguiti in ordine come "1 98 OP_ADD 99 OP_EQUAL".

Valutare il risultato:
```
Script: 1 98 OP_ADD 99 OP_EQUAL
Stack: []

Script: 98 OP_ADD 99 OP_EQUAL
Stack: [ 1 ]

Script: OP_ADD 99 OP_EQUAL
Stack: [ 1 98 ]

Script: 99 OP_EQUAL
Running: 1 98 OP_ADD
Stack: [ 99 ]

Script: OP_EQUAL
Stack: [ 99 99 ]

Script: 
Running: 99 99 OP_EQUAL
Stack: [ True ]
```
Questa astrazione non è del tutto accurata: per motivi di sicurezza, viene eseguito `scriptSig`, quindi il contenuto dello stack viene trasferito per l'esecuzione di `scriptPubKey`, ma è sufficientemente accurato per comprendere come si inserisce la chiave di `scriptSig` il blocco di `scriptPubKey`.

> :warning: **AVVISO** Quanto sopra è un tipo di transazione non standard. In realtà non sarebbe accettato dai nodi che eseguono Bitcoin Core con le impostazioni standard. [Capitolo 10.1:Comprendere la Base di P2SH](10_1_Comprendere_la_Base_di_P2SH.md) spiega come effettivamente _potresti_ eseguire uno script Bitcoin come questo, utilizzando la potenza di P2SH.

### Ottieni i risultati

Bitcoin verificherà una transazione e consentirà di spendere l'UTXO se questi due criteri vengono soddisfatti durante l'esecuzione di `scriptSig` e `scriptPubKey`:

 1. L'esecuzione non è stata contrassegnata come non valida in nessun momento, ad esempio con un `OP_VERIFY` non riuscito o con l'utilizzo di un codice operativo disabilitato.
 2. L'elemento in cima allo stack alla fine dell'esecuzione è `true` (diverso da zero).

Nell'esempio sopra, la transazione avrebbe successo perché lo stack ha un `true` in cima. Ma sarebbe altrettanto lecito terminare con uno stack completo e il numero `42` in cima.

## Riepilogo: Eseguire uno script Bitcoin

Per eseguire uno script Bitcoin, si eseguie uno `scriptSig` seguito dallo `scriptPubKey` che lo sta sbloccando. Questi comandi vengono eseguiti in ordine, da sinistra a destra, con le costanti che vengono inserite in uno stack e gli operatori che estraggono elementi da quello stack, quindi reinseriscono i risultati su di esso. Se lo script non si ferma a metà e se l'elemento in cima allo stack alla fine è diverso da zero, l'UTXO è sbloccato.

## Qual è il prossimo argomento?

Continua con "Introduzione agli script Bitcoin" nel [Capitolo 9.3: Provare uno Script Bitcoin](09_3_Provare_uno_Script_Bitcoin.md).
