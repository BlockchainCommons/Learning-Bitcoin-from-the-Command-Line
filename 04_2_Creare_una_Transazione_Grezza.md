# 4.2 Creating a Raw Transaction

You're now ready to create Bitcoin raw transactions. This allows you to send money but to craft the transactions as precisely as you want. This first section focuses on a simple one-input, one-output transaction. This sort of transaction _isn't_ actually that useful, because you're rarely going to want to send all of your money to one person (unless you're actually just forwarding it on, such as if you're moving things from one wallet to another). Thus, we don't label this section as a way to send money. It's just a foundational stepping stone to _actually_ sending money with a raw transaction.

## Understand the Bitcoin Transaction

Before you dive into actually creating raw transactions, you should make sure you understand how a Bitcoin transaction works. It's all about the UTXOs.

> :book: ***What is a UTXO?*** When you receive cash in your Bitcoin wallet, it appears as an individual transaction. Each of these transactions is called a Unspent Transaction Output (UTXO). It doesn't matter if various payments were made to the same address or to multiple addresses: each incoming transaction remains distinct in your wallet as a UTXO.

When you create a new outgoing transaction, you gather together one or more UTXOs, each of which represents a blob of money that you received. You use these as inputs for a new transaction. Together their amount must equal what you want to spend _or more_. Then, you generate one or more outputs, which give the money represented by the inputs to one or more people. This creates new UTXOs for the recipients, which may then use _those_ to fund future transactions.

Here's the trick: _all of the UTXOs that you gather are spent in full!_ That means that if you want to send just part of the money in a UTXO to someone else, then you also have to generate an additional output that sends the rest back to you! For now, we won't worry about that, but the use of a change address will be vital when moving on from the theory of this chapter to more practical transactions.

## List Your Unspent Transactions

In order to create a new raw transaction, you must know what UTXOs you have on-hand to spend. You can determine this information with the `bitcoin-cli listunspent` command:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36",
    "vout": 0,
    "address": "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
    "label": "",
    "scriptPubKey": "76a9141b72503639a13f190bf79acf6d76255d772360b788ac",
    "amount": 0.00010000,
    "confirmations": 20,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/1']02fd5740996d853ea51a6904cf03257fc11204b0179f344c49739ec5b20b39c9ba)#62rud39c",
    "safe": true
  },
  {
    "txid": "61f3b7016bf1ecc3987b8805207e79362e4de8026682e149107999b779426e3a",
    "vout": 1,
    "address": "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
    "label": "",
    "scriptPubKey": "76a9142d573900aa357a38afd741fbf24b075d263ea6e088ac",
    "amount": 0.00050000,
    "confirmations": 3,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/3']0278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132)#nhjc3f8y",
    "safe": true
  },
  {
    "txid": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
    "vout": 0,
    "address": "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
    "label": "",
    "scriptPubKey": "76a9142d573900aa357a38afd741fbf24b075d263ea6e088ac",
    "amount": 0.00022000,
    "confirmations": 3,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/3']0278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132)#nhjc3f8y",
    "safe": true
  }
]

```
This listing shows three different UTXOs, worth .0001, .0005 and .00022 BTC. Note that each has its own distinct txid and remains distinct in the wallet, even the last two, which were sent to the same address.

When you want to spend a UTXO, it's not sufficient to just know the transaction id. That's because each transaction can have multiple outputs! Remember that first chunk of money that the faucet sent us? In the transaction, some money went to us and some went to someone else. The `txid` refers to the overall transaction, while a `vout` says which of multiple outputs you've received. In this list, each of these transactions is the 0th `vout` of a previous transaction, but _that doesn't have to be the case_.

So, txid+vout=UTXO. This will be the foundation of any raw transaction.

## Write a Raw Transaction with One Output

You're now ready to write a simple, example raw transaction that shows how to send the entirety of a UTXO to another party. As noted, this is not necessarily a very realistic real-world case.

> :warning: **WARNING:** It is very easy to lose money with a raw transaction. Consider all instructions on sending bitcoins via raw transactions to be _very_, _very_ dangerous. Whenever you're actually sending real money to other people, you should instead use one of the other methods explained in this chapter. Creating raw transactions is extremely useful if you're writing bitcoin programs, but _only_ when you're writing bitcoin programs. (For example: in writing this example for one version of this tutorial, we accidentally spent the wrong transaction, even though it had about 10x as much value. Almost all of that was lost to the miners.)

### Prepare the Raw Transaction

For best practices, we'll start out each transaction by carefully recording the txids and vouts that we'll be spending.

In this case, we're going to spend the one worth .00050000 BTC because it's the only one with a decent value.
```
$ utxo_txid="61f3b7016bf1ecc3987b8805207e79362e4de8026682e149107999b779426e3a"
$ utxo_vout="1"
```

You should similarly record your recipient address, to make sure you have it right. We're again sending some money back to the TP faucet:
```
$ recipient="n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi"
```
As always, check your variables carefully, to make sure they're what you expect!
```
$ echo $utxo_txid
61f3b7016bf1ecc3987b8805207e79362e4de8026682e149107999b779426e3a
$ echo $utxo_vout
1
$ echo $recipient
n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi
```
That recipient is particularly important, because if you mess it up, your money is _gone_! (And as we already saw, choosing the wrong transaction can result in lost money!) So triple check it all.

### Understand the Transaction Fee

Each transaction has a fee associated with. It's _implicit_ when you send a raw transaction: the amount that you will pay as a fee is always equal to the amount of your input minus the amount of your output. So, you have to decrease your output a little bit from your input to make sure that your transaction goes out.

> :warning: **WARNING:** This is the very dangerous part of raw transactions!! Because you automatically expend all of the amount in the UTXOs that you use, it's critically important to make sure that you know: (1) precisely what UTXOs you're using; (2) exactly how much money they contain; (3) exactly how much money you're sending out; and (4) what the difference is. If you mess up and you use the wrong UTXO (with more money than you thought) or if you send out too little money, the excess is lost. Forever. Don't make that mistake! Know your inputs and outputs _precisely_. Or better, don't use raw transactions except as part of a carefully considered and triple-checked program.

> :book: ***How much should you spend on transaction fees?*** [Bitcoin Fees](https://bitcoinfees.21.co/) has a nice live assessment. It says that the "fastest and cheapest transaction fee is currently 42 satoshis/byte" and that "For the median transaction size of 224 bytes, this results in a fee of 9,408 satoshis".

Currently Bitcoin Fees suggests a transaction fee of about 10,000 satoshis, which is the same as .0001 BTC. Yes, that's for the mainnet, not the testnet, but we want to test out things realistically, so that's what we're going to use.

In this case, that means taking the .0005 BTC in the UTXO we're selected, reducing it by .0001 BTC for the transaction fee, and sending the remaining .0004 BTC. (And this is an example of why micropayments don't work on the Bitcoin network, because a $1 or so transaction fee is pretty expensive when you're sending $4, let alone if you were trying to make a micropayment of $0.50. But that's always why we have Lightning.)

> :warning: **WARNING:** The lower that you set your transaction fee, the longer before your transaction is built into a block. The Bitcoin Fees site lists expected times, from an expected 0 blocks, to 22. Since blocks are built on average every 10 minutes, that's the difference between a few minutes and a few hours! So, choose a transaction fee that's appropriate for what you're sending. Note that you should never drop below the minimum relay fee, which is .0001 BTC.

### Write the Raw Transaction

You're now ready to create the raw transaction. This uses the `createrawtransaction` command, which might look a little intimidating. That's because the `createrawtransaction` command doesn't entirely shield you from the JSON RPC that the bitcoin-cli uses. Instead, you are going to input a JSON array to list the UTXOs that you're spending and a JSON object to list the outputs.

Here's the standard format:
```
$ bitcoin-cli createrawtransaction
'''[
     {
       "txid": "'$your_txid'",
       "vout": '$your_vout'
      }
]'''
'''{
   "'$your_recipient'": bitcoin_amount
 }'''
 ```
 Yeah, there are all kinds of crazy quotes there, but trust that they'll do the right thing. Use `'''` to mark the start and end of the JSON array and the JSON object. Protect normal words like `"this"`, but you don't need to protect normal numbers: `0`. If they're variables, insert single quotes, like `"'$this_word'"` and `'$this_num'`. (Whew. You'll get used to it.)

 Here's a command that creates a raw transaction to send your $utxo to your $recipient
 ```
$ rawtxhex=$(bitcoin-cli createrawtransaction '''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' '''{ "'$recipient'": 0.0004 }''')
$ echo $rawtxhex
02000000013a6e4279b799791049e1826602e84d2e36797e2005887b98c3ecf16b01b7f3610100000000ffffffff01409c0000000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000
```

### Verify Your Raw Transaction

You should next verify your rawtransaction with `decoderawtransaction` to make sure that it will do the right thing.
```
$ bitcoin-cli decoderawtransaction $rawtxhex
{
  "txid": "dcd2d8f0ec5581b806a1fbe00325e1680c4da67033761b478a26895380cc1298",
  "hash": "dcd2d8f0ec5581b806a1fbe00325e1680c4da67033761b478a26895380cc1298",
  "version": 2,
  "size": 85,
  "vsize": 85,
  "weight": 340,
  "locktime": 0,
  "vin": [
    {
      "txid": "61f3b7016bf1ecc3987b8805207e79362e4de8026682e149107999b779426e3a",
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
      "value": 0.00040000,
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
```

Check the `vin`. Are you spending the right transaction? Does it contain the expected amount of money? (Check with `bitcoin-cli gettransaction` and be sure to look at the right `vout`.) Check your `vout`. Are you sending the right amount? Is it going to the right address? Finally, do the math to make sure the money balances. Does the value of the UTXO minus the amount being spent equal the expected transaction fee?

> :information_source:  **NOTE - SEQUENCE:** You may note that each input has a sequence number, set here to  4294967295, which is 0xFFFFFFFF. This is the last frontier of Bitcoin transactions, because it's a standard field in transactions that was originally intended for a specific purpose, but was never fully implemented. So now there's this integer sitting around in transactions that could be repurposed for other uses. And, in fact, it has been. As of this writing there are three different uses for the variable that's called `nSequence` in the Bitcoin Core code: it enables RBF, `nLockTime`, and relative timelocks. If there's nothing weird going on, `nSequence` will be set to 4294967295. Setting it to a lower value signals that special stuff is going on.

### Sign the Raw Transaction

To date, your raw transaction is just something theoretical: you _could_ send it, but nothing has been promised. You have to do a few things to get it out onto the network.

First, you need to sign your raw transaction:
```

$ bitcoin-cli signrawtransactionwithwallet $rawtxhex
{
  "hex": "02000000013a6e4279b799791049e1826602e84d2e36797e2005887b98c3ecf16b01b7f361010000006a4730440220335d15a2a2ca3ce6a302ce041686739d4a38eb0599a5ea08305de71965268d05022015f77a33cf7d613015b2aba5beb03088033625505ad5d4d0624defdbea22262b01210278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132ffffffff01409c0000000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000",
  "complete": true
}
$ signedtx="02000000013a6e4279b799791049e1826602e84d2e36797e2005887b98c3ecf16b01b7f361010000006a4730440220335d15a2a2ca3ce6a302ce041686739d4a38eb0599a5ea08305de71965268d05022015f77a33cf7d613015b2aba5beb03088033625505ad5d4d0624defdbea22262b01210278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132ffffffff01409c0000000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000"
```
Note that we captured the signed hex by hand, rather than trying to parse it out of the JSON object. A software package called "JQ" could do better, as we'll explain in an upcoming interlude.

### Send the Raw Transaction

You've now got a ready-to-go raw transaction, but it doesn't count until you actually put it on the network, which you do with the `sendrawtransaction` command. You'll get back a txid:
```
$ bitcoin-cli sendrawtransaction $signedtx
a1fd550d1de727eccde6108c90d4ffec11ed83691e96e119d842b3f390e2f19a
```
You'll immediately see that the UTXO and its money have been removed from your wallet:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36",
    "vout": 0,
    "address": "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
    "label": "",
    "scriptPubKey": "76a9141b72503639a13f190bf79acf6d76255d772360b788ac",
    "amount": 0.00010000,
    "confirmations": 23,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/1']02fd5740996d853ea51a6904cf03257fc11204b0179f344c49739ec5b20b39c9ba)#62rud39c",
    "safe": true
  },
  {
    "txid": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
    "vout": 0,
    "address": "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
    "label": "",
    "scriptPubKey": "76a9142d573900aa357a38afd741fbf24b075d263ea6e088ac",
    "amount": 0.00022000,
    "confirmations": 6,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/3']0278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132)#nhjc3f8y",
    "safe": true
  }
]

$ bitcoin-cli getbalance
0.00032000
```
Soon `listtransactions` should show a confirmed transaction of category 'send".
```
 {
    "address": "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi",
    "category": "send",
    "amount": -0.00040000,
    "vout": 0,
    "fee": -0.00010000,
    "confirmations": 1,
    "trusted": true,
    "txid": "a1fd550d1de727eccde6108c90d4ffec11ed83691e96e119d842b3f390e2f19a",
    "walletconflicts": [
    ],
    "time": 1592608574,
    "timereceived": 1592608574,
    "bip125-replaceable": "no",
    "abandoned": false
  }
```
You can see that it matches the `txid` and the `recipient` address. Not only does it show the `amount` sent, but it also shows the transaction `fee`. And, it's already received a confirmation, because we offered a fee that would get it swept up into a block quickly.

Congratulations! You're now a few satoshis poorer!

## Summary: Creating a Raw Transaction

When money comes into your Bitcoin wallet, it remains as distinct amounts, called UTXOs. When you create a raw transaction to send that money back out, you use one or more UTXOs to fund it. You then can create a raw transaction, sign it, and send it on the Bitcoin network. However, this is just a foundation: you'll usually need to create a raw transaction with multiple outputs to actually send something on the bitcoin network!

## What's Next?

Step Back from "Sending Bitcoin Transactions" with [Interlude: Using JQ](04_2__Interlude_Using_JQ.md).
