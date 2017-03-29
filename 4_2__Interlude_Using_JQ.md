# Interlude: Using JQ

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Creating a raw transaction revealed how more complex bitcoin-cli results can't be easily saved into command-line variables. The answer is JQ, which allows you to filter out individual elements from more complex JSON data.

## Install JQ

JQ is available from a [Github repository](https://stedolan.github.io/jq/). Just download for Linux, OS X, or Windows, as appropriate.

Once you've downloaded the binary, you can install it on your system:
```
$ mv jq-linux64 jq
$ sudo /usr/bin/install -m 0755 -o root -g root -t /usr/local/bin jq
```
## Use JQ to Capture a JSON Object Key-Value by Key

**Usage Example:** _Capture the hex from a signed raw transaction._

In the previous section, the use of `signrawtransaction` offered the first example of not being able to easily capture data into variables due to the use of JSON:
```
$ bitcoin-cli signrawtransaction $rawtxhex
{
  "hex": "0200000001735dfa1584b930a78ad2c1d6db72dd2a80ae5e5d552ad97e19f1d50d41fdd6d8000000006a47304402202210ce4b2a037da02622c380278cd79fec4e0e016e66f3eb894a2dcbb9ee998f02202cac167e6abdbbf08af139fb7c6b86e9d2e58e5516cd566ae2d54953ead9923b012102111bb978a3c93a00038ae344a1a017d7fee8a9be9d0558b5793ce6f440704a96ffffffff01b0e78604000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000",
  "complete": true
}
```
JQ can be easily used to capture information from a JSON Object of this sort. 

To start with, always run `jq` at the backend of a pipe, and always use the standard invocation of `jq -r .`. The `-r` tells JQ to produce raw output, which will work for command-line variables and future reuse, while the `.` tells jq to ouput. 

To capture a specific key-value from a JSON object, you just list the key after the `.`:
```
$ bitcoin-cli signrawtransaction $rawtxhex | jq -r .hex
0200000001735dfa1584b930a78ad2c1d6db72dd2a80ae5e5d552ad97e19f1d50d41fdd6d8000000006a47304402202210ce4b2a037da02622c380278cd79fec4e0e016e66f3eb894a2dcbb9ee998f02202cac167e6abdbbf08af139fb7c6b86e9d2e58e5516cd566ae2d54953ead9923b012102111bb978a3c93a00038ae344a1a017d7fee8a9be9d0558b5793ce6f440704a96ffffffff01b0e78604000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000
```
With that tool in hand, you can capture information from JSON objects to command-line variables, as usual:
```
$ signedtx=$(bitcoin-cli signrawtransaction $rawtxhex | jq -r .hex)
$ echo $signedtx
0200000001735dfa1584b930a78ad2c1d6db72dd2a80ae5e5d552ad97e19f1d50d41fdd6d8000000006a47304402202210ce4b2a037da02622c380278cd79fec4e0e016e66f3eb894a2dcbb9ee998f02202cac167e6abdbbf08af139fb7c6b86e9d2e58e5516cd566ae2d54953ead9923b012102111bb978a3c93a00038ae344a1a017d7fee8a9be9d0558b5793ce6f440704a96ffffffff01b0e78604000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000
$ bitcoin-cli sendrawtransaction $signedtx
3f9ccb6e16663e66dc119de1866610cc4f7a83079bfec2abf0598ed3adf10a78
```
## Use JQ to Capture a JSON Array Key-Value by Key

**Usage Example:** _Capture the txid and vout for a selected unspent transaction._

So what happens when you instead use `jq` to parse a JSON Array? The `listunspent` command offers a great example, because it'll usually contain a number of different transactions.

When working with an array, the first thing you need to tell JQ is which index to access. For example, `jq '.[0]'` says to access the first element of an array. (Note that we're now protecting our jq arguments with single quotes, because of their increased complexity.)
```
$ bitcoin-cli listunspent | jq -r '.[0]'
{
  "txid": "2b5f5798359e0e23e02764588166f222d4ce056419dec83c743b72aad171d708",
  "vout": 1,
  "address": "mjtEqr4Fffd1XtpAkKoDkMBP54mMXJeQ3j",
  "account": "",
  "scriptPubKey": "76a9142fe70d51e886b9ef73b76c1743c5a2bb2894db8588ac",
  "amount": 0.76,
  "confirmations": 6578,
  "spendable": true,
  "solvable": true
}
```
You can then capture an individual key-value from that array by (1) using a pipe within the JQ arguments; and then (2) listing the key-value you want to capture after the pipe.
```
~$ bitcoin-cli listunspent | jq -r '.[0] | .txid'
2b5f5798359e0e23e02764588166f222d4ce056419dec83c743b72aad171d708
```
This method can be used to fill in variables for a UTXO that you want to use:
```
$ newtxid=$(bitcoin-cli listunspent | jq -r '.[0] | .txid')
$ newvout=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
$ echo $newtxid
2b5f5798359e0e23e02764588166f222d4ce056419dec83c743b72aad171d708
$ echo $newvout
1
```
Voila! We could now create a new raw transaction using our 0th UTXO as an input, without having to type in any of the UTXO info by hand!

## Use JQ to Capture Multiple JSON Array Key-Values by Key

**Usage Example:** _List the value of all unspent funds._

You can alternatively use `jq '.[]'` to access all elements of a JSON Array. You might wonder why that's useful. It's because you can then capture _all_ of a specific value, from _all_ of the array indices, using the same methodology you just used to capture a simple element. For example, this would list all of our unspent funds:
```
$ bitcoin-cli listunspent | jq -r '.[] | .amount'
0.76
3.9
1.95
```

## Use JQ for Simple Calculations by Key

**Usage Example:** _Sum the value of all unspent funds._

At this point, you can start using JQ output for simple math. For example, adding up the amount of those unspent transactions with a simple `awk` script would give you the equivalent of `getbalance`:
```
$ bitcoin-cli listunspent | jq -r '.[] | .amount' | awk '{s+=$1} END {print s}'
6.61
$ bitcoin-cli getbalance
6.61000000
```

## Use JQ to Display Multiple Values by Multiple Keys

**Usage Example:** _List critical information for all unspent funds._

JQ is great for capturing individual elements from JSON objects and arrays, and placing those elements into variables. That will be its prime use in future sections. However, it can also be used to cut down huge amounts of information into reasonable amounts of information.
```
$ bitcoin-cli listunspent | jq -r '.[] | .txid, .vout, .amount'
2b5f5798359e0e23e02764588166f222d4ce056419dec83c743b72aad171d708
1
0.76
ec0598918f6f5476cb90365651e8a2724ef26f949290bbf196f41ed96092a52f
0
3.9
3470e5fe08633583d136b9cd49bb1a224c9d9313a0b4584fd3b7438dbdf34dbd
0
1.95
```
That gets you the info you need to decide what UTXOs to spend, but it's not very pretty. If you want to be really fancy, you can put those elements in `{}`s to build a new JSON object and then name each of the individual elements in the object. The result should be much more intuitive and less prone to error (though obviously, less useful for dumping info straight into variables). The following example shows this same look at `listunspent`, but with each object rebuilt as a new JSON Object, with all of the new key-values named with their old keys:
```
$ bitcoin-cli listunspent | jq -r '.[] | { txid: .txid, vout: .vout, amount: .amount }'
{
  "txid": "2b5f5798359e0e23e02764588166f222d4ce056419dec83c743b72aad171d708",
  "vout": 1,
  "amount": 0.76
}
{
  "txid": "ec0598918f6f5476cb90365651e8a2724ef26f949290bbf196f41ed96092a52f",
  "vout": 0,
  "amount": 3.9
}
{
  "txid": "3470e5fe08633583d136b9cd49bb1a224c9d9313a0b4584fd3b7438dbdf34dbd",
  "vout": 0,
  "amount": 1.95
}
```
You can of course rename your new keys as you see fit:
```
~$ bitcoin-cli listunspent | jq -r '.[] | { tx: .txid, output: .vout, bitcoins: .amount }'
{
  "tx": "2b5f5798359e0e23e02764588166f222d4ce056419dec83c743b72aad171d708",
  "output": 1,
  "bitcoins": 0.76
}
{
  "tx": "ec0598918f6f5476cb90365651e8a2724ef26f949290bbf196f41ed96092a52f",
  "output": 0,
  "bitcoins": 3.9
}
{
  "tx": "3470e5fe08633583d136b9cd49bb1a224c9d9313a0b4584fd3b7438dbdf34dbd",
  "output": 0,
  "bitcoins": 1.95
}
```
## Use JQ to Capture a JSON Object Key-Value by Value

**Usage Example:** _Automatically look up a UTXO being used in a transaction._

The JQ lookups so far have been fairly simple: you use a key to lookup one or more key-values in a JSON Object or Array. But what if you instead want to lookup a key-value in an object ... by another key-value? This sort of indirect lookup has real applicability when you're working with transactions built on existing UTXOs. For example, what if you wanted to figure out the value of a UTXO we're using, something that we know is vitally important in raw transactions?

To start with, we have a raw transaction:
```
$ bitcoin-cli decoderawtransaction $rawtxhex
{
  "txid": "f445f121085d98635f7302e641f815d1ca4ce70f0e1b03f144ad1661dc5e10e7",
  "hash": "f445f121085d98635f7302e641f815d1ca4ce70f0e1b03f144ad1661dc5e10e7",
  "size": 85,
  "vsize": 85,
  "version": 2,
  "locktime": 0,
  "vin": [
    {
      "txid": "2b5f5798359e0e23e02764588166f222d4ce056419dec83c743b72aad171d708",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 0.75950000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 e7c1345fc8f87c68170b3aa798a956c2fe6a9eff OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi"
        ]
      }
    }
  ]
}
```
To retrieve the 0th (and in this case only) txid, we need to `jq` to the .vin key-value, then to its 0th array, then to that array's .txid key-value. Easy:
```
$ usedtxid=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[0] | .txid')
$ echo $usedtxid
2b5f5798359e0e23e02764588166f222d4ce056419dec83c743b72aad171d708
```
A magic invocation of `select` and `contains`, used after a typical pipe through all the JSON objects, can then be used to look up the transaction in question:
```
$ bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'$usedtxid'"))'
{
  "txid": "2b5f5798359e0e23e02764588166f222d4ce056419dec83c743b72aad171d708",
  "vout": 1,
  "address": "mjtEqr4Fffd1XtpAkKoDkMBP54mMXJeQ3j",
  "account": "",
  "scriptPubKey": "76a9142fe70d51e886b9ef73b76c1743c5a2bb2894db8588ac",
  "amount": 0.76,
  "confirmations": 6617,
  "spendable": true,
  "solvable": true
}
```
Which leaves us with a wonderful two-line script to determine how much money was in the first UTXO input for a transaction:
```
$ usedtxid=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[0] | .txid')
$ bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'$usedtxid'")) | .amount'
0.76
```
For more JSON magic (and if any of this isn't clear), please read the [JSON Manual](https://stedolan.github.io/jq/manual/) and the [JSON Cookbook](https://github.com/stedolan/jq/wiki/Cookbook).

## Use JQ for Complex Calculations

**Usage Example:** _Calculate the fee for a simple one-input, one-output transaction._

With this in hand, you can now use a few lines of code to see the transaction fee for the simple, one-input, one-output example raw transaction that we wrote in the previous section:
```
$ usedtxid=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[0] | .txid')
$ btcin=$(bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'$usedtxid'")) | .amount')
$ btcout=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vout  [0] | .value')
$ echo "$btcin-$btcout"| /usr/bin/bc
.0005
```

### Use JQ to Calculate Transaction Fees for Real

**Usage Example:** _Calculate the fee for a real transaction._

However, as we've noted, a real transaction may have more than one input and will almost always have more than one output. We'll construct one of those in a future section, but for now here's a more robust bash script that uses JQ to calculate the transaction fee for a transaction with multiple vins and vouts.

> **WARNING:** This script has not been robustly checked. If you are going to use it to verify real transaction fees you should only do it as a triple-check after you've already done all the math yourself.


