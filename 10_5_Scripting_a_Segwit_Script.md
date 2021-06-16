# 10.5: Scripting a Segwit Script

> :information_source: **NOTE:** This section has been recently added to the course and is an early draft that may still be awaiting review. Caveat reader.

Segwit introduced a number of new options for address (and thus scripting) types. [§9.5: Scripting a P2WPKH](09_5_Scripting_a_P2WPKH.md) explained how the new Bech32 address type varied the standard scripts found in most traditional transactions. This chapter looks at the three other sorts of scripts introduced by the Segwit upgrade: the P2SH-Segwit (which was the transitional "nested Segwit" address, as Segwit came into usage), the P2WSH (which is the Segwit equivalent of the P2SH address, just like P2WPKH is the Segwit equivalent of the P2PKH address), and the nested P2WSH address.

This is another situation where you won't really have to worry about these nuances while working with `bitcoin-cli`, but it's useful to know how it all works.

## Understand a P2SH-Segwit Script

The P2SH-Segwit address is a dying breed. It was basically a stopgap measure while Bitcoin was transitioning to Segwit that allowed a user to create a Segwit address and then have someone with a non-Segwit-enabled exchange or wallet fund that address. 

If you ever need to use one, there's an option to create a P2SH-Segwit address using `getnewaddress`:
```
$ bitcoin-cli getnewaddress -addresstype p2sh-segwit
2NEzBvokxh4ME4ahdT18NuSSoYvvhS7EnMU
```
The address starts with a `2` (or a `3`) revealing it as a script

> :book: ***Why can't old nodes send to native Segwit addresses?*** [§10.1](10_1_Understanding_the_Foundation_of_P2SH.md) noted that there were a set number of "standard" Bitcoin transactions. You can't actually lock a transaction with a script that isn't one of those standard types. Segwit is now recognized as one of those standards, but an old node won't know that, and so it will refuse to send on such a transaction for the protection of the sender. Wrapping a Segwit address inside a standard script hash resolves the problem.

When you look at a UTXO sent to that address, you can see the `desc` is different, revealing a WPKH address wrapped in a script:
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
More importantly, there's a `redeemScript`, which decodes to `OP_0 OP_PUSHDATA (20 bytes) 3ab2a09a1a5f2feb6c799b5ab345069a96e1a0a`. The should look familiar, because it's an `OP_0` followed by 20-byte hexcode of a public key hash. In other words, a P2SH-SegWit is just a SegWit `scriptPubKey` jammed into a script. That's all there is to it. It precisely matches how modern multisigs are a multsig jammed into a P2SH, as discussed in [§10.4: Scripting a Multisig](10_4_Scripting_a_Multisig.md).

The raw transaction reveals a bit more when you look at the `vout` `1`:
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
This confirms that this is just a normal P2SH, locked by `"OP_DUP OP_HASH160 41d83eaffbf80f82dee4c152de59a38ffd0b6021 OP_EQUALVERIFY OP_CHECKSIG"`. It's when the redeem script is run that the magic occurs. Just as with a P2WPKH, an old node wil see `OP_0 OP_PUSHDATA (20 bytes) 3ab2a09a1a5f2feb6c799b5ab345069a96e1a0a` and verify it automatically, while a new node will see that, know it's a P2WPKH, and so go out to the `witnesses`. See [§9.5: Scripting a P2WPKH](09_5_Scripting_a_P2WPKH.md).

> :book: ***What are the disadvantages of nested Segwit transactions?*** They're bigger than native Segwit transactions, so you get some of advantages of Segwit, but not all of them.

## Understand a P2WSH Script

Contrariwise, the P2WSH transactions should be ever-increasing in usage, since they're the native Segwit replacement for P2SH, offering all the same advantages of blocksize that were created with native Segwit P2WPKH transactions.

This is example of P2WSH address:
[https://blockstream.info/testnet/address/tb1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3q0sl5k7](https://blockstream.info/testnet/address/tb1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3q0sl5k7)

The details show that a UTXO sent to this address is locked with a `scriptPubKey` like this:
```
OP_0 OP_PUSHDATA (32 bytes) 1863143c14c5166804bd19203356da136c985678cd4d27a1b8c6329604903262
```
This works just like a P2WPKH address, the only difference being that instead of a 20-byte public-key-hash, the UTXO includes a 32-byte script-hash. Just as with a P2WPKH, old nodes just verify this, while new nodes recognize this is a P2WSH and so internally verify the script as described in previous sections, but using the `witness` data, which now includes the redeem script.

There is also one more variant, a P2WSH script embedded in a P2SH script, which works much like the P2SH-Segwit described above, but for nested P2WSH scripts. (Whew!)

## Summary: Scripting a Pay to Witness Public Key Hash

There are two sorts of P2SH scripts that relate to Segwit. 

The P2SH-Segwit address is a nested Segwit address that embed the simple Segwit `scriptPubkey` inside a Script, just like multisigs are embedded in scripts nowadays: the Segwit-style key is unwound, and then parsed like normal on a machine that understands Segwit. The purpose is backward compatibility to old nodes that might not otherwise be able to send to native Segwit addresses.

The P2WSH address is a Segwit variant of P2SH, just as P2WPKH is a Segwit variant of P2WSH. It works with the same logic, and is identified by having a 32-byte hash instead of a 20-byte hash. The purpose is to extend the advantages of Segwit to other sorts of scripts.

## What's Next?

Continue "Embedding Bitcoin Scripts" with [§10.6: Spending a P2SH Transaction](10_6_Spending_a_P2SH_Transaction.md).
