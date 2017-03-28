# 4.1: Sending Coins the Easy Way

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

The `bitcoin-cli` offers three major ways to send coins: as a simple command; as a raw transaction; and as a raw transaction with calculation. Each has their own advantages and disadvantages. This first method for sending coins is also the simplest.

## Set Your Transaction Fee

Before you send any money on the Bitcoin network, you should think about what transaction fees you're going to pay.

_What is a transaction fee?_ There's no such thing as a free lunch. Miners incorporate transactions into blocks because they're paid to do so. Not only do they get paid by the network for making the block, but they also get paid by transactors for including their transactions. If you don't pay a fee, your transaction might get stuck ... forever (or, until saved by some of the tricks in section five). 

When you're using the simple and automated methods for creating transactions, as outlined in sections 4.1 and 4.4, Bitcoin will calculate transaction fees for you. This is done using Floating Fees, where the `bitcoind` watches how long transactions are taking to confirm and automatically calculates for you what to spend.

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

You need somewhere to send your coins to. Usually, someone would send you an address, and perhaps give you a signature to prove they own that address. Alternatively, they might give you a QR code to scan, so that you can'tmake mistakes when typing in the address. In our case, we're going to send coins to `n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi`, which is a return address for TP's TestNet faucet.

_What is a QR code?_ A QR code is just an encoding of a Bitcoin address. Many wallets will generate QR codes for you, while some sites will convert from an address to a QR code. Obviously, you should only accept a QR code from a site that you absolutely trust. A payer can use a bar-code scanner to read in the QR code, then pay to it.

## Send the Coins

You're now ready to send some coins. This is actually quite simple via the command line. You just use `bitcoin-cli sendtoaddress [address] [amount]`. So, to send a little coinage to the address `n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi` just requires:
```
$ bitcoin-cli sendtoaddress n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi 0.1
6ad295c280798e9746dcdf7e5a60dfb6219d93bf31aab9b540ce892537c41e0c
```
Make sure the address you write in is where you want the money to go. Make _double_ sure. If you make mistakes in Bitcoin, there's no going back. 

You'll receive a txid back when you issue this command.

> **WARNING:** The `bitcoin-cli` command actually generates JSON-RPC commands when it's talking to the bitcoind. They can be really picky. This is an example: if you list the bitcoin amount without the leading zero (i.e. ".1" instead of "0.1"), then bitcoin-cli will fail with a mysterious message.

## Examine Your Transaction

You'll be able to see the transaction in your list immediately. 
```
$ bitcoin-cli listtransactions

```
However, note that as always it'll take a while for your balances to settle (particularly if your transaction minimums are set verylow).

## Summary: Sending Coins the Easy Way

To send coins the easy way, make sure your transaction defaults are rationale, get an address, and send coins there. That's why they call it easy!

### Why Use The Easy Way?

_The advantages._ It's easy. You don't have to worry about arcane things like UTXOs. You don't have to calculate transaction fees by hand, so you're not likely to make mistakes that cost you large amounts of money. If your sole goal is to sit down at your computer and send some money, this is the way to go.

_The disadvantages._ You have very little control over what's happening, and you can't do anything fancy. If you're planning to write more complex Bitcoin software or want a deeper understanding of how Bitcoin works, then the easy way is to more than a dull diversion before you get to the real stuff.
