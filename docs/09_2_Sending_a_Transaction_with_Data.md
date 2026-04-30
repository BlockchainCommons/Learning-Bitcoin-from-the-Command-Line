# 9.2: Sending a Transaction with Data

The final way to vary how you send a basic transaction is to use the
transaction to send data instead of funds (or really, in addition to
funds). This gives you the ability to embed information in the
blockchain. It is done through a special `OP_RETURN` command.

The catch? It's controversial. Many people think you shouldn't put
data onto the Bitcoin chain at all. For a long time, `OP_RETURN` was
limited to 80 bytes to minimize its use, but in recent years it's
become unlimited.

## Create Your Data

The first thing you need to do is create the data that you'll be
recording in your `OP_RETURN`. This might be as simple as preparing a
message or you might be hashing existing data. For example,
`sha256sum` produces 256 bits of data, which you can use to commit to
a document such as a contract.

```
$ sha256sum contract.pdf
b9f81a8919e5aba39aeb86145c684010e6e559b580a85003ae25d78237a12e75  contract.pdf
$ op_return_data="b9f81a8919e5aba39aeb86145c684010e6e559b580a85003ae25d78237a12e75"
```

> 📖 _What is an OP_RETURN?_ All Bitcoin transactions are built upon
opcode scripts that we'll meet in later chapters. The `OP_RETURN` is a
simple opcode that defines an OUTPUT as invalid. Convention has
resulted in it being used to embed data on the blockchain.

## Prepare Some Money

Your purpose in creating a data transaction isn't to send money to
anyone, it's to put data into the blockchain. However, you _must_ send
money to do so. You just need to use a change address as your _only_
recipient. Then you can identify a UTXO and send that to your change
address, minus a transaction fee, while also using the same
transaction to create an OP_RETURN.

Here's the standard setup:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "9c1f8e701638cf58c8430f8ffd30c5235948268ecabb6af07d0dc378fff29d9e",
    "vout": 478,
    "address": "tb1qqgep7ahfekud8fg4nu9h80llxm6a33g00w3k6h",
    "label": "",
    "scriptPubKey": "001402321f76e9cdb8d3a5159f0b73bfff36f5d8c50f",
    "amount": 0.00233627,
    "confirmations": 5,
    "spendable": true,
    "solvable": true,
    "desc": "wpkh([d2d90038/84h/1h/0h/0/0]02323fe5e590877ec4308c3f19017498b5c1dcb39b4072a3204ddea0aad1b8d7f4)#7ln5e0qc",
    "parent_descs": [
      "wpkh([d2d90038/84h/1h/0h]tpubDD6bEYfJAjaN62Ndw8h39GLSkJ4JhWw8DBMKjJezKfB95A7JcWr2DZSRix5YrNwbU8fcjgDLy2ecNp6QT6E6h4QK9TFpqnJ6qkuUwyqfDhL/0/*)#kjjtltgy"
    ],
    "safe": true
  }
]

$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[0] | .txid')
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
$ changeaddress=$(bitcoin-cli getrawchangeaddress)
```

## Write A Raw Transaction

You can now write a new rawtransaction with two outputs: one is your
change address to get back (most of) your money, the other is a data
address, which is the `bitcoin-cli` term for an OP_RETURN.

```
rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "data": "'$op_return_data'", "'$changeaddress'": 0.00233 }''')
```


Here's what that transaction actually looks like:
```
$ bitcoin-cli -named decoderawtransaction hexstring=$rawtxhex
{
  "txid": "56af5070c97995ac30ad69770ed0f2afbc25daedb12fe20a79216f50801618b1",
  "hash": "56af5070c97995ac30ad69770ed0f2afbc25daedb12fe20a79216f50801618b1",
  "version": 2,
  "size": 125,
  "vsize": 125,
  "weight": 500,
  "locktime": 0,
  "vin": [
    {
      "txid": "9c1f8e701638cf58c8430f8ffd30c5235948268ecabb6af07d0dc378fff29d9e",
      "vout": 478,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967293
    }
  ],
  "vout": [
    {
      "value": 0.00000000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_RETURN b9f81a8919e5aba39aeb86145c684010e6e559b580a85003ae25d78237a12e75",
        "desc": "raw(6a20b9f81a8919e5aba39aeb86145c684010e6e559b580a85003ae25d78237a12e75)#nvcr5fqt",
        "hex": "6a20b9f81a8919e5aba39aeb86145c684010e6e559b580a85003ae25d78237a12e75",
        "type": "nulldata"
      }
    },
    {
      "value": 0.00233000,
      "n": 1,
      "scriptPubKey": {
        "asm": "0 b6dee3adaf8e3a7fd51645d0c76ae5c914fb2604",
        "desc": "addr(tb1qkm0w8td03ca8l4gkghgvw6h9ey20kfsyreqwny)#53rfascv",
        "hex": "0014b6dee3adaf8e3a7fd51645d0c76ae5c914fb2604",
        "address": "tb1qkm0w8td03ca8l4gkghgvw6h9ey20kfsyreqwny",
        "type": "witness_v0_keyhash"
      }
    }
  ]
}

```

As you can see, this sends the majority of the money straight back to
the change address (`tb1qkm0w8td03ca8l4gkghgvw6h9ey20kfsyreqwny`)
minus a small transaction fee. More importantly, the first output
shows an OP_RETURN with the data
(`b9f81a8919e5aba39aeb86145c684010e6e559b580a85003ae25d78237a12e75`)
right after it.

## Send A Raw Transaction

Sign your raw transaction and send it, and soon that OP_RETURN will be
embedded in the blockchain!

```
signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r \
'.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
56af5070c97995ac30ad69770ed0f2afbc25daedb12fe20a79216f50801618b1
```

## Check Your OP_RETURN

Again, remember that you can look at this transaction using a blockchain explorer: 
[https://mempool.space/signet/tx/56af5070c97995ac30ad69770ed0f2afbc25daedb12fe20a79216f50801618b1?showDetails=true](https://mempool.space/signet/tx/56af5070c97995ac30ad69770ed0f2afbc25daedb12fe20a79216f50801618b1?showDetails=true).

Some explors may note that the data is in an "unknown protocol". If
you were designing some regular use of `OP_RETURN` data, you'd
probably mark it with a special prefix, to mark that protocol. Then,
the actual OP_RETURN data might be something like
"CONTRACTS3b110a164aa18d3a5ab064ba93fdce62". This example didn't use a
prefix to avoid muddying the data space.

## Summary: Sending a Transaction with Data

You can use an `OP_RETURN` opcode to store up to 80 bytes of data on
the blockchain. You do this with the `data` codeword for a `vout`. You
still have to send money along too, but you just send it back to a
change address, minus a transaction fee.

> 🔥 _What is the Power of OP_RETURN?_ The OP_RETURN opens up whole
new possibilities for the blockchain, because you can embed data that
proves that certain things happened at certain times. Various
organizations have used OP_RETURNs for proof of existence, for
copyright, for colored coins, and for other purposes. But beware of
the possibility for pushback when you use OP_RETURN, and so if you use
it, do so responsibly, ideally embedding small amounts of data that
can be used as commitments rather than larger data dumps. This will
still support proof of existence and timestamping.

## What's Next?

Move on to "Importing & Exporting Your Secrets" with [Chapter Ten:
Creating Bitcoin Secrets](10_0_Creating_Bitcoin_Secrets.md)
