# Chapter 19: Using Tapscript

Schnorr is the enabler of the Taproot upgrade: it's what makes
everything else possible. Sure, it has advantages of its own, such as
the ability to aggregate FROST and MuSig2 multisigs, but its greatest
power may be its support for combining keypaths and scriptpaths into a
single address.

Doing so requires two additional elements, both of which are outlined
in this chapter. First, it requires the ability to store multiple
scripts in a Merkle Tree that can be represented by its root
hash. Second, it requires a slight variant of Bitcoin Script designed
to work with these Merkles Trees.

Both of these additions are covered in this chapter. Though Bitcoin
Core doesn't currently show off either of these capabilities, they're
embedded in one feature: Tapscript multisigs.

## Objectives for This Chapter

After working through this chapter, a developer will be able to:

   * Create Taproot Multisigs
   * Sign for Taproot Multisigs

Supporting objectives include the ability to:

   * Understand Merkle Trees
   * Understand the Differences of Tapscript
   * Understand the Varieties of Multisig

## Table of Contents

* [Section One: Understanding MAST](19_1_Understanding_MAST.md)
* [Section Two: Designing Tapscripts](19_2_Designing_Tapscripts.md)
* [Section Three: Creating a Taproot Multisig](19_3_Creating_a_Taproot_Multisig.md)
