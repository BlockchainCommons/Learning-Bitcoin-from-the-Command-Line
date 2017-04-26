# Chapter One: Introducing Bitcoin

These tutorial will lead you through basic Bitcoin usage and programming, using a variety of techniques from the command line to scripting. However, before you should get started, you should have a base understanding of what Bitcoin is, and how it works. This chapter provides that overview. Many more definitions will appear within the document itself; this is only intended to lay the foundation.

## Objectives for this Chapter

After working through this chapter, a developer will be able to:

   * Assess the Parts of the Bitcoin System
   * Decide Whether Blockchain is Right for You

Supporting objectives include the ability to:

   * Understand What Bitcoin Is
   * Understand What Blockchain Transactions Are
   * Understand What Blockchain Is
   
## What is Bitcoin?

Bitcoin is a system that allows for the transfer of the bitcoin currency. It is enabled by a peer-to-peer system of nodes, which include full nodes, wallets, and miners. Working together, they ensure that bitcoin transactions are fast and non-repudiable.

### How Are Coins Transferred?

Bitcoin currency isn't physical coins. Instead it's an endless series of ownership reassignments. When one person sends coins to another, that transfer is stored as a transaction. It's the transaction that actually records the ownership of the money, not any token or record local to your wallet or your machine.

### Who Can You Send Transactions To?

The vast majority of bitcoin transactions are send to individual people (or at least to individual Bitcoin addresses). However, more complex methodologies instead send bitcoins to groups of people or to scripts. These various methodologies have names like P2PKH, multisig, and P2SH.

### How Are Transactions Stored?

Transactions are built into blocks of data, which are stored on the network. These blocks are built in such a way that they can not be replaced or rewritten once a few more blocks have been built after them. This is what makes bitcoins non-repudiable: the decentralized global ledger where everything is recorded is effectively a permanent and unchangeable database.

However, the process of building these blocks is stochastic: it's somewhat random, and so you can never be assured that a transaction will be placed in a specific block. Moreso, there can be changes in blocks if they're very recent, but only if they're _very_ recent. So, things become non-repudiable after a little bit of time.

### How Are Transactions Protected?

The funds contained in each Bitcoin transaction are locked with a cryptographic puzzle. These puzzles are typically designed in such a way that they can be easily opened up by the person who the funds were sent to. This is done using the power of public-key cryptography. 

## How Are Transactions Written?

The heart of a Bitcoin transaction is a FORTH-like scripting language that is used to lock transactions. To respend the money, the recipient provides specific information to the script that proves he's the intended recipient.

However, these Bitcoin scripts are a fairly low-level of abstraction. Much Bitcoin work is done through the `bitcoind` Bitcoin daemon, which is controlled through RPC commands. Many people send those RPC commands through the `bitcoin-cli` program, which provides an even simpler interface. (And non-programmers don't even worry about these minutia, but instead use programmed wallets with simpler interfaces.)

### So, What is Bitcoin!?

One way to think of Bitcoin is as: _a sequence of atomic transactions, each of which is enabled by the sender with the solution to a cryptographic puzzle, which was stored as a script, and is then locked for the recipient with a new cryptographic puzzle, which is stored as a script, where all those transactions are recorded in a permanent global ledger._

## What is Public-Key Cryptography?

Public-key cryptography is a mathematical system for protecting data and proving ownership through an asymmetric pair of linked keys: the public key and the private key.

### What is a Public Key?

A public key is the key that you give out to other people. In a typical public-key system, a user generates a public key and a private key, then he gives us the public-key to all and sundry. They can then encrypt information with the public key, but it can't be decrypted with the public key because of the asymmetry of the key pair.

### What is a Private Key?

A private key is linked to a public key in a key pair. In a typical public-key system, the user keeps his private key secure, and uses it to decrypt messages sent to him, encrypted with the public key.

### What is a Signature?

A message (or more commonly, a hash of a message) can be signed with a private key, creating a signature. Anyone with the corresponding public key can then validate that the signature, which verifies that the signer owns the private key associated with the public key in question. Basically, it lets you know who you're working with.

### What is a Hash?

A hash is an algorithm frequently used with cryptograpy. It's a way to squish an arbitrary amount of data down to a small, fixed amount of data. Thus, you might sign a small hash of a data file rather than a very large data file, for both computational and transmission efficiency.

### So, What is Public-Key Cryptography!?

One way to think of public-key cryptography is: _a way for anyone to protect data that only one person can access, and for that one person to prove that he will have that access._

## What is Blockchain?

Blockchain is the generalization of the methodology used by Bitcoin to create a permanent, global ledger. Bitcoin is a blockchain, as are any number of alt-coins, each of which lives in its networks and writes to its own chain. Sidechains like Liquid are blockchains too. Blockchains don't even need to have anything to do with finances. For example, there have been many discussions of using blockchains to protect self-sovereign identities.

### Why Is It Called a Chain?

Each block in the blockchain stores a hash of the block before it. This links the current block all the way back the original "genesis block" through an unbroken chain.

### What is a Fork?

Occasionally two blocks are created around the same time. This temporarily creates a one-block fork, where either current block could be the "real" one. When one of the blocks is chosen and built atop of, the other one becomes "orphaned". This is part of the stochastic process of block creation, and demonstrates that a few blocks must be built atop a block before it can be considered truly trustworthy and considered non-repudiable.

### So, What is Blockchain!?

One way to think of blockchain is: _a linked series of blocks of non-changeable data, going back in time_.

## Is Blockchain Right for Me?

Blockchain has becomea popular buzz-word, but it's not a magic bullet for all technical problems. With that said, blockchains do have some very specific situations where it's a superior technology.

This includes when:

  * Users don't trust each other.
    * Or: Users are across various borders.
  * Users don't trust central authorities.
     * And: Users want to control their own destinies.
     * And: Users want transparent technology.
  * Users want to share something.
    * And: Users want what's shared to be permanently recorded.
  * Users want fast transaction finality.
    * But: Users don't need instant transaction finality.
    
