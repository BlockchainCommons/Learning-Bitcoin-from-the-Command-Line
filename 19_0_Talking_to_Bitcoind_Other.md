# Chapter 18: Talking to Bitcoind with Other Languages

You should now have a solid foundation for working with Bitcoin in C, not only using RPC, JSON, and ZMQ libraries to directly interact with `bitcoind`, but also utilizing the Libwally libraries to complement that work. And C is a great language for prototyping and abstraction — but it's probably not what you're programming in. This chapter thus takes a whirlwind tour of six other programming languages, demonstrating the barest Bitcoin functionality in each and allowing you to expand the lessons of the command line and C to the programming language of your choice.

Each of the sections contains approximately the same information, focused on: creating an RPC connection; examining the wallet; creating a new address, and creating a transaction. However, there's some variety among the languages, showing off different aspects of Bitcoin's RPC commands in different examples. In particular, some languages use the easy methodology of `sendtoaddress` while others use the hard methodology of creating a raw transaction from scratch.

## Objectives for This Chapter

After working through this chapter, a developer will be able to:

   * Prepare Bitcoin Development Environments for a Variety of Languages
   * Use Wallet Functions in a Variety of Languages
   * Use Transaction Functions in a Variety of Languages
   
Supporting objectives include the ability to:

  * Understand More about Bitcoin RPC through Interactions with a Variety of Languages
   
## Table of Contents

  * [Section One: Accessing Bitcoind with Go](18_1_Accessing_Bitcoind_with_Go.md)
  * [Section Two: Accessing Bitcoind with Java](18_2_Accessing_Bitcoind_with_Java.md)
  * [Section Three: Accessing Bitcoind with NodeJS](18_3_Accessing_Bitcoind_with_NodeJS.md)
  * [Section Four: Accessing Bitcoind with Python](18_4_Accessing_Bitcoind_with_Python.md)
  * [Section Five: Accessing Bitcoind with Rust](18_5_Accessing_Bitcoind_with_Rust.md)
  * [Section Six: Accessing Bitcoind with Swift](18_6_Accessing_Bitcoind_with_Swift.md)
