# Learning Bitcoin from the Command-Line #

> NOTE: This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for use.

# Copyright

Unless otherwise noted, the contents of this repository areCopyright ©2017 by Christopher Allen \<ChristopherA@LifeWithAlacrity.com\> and are licensed [CC-BY](./LICENSE-CC-BY-4.0.md).

## Introduction

The best way to learn to learn deeply about bitcoin is to avoid GUIs (even bitcoin-qt), and instead learn it from the command line.

## Requirements

This tutorial assumes that you have some minimal background of how to use the command line interface. If not, there are many tutorials available, and I have one for Mac users at https://github.com/ChristopherA/intro-mac-command-line

## Files

* [1.0: Introducing Bitcoin](1_0_Introducing_Bitcoin.md)
* [2.0: Setting Up a Bitcoin-Core VPS](2_0_Setting_Up_a_Bitcoin-Core_VPS.md)
  * [2.1: Setting Up a Bitcoin-Core VPS by Hand](2_1_Setting_Up_a_Bitcoin-Core_VPS_by_Hand.md)
  * [2.2: Setting Up a Bitcoin-Core VPS with StackScript.md](2_2_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md)
    * [Linode_Bitcoin-Core_VPS_Setup.stackscript](2_2__Script_Linode_Setup.stackscript)
* [3.0: Understanding Your Bitcoin Setup](3_0_Understanding_Your_Bitcoin_Setup.md)
  * [3.1: Verifying Your Bitcoin Setup](3_1_Verifying_Your_Bitcoin_Setup.md)
  * [3.2: Knowing Your Bitcoin Setup](3_2_Knowing_Your_Bitcoin_Setup.md)
  * [3.3: Setting Up Your Wallet](3_3_Setting_Up_Your_Wallet.md)
    * [Interlude: Using Command-Line Variables](3_3__Interlude_Using_Command-Line_Variables.md)
  * [3.4: Receiving a Transaction](3_4_Receiving_a_Transaction.md)
* [4.0: Creating Basic Bitcoin Transactions](4_0_Creating_Basic_Bitcoin_Transactions.md)
  * [4.1: Sending Coins the Easy Way](4_1_Sending_Coins_The_Easy_Way.md)
  * [4.2: Creating a Raw Transaction](4_2_Creating_a_Raw_Transaction.md)
     * [Interlude: Using JQ](4_2__Interlude_Using_JQ.md)
  * [4.3: Creating a Raw Transaction with Named Arguments](4_3_Creating_a_Raw_Transaction_with_Named_Arguments.md)
  * [4.4: Sending Coins with Raw Transactions](4_4_Sending_Coins_with_a_Raw_Transaction.md)
  * [4.5: Sending Coins with Automated Raw Transactions](4_5_Sending_Coins_with_Automated_Raw_Transactions.md)
* [5.0: Controlling Bitcoin Transactions](5_0_Controlling_Bitcoin_Transactions.md)
  * [5.1 Watching for Stuck Transactions](5_1_Watching_for_Stuck_Transactions.md)
  * [5.2: Resending a Transaction with RBF](5_2_Resending_a_Transaction_with_RBF.md)
  * [5.3: Funding a Transaction with CPFP](5_3_Funding_a_Transaction_with_CPFP.md)
* [6.0: Expanding Bitcoin Transactions](6_0_Expanding_Bitcoin_Transactions.md)
  * [6.1: Sending a Transaction with a Multsig](6_1_Sending_a_Transaction_to_a_Multisig.md) — Pending
  * [6.2: Sending a Transaction with a Locktime](6_2_Sending_a_Transaction_with_a_Locktime.md)
  * [6.3: Sending a Transaction with Data](6_3_Sending_a_Transaction_with_Data.md)

## Planned Future Work

* 7.0: Introducing Bitcoin Scripting
  * 7.1: Understanding the Foundation of Bitcoin
  * 7.2: Scripting a Pay to Public Key Hash
  * 7.3: Executing a Script
* 8.0: Empowering Multisigs with Scripts
  * 8.1: Scripting with a Multisig
  * 8.2: Scripting with a Multisig Script
  * 8.3: Creating Multisig Scripts
* 9.0: Empowering Timelocks with Scripts
  * 9.1: Understanding Timelock Options
  * 9.2: Using CLTV in Scripts
  * 9.3: Using CSV in Scripts
* 10.0: Creating Original Bitcoin Scripts
  * 10.X: Scripting a Pay to Script Hash
  * 10.X: Scripting Conditionals
* 11.0: Writing Bitcoin RPC
   * 11.1: Accessing RPC with Curl
   * 11.2: Accessing RPC with C
   * 11.3: Watching Bitcoin Addresses in C
   
<hr>
This is probably all a future section on Elements.

* 11.0: Introducing SegWit
   * 11.X Scripting a Pay to Witness
   
* X.0: Introducing Elements
* X.0: Individual Elements

* Appendix: Compiling Bitcoin
   
