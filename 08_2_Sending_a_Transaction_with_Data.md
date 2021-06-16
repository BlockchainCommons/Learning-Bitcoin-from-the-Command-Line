# 8.2: Sending a Transaction with Data

The final way to vary how you send a basic transaction is to use the transaction to send data instead of funds (or really, in addition to funds). This gives you the ability to embed information in the blockchain. It is done through a special `OP_RETURN` command.

The catch? You can only store 80 bytes at a time!

## Create Your Data

The first thing you need to do is create the 80 bytes (or less) of data that you'll be recording in your `OP_RETURN`. This might be as simple as preparing a message or you might be hashing existing data. For example, `sha256sum` produces 256 bits of data, which is 32 bytes, well under the limits:
```
$ sha256sum contract.jpg
b9f81a8919e5aba39aeb86145c684010e6e559b580a85003ae25d78237a12e75  contract.jpg
$ op_return_data="b9f81a8919e5aba39aeb86145c684010e6e559b580a85003ae25d78237a12e75"
```
> :book: _What is an OP_RETURN?_ All Bitcoin transactions are built upon opcode scripts that we'll meet in the next chapter. The `OP_RETURN` is a simple  opcode that defines an OUTPUT as invalid. Convention has resulted in it being used to embed data on the blockchain.

## Prepare Some Money

Your purpose in creating a data transaction isn't to send money to anyone, it's to put data into the blockchain. However, you _must_ send money to do so. You just need to use a change address as your _only_ recipient. Then you can identify a UTXO and send that to your change address, minus a transaction fee, while also using the same transaction to create an OP_RETURN.

Here's the standard setup:
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

## Write A Raw Transaction

You can now write a new rawtransaction with two outputs: one is your change address to get back (most of) your money, the other is a data address, which is the `bitcoin-cli` term for an OP_RETURN.
```
rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "data": "'$op_return_data'", "'$changeaddress'": 0.0146 }''')
```


Here's what that transaction actually looks like:
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
As you can see, this sends the majority of the money straight back to the change address (`tb1qnx9fkrksw6aaaswc3kj0gademhn4ud3q7cz4fm`) minus a small transaction fee. More importantly, the first output shows an OP_RETURN with the data (`b9f81a8919e5aba39aeb86145c684010e6e559b580a85003ae25d78237a12e75`) right after it.

## Send A Raw Transaction

Sign your raw transaction and send it, and soon that OP_RETURN will be embedded in the blockchain!

## Check Your OP_RETURN

Again, remember that you can look at this transaction using a blockchain explorer: 
[https://live.blockcypher.com/btc-testnet/tx/a600148ac3b05f0c774b8687a71c545077ea5dfb9677e5c6d708215053d892e8/](https://live.blockcypher.com/btc-testnet/tx/a600148ac3b05f0c774b8687a71c545077ea5dfb9677e5c6d708215053d892e8/)

You may note a warning about the data being in an "unknown protocol". If you were designing some regular use of `OP_RETURN` data, you'd probably mark it with a special prefix, to mark that protocol. Then, the actual OP_RETURN data might be something like "CONTRACTS3b110a164aa18d3a5ab064ba93fdce62". This example didn't use a prefix to avoid muddying the data space.

## Summary: Sending a Transaction with Data

You can use an `OP_RETURN` opcode to store up to 80 bytes of data on the blockchain. You do this with the `data` codeword for a `vout`. You still have to send money along too, but you just send it back to a change address, minus a transaction fee.

> :fire: _What is the Power of OP_RETURN?_ The OP_RETURN opens up whole new possibilities for the blockchain, because you can embed data that proves that certain things happened at certain times. Various organizations have used OP_RETURNs for proof of existence, for copyright, for colored coins, and [for other purposes](https://en.bitcoin.it/wiki/OP_RETURN). Though 80 bytes might not seem a lot, it can be quite effective if OP_RETURNs are used to store hashes of the actual data. Then, you can prove the existence of your digital data by demonstrating that the hash of it matches the hash on the blockchain.

Note that there is some controversy over using the Bitcoin blockchain in this way.

## What's Next?

Move on to "Bitcoin Scripting" with [Chapter Nine: Introducing Bitcoin Scripts](09_0_Introducing_Bitcoin_Scripts.md).
