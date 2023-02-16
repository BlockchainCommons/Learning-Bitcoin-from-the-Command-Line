# Chapter 17: Programming with Libwally

The previous chapter presented three C Libraries, for RPC, JSON, and ZMQ, all of which are intended to interact directly with `bitcoind`, just like you've been doing since the start. But, sometimes you might want to code without direct access to a `bitcoind`. This might be due to an offline client, or just because you want to keep some functionality internal to your C program. You also might want to get into deeper wallet functionality, like mnemonic word creation or address derivation. That's where Libwally comes in: it's a wallet library for C, C++, Java, NodeJS, or Python, with wrappers also available for other languages, such as Swift.

This chapter touches upon the functionality possible within Libwally, most of which complements the work you've done through RPC access to `bitcoind`, but some of which replicates it. It also shows how to integrate that work with the RPC clients that you're more familiar with. However, note that this is just the barest introduction to Libwally. Several of its more important function sets are highlighted, but we never do more than stick our toes in. If you find its functions useful or intriguing, then you'll need to dig in much more deeply than this course can cover.

## Objectives for This Chapter

After working through this chapter, a developer will be able to:

   * Use Wallet Functions with Libwally
   * Perform Manipulations of PSBTs and Transactions with Libwally
   * Implement Designs that Mix Libwally and RPC Work
   
Supporting objectives include the ability to:

  * Understand BIP39 Mnemonic Words
  * Understand More about BIP32 Hierarchical Wallets
  * Summarize the Functional Depth of Libwally
   
## Table of Contents

  * [Section One: Setting Up Libwally](17_1_Setting_Up_Libwally.md)
  * [Section Two: Using BIP39 in Libwally](17_2_Using_BIP39_in_Libwally.md)
  * [Section Three: Using BIP32 in Libwally](17_3_Using_BIP32_in_Libwally.md)
  * [Section Four: Using PSBTs in Libwally](17_4_Using_PSBTs_in_Libwally.md) 
  * [Section Five: Using Scripts in Libwally](17_5_Using_Scripts_in_Libwally.md)
  * [Section Six: Using Other Functions in Libwally](17_6_Using_Other_Functions_in_Libwally.md)
  * [Section Seven: Integrating Libwally and Bitcoin-CLI](17_7_Integrating_Libwally_and_Bitcoin-CLI.md)
