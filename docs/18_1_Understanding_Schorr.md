# 18.1: Understanding Schnorr Signatures

Schnorr signatures are a gamechanger for Bitcoin. Here's a bit about what makes them uniquely useful.

## Understand the Foundation of Schnorr Signatures

Mathematically, Schnorr signatures take advantage of finite fields
(originally, Schnorr groups, but in Bitcoin, an elliptic curve) to
empower the discrete logarithm problem. It's easy to get from a
private key to a public key, but all but impossible to go the opposite
direction, with is obviously the characteristic required for
public-key cryptography (and so signature systems) to work.

But the math doesn't really matter to your understanding of why
Schnorr signatures are a big upgrade for Bitcoin. That largely comes
done to another singular characteristic of Schnorr signatures: because
of their linearity, they're simply aggregatable.

### Understand Aggregatable Multisig

If you have two public keys, pA and pB, and you use the corresponding
private keys to make two signatures, SA and sB, the two signatures can
be added together (sA+sB) and they can be validated by adding together
the two public keys (pA+pB). The summed-up signature is the same size
as a singular signature, and the summed-up public key is the same size
as a singular public key. There's also no way to distinguish a
summed-up key or signature from a singular key or
signature. Advantages of this include:

* **Static Size.** ECDSA multisigs got bigger the more signatures you
added, but a Schnorr signature always stays the same size, even if you
have hundreds or thousands of signers.
* **Privacy.** No one can see that you're using a multisig rather than
a single signature.

There are some challenges to using Schnorr for multisigs. In
particular, simplistic ("naive") aggregation of signatures leads to
the key cancellation attack, where a key can be removed from an
aggregate if an attacker knows certain information. This has resulted
in more complex signing systems such as FROST (discussed in
[§18.3](18_3_Using_Bitcoin_With_FROST.md)), MuSig2 (discussed in
[§18.4](18_4_Using_Bitcoin_With_MuSig2.md)), and use of a Merkle tree
(discussed in [§19.3](19_3_Creating_a_Schnorr_Multisig.md)) being
required to access Schnorr's advantages.

### Understand Tweaked Signatures

Because a Schnorr signature's equation is linear it can modified in
other ways than just adding Schnorr signatures together. In
particular, you can add a "tweak" to it, which is a static value. You
tweak the public key (and so the address) with a specific value. You
then tweak the private key with the same value before signing, and the
tweaked signature matches the tweaked public key.

This is how Taproot addresses are constructed: the public key is
tweaked with the Merkle root hash to commit to the script path; or if
there is no Merkle tree, it's instead tweaked with an unspendable
opcode.

### Understand Adaptor Signatures

Finally, Schnorr signature's aggregatability also supports the creator
of adaptor signatures. These allow multiple parties to commit to
"tweaks" that are revealed simultaneously, allowing for the creation
of CoinJoin protocols, atomic swaps, and other trustless exchanges.

(They also go beyond the scope of this course currently, as they're
less well-used to date than aggregated multisigs or tweaked
signatures.)

## Know the Advantages of Schnorr

Besides aggregatability (and the efficiency and privacy that it brings),
Schnorr signatures have a number of different advantages over
ECDSA. These include:

* **Smaller.** 64 bytes instead of 70-72.
* **Batch Verification.** Multiple signatures can be verified at once.
* **Not Malleable.** ECDSA signatures could be changed, Schnorr cannot.

## Summary: Understanding Schnorr Signatures

Schnorr signatures are a big improvement over ECDSA in large part
because of their linearity, which makes them trivially
aggregatable. This allows for the creation of static-sized, private
multisigs and also is used to support the creation of Taproot addresses.

> 🔥 ***What is the power of Schnorr?*** In large part, it's
aggregatability. Any two signatures look the same, whether they're
actually singular sigs, a bunch signatures added together, or a
signature with a tweak to define something else (such as a script
path).

## What's Next?

Continue "Using Schnorr" with [§18.2: Creating Taproot Addresses]
(18_2_Creating_Taproot_Addresses.md).

