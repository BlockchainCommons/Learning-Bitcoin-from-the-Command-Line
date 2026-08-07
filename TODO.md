# Todo List for v3.0 of Learning Bitcoin

The following TODO items are intended for a 3.0 version of Learning Bitcoin from the Command Line

## Immediate TODO (for 8/13): Cleanup & Taproot

1. Write Chapter 17

## Taproot Plan

**Part Six: Expanding Bitcoin with Taproot**

**Chapter 17: Introducing Taproot**

* 17.1: Breaking Down Taproot
   * Learn About the Taproot Soft Fork
      * Bitcoin Taproot is a soft fork upgrade activated on November 14, 2021. It bundles three Bitcoin Improvement Proposals: Schnorr signatures (BIP340), Taproot (BIP341), and Tapscript (BIP342).
      * Informally, the resulting design is as follows: a new witness version is added (version 1), whose programs consist of 32-byte encodings of points Q. Q is computed as P + hash(P||m)G for a public key P, and the root m of a Merkle tree whose leaves consist of a version number and a script. These outputs can be spent directly by providing a signature for Q, or indirectly by revealing P, the script and leaf version, inputs that satisfy the script, and a Merkle path that proves Q committed to that leaf. All hashes in this construction (the hash for computing Q from P, the hashes inside the Merkle tree's inner nodes, and the signature hashes used) are tagged to guarantee domain separation.
      * Reference: https://github.com/bitcoin/bips/blob/master/bip-0340.mediawiki
      * Reference: https://github.com/bitcoin/bips/blob/master/bip-0341.mediawiki
      * Reference: https://github.com/bitcoin/bips/blob/master/bip-0342.mediawiki
      * BIP 114, 117?
      * Nice diagrams: https://www.bitstack-app.com/en/learn-bitcoin/understanding-bitcoin-taproot-upgrade?c=EUR
      * Explanation: Expanding Segwit
      * Explanation: Integrating Schnorr Signatures
      * [original roadmap, advantages: https://bitcoincore.org/en/2017/03/23/schnorr-signature-aggregation/]
   * Learn About bech32m
* 17.2: Updating to Taproot
   * Update to Taproot: bitcoin-cli createwalletdescriptor

**Chapter 18: Using Schnorr Signatures**

* 18.1: Understanding Schnorr Signatures
   * Understand the Math of Schnorr
      * Explanation: Add + subtract for one signature
   * Understand the Use of Adapter Signatures
   * Know the Advantages of Schnorr
      * Explanation: size, 64 bytes vs 72, better for multisigs
      * Explanation: speed, linear, validate a million-sig multisig in 2 minutes
      * Explanation: privacy, no difference between MuSig and sig, no detection of Lightning
      * Explanation: even more privacy; scripts and other addresses are indistinbuishable
      * Explanation: also better security, non-malleability
      * Reference: https://github.com/bitcoin/bips/blob/master/bip-0340.mediawiki
* 18.2: Creating Schnorr Addresses
   * Create a Schnorr Address
   * Import a Schnorr Descriptor
   * Spend a Taproot Transaction
* 18.3: Using Bitcoin with FROST
* 18.4: Using Bitcoin with Musig2 (?)

**Chapter 19: Using Tapscript** 

* 19.1: Understanding MAST
   * Understand the Design of MAST [BIP114]
   * Know the Advantages of MAST
      * Explanation: larger scripts
      * Explanation: hidden branches of scripts
      * Explanation: privacy
      * Explanation: fungibility
      * https://github.com/bitcoin/bips/blob/master/bip-0114.mediawiki
   * Understand Tapscript
   *   Privacy with MAST
   * New Subsection: Laying out a Script in MAST
   * New Subsection: Knowing the Advantages of MAST
* 19.2: Designing Tapscripts
   * Create a Tapscript
      * Explanation: Segwit v1, 32-byte program, not P2SH wrapped, leaf version is 0xc0
      * See "Script Execution" in BIP 342 for Changes from Script
      * New Codes: OP_CHECKSIGADD and OP_SUCCESS
      * Cut Codes: OP_CHECKMULTISIG and OP_CHECKMULTISIGVERIFY
   * [no way to use currently]
* 19.3: Creating a Schnorr Multisig
   * Undestand multi_a [BIP 387]
   * Import a Schnorr Multisig
   * Spend a Schnorr Multisig

## Longer-Term TODO (Small Tasks)

The following updates involve updates or the creation of new chapters, but their additions are generally bounded and known.

1. Chapter 17+18
   * Talk about anti-correlation, anti-censorship, and anti-coercion as the three "c"s ("ac"s of privacy)
1. Close out with return to chapter 1.0 to update (a) new-for-3.0 links; and (b) what the major parts are [and what's happened to Lightning/coding]
1. If lightning is pulled to a parallel course (probably), also move lightning definitions in 1.1
     
## Longer-Longer-Term TODO (Big Tasks)

The following updates involve the large-scale work done on Schnorr and Taproot in Bitcoin Core 0.21 and 22. This represents a first cut at how to layout the work, but revision and expansion will likely be needed as everyone's understanding of these new technologies matures.



## Further Updates (TIME PERMITTING)

The following updates could be part of v3.0 or could be further future, depending on how long the above takes. My guess is that they'll get incorporated come late spring or summer.

1. New Graphics
   * Mermaid or Mermaid-like diagrams.
   * For example in §3.4 could diagram HD keys, derivation of seed phrase to seed to key, etc
1. New Graphics: Animated GIFs for key demos.
   * Reference: https://github.com/faressoft/terminalizer

As part of this, a new chapter
* Put seed words into keytool to create xprv for Bitcoin Core
* Convert Xprv from Bitcoin Core into SSKR

## Lightning Updates

The Lightning content _may_ be moved to a separate document. That decision would be made before the following potential updates happen.

1. New Chapter: Talking to Lightningd with C (after chapter 20)
   * 22.1: Creating a Lightning Channel with C
   * 22.2: Creating a Payment Request with C
   * 22.3: Paying an Invoice with C
   * 22.4: Closing a Lightning Channel with C
      * Alternatives: Consider Swift instead of C, depending on Lightning support
      * Reference: https://diyhpl.us/wiki/transcripts/blockstream-webinars/2019-07-31-rusty-russell-getting-started-with-c-lightning/
      * Reference: https://twitter.com/roasbeef/status/1389649064753471488_

## Software Updates

The software work _may_ be deprecated or moved to a separate docment. That decision would be made before the following potential updates happen.

1. New Content: Expand the PSBT Libwally Content (17.4) into Two Sections
   * 17.4: Signing PSBTs in Libwally
      * Explanation: Contains the current info on importing a PSBT, and shows how to sign it
   * 17.5: Creating PSBTs in Libwally
      * Explanation: Contains the current info on creating a PSBT, and shows how to export it
1. New Chapter: Talking to Bitcoind with Swift (between 17+18)
   * 19.1: Accessing Bitcoind with Swift (existing section)
   * 19.2: Using Swift with Bitcoin Lib [Wolf's library]
   * 19.3: Using Swift with Libwally [Wolf's shim]
  
--

# Listing of Bitcoin Updates

## 28.0 Updates

See [release notes](https://bitcoincore.org/en/releases/28.0/).

* testnet4
* [ ] CPFP updates with P2A

## 25.0 Updates

See [Bitcoin Mag](https://bitcoinmagazine.com/technical/bitcoin-core-version-25-0-released) and [release notes](https://bitcoin.org/en/releases/25.0/)

* [ ] `args` parameter for RPC

## 24.0 Updates

See [Bitcoin Mag](https://bitcoinmagazine.com/technical/bitcoin-core-24-0-released-what-is-new) and [release notes](https://bitcoincore.org/en/releases/24.0.1/)

* [ ] Miniscript
* [ ] `sendall` RPC
* [ ] RBF is now default

## 23.0 Updates

See also [#575](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/issues/575) and of course the [Bitcoin Core 23.0 release notes](https://github.com/bitcoin/bitcoin/blob/master/doc/release-notes/release-notes-23.0.md).

* [ ] **Schnorr & Taproot**
   * Obviously, the biggest update. We'll need to teach the basics of both and why they're exciting. The main question here is: what commands actually take advantage of Schnorr & Taproot, and how can we show this off.
   * If there's just a little bit of functionality right now, it can go into chapter 8, probably in two new sections: 8.3 & 8.4, on Schnorr and Taproot. If there's already a lot of functionality, it should go into its own chapter, and 8 (and everything beyond it) should be shifted back.
* [ ] **Descriptor Wallets**
   * Descriptor wallets are now the default. There's some unfortunate lack of integration with multisigs, but we should otherwise give them more attention. What can we do new about inputting and outputting descriptor wallets? Are there any other functions of note?
   * This will likely go in [3.5](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/03_5_Understanding_the_Descriptor.md), though it's possible it go split into two chapters: Understanding the Descriptor and Understanding the Descriptor Wallet. See the [0.21 release notes at the bottom](https://bitcoincore.org/en/releases/0.21.0/) for everything about Descriptor Wallets. But note that we explicitly turn them off in 3.2, so any discussions explicitly about descriptor wallets will require creating a new wallet for that purpose
* [ ] **Freezing Coins**
   * This is an interesting new ability that allows you to prevent UTXOs from being automatically selected when transactions are created. 
   * This would probably fit well into 4.5 as a final header-section.
* [ ] **CJDNS Network** 
   * I'm not familiar with CJDNS, but it sounds like a privacy option that would fit in with Tor and i2p
   * If that's correctly, this would be a good 15.2, with the 15.0 chapter renamed "Using Other Privacy Options" or something like that
* [ ] **RPC Changes**
   * The following RPC commands have had changes to their output and any examples should be rerun. If they are part of a sequence of commands (e.g., building out a transaction), then the whole sequence should be rerun.
   * [ ] `createmultisig`, 
   * [ ] `addmultisigaddress`, 
   * [ ] `listunspent`, 
   * [ ] `getblockchaininfo`
   * Updated RPCs may or may not exist in the text. The best way to find out is to search.

## 22.0 Updates

See [release notes](https://bitcoincore.org/en/releases/22.0/).

* [ ] **New External Signer Commands**
  * There are some new external signer commands: `enumeratesigners` and `displayaddress`. Are they relevant to what we're teaching? If so, should we add info on them.
  * See https://github.com/bitcoin/bitcoin/blob/22.x/doc/external-signer.md
  * Any updates would go in [7.3](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/07_3_Integrating_with_Hardware_Wallets.md).

## 0.21.0 Updates

See [release notes](https://bitcoincore.org/en/releases/0.21.0/).

* [ ] **Signet**
   * Signet is considered more controlled and reliable than testnet, and so should be used as our test network, along with an explanation of what it is and how it differs from other networks.
   * The setup and explanation of networks appears in [3.1](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/03_1_Verifying_Your_Bitcoin_Setup.md). That should be changed, and with the change of the alias there, we should mostly be used Signet. Then the rest of the course can be searched for any references to testnet.
     
---

# Progress Log

## Immediate TODO (for 8/6): Cleanup & Taproot

1. <strike>Updated Scripts to BTCC 31.1</strike>
2. <strike>Removed "" Wallet per 31.1</strike>
3. <strike>Edited 16.4</strike>
4. <strike>Reformatted Output of Chapter 5-6</strike>
5. <strike>Planned Out Chapter 17-19 on Taproot</strike>

## Immediate TODO (for 7/23): Miniscript

1. <strike>Write 16.3: Miniscript & Descriptors</strike>
2. <strike>Write 16.4: BDK</strike>

## Immediate TODO (for 7/9): Scripts

1. <strike>Revise Chapter 15</strike>
2. <strike>Wrote & Edited 16.0-16.1: Intros</strike>
3. <strike>Wrote 16.2: Converting Miniscript</strike>
4. <strike>Reformatted Output of Chapters 1-4</strike>

## Immediate TODO (for 6/25): Scripts

1. <strike>Edit §12.0, 12.1</strike>
2. <strike>Incorporate P2WSH into Rest of Chapter 12</strike>
3. <strike>Revise Chapter 13</strike>
4. <strike>Revise Chapter 14</strike>
5. <strike>Plot Out New Chapter 16, miniscript</strike>

## Immediate TODO (for 6/11): Scripts

1. <strike>Investigate Miniscript & BDK</strike>
2. <strike>Revise Chapter 11</strike>
3. <strike>Revise §12.0, §12.1</strike>

## Immediate TODO (for 5/28): Other CLI Tools

1. <strike>Edit 10.3</strike>
1. <strike>Write 10.4: Exporting Keys from Bitcoin-CLI</strike>
2. <strike>Write 10.5: Storing Keys with Envelope</strike>
3. <strike>Rewrite 10.0: Chapter Intro</strike>

## Immediate TODO (for 5/14): Other CLI Tools

1. <strike>Test Out Means for Moving Master Key into Bitcoin CLI</strike>
2. <strike>Write 10.0: Intro to Import/Export</strike>
3. <strike>Write 10.1: Creating Seeds with Seedtool</strike>
4. <strike>Write 10.2: Transforming Keys with Keytool</strike>
5. <strike>Write 10.3: Importing Keys into Bitcoin-CLI</strike>

## Immediate TODO (for 4/30): Hardware Wallets & Book & Locktime

1. <strike>Edit §8.4: Animated QRs</strike>
2. <strike>Get README & Ch. 8 Summary up to date with new section ordering</strike>
3. <strike>Prepare Index for Mkdocs</strike>
4. <strike>Prepare MkDocs for Chapters 1-8</strike>
5. <strike>Update Chapter 9 on Locktime</strike>

## Immediate TODO (for 4/16): Multisigs & PSBTs

1. <strike>Make final decisions on what to include in §7.3: Spending Multisigs</strike>
2. <strike>Rewrite §7.3</strike>
3. <strike>Reorg/Create New §8.2: "Advanced Functions for PSBTs"</strike>
4. <strike>Reorg/Create New §8.3: "Other Use Cases for PSBTs"</strike>
5. <strike>Write §8.4: Creating Animated QRs</strike>

## Immediate TODO (for 4/2): Multisigs & PSBTs

1. <strike>See if Signet faucets or back and if not consider solutions</strike>
2. <strike>Edit §7.1 + 7.2 on Multisigs</strike>
3. <strike>Edit/Revise §4.2 on Creating Descriptors</strike>
4. <strike>Renumber links on chapters 5+6</strike>
5. <strike>Rewrite §8.1 to be specifically "Spending Multisigs with PSBTs"</strike>

## Immediate TODO (for 3/26): Multisigs & Descriptors

1. <strike>Update §6.1 for creating different types of multisigs.</strike>
2. <strike>New §6.2: creating multisigs from descriptors.</strike>
3. <strike>Break Chapter Three into Chapters 3 + 4.</strike>
4. <strike>New §4.2: Integrating Addresses & Descriptors.</strike>

## Immediate TODO (for 3/12): Cleanup

1. <strike>Bring chapter 1 up to date</strike>
2. <strike>Clear Recent PRs</strike>
3. <strike>Bring chapter 2 up to date</strike>
4. <strike>Replace Gordian Server in §2.2 with Fully Noded Server</strike>
5. <strike>Double check §3.1 + 3.2</strike>
6. <strike>Put in some discussions about how we'll later import seed words in §3.4</strike>
7. <strike>Lightly edit §5.0, §5.1, §5.3 for updates.</strike>
8. <strike>Updated §5.2 for changes to RBF in recent years.</strike>

## Immediate TODO (for 2/26): Sending Transactions

1. <strike>Rewrite transaction fees in §4.1 to talk about vB, et al</strike>
2. <strike>Add sendall RPC to §4.1</strike>
3. <strike>Add args variable option to §4.3</strike>
4. <strike>Add Freezing Coins to §4.4</strike>
4. <strike>Check & Revise §4.1-4.5</strike>
6. <strike>Remove §4.6 on Segwit</strike>
7. <strike>Write New §4.6 on Sending to Different Addresses</strike>

## Immediate TODO (for 2/19): Segwit

1. <strike>Reorg of Chapter 3</strike>
2. <strike>Edit Updated Content in §3.3+3.4</strike>
3. <strike>Write new Understanding Addresses (§3.5)</strike>
4. <strike>Write new QR Code Interlude (§3.6)</strike>
5. <strike>Update §3.7 to fit into new flow</strike>
6. <strike>Update §3.7a to fit into new flow</strike>

## Immediate TODO (for 2/5): Descriptor Wallets

1. <strike>Edit §3.1-3.2, esp. for any references to legacy addresses</strike> (Still need to update `bitcoin-cli -getinfo` command) 
2. <strike>Update §3.3 to use a descriptor wallet</strike> 
3. <strike>Update any mentions of classic wallets</strike> (done for this section, need to watch in other sections)
4. <strike>Thoroughly rewrite §3.4 for new focus on descriptors, possibly expanding to two sections.</strike>
6. <strike>Remove all references to things appearing at certain versions (they're all pretty standardized now)</strike>
7. <strike>But write a note early on about potential missing features</strike>

## Immediate TODO (for 1/22): Signet

1. <strike>Update Stackscripts to support/default signet</strike>
2. <strike>Update non-Linode script too</strike>
3. <strike>Revise §2.1 for newest Stackscript</strike>
4. <strike>Puzzle out organization for overall project (largely based on updates below and remaining Issues)</strike>
5. <strike>Change testnet to signet + list out network options</strike>

## Initial TODO (for 1/15): Setup

1. <strike>Update Standup Script for BTC 30.2 & Debian 13</strike>
2. <strike>Work through PRs & Issues</strike>
3. <strike>Tag 2.2.1 and Branched 3.0</strike>
4. <strike>Log relevant updates for Bitcoin Core up to 30.2</strike>
