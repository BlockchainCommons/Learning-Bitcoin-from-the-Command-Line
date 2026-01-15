# 8.2: Inviare una transazione con dati

L'ultimo modo per variare la modalità di invio di una transazione di base è utilizzare la transazione per inviare dati anziché fondi (o, in realtà, in aggiunta di fondi). Questo ti dà la possibilità di incorporare informazioni nella blockchain. Viene eseguito tramite uno speciale comando `OP_RETURN`.

Il limite? Puoi memorizzare solo 80 byte alla volta!

## Crea i tuoi dati

La prima cosa che devi fare è creare gli 80 byte (o meno) di dati che registrerai nel tuo `OP_RETURN`. Potrebbe essere semplice come preparare un messaggio oppure potresti eseguire l'hashing dei dati esistenti. Ad esempio, "sha256sum" produce 256 bit di dati, ovvero 32 byte, ben al di sotto dei limiti:
```
$ sha256sum contratto.jpg
b9f81a8919e5aba39aeb86145c684010e6e559b580a85003ae25d78237a12e75 contratto.jpg
$ op_return_data="b9f81a8919e5aba39aeb86145c684010e6e559b580a85003ae25d78237a12e75"
```
> :book: _Che cos'è un OP_RETURN?_ Tutte le transazioni Bitcoin sono basate su script opcode che incontreremo nel prossimo capitolo. `OP_RETURN` è un semplice codice operativo che definisce un OUTPUT come non valido. La convenzione ha portato al suo utilizzo per incorporare dati nella blockchain.

## Prepara dei soldi

Il tuo scopo nel creare una transazione di dati non è inviare denaro a nessuno, ma inserire i dati nella blockchain. Tuttavia, devi_ inviare denaro per farlo. Devi solo utilizzare un indirizzo di resto come _unico_ destinatario. Quindi puoi identificare un UTXO e inviarlo al tuo indirizzo di modifica, meno una commissione di transazione, utilizzando anche la stessa transazione per creare un OP_RETURN.

Ecco la configurazione standard:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "854a833b667049ac811b4cf1cad40fa7f8dce8b0f4c1018a58b84559b6e05f42",
    "vout": 0,
    "address": "tb1q6kgsjxuqwj3rwhkenpdfcjccalk06st9z0k0kh",
    "scriptPubKey": "0014d591091b8074a2375ed9985a9c4b18efecfd4165",
    "amount": 0.01463400,
    "confirmations": 1392,
    "spendable": true,
    "solvable": true,
    "desc": "wpkh([d6043800/0'/1'/12']02883bb5463e37d55252d8b3d5c2141b007b37c8a7db6211f75c955acc5ea325eb)#cjr03mru",
    "safe": true
  }
]

$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[0] | .txid')
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
$ changeaddress=$(bitcoin-cli getrawchangeaddress)
```

## Scrivi una transazione grezza

Ora puoi scrivere una nuova transazione grezza con due output: uno è il tuo indirizzo di resto per recuperare (la maggior parte) dei tuoi soldi, l'altro è un idata address, che è il termine `bitcoin-cli` per un OP_RETURN.
```
rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "data": "'$op_return_data'", "'$changeaddress'": 0.0146 }''')
```


Ecco come appare effettivamente la transazione:
```
{
  "txid": "a600148ac3b05f0c774b8687a71c545077ea5dfb9677e5c6d708215053d892e8",
  "hash": "a600148ac3b05f0c774b8687a71c545077ea5dfb9677e5c6d708215053d892e8",
  "version": 2,
  "size": 125,
  "vsize": 125,
  "weight": 500,
  "locktime": 0,
  "vin": [
    {
      "txid": "854a833b667049ac811b4cf1cad40fa7f8dce8b0f4c1018a58b84559b6e05f42",
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
      "value": 0.00000000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_RETURN b9f81a8919e5aba39aeb86145c684010e6e559b580a85003ae25d78237a12e75",
        "hex": "6a20b9f81a8919e5aba39aeb86145c684010e6e559b580a85003ae25d78237a12e75",
        "type": "nulldata"
      }
    },
    {
      "value": 0.01460000,
      "n": 1,
      "scriptPubKey": {
        "asm": "0 998a9b0ed076bbdec1d88da4f475b9dde75e3620",
        "hex": "0014998a9b0ed076bbdec1d88da4f475b9dde75e3620",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1qnx9fkrksw6aaaswc3kj0gademhn4ud3q7cz4fm"
        ]
      }
    }
  ]
}

```
Come puoi vedere, questo rimanda la maggior parte del denaro direttamente all'indirizzo modificato (`tb1qnx9fkrksw6aaaswc3kj0gademhn4ud3q7cz4fm`) meno una piccola commissione di transazione. Ancora più importante, il primo output mostra un OP_RETURN con i dati (`b9f81a8919e5aba39aeb86145c684010e6e559b580a85003ae25d78237a12e75`) subito dopo.

## Invia la transazione grezza

Firma la tua transazione grezza e inviala, e presto OP_RETURN sarà incorporato nella blockchain!

## Controlla il tuo OP_RETURN

Ancora una volta, ricorda che puoi guardare questa transazione usando un blockchain explorer:
[https://live.blockcypher.com/btc-testnet/tx/a600148ac3b05f0c774b8687a71c545077ea5dfb9677e5c6d708215053d892e8/](https://live.blockcypher.com/btc-testnet/tx/a600148ac3b05f0c7 74b8687a71c545077ea5dfb9677e5c6d708215053d892e8/)

Potresti notare un avviso relativo ai dati che si trovano in un "protocollo sconosciuto". Se stessi progettando un uso regolare dei dati `OP_RETURN`, probabilmente lo contrassegneresti con un prefisso speciale, per contrassegnare quel protocollo. Quindi, i dati OP_RETURN effettivi potrebbero essere qualcosa come "CONTRACTS3b110a164aa18d3a5ab064ba93fdce62". In questo esempio non è stato utilizzato un prefisso per evitare di confondere lo spazio dati.

## Riepilogo: invio di una transazione con dati

Puoi utilizzare un codice operativo `OP_RETURN` per archiviare fino a 80 byte di dati sulla blockchain. Puoi farlo con la parola in codice `data` per un `vout`. Devi comunque inviare anche del denaro, ma devi semplicemente rispedirlo a un indirizzo diverso, meno una commissione di transazione.

> :fire: _Qual è il potere di OP_RETURN?_ OP_RETURN apre possibilità completamente nuove per la blockchain, perché puoi incorporare dati che dimostrano che determinate cose sono accadute in determinati momenti. Varie organizzazioni hanno utilizzato OP_RETURN per provare l'esistenza, per il copyright, per monete colorate e [per altri scopi](https://en.bitcoin.it/wiki/OP_RETURN). Anche se 80 byte potrebbero non sembrare molti, possono essere abbastanza efficaci se si utilizzano OP_RETURN per archiviare gli hash dei dati effettivi. Quindi, puoi dimostrare l'esistenza dei tuoi dati digitali dimostrando che il loro hash corrisponde all'hash sulla blockchain.

Tieni presente che c'è qualche controversia sull'utilizzo della blockchain Bitcoin in questo modo.

## Qual è il prossimo argomento?

Passa a "Bitcoin Scripting" nel [Capitolo 9: Introduzione script di Bitcoin](09_0_Introduzione_script_di_Bitcoin.md).
