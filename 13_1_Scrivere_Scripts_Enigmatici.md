# 13.1: Scrivere Script Enigmatici

Gli Script Bitcoin _non_ devono necessariamente dipendere dalla conoscenza di una chiave segreta. Possono invece essere enigmi di qualsiasi tipo.

## Scrivere Script di Algebra Semplici

Il nostro primo vero script, dal [Capitolo 9.2: Eseguire uno Script di Bitcoin](009_2_Eseguire_uno_Script_di_Bitcoin.md), era un enigma algebrico. Quello script Bitcoin, `OP_ADD 99 OP_EQUAL`, potrebbe essere descritto alternativamente come `x + y = 99`.

Questo tipo di script non ha molta applicabilità nel mondo reale, poiché è troppo facile rivendicare i fondi. Tuttavia, un concorso di risoluzione di enigmi che distribuisce piccole quantità di Bitcoin potrebbe proporlo come un intrattenimento divertente.

Più precisamente, creare enigmi algebrici ti dà una buona comprensione di come funzionano le funzioni aritmetiche negli script Bitcoin.

### Scrivere uno Script Moltiplicatore

Bitcoin Script ha un certo numero di opcode che sono stati disabilitati per mantenere la sicurezza del sistema. Uno di questi è `OP_MUL`, che avrebbe permesso la moltiplicazione... ma è invece disabilitato.

Quindi, come scriveresti una funzione algebrica come `3x + 7 = 13`?

La risposta più ovvia è `OP_DUP` il numero di input dallo script di blocco due volte. Poi puoi aggiungere `7` e continuare ad aggiungere fino a ottenere il totale. Lo script di blocco completo sarebbe così: `OP_DUP OP_DUP 7 OP_ADD OP_ADD OP_ADD 13 OP_EQUAL`.

Ecco come verrebbe eseguito se eseguito con lo script di sblocco corretto di `2`:

```
Script: 2 OP_DUP OP_DUP 7 OP_ADD OP_ADD OP_ADD 13 OP_EQUAL
Stack: [ ]

Script: OP_DUP OP_DUP 7 OP_ADD OP_ADD OP_ADD 13 OP_EQUAL
Stack: [ 2 ]

Script: OP_DUP 7 OP_ADD OP_ADD OP_ADD 13 OP_EQUAL
Running: 2 OP_DUP
Stack: [ 2 2 ]

Script: 7 OP_ADD OP_ADD OP_ADD 13 OP_EQUAL
Running: 2 OP_DUP
Stack: [ 2 2 2 ]

Script: OP_ADD OP_ADD OP_ADD 13 OP_EQUAL
Stack: [ 2 2 2 7 ]

Script: OP_ADD OP_ADD 13 OP_EQUAL
Running: 2 7 OP_ADD
Stack: [ 2 2 9 ]

Script: OP_ADD 13 OP_EQUAL
Running: 2 9 OP_ADD
Stack: [ 2 11 ]

Script: 13 OP_EQUAL
Running: 2 11 OP_ADD
Stack: [ 13 ]

Script: OP_EQUAL
Stack: [ 13 13 ]

Script: 
Running: 13 13 OP_EQUAL
Stack: [ True ]
```
Oppure se preferisci `btcdeb`:
```
$ btcdeb '[2 OP_DUP OP_DUP 7 OP_ADD OP_ADD OP_ADD 13 OP_EQUAL]'
btcdeb 0.2.19 -- type `btcdeb -h` for start up options
valid script
9 op script loaded. type `help` for usage information
script   |  stack 
---------+--------
2        | 
OP_DUP   | 
OP_DUP   | 
7        | 
OP_ADD   | 
OP_ADD   | 
OP_ADD   | 
13       | 
OP_EQUAL | 

#0000 2
btcdeb> step
		<> PUSH stack 02
script   |  stack 
---------+--------
OP_DUP   | 02
OP_DUP   | 
7        | 
OP_ADD   | 
OP_ADD   | 
OP_ADD   | 
13       | 
OP_EQUAL | 

#0001 OP_DUP
btcdeb> step
		<> PUSH stack 02
script   |  stack 
---------+--------
OP_DUP   |      02
7        | 02
OP_ADD   | 
OP_ADD   | 
OP_ADD   | 
13       | 
OP_EQUAL | 

#0002 OP_DUP
btcdeb> step
		<> PUSH stack 02
script   |  stack 
---------+--------
7        |      02
OP_ADD   |      02
OP_ADD   | 02
OP_ADD   | 
13       | 
OP_EQUAL | 

#0003 7
btcdeb> step
		<> PUSH stack 07
script   |  stack 
---------+--------
OP_ADD   |      07
OP_ADD   |      02
OP_ADD   |      02
13       | 02
OP_EQUAL | 

#0004 OP_ADD
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 09
script   |  stack 
---------+--------
OP_ADD   |      09
OP_ADD   |      02
13       | 02
OP_EQUAL | 

#0005 OP_ADD
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 0b
script   |  stack 
---------+--------
OP_ADD   |      0b
13       | 02
OP_EQUAL | 

#0006 OP_ADD
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 0d
script   |  stack 
---------+--------
13       | 0d
OP_EQUAL | 
#0007 13
btcdeb> step
		<> PUSH stack 0d
script   |  stack 
---------+--------
OP_EQUAL |      0d
         | 0d

#0008 OP_EQUAL
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 01
script   |  stack 
---------+--------
         | 01
```
### Scrivere un Sistema di Equazioni

E se volessi invece scrivere un sistema di equazioni, come `x + y = 3`, `y + z = 5` e `x + z = 4`? Un po' di algebra ti dice che le risposte sono `x = 1`, `y = 2` e `z = 3`. Ma come lo scripti?

Ovviamente, dopo che il redentore ha inserito i tre numeri, avrai bisogno di due copie di ogni numero, poiché ogni numero entra in due equazioni diverse. `OP_3DUP` si occupa di questo e risulta in `x y z x y z` sullo stack. Eliminando due elementi alla volta otterrai `y z`, `z x` e `x y`. Voilà! Queste sono le tre equazioni, quindi devi solo sommarle e testarle nell'ordine giusto! Ecco lo script completo: `OP_3DUP OP_ADD 5 OP_EQUALVERIFY OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL`.

Ecco come funziona con lo script di sblocco corretto di `1 2 3`:

```
Script: 1 2 3 OP_3DUP OP_ADD 5 OP_EQUALVERIFY OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Stack: [ ]

Script: OP_3DUP OP_ADD 5 OP_EQUALVERIFY OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Stack: [ 1 2 3 ]

Script: OP_ADD 5 OP_EQUALVERIFY OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Running: 1 2 3 OP_3DUP
Stack: [ 1 2 3 1 2 3 ]

Script: 5 OP_EQUALVERIFY OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Running: 2 3 OP_ADD
Stack: [ 1 2 3 1 5 ]

Script: OP_EQUALVERIFY OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Stack: [ 1 2 3 1 5 5 ]

Script: OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Running: 5 5 OP_EQUALVERIFY
Stack: [ 1 2 3 1 ] — Does Not Exit

Script: 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Running: 3 1 OP_ADD
Stack: [ 1 2 4 ]

Script: OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Stack: [ 1 2 4 4 ]

Script: OP_ADD 3 OP_EQUAL
Running: 4 4 OP_EQUALVERIFY
Stack: [ 1 2 ] — Does Not Exit

Script: 3 OP_EQUAL
Running: 1 2 OP_ADD
Stack: [ 3 ]

Script: OP_EQUAL
Stack: [ 3 3 ]

Script: 
Running: 3 3 OP_EQUAL
Stack: [ True ]
```
Ecco in `btcdeb`:
```
$ btcdeb '[1 2 3 OP_3DUP OP_ADD 5 OP_EQUALVERIFY OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL]'
btcdeb 0.2.19 -- type `btcdeb -h` for start up options
valid script
13 op script loaded. type `help` for usage information
script         |  stack 
---------------+--------
1              | 
2              | 
3              | 
OP_3DUP        | 
OP_ADD         | 
5              | 
OP_EQUALVERIFY | 
OP_ADD         | 
4              | 
OP_EQUALVERIFY | 
OP_ADD         | 
3              | 
OP_EQUAL       | 

#0000 1
btcdeb> step
		<> PUSH stack 01
script         |  stack 
---------------+--------
2              | 01
3              | 
OP_3DUP        | 
OP_ADD         | 
5              | 
OP_EQUALVERIFY | 
OP_ADD         | 
4              | 
OP_EQUALVERIFY | 
OP_ADD         | 
3              | 
OP_EQUAL       | 

#0001 2
btcdeb> step
		<> PUSH stack 02
script         |  stack 
---------------+--------
3              |      02
OP_3DUP        | 01
OP_ADD         | 
5              | 
OP_EQUALVERIFY | 
OP_ADD         | 
4              | 
OP_EQUALVERIFY | 
OP_ADD         | 
3              | 
OP_EQUAL       | 

#0002 3
btcdeb> step
		<> PUSH stack 03
script         |  stack 
---------------+--------
OP_3DUP        |      03
OP_ADD         |      02
5              | 01
OP_EQUALVERIFY | 
OP_ADD         | 
4              | 
OP_EQUALVERIFY | 
OP_ADD         | 
3              | 
OP_EQUAL       | 

#0003 OP_3DUP
btcdeb> step
		<> PUSH stack 01
		<> PUSH stack 02
		<> PUSH stack 03
script         |  stack 
---------------+--------
OP_ADD         |      03
5              |      02
OP_EQUALVERIFY |      01
OP_ADD         |      03
4              |      02
OP_EQUALVERIFY | 01
OP_ADD         | 
3              | 
OP_EQUAL       | 

#0004 OP_ADD
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 05
script         |  stack 
---------------+--------
5              |      05
OP_EQUALVERIFY |      01
OP_ADD         |      03
4              |      02
OP_EQUALVERIFY | 01
OP_ADD         | 
3              | 
OP_EQUAL       | 

#0005 5
btcdeb> step
		<> PUSH stack 05
script         |  stack 
---------------+--------
OP_EQUALVERIFY |      05
OP_ADD         |      05
4              |      01
OP_EQUALVERIFY |      03
OP_ADD         |      02
3              | 01
OP_EQUAL       | 

#0006 OP_EQUALVERIFY
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 01
		<> POP  stack
script         |  stack 
---------------+--------
OP_ADD         |      01
4              |      03
OP_EQUALVERIFY |      02
OP_ADD         | 01
3              | 
OP_EQUAL       | 

#0007 OP_ADD
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 04
script         |  stack 
---------------+--------
4              |      04
OP_EQUALVERIFY |      02
OP_ADD         | 01
3              | 
OP_EQUAL       | 

#0008 4
btcdeb> step
		<> PUSH stack 04
script         |  stack 
---------------+--------
OP_EQUALVERIFY |      04
OP_ADD         |      04
3              |      02
OP_EQUAL       | 01

#0009 OP_EQUALVERIFY
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 01
		<> POP  stack
script         |  stack 
---------------+--------
OP_ADD         |      02
3              | 01
OP_EQUAL       | 

#0010 OP_ADD
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 03
script         |  stack 
---------------+--------
3              | 03
OP_EQUAL       | 

#0011 3
btcdeb> step
		<> PUSH stack 03
script         |  stack 
---------------+--------
OP_EQUAL       |      03
               | 03

#0012 OP_EQUAL
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 01
script         |  stack 
---------------+--------
               | 01
```

> :warning: **ATTENZIONE** `btcdeb` non è solo utile per fornire una visualizzazione di questi script, ma anche per controllare i risultati. Sicuramente, abbiamo sbagliato la prima volta, testando le equazioni nell'ordine sbagliato. È così facile commettere un errore finanziario fatale in uno script Bitcoin, ed è per questo che ogni script deve essere testato.

## Scrivere Script Computazionali Semplici

Anche se gli script enigmatici sono banali, possono effettivamente avere un'utilità nel mondo reale se vuoi fare crowdsourcing di un calcolo. Crei semplicemente uno script che richiede la risposta al calcolo e invii fondi all'indirizzo P2SH come ricompensa. Rimarrà lì fino a quando qualcuno non troverà la risposta.

Ad esempio, Peter Todd ha [offerto ricompense](https://bitcointalk.org/index.php?topic=293382.0) per risolvere equazioni che dimostrano collisioni per algoritmi crittografici standard. Ecco il suo script per confermare una collisione SHA1: `OP_2DUP OP_EQUAL OP_NOT OP_VERIFY OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL`. Richiede due input, che saranno i due numeri che collidono.

Ecco come funziona con le risposte corrette.

Per prima cosa, riempiamo il nostro stack:

```
Script: <numA> <numB> OP_2DUP OP_EQUAL OP_NOT OP_VERIFY OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL
Stack: [ ]

Script: OP_2DUP OP_EQUAL OP_NOT OP_VERIFY OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL
Stack: [ <numA> <numB> ]

Script: OP_EQUAL OP_NOT OP_VERIFY OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL
Running: <numA> <numB> OP_2DUP
Stack: [ <numA> <numB> <numA> <numB> ]
```
Poi, ci assicuriamo che i due numeri non siano uguali, uscendo se lo sono:
```
Script: OP_NOT OP_VERIFY OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL
Running: <numA> <numB> OP_EQUAL
Stack: [ <numA> <numB> False ]

Script: OP_VERIFY OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL
Running: False OP_NOT
Stack: [ <numA> <numB> True ]

Script: OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL
Running: True OP_VERIFY
Stack: [ <numA> <numB> ] — Does Not Exit
```
Ora creiamo due SHA:
```
Script: OP_SWAP OP_SHA1 OP_EQUAL
Running: <numB> OP_SHA1
Stack: [ <numA> <hashB> ]

Script: OP_SHA1 OP_EQUAL
Running: <numA> <hashB> OP_SWAP
Stack: [ <hashB> <numA> ]

Script: OP_EQUAL
Running: <numA> OP_SHA1
Stack: [ <hashB> <hashA> ]
```
Infine, vediamo se corrispondono.
```
Script: 
Running: <hashB> <hashA> OP_EQUAL
Stack: [ True ]
```
Questo è un buon script perché mostra un uso attento della logica (con `OP_NOT` e `OP_VERIFY`) e un buon uso delle funzioni stack (con `OP_SWAP`). È tutto sommato un ottimo esempio di una funzione del mondo reale. Ed è davvero del mondo reale. Quando [SHA-1 è stato violato](https://shattered.io/), 2.48 BTC sono stati rapidamente liberati dall'indirizzo, con un valore totale di circa $3,000 all'epoca.

`btcdeb` può essere eseguito per dimostrare la collisione (e lo script):

```
$ btcdeb '[255044462d312e330a25e2e3cfd30a0a0a312030206f626a0a3c3c2f57696474682032203020522f4865696768742033203020522f547970652034203020522f537562747970652035203020522f46696c7465722036203020522f436f6c6f7253706163652037203020522f4c656e6774682038203020522f42697473506572436f6d706f6e656e7420383e3e0a73747265616d0affd8fffe00245348412d3120697320646561642121212121852fec092339759c39b1a1c63c4c97e1fffe017f46dc93a6b67e013b029aaa1db2560b45ca67d688c7f84b8c4c791fe02b3df614f86db1690901c56b45c1530afedfb76038e972722fe7ad728f0e4904e046c230570fe9d41398abe12ef5bc942be33542a4802d98b5d70f2a332ec37fac3514e74ddc0f2cc1a874cd0c78305a21566461309789606bd0bf3f98cda8044629a1 255044462d312e330a25e2e3cfd30a0a0a312030206f626a0a3c3c2f57696474682032203020522f4865696768742033203020522f547970652034203020522f537562747970652035203020522f46696c7465722036203020522f436f6c6f7253706163652037203020522f4c656e6774682038203020522f42697473506572436f6d706f6e656e7420383e3e0a73747265616d0affd8fffe00245348412d3120697320646561642121212121852fec092339759c39b1a1c63c4c97e1fffe017346dc9166b67e118f029ab621b2560ff9ca67cca8c7f85ba84c79030c2b3de218f86db3a90901d5df45c14f26fedfb3dc38e96ac22fe7bd728f0e45bce046d23c570feb141398bb552ef5a0a82be331fea48037b8b5d71f0e332edf93ac3500eb4ddc0decc1a864790c782c76215660dd309791d06bd0af3f98cda4bc4629b1 OP_2DUP OP_EQUAL OP_NOT OP_VERIFY OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL]'
```

Le altre [ricompense](https://bitcointalk.org/index.php?topic=293382.0) di Peter Todd rimangono non rivendicate al momento della scrittura. Sono tutte scritte nello stesso modo dell'esempio SHA-1 sopra.

## Comprendere le Limitazioni degli Script Enigmatici

Gli script enigmatici sono ottimi per esaminare ulteriormente la Scrittura Bitcoin, ma li vedrai nel mondo reale solo se contengono piccole quantità di fondi o se sono destinati a essere riscattati da utenti molto abili. C'è una ragione per questo: non sono sicuri.

Ecco dove la sicurezza cade:

Innanzitutto, chiunque può riscattarli senza sapere molto di un segreto. Devono avere lo `script di riscatto`, che offre una certa protezione, ma una volta che lo hanno, probabilmente è l'unico segreto necessario — a meno che il tuo enigma non sia _davvero_ difficile, come un enigma computazionale.

In secondo luogo, il riscatto effettivo non è sicuro. Normalmente, una transazione Bitcoin è protetta dalla firma. Poiché la firma copre la transazione, nessuno sulla rete può riscrivere quella transazione per inviarla al proprio indirizzo senza invalidare la firma (e quindi la transazione). Questo non è vero con una transazione i cui input sono solo numeri. Chiunque potrebbe prendere la transazione e riscriverla per consentire loro di rubare i fondi. Se riescono a far entrare la loro transazione in un blocco prima della tua, vincono e tu non ottieni i soldi dell'enigma. Ci sono soluzioni per questo, ma coinvolgono il mining del blocco da soli o avere un pool di fiducia che lo mini, e nessuna di queste opzioni è razionale per un utente Bitcoin medio.

Eppure, le ricompense crittografiche di Peter Todd dimostrano che gli script enigmatici hanno qualche applicazione nel mondo reale.

## Riepilogo: Scrivere Script Enigmatici

Gli script enigmatici sono una grande introduzione a script Bitcoin più realistici e complessi. Dimostrano la potenza delle funzioni matematiche e di stack in Bitcoin Script e come possono essere combinati con cura per creare domande che richiedono risposte molto specifiche. Tuttavia, il loro uso nel mondo reale è limitato dai problemi di sicurezza inerenti alle transazioni Bitcoin non firmate.

> :fire: ***Qual è la potenza degli script enigmatici?*** Nonostante le loro limitazioni, gli script enigmatici sono stati usati nel mondo reale come premi per ricompense computazionali. Chiunque riesca a risolvere un enigma complesso, la cui soluzione presumibilmente ha un impatto nel mondo reale, può vincere la ricompensa. Se riescono a tenerla realmente è un'altra questione.

## Cosa c'è dopo?

Continua "Progettare Veri Script Bitcoin" con [Capitolo 13.2: Scrivere Scripts Multifirma Complessi](13_2_Scrivere_Scripts_Multifirma_Complessi.md). 
