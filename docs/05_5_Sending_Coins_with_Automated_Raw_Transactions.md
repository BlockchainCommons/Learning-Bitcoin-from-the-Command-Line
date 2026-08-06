# 5.5: Sending Coins with Automated Raw Transactions

This chapter lays out three ways to send funds via Bitcoin's cli
interface. [§5.1](05_1_Sending_Coins_The_Easy_Way.md) described how to
do so with a simple command and
[§5.4](05_4_Sending_Coins_with_a_Raw_Transaction.md) detailed how to
use a more dangerous raw transaction. This final section splits the
difference by showing how to make raw transactions simpler and safer
using automation.

## Let Bitcoin Calculate For You

The methodology for automated raw transactions is simple: you create a raw transaction, but you use the `fundrawtransaction` command to ask the bitcoind to run the calculations for you.

In order to use this command, you'll need to ensure that your ~/.bitcoin/bitcoin.conf file contains rational variables for calculating transaction fees. Please see [§5.1: Sending Coins The Easy Way](05_1_Sending_Coins_The_Easy_Way.md) for more information on this.

## Create a Bare Bones Raw Transaction

To use `fundrawtransaction` you first need to create a bare-bones raw transaction that lists _no_ inputs and _no_ change address. You'll just list your recipient and how much you want to send them, in this case `$recipient` and `0.0002` BTC.

```sh
recipient=tb1qg3lau83hm9e9tdvzr5k7aqtw3uv0dwkfct4xdn
unfinishedtx=$(bitcoin-cli -named createrawtransaction inputs='''[]''' outputs='''{ "'$recipient'": 0.002 }''')
```

## Fund Your Bare Bones Transaction

You then tell `bitcoin-cli` to fund that bare-bones transaction:

```sh
bitcoin-cli -named fundrawtransaction hexstring=$unfinishedtx

| {
|   "hex": "02000000016524475f2cb9b7af59f86d25183d6dbe79b41b248a5a40c5e608ce79ca9f61800000000000fdffffff02400d030000000000160014447fde1e37d97255b5821d2dee816e8f18f6bac9be711b0000000000160014362520eaa80f35e99a8823ef8ac56e67e4a6c2af00000000",
|   "fee": 0.00001410,
|   "changepos": 1
| }
```
That provides useful information such as the seelected fee, but once you're confident with how it works, you'll want to use JQ to save your hex to a variable, as usual:

```sh
rawtxhex3=$(bitcoin-cli -named fundrawtransaction hexstring=$unfinishedtx | jq -r '.hex')
```
## Verify Your Funded Transaction

It seems like magic, so the first few times you use `fundrawtransaction`, you'll probably want to verify it.

Running `decoderawtransaction` will show that the raw transaction is
now laid out correctly, using one or more of your UTXOs and sending
excess funds back to a change address:

```sh
bitcoin-cli -named decoderawtransaction hexstring=$rawtxhex3

| {
|   "txid": "031c88b5ef382f20773381543f00a7d34694d7372382d5d4f6029d9bf86693f5",
|   "hash": "031c88b5ef382f20773381543f00a7d34694d7372382d5d4f6029d9bf86693f5",
|   "version": 2,
|   "size": 113,
|   "vsize": 113,
|   "weight": 452,
|   "locktime": 0,
|   "vin": [
|     {
|       "txid": "80619fca79ce08e6c5405a8a241bb479be6d3d18256df859afb7b92c5f472465",
|       "vout": 0,
|       "scriptSig": {
|         "asm": "",
|         "hex": ""
|       },
|       "sequence": 4294967293
|     }
|   ],
|   "vout": [
|     {
|       "value": 0.01798590,
|       "n": 0,
|       "scriptPubKey": {
|         "asm": "0 fe1f80781617de742b087aa63df65179fe3c2512",
|         "desc": "addr(tb1qlc0cq7qkzl08g2cg02nrmaj308lrcfgjzec9lx)#7kazgeeu",
|         "hex": "0014fe1f80781617de742b087aa63df65179fe3c2512",
|         "address": "tb1qlc0cq7qkzl08g2cg02nrmaj308lrcfgjzec9lx",
|         "type": "witness_v0_keyhash"
|       }
|     },
|     {
|       "value": 0.00200000,
|       "n": 1,
|       "scriptPubKey": {
|         "asm": "0 447fde1e37d97255b5821d2dee816e8f18f6bac9",
|         "desc": "addr(tb1qg3lau83hm9e9tdvzr5k7aqtw3uv0dwkfct4xdn)#zau8pzqg",
|         "hex": "0014447fde1e37d97255b5821d2dee816e8f18f6bac9",
|         "address": "tb1qg3lau83hm9e9tdvzr5k7aqtw3uv0dwkfct4xdn",
|         "type": "witness_v0_keyhash"
|       }
|     }
|   ]
| }
```

Though we saw the fee in the `fundrawtransaction` output, it's not
visible here. However, you can verify it with the `txfee-calc.sh` JQ
script created in the [JQ
Interlude](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/05_2a_Interlude_Using_JQ.md):

```sh
~/txfee-calc.sh $rawtxhex3

| .00001400
```

Finally, you can use `getaddressinfo` to see that the generated change address (the one receiving the surplus funds of 0.01798590) really belongs to you:

```sh
bitcoin-cli -named getaddressinfo address=tb1qlc0cq7qkzl08g2cg02nrmaj308lrcfgjzec9lx

| {
|   "address": "tb1qlc0cq7qkzl08g2cg02nrmaj308lrcfgjzec9lx",
|   "scriptPubKey": "0014fe1f80781617de742b087aa63df65179fe3c2512",
|   "ismine": true,
|   "solvable": true,
|   "desc": "wpkh([b8309bae/84h/1h/0h/1/8]0305e3de867c6d9927dc671845e46ea29149ec23dc91d367e4cbdcde4457b8566b)#hwpl2vrq",
|   "parent_desc": "wpkh([b8309bae/84h/1h/0h]tpubDDpSvPDUjstxFUEWzHkaL4qykf8vjNCspm8SZ26Z1wgPFbd63AdYrn4bDpEGPT1giJ6gcLW8Xou8fnhi35DJrUza9ikgu5dg2mDkd8jQpA6/1/*)#eeegkjmk",
|   "iswatchonly": false,
|   "isscript": false,
|   "iswitness": true,
|   "witness_version": 0,
|   "witness_program": "fe1f80781617de742b087aa63df65179fe3c2512",
|   "pubkey": "0305e3de867c6d9927dc671845e46ea29149ec23dc91d367e4cbdcde4457b8566b",
|   "ischange": true,
|   "timestamp": 1772127861,
|   "hdkeypath": "m/84h/1h/0h/1/8",
|   "hdseedid": "0000000000000000000000000000000000000000",
|   "hdmasterfingerprint": "b8309bae",
|   "labels": [
|   ]
| }
```
Note the `ismine` results.

## Send Your Funded Transaction

At this point you can sign and send the transaction as usual.

```sh
signedtx3=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex3 | jq -r '.hex')
bitcoin-cli -named sendrawtransaction hexstring=$signedtx3

| 8b9dd66c999966462a3d88d6ac9405d09e2aa409c0aa830bdd08dbcbd34a36fa
```
In several minutes, you'll have your change back:

```sh
bitcoin-cli listunspent

| [
|   {
|     "txid": "031c88b5ef382f20773381543f00a7d34694d7372382d5d4f6029d9bf86693f5",
|     "vout": 0,
|     "address": "tb1qlc0cq7qkzl08g2cg02nrmaj308lrcfgjzec9lx",
|     "scriptPubKey": "0014fe1f80781617de742b087aa63df65179fe3c2512",
|     "amount": 0.01798590,
|     "confirmations": 1,
|     "spendable": true,
|     "solvable": true,
|     "desc": "wpkh([b8309bae/84h/1h/0h/1/8]0305e3de867c6d9927dc671845e46ea29149ec23dc91d367e4cbdcde4457b8566b)#hwpl2vrq",
|     "parent_descs": [
|       "wpkh([b8309bae/84h/1h/0h]tpubDDpSvPDUjstxFUEWzHkaL4qykf8vjNCspm8SZ26Z1wgPFbd63AdYrn4bDpEGPT1giJ6gcLW8Xou8fnhi35DJrUza9ikgu5dg2mDkd8jQpA6/1/*)#eeegkjmk"
|     ],
|     "safe": true
|   }
| ]
```

## Freeze Your Coins

The `fundrawtransaction` makes it easy to fund raw transactions by automatically selecting your UTXOs, saving you from having to search them up. But sometimes there might be UTXOs that you _don't_ want to use, most frequently because they might correlate activities that you don't want correlated (e.g., if you do programming for politicians under a pseudonym, you might not want to spend money earned in that way on a family vacation, because it could break your pseudonymity). 

To prevent this sort of correlation from accidentally happening, you can "lock" a sensitive UTXO so that it's never selected for automatic fund creation, such as with `fundrawtransaction`. 

First, you need to identify the problematic UTXO:

```sh
txid="031c88b5ef382f20773381543f00a7d34694d7372382d5d4f6029d9bf86693f5"
vout="0"
```
Then you need to run the `lockunspent` command with an `unlock=false` (e.g., "lock") argument and a JSON array of orbjects that each contain a UTXO.

```sh
bitcoin-cli -named lockunspent unlock=false transactions='''[ { "txid": "'$txid'", "vout": '$vout' } ]'''

| true
```
This will maintain the lock in memory. If you want it instead written to the wallet database, chose:

```sh
bitcoin-cli -named lockunspent unlock=false transactions='''[ { "txid": "'$txid'", "vout": '$vout' } ]''' persistent=true

| true
```
You can see the UTXO is locked with `listlockunspent`:

```sh
bitcoin-cli listlockunspent

| [
|   {
|     "txid": "031c88b5ef382f20773381543f00a7d34694d7372382d5d4f6029d9bf86693f5",
|     "vout": 0
|   }
| ]
```
It's now also missing from your `listunspent` list:

```sh
bitcoin-cli listunspent

| [
|   {
|     "txid": "8a0d9ab73d81a1ce043d1ede0e737136e6d3352d3e0bc9590f9e8bbd91036dc2",
|     "vout": 0,
|     "address": "tb1q7xkh94r24zw4uc3wg2amkagxfm4an5gpwrgsnl",
|     "label": "",
|     "scriptPubKey": "0014f1ad72d46aa89d5e622e42bbbb75064eebd9d101",
|     "amount": 0.00099000,
|     "confirmations": 15,
|     "spendable": true,
|     "solvable": true,
|     "desc": "wpkh([b8309bae/84h/1h/0h/0/5]027650c44c6f71d50a173f3f18858490cf26d2661a1d9929deba9aca57c76d1e5b)#7ss7jfnn",
|     "parent_descs": [
|       "wpkh([b8309bae/84h/1h/0h]tpubDDpSvPDUjstxFUEWzHkaL4qykf8vjNCspm8SZ26Z1wgPFbd63AdYrn4bDpEGPT1giJ6gcLW8Xou8fnhi35DJrUza9ikgu5dg2mDkd8jQpA6/0/*)#gduft8tw"
|     ],
|     "safe": true
|   },
|   {
|     "txid": "d4ce7ab7db122373567addca4aa5a2866fc617b720db8147748b4108834144ff",
|     "vout": 1,
|     "address": "tb1q79st28w04n57z6ht8zasfrdy9804qe9u5l4ahf",
|     "scriptPubKey": "0014f160b51dcface9e16aeb38bb048da429df5064bc",
|     "amount": 0.00050000,
|     "confirmations": 3,
|     "spendable": true,
|     "solvable": true,
|     "desc": "wpkh([b8309bae/84h/1h/0h/1/6]0202c9ea711bd943d8c34a9a5ffb848a58609deacb432de4b097c5dfc32dc5595c)#4zwtrgdu",
|     "parent_descs": [
|       "wpkh([b8309bae/84h/1h/0h]tpubDDpSvPDUjstxFUEWzHkaL4qykf8vjNCspm8SZ26Z1wgPFbd63AdYrn4bDpEGPT1giJ6gcLW8Xou8fnhi35DJrUza9ikgu5dg2mDkd8jQpA6/1/*)#eeegkjmk"
|     ],
|     "safe": true
|   }
| ]
```
Most importantly if you try try to fund an unfunded transaction, the locked UTXO cannot be spent:

```sh
nolock=$(bitcoin-cli -named createrawtransaction inputs='''[]''' outputs='''{ "'$recipient'": 0.01 }''')
```
In this case, the wallet has sufficient funds, but only if the locked UTXO is used, so `fundrawtransaction` refuses:

```sh
bitcoin-cli -named fundrawtransaction hexstring=$nolock

| error code: -4
| error message:
| Insufficient funds
```
The lock can be removed just by changing the `unlock` variable to `true`. (`persistent` no longer matters.)

```sh
bitcoin-cli -named lockunspent unlock=true transactions='''[ { "txid": "'$txid'", "vout": '$vout' } ]'''

| true
```
And now that raw transaction will fund correctly:

```
bitcoin-cli -named fundrawtransaction hexstring=$nolock

| {
|   "hex": "0200000001f59366f89b9d02f6d4d5822337d79446d3a7003f54813377202f38efb5881c030000000000fdffffff02fc290c00000000001600142d5717c0d7b3a2d68d56be8d9231ed7a65d4d3f840420f0000000000160014447fde1e37d97255b5821d2dee816e8f18f6bac900000000",
|   "fee": 0.00001410,
|   "changepos": 0
| }
```

## Summary: Sending Coins with Automated Raw Transactions

If you must send funds with raw transactions then `fundrawtransaction` gives you a nice alternative where fees, inputs, and outputs are calculated for you, so you don't accidentally lose a bunch of money.

> 🔥 ***What is the power of sending coins with automated raw transactions?***

> _The advantages._ It provides a nice balance. If you're sending funds by hand and `sendtoaddress` doesn't offer enough control for whatever reason, you can get some of the advantages of raw transactions without the dangers. This methodology should be used whenever possible if you're sending raw transactions by hand.

> _The disadvantages._ It's a hodge-podge. Though there are a few additional options for the `fundrawtransaction` command that weren't mentioned here, but even with the ability to lock UTXOs, your control is still limited. You'd probably never want to use this method if you were writing a program where the whole goal is to know exactly what's going on.

## What's Next?

Complete your "Sending of Bitcoin Transactions" with [§5.6: Sending Coins to Other Addresses](05_6_Sending_Coins_to_Other_Addresses.md).

