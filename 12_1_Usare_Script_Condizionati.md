# 12.1: Utilizzo dei Condizionali negli Script

C'è un ultimo aspetto degli Script Bitcoin che è cruciale per sbloccare il loro vero potere: i condizionali ti permettono di creare vari percorsi di esecuzione.

## Comprendere Verify

Hai già visto un condizionale negli script: `OP_VERIFY` (0x69). Estrae l'elemento in cima allo stack e verifica se è vero; in caso contrario _termina l'esecuzione dello script_.

Verify è solitamente incorporato in altri opcode. Hai già visto `OP_EQUALVERIFY` (0xad), `OP_CHECKLOCKTIMEVERIFY` (0xb1) e `OP_CHECKSEQUENCEVERIFY` (0xb2). Ciascuno di questi opcode esegue la sua azione principale (equal, checklocktime o checksequence) e poi esegue un verify successivamente. Gli altri opcode verify che non hai visto sono: `OP_NUMEQUALVERIFY` (0x9d), `OP_CHECKSIGVERIFY` (0xad) e `OP_CHECKMULTISIGVERIFY` (0xaf).

Quindi come è `OP_VERIFY` un condizionale? È il tipo più potente di condizionale. Usando `OP_VERIFY`, _se_ una condizione è vera, lo Script continua l'esecuzione, _altrimenti_ lo Script termina. Questo è il modo in cui controlli le condizioni che sono assolutamente necessarie affinché uno Script abbia successo. Ad esempio, lo script P2PKH (`OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG`) ha due condizioni richieste: (1) che la chiave pubblica fornita corrisponda all'hash della chiave pubblica; e (2) che la firma fornita corrisponda a quella chiave pubblica. Un `OP_EQUALVERIFY` è usato per il confronto dell'hash della chiave pubblica perché è una condizione assolutamente necessaria. Non vuoi che lo script continui se ciò fallisce.

Potresti notare che non c'è `OP_VERIFY` alla fine di questo (o della maggior parte degli) script, nonostante la condizione finale sia richiesta. Questo perché Bitcoin effettua effettivamente un `OP_VERIFY` alla fine di ogni Script, per garantire che il risultato finale dello stack sia vero. Non vuoi fare un `OP_VERIFY` prima della fine dello script, perché devi lasciare qualcosa nello stack da verificare!

## Comprendere If/Then

L'altro principale condizionale in Bitcoin Script è il classico `OP_IF` (0x63) / `OP_ELSE` (0x67) / `OP_ENDIF` (0x68). Questo è il controllo di flusso tipico: se `OP_IF` rileva una dichiarazione vera, esegue il blocco sotto di esso; altrimenti, se c'è un `OP_ELSE`, esegue quello; e `OP_ENDIF` segna la fine del blocco finale.

> :warning: **ATTENZIONE:** Questi condizionali sono tecnicamente anche opcode, ma come per i numeri piccoli, lasceremo il prefisso `OP_` per brevità e chiarezza. Quindi scriveremo `IF`, `ELSE` e `ENDIF` invece di `OP_IF`, `OP_ELSE` e `OP_ENDIF`.

### Comprendere l'Ordine di If/Then

Ci sono due grandi problemi nei condizionali. Rendono più difficile leggere e valutare gli script se non stai attento.

Primo, il condizionale `IF` controlla la verità di ciò che è _prima di esso_ (cioè ciò che è nello stack), non ciò che è dopo di esso.

Secondo, il condizionale `IF` tende a essere nello script di blocco e ciò che sta controllando tende a essere nello script di sblocco.

Certo, potresti dire, è così che funziona Bitcoin Script. I condizionali usano la notazione polacca inversa e adottano il paradigma standard di sblocco/blocco, proprio come _tutto il resto_ nel Bitcoin Scripting. Tutto ciò è vero, ma va anche contro il modo standard in cui leggiamo i condizionali IF/ELSE in altri linguaggi di programmazione; quindi, è facile leggere inconsciamente i condizionali Bitcoin in modo errato.

Considera il seguente codice: `IF OP_DUP OP_HASH160 <pubKeyHashA> ELSE OP_DUP OP_HASH160 <pubKeyHashB> ENDIF OP_EQUALVERIFY OP_CHECKSIG`.

Guardando i condizionali in notazione prefissa potresti leggere questo come::
```
IF (OP_DUP) THEN
    OP_HASH160 
    OP_PUSHDATA <pubKeyHashA> 
ELSE 
    OP_DUP 
    OP_HASH160 
    OP_PUSHDATA <pubKeyHashB> 
ENDIF 
 OP_EQUALVERIFY 
 OP_CHECKSIG
```
Quindi, potresti pensare, se `OP_DUP` ha successo, allora eseguiamo il primo blocco, altrimenti il secondo. Ma ciò non ha senso! Perché `OP_DUP` non dovrebbe avere successo?!

E, infatti, non ha senso, perché abbiamo letto accidentalmente l'affermazione usando la notazione sbagliata. La lettura corretta di questo è:

```
IF 
    OP_DUP
    OP_HASH160 
    OP_PUSHDATA <pubKeyHashA> 
ELSE 
    OP_DUP 
    OP_HASH160 
    OP_PUSHDATA <pubKeyHashB> 
ENDIF 
 OP_EQUALVERIFY 
 OP_CHECKSIG
```
L'affermazione che sarà valutata come `True` o `False` è posta nello stack _prima_ di eseguire l'`IF`, quindi il blocco di codice corretto viene eseguito in base a quel risultato.

Questo esempio di codice è inteso come una multisignature 1-di-2 di basso livello. Il proprietario di `<privKeyA>` metterebbe `<signatureA> <pubKeyA> True` nel suo script di sblocco, mentre il proprietario di `<privKeyB>` metterebbe `<signatureB> <pubKeyB> False` nel suo script di sblocco. Quel `True` o `False` finale è ciò che viene controllato dall'istruzione `IF`/`ELSE`. Dice allo script quale hash della chiave pubblica controllare, quindi `OP_EQUALVERIFY` e `OP_CHECKSIG` alla fine fanno il lavoro reale.

### Eseguire una Multisig If/Then

Con una comprensione di base dei condizionali Bitcoin, siamo ora pronti a eseguire uno script. Creeremo una leggera variante della nostra multisignature 1-di-2 di basso livello dove i nostri utenti non devono ricordare se sono `True` o `False`. Invece, se necessario, lo script controlla entrambi gli hash delle chiavi pubbliche, richiedendo solo un successo:

```
OP_DUP OP_HASH160 <pubKeyHashA> OP_EQUAL
IF
    OP_CHECKSIG
ELSE
    OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG
ENDIF
```
Ricorda la notazione polacca inversa! Quell'istruzione `IF` si riferisce all'`OP_EQUAL` prima di essa, non all'`OP_CHECKSIG` dopo di essa!

#### Eseguire il Ramo True

Ecco come funziona effettivamente se sbloccato con `<signatureA> <pubKeyA>`:

```
Script: <signatureA> <pubKeyA> OP_DUP OP_HASH160 <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Stack: [ ]
```
Per prima cosa, mettiamo le costanti nello stack:

```
Script: OP_DUP OP_HASH160 <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Stack: [ <signatureA> <pubKeyA> ]
```
Poi eseguiamo i primi comandi ovvi, `OP_DUP` e `OP_HASH160`, e mettiamo un'altra costante:

```
Script: OP_HASH160 <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Running: <pubKeyA> OP_DUP
Stack: [ <signatureA> <pubKeyA> <pubKeyA> ]

Script: <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Running: <pubKeyA> OP_HASH160
Stack: [ <signatureA> <pubKeyA> <pubKeyHashA> ]

Script: OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Stack: [ <signatureA> <pubKeyA> <pubKeyHashA> <pubKeyHashA> ]
```
Successivamente, eseguiamo l'`OP_EQUAL`, che è ciò che alimenterà l'`IF`:
```
Script: IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Running: <pubKeyHashA> <pubKeyHashA> OP_EQUAL
Stack: [ <signatureA> <pubKeyA> True ]
```
Ora l'`IF` viene eseguito e, poiché c'è un `True`, esegue solo il primo blocco, eliminando tutto il resto:
```
Script: OP_CHECKSIG
Running: True IF
Stack: [ <signatureA> <pubKeyA> ]
```
E l'`OP_CHECKSIG` risulterà `True`:
```
Script: 
Running: <signatureA> <pubKeyA> OP_CHECKSIG
Stack: [ True ]
```
##### Eseguire il Ramo False

Ecco come funziona effettivamente se sbloccato con `<signatureB> <pubKeyB>`:

```
Script: <signatureB> <pubKeyB> OP_DUP OP_HASH160 <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Stack: [ ]
```
Per prima cosa, mettiamo le costanti nello stack:
```
Script: OP_DUP OP_HASH160 <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Stack: [ <signatureB> <pubKeyB> ]
```
Poi eseguiamo i primi comandi ovvi, `OP_DUP` e `OP_HASH160`, e mettiamo un'altra costante:
```
Script: OP_HASH160 <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Running: <pubKeyB> OP_DUP
Stack: [ <signatureB> <pubKeyB> <pubKeyB> ]

Script: <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Running: <pubKeyB> OP_HASH160
Stack: [ <signatureB> <pubKeyB> <pubKeyHashB> ]

Script: OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Stack: [ <signatureB> <pubKeyB> <pubKeyHashB> <pubKeyHashA> ]
```
Successivamente, eseguiamo l'`OP_EQUAL`, che è ciò che alimenterà l'`IF`:
```
Script: IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Running: <pubKeyHashB> <pubKeyHashA> OP_EQUAL
Stack: [ <signatureB> <pubKeyB> False ]
```
Ops! Il risultato è stato `False` perché `<pubKeyHashB>` non è uguale a `<pubKeyHashA>`. Ora quando l'`IF` viene eseguito, si riduce solo all'istruzione `ELSE`:
```
Script: OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG
Running: False IF
Stack: [ <signatureB> <pubKeyB> ]
```
Successivamente, rifacciamo tutto il processo, iniziando con un altro `OP_DUP`, ma alla fine testando contro l'altro `pubKeyHash`:
```
Script: OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG
Running: <pubKeyB> OP_DUP
Stack: [ <signatureB> <pubKeyB> <pubKeyB> ]

Script: <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG
Running: <pubKeyB> OP_HASH160
Stack: [ <signatureB> <pubKeyB> <pubKeyHashB> ]

Script: OP_EQUALVERIFY OP_CHECKSIG
Stack: [ <signatureB> <pubKeyB> <pubKeyHashB> <pubKeyHashB> ]

Script:OP_CHECKSIG
Running: <pubKeyHashB> <pubKeyHashB> OP_EQUALVERIFY 
Stack: [ <signatureB> <pubKeyB> ]

Script: 
Running: <signatureB> <pubKeyB> OP_CHECKSIG
Stack: [ True ]
```
TQuesto probabilmente non è altrettanto efficiente come una vera multisignature di Bitcoin, ma è un buon esempio di come i risultati inseriti nello stack dai test precedenti possono essere utilizzati per alimentare i condizionali futuri. In questo caso, è il fallimento della prima firma che dice al condizionale che dovrebbe controllare la seconda.

## Comprendere Altri Condizionali

Ci sono alcuni altri condizionali degni di nota. Il più importante è `OP_NOTIF` (0x64), che è l'opposto di `OP_IF`: esegue il blocco successivo se l'elemento in cima è `False`. Un `ELSE` può essere inserito con esso, che come al solito viene eseguito se il primo blocco non viene eseguito. Si termina ancora con `OP_ENDIF`.

C'è anche un `OP_IFDUP` (0x73), che duplica l'elemento in cima allo stack solo se non è 0.

Queste opzioni sono usate molto meno spesso della costruzione principale `IF`/`ELSE`/`ENDIF`.

## Riepilogo: Utilizzo dei Condizionali negli Script

I condizionali in Bitcoin Script ti permettono di fermare lo script (usando `OP_VERIFY`) o di scegliere diversi rami di esecuzione (usando `OP_IF`). Tuttavia, leggere `OP_IF` può essere un po' complicato. Ricorda che è l'elemento inserito nello stack _prima_ che l'`OP_IF` venga eseguito a controllarne l'esecuzione; quell'elemento sarà tipicamente parte dello script di sblocco (o altrimenti un risultato diretto degli elementi nello script di sblocco).

> :fire: ***Qual è il potere dei condizionali?*** I Condizionali degli Script sono l'ultimo grande blocco costitutivo negli Script Bitcoin. Sono ciò che è necessario per trasformare semplici Script Bitcoin statici in Script Bitcoin complessi e dinamici che possono essere valutati in modo diverso in base a tempi diversi, circostanze diverse o input diversi degli utenti. In altre parole, sono la base finale dei contratti intelligenti.

## Cosa Succede Dopo?

Continua a "Espandere gli Script Bitcoin" col [Capitolo 12.2Usare Altri Comandi di Scripting](12_2_Usare_Altri_Comandi_di_Scripting.md).
