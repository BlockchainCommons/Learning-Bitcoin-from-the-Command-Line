# 18.3: Using Bitcoin with FROST

As demonstrated in the previous section, signing with Schnorr is
easy. But what abouting signing Schnorr multisigs? There are at the
moment three main ways to do so: FROST and MuSig2 can be seamlessly
integrated into a Bitcoin transaction (but aren't seamlessly
integrated into Bitcoin Core). Instead, Bitcoin Core depends on its
Merkle trees for multisigs. This section and the next cover FROST and
MuSig2 (in limited ways), while the next chapter includes discusison
of MAST multisigs.

## Understand FROST

Bitcoin multisigs can't be naively created using Schnorr. If you just
aggregate two keys, it's possible to cancel out one of the keys
(provided you *know* the public key you're cancelling) and then sign
with the other key. To resolve this requires a more complex
protocols. One of these is FROST.

FROST, which stands for Flexible Round-Optimized Schnorr Threshold
Signatures is a threshold signature that allows for m-of-n Schnorr
multisigs without succumbation to the cancellation attack. It does
this through "signing shares" that can either be generated on a single
machine and sent out or (more securely) generated using secure
multi-party computation (SMPC) so that the full key never exists in a
single place.

## Use FROST

Any Bitcoin wallet can create a Taproot address that is built on a
FROST-generated key rather than a singular Schnorr key. That's the
beauty of Schnorr: a multisig FROST key looks exactly the same as a
singular Schnorr key. They're indistinguishable, and so they look just
the same to a wallet.

The trick comes in signing so that you can unlock the puzzle box of
the FROST lock. A wallet _could_ support this functionality, but
Bitcoin Core does not. In order to unlock funds locked into an address
defined by a FROST public key, you therefore have to generate the
appropriate signature _outside_ of Bitcoin Core and then sneak it into
a Bitcoin transaction. This _is_ possible, but only with quite a few
additional tools.

Our [Learning FROST from the Command
Line](https://learningfrost.blockchaincommons.com/) course is a
four-chapter course that's largely focused on that end product:
signing a transaction. Early chapters demonstrate how to install the
ZF FROST tools, which are some of the best tools to date to do this
work from the command line, and how to create signature shares among
the members of a "FROST group". [Learning FROST
§4.2](https://learningfrost.blockchaincommons.com/04_2_Signing_PSBTs_with_FROST/)
then shows the precise methodology for signing with FROST, once you
have everything installed and built.

What follows are just previous outlines to explain the methodology;
consulting [Learning FROST from the Command
Line](https://learningfrost.blockchaincommons.com/) for all the
specifics.

### Create a FROST Address

Creating a FROST address is a simple creation of a FROST group
followed by implementation of the techniques that you've previously
used to work with descriptor-based content.

1. Choose the "n" members of a FROST group.
2. Choose what the "m" threshold will be.
3. Use the FROST protocol to generate individual signing shares and an aggregate public key.
4. Generate a descriptor by pairing "tr(aggregate public key)" with a generated checksum, per the technique in [§4.2](04_2_Integrating_Addresses_Descriptors/).
5. Use `deriveaddresses` to generate one or more addresses for the descriptor.

### Spend a FROST Address

Spending from a FROST address is the part that's not currently
well-supported by Bitcoin Core.

1. Import the tr(...) descriptor you created into a new watch-only wallet.
2. Generate a PSBT to spend funds.
3. Extract the sighash from PSBT using a special tool.
4. Use a FROST tool such as ZF FROST to create a threshold signature of the sighash.
5. Use a special tool to place the signature on the UTXO for the FROST address with the proper `sighash` flag.
6. Finalize and send th EPSBT.

Again, [Learning FROST from the Command
Line](https://learningfrost.blockchaincommons.com/) contains all the
tools you'll need for this process.

> 📖 ***What is a Sighash?*** When Bitcoin signs a transaction, it
actually signs a hash of the transaction: the sighash. Each
transaction has multiple inputs (where funds are coming from) and
multiple outputs (where funds are going to). Whether a sighash
contains all, some, or none of the inputs and all, some, or none of
the outputs is determined by a specific sighash flag, which is applied
to each signature.

## Summary: Using Bitcoin with FROST

Bitcoin Core doesn't directly support FROST signatures. But, the
aggregatable nature of FROST signatures, which results in all of them
looking the same, means that it will correctly process a FROST
signature if you give it one. You can create a FROST address with
"tr(public-key)", where the public key is a FROST aggregated key; you
can generate an address from that descriptor as usual; and then you
can spend funds by signing with the appropriate signing shares and
inserting that signature into a transaction.

## What's Next?

Continue "Using Schnorr" with [§18.4: Using Bitcoin With MuSig2](18_4_Using_Bitcoin_With_MuSig2.md).

