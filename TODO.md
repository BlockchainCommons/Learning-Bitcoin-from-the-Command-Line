# Todo List for v3.0 of Learning Bitcoin

The following TODO items are intended for a 3.0 version of Learning Bitcoin from the Command Line

## Immediate TODO (for 2/19): Segwit

1. <strike>Reorg of Chapter 3</strike>
2. <strike>Edit Updated Content in §3.3+3.4</strike>
2. <strike>Write new Understanding Addresses (§3.5)</strike>
3. Write new QR Code Interlude (§3.6)
4. Update §3.7 to fit into new flow
5. Change §4.1-4.5 to use Bech32 (incorporate existing §4.6)
6. Remove legacy address references from the rest of the course

## Longer-Term TODO (Small Tasks)

The following updates involve updates or the creation of new chapters, but their additions are generally bounded and known.

1. Big changes to defaults
   * <strike>Signet</strike>
   * SEGWIT
      * Talk about address types: https://www.unchained.com/blog/bitcoin-address-types-compared
   * <strike>Descriptor Wallets</strike>
1. Revise 4.4
   * freezing coins
1. New Interlude: Creating Animated QR Codes (after 7.1)
   * New Subsection: Understanding Uniform Resources
   * New Subsection: Creating an Animated QR
   * New Subsection: Creating an Animated QR of a PSBT
1. Revise Section: Understanding Timelock Options (11.1)
   * Explanation: Better distinguish differences
   * Reference: consider chart at in https://prestwi.ch/bitcoin-time-locks/
  
The next step in revisions is:

1. Run through entire course & make sure all commands & responses are still accurate (likely redoing them all).

## Longer-Longer-Term TODO (Big Tasks)

The following updates involve the large-scale work done on Schnorr and Taproot in Bitcoin Core 0.21 and 22. This represents a first cut at how to layout the work, but revision and expansion will likely be needed as everyone's understanding of these new technologies matures.

**Chapter X: Expanding Bitcoin Transactions with Schnorr** (currently new chapters 9+10, with revised TOC)

* New Section X.1: Understanding Schnorr Signatures
   * New Subsection: Understanding the Math of Schnorr
      * Explanation: Add + subtract for one signature
   * New Subsection: Supporting MuSig
   * New Subsection: Understanding the Use of Adapter Signatures
   * New Subsection: Knowing the Advantages of Schnorr
      * Explanation: size, 64 bytes vs 72, better for multisigs
      * Explanation: speed, linear, validate a million-sig multisig in 2 minutes
      * Explanation: privacy, no difference between MuSig and sig, no detection of Lightning
      * Explanation: also better security, non-malleability
      * Reference: https://github.com/bitcoin/bips/blob/master/bip-0340.mediawiki
* New Section X.2: Using Schnorr Signatures
   * New Subsection: Signing with Schnorr
   * New Subsection: Adding a Schnorr Signature
   * New Subsection: Reading a Schnorr Signature 
   * New Subsection: Using Schnorr with Taproot   
* Update Chapter 6 (Multisigs) to Integrate with Schnorr

**Chapter Y: Improving Bitcoin Scripts with Taproot** (currently new chapter 15, with plans to revise TOC)

* New Section Y.1: Understanding MAST
   * New Subsection: Improving Privacy with MAST
   * New Subsection: Laying out a Script in MAST
   * New Subsection: Knowing the Advantages of MAST
      * Explanation: larger scripts
      * Explanation: hidden branches of scripts
      * Explanation: fungibility
      * https://github.com/bitcoin/bips/blob/master/bip-0114.mediawiki
* New Section Y.2: Understanding Taproot
   * New Subsection: Integrating MAST with Taproot
      * Explanation: Expanding Segwit
      * Explanation: Integrating Schnorr Signatures
   * New Subsection: KNowing the Advantages of Taproot
      * Explanation: even more privacy; scripts and other addresses are indistinbuishable
      * Reference: https://github.com/bitcoin/bips/blob/master/bip-0341.mediawiki
      * Reference: https://github.com/bitcoin/bips/blob/master/bip-0342.mediawiki
* New Subsection Y.3: Creating a Taproot Script
   * New Subsection: Defining a Taproot Script
      * Explanation: Segwit v1, 32-byte program, not P2SH wrapped, leaf version is 0xc0
   * New Codes: OP_CHECKSIGADD and OP_SUCCESS
   * Cut Codes: OP_CHECKMULTISIG and OP_CHECKMULTISIGVERIFY
   * Reference: https://twitter.com/pwuille/status/1459778730369368067
* New Subsection Y.4: Importing a tr Desciptor
* New Subsection Y.5: Using Taproot in Practice
   * New Subsection: Making a Taproot Payment
   * New Subsection: Validating a Taproot Script
* Update Chapter 9 to Integrate with Taproot
   * Mention Taproot in 9.1 or 9.2
   * Add New Section 9.6: Scripting a P2TR (mostly a pointer to Chapter Y)

## Further Updates (TIME PERMITTING)

The following updates could be part of v3.0 or could be further future, depending on how long the above takes. My guess is that they'll get incorporated come late spring or summer.

1. New Graphics
   * Mermaid or Mermaid-like diagrams.
   * For example in §3.4 could diagram HD keys, derivation of seed phrase to seed to key, etc
1. New Graphics: Animated GIFs for key demos.
   * Reference: https://github.com/faressoft/terminalizer
1. New Chapter: Using Other Command-Line Tools (between 8+9)
   * 9.1: Using seedtool
   * 9.2: Using keytool
   * 9.3: Using bytewords-cli
   * 9.4: Using envelope
1. New Chapter: Using Miniscript Command-Line Tools (between 13+14)
   * 15.1: Using miniscript
   * 15.2: Using Bitcoin Dev Kit (BDK)
   * 15.3: Planning for the Future

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

## Listing of Updates

## Legacy Updates

* [ ] **Segwit**
   * Segwit is now old enough that we should teach it as the default. That means that sections 3.1-4.5 should be rewritten to use Segwit as the default and 4.6 should be removed (with perhaps a bit of the information about the different types of addresses being preserved).
   * It's _possible_ that the same should occur with 10.5, but it should first be reviewed to see if it's a meaningful building block in the scripting process (or not).
* [ ] **Signet**
* [ ] **Descriptor Wallets**
* [ ] **Fees**
   * There was some question of if `mintxfee` is still current, or if `paytxfee` should be used. I haven't seen any evidence of obsolence, but it'd be good to check this and make sure we're still on the best practices.
   * This is discussed in [4.1](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/04_1_Sending_Coins_The_Easy_Way.md)

## 30.0 Updates

See [release notes](https://bitcoincore.org/en/releases/30.0/)

* [ ] Huge OP_RETURN update: 100,000 bytes, multiple OP_RETURN outputs

## 29.0 Updates

See [release notes](https://bitcoincore.org/en/releases/29.0/)

* RBF now absolute standard

## 28.0 Updates

See [release notes](https://bitcoincore.org/en/releases/28.0/).

* testnet4
* [ ] CPFP updates with P2A

## 27.0 Updates

* v2 transport becomes default

## 26.0 Updates

* v2 transport update

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
