# Chapter 15: Talking to Bitcoind with C

While working with Bitcoin Scripts, we hit the boundaries of what's possible with `bitcoin-cli`: it can't currently be used to generate transactions containing unusual scripts. Fortunately, there are other ways to access the Bitcoin network: programming APIs. Though some APIs will allow you to access _all_ of Bitcoin's functionality, this course will focus on software that uses the same RPC commands that you've already learned, except accessed through programming languages rather than through the command line.

This first programming chapter focuses on the foundational C language and uses it to repeat some of the lessons learned from [Chapter 4](04_0_Sending_Bitcoin_Transactions.md): sending basic transactions before covering some new ground. Consider it a new way to use what you already know and a gateway to the larger world of working with APIs.

## Objectives for This Chapter

After working through this chapter, a developer will be able to:

   * Create C Programs that Talk to the Bitcoind
   * Create Programs Based on Bitcoind Notifications
   
Supporting objectives include the ability to:

   * Understand How to Access RPC with C
   * Understand How to Access Bitcoind with C
   
## Table of Contents

  * [Section One: Accessing Bitcoind with C](15_1_Accessing_Bitcoind_with_C.md)
  * [Section Two: Programming Bitcoind with C](15_2_Programming_Bitcoind_with_C.md)
  * [Section Three: Receiving Bitcoind Notifications with C](15_3_Receiving_Bitcoind_Notifications_with_C.md)
