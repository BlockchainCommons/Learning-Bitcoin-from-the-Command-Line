# Interlude: Introducing Bitcoin

Before you can get started programming Bitcoin (and Lightning), you should have a basic understanding of what they are and how they work. This section provides that overview. Many more definitions will appear within the document itself; this is only intended to lay the foundation.

## About Bitcoin

Bitcoin is a programmatic system that allows for the transfer of the bitcoin currency. It is enabled by a decentralized, peer-to-peer system of nodes, which include full nodes, wallets, and miners. Working together, they ensure that bitcoin transactions are fast and non-repudiable. Thanks to the decentralized nature of the system, these transactions are also censorship-resistant and can provide other advantages such as pseudonymity and non-correlation if used well.

Obviously, Bitcoin is the heart of this book, but it's also the originator of many other systems, including blockchains and Lightning, which are both detailed in this tutorial, and many other cryptocurrencies such as Ethereum and Litecoin, which are not.

> 📖 **_How are coins transferred?_** Bitcoin currency isn't physical coins. Instead it's an endless series of ownership reassignments. When one person sends coins to another, that transfer is stored as a transaction. It's the transaction that actually records the ownership of the money, not any token local to the owner's wallet or their machine.

> 📖 **_Who can you send coins to?_** The vast majority of bitcoin transactions involve coins being sent to individual people (or at least to individual Bitcoin addresses). However, more complex methodologies can be used to send bitcoins to groups of people or to scripts. These various methodologies have names like P2WPKH, multisig, and P2SH. [§3.5](03_5_Understanding_the_Address.md) will cover many of the methodologies.

> 📖 **_How are transactions stored?_** Transactions are combined into larger blocks of data, which are then written to the blockchain ledger. A block is built in such a way that it cannot be replaced or rewritten once several blocks have been built atop (following) it. This is what makes bitcoins non-repudiable: the decentralized global ledger where everything is recorded is effectively a permanent and unchangeable database. However, the process of building these blocks is stochastic: it's somewhat random, so you can never be assured that a transaction will be placed in a specific block. There can also be changes in blocks if they're very recent, but only if they're _very_ recent. That means that things only become non-repudiable (and permanent and unchangeable) after a little bit of time.

> 📖 **_How are transactions protected?_** The funds contained in a Bitcoin transaction are locked with a cryptographic puzzle. These puzzles are designed so that they can be easily solved by the person who the funds were sent to. This is done using the power of public-key cryptography. Technically, a transaction is protected by a signature that proves you're the owner of the public key that a transaction was sent to: this proof of ownership is the puzzle that's being solved. Funds are further protected by the use of hashes. Public keys aren't actually stored in the blockchain until the funds are spent: only public-key hashes are. This means that even if quantum computer were to come along, Bitcoin transactions would remain protected by this second level of cryptography.

> 📖 **_How are transactions created?_** The heart of each Bitcoin transaction is a FORTH-like scripting language that is used to lock the transaction. To respend the money, the recipient provides specific information to the script that proves he's the intended recipient. However, these Bitcoin scripts are the lowest level of Bitcoin functionality. Much Bitcoin work is done through the `bitcoind` Bitcoin daemon, which is controlled through RPC commands. Many people send those RPC commands through the `bitcoin-cli` program, which provides an even simpler interface. Non-programmers don't even worry about these minutia, but instead use programmed wallets with simpler interfaces.

### Bitcoin — In Short

One way to think of Bitcoin is as _a sequence of atomic transactions_. Each transaction is authenticated by a sender with the solution to a previous cryptographic puzzle that was stored as a script. The new transaction is locked for the recipient with a new cryptographic puzzle that is also stored as a script. Every transaction is recorded in an immutable global ledger.

> 🔥 ***What is the power of Bitcoin?*** Bitcoin allows for the creation of pseudonymous identifiers (addresses based on the hashes of public keys) that can be used to transfer digital currency. It supports the right to transact through a reduction in potential censorship and coercion. That makes it particularly important in places where centralized authorities are not trusted to take on these responsibilities, whether that's due to a corrupt government or the censorship of a traditional payment processor.

## About Public-Key Cryptography

Public-key cryptography is a mathematical system for protecting data and proving ownership through an asymmetric pair of linked keys: the public key and the private key. It's important to Bitcoin (and to most blockchain systems) because it's the basis of a lot of the cryptography that protects the cryptocurrency funds. A Bitcoin transaction is typically sent to an address that is a hashed public key. The recipient is then able to retrieve the money by revealing both the public key and the private key.

> 📖 **_What is a public key?_** A public key is the key given out to other people. In a typical public-key system, a user generates a public key and a private key, then he gives the public key to all and sundry. 

> 📖 **_What is a private key?_** A private key is linked to a public key in a key pair. In a typical public-key system, a user keeps his private key secure and uses it to engage in asymmetric signing and encryption activities.

> 📖 **_What is encryption?_** Encryption is a methodology for making data unreadable. In symmetric encryption, the encoding is done with a singular "symmetric" key that allows either encryption or decryption. In asymmetric encryption, the encryption is typically done with a public key, which then requires the private key for decryption.

> 📖 **_What is a signature?_** A message (or more commonly, a hash of a message) can be signed with a private key, creating a signature. Anyone with the corresponding public key can then validate the signature, which verifies that the signer owns the private key associated with the public key in question. _SegWit_ is a specific format for storing a signature on the Bitcoin network that we'll meet down the line.

> 📖 **_What is a hash function?_** A hash function is an algorithm frequently used with cryptography. It's a way to map a large, arbitrary amount of data to a small, fixed amount of data. Hash functions used in cryptography are one-way and collision-resistant, meaning that a hash can reliably be linked to the original data, but the original data can not be regenerated from the hash. Hashes therefore allow the transmission of small amounts of data to represent large amounts of data, which can be important for efficiency and storage requirements. Bitcoin uses hashing to disguise the original data, which allows concealment of a user's actual public key, making transactions resistant to quantum computing.

### Public-Key Cryptography — In Short

One way to think of public-key cryptography is: _a way for anyone to protect data such that only an authorized person can access it, and such that the authorized person can prove that he will have that access._

> 🔥 ***What is the power of public-key cryptography?*** Public-key cryptography allows asymmetric (trap door) activities: one user can encrypt data that can only be decrypted by another; or one person can create signatures that can be verified (but not duplicated) by others. Asymmetric signatures are what make Bitcoin go round.
 
## About ECC

ECC stands for elliptic-curve cryptography. It's a specific branch of public-key cryptography that depends on mathematical calculations conducted using elliptic curves defined over finite fields. It's more complex and harder to explain than classic public-key cryptography (which used prime numbers), but it has some nice advantages.

ECC does not receive much attention in this tutorial. That's because this tutorial is all about integrating with Bitcoin Core servers that have already taken care of the cryptography for you. In fact, this tutorial's intention is that you don't have to worry about cryptography at all, because that's something that you _really_ want experts to deal with.

> 📖 **_What is an elliptic curve?_** An elliptic curve is a geometric curve that takes the form `y`<sup>`2`</sup> = `x`<sup>`3`</sup>` + ax + b`. A specific elliptic curve is chosen by selecting specific values of `a` and `b`. The curve must then be carefully examined to determine if it works well for cryptography. For example, the secp256k1 curve used by Bitcoin is defined as `a=0` and `b=7`. Any line that intersects an elliptic curve will typically so at 3 points (absent a few cases for infinity and intersections) ... and that's the basis of elliptic-curve cryptography.

> 📖 **_What are finite fields?_** A finite field is a finite set of numbers, where all addition, subtraction, multiplication, and division is defined so that it results in other numbers also in the same finite field. One simple way to create a finite field is through the use of a modulo function.

> 📖 **_How is an elliptic curve defined over a finite field?_** An elliptic curve defined over a finite field has all of the points on its curve drawn from a specific finite field. This takes the form: `y`<sup>`2`</sup> `% field-size = (x`<sup>`3`</sup>` + ax + b) % field-size` The finite field used for secp256k1 is `2`<sup>`256`</sup>` - 2`<sup>`32`</sup>` - 2`<sup>`9`</sup>` - 2`<sup>`8`</sup>` - 2`<sup>`7`</sup>` - 2`<sup>`6`</sup>` - 2`<sup>`4`</sup>` - 1`.

> 📖 **_How are elliptic curves used in cryptography?_** In elliptic-curve cryptography, a user selects a very large (256-bit) number as their private key. They then add a set base point on the curve to itself that many times. (In secp256k1, the base point is `G = 04 79BE667E F9DCBBAC 55A06295 CE870B07 029BFCDB 2DCE28D9 59F2815B 16F81798 483ADA77 26A3C465 5DA4FBFC 0E1108A8 FD17B448 A6855419 9C47D08F FB10D4B8`, which prefixes the two parts of the tuple with an `04` to say that the data point is in uncompressed form. If you prefer a straight geometric definition, it's the point "0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798,0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8") The resultant number is the public key. Various mathematical formula can then be used to prove ownership of the public key, given the private key. As with any cryptographic function, this one is a trap door: it's easy to go from private key to public key and largely impossible to go from public key to private key. This particular methodology also explains why finite fields are used in elliptic curves: it ensures that the private key will not grow too large. (Note that the finite field for secp256k1 is slightly smaller than 256 bits, which means that all public keys will be 256 bits long, just like the private keys are.)

**_What are the advantages of ECC?_** The main advantage of ECC is that it allows the same security as classic public-key cryptography with a much smaller key. A 256-bit elliptic-curve public key corresponds to a 3072-bit traditional (RSA) public key.

### ECC - In Short

One way to think of ECC is: _a way to enable public-key cryptography that uses very small keys and very obscure math._

> 🔥 ***What is the power of ECC?*** The power of ECC is in its small keys. This is important for blockchains because every signature with every private key is recorded on the blockchain forever. Before the block size wars of the '10s, blocks were getting too crowded, making it harder (and more expensive) to create successful Bitcoin transactions. That would be even worse without the power of ECC.

## About Blockchains

Blockchain is the generalization of the methodology used by Bitcoin to create a distributed global ledger. The more general methodology can be used for many purposes: there are lots of other cryptocurrencies that each have their own blockchain, and blockchains have also been used for other purposes, such as recording the provenance of digital assets. Because blockchains have become a wide category of technology, the basic concepts discussed in this course are likely to be applicable to many other projects in this growing technology sector. The specific programming commands learned in this book will not be, however, as they're fairly specific to Bitcoin.

Though you need to understand the basics of how a blockchain works in order to understand how transactions work in Bitcoin, you won't need to go any further than that. B

> 📖 **_Why is it called a chain?_** Each block in the blockchain stores a hash of the block before it. This links the current block to the previous block and goes all the way back to the original "genesis block" through an unbroken chain. It's a way to create absolute order among possibly conflicting data. This also provides the security of blockchain, because each block that is stacked atop an old one makes it harder to recreate the old block due to the proof-of-work algorithms used in block creation. Once several blocks have been built atop a block in the chain, it's essentially irreversible.

> 📖 **_What is proof of work_** Blockchains are a "trustless" technology, which means that no one has to actually trust anyone else involved in transactions. Instead, trust is created by the technology itself. For Bitcoin, that means that when a transaction happens, you know the funds have been transferred, and there's no way to reverse it (once a few blocks are built on top of the block with your transaction). You know this because of "proof of work". That means that it takes an intense amount of computation to create a block. When you multiply that by a few blocks stacked atop each other, the proof of work has become so high that it will never be reversed. Other blockchains create their implicit trust in other ways. For example, Ethereum has been using "proof of stake" since 2022, which means that the permanence of blocks is assured by people putting their own currency behind a block, "staking" it.
 
**_What is a fork?_** Occasionally two blocks are created around the same time. This temporarily creates a one-block fork, where either of the current blocks could be the "real" one. Every once in a while, a fork might expand to become two blocks, three blocks, or even four blocks long, but pretty quickly one side of the fork is determined to be the real one, and the other is "orphaned". This is part of the stochastic process of block creation, and demonstrates why several blocks must be built atop a block before it can be considered truly trustworthy and non-repudiable.

### Blockchain — In Short

One way to think of blockchain is: _a linked series of blocks of unchangeable data, going back in time_. Another way is: _a linked series of blocks to absolutely order data that could be conflicting_.

> 🔥 ***What is the power of blockchain?*** Blockchains have a number of advantages that offset potential costs (as discussed below). The biggest include their permanent and immutable ledgers and the ability to transact in an environment where the counterparties don't trust each other.

## Is Blockchain Right for Me?

If you want to transact bitcoins, then obviously Bitcoin is right for you. However, more widely, blockchain has become a popular buzz-word even though it's not a magic bullet for all technical problems. With that said, there are many specific situations where blockchain is a superior technology.

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
    * e.g.: Counterparties should be secret.
    * Unless: A methodology for cryptographic secrecy is carefully considered, analyzed, and tested.
  * Users need instant transaction finality.
    * e.g.: in less than 10 minutes on a Bitcoin-like network, in less than 2.5 minutes on a Litecoin-like network, in less than 15 seconds on an Ethereum-like network

Do note that there may still be solutions for some of these situations within the Bitcoin ecosystem. For example, payment channels are rapidly addressing questions of liquidity and payment finality, while related cryptocurrencies such as Zcash allow for secrecy using "shielded" transactions.

## About Lightning

Lightning is a layer-2 protocol that interacts with Bitcoin to allow users to exchange their bitcoins "off-chain". It has both advantages and disadvantages over using Bitcoin on its own.

Lightning is also the secondary focus of this tutorial. Though the course is mostly about interacting directly with Bitcoin (and the `bitcoind`), it pays some attention to Lightning because it's an upcoming technology that at times has offered advantages over Bitcoin (particularly when fees cranked up during the block size wars). This book takes the same approach to Lightning as to Bitcoin: it teaches how to interact directly with a trusted Lightning daemon from the command line.

Unlike with Bitcoin, there are actually several variants of Lightning. This tutorial uses the standard-compliant [core lightning](https://github.com/ElementsProject/lightning) implementation as its trusted Lightning server.

> 📖 **_What is a layer-2 protocol?_** A layer-2 Bitcoin protocol works on top of Bitcoin. In this case, Lightning works atop Bitcoin, interacting with it through smart contracts.

> 📖 **_What is a Lightning Channel?_** A Lightning Channel is a connection between two Lightning users. Each of the users locks up some number of bitcoins on the Bitcoin blockchain using a multi-sig signed by both of them. The two users can then exchange bitcoins through their Lightning channel without ever writing to the Bitcoin blockchain. Only when they want to close out their channel do they settle their bitcoins, based on the final division of coins.

> 📖 **_What is a Lightning Network?_** Putting together a number of Lightning Channels creates the Lightning Network. This allows two users who have not created a channel between themselves to exchange bitcoins using Lightning: the protocol forms a chain of Channels between the two users, then exchanges the coins through the chain using time-locked transactions.

> 📖 **_What are the advantages of Lightning?_** Lightning allows for faster transactions with lower fees. This creates the real possibility of bitcoin-funded micropayments. It also offers better privacy, since it's off-chain with only the first and last states of the transaction being written to the immutable Bitcoin ledger.

> 📖 **_What are the disadvantages of Lightning?_** Lightning is still a very new technology and hasn't been tested as thoroughly as Bitcoin. That's not just a question of the technological implementation, but also whether the design itself can be gamed in any unexpected ways.

### Lightning - In Short

One way to think of Lightning is: _a way to transact bitcoins using off-chain channels between pairs of people, so that only a first and final state have to be written to the blockchain_.

> 🔥 ***What is the power of Lightning?*** Lightning has a pair of major advantages: it's private and cheap. The privacy comes from the fact that much of it is conducted off-chain, meaning that many of your transactions aren't written to the permanent ledger of Bitcoin. The cheapness just means that fees are lower. At various times, the cost to send Bitcoin transactions has varied from a few cents to ten or twenty dollars. Whenever those fees come up from the lowest levels, they defeat the ability to send microtransactions. Lightning maintains that ability.

## Summary: Introducing Bitcoin

Bitcoin is a peer-to-peer system that allows for the transfer of funds through transactions that are locked with puzzles. These puzzles are dependent upon public-key elliptic-curve cryptography. When you generalize the ideas behind Bitcoin, you get blockchains, a technology that's currently growing and innovating. When you expand the ideas behind Bitcoin, you get layer-2 protocols such as Lightning, which expand the currency's potential.

## What's Next?

Advance through "Preparing for Bitcoin" with [Chapter Two: Setting Up a Bitcoin-Core VPS](02_0_Setting_Up_a_Bitcoin-Core_VPS.md).
