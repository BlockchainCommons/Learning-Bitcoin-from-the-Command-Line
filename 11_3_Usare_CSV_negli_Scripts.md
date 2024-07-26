# 11.3: Utilizzo di CSV negli Script

`nLockTime` e `OP_CHECKLOCKTIMEVERIFY` (o CLTV) sono solo una faccia dell'equazione del timelock. Dall'altra parte ci sono `nSequence` e `OP_CHECKSEQUENCEVERIFY`, che possono essere utilizzati per controllare i tempi relativi piuttosto che i tempi assoluti.

> :warning: **AVVISO VERSIONE:** CSV è diventato disponibile con Bitcoin Core 0.12.1, nella primavera del 2016.

## Comprendere nSequence

Ogni input in una transazione ha un valore `nSequence` (o se preferisci `sequence`). È stato uno strumento primario per le espansioni di Bitcoin come discusso in precedenza nel [Capitolo 5.2 Rinviare le Transazioni con RBF](05_2_Rinviare_le_Transazioni_con_RBF.md) e nel [Capitolo 8.1 Inviare una Transazione con Blocco temporale](08_1_Inviare_una_Transazione_con_Blocco_temporale.md), dove è stato utilizzato per segnalare RBF e `nLockTime`, rispettivamente. Tuttavia, c'è un altro uso per `nSequence`, descritto da [BIP 68](https://github.com/bitcoin/bips/blob/master/bip-0068.mediawiki): puoi usarlo per creare un timelock relativo su una transazione.

Un timelock relativo è un blocco che viene posto su un input specifico di una transazione e che viene calcolato in relazione alla data di mining dell'UTXO utilizzato nell'input. Ad esempio, se un UTXO è stato minato al blocco #468260 e una transazione è stata creata dove l'input per quell'UTXO è stato dato un `nSequence` di 100, allora la nuova transazione non potrebbe essere minata fino almeno al blocco #468360.

Facile!

> :information_source: **NOTA — SEQUENCE:** Questo è il terzo utilizzo del valore `nSequence` in Bitcoin. Qualsiasi valore `nSequence` senza il 32° bit impostato (1<<31), quindi da 0x00000001 a 0x7ffffffff, sarà interpretato come un timelock relativo se `nVersion ≥ 2` (che è il valore predefinito a partire da Bitcoin Core 0.14.0). Dovresti fare attenzione per garantire che i timelock relativi non confliggano con gli altri due usi di `nSequence`, per segnalare `nTimeLock` e RBF. `nTimeLock` di solito imposta un valore di 0xffffffff-1, dove un timelock relativo è disattivato; e RBF di solito imposta un valore di "1", dove un timelock relativo è irrilevante, perché definisce un timelock di 1 blocco.

> In generale, ricorda: con un valore `nVersion` di 2, un valore `nSequence` da 0x00000001 a 0x7fffffff consente timelock relativi, RBF e `nTimeLock`; un valore `nSequence` da 0x7fffffff a 0xffffffff-2 consente RBF e `nTimeLock`; un valore `nSequence` di 0xffffffff-1 consente solo `nTimeLock`; un valore `nSequence` di 0xffffffff non consente nessuno; e `nVersion` può essere impostato a 1 per disabilitare i timelock relativi per qualsiasi valore di `nSequence`. Uff!

### Creare un Tempo Relativo a Blocchi CSV

Il formato per utilizzare `nSequence` per rappresentare i timelock relativi è definito in [BIP 68](https://github.com/bitcoin/bips/blob/master/bip-0068.mediawiki) ed è leggermente più complesso del semplice inserimento di un numero, come hai fatto per `nTimeLock`. Invece, le specifiche BIP dividono il numero a quattro byte in tre parti:

* I primi due byte vengono utilizzati per specificare un locktime relativo.
* Il 23° bit viene utilizzato per segnalare positivamente se il lock si riferisce a un tempo anziché a un'altezza di blocco.
* Il 32° bit viene utilizzato per segnalare positivamente se i timelock relativi sono disattivati.

Detto ciò, la costruzione di un timelock relativo basato su blocchi è ancora abbastanza semplice, perché i due bit segnalati sono impostati a `0`, quindi imposti semplicemente `nSequence` a un valore tra 1 e 0xffff (65535). La nuova transazione può essere minata quel numero di blocchi dopo che l'UTXO associato è stato minato.

### Creare un Tempo Relativo CSV

Puoi invece impostare `nSequence` come un tempo relativo, dove il lock dura per 512 secondi moltiplicato per il valore di `nSequence`.

Per fare ciò:

1. Decidi quanto in avanti impostare il tuo timelock relativo.
2. Converti quel tempo in secondi.
3. Dividi per 512.
4. Arrotonda quel valore in su o in giù e impostalo come `nSequence`.
5. Imposta il 23° bit a true.

Per impostare un tempo 6 mesi nel futuro, devi prima calcolare come segue:
```
$ seconds=$((6*30*24*60*60))
$ nvalue=$(($seconds/512))
```
Poi, trasformalo in hex:
```
$ hexvalue=$(printf '%x\n' $nvalue)
```
Infine, esegui un'operazione bitwise-or sul 23° bit nel valore hex che hai creato:
```
$ relativevalue=$(printf '%x\n' $((0x$hexvalue | 0x400000)))
$ echo $relativevalue
4224679
```
Se converti nuovamente, vedrai che 4224679 = 10000000111011010100111. Il 23° bit è impostato a "1"; nel frattempo, i primi 2 byte, 0111011010100111, si convertono in 76A7 in hex o 30375 in decimale. Moltiplicalo per 512 e ottieni 15,55 milioni di secondi, che sono effettivamente 180 giorni.

## Creare una Transazione con un Timelock Relativo

Quindi vuoi creare una semplice transazione con un timelock relativo? Tutto ciò che devi fare è emettere una transazione dove il `nSequence` in un input è impostato come mostrato sopra: con il `nSequence` per quell'input impostato in modo che i primi due byte definiscano il timelock, il 23° bit definisca il tipo di timelock e il 32° bit sia impostato a falso.

Emetti la transazione e vedrai che non può essere legalmente minata fino a quando non saranno passati abbastanza blocchi o abbastanza tempo oltre il momento in cui l'UTXO è stato minato.

Tranne che praticamente nessuno lo fa. Le definizioni di [BIP 68](https://github.com/bitcoin/bips/blob/master/bip-0068.mediawiki) per `nSequence` sono state incorporate in Bitcoin Core allo stesso tempo di [BIP 112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki), che descrive l'opcode CSV, l'equivalente di `nSequence` dell'opcode CLTV. Proprio come CLTV, CSV offre capacità aumentate. Quindi, quasi tutto l'uso dei timelock relativi è stato con l'opcode CSV, non con il valore grezzo `nSequence` da solo.

|                      | Timelock Assoluto | Timelock Relativo |
|:--------------------:|-------------------|-------------------|
| **Blocco Transazione** | nTimeLock         | nSequence         |
| **Blocco Output**      | OP_CHECKLOCKTIMEVERIFY| OP_CHECKSEQUENCEVERIFY |

## Comprendere l'Opcode CSV

`OP_SEQUENCEVERIFY` negli Script Bitcoin funziona praticamente come `OP_LOCKTIMEVERIFY`.

Potresti richiedere che un UTXO sia mantenuto per cento blocchi dopo il suo mining:

```
100 OP_CHECKSEQUENCEVERIFY
```
Oppure potresti fare un calcolo più complesso per richiedere che un UTXO sia mantenuto per sei mesi, nel qual caso finirai con un numero più complesso:
```
4224679 OP_CHECKSEQUENCEVERIFY
```
In questo caso useremo una scorciatoia:
```
<+6Months> OP_CHECKSEQUENCEVERIFY
```
> :warning: **AVVISO:** Ricorda che un timelock relativo è un intervallo di tempo dal mining dell'UTXO utilizzato come input. Non è un intervallo di tempo dopo che crei la transazione. Se utilizzi un UTXO che è già stato confermato cento volte e applichi un timelock relativo di 100 blocchi, sarà idoneo per il mining immediatamente. I timelock relativi hanno alcuni usi molto specifici, ma probabilmente non si applicano se il tuo unico obiettivo è determinare un tempo specifico nel futuro.

### Comprendere Come Funziona Davvero CSV

CSV ha molte delle stesse sottigliezze nell'uso di CLTV:

* Il campo `nVersion` deve essere impostato a 2 o più.
* Il campo `nSequence` deve essere impostato a meno di 0x80000000.
* Quando CSV viene eseguito, ci deve essere un operando sullo stack che sia compreso tra 0 e 0xf0000000-1.
* Sia l'operando dello stack che `nSequence` devono avere lo stesso valore nel 23° bit.
* Il `nSequence` deve essere maggiore o uguale all'operando dello stack.

Proprio come con CLTV, quando stai spendendo nuovamente un UTXO con un CSV nelle sue condizioni di blocco, devi impostare `nSequence` per abilitare la transazione. Di solito lo imposterai al valore esatto nello script di blocco.

## Scrivere uno Script CSV

Proprio come `OP_CHECKLOCKTIMEVERIFY`, `OP_CHECKSEQUENCEVERIFY` include un implicito `OP_VERIFY` e lascia i suoi argomenti sullo stack, richiedendo un `OP_DROP` quando hai finito.

Uno script che bloccherebbe i fondi fino a sei mesi dopo il mining dell'input e che richiederebbe quindi una firma in stile P2PKH standard sarebbe il seguente:
```
<+6Months> OP_CHECKSEQUENCEVERIFY OP_DROP OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
```

#
### Codificare uno Script CSV

Quando codifichi uno script CSV, fai attenzione a come codifichi il valore intero per il timelock relativo. Dovrebbe essere passato come un intero a 3 byte, il che significa che stai ignorando il byte superiore, che potrebbe disattivare il timelock relativo. Poiché è un intero, assicurati di convertirlo in little-endian.

Questo può essere fatto con lo script `integer2lehex.sh` dalla capitolo precedente.

Per un tempo relativo di 100 blocchi:
```
$ ./integer2lehex.sh 100
Integer: 100
LE Hex: 64
Length: 1 bytes
Hexcode: 0164
```
Anche se dovrebbe essere completato a `000064`, richiedendo un codice di `03000064`.

Per un tempo relativo di 6 mesi:

```
$ ./integer2lehex.sh 4224679
Integer: 4224679
LE Hex: a77640
Length: 3 bytes
Hexcode: 03a77640
```


## Spendere un UTXO CSV

Per spendere un UTXO bloccato con uno script CSV, devi impostare il `nSequence` di quell'input a un valore maggiore del requisito nello script, ma inferiore al tempo tra l'UTXO e il blocco presente. Sì, questo significa che devi conoscere il requisito esatto nello script di blocco... ma hai una copia del `redeemScript`, quindi se non conosci i requisiti, lo deserializzi e poi imposti il `nSequence` al numero mostrato lì.

## Riepilogo: Utilizzo di CSV negli Script

`nSequence` e CSV offrono un'alternativa a `nLockTime` e CLTV dove blocchi una transazione basata su un tempo relativo da quando l'input è stato minato, anziché basare il blocco su un tempo stabilito nel futuro. Funzionano quasi in modo identico, a parte il fatto che il valore `nSequence` è codificato in modo leggermente diverso dal valore `nLockTime`, con bit specifici che significano cose specifiche.

> :fire: ***Qual è il potere di CSV?*** CSV non è solo un modo pigro per bloccare, quando non vuoi calcolare un tempo nel futuro. Invece, è un paradigma totalmente diverso, un blocco che useresti se fosse importante creare una durata minima specifica tra quando una transazione è minata e quando i suoi fondi possono essere spesi nuovamente. L'uso più ovvio è (ancora una volta) per un escrow, quando vuoi un tempo preciso tra l'input dei fondi e il loro output. Tuttavia, ha possibilità molto più potenti nelle transazioni off-chain, inclusi i canali di pagamento. Queste applicazioni sono per definizione basate su transazioni che non sono effettivamente inserite nella blockchain, il che significa che se sono successivamente inserite nella blockchain, un intervallo di tempo forzato può essere molto utile. [Contratti Hashed Timelock](https://en.bitcoin.it/wiki/Hashed_Timelock_Contracts) sono stati una di queste implementazioni, potenziando la rete di pagamenti Lightning. Sono discussi nel[Capitolo 13.3: Potenziare Bitcoin con Scripts](13_3_Potenziare_Bitcoin_con_Scripts.md).

## Cosa Succede Dopo?

Avanza attraverso "Bitcoin Scripting" col [Capitolo 12: Ampliando Scripts di Bitcoin](12_0_Ampliando_Scripts_di_Bitcoin.md).
