# 7.1: Creating a Partially Signed Bitcoin Transaction

> :information_source: **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Partially Signed Bitcoin Transactions (PSBTs) are the newest way to vary the creation of basic Bitcoin transactions. They do so by introducing collaboration into every step of the process, allowing people (or programs) to not just authenticate transactions together (as with multisigs), but also to create, fund, authenticate, and broadcast collaboratively. 

> :warning: **VERSION WARNING:** This is an innovation from Bitcoin Core v 0.17.0. Earlier versions of Bitcoin Core will not be able to work with the PSBT while it is in process (though they will still be able to recognize the final transaction). Some updates and upgrades for PSBTs have continued through 0.20.0.

## Understand How PSBTs Work

Multisignatures were great for the very specific case of jointly holding funds and setting rules for who among the joint signers could OK the use of those funds. There are many use cases from a spousal joint bank account (a 1-of-2 signature) to a fiduciary requirement for dual control (a 2-of-2 signature) to an escrow (a 2-of-3 signature) and more. 

PSBTs may initially look sort of the same because they have a single overlapping bit of functionality, the ability to jointly sign a transaction, but they were created for a totally different use case. They recognize the need for multiple programs to jointly create a PSBT for a number of different reasons, and they provide a standardized format for doing so. They're especially useful for use cases involving hardware wallets (for which, see §7.3).

In the previous chapter, the mechanisms you learned for signing a multisig transactions and passing it between multiple recipients were very specific to `bitcoin-cli` (and somewhat haphazard, witness the "error" messages you received for a transaction not signed by sufficient signers). In this chapter, you can expect the PSBT methodologies to be more programatically standardized, as they were explicitly created to pass unfinished transactions between multiple parties.

> :book: ***What is a PSBT?*** As the name suggests, a PSBT is a transaction that has not been fully signed. That's important, because once a transaction is signed, its content is logged in. There's an abstracted methodology for putting them together that describes the collaboration (and standardizes it). A *Creator* proposes a transaction; one or more *Updaters* supplement it; and one or more *Signers* authenticate it; before a *Finalizer* and *Extracter* turn it into a transaction.

## Create a PSBT the Old-Fashioned Way

The easiest way to create a PSBT is to take an existing transaction and to use `converttopsbt` to turn it into a PSBT.


## Create a PSBT the Hard Way

## Create a PSBT the Easy Way

## Examine a PSBT

## Update a PSBT

## Sign a PSBT

## Summary: Creating a Partially Signed Bitcoin Transaction

## What's Next?

Continue "Expanding Bitcoin Transactions" with [§7.1: Using a Partially Signed Bitcoin Transaction](06_7_Using_a_Partially_Signed_Bitcoin_Transaction.md).
