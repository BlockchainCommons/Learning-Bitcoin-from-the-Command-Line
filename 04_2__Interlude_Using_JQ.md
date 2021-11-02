# Interlude: Using JQ

Creating a raw transaction revealed how more complex bitcoin-cli results can't easily be saved into command-line variables. The answer is JQ, which allows you to filter out individual elements from more complex JSON data.

## Install JQ

JQ is available from a [Github repository](https://stedolan.github.io/jq/). Just download for Linux, OS X, or Windows, as appropriate.

Once you've downloaded the binary, you can install it on your system. If you're working on a Debian VPS as we suggest, your installation will look like this:
```
$ mv jq-linux64 jq
$ sudo /usr/bin/install -m 0755 -o root -g root -t /usr/local/bin jq
```
> :book: ***What is JQ?*** The repository explains it best, saying "jq is like sed for JSON data - you can use it to slice and filter and map and transform structured data with the same ease that sed, awk, grep and friends let you play with text."

## Use JQ to Access a JSON Object Value by Key

**Usage Example:** _Capture the hex from a signed raw transaction._

In the previous section, the use of `signrawtransaction` offered an example of not being able to easily capture data into variables due to the use of JSON output:
```
$ bitcoin-cli signrawtransactionwithwallet $rawtxhex
{
  "hex": "02000000013a6e4279b799791049e1826602e84d2e36797e2005887b98c3ecf16b01b7f361010000006a4730440220335d15a2a2ca3ce6a302ce041686739d4a38eb0599a5ea08305de71965268d05022015f77a33cf7d613015b2aba5beb03088033625505ad5d4d0624defdbea22262b01210278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132ffffffff01409c0000000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000",
  "complete": true
}
```
Fortunately, JQ can easily capture data of that sort!

To use JQ, run `jq` at the backend of a pipe, and always use the standard invocation of `jq -r '.'`. The `-r` tells JQ to produce raw output, which will work for command-line variables, while the `.` tells jq to output. We protect that argument in `' '` because we'll need that protection later as our `jq` invocations get more complex.

To capture a specific value from a JSON object, you just list the key after the `.`:
```
$ bitcoin-cli signrawtransactionwithwallet $rawtxhex | jq -r '.hex'
02000000013a6e4279b799791049e1826602e84d2e36797e2005887b98c3ecf16b01b7f361010000006a4730440220335d15a2a2ca3ce6a302ce041686739d4a38eb0599a5ea08305de71965268d05022015f77a33cf7d613015b2aba5beb03088033625505ad5d4d0624defdbea22262b01210278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132ffffffff01409c0000000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000
```
With that tool in hand, you can capture information from JSON objects to command-line variables:
```
$ signedtx=$(bitcoin-cli signrawtransactionwithwallet $rawtxhex | jq -r '.hex')
$ echo $signedtx
02000000013a6e4279b799791049e1826602e84d2e36797e2005887b98c3ecf16b01b7f361010000006a4730440220335d15a2a2ca3ce6a302ce041686739d4a38eb0599a5ea08305de71965268d05022015f77a33cf7d613015b2aba5beb03088033625505ad5d4d0624defdbea22262b01210278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132ffffffff01409c0000000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac00000000
```
You can then use those variables easily and without error:
```
$ bitcoin-cli sendrawtransaction $signedtx
3f9ccb6e16663e66dc119de1866610cc4f7a83079bfec2abf0598ed3adf10a78
```
## Use JQ to Access Single JSON Object Values in an Array by Key

**Usage Example:** _Capture the txid and vout for a selected UTXO._

Grabbing data out of a JSON object is easy, but what if that JSON object is in a JSON array? The `listunspent` command offers a great example, because it'll usually contain a number of different transactions. What if you want to capture specific information from _one_ of them?

When working with a JSON array, the first thing you need to do is tell JQ which index to access. For example, you might have looked through your transactions in `listunspent` and decided that you wanted to work with the second of them. You use `'.[1]'` to access that second element. The `[]` says that we're referencing a JSON array and the `1` says we want the 1st index.
```
$ bitcoin-cli listunspent | jq -r '.[1]'
{
  "txid": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
  "vout": 0,
  "address": "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
  "label": "",
  "scriptPubKey": "76a9142d573900aa357a38afd741fbf24b075d263ea6e088ac",
  "amount": 0.00022,
  "confirmations": 9,
  "spendable": true,
  "solvable": true,
  "desc": "pkh([d6043800/0'/0'/3']0278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132)#nhjc3f8y",
  "safe": true
}
```
You can then capture an individual value from that selected array by (1) using a pipe _within_ the JQ arguments; and then (2) requesting the specific value afterward, as in the previous example. The following would capture the `txid` from the 1st JSON object in the JSON array produced by `listunspent`:
```
$ bitcoin-cli listunspent | jq -r '.[1] | .txid'
91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c
```
Carefully note how the `' 's` go around the whole JQ expression _including_ the pipe.

This method can be used to fill in variables for a UTXO that you want to use:
```
$ newtxid=$(bitcoin-cli listunspent | jq -r '.[1] | .txid')
$ newvout=$(bitcoin-cli listunspent | jq -r '.[1] | .vout')
$ echo $newtxid
91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c
$ echo $newvout
0
```
Voila! We could now create a new raw transaction using our 1st UTXO as an input, without having to type in any of the UTXO info by hand!

## Use JQ to Access Matching JSON Object Values in an Array by Key

**Usage Example:** _List the value of all unspent UTXOs._

Instead of accessing a single, specific value in a specific JSON object, you could instead access all of a specific value across all the JSON objects. This is done with `.[]`, where no index is specified. For example, this would list all unspent funds:
```
$ bitcoin-cli listunspent | jq -r '.[] | .amount'
0.0001
0.00022
```

## Use JQ for Simple Calculations by Key

**Usage Example:** _Sum the value of all unspent UTXOs._

At this point, you can start using JQ output for simple math. For example, adding up the values of those unspent transactions with a simple `awk` script would give you the equivalent of `getbalance`:
```
$ bitcoin-cli listunspent | jq -r '.[] | .amount' | awk '{s+=$1} END {print s}'
0.00032
$ bitcoin-cli getbalance
0.00032000
```

## Use JQ to Display Multiple JSON Object Values in an Array by Multiple Keys

**Usage Example:** _List usage information for all UTXOs._

JQ can easily capture individual elements from JSON objects and arrays and place those elements into variables. That will be its prime use in future sections. However, it can also be used to cut down huge amounts of information output by `bitcoin-cli` into reasonable amounts of information.

For example, you might want to see a listing of all your UTXOs (`.[]`) and get a listing of all of their most important information (`.txid, .vout, .amount`):
```
$ bitcoin-cli listunspent | jq -r '.[] | .txid, .vout, .amount'
ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36
0
0.0001
91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c
0
0.00022
```
This makes it easy to decide which UTXOs to spend in a raw transaction, but it's not very pretty.

Fortunately, JQ also lets you be fancy. You can use `{}`s to create new JSON objects (either for additional parsing or for pretty output). You also get to define the name of the new key for each of your values. The resulting output should be much more intuitive and less prone to error (though obviously, less useful for dumping info straight into variables).

The following example shows the exact same parsing of `listunspent`, but with the each old JSON object rebuilt as a new, abridged JSON object, with all of the new values named with their old keys:
```
$ bitcoin-cli listunspent | jq -r '.[] | { txid: .txid, vout: .vout, amount: .amount }'
{
  "txid": "ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36",
  "vout": 0,
  "amount": 0.0001
}
{
  "txid": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
  "vout": 0,
  "amount": 0.00022
}
```
You could of course rename your new keys as you see fit. There's nothing magic in the original names:
```
$ bitcoin-cli listunspent | jq -r '.[] | { tx: .txid, output: .vout, bitcoins: .amount }'
{
  "tx": "ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36",
  "output": 0,
  "bitcoins": 0.0001
}
{
  "tx": "91261eafae15ea53dedbea7c1db748c52bbc04a85859ffd0d839bda1421fda4c",
  "output": 0,
  "bitcoins": 0.00022
}
```
## Use JQ to Access JSON Objects by Looked-Up Value

**Usage Example:** _Automatically look up UTXOs being used in a transaction._

The JQ lookups so far have been fairly simple: you use a key to look up one or more values in a JSON object or array. But what if you instead want to look up a value in a JSON object ... by another value? This sort of indirect lookup has real applicability when you're working with transactions built on existing UTXOs. For example, it can allow you to calculate the sum value of the UTXOs being used in a transaction, something that is vitally important.

This example uses the following raw transaction. Note that this is a more complex raw transaction with two inputs and two outputs. We'll learn about making those in a few sections; for now, it's necessary to be able to offer robust examples. Note that unlike our previous examples, this one has two objects in its `vin` array and two in its `vout` array.
```
$ bitcoin-cli decoderawtransaction $rawtxhex
{
  "txid": "6f83a0b78c598de01915554688592da1d7a3047eacacc8a9be39f5396bf0a07e",
  "hash": "6f83a0b78c598de01915554688592da1d7a3047eacacc8a9be39f5396bf0a07e",
  "size": 160,
  "vsize": 160,
  "version": 2,
  "locktime": 0,
  "vin": [
    {
      "txid": "d261b9494eb29084f668e1abd75d331fc2d6525dd206b2f5236753b5448ca12c",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967295
    },
    {
      "txid": "c7c7f6371ec19330527325908a544bbf8401191645598301d24b54d37e209e7b",
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
      "value": 1.00000000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 cfc39be7ea3337c450a0c77a839ad0e160739058 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914cfc39be7ea3337c450a0c77a839ad0e16073905888ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mzTWVv2QSgBNqXx7RC56zEhaQPve8C8VS9"
        ]
      }
    },
    {
      "value": 0.04500000,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 166692bda9f25ced145267bb44286e8ee3963d26 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914166692bda9f25ced145267bb44286e8ee3963d2688ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mhZQ3Bih6wi7jP1tpFZrCcyr4NsfCapiZP"
        ]
      }
    }
  ]
}
```

### Retrieve the Value(s)

Assume that we know exactly how this transaction is constructed: we know that it uses two UTXOs as input. To retrieve the txid for the two UTXOs, we could use `jq` to look up the transaction's .vin value, then reference the .vin's 0th array, then that array's .txid value. Afterward, we could do the same with the 1st array, then the same with the .vin's two .vout values. Easy:
```
$ usedtxid1=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[0] | .txid')
$ echo $usedtxid1
d261b9494eb29084f668e1abd75d331fc2d6525dd206b2f5236753b5448ca12c
$ usedtxid2=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[1] | .txid')
$ echo $usedtxid2
c7c7f6371ec19330527325908a544bbf8401191645598301d24b54d37e209e7b

$ usedvout1=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[0] | .vout')
$ echo $usedvout1
1
$ usedvout2=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[1] | .vout')
$ echo $usedvout2
1
```
However, it would be better to have a general case that _automatically_ saved all the txids of our UTXOs.

We already know that we can access all of the `.txid`s by using an `.[]` array value. We can use that to build a general .txid lookup:
```
$ usedtxid=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .txid'))
$ echo ${usedtxid[0]}
d261b9494eb29084f668e1abd75d331fc2d6525dd206b2f5236753b5448ca12c
$ echo ${usedtxid[1]}
c7c7f6371ec19330527325908a544bbf8401191645598301d24b54d37e209e7b

$ usedvout=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .vout'))
$ echo ${usedvout[0]}
1
$ echo ${usedvout[1]}
1
```
The only real trick here is how we saved the information using the bash shell. Rather than saving to a variable with `$(command)`, we instead saved to an array with `($(command))`. We were then able to access the individual bash array elements with a `${variable[n]}` construction. We could instead access the whole array with `${variable[@]}`. (Yeah, no one ever said bash was pretty.)

> :warning: **WARNING:** Always remember that a UTXO is a transaction _plus_ a vout. We missed the vout the first time we wrote this JQ example, and it stopped working when we ended up with a situation where we'd been sent two `vouts` from the same transaction.

### Retrieve the Related Object(s)

You can now use your saved `txid` and `vout` information to reference UTXOs in `listunspent`. To find the information on the UTXOs being used by the raw transaction, you need to look through the entire JSON array (`[]`) of unspent transactions. You can then choose (`select`) individual JSON objects that include (`contains`) the txids. You _then_ select (`select`) the transactions among those that _also_ contains (`contain`) the correct vout.

The use of another level of pipe is the standard methodology of JQ: you grab a set of data, then you whittle it down to all the relevant transactions, then you whittle it down to the vouts that were actually used from those transactions. However, the `select` and `contains` arguments are something new. They show off some of the complexity of JSON that goes beyond the scope of this tutorial; for now just know that this particular invocation will work to grab matching objects.

To start simply, this picks out the two UTXOs one at a time:
```
$ bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${usedtxid[0]}'")) | select(.vout | contains('${usedvout[0]}'))'
{
  "txid": "d261b9494eb29084f668e1abd75d331fc2d6525dd206b2f5236753b5448ca12c",
  "vout": 1,
  "address": "miSrC3FvkPPZgqqvCiQycq7io7wTSVsAFH",
  "scriptPubKey": "76a91420219e4f3c6bc0f6524d538009e980091b3613e888ac",
  "amount": 0.9,
  "confirmations": 6,
  "spendable": true,
  "solvable": true
}
$ bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${usedtxid[1]}'")) | select(.vout | contains('${usedvout[1]}'))'
{
  "txid": "c7c7f6371ec19330527325908a544bbf8401191645598301d24b54d37e209e7b",
  "vout": 1,
  "address": "mzizSuAy8aL1ytFijds7pm4MuDPx5aYH5Q",
  "scriptPubKey": "76a914d2b12da30320e81f2dfa416c5d9499d08f778f9888ac",
  "amount": 0.4,
  "confirmations": 5,
  "spendable": true,
  "solvable": true
}
```
A simple bash for-loop could instead give you _all_ of your UTXOs:
```
$ for ((i=0; i<${#usedtxid[*]}; i++)); do txid=${usedtxid[i]}; vout=${usedvout[i]}; bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${txid}'")) | select(.vout | contains('$vout'))'; done;
{
  "txid": "d261b9494eb29084f668e1abd75d331fc2d6525dd206b2f5236753b5448ca12c",
  "vout": 1,
  "address": "miSrC3FvkPPZgqqvCiQycq7io7wTSVsAFH",
  "scriptPubKey": "76a91420219e4f3c6bc0f6524d538009e980091b3613e888ac",
  "amount": 0.9,
  "confirmations": 7,
  "spendable": true,
  "solvable": true
}
{
  "txid": "c7c7f6371ec19330527325908a544bbf8401191645598301d24b54d37e209e7b",
  "vout": 1,
  "address": "mzizSuAy8aL1ytFijds7pm4MuDPx5aYH5Q",
  "scriptPubKey": "76a914d2b12da30320e81f2dfa416c5d9499d08f778f9888ac",
  "amount": 0.4,
  "confirmations": 6,
  "spendable": true,
  "solvable": true
}

```
Note that we used yet another bit of array ugliness `${#usedtxid[*]}` to determine the size of the array, then accessed each value in the `usedtxid` array and each value in the parallel `usedvout` array, putting them into simpler variables for less-ugly access.

## Use JSON for Simple Calculation by Value

**Usage Example:** _Automatically calculate the value of the UTXOs used in a transaction._

You can now go one step further, and request the .amount (or any other JSON key-value) from the UTXOs you're retrieving.

This example repeats the usage the `$usedtxid` and `$usedvout` arrays that were set as follows:
```
$ usedtxid=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .txid'))
$ usedvout=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .vout'))
```
The same `for` script can be used to step through those arrays, but with an added pipe in the JQ that outputs the `amount` value for each of the UTXOs selected.
```
$ for ((i=0; i<${#usedtxid[*]}; i++)); do txid=${usedtxid[i]}; vout=${usedvout[i]}; bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${txid}'")) | select(.vout | contains('$vout')) | .amount'; done;
0.9
0.4
```
At this point, you can also sum up the .amounts with an `awk` script, to really see how much money is in the UTXOs that the transaction is spending:
```
$ for ((i=0; i<${#usedtxid[*]}; i++)); do txid=${usedtxid[i]}; vout=${usedvout[i]}; bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${txid}'")) | select(.vout | contains('$vout')) | .amount'; done | awk '{s+=$1} END {print s}'
1.3
```
Whew!

## Use JQ for Complex Calculations

**Usage Example:** _Calculate the fee for a transaction._

Figuring out the complete transaction fee at this point just requires one more bit of math: determining how much money is going through the .vout. That's a simple use of JQ where you just use `awk` to sum up the `value` of all the `vout` information:
```
$ bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vout  [] | .value' | awk '{s+=$1} END {print s}'
1.045
```
To complete the transaction fee calculation, you subtract the .vout .amount (1.045) from the .vin .amount (1.3).

To do this, you'll need to install `bc`:
```
$ sudo apt-get install bc
```

Putting it all together creates a complete calculator in just five lines of script:
```
$ usedtxid=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .txid'))
$ usedvout=($(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vin | .[] | .vout'))
$ btcin=$(for ((i=0; i<${#usedtxid[*]}; i++)); do txid=${usedtxid[i]}; vout=${usedvout[i]}; bitcoin-cli listunspent | jq -r '.[] | select (.txid | contains("'${txid}'")) | select(.vout | contains('$vout')) | .amount'; done | awk '{s+=$1} END {print s}')
$ btcout=$(bitcoin-cli decoderawtransaction $rawtxhex | jq -r '.vout  [] | .value' | awk '{s+=$1} END {print s}')
$ echo "$btcin-$btcout"| /usr/bin/bc
.255
```
And that's also a good example of why you double-check your fees: we'd intended to send a transaction fee of 5,000 satoshis, but sent 255,000 satoshis instead. Whoops!

> :warning: **WARNING:** The first time we wrote up this lesson, we genuinely miscalculated our fee and didn't see it until we ran our fee calculator. It's *that* easy, then your money is gone. (The example above is actually from our second iteration of the calculator, and that time we made the mistake on purpose.)

For more JSON magic (and if any of this isn't clear), please read the [JSON Manual](https://stedolan.github.io/jq/manual/) and the [JSON Cookbook](https://github.com/stedolan/jq/wiki/Cookbook). We'll be regularly using JQ in future examples.

## Make Some New Aliases

JQ code can be a little unwieldy, so you should consider adding some longer and more interesting invocations to your ~/.bash_profile. 

Any time you're looking through a large mass of information in a JSON object output by a `bitcoin-cli` command, consider writing an alias to strip it down to just what you want to see.
```
alias btcunspent="bitcoin-cli listunspent | jq -r '.[] | { txid: .txid, vout: .vout, amount: .amount }'"
```

## Run The Transaction Fee Script

The [Fee Calculation Script](src/04_2_i_txfee-calc.sh) is available in src-code directory. You can download it and save it as `txfee-calc.sh`.

> :warning: **WARNING:** This script has not been robustly checked. If you are going to use it to verify real transaction fees you should only do it as a triple-check after you've already done all the math yourself.

Be sure the permissions on the script are right:
```
$ chmod 755 txfee-calc.sh
```
You can then run the script as follows:
```
$ ./txfee-calc.sh $rawtxhex
.255
```
You may also want to create an alias:
```
alias btctxfee="~/txfee-calc.sh"
```

## Summary: Using JQ

JQ makes it easy to extract information from JSON arrays and objects. It can also be used in shell scripts for fairly complex calculations that will make your life easier.

## What's Next?

Continue "Sending Bitcoin Transactions" with [§4.3 Creating a Raw Transaction with Named Arguments](04_3_Creating_a_Raw_Transaction_with_Named_Arguments.md).

