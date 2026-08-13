# 17.1: Breaking Down Taproot

Taproot was a Bitcoin upgrade that activated on November 14, 2021 and
supports the new "Taproot" SegWit v1 address. However, it's much more
than that due to its incorporation of multiple Bitcoin Improvement
Proposals, which together create a complex network of new rules and
new capabilities.

## Learn about the Taproot Soft Fork

The Taproot activation simultaneously deployed three Bitcoin
Improvement Proposals: [BIP 340: Schnorr
Signatures](https://github.com/bitcoin/bips/blob/master/bip-0340.mediawiki),
[BIP 341:
Taproot](https://github.com/bitcoin/bips/blob/master/bip-0341.mediawiki),
and [BIP 342:
Tapscript](https://github.com/bitcoin/bips/blob/master/bip-0342.mediawiki). These
BIPs also incorporated ideas from older BIPs that have since been
closed, most notably [BIP 114:
MAST](https://github.com/bitcoin/bips/blob/master/bip-0114.mediawiki)
and its successors. Here's a bit more about what that all means.

### Understand Schnorr Signatures (BIP 340)

The first building block of Taproot is the Schnorr Signature.

Signatures are, of course, what unlock Bitcoin transactions. P2PKH and
PW2PKH UTXOs are locked with a public key, which is hashed to form the
receiving address of a transaction. A signature from the linked
private key then unlocks the transaction and allows it to be
spent. That signature was traditionally an ECDSA signature.

> 📖 ***What is ECDSA?** ECDSA is a version of the DSA signature
algorithm that uses elliptic curves. DSA is a NIST standard that is
secured with the discrete logarithm problem.

Taproot addresses are instead locked and unlocked with Schnorr
signatures. These signatures have long been seen as an improvement
over ECDSA and other traditional signature schemes particularly
because of their aggregatable signatures and their adapter signatures.
However, they were still under patent in the United States when
Bitcoin was released, and even afterward it took a while for their use
to be fully regularized. The Taproot update finally brought them into
Bitcoin.

More on Schnorr Signatures can be found in [§18.1: Understanding
Schnorr](18_1_Understanding_Schorr.md).]

### Understanding Taproot (BIP 341)

The Taproot BIP proper defines the Segwit v1 (P2TR) address. It uses
the aggregatability of Schnorr signatures to allow addresses to be
unlocked in one of two ways: via a key path (private key signature) or
via a script path (scripted spending conditions).

Script paths are embedded in a Merkle Tree, or a MAST, a concept that
was previously described in [BIP
114](https://github.com/bitcoin/bips/blob/master/bip-0114.mediawiki). A
MAST contains different scripts within a binary hash tree: each leaf
commits to a script with a hash. A Taproot address then commits to the
root of the hash tree as a "tweak" that is added to the Taproot
address thanks to the aggregatability of Schnorr.

> 📖 ***What is a Commitment?** When you hash data, you create a
representation of the data that doesn't reveal the data itself, but
can later be used to prove what the data is (and that you know
it). This is how P2SH and P2WSH work: the "Script Hash" is a
commitment to the script that can unlock the UTXO. With Taproot, an
address commits to a Merkle Tree of scripts using the Merkle Root
hash--which is applied as a tweak to the public key to form the
address.

When a Taproot UTXO is spent, the address can be unlocked with just
the private key, which checks the key path, or with multiple elements,
one of which is the script and another of which is a "control block"
(which combines the untweaked public key, a map to the script in the
MAST [if there is a Merkle tree], and a few other bits of
information), which allows spending through the script path.

The bottom line? Taproot allows an address to be spent using a typical
private key or one of a number of scripts. 

Any P2TR address (`bc1p` on mainnet or `tb1q` on testnet) is a Taproot
address that is locked and unlocked with Schnorr signatures. More on Taproot addresses
can be found in [§18.2: Understanding Taproot
Addresses](18_2_Creating_Taproot_Addresses.md).

More on MAST can be found in [§19.1: Understanding
MAST](19_1_Understanding_MAST.md).

### Understanding Tapscript (BIP 342)

The final element of the Taproot upgrade, BIP 342, defines the script
opcodes that can be used in the MAST. They are largely Bitcoin Script,
but with a few changes to which opcodes are available and and how
script execution and signature verification work.

More on Tapscript can be found in [§19.2: Designing
Tapscripts](19_2_Designing_Tapscripts.md).

## Summary: Breaking Down Taproot

The Taproot update comes in three parts:

* Schnorr signatures offer a new way to sign Bitcoin transactions.
* Taproot uses the power of Schnorr to merge key spends and script spends into a single address.
* Tapscript slightly varies Bitcoin Script for use with Taproot.

Each of these parts will receive additional discussion in later sections.

> 🔥 ***What is the power of Taproot?*** Taproot allows keys and a
huge multitude of scripts to be used interchangeably and privately
with a single Bitcoin address.

## What's Next?

Continue "Introducing Taproot" with [§17.2: Updating to
Taproot](17_2_Updating_to_Taproot.md).
