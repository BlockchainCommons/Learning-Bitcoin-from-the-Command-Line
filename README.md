# Learning Bitcoin #

> NOTE: This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for use.

# Copyright

Unless otherwise noted, the contents of this repository are Copyright ©2017 by Christopher Allen [@ChristopherA](https://github.com/ChristopherA) \<ChristopherA@LifeWithAlacrity.com\> and are licensed [CC-BY](./LICENSE-CC-BY-4.0.md).

### Credits

| Role                | Names                                    |
| ------------------- | ---------------------------------------- |
| ***Lead Authors:*** | Christopher Allen [@ChristopherA](https://github.com/ChristopherA), Shannon Appelcline [@ShannonA](https://github.com/ShannonA) |
| ***Contributors:*** |                                          |
| ***Reviewers:***    | Glen Willem [@gwillem](https://github.com/gwillem) |
| ***Sponsors:***     | Blockstream Corporation                  |

## Introduction

The best way to learn to learn deeply about bitcoin is to avoid GUIs (even bitcoin-qt), and instead learn it from the command line.

## Requirements

This tutorial assumes that you have some minimal background of how to use the command line interface. If not, there are many tutorials available, and I have one for Mac users at https://github.com/ChristopherA/intro-mac-command-line

## Files

**PART ONE: PREPARING FOR BITCOIN**

* [1.0: Introducing Bitcoin](1_0_Introducing_Bitcoin.md)
* [2.0: Setting Up a Bitcoin-Core VPS](2_0_Setting_Up_a_Bitcoin-Core_VPS.md)
  * [2.1: Setting Up a Bitcoin-Core VPS by Hand](2_1_Setting_Up_a_Bitcoin-Core_VPS_by_Hand.md)
  * [2.2: Setting Up a Bitcoin-Core VPS with StackScript.md](2_2_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md)
    * [Linode_Bitcoin-Core_VPS_Setup.stackscript](2_2__Script_Linode_Setup.stackscript)

**PART TWO: BITCOIN-CLI**

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
  * [6.1: Sending a Transaction with a Multsig](6_1_Sending_a_Transaction_to_a_Multisig.md)
  * [6.2: Spending a Transaction with a Multsig](6_2_Spending_a_Transaction_to_a_Multisig.md)
  * [6.3: Sending & Spending an Automated Multisig](6_3_Sending_an_Automated_Multisig.md)
  * [6.4: Sending a Transaction with a Locktime](6_4_Sending_a_Transaction_with_a_Locktime.md)
  * [6.5: Sending a Transaction with Data](6_5_Sending_a_Transaction_with_Data.md)

**PART THREE: BITCOIN SCRIPTING**

* [7.0: Introducing Bitcoin Scripts](7_0_Introducing_Bitcoin_Scripts.md)
  * [7.1: Understanding the Foundation of Transactions](7_1_Understanding_the_Foundation_of_Transactions.md)
  * [7.2: Running a Bitcoin Script](7_2_Running_a_Bitcoin_Script.md)
  * [7.3: Scripting a P2PKH](7_3_Scripting_a_P2PKH.md)
  * [7.4: Testing a Bitcoin Script](7_4_Testing_a_Bitcoin_Script.md)
* [8.0: Embedding Bitcoin Scripts in P2SH Transactions](8_0_Embedding_Bitcoin_Scripts_in_P2SH_Transactions.md)
  * [8.1: Understanding the Foundation of P2SH](8_1_Understanding_the_Foundation_of_P2SH.md)
  * [8.2: Building the Structure of P2SH](8_2_Building_the_Structure_of_P2SH.md)
  * [8.3: Running a Bitcoin Script with P2SH](8_3_Running_a_Bitcoin_Script_with_P2SH.md)
  * [8.4: Scripting a Multisig](8_4_Scripting_a_Multisig.md)
  * [8.5: Spending a P2SH Transaction](8_5_Spending_a_P2SH_Transaction.md) — Awaiting API Work
* [9.0: Empowering Timelock with Bitcoin Scripts](9_0_Empowering_Timelock_with_Bitcoin_Scripts.md)
  * [9.1: Understanding Timelock Options](9_1_Understanding_Timelock_Options.md)
  * [9.2: Using CLTV in Scripts](9_2_Using_CLTV_in_Scripts.md)
  * [9.3: Using CSV in Scripts](9_3_Using_CSV_in_Scripts.md)
* [10.0: Expanding Bitcoin Scripts](10_0_Expanding_Bitcoin_Scripts.md)
  * [10.1: Using Script Conditionals](10_1_Using_Script_Conditionals.md)
  * [10.2: Using Other Script Commands](10_2_Using_Other_Script_Commands.md)
* [11.0: Designing Real Bitcoin Scripts](11_0_Designing_Real_Bitcoin_Scripts.md)
  * [11.1: Writing Puzzles Scripts](11_1_Writing_Puzzle_Scripts.md)
  * [11.2: Writing Complex Multisig Scripts](11_2_Writing_Complex_Multisig_Scripts.md)
  * [11.3: Empowering Bitcoin with Scripts](11_3_Empowering_Bitcoin_with_Scripts.md)

## Planned Future Work

** PART FOUR: BITCOIN APIS **

* 12.0: Talking to Bitcoind
   * 12.1: Accessing Bitcoind with Curl
   * 12.2: Accessing Bitcoind with JavaScript
   * 12.3: Accessing Bitcoind with C
      * Watching Bitcoin Addresses in C?
* 13.0: Programming with LibWally
   * 13.1: Programming Transactions
   * 13.2: Programming Scripts
* 14.0: Programming with secp256k1

** PART FIVE: BITCOIN FUTURES **

* 15.0: Setting Up a Bitcoin Regtest
   * 15.1: Building the Regtest
   * 15.2: Testing with Regtest
   * 15.3: Mining with Regtest

** APPENDICES **

* Appendix: Compiling Bitcoin
