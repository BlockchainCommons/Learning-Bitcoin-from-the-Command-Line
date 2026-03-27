# 5.2: Resending a Transaction with RBF

If your Bitcoin transaction is stuck, and you're the sender, you can _replace_ (resend) it using RBF (replace-by-fee). However, that's not all that RBF can do: it's generally a powerful and multipurpose feature that allows Bitcoin senders to recreate transactions for a variety of reasons.

## Understand Opt-In RBF vs Full RBF

There are currently two versions of RBF that are present on the Bitcoin network: opt-in RBF and full RBF. Both do the same thing: they allow you to replace an existing unconfirmed transaction with a newer transaction that has a higher fee (and so is more likely to be confirmed).

* **Opt-In RBF** is the older version of RBF, originally defined by [BIP-125](https://github.com/bitcoin/bips/blob/master/bip-0125.mediawiki). It required you to signal in a transaction that the transaction could later be replaced if needed. Without the signal in the original transaction, RBF was not allowed.
* **Full RBF** was introduced by Bitcoin Core as an option in 2022 and became the default in additional updates from 2024-2025. It allows any unconfirmed transaction to be updated with RBF, even without the opt-in signal.

Both versions of RBF still exist on the network and as of this writing [opt-in RBF transactions tend to outnumber full RBF transactions](https://mempool.space/rbf). This may just be because many wallets got into the habit of opting in to RBF for all transactions, because you never knew when you'd have to replace one, but it also may be because RBF is ultimately a node policy: each node (each miner, each bitcoind, etc) gets to decide whether they will accept a replaced RBF transaction or not: if enough nodes reject an RBF transaction because it doesn't have an opt-in, it fails.

There is almost certainly enough acceptance of Full RBF today for transactions to be replaceable even without the RBF flag, and it's almost certainly safer to include the RBF flag anyway, because it's simple to do as a default. As a result, this section will talk about how the RBF opt-in flag is set on Bitcoin transactions and then how to replace transactions with RBF (whether the flag was set on the original transaction or not).

## Understand the Sequence Variable

Satoshi Nakamoto originally allowed for replacement of Bitcoin transactions using the `sequence` variable (usually called `nSequence`), which is set on the inputs to a transactions. The idea was that a larger sequence should be used as a replacement if a previous sequence had not been confirmed. If was simple and it was quickly tossed out for a variety of reasons.

When RBF came around as a new version of transaction replacement, it went back to the existing `sequence` variable, but with some new mechanics:

* A `sequence` that was larger than 0 and less than than 0xffffffff-1 (4294967294) opted in to RBF.
* The opt-in only required one input to have its `sequence`. If most of a transaction's inputs were set to disallow RBF, but a single input enabled RBF, then RBF was allowed.
* If RBF was allowed on a transaction, it could then later be replaced.

That was it! With RBF enabled, transactions could be replaced; with RBF disabled, transactions could not. (Except today, all transactios can be replaced, _in theory_.)

> ℹ️ **Sequence Usage (v1).** This is the first use of the `sequence` value in Bitcoin. You can set it between 1 and 0xffffffff-2 (4294967293) and enable RBF, but if you're not careful you can run up against the parallel use of `sequence` for relative timelocks. We suggest always setting it to "0xffffffff-2", which is what Bitcoin Core does, but the other option is to set it to to "1" or to a value between 0xf0000000 (4026531840) and 0xffffffff-2 (4294967293). Setting `sequence` to "1" effectively makes relative timelocks irrelevent and setting it to 0xf0000000 or higher deactivates them. This is all explained in later sections. For now, just choose one of the non-conflicting values for `sequence`.

## Understand How RBF Works

Here's the complete sequence of how RBF works, summarised from BIP-125. It's probably more than you need to know:

1. The original transaction must signal replaceability (or more recently is always allowed).
2. The new transaction fee must be higher than the original.
3. The new transaction must not have any new unconfirmed inputs (and it must share at least one input with the original).
4. The new transaction must pay double the relay fees (which are a minor fee related to the cost of transmitting transactions, and are usually ignored here because of their small size).
5. No more than 100 transactions and descendents can be replaced at one time.

The core thing to understand about RBF is that in order to use it, you must double-spend. As noted, you're reusing one or more of the UTXOs that were in your original transaction. Just sending another transaction with a different UTXO to the same recipient won't do the trick (and will likely result in your losing money). Instead, you must purposefully create a conflict, where the same UTXO is used in two different transactions. Faced with this conflict, the miners will know to use the conflicted transaction with the higher fee, and they'll be incentivized to do so by that higher fee.

> 📖 ***What is a double-spend?*** A double-spend occurs when someone sends the same electronic funds to two different people (or, to the same person twice, in two different transactions). This is a central problem for any e-cash system. It's solved in Bitcoin by the immutable ledger: once a transaction is sufficiently confirmed, no miners will verify transactions that reuse the same UTXO. However, it's possible to double-spend _before_ a transaction has been confirmed — which is why you always want one or more confirmations before you finalize a transaction. In the case of RBF, you purposefully double-spend because an initial transaction has stalled, and the miners accept your double-spend if you meet the specific criteria laid out by BIP 125.

## Check the Sequence Variable

You can check the `sequence` variable of a transaction by looking at the `verbose` format of `gettransaction`.
```
$ bitcoin-cli -named gettransaction txid=1050f6e2d68e7e4555bd682f24157f9bb19f666cf2ba1d1b3ff959ff4f893654 verbose=true
{
  "amount": -0.00100000,
  "fee": -0.00000141,
  "confirmations": 1,
  "blockhash": "0000000c1dc879a623845990aae757f8e38c83474c4a0e1de9cb988cef69f15f",
  "blockheight": 295311,
  "blockindex": 50,
  "blocktime": 1773350929,
  "txid": "1050f6e2d68e7e4555bd682f24157f9bb19f666cf2ba1d1b3ff959ff4f893654",
  "wtxid": "d98414f5627f10337c9c2f4ff096a4ec5d15b91898ebf3fac3fa21b9ad299740",
  "walletconflicts": [
  ],
  "mempoolconflicts": [
  ],
  "time": 1773350380,
  "timereceived": 1773350380,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "tb1qg3lau83hm9e9tdvzr5k7aqtw3uv0dwkfct4xdn",
      "category": "send",
      "amount": -0.00100000,
      "vout": 1,
      "fee": -0.00000141,
      "abandoned": false
    }
  ],
  "hex": "02000000000101b345bc2c6d1410c949c25edbdd96eecea02a0675f2c561d473b8a58b8e88a8735d01000000fdffffff02741b06000000000016001499981dc044f0b3f893d27cb4b246dd65ffd8bde7a086010000000000160014447fde1e37d97255b5821d2dee816e8f18f6bac90247304402200391dde68ab21714f3ff352a4ef0c38b038d5bf6695787ad2c0583dc5b80b7110220581985bb88df2b452e188a60a558b2b26d47d1e0adc9c60d7d7eb569419e9d920121021b78fa3ed908a1ed1c2623a2311015b347de707d507d147dc0e83d84ec06255200000000",
  "decoded": {
    "txid": "1050f6e2d68e7e4555bd682f24157f9bb19f666cf2ba1d1b3ff959ff4f893654",
    "hash": "d98414f5627f10337c9c2f4ff096a4ec5d15b91898ebf3fac3fa21b9ad299740",
    "version": 2,
    "size": 222,
    "vsize": 141,
    "weight": 561,
    "locktime": 0,
    "vin": [
      {
        "txid": "73a8888e8ba5b873d461c5f275062aa0ceee96dddb5ec249c910146d2cbc45b3",
        "vout": 349,
        "scriptSig": {
          "asm": "",
          "hex": ""
        },
        "txinwitness": [
          "304402200391dde68ab21714f3ff352a4ef0c38b038d5bf6695787ad2c0583dc5b80b7110220581985bb88df2b452e188a60a558b2b26d47d1e0adc9c60d7d7eb569419e9d9201",
          "021b78fa3ed908a1ed1c2623a2311015b347de707d507d147dc0e83d84ec062552"
        ],
        "sequence": 4294967293
      }
    ],
    "vout": [
      {
        "value": 0.00400244,
        "n": 0,
        "scriptPubKey": {
          "asm": "0 99981dc044f0b3f893d27cb4b246dd65ffd8bde7",
          "desc": "addr(tb1qnxvpmszy7zel3y7j0j6ty3kavhla30088mrk3d)#e4e0tqst",
          "hex": "001499981dc044f0b3f893d27cb4b246dd65ffd8bde7",
          "address": "tb1qnxvpmszy7zel3y7j0j6ty3kavhla30088mrk3d",
          "type": "witness_v0_keyhash"
        }
      },
      {
        "value": 0.00100000,
        "n": 1,
        "scriptPubKey": {
          "asm": "0 447fde1e37d97255b5821d2dee816e8f18f6bac9",
          "desc": "addr(tb1qg3lau83hm9e9tdvzr5k7aqtw3uv0dwkfct4xdn)#zau8pzqg",
          "hex": "0014447fde1e37d97255b5821d2dee816e8f18f6bac9",
          "address": "tb1qg3lau83hm9e9tdvzr5k7aqtw3uv0dwkfct4xdn",
          "type": "witness_v0_keyhash"
        }
      }
    ]
  },
  "lastprocessedblock": {
    "hash": "0000000c1dc879a623845990aae757f8e38c83474c4a0e1de9cb988cef69f15f",
    "height": 295311
  }
}
```
As you can see, the single input for the transaction has an `sequence` value of `4294967293`. RBF is allowed (and relative timelocks are not).

You might also notice a related value, `bip125-replaceable`, which is a `bitcoin-cli` shorthand that tells you whether a transaction can be replaced by RBF. To be eligible, the transaction must have a `sequence` variable in the appropriate range, and it must not be confirmed. (This `bip125-replaceable` is set to `no` because the transaction has a confirmation. No more replacement at that point!)

You can also use `decoderawtransaction` to view the `sequence` variable in slightly terser form:
```
$ bitcoin-cli decoderawtransaction $rawtxhex3
{
  "txid": "1050f6e2d68e7e4555bd682f24157f9bb19f666cf2ba1d1b3ff959ff4f893654",
  "hash": "1050f6e2d68e7e4555bd682f24157f9bb19f666cf2ba1d1b3ff959ff4f893654",
  "version": 2,
  "size": 113,
  "vsize": 113,
  "weight": 452,
  "locktime": 0,
  "vin": [
    {
      "txid": "73a8888e8ba5b873d461c5f275062aa0ceee96dddb5ec249c910146d2cbc45b3",
      "vout": 349,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967293
    }
  ],
  "vout": [
    {
      "value": 0.00400244,
      "n": 0,
      "scriptPubKey": {
        "asm": "0 99981dc044f0b3f893d27cb4b246dd65ffd8bde7",
        "desc": "addr(tb1qnxvpmszy7zel3y7j0j6ty3kavhla30088mrk3d)#e4e0tqst",
        "hex": "001499981dc044f0b3f893d27cb4b246dd65ffd8bde7",
        "address": "tb1qnxvpmszy7zel3y7j0j6ty3kavhla30088mrk3d",
        "type": "witness_v0_keyhash"
      }
    },
    {
      "value": 0.00100000,
      "n": 1,
      "scriptPubKey": {
        "asm": "0 447fde1e37d97255b5821d2dee816e8f18f6bac9",
        "desc": "addr(tb1qg3lau83hm9e9tdvzr5k7aqtw3uv0dwkfct4xdn)#zau8pzqg",
        "hex": "0014447fde1e37d97255b5821d2dee816e8f18f6bac9",
        "address": "tb1qg3lau83hm9e9tdvzr5k7aqtw3uv0dwkfct4xdn",
        "type": "witness_v0_keyhash"
      }
    }
  ]
}
```

## Reset the Sequence Variable

The above examples drew from a default transaction created by `bitcoin-cli` . The sequence was already (by default!) set to "4294967293". By default, Bitcoin Core now flags all transactions with the RBF opt-in flag, so RBF is always allowed; whether other nodes use Full RBF or not, you'll be able to replace your transactions originally generated with `bitcoin-cli`. 

However, you can chose to change this behavior. To do so, you create a raw transaction as described in [§4.2](04_2_Creating_a_Raw_Transaction.md), but you add a `sequence` variable to the `txid` and `vout` that you already have for one of the inputs. Since RBF is now on by default, you'd either be setting this to `0` to turn it off (though there's little purpose when Full RBF is widely accepted on the 'net), or to some other value to allow timelocks (more on that in chapter 8).
```
$ seqtx=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "sequence": 0 } ]''' outputs='''{ "'$recipient'": 0.001, "'$changeaddress'": 0.003 }''')
```
If you're just dealing with RBF, there's no point in doing this, but you'll need to in the future for other purposes.

## Replace a Transaction the Hard Way: By Hand

You now know how to enable RBF: it's probably allowed by Full RBF and it's probably allowed because your `sequence` variable is set to a value between 1 and 4294967293, but if it weren't, you could set the `sequence` by hand when you created a transaction. But how do you actually use RBF to replace an existing transaction?

In order to create replacement RBF transaction by hand, all you have to do is create a raw transaction that: (1) replaces a previous raw transaction that is not confirmed; (2) reuses one or more of the same UTXOs; (3) increases fees; and (4) pays the minimum bandwidth of both transactions [which is probably already taken care of by (3)].

Here's an example of sending a transaction with a very low fee:
```
$ seqtx=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.001, "'$changeaddress'": 0.003002 }''')
$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$seqtx | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
cffcebcb841e4177a682527c8a10a7f7b1889d827b095c55f946d68ab8195508
```
Realizing the problem, we then resend it with a better fee using at least one of the same UTXOs. We do that by decreasing the change amount; as we saw previously, any remaining funds always pay the transaction fee.
```
$ seqtx=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.001, "'$changeaddress'": 0.0029 }''')
$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$seqtx | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
eb9980a5a6dc5b7859bb5b455e6dea8a71c1906a390edcd2a5acc6051c4a8149
```
That's all that's required to use RBF in the modern day where RBF is turned on by default by most wallets, and where Full RBF would be available it it weren't!

Initially `listtransactions` demonstrates both transactions:
```
$ bitcoin-cli listtransactions
[
  {
    "address": "tb1qg3lau83hm9e9tdvzr5k7aqtw3uv0dwkfct4xdn",
    "category": "send",
    "amount": -0.00100000,
    "vout": 0,
    "fee": -0.00000044,
    "confirmations": 0,
    "trusted": false,
    "txid": "cffcebcb841e4177a682527c8a10a7f7b1889d827b095c55f946d68ab8195508",
    "wtxid": "ded34ab60bdc9cd298348c91929bcee3f0458ccd5cef3f51cc95b5fa80318fe4",
    "walletconflicts": [
      "eb9980a5a6dc5b7859bb5b455e6dea8a71c1906a390edcd2a5acc6051c4a8149"
    ],
    "mempoolconflicts": [
      "eb9980a5a6dc5b7859bb5b455e6dea8a71c1906a390edcd2a5acc6051c4a8149"
    ],
    "time": 1773354920,
    "timereceived": 1773354920,
    "bip125-replaceable": "yes",
    "abandoned": false
  },
  {
    "address": "tb1qg3lau83hm9e9tdvzr5k7aqtw3uv0dwkfct4xdn",
    "category": "send",
    "amount": -0.00100000,
    "vout": 0,
    "fee": -0.00010244,
    "confirmations": 0,
    "trusted": true,
    "txid": "eb9980a5a6dc5b7859bb5b455e6dea8a71c1906a390edcd2a5acc6051c4a8149",
    "wtxid": "4715973d9aed02a3f26c42eb81f1077f464421ea50505152782f792f855d8756",
    "walletconflicts": [
      "cffcebcb841e4177a682527c8a10a7f7b1889d827b095c55f946d68ab8195508"
    ],
    "mempoolconflicts": [
    ],
    "time": 1773354943,
    "timereceived": 1773354943,
    "bip125-replaceable": "yes",
    "abandoned": false
  }
]
```
As shown, they're listed as `bip125-replaceable` (which means that the RBF opt-in flag is set, so we don't even have to fall back on Full RBF) and that they have `conflicts`, both in the `wallet` and the `mempool` (because they use the same UTXO!)

Eventually, the transaction with the larger fee should be accepted:
```
$ bitcoin-cli -named gettransaction txid=eb9980a5a6dc5b7859bb5b455e6dea8a71c1906a390edcd2a5acc6051c4a8149
{
  "amount": -0.00100000,
  "fee": -0.00010244,
  "confirmations": 2,
  "blockhash": "00000003be7eb7f4eb252441a0eda9cc55ef7ae8342995d8151eea3409885c24",
  "blockheight": 295314,
  "blockindex": 2,
  "blocktime": 1773355616,
  "txid": "eb9980a5a6dc5b7859bb5b455e6dea8a71c1906a390edcd2a5acc6051c4a8149",
  "wtxid": "4715973d9aed02a3f26c42eb81f1077f464421ea50505152782f792f855d8756",
  "walletconflicts": [
    "cffcebcb841e4177a682527c8a10a7f7b1889d827b095c55f946d68ab8195508"
  ],
  "mempoolconflicts": [
  ],
  "time": 1773354943,
  "timereceived": 1773354943,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "tb1qg3lau83hm9e9tdvzr5k7aqtw3uv0dwkfct4xdn",
      "category": "send",
      "amount": -0.00100000,
      "vout": 0,
      "fee": -0.00010244,
      "abandoned": false
    }
  ],
  "hex": "020000000001015436894fff59f93f1b1dbaf26c669fb19b7f15242f68bd55457e8ed6e2f650100000000000fdffffff02a086010000000000160014447fde1e37d97255b5821d2dee816e8f18f6bac9d06c0400000000001600147b1212b39c2796ae73b726e192aa207c334ce52f02473044022016edca707550fddb4457778865cad3a18bc50025e34c59858bce01f18649788102202f0075018f6a4db8b713b939d56a452c56645d5b51b6a93242c3532fd432709e0121036d3ef06bbf57ab531fcc457b79176bea87e322db5e7d254d8702c6e59c5d3b9c00000000",
  "lastprocessedblock": {
    "hash": "000000145528df59836e3e85e619b399f4b95352cb39d4c3db706d5dbab0ae44",
    "height": 295315
  }
}
```
Now the `mempoolconflicts` are gone and that `bip125-replaceable` is set to `no` because of the confirmation.

Meanwhile, the original transaction with the lower fee starts picking up negative confirmations, to show its divergence from the blockchain:
```
$ bitcoin-cli -named gettransaction txid=cffcebcb841e4177a682527c8a10a7f7b1889d827b095c55f946d68ab8195508
{
  "amount": -0.00100000,
  "fee": -0.00000044,
  "confirmations": -2,
  "trusted": false,
  "txid": "cffcebcb841e4177a682527c8a10a7f7b1889d827b095c55f946d68ab8195508",
  "wtxid": "ded34ab60bdc9cd298348c91929bcee3f0458ccd5cef3f51cc95b5fa80318fe4",
  "walletconflicts": [
    "eb9980a5a6dc5b7859bb5b455e6dea8a71c1906a390edcd2a5acc6051c4a8149"
  ],
  "mempoolconflicts": [
  ],
  "time": 1773354920,
  "timereceived": 1773354920,
  "bip125-replaceable": "yes",
  "details": [
    {
      "address": "tb1qg3lau83hm9e9tdvzr5k7aqtw3uv0dwkfct4xdn",
      "category": "send",
      "amount": -0.00100000,
      "vout": 0,
      "fee": -0.00000044,
      "abandoned": false
    }
  ],
  "hex": "020000000001015436894fff59f93f1b1dbaf26c669fb19b7f15242f68bd55457e8ed6e2f650100000000000fdffffff02a086010000000000160014447fde1e37d97255b5821d2dee816e8f18f6bac9a8940400000000001600147b1212b39c2796ae73b726e192aa207c334ce52f0247304402206d52697eab994b764ffae5220d06c4e867a9ceff609c79a3338de78e0ca427e802202d42489c026e2c28ac4b3c3fb7262fadabe944485994b696c07fe88a922e765f0121036d3ef06bbf57ab531fcc457b79176bea87e322db5e7d254d8702c6e59c5d3b9c00000000",
  "lastprocessedblock": {
    "hash": "000000145528df59836e3e85e619b399f4b95352cb39d4c3db706d5dbab0ae44",
    "height": 295315
  }
}
```
Our recipients have their money, and the original, failed transaction will eventually fall out of the mempool.

## Replace a Transaction the Easy Way: By bumpfee

Raw transactions are very powerful, and you can do a lot of interesting things by combining them with RBF. However, sometimes _all_ you want to do is free up a transaction that's been hanging. You can now do that with a simple command, `bumpfee`.

For example, to increase the fee of transaction `1c12afac570292e99b4aed34c0452513b4ab8b7397e6a803e98a53b606e31c11` you would run:
```
$ bitcoin-cli -named bumpfee 1c12afac570292e99b4aed34c0452513b4ab8b7397e6a803e98a53b606e31c11
{
  "txid": "938d3755a91efbeead1dda30c0b808986410db1078f02500fc512a0ff6704872",
  "origfee": 0.00000100,
  "fee": 0.00000806,
  "errors": [
  ]
}
```
The result is the automatic generation of a new transaction that has a fee determined by your bitcoin.conf file. The new transaction can be discovered by looking at `listtransactions` and then examined with `gettransaction`
```
$ bitcoin-cli gettransaction 938d3755a91efbeead1dda30c0b808986410db1078f02500fc512a0ff6704872
{
  "amount": -0.00010000,
  "fee": -0.00000806,
  "confirmations": 0,
  "trusted": true,
  "txid": "938d3755a91efbeead1dda30c0b808986410db1078f02500fc512a0ff6704872",
  "wtxid": "73afdafc1c8f77d51597beafba472e5241e9266e4a93dfbaf00bc8f5807384a5",
  "walletconflicts": [
    "1c12afac570292e99b4aed34c0452513b4ab8b7397e6a803e98a53b606e31c11"
  ],
  "mempoolconflicts": [
  ],
  "time": 1773356653,
  "timereceived": 1773356653,
  "bip125-replaceable": "yes",
  "replaces_txid": "1c12afac570292e99b4aed34c0452513b4ab8b7397e6a803e98a53b606e31c11",
  "details": [
    {
      "address": "tb1qg3lau83hm9e9tdvzr5k7aqtw3uv0dwkfct4xdn",
      "category": "send",
      "amount": -0.00010000,
      "vout": 1,
      "fee": -0.00000806,
      "abandoned": false
    }
  ],
  "hex": "0200000000010149814a1c05c6aca5d2dc0e396a90c1718aea6d5e455bbb59785bdca6a58099eb0100000000fdffffff029a420400000000001600147b1212b39c2796ae73b726e192aa207c334ce52f1027000000000000160014447fde1e37d97255b5821d2dee816e8f18f6bac90247304402204c080988284fcc0f4b825c97a046df7f6ddbe5276cd76059ece272d18a3a8ab6022009cbb87b2707b899356958b4fc0d61319b61659cda8847ba184bd678c3f2535201210302b2db8ca5241f14879051ee01413643da8d798cb348ace49ea7d71d3e135bb794810400",
  "lastprocessedblock": {
    "hash": "0000000c96de4354c533b4459f2b86581871825ce25f8797bb04425e68d49fdc",
    "height": 295316
  }
}
```

## Summary: Resending a Transaction with RBF

If a transaction is stuck, and you don't want to wait for it to expire entirely, then you can double-spend using RBF to create a replacement transaction (or just use `bumpfee`). You used to have to opt in, but now most wallets opt-in by default, and even if they didn't Full RBF should be available.

> 🔥 ***What is the power of RBF?*** Obviously, RBF is very helpful if you created a transaction with too low of a fee and you need to get those funds through. However, the ability to generally replace unconfirmed transactions with updated ones has more power than just that (and is why you might want to continue using RBF with raw transactions, even following the advent of `bumpfee`).  For example, you might send a transaction, and then before it's confirmed, combine it with a second transaction. This allows you to compress multiple transactions down into a single one, decreasing overall fees. It might also offer benefits to privacy. There are other reasons to use RBF too, for smart contracts or transaction cut-throughs, as described in the [Opt-in RBF FAQ](https://bitcoincore.org/en/faq/optin_rbf/).

## What's Next?

Continue "Controlling Bitcoin Transactions" with [§5.3: Funding a Transaction with CPFP](05_3_Funding_a_Transaction_with_CPFP.md).  
