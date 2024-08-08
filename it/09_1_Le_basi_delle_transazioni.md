# 9.1: Comprendere le basi delle transazioni

Le basi di Bitcoin sono la capacità di proteggere le transazioni, cosa che viene fatta con un semplice linguaggio di scripting.

## Conoscere le parti del puzzle crittografico

Come descritto nel [Capitolo 1](01_0_Introduzione.md), i fondi in ogni transazione Bitcoin sono bloccati con un puzzle crittografico. Per essere precisi, abbiamo detto che Bitcoin è composto da "una sequenza di transazioni atomiche". Abbiamo notato che: "Ogni transazione è autenticata da un mittente con la soluzione a un precedente puzzle crittografico che è stato memorizzato come uno script. La nuova transazione è bloccata per il destinatario con un nuovo puzzle crittografico che è anche memorizzato come uno script". Questi script, che bloccano e sbloccano le transazioni, sono scritti in Bitcoin Script.

> :book: ***Cos'è Bitcoin Script?*** Bitcoin Script è un linguaggio simile a Forth basato su stack che evita volutamente i loop e quindi non è Turing-completo. È composto da singoli opcode. Ogni singola transazione in Bitcoin è bloccata con uno script Bitcoin; quando la transazione di blocco per un UTXO viene eseguita con gli input corretti, quell'UTXO può essere speso.

Il fatto che le transazioni siano bloccate con script significa che possono essere bloccate in vari modi diversi, richiedendo una varietà di chiavi diverse. Infatti, abbiamo incontrato diversi meccanismi di blocco fino ad oggi, ognuno dei quali utilizzava diversi opcode:

* OP_CHECKSIG, che controlla una chiave pubblica rispetto a una firma, è la base del classico indirizzo P2PKH, come verrà spiegato in dettaglio in [Capitolo 9.4: Programmare una P2PKH](09_4_Programmare_una_P2PKH.md).
* OP_CHECKMULTISIG controlla in modo simile i multisig, come verrà spiegato in dettaglio in [Capitolo 10.4: Programmare una Multifirma](10_4_Programmare_una_Multifirma.md).
* OP_CHECKLOCKTIMEVERIFY e OP_SEQUENCEEVERIFY costituiscono la base di Timelock più complessi, come verrà spiegato in dettaglio nel [Capitolo 11.2: Usare CLTV negli Scripts](11_2_Usare_CLTV_negli_Scripts.md).
* OP_RETURN è un segno di una transazione non spendibile, motivo per cui viene utilizzato per trasportare dati, come accennato in [Capitolo 8.2: Inviare una Transazione con Dati](08_2_Inviare_una_Transazione_con_Dati.md).

## Script di accesso nelle tue transazioni

Potresti non rendertene conto, ma hai già visto questi script di blocco e sblocco come parte delle transazioni raw con cui hai lavorato. Il modo migliore per esaminare questi script in modo più approfondito è quindi creare una transazione raw, quindi esaminarla.

### Crea una transazione di prova

Per esaminare gli script di sblocco e blocco, crea una rapida transazione raw prendendo un UTXO Legacy non speso e reinviandolo a un indirizzo di resto Legacy, meno una commissione di transazione:
```
$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[1] | .txid') 
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[1] | .vout')
$ recipient=$(bitcoin-cli -named getrawchangeaddress address_type=legacy)
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.0009 }''')
$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r '.hex')
```
In realtà non è necessario inviarlo: l'obiettivo è semplicemente produrre una transazione completa che puoi esaminare.

> **NOTA:** Perché indirizzi legacy? Perché i loro script sono più significativi. Tuttavia, offriremo anche un esempio di un SegWit P2WPKH nativo nel [Capitolo 9.5](09_5_Programmare_una_P2WPKH.md).

### Esamina la tua transazione di prova

Ora puoi esaminare la tua transazione in modo approfondito utilizzando `decoderawtransaction` su `$signedtx`:
```
$ bitcoin-cli -named decoderawtransaction hexstring=$signedtx
{
  "txid": "34151dac704d94a269cd33f80be34c122152edc9bfbb9323852966bf0ce937ed",
  "hash": "34151dac704d94a269cd33f80be34c122152edc9bfbb9323852966bf0ce937ed",
  "version": 2,
  "size": 191,
  "vsize": 191,
  "weight": 764,
  "locktime": 0,
  "vin": [
    {
      "txid": "bb4362dec15e67d366088f5493c789f22fb4a604e767dae1f6a631687e2784aa",
      "vout": 0,
      "scriptSig": {
        "asm": "304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b92cbab7d1022066f273178febc7a37568e2e9f4dec980a2e9a95441abe838c7ef64c39d85849c[ALL] 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b",
        "hex": "47304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b92cbab7d1022066f273178febc7a37568e2e9f4dec980a2e9a95441abe838c7ef64c39d85849c01210315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b"
      },
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 0.00090000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 06b5c6ba5330cdf738a2ce91152bfd0e71f9ec39 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a91406b5c6ba5330cdf738a2ce91152bfd0e71f9ec3988ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mg8S7F1gY3ivV9M9GrWwe6ziWvK2MFquCf"
        ]
      }
    }
  ]
}

```
I due script si trovano in due parti diverse della transazione.

Lo `scriptSig` si trova in `vin`. Questo è lo script di _sblocco_. È ciò che viene eseguito per accedere all'UTXO utilizzato per finanziare questa transazione. Ci sarà uno `scriptSig` per UTXO in una transazione.

Lo `scriptPubKey` si trova in `vout`. Questo è lo script di _blocco_. È ciò che blocca il nuovo output dalla transazione. Ci sarà uno `scriptPubKey` per output in una transazione.

> :book: ***Come interagiscono scriptSig e scriptPubKey?*** Lo `scriptSig` di una transazione sblocca l'UTXO precedente; l'output di questa nuova transazione verrà quindi bloccato con uno `scriptPubKey`, che a sua volta può essere sbloccato dallo `scriptSig` della transazione che riutilizza quell'UTXO.

### Leggi gli script nella tua transazione

Guarda i due script e vedrai che ognuno include due rappresentazioni diverse: `hex` è ciò che viene effettivamente memorizzato, ma il linguaggio assembly più leggibile (`asm`) può in un certo senso mostrarti cosa sta succedendo.

Dai un'occhiata a `asm` dello script di sblocco e avrai una prima occhiata a come appare Bitcoin Scripting:
```
04402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b92cbab7d1022066f273178febc7a37568e2e9f4dec980a2e9a95441abe838c7ef64c39d85849c[ALL] 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b
```
Come capita, quel pasticcio di numeri è una firma a chiave privata seguita dalla chiave pubblica associata. O almeno si spera che sia così, perché è ciò che serve per sbloccare l'UTXO P2PKH che questa transazione sta utilizzando.

Leggi lo script di blocco e vedrai che è molto più ovvio:
```
OP_DUP OP_HASH160 06b5c6ba5330cdf738a2ce91152bfd0e71f9ec39 OP_EQUALVERIFY OP_CHECKSIG
```
Questo è il metodo standard in Bitcoin Script per bloccare una transazione P2PKH.

Il [Capitolo 9.4](09_4_Programmare_una_P2PKH.md) spiegherà come questi due script vanno insieme, ma prima dovrai sapere come vengono valutati gli script Bitcoin.

## Esaminare una transazione diversa

Prima di abbandonare questa base, esamineremo un diverso tipo di script di blocco. Ecco `scriptPubKey` dalla transazione multisig che hai creato in [Capitolo 6.1: Inviare una Transazione a un Indirizzo Multifirma](06_1_Inviare_una_Transazione_a_un_Indirizzo_Multifirma.md).
```
      "scriptPubKey": {
        "asm": "OP_HASH160 a5d106eb8ee51b23cf60d8bd98bc285695f233f3 OP_EQUAL",
        "hex": "a914a5d106eb8ee51b23cf60d8bd98bc285695f233f387",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr"
        ]
      }
```

Confrontalo con lo `scriptPubKey` della tua nuova transazione P2PKH:
```
    "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 06b5c6ba5330cdf738a2ce91152bfd0e71f9ec39 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a91406b5c6ba5330cdf738a2ce91152bfd0e71f9ec3988ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mg8S7F1gY3ivV9M9GrWwe6ziWvK2MFquCf"
        ]
      }
```

Queste due transazioni sono _sicuramente_ bloccate in modi diversi. Bitcoin riconosce la prima come `scripthash` (P2SH) e la seconda come `pubkeyhash` (P2PKH), ma dovresti anche essere in grado di vedere la differenza nel diverso codice `asm`: `OP_HASH160 a5d106eb8ee51b23cf60d8bd98bc285695f233f3 OP_EQUAL` contro `OP_DUP OP_HASH160 06b5c6ba5330cdf738a2ce91152bfd0e71f9ec39 OP_EQUALVERIFY OP_CHECKSIG`. Questo è il potere dello scripting: può semplicemente produrre alcuni dei tipi di transazioni radicalmente diversi che hai imparato nei capitoli precedenti.

## Riepilogo: comprendere le basi delle transazioni

Ogni transazione Bitcoin include almeno uno script di sblocco (`scriptSig`), che risolve un precedente puzzle crittografico, e almeno uno script di blocco (`scriptPubKey`), che crea un nuovo puzzle crittografico. C'è uno `scriptSig` per input e uno `scriptPubKey` per output. Ognuno di questi script è scritto in Bitcoin Script, un linguaggio simile a Forth che potenzia ulteriormente Bitcoin.

> :fire: ***Qual è il potere degli script?*** Gli script sbloccano tutta la potenza degli Smart Contract. Con gli opcode appropriati, puoi prendere decisioni molto precise su chi può riscattare i fondi, quando possono riscattare i fondi e come possono riscattare i fondi. Regole più complesse per la spesa aziendale, la spesa di partnership, la spesa proxy e altre metodologie possono anche essere codificate all'interno di uno Script. Potenzia persino servizi Bitcoin più complessi come Lightning e sidechain.

## Cosa c'è dopo?

Continua "Introduzione agli script Bitcoin" col [Capitolo 9.2:Eseguire uno Script di Bitcoin](09_2_Eseguire_uno_Script_di_Bitcoin.md).
