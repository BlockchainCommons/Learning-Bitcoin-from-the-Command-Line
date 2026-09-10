# 5.1: Sending Coins the Easy Way

The `bitcoin-cli` offers three major ways to send coins: as one of a
few simple commands; as a raw transaction; and as a raw transaction
with calculation. Each has their own advantages and
disadvantages. This first method for sending coins is also the
simplest.

## Set Your Transaction Fee

Before you send any money on the Bitcoin network, you should think about what transaction fees you're going to pay.

> 📖 ***What is a transaction fee?*** There's no such thing as a free
lunch. Miners incorporate transactions into blocks because they're
paid to do so. Not only do they get paid by the network for making the
block, but they also get paid by transactors for including their
transactions. If you don't pay a fee, your transaction might get stuck
... forever (or, until saved by some of the tricks in [Chapter
Six](06_0_Controlling_Bitcoin_Transactions.md)).

When you're using the simple and automated methods for creating
transactions, as outlined here and in [§5.5: Sending Coins with
Automated Raw
Transactions](05_5_Sending_Coins_with_Automated_Raw_Transactions.md),
Bitcoin will calculate transaction fees for you. This is done using
Floating Fees, where the `bitcoind` watches how long transactions are
taking to confirm and automatically calculates for you what to spend.

You can help control how Floating Fees determines your transaction fees by putting rational values into your `~/.bitcoin/bitcoin.conf`. This is done by setting up to five values. They would usually be placed in the top of your file, but they could be placed in a `[test]`, `[regtest]`, or `[signet]` section if you want to place higher values for your testing (when you're waiting around), but lower values for actual payments on a real network.

| Variable | Default | Explanation |
|----------|---------|-------------|
| `fallbackfee` | 0 | Precise BTC/kvB for transaction if Floating Fees fails |
| `maxtxfee` | .1 | Maximum total BTC for transaction using Floating Fees |
| `mintxfee` | 0.00001 | Minimum BTC/kvB for transaction using Floating Fees |
| `paytxfee` | 0 | Precise BTC/kvB of transaction size without Floating Fees (deprecated) |
| `txconfirmtarget` | 6 | Average confirmations for automated fee calculation using Floating Fees |

Since the `paytxfee` has been deprecated, you'll mostly be depending on `mintxfee` and `txconfirmtarget` (and perhaps `fallbackfee` for testing), which work like this:

1. If `paytxfee` is set, Floating Fees is not used.
2. If there is insufficient data for Floating Fees, `fallbackfee` is used.
3. Otherwise, Floating Fees calculates how much fee is required for your transaction to be accepted within `txconfirmtarget` blocks.
4. Floating Fees sets the transaction fee for your transaction to that value.
5. Floating Fees sees if that value is lower than `mintxfee`, if so it increases your fee to the value of `mintxfee`.
6. Floating Fees sees if the total value would be higher than `maxtxfee`, if so it decreases your fee to the value of `maxtxfee`.

As shown, the default value for `mintxfee` is 0.00001 BTC/kvB or
one-one hundredth of a BTC for every virtual kB of data in your
transaction. That's the equivalent to 1,000 Satoshis per kvB or (more
simply) 1 Satoshi per virtual Byte. The sats/vB or sat/B measure is
what most wallets and explorers use nowadays, because small numbers in
the range of .1 sat/vB to 10 sat/vB are much easier to understand than
values from 0.0001 BTC to 0.000001 BTC. However, Bitcoin Core
continues to use the older measures.

> 📖 ***What is a virtual byte or virtual kilobyte?*** Transactions
fees used to be measured as BTC/kB, but following the Block Size Wars
and the adoption of SegWit, the value was changed to BTC/kvB or
sat/vB. The "v" stands for virtual and it reflects the fact that the
size of Segwit transactions are "discounted" because their signatures
are placed in block space that didn't previously exist. So a virtual
byte (or virtual kilobyte) isn't the _actual_ size of a transaction,
but the size that you're required to pay for, including discounts.

To put this all in perspective, the average size of a transaction runs
as low as 226 bytes for a legacy P2PKH transaction, which discounts
down to 144 virtual bytes for a modern P2WPKH transaction. Precise
size depends on the number of inputs to your transactions, the number
of outputs, what type of Signature system you're using, and how many
signatures you have. But assuming an average transaction, here's what
it cost if Bitcoin has a current value of $100,000 USD/BTC:

| Transaction Type | Size | `mintxfee` | sat/vB | Satoshi Cost | USD Cost |
|------------------|------|------------|--------|--------------|----------|
| Legacy, 2-output | 226 B | 0.00001 | 1 | 226 | $.23 |
| Legacy, 2-output | 226 B | 0.00003 | 3 | 678 | $.68 |
| Legacy, 2-output | 226 B | 0.0001 | 10 | 2260 | $2.26 |
| SegWit, 2-output | 144 B | 0.00001 | 1 | 144 | $.15 |
| SegWit, 2-output | 144 B | 0.00003 | 1 | 432 | $.43 |
| SegWit, 2-output | 144 B | 0.0001 | 1 | 1440 | $1.44 |

The default values (`mintxfee=0.00001` and `txconfirmtarget=6`) are
fine for most real-world use as of this writing, where fees tend to
average 1 sat/vB except over short periods (usually hours) of high
blockchain usage. And of course those values are just the starting
point for the Floating Fees calculation: they'll go higher if the
calculations suggest that you need to pay more money to get a
transaction in the next 6 blocks.

For testing, where a more immediate response is more important, and where you might not have enough data to calculate Floating Fees, you may want to push the values up, however, perhaps to:

```
fallbackfee=0.0001
mintxfee=0.0001
txconfirmtarget=1
```

In order to get through this tutorial, we're willing to spend 10,000
satoshis per kB on every transaction (about a buck and a half for
common SegWit transactions), and we want to get each transaction into
the next block! (Not recommended for real money!)

After you've edited your bitcoin.conf file, you'll want to kill and restart bitcoind.

```sh
bitcoin-cli stop

bitcoind -daemon
```

## Get an Address

You need somewhere to send your coins to. Usually, someone would send
you an address, and perhaps give you a signature to prove they own
that address. Alternatively, they might give you a [QR
code](04_3_Creating_QR_Codes_for_Addresses.md) to scan, so that you
can't make mistakes when typing in the address. In our case, we're
going to send coins to `tb1qg3lau83hm9e9tdvzr5k7aqtw3uv0dwkfct4xdn`,
which is a return address for one of the Signet faucets.

## Send the Coins

You're now ready to send some coins. This is actually quite simple via
the command line. You just use `bitcoin-cli sendtoaddress [address]
[amount]`. So, to send a little coinage to the address
`tb1qg3lau83hm9e9tdvzr5k7aqtw3uv0dwkfct4xdn` just requires:

```sh
txid=$(bitcoin-cli sendtoaddress tb1qg3lau83hm9e9tdvzr5k7aqtw3uv0dwkfct4xdn 0.001)
echo $txid

| cb48282e86c846b8357374d74e9ea24deeaeb48cf447634a8b951c98cfc559c5
```

Make sure the address you write in is where you want the money to go. Make _double_ sure. If you make mistakes in Bitcoin, there's no going back.

> 🙏 **Save the Faucets!** To help keep Signet faucets alive, try to use the return address of a Signet faucet if one is provided, once you're done testing with coins.

You'll receive a txid back when you issue this command.

> ⚠️ **WARNING: Picky Input.** The `bitcoin-cli` command actually
generates JSON-RPC commands when it's talking to `bitcoind`. JSON-RPC
can be really picky. This is an example: if you list the bitcoin
amount without the leading zero (i.e. ".001" instead of "0.001"), then
bitcoin-cli will fail with a mysterious message, currently `error:
Error parsing JSON: .001`.

> ⚠️ **WARNING: Insufficient Funds.** You may end up with an error code
if you don't have enough funds in your wallet to send the
transaction. This may be non-intuitive, because you may see you have
the funds to cover a transaction, but you could be forgetting about
the transaction fee that `sendtoaddress` automatically calculates for
you. Depending on your current balance of `bitcoin-cli getbalance`,
you may therefore need to adjust the amount to be sent to account for
the amount being sent along with the transaction fee.

> ⚠️ **WARNING: Insufficient Blocks.** If you see the error "Fee
estimation failed. Fallbackfee is disabled" that means that your
`bitcoind` doesn't have enough data to estimate fees, probably because
the blockchain isn't synced yet. If so, set the `fallbackfee` value
noted above, restart your `bitcoind` and try again.

## Examine Your Transaction

You can look at your transaction using your transaction id:

```sh
bitcoin-cli gettransaction $txid

| {
|   "amount": -0.00100000,
|   "fee": -0.00001410,
|   "confirmations": 4,
|   "blockhash": "00000000b594f3a382e1b13fac4847d8790dda630f31335e0e0c42abaaaef7b6",
|   "blockheight": 293297,
|   "blockindex": 2,
|   "blocktime": 1772134375,
|   "txid": "cb48282e86c846b8357374d74e9ea24deeaeb48cf447634a8b951c98cfc559c5",
|   "wtxid": "f8f52643b864848b668307df03e192bedf0349ba3c067258c9fb698652061f82",
|   "walletconflicts": [
|   ],
|   "mempoolconflicts": [
|   ],
|   "time": 1772133414,
|   "timereceived": 1772133414,
|   "bip125-replaceable": "no",
|   "details": [
|     {
|       "address": "tb1qg3lau83hm9e9tdvzr5k7aqtw3uv0dwkfct4xdn",
|       "category": "send",
|       "amount": -0.00100000,
|       "vout": 0,
|       "fee": -0.00001410,
|       "abandoned": false
|     }
|   ],
|   "hex": "02000000000101356d1fee7ad60b61af6289b394394e83470b7eb31014533bca3753387b1a53af8702000000fdffffff02a086010000000000160014447fde1e37d97255b5821d2dee816e8f18f6bac9420c0300000000001600141026285b51dc0aa759119f4b5f63cb364a5aa8cd02473044022069e48cacbeabe4e83892d84380d2bc01ccb0d7bc253fb1d9e9198e7fbd26544702201e9b1eeb83f380a184f9f24882d8c8aa2a3516fb9013816038578869f8ce5b58012103abf1f9337ad039d7c80f34e9de656cf479de074251ae2601bca7f8c05e984a23ac790400",
|   "lastprocessedblock": {
|     "hash": "00000007d62a1ce40b164b7e8b909e743e1a43f1f0a740e5d4d4b776f40d841b",
|     "height": 293300
|   }
| }
```
You can see not only the amount transferred (`0.001` BTC) but also a transaction fee (`0.00001410` BTC).

If you look at `bitcoin-cli listunspent`, you will see that one of the transactions where you received money (probably from a faucet) is entirely gone. Don't panic! After your `sendtoaddress`  has been confirmed, it will be replaced by a new transaction that's dated to the same time as the money you sent out:

```sh
|   {
|     "txid": "cb48282e86c846b8357374d74e9ea24deeaeb48cf447634a8b951c98cfc559c5",
|     "vout": 1,
|     "address": "tb1qzqnzsk63ms92wkg3na947c7txe9942xdlajgzs",
|     "scriptPubKey": "00141026285b51dc0aa759119f4b5f63cb364a5aa8cd",
|     "amount": 0.00199746,
|     "confirmations": 4,
|     "spendable": true,
|     "solvable": true,
|     "desc": "wpkh([b8309bae/84h/1h/0h/1/0]02af4466702f92acef7f9e333cfed298740c15ea0fbeaae0c307d41745792483c6)#up0l68qf",
|     "parent_descs": [
|       "wpkh([b8309bae/84h/1h/0h]tpubDDpSvPDUjstxFUEWzHkaL4qykf8vjNCspm8SZ26Z1wgPFbd63AdYrn4bDpEGPT1giJ6gcLW8Xou8fnhi35DJrUza9ikgu5dg2mDkd8jQpA6/1/*)#eeegkjmk"
|     ],
|     "safe": true
|   }
```

This is all expected. Cryptocurrency doesn't come in bills and coins,
like physical money, but instead in singular blobs. If someone sends
you 0.01798971 BTC, then that entire amount is stored in the
transaction that sent you the money. When you pay someone else, you
use that transactions, send part of it to your recipient _and_ send
the rest back to yourself as "change", forming a new address. More on
that in the next section. Fortunately, `sendtoaddress` takes care of
this for you, generating a change address and looping the remaining
funds back: that's the additional transaction you see in your wallet
(and why a previous transaction disappeared). It'll just take a short
time to show up (usually 10 minutes or less if you're using robust
transaction-fee values on Signet).

## Send Coins the Even Easier Way

There's perhaps an even easier way to send coins in Bitcoin, though it
has less applicability than `sendtoaddress`. It's `sendall` and it
works like this:

```sh
bitcoin-cli sendall '["tb1qxe0nn84xxw76jfc0s93g8kuefu2llglcvscy28"]'

| {
|   "txid": "593a8eec3302c76532f6cd5f2e2c0e2f5eb1b3bd9b45d72279ad23b23cf13005",
|   "complete": true
| }
```

You give `sendall` a JSON array of address (just one here), and it'll
send _all_ the money from your wallet to those addresses, equally
split among them. There are some additional options, such as
specifying amounts for certain addresses, with the balance of the
funds then being divided among the rest.

If you start with a wallet with multiple transactions:

```sh
bitcoin-cli listunspent

| [
|   {
|     "txid": "f2351b40729f714abd5eaeb4bd387b97268c89995c0d5f90fc3558954d5911c5",
|     "vout": 1,
|     "address": "tb1qxmx50fc06jt67nw6eqdcsm3rexgn6qvcet3rcm",
|     "label": "",
|     "scriptPubKey": "001436cd47a70fd497af4ddac81b886e23c9913d0198",
|     "amount": 0.01798971,
|     "confirmations": 9,
|     "spendable": true,
|     "solvable": true,
|     "desc": "wpkh([b8309bae/84h/1h/0h/0/2]033dbb8f06ec43956d9f177e43d9d3bb9b2021cf78feb2a48116a984a54ef5a7db)#q3jnn2nv",
|     "parent_descs": [
|       "wpkh([b8309bae/84h/1h/0h]tpubDDpSvPDUjstxFUEWzHkaL4qykf8vjNCspm8SZ26Z1wgPFbd63AdYrn4bDpEGPT1giJ6gcLW8Xou8fnhi35DJrUza9ikgu5dg2mDkd8jQpA6/0/*)#gduft8tw"
|     ],
|     "safe": true
|   },
|   {
|     "txid": "cb48282e86c846b8357374d74e9ea24deeaeb48cf447634a8b951c98cfc559c5",
|     "vout": 1,
|     "address": "tb1qzqnzsk63ms92wkg3na947c7txe9942xdlajgzs",
|     "scriptPubKey": "00141026285b51dc0aa759119f4b5f63cb364a5aa8cd",
|     "amount": 0.00199746,
|     "confirmations": 4,
|     "spendable": true,
|     "solvable": true,
|     "desc": "wpkh([b8309bae/84h/1h/0h/1/0]02af4466702f92acef7f9e333cfed298740c15ea0fbeaae0c307d41745792483c6)#up0l68qf",
|     "parent_descs": [
|       "wpkh([b8309bae/84h/1h/0h]tpubDDpSvPDUjstxFUEWzHkaL4qykf8vjNCspm8SZ26Z1wgPFbd63AdYrn4bDpEGPT1giJ6gcLW8Xou8fnhi35DJrUza9ikgu5dg2mDkd8jQpA6/1/*)#eeegkjmk"
|     ],
|     "safe": true
|   }
| ]
```

It'll be emptied out by this command:

```sh
bitcoin-cli listunspent

| [
| ]
```

After sufficient blocks have been confirmed, the recipient should see the funds, minus transaction fees (which are by default set to 1 sat/vB, ignoring values such as `mintxfee`).


```sh
bitcoin-cli listunspent

| [
|   {
|     "txid": "593a8eec3302c76532f6cd5f2e2c0e2f5eb1b3bd9b45d72279ad23b23cf13005",
|     "vout": 0,
|     "address": "tb1qxe0nn84xxw76jfc0s93g8kuefu2llglcvscy28",
|     "label": "",
|     "scriptPubKey": "0014365f399ea633bda9270f816283db994f15ffa3f8",
|     "amount": 0.01996947,
|     "confirmations": 1,
|     "spendable": true,
|     "solvable": true,
|     "desc": "wpkh([b8309bae/84h/1h/0h/0/3]02e60a25d4492c3ddd4a31cdba5d57df163359e623e3fb2eda15b8ffc050e0cc42)#wfg3yz80",
|     "parent_descs": [
|       "wpkh([b8309bae/84h/1h/0h]tpubDDpSvPDUjstxFUEWzHkaL4qykf8vjNCspm8SZ26Z1wgPFbd63AdYrn4bDpEGPT1giJ6gcLW8Xou8fnhi35DJrUza9ikgu5dg2mDkd8jQpA6/0/*)#gduft8tw"
|     ],
|     "safe": true
|   }
| ]
```

Why would you do this? Mainly to sweep a wallet.

> 📖 **What is Sweeping?** Sweeping means that you're emptying a
wallet (or address) and moving everything to another wallet (or
address). This might be done if a wallet's keys have been compromised,
if some keys in a multisig have been lost, or if you're consolidating
funds to a different location. `bitcoin-cli` is able to sweep your
whole wallet because of its descriptors: those function-like
descriptions help `bitcoin-cli` to find all the funds you
prossess. There is of course always a fee for moving funds around on
Bitcoin, even if you're transferring from one wallet you control to
another.

## Summary: Sending Coins the Easy Way

To send coins the easy way, make sure your transaction defaults are
rationale, get an address, and send coins there. That's why they call
it easy! Besides using `sendtoaddress`, you can also use `sendall`,
but only if you want to empty a wallet!

> 🔥 ***What is the power of sending coins the easy way?***

> _The advantages._ It's easy. You don't have to worry about arcane things like UTXOs. You don't have to calculate transaction fees by hand, so you're not likely to make mistakes that cost you large amount`s of money. If your sole goal is to sit down at your computer and send some money, this is the way to go.

> _The disadvantages._ It's high level. You have very little control over what's happening, and you can't do anything fancy. If you're planning to write more complex Bitcoin software or want a deeper understanding of how Bitcoin works, then the easy way is just a dull diversion before you get to the real stuff.

## What's Next?

Continue "Sending Bitcoin Transactions" with [§5.2 Creating a Raw Transaction](05_2_Creating_a_Raw_Transaction.md).
