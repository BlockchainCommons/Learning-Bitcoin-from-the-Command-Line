# 11.1: Comprendere le Opzioni di Timelock

Nel [Capitolo 8.1: Inviare una Transazione con Blocco temporale](08_1_Inviare_una_Transazione_con_Blocco_temporale.md), `nLocktime` ha offerto una grande prima opzione per bloccare le transazioni in modo che non potessero essere spese fino a un certo punto nel futuro — basato sia sul tempo che sull'altezza del blocco. Ma, non è l'unico modo per mettere un timelock su una transazione.

## Comprendere i Limiti di nLockTime

`nLockTime` è un modo semplice e potente per bloccare una transazione, ma ha alcuni limiti:

1. **Nessuna Divisione.** `nLocktime` blocca l'intera transazione.
2. **Nessun Networking.** La maggior parte dei nodi moderni non accetteranno un `nLockTime` nella mempool fino a quando non è quasi pronto per essere finalizzato.
3. **Nessuno Script.** L'uso originale e semplice di `nLockTime` non permetteva che fosse usato negli Script.
4. **Nessuna Protezione.** `nLockTime` permette ai fondi di essere spesi con una transazione diversa, non bloccata.

L'ultimo punto è stato spesso il punto di rottura per `nLockTime`. Impediva che una transazione venisse spesa, ma non impediva che i fondi fossero usati in una transazione diversa. Quindi, aveva usi, ma tutti dipendevano dalla fiducia.

## Comprendere le Possibilità degli Script Timelock

Negli anni più recenti, Bitcoin Core si è espanso per permettere la manipolazione dei timelock a livello di opcode con _OP_CHECKLOCKTIMEVERIFY_ (CLTV) e _OP_CHECKSEQUENCEVERIFY_ (CSV). Entrambi funzionano con una nuova metodologia che potenzia ulteriormente Bitcoin.

_Sono Opcode._ Poiché sono opcode, CLTV e CSV possono essere usati come parte di condizioni di riscatto più complesse. Molto spesso sono collegati con le condizioni descritte nel prossimo capitolo.

_Bloccano gli Output._ Poiché sono opcode che sono inclusi nelle transazioni come parte di un `sigPubKey`, bloccano solo quell'output singolo. Ciò significa che le transazioni sono accettate sulla rete Bitcoin e che gli UTXO usati per finanziare quelle transazioni sono spesi. Non si può tornare indietro su una transazione bloccata con CLTV o CSV come si può fare con un semplice `nLockTime`. Spendere nuovamente l'UTXO risultante richiede quindi che le condizioni del timelock siano soddisfatte.

Ecco un'avvertenza per l'uso dei timelock: _Sono blocchi unidirezionali._ I timelock sono progettati in modo da sbloccare i fondi in un certo momento. Non possono quindi ri-bloccare un fondo: una volta che un fondo bloccato nel tempo è disponibile per essere speso, rimane disponibile per essere speso.

### Comprendere le Possibilità di CLTV

_OP_CHECKLOCKTIMEVERIFY_ o CLTV è una corrispondenza per la caratteristica classica `nLockTime`, ma nel nuovo paradigma basato su opcode. Permette a un UTXO di diventare accessibile in un certo momento o a una certa altezza di blocco.

CLTV è stato dettagliato per la prima volta in [BIP 65](https://github.com/bitcoin/bips/blob/master/bip-0065.mediawiki).

### Comprendere le Possibilità di CSV

_OP_CHECKSEQUENCEVERIFY_ o CSV dipende da un nuovo tipo di "relative locktime", che è impostato nel campo _nSequence_ della transazione. Come di consueto, può essere impostato come tempo o come altezza del blocco. Se è impostato come tempo, "n", allora una transazione bloccata nel tempo relativa è spendibile "n x 512" secondi dopo che il suo UTXO è stato minato, e se è impostato come blocco, "n", allora una transazione bloccata nel tempo relativa è spendibile "n" blocchi dopo che il suo UTXO è stato minato.

L'uso di `nSequence` per un timelock relativo è stato dettagliato per la prima volta in [BIP 68](https://github.com/bitcoin/bips/blob/master/bip-0068.mediawiki), poi l'opcode CSV è stato aggiunto in [BIP 112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki).

## Riepilogo: Comprendere le Opzioni di Timelock

Ora hai quattro opzioni per Timelock:

* `nLockTime` per tenere una transazione fuori dalla blockchain fino a un momento specifico.
* `nSequence` per tenere una transazione fuori dalla blockchain fino a un tempo relativo.
* CLTV per rendere un UTXO non spendibile fino a un momento specifico.
* CSV per rendere un UTXO non spendibile fino a un tempo relativo.

## Cosa Succede Dopo?

Continua a "Potenziare Timelock" nel [Capitolo 11.2: Usare CLTV negli Scripts](11_2_Usare_CLTV_negli_Scripts.md).
