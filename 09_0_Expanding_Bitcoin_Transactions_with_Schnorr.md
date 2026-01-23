This is the initial theory for this section:

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
