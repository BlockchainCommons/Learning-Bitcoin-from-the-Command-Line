# 7.1: Creating a Partially Signed Bitcoin Transaction

> :information_source: **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Partially Signed Bitcoin Transactions (PSBTs) are the newest way to vary the creation of basic Bitcoin transactions. They do so by introducing collaboration into every step of the process, allowing people (or programs) to not just authenticate transactions together (as with multisigs), but also to create, fund, authenticate, and broadcast collaboratively. 

> :warning: **VERSION WARNING:** This is an innovation from Bitcoin Core v 0.17.0. Earlier versions of Bitcoin Core will not be able to work with the PSBT while it is in process (though they will still be able to recognize the final transaction). Some updates and upgrades for PSBTs have continued through 0.20.0.

## Understand How PSBTs Work

Multisignatures were great for the very specific case of jointly holding funds and setting rules for who among the joint signers could authenticate the use of those funds. There are many use cases, such as: a spousal joint bank account (a 1-of-2 signature); a fiduciary requirement for dual control (a 2-of-2 signature); and an escrow (a 2-of-3 signature). 

> :book: ***What is a PSBT?*** As the name suggests, a PSBT is a transaction that has not been fully signed. That's important, because once a transaction is signed, its content is locked in. [BIP174](https://github.com/bitcoin/bips/blob/master/bip-0174.mediawiki) defined an abstracted methodology for putting PSBTs together that describes roles in the collaboration (and standardizes them). A *Creator* proposes a transaction; one or more *Updaters* supplement it; and one or more *Signers* authenticate it; before a *Finalizer* completes it; and an *Extracter* turn it into a transaction for the Bitcoin network. There may also be a *Combiner* who merges parallel PSBTs from different users. 

PSBTs may initially look sort of the same because they have a single overlapping bit of functionality: the ability to jointly sign a transaction. However, they were created for a totally different use case. They recognize the need for multiple programs to jointly create a transaction for a number of different reasons, and they provide a regularized format for doing so. They're especially useful for use cases involving hardware wallets (for which, see §7.3), which may be airgapped or have minimum transaction history.

In general, PSBTs provide a number of functional elements that improve these use cases:

1. They provide a _standard_ for collaboratively creating transactions, whereas previous methodologies (including the multi-sig one from the previous chapter) were implementation dependent.
2. They support a _wider variety of use cases_, including joint funding.
3. They support _airgapped wallets_ and other cases where a wallet may not have full transaction history.
4. They optional allow for the combination of _non-serialized transaction creation_, not requiring an ever-bigger hex code to be passed from user to user.

PSBTs do their work by supplementing normal transaction information with a set of inputs and outputs, each of which defines everything you need to know about those UTXOs, so that even an airgapped wallet can make an informed decision about signatures. Thus, an input lists out the amount of money in a UTXO and what needs to be done to spend it, while an output does the same for the UTXOs it's creating.

## Create a PSBT the Old-Fashioned Way
#### PSBT Role: Creator

The easiest way to create a PSBT is to take an existing transaction and use `converttopsbt` to turn it into a PSBT. This is certainly not the _best_ way since it requires you to make a transaction for one format (a raw transaction) then convert it to another (PSBT), but if you've got old software that can only generate raw transaction, you may need to use it.

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

But if you can, you want to create the PSBT directly instead ...

## Create a PSBT the Hard Way
#### PSBT Role: Creator

The first methodology for creating a PSBT without going through another format is the PSBT-equivalent of `createrawtransaction`. It's called `createpsbt` and it gives you maximal control at the cost of maximal labor and the maximal opportunity for error.

The CLI should look quite familiar, just with a new RPC command:
```
$ psbt_1=$(bitcoin-cli -named createpsbt inputs='''[ { "txid": "'$utxo_txid_1'", "vout": '$utxo_vout_1' }, { "txid": "'$utxo_txid_2'", "vout": '$utxo_vout_2' } ]''' outputs='''{ "'$recipient'": 0.000008 }''')
```
The Bitcoin Core team made sure that `createpsbt` worked much like `createrawtransaction`, so you don't need to learn a different creation format.

You can verify that the new PSBT is the same as the one created by `converttopsbt`:
```
$ echo $psbt_1
cHNidP8BAHsCAAAAAhuVpgVRdOxkuC7wW2rvw4800OVxl+QCgezYKHtCYN7GAQAAAAD/////HPTH9wFgyf4iQ2xw4DIDP8t9IjCePWDjhqgs8fXvSIcAAAAAAP////8BIAMAAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBQAAAAAAAAAA
$ if [ "$psbt" == "$psbt_1" ]; then     echo "PSBTs are equal"; else     echo "PSBTs are not equal"; fi
PSBTs are equal
```

## Examine a PSBT
#### PSBT Role: Any

So what does your PSBT actually look like. You can see that with the `decodepsbt` command:
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
It's important to note that even though we've defined the fundamentals of the transaction: the `vins` of where the money is coming from and the `vouts` of where it's going to, we haven't yet defined the `inputs` and `outputs` that are the heart of a PSBT and that are required for offline users to assess them. This is expected: the role of the Creator as defined in [BIP174](https://github.com/bitcoin/bips/blob/master/bip-0174.mediawiki) is to outline the transaction, while the role of the Updater is to start filling in the PSBT-specific data. And, that's all that `createpsbt` did because it doesn't have access to your wallet! (We'll meet some `wallet` commands momentarily.)

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
Similarly, `analyzepsbt` shows us a PSBT that needs work. We get a look at each of the two `inputs` (corresponding to the two `vins`), and neither one has the information it needs.

## Finalize a PSBT
#### PSBT Role: Updater, Signer, Finalizer

There is a `utxoupdatepsbt` command that can be used to automatically update UTXOs and import their descriptor information by hand, but you don't want to use it unless you have a use case where you don't have all of that information in the wallets of everyone who will be signing the PSBT.

Instead, you should move straight on to finalizing your transaction with `walletprocesspsbt`:
```
$ bitcoin-cli walletprocesspsbt $psbt
{
  "psbt": "cHNidP8BAHsCAAAAAhuVpgVRdOxkuC7wW2rvw4800OVxl+QCgezYKHtCYN7GAQAAAAD/////HPTH9wFgyf4iQ2xw4DIDP8t9IjCePWDjhqgs8fXvSIcAAAAAAP////8BIAMAAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBQAAAAAAAQEfAQAAAAAAAAAWABRsRdOvqHYghsS9dtinGsfJduGRlgEIawJHMEQCIATeBwStEFO5UXTVvf7oZb0r721WcWnrV3FEsq2pRI1BAiBOMY3nqXajbAKENLdXuKSPJgOJpNq54Lo0SppvyYcKIgEhArrDpkX9egpTfGJ6039faVBYxY0ZzrADPpE/Gpl14A3uAAEBH0gDAAAAAAAAFgAU1ZEJG4B0ojde2ZhanEsY7+z9QWUBCGsCRzBEAiA1dVYc0/PYalniefsvbPpgMqdvQMxG9MPQfYT/jL6s+AIgcV4q335UMp6SYtGdOCSWgIwpSSNGel/fq8UoGKwjM1MBIQKIO7VGPjfVUlLYs9XCFBsAezfIp9tiEfdclVrMXqMl6wAA",
  "complete": true
}
```
Obviously, you're going to need to save that `psbt` information using `jq`:
```
$ psbt_f=$(bitcoin-cli walletprocesspsbt $psbt | jq -r '.psbt')
```
You can see the `inputs` have now been filled in:
```
$ bitcoin-cli decodepsbt $psbt_f
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
      "witness_utxo": {
        "amount": 0.00000001,
        "scriptPubKey": {
          "asm": "0 6c45d3afa8762086c4bd76d8a71ac7c976e19196",
          "hex": "00146c45d3afa8762086c4bd76d8a71ac7c976e19196",
          "type": "witness_v0_keyhash",
          "address": "tb1qd3za8tagwcsgd39awmv2wxk8e9mwryvktqmkkg"
        }
      },
      "final_scriptwitness": [
        "3044022004de0704ad1053b95174d5bdfee865bd2bef6d567169eb577144b2ada9448d4102204e318de7a976a36c028434b757b8a48f260389a4dab9e0ba344a9a6fc9870a2201",
        "02bac3a645fd7a0a537c627ad37f5f695058c58d19ceb0033e913f1a9975e00dee"
      ]
    },
    {
      "witness_utxo": {
        "amount": 0.00000840,
        "scriptPubKey": {
          "asm": "0 d591091b8074a2375ed9985a9c4b18efecfd4165",
          "hex": "0014d591091b8074a2375ed9985a9c4b18efecfd4165",
          "type": "witness_v0_keyhash",
          "address": "tb1q6kgsjxuqwj3rwhkenpdfcjccalk06st9z0k0kh"
        }
      },
      "final_scriptwitness": [
        "304402203575561cd3f3d86a59e279fb2f6cfa6032a76f40cc46f4c3d07d84ff8cbeacf80220715e2adf7e54329e9262d19d382496808c294923467a5fdfabc52818ac23335301",
        "02883bb5463e37d55252d8b3d5c2141b007b37c8a7db6211f75c955acc5ea325eb"
      ]
    }
  ],
  "outputs": [
    {
    }
  ],
  "fee": 0.00000041
}
$ bitcoin-cli analyzepsbt $psbt_f
{
  "inputs": [
    {
      "has_utxo": true,
      "is_final": true,
      "next": "extractor"
    },
    {
      "has_utxo": true,
      "is_final": true,
      "next": "extractor"
    }
  ],
  "estimated_vsize": 177,
  "estimated_feerate": 0.00000231,
  "fee": 0.00000041,
  "next": "extractor"
}
```
Or to be more precise: (1) the PSBT has been updated with the `witness_utxo` information; (2) the PSBT has been signed; and (3) the PSBT has been finalized.

> :information_source: **NOTE:** If you'd chosen to Update the PSBT with `utxoupdatepsbt`, you'd still need to use `walletprocesspsbt` to Sign it: it's the only Signer-role command for PSBTs that's available in PSBT.

## Create a PSBT the Easy Way
#### PSBT Role: Creator, Updater

If you think that there should be a command that's the equivalent of `fundrawtransaction`, you'll be pleased to know there is: `walletcreatedfundedpsbt`. You could use it just the same as `createpsbt`:
```
$ bitcoin-cli -named walletcreatefundedpsbt inputs='''[ { "txid": "'$utxo_txid_1'", "vout": '$utxo_vout_1' }, { "txid": "'$utxo_txid_2'", "vout": '$utxo_vout_2' } ]''' outputs='''{ "'$recipient'": 0.000008 }'''
{
  "psbt": "cHNidP8BAOwCAAAABBuVpgVRdOxkuC7wW2rvw4800OVxl+QCgezYKHtCYN7GAQAAAAD/////HPTH9wFgyf4iQ2xw4DIDP8t9IjCePWDjhqgs8fXvSIcAAAAAAP////+F4znnstDchqSWKbMzVhfABTYrhWlrI5Dq5N+SafvZCgAAAAAA/v///+4XB6sA0qPJUrpZpHuDOVf6U5/45Gx+M/ktgJI0huFoAAAAAAD+////AiADAAAAAAAAFgAUx3LW+VVC4R7xHo78fHppgwrTigVAIxAAAAAAABYAFIXoteaWZc8dsEBQFhlsg63+L4AUAAAAAAABAR8BAAAAAAAAABYAFGxF06+odiCGxL122Kcax8l24ZGWIgYCusOmRf16ClN8YnrTf19pUFjFjRnOsAM+kT8amXXgDe4Q1gQ4AAAAAIABAACADgAAgAABAR9IAwAAAAAAABYAFNWRCRuAdKI3XtmYWpxLGO/s/UFlIgYCiDu1Rj431VJS2LPVwhQbAHs3yKfbYhH3XJVazF6jJesQ1gQ4AAAAAIABAACADAAAgAABAR8ThgEAAAAAABYAFNWRCRuAdKI3XtmYWpxLGO/s/UFlIgYCiDu1Rj431VJS2LPVwhQbAHs3yKfbYhH3XJVazF6jJesQ1gQ4AAAAAIABAACADAAAgAABAIwCAAAAAdVmsvkSBmfeHqNAe/wDCQ5lEp9F/587ftzCD1UL60nMAQAAABcWABRzFxRJfFPl8FJ6SxjAJzy3mCAMXf7///8CQEIPAAAAAAAZdqkUf0NzebzGbEB0XtwYkeprODDhl12IrMEwLQAAAAAAF6kU/d+kMX6XijmD+jWdUrLZlJUnH2iHPhQbACIGA+/e40wACf0XXzsgteWlUX/V0WdG8uY1tEYXra/q68OIENYEOAAAAACAAAAAgBIAAIAAACICAykwveCMRaAvXurm5dP7a1thD33hyGTaharyzLkhCnVRENYEOAAAAACAAQAAgBMAAIAA",
  "fee": 0.00042300,
  "changepos": 1
}
```
However, the big advantage is that you can use it to self-fund, just like `fundrawtransaction`. 
```
$ psbt_new=$(bitcoin-cli -named walletcreatefundedpsbt inputs='''[]''' outputs='''{ "'$recipient'": 0.000008 }''' | jq -r '.psbt')
$ bitcoin-cli decodepsbt $psbt_new
{
  "tx": {
    "txid": "53b63fe92abf334ec4018cee51f985c4c02dbda79ca5d72af4a0e1457d69c888",
    "hash": "53b63fe92abf334ec4018cee51f985c4c02dbda79ca5d72af4a0e1457d69c888",
    "version": 2,
    "size": 154,
    "vsize": 154,
    "weight": 616,
    "locktime": 0,
    "vin": [
      {
        "txid": "68e1863492802df9337e6ce4f89f53fa5739837ba459ba52c9a3d200ab0717ee",
        "vout": 0,
        "scriptSig": {
          "asm": "",
          "hex": ""
        },
        "sequence": 4294967294
      },
      {
        "txid": "8748eff5f12ca886e3603d9e30227dcb3f0332e0706c4322fec96001f7c7f41c",
        "vout": 0,
        "scriptSig": {
          "asm": "",
          "hex": ""
        },
        "sequence": 4294967294
      }
    ],
    "vout": [
      {
        "value": 0.00971240,
        "n": 0,
        "scriptPubKey": {
          "asm": "0 bbb0caabea5ef17717d3280af131a5cc61691b79",
          "hex": "0014bbb0caabea5ef17717d3280af131a5cc61691b79",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1qhwcv42l2tmchw97n9q90zvd9e3skjxmeqmpjq0"
          ]
        }
      },
      {
        "value": 0.00000800,
        "n": 1,
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
      "non_witness_utxo": {
        "txid": "68e1863492802df9337e6ce4f89f53fa5739837ba459ba52c9a3d200ab0717ee",
        "hash": "68e1863492802df9337e6ce4f89f53fa5739837ba459ba52c9a3d200ab0717ee",
        "version": 2,
        "size": 140,
        "vsize": 140,
        "weight": 560,
        "locktime": 1774654,
        "vin": [
          {
            "txid": "cc49eb0b550fc2dc7e3b9fff459f12650e0903fc7b40a31ede670612f9b266d5",
            "vout": 1,
            "scriptSig": {
              "asm": "0014731714497c53e5f0527a4b18c0273cb798200c5d",
              "hex": "160014731714497c53e5f0527a4b18c0273cb798200c5d"
            },
            "sequence": 4294967294
          }
        ],
        "vout": [
          {
            "value": 0.01000000,
            "n": 0,
            "scriptPubKey": {
              "asm": "OP_DUP OP_HASH160 7f437379bcc66c40745edc1891ea6b3830e1975d OP_EQUALVERIFY OP_CHECKSIG",
              "hex": "76a9147f437379bcc66c40745edc1891ea6b3830e1975d88ac",
              "reqSigs": 1,
              "type": "pubkeyhash",
              "addresses": [
                "ms7ruzvL4atCu77n47dStMb3of6iScS8kZ"
              ]
            }
          },
          {
            "value": 0.02961601,
            "n": 1,
            "scriptPubKey": {
              "asm": "OP_HASH160 fddfa4317e978a3983fa359d52b2d99495271f68 OP_EQUAL",
              "hex": "a914fddfa4317e978a3983fa359d52b2d99495271f6887",
              "reqSigs": 1,
              "type": "scripthash",
              "addresses": [
                "2NGParh82hE2Zif5PVK3AfLpYhfwF5FyRGr"
              ]
            }
          }
        ]
      },
      "bip32_derivs": [
        {
          "pubkey": "03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388",
          "master_fingerprint": "d6043800",
          "path": "m/0'/0'/18'"
        }
      ]
    },
    {
      "witness_utxo": {
        "amount": 0.00000840,
        "scriptPubKey": {
          "asm": "0 d591091b8074a2375ed9985a9c4b18efecfd4165",
          "hex": "0014d591091b8074a2375ed9985a9c4b18efecfd4165",
          "type": "witness_v0_keyhash",
          "address": "tb1q6kgsjxuqwj3rwhkenpdfcjccalk06st9z0k0kh"
        }
      },
      "bip32_derivs": [
        {
          "pubkey": "02883bb5463e37d55252d8b3d5c2141b007b37c8a7db6211f75c955acc5ea325eb",
          "master_fingerprint": "d6043800",
          "path": "m/0'/1'/12'"
        }
      ]
    }
  ],
  "outputs": [
    {
      "bip32_derivs": [
        {
          "pubkey": "02d856a663c46aece1e9d6eca3af2cbece0ceca617d72dd7f16c6e7383a0f63c2e",
          "master_fingerprint": "d6043800",
          "path": "m/0'/1'/22'"
        }
      ]
    },
    {
    }
  ],
  "fee": 0.00028800
}
$ bitcoin-cli analyzepsbt $psbt_new
{
  "inputs": [
    {
      "has_utxo": true,
      "is_final": false,
      "next": "signer",
      "missing": {
        "signatures": [
          "7f437379bcc66c40745edc1891ea6b3830e1975d"
        ]
      }
    },
    {
      "has_utxo": true,
      "is_final": false,
      "next": "signer",
      "missing": {
        "signatures": [
          "d591091b8074a2375ed9985a9c4b18efecfd4165"
        ]
      }
    }
  ],
  "estimated_vsize": 288,
  "estimated_feerate": 0.00100000,
  "fee": 0.00028800,
  "next": "signer"
}
```
As you can see it Created the PSBT, and then Updated it with all the information it could find locally.

From there, you need to use `walletprocesspsbt` to finalize it, as usual:
```
$ psbt_new_f=$(bitcoin-cli walletprocesspsbt $psbt | jq -r '.psbt')
```
Afterward, an analysis will show it's about ready to go too:
```
$ bitcoin-cli analyzepsbt $psbt_new_f
{
  "inputs": [
    {
      "has_utxo": true,
      "is_final": true,
      "next": "extractor"
    },
    {
      "has_utxo": true,
      "is_final": true,
      "next": "extractor"
    }
  ],
  "estimated_vsize": 177,
  "estimated_feerate": 0.00000231,
  "fee": 0.00000041,
  "next": "extractor"
}
```
Now would you realy want to use `walletcreatefundedpsbt` if you were creating a Bitcoin-CLI program? Probably not. But it's the same analysis as whether to use `fundrawtransaction`. Do you let Bitcoin Core do the analysis and calculation and decisions, or do you take that on yourself?

## Send a PSBT

## Summary: Creating a Partially Signed Bitcoin Transaction

## What's Next?

Continue "Expanding Bitcoin Transactions" with [§7.1: Using a Partially Signed Bitcoin Transaction](06_7_Using_a_Partially_Signed_Bitcoin_Transaction.md).
