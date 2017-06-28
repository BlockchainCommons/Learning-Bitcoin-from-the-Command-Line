# Chapter 12: Talking to Bitcoind

While working with Bitcoin Scripts, we hit the boundaries of what's possible with `bitcoin-cli`. Fortunately, there are other ways to access  the Bitcoin network: programming APIs. Though some APIs will allow you to access _all_ of Bitcoin's functionality, we're going to start off with the simpler APIs that largely duplicate `bitcoin-cli` by accessing `bitcoind` through RPC, with a focus on a basic C library. Consider it a new way to do what you already know and a gateway to the larger world of working with APIs.

## Objectives for This Chapter

After working through this chapter, a developer will be able to:

   * Decide Between Different Methods of Talking to Bitcoind
   * Create Bitcoin Transactions by Talking Directly to Bitcoind
   * Create Programs Based on Bitcoind Notifications
   
Supporting objectives include the ability to:

   * Understand How RPC and Curl Interact
   * Understand How to Use APIs
   * Create a Foundation for Accessing Bitcoind 
   
## Table of Contents

  * [Section One: Accessing Bitcoind with Curl](12_1_Accessing_Bitcoind_with_Curl.md)
  * [Section Two: Accessing Bitcoind with C](12_2_Accessing_Bitcoind_with_C.md)
  * [Section Three: Programming Bitcoind with C](12_3_Programming_Bitcoind_with_C.md)
  * [Section Four: Receiving Bitcoind Notifications with C](12_4_Receiving_Bitcoind_Notifications_with_C.md) — Pending
  * [Section Five: Accessing Bitcoind with Other Languages](12_5_Accessing_Bitcoind_with_Other_Languages.md)  — Writing
