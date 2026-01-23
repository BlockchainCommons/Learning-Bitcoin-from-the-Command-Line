This was the initial theory for this section:

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
* Update Chapter 6 (Multisigs) to Integrate with Schnorr [this might be another section]

We also may want some of this:

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
