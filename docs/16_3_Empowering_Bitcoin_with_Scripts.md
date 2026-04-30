 # 13.3: Empowering Bitcoin with Scripts

Bitcoin Scripts can go far beyond the relatively simple financial instruments detailed to date. They're also the foundation of most  complex usages of the Bitcoin network, as demonstrated by these real-world examples of off-chain functionality, drawn from the Lightning Network examples in [BIP 112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki).

## Lock for the Lightning Network

The [Lightning Network](https://rusty.ozlabs.org/?p=450) is a payment channel that allows users to take funds off-chain and engage in numerous microtransactions before finalizing the payment channel and bringing the funds back into Bitcoin. Benefits include lower fees and faster transaction speeds. It's discussed in more detail, with examples of how to use it from the command line, starting [Chapter 19](19_0_Understanding_Your_Lightning_Setup.md).

[BIP 112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki) contains a few examples of how these off-chain transactions could be generated, using Bitcoin locking scripts.

### Lock with Revocable Commitment Transactions

The trick with Lightning is the fact that it's off-chain. To use Lightning, participants jointly lock funds on the Bitcoin blockchain with an n-of-n multisignature. Then, they engage in a number of transactions between themselves. Each new "commitment transaction" splits those joint funds in a different way; these transactions are partially signed but _they aren't put on the blockchain_. 

If you have a mass of unpublished transactions, any of which _could_ be placed on the Blockchain, how do you keep one of the participants from reverting back to an old transaction that's more beneficial to them? The answer is _revocation_. A simplified example in BIP 112, which offers one of the stepping stones to Lightning, shows how: you give the participant who would be harmed by reversion to a revoked transaction the ability to reclaim the funds himself if the the other participant illegitimately tries to use the revoked transaction.

For example, presume that Alice and Bob update the commitment transaction to give more of the funds to Bob (effectively: Alice sent funds to Bob via the Lightning network). They partially sign new transactions, but they also each offer up their own `revokeCode` for previous transactions. This effectively guarantees that they won't publish previous transactions, because doing so would allow their counterparty to claim those previous funds.

So what does the old transaction look like? It was a commitment transaction showing funds intended for Alice, before she gave them to Bob. It had a locking script as follows:
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
The `ELSE` block is where Alice got her funds, after a 24-hour delay. However now it's been superceded; that's the whole point of a Lightning-style payment channel, after all. In this situation, this transaction should never be published. Bob has no incentive to because he has a newer transaction, which benefits him more because he's been sent some of Alice's funds. Alice has no incentive either, because she loses the funds if she tries because of that `revokeCode`. So no one puts the transaction onto the blockchain, and the off-chain transactions continue.

It's worth exploring how this script would work in a variety of situations, most of which involve Alice trying to cheat by reverting to this older transaction, which describes the funds _before_ Alice sent some of them to Bob.

#### Run the Lock Script for Cheating Alice, with Revocation Code

Alice could try to use revocation code that she gave to Bob to immediately claim the funds. She writes a locking script of `<sigAlice> <revokeCode>`:
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
The `OP_EQUAL` feeds the `IF` statement. Because Alice uses the `revokeCode`, she gets into the branch that allows her to redeem the funds immediately, collapsing the rest of the script down to `<pubKeyBob>` (within the conditional) and `OP_CHECKSIG` (afterward).
```
Script: <pubKeyBob> OP_CHECKSIG
Running: True IF
Stack: [ <sigAlice> ]
```
Curses! Only Bob can sign immediately using the `redeemCode`!
```
Script:  OP_CHECKSIG
Stack: [ <sigAlice> <pubKeyBob> ]

Script:
Running: <sigAlice> <pubKeyBob> OP_CHECKSIG
Stack: [ False ]
```
#### Run the Lock Script for Cheating Alice, without Revocation Code

So what if Alice instead tries to use her own signature, without the `revokeCode`? She uses an unlocking script of `<sigAlice> <notRevokeCode>`.
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
We now collapse down to the `ELSE` statement and what comes after the conditional:
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
#### Run the Lock Script for Victimized Bob

What this means is that Bob has 24 hours to reclaim his funds if Alice ever tries to cheat, using the `<revokeCode>` and his signature as his unlocking script:
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

All of Alice's commitment transactions are locked with this same locking script, whether they've been revoked or not. That means that the newest commitment transaction, which is the currently valid one, is locked with it as well. Alice has never sent a newer transaction to Bob and thus never sent him the previous `revokeCode`. 

In this situation, she could virtuously publish the transaction, closing down the proto-Lightning channel. She puts the transaction on the chain and she waits 24 hours. Bob can't do anything about it because he doesn't have the recovation code. Then, after the wait, Alice reclaims her funds. (Bob does the same thing with his own final commtiment transaction.)

### Lock with Hashed Time-Lock Contracts

The Revocable Commitment Transactions were just a stepping stone to Lightning. The actual Lightning Network uses a more complex mechanism called a [hashed timelock contract](https://en.bitcoin.it/wiki/Hashed_Timelock_Contracts), or HTLC.

The main purpose of HTLCs is to create a comprehensive network of participants. Transactions are no longer just between a pair of participants who have entered the network together, but can now be between previously unassociated people. When funds are sent, a string of transactions are created, each of them locked with a `secretHash`. When the corresponding `secretCode` is revealed, the entire string of transactions can be spent. This is what allows singular transactions to actually become a network.

There's also a bit more complexity in Lightning Network locking scripts. There are separate locks for the sender and the recipient of each transaction that are more widely divergent than the differing commitment transactions alluded to in the previous section. We're going to show both of them, to demonstrate the power of these locking scripts, but we're not going to dwell on how they interact with each other. 

#### Lock the Recipient's Transaction

Once more, we're going to start looking at Alice's commitment transaction, which shows funds that she's received:
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
The key to these new HTLCs is the `secretHash`, which we said is what allows a transaction to span the network. When the transaction has spanned from its originator to its intended recipient, the `secretCode` is revealed, which allows all the participants to create a `secretHash` and unlock the whole network of payments.

After the `secretCode` has been revealed, the `IF` branch opens up: Alice can claim the funds 24 hours after the transaction is put on the Bitcoin network.

However, there's also the opportunity for Bob to reclaim his funds, which appears in the `ELSE` branch. He can do so if the transaction has been revoked (but Alice puts it on the blockchain anyway),  _or if_ an absolute timeout has occurred.

#### Lock the Sender's Transaction

Here's the alternative commitment transaction locking script used by the sender:
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

Initial Script: 
Running: <wasItSecretHash?> <wasItRevokeHash?> OP_ADD
Stack: [ <wasItSecretOrRevokeHash?> ]
```
Running through the script reveals that the initial checks, above the `IF`/`ELSE`/`ENDIF`, determine if the hash was _either_ the `secretCode` _or_ the `revokeCode`. If so, Alice can take the funds in the first block. If not, Bob can take the funds, but only after Alice has had her chance and after both the 24 hour timeout and the absolute timeout have passed.

#### Understand HTLCs

HTLCs are quite complex, and this overview doesn't try to explain all of their intricacies. Rusty Russell's [overview](https://rusty.ozlabs.org/?p=462) explains more, and there's even more detail in his [Deployable Lightning](https://github.com/ElementsProject/lightning/blob/master/doc/deployable-lightning.pdf) paper. But don't worry if some of the intricacies still escape you, particularly the interrelations of the two scripts.

For the purposes of this tutorial, there are two important lessons for HTLCs:

   * Understand that a very complex structure like an HTLC can be created with Bitcoin Script.
   * Analyze how to run each of the HTLC scripts.
   
It's worth your time running each of the two HTLC scripts through each of its permutations, one stack item at a time.

## Summary: Empowering Bitcoin with Scripts

We're closing our examination of Bitcoin Scripts with a look at how truly powerful they can be. In 20 opcodes or less, a Bitcoin Script can form the basis of an entire off-chain payment channel. Similarly, two-way pegged sidechains are the product of less than twenty opcodes, as also briefly noted in [BIP 112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki).

If you've ever seen complex Bitcoin functionality or Bitcoin-adjacent systems, they were probably built on Bitcoin Scripts. And now you have all the tools to do the same yourself.

## What's Next?

Move on to "Using Tor" with [Chapter Fourteen: Using Tor](14_0_Using_Tor.md).

Or, if you prefer, there are two alternate paths:

If you want to stay focused on Bitcoin, move on to "Programming with RPC" with [Chapter Sixteen: Talking to Bitcoind with C](16_0_Talking_to_Bitcoind.md).

Or, if you want to stay focused on the command-line because you're not a programmer, you can skip to [Chapter Nineteen: Understanding Your Lightning Setup](19_0_Understanding_Your_Lightning_Setup.md) to continue your command-line education with the Lightning Network.
