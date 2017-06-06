 # 11.3: Empowering Bitcoin with Scripts

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Bitcoin Scripts can go far beyond the relatively simple financial instruments detailed to date. They're also the foundation of most more complex usages of the Bitcoin network, as demonstrated by these real-world examples of off-chain functionality, drawn from the Lightning Network examples in BIP 112.

## Lock for the Lightning Network

The [Lightning Network](https://rusty.ozlabs.org/?p=450) is a payment channel that allows users to take funds off-chain and engage in numerous microtransactions before finalizing the payment channel and bringing the funds back into Bitcoin. Benefits include lower fees and faster transaction speeds.

[BIP 112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki) contains a few examples of how transactions could be locked in the Lightning Network.

### Lock with Revocable Commitment Transactions

The trick with Lightning is that it's off-chain. The participants jointly lock funds on Bitcoin with an n-of-n multisignature, then they engage in a number of transactions between themselves. Each new "commitment transaction" splits those joint funds in a different way; it's partially signed but _it isn't put on the blockchain_. 

So how do you keep one of the participants from reverting back to an old transaction that's more beneficial to them? That's where revocation comes in, as is demonstrated in the following example from BIP 112, which was intended as a stepping stone toward Lightning. You give the participant who would be harmed by reversion to a revoked transaction the ability to reclaim the funds himself if the another participant illegitamately tried to use the revoked transaction.

For example, presume that Alice updated the commitment transaction to give more of the funds to Bob (effectively: she sent funds to Bob via the Lightning network). As part of this new transaction, she gives Bob a `revokeHash` which can be used to claim the funds from the previous transaction, before Alice gave Bob the new funds.

The locking script from that revoked commitment transaction looks as follows:
```
OP_HASH160 
<revokeHash> 
OP_EQUAL
    
IF
        
    <pubKeyBob>
    
ELSE

    <+24Hours> 
    OP_CHECKSEQUENCEVERIFY 
    OP_DROP
    <pubKeyAlice>
    
ENDIF
OP_CHECKSIG
```
Theoretically, this transaction should never be spent at the point. Bob has no incentive to because he has the newer transaction, after Alice sent him the new funds. Alice has no incentive too, because she loses the funds if she tries. So no one puts the transaction onto the blockchain, and the off-chain transactions continue.

But what if Alice tried to cheat? She puts the transaction back onto the blockchain even though it's been revoked. Now the locking script comes to the rescue!

#### Run the Lock Script for Cheating Alice, with Revocation Code

Alice could try to use revocation code that she gave to Bob to immediately claim the funds. She sends in `<sigAlice> <revokeCode>`:
```
Script: <sigAlice> <revokeCode> OP_HASH160 <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ ]

Script: OP_HASH160 <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ <sigAlice> <revokeCode> ]

Script: <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Running: <revokeCode> OP_HASH160 
Stack: [ <sigAlice> <revokeHash> ]

Script: OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ <sigAlice> <revokeHash> <revokeHash> ]

Script: IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Running: <revokeHash> <revokeHash> OP_EQUAL
Stack: [ <sigAlice> True ]
```
The `OP_EQUAL` feeds the `IF` statement. Because Alice uses the hash, she gets into the branch that allows her to redeem the funds immediately, collapsing the rest of the script down to `<pubKeyBob>` (within the conditional) and `OP_CHECKSIG` (afterward).
```
Script: <pubKeyBob> OP_CHECKSIG
Running: True IF
Stack: [ <sigAlice> ]
```
Curses! Only Bob can sign immediately with the hash!
```
Script:  OP_CHECKSIG
Stack: [ <sigAlice> <pubKeyBob> ]

Script:
Running: <sigAlice> <pubKeyBob> OP_CHECKSIG
Stack: [ False ]
```
#### Run the Lock Script for Cheating Alice, without Revocation Code

So what if Alice instead tries to use her own signature, without the revocation code?

```
Script: <sigAlice> 0 OP_HASH160 <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ ]

Script: OP_HASH160 <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ <sigAlice> 0 ]

Script: <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Running: 0 OP_HASH160 
Stack: [ <sigAlice> <0Hash> ]

Script: OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ <sigAlice> <0Hash> <revokeHash> ]

Script: IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Running: <0Hash> <revokeHash> OP_EQUAL
Stack: [ <sigAlice> False ]
```
We now collapse down to the `ELSE` statement and what comes after the conditional
```
Script: <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> OP_CHECKSIG
Running: False IF
Stack: [ <sigAlice> ]

Script: OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> OP_CHECKSIG
Stack: [ <sigAlice> <+24Hours> ]
```
And then Alice is foiled again because 24 hours haven't gone by!
```
Script: OP_DROP <pubKeyAlice> OP_CHECKSIG
Running: <+24Hours> OP_CHECKSEQUENCEVERIFY 
Stack: [ <sigAlice> <+24Hours> ] — Script EXITS
```
#### Run the Lock Script for Virtuous Bob

What this means is that Bob has 24 hours to reclaim his funds if Alice ever tries to cheat, using the `<revokeCode>` and his signature:

```
Script: <SigBob> <revokeCode> OP_HASH160 <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ ]

Script: OP_HASH160 <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ <SigBob> <revokeCode> ]

Script: <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Running: <revokeCode> OP_HASH160 
Stack: [ <SigBob> <revokeHash> ]

Script: OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ <SigBob> <revokeHash> <revokeHash> ]

Script: IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Running: <revokeHash> <revokeHash> OP_EQUAL
Stack: [ <SigBob> True ]

Script: <pubKeyBob> OP_CHECKSIG
Running: True IF
Stack: [ <SigBob> ]

Script:  OP_CHECKSIG
Stack: [ <SigBob> <pubKeyBob> ]

Script:
Running: <SigBob> <pubKeyBob> OP_CHECKSIG
Stack: [ True ]
```
#### Run the Lock Script for Virtuous Alice

All of the commitment scripts are locked with this same transaction, whether they've been revoked or not. That means that the newest commitment script, which is the currently valid one, is locked with it as well. In this situation Alice has never sent a new transaction to Bob and thus never sent him the `revokeCode`. She can now close down the Lightning channel at this point. She puts the transaction on the chain and she waits 24 hours. Bob can't do anything about it because he doesn't have the recovation code. Then, after the wait, Alice reclaims her funds. (Bob does the last thing with his last valid transaction.)

### Lock with Hashed Time-Lock Contracts

BIP 112 also offers a slightly more complex mechanism for protecting Lightning-like transactions: a [hashed timelock contract](https://en.bitcoin.it/wiki/Hashed_Timelock_Contracts), or HTLCs. This is what allows singular transactions to actually become a network and is what's actually used as the basis of the Lightning network.

#### Lock the Recipient's Transaction

Take as an example the following commitment transaction created for new funds that Alice has received:
```
OP_HASH160 
OP_DUP 
<secretHash> 
OP_EQUAL
    
IF

    <+24Hours>
    OP_CHECKSEQUENCEVERIFY
    OP_2DROP
    <pubKeyAlice>
    
ELSE

    <revokeHash> 
    OP_EQUAL
        
    OP_NOTIF
            
        <Date> 
        OP_CHECKLOCKTIMEVERIFY 
        OP_DROP
        
    ENDIF
        
    <pubKeyBob>
    
ENDIF
    
OP_CHECKSIG
```
The key to this is the `secretHash`, which is what allows a transaction to span the network. Each of several transactions is locked with the `secretHash`. When the transaction has spanned from its originator to its intended recipient, the `secretCode` is revealed, which allows all the participants to create a `secretHash` and unlock the whole network of payments: after the `secretCode` has been revealed, Alice can claim the funds 24 hours after the transaction is put on the Bitcoin network.

However, the hash could alternatively be a `revokeHash`, which was supplied after the transaction was supplanted by a new one. Bob can relcaim the funds in that situation _or if_ an absolute timeout has occurred.

#### Lock the Sender's Transaction

Due to the additional complexity of HTLCs, an additional commitment transaction is required for the sender of every HTLC transaction:
```
OP_HASH160 
OP_DUP 
<secretHash>
OP_EQUAL
OP_SWAP 
<revokeHash> 
OP_EQUAL 
OP_ADD
   
IF

    <pubKeyAlice>

ELSE

    <Date>
    OP_CHECKLOCKTIMEVERIFY
    <+24Hours>
    OP_CHECKSEQUENCEVERIFY
    OP_2DROP
    <pubKeyBob>

ENDIF
OP_CHECKSIG
```
The initial part of their Script is quite clever and so worth running:
```
Initial Script: <suppliedCode> OP_HASH160 OP_DUP <secretHash> OP_EQUAL OP_SWAP <revokeHash> OP_EQUAL OP_ADD
Stack: [ ]

Initial Script: OP_HASH160 OP_DUP <secretHash> OP_EQUAL OP_SWAP <revokeHash> OP_EQUAL OP_ADD
Stack: [ <suppliedCode> ]

Initial Script: OP_DUP <secretHash> OP_EQUAL OP_SWAP <revokeHash> OP_EQUAL OP_ADD
Running: <suppliedCode> OP_HASH160 
Stack: [ <suppliedHash> ]

Initial Script: <secretHash> OP_EQUAL OP_SWAP <revokeHash> OP_EQUAL OP_ADD
Running: <suppliedHash> OP_DUP 
Stack: [ <suppliedHash> <suppliedHash> ]

Initial Script: OP_EQUAL OP_SWAP <revokeHash> OP_EQUAL OP_ADD
Running: <suppliedHash> OP_DUP 
Stack: [ <suppliedHash> <suppliedHash> <secretHash> ]

Initial Script: OP_EQUAL OP_SWAP <revokeHash> OP_EQUAL OP_ADD
Running: <suppliedHash> OP_DUP 
Stack: [ <suppliedHash> <suppliedHash> <secretHash> ]

Initial Script: OP_SWAP <revokeHash> OP_EQUAL OP_ADD
Running: <suppliedHash> <secretHash> OP_EQUAL
Stack: [ <suppliedHash> <wasItSecretHash?> ]

Initial Script: <revokeHash> OP_EQUAL OP_ADD
Running: <suppliedHash> <wasItSecretHash?> OP_SWAP
Stack: [ <wasItSecretHash?> <suppliedHash> ]

Initial Script: OP_EQUAL OP_ADD
Stack: [ <wasItSecretHash?> <suppliedHash> <revokeHash> ]

Initial Script: OP_ADD
Running: <suppliedHash> <revokeHash> OP_EQUAL
Stack: [ <wasItSecretHash?> <wasItRevokeHash?> ]

Initial Script: OP_ADD
Running: <suppliedHash> <revokeHash> OP_EQUAL
Stack: [ <wasItSecretHash?> <wasItRevokeHash?> ]

Initial Script: 
Running: <wasItSecretHash?> <wasItRevokeHash?> OP_ADD
Stack: [ <wasItSecretOrRevokeHash?> ]
```
Running through the script, it becomes obvious that the initial checks determine if the hash was either the `secretCode` or the `revokeCode`. If so, Alice can take the funds in the first block. If not, Bob can take the funds, but only after Alice has had her chance, and both the 24 hour timeout and the absolute timeout have passed.

#### Understand HTLCs

HTLCs are quite complex, and you may not entirely understand them from just this overview. Rusty Russell's [overview](https://rusty.ozlabs.org/?p=462) of them has more and there's even more in his [Deployable Lightning](https://github.com/ElementsProject/lightning/blob/master/doc/deployable-lightning.pdf) paper. 

For the purposes of this tutorial, two things are important for HTLCs:

   * Understand that a very complex structure like an HTLC can be created with Bitcoin Script.
   * Analyze how to run each of the HTLC scripts.
   
It's worth your time running each of the HTLC scripts through each of its permutations, one stack item at a time.

### Summary: Empowering Bitcoin with Scripts

We're closing our examination of Bitcoin Scripts with a look at how truly powerful they can be. In 20 opcodes or less, a Bitcoin Script can form the basis of an entire off-chain payment channel. Similarly, two-way pegged sidechains are the product of less than twenty opcodes, as briefly noted in [BIP 112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki).

If you've ever seen complex Bitcoin functionality or Bitcoin-adjacent systems, they were problem built on Bitcoin Scripts. And now you have all the tools to do the same yourself.
