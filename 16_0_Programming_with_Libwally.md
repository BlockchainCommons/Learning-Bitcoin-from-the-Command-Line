# Chapter 16: Programming with Libwally

The previous chapter presented three C Libraries, for RPC, JSON, and ZMQ, all of which are intended to interact directly with `bitcoind`, just like you've been doing since the start. But, sometimes you might want to code without direct access to a `bitcoind`. This might be due to an offline client, or just because you want to keep some functionality internal to your C program. You also might want to get into deeper wallet functionality, like cryptography or address derivation. That's where Libwally comes in: it's a wallet library for C, C++, Java, NodeJS, or Python, with wrappers also available for other languages, such as Swift.

This chapter touches upon the functionality possible within Libwally, most of which complements the work you've done through RPC access to `bitcoind`, but some of which replicates it. Obviously, this could be used with all three of the libraries from the previous chapter, and together they can create a strong basis for your C programming of Bitcoin.

## Objectives for This Chapter

After working through this chapter, a developer will be able to:

   * Use Cryptographic Functions with Libwally
   * Use Wallet Functions with Libwally
   * Create C Programs that Integrate Multiple Functionality
   
Supporting objectives include the ability to:

[TODO]
   
## Table of Contents

  * [Section One: Setting Up Libwally](16_1_Setting_Up_Libwally.md)
