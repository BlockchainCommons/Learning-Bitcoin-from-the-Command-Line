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

### How Are Transactions Stored?

Transactions are built into blocks of data, which are stored on the network. These blocks are built in such a way that they can not be replaced or rewritten once a few more blocks have been built after them. This is what makes bitcoins non-repudiable: the decentralized global ledger where everything is recorded is effectively a permanent and unchangeable database.

However, the process of building these blocks is stochastic: it's somewhat random, and so you can never be assured that a transaction will be placed in a specific block. Moreso, there can be changes in blocks if they're very recent, but only if they're _very_ recent.

### How Are Transactions Protected?

The funds contained in each Bitcoin transaction are locked with a cryptographic puzzle. These puzzles are typically designed in such a way that they can be easily opened up by the person who the funds were sent to. This is done using the power of public-key cryptography. 

### So, What is Bitcoin!?

One way to think of Bitcoin is as: <i>a sequence of atomic transactions, each of which is enabled by the sender with the solution to a cryptographic puzzle, and is then locked for the recipient with a new cryptographic puzzle, where all those transactions are recorded in a permanent global ledger.</i>

Bitcoin
- cash
- transactions
- cryptographic puzzle to spend
- stochastic
Public Key Cryptography
- Public Key
-- address = public-key-hash
- Private Key
- Signature [of a message]
- Hash
Different Methodologies for Access
- all ultimatelyaccess a scripting language
Payment to Different Sorts of People
Blockchain
- hashes connecting blocks
- into a long chain
Is It Right For You
