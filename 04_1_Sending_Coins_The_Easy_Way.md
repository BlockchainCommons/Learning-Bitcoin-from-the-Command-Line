TODO:
* Also: sendall as an alternative

==

# 4.1: Sending Coins the Easy Way

The `bitcoin-cli` offers three major ways to send coins: as a simple command; as a raw transaction; and as a raw transaction with calculation. Each has their own advantages and disadvantages. This first method for sending coins is also the simplest.

## Set Your Transaction Fee

Before you send any money on the Bitcoin network, you should think about what transaction fees you're going to pay.

> :book: ***What is a transaction fee?*** There's no such thing as a free lunch. Miners incorporate transactions into blocks because they're paid to do so. Not only do they get paid by the network for making the block, but they also get paid by transactors for including their transactions. If you don't pay a fee, your transaction might get stuck ... forever (or, until saved by some of the tricks in [Chapter Five](05_0_Controlling_Bitcoin_Transactions.md)).

When you're using the simple and automated methods for creating transactions, as outlined here and in [§4.5: Sending Coins with Automated Raw Transactions](04_5_Sending_Coins_with_Automated_Raw_Transactions.md), Bitcoin will calculate transaction fees for you. This is done using Floating Fees, where the `bitcoind` watches how long transactions are taking to confirm and automatically calculates for you what to spend.

You can help control how Floating Fees determines your transaction fees by putting rational values into your `~/.bitcoin/bitcoin.conf`. This is done by setting up to four values. They would usually be placed in the top of your file, but they could be placed in a `[test]`, `[regtest]`, or `[signet]` section if you want to place higher values for your testing (when you're waiting around), but lower values for actual payments on a real network.

| Variable | Default | Explanation |
|----------|---------|-------------|
| `fallbackfee` | 0 | Precise BTC/kvB for transaction if Floating Fees fails |
| `maxtxfee` | .1 | Maximum total BTC for transaction using Floating Fees |
| `mintxfee` | 0.00001 | Minimum BTC/kvB for transaction using Floating Fees |
| `paytxfee` | 0 | Precise BTC/kvB of transaction size without Floating Fees (deprecated) |
| `txconfirmtarget` | 6 | Average confirmations for automated fee calculation using Floating Fees |

Since the `paytxfee` has been deprecated, you'll mostly be depending on `mintxfee` and `txconfirmtarget`, which work like this:

1. If `paytxfee` is set, Floating Fees is not used.
2. If there is insufficient data for Floating Fees, use `fallbackfee`.
3. Otherwise, Floating Fees sees how much fee is required for your transaction to be accepted within `txconfirmtarget` blocks.
4. Floating Fees sets the transaction fee for your transaction to that value.
5. Floating Fees sees if that value is lower than `mintxfee`, if so it increases your fee to the value of `mintxfee`.
6. Floating Fees sees if the total value would be higher than `maxtxfee`, if so it decreases your fee to the value of `maxtxfee`.

As shown, the default value for `mintxfee` is 0.00001 BTC/kvB or one-one hundredth of a BTC for every virtual kB of data in your transaction. That's the equivalent to 1,000 Satoshis per kvB or (more simply) 1 Satoshi per virtual Byte. The sats/vB or sat/B measure is what most wallets and explorers use nowadays, because small numbers in the range of .1 sat/vB to 10 sat/vB are much easier to understand than values such as 0.000001 BTC or 0.0001 BTC. However, Bitcoin Core continues to use the older measures.

> :book: ***What is a virtual byte or virtual kilobyte?*** Transactions fees used to be measured as BTC/kB, but following the block size wars and the adoption of Segwit, the value was changed to BTC/kvB or sat/vB. The "v" stands for virtual and it reflects the fact that the size of Segwit transactions are "discounted" because their signatures are placed in block space that didn't previously exist. So a virtual byte (or virtual kilobyte) isn't the _actual_ size of a transaction, but the size that you're required to pay for, including discounts.

To put this all in perspective, the average size of a transaction runs as low as 226 bytes for a legacy P2PKH transaction, which discounts down to 144 virtual bytes for a modern P2WPKH transaction. Precise size depends on how many inputs in your transactions, how many outputs, what type of Signature system you're using, and how many signatures you have. But assuming an average transaction, here's what it cost if Bitcoin has a current value of $100,000 USD/BTC:

| Transaction Type | Size | `mintxfee` | sat/vB | Satoshi Cost | USD Cost |
|------------------|------|------------|--------|--------------|----------|
| Legacy, 2-output | 226 B | 0.00001 | 1 | 226 | $.23 |
| Legacy, 2-output | 226 B | 0.00003 | 3 | 678 | $.68 |
| Legacy, 2-output | 226 B | 0.0001 | 10 | 2260 | $2.26 |
| SegWit, 2-output | 144 B | 0.00001 | 1 | 144 | $.15 |
| SegWit, 2-output | 144 B | 0.00003 | 1 | 432 | $.43 |
| SegWit, 2-output | 144 B | 0.0001 | 1 | 1440 | $1.44 |

The default values (`mintxfee=0.00001` and `txconfirmtarget=6`) would be fine for most real-world use as of this writing, where fees tend to average 1 sat/vB except over short periods (usually hours) of high usage. And of course those values are just the starting point for the Floating Fees calculation: they'll go higher if the calculations suggest that you need to pay more money to get a transaction in the next 6 blocks. 

For testing, where a more immediate response is more important, and where you might not have enough data to calculate Floating Fees, you may want to push the values up, however, perhaps to:
```
fallbackfee=0.0001
mintxfee=0.0001
txconfirmtarget=1
```
In order to get through this tutorial, we're willing to spend 10,000 satoshis per kB on every transaction (about a buck and a half for common SegWit transactions), and we want to get each transaction into the next block! (Not recommended for real money!)

After you've edited your bitcoin.conf file, you'll want to kill and restart bitcoind.
```
$ bitcoin-cli stop
$ bitcoind -daemon
```

## Get an Address

You need somewhere to send your coins to. Usually, someone would send you an address, and perhaps give you a signature to prove they own that address. Alternatively, they might give you a [QR code](03_6_Creating_QR_Codes_for_Addresses.md) to scan, so that you can't make mistakes when typing in the address. In our case, we're going to send coins to `tb1qg3lau83hm9e9tdvzr5k7aqtw3uv0dwkfct4xdn`, which is a return address for one of the Signet faucets.

## Send the Coins

You're now ready to send some coins. This is actually quite simple via the command line. You just use `bitcoin-cli sendtoaddress [address] [amount]`. So, to send a little coinage to the address `tb1qg3lau83hm9e9tdvzr5k7aqtw3uv0dwkfct4xdn` just requires:
```
$ txid=$(bitcoin-cli sendtoaddress tb1qg3lau83hm9e9tdvzr5k7aqtw3uv0dwkfct4xdn 0.001)
$ echo $txid
cb48282e86c846b8357374d74e9ea24deeaeb48cf447634a8b951c98cfc559c5
```

Make sure the address you write in is where you want the money to go. Make _double_ sure. If you make mistakes in Bitcoin, there's no going back.

> 🙏 **Save the Faucets!** To help keep signet faucets alive, try to use the return address of a Signet faucet if one is provided, once you're done testing with coins.

You'll receive a txid back when you issue this command.

> ⚠️ **WARNING: Picky Input.** The `bitcoin-cli` command actually generates JSON-RPC commands when it's talking to the bitcoind. JSON-RPC can be really picky. This is an example: if you list the bitcoin amount without the leading zero (i.e. ".001" instead of "0.001"), then bitcoin-cli will fail with a mysterious message, currently `error: Error parsing JSON: .001`.

> ⚠️ **WARNING: Insufficient Funds.** You may end up with an error code if you don't have enough funds in your wallet to send the transaction. This may be non-intuitive, because you may see you have the funds to cover a transaction, but you could be forgetting about the transaction fee that `sendtoaddress` automatically calculates for you. Depending on your current balance of `bitcoin-cli getbalance`, you may therefore need to adjust the amount to be sent to account for the amount being sent along with the transaction fee.

> ⚠️ **WARNING: Insufficient Blocks.** If you see the error "Fee estimation failed. Fallbackfee is disabled" that means that your `bitcoind` doesn't have enough data to estimate fees, probably because the blockchain isn't synced yet. If so, set the `fallbackfee` value noted above, restart your `bitcoind` and try again.

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
