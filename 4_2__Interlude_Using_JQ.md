# Interlude: Using JQ

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Creating a raw transaction revealed how more complex bitcoin-cli results can't easily be saved into command-line variables. The answer is JQ, which allows you to filter out individual elements from more complex JSON data.

## Install JQ

JQ is available from a [Github repository](https://stedolan.github.io/jq/). Just download for Linux, OS X, or Windows, as appropriate.

Once you've downloaded the binary, you can install it on your system. If you're working on a Debian VPS as we suggest, your installation will look like this:
```
$ mv jq-linux64 jq
$ sudo /usr/bin/install -m 0755 -o root -g root -t /usr/local/bin jq
```
_What is JQ?_ The repository explains it best, saying "jq is like sed for JSON data - you can use it to slice and filter and map and transform structured data with the same ease that sed, awk, grep and friends let you play with text."

## Use JQ to Access a JSON Object Value by Key

**Usage Example:** _Capture the hex from a signed raw transaction._

In the previous section, the use of `signrawtransaction` offered an example of not being able to easily capture data into variables due to the use of JSON output:
```
$ bitcoin-cli signrawtransaction $rawtxhex
{
  "hex": "0200000001735dfa1584b930a78ad2c1d6db72dd2a80ae5e5d552ad97e19f1d50d41fdd6d8000000006a47304402202210ce4b2a037da02622c380278cd79fec4e0e016e66f3eb894a2dcbb9ee998f02202cac167e6abdbbf08af139fb7c6b86e9d2e58e5516cd566ae2d54953ead9923b012102111bb978a3c93a00038ae344a1a017d7fee8a9be9d0558b5793ce6f440704a96ffffffff01b0e78604000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000",
  "complete": true
}
```
Fortunately, JQ can easily capture data of that sort!

To use JQ, run `jq` at the backend of a pipe, and always use the standard invocation of `jq -r '.'`. The `-r` tells JQ to produce raw output, which will work for command-line variables, while the `.` tells jq to output. We protect that argument in `' '` because we'll need that protection later as our `jq` invocations get more complex.

To capture a specific value from a JSON object, you just list the key after the `.`:
```
$ bitcoin-cli signrawtransaction $rawtxhex | jq -r '.hex'
0200000001735dfa1584b930a78ad2c1d6db72dd2a80ae5e5d552ad97e19f1d50d41fdd6d8000000006a47304402202210ce4b2a037da02622c380278cd79fec4e0e016e66f3eb894a2dcbb9ee998f02202cac167e6abdbbf08af139fb7c6b86e9d2e58e5516cd566ae2d54953ead9923b012102111bb978a3c93a00038ae344a1a017d7fee8a9be9d0558b5793ce6f440704a96ffffffff01b0e78604000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000
```
With that tool in hand, you can capture information from JSON objects to command-line variables:
```
$ signedtx=$(bitcoin-cli signrawtransaction $rawtxhex | jq -r '.hex')
$ echo $signedtx
0200000001735dfa1584b930a78ad2c1d6db72dd2a80ae5e5d552ad97e19f1d50d41fdd6d8000000006a47304402202210ce4b2a037da02622c380278cd79fec4e0e016e66f3eb894a2dcbb9ee998f02202cac167e6abdbbf08af139fb7c6b86e9d2e58e5516cd566ae2d54953ead9923b012102111bb978a3c93a00038ae344a1a017d7fee8a9be9d0558b5793ce6f440704a96ffffffff01b0e78604000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000
```
You can then use those variables easily and without error:
```
$ bitcoin-cli sendrawtransaction $signedtx
3f9ccb6e16663e66dc119de1866610cc4f7a83079bfec2abf0598ed3adf10a78
```
## Use JQ to Access Single JSON Object Values in an Array by Key

**Usage Example:** _Capture the txid and vout for a selected UTXO._

Grabbing data out of a JSON object is easy, but what if that JSON object is in a JSON array? The `listunspent` command offers a great example, because it'll usually contain a number of different transactions. What if you want to capture specific information from _one_ of them?

When working with a JSON array, the first thing you need to do is tell JQ which index to access. For example, you might have looked through your transactions in `listunspent` and decided that you wanted to work with the first of them. You use `'.[0]'` to access that first element. The `[]` says that we're referencing a JSON array and the `0` says we want the 0th index.
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
You can then capture an individual value from that selected array by (1) using a pipe _within_ the JQ arguments; and then (2) requesting the specific value afterward, as in the previous example. The following would capture the `txid` from the 0th JSON object in the JSON array produced by `listunspent`:
```
~$ bitcoin-cli listunspent | jq -r '.[0] | .txid'
2b5f5798359e0e23e02764588166f222d4ce056419dec83c743b72aad171d708
```
Carefully note how the `' 's` go around the whole JQ expression _including_ the pipe.

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

## Use JQ to Access Matching JSON Object Values in an Array by Key

**Usage Example:** _List the value of all unspent UTXOs._

Instead of accessing a single, specific value in a specific JSON object, you could instead access all of a specific value across all the JSON objects. This is done with `.[]`, where no index is specified. For example, this would list all unspent funds:
```
$ bitcoin-cli listunspent | jq -r '.[] | .amount'
0.76
3.9
1.95
```

## Use JQ for Simple Calculations by Key

**Usage Example:** _Sum the value of all unspent UTXOs._

At this point, you can start using JQ output for simple math. For example, adding up the values of those unspent transactions with a simple `awk` script would give you the equivalent of `getbalance`:
```
$ bitcoin-cli listunspent | jq -r '.[] | .amount' | awk '{s+=$1} END {print s}'
6.61
$ bitcoin-cli getbalance
6.61000000
```

## Use JQ to Display Multiple JSON Object Values in an Array by Multiple Keys

**Usage Example:** _List usage information for all UTXOs._

JQ can easily capture individual elements from JSON objects and arrays and place those elements into variables. That will be its prime use in future sections. However, it can also be used to cut down huge amounts of information output by `bitcoin-cli` into reasonable amounts of information.

For example, you might want to see a listing of all your UTXOs (`.[]`) and get a listing of all of their most important information (`.txid, .vout, .amount`):
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
This makes it easy to decide which UTXOs to spend in a raw transaction, but it's not very pretty. 

Fortunately, JQ also lets you be fancy. You can use `{}`s to create new JSON objects (either for additional parsing or for pretty output). You also get to define the name of the new key for each of your values. The resulting output should be much more intuitive and less prone to error (though obviously, less useful for dumping info straight into variables). 

The following example shows the exact same parsing of `listunspent`, but with the each old JSON object rebuilt as a new, abridged JSON object, with all of the new values named with their old keys:
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
You could of course rename your new keys as you see fit. There's nothing magic in the original names:
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
## Use JQ to Access JSON Objects by Looked-Up Value

**Usage Example:** _Automatically look up UTXOs being used in a transaction._

**[[[[TODO: THIS SHOULD LOOKUP BY BOTH TXID + VOUT!]]]]]

The JQ lookups so far have been fairly simple: you use a key to look up one or more values in a JSON object or array. But what if you instead want to look up a value in a JSON object ... by another value? This sort of indirect lookup has real applicability when you're working with transactions built on existing UTXOs. For example, it can allow you to calculate the sum value of the UTXOs being used in a transaction, something that is vitally important.

This example uses the following raw transaction. Note that this is a more complex raw transaction with two inputs and two outputs. We'll learn about making those in a few sections; for now, it's necessary to be able to offer robust examples. Note that unlike our previous examples, this one has two objects in its `vin` array and two in its `vout` array.
```
$ bitcoin-cli decoderawtransaction $rawtxhex
{
  "txid": "959190d447589e4c0f6ad31459579a4e4f6945b8791eadc9e5f0e55fc6205317",
  "hash": "959190d447589e4c0f6ad31459579a4e4f6945b8791eadc9e5f0e55fc6205317",
  "size": 160,
  "vsize": 160,
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
    }, 
    {
      "txid": "ec0598918f6f5476cb90365651e8a2724ef26f949290bbf196f41ed96092a52f",
      "vout": 0,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 0.65050000,
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
    }, 
    {
      "value": 4.00000000,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 29193095edbef6378fbcd84a8ae7c8164e433ca7 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a91429193095edbef6378fbcd84a8ae7c8164e433ca788ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mjGGB8rSXAHSd9xGA1bLfqMpB41XWuDdSF"
        ]
      }
    }
  ]
}

```

### Retrieve the Value(s)

Assume that we know exactly how this transaction is constructed: we know that it uses two UTXOs as input. To retrieve the txid for the two UTXOs, we could use `jq` to look up the transaction's .vin value, then reference the .vin's 0th array, then that array's .txid value. Afterward, we could do the same with the 1st array. Easy:
```
$ usedtxid1=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[0] | .txid')
$ echo $usedtxid1
2b5f5798359e0e23e02764588166f222d4ce056419dec83c743b72aad171d708
$ usedtxid2=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[1] | .txid')
$ echo $usedtxid2
ec0598918f6f5476cb90365651e8a2724ef26f949290bbf196f41ed96092a52f
```
However, it would be better to have a general case that _automatically_ saved all the txids of our UTXOs.

We already know that we can access all of the `.txid`s by using an `.[]` array value. We can use that to build a general .txid lookup:
```
usedtxid=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .txid'))
$ echo ${usedtxid[0]}
2b5f5798359e0e23e02764588166f222d4ce056419dec83c743b72aad171d708
$ echo ${usedtxid[1]}
ec0598918f6f5476cb90365651e8a2724ef26f949290bbf196f41ed96092a52f
```
The only real trick here is how we saved the information to the bash shell. Rather than saving to a variable with `$(command)`, we instead saved to an array with `($(command))`. We were then able to access the individual bash array elements with a `${variable[n]}` construction. We could instead access the whole array with `${variable[@]}`, which we'll use in the next section. (Yeah, no one ever said bash was pretty.)

### Retrieve the Related Object(s)

We can now use this information to reference UTXOs in `listunspent`. To find the information on the UTXOs being used by the raw transaction, we need to look through the entire JSON array (`[]`). We can then choose (`select`) individual JSON objects that include (`contains`) our txids. (The `select` and `contains` arguments show off some of the complexity of JSON, but for now just know that this particular invocation will work.)

To start simply, this picks out the two UTXOs one at a time:
```
$ bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${usedtxid[0]}'"))'
{
  "txid": "2b5f5798359e0e23e02764588166f222d4ce056419dec83c743b72aad171d708",
  "vout": 1,
  "address": "mjtEqr4Fffd1XtpAkKoDkMBP54mMXJeQ3j",
  "account": "",
  "scriptPubKey": "76a9142fe70d51e886b9ef73b76c1743c5a2bb2894db8588ac",
  "amount": 0.76,
  "confirmations": 6689,
  "spendable": true,
  "solvable": true
}
$ bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${usedtxid[1]}'"))'
{
  "txid": "ec0598918f6f5476cb90365651e8a2724ef26f949290bbf196f41ed96092a52f",
  "vout": 0,
  "address": "mjtEqr4Fffd1XtpAkKoDkMBP54mMXJeQ3j",
  "account": "",
  "scriptPubKey": "76a9142fe70d51e886b9ef73b76c1743c5a2bb2894db8588ac",
  "amount": 3.9,
  "confirmations": 7191,
  "spendable": true,
  "solvable": true
}
```
A simple bash for-loop can instead give you _all_ of your UTXOs:
```
for txid in ${usedtxid[@]}; do bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'$txid'"))'; done
{
  "txid": "2b5f5798359e0e23e02764588166f222d4ce056419dec83c743b72aad171d708",
  "vout": 1,
  "address": "mjtEqr4Fffd1XtpAkKoDkMBP54mMXJeQ3j",
  "account": "",
  "scriptPubKey": "76a9142fe70d51e886b9ef73b76c1743c5a2bb2894db8588ac",
  "amount": 0.76,
  "confirmations": 6699,
  "spendable": true,
  "solvable": true
}
{
  "txid": "ec0598918f6f5476cb90365651e8a2724ef26f949290bbf196f41ed96092a52f",
  "vout": 0,
  "address": "mjtEqr4Fffd1XtpAkKoDkMBP54mMXJeQ3j",
  "account": "",
  "scriptPubKey": "76a9142fe70d51e886b9ef73b76c1743c5a2bb2894db8588ac",
  "amount": 3.9,
  "confirmations": 7202,
  "spendable": true,
  "solvable": true
}
```

## Use JSON for Simple Calculation by Value

**Usage Example:** _Automatically calculate the value of the UTXOs used in a transaction._

You can now go one step further, and request the .amount (or any other JSON key-value) from the UTXOs you're retrieving. 

This example uses the `$usedtxid` array that was set as follows:
```
$ usedtxid=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .txid'))
```
You can use it to look at .amounts individually with a bash `for` script:
```
$ for txid in ${usedtxid[@]}; do bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'$txid'")) | .amount'; done
0.76
3.9
```
Alternatively, you can sum up the .amounts with an `awk` script, to really see how much money is in the UTXOs that the transaction is spending:
```
$ for txid in ${usedtxid[@]}; do bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'$txid'")) | .amount'; done | awk '{s+=$1} END {print s}'
4.66
```

## Use JQ for Complex Calculations

**Usage Example:** _Calculate the fee for a transaction._

**[[[[TODO: THIS SCRIPT SHOULD LOOKUP BY BOTH TXID + VOUT!]]]]]


To figure out the complete transaction fee at this point just requires determining out how much money is going through the .vout:
```
$ bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vout  [] | .value' | awk '{s+=$1} END {print s}'
4.6505
```
Then, you subtract the .vout .amount (4.6505) from the .vin .amount (4.66) and you have a fee calculator!

Putting it all together gives us a complete calculator in just four lines of script:
```
$ usedtxid=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .txid'))
$ btcin=$(for txid in ${usedtxid[@]}; do bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'$txid'")) | .amount'; done | awk '{s+=$1} END {print s}')
$ btcout=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vout  [] | .value' | awk '{s+=$1} END {print s}')
$ echo "$btcin-$btcout"| /usr/bin/bc
.0095
```
And that's also a good example of why you double-check your fees: we'd intended to send a transaction fee of 5,000 satoshis, but sent 95,000 satoshis instead. Whoops!

> **WARNING:** This was not an intentional lesson. We genuinelly messed up our calculation of the transaction fee when we wrote our raw transaction. It's *that* easy, then your money is gone.

For more JSON magic (and if any of this isn't clear), please read the [JSON Manual](https://stedolan.github.io/jq/manual/) and the [JSON Cookbook](https://github.com/stedolan/jq/wiki/Cookbook). We'll be regularly using JQ in future examples.

### The Transaction Fee Script

If you'd like to have this JQ in a script, you can use the following.

> **WARNING:** This script has not been robustly checked. If you are going to use it to verify real transaction fees you should only do it as a triple-check after you've already done all the math yourself.

```
file: txfee-calc.sh

#!/bin/bash

if [ -z $1 ];
then
    echo "You must include the raw transaction hex as an argument.";
    exit;
fi

usedtxid=($(bitcoin-cli decoderawtransaction $1 | jq -r '.vin | .[] | .txid'))
btcin=$(for txid in ${usedtxid[@]}; do bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'$txid'")) | .amount'; done | awk '{s+=$1} END {print s}')
btcout=$(bitcoin-cli decoderawtransaction $1 | jq -r '.vout  [] | .value' | awk '{s+=$1} END {print s}')
echo "$btcin-$btcout"| /usr/bin/bc

```
You can then run the script as follows:
```
$ ./txfee-calc.sh $rawtxhex
.0095
```

## Make Some New Aliases

JQ code can be a little unwieldly, so you should consider adding some longer and more interesting invocations to your ~/.bash_profile. 

Any time you're looking through a large mass of information in a JSON object output by a bitcoin-cli command, consider writing an alias to strip it down to just what you want to see.
```
alias btcunspent="bitcoin-cli listunspent | jq -r '.[] | { txid: .txid, vout: .vout, amount: .amount }'"
alias btctxfee="~/txfee-calc.sh"
```


