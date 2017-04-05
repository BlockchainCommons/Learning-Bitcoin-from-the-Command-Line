# 4.4: Sending Coins with Raw Transactions

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

As noted at the start of this chapter, the `bitcoin-cli` interface offers three major ways to send coins. Section 4.1 talking about sending them the first way, with the `sendtoaddress` command. Since then, we've been building details on how to send coins a second way, with raw transactions. Section 4.2 taught how to create a raw transaction, an Interlude explained JQ, and section 4.3 demonstrated named arguments.

We can now put those together and actually send funds using a raw transaction.

## Create a Change Address

Our sample raw transaction in section 4.2 was very simplistic: we sent the entirety of a UTXO to a new address. More frequently, you'll want to send someone an amount of money that doesn't match a UTXO. But you'll recall that the excess money from a UTXO that's not sent to your recipient just becomes a transaction fee. So, how do you send someone just part of a UTXO, while keeping the rest for yourself?

The solution is actually to _send_ the rest of the funds to a second address, a change address that you've created in your wallet specifically to receive them:
```
$ changeaddress=$(bitcoin-cli getrawchangeaddress)
$ echo $changeaddress
myrK8U3SE1nWh9y9XPho5aTrKYW6n8qSQv
```
Note that this uses a new function: `getrawchangeaddress`. It's largely the same as `getnewaddress` but is optimized for use as a change address in a raw transaction, so it doesn't do things like make entries in your address book.

You now have an additional address inside your wallet, so that you can receive change from a UTXO! In order to use it, you'll need to create a raw transaction with two outputs. 

## Pick Sufficient UTXOs

Our sample raw transaction was simple in another way: it assumed that there was enough money in a single UTXO to cover the transaction. Often this will be the case, but sometimes you'll want to create transactions that expend more money than you have in a single UTXO. To do so, you must create a raw transaction with two (or more) inputs.

## Write a Real Raw Transaction

To summarize: creating a real raw transaction to send coins will sometimes require multiple inputs and will almost always require multiple outputs, one of which is a change address. So that's what we'll be doing here, in a new example that shows a real-life example of sending funds via Bitcoin's second methodology, raw transactions.

Here's the transactions we'll be using:
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
In our example, we're going to send 4.0 BTC, which is larger than either of our UTXOs. This requires combining them, then using our change address to retrieve the unspent funds.

### Set Up Your Variables

We already have `$changeaddress` and `$recipient` variables from previous examples:
```
$ echo $changeaddress
myrK8U3SE1nWh9y9XPho5aTrKYW6n8qSQv
~$ echo $recipient
n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi
```
We also need to record the txid and vout for each of our two UTXOs. Having identified the UTXOs that we want to spend, we can use our JQ techniques to make sure doing so is error free:
```
$ utxo_txid_1=$(bitcoin-cli listunspent | jq -r '.[0] | .txid') 
$ utxo_vout_1=$(bitcoin-cli listunspent | jq -r '.[0] | .vout') 
$ utxo_txid_2=$(bitcoin-cli listunspent | jq -r '.[1] | .txid') 
$ utxo_vout_2=$(bitcoin-cli listunspent | jq -r '.[1] | .vout') 
```

### Write the Transaction

Writing these more complex raw transaction is surprisingly simple. All you need to do is include an additional, comma-separated JSON object in the JSON array of inputs and an additional, comma-separated key-value pair in the JSON object of outputs.

Here's the example. Note the multiple inputs after the `inputs` arg and the multiple outputs after the `outputs` arg.
```
$ rawtxhex2=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid_1'", "vout": '$utxo_vout_1' }, { "txid": "'$utxo_txid_2'", "vout": '$utxo_vout_2' } ]''' outputs='''{ "'$recipient'": 4.0, "'$changeaddress'": 1.8495 }''')
```
We were _very_ careful figuring out our money math. These two UTXOs contain 5.85 BTC. After sending 4.0 BTC, we'll have 1.85 BTC left. We again chose .0005 BTC for the transaction fee. To accomodate that fee, we set our change to 1.8495 BTC. If we'd messed up our math and instead set our change to 1.7495 BTC, that additional .1 BTC would be lost. That's $100 gone to the miners! If we'd forgot to make change at all, then the whole 1.85 BTC ($2000!) would have disappeared. So, again, _be careful_. 

Fortunately, we can triple-check with the `btctxfee` alias from the JQ Interlude:
```
$ btctxfee $rawtxhex2
.0005
```

### Finish It Up

You can now sign, seal, and deliver your transaction, and it's yours (and the faucet's):
```
$ signedtx2=$(bitcoin-cli -named signrawtransaction hexstring=$rawtxhex2 | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx2
69c9ef4d1adb48596c470146ee9b60593023b6eb870b79ef666a8c9369768469
```

### Wait

As usual, your money will be in flux for a while: the change will be unavailable until the transaction actually gets confirmed and a new UTXO is given to you.

But, in 10 minutes or less (probably), you'll have your remaining money back and fully spendable again:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "69c9ef4d1adb48596c470146ee9b60593023b6eb870b79ef666a8c9369768469",
    "vout": 1,
    "address": "myrK8U3SE1nWh9y9XPho5aTrKYW6n8qSQv",
    "scriptPubKey": "76a914c91b8f2f983aa9f8f0ba552adf6b6491ac01e02888ac",
    "amount": 1.84950000,
    "confirmations": 2,
    "spendable": true,
    "solvable": true
  } 
]
```

This also might be a good time to revisit a blockchain explorer, so that you can see more intuitively how the inputs, outputs, and transaction fee are all laid out: [69c9ef4d1adb48596c470146ee9b60593023b6eb870b79ef666a8c9369768469](https://live.blockcypher.com/btc-testnet/tx/69c9ef4d1adb48596c470146ee9b60593023b6eb870b79ef666a8c9369768469/).

## Summary: Sending Coins with Raw Transactions

To send coins with raw transactions, you need to create a raw transaction with one or more inputs (to have sufficient funds) and one or more outputs (to retrieve change). Then, you can follow your normal procedure of using `createrawtransaction` with named arguments and JQ, as laid out in previous sections.

### Why Use Raw Transactions

_The advantages._ It gives you the best control. If your goal is to write a more intricate script or Bitcoin program, you'll probably use raw transactions so that you know exactly what's going on. This is also the _safest_ situation to use raw transactions, because you can programmatically ensure that you don't make mistakes.

_The disadvantages._ It's easy to lose money. There are no warnings, no safeguards, and no programmatic backstops, unless you write them. It's also arcane. The formatting is obnoxious, even using the easy-to-use `bitcoin-cli` interface, and you have to do a lot of lookup and calculation by hand.
