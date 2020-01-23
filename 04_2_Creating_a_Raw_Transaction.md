# 4.2 Creating a Raw Transaction

> :information_source: **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

You're now ready to create Bitcoin raw transactions. This allows you to send money but to craft the transactions as precisely as you want. This first section focuses on a simple one-input, one-output transaction. This sort of transaction _isn't_ actually that useful, because you're rarely going to want to send all of your money to one person (unless you're actually just forwarding it on, such as if you're moving things from one wallet to another). Thus, we don't label this section as a way to send money. It's just a foundational stepping stone to _actually_ sending money with a raw transaction.

## Understand the Bitcoin Transaction

Before you dive into actually creating raw transactions, you should make sure you understand how a Bitcoin transaction works. It's all about the UTXOs.

_What is a UTXO?_ When you receive cash in your Bitcoin wallet, it appears as an individual transaction. Each of these transactions is called a Unspent Transaction Output (UTXO). It doesn't matter if various payments were made to the same address or to multiple addresses: each incoming transaction remains distinct in your wallet as a UTXO.

When you create a new outgoing transaction, you gather together one or more UTXOs, each of which represents a blob of money that you received. You use these as inputs for a new transaction. Together their amount must equal what you want to spend _or more_. Then, you generate one or more outputs, which give the money represented by the inputs to one or more people. This creates new UTXOs for the recipients, which may then use _those_ to fund future transactions.

Here's the trick: _all of the UTXOs that you gather are spent in full!_ That means that if you want to send just part of the money in a UTXO to someone else, then you also have to generate an additional output that sends the rest back to you! For now, we won't worry about that, but the use of a change address will be vital when moving on from the theory of this chapter to more practical transactions.

## List Your Unspent Transactions

In order to create a new raw transaction, you must know what UTXOs you have on-hand to spend. You can determine this information with the `bitcoin-cli listunspent` command:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "ee9805676271f6244eba94c3d1a48b303a8f8359bf711c630eb6f2ea339d0e72",
    "vout": 0,
    "address": "mrS1ypy2pCweh2nBpkMD7r2T3Zj344wxaY",
    "account": "",
    "scriptPubKey": "76a91477ba616a2778b05a5fd73c7449964050fd1a6fd288ac",
    "amount": 0.08000000,
    "confirmations": 2,
    "spendable": true,
    "solvable": true
  },
  {
    "txid": "c1abb6951e6a9aae7e384412b69b69e59c10daac9397d01d0c52b7bc6278d589",
    "vout": 1,
    "address": "mygxipnJUsBgFvscKAaoxDdE8aCmHhRfTZ",
    "account": "",
    "scriptPubKey": "76a914c756c7bd67bf83d83c04e3dc6fd1ff0c6fe8ea9888ac",
    "amount": 0.07800000,
    "confirmations": 1,
    "spendable": true,
    "solvable": true
  },
  {
    "txid": "ab7ca727055b812df882298f4e6e10ec699fb6250d843c813623171781f896d8",
    "vout": 0,
    "address": "mygxipnJUsBgFvscKAaoxDdE8aCmHhRfTZ",
    "account": "",
    "scriptPubKey": "76a914c756c7bd67bf83d83c04e3dc6fd1ff0c6fe8ea9888ac",
    "amount": 0.07800000,
    "confirmations": 1,
    "spendable": true,
    "solvable": true
  }
]
```
This listing shows three different UTXOs, worth .08, .078, and .078 BTC. Note that each has its own distinct txid and remains distinct in the wallet, even though two of them were sent to the same address. (Bad practice! But used as an example here to show that transaction stay distinct, no matter what!)

When you want to spend a UTXO, it's not sufficient to just know the transaction id. That's because each transaction can have multiple outputs! Remember that first chunk of money that the faucet sent us? In the transaction, some money went to us and some went to someone else. The `txid` refers to the overall transaction, while a `vout` says which of multiple outputs you've received. In this list, two of our UTXOs are the 0th vout of a transaction, and the other is the 1st. This makes a difference!

So, txid+vout=UTXO. This will be the foundation of any raw transaction.

## Write a Raw Transaction with One Output

You're now ready to write a simple, example raw transaction that shows how to send the entirety of a UTXO to another party. As noted, this is not necessarily a very realistic real-world case.

> **WARNING:** It is very easy to lose money with a raw transaction. Consider all instructions on sending bitcoins via raw transactions to be _very_, _very_ dangerous. Whenever you're actually sending real money to other people, you should instead use one of the other methods explained in this chapter. Creating raw transactions is extremely useful if you're writing bitcoin programs, but _only_ when you're writing bitcoin programs.

### Prepare the Raw Transaction

For best practices, we'll start out each transaction by carefully recording the txids and vouts that we'll be spending.

In this case, we're going to spend the oldest transaction, worth .08 BTC, because that's the one that's been validated the most:
```
$ utxo_txid="ee9805676271f6244eba94c3d1a48b303a8f8359bf711c630eb6f2ea339d0e72"
$ utxo_vout="0"
```

> :link: **TESTNET vs MAINNET:** Obviously the "validated-the-most" criteria would matter a lot more on mainnet, where real money is being used.

You should similarly record your recipient address, to make sure you have it right. We're again sending some money back to the TP faucet:
```
$ recipient="n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi"
```
As always, check your variables carefully, to make sure they're what you expect!
```
$ echo $utxo_txid
ee9805676271f6244eba94c3d1a48b303a8f8359bf711c630eb6f2ea339d0e72
$ echo $utxo_vout
0
$ echo $recipient
n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi
```
That recipient is particularly important, because if you mess it up, your money is _gone_! So triple check it.

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
 Yeah, there are all kinds of crazy quotes there, but trust that they'll do the right thing. Use `'''` to mark the start and end of the JSON array and the JSON object. Protect normal words like `"this"` and normal numbers like `0`. If they're variables, insert single quotes, like `"'$this_word'"` and `'$this_num'`. (Whew. You'll get used to it.)

 Here's a command that creates a raw transaction to send your $utxo to your $recipient
 ```
$ rawtxhex=$(bitcoin-cli createrawtransaction '''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' '''{ "'$recipient'": 0.0795 }''')
$ echo $rawtxhex
0100000001720e9d33eaf2b60e631c71bf59838f3a308ba4d1c394ba4e24f67162670598ee0000000000ffffffff01b04e7900000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000
```

### Understand the Transaction Fee

You'll note that we didn't send the whole .08 BTC to our recipient. That's because of the transaction fee, which is _implicit_ when you send a raw transaction: the amount that you will pay as a fee is always equal to the amount of your input minus the amount of your output. So, you have to decrease your output a little bit from your input to make sure that your transaction goes out.

> :warning: **WARNING:** This is the very dangerous part of raw transactions!! Because you automatically expend all of the amount in the UTXOs that you use, it's critically important to make sure that you know: (1) precisely what UTXOs you're using; (2) exactly how much money they contain; (3) exactly how much money you're sending out; and (4) what the difference is. If you mess up and you use the wrong UTXO (with more money than you thought) or if you send out too little money, the excess is lost. Forever. Don't make that mistake! Know your inputs and outputs _precisely_. Or better, don't use raw transactions except as part of a carefully considered and triple-checked program.

_How much should you spend on transaction fees?_ [Bitcoin Fees](https://bitcoinfees.21.co/) has a nice live assessment. It says that the "fastest and cheapest transaction fee is currently 220 satoshis/byte" and that "For the median transaction size of 226 bytes, this results in a fee of 49,720 satoshis".

That basic info is what we used to construct our raw transaction. We just subtracted 50,000 satoshis, which is .0005 BTC,  from the amount we were sending: .0800 BTC - .0005 BC= .0795, which is what we sent. (Often transactions don't need to be the "fastest" and can get away with much lower transaction fees; we opted not to because we don't want to delay working through this tutorial.)

> :warning: **WARNING:** The lower that you set your transaction fee, the longer before your transaction is built into a block. The Bitcoin Fees sites lists expected times, from an expected 0 blocks, to 22. Since blocks are built on average every 10 minutes, that's the difference between a few minutes and a few hours! So, choose a transaction fee that's appropriate for what you're sending. Note that you should never drop below the minimum relay fee, which is .0001 BTC.

### Verify Your Raw Transaction

You should next verify your rawtransaction with `decoderawtransaction` to make sure that it will do the right thing.
```
$ bitcoin-cli decoderawtransaction $rawtxhex
{
  "txid": "6d39d612fe382eaab9897d3ea4d5e44233d8acef40e63fe0f06df785fd7a0c45",
  "hash": "6d39d612fe382eaab9897d3ea4d5e44233d8acef40e63fe0f06df785fd7a0c45",
  "size": 85,
  "vsize": 85,
  "version": 1,
  "locktime": 0,
  "vin": [
    {
      "txid": "ee9805676271f6244eba94c3d1a48b303a8f8359bf711c630eb6f2ea339d0e72",
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
      "value": 0.07950000,
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
Check the `vin`. Are you spending the right transaction? Does it contain the expected amount of money? Check your vout. Are you sending the right amount? Is it going to the right address? Finally, do the math to make sure the money balances. Does the value of the UTXO minus the amount being spent equal the expected transaction fee?

> **SEQUENCE NOTE (0):** You may note that each input has a sequence number, set here to  4294967295, which is 0xFFFFFFFF. This is the last frontier of Bitcoin transactions, because it's a standard field in transactions that was originally intended for a specific purpose, but was never fully implemented. So now there's this integer sitting around in transactions that could be repurposed for other uses. And, in fact, it has been. As of this writing there are three different uses for the variable that's called `nSequence` in the Bitcoin Core code: it enables RBF, `nLockTime`, and relative timelocks. If there's nothing weird going on, `nSequence` will be set to 4294967295. Setting it to a lower value signals that special stuff is going on.

### Sign the Raw Transaction

To date, your raw transaction is just something theoretical: you _could_ send it, but nothing has been promised. You have to do a few things to get it out onto the network.

First, you need to sign your raw transaction:
```

$ bitcoin-cli signrawtransactionwithwallet $rawtxhex
{
  "hex": "0100000001720e9d33eaf2b60e631c71bf59838f3a308ba4d1c394ba4e24f67162670598ee000000006b483045022100d8f17dadc2501596f75f2c90b8279130e588638d4f7a4f7d5ebb10fea15252f702200ceb164e81335c430893780d06cfe194c36acec26886f180408e3ac4a7d2292f0121035de6239e70523c8f392e32f98e65f6ef704c4b6b0df994e407212b839bf51048ffffffff01b04e7900000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000",
  "complete": true
}
$ signedtx="0100000001720e9d33eaf2b60e631c71bf59838f3a308ba4d1c394ba4e24f67162670598ee000000006b483045022100d8f17dadc2501596f75f2c90b8279130e588638d4f7a4f7d5ebb10fea15252f702200ceb164e81335c430893780d06cfe194c36acec26886f180408e3ac4a7d2292f0121035de6239e70523c8f392e32f98e65f6ef704c4b6b0df994e407212b839bf51048ffffffff01b04e7900000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000"
```
Note that we captured the signed hex by hand, rather than trying to parse it out of the JSON object. A software package called "JQ" could do better, as we'll explain in an upcoming interlude.

### Send the Raw Transaction

You've now got a ready-to-go raw transaction, but it doesn't count until you actually put it on the network, which you do with the `sendrawtransaction` command. You'll get back a txid:
```
$ bitcoin-cli sendrawtransaction $signedtx
bf02068e3d2a99542c6a9ad07d915f1664374e911d809afe156dbacc46a5183e
```
You'll immediately see that the UTXO and its money have been removed from your account:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "c1abb6951e6a9aae7e384412b69b69e59c10daac9397d01d0c52b7bc6278d589",
    "vout": 1,
    "address": "mygxipnJUsBgFvscKAaoxDdE8aCmHhRfTZ",
    "account": "",
    "scriptPubKey": "76a914c756c7bd67bf83d83c04e3dc6fd1ff0c6fe8ea9888ac",
    "amount": 0.07800000,
    "confirmations": 12,
    "spendable": true,
    "solvable": true
  },
  {
    "txid": "ab7ca727055b812df882298f4e6e10ec699fb6250d843c813623171781f896d8",
    "vout": 0,
    "address": "mygxipnJUsBgFvscKAaoxDdE8aCmHhRfTZ",
    "account": "",
    "scriptPubKey": "76a914c756c7bd67bf83d83c04e3dc6fd1ff0c6fe8ea9888ac",
    "amount": 0.07800000,
    "confirmations": 12,
    "spendable": true,
    "solvable": true
  }
]
$ bitcoin-cli getbalance
0.15600000
```
Soon `listtransactions` should show a confirmed transaction of category 'send".
```
 {
    "account": "",
    "address": "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi",
    "category": "send",
    "amount": -0.07950000,
    "vout": 0,
    "fee": -0.00050000,
    "confirmations": 1,
    "blockhash": "0000000000000dd6f6f455be7eecaf8055bb61d5d18d142d75bcdf8aa6d81456",
    "blockindex": 3,
    "blocktime": 1488410944,
    "txid": "bf02068e3d2a99542c6a9ad07d915f1664374e911d809afe156dbacc46a5183e",
    "walletconflicts": [
    ],
    "time": 1488410872,
    "timereceived": 1488410872,
    "bip125-replaceable": "no",
    "abandoned": false
  }
```
You can see that it matches the txid and the recipient address. Not only does it show the amount sent, but it also shows the transaction fee. And, it's already received a confirmation, because we offered a fee that would get it swept up into a block quickly.

Congratulations! You're now a few satoshis poorer!

## Summary: Creating a Raw Transaction

When money comes into your Bitcoin wallet, it remains as distinct amounts, called UTXOs. When you create a raw transaction to send that money back out, you use one or more UTXOs to fund it. You then can create a raw transaction, sign it, and send it on the Bitcoin network. However, this is just a foundation: you'll usually need to create a raw transaction with multiple outputs to actually send something on the bitcoin network!

## What's Next?

Step Back from "Sending Bitcoin Transactions" with [Interlude: Using JQ](04_2__Interlude_Using_JQ.md).
