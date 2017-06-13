# 8.1: Understanding the Foundation of P2SH

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

You know that Bitcoin Scripts can be used to control the redemption of UTXOs. The next step is creating Scripts of your own ... but that requires a very specific techniques.

## Know the Bitcoin Standards

Here's the gotcha for using Bitcoin Scripts: for security reasons, most Bitcoin nodes will only accept five types of "standard" Bitcoin transactions.

* __Pay to Public Key (P2PK)__ — An older, deprecated transaction (`<pubKey> OP_CHECKSIG`) that has been replaced by the better security of P2PKH.
* __Pay to Public Key Hash (P2PKH)__ — A standard transaction (`OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG`) that pays to the hash of a public key.
* __Multisig__ — A transaction for a group of keys, as explained more fully in [§8.4](08_4_Scripting_a_Multisig.md).
* __Null Data__ — An unspendable transaction (`OP_RETURN Data`).
* __Pay to Script Hash (P2SH)__ — A transaction that pays out to a specific script, as explained more fully here.

So how do you write a more complex Bitcoin Script? The answer is in that last sort of standard transaction, the P2SH. You can put any sort of long and complex script into a P2SH transaction, and as long as you follow the standard rules for embedding your script and for redeeming the funds, you'll get the full benefits of Bitcoin Scripting.

> **VERSION WARNING:** Arbitrary P2SH scripts only became standard as of Bitcoin Core 0.10.0. Before that, only P2SH Multisigs were allowed.

## Understand the P2SH Script

You already saw a P2SH transaction when you created a multisig in [§6.1: Sending a Transaction to a Multisig](06_1_Sending_a_Transaction_to_a_Multisig.md). Though multisig is one of the standard transaction types, `bitcoin-cli` simplifies the usage of its multisigs by embedding them into P2SH transactions, as described more fully in [§8.4: Scripting a Multisig](08_4_Scripting_a_Multisig.md).

So, let's look one more time at the `scriptPubKey` of that P2SH multisig:
```
      "scriptPubKey": {
        "asm": "OP_HASH160 babf9063cee8ab6e9334f95f6d4e9148d0e551c2 OP_EQUAL",
        "hex": "a914babf9063cee8ab6e9334f95f6d4e9148d0e551c287",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2NAGfA4nW6nrZkD5je8tSiAcYB9xL2xYMCz"
        ]
      }
```
The locking script is quite simple looking: `OP_HASH160 babf9063cee8ab6e9334f95f6d4e9148d0e551c2 OP_EQUAL`. As usual, there's a big chunk of data in the middle. This is a hash of another, hidden locking script (`redeemScript`) that will only be revealed when the funds are redeemed. In other words, the standard locking script for a P2SH address is: `OP_HASH160 <redeemScriptHash> OP_EQUAL`.

_What is a redeemScript?_ Each P2SH transaction carries the fingerprint of a hidden locking script within it as a 20-byte hash. When a P2SH transaction is redeemed, the full (unhashed) `redeemScript` is included as part of the `scriptSig`. Bitcoin will make sure the `redeemScript` matches the hash; then it actually runs the `redeemScript` to see if the funds can be spent (or not).

One of the interesting elements of P2SH transactions is that neither the sender nor the Blockchain actually knows what the `redeemScript` is! A sender just sends to a standardized P2SH addressesd marked with a "2" prefix and they don't worry about how the recipient is going to retrieve the funds at the end.

> **TESTNET vs MAINNET:** Reminder: on testnet, the prefix for P2SH addresses is `2`, while on mainnet, it's `3`.

## Understand How to Build a P2SH Script

Since the visible locking script for a P2SH transaction is so simple, creating a transaction of this sort is quite simple too. In theory. All you need to do is create a transaction whose locking script includes a 20-byte hash of the `redeemScript`. That hashing is done with Bitcoin's standard `OP_HASH160`.

_What is OP_HASH160?_ The standard hash operation for Bitcoin performs a SHA-256 hash, then a RIPEMD-160 hash.

Overall, four steps are required:

1. Create an arbitrary locking script with Bitcoin Script.
2. Create a serialized version of that locking script.
3. Perform a SHA-256 hash on those serialized bytes.
4. Perform a RIPEMD-160 hash on the results of that SHA-256 hash.

Each of those steps of course takes some work on its own, and some of them can be pretty intricate. The good news is that you don't really have to worry about them, because they're sufficiently complex that you'll usually have an API take care of it all for you. 

So for now, we'll just provide you with an overview, so that you understand the general methodology. In [§8.2: Building the Structure of P2SH](08_2_Building_the_Structure_of_P2SH.md) we'll provide a more in-depth look at script creation, in case you ever want to understand the guts of this process.

## Understand How to Send a P2SH Script Transaction

So how do you actually send your P2SH transaction? Again, the theory is very simple:

1. Embed your hash in a `OP_HASH160 <redeemScriptHash> OP_EQUAL` script.
2. Translate that into hexcode.
3. Use that hex as your `scriptPubKey`. 
4. Create the rest of the transaction.

Unfortunately, this is another place where you're going to need to fall back to APIs, in large part because `bitcoin-cli` doesn't provide any support for sending P2SH transactions.

## Understand How to Unlock a P2SH Script Transaction

The trick to redeeming a P2SH transaction is that the recipient must have saved the secret serialized locking script that was hashed to create the P2SH address. This is called a `redeemScript` because it's what the recipient needs to redeem his funds. 

An unlocking `scriptSig` for a P2SH transaction is formed as: `... data ... <redeemScript>`. The `data` must _solely_ be data that is pushed onto the stack, not operators. ([BIP 16](https://github.com/bitcoin/bips/blob/master/bip-0016.mediawiki) calls them signatures, but that's not an actual requirement.)

> **WARNING:** Though signatures are not a requirement, a P2SH script actually isn't very secure if it doesn't require at least one signature in its inputs. The reasons for this are described in [§11.1: Writing Puzzle Scripts](11_1_Writing_Puzzle_Scripts.md).

When a UTXO is redeemed, it runs in two rounds of verification:

1. First, the redeemScript in the `scriptSig` is hashed and compared to the hashed script in the `scriptPubKey`. 
2. If they match, then a second round of verification begins.
3. Second, the redeemScript is run using the prior data that was pushed on the stack. 
4. If that second round of verification _also_ succeeds, the UTXO is unlocked.

Whereas you can't easily create a P2SH transaction without an API, you should be able to easily redeem a P2SH transaction with `bitcoin-cli`. In fact, you already have. The exact process is described in [§8.5: Spending a P2SH Transaction.md](08_5_Spending_a_P2SH_Transaction.md), after we've finished with all the intricacies of P2SH transaction creation.

> **WARNING:** You can create a perfectly valid transaction with a hashed redeemScript, but if the redeemScript doesn't run, or doesn't run correctly, your funds are lost forever. That's why it is so important to test your Scripts, as discussed in [§7.4: Testing a Bitcoin Script](07_4_Testing_a_Bitcoin_Script.md)

## Summary: Understanding the Foundation of P2SH

Arbitrary Bitcoin Scripts are non-standard in Bitcoin. However, you can incorporate them into standard transactions by using the P2SH address type. You just hash your script as part of the locking script, then you reveal and run it as part of the unlocking script. As long as you can also satisfy the `redeemScript`, the UTXO can be spent. 

_What is the power of P2SH?_ You already know the power of Bitcoin Script, which allows you to create more complex Smart Contracts of all sorts. P2SH is what actually unleashes that power by letting you include arbitrary Bitcoin Script in standard Bitcoin transactions.

## What's Next?

Continue "Embedding Bitcoin Scripts" with [§8.2: Building the Structure of P2SH](08_2_Building_the_Structure_of_P2SH.md).
