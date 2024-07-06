# 4.3 Creating a Raw Transaction with Named Arguments

It can sometimes be daunting to figure out the right order for the arguments to a bitcoin-cli command. Fortunately, you can use _named arguments_ as an alternative.

> :warning: **VERSION WARNING:** This is an innovation from Bitcoin Core v 0.14.0. If you used our setup scripts, that's what you should have, but double-check your version if you have any problems. There is also a bug in the `createrawtransaction` command's use of named arguments that will presumably be fixed in 0.14.1.

## Create a Named Argument Alias

To use a named argument you must run `bitcoin-cli` with the `-named` argument. If you plan to do this regularly, you'll probably want to create an alias:
```
alias bitcoin-cli="bitcoin-cli -named"
```
As usual, that's for your ease of use, but we'll continue using the whole commands  to maintain clarity.

## Test Out a Named Argument

To learn what the names are for the arguments of a command, consult `bitcoin-cli help`. It will list the arguments in their proper order, but will now also give names for each of them.

For example, `bitcoin-cli help getbalance` lists these arguments:

   1. dummy [used to be account]
   2. minconf
   3. include_watchonly
   4. avoid_reuse
   
The following shows a traditional, unintuitive usage of `getbalance` using the minimum confirmation argument:
```
$ bitcoin-cli getbalance "*" 1
```
With named arguments, you can clarify what you're doing, which also minimizes mistakes:
```
$ bitcoin-cli -named getbalance minconf=1
```

## Test Out a Raw Transaction

Here's what the commands for sending a raw transaction would look like with named arguments:
```
$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[0] | .txid') 
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
$ recipient="n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi"

$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.00001 }''')
$ bitcoin-cli -named decoderawtransaction hexstring=$rawtxhex 
{
  "txid": "2b59c31bc232c0399acee4c2a381b564b6fec295c21044fbcbb899ffa56c3da5",
  "hash": "2b59c31bc232c0399acee4c2a381b564b6fec295c21044fbcbb899ffa56c3da5",
  "version": 2,
  "size": 85,
  "vsize": 85,
  "weight": 340,
  "locktime": 0,
  "vin": [
    {
      "txid": "ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36",
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
      "value": 0.00001000,
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

$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
e70dd2aa13422d12c222481c17ca21a57071f92ff86bdcffd7eaca71772ba172
```
Voila! You've sent out another raw transaction, but this time using named arguments for clarity and to reduce errors.

> :warning: **VERSION WARNING:** There is where the bug in Bitcoin Core 0.14 shows up: the 'inputs' argument for 'createrawtransaction' is misnamed 'transactions'. So, if you're on Bitcoin Core 0.14.0, substitute the named argument 'inputs' with 'transactions' for this and future examples. However, as of Bitcoin Core 0.14.1, this code should work as shown.

## Summary: Creating a Raw Transaction with Named Arguments

By running `bitcoin-cli` with the `-named` flag, you can use named arguments rather than depending on ordered arguments. `bitcoin-cli help` will always show you the right name for each argument. This can result in more robust, easier-to-read, less error-prone code.

_These docs will use named arguments for all future examples, for clarity and to establish best practices. However, it will also show all arguments in the correct order. So, if you prefer not to use named args, just strip out the '-named' flag and all of the "name="s and the examples should continue to work correctly._

## What's Next?

Continue "Sending Bitcoin Transactions" with [§4.4: Sending Coins with Raw Transactions](04_4_Sending_Coins_with_a_Raw_Transaction.md).
