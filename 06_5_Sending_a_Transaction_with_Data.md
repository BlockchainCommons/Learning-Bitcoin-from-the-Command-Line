# 6.5: Sending a Transaction with Data

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

The third way to vary how you send a basic transaction is to use the transaction to send data instead of funds (or really, in addition to funds). This gives you the ability to embed information in the blockchain.

## Create Your Data

The first thing you need to do is create the 80 bytes (or less) of data that you'll be recording in your OP_RETURN. This might be as simple as preparing a message or you might be hahsing existing data. For example, `sha256sum` produces 256 bits of data, which is 32 bytes, well under the limits:
```
$ sha256sum contract.jpg
fe7f0a3b69f56ef2d055a78823ed3bd1422e46c3183658ea854253033ae0ccef  contract.jpg
$ op_return_data="fe7f0a3b69f56ef2d055a78823ed3bd1422e46c3183658ea854253033ae0ccef"
```
_What is an OP_RETURN?_ All Bitcoin transactions are built upon opcode scripts that we'll meet in the next chapter. The OP_RETURN is a simple  opcode that defines a transaction as invalid. Convention has resulted in it being used to embed data on the blockchain.

## Prepare Some Money

Your purpose in creating a data transaction isn't to send money to anyone, it's to put data into the blockchain. However, you _must_ send money to do so. You just need to use a change address as your _only_ recipient. Then you can identify a UTXO and send that to your change address, minus a transaction fee, while also using the same transaction to create an OP_RETURN.

Here's the standard setup:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "4460175e8276d5a1935f6136e36868a0a3561532d44ddffb09b7cb878f76f927",
    "vout": 1,
    "address": "my7UvPbPWDbqCxYpAcUZELDbK1X7w9Whmc",
    "scriptPubKey": "76a914c101d8c34de7b8d83b3f8d75416ffaea871d664988ac",
    "amount": 0.90000000,
    "confirmations": 682,
    "spendable": true,
    "solvable": true
  }
]

$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[0] | .txid')
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
$ changeaddress=$(bitcoin-cli getrawchangeaddress)
```

## Write A Raw Transaction

You can now write a new rawtransaction with two outputs: one is your change address to get back (most of) your money, the other is a data address, which is the `bitcoin-cli` term for an OP_RETURN.
```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "data": "'$op_return_data'", "'$changeaddress'": 0.8995 }''')
```

Here's what that transaction actually looks like:
```
$ bitcoin-cli -named decoderawtransaction hexstring=$rawtxhex 
{
  "txid": "531660f9bcb2571b2b790ebe6e7b6ed40618f608f3f6722a511275257942790b",
  "hash": "531660f9bcb2571b2b790ebe6e7b6ed40618f608f3f6722a511275257942790b",
  "size": 128,
  "vsize": 128,
  "version": 2,
  "locktime": 0,
  "vin": [
    {
      "txid": "4460175e8276d5a1935f6136e36868a0a3561532d44ddffb09b7cb878f76f927",
      "vout": 1,
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
        "asm": "OP_RETURN fe7f0a3b69f56ef2d055a78823ed3bd1422e46c3183658ea854253033ae0ccef",
        "hex": "6a20fe7f0a3b69f56ef2d055a78823ed3bd1422e46c3183658ea854253033ae0ccef",
        "type": "nulldata"
      }
    }, 
    {
      "value": 0.89950000,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 166692bda9f25ced145267bb44286e8ee3963d26 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914166692bda9f25ced145267bb44286e8ee3963d2688ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mhZQ3Bih6wi7jP1tpFZrCcyr4NsfCapiZP"
        ]
      }
    }
  ]
}
```
As you can see, this sends the majority of the money straight back to the change address (mhZQ3Bih6wi7jP1tpFZrCcyr4NsfCapiZP) minus that standard transaction fee we've been using of 0.0005 BTC. More importantly, the first output shows an OP_RETURN with the data (fe7f0a3b69f56ef2d055a78823ed3bd1422e46c3183658ea854253033ae0ccef) right after it.

## Send A Raw Transaction

Sign your raw transaction and send it, and soon that OP_RETURN will be embedded in the blockchain!

## Check Your OP_RETURN

Again, remember that you can look at this transaction using a blockchain explorer: [https://live.blockcypher.com/btc-testnet/tx/3a62b396afb8d8a59ebe7b9e52d6aa2485f1082a1d3fc6ece61fb8b55373823d/](https://live.blockcypher.com/btc-testnet/tx/3a62b396afb8d8a59ebe7b9e52d6aa2485f1082a1d3fc6ece61fb8b55373823d/)

You may note a warning about the data being in an "unknown protocol". If you were designing some regular use of OP_RETURN data, you'd probably mark it with a special prefix, to mark that protocol. Then, the actual OP_RETURN data might be something like "CONTRACTS3b110a164aa18d3a5ab064ba93fdce62". This example didn't use a prefix to avoid muddying the data space.

[Coinsecrets](http://coinsecrets.org/) offers another interesting way to look at OP_RETURN data. It does its best to keep abreast of protocols, so that it can tell you who is doing what in the blockchain. Here's this transaction there: [https://www.blocktrail.com/tBTC/tx/3a62b396afb8d8a59ebe7b9e52d6aa2485f1082a1d3fc6ece61fb8b55373823d](https://www.blocktrail.com/tBTC/tx/3a62b396afb8d8a59ebe7b9e52d6aa2485f1082a1d3fc6ece61fb8b55373823d)

## Summary: Sending a Transaction with Data

You can use an OP_RETURN opcode to store up to 80 bytes of data on the blockchain. You do this with the `data` codeword for a `vout`. You still have to send money along too, but you just send it back to a change address, minus a transaction fee.

_What is the Power of OP_RETURN?_ The OP_RETURN opens up whole new possibilities for the blockchain, because you can embed data that proves that certain things happened at certain times. Various organizations have used OP_RETURNs for proof of existence, for copyright, for colored coins, and [for other purposes](https://en.bitcoin.it/wiki/OP_RETURN). Though 80 bytes might not seem a lot, it can be quite effective if OP_RETURNs are used to store hashes of the actual data. Then, you can prove the existence of your digital data by deminstrating that the hash of it matches the hash on the blockchain.

Note that there is some controversy over using the Bitcoin blockchain in this way.

## What's Next?

Move on to "Bitcoin Scripting" with [Chapter Seven: Introducing Bitcoin Scripts](07_0_Introducing_Bitcoin_Scripts.md).
