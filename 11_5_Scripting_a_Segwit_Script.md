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
More importantly, there's a `redeemScript`, which decodes to `OP_0 OP_PUSHDATA (20 bytes) 3ab2a09a1a5f2feb6c799b5ab345069a96e1a0a`. This should look familiar, because it's an `OP_0` followed by 20-byte hexcode of a public key hash. In other words, a P2SH-SegWit is just a SegWit `scriptPubKey` jammed into a script. That's all there is to it. It precisely matches how modern multisigs are a multsig placed inside a P2SH, as discussed in [§10.4: Scripting a Multisig](10_4_Scripting_a_Multisig.md).

Conversely, when we spend this transaction, it looks exactly like a P2SH:
```
$ bitcoin-cli getrawtransaction ed752673bfd4338ccf0995983086da846ad652ae0f28280baf87f9fd44b3c45f 1
{
  "txid": "ed752673bfd4338ccf0995983086da846ad652ae0f28280baf87f9fd44b3c45f",
  "hash": "aa4b1c2bde86ea446c9a9db2f77e27421316f26a8d88869f5b195f03b1ac4f23",
  "version": 2,
  "size": 247,
  "vsize": 166,
  "weight": 661,
  "locktime": 1781316,
  "vin": [
    {
      "txid": "59178b02cfcbdee51742a4b2658df35b63b51115a53cf802bc6674fd94fa593a",
      "vout": 1,
      "scriptSig": {
        "asm": "00149ef51fb1f5adb44e20eff758d34ae64fa781fa4f",
        "hex": "1600149ef51fb1f5adb44e20eff758d34ae64fa781fa4f"
      },
      "txinwitness": [
        "3044022069a23fcfc421b44c622d93b7639a2152f941dbfd031970b8cef69e6f8e97bd46022026cb801f38a1313cf32a8685749546a5825b1c332ee4409db82f9dc85d99086401",
        "030aec1384ae0ef264718b8efc1ef4318c513403d849ea8466ef2e4acb3c5ccce6"
      ],
      "sequence": 4294967294
    }
  ],
  "vout": [
    {
      "value": 8.49029534,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_HASH160 b4b656f4c4b14ee0d098299d1d6eb42d2e22adcd OP_EQUAL",
        "hex": "a914b4b656f4c4b14ee0d098299d1d6eb42d2e22adcd87",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2N9ik3zihJ91VGNF55sZFe9GiCAXh2cVKKW"
        ]
      }
    },
    {
      "value": 0.00095000,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_HASH160 ee7aceea0865a05a29a28d379cf438ac5b6cd9c6 OP_EQUAL",
        "hex": "a914ee7aceea0865a05a29a28d379cf438ac5b6cd9c687",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2NEzBvokxh4ME4ahdT18NuSSoYvvhS7EnMU"
        ]
      }
    }
  ],
  "hex": "020000000001013a59fa94fd7466bc02f83ca51511b5635bf38d65b2a44217e5decbcf028b175901000000171600149ef51fb1f5adb44e20eff758d34ae64fa781fa4ffeffffff029e299b320000000017a914b4b656f4c4b14ee0d098299d1d6eb42d2e22adcd87187301000000000017a914ee7aceea0865a05a29a28d379cf438ac5b6cd9c68702473044022069a23fcfc421b44c622d93b7639a2152f941dbfd031970b8cef69e6f8e97bd46022026cb801f38a1313cf32a8685749546a5825b1c332ee4409db82f9dc85d9908640121030aec1384ae0ef264718b8efc1ef4318c513403d849ea8466ef2e4acb3c5ccce6442e1b00",
  "blockhash": "0000000069cbe44925fab2d472870608c7e1e241a1590fd78be10c63388ed6ee",
  "confirmations": 282952,
  "time": 1595360859,
  "blocktime": 1595360859
}
```
Each `vout` is of the form `OP_HASH160 <HASH> OP_EQUAL`. That's a normal P2SH per [§10.2](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/82ca897286aac612804ae849b260750229fa3a52/10_2_Building_the_Structure_of_P2SH.md), which means that it's only when the redeem script is run that the magic occurs. Just as with a P2WPKH, an old node wil see `OP_0 OP_PUSHDATA (20 bytes) 3ab2a09a1a5f2feb6c799b5ab345069a96e1a0a` in the redeem script and verify it automatically, while a new node will see that, know it's a P2WPKH, and so go out to the `witnesses`. See [§9.5: Scripting a P2WPKH](09_5_Scripting_a_P2WPKH.md).

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

## Summary: Scripting a Segwit Script

There are two sorts of P2SH scripts that relate to Segwit. 

The P2SH-Segwit address is a nested Segwit address that embed the simple Segwit `scriptPubkey` inside a Script, just like multisigs are embedded in scripts nowadays: the Segwit-style key is unwound, and then parsed like normal on a machine that understands Segwit. The purpose is backward compatibility to old nodes that might not otherwise be able to send to native Segwit addresses.

The P2WSH address is a Segwit variant of P2SH, just as P2WPKH is a Segwit variant of P2WSH. It works with the same logic, and is identified by having a 32-byte hash instead of a 20-byte hash. The purpose is to extend the advantages of Segwit to other sorts of scripts.

## What's Next?

Continue "Embedding Bitcoin Scripts" with [§10.6: Spending a P2SH Transaction](10_6_Spending_a_P2SH_Transaction.md).
