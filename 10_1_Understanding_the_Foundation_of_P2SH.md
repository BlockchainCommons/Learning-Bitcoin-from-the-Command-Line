# 10.1: Understanding the Foundation of P2SH

You know that Bitcoin Scripts can be used to control the redemption of UTXOs. The next step is creating Scripts of your own ... but that requires a very specific technique.

## Know the Bitcoin Standards

Here's the gotcha for using Bitcoin Scripts: for security reasons, most Bitcoin nodes will only accept six types of "standard" Bitcoin transactions.

* __Pay to Public Key (P2PK)__ — An older, deprecated transaction (`<pubKey> OP_CHECKSIG`) that has been replaced by the better security of P2PKH.
* __Pay to Public Key Hash (P2PKH)__ — A standard transaction (`OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG`) that pays to the hash of a public key.
* __Pay to Witness Public Key hash (P2WPKH)__ — The newest sort of public-key transaction. It's just (`OP_0 <pubKeyHash>`) because it depends on miner consensus to work, as described in [§9.5](09_5_Scripting_a_P2WPKH.md).
* __Multisig__ — A transaction for a group of keys, as explained more fully in [§10.4](10_4_Scripting_a_Multisig.md).
* __Null Data__ — An unspendable transaction (`OP_RETURN Data`).
* __Pay to Script Hash (P2SH)__ — A transaction that pays out to a specific script, as explained more fully here.

So how do you write a more complex Bitcoin Script? The answer is in that last sort of standard transaction, the P2SH. You can put any sort of long and complex script into a P2SH transaction, and as long as you follow the standard rules for embedding your script and for redeeming the funds, you'll get the full benefits of Bitcoin Scripting.

> :warning: **VERSION WARNING:** Arbitrary P2SH scripts only became standard as of Bitcoin Core 0.10.0. Before that, only P2SH Multisigs were allowed.

## Understand the P2SH Script

You already saw a P2SH transaction when you created a multisig in [§6.1: Sending a Transaction to a Multisig](06_1_Sending_a_Transaction_to_a_Multisig.md). Though multisig is one of the standard transaction types, `bitcoin-cli` simplifies the usage of its multisigs by embedding them into P2SH transactions, as described more fully in [§10.4: Scripting a Multisig](10_4_Scripting_a_Multisig.md).

So, let's look one more time at the `scriptPubKey` of that P2SH multisig:
```
      "scriptPubKey": {
        "asm": "OP_HASH160 a5d106eb8ee51b23cf60d8bd98bc285695f233f3 OP_EQUAL",
        "hex": "a914a5d106eb8ee51b23cf60d8bd98bc285695f233f387",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr"
        ]
      }
```
The locking script is quite simple looking: `OP_HASH160 a5d106eb8ee51b23cf60d8bd98bc285695f233f3 OP_EQUAL`. As usual, there's a big chunk of data in the middle. This is a hash of another, hidden locking script (`redeemScript`) that will only be revealed when the funds are redeemed. In other words, the standard locking script for a P2SH address is: `OP_HASH160 <redeemScriptHash> OP_EQUAL`.

> :book: ***What is a redeemScript?*** Each P2SH transaction carries the fingerprint of a hidden locking script within it as a 20-byte hash. When a P2SH transaction is redeemed, the full (unhashed) `redeemScript` is included as part of the `scriptSig`. Bitcoin will make sure the `redeemScript` matches the hash; then it actually runs the `redeemScript` to see if the funds can be spent (or not).

One of the interesting elements of P2SH transactions is that neither the sender nor the Blockchain actually knows what the `redeemScript` is! A sender just sends to a standardized P2SH address marked with a "2" prefix and they don't worry about how the recipient is going to retrieve the funds at the end.

> :link: **TESTNET vs MAINNET:** on testnet, the prefix for P2SH addresses is `2`, while on mainnet, it's `3`.

## Understand How to Build a P2SH Script

Since the visible locking script for a P2SH transaction is so simple, creating a transaction of this sort is quite simple too. In theory. All you need to do is create a transaction whose locking script includes a 20-byte hash of the `redeemScript`. That hashing is done with Bitcoin's standard `OP_HASH160`.

> :book: ***What is OP_HASH160?*** The standard hash operation for Bitcoin performs a SHA-256 hash, then a RIPEMD-160 hash.

Overall, four steps are required:

1. Create an arbitrary locking script with Bitcoin Script.
2. Create a serialized version of that locking script.
3. Perform a SHA-256 hash on those serialized bytes.
4. Perform a RIPEMD-160 hash on the results of that SHA-256 hash.

Each of those steps of course takes some work on its own, and some of them can be pretty intricate. The good news is that you don't really have to worry about them, because they're sufficiently complex that you'll usually have an API take care of it all for you. 

So for now, we'll just provide you with an overview, so that you understand the general methodology. In [§10.2: Building the Structure of P2SH](10_2_Building_the_Structure_of_P2SH.md) we'll provide a more in-depth look at script creation, in case you ever want to understand the guts of this process.

## Understand How to Send a P2SH Script Transaction

So how do you actually send your P2SH transaction? Again, the theory is very simple:

1. Embed your hash in a `OP_HASH160 <redeemScriptHash> OP_EQUAL` script.
2. Translate that into hexcode.
3. Use that hex as your `scriptPubKey`. 
4. Create the rest of the transaction.

Unfortunately, this is another place where you're going to need to fall back to APIs, in large part because `bitcoin-cli` doesn't provide any support for creating P2SH transactions. (It can redeem them just fine.)

## Understand How to Unlock a P2SH Script Transaction

The trick to redeeming a P2SH transaction is that the recipient must have saved the secret serialized locking script that was hashed to create the P2SH address. This is called a `redeemScript` because it's what the recipient needs to redeem his funds. 

An unlocking `scriptSig` for a P2SH transaction is formed as: `... data ... <redeemScript>`. The `data` must _solely_ be data that is pushed onto the stack, not operators. ([BIP 16](https://github.com/bitcoin/bips/blob/master/bip-0016.mediawiki) calls them signatures, but that's not an actual requirement.)

> :warning: **WARNING:** Though signatures are not a requirement, a P2SH script actually isn't very secure if it doesn't require at least one signature in its inputs. The reasons for this are described in [§13.1: Writing Puzzle Scripts](13_1_Writing_Puzzle_Scripts.md).

When a UTXO is redeemed, it runs in two rounds of verification:

1. First, the `redeemScript` in the `scriptSig` is hashed and compared to the hashed script in the `scriptPubKey`. 
2. If they match, then a second round of verification begins.
3. Second, the `redeemScript` is run using the prior data that was pushed on the stack. 
4. If that second round of verification _also_ succeeds, the UTXO is unlocked.

Whereas you can't easily create a P2SH transaction without an API, you should be able to easily redeem a P2SH transaction with `bitcoin-cli`. In fact, you already did in [§6.2: Sending a Transaction to a Multisig](06_2_Spending_a_Transaction_to_a_Multisig.md). The exact process is described in [§10.6: Spending a P2SH Transaction](10_6_Spending_a_P2SH_Transaction.md), after we've finished with all the intricacies of P2SH transaction creation.

> :warning: **WARNING:** You can create a perfectly valid transaction with a correcly hashed redeemScript, but if the redeemScript doesn't run, or doesn't run correctly, your funds are lost forever. That's why it is so important to test your Scripts, as discussed in [§9.3: Testing a Bitcoin Script](09_3_Testing_a_Bitcoin_Script.md).

## Summary: Understanding the Foundation of P2SH

Arbitrary Bitcoin Scripts are non-standard in Bitcoin. However, you can incorporate them into standard transactions by using the P2SH address type. You just hash your script as part of the locking script, then you reveal and run it as part of the unlocking script. As long as you can also satisfy the `redeemScript`, the UTXO can be spent. 

> :fire: ***What is the power of P2SH?*** You already know the power of Bitcoin Script, which allows you to create more complex Smart Contracts of all sorts. P2SH is what actually unleashes that power by letting you include arbitrary Bitcoin Script in standard Bitcoin transactions.

## What's Next?

Continue "Embedding Bitcoin Scripts" with [§10.2: Building the Structure of P2SH](10_2_Building_the_Structure_of_P2SH.md).
