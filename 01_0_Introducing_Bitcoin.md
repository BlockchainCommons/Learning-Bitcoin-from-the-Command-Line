# Chapter One: Introducing Bitcoin

These tutorial will lead you through basic Bitcoin usage and programming, highlighting a variety of techniques from the command line to scripting. However, before you should get started, you should have a basic understanding of what Bitcoin is, and how it works. This chapter provides that overview. Many more definitions will appear within the document itself; this is only intended to lay the foundation.

## Objectives for this Chapter

After working through this chapter, a developer will be able to:

   * Assess the Parts of the Bitcoin System
   * Decide Whether Blockchain is Right for Them

Supporting objectives include the ability to:

   * Understand What Bitcoin Is
   * Understand What Blockchain Transactions Are
   * Understand What Blockchain Is
   
## What Is Bitcoin?

Bitcoin is a programmatic system that allows for the transfer of the bitcoin currency. It is enabled by a peer-to-peer system of nodes, which include full nodes, wallets, and miners. Working together, they ensure that bitcoin transactions are fast and non-repudiable.

### How Are Coins Transferred?

Bitcoin currency isn't physical coins. Instead it's an endless series of ownership reassignments. When one person sends coins to another, that transfer is stored as a transaction. It's the transaction that actually records the ownership of the money, not any token local to the owner's wallet or their machine.

### Who Can You Send Transactions To?

The vast majority of bitcoin transactions are sent to individual people (or at least to individual Bitcoin addresses). However, more complex methodologies can be used to send bitcoins to groups of people or to scripts. These various methodologies have names like P2PKH, multisig, and P2SH.

### How Are Transactions Stored?

Transactions are combined into larger blocks of data, which are stored on the network. A block is built in such a way that it can not be replaced or rewritten once several blocks have been built after it. This is what makes bitcoins non-repudiable: the decentralized global ledger where everything is recorded is effectively a permanent and unchangeable database.

However, the process of building these blocks is stochastic: it's somewhat random, and so you can never be assured that a transaction will be placed in a specific block. Moreso, there can be changes in blocks if they're very recent, but only if they're _very_ recent. So, things become non-repudiable (and permanent and unchangeable) after a little bit of time.

### How Are Transactions Protected?

The funds contained in a Bitcoin transaction are locked with a cryptographic puzzle. These puzzles are designed so that they can be easily solved by the person who the funds were sent to. This is done using the power of public-key cryptography. 

### How Are Transactions Written?

The heart of each Bitcoin transaction is a FORTH-like scripting language that is used to lock the transaction. To respend the money, the recipient provides specific information to the script that proves he's the intended recipient.

However, these Bitcoin scripts are the lowest level of Bitcoin functionality. Much Bitcoin work is done through the `bitcoind` Bitcoin daemon, which is controlled through RPC commands. Many people send those RPC commands through the `bitcoin-cli` program, which provides an even simpler interface. Non-programmers don't even worry about these minutia, but instead use programmed wallets with simpler interfaces.

### Bitcoin — In Short

One way to think of Bitcoin is as: _a sequence of atomic transactions: each of which is enabled by the sender with the solution to a previous cryptographic puzzle that was stored as a script; each of which is locked for the recipient with a new cryptographic puzzle that is stored as a script; and each of which is recorded in an immutable global ledger._

## What Is Public-Key Cryptography?

Public-key cryptography is a mathematical system for protecting data and proving ownership through an asymmetric pair of linked keys: the public key and the private key.

### What Is a Public Key?

A public key is the key given out to other people. In a typical public-key system, a user generates a public key and a private key, then he gives the public key to all and sundry. Those recipients can encrypt information with the public key, but it can't be decrypted with the same public key because of the asymmetry of the key pair.

### What Is a Private Key?

A private key is linked to a public key in a key pair. In a typical public-key system, a user keeps his private key secure and uses it to decrypt messages sent to him, encrypted with the public key.

### What Is a Signature?

A message (or more commonly, a hash of a message) can be signed with a private key, creating a signature. Anyone with the corresponding public key can then validate the signature, which verifies that the signer owns the private key associated with the public key in question. 

### What Is a Hash Function?

A hash function is an algorithm frequently used with cryptography. It's a way to map a large, arbitrary amount of data to a small, fixed amount of data. Hash functions used in cryptography are one-way and collision-resistant, meaning that a hash can very reliably be linked to the original data, but the original data can not be regenerated from the hash. Hashes thus allow the transmission of small amounts of data to represent large amounts of data, which can be important for efficiency and storage requirements. 

### Public-Key Cryptography — In Short

One way to think of public-key cryptography is: _a way for anyone to protect data such that only an authorized person can access it, and for that authorized person to prove that he will have that access._

## What Is Blockchain?

Blockchain is the generalization of the methodology used by Bitcoin to create a distributed global ledger. Bitcoin is a blockchain as are any number of alt-coins, each of which lives in its networks and writes to its own chain. Sidechains like Liquid are blockchains too. Blockchains don't even need to have anything to do with finances. For example, there have been many discussions of using blockchains to protect self-sovereign identities.

### Why Is It Called a Chain?

Each block in the blockchain stores a hash of the block before it. This links the current block all the way back the original "genesis block" through an unbroken chain. It's a way to create absolute order among possibly conflicting data. This also provides the security of blockchain, because each block stacked atop an old one makes it harder to recreate the old block due to the proof-of-work algorithms used in block creation. Once several blocks have been built atop a block in the chain, it's essentially irreversible. 

### What is a Fork?

Occasionally two blocks are created around the same time. This temporarily creates a one-block fork, where either current block could be the "real" one. Every once in a while, a fork might expand to become two blocks, three blocks, or even four blocks long, but pretty quickly one side of the fork is determined to be the real one, and the other is "orphaned". This is part of the stochastic process of block creation, and demonstrates why several blocks must be built atop a block before it can be considered truly trustworthy and non-repudiable.

### Blockchain — In Short

One way to think of blockchain is: _a linked series of blocks of unchangeable data, going back in time_. Another way is: _a linked series of blocks that absolutely order data that could be conflicting_.

## Is Blockchain Right for Me?

If you want to transact bitcoins, then obviously Bitcoin is right for you. However, more widely, Blockchain has become a popular buzz-word even though it's not a magic bullet for all technical problems. With that said, there are many specific situations where blockchain is a superior technology.

Blockchains probably _will_ be helpful if:

  * Users don't trust each other.
    * Or: Users exist across various borders.
  * Users don't trust central authorities.
    * And: Users want to control their own destinies.
  * Users want transparent technology.
  * Users want to share something.
    * And: Users want what's shared to be permanently recorded.
  * Users want fast transaction finality.
    * But: Users don't need instant transaction finality.
    
Blockchains probably _will not_ be helpful if:

  * Users are trusted:
    * e.g.: transactions occur within a business or organization.
    * e.g.: transactions are overseen by a central authority.
  * Secrecy is required:
    * e.g.: Information should be secret.
    * e.g.: Transactions should be secret.
    * e.g.: Transactors should be secret.
  * Users need instant transaction finality, in less than an hour.
  
Do note that there may still be solutions for some of these situations within the Bitcoin ecosystem. For example, payment channels are rapidly addressing questions of liquidity and payment finality. 

## Summary: Introducing Bitcoin

Bitcoin is a peer-to-peer system that allows for the transfer of funds through transactions that are locked with puzzles. These puzzles are dependent upon public-key cryptography. When you generalize the ideas behind Bitcoin, you get blockchains, a technology that's currently growing and innovating.

## What's Next?

Advance through "Preparing for Bitcoin" with [Chapter Two: Setting Up a Bitcoin-Core VPS](02_0_Setting_Up_a_Bitcoin-Core_VPS.md).
