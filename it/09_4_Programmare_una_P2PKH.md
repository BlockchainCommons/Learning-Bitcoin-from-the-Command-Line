# 9.4: Script di un P2PKH

Gli indirizzi P2PKH stanno rapidamente perdendo popolarità a causa dell'avvento di SegWit, ma rimangono comunque un ottimo elemento fondamentale per comprendere Bitcoin, e soprattutto per comprendere gli script Bitcoin. (Daremo una rapida occhiata a come gli script P2WPKH nativi di Segwit funzionano in modo diverso nella sezione successiva.)

## Comprendere lo script di sblocco

Da tempo sosteniamo che quando i fondi vengono inviati a un indirizzo Bitcoin, sono bloccati nella chiave privata associata a quell'indirizzo. Questo viene gestito tramite "scriptPubKey" di una transazione P2PKH, progettata in modo tale da richiedere al destinatario di avere la chiave privata associata all'indirizzo Bitcoin P2PKH. Per la precisione, il destinatario deve fornire sia la chiave pubblica collegata alla chiave privata, sia una firma generata dalla chiave privata.

Dai un'altra occhiata alla transazione che hai creato nel [Capitolo 9.1](09_1_Le_basi_delle_transazioni.md):
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
Puoi vedere che il suo script di sblocco `scriptSig` ha due valori. Questo è una `<signature>` (e un `[ALL]`) e una `<pubKey>`:
```
304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b92cbab7d1022066f273178febc7a37568e2e9f4dec980a2e9a95441abe838c7ef64c39d85849c[ALL] 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b
```
Questo è tutto, uno script di sblocco! (Per un P2PKH.)

## Comprendere lo script di blocco

Ricorda che ogni script di sblocco sblocca un UTXO precedente. Nell'esempio sopra, `vin` rivela che sta effettivamente sbloccando vout `0` di txid `bb4362dec15e67d366088f5493c789f22fb4a604e767dae1f6a631687e2784aa`.

Puoi esaminare l'UTXO con  `gettransaction`.
```
$ bitcoin-cli gettransaction "bb4362dec15e67d366088f5493c789f22fb4a604e767dae1f6a631687e2784aa"
{
  "amount": 0.00095000,
  "confirmations": 12,
  "blockhash": "0000000075a4c1519da5e671b15064734c42784eab723530a6ace83ca1e66d3f",
  "blockheight": 1780789,
  "blockindex": 132,
  "blocktime": 1594841768,
  "txid": "bb4362dec15e67d366088f5493c789f22fb4a604e767dae1f6a631687e2784aa",
  "walletconflicts": [
  ],
  "time": 1594841108,
  "timereceived": 1594841108,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "mmX7GUoXq2wVcbnrnFJrGKsGR14fXiGbD9",
      "category": "receive",
      "amount": 0.00095000,
      "label": "",
      "vout": 0
    }
  ],
  "hex": "020000000001011efcc3bf9950ac2ea08c53b43a0f8cc21e4b5564e205f996f7cadb7d13bb79470000000017160014c4ea10874ae77d957e170bd43f2ee828a8e3bc71feffffff0218730100000000001976a91441d83eaffbf80f82dee4c152de59a38ffd0b602188ac713b10000000000017a914b780fc2e945bea71b9ee2d8d2901f00914a25fbd8702473044022025ee4fd38e6865125f7c315406c0b3a8139d482e3be333727d38868baa656d3d02204b35d9b5812cb85894541da611d5cec14c374ae7a7b8ba14bb44495747b571530121033cae26cb3fa063c95e2c55a94bd04ab9cf173104555efe448b1bfc3a68c8f873342c1b00"
}
```
Ma come puoi vedere, non hai ottenuto "scriptPubKey" con "gettransaction". Devi fare un ulteriore passo per recuperarlo esaminando le informazioni grezze sulla transazione (che sono `hex`) con `decoderawtransaction`:
```
$ hex=$(bitcoin-cli gettransaction "bb4362dec15e67d366088f5493c789f22fb4a604e767dae1f6a631687e2784aa" | jq -r '.hex')
$ bitcoin-cli decoderawtransaction $hex
{
  "txid": "bb4362dec15e67d366088f5493c789f22fb4a604e767dae1f6a631687e2784aa",
  "hash": "6866490b16a92d68179e1cf04380fd08f16ec80bf66469af8d5e78ae624ff202",
  "version": 2,
  "size": 249,
  "vsize": 168,
  "weight": 669,
  "locktime": 1780788,
  "vin": [
    {
      "txid": "4779bb137ddbcaf796f905e264554b1ec28c0f3ab4538ca02eac5099bfc3fc1e",
      "vout": 0,
      "scriptSig": {
        "asm": "0014c4ea10874ae77d957e170bd43f2ee828a8e3bc71",
        "hex": "160014c4ea10874ae77d957e170bd43f2ee828a8e3bc71"
      },
      "txinwitness": [
        "3044022025ee4fd38e6865125f7c315406c0b3a8139d482e3be333727d38868baa656d3d02204b35d9b5812cb85894541da611d5cec14c374ae7a7b8ba14bb44495747b5715301",
        "033cae26cb3fa063c95e2c55a94bd04ab9cf173104555efe448b1bfc3a68c8f873"
      ],
      "sequence": 4294967294
    }
  ],
  "vout": [
    {
      "value": 0.00095000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 41d83eaffbf80f82dee4c152de59a38ffd0b6021 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a91441d83eaffbf80f82dee4c152de59a38ffd0b602188ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mmX7GUoXq2wVcbnrnFJrGKsGR14fXiGbD9"
        ]
      }
    },
    {
      "value": 0.01063793,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_HASH160 b780fc2e945bea71b9ee2d8d2901f00914a25fbd OP_EQUAL",
        "hex": "a914b780fc2e945bea71b9ee2d8d2901f00914a25fbd87",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2N9yWARt5E3TQsX2RjsauxSZaEZVhinAS4h"
        ]
      }
    }
  ]
}
```
Ora puoi guardare `vout` `0` e vedere che era bloccato con `scriptPubKey` di `OP_DUP OP_HASH160 41d83eaffbf80f82dee4c152de59a38ffd0b6021 OP_EQUALVERIFY OP_CHECKSIG`. Questa è la metodologia di blocco standard utilizzata per un vecchio indirizzo P2PKH con `<pubKeyHash>` bloccato nel mezzo.

Eseguirlo dimostrerà come funziona.

## Esegui uno script P2PKH

Quando sblocchi un P2PKH UTXO, concateni (effettivamente) gli script di sblocco e blocco. Per un indirizzo P2PKH, come nell'esempio utilizzato in questo capitolo, che produce:

```
Script: <signature> <pubKey> OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
```
Detto questo, puoi esaminare come viene sbloccato un P2PKH UTXO.

Innanzitutto, inserisci le costanti iniziali nello stack, quindi crea un duplicato di pubKey con `OP_DUP`:
```
Script: <signature> <pubKey> OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Stack: [ ]

Script: <pubKey> OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Stack: [ <signature> ]

Script: OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Stack: [ <signature> <pubKey> ]

Script: OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Running: <pubKey> OP_DUP
Stack: [ <signature> <pubKey> <pubKey> ]
```
Perché il duplicato? Perché bisogna controllare i due elementi di sblocco: la chiave pubblica e la firma.

Successivamente, `OP_HASH160` estrae `<pubKey>` dallo stack, ne esegue l'hashing e reinserisce il risultato nello stack.
```
Script: <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Running: <pubKey> OP_HASH160
Stack: [ <signature> <pubKey> <pubKeyHash> ]
```
Quindi, inserisci nello stack il valore `<pubKeyHash>` che si trovava nello script di blocco:
```
Script: OP_EQUALVERIFY OP_CHECKSIG
Stack: [ <signature> <pubKey> <pubKeyHash> <pubKeyHash> ]
```
`OP_EQUALVERIFY` è effettivamente composto da due codici operativi: `OP_EQUAL`, che estrae due elementi dallo stack e inserisce `True` o `False` in base al confronto; e `OP_VERIFY` che visualizza il risultato e contrassegna immediatamente la transazione come non valida se è `False`. (Il Capitolo 12 parla più approfonditamente dell'uso di `OP_VERIFY` come condizionale.)

Supponendo che i due `<pubKeyHash>es` siano uguali, otterrai il seguente risultato:
```
Script: OP_CHECKSIG
Running: <pubKeyHash> <pubKeyHash> OP_EQUALVERIFY
Stack: [ <signature> <pubKey> ]
```
A questo punto hai dimostrato che il `<pubKey>` fornito in `scriptSig` ha l'hash dell'indirizzo Bitcoin in questione, quindi sai che il redeemer conosceva la chiave pubblica. Ma devono anche dimostrare di conoscere la chiave privata, cosa che viene fatta con `OP_CHECKSIG`, che conferma che la firma dello script di sblocco corrisponde a quella chiave pubblica.
```
Script:
Running: <signature> <pubKey> OP_CHECKSIG
Stack: [ True ]
```
Lo Script ora termina e, se ha avuto successo, la transazione può spendere nuovamente l'UTXO in questione.

### Usa btcdeb per un esempio P2PKH

Testare le effettive transazioni Bitcoin con "btcdeb" è un po' più complicato, perché è necessario conoscere la chiave pubblica e una firma per far funzionare tutto, e generare quest'ultima è piuttosto difficile. Tuttavia, un modo per testare le cose è lasciare che Bitcoin faccia il lavoro per te nel generare una transazione che _sbloccherebbe_ un UTXO. Questo è quello che hai fatto sopra: la generazione della transazione per spendere l'UTXO ha fatto sì che `bitcoin-cli` calcolasse `<signature>` e `<pubKey>`. Quindi esamini le informazioni grezze sulla transazione dell'UTXO per apprendere lo script di blocco incluso `<pubKeyHash>`

Puoi mettere insieme lo script di blocco, la firma e la pubkey utilizzando `btcdeb`, mostrando quanto sia semplice uno script P2PKH.
```
$ btcdeb '[304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b92cbab7d1022066f273178febc7a37568e2e9f4dec980a2e9a95441abe838c7ef64c39d85849c 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b OP_DUP OP_HASH160 41d83eaffbf80f82dee4c152de59a38ffd0b6021 OP_EQUALVERIFY OP_CHECKSIG]'
btcdeb 0.2.19 -- type `btcdeb -h` for start up options
unknown key ID 41d83eaffbf80f82dee4c152de59a38ffd0b6021: returning fake key
valid script
7 op script loaded. type `help` for usage information
script                                                             |                                                             stack 
-------------------------------------------------------------------+-------------------------------------------------------------------
304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b... | 
0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b | 
OP_DUP                                                             |
OP_HASH160                                                         |
41d83eaffbf80f82dee4c152de59a38ffd0b6021                           | 
OP_EQUALVERIFY                                                     | 
OP_CHECKSIG                                                        | 
                                                                   | 
                                                                   | 
#0000 304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b92cbab7d1022066f273178febc7a37568e2e9f4dec980a2e9a95441abe838c7ef64c39d85849c
```
Spingi la `<signature>` e la `<pubKey>` nello stack:
```
btcdeb> step
		<> PUSH stack 304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b92cbab7d1022066f273178febc7a37568e2e9f4dec980a2e9a95441abe838c7ef64c39d85849c
script                                                             |                                                             stack 
-------------------------------------------------------------------+-------------------------------------------------------------------
0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b | 304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b...
OP_DUP                                                             | 
OP_HASH160                                                         | 
41d83eaffbf80f82dee4c152de59a38ffd0b6021                           |  
OP_EQUALVERIFY                                                     |  
OP_CHECKSIG                                                        |     
                                                                   |   
                                                                   | 
                                                                   | 
                                                                   | 
                                                                   | 
#0001 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b
btcdeb> step
		<> PUSH stack 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b
script                                                             |                                                             stack 
-------------------------------------------------------------------+-------------------------------------------------------------------
OP_DUP                                                             | 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b
OP_HASH160                                                         | 304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b...
41d83eaffbf80f82dee4c152de59a38ffd0b6021                           | 
OP_EQUALVERIFY                                                     |
OP_CHECKSIG                                                        |  
                                                                   |   
                                                                   |    
                                                                   | 
                                                                   |
                                                                   | 
                                                                   | 
                                                               | 
```
Il tuo `OP_DUP` e `OP_HASH` la `<pubKey>`:
```
#0002 OP_DUP
btcdeb> step
		<> PUSH stack 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b
script                                                             |                                                             stack 
-------------------------------------------------------------------+-------------------------------------------------------------------
OP_HASH160                                                         | 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b
41d83eaffbf80f82dee4c152de59a38ffd0b6021                           | 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b
OP_EQUALVERIFY                                                     | 304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b...
OP_CHECKSIG                                                        | 
                                                                   | 
                                                                   |   
                                                                   |  
                                                                   |   
                                                                   |   
                                                                   | 
                                                                   | 
                                                                   |
                                                                   | 
#0003 OP_HASH160
btcdeb> step
		<> POP  stack
		<> PUSH stack 41d83eaffbf80f82dee4c152de59a38ffd0b6021
script                                                             |                                                             stack 
-------------------------------------------------------------------+-------------------------------------------------------------------
41d83eaffbf80f82dee4c152de59a38ffd0b6021                           |                           41d83eaffbf80f82dee4c152de59a38ffd0b6021
OP_EQUALVERIFY                                                     | 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b
OP_CHECKSIG                                                        | 304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b...
                                                                   | 
                                                                   | 
                                                                   |   
                                                                   |    
                                                                   |    
                                                                   |     
                                                                   | 
                                                                   | 
                                                                   | 
                                                                   | 
```
Spingi la `<pubKeyHash>` dallo script di blocco nella pila e verifichi:
```
#0004 41d83eaffbf80f82dee4c152de59a38ffd0b6021
btcdeb> step
		<> PUSH stack 41d83eaffbf80f82dee4c152de59a38ffd0b6021
script                                                             |                                                             stack 
-------------------------------------------------------------------+-------------------------------------------------------------------
OP_EQUALVERIFY                                                     |                           41d83eaffbf80f82dee4c152de59a38ffd0b6021
OP_CHECKSIG                                                        |                           41d83eaffbf80f82dee4c152de59a38ffd0b6021
                                                                   | 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b
                                                                   | 304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b...
                                                                   | 
                                                                   | 
                                                                   |  
                                                                   |  
                                                                   |  
                                                                   |       
                                                                   |   
                                                                   | 
                                                                   |
                                                                   | 
#0005 OP_EQUALVERIFY
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 01
		<> POP  stack
script                                                             |                                                             stack 
-------------------------------------------------------------------+-------------------------------------------------------------------
OP_CHECKSIG                                                        | 0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b
                                                                   | 304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c9dfba68b...
                                                                   | 
                                                                   | and_v(
                                                                   |     sig(304402201cc39005b076cb06534cd084fcc522e7bf937c4c9654c1c...
                                                                   |     and_v(
                                                                   |         pk(0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3...
                                                                   |         c:pk_h(030500000000000000000000000000000000000000000000...
                                                                   |     )
                                                                   | 
                                                                   | )
                                                                   | 
			
```
A questo punto, serve soltanto il `OP_CHECKSIG`:
```
#0006 OP_CHECKSIG
btcdeb> step
error: Signature is found in scriptCode
```
(Sfortunatamente questo controllo potrebbe funzionare o meno in qualsiasi momento a causa dei capricci del Bitcoin Core e del codice `btcdeb`.)

Come si vede, un P2PKH è abbastanza semplice: la sua protezione avviene attraverso la forza della sua crittografia.

### Come cercare manualmente la PubKey e la firma

E se volessi generare le informazioni `<signature>` e `<PubKey>` necessarie per sbloccare tu stesso un UTXO, senza appoggiarti a "bitcoin-cli" per creare una transazione?

Si scopre che è abbastanza semplice ottenere un `<pubKey>`. Devi solo usare `getaddressinfo` per esaminare l'indirizzo in cui si trova attualmente l'UTXO:
```
$ bitcoin-cli getaddressinfo mmX7GUoXq2wVcbnrnFJrGKsGR14fXiGbD9
{
  "address": "mmX7GUoXq2wVcbnrnFJrGKsGR14fXiGbD9",
  "scriptPubKey": "76a91441d83eaffbf80f82dee4c152de59a38ffd0b602188ac",
  "ismine": true,
  "solvable": true,
  "desc": "pkh([f004311c/0'/0'/2']0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b)#t3g5mjk9",
  "iswatchonly": false,
  "isscript": false,
  "iswitness": false,
  "pubkey": "0315a0aeb37634a71ede72d903acae4c6efa77f3423dcbcd6de3e13d9fd989438b",
  "iscompressed": true,
  "ischange": false,
  "timestamp": 1594835792,
  "hdkeypath": "m/0'/0'/2'",
  "hdseedid": "f058372260f71fea37f7ecab9e4c5dc25dc11eac",
  "hdmasterfingerprint": "f004311c",
  "labels": [
    ""
  ]
}
```
Per capire quella firma, tuttavia, è necessario comprendere veramente i dettagli di come vengono create le transazioni Bitcoin. Quindi lo lasciamo come uno studio avanzato per il lettore: creare una transazione `bitcoin-cli` per `risolvere` un UTXO è la soluzione migliore per il momento.

## Riepilogo: Programmare Pay to Public Key Hash

L'invio a un indirizzo P2PKH era relativamente semplice quando utilizzavi solo `bitcoin-cli`. L'esame dello script Bitcoin sottostante mette a nudo le funzioni crittografiche implicite nel finanziamento di quella transazione: come l'UTXO è stato sbloccato con una firma e una chiave pubblica.

## Qual è il prossimo argomento?

Continua l' "Introduzione agli script Bitcoin" con [Capitolo 9.5: Scripting a P2WPKH](09_5_Scripting_a_P2WPKH.md).
