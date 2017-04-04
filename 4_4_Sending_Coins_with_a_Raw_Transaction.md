# 4.4: Sending Coins with Raw Transactions

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

As we said, the `bitcoin-cli` offers three major ways to send coins. Section 4.1 talking about sending them with the `sendtoaddress` command. Since then, we've been building details on how to send coins a second way, with raw transactions. Section 4.2 taught how to create a raw transaction, an Interlude taught how to use JQ, and Section 4.3 taught how to use named arguments.

We can now put those together and actually send cash using a raw transaction.

## Create a Change Address

Our sample raw transaction was very simplistic: we sent the entirety of a UTXO to a new address. More frequently, you'll want to send someone an amount of money that doesn't match a UTXO. But you'll recall that the excess money from a UTXO that's not sent to your recipient just becomes a transaction fee. So, how do you send someone just part of a UTXO, while keeping the rest for yourself?

The solution is to create a change address:
```
$ changeaddress=$(bitcoin-cli getrawchangeaddress)
$ echo $changeaddress
myrK8U3SE1nWh9y9XPho5aTrKYW6n8qSQv
```
Note that this is a new function: `getrawchangeaddress`. It's largely the same as `getnewaddress` but is optimized for use as a change address in a raw transaction, so it doesn't do things like make entries in your address book.

You now have an additional address inside your wallet, so that you can use to receive change from a UTXO! In order to use it, you'll need to create a raw transaction with two outputs. 

## Pick Sufficient UTXOs

Our simple raw transaction was simple in another way: it assumed that there was enough money in a single UTXO to cover the transaction. However, you'll often want create transactions that expend more money than is in a single UTXO. To do this, you must create a raw transaction with two (or more) inputs.

## Write a Real Raw Transaction

To summarize: creating a real raw transaction to send coins will often require multiple inputs and will almost always require multiple outputs, one of which is a change address.

Here's the transactions we'll be using in this example:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "ec0598918f6f5476cb90365651e8a2724ef26f949290bbf196f41ed96092a52f",
    "vout": 0,
    "address": "mjtEqr4Fffd1XtpAkKoDkMBP54mMXJeQ3j",
    "account": "",
    "scriptPubKey": "76a9142fe70d51e886b9ef73b76c1743c5a2bb2894db8588ac",
    "amount": 3.90000000,
    "confirmations": 9551,
    "spendable": true,
    "solvable": true
  }, 
  {
    "txid": "3470e5fe08633583d136b9cd49bb1a224c9d9313a0b4584fd3b7438dbdf34dbd",
    "vout": 0,
    "address": "msiyutru5TV33Q2UGK2Bbh2ewdrYALBzTb",
    "account": "",
    "scriptPubKey": "76a91485e7d9fe99708d575f3b93be13c0c55c6ffb765088ac",
    "amount": 1.95000000,
    "confirmations": 9542,
    "spendable": true,
    "solvable": true
  }
]

```
In this example we need to send 4.0 BTC, which is larger than either of our UTXOs. This requires combining them, then using our change address to get the unspent funds.

### Set Up Your Variables

We've already have `$changeaddress` and `$recipient` variables from our previous examples:
```
$ echo $changeaddress
myrK8U3SE1nWh9y9XPho5aTrKYW6n8qSQv
~$ echo $recipient
n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi
```
We also need to record the txid and vout for each of our two UTXOs:
```
$ utxo_txid_1=$(bitcoin-cli listunspent | jq -r '.[0] | .txid') 
$ utxo_txid_2=$(bitcoin-cli listunspent | jq -r '.[1] | .txid') 
$ utxo_vout_1=$(bitcoin-cli listunspent | jq -r '.[0] | .vout') 
$ utxo_vout_2=$(bitcoin-cli listunspent | jq -r '.[1] | .vout') 
```

### Write the Transaction

Writing the new raw transaction is surprisingly simple. All you need to do is include an additional, comma-separated JSON object in the JSON array of inputs and an additional, comma-separated key-value pair in the JSON object of outputs.

Here's the example. Take a look at it carefully to understand where the inputs end the outputs begin:
```
$ rawtxhex2=$(bitcoin-cli -named createrawtransaction transactions='''[ { "txid": "'$utxo_txid_1'", "vout": '$utxo_vout_1' }, { "txid": "'$utxo_txid_2'", "vout": '$utxo_vout_2' } ]''' outputs='''{ "'$recipient'": 4.0, "'$changeaddress'": 1.8495 }''')
```
We were _very_ careful figuring out our money math. These two UTXOs contain 5.85 BTC. After sending 4.0 BTC, we'll have 1.85 BTC left. We again chose .0005 BTC for the transaction fee. To accomodate that fee, we set our change to 1.849 BTC. If we'd messed up our math and instead set our change to 1.749 BTC, that additional .1 BTC would be lost. That's $100 gone to the miners! If we'd forgot to make change at all, then the whole 1.85 BTC ($2000!) would have disappeared. So, again, _be careful_. 

Fortunately, we can triple-check with our JQ alias:
```
$ btctxfee $rawtxhex2
.0005
```

### Finish It Up

You can now sign, seal, and deliver your transaction, and it's yours (and the faucet's):
```
$ signedtx2=$(bitcoin-cli -named signrawtransaction hexstring=$rawtxhex2 | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx2
e9f5979c6a57ac9154c9583a1eec7841841dbf085212ac7ec31aff020a212c25
```

[[REVISED UP TO HERE; JUST NEED TO PUT IN NEW DATA]]
### Wait

As usual, your money will be in flux for a while. You used all of your UTXOs, so your `getbalance` may show that you have cash, but the transaction must be confirmed before you see a UTXO with your change.

But, in 10 minutes or less (probably), you'll have your remaining money back and fully spendable again:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "18f1a791e3f7735dbe1c92b86645ba9b1f62b8044f61efd76d082a084982c9ae",
    "vout": 1,
    "address": "mxU9cmhJfkKWDtBspHaA36LkeafEDeaogJ",
    "account": "",
    "scriptPubKey": "76a914b9f2579814712f82a3dadfd73e0356339c6705b488ac",
    "amount": 0.05550000,
    "confirmations": 2,
    "spendable": true,
    "solvable": true
  }
]
```

This also might be a good time to revisit a blockchain explorer, so that you can see more intuitively how the inputs, outputs, and transaction fee are all laid out: [https://live.blockcypher.com/btc-testnet/tx/18f1a791e3f7735dbe1c92b86645ba9b1f62b8044f61efd76d082a084982c9ae/](https://live.blockcypher.com/btc-testnet/tx/18f1a791e3f7735dbe1c92b86645ba9b1f62b8044f61efd76d082a084982c9ae/).
$ bitcoin-cli decoderawtransaction $rawtxhex2
{
  "txid": "99b7454c20313806454f9fd4f3e0454ce93e6edd5e84dfc731bcc0ad352b2260",
  "hash": "99b7454c20313806454f9fd4f3e0454ce93e6edd5e84dfc731bcc0ad352b2260",
  "size": 160,
  "vsize": 160,
  "version": 1,
  "locktime": 0,
  "vin": [
    {
      "txid": "c1abb6951e6a9aae7e384412b69b69e59c10daac9397d01d0c52b7bc6278d589",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967295
    }, 
    {
      "txid": "ab7ca727055b812df882298f4e6e10ec699fb6250d843c813623171781f896d8",
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
      "value": 0.10000000,
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
    }, 
    {
      "value": 0.05550000,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 b9f2579814712f82a3dadfd73e0356339c6705b4 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914b9f2579814712f82a3dadfd73e0356339c6705b488ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mxU9cmhJfkKWDtBspHaA36LkeafEDeaogJ"
        ]
      }
    }
  ]
}
```
[[THEN NEW CONCLUSION]]
