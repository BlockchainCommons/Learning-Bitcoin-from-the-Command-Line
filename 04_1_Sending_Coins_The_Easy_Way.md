# 4.1: Sending Coins the Easy Way

The `bitcoin-cli` offers three major ways to send coins: as a simple command; as a raw transaction; and as a raw transaction with calculation. Each has their own advantages and disadvantages. This first method for sending coins is also the simplest.

## Set Your Transaction Fee

Before you send any money on the Bitcoin network, you should think about what transaction fees you're going to pay.

> :book: ***What is a transaction fee?*** There's no such thing as a free lunch. Miners incorporate transactions into blocks because they're paid to do so. Not only do they get paid by the network for making the block, but they also get paid by transactors for including their transactions. If you don't pay a fee, your transaction might get stuck ... forever (or, until saved by some of the tricks in [Chapter Five](05_0_Controlling_Bitcoin_Transactions.md)).

When you're using the simple and automated methods for creating transactions, as outlined here and in [§4.5: Sending Coins with Automated Raw Transactions](04_5_Sending_Coins_with_Automated_Raw_Transactions.md), Bitcoin will calculate transaction fees for you. This is done using Floating Fees, where the `bitcoind` watches how long transactions are taking to confirm and automatically calculates for you what to spend.

You can help control this by putting rational values into your `~/.bitcoin/bitcoin.conf`. The following low-cost values would ensure that there was a minimum transaction fee of 10,000 satoshis per kByte of data in your transaction and request that the floating fees figure out a good amount to get your transaction somewhere into the next six blocks.
```
mintxfee=0.0001
txconfirmtarget=6
```
However, under the theory that you don't want to wait around while working on a tutorial, we've adopted the following higher values:
```
mintxfee=0.001
txconfirmtarget=1
```
You should enter these into `~/.bitcoin/bitcoin.conf`, in the main section, toward the top of the file or if you want to be sure you never use it elsewhere, under the `[test]` section.

In order to get through this tutorial, we're willing to spend 100,000 satoshis per kB on every transaction (about $10!), and we want to get each transaction into the next block! (To put that in perspective, a typical transaction runs between .25 kB and 1 kB, so you'll actually be paying more like $2.50 than $10 ... if this were real money.)

After you've edited your bitcoin.conf file, you'll want to kill and restart bitcoind.
```
$ bitcoin-cli stop
$ bitcoind -daemon
```

## Get an Address

You need somewhere to send your coins to. Usually, someone would send you an address, and perhaps give you a signature to prove they own that address. Alternatively, they might give you a QR code to scan, so that you can't make mistakes when typing in the address. In our case, we're going to send coins to `n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi`, which is a return address for an old Tesetnet faucet.

> :book: ***What is a QR code?*** A QR code is just an encoding of a Bitcoin address. Many wallets will generate QR codes for you, while some sites will convert from an address to a QR code. Obviously, you should only accept a QR code from a site that you absolutely trust. A payer can use a bar-code scanner to read in the QR code, then pay to it.

## Send the Coins

You're now ready to send some coins. This is actually quite simple via the command line. You just use `bitcoin-cli sendtoaddress [address] [amount]`. So, to send a little coinage to the address `n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi` just requires:
```
$ txid=$(bitcoin-cli sendtoaddress n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi 0.001)
$ echo $txid
93250d0cacb0361b8e21030ac65bc4c2159a53de1075425d800b2d7a8ab13ba8
```

> 🙏 To help keep testnet faucets alive, try to use the return address of the same faucet you used in the previous chapter on receiving transactions. 

Make sure the address you write in is where you want the money to go. Make _double_ sure. If you make mistakes in Bitcoin, there's no going back.

You'll receive a txid back when you issue this command.

> ❕ You may end up with an error code if you don't have enough funds in your wallet to send the transaction. Depending on your current balance `bitcoin-cli getbalance` you may need to adjust the amount to be sent to account for the amount being sent along with the transaction fee. If your current balance is 0.001, then you could try sending 0.0001. Alternatively, it would be better to instead subtract the expected fee given in the error message from your current balance. This is good practice as many wallets expect you to calculate your own amount + fees when withdrawing, even among popular exchanges. 

> :warning: **WARNING:** The `bitcoin-cli` command actually generates JSON-RPC commands when it's talking to the bitcoind. They can be really picky. This is an example: if you list the bitcoin amount without the leading zero (i.e. ".1" instead of "0.1"), then bitcoin-cli will fail with a mysterious message.

> :warning: **WARNING:** Even if you're careful with your inputs, you could see "Fee estimation failed. Fallbackfee is disabled." Fundamentally, this means that your local `bitcoind` doesn't have enough information to estimate fees. You should really never see it if you've waited for your blockchain to sync and set up your system with Bitcoin Standup. But if you're not entirely synced, you may see this. It also could be that you're not using a standard `bitcoin.conf`: the entry `blocksonly=1` will cause your `bitcoind` to be unable to estimate fees.

## Examine Your Transaction

You can look at your transaction using your transaction id:
```
{
  "amount": -0.00100000,
  "fee": -0.00022200,
  "confirmations": 0,
  "trusted": true,
  "txid": "93250d0cacb0361b8e21030ac65bc4c2159a53de1075425d800b2d7a8ab13ba8",
  "walletconflicts": [
  ],
  "time": 1592604194,
  "timereceived": 1592604194,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi",
      "category": "send",
      "amount": -0.00100000,
      "vout": 1,
      "fee": -0.00022200,
      "abandoned": false
    }
  ],
  "hex": "0200000001e982921bb0189afc486e20bb05cc5825c71a0ba8868043ed04ece9ab0cb12a8e010000006a47304402200fc493a01c5c9d9574f7c321cee6880f7f1df847be71039e2d996f7f75c17b3d02203057f5baa48745ba7ab5f1d4eed11585bd8beab838b1ca03a4138516fe52b3b8012102fd5740996d853ea51a6904cf03257fc11204b0179f344c49739ec5b20b39c9bafeffffff02e8640d0000000000160014d37b6ae4a917bcc873f6395741155f565e2dc7c4a0860100000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac780b1b00"
}
```
You can see not only the amount transferred (.001 BTC) but also a transaction fee (.000222 BTC), which is about a quarter of the .001 BTC/kB minimum fee that was set, which suggests that the transaction was about a quarter of a kB in size.

While you are waiting for this transaction to clear, you'll note that `bitcoin-cli getbalance` shows that all of your money is gone (or, at least, all of your money from a single incoming transaction). Similarly, `bitcoin-cli listunspent` will show that an entire transaction is gone, even if it was more than what you wanted to send. There's a reason for this: whenever you get money in, you have to send it _all_ out together, and you have to perform some gymnastics if you actually want to keep some of it! Once again, `sendtoaddress` takes care of this all for you, which means you don't have to worry about making change until you send a raw transaction. In this case, a new transaction will appear with your change when your spend is incorporated into a block.

## Summary: Sending Coins the Easy Way

To send coins the easy way, make sure your transaction defaults are rationale, get an address, and send coins there. That's why they call it easy!

> :fire: ***What is the power of sending coins the easy way?***

> _The advantages._ It's easy. You don't have to worry about arcane things like UTXOs. You don't have to calculate transaction fees by hand, so you're not likely to make mistakes that cost you large amounts of money. If your sole goal is to sit down at your computer and send some money, this is the way to go.

> _The disadvantages._ It's high level. You have very little control over what's happening, and you can't do anything fancy. If you're planning to write more complex Bitcoin software or want a deeper understanding of how Bitcoin works, then the easy way is just a dull diversion before you get to the real stuff.

## What's Next?

Continue "Sending Bitcoin Transactions" with [§4.2 Creating a Raw Transaction](04_2_Creating_a_Raw_Transaction.md).
