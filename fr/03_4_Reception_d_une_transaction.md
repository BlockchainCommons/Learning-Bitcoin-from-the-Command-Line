# AFAIRE 3.4: Réception d'une transaction

> :information_source: **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

You're now ready to receive some money at the new address you set up.

## Get Some Money

To do anything more, you need to get some money. On testnet this is done through faucets. Since the money is all pretend, you just go to a faucet, request some money, and it will be sent over to you. We suggest using the faucet at https://testnet.coinfaucet.eu/en/. If it's not available for some reason, search for "bitcoin testnet faucet", and you should find others.

To use a faucet, you'll usually need to go to a URL and copy and paste in your address. Note that this is one of those cases where you won't be able to use command-line variables, alas. Afterward, a transaction will be created that sends money from the faucet to you.

_What is a transaction?_ A transaction is a bitcoin exchange. The owner of some bitcoins uses his private key to access those coins, then locks the transaction using the recipient's public key.

> :link: **TESTNET vs MAINNET:** Sadly, there are no faucets in real life. If you were playing on the mainnet, you'd need to go and actually buy bitcoins at a bitcoin exchange or ATM, or you'd need to get someone to send them to you. Testnet life is much easier.

## Verify Your Money

After you've requested your money, you should be able to verify it with the 'bitcoin-cli getbalance' command:
```
$ bitcoin-cli getbalance
0.00000000
```
But wait, there's no balance yet!?

Welcome to the world of Bitcoin latency.The problem is that your transaction hasn't yet been recorded in a block!

_What is a block?_ Transactions are transmitted across the network and gathered into blocks by miners. These blocks are secured with a mathematical proof-of-work, which proves that computing power has been expended as part of the block creation. It's that proof-of-work (multiplied over many blocks, each built atop the last) that ultimately keeps Bitcoin secure.

_What is a miner?_ A miner is a participant of the Bitcoin network who works to create blocks. It's a paying job: when a miner successfully creates a block, he is paid a one-time reward plus the fees for the transactions in his block. Mining is big business. Miners tend to run on special hardware, accelerated in ways that make it more likely that they'll be able to create blocks. They also tend to be part of mining pools, where the miners all agree to share out the rewards when one of them successfully creates a block.

Fortunately, `bitcoin-cli getunconfirmedbalance` should still show your updated balance as long as the initial transaction has been created:
```
$ bitcoin-cli getunconfirmedbalance
0.47000000
```
If that's still showing a zero too, you're probably moving through this tutorial too fast. Wait a second. The coins should show up unconfirmed, then rapidly move to confirmed. Do note that a coin can move from unconfirmedbalance to confirmedbalance almost immediately, so make sure you check both. However, if your `getbalance` and your `getunconfirmedbalance` both still show zero in ten minutes, then there's probably something wrong with the faucet, and you'll need to pick another.

### Gain Confidence in Your Money

You can use `bitcoin-cli getbalance "*" [n]` to see if a confirmed balance is 'n' blocks deep.

_What is block depth?_ After a block is built and confirmed, another block is built on top of it, and another ... Because this is a stochastic process, there's some chance for reversal when a block is still new. Thus, a block has to be buried several blocks deep in a chain before you can feel totally confident in your funds. Each of those blocks tends to be built in an average of 10 minutes ... so it usually takes about an hour for a confirmed transaction to receive full confidence.

The following shows that our transaction has been confirmed one time, but not twice:
```
$ bitcoin-cli getbalance "*" 1
0.47000000
$ bitcoin-cli getbalance "*" 2
0.00000000
```
Obviously, every ten minutes or so this depth will increase.

Of course, on the testnet, no one is that worried about how reliable your funds are. You'll be able to spend your money as soon as it's confirmed.

## Verify Your Wallet

The `bitcoin-cli getwalletinfo` command gives you more information on the balance of your wallet:
```
$ bitcoin-cli getwalletinfo
{
  "walletversion": 130000,
  "balance": 0.47000000,
  "unconfirmed_balance": 0.00000000,
  "immature_balance": 0.00000000,
  "txcount": 1,
  "keypoololdest": 1488216266,
  "keypoolsize": 100,
  "paytxfee": 0.00000000,
  "hdmasterkeyid": "b91d5ec57d5ae3e90fff50d12e819429b6496b94"
}
```

## Discover Your Transaction ID

Your money came into your wallet via a transaction. You can discover that transactionid (txid) with the `bitcoin-cli listtransactions` command:
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
    "confirmations": 2,
    "blockhash": "00000000fa4fdd22a2c33c6200b68239939ad65af3f1a48ecea25f8200f5d66b",
    "blockindex": 45,
    "blocktime": 1488307692,
    "txid": "88e5d5f3077517d76f5a61491fa52e6aaae078c52bc62d849f09507ef0cfada2",
    "walletconflicts": [
    ],
    "time": 1488307692,
    "timereceived": 1488307696,
    "bip125-replaceable": "no"
  }
]
```
This shows one transaction ("88e5d5f3077517d76f5a61491fa52e6aaae078c52bc62d849f09507ef0cfada2") that was received ("receive") by a specific address in my wallet ("n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf") for a specific amount ("0.47000000").

You can access similar information with the `bitcoin-cli listunspent` command, but it only shows the transactions for the money that you haven't spent. These are called UTXOs, and will be vitally important when you're sending money back out into the Bitcoin world:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "88e5d5f3077517d76f5a61491fa52e6aaae078c52bc62d849f09507ef0cfada2",
    "vout": 0,
    "address": "n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf",
    "account": "",
    "scriptPubKey": "76a914fd67e8a7c7813e7a5c376eb71074f373d924d96888ac",
    "amount": 0.47000000,
    "confirmations": 3,
    "spendable": true,
    "solvable": true
  }
]
```
Note that bitcoins are not just a homogeneous mess of cash jammed into your pocket. Each individual transaction that you receive or that you send is placed into the immutable blockchain ledger, in a block. You can see these individual transactions when you look at your unspent money. This means that bitcoin spending isn't quite as anonymous as you'd think. Though the addresses are fairly private, transactions can be examined as they go in and out of addresses. This makes privacy vulnerable to statistical analysis. It also introduces some potential non-fungibility to bitcoins, as you can track back through series of transactions, even if you can't track a specific "bitcoin".

_Why are all of these bitcoin amounts in fractions?_ Bitcoins are produced slowly, and so there are relatively few in circulation. As a result, each bitcoin over on the mainnet is worth quite a bit (~ $1,000 at the time of this writing). This means that people usually work in fractions. In fact, .47 BTC would be quite a lot in the real-world. You'll often be dealing with even smaller fractions on mainnet. For this reason, names have appeared for smaller amounts of bitcoins, including millibitcoins or mBTCs (one-thousandth of a bitcoin), microbitcoins or bits or μBTCs (one-millionth of a bitcoin), and satoshis (one hundred millionth of a bitcoin).

## Examine Your Transaction

You can get more information on a transaction with the `bitcoin-cli getrawtransaction` command:
```
$ bitcoin-cli getrawtransaction "88e5d5f3077517d76f5a61491fa52e6aaae078c52bc62d849f09507ef0cfada2"
010000000133261a25b44689bab2c6a207381ca21d243de9bbf21f0fa40c3a26ba7282a87b000000006b483045022100a2640761810dfc34dabed599928243afe24e13f520f780ceb382843a530a577c022050b92f5d9843d70ddb60a0aa294938862f2b7372818d6149ffd4f6adec5cf6c80121034dcaa515c2fda0f4a50b90a6d798e01c00a870bef0bd97154066fe202d2b5d75feffffff02c029cd02000000001976a914fd67e8a7c7813e7a5c376eb71074f373d924d96888ac17791703000000001976a914e176ee39c642344df2180863e27e2e936307273c88ac07a41000
```
Granted, this isn't super useful, because it's the hex-encoded transaction data. Fortunately, you can get a more verbose description just by adding a '1' to your command:
```
$ bitcoin-cli getrawtransaction "88e5d5f3077517d76f5a61491fa52e6aaae078c52bc62d849f09507ef0cfada2" 1
{
  "hex": "010000000133261a25b44689bab2c6a207381ca21d243de9bbf21f0fa40c3a26ba7282a87b000000006b483045022100a2640761810dfc34dabed599928243afe24e13f520f780ceb382843a530a577c022050b92f5d9843d70ddb60a0aa294938862f2b7372818d6149ffd4f6adec5cf6c80121034dcaa515c2fda0f4a50b90a6d798e01c00a870bef0bd97154066fe202d2b5d75feffffff02c029cd02000000001976a914fd67e8a7c7813e7a5c376eb71074f373d924d96888ac17791703000000001976a914e176ee39c642344df2180863e27e2e936307273c88ac07a41000",
  "txid": "88e5d5f3077517d76f5a61491fa52e6aaae078c52bc62d849f09507ef0cfada2",
  "hash": "88e5d5f3077517d76f5a61491fa52e6aaae078c52bc62d849f09507ef0cfada2",
  "size": 226,
  "vsize": 226,
  "version": 1,
  "locktime": 1090567,
  "vin": [
    {
      "txid": "7ba88272ba263a0ca40f1ff2bbe93d241da21c3807a2c6b2ba8946b4251a2633",
      "vout": 0,
      "scriptSig": {
        "asm": "3045022100a2640761810dfc34dabed599928243afe24e13f520f780ceb382843a530a577c022050b92f5d9843d70ddb60a0aa294938862f2b7372818d6149ffd4f6adec5cf6c8[ALL] 034dcaa515c2fda0f4a50b90a6d798e01c00a870bef0bd97154066fe202d2b5d75",
        "hex": "483045022100a2640761810dfc34dabed599928243afe24e13f520f780ceb382843a530a577c022050b92f5d9843d70ddb60a0aa294938862f2b7372818d6149ffd4f6adec5cf6c80121034dcaa515c2fda0f4a50b90a6d798e01c00a870bef0bd97154066fe202d2b5d75"
      },
      "sequence": 4294967294
    }
  ],
  "vout": [
    {
      "value": 0.47000000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 fd67e8a7c7813e7a5c376eb71074f373d924d968 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914fd67e8a7c7813e7a5c376eb71074f373d924d96888ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf"
        ]
      }
    },
    {
      "value": 0.51869975,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 e176ee39c642344df2180863e27e2e936307273c OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914e176ee39c642344df2180863e27e2e936307273c88ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "n256of3JH1A6X8AQUU7LYkcaRcmrfGjGKC"
        ]
      }
    }
  ],
  "blockhash": "00000000fa4fdd22a2c33c6200b68239939ad65af3f1a48ecea25f8200f5d66b",
  "confirmations": 3,
  "time": 1488307692,
  "blocktime": 1488307692
}
```
Now you can see the full information on the transaction, including all of the inputs ("vin") and all the outputs ("vout). One of the interesting things to note is that although we received .47 BTC in the transaction, another .51869975 was sent to another address. That was probably a change address, a concept that is explored in the next section. It is quite typical for a transaction to have multiple inputs and/or multiple outputs.

> :warning: **WARNING:** These commands will not work in some situations. To be able to view a raw transaction on a standard node, some of the money must be unspent, or the transaction must still be in your mempool — which means that this command will work fine for the money you've just received, but not for old stuff. If you want to be able to view older transactions that have been spent, you can do so by maintaining a set of all transactions with the txindex=1 configuration, which is what our scripts suggest for all non-pruned instances. (You can't maintain a transaction index if your node is pruned.)

## Optional: Use a Block Explorer

Even looking at the verbose information for a transaction can be a little intimidating. The main goal of this tutorial is to teach how to deal with raw transactions from the command line, but we're happy to talk about other tools when they're applicable. One of those tools is a block explorer, which you can use to look at transactions from a web browser in a much friendlier format.

Currently, our preferred block explorer is [https://live.blockcypher.com/](https://live.blockcypher.com/).

You can use it to look up transactions for an address:

[https://live.blockcypher.com/btc-testnet/address/n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf/](https://live.blockcypher.com/btc-testnet/address/n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf/)

You can also use it to look at individual transactions:

[https://live.blockcypher.com/btc-testnet/tx/88e5d5f3077517d76f5a61491fa52e6aaae078c52bc62d849f09507ef0cfada2/](https://live.blockcypher.com/btc-testnet/tx/88e5d5f3077517d76f5a61491fa52e6aaae078c52bc62d849f09507ef0cfada2/)

A block explorer doesn't generally provide any more information than a command line look at a raw transaction; it just does a good job of highlighting the important information and putting together the puzzle pieces, including the transaction fees behind a transaction — another concept that we'll be covering in future sections.

## Summary: Receiving a Transaction

Faucets will give you money on the testnet. They come in as raw transactions, which can be examined with `getrawtransaction` or a block explorer. Once you've receive a transaction, you can see it in your balance and your wallet.

## What's Next?

Advance through "bitcoin-cli" with [Chapter Four: Sending Bitcoin Transactions](04_0_Sending_Bitcoin_Transactions.md).
