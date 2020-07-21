# 10.5: Scripting a Segwit Script

> :information_source: **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Segwit introduced a number of new options for address (and thus scripting) types. [§9.5: Scripting a P2WPKH](09_5_Scripting_a_P2WPKH.md) explained how the new Bech32 address type varied the standard scripts found in most traditional transactions. This chapter looks at the two other sorts of scripts introduced by the Segwit upgrade: the P2SH-Segwit (which was the transitional "nested Segwit" address, as Segwit came into usage) and P2WSH (which is the Segwit equivalent of the P2SH address, just like P2WPKH is the Segwit equivalent of the P2PKH address).

This is another situation where you won't really have to worry about these nuances while working with `bitcoin-cli`, but it's useful to know how it all works.

## Understand a P2SH-Segwit Script

The P2SH-Segwit address is a dying breed. It was basically a stopgap measure while Bitcoin was transitioning to Segwit that allowed a user to create a Segwit address and then have someone with a non-Segwit-enabled exchange or wallet fund that address. 

But if you ever need to use one, there's an option to create one using `getnewaddress`:
```
$ bitcoin-cli getnewaddress -addresstype p2sh-segwit
2NEzBvokxh4ME4ahdT18NuSSoYvvhS7EnMU
```
The address starts with a `2` (or a `3`) revealing it as a script

When you look at a UTXO for that address, you can see the `desc` is different, revealing a WPKH address wrapped in a script:
```
$ bitcoin-cli listunspent
  {
    "txid": "ed752673bfd4338ccf0995983086da846ad652ae0f28280baf87f9fd44b3c45f",
    "vout": 1,
    "address": "2NEzBvokxh4ME4ahdT18NuSSoYvvhS7EnMU",
    "redeemScript": "001443ab2a09a1a5f2feb6c799b5ab345069a96e1a0a",
    "scriptPubKey": "a914ee7aceea0865a05a29a28d379cf438ac5b6cd9c687",
    "amount": 0.00095000,
    "confirmations": 1,
    "spendable": true,
    "solvable": true,
    "desc": "sh(wpkh([f004311c/0'/0'/3']03bb469e961e9a9cd4c23db8442d640d9b0b11702dc0126462ac9eb88b64a4dd48))#p29e839h",
    "safe": true
  }
```
The `redeemScript` here is particularly interesting, because it's just `OP_0 OP_PUSH20 3ab2a09a1a5f2feb6c799b5ab345069a96e1a0a`.

You can then look at the raw transaction:
```
$ hex=$(bitcoin-cli gettransaction "bb4362dec15e67d366088f5493c789f22fb4a604e767dae1f6a631687e2784aa" | jq -r '.hex')
$ bitcoin-cli decoderawtransaction $hex
{
  "txid": "bb4362dec15e67d366088f5493c789f22fb4a604e767dae1f6a631687e2784aa",
  "hash": "6866490b16a92d68179e1cf04380fd08f16ec80bf66469af8d5e78ae624ff202",
  "version": 2,
  "size": 249,
  "vsize": 168,
  "weight": 669,
  "locktime": 1780788,
  "vin": [
    {
      "txid": "4779bb137ddbcaf796f905e264554b1ec28c0f3ab4538ca02eac5099bfc3fc1e",
      "vout": 0,
      "scriptSig": {
        "asm": "0014c4ea10874ae77d957e170bd43f2ee828a8e3bc71",
        "hex": "160014c4ea10874ae77d957e170bd43f2ee828a8e3bc71"
      },
      "txinwitness": [
        "3044022025ee4fd38e6865125f7c315406c0b3a8139d482e3be333727d38868baa656d3d02204b35d9b5812cb85894541da611d5cec14c374ae7a7b8ba14bb44495747b5715301",
        "033cae26cb3fa063c95e2c55a94bd04ab9cf173104555efe448b1bfc3a68c8f873"
      ],
      "sequence": 4294967294
    }
  ],
  "vout": [
    {
      "value": 0.00095000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 41d83eaffbf80f82dee4c152de59a38ffd0b6021 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a91441d83eaffbf80f82dee4c152de59a38ffd0b602188ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "mmX7GUoXq2wVcbnrnFJrGKsGR14fXiGbD9"
        ]
      }
    },
    {
      "value": 0.01063793,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_HASH160 b780fc2e945bea71b9ee2d8d2901f00914a25fbd OP_EQUAL",
        "hex": "a914b780fc2e945bea71b9ee2d8d2901f00914a25fbd87",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2N9yWARt5E3TQsX2RjsauxSZaEZVhinAS4h"
        ]
      }
    }
  ]
}
```
The script pubKey is then `"OP_DUP OP_HASH160 41d83eaffbf80f82dee4c152de59a38ffd0b6021 OP_EQUALVERIFY OP_CHECKSIG"`. In other words, this is just a standard P2SH address! Which is important, because an old exchange or wallet might not know how to send to a Bech32 address, but it can send to a P2SH address.

The node where you try to spend that UTXO, however, must be up-to-date. Here's what the signed transaction that spends the UTXO looks like:
```
$ bitcoin-cli decoderawtransaction $signtx
{
  "txid": "2d8c306071c1801b97541b654f2f2af9d3941fc1aab95626fb3cf2a12d7563ba",
  "hash": "acac8706b69bd6099f4748d6b2545a0d2d0487e2cfaef58eb67e08ffa97bed07",
  "version": 2,
  "size": 214,
  "vsize": 133,
  "weight": 529,
  "locktime": 0,
  "vin": [
    {
      "txid": "ed752673bfd4338ccf0995983086da846ad652ae0f28280baf87f9fd44b3c45f",
      "vout": 1,
      "scriptSig": {
        "asm": "001443ab2a09a1a5f2feb6c799b5ab345069a96e1a0a",
        "hex": "16001443ab2a09a1a5f2feb6c799b5ab345069a96e1a0a"
      },
      "txinwitness": [
        "3044022067210ddd31c35312ed6c1fb073978df8e0fc5121e75fc2df46dbc2e8bc6c1c8b02204d5da4da46257ffe5857e3e57813dd25ed6e423ceef263eade6f0a8b72117c2001",
        "03bb469e961e9a9cd4c23db8442d640d9b0b11702dc0126462ac9eb88b64a4dd48"
      ],
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 0.00090000,
      "n": 0,
      "scriptPubKey": {
        "asm": "0 f54449dc190e1824086af17560f7d7528e189823",
        "hex": "0014f54449dc190e1824086af17560f7d7528e189823",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1q74zynhqepcvzgzr2796kpa7h228p3xprxxrdlr"
        ]
      }
    }
  ]
}
```
It's got a simple `asm` of 

[example]
[how to spend]

## Understand a P2WSH Script

Contrariwise, the P2WSH transactions should be ever-increasing in usage, since they're the native Segwit replacement for P2SH, offering all the same advantages of blocksize that were created with native Segwit P2WPKH transactions.


## Summary: Scripting a Pay to Witness Public Key Hash

To a large extent you _don't_ script a P2WPKH. Instead, Bitcoin Core creates the transaction in a different way, placing the witness information in a different place rather than a traditional `scriptSig`. That means that P2WPKHs are a digression from the Bitcoin Scripts of this part of the book, because they're an expansion of Bitcoin that steps away from traditional Scripting.

However, SegWit was also a clever usage of Bitcoin Scripts. Knowing that there would be nodes that didn't upgrade and needing to stay backward compatible, the developers created the P2WPKH format so that it generated a script that always validated on old nodes (while still having that script provide information to new nodes in the form of a version number and a hashed public key).

When you're programming from the command line, you fundamentally don't have to worry about this, other than knowing that you won't find traditional scripts in raw SegWit transactions (which, again, was the point).

## What's Next?

Continue "Embedding Bitcoin Scripts" with [§10.6: Spending a P2SH Transaction](10_6_Spending_a_P2SH_Transaction.md).
