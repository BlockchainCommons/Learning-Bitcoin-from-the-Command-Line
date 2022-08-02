# Chapter X: Expanding Bitcoin Transactions with Schnorr
The traditionally used ECDSA signatures in Bitcoin for authenticating transactions are standardized but have a number of downsides like non-linearity, signature malleability, to name a few. 
These issues do not exist in the case of Schnorr signatures. Schnorr, being inherently non-malleable and linear, allows usage of various cryptographic tools in Bitcoin like MuSig, Adopter Signature and Cross-Input signature aggregation. This section aims to develop an understanding of Schnorr Signatures and their usage in Bitcoin transactions.
## Objectives for This Section
After working through this chapter, a developer will be able to:
   * Understand the Math of Schnorr and it's advantages 
   * Sign, Add and Read a Schnorr Signature
   
 
Supporting objectives include the ability to: 
   * Use Schnorr with Taproot
   * Understand the Use of Adapter Signatures 
   * Support MuSig with Schnorr
## Table of Contents

   * [Section One: Understanding Schnorr Signatures]()
   * [Section Two: Using Schnorr Signatures]()
