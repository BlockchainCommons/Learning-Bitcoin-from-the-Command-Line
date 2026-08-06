# 5.3 Creating a Raw Transaction with Named Arguments

It can sometimes be daunting to figure out the right order for the arguments to a bitcoin-cli command. Fortunately, you can use _named arguments_ as an alternative.

## Create a Named Argument Alias

To use a named argument you must run `bitcoin-cli` with the `-named` argument. If you plan to do this regularly, you'll probably want to create an alias:

```sh
alias bitcoin-cli="bitcoin-cli -named"
```

As usual, that's for your ease of use, but we'll continue using the
whole commands to maintain clarity.

## Test Out a Named Argument

To learn what the names are for the arguments of a command, consult `bitcoin-cli help` for that command. It will list the arguments in their proper order, but will now also give names for each of them.

For example, `bitcoin-cli help getbalance` lists these arguments:

   1. dummy [used to be account]
   2. minconf
   3. include_watchonly
   4. avoid_reuse
   
The following shows a traditional, unintuitive usage of `getbalance` using the minimum confirmation argument to only show the balance for transactions with at least one confirmation:

```sh
bitcoin-cli getbalance "*" 1
```
With named arguments, you can clarify what you're doing, which also minimizes mistakes:

```sh
bitcoin-cli -named getbalance minconf=1
```

## Test Out a Raw Transaction

Here's what the commands for sending a raw transaction would look like with named arguments:

```sh
utxo_txid=$(bitcoin-cli listunspent | jq -r '.[0] | .txid') 
utxo_vout=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
recipient="tb1qlw280hk7cf7r97sm896s83wysat4vrqzpalm7e"

rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.01895 }''')
bitcoin-cli -named decoderawtransaction hexstring=$rawtxhex

| {
|   "txid": "615813d052e48b9761afed8732b78ff67f3ee26390de9e1274418629bd80e45c",
|   "hash": "615813d052e48b9761afed8732b78ff67f3ee26390de9e1274418629bd80e45c",
|   "version": 2,
|   "size": 82,
|   "vsize": 82,
|   "weight": 328,
|   "locktime": 0,
|   "vin": [
|     {
|       "txid": "f67ffd9acd5e264c3fcce9b96f4f713e56adf99e65c1a9a55353d9b9706629fd",
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
|       "value": 0.01895000,
|       "n": 0,
|       "scriptPubKey": {
|         "asm": "0 f1ad72d46aa89d5e622e42bbbb75064eebd9d101",
|         "desc": "addr(tb1q7xkh94r24zw4uc3wg2amkagxfm4an5gpwrgsnl)#tn6nft96",
|         "hex": "0014f1ad72d46aa89d5e622e42bbbb75064eebd9d101",
|         "address": "tb1q7xkh94r24zw4uc3wg2amkagxfm4an5gpwrgsnl",
|         "type": "witness_v0_keyhash"
|       }
|     }
|   ]
| }

signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r '.hex')
bitcoin-cli -named sendrawtransaction hexstring=$signedtx

| 615813d052e48b9761afed8732b78ff67f3ee26390de9e1274418629bd80e45c
```
Voila! You've sent out another raw transaction, but this time using named arguments for clarity and to reduce errors.

## Undestand Args Parameters

Many RPC commands also have one or more variables designated as "args", which allows them to be input into a `-named` command in the proper order without requiring the name.

For example, where traditionally you'd type this to designate the first two variables for `getbalance`:

```sh
bitcoin-cli -named getbalance dummy="*" minconf=1
```

You can instead type:

```sh
bitcoin-cli -named getbalance "*" minconf=1
```

Because the `dummy` variable is (somewhat inexplicably) defined as an `args` variable.

This technique will _not_ be used in this course because it obfuscates what you're doing and requires lookups in the documents to see what variables you're referring to.

## Summary: Creating a Raw Transaction with Named Arguments

By running `bitcoin-cli` with the `-named` flag, you can use named arguments rather than depending on ordered arguments. `bitcoin-cli help` will always show you the right name for each argument. This can result in more robust, easier-to-read, less error-prone code.

_These docs will use named arguments for all future examples, for clarity and to establish best practices. However, it will also show all arguments in the correct order. So, if you prefer not to use named args, just strip out the '-named' flag and all of the "name="s and the examples should continue to work correctly, except in cases where we've skipped arguments._

## What's Next?

Continue "Sending Bitcoin Transactions" with [§5.4: Sending Coins with Raw Transactions](05_4_Sending_Coins_with_a_Raw_Transaction.md).
