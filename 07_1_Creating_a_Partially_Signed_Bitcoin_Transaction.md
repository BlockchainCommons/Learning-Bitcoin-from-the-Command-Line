# 7.1: Creating a Partially Signed Bitcoin Transaction

> :information_source: **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Partially Signed Bitcoin Transactions (PSBTs) are the newest way to vary the creation of basic Bitcoin transactions. They do so by introducing collaboration into every step of the process, allowing people (or programs) to not just authenticate transactions together (as with multisigs), but also to create, fund, authenticate, and broadcast collaboratively. 

> :warning: **VERSION WARNING:** This is an innovation from Bitcoin Core v 0.17.0. Earlier versions of Bitcoin Core will not be able to work with the PSBT while it is in process (though they will still be able to recognize the final transaction). Some updates and upgrades for PSBTs have continued through 0.20.0.

## Understand How PSBTs Work

Multisignatures were great for the very specific case of jointly holding funds and setting rules for who among the joint signers could OK the use of those funds. There are many use cases from a spousal joint bank account (a 1-of-2 signature) to a fiduciary requirement for dual control (a 2-of-2 signature) to an escrow (a 2-of-3 signature) and more. 

PSBTs may initially look sort of the same because they have a single overlapping bit of functionality, the ability to jointly sign a transaction, but they were created for a totally different use case. They recognize the need for multiple programs to jointly create a PSBT for a number of different reasons, and they provide a standardized format for doing so. They're especially useful for use cases involving hardware wallets (for which, see §7.3).

In the previous chapter, the mechanisms you learned for signing a multisig transactions and passing it between multiple recipients were implementation-dependent upon `bitcoin-cli` (and somewhat haphazard, witness the "error" messages you received for a transaction not signed by sufficient signers). In this chapter, you can expect the PSBT methodologies to be more programatically standardized, as they were explicitly created to pass unfinished transactions between multiple parties.

> :book: ***What is a PSBT?*** As the name suggests, a PSBT is a transaction that has not been fully signed. That's important, because once a transaction is signed, its content is logged in. There's an abstracted methodology for putting them together that describes the collaboration (and standardizes it). A *Creator* proposes a transaction; one or more *Updaters* supplement it; and one or more *Signers* authenticate it; before a *Finalizer* and *Extracter* turn it into a transaction.

PSBTs do their work ...

## Create a PSBT the Old-Fashioned Way

The easiest way to create a PSBT is to take an existing transaction and to use `converttopsbt` to turn it into a PSBT. This is certainly not the _best_ way since it requires you to make a transaction for one format (a raw transaction) then convert it to another (PSBT), but if you've got old software that can only generate raw transaction, you may need to use it.

You just create your raw transaction normally:

```
$ utxo_txid_1=$(bitcoin-cli listunspent | jq -r '.[0] | .txid')
$ utxo_vout_1=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
$ utxo_txid_2=$(bitcoin-cli listunspent | jq -r '.[1] | .txid')
$ utxo_vout_2=$(bitcoin-cli listunspent | jq -r '.[1] | .vout')
$ echo $utxo_txid_1 $utxo_vout_1 $utxo_txid_2 $utxo_vout_2
c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b 1 8748eff5f12ca886e3603d9e30227dcb3f0332e0706c4322fec96001f7c7f41c 0
$ recipient=tb1qcaedd724gts3aug73m78c7nfsv9d8zs9q6h2kd
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid_1'", "vout": '$utxo_vout_1' }, { "txid": "'$utxo_txid_2'", "vout": '$utxo_vout_2' } ]''' outputs='''{ "'$recipient'": 0.000008 }''')
```
Then you convert it:
```
$ psbt=$(bitcoin-cli -named converttopsbt hexstring=$rawtxhex)
$ echo $psbt
cHNidP8BAHsCAAAAAhuVpgVRdOxkuC7wW2rvw4800OVxl+QCgezYKHtCYN7GAQAAAAD/////HPTH9wFgyf4iQ2xw4DIDP8t9IjCePWDjhqgs8fXvSIcAAAAAAP////8BIAMAAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBQAAAAAAAAAA
```
You'll note that the PSBT encoding looks very different from the transaction hex.

We'll start looking at the contents of that PSBT shortly, but for a moment there are two other ways that you can create a PSBT.

## Create a PSBT the Hard Way

The first methodology for creating a PSBT without going through another format is the PSBT-equivalent of `createrawtransaction`. It's called `createpsbt` and it gives you maximal control at the cost of maximal labor and the maximal opportunity for error.

The command should look quite familiar:
```
$ psbt_1=$(bitcoin-cli -named createpsbt inputs='''[ { "txid": "'$utxo_txid_1'", "vout": '$utxo_vout_1' }, { "txid": "'$utxo_txid_2'", "vout": '$utxo_vout_2' } ]''' outputs='''{ "'$recipient'": 0.000008 }''')
```
The Bitcoin Core team made sure it worked exactly like `createrawtransaction`, so you don't actually need to do anything different here.

And to verify, the PSBT that's produced is just the same:
```
$ echo $psbt_1
cHNidP8BAHsCAAAAAhuVpgVRdOxkuC7wW2rvw4800OVxl+QCgezYKHtCYN7GAQAAAAD/////HPTH9wFgyf4iQ2xw4DIDP8t9IjCePWDjhqgs8fXvSIcAAAAAAP////8BIAMAAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBQAAAAAAAAAA
$ if [ "$psbt" == "$psbt_1" ]; then     echo "Strings are equal"; else     echo "Strings are not equal"; fi
Strings are equal
```

## Examine a PSBT

So what does your PSBT actually look like. You can assess that with the `decodepsbt` command:
```
$ bitcoin-cli -named decodepsbt psbt=$psbt
{
  "tx": {
    "txid": "7380bbb8cf17cf411aed5704e8cd348c2f5451fbf6f6da7d66e69f2dcc39887e",
    "hash": "7380bbb8cf17cf411aed5704e8cd348c2f5451fbf6f6da7d66e69f2dcc39887e",
    "version": 2,
    "size": 123,
    "vsize": 123,
    "weight": 492,
    "locktime": 0,
    "vin": [
      {
        "txid": "c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b",
        "vout": 1,
        "scriptSig": {
          "asm": "",
          "hex": ""
        },
        "sequence": 4294967295
      },
      {
        "txid": "8748eff5f12ca886e3603d9e30227dcb3f0332e0706c4322fec96001f7c7f41c",
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
        "value": 0.00000800,
        "n": 0,
        "scriptPubKey": {
          "asm": "0 c772d6f95542e11ef11e8efc7c7a69830ad38a05",
          "hex": "0014c772d6f95542e11ef11e8efc7c7a69830ad38a05",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1qcaedd724gts3aug73m78c7nfsv9d8zs9q6h2kd"
          ]
        }
      }
    ]
  },
  "unknown": {
  },
  "inputs": [
    {
    },
    {
    }
  ],
  "outputs": [
    {
    }
  ]
}
```

You can also use the `analyzepsbt` command to look at its current state:
```
standup@btctest20:~$ bitcoin-cli -named analyzepsbt psbt=$psbt
{
  "inputs": [
    {
      "has_utxo": false,
      "is_final": false,
      "next": "updater"
    },
    {
      "has_utxo": false,
      "is_final": false,
      "next": "updater"
    }
  ],
  "next": "updater"
}
```

## Update a PSBT


## Create a PSBT the Easy Way

If you think that there should be a command that's the equivalent of `fundrawtransaction`, you'll be pleased to know there is. 

$ bitcoin-cli -named walletcreatefundedpsbt inputs='''[ { "txid": "'$utxo_txid_1'", "vout": '$utxo_vout_1' }, { "txid": "'$utxo_txid_2'", "vout": '$utxo_vout_2' } ]''' outputs='''{ "'$recipient'": 0.000008 }'''
{
  "psbt": "cHNidP8BAOwCAAAABBuVpgVRdOxkuC7wW2rvw4800OVxl+QCgezYKHtCYN7GAQAAAAD/////HPTH9wFgyf4iQ2xw4DIDP8t9IjCePWDjhqgs8fXvSIcAAAAAAP////+F4znnstDchqSWKbMzVhfABTYrhWlrI5Dq5N+SafvZCgAAAAAA/v///+4XB6sA0qPJUrpZpHuDOVf6U5/45Gx+M/ktgJI0huFoAAAAAAD+////AiADAAAAAAAAFgAUx3LW+VVC4R7xHo78fHppgwrTigVAIxAAAAAAABYAFIXoteaWZc8dsEBQFhlsg63+L4AUAAAAAAABAR8BAAAAAAAAABYAFGxF06+odiCGxL122Kcax8l24ZGWIgYCusOmRf16ClN8YnrTf19pUFjFjRnOsAM+kT8amXXgDe4Q1gQ4AAAAAIABAACADgAAgAABAR9IAwAAAAAAABYAFNWRCRuAdKI3XtmYWpxLGO/s/UFlIgYCiDu1Rj431VJS2LPVwhQbAHs3yKfbYhH3XJVazF6jJesQ1gQ4AAAAAIABAACADAAAgAABAR8ThgEAAAAAABYAFNWRCRuAdKI3XtmYWpxLGO/s/UFlIgYCiDu1Rj431VJS2LPVwhQbAHs3yKfbYhH3XJVazF6jJesQ1gQ4AAAAAIABAACADAAAgAABAIwCAAAAAdVmsvkSBmfeHqNAe/wDCQ5lEp9F/587ftzCD1UL60nMAQAAABcWABRzFxRJfFPl8FJ6SxjAJzy3mCAMXf7///8CQEIPAAAAAAAZdqkUf0NzebzGbEB0XtwYkeprODDhl12IrMEwLQAAAAAAF6kU/d+kMX6XijmD+jWdUrLZlJUnH2iHPhQbACIGA+/e40wACf0XXzsgteWlUX/V0WdG8uY1tEYXra/q68OIENYEOAAAAACAAAAAgBIAAIAAACICAykwveCMRaAvXurm5dP7a1thD33hyGTaharyzLkhCnVRENYEOAAAAACAAQAAgBMAAIAA",
  "fee": 0.00042300,
  "changepos": 1
}

## Sign a PSBT

## Summary: Creating a Partially Signed Bitcoin Transaction

## What's Next?

Continue "Expanding Bitcoin Transactions" with [§7.1: Using a Partially Signed Bitcoin Transaction](06_7_Using_a_Partially_Signed_Bitcoin_Transaction.md).
