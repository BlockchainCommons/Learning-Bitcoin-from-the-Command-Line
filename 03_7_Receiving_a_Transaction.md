# 3.7: Receiving a Transaction

You're now ready to receive some money at an address you created.

## Get Some Money

To do anything more, you need to get some money. On signet this is done through faucets. Since the money is all pretend, you just go to a faucet, request some money, and it will be sent over to you. We suggest using the faucet at [https://signetfaucet.com/](https://signetfaucet.com/) or [https://signet257.bublina.eu.org/](https://signet257.bublina.eu.org/). If they're not available for some reason, search for "bitcoin signet faucet", and you should find others.

To use a faucet, you'll usually need to go to a URL and copy and paste in your address. Afterward, a transaction will be created that sends money from the faucet to you.

> :book: ***What is a transaction?*** A transaction is a bitcoin exchange. The owner of some bitcoins uses his private key to access those coins, then locks the transaction using the recipient's public key, at least for a P2WPKH transaction.

> :link: **SIGNET vs MAINNET:** Sadly, there are no faucets in real life. If you were playing on the mainnet, you'd need to go and actually buy bitcoins at a bitcoin exchange or ATM, or you'd need to get someone to send them to you. Signet life is much easier.

## Verify Your Money

After you've requested your money, you should be able to verify it with the `bitcoin-cli getbalance` command:
```
$ bitcoin-cli getbalance
0.00000000
```
But wait, there's no balance yet!?

Welcome to the world of Bitcoin latency. The problem is that your transaction hasn't yet been recorded in a block!

> :book: ***What is a block?*** Transactions are transmitted across the network and gathered into blocks by miners. These blocks are secured with a mathematical proof-of-work, which proves that computing power has been expended as part of the block creation. It's that proof-of-work (multiplied over many blocks, each built atop the last) that ultimately keeps Bitcoin secure.

> :book: ***What is a miner?*** A miner is a participant of the Bitcoin network who works to create blocks. It's a paying job: when a miner successfully creates a block, they are paid a one-time reward plus the fees for the transactions in his block. Mining is big business. Miners tend to run on special hardware, accelerated in ways that make it more likely that they'll be able to create blocks. They also tend to be part of mining pools, where the miners all agree to share out the rewards when one of them successfully creates a block.

You might have to wait patiently for those funds. Some faucets have a waiting queue before they'll distribute funds, and in any case it takes 10 minutes to make every block, even if a faucet sent a transaction immediately, it might take 10 minutes to arrive. Eventually, however, you should see funds:
```
$ bitcoin-cli getbalance
0.00500090
```

> :book: **Is Ten Minutes the Rule?** Ten minutes is the average amount of time it should take to generate a Bitcoin block. On Signet, it's pretty close to the rule, because the whole point of Signet is to have a regular, reliable testing environment. On Mainnet, the block time depends on when someone can randomly solve the tough proof-of-work problem that allows them to create a block. Sometimes it might take 30 minutes to generate a block, and then suddenly two more arrive a few minutes later each. Over time, the difficulty of the proof-of-work problem is moved up or down to bring the average back to 10 minutes per block.

### Gain Confidence in Your Money

You can use `bitcoin-cli getbalance "*" [n]`, where you replace `[n]` with an integer, to see if a confirmed balance is 'n' blocks deep.

> :book: ***What is block depth?*** After a block is built and confirmed, another block is built on top of it, and another ... Because this is a stochastic process, there's some chance for reversal when a block is still new. Therefore, a block has to be buried several blocks deep in a chain before you can feel totally confident in your funds. Since each of those blocks tends to be built in an average of 10 minutes, it usually takes about an hour for a confirmed transaction to receive six blocks deep, which is the measure for full confidence in Bitcoin.

The following shows that our transactions have been confirmed one time, but not twice:
```
$ bitcoin-cli getbalance "*" 1
0.00500090
$ bitcoin-cli getbalance "*" 2
0.00000000
```
Obviously, every ten minutes or so this depth will increase.

Of course, on the Signet, no one is that worried about how reliable your funds are. You'll be able to spend your money as soon as it's confirmed.

## Verify Your Wallet

The `bitcoin-cli getwalletinfo` command gives you more information on the balance of your wallet:
```
$ bitcoin-cli getwalletinfo
{
  "walletname": "",
  "walletversion": 169900,
  "format": "sqlite",
  "txcount": 3,
  "keypoolsize": 3997,
  "keypoolsize_hd_internal": 4000,
  "paytxfee": 0.00000000,
  "private_keys_enabled": true,
  "avoid_reuse": false,
  "scanning": false,
  "descriptors": true,
  "external_signer": false,
  "blank": false,
  "birthtime": 1771543118,
  "flags": [
    "last_hardened_xpub_cached",
    "descriptor_wallet"
  ],
  "lastprocessedblock": {
    "hash": "000000012b1a2e0d5df2eeba0aa678f69ced5cf036c67299547f0894afbbdf8a",
    "height": 292310
  }
}
```

## Discover Your Transaction ID

Your money came into your wallet via a transaction. You can discover that transactionid (txid) with the `bitcoin-cli listtransactions` command:
```
$ bitcoin-cli listtransactions
[
  {
    "address": "2NAzFNuopaor2YnqVA2QC4KYme6HxWTk7jq",
    "parent_descs": [
      "sh(wpkh([1091501a/49h/1h/0h]tpubDCsAfCNPzp1BvrL8u6rdgD693s9MjcPvswzwkdzRT24BBHVc4cRNH13Z9yKTtdEcg4kmZLUa3FxZgwKfr6LVMoPfKB6x5xMz9HietddEcaX/0/*))#0xkvj5k7"
    ],
    "category": "receive",
    "amount": 0.00500090,
    "label": "",
    "vout": 48,
    "abandoned": false,
    "confirmations": 1,
    "blockhash": "000000012b1a2e0d5df2eeba0aa678f69ced5cf036c67299547f0894afbbdf8a",
    "blockheight": 292310,
    "blockindex": 28,
    "blocktime": 1771548933,
    "txid": "8954c067d0d3b47825300effeff9501a584c9644bfd58764a6af08a1f084f67c",
    "wtxid": "96f40640ad3242b593de502fc47a7633a0be8c7eb5996dd843fb1ad4a0ada46c",
    "walletconflicts": [
      "078bfa57116a7a5fbc8119606a054f34fa0dce0d90d15df0b6aebfa817010d94",
      "a69f25e4bf09ae1eebf3bdae56744f02c2d5a0dfed4f8d078ee99ee3a22571e9"
    ],
    "mempoolconflicts": [
    ],
    "time": 1771548914,
    "timereceived": 1771548914,
    "bip125-replaceable": "no"
  }
]
```
This shows one transaction (`8954c067d0d3b47825300effeff9501a584c9644bfd58764a6af08a1f084f67c`) for a specific ammount (`0.00500090`) which was received by a specific address in your wallet (`2NAzFNuopaor2YnqVA2QC4KYme6HxWTk7jq`).

You can access similar information with the `bitcoin-cli listunspent` command, but it only shows the transactions for the money that you haven't spent. These are called UTXOs, and will be vitally important when you're sending money back out into the Bitcoin world:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "8954c067d0d3b47825300effeff9501a584c9644bfd58764a6af08a1f084f67c",
    "vout": 48,
    "address": "2NAzFNuopaor2YnqVA2QC4KYme6HxWTk7jq",
    "label": "",
    "redeemScript": "001461a3a217641fdea3b3ff444b4f582763ea949132",
    "scriptPubKey": "a914c29d2f62bb6593e01719ada872a779f4f2990e7087",
    "amount": 0.00500090,
    "confirmations": 2,
    "spendable": true,
    "solvable": true,
    "desc": "sh(wpkh([1091501a/49h/1h/0h/0/0]038f833378b831969986212fcb557ac18c5e0001e1ae9066423c2376dd9d451513))#a2fumkh9",
    "parent_descs": [
      "sh(wpkh([1091501a/49h/1h/0h]tpubDCsAfCNPzp1BvrL8u6rdgD693s9MjcPvswzwkdzRT24BBHVc4cRNH13Z9yKTtdEcg4kmZLUa3FxZgwKfr6LVMoPfKB6x5xMz9HietddEcaX/0/*))#0xkvj5k7"
    ],
    "safe": true
  }
]
```
Note that bitcoins are not just a homogeneous mess of cash jammed into your pocket. Each individual transaction that you receive or that you send is placed into the immutable blockchain ledger, in a block. You can see these individual transactions when you look at your unspent money. This means that bitcoin spending isn't quite as anonymous as you'd think. Though the addresses are fairly private, transactions can be examined as they go in and out of addresses. This makes privacy vulnerable to statistical analysis. It also introduces some potential non-fungibility to bitcoins, as you can track back through series of transactions, even if you can't track a specific "bitcoin".

> :book: ***Why are all of these bitcoin amounts in decimals?*** Bitcoins are produced slowly, and so there are relatively few in circulation. As a result, each bitcoin over on the mainnet is worth quite a bit (~ $67,000 at the time of this writing). This means that people usually work in fractional amounts. In fact, the .005 in Signet coins would be worth about $300 if they were on the mainnet. For this reason, names have appeared for smaller amounts of bitcoins, including millibitcoins or mBTCs (one-thousandth of a bitcoin), microbitcoins or bits or μBTCs (one-millionth of a bitcoin), and satoshis (one hundred millionth of a bitcoin).

## Examine Your Transaction

You can get more information on a transaction with the `bitcoin-cli gettransaction` command:
```
$ bitcoin-cli gettransaction 8954c067d0d3b47825300effeff9501a584c9644bfd58764a6af08a1f084f67c
{
  "amount": 0.00500090,
  "confirmations": 1,
  "blockhash": "000000012b1a2e0d5df2eeba0aa678f69ced5cf036c67299547f0894afbbdf8a",
  "blockheight": 292310,
  "blockindex": 28,
  "blocktime": 1771548933,
  "txid": "8954c067d0d3b47825300effeff9501a584c9644bfd58764a6af08a1f084f67c",
  "wtxid": "96f40640ad3242b593de502fc47a7633a0be8c7eb5996dd843fb1ad4a0ada46c",
  "walletconflicts": [
    "078bfa57116a7a5fbc8119606a054f34fa0dce0d90d15df0b6aebfa817010d94",
    "a69f25e4bf09ae1eebf3bdae56744f02c2d5a0dfed4f8d078ee99ee3a22571e9"
  ],
  "mempoolconflicts": [
  ],
  "time": 1771548914,
  "timereceived": 1771548914,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "2NAzFNuopaor2YnqVA2QC4KYme6HxWTk7jq",
      "parent_descs": [
        "sh(wpkh([1091501a/49h/1h/0h]tpubDCsAfCNPzp1BvrL8u6rdgD693s9MjcPvswzwkdzRT24BBHVc4cRNH13Z9yKTtdEcg4kmZLUa3FxZgwKfr6LVMoPfKB6x5xMz9HietddEcaX/0/*))#0xkvj5k7"
      ],
      "category": "receive",
      "amount": 0.00500090,
      "label": "",
      "vout": 48,
      "abandoned": false
    }
  ],
  "hex": "02000000000101653...733fd5750400",
  "lastprocessedblock": {
    "hash": "0000001058c350b95ba1dc2b689ad905a9907091255b28396b6eeb0e9d4ab167",
    "height": 292311
  }
}
```
The `gettransaction` command will detail transactions that are in your wallet, such as this one, that was sent to us.

Note that `gettransaction` has two optional arguments:
```
$ bitcoin-cli help gettransaction
gettransaction "txid" ( include_watchonly verbose )

Get detailed information about in-wallet transaction <txid>

Arguments:
1. txid                 (string, required) The transaction id
2. include_watchonly    (boolean, optional, default=false) (DEPRECATED) No longer used
3. verbose              (boolean, optional, default=false) Whether to include a `decoded` field containing the decoded transaction (equivalent to RPC decoderawtransaction)

```
By setting these two true or false, we can choose to include watch-only addresses in the output (which we don't care about) or look at more verbose output (which we do).

Here's what this data instead looks at when we set `include_watchonly` to `false` and `verbose` to `true`.
```
$ bitcoin-cli gettransaction 8954c067d0d3b47825300effeff9501a584c9644bfd58764a6af08a1f084f67c false true
{
  "amount": 0.00500090,
  "confirmations": 2,
  "blockhash": "000000012b1a2e0d5df2eeba0aa678f69ced5cf036c67299547f0894afbbdf8a",
  "blockheight": 292310,
  "blockindex": 28,
  "blocktime": 1771548933,
  "txid": "8954c067d0d3b47825300effeff9501a584c9644bfd58764a6af08a1f084f67c",
  "wtxid": "96f40640ad3242b593de502fc47a7633a0be8c7eb5996dd843fb1ad4a0ada46c",
  "walletconflicts": [
    "078bfa57116a7a5fbc8119606a054f34fa0dce0d90d15df0b6aebfa817010d94",
    "a69f25e4bf09ae1eebf3bdae56744f02c2d5a0dfed4f8d078ee99ee3a22571e9"
  ],
  "mempoolconflicts": [
  ],
  "time": 1771548914,
  "timereceived": 1771548914,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "2NAzFNuopaor2YnqVA2QC4KYme6HxWTk7jq",
      "parent_descs": [
        "sh(wpkh([1091501a/49h/1h/0h]tpubDCsAfCNPzp1BvrL8u6rdgD693s9MjcPvswzwkdzRT24BBHVc4cRNH13Z9yKTtdEcg4kmZLUa3FxZgwKfr6LVMoPfKB6x5xMz9HietddEcaX/0/*))#0xkvj5k7"
      ],
      "category": "receive",
      "amount": 0.00500090,
      "label": "",
      "vout": 48,
      "abandoned": false
    }
  ],
  "hex": "02000000000101653...733fd5750400",
  "decoded": {
    "txid": "8954c067d0d3b47825300effeff9501a584c9644bfd58764a6af08a1f084f67c",
    "hash": "96f40640ad3242b593de502fc47a7633a0be8c7eb5996dd843fb1ad4a0ada46c",
    "version": 2,
    "size": 3563,
    "vsize": 3512,
    "weight": 14048,
    "locktime": 292309,
    "vin": [
      {
        "txid": "c550ee29fdc62e1d42c966bd67282e7db922773945344ddeef699db875603c65",
        "vout": 0,
        "scriptSig": {
          "asm": "",
          "hex": ""
        },
        "txinwitness": [
          "f2183e5e3f233bb9e11f33a2c6c1691bea3b4bb2f19a8674cff8dfecdda4685789eba8745609a560e827b8e0eb9909772baa00a76d23c1faac72c40b83bd733f"
        ],
        "sequence": 4294967293
      }
    ],
    "vout": [
      {
        "value": 1256.11643555,
        "n": 0,
        "scriptPubKey": {
          "asm": "1 aac35fe91f20d48816b3c83011d117efa35acd2414d36c1e02b0f29fc3106d90",
          "desc": "rawtr(aac35fe91f20d48816b3c83011d117efa35acd2414d36c1e02b0f29fc3106d90)#au3flvfu",
          "hex": "5120aac35fe91f20d48816b3c83011d117efa35acd2414d36c1e02b0f29fc3106d90",
          "address": "tb1p4tp4l6glyr2gs94neqcpr5gha7344nfyznfkc8szkreflscsdkgqsdent4",
          "type": "witness_v1_taproot"
        }
      },
      {
        "value": 0.00000000,
        "n": 1,
        "scriptPubKey": {
          "asm": "OP_RETURN 616c742e7369676e65746661756365742e636f6d207c203930207c205468697320697320612074657374206e6574776f726b2e20436f696e732068617665206e6f2076616c75652e207c2076623962383365",
          "desc": "raw(6a4c52616c742e7369676e65746661756365742e636f6d207c203930207c205468697320697320612074657374206e6574776f726b2e20436f696e732068617665206e6f2076616c75652e207c2076623962383365)#zz3mwpqc",
          "hex": "6a4c52616c742e7369676e65746661756365742e636f6d207c203930207c205468697320697320612074657374206e6574776f726b2e20436f696e732068617665206e6f2076616c75652e207c2076623962383365",
          "type": "nulldata"
        }
      },
      {
        "value": 0.00500090,
        "n": 2,
        "scriptPubKey": {
          "asm": "0 02b9d2f57dd3fadbbeb100d49a686e06b2b43ce5",
          "desc": "addr(tb1qq2ua9ata60adh043qr2f56rwq6etg089xshs0g)#lwpdnyre",
          "hex": "001402b9d2f57dd3fadbbeb100d49a686e06b2b43ce5",
          "address": "tb1qq2ua9ata60adh043qr2f56rwq6etg089xshs0g",
          "type": "witness_v0_keyhash"
        }
      },
      {
        "value": 0.00500090,
        "n": 3,
        "scriptPubKey": {
          "asm": "0 0bec08492d89e0a5ff6947f57e58b10793e1c3f6",
          "desc": "addr(tb1qp0kqsjfd38s2tlmfgl6huk93q7f7rslkcl50fl)#nkve3qrw",
          "hex": "00140bec08492d89e0a5ff6947f57e58b10793e1c3f6",
          "address": "tb1qp0kqsjfd38s2tlmfgl6huk93q7f7rslkcl50fl",
          "type": "witness_v0_keyhash"
        }
      },
      {
        "value": 0.00500090,
        "n": 4,
        "scriptPubKey": {
          "asm": "0 0c618d8a36ed23cc13e1f92f880f879918aaa815",
          "desc": "addr(tb1qp3scmz3ka53ucylplyhcsru8nyv242q4ucf9lt)#6lm9vhtp",
          "hex": "00140c618d8a36ed23cc13e1f92f880f879918aaa815",
          "address": "tb1qp3scmz3ka53ucylplyhcsru8nyv242q4ucf9lt",
          "type": "witness_v0_keyhash"
        }
      },
      {
        "value": 0.00500090,
        "n": 5,
        "scriptPubKey": {
          "asm": "0 0ca3037bc9d5dfdbd11aac2055445049c11d569b",
          "desc": "addr(tb1qpj3sx77f6h0ah5g64ss923zsf8q3645musu6jd)#u9q7ehwz",
          "hex": "00140ca3037bc9d5dfdbd11aac2055445049c11d569b",
          "address": "tb1qpj3sx77f6h0ah5g64ss923zsf8q3645musu6jd",
          "type": "witness_v0_keyhash"
        }
      },
      ...
  "lastprocessedblock": {
    "hash": "0000001058c350b95ba1dc2b689ad905a9907091255b28396b6eeb0e9d4ab167",
    "height": 292311
  }
}
```
Now you can see the full information on the transaction, including all of the inputs ("vin") and all the outputs ("vout). One of the interesting things to note is that although we received .00500090 BTC in the transaction, the same amount was sent to dozens of additional addresses (the full list was cut down to just the first several). It is quite typical for a transaction to have multiple inputs and/or multiple outputs.

There is another command, `getrawtransaction`, which allows you to look at transactions that are not in your wallet. However, it requires you to have an unpruned node and `txindex=1` in your `bitcoin.conf` file. Unless you have a serious need for information not in your wallet, it's probably just better to use a Bitcoin explorer for this sort of thing ...

## Optional: Use a Block Explorer

Even looking at the verbose information for a transaction can be a little intimidating. The main goal of this tutorial is to teach how to deal with raw transactions from the command line, but we're happy to talk about other tools when they're applicable. One of those tools is a block explorer, which you can use to look at transactions from a web browser in a much friendlier format.

Currently, our preferred block explorer is [https://mempool.space/](https://mempool.space/), which has options for mainnet, signet, testnet3, and testnet4.

You can use it to look up transactions for an address:

[https://mempool.space/signet/address/2NAzFNuopaor2YnqVA2QC4KYme6HxWTk7jq](https://mempool.space/signet/address/2NAzFNuopaor2YnqVA2QC4KYme6HxWTk7jq)

You can also use it to look at individual transactions:

[https://mempool.space/signet/tx/8954c067d0d3b47825300effeff9501a584c9644bfd58764a6af08a1f084f67c](https://mempool.space/signet/tx/8954c067d0d3b47825300effeff9501a584c9644bfd58764a6af08a1f084f67c)

A block explorer doesn't generally provide any more information than a command line look at a raw transaction; it just does a good job of highlighting the important information and putting together the puzzle pieces, including the transaction fees behind a transaction, another concept that we'll be covering in future sections.

## Summary: Receiving a Transaction

Faucets will give you money on the testnet. They come in as raw transactions, which can be examined with `gettransaction` or a block explorer. Once you've receive a transaction, you can see it in your balance and your wallet.

## What's Next?

For an aside on using command-line variables that will be vital to the rest of this course, read [Interlude: Using Command-Line Variables](03_7a_Interlude_Using_Command-Line_Variables.md).

If you're already comfortable with them, continue on to [Chapter Four: Sending Bitcoin Transactions](04_0_Sending_Bitcoin_Transactions.md).



