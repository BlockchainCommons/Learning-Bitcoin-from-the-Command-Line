> **WARNING:** This chapter is awaiting the writing of chapter 12+

# 8.5: Spending a P2SH Transaction

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Before we close out this overview of P2SH transactions, we're going to touch upon how to spend them.

## Use the Redeem Script

As we saw in [§6.2: Spending a Transaction to a Multisig](6_2_Spending_a_Transaction_to_a_Multisig.md), spending a P2SH transaction is all about having that serialized version of the locking script, the so-called _redeemScript_. So, the first step in being able to spend a P2SH transaction is making sure that you save the _redeemScript_ before you give out the P2SH address to everyone. 

### Collect Your Variables

Because P2SH addresses aren't integrated into `bitcoin-cli` there will be no short-cuts for P2SH spending like you saw in [§6.3: Sending an Automated Multisig](6_3_Sending_an_Automated_Multisig.md). You're going to need to collect all the more complex variables on your own!

[[PENDING]]

[[Notes: Example of saving redeemScript, recipient, keys, and scriptPubKey.]]

## Create the Transaction

[[PENDING]]

[Notes: I *think* this is just the same as what we did in chapter 6.2, but I need to actually receive a P2SH transaction to test it, and that requires first getting to the APIs, then using one to send myself a P2SH transaction, then writing up how to respend that.]

## Summary: Spending a Transaction with a Bitcoin Script

[[PENDING]]

[Notes: Say it's just like respending that multisig, the hard way]
