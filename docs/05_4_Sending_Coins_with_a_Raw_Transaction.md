# 5.4: Sending Coins with Raw Transactions

As noted at the start of this chapter, the `bitcoin-cli` interface
offers three major ways to send
coins. [§5.1](05_1_Sending_Coins_The_Easy_Way.md) talked about sending
them the first way, using the `sendtoaddress` command. Since then,
we've been building details on how to send coins a second way, with
raw transactions. [§5.2](05_2_Creating_a_Raw_Transaction.md) taught
how to create a raw transaction, an
[Interlude](05_2a_Interlude_Using_JQ.md) explained JQ, and
[§5.3](05_3_Creating_a_Raw_Transaction_with_Named_Arguments.md)
demonstrated named arguments.

We can now put those together and actually send funds using a raw transaction (or more precisely, we can send funds in a way that actually represents a real transaction, rather than sending complete UTXOs, as was did in the previous two sections).

## Create a Change Address

Our sample raw transactions in §5.2 and §5.3 were very simplistic: we sent the entirety of a UTXO to a new address. In real-life, you'll want to send someone an amount of money that doesn't match a UTXO. But, you'll recall that the excess money from a UTXO that's not sent to your recipient just becomes a transaction fee. So, how do you send someone just part of a UTXO, while keeping the rest for yourself?

The solution is to _send_ the rest of the funds to a second address, a change address that you've created in your wallet specifically to receive them:

```sh
changeaddress=$(bitcoin-cli getrawchangeaddress)
echo $changeaddress

| tb1qjrs9jr572nfyg5un8f2rzjdngml0mvskxlyd09
```
Note that this uses a new function: `getrawchangeaddress`. It's largely the same as `getnewaddress` but is optimized for use as a change address in a raw transaction, so it doesn't do things like make entries in your address book. You now have an additional address inside your wallet, so that you can receive change from a UTXO! In order to use it, you'll need to create a raw transaction with two outputs.

## Pick Sufficient UTXOs

Our sample raw transactions of the previous two sections were simple in another way: they assumed that there was enough money in a single UTXO to cover the transaction. Often this will be the case, but sometimes you'll want to create transactions that spends more money than you have in a single UTXO. To do so, you must create a raw transaction with two (or more) inputs.

## Write a Real Raw Transaction

To summarize: creating a real raw transaction to send coins will sometimes require multiple inputs and will almost always require multiple outputs, one of which is a change address. We'll be creating that sort of more realistic transaction here, in a new example that shows a real-life example of sending funds via Bitcoin's second methodology, raw transactions.

We've got three UTXOs in our wallet. 

```sh
| [
|   {
|     "txid": "3c31ad2dec55a1c4b82e1c3615c71e68bc492dc81cbe2e9c12deba2064b0186f",
|     "vout": 49,
|     "address": "tb1qukwva953qreduzm3mgnjwh5ymxupa4jrrkj8ym",
|     "label": "",
|     "scriptPubKey": "0014e59cce969100f2de0b71da27275e84d9b81ed643",
|     "amount": 0.00300147,
|     "confirmations": 1,
|     "spendable": true,
|     "solvable": true,
|     "desc": "wpkh([b8309bae/84h/1h/0h/0/8]028f44c673718437fe9b22957e5551b3a186a2d7435b9f477f965ace21f852195f)#nzce84pw",
|     "parent_descs": [
|       "wpkh([b8309bae/84h/1h/0h]tpubDDpSvPDUjstxFUEWzHkaL4qykf8vjNCspm8SZ26Z1wgPFbd63AdYrn4bDpEGPT1giJ6gcLW8Xou8fnhi35DJrUza9ikgu5dg2mDkd8jQpA6/0/*)#gduft8tw"
|     ],
|     "safe": true
|   },
|   {
|     "txid": "8a0d9ab73d81a1ce043d1ede0e737136e6d3352d3e0bc9590f9e8bbd91036dc2",
|     "vout": 0,
|     "address": "tb1q7xkh94r24zw4uc3wg2amkagxfm4an5gpwrgsnl",
|     "label": "",
|     "scriptPubKey": "0014f1ad72d46aa89d5e622e42bbbb75064eebd9d101",
|     "amount": 0.00099000,
|     "confirmations": 7,
|     "spendable": true,
|     "solvable": true,
|     "desc": "wpkh([b8309bae/84h/1h/0h/0/5]027650c44c6f71d50a173f3f18858490cf26d2661a1d9929deba9aca57c76d1e5b)#7ss7jfnn",
|     "parent_descs": [
|       "wpkh([b8309bae/84h/1h/0h]tpubDDpSvPDUjstxFUEWzHkaL4qykf8vjNCspm8SZ26Z1wgPFbd63AdYrn4bDpEGPT1giJ6gcLW8Xou8fnhi35DJrUza9ikgu5dg2mDkd8jQpA6/0/*)#gduft8tw"
|     ],
|     "safe": true
|   },
|   {
|     "txid": "615813d052e48b9761afed8732b78ff67f3ee26390de9e1274418629bd80e45c",
|     "vout": 0,
|     "address": "tb1q7xkh94r24zw4uc3wg2amkagxfm4an5gpwrgsnl",
|     "label": "",
|     "scriptPubKey": "0014f1ad72d46aa89d5e622e42bbbb75064eebd9d101",
|     "amount": 0.01895000,
|     "confirmations": 2,
|     "spendable": true,
|     "solvable": true,
|     "desc": "wpkh([b8309bae/84h/1h/0h/0/5]027650c44c6f71d50a173f3f18858490cf26d2661a1d9929deba9aca57c76d1e5b)#7ss7jfnn",
|     "parent_descs": [
|       "wpkh([b8309bae/84h/1h/0h]tpubDDpSvPDUjstxFUEWzHkaL4qykf8vjNCspm8SZ26Z1wgPFbd63AdYrn4bDpEGPT1giJ6gcLW8Xou8fnhi35DJrUza9ikgu5dg2mDkd8jQpA6/0/*)#gduft8tw"
|     ],
|     "safe": true
|   }
| ]
```

We're going to be spending the 0th and the 2nd so that we can sum up .02 BTC.

### Set Up Your Variables

We already have `$changeaddress` and `$recipient` variables from previous examples:

```sh
echo $changeaddress

| tb1qjrs9jr572nfyg5un8f2rzjdngml0mvskxlyd09

recipient=$(bitcoin-cli getnewaddress)
echo $recipient

| tb1qdu69s5f9nw8x88lvsc8nt5d7gy9rgtnef0upfu
```
We also need to record the txid and vout for each of our two UTXOs. Having identified the UTXOs that we want to spend by hand, we can use our JQ techniques to make sure accessing them is error free:

```sh
utxo_txid_1=$(bitcoin-cli listunspent | jq -r '.[0] | .txid')
utxo_vout_1=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
utxo_txid_2=$(bitcoin-cli listunspent | jq -r '.[2] | .txid')
utxo_vout_2=$(bitcoin-cli listunspent | jq -r '.[2] | .vout')
```

### Write the Transaction

Writing the actual raw transaction is surprisingly simple. All you need to do is include an additional, comma-separated JSON object in the JSON array of inputs and an additional, comma-separated key-value pair in the JSON object of outputs.

Here's the example. Note the multiple inputs after the `inputs` arg and the multiple outputs after the `outputs` arg.

```sh
rawtxhex2=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid_1'", "vout": '$utxo_vout_1' }, { "txid": "'$utxo_txid_2'", "vout": '$utxo_vout_2' } ]''' outputs='''{ "'$recipient'": 0.02, "'$changeaddress'": 0.00194 }''')
```
We were _very_ careful figuring out our money math. These two UTXOs contain 0.02195147 BTC. After sending 0.02 BTC, we'll have 0.00195147 BTC left. We chose 0.00001147 BTC the transaction fee. To accommodate that fee, we set our change to .00194 BTC. If we'd messed up our math and instead set our change to .000195 BTC, that additional BTC would be lost to the miners! If we'd forgot to make change at all, then the whole excess would have disappeared. So, again, _be careful_. 

Fortunately, we can triple-check with the `btctxfee` alias from the JQ Interlude:

```
./txfee-calc.sh $rawtxhex2

| .00001150
```

### Finish It Up

You can now sign, seal, and deliver your transaction, and it's yours (and the faucet's):

```sh
signedtx2=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex2 | jq -r '.hex')
bitcoin-cli -named sendrawtransaction hexstring=$signedtx2

| 80619fca79ce08e6c5405a8a241bb479be6d3d18256df859afb7b92c5f472465
```

### Wait

As usual, your money will be in flux for a while: the change will be unavailable until the transaction actually gets confirmed and a new UTXO is given to you.

But, in 10 minutes or less (probably), you'll have your remaining money back and fully spendable again. For now, we're still waiting, and so can only see that one UTXO that we didn't spend:

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
|     "confirmations": 8,
|     "spendable": true,
|     "solvable": true,
|     "desc": "wpkh([b8309bae/84h/1h/0h/0/5]027650c44c6f71d50a173f3f18858490cf26d2661a1d9929deba9aca57c76d1e5b)#7ss7jfnn",
|     "parent_descs": [
|       "wpkh([b8309bae/84h/1h/0h]tpubDDpSvPDUjstxFUEWzHkaL4qykf8vjNCspm8SZ26Z1wgPFbd63AdYrn4bDpEGPT1giJ6gcLW8Xou8fnhi35DJrUza9ikgu5dg2mDkd8jQpA6/0/*)#gduft8tw"
|     ],
|     "safe": true
|   }
| ]
```

But eventually the change will arrive (and for that matter, the spent funds too, since we sent them to another of our addresses this time):

```sh
| [
|   {
|     "txid": "80619fca79ce08e6c5405a8a241bb479be6d3d18256df859afb7b92c5f472465",
|     "vout": 1,
|     "address": "tb1qjrs9jr572nfyg5un8f2rzjdngml0mvskxlyd09",
|     "scriptPubKey": "001490e0590e9e54d24453933a543149b346fefdb216",
|     "amount": 0.00194000,
|     "confirmations": 1,
|     "spendable": true,
|     "solvable": true,
|     "desc": "wpkh([b8309bae/84h/1h/0h/1/3]0347d61d5a0d09bf411b059a8ee97225e3430171b303a71ec40c5f414029cd84db)#gl740gxy",
|     "parent_descs": [
|       "wpkh([b8309bae/84h/1h/0h]tpubDDpSvPDUjstxFUEWzHkaL4qykf8vjNCspm8SZ26Z1wgPFbd63AdYrn4bDpEGPT1giJ6gcLW8Xou8fnhi35DJrUza9ikgu5dg2mDkd8jQpA6/1/*)#eeegkjmk"
|     ],
|     "safe": true
|   },
|   {
|     "txid": "80619fca79ce08e6c5405a8a241bb479be6d3d18256df859afb7b92c5f472465",
|     "vout": 0,
|     "address": "tb1qdu69s5f9nw8x88lvsc8nt5d7gy9rgtnef0upfu",
|     "label": "",
|     "scriptPubKey": "00146f345851259b8e639fec860f35d1be410a342e79",
|     "amount": 0.02000000,
|     "confirmations": 1,
|     "spendable": true,
|     "solvable": true,
|     "desc": "wpkh([b8309bae/84h/1h/0h/0/11]03d9cbd48a1238cc2c7371fd0c7169995106db6dec9fb287e4490c08cbd2b35cf1)#n3vc09ew",
|     "parent_descs": [
|       "wpkh([b8309bae/84h/1h/0h]tpubDDpSvPDUjstxFUEWzHkaL4qykf8vjNCspm8SZ26Z1wgPFbd63AdYrn4bDpEGPT1giJ6gcLW8Xou8fnhi35DJrUza9ikgu5dg2mDkd8jQpA6/0/*)#gduft8tw"
|     ],
|     "safe": true
|   },
|   {
|     "txid": "8a0d9ab73d81a1ce043d1ede0e737136e6d3352d3e0bc9590f9e8bbd91036dc2",
|     "vout": 0,
|     "address": "tb1q7xkh94r24zw4uc3wg2amkagxfm4an5gpwrgsnl",
|     "label": "",
|     "scriptPubKey": "0014f1ad72d46aa89d5e622e42bbbb75064eebd9d101",
|     "amount": 0.00099000,
|     "confirmations": 9,
|     "spendable": true,
|     "solvable": true,
|     "desc": "wpkh([b8309bae/84h/1h/0h/0/5]027650c44c6f71d50a173f3f18858490cf26d2661a1d9929deba9aca57c76d1e5b)#7ss7jfnn",
|     "parent_descs": [
|       "wpkh([b8309bae/84h/1h/0h]tpubDDpSvPDUjstxFUEWzHkaL4qykf8vjNCspm8SZ26Z1wgPFbd63AdYrn4bDpEGPT1giJ6gcLW8Xou8fnhi35DJrUza9ikgu5dg2mDkd8jQpA6/0/*)#gduft8tw"
|     ],
|     "safe": true
|   }
| ]
```

This also might be a good time to revisit a blockchain explorer, so that you can see more intuitively how the inputs, outputs, and transaction fee are all laid out: [80619fca79ce08e6c5405a8a241bb479be6d3d18256df859afb7b92c5f472465](https://mempool.space/signet/tx/80619fca79ce08e6c5405a8a241bb479be6d3d18256df859afb7b92c5f472465).

## Summary: Sending Coins with Raw Transactions

To send coins with raw transactions, you need to create a raw transaction with one or more inputs (to have sufficient funds) and one or more outputs (to retrieve change). Then, you can follow your normal procedure of using `createrawtransaction` with named arguments and JQ, as laid out in previous sections.

> 🔥 ***What is the power of sending coins with raw transactions?***

> _The advantages._ It gives you the best control. If your goal is to write a more intricate Bitcoin script or program, you'll probably use raw transactions so that you know exactly what's going on. That is also the _safest_ situation in which to use raw transactions, because you can programmatically ensure that you don't make mistakes.

> _The disadvantages._ It's easy to lose money. There are no warnings, no safeguards, and no programmatic backstops unless you write them. It's also arcane. The formatting is obnoxious, even using the easy-to-use `bitcoin-cli` interface, and you have to do a lot of lookup and calculation by hand.

## What's Next?

See another alternative way to input commands with [Interlude: Using
Curl](05_4a_Interlude_Using_Curl.md).

Or, if you prefer to skip what's frankly a digression, learn a final way to "Send Bitcoin Transactions" with [§5.5 Sending Coins with Automated Raw Transactions](05_5_Sending_Coins_with_Automated_Raw_Transactions.md).

