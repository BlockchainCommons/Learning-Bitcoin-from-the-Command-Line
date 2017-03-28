# 3.4: Sending a Raw Transaction to a P2PKH

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

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

## Write a Raw Transaction with One Output

You're now ready to write a simple raw transaction where you're be sending the entirety of a UTXO to another party; that is, you'll be acting like a go-between, taking all of the money you received in one blob and just passing it on.

### Prepare the Raw Transaction

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

### Write the Raw Transaction

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
Check the vin. Are you spending the right transaction? Does it contain the expected amount of money? Check your vout. Are you sending the right amount? Is it going to the right address? Finally, do the math to make sure the money balances. Does the value of the UTXO minus the amount being spent equal the expected transaction fee?

### Understand the Transaction Fee

You'll note that we didn't send the whole .08 BTC to our recipient. That's because you have to pay minor fees to use the Bitcoin network. The amount that you will pay as a fee is always equal to the amount of your input minus the amount of your output. So, you have to decrease your output a little bit from your input to make sure that your transaction goes out.

> **WARNING:** This is very dangerous!! Because you automatically expend all of the amount in the UTXOs that you use, it's critically important to make sure that you know: (1) precisely what UTXOs you're using; (2) exactly how much money they contain; (3) exactly how much money you're sending out; and (4) what the difference is. If you mess up and you use the wrong UTXO (with more money than you thought) or if you send out too little money, the excess is lost. Forever. Don't make that mistake! Know your inputs and outputs _precisely_.

How much should you spend? [Bitcoin Fees](https://bitcoinfees.21.co/) has a nice live assessment. It says that the "fastest and cheapest transaction fee is currently 220 satoshis/byte" and that "For the median transaction size of 226 bytes, this results in a fee of 49,720 satoshis". Since this transaction will have just one input and one output, we decided that was more than enough. So, we subtracted 50,000 satoshis, which is .0005 BTC. .0800 BTC - .0005 BC= .0795, which is what we sent. (Often transactions don't need to be the "fastest" and can get away with much lower transaction fees; we opted not to because we don't want to delay working through this tutorial.)

> **WARNING:** The lower that you set your transaction fee, the longer before your transaction is built into a block. The Bitcoin Fees sites lists expected times, from an expected 0 blocks, to 22. Since blocks are built on average every 10 minutes, that's the difference between a few minutes and a few hours! So, choose a transaction fee that's appropriate for what you're sending. Note that you should never drop below the minimum relay fee, which is .0001 BTC.

### Sign the Raw Transaction

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

### Send the Raw Transaction

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

## Create a Change Address

Our first raw transaction was very simplistic: we sent the entirety of a UTXO to a new address. More frequently, you'll want to send someone an amount of money that doesn't match a UTXO. But you'll recall that the excess money from a UTXO that's not sent to your recipient just becomes a transaction fee. So, how do you send someone just part of a UTXO, while keeping the rest for yourself?

The solution is to create a change address:
```
$ changeaddress=$(bitcoin-cli getrawchangeaddress)
$ echo $changeaddress
mxU9cmhJfkKWDtBspHaA36LkeafEDeaogJ
```
Note that this is a new function: `getrawchangeaddress`. It's largely the same as `getnewaddress` but is optimized for use as a change address in a raw transaction, so it doesn't do things like make entries in your address book.

You now have an additional address inside your wallet that you can use to receive change from a UTXO!

## Write a Raw Transaction with Two Outputs

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

### Set Up Your Variables

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

### Write the Transaction

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
### Finish It Up

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

### Summary: Sending a Raw Transaction to a P2PKH

When money comes into your Bitcoin wallet, it remains as distinct amounts, called UTXOs. When you create a raw transaction to send that money back out, you use one or more UTXOs to fund it. If you want change back from the UTXOs you're spending, you _must_ create a change address to receive the excess funds. You must also define how much money to spend as the transaction fee. Once you've created a raw transaction, you can check it, sign it, and then actually send it to the Bitcoin network.
