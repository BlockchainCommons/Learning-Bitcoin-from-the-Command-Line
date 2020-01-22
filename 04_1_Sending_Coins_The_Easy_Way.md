# 4.1: Sending Coins the Easy Way

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

The `bitcoin-cli` offers three major ways to send coins: as a simple command; as a raw transaction; and as a raw transaction with calculation. Each has their own advantages and disadvantages. This first method for sending coins is also the simplest.

## Set Your Transaction Fee

Before you send any money on the Bitcoin network, you should think about what transaction fees you're going to pay.

_What is a transaction fee?_ There's no such thing as a free lunch. Miners incorporate transactions into blocks because they're paid to do so. Not only do they get paid by the network for making the block, but they also get paid by transactors for including their transactions. If you don't pay a fee, your transaction might get stuck ... forever (or, until saved by some of the tricks in section five).

When you're using the simple and automated methods for creating transactions, as outlined here and in [§4.5: Sending Coins with Automated Raw Transactions](04_5_Sending_Coins_with_Automated_Raw_Transactions.md), Bitcoin will calculate transaction fees for you. This is done using Floating Fees, where the `bitcoind` watches how long transactions are taking to confirm and automatically calculates for you what to spend.

You can help control this by putting rational values into your ~/.bitcoin/bitcoin.conf. The following low-cost values would ensure that there was a minimum transaction fee of 10,000 satoshis per kByte of data in your transaction and request that the floating fees figure out a good amount to get your transaction somewhere into the next six blocks.
```
mintxfee=0.0001
txconfirmtarget=6
```
However, under the theory that you don't want to wait around while working on a tutorial, we've adopted the following higher values:
```
mintxfee=0.001
txconfirmtarget=1
```
In order to get through this tutorial, we're willing to spend 100,00 satoshis per kB on every transaction (about $1!) and we want to get each transaction into the next block! (To put that in perspective, a typical transaction runs between .25 kB and 1 kB, so you'll actually be paying more like a a quarter than a buck.)

After you've edited your bitcoin.conf file, you'll want to kill and restart bitcoind.

## Get an Address

You need somewhere to send your coins to. Usually, someone would send you an address, and perhaps give you a signature to prove they own that address. Alternatively, they might give you a QR code to scan, so that you can't make mistakes when typing in the address. In our case, we're going to send coins to `n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi`, which is a return address for TP's TestNet faucet.

_What is a QR code?_ A QR code is just an encoding of a Bitcoin address. Many wallets will generate QR codes for you, while some sites will convert from an address to a QR code. Obviously, you should only accept a QR code from a site that you absolutely trust. A payer can use a bar-code scanner to read in the QR code, then pay to it.

## Send the Coins

You're now ready to send some coins. This is actually quite simple via the command line. You just use `bitcoin-cli sendtoaddress [address] [amount]`. So, to send a little coinage to the address `n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi` just requires:
```
$ txid=$(bitcoin-cli sendtoaddress n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi 0.1)
$ echo $txid
586b3ff591d43948ed4107216be52d831c551747b469626a6b7c84bbf1639f76
```
Make sure the address you write in is where you want the money to go. Make _double_ sure. If you make mistakes in Bitcoin, there's no going back.

You'll receive a txid back when you issue this command.

> **WARNING:** The `bitcoin-cli` command actually generates JSON-RPC commands when it's talking to the bitcoind. They can be really picky. This is an example: if you list the bitcoin amount without the leading zero (i.e. ".1" instead of "0.1"), then bitcoin-cli will fail with a mysterious message.

## Examine Your Transaction

You can look at your transaction using your transaction id:
```
$ bitcoin-cli gettransaction $txid
{
  "amount": -0.10000000,
  "fee": -0.00022600,
  "confirmations": 0,
  "trusted": false,
  "txid": "586b3ff591d43948ed4107216be52d831c551747b469626a6b7c84bbf1639f76",
  "walletconflicts": [
  ],
  "time": 1490742233,
  "timereceived": 1490742233,
  "bip125-replaceable": "unknown",
  "details": [
    {
      "account": "",
      "address": "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi",
      "category": "send",
      "amount": -0.10000000,
      "vout": 0,
      "fee": -0.00022600,
      "abandoned": false
    }
  ],
  "hex": "0200000001a8b61dba544525ad267644cb78f07c1ba58586ff9089aec3ac24d8764dc21dfb000000006a47304402204c38c2530d3283200e4fd3b2d22e609fc6dc941fd3ac4bc8b73ad5a86607e723022050056ae6cfc3233fb38459a6fd5e63d54e4c85e17b91d66fb915e3977a1c77dd0121027a313901f2ac34c87761513cabe69ca9ca61e2db3c7e6f89d7eccd7fc0a5917cfeffffff0280969800000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac4082820b000000001976a914a091d978794d50e5caa3e5454cc8633240640d6688aca6de1000"
}
```
You can see not only the amount transferred (.1 BTC) but also a transaction fee (.000226 BTC), which is about a quarter of the .001 BTC/kB minimum fee that was set, which suggests that the transaction was about a quarter of a kB in size.

While you are waiting for this transaction to clear, you'll note that `bitcoin-cli getbalance` shows that all of your money is gone (or, at least, all of your money from a single incoming transaction). Similarly, `bitcoin-cli listunspent` would show an appropriately sized blob of incoming money was gone. There's a reason for this: whenever you get money in, you have to send it _all_ out together, and you have to perform some gymnastics if you actually want to keep some of it! Once again, `sendtoaddress` takes care of this all for you, which means you don't have to worry about it in full until you use a raw transaction to send out money.

## Summary: Sending Coins the Easy Way

To send coins the easy way, make sure your transaction defaults are rationale, get an address, and send coins there. That's why they call it easy!

### Why Use The Easy Way?

_The advantages._ It's easy. You don't have to worry about arcane things like UTXOs. You don't have to calculate transaction fees by hand, so you're not likely to make mistakes that cost you large amounts of money. If your sole goal is to sit down at your computer and send some money, this is the way to go.

_The disadvantages._ It's high level. You have very little control over what's happening, and you can't do anything fancy. If you're planning to write more complex Bitcoin software or want a deeper understanding of how Bitcoin works, then the easy way is just a dull diversion before you get to the real stuff.

## What's Next?

Continue "Sending Bitcoin Transactions" with [§4.2 Creating a Raw Transaction](04_2_Creating_a_Raw_Transaction.md).
