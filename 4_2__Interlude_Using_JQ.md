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
## Use JQ to Capture a JSON Object Key-Values

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
## Use JQ to Capture a JSON Array Key-Value

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
You can also use `jq '.[]'` to access all elements of an array. You might wonder why that's useful. It's because you can use that as an input _within JQ_ and then access individual key-values from within the individual elements of the array. To do this you (1) use a pipe within the JQ arguments; and then (2) list the key-value you want to capture after the pipe.

This offers an easy way to capture _all_ the like information from an array. For example, this would list all of our unspent funds:
```
$ bitcoin-cli listunspent | jq -r '.[] | .amount'
0.76
3.9
1.95
```

### Use JQ for Basic Math

At this point, you can start using JQ output for simple math. For example, adding up the amount of those unspent transactions with a simple `awk` script would give you the equivalent of `getbalance`:
```rmatio
$ bitcoin-cli listunspent | jq -r '.[] | .amount' | awk '{s+=$1} END {print s}'
6.61
$ bitcoin-cli getbalance
6.61000000
```

## Use JQ to Display Multiple Values

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
If you want to be really fancy, you can put those elements in `{}`s to build a new JSON array, and name each of the individual elements. The following example shows this same look at `listunspent`, but with each object rebuilt as a new object, with all of the new key-values named with their old keys:
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

## Use JQ to Calculate Transaction Fees

