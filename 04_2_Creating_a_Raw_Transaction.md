# 4.2 Creating a Raw Transaction

You're now ready to create Bitcoin raw transactions. This allows you to send money but to craft the transactions as precisely as you want. This first section focuses on a simple one-input, one-output transaction. This sort of transaction _isn't_ actually that useful, because you're rarely going to want to send all of your money to one person (unless you're actually just forwarding it on, such as if you're sweeping things from one wallet to another). Therefore, we don't label this section as a way to send money. It's just a foundational stepping stone to _actually_ sending money with a raw transaction.

## Understand the Bitcoin Transaction

Before you dive into actually creating raw transactions, you should make sure you understand how a Bitcoin transaction works. It's all about the UTXOs.

> 📖 ***What is a UTXO?*** When you receive cash in your Bitcoin wallet, it appears as an individual transaction: a singular blob. Each of these transactions is called a Unspent Transaction Output (UTXO). It doesn't matter if various payments were made to the same address or to multiple addresses: each incoming transaction remains distinct in your wallet as a UTXO.

When you create a new outgoing transaction, you gather together one or more UTXOs, each of which represents a blob of money that you received. You use these as inputs for a new transaction. Together their amount must equal what you want to spend _or more_. Then, you generate one or more outputs, which give the money represented by the inputs to one or more people. This creates new UTXOs for the recipients, which may then use _those_ to fund future transactions.

Here's the trick: _all of the UTXOs that you gather are spent in full!_ That means that if you want to send just part of the money in a UTXO to someone else, then you also have to generate an additional output that sends the rest back to you! For now, we won't worry about that, but the use of a change address will be vital when moving on from the theory of this chapter to more practical transactions.

## List Your Unspent Transactions

In order to create a new raw transaction, you must know what UTXOs you have on-hand to spend. You can determine this information by returning to an old favorite, `bitcoin-cli listunspent` command:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "f67ffd9acd5e264c3fcce9b96f4f713e56adf99e65c1a9a55353d9b9706629fd",
    "vout": 0,
    "address": "tb1qr2yek5wu6ce245uexcf624laphdwwu4qlhhggm",
    "scriptPubKey": "00141a899b51dcd632aad3993613a557fd0ddae772a0",
    "amount": 0.01895537,
    "confirmations": 1,
    "spendable": true,
    "solvable": true,
    "desc": "wpkh([b8309bae/84h/1h/0h/1/1]03c548187125947845c252fb14726ff3ce688d1b977235e9b8506c4ad64dfe1ea3)#dutca0cf",
    "parent_descs": [
      "wpkh([b8309bae/84h/1h/0h]tpubDDpSvPDUjstxFUEWzHkaL4qykf8vjNCspm8SZ26Z1wgPFbd63AdYrn4bDpEGPT1giJ6gcLW8Xou8fnhi35DJrUza9ikgu5dg2mDkd8jQpA6/1/*)#eeegkjmk"
    ],
    "safe": true
  },
  {
    "txid": "f67ffd9acd5e264c3fcce9b96f4f713e56adf99e65c1a9a55353d9b9706629fd",
    "vout": 1,
    "address": "tb1q8lpqzydarurhsqttvw2qkf2qjxrwdv0kkej2ux",
    "label": "",
    "scriptPubKey": "00143fc20111bd1f0778016b63940b25409186e6b1f6",
    "amount": 0.00100000,
    "confirmations": 1,
    "spendable": true,
    "solvable": true,
    "desc": "wpkh([b8309bae/84h/1h/0h/0/4]02b412b4329e450c854293c4c4f9d6e716aa9b6f7c7aa622500ddb15f28f215e04)#av5hsrfr",
    "parent_descs": [
      "wpkh([b8309bae/84h/1h/0h]tpubDDpSvPDUjstxFUEWzHkaL4qykf8vjNCspm8SZ26Z1wgPFbd63AdYrn4bDpEGPT1giJ6gcLW8Xou8fnhi35DJrUza9ikgu5dg2mDkd8jQpA6/0/*)#gduft8tw"
    ],
    "safe": true
  }
]
```
This listing shows two different UTXOs, worth .01895537 and .00100000 BTC. Each UTXO will be distinct in your wallet. Most will have different transaction IDs (`txid`), though in this case the funds came from the same transaction, but they're different outputs (`vout`). In other words `txid`+`vout`=UTXO. You'll need to know both of them to spend a transaction.

## Write a Raw Transaction with One Output

You're now ready to write a simple, example raw transaction that shows how to send the entirety of a UTXO to another party. As noted, this is not necessarily a very realistic real-world case.

> ⚠️ **WARNING: Raw Transactions are Dangerous!** It is very easy to lose money with a raw transaction. Consider all instructions on sending Bitcoins via raw transactions to be _very_, _very_ dangerous. Whenever you're actually sending real money to other people, you should instead use one of the other methods explained in this chapter. Creating raw transactions is extremely useful if you're writing Bitcoin programs, but _only_ when you're writing Bitcoin programs. (For example: in writing this example for one version of this tutorial, we accidentally spent the wrong transaction, even though it had about 10x as much value. Almost all of that was lost to the miners.)

### Prepare the Raw Transaction

For best practices, we'll start out each transaction by carefully recording the txids and vouts that we'll be spending.

In this case, we're going to be spending the UTXO worth 0.00100000 BTC. 
```
$ utxo_txid="f67ffd9acd5e264c3fcce9b96f4f713e56adf99e65c1a9a55353d9b9706629fd"
$ utxo_vout="1"
```

You should similarly record your recipient address, to make sure you have it right. This will send the funds to a new address:
```
$ recipient="tb1q7xkh94r24zw4uc3wg2amkagxfm4an5gpwrgsnl"
```
As always, check your variables carefully, to make sure they're what you expect!
```
$ echo $utxo_txid
f67ffd9acd5e264c3fcce9b96f4f713e56adf99e65c1a9a55353d9b9706629fd
$ echo $utxo_vout
1
$ echo $recipient
tb1q7xkh94r24zw4uc3wg2amkagxfm4an5gpwrgsnl
```
That recipient is particularly important, because if you mess it up, your money is _gone_! (And as we already saw, choosing the wrong transaction can result in lost money!) So triple check it all.

### Understand the Transaction Fee

Each transaction has a fee associated with. It's _implicit_ when you send a raw transaction: the amount that you will pay as a fee is always equal to the amount of your input minus the amount of your output. So, you have to decrease your output a little bit from your input to make sure that your transaction goes out.

> ⚠️ **WARNING: Dangerous Calculations Ahead.** Calculating the transaction fee is a very dangerous part of raw transactions!! Because you automatically expend all of the amount in the UTXOs that you use, it's critically important to make sure that you know: (1) precisely what UTXOs you're using; (2) exactly how much money they contain; (3) exactly how much money you're sending out; and (4) what the difference is. If you mess up and you use the wrong UTXO (with more money than you thought) or if you send out too little money, the excess is lost. Forever. Don't make that mistake! Know your inputs and outputs _precisely_. Or better, don't use raw transactions except as part of a carefully considered and triple-checked program.

> 📖 ***How much should you spend on transaction fees?*** [§4.1](04_1_Sending_Coins_The_Easy_Way.md#set-your-transaction-fee) contains extensive discussions of transaction fees. The TL;DR is that 1 sat/vB is usually sufficient in the modern day, though this course tends to use 10 sat/vB just to ensure that you don't have to sit around waiting for transactions to come back. A Bitcoin Explorer such as the [mempool explorer](https://mempool.space/) can alternatively show you what fees are currently being paid.

Assuming a fee of 1 sat/vB and a SegWit transaction that's less than 250 Bytes would mean a fee of 250 satoshis, or 0.0000025 BTC, which would be a quarter if Bitcoin were worth $100,000 each. Not bad! (But we're going to move that up to 0.00001 for quick testing purposes.)

You should either enter the amount you're going to be sending into a `$btc` variable or else install `bc` to run the calculation (and it's always better to let the computer do the calculation, as long as you're using a reliable calculator and not AI). Note that you'll probably have to engage in some `awk` tomfoolery to make sure your number has a leading `0`, because `bitcoin-cli` will choke otherwise.
```
$ sudo apt-get install bc
$ btc=$(echo "0.001-0.00001" | bc -l | awk '{printf "%.5f\n", $0}')
$ echo $btc
.00099
```

> :warning: **WARNING: Fee Slowdowns.** The lower that you set your transaction fee, the longer before your transaction is built into a block. Since blocks are built on average every 10 minutes, that could be the difference between a few minutes, a few hours, and never! So, choose a transaction fee that's appropriate for what you're sending. 

### Write the Raw Transaction

You're now ready to create the raw transaction. This uses the `createrawtransaction` command, which might look a little intimidating. That's because the `createrawtransaction` command doesn't entirely shield you from the JSON RPC that the bitcoin-cli uses (and that we've touched on from time to time). This time, you're going to have to input a JSON array (`[]`) of objects (`{}`) to list the UTXOs that you're spending and a JSON object (`{}`) to list the outputs.

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
Yeah, there are all kinds of crazy quotes there, but trust that they'll do the right thing. Use `'''` to mark the start and end of the JSON array and the JSON object. Protect normal words like `"this"`, but you don't need to protect normal numbers: `0`. If they're variables, also insert single quotes, like `"'$this_word'"` and `'$this_num'`. (Whew. You'll get used to it.)

 Here's a command that creates a raw transaction to send your $utxo to your $recipient
 ```
$ rawtxhex=$(bitcoin-cli createrawtransaction '''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' '''{ "'$recipient'": '$btc' }''')
$ echo $rawtxhex
0200000001fd296670b9d95353a5a9c1659ef9ad563e714f6fb9e9cc3f4c265ecd9afd7ff60100000000fdffffff01b882010000000000160014f1ad72d46aa89d5e622e42bbbb75064eebd9d10100000000
```

### Verify Your Raw Transaction

You should next verify your rawtransaction with `decoderawtransaction` to make sure that it will do the right thing.
```
$ bitcoin-cli decoderawtransaction $rawtxhex
{
  "txid": "8a0d9ab73d81a1ce043d1ede0e737136e6d3352d3e0bc9590f9e8bbd91036dc2",
  "hash": "8a0d9ab73d81a1ce043d1ede0e737136e6d3352d3e0bc9590f9e8bbd91036dc2",
  "version": 2,
  "size": 82,
  "vsize": 82,
  "weight": 328,
  "locktime": 0,
  "vin": [
    {
      "txid": "f67ffd9acd5e264c3fcce9b96f4f713e56adf99e65c1a9a55353d9b9706629fd",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967293
    }
  ],
  "vout": [
    {
      "value": 0.00099000,
      "n": 0,
      "scriptPubKey": {
        "asm": "0 f1ad72d46aa89d5e622e42bbbb75064eebd9d101",
        "desc": "addr(tb1q7xkh94r24zw4uc3wg2amkagxfm4an5gpwrgsnl)#tn6nft96",
        "hex": "0014f1ad72d46aa89d5e622e42bbbb75064eebd9d101",
        "address": "tb1q7xkh94r24zw4uc3wg2amkagxfm4an5gpwrgsnl",
        "type": "witness_v0_keyhash"
      }
    }
  ]
}
```

Check the `vin`. Are you spending the right transaction? Does it contain the expected amount of money? (Check with `bitcoin-cli gettransaction` and be sure to look at the right `vout`.) Check your `vout`. Are you sending the right amount? Is it going to the right address? Finally, do the math to make sure the money balances. Does the value of the UTXO minus the amount being spent equal the expected transaction fee?

> ℹ️  **NOTE: Sequence:** You may note that each input has a sequence number, set here to `4294967293`, which is `0xFFFFFFFF-2`. This is the last frontier of Bitcoin transactions, because it's a standard field in transactions that was originally intended for a specific purpose, but was never fully implemented. So now there's this integer sitting around in transactions that could be repurposed for other uses. And, in fact, it has been. As of this writing there are three different uses for the variable that's called `nSequence` in the Bitcoin Core code: it enables RBF, `nLockTime`, and relative timelocks. It used to always be set to `4294967295` (`0xFFFFFFFF`), which meant "nothing special", but nowadays it's set to  `4294967293` (`0xFFFFFFFF-2`), which means "allow Replace-by-Fee by default", which is explained in [§5.2](05_2_Resending_a_Transaction_with_RBF.md). Other values mean other things.

### Sign the Raw Transaction

To date, your raw transaction is just something theoretical: you _could_ send it, but nothing has been promised. You have to do a few things to get it out onto the network.

First, you need to sign your raw transaction:
```

$ bitcoin-cli signrawtransactionwithwallet $rawtxhex
{
  "hex": "02000000000101fd296670b9d95353a5a9c1659ef9ad563e714f6fb9e9cc3f4c265ecd9afd7ff60100000000fdffffff01b882010000000000160014f1ad72d46aa89d5e622e42bbbb75064eebd9d1010247304402202cc47fe2a529a14765010845eaf393becd91e95df7ba5c142f33a0246382c107022065d7abafe6144e6af899d3311ba59a7c3c9fe8b01df6f022f28ca81fc1eac181012102b412b4329e450c854293c4c4f9d6e716aa9b6f7c7aa622500ddb15f28f215e0400000000",
  "complete": true
}

$ signedtx="02000000000101fd296670b9d95353a5a9c1659ef9ad563e714f6fb9e9cc3f4c265ecd9afd7ff60100000000fdffffff01b882010000000000160014f1ad72d46aa89d5e622e42bbbb75064eebd9d1010247304402202cc47fe2a529a14765010845eaf393becd91e95df7ba5c142f33a0246382c107022065d7abafe6144e6af899d3311ba59a7c3c9fe8b01df6f022f28ca81fc1eac181012102b412b4329e450c854293c4c4f9d6e716aa9b6f7c7aa622500ddb15f28f215e0400000000"
```
Note that we captured the signed hex by hand, rather than trying to parse it out of the JSON object. A software package called "JQ" could do better, as we'll explain in an upcoming interlude.

### Send the Raw Transaction

You've now got a ready-to-go raw transaction, but it doesn't count until you actually put it on the network, which you do with the `sendrawtransaction` command. You'll get back a txid:
```
$ bitcoin-cli sendrawtransaction $signedtx
8a0d9ab73d81a1ce043d1ede0e737136e6d3352d3e0bc9590f9e8bbd91036dc2
```
As usual, you'll immediately see that the UTXO and its money have been removed from your wallet (but you'll have a bit better understanding this time than in [§4.1](04_1_Sending_Coins_The_Easy_Way.m) since you explicitly chose the UTXO to be spent:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "f67ffd9acd5e264c3fcce9b96f4f713e56adf99e65c1a9a55353d9b9706629fd",
    "vout": 0,
    "address": "tb1qr2yek5wu6ce245uexcf624laphdwwu4qlhhggm",
    "scriptPubKey": "00141a899b51dcd632aad3993613a557fd0ddae772a0",
    "amount": 0.01895537,
    "confirmations": 7,
    "spendable": true,
    "solvable": true,
    "desc": "wpkh([b8309bae/84h/1h/0h/1/1]03c548187125947845c252fb14726ff3ce688d1b977235e9b8506c4ad64dfe1ea3)#dutca0cf",
    "parent_descs": [
      "wpkh([b8309bae/84h/1h/0h]tpubDDpSvPDUjstxFUEWzHkaL4qykf8vjNCspm8SZ26Z1wgPFbd63AdYrn4bDpEGPT1giJ6gcLW8Xou8fnhi35DJrUza9ikgu5dg2mDkd8jQpA6/1/*)#eeegkjmk"
    ],
    "safe": true
  }
]
```
But soon `listtransactions` should show a confirmed transaction of category 'send".
```
  {
    "address": "tb1q7xkh94r24zw4uc3wg2amkagxfm4an5gpwrgsnl",
    "category": "send",
    "amount": -0.00099000,
    "label": "",
    "vout": 0,
    "fee": -0.00001000,
    "confirmations": 0,
    "trusted": true,
    "txid": "8a0d9ab73d81a1ce043d1ede0e737136e6d3352d3e0bc9590f9e8bbd91036dc2",
    "wtxid": "f9cfb18a4683a03bae228f3e21a4939803655efaae51e67388673338103543bc",
    "walletconflicts": [
    ],
    "mempoolconflicts": [
    ],
    "time": 1772141891,
    "timereceived": 1772141891,
    "bip125-replaceable": "yes",
    "abandoned": false
  }
```
You can see that it matches the `txid` and the `recipient` address. Not only does it show the `amount` sent, but it also shows the transaction `fee`, which is _hopefully_ what you intended (and indeed it matches the `0.00001` BTC that was planned, thanks to the wonders of `bc`).

Congratulations! You're now a few satoshis poorer!

## Summary: Creating a Raw Transaction

When money comes into your Bitcoin wallet, it remains as distinct amounts, called UTXOs. When you create a raw transaction to send that money back out, you use one or more UTXOs to fund it. You then can create a raw transaction, sign it, and send it on the Bitcoin network. However, this is just a foundation: you'll usually need to create a raw transaction with multiple outputs to actually send something on the bitcoin network!

## What's Next?

Step Back from "Sending Bitcoin Transactions" with [Interlude: Using JQ](04_2a_Interlude_Using_JQ.md).
