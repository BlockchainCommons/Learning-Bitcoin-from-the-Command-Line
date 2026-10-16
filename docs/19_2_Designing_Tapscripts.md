# 19.2 Designing Tapscripts

Tapscript is the language used in leaf scripts, which are scripts
placed in a Merkle Tree and incorporated into a Taproot address.  It's
mostly Bitcoin Script, with just a few changes.

## Understand the Differences of Tapscript

Tapscript is fully defined in [BIP
342](https://github.com/bitcoin/bips/blob/master/bip-0342.mediawiki#user-content-Specification). Its
rules only apply to Taproot script-path spends that have a leaf
version of `0xc0` (which is currently the case, but allows for changes
in the future).

It includes the following major variations to Bitcoin Script:

* `OP_CHECKSIG` and `OP_CHECKSIGVERIFY` expect Schnorr signatures, not ECDSA.
* `OP_CHECKMULTISIG` and `OP_CHECKMULTISIGVERIFY` are disabled.
* `OP_CHECKSIGADD` is added as a replacement for the multisig opcodes. It checks a single signatures and updates a threshold counter if successful. This supports batch verification.
* `OP_IF` and `OP_NOTIF` now require binary inputs (0 or 1).

## Understand the Differences in Multisig

Obviously, the big change is in multisigs.

[§12.4](2_4_Scripting_a_Multisig/) showed how multisigs were laid out
with traditional Bitcoin Script:

This is a 2-of-2 multisig Bitcoin Script:

```sh
2 $pubkey1 $pubkey2 2 OP_CHECKMULTISIG
```

This is the same 2-of-2 multisig in Tapscript:

```
$pubkey1 OP_CHECKSIG $pubkey2 OP_CHECKSIGADD 2 OP_NUMEQUAL
```

This is a 2-of-3 multisig in Bitcoin Script:

```sh
2 $pubkey1 $pubkey2 $pubkey3 3 OP_CHECKMULTISIG
```

This is the same 2-of-3 multisig in Tapscript:


```sh
$pubkey1 OP_CHECKSIG $pubkey2 OP_CHECKSIGADD $pubkey3 OP_CHECKSIGADD 2 OP_NUMEQUAL
```

Multisigs can also be rebuilt in Taproot by using multiple leaves of
the Merkle Tree. For example, an m-of-n threshold signature could be
built using a number of Merkle leaves that each check a different
m-of-m threshold. For example, if you wanted a 2-of-3 threshold
signature that allowed any two of ABC to sign, you could build leaves
for three 2-of-2 multisigs, covering AB, BC, and AC.  (This is the
methodology needed to turn MuSig2 into a threshold signature system.)
Alternatively, leaves could be built to check against aggregate public
keys, pushing the whole question of multisigs out to Schnorr.

But we're going to stick with the basic method of scripting with
`OP_CHECKSIGADD`, because that's the method built into Bitcoin
Core—and a method that fully reveals the power of integrating key
paths and script paths.

## Understand the tr() Descriptor

There's one more thing you should understand before you try to use
Taproot multisig in the next chapter: how the `tr()` descriptor works.
As described in [BIP
386](https://github.com/bitcoin/bips/blob/master/bip-0386.mediawiki),
a `tr()` descriptor comes in two forms: `tr(KEY)` and `tr(KEY,TREE)`.

`tr(KEY)` is a descriptor for a Taproot address that _only_ has a key
path spend. When you run `listdescriptors` and see the following, this
is a `tr(KEY)` descriptor:

```sh
tr([3f3521a6/86h/1h/0h]tpubDCW7vJKTJGE1e5WTX7mWDmafQmsEMpaVbqFkZih8uQympr8a1FZkw7paeNXkVgE28jdZd13aexRzuCzF3NfuzQNFgAqmSU9p1DDrYeRUUoE/0/*)#e7freggd
```

Similarly, when you created a MuSig2 descriptor in
[§18.3](18_3_Using_Bitcoin_With_MuSig2.md) with
`tr(musig($musigpubkey1,$musigpubkey2))`, that was a `tr(KEY)`
descriptor. Though the `musig()` function was there, it created a
single aggregated public key, not a script path.

`tr(KEY,PATH)` is a descriptor for a Taproot address that _also_ has a
script path spend. However, there's still a key path spend! Sometimes
that key path might be obvious, sometimes not. [BIP
341](https://github.com/bitcoin/bips/blob/master/bip-0341.mediawiki)
suggests the key path spend could be the most likely key to be used or
an aggregate of all the keys or a key path with no solution. However,
you have to put _something_ in when you're defining a taproot script
path with a descriptor (as we'll see next section).

## Summary: Understanding Tapscript

Tapscript is Bitcoin Script in Merkle Trees integrated through
Taproot. It has some slight variations to Bitcoin Script, the most
important are which are: requirements for binary in IF/NOTIF inputs;
and different methods to create multisigs. One of the ways to access
it is through a `tr(KEY,PATH)` descriptor.

## What's Next?

Continue "Using Tapscript" with [§19.3: Creating a Taproot
Multisig](19_3_Creating_a_Taproot_Multisig.md).

