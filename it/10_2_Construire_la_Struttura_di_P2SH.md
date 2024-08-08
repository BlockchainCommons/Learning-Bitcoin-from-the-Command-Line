# 10.2: Costruire la Struttura di P2SH

Nella sezione precedente abbiamo presentato la teoria su come creare transazioni P2SH per contenere Script Bitcoin. La pratica effettiva è _molto più difficile_, ma per completezza, la esamineremo qui. Probabilmente non è qualcosa che faresti mai senza un'API, quindi se diventa troppo intimidatorio, sappi che torneremo a Script di alto livello tra un momento.

## Creare uno Script di Blocco

Qualsiasi transazione P2SH inizia con uno script di blocco. Questo è l'argomento dei capitoli 9 e 11-12. Puoi usare uno qualsiasi dei metodi di Bitcoin Script descritti in quei capitoli per creare qualsiasi tipo di script di blocco, purché il `redeemScript` serializzato risultante sia di 520 byte o meno.

> :book: ***Perché gli script P2SH sono limitati a 520 byte?*** Come per molte cose in Bitcoin, la risposta è la retrocompatibilità: la nuova funzionalità deve essere costantemente costruita entro i vecchi vincoli del sistema. In questo caso, 520 byte è il massimo che può essere spinto nello stack in una volta. Poiché l'intero redeemScript viene spinto nello stack come parte del processo di riscatto, raggiunge quel limite.

## Serializzare uno Script di Blocco nel Modo Difficile

Dopo aver creato uno script di blocco, devi serializzarlo prima che possa essere inserito in Bitcoin. Questo è un processo in due parti. Prima devi trasformarlo in codice esadecimale, poi devi trasformare quel codice esadecimale in binario.

### Creare il Codice Esadecimale

Creare il codice esadecimale necessario per serializzare uno script è sia una semplice traduzione sia qualcosa di abbastanza complesso da andare oltre qualsiasi script shell che probabilmente scriverai. Questo passaggio è uno dei motivi principali per cui hai bisogno di un'API per creare transazioni P2SH.

Crei il codice esadecimale passando attraverso il tuo script di blocco e trasformando ciascun elemento in un comando esadecimale di un byte, eventualmente seguito da dati aggiuntivi, secondo la guida alla [pagina del Bitcoin Wiki Script](https://en.bitcoin.it/wiki/Script):

* Gli operatori sono tradotti nel byte corrispondente per quell'opcode
* Le costanti 1-16 sono tradotte negli opcode 0x51 a 0x61 (OP_1 a OP_16)
* La costante -1 è tradotta nell'opcode 0x4f (OP_1NEGATE)
* Altre costanti sono precedute dagli opcode 0x01 a 0x4e (OP_PUSHDATA, con il numero che specifica quanti byte spingere)
   * Gli interi sono tradotti in esadecimale usando la notazione a segno e magnitudine a little-endian

### Tradurre gli Interi

Gli interi sono la parte più problematica della traduzione di uno script di blocco.

Per prima cosa, dovresti verificare che il tuo numero rientri tra -2147483647 e 2147483647, l'intervallo degli interi a quattro byte quando il byte più significativo è usato per la firma.

In secondo luogo, devi tradurre il valore decimale in esadecimale e riempirlo fino a ottenere un numero pari di cifre. Questo può essere fatto con il comando `printf`:

```
$ integer=1546288031
$ hex=$(printf '%08x\n' $integer | sed 's/^\(00\)*//')
$ echo $hex
5c2a7b9f
```
In terzo luogo, devi aggiungere un byte aggiuntivo di `00` se la cifra superiore è "8" o maggiore, in modo che il numero non sia interpretato come negativo.

```
$ hexfirst=$(echo $hex | cut -c1)
$ [[ 0x$hexfirst -gt 0x7 ]] && hex="00"$hex
```
In quarto luogo, devi tradurre l'esadecimale da big-endian (byte meno significativo per ultimo) a little-endian (byte meno significativo per primo). Puoi fare questo con il comando `tac`:

```
$ lehex=$(echo $hex | tac -rs .. | echo "$(tr -d '\n')")
$ echo $lehex
9f7b2a5c
```
Inoltre, devi sempre conoscere la dimensione di qualsiasi dato che metti nello stack, in modo da poterlo precedere con l'opcode corretto. Puoi semplicemente ricordare che ogni due caratteri esadecimali è un byte. Oppure, puoi usare `echo -n` pipato a `wc -c`, e dividere per due:

```
$ echo -n $lehex | wc -c | awk '{print $1/2}'
4
```
Con tutta questa trafila, sapresti che potresti tradurre l'intero 1546288031 in un opcode `04` (per spingere quattro byte nello stack) seguito da `9f7b2a5c` (la rappresentazione esadecimale a little-endian di 1546288031).

Se invece avessi un numero negativo, dovresti (1) fare i tuoi calcoli sul valore assoluto del numero, poi (2) fare un or bit a 0x80 sul tuo risultato finale a little-endian. Ad esempio, `9f7b2a5c`, che è 1546288031, diventerebbe `9f7b2adc`, che è -1546288031:

```
$ neglehex=$(printf '%x\n' $((0x$lehex | 0x80)))
$ echo $neglehex
9f7b2adc
```
### Trasformare l'Esadecimale in Binario

Per completare la tua serializzazione, devi tradurre il codice esadecimale in binario. Da riga di comando, questo richiede solo una semplice invocazione di `xxd -r -p`. Tuttavia, probabilmente vorrai farlo come parte di una singola pipe che hasherà anche lo script...

## Eseguire lo Script di Conversione degli Interi

Uno script completo per cambiare un intero tra -2147483647 e 2147483647 in una rappresentazione a segno e magnitudine a little-endian in esadecimale può essere trovato nella [directory del codice sorgente](src/10_2_integer2lehex.sh). Puoi scaricarlo come `integer2lehex.sh`.

> :warning: **AVVERTENZA:** Questo script non è stato controllato in modo robusto. Se hai intenzione di usarlo per creare veri script di blocco dovresti assicurarti di ricontrollare e testare i tuoi risultati.

Assicurati che i permessi sullo script siano corretti:

```
$ chmod 755 integer2lehex.sh
```
Puoi quindi eseguire lo script come segue:
```
$ ./integer2lehex.sh 1546288031
Integer: 1546288031
LE Hex: 9f7b2a5c
Length: 4 bytes
Hexcode: 049f7b2a5c

$ ./integer2lehex.sh -1546288031
Integer: -1546288031
LE Hex: 9f7b2adc
Length: 4 bytes
Hexcode: 049f7b2adc
```


## Analizzare un Multisig P2SH

Per capire meglio questo processo, esamineremo il multisig P2SH che abbiamo creato in [Capitolo 6.1: Inviare una Transazione a un IndirizzoMultifirma](06_1_Inviare_una_Transazione_a_un_Indirizzo_Multifirma.md). Dai un'occhiata al `redeemScript` che hai usato, che ora sai essere la versione esadecimale serializzata dello script di blocco:

```
522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae
```
Puoi tradurlo di nuovo in Script manualmente usando la [pagina del Bitcoin Wiki Script](https://en.bitcoin.it/wiki/Script) come riferimento. Basta guardare un byte (due caratteri esadecimali) di dati alla volta, a meno che non ti venga detto di guardare di più da un comando OP_PUSHDATA (un opcode nell'intervallo da 0x01 a 0x4e).

L'intero script si scomporrà come segue:

```
52 / 21 / 02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191 / 21 / 02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3 / 52 / ae
```
Ecco cosa significano le singole parti:

* 0x52 = OP_2
* 0x21 = OP_PUSHDATA 33 bytes (hex: 0x21)
* 0x02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191 = i prossimi 33 byte (public-key hash)
* 0x21 = OP_PUSHDATA 33 bytes (hex: 0x21)
* 0x02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3 = i prossimi 33 byte (public-key hash)
* 0x52 = OP_2
* 0xae = OP_CHECKMULTISIG

In altre parole, quel `redeemScript` era una traduzione di `2 02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191 02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3 2 OP_CHECKMULTISIG`. Torneremo su questo script nel [Capitolo 10.4:Scripting a Multisig](10_4_Scripting_a_Multisig.md) quando dettaglieremo esattamente come funzionano i multisig all'interno del paradigma P2SH.

Se desideri un aiuto meccanico con questo tipo di traduzione in futuro, puoi usare `bitcoin-cli decodescript`:

```
$ bitcoin-cli -named decodescript hexstring=522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae
{
  "asm": "2 02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191 02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3 2 OP_CHECKMULTISIG",
  "reqSigs": 2,
  "type": "multisig",
  "addresses": [
    "mmC2x2FoYwBnVHMPRUAzPYg6WDA31F1ot2",
    "mhwZFJUnWqTqy4Y7pXVum88qFtUnVG1keM"
  ],
  "p2sh": "2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr",
  "segwit": {
    "asm": "0 6fe9f451ccedb8e4090b822dcad973d0388a37b4c89fd1aed485110adecab2a9",
    "hex": "00206fe9f451ccedb8e4090b822dcad973d0388a37b4c89fd1aed485110adecab2a9",
    "reqSigs": 1,
    "type": "witness_v0_scripthash",
    "addresses": [
      "tb1qdl5lg5wvakuwgzgtsgku4ktn6qug5da5ez0artk5s5gs4hk2k25szvjky9"
    ],
    "p2sh-segwit": "2NByn92W1vH5oQC1daY69F5sU7PEStKKQBR"
  }
}
```
È particolarmente utile per controllare il tuo lavoro quando stai serializzando.

## Serializzare uno Script di Blocco nel Modo Facile

Quando hai installato `btcdeb` nel [Capitolo 9.3](09_3_Provare_uno_Script_Bitcoin.md) hai anche installato `btcc` che può essere usato per serializzare gli script Bitcoin:

```
$ btcc 2 02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191 02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3 2 OP_CHECKMULTISIG
warning: ambiguous input 2 is interpreted as a numeric value; use OP_2 to force into opcode
warning: ambiguous input 2 is interpreted as a numeric value; use OP_2 to force into opcode
522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae
```
È molto più facile che capirlo a mano!

Considera anche il [Transaction Script Compiler](https://github.com/Kefkius/txsc) di Python, che traduce avanti e indietro.

## Hashare uno Script Serializzato

Dopo aver creato uno script di blocco e serializzato, il terzo passaggio nella creazione di una transazione P2SH è hashare lo script di blocco. Come notato in precedenza, un hash OP_HASH160 di 20 byte è creato attraverso una combinazione di un hash SHA-256 e un hash RIPEMD-160. Hashare uno script serializzato richiede quindi due comandi: `openssl dgst -sha256 -binary` esegue l'hash SHA-256 e produce un binario da inviare tramite pipe, poi `openssl dgst -rmd160` prende quel flusso binario, esegue un hash RIPEMD-160 e infine produce un codice esadecimale leggibile.

Ecco l'intera pipe, inclusa la precedente trasformazione dello script serializzato esadecimale in binario:

```
$ redeemScript="522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae"
$ echo -n $redeemScript | xxd -r -p | openssl dgst -sha256 -binary | openssl dgst -rmd160
(stdin)= a5d106eb8ee51b23cf60d8bd98bc285695f233f3
```
## Creare una Transazione P2SH

Creare il tuo hash di 20 byte ti dà solo l'hash al centro di uno script di blocco P2SH. Devi ancora metterlo insieme agli altri opcode che creano una transazione P2SH standard: `OP_HASH160 a5d106eb8ee51b23cf60d8bd98bc285695f233f3 OP_EQUAL`.

A seconda della tua API, potresti essere in grado di inserire questo come un `scriptPubKey` in stile `asm` per la tua transazione, oppure potresti doverlo tradurre anche in codice `hex`. Se devi tradurre, usa gli stessi metodi descritti sopra per "Creare il Codice Esadecimale" (oppure usa `btcc`), risultando in `a914a5d106eb8ee51b23cf60d8bd98bc285695f233f387`.

Nota che il `scriptPubKey` esadecimale per una transazione Script P2SH inizierà _sempre_ con un `a914`, che è l'`OP_HASH160` seguito da un `OP_PUSHDATA` di 20 byte (hex: `0x14`); e terminerà _sempre_ con un `87`, che è un `OP_EQUAL`. Quindi tutto ciò che devi fare è mettere il tuo redeem script hashato tra quei numeri.

## Riepilogo: Costruire la Struttura di P2SH

Creare effettivamente lo script di blocco P2SH ti porta più a fondo nel cuore di Bitcoin di quanto tu abbia mai fatto prima. Sebbene sia utile sapere come funziona tutto questo a un livello molto basso, è molto probabile che avrai un'API che si occuperà di tutto il lavoro pesante per te. Il tuo compito sarà semplicemente creare il Bitcoin Script per fare il blocco... che è l'argomento principale dei capitoli 9 e 11-12.

## Cosa c'è Dopo?

Continua "Incorporare Bitcoin Scripts" con [Capitolo 10.3: Eseguire uno Script Bitcoin con P2SH](10_3_Eseguire_uno_Script_Bitcoin_con_P2SH.md).


