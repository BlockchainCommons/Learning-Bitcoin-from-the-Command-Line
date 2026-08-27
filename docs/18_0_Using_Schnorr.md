# Chapter 18: Using Schnorr

The Taproot update is built on Schnorr, a digital signature
methodology invented by Claus Schnorr. It's a signature system that
wasn't quite available when the genesis block was created, but which
has been under consideration ever since for use with Bitcoin because
of the advantages it offers.

All new P2TR addresses are signed with Schnorr signatures. This can
include standard signatures internal to Bitcoin but also signatures
from more advanced Schnorr signature systems such as MuSig2 multisigs
and FROST threshold sigs.

## Objectives for This Chapter

After working through this chapter, a developer will be able to:

   * Sign with Schnorr
   * Sign with FROST
   * Sign with MuSig

Supporting objectives include the ability to:

   * Understand the Advantages of Schnorr Signatures

## Table of Contents

* [Section One: Understanding Schnorr](18_1_Understanding_Schorr.md)
* [Section Two: Creating Taproot Addresses](18_2_Creating_Taproot_Addresses.md)
* [Section Three: Using Bitcoin with FROST](18_3_Using_Bitcoin_With_FROST.md)A
* [Section Four: Using Bitcoin with MuSig2](18_4_Using_Bitcoin_With_MuSig2.md)
