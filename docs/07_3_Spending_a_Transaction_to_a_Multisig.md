# 7.3: Spending a Transaction with a Multisig

You've created a multisig and you've even imported it into a multisig
wallet, but how do you spend it? It turns out that's a slightly
difficult question in `bitcoin-cli` as it currently stands, which is
going to require us to turn the page to the next chapter to find the
best solution.

## The Classic Methods

Traditionally, `bitcoin-cli` offered two methodologies for spending multisigs:

1. You created a transaction by hand; each user dumped their private keys (`dumpprivkey`) for their addresses; and then each signed the transaction (`signrawtransactionwithkey`) while also adding in the `redeemScript` by hand; or
2. You passed a transaction around by hand and had each user sign it with their wallet (`signrawtransactionwithwallet`) while also adding on the `redeemScript`.

In both techniques, the general workflow was the same: create a transaction; pass the transaction around to each signer; have each signer individually sign the transaction on their own machine; and then send it to the Bitcoin network when sufficient people has signed. This workflow is crucial to multisig spending and something that we'll meet again in the next chapter (albeit, with somewhat more structure).

However, we won't be using it with these specific techniques.

The first technique is no longer available because the advent of
descriptor wallets has taken away the ability to dump keys for
individual addresses. (You could still sign with a key if you managed
to access a key, but that's not particularly possible in `bitcoin-cli`
at the moment, so this is mainly vestigial functionality.)

The second technique theoretically still works, but it's become very
[complex to
use](https://bitcoin.stackexchange.com/questions/130639/signing-a-bech32-multisig-with-bitcoin-core?noredirect=1#comment146403_130639)
because of the way the witness scripts work in Segwit. Bitcoin experts
["strongly
recommend"](https://bitcoin.stackexchange.com/questions/130639/signing-a-bech32-multisig-with-bitcoin-core?noredirect=1#comment146403_130639)
that you don't.

As a result, we're not using either of these techniques in this
course, but instead point them out simply as methods that you might
use in your own wallets that you develop. (And we're maintaining this
section in case Bitcoin Core decides to restore some of this
functionality in the future.)

Fortunately, there's a different method for spending multisigs that's not only fairly simple, but which also was purposefully built to enable multiple people to work on a Bitcoin transaction: it's called the Partially Signed Bitcoin Transaction, or PSBT, and is the topic of the whole next chapter.

## Summary: Spending a Transaction with a Multisig

The methodology for spending multisig transactions has changed over
time. Where it once upon a time was supported with a specific set of
commands, now it's instead a part of a larger technique called
PSBTs. Even if Bitcoin Core were to restore some of its classic
multisig functionality, PSBTs are the best-practice way to spend
multisigs, because they're a large, interoperable specification. Which
is what we're talking about next.

## What's Next?

Take the next step in "Expanding Bitcoin Transactions" in [Chapter
Eight: Expanding Bitcoin Transactions with
PSBTs](08_0_Expanding_Bitcoin_Transactions_PSBTs.md).
