# 12.1: Understanding the Foundation of P2SH & P2WSH

You know that Bitcoin Scripts can be used to control the redemption of
UTXOs. The next step is creating Scripts of your own ... but that
requires a very specific technique.

## Know the Bitcoin Standards

Here's the gotcha for using Bitcoin Scripts: for security reasons,
most Bitcoin nodes will only accept "standard" Bitcoin transactions:
the deprecated P2PK and P2MS, the older P2PKH and P2SH, the unspendable
OP_RETURN, the anyone-can-spend P2WPKH and P2WSH, and the
we-haven't-met-it-yet P2TR. If anything other than the well-understood
sets of opcodes that define these addresses appears as a
`scriptPubKey`, it won't be broadcast.

For the various ways to pay to public keys (or do nothing), the
`scriptPubKey` must be one of the following:

* __Pay to Public Key (P2PK)__ — `<pubKey> OP_CHECKSIG`
* __Pay to Multisig (P2MS)__ — `<m> n<pubKeys> <n> OP_CHECKMULTISIG`
* __Pay to Public Key Hash (P2PKH)__ — `OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG`
* __Pay to Witness Public Key Hash (P2WPKH)__ — `OP_0 <pubKeyHash>`
* __Null Data__ — `OP_RETURN Data`
* __Pay to Taproot (P2TR)__ - `OP_1 <pubKeyTweaked>` (we'll get to this)

So how do you write a more complex Bitcoin Script? The answer to that
lies in two of the final sorts of standard transactions: P2SH and
P2WSH.  You can put any sort of long and complex script into these
transactions, but the scripts are then hashed, and that hash is
compared as part of the `scriptPubKey` sequence. It's a clever way to
simultaneously allow arbitrary scripts but also keep things
standardized.

## Understand the P2SH Script

For pay-to-script addresses, we're going to follow the same sequence
we used to examine public key payments: we're going to start out by
looking at the standard P2SH script, then look at how P2WSH has
extended and varied that.

The first step in looking at a P2SH address is creating one. Though
scripts can get quite complex, this it's easy to create a reference
P2SH script because Bitcoin Core recognizes two standard addresses
that are created as P2SHes: multisigs and the transitionary
P2SH-Segwit address.

We're going to create a P2SH multisig address by gathering public
keys, then using a descriptor to input them, using the same technique
as in [§7.2](07_2_Creating_Multisig_Descriptors.md):

```
echo $msdescwithcs

| sh(sortedmulti(1,028e10b07f53498fe64b248fc820c88ebf4ae55be328125bea2f68c88b2668b2e2,034b1cd6a8a75d5875bd7d1cec28b4c105c82729181902bd75051418606c88e6fa))#v3lcnns7
```

We're going to place that in a special `multi-sh` wallet and then fund that address:
```
bitcoin-cli -rpcwallet="multi-sh" importdescriptors '''[{ "desc": "'$msdescwithcs'", "timestamp": 1780329126 }]'''

$ bitcoin-cli -rpcwallet="multi-sh" getaddressesbylabel ""
{
  "2Mwn9cDed35LbNoQtJhucjaap1UFAtbQnfm": {
    "purpose": "receive"
  }
}
```

We fund it so that we can examine the `scriptPubKey` (locking script)
when those funds arrive at the address:

```
      "scriptPubKey": {
        "asm": "OP_HASH160 31b9bdc91eae21a3abbe68c8f71c4b6a3b5cc772 OP_EQUAL",
        "desc": "addr(2Mwn9cDed35LbNoQtJhucjaap1UFAtbQnfm)#0kv5wxm4",
        "hex": "a91431b9bdc91eae21a3abbe68c8f71c4b6a3b5cc77287",
        "address": "2Mwn9cDed35LbNoQtJhucjaap1UFAtbQnfm",
        "type": "scripthash"
      }
```

The locking script appears to be quite simple: `OP_HASH160
31b9bdc91eae21a3abbe68c8f71c4b6a3b5cc772 OP_EQUAL`. As usual, there's
a big chunk of data in the middle. But this time, it's not the hash of
a public key, but instead the hash of another, hidden locking script
(`redeemScript`) that will only be revealed when the funds are
redeemed. In other words, the standard locking script for a P2SH
address is: `OP_HASH160 <redeemScriptHash> OP_EQUAL`.

> 📖 ***What is a redeemScript?*** Each P2SH or P2WSH transaction
carries the fingerprint of a hidden locking script within it as a
hash. When a P2SH transaction is redeemed, the full (unhashed)
`redeemScript` is included as part of the `scriptSig`. Bitcoin will
make sure the `redeemScript` matches the hash; then it actually runs
the `redeemScript` to see if the funds can be spent (or not).

One of the interesting elements of P2SH transactions is that neither
the sender nor the Blockchain actually knows what the `redeemScript`
is! A sender just sends to a standardized P2SH address marked with a
"2" prefix and they don't worry about how the recipient is going to
retrieve the funds at the end.

## Understand How to Build a P2SH Script

Since the visible locking script for a P2SH transaction is so simple,
creating a transaction of this sort is quite simple too. In
theory. All you need to do is create a transaction whose locking
script includes a 20-byte hash of the `redeemScript`. That hashing (at
least for P2SH) is done with Bitcoin's standard `OP_HASH160`.

> 📖 ***What is OP_HASH160?*** The standard hash operation for Bitcoin
performs a SHA-256 hash, then a RIPEMD-160 hash. It's now considered
somewhat legacy, but continues to be used in P2PKH and P2WPKH scripts
(for public key hashes), and in P2SH scripts (for older script
hashes).

Overall, five steps are required to create the hash for a P2SH address:

1. Create an arbitrary locking script with Bitcoin Script.
2. Create a serialized version of that locking script.
3. Translate the serialization to binary.
4. Perform a SHA-256 hash on those serialized bytes.
5. Perform a RIPEMD-160 hash on the results of that SHA-256 hash.

Each of those steps of course takes some work on its own, and some of
them can be pretty intricate. The good news is that you don't really
have to worry about them, because they're sufficiently complex that
you'll usually have an API take care of it all for you.

So for now, we'll just provide you with an overview, so that you
understand the general methodology. In [§12.2: Building the Structure
of P2SH & P2WSH](12_2_Building_the_Structure_of_P2WSH.md) we'll
provide a more in-depth look at script creation, in case you ever want
to understand the guts of this process.

## Understand How to Send a P2SH Script Transaction

So how do you actually send your P2SH transaction? Again, the theory
is very simple:

1. Embed your hash in a `OP_HASH160 <redeemScriptHash> OP_EQUAL` script.
2. Translate that into hexcode.
3. Use that hex as your `scriptPubKey`. 
4. Create the rest of the transaction.

Unfortunately, this is another place where you're going to need to
fall back to APIs, in large part because `bitcoin-cli` doesn't provide
any support for creating P2SH transactions. (It can redeem them just
fine.)

## Understand How to Unlock a P2SH Script Transaction

The trick to redeeming a P2SH transaction is that the recipient must
have saved the secret serialized locking script that was hashed to
create the P2SH address. This is called a `redeemScript` because it's
what the recipient needs to redeem his funds.

An unlocking `scriptSig` for a P2SH transaction is formed as:
`... data ... <redeemScript>`. The `data` must _solely_ be data that
is pushed onto the stack, not operators. ([BIP
16](https://github.com/bitcoin/bips/blob/master/bip-0016.mediawiki)
calls them signatures, but that's not an actual requirement.)

> ⚠️ ***Signatures are the Safest.** Though signatures are not a
requirement, a P2SH script actually isn't very secure if it doesn't
require at least one signature in its inputs. The reasons for this are
described in [§15.1: Writing Puzzle
Scripts](15_1_Writing_Puzzle_Scripts.md).

When a UTXO is redeemed, it runs in two rounds of verification:

1. First, the `redeemScript` in the `scriptSig` is hashed and compared to the hashed script in the `scriptPubKey`. (That's `OP_HASH160 <redeemScriptHash> OP_EQUAL` for P2SH.) 
2. If they match, then a second round of verification begins.
3. Now, the `redeemScript` is run using the prior data that was pushed on the stack. 
4. If that second round of verification _also_ succeeds, the UTXO is unlocked.

Whereas you can't easily create a P2SH transaction without an API, you
should be able to easily redeem a P2SH or P2WSH transaction with
`bitcoin-cli`. In fact, you already did when you used a PSBT to spend
a multisig in [§8.1](08_1_Spending_a_Multisig_with_a_PSBT.md).  The
exact process is described in [§12.5: Spending a P2SH or P2WSH
Transaction](12_5_Spending_a_P2WSH_Transaction.md), after we've
finished with all the intricacies of P2SH transaction creation.

> ⚠️ **Scripts Can Be Invalid!** You can create a perfectly valid
transaction with a correcly hashed redeemScript, but if the
redeemScript doesn't run, or doesn't run correctly, your funds are
lost forever. That's why it is so important to test your Scripts, as
discussed in [§11.3: Testing a Bitcoin
Script](11_3_Testing_a_Bitcoin_Script.md).

## Understand the P2WSH Script

So far we've talked about P2SH scripts. That's for the same reason
that we first looked at the Bitcoin Scripting of P2PKH addresses,
before expanding to P2PWPKH. The fundamental scripting is more
apparent in the old addresses, before the `scriptSig` was sent off to
the witness area and the actual script was hidden in the code.

Here's an identical multisig made using P2WSH instead of P2SH:

```
echo $msdescwithcs
wsh(sortedmulti(1,028e10b07f53498fe64b248fc820c88ebf4ae55be328125bea2f68c88b2668b2e2,034b1cd6a8a75d5875bd7d1cec28b4c105c82729181902bd75051418606c88e6fa))#756kxqyu
```

Its `scriptPubKey` replaces the `OP_HASH160` and `OP_EQUAL` with a
simple `OP_0` followed by the script hash:

```
      "scriptPubKey": {
        "asm": "0 ac21648dd4d385911583e9d3f4b8c702733f12389a0f2dc117cc140e9b925c5f",
        "desc": "addr(tb1q4sskfrw56wzez9vra8flfwx8qfen7y3cng8jmsghes2qaxujt30shnqhyp)#j22cz2jw",
        "hex": "0020ac21648dd4d385911583e9d3f4b8c702733f12389a0f2dc117cc140e9b925c5f",
        "address": "tb1q4sskfrw56wzez9vra8flfwx8qfen7y3cng8jmsghes2qaxujt30shnqhyp",
        "type": "witness_v0_scripthash"
      }
```

This was once more done to create an anyone-can-spend transaction for
older hosts. Anyone up-to-date with SegWit knows that the `OP_0` means
it should be interpreted as a SegWit v0 and then verifies the hash
matches the redeem script supplied, as usual. The catch is that it
doesn't uses RIPEMD-160 for the comparison, but instead a simpler
SHA-256 hash.

Ultimately, it doesn't matter if those commands are run because of the
explicit `scriptPubKey` or the implicit understanding of SegWit. In
both cases, scripts are run the same way: the hash is checked (via
RIPEMD-160 for P2SH and SHA-256 for P2WSH) and then the script is run
against additional arguments.

## Summary: Understanding the Foundation of P2WSH

Arbitrary Bitcoin Scripts are non-standard in Bitcoin. However, you
can incorporate them into standard transactions by using the P2SH or
P2WSH address type. You just hash your script as part of the locking
script, then you reveal and run it as part of the unlocking script. As
long as you can also satisfy the `redeemScript`, the UTXO can be
spent.

> 🔥 ***What is the power of P2SH & P2WSH?*** You already know the
power of Bitcoin Script, which allows you to create more complex Smart
Contracts of all sorts. P2SH and P2WSH are what actually unleashes
that power by letting you include arbitrary Bitcoin Script in standard
Bitcoin transactions.

## What's Next?

Continue "Embedding Bitcoin Scripts" with [§12.2: Building the
Structure of P2SH & P2WSH](12_2_Building_the_Structure_of_P2WSH.md).
