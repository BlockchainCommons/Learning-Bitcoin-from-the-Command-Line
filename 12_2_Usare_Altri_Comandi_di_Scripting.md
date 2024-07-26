# 12.2: Utilizzo di Altri Comandi degli Script

Probabilmente hai già a portata di mano la maggior parte degli opcode di Bitcoin Script che utilizzerai nella maggior parte degli script. Tuttavia, Bitcoin Script offre molte più opzioni, che potrebbero essere esattamente ciò di cui hai bisogno per creare lo strumento finanziario dei tuoi sogni.

Dovresti consultare la [pagina Bitcoin Script](https://en.bitcoin.it/wiki/Script) per uno sguardo più approfondito a tutti questi e molti altri comandi. Questa sezione evidenzia solo gli opcode più notevoli.

## Comprendere gli Opcode Aritmetici

Gli opcode aritmetici manipolano o testano i numeri.

Manipolare un numero:

* OP_1ADD	(0x8b) — Incrementa di uno
* OP_1SUB	(0x8c)	 — Decrementa di uno
* OP_NEGATE	(0x8f)	— Inverte il segno del numero
* OP_ABS	(0x90)	— Rende il numero positivo
* OP_NOT (0x91) — Inverte 1 e 0, altrimenti 0

Vedi anche: `OP_0NOTEQUAL` (0x92)

Manipolare due numeri matematicamente:

* OP_ADD (0x93) — Somma due numeri
* OP_SUB (0x94) — Sottrae due numeri
* OP_MIN (0xa3) — Restituisce il minore di due numeri
* OP_MAX (0xa4) — Restituisce il maggiore di due numeri

Manipolare due numeri logicamente:

* OP_BOOLAND (0x9a)	— 1 se entrambi i numeri non sono 0, altrimenti 0
* OP_BOOLOR	(0x9b) — 1 se uno dei due numeri non è 0, altrimenti 0

Testare due numeri:

* OP_NUMEQUAL	(0x9c) — 1 se entrambi i numeri sono uguali, altrimenti 0
* OP_LESSTHAN	(0x9f) — 1 se il primo numero è minore del secondo, altrimenti 0
* OP_GREATERTHAN (0xa0) — 1 se il primo numero è maggiore del secondo, altrimenti 0
* OP_LESSTHANOREQUAL (0xa1) — 1 se il primo numero è minore o uguale al secondo, altrimenti 0
* OP_GREATERTHANOREQUAL (0xa2) — 1 se il primo numero è maggiore o uguale al secondo, altrimenti 0

Vedi anche: `OP_NUMEQUALVERIFY` (0x9d), `OP_NUMNOTEQUAL` (0x9e)

Testare tre numeri:

* OP_WITHIN	(0xa5) — 1 se un numero è nell'intervallo di due altri numeri

## Comprendere gli Opcode dello Stack

Ci sono un numero sorprendente di opcode dello stack, ma a parte `OP_DROP`, `OP_DUP` e talvolta `OP_SWAP`, generalmente non sono necessari se sei attento all'ordinamento dello stack. Tuttavia, ecco alcuni dei più interessanti:

* OP_DEPTH (0x74) — Inserisce la dimensione dello stack
* OP_DROP	(0x75) — Rimuove l'elemento in cima allo stack
* OP_DUP	(0x76) — Duplica l'elemento in cima allo stack
* OP_PICK (0x79) — Duplica l'elemento n dello stack come cima dello stack
* OP_ROLL (0x7a) — Sposta l'elemento n dello stack in cima allo stack
* OP_SWAP (0x7c) — Scambia i primi due elementi dello stack

Vedi anche: `OP_TOALTSTACK` (0x6b), `OP_FROMALTSTACK` (0x6c), `OP_2DROP` (0x6d), `OP_2DUP` (0x6e), `OP_3DUP` (0x6f), `OP_2OVER` (0x70), `OP_2ROT` (0x71), `OP_2SWAP` (0x72), `OP_IFDUP` (0x73), `OP_NIP` (0x77), `OP_OVER` (0x78), `OP_ROT` (0x7b) e `OP_TUCK` (0x7d).

## Comprendere gli Opcode Crittografici

Infine, una varietà di opcode supporta l'hashing e il controllo delle firme:

Hash:

* OP_RIPEMD160 (0xa6) — RIPEMD-160
* OP_SHA1	(0xa7)	— SHA-1
* OP_SHA256	(0xa8)	- SHA-256
* OP_HASH160	(0xa9)	— SHA-256 + RIPEMD-160
* OP_HASH256	(0xaa)	— SHA-256 + SHA-256

Controllo delle Firme:

* OP_CHECKSIG (0xac) — Controlla una firma
* OP_CHECKMULTISIG (0xae) — Controlla una multisig m-di-n

Vedi anche: `OP_CODESEPARATOR` (0xab), `OP_CHECKSIGVERIFY` (0xad) e `OP_CHECKMULTISIGVERIFY` (0xaf).

## Riepilogo: Utilizzo di Altri Comandi degli Script

Bitcoin Script include una vasta gamma di opcode aritmetici, di stack e crittografici. La maggior parte di questi opcode aggiuntivi probabilmente non è comune come quelli discussi nelle sezioni precedenti, ma sono comunque disponibili se sono esattamente ciò di cui hai bisogno per scrivere il tuo Script!

## Cosa Succede Dopo?

Avanza attraverso "Bitcoin Scripting" col [Capitolo 13: Progettare Scripts di Bitcoin reali](13_0_Progettare_Scripts_di_Bitcoin_reali.md).
