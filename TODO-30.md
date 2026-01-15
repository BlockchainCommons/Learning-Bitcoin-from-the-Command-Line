# Todo List for v3.0 of Learning Bitcoin

The following TODO items are intended for a 3.0 version of Learning Bitcoin from the Command Line

## Medium-Scale Updates

The following updates involve updates or the creation of new chapters, but their additions are generally bounded and known.

1. General Update: Consider replacing testnet with signet
1. Big changes to defaults
   * Signet
   * SEGWIT
   * Descriptor Wallets
1. New Interlude: Creating QR Codes (after 3.3)
   * New Subsection: Creating a QR
   * New Subsection: Creating a Quick Connect QR
1. Revise Section: Understanding the Descriptor (3.5)
   * New Content: Descriptor Wallets
   * Possibly also remove legacy wallets (3.3) and replace them entire with descriptors
   * New Content: Complex Descriptors
   * Consider: Breaking into Two Sections
1. New Interlude: Creating Animated QR Codes (after 7.1)
   * New Subsection: Understanding Uniform Resources
   * New Subsection: Creating an Animated QR
   * New Subsection: Creating an Animated QR of a PSBT
1. New Chapter: Using Other Command-Line Tools (between 8+9)
   * 9.1: Using seedtool
   * 9.2: Using keytool
   * 9.3: Using bytewords-cli
1. Revise Section: Understanding Timelock Options (11.1)
   * Explanation: Better distinguish differences
   * Reference: consider chart at in https://prestwi.ch/bitcoin-time-locks/
1. New Chapter: Using Miniscript Command-Line Tools (between 13+14)
   * 15.1: Using miniscript
   * 15.2: Using Bitcoin Dev Kit (BDK)
   * 15.3: Planning for the Future
1. New Content: Expand the PSBT Libwally Content (17.4) into Two Sections
   * 17.4: Signing PSBTs in Libwally
      * Explanation: Contains the current info on importing a PSBT, and shows how to sign it
   * 17.5: Creating PSBTs in Libwally
      * Explanation: Contains the current info on creating a PSBT, and shows how to export it
1. New Chapter: Talking to Bitcoind with Swift (between 17+18)
   * 19.1: Accessing Bitcoind with Swift (existing section)
   * 19.2: Using Swift with Bitcoin Lib [Wolf's library]
   * 19.3: Using Swift with Libwally [Wolf's shim]

## Large-Scale Updates

The following updates involve the large-scale work done on Schnorr and Taproot in Bitcoin Core 0.21 and 22. This represents a first cut at how to layout the work, but revision and expansion will likely be needed as everyone's understanding of these new technologies matures.

**Chapter X: Expanding Bitcoin Transactions with Schnorr** (probably between chapters 6+7)

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

**Chapter Y: Improving Bitcoin Scripts with Taproot** (probably between chapters 13+14, possibly expanding to two chapters)

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

## Further Updates

The following updates could be part of v3.0 or could be further future, depending on interest and funding.

1. New Graphics: Animated GIFs for key demos.
   * Reference: https://github.com/faressoft/terminalizer
1. New Chapter: Talking to Lightningd with C (after chapter 20)
   * 22.1: Creating a Lightning Channel with C
   * 22.2: Creating a Payment Request with C
   * 22.3: Paying an Invoice with C
   * 22.4: Closing a Lightning Channel with C
      * Alternatives: Consider Swift instead of C, depending on Lightning support
      * Reference: https://diyhpl.us/wiki/transcripts/blockstream-webinars/2019-07-31-rusty-russell-getting-started-with-c-lightning/
      * Reference: https://twitter.com/roasbeef/status/1389649064753471488_

