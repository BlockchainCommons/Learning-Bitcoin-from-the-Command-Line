# 4.3 Creating a Raw Transaction with Named Arguments

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

It can sometimes be daunting to figure out the right order for the arguments to a bitcoin-cli command. Fortunately, you can use _named arguments_ as an alternative.

> **VERSION WARNING:** This is an innovation from Bitcoin Core v 0.14.0. If you used our setup scripts, that's what you should have, but double-check your version if you have any problems. There is also a bug in the `createrawtransaction` command's use of named arguments that will presumably be fixed in 0.14.1.

## Create a Named Argument Alias

To use a named argument you must run `bitcoin-cli` with the `-named` argument. If you plan to do this regularly, you'll probably want to create an alias:
```
alias bitcoin-cli="bitcoin-cli -named"
```
As usual, that's for your ease of use, but we'll continue using the whole commands  to maintain clarity.

## Test Out a Named Argument

To learn what the names are for the arguments of a command, consult `bitcoin-cli help`. It will list the arguments in their proper order, but will now also give names for each of them.

For example, `bitcoin-cli help getbalance` lists these arguments:

   1. account
   2. minconf
   3. include watchonly
   
The following shows a traditional, unintuitive usage of `getbalance` using the minimum confirmation argument:
```
$ bitcoin-cli getbalance "*" 1
```
With named arguments, you can clarify what you're doing, which also minimizes mistakes:
```
$ bitcoin-cli -named getbalance account="*" minconf=1
```

## Test Out a Raw Transaction

Here's what the commands for sending a raw transaction would look like with named arguments:
```
$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[0] | .txid') 
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
$ recipient="n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi"

$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.7595 }''')
$ bitcoin-cli -named decoderawtransaction hexstring=$rawtxhex 
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

$ signedtx=$(bitcoin-cli -named signrawtransaction hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
8000dca7b1e7ab70f4056bc4512af6ffff7727d1588436521da3e5d886dbcddf
```
Voila! You've sent out another raw transaction, but this time using named arguments for clarity and to reduce errors.

> **VERSION WARNING:** There is where the bug in Bitcoin Core 0.14 shows up: the 'inputs' argument for 'createrawtransaction' is misnamed 'transactions'. So, if you're on Bitcoin Core 0.14.0, substitute the named argument 'inputs' with 'transactions' for this and future examples. Based on the current code updates, this example should work as shown when Bitcoin Core 0.14.1 is released.

## Summary: Creating a Raw Transaction with Named Arguments

By running `bitcoin-cli` with the `-named` flag, you can use named arguments rather than depending on ordered arguments. `bitcoin-cli help` will always show you the right name for each argument. This can result in more robust, easier-to-read, less error-prone code.

_These docs will use named arguments for all future examples, for clarity and to establish best practices. However, it will also show all arguments in the correct order. So, if you prefer not to use named args, just strip out the '-named' flag and all of the "name="s and the examples should continue to work correctly._
