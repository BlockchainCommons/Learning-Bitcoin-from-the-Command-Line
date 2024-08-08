# 4.3 Creare una transazione raw con alias

A volte può essere scoraggiante capire l'ordine giusto per gli argomenti di un comando bitcoin-cli. Fortunatamente, puoi usare _Named Arguments_ come alternativa.

> :avviso: **AVVERTIMENTO VERSIONE:** Questa è un'innovazione di Bitcoin Core v 0.14.0. Se hai utilizzato i nostri script di installazione, questo è ciò che dovresti avere, ma ricontrolla la tua versione in caso di problemi. C'è anche un bug nell'uso degli argomenti con nome da parte del comando `createrawtransaction` che sarà presumibilmente corretto nella versione 0.14.1.

## Crea un alias

Per utilizzare un alias devi eseguire `bitcoin-cli` con l'argomento `-named`. Se prevedi di farlo regolarmente, probabilmente vorrai creare un alias:
```
alias bitcoin-cli="bitcoin-cli -named"
```
Come al solito, è per la tua facilità d'uso, ma continueremo a utilizzare tutti i comandi per mantenerne la chiarezza.

## Prova un alias

Per sapere quali sono i nomi degli argomenti di un comando, consulta `bitcoin-cli help`. Elencherà gli argomenti nel loro ordine corretto, ma ora fornirà anche i nomi per ciascuno di essi.

Ad esempio, `bitcoin-cli help getbalance` elenca questi argomenti:

   1. dummy [used to be account]
   2. minconf
   3. include_watchonly
   4. avoid_reuse

Quanto segue mostra un utilizzo tradizionale e non intuitivo di "getbalance" utilizzando l'argomento di conferma minima:
```
$ bitcoin-cli getbalance "*" 1
```
Con gli argomenti con nome è piu chiaro vedere cosa stai facendo, e cosi riduci al minimo gli errori:
```
$ bitcoin-cli -named getbalance minconf=1
```

## Prova una transazione raw

Ecco come apparirebbero i comandi per l'invio di una transazione grezza con argomenti denominati:

```
$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[0] | .txid') 
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
$ recipient="n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi"

$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.00001 }''')
$ bitcoin-cli -named decoderawtransaction hexstring=$rawtxhex 
{
  "txid": "2b59c31bc232c0399acee4c2a381b564b6fec295c21044fbcbb899ffa56c3da5",
  "hash": "2b59c31bc232c0399acee4c2a381b564b6fec295c21044fbcbb899ffa56c3da5",
  "version": 2,
  "size": 85,
  "vsize": 85,
  "weight": 340,
  "locktime": 0,
  "vin": [
    {
      "txid": "ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36",
      "vout": 0,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 0.00001000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 e7c1345fc8f87c68170b3aa798a956c2fe6a9eff OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi"
        ]
      }
    }
  ]
}

$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
e70dd2aa13422d12c222481c17ca21a57071f92ff86bdcffd7eaca71772ba172
```
Ecco! Hai inviato un'altra transazione grezza utilizzando alias `-named`, ma questa volta in modo piu chiaro e riducendo errori.

> :warning: **AVVERTIMENTO VERSIONE:** È qui che si manifesta il bug in Bitcoin Core 0.14: l'argomento `inputs` per `createrawtransaction` è chiamato erroneamente `transactions`. Quindi, se utilizzi Bitcoin Core 0.14.0, sostituisci l'argomento denominato `inputs` con `transazioni` per questo e gli esempi futuri. Tuttavia, a partire da Bitcoin Core 0.14.1, questo codice dovrebbe funzionare come mostrato.
> **UPDATE** Ormai siamo a luglio del 2024 e abbiamo la versione Bitcoin Core 27.0, funziona tutto.

## Riepilogo: creazione di una transazione grezza con alias

Eseguendo `bitcoin-cli` con il flag `-named`, puoi utilizzare alias anziché dipendere da argomenti ordinati. `bitcoin-cli help` qui vedrai sempre il nome giusto per ogni argomento. Ciò può comportare un codice più robusto, più facile da leggere e meno soggetto a errori.

_Questi documenti utilizzeranno alias per tutti gli esempi futuri, per chiarezza e per stabilire le migliori pratiche. Tuttavia, mostrerà anche tutti gli argomenti nell'ordine corretto. Quindi, se preferisci non utilizzare argomenti con nome, rimuovi semplicemente il flag '-named' e tutti i "name=" e gli esempi dovrebbero continuare a funzionare correttamente._

## Con cosa andiamo avanti?

Continua a inviare bitcoin, guarda come farlo con una transazione raw qui: [Capitolo 4.4 Inviare Monete con Transazione Grezza](04_4_Inviare_Monete_con_Transazione_Grezza.md).
