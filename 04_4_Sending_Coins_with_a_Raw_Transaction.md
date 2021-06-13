# 4.4: Sending Coins with Raw Transactions

As noted at the start of this chapter, the `bitcoin-cli` interface offers three major ways to send coins. [§4.1](04_1_Sending_Coins_The_Easy_Way.md) talked about sending them the first way, using the `sendtoaddress` command. Since then, we've been building details on how to send coins a second way, with raw transactions. [§4.2](04_2_Creating_a_Raw_Transaction.md) taught how to create a raw transaction, an [Interlude](04_2__Interlude_Using_JQ.md) explained JQ, and [§4.3](04_3_Creating_a_Raw_Transaction_with_Named_Arguments.md) demonstrated named arguments.

We can now put those together and actually send funds using a raw transaction.

## Create a Change Address

Our sample raw transaction in section §4.2 was very simplistic: we sent the entirety of a UTXO to a new address. More frequently, you'll want to send someone an amount of money that doesn't match a UTXO. But, you'll recall that the excess money from a UTXO that's not sent to your recipient just becomes a transaction fee. So, how do you send someone just part of a UTXO, while keeping the rest for yourself?

The solution is to _send_ the rest of the funds to a second address, a change address that you've created in your wallet specifically to receive them:
```
$ changeaddress=$(bitcoin-cli getrawchangeaddress legacy)
$ echo $changeaddress
mk9ry5VVy8mrA8SygxSQQUDNSSXyGFot6h
```
Note that this uses a new function: `getrawchangeaddress`. It's largely the same as `getnewaddress` but is optimized for use as a change address in a raw transaction, so it doesn't do things like make entries in your address book. We again selected the `legacy` address, instead of going with the default of `bech32`, simply for consistency. This is a situation where it would have been entirely safe to generate a default Bech32 address, just by using `bitcoin-cli getrawchangeaddress`, because it would being sent and received by you on your Bitcoin Core node which fully supports this. But, hobgoblins; we'll shift this over to Bech32 as well in [§4.6](04_6_Creating_a_Segwit_Transaction.md).

You now have an additional address inside your wallet, so that you can receive change from a UTXO! In order to use it, you'll need to create a raw transaction with two outputs.

## Pick Sufficient UTXOs

Our sample raw transaction was simple in another way: it assumed that there was enough money in a single UTXO to cover the transaction. Often this will be the case, but sometimes you'll want to create transactions that spends more money than you have in a single UTXO. To do so, you must create a raw transaction with two (or more) inputs.

## Write a Real Raw Transaction

To summarize: creating a real raw transaction to send coins will sometimes require multiple inputs and will almost always require multiple outputs, one of which is a change address. We'll be creating that sort of more realistic transaction here, in a new example that shows a real-life example of sending funds via Bitcoin's second methodology, raw transactions.

We're going to use our 0th and 2nd UTXOs:
```
$ bitcoin-cli listunspent
[
[
  {
    "txid": "0619fecf6b2668fab1308fbd7b291ac210932602a6ac6b8cc11c7ae22c43701e",
    "vout": 1,
    "address": "mwJL7cRiW2bUnY81r1thSu3D4jtMmwyU6d",
    "label": "",
    "scriptPubKey": "76a914ad1ed1c5971b2308f89c1362d4705d020a40e8e788ac",
    "amount": 0.00899999,
    "confirmations": 1,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/4']03eae28c93035f95a620dd96e1822f2a96e0357263fa1f87606a5254d5b9e6698f)#wwnfx2sp",
    "safe": true
  },
  {
    "txid": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
    "vout": 0,
    "address": "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
    "label": "",
    "scriptPubKey": "76a9142d573900aa357a38afd741fbf24b075d263ea6e088ac",
    "amount": 0.00022000,
    "confirmations": 15,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/3']0278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132)#nhjc3f8y",
    "safe": true
  },  
  {
    "txid": "0df23a9dba49e822bbc558f15516f33021a64a5c2e48962cec541e0bcc79854d",
    "vout": 0,
    "address": "mwJL7cRiW2bUnY81r1thSu3D4jtMmwyU6d",
    "label": "",
    "scriptPubKey": "76a914ad1ed1c5971b2308f89c1362d4705d020a40e8e788ac",
    "amount": 0.00100000,
    "confirmations": 1,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/4']03eae28c93035f95a620dd96e1822f2a96e0357263fa1f87606a5254d5b9e6698f)#wwnfx2sp",
    "safe": true
   }
]

```
In our example, we're going to send .009 BTC, which is (barely) larger than either of our UTXOs. This requires combining them, then using our change address to retrieve the unspent funds.

### Set Up Your Variables

We already have `$changeaddress` and `$recipient` variables from previous examples:
```
$ echo $changeaddress
mk9ry5VVy8mrA8SygxSQQUDNSSXyGFot6h
$ echo $recipient
n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi
```
We also need to record the txid and vout for each of our two UTXOs. Having identified the UTXOs that we want to spend, we can use our JQ techniques to make sure accessing them is error free:
```
$ utxo_txid_1=$(bitcoin-cli listunspent | jq -r '.[0] | .txid')
$ utxo_vout_1=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
$ utxo_txid_2=$(bitcoin-cli listunspent | jq -r '.[2] | .txid')
$ utxo_vout_2=$(bitcoin-cli listunspent | jq -r '.[2] | .vout')
```

### Write the Transaction

Writing the actual raw transaction is surprisingly simple. All you need to do is include an additional, comma-separated JSON object in the JSON array of inputs and an additional, comma-separated key-value pair in the JSON object of outputs.

Here's the example. Note the multiple inputs after the `inputs` arg and the multiple outputs after the `outputs` arg.
```
$ rawtxhex2=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid_1'", "vout": '$utxo_vout_1' }, { "txid": "'$utxo_txid_2'", "vout": '$utxo_vout_2' } ]''' outputs='''{ "'$recipient'": 0.009, "'$changeaddress'": 0.0009 }''')
```
We were _very_ careful figuring out our money math. These two UTXOs contain 0.00999999 BTC. After sending 0.009 BTC, we'll have .00099999 BTC left. We chose .00009999 BTC the transaction fee. To accommodate that fee, we set our change to .0009 BTC. If we'd messed up our math and instead set our change to .00009 BTC, that additional BTC would be lost to the miners! If we'd forgot to make change at all, then the whole excess would have disappeared. So, again, _be careful_. 

Fortunately, we can triple-check with the `btctxfee` alias from the JQ Interlude:
```
$ ./txfee-calc.sh $rawtxhex2
.00009999
```

### Finish It Up

You can now sign, seal, and deliver your transaction, and it's yours (and the faucet's):
```
$ signedtx2=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex2 | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx2
e7071092dee0b2ae584bf6c1ee3c22164304e3a17feea7a32c22db5603cd6a0d
```

### Wait

As usual, your money will be in flux for a while: the change will be unavailable until the transaction actually gets confirmed and a new UTXO is given to you.

But, in 10 minutes or less (probably), you'll have your remaining money back and fully spendable again. For now, we're still waiting:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
    "vout": 0,
    "address": "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
    "label": "",
    "scriptPubKey": "76a9142d573900aa357a38afd741fbf24b075d263ea6e088ac",
    "amount": 0.00022000,
    "confirmations": 15,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/3']0278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132)#nhjc3f8y",
    "safe": true
  }
]
```
And the change will eventuall arrive:
```
[
  {
    "txid": "e7071092dee0b2ae584bf6c1ee3c22164304e3a17feea7a32c22db5603cd6a0d",
    "vout": 1,
    "address": "mk9ry5VVy8mrA8SygxSQQUDNSSXyGFot6h",
    "scriptPubKey": "76a91432db726320e4ad170c9c1ee83cd4d8a243c3435988ac",
    "amount": 0.00090000,
    "confirmations": 1,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/1'/2']02881697d252d8bf181d08c58de1f02aec088cd2d468fc5fd888c6e39909f7fabf)#p6k7dptk",
    "safe": true
  },
  {
    "txid": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
    "vout": 0,
    "address": "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
    "label": "",
    "scriptPubKey": "76a9142d573900aa357a38afd741fbf24b075d263ea6e088ac",
    "amount": 0.00022000,
    "confirmations": 16,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([d6043800/0'/0'/3']0278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132)#nhjc3f8y",
    "safe": true
  }
]
```

This also might be a good time to revisit a blockchain explorer, so that you can see more intuitively how the inputs, outputs, and transaction fee are all laid out: [e7071092dee0b2ae584bf6c1ee3c22164304e3a17feea7a32c22db5603cd6a0d](https://live.blockcypher.com/btc-testnet/tx/e7071092dee0b2ae584bf6c1ee3c22164304e3a17feea7a32c22db5603cd6a0d/).

## Summary: Sending Coins with Raw Transactions

To send coins with raw transactions, you need to create a raw transaction with one or more inputs (to have sufficient funds) and one or more outputs (to retrieve change). Then, you can follow your normal procedure of using `createrawtransaction` with named arguments and JQ, as laid out in previous sections.

> :fire: ***What is the power of sending coins with raw transactions?***

> _The advantages._ It gives you the best control. If your goal is to write a more intricate Bitcoin script or program, you'll probably use raw transactions so that you know exactly what's going on. This is also the _safest_ situation to use raw transactions, because you can programmatically ensure that you don't make mistakes.

> _The disadvantages._ It's easy to lose money. There are no warnings, no safeguards, and no programmatic backstops unless you write them. It's also arcane. The formatting is obnoxious, even using the easy-to-use `bitcoin-cli` interface, and you have to do a lot of lookup and calculation by hand.

## What's Next?

See another alternative way to input commands with [Interlude: Using Curl](04_4__Interlude_Using_Curl.md).

Or, you prefer to skip what's frankly a digression, learn one more way to "Send Bitcoin Transactions" with [§4.5 Sending Coins with Automated Raw Transactions](04_5_Sending_Coins_with_Automated_Raw_Transactions.md).

