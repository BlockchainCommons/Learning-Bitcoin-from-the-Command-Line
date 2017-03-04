# 3.0: Playing with Bitcoin Using bitcoin-cli

This document explains how to begin using Bitcoin from the command line, using bitcoin-cli. 

* [Part One: Verifying Your bitcoin-cli Setup](3_1_Verifying_Your_Bitcoin-CLI_Setup.md)
* [Part Two: Setting Up Your Wallet](3_2_Setting_Up_Your_Wallet.md)
* [Part Three: Receiving a Transaction](3_3_Receiving_a_Transaction.md)

This section presumes that you have a VPS with bitcoin installed, running bitcoind. It also presumes that you are connected to testnet, allowing for access to bitcoins without using real funds. You can either do this by hand, per [2.1: Setting up a Bitcoin-Core VPS by Hand](./2_1_Setting_Up_a_Bitcoin-Core_VPS_by_Hand.md), or do it with a Linode StackScript at Linode.com, per [2.2: Setting up a Bitcoin-Core VPS with StackScript](./2_2_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md).

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

### Interlude: Sending Coins the Easy Way

You're now ready to send some coins, and that's actually quite simple via the command line. You just type `bitcoin-cli sendtoaddress [address] [amount]`. So, to send a little coinage to the address `msoix3SHNr6eRDUJsRSqQwZRhxZnLXhNef` just requires:
```
$ bitcoin-cli sendtoaddress msoix3SHNr6eRDUJsRSqQwZRhxZnLXhNef 0.1
6ad295c280798e9746dcdf7e5a60dfb6219d93bf31aab9b540ce892537c41e0c
```
Make sure the address you write in is where you want the money to go. Make _double_ sure. If you make mistakes in Bitcoin, there's no going back. 

You'll receive a txid back when you issue this command.

> **WARNING:** As noted previously, the bitcoin-cli command generates JSON-RPC commands to talk to the bitcoind. They can be really picky. This is an example: if you list the bitcoin amount without the leading zero (i.e. ".1" instead of "0.1"), then bitcoin-cli will fail with a mysterious message.

You'll be able to see the transaction in your list immediately. 
```
$ bitcoin-cli listtransactions
[
  {
    "account": "",
    "address": "n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf",
    "category": "receive",
    "amount": 0.47000000,
    "label": "",
    "vout": 0,
    "confirmations": 15,
    "blockhash": "00000000fa4fdd22a2c33c6200b68239939ad65af3f1a48ecea25f8200f5d66b",
    "blockindex": 45,
    "blocktime": 1488307692,
    "txid": "88e5d5f3077517d76f5a61491fa52e6aaae078c52bc62d849f09507ef0cfada2",
    "walletconflicts": [
    ],
    "time": 1488307692,
    "timereceived": 1488307696,
    "bip125-replaceable": "no"
  }, 
 
  {
    "account": "",
    "address": "msoix3SHNr6eRDUJsRSqQwZRhxZnLXhNef",
    "category": "send",
    "amount": -0.10000000,
    "label": "",
    "vout": 0,
    "fee": -0.00004520,
    "confirmations": 0,
    "trusted": false,
    "txid": "6ad295c280798e9746dcdf7e5a60dfb6219d93bf31aab9b540ce892537c41e0c",
    "walletconflicts": [
    ],
    "time": 1488321652,
    "timereceived": 1488321652,
    "bip125-replaceable": "unknown",
    "abandoned": false
  }
]
```
However, note that as always it'll take a while for your balances to settle. Be aware that the default transaction fee for `sendtoaddress` is quite low and should probably be increased, as we note in later sections. This means that your transaction might not placed into the first several blocks, until it's reached enough priority. This is usually fine, if you're sending money to someone. It's less fine if you're working through a tutorial, wanting to get to the next step. 

Using the `sendtoaddress` command isn't necessarily that interesting, if you're planning to write your own raw transactions. However, it's a great test so that you can successfully see a transaction leave your machine, taking some of your money with it.

## Part Three: Sending a Raw Transaction to a P2PKH

You're now ready to create Bitcoin raw transactions. This allows you to send money (as in the interlude above) but to craft the transactions precisely as you want. 

This section focuses on paying to addresses that are P2PKH: pay to public-key hashes. In other words: just a normal Bitcoin address, representing some person who wants your money.

## Understand the Bitcoin Transaction

Before you dive into actually creating raw transactions, you should make sure you understand how a Bitcoin transaction works. 

When you receive cash in your Bitcoin wallet, it appears as an individual transaction. Each of these transactions is called a Unspent Transaction Output (UTXO). It doesn't matter if multiple blobs of money came into the same address or to multiple addresses: each UTXO (incoming transaction) remains distinct in your wallet.

When you create a new outgoing transaction, you gather together one or more UTXOs, each of which represents a blob of money that you received. Together their amount must equal what you want to spend _or more_. You use these as inputs into the new transaction. Then, you generate one or more outputs, which give the money represented by the inputs to one or more people. This creates new UTXOs for the recipients, which may then use _those_ to fund future transactions.

Here's the trick: _all of the UTXOs that you gather are spent in full!_ That means that if you want to send just part of the money in a UTXO to someone else, then you also have to generate an additional output that sends the rest back to you!

## List Your Unspent Transactions

In order to create a new raw transaction, you must know what UTXOs you have on-hand to spent. You can determine this information with the `bitcoin-cli listunspent` command:
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
This listing shows three different UTXOs, worth .08, .078, and .078 BTC. Note that each has its own distinct txid and remains distinct in the wallet, even though two of them were sent to the same address.

However, when you spend a UTXO you need more than just the transaction id. That's because each transaction can have multiple outputs! Remember the example of that first bit of money we got from the faucet: some of it went to us, but some went so someone else (probably a change address). The `txid` just refers to the overall transaction, while a `vout` says which of multiple outputs you've received. In this list, two of our UTXOs are the 0th vout of a transaction, and the other is the 1st. This makes a difference!

So txid+vout=UTXO. This will be the foundation of any raw transaction.

### Write a Raw Transaction with One Output

You're now ready to write a simple raw transaction where you're be sending the entirety of a UTXO to another party; that is, you'll be acting like a money launderer, taking all of the money you received in one blob and just passing it on.

#### Prepare the Raw Transaction

For best practices, we'll start out each transaction by carefully recording the txids and vouts that we'll be spending.

In this case, we're going to spend the oldest transaction, because that's the one that's been validated the most:
```
$ utxo_txid="ee9805676271f6244eba94c3d1a48b303a8f8359bf711c630eb6f2ea339d0e72"
$ utxo_vout="0"
```

> **TESTNET vs MAINNET:** Obviously the "validated the most" criteria would matter a lot more on mainnet, where real money is being used.

You should similarly record your recipient address, to make sure there are no problems. We're sending some money back to the TP faucet:
```
$ recipient="n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi"
```
As always, check your variables carefully, to make sure they match!
```
$ echo $utxo_txid
ee9805676271f6244eba94c3d1a48b303a8f8359bf711c630eb6f2ea339d0e72
$ echo $utxo_vout
0
$ echo $recipient
n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi
```
That recipient is particularly important, because if you mess it up, your money is _gone_! So triple check it. 

#### Write the Raw Transaction

You're now ready to create the raw transaction. This uses the `createrawtransaction` command, which might look a little intimidating. That's because the `createrawtransaction` command doesn't entirely shield you from the JSON RPC that the bitcoin-cli uses. Instead, you are going to input a JSON array to list the UTXOs that you're spending and a JSON object to list the outputs.

Here's the standard format:
```
$ bitcoin-cli createrawtransaction 
'''[
     {
       "txid": "'$your_txid'",
       "vout": "'$your_vout'"
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
```
Note that we followed our best practices to immediately save that to variable, because this is another complex result that we don't want to mess up. The result is hex, and here's what it looks like:
```
$ echo $rawtxhex
0100000001720e9d33eaf2b60e631c71bf59838f3a308ba4d1c394ba4e24f67162670598ee0000000000ffffffff01b04e7900000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000
```

#### Verify Your Raw Transaction

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
Check the vin. Are you spending the right transaction? Does it contain the expected amount of money? Check your vout. Are you sending the right amount? Is it going to the right address? Finally, do the math to make sure the money balances. Does the value of the UTXO minus the amount being spent equal the expected transaction fee?

#### Understand the Transaction Fee

You'll note that we didn't send the whole .08 BTC to our recipient. That's because you have to pay minor fees to use the Bitcoin network. The amount that you will pay as a fee is always equal to the amount of your input minus the amount of your output. So, you have to decrease your output a little bit from your input to make sure that your transaction goes out.

> **WARNING:** This is very dangerous!! Because you automatically expend all of the amount in the UTXOs that you use, it's critically important to make sure that you know: (1) precisely what UTXOs you're using; (2) exactly how much money they contain; (3) exactly how much money you're sending out; and (4) what the difference is. If you mess up and you use the wrong UTXO (with more money than you thought) or if you send out too little money, the excess is lost. Forever. Don't make that mistake! Know your inputs and outputs _precisely_.

How much should you spend? [Bitcoin Fees](https://bitcoinfees.21.co/) has a nice live assessment. It says that the "fastest and cheapest transaction fee is currently 220 satoshis/byte" and that "For the median transaction size of 226 bytes, this results in a fee of 49,720 satoshis". Since this transaction will have just one input and one output, we decided that was more than enough. So, we subtracted 50,000 satoshis, which is .0005 BTC. .0800 BTC - .0005 BC= .0795, which is what we sent. (Often transactions don't need to be the "fastest" and can get away with much lower transaction fees; we opted not to because we don't want to delay working through this tutorial.)

> **WARNING:** The lower that you set your transaction fee, the longer before your transaction is built into a block. The Bitcoin Fees sites lists expected times, from an expected 0 blocks, to 22. Since blocks are built on average every 10 minutes, that's the difference between a few minutes and a few hours! So, choose a transaction fee that's appropriate for what you're sending. Note that you should never drop below the minimum relay fee, which is .0001 BTC.

#### Sign the Raw Transaction

To date, your raw transaction is just something theoretical: you _could_ send it, but nothing has been promised. You have to do a few things to get it out onto the network.

First, you need to sign your raw transaction:
```
$ bitcoin-cli signrawtransaction $rawtxhex
{
  "hex": "0100000001720e9d33eaf2b60e631c71bf59838f3a308ba4d1c394ba4e24f67162670598ee000000006b483045022100d8f17dadc2501596f75f2c90b8279130e588638d4f7a4f7d5ebb10fea15252f702200ceb164e81335c430893780d06cfe194c36acec26886f180408e3ac4a7d2292f0121035de6239e70523c8f392e32f98e65f6ef704c4b6b0df994e407212b839bf51048ffffffff01b04e7900000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000",
  "complete": true
}
$ signedtx="0100000001720e9d33eaf2b60e631c71bf59838f3a308ba4d1c394ba4e24f67162670598ee000000006b483045022100d8f17dadc2501596f75f2c90b8279130e588638d4f7a4f7d5ebb10fea15252f702200ceb164e81335c430893780d06cfe194c36acec26886f180408e3ac4a7d2292f0121035de6239e70523c8f392e32f98e65f6ef704c4b6b0df994e407212b839bf51048ffffffff01b04e7900000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000"
```
Note that we captured the signed hex by hand, rather than trying to parse it out of the JSON object. A software package called "JQ" could do better, but for the moment we're keeping things simple and clear.

#### Send the Raw Transaction

You've now got a ready-to-go raw transaction, but it doesn't count until you actually put it on the network, which you do with the `sendrawtransaction` command. You'll get back a txid:
```
$ bitcoin-cli sendrawtransaction $signedtx
bf02068e3d2a99542c6a9ad07d915f1664374e911d809afe156dbacc46a5183e
```
You'll immediately be able to see that the UTXO and its money have been removed from your account:
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
Finally, `listtransactions` should soon show a confirmed transaction of category 'send".
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

### Create a Change Address

Our first raw transaction was very simplistic: we sent the entirety of a UTXO to a new address. More frequently, you'll want to send someone an amount of money that doesn't match a UTXO. But you'll recall that the excess money from a UTXO that's not sent to your recipient just becomes a transaction fee. So, how do you send someone just part of a UTXO, while keeping the rest for yourself?

The solution is to create a change address:
```
$ changeaddress=$(bitcoin-cli getnewaddress)
$ echo $changeaddress
mxU9cmhJfkKWDtBspHaA36LkeafEDeaogJ
```
You now have an additional address inside your wallet that you can use to receive change from a UTXO!

### Write a Raw Transaction with Two Outputs

You're now ready to write a more complex raw transaction. It will be based on the two unspent UTXOs still in our wallet:
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
    "confirmations": 237,
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
    "confirmations": 237,
    "spendable": true,
    "solvable": true
  }
]
```
We want to send 0.1 BTC back to the TP faucet. This requires combining our two UTXOs, because neither contains quite that amount of bitcoin. It also requires sending to two addresses: one for the TP faucet (n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi) and one as a change address (mxU9cmhJfkKWDtBspHaA36LkeafEDeaogJ). In other words, we're going to have two inputs and two outputs in our raw transaction

#### Set Up Your Variables

We've already have `$changeaddress` and `$recipient` variables from our previous examples:
```
$ echo $changeaddress
mxU9cmhJfkKWDtBspHaA36LkeafEDeaogJ
$ echo $recipient
n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi
```
We also need to record the txid and vout for each of our two UTXOs:
```
$ utxo_txid_1="c1abb6951e6a9aae7e384412b69b69e59c10daac9397d01d0c52b7bc6278d589"
$ utxo_vout_1="1"
$ utxo_txid_2="ab7ca727055b812df882298f4e6e10ec699fb6250d843c813623171781f896d8"
$ utxo_vout_2="0"

$ echo $utxo_txid_1
c1abb6951e6a9aae7e384412b69b69e59c10daac9397d01d0c52b7bc6278d589
$ echo $utxo_vout_1
1
$ echo $utxo_txid_2
ab7ca727055b812df882298f4e6e10ec699fb6250d843c813623171781f896d8
$ echo $utxo_vout_2
0
```

#### Write the Transaction

Writing the new raw transaction is surprisingly simple. All you need to do is include an additional, comma-separated JSON object in the JSON array of inputs and an additional, comma-separated key-value pair in the JSON object of outputs.

Here's the example. Take a look at it carefully to understand where the inputs end the outputs begin:
```
rawtxhex2=$(bitcoin-cli createrawtransaction '''[ { "txid": "'$utxo_txid_1'", "vout": '$utxo_vout_1' }, { "txid": "'$utxo_txid_2'", "vout": '$utxo_vout_2' } ]''' '''{ "'$recipient'": 0.1, "'$changeaddress'": 0.0555 }''')
```
We were _very_ careful figuring out our money math. These two UTXOs contain .156 BTC. After sending .1 BTC, we'll have .056 BTC left. We again chose .0005 BTC for the transaction fee: this transaction about twice as big, but it's still under the average transaction size quoted by Bitcoin Fees, so that should be more than enough to make this transaction go through quickly. To accomodate that fee, we set our change to .0555 BTC. If we'd messed up our math and instead set our change to .051 BTC, that additional .0045 BTC would be lost. That's $5 or $6 gone to the miners when it didn't need to be. If we'd forgot to make change at all, then the whole .056 ($70!) would have disappeared. So, again, _be careful_. 

Back to the raw transaction itself: we'll be honest, it took us a few times to get all the commas, quotes, and brackets in the right places, writing from the command line. We got JSON errors when we didn't format things quite right; when we fixed the errors, our variable (correctly) got filled with the hex for the raw transaction. So, as always, be careful here too, and check your work:
```
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
#### Finish It Up

You can now sign, seal, and deliver your transaction, and it's yours (and the faucet's):
```
$ bitcoin-cli signrawtransaction $rawtxhex2
{
  "hex": "010000000289d57862bcb7520c1dd09793acda109ce5699bb61244387eae9a6a1e95b6abc1010000006a4730440220194cf452c76630419b90ef16bc9c0f31bd3187b97357eff6b610a3054968579b02200bcdb125ac182bda338c1a64d7887efd3e4eaf1a8e7f517eb7630d4dc258bf88012102f9006968b057f5212752964546d4c259f359601d00de32ec027fd84760bb4a46ffffffffd896f88117172336813c840d25b69f69ec106e4e8f2982f82d815b0527a77cab000000006a47304402201e65fe69dd231f73a9f493ca6c49503d51a766cf27be1281c9a4eefd04b4ac1c022003750d1ec93ef7be552aa55656c1860c986ef04a8733587f4f7be38707cccaf2012102f9006968b057f5212752964546d4c259f359601d00de32ec027fd84760bb4a46ffffffff0280969800000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88acb0af5400000000001976a914b9f2579814712f82a3dadfd73e0356339c6705b488ac00000000",
  "complete": true
}
$ signedtx2="010000000289d57862bcb7520c1dd09793acda109ce5699bb61244387eae9a6a1e95b6abc1010000006a4730440220194cf452c76630419b90ef16bc9c0f31bd3187b97357eff6b610a3054968579b02200bcdb125ac182bda338c1a64d7887efd3e4eaf1a8e7f517eb7630d4dc258bf88012102f9006968b057f5212752964546d4c259f359601d00de32ec027fd84760bb4a46ffffffffd896f88117172336813c840d25b69f69ec106e4e8f2982f82d815b0527a77cab000000006a47304402201e65fe69dd231f73a9f493ca6c49503d51a766cf27be1281c9a4eefd04b4ac1c022003750d1ec93ef7be552aa55656c1860c986ef04a8733587f4f7be38707cccaf2012102f9006968b057f5212752964546d4c259f359601d00de32ec027fd84760bb4a46ffffffff0280969800000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88acb0af5400000000001976a914b9f2579814712f82a3dadfd73e0356339c6705b488ac00000000"
$ bitcoin-cli sendrawtransaction $signedtx2
18f1a791e3f7735dbe1c92b86645ba9b1f62b8044f61efd76d082a084982c9ae
```
#### Wait

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

### Optional: Write a Raw Transaction with Automatic Funding

The purpose of this tutorial is to show you the very basics of Bitcoin raw transactions, so that you can work at things at a fundamental level. If you were writing a wallet or something other Bitcoin software, you'd probably want to do things exactly as described here. However, if you were (satoshi forfend!) regularly sending bitcoins about through raw transactions created by hand, then you'd want to have a little better insurance that you weren't making mistakes.

The bitcoin-cli accomodates this with a `fundrawtransaction` command. First, you have to make sure that your ~/.bitcoin/bitcoin.conf file has some rational variables for calculating transaction fees. We've been quite aggressive with fees in this tutorial, to make sure the examples finish quickly, but the following would allow for cheaper transmissions that might take a few hours:
```
 paytxfee=0.0001
 mintxfee=0.0001
 txconfirmtarget=25
 ```
 With that in hand (and in bitcoin.conf) you can use `createrawtransaction` with just your output(s), then run `fundrawtransaction` on the resulting hex:
 ```
$ unfinishedtx=$(bitcoin-cli createrawtransaction '''[]''' '''{ "'$recipient'": 0.04 }''')
$ bitcoin-cli fundrawtransaction $unfinishedtx
{
  "hex": "0100000001aec98249082a086dd7ef614f04b8621f9bba4566b8921cbe5d73f7e391a7f1180100000000feffffff0208951700000000001976a914f26e11dc0fcc79fe76fca1d24d7588798922ca7488ac00093d00000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000",
  "changepos": 0,
  "fee": 0.00004520
}
$ rawtxhex3="0100000001aec98249082a086dd7ef614f04b8621f9bba4566b8921cbe5d73f7e391a7f1180100000000feffffff0208951700000000001976a914f26e11dc0fcc79fe76fca1d24d7588798922ca7488ac00093d00000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000"
```
It seems like magic, but when you examine the raw transaction, you'll see it works:
```
$ bitcoin-cli decoderawtransaction $rawtxhex3
{
  "txid": "3099716446e0761e823077fc2f33c158b1dbaa7c157cb03c25a7cf6b7ac4921d",
  "hash": "3099716446e0761e823077fc2f33c158b1dbaa7c157cb03c25a7cf6b7ac4921d",
  "size": 119,
  "vsize": 119,
  "version": 1,
  "locktime": 0,
  "vin": [
    {
      "txid": "18f1a791e3f7735dbe1c92b86645ba9b1f62b8044f61efd76d082a084982c9ae",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967294
    }
  ],
  "vout": [
    {
      "value": 0.01545480,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 f26e11dc0fcc79fe76fca1d24d7588798922ca74 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914f26e11dc0fcc79fe76fca1d24d7588798922ca7488ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "n3cogFw4y3A1LJgRz2G97otEGT7BK8ca3D"
        ]
      }
    }, 
    {
      "value": 0.04000000,
      "n": 1,
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
As you can see, `fundrawtransaction` chose UTXOs for the inputs, it created a change address, it calculated a (low) transaction fee based on the variables in the bitcoin.conf file, and it sent the amount of the UTXO minus the tx fee back to the change address.

You can even verify that the new address belongs to you:
```
$ bitcoin-cli validateaddress n3cogFw4y3A1LJgRz2G97otEGT7BK8ca3D
{
  "isvalid": true,
  "address": "n3cogFw4y3A1LJgRz2G97otEGT7BK8ca3D",
  "scriptPubKey": "76a914f26e11dc0fcc79fe76fca1d24d7588798922ca7488ac",
  "ismine": true,
  "iswatchonly": false,
  "isscript": false,
  "pubkey": "0347251fdd4fe8f9c66c0c137cc641e864dd27fc5dc0c8d0c85ff884d2a3fa1574",
  "iscompressed": true,
  "hdkeypath": "m/0'/0'/4'",
  "hdmasterkeyid": "75807dcb1226537ceb54c38c4a75a61407fdd02d"
}
```
At this point you could sign and send the transaction as usual ... then wait much longer for it to come back due to the lower transaction fees!
```
$ signedtx3="0100000001aec98249082a086dd7ef614f04b8621f9bba4566b8921cbe5d73f7e391a7f118010000006b483045022100a9b1454114bb2c04b51619eb5a00ad391605920ae801405b6191a64d1fb1e6e8022054def9ccbd75cb7929279cfef73ac573cdac7a325a1e3c8f43e139a1340b5d4b012103f7c794378db1c070b07d74f427f394f8a5d53f1abe1d2dab100d5a7a49db8785feffffff0208951700000000001976a914f26e11dc0fcc79fe76fca1d24d7588798922ca7488ac00093d00000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000"
$ bitcoin-cli sendrawtransaction $signedtx3
ecfc625fff594683e90d21618b64f44f7046c55bcda6468c1c37c1abe8b83913
```
And voila!
```
$ bitcoin-cli getbalance
0.01545480
```
We will _not_ be using this technique in the rest of the tutorial.

### Summary: Sending a Raw Transaction to a P2PKH

When money comes into your Bitcoin wallet, it remains as distinct amounts, called UTXOs. When you create a raw transaction to send that money back out, you use one or more UTXOs to fund it. If you want change back from the UTXOs you're spending, you _must_ create a change address to receive the excess funds. You must also define how much money to spend as the transaction fee. Once you've created a raw transaction, you can check it, sign it, and then actually send it to the Bitcoin network.

## Part Four: Sending a Raw Transaction to an OP_RETURN

P2PKHs are the simplest sort of Bitcoin recipient ... except perhaps the OP_RETURN. That's because an OP_RETURN is basically a null: an invalid output. Why would you use one? To store data on the blockchain: up to 80 bytes. 

This opens up whole new possibilities for the blockchain, because you can embed data that proves that certain things happened at certain times. Though there is some controversy over using the Bitcoin blockchain in this way, various organizations have used OP_RETURNs for proof of existence, for copyright, to color coins, and [for other purposes](https://en.bitcoin.it/wiki/OP_RETURN). Though 80 bytes might not seem a lot, it can be quite effective if OP_RETURNs are used to store hashes of the actual data.

### Create Your OP_RETURN Data

The first thing you need to do is create the 80 bytes (or less) of data that you'll be recording in your OP_RETURN. This might be as simple as preparing a message or you might be hasing existing data. For example, `md5sum` produces 128 bits of data, which is 16 bytes, well under the limits:
```
$ md5sum mycontract.jpg 
3b110a164aa18d3a5ab064ba93fdce62  mycontract.jpg
$ op_return_data="3b110a164aa18d3a5ab064ba93fdce62"
```

### Prepare Some Money

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
$ changeaddress_4=$(bitcoin-cli getnewaddress)
```

### Write A Rawtransaction

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

### Check Your OP_RETURN

Again, remember that you can look at this transaction using a blockchain explorer: [https://live.blockcypher.com/btc-testnet/tx/ed445dc970bb40b17c207109e19a37b2be301acb474ccd30680c431cb681bce2/](https://live.blockcypher.com/btc-testnet/tx/ed445dc970bb40b17c207109e19a37b2be301acb474ccd30680c431cb681bce2/)

You may note a warning about the data being in an "unknown protocol". If you were designing some regular use of OP_RETURN data, you'd probably mark it with a special prefix, to mark that protocol. Then, the actual OP_RETURN data might be something like "CONTRACTS3b110a164aa18d3a5ab064ba93fdce62". This example didn't use a prefix to avoid muddying the data space.

[Coinsecrets](http://coinsecrets.org/) offers another interesting way to look at OP_RETURN data. It does its best to keep abreast of protocols, so that it can tell you who is doing what in the blockchain. Here's thisour transaction there: [https://www.blocktrail.com/tBTC/tx/ed445dc970bb40b17c207109e19a37b2be301acb474ccd30680c431cb681bce2](https://www.blocktrail.com/tBTC/tx/ed445dc970bb40b17c207109e19a37b2be301acb474ccd30680c431cb681bce2)

### Summary: Sending a Raw Transaction to an OP_RETURN

You can use an OP_RETURN output to store up to 80 bytes of data on the blockchain. You do this with the 'data' codeword for a 'vout'. You still have to send money along too, but you just send it back to a change address, minus a transaction fee.

### Interlude: Using JQ



