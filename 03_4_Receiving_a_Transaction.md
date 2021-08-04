# 3.4: Receiving a Transaction

You're now ready to receive some money at the new address you set up.

## Get Some Money

To do anything more, you need to get some money. On testnet this is done through faucets. Since the money is all pretend, you just go to a faucet, request some money, and it will be sent over to you. We suggest using the faucet at https://testnet-faucet.mempool.co/, https://bitcoinfaucet.uo1.net/, or https://testnet.coinfaucet.eu/en/. If they're not available for some reason, search for "bitcoin testnet faucet", and you should find others.

To use a faucet, you'll usually need to go to a URL and copy and paste in your address. Note that this is one of those cases where you won't be able to use command-line variables, alas. Afterward, a transaction will be created that sends money from the faucet to you.

> :book: ***What is a transaction?*** A transaction is a bitcoin exchange. The owner of some bitcoins uses his private key to access those coins, then locks the transaction using the recipient's public key.

> :link: **TESTNET vs MAINNET:** Sadly, there are no faucets in real life. If you were playing on the mainnet, you'd need to go and actually buy bitcoins at a bitcoin exchange or ATM, or you'd need to get someone to send them to you. Testnet life is much easier.

## Verify Your Money

After you've requested your money, you should be able to verify it with the `bitcoin-cli getbalance` command:
```
$ bitcoin-cli getbalance
0.00000000
```
But wait, there's no balance yet!?

Welcome to the world of Bitcoin latency. The problem is that your transaction hasn't yet been recorded in a block!

> :book: ***What is a block?*** Transactions are transmitted across the network and gathered into blocks by miners. These blocks are secured with a mathematical proof-of-work, which proves that computing power has been expended as part of the block creation. It's that proof-of-work (multiplied over many blocks, each built atop the last) that ultimately keeps Bitcoin secure.

> :book: ***What is a miner?*** A miner is a participant of the Bitcoin network who works to create blocks. It's a paying job: when a miner successfully creates a block, he is paid a one-time reward plus the fees for the transactions in his block. Mining is big business. Miners tend to run on special hardware, accelerated in ways that make it more likely that they'll be able to create blocks. They also tend to be part of mining pools, where the miners all agree to share out the rewards when one of them successfully creates a block.

Fortunately, `bitcoin-cli getunconfirmedbalance` should still show your updated balance as long as the initial transaction has been created:
```
$ bitcoin-cli getunconfirmedbalance
0.01010000
```
If that's still showing a zero too, you're probably moving through this tutorial too fast. Wait a second. The coins should show up unconfirmed, then rapidly move to confirmed. Do note that a coin can move from unconfirmedbalance to confirmedbalance almost immediately, so make sure you check both. However, if your `getbalance` and your `getunconfirmedbalance` both still show zero in ten minutes, then there's probably something wrong with the faucet, and you'll need to pick another.

### Gain Confidence in Your Money

You can use `bitcoin-cli getbalance "*" [n]`, where you replace `[n]` with an integer, to see if a confirmed balance is 'n' blocks deep.

> :book: ***What is block depth?*** After a block is built and confirmed, another block is built on top of it, and another ... Because this is a stochastic process, there's some chance for reversal when a block is still new. Thus, a block has to be buried several blocks deep in a chain before you can feel totally confident in your funds. Each of those blocks tends to be built in an average of 10 minutes ... so it usually takes about an hour for a confirmed transaction to receive six blocks deep, which is the measure for full confidence in Bitcoin.

The following shows that our transactions have been confirmed one time, but not twice:
```
$  bitcoin-cli getbalance "*" 1
0.01010000
$  bitcoin-cli getbalance "*" 2
0.00000000
```
Obviously, every ten minutes or so this depth will increase.

Of course, on the testnet, no one is that worried about how reliable your funds are. You'll be able to spend your money as soon as it's confirmed.

## Verify Your Wallet

The `bitcoin-cli getwalletinfo` command gives you more information on the balance of your wallet:
```
$ bitcoin-cli getwalletinfo
{
  "walletname": "",
  "walletversion": 169900,
  "balance": 0.01010000,
  "unconfirmed_balance": 0.00000000,
  "immature_balance": 0.00000000,
  "txcount": 2,
  "keypoololdest": 1592335137,
  "keypoolsize": 999,
  "hdseedid": "fdea8e2630f00d29a9d6ff2af7bf5b358d061078",
  "keypoolsize_hd_internal": 1000,
  "paytxfee": 0.00000000,
  "private_keys_enabled": true,
  "avoid_reuse": false,
  "scanning": false
}
```

## Discover Your Transaction ID

Your money came into your wallet via a transaction. You can discover that transactionid (txid) with the `bitcoin-cli listtransactions` command:
```
$ bitcoin-cli listtransactions
[
  {
    "address": "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
    "category": "receive",
    "amount": 0.01000000,
    "label": "",
    "vout": 1,
    "confirmations": 1,
    "blockhash": "00000000000001753b24411d0e4726212f6a53aeda481ceff058ffb49e1cd969",
    "blockheight": 1772396,
    "blockindex": 73,
    "blocktime": 1592600085,
    "txid": "8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9",
    "walletconflicts": [
    ],
    "time": 1592599884,
    "timereceived": 1592599884,
    "bip125-replaceable": "no"
  },
  {
    "address": "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
    "category": "receive",
    "amount": 0.00010000,
    "label": "",
    "vout": 0,
    "confirmations": 1,
    "blockhash": "00000000000001753b24411d0e4726212f6a53aeda481ceff058ffb49e1cd969",
    "blockheight": 1772396,
    "blockindex": 72,
    "blocktime": 1592600085,
    "txid": "ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36",
    "walletconflicts": [
    ],
    "time": 1592599938,
    "timereceived": 1592599938,
    "bip125-replaceable": "no"
  }
]

```
This shows two transactions (`8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9`) and (`ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36`) for a specific amount (`0.01000000` and `0.00010000`), which were both received (`receive`) by the same address in our wallet (`mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE`). That's bad key hygeine, by the way: you should use a new address for every single Bitcoin you ever receive. In this case, we got impatient because the first faucet didn't seem to be working.

You can access similar information with the `bitcoin-cli listunspent` command, but it only shows the transactions for the money that you haven't spent. These are called UTXOs, and will be vitally important when you're sending money back out into the Bitcoin world:
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
    "confirmations": 1,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/1']02fd5740996d853ea51a6904cf03257fc11204b0179f344c49739ec5b20b39c9ba)#62rud39c",
    "safe": true
  },
  {
    "txid": "8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9",
    "vout": 1,
    "address": "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
    "label": "",
    "scriptPubKey": "76a9141b72503639a13f190bf79acf6d76255d772360b788ac",
    "amount": 0.01000000,
    "confirmations": 1,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/1']02fd5740996d853ea51a6904cf03257fc11204b0179f344c49739ec5b20b39c9ba)#62rud39c",
    "safe": true
  }
]
```
Note that bitcoins are not just a homogeneous mess of cash jammed into your pocket. Each individual transaction that you receive or that you send is placed into the immutable blockchain ledger, in a block. You can see these individual transactions when you look at your unspent money. This means that bitcoin spending isn't quite as anonymous as you'd think. Though the addresses are fairly private, transactions can be examined as they go in and out of addresses. This makes privacy vulnerable to statistical analysis. It also introduces some potential non-fungibility to bitcoins, as you can track back through series of transactions, even if you can't track a specific "bitcoin".

> :book: ***Why are all of these bitcoin amounts in fractions?*** Bitcoins are produced slowly, and so there are relatively few in circulation. As a result, each bitcoin over on the mainnet is worth quite a bit (~ $9,000 at the time of this writing). This means that people usually work in fractions. In fact, the .0101 in Testnet coins would be worth about $100 if they were on the mainnet. For this reason, names have appeared for smaller amounts of bitcoins, including millibitcoins or mBTCs (one-thousandth of a bitcoin), microbitcoins or bits or μBTCs (one-millionth of a bitcoin), and satoshis (one hundred millionth of a bitcoin).

## Examine Your Transaction

You can get more information on a transaction with the `bitcoin-cli gettransaction` command:
```
$ bitcoin-cli gettransaction "8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9"
{
  "amount": 0.01000000,
  "confirmations": 1,
  "blockhash": "00000000000001753b24411d0e4726212f6a53aeda481ceff058ffb49e1cd969",
  "blockheight": 1772396,
  "blockindex": 73,
  "blocktime": 1592600085,
  "txid": "8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9",
  "walletconflicts": [
  ],
  "time": 1592599884,
  "timereceived": 1592599884,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
      "category": "receive",
      "amount": 0.01000000,
      "label": "",
      "vout": 1
    }
  ],
  "hex": "0200000000010114d04977d1b0137adbf51dd5d79944b9465a2619f3fa7287eb69a779977bf5800100000017160014e85ba02862dbadabd6d204fcc8bb5d54658c7d4ffeffffff02df690f000000000017a9145c3bfb36b03f279967977ca9d1e35185e39917788740420f00000000001976a9141b72503639a13f190bf79acf6d76255d772360b788ac0247304402201e74bdfc330fc2e093a8eabe95b6c5633c8d6767249fa25baf62541a129359c202204d462bd932ee5c15c7f082ad7a6b5a41c68addc473786a0a9a232093fde8e1330121022897dfbf085ecc6ad7e22fc91593414a845659429a7bbb44e2e536258d2cbc0c270b1b00"
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
2. include_watchonly    (boolean, optional, default=true for watch-only wallets, otherwise false) Whether to include watch-only addresses in balance calculation and details[]
3. verbose              (boolean, optional, default=false) Whether to include a `decoded` field containing the decoded transaction (equivalent to RPC decoderawtransaction)
```
By setting these two true or false, we can choose to include watch-only addresses in the output (which we don't care about) or look at more verbose output (which we do).

Here's what this data instead looks at when we set `include_watchonly` to `false` and `verbose` to `true`.
```
$ bitcoin-cli gettransaction "8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9" false true
{
  "amount": 0.01000000,
  "confirmations": 3,
  "blockhash": "00000000000001753b24411d0e4726212f6a53aeda481ceff058ffb49e1cd969",
  "blockheight": 1772396,
  "blockindex": 73,
  "blocktime": 1592600085,
  "txid": "8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9",
  "walletconflicts": [
  ],
  "time": 1592599884,
  "timereceived": 1592599884,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
      "category": "receive",
      "amount": 0.01000000,
      "label": "",
      "vout": 1
    }
  ],
  "hex": "0200000000010114d04977d1b0137adbf51dd5d79944b9465a2619f3fa7287eb69a779977bf5800100000017160014e85ba02862dbadabd6d204fcc8bb5d54658c7d4ffeffffff02df690f000000000017a9145c3bfb36b03f279967977ca9d1e35185e39917788740420f00000000001976a9141b72503639a13f190bf79acf6d76255d772360b788ac0247304402201e74bdfc330fc2e093a8eabe95b6c5633c8d6767249fa25baf62541a129359c202204d462bd932ee5c15c7f082ad7a6b5a41c68addc473786a0a9a232093fde8e1330121022897dfbf085ecc6ad7e22fc91593414a845659429a7bbb44e2e536258d2cbc0c270b1b00",
  "decoded": {
    "txid": "8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9",
    "hash": "d4ae2b009c43bfe9eba96dcd16e136ceba2842df3d76a67d689fae5975ce49cb",
    "version": 2,
    "size": 249,
    "vsize": 168,
    "weight": 669,
    "locktime": 1772327,
    "vin": [
      {
        "txid": "80f57b9779a769eb8772faf319265a46b94499d7d51df5db7a13b0d17749d014",
        "vout": 1,
        "scriptSig": {
          "asm": "0014e85ba02862dbadabd6d204fcc8bb5d54658c7d4f",
          "hex": "160014e85ba02862dbadabd6d204fcc8bb5d54658c7d4f"
        },
        "txinwitness": [
          "304402201e74bdfc330fc2e093a8eabe95b6c5633c8d6767249fa25baf62541a129359c202204d462bd932ee5c15c7f082ad7a6b5a41c68addc473786a0a9a232093fde8e13301",
          "022897dfbf085ecc6ad7e22fc91593414a845659429a7bbb44e2e536258d2cbc0c"
        ],
        "sequence": 4294967294
      }
    ],
    "vout": [
      {
        "value": 0.01010143,
        "n": 0,
        "scriptPubKey": {
          "asm": "OP_HASH160 5c3bfb36b03f279967977ca9d1e35185e3991778 OP_EQUAL",
          "hex": "a9145c3bfb36b03f279967977ca9d1e35185e399177887",
          "reqSigs": 1,
          "type": "scripthash",
          "addresses": [
            "2N1ev1WKevSsdmAvRqZf7JjvDg223tPrVCm"
          ]
        }
      },
      {
        "value": 0.01000000,
        "n": 1,
        "scriptPubKey": {
          "asm": "OP_DUP OP_HASH160 1b72503639a13f190bf79acf6d76255d772360b7 OP_EQUALVERIFY OP_CHECKSIG",
          "hex": "76a9141b72503639a13f190bf79acf6d76255d772360b788ac",
          "reqSigs": 1,
          "type": "pubkeyhash",
          "addresses": [
            "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE"
          ]
        }
      }
    ]
  }
}
```
Now you can see the full information on the transaction, including all of the inputs ("vin") and all the outputs ("vout). One of the interesting things to note is that although we received .01 BTC in the transaction, another .01010143 was sent to another address. That was probably a change address, a concept that is explored in the next section. It is quite typical for a transaction to have multiple inputs and/or multiple outputs.

There is another command, `getrawtransaction`, which allows you to look at transactions that are not in your wallet. However, it requires you to have unpruned node and `txindex=1` in your `bitcoin.conf` file. Unless you have a serious need for information not in your wallet, it's probably just better to use a Bitcoin explorer for this sort of thing ...

## Optional: Use a Block Explorer

Even looking at the verbose information for a transaction can be a little intimidating. The main goal of this tutorial is to teach how to deal with raw transactions from the command line, but we're happy to talk about other tools when they're applicable. One of those tools is a block explorer, which you can use to look at transactions from a web browser in a much friendlier format.

Currently, our preferred block explorer is [https://live.blockcypher.com/](https://live.blockcypher.com/).

You can use it to look up transactions for an address:

[https://live.blockcypher.com/btc-testnet/address/mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE/](https://live.blockcypher.com/btc-testnet/address/mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE/)

You can also use it to look at individual transactions:

[https://live.blockcypher.com/btc-testnet/tx/8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9/](https://live.blockcypher.com/btc-testnet/tx/8e2ab10cabe9ec04ed438086a80b1ac72558cc05bb206e48fc9a18b01b9282e9/)

A block explorer doesn't generally provide any more information than a command line look at a raw transaction; it just does a good job of highlighting the important information and putting together the puzzle pieces, including the transaction fees behind a transaction — another concept that we'll be covering in future sections.

## Summary: Receiving a Transaction

Faucets will give you money on the testnet. They come in as raw transactions, which can be examined with `gettransaction` or a block explorer. Once you've receive a transaction, you can see it in your balance and your wallet.

## What's Next?

For a deep dive into how addresses are described, so that they can be transferred or made into parts of a multi-signature, see [§3.5: Understanding the Descriptor](03_5_Understanding_the_Descriptor.md).

But if that's too in-depth, continue on to [Chapter Four: Sending Bitcoin Transactions](04_0_Sending_Bitcoin_Transactions.md).

