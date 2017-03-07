# 3.5: Sending a Raw Transaction with an OP_RETURN

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

A P2PKH transaction is one of the simplest forms of Bitcoin transaction... except perhaps the OP_RETURN. That's because an OP_RETURN is basically a null: an invalid output. Why would you use one? To store data on the blockchain: up to 80 bytes. 

This opens up whole new possibilities for the blockchain, because you can embed data that proves that certain things happened at certain times. Though there is some controversy over using the Bitcoin blockchain in this way, various organizations have used OP_RETURNs for proof of existence, for copyright, to color coins, and [for other purposes](https://en.bitcoin.it/wiki/OP_RETURN). Though 80 bytes might not seem a lot, it can be quite effective if OP_RETURNs are used to store hashes of the actual data.

## Create Your OP_RETURN Data

The first thing you need to do is create the 80 bytes (or less) of data that you'll be recording in your OP_RETURN. This might be as simple as preparing a message or you might be hasing existing data. For example, `md5sum` produces 128 bits of data, which is 16 bytes, well under the limits:
```
$ md5sum mycontract.jpg 
3b110a164aa18d3a5ab064ba93fdce62  mycontract.jpg
$ op_return_data="3b110a164aa18d3a5ab064ba93fdce62"
```

## Prepare Some Money

Your purpose in creating an OP_RETURN isn't to send money to anyone, it's to put data into the blockchain. However, you _must_ send money to do so. You just need to use a change address as your _only_ recipient. Then you can identify a UTXO and send that to your change address, minus a transaction, while also using the same transaction to create an OP_RETURN.

Here's that process:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "0c198125f5a0e2e598ce3b7e4253a95dce780dec12601ed0a44c8544606782b2",
    "vout": 0,
    "address": "n3cogFw4y3A1LJgRz2G97otEGT7BK8ca3D",
    "account": "",
    "scriptPubKey": "76a914f26e11dc0fcc79fe76fca1d24d7588798922ca7488ac",
    "amount": 0.06600000,
    "confirmations": 11,
    "spendable": true,
    "solvable": true
  }
]

$ utxo_txid_4="0c198125f5a0e2e598ce3b7e4253a95dce780dec12601ed0a44c8544606782b2"
$ utxo_vout_4="0"
$ changeaddress_4=$(bitcoin-cli getrawchangeaddress)
```

## Write A Rawtransaction

You can now write a new rawtransaction with two outputs: one is your change address to get back (most of) your money, the other is a data address, which is bitcoin-cli's term for an OP_RETURN.
```
$ rawtxhex4=$(bitcoin-cli createrawtransaction '''[ { "txid": "'$utxo_txid_4'", "vout": '$utxo_vout_4' } ]''' '''{ "data": "'$op_return_data'", "'$changeaddress_4'": 0.0655 }''')
```

Here's what that transaction actually looks like:
```
$ bitcoin-cli decoderawtransaction $rawtxhex4
{
  "txid": "58a75a85833c396082e6603a0a2537c7e8c207d188656791679df7da7fe0879a",
  "hash": "58a75a85833c396082e6603a0a2537c7e8c207d188656791679df7da7fe0879a",
  "size": 112,
  "vsize": 112,
  "version": 1,
  "locktime": 0,
  "vin": [
    {
      "txid": "0c198125f5a0e2e598ce3b7e4253a95dce780dec12601ed0a44c8544606782b2",
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
        "asm": "OP_RETURN 3b110a164aa18d3a5ab064ba93fdce62",
        "hex": "6a103b110a164aa18d3a5ab064ba93fdce62",
        "type": "nulldata"
      }
    }, 
    {
      "value": 0.06550000,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 effa6f1be9433e051ddbc33d7021259f5e1333c7 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914effa6f1be9433e051ddbc33d7021259f5e1333c788ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "n3PqoMxnQQmafdup2hAmV6SzanJGo4FG8f"
        ]
      }
    }
  ]
}
```
As you can see, this sends the majority of the money straight back to the change address (n3PqoMxnQQmafdup2hAmV6SzanJGo4FG8f) minus that standard transaction fee we've been using of 0.0005 BTC. _Do_ note that these transactions get larger as they add extra inputs and outputs — and now 16 bytes of data. More importantly, the first output shows an OP_RETURN with the data (3b110a164aa18d3a5ab064ba93fdce62) right after it.

Sign it and send it, and soon that OP_RETURN will be embedded in the blockchain!

## Check Your OP_RETURN

Again, remember that you can look at this transaction using a blockchain explorer: [https://live.blockcypher.com/btc-testnet/tx/ed445dc970bb40b17c207109e19a37b2be301acb474ccd30680c431cb681bce2/](https://live.blockcypher.com/btc-testnet/tx/ed445dc970bb40b17c207109e19a37b2be301acb474ccd30680c431cb681bce2/)

You may note a warning about the data being in an "unknown protocol". If you were designing some regular use of OP_RETURN data, you'd probably mark it with a special prefix, to mark that protocol. Then, the actual OP_RETURN data might be something like "CONTRACTS3b110a164aa18d3a5ab064ba93fdce62". This example didn't use a prefix to avoid muddying the data space.

[Coinsecrets](http://coinsecrets.org/) offers another interesting way to look at OP_RETURN data. It does its best to keep abreast of protocols, so that it can tell you who is doing what in the blockchain. Here's thisour transaction there: [https://www.blocktrail.com/tBTC/tx/ed445dc970bb40b17c207109e19a37b2be301acb474ccd30680c431cb681bce2](https://www.blocktrail.com/tBTC/tx/ed445dc970bb40b17c207109e19a37b2be301acb474ccd30680c431cb681bce2)

## Summary: Sending a Raw Transaction with an OP_RETURN

You can use an OP_RETURN output to store up to 80 bytes of data on the blockchain. You do this with the 'data' codeword for a 'vout'. You still have to send money along too, but you just send it back to a change address, minus a transaction fee.



