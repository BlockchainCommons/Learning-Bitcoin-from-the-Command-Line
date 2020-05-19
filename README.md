# Programming with Bitcoin Core and Lightning
## (Or: Programming Bitcoind and Lightningd)

_This is a tutorial for working with Bitcoin (and Lightning) that teaches direct interaction with the servers themselves, as the most robust and secure way to begin cryptocurrency work._

> NOTE: This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for use.

# Copyright

Unless otherwise noted, the contents of this repository are Copyright ©2018 by Christopher Allen [@ChristopherA](https://github.com/ChristopherA) \<ChristopherA@LifeWithAlacrity.com\> and are licensed [CC-BY](./LICENSE-CC-BY-4.0.md).

### Credits

| Role                | Names                                    |
| ------------------- | ---------------------------------------- |
| ***Lead Authors:*** | Christopher Allen [@ChristopherA](https://github.com/ChristopherA), Shannon Appelcline [@ShannonA](https://github.com/ShannonA) |
| ***Contributors:*** |                                          |
| ***Reviewers:***    | Glen Willem [@gwillem](https://github.com/gwillem) |
| ***Sponsors:***     | Blockstream Corporation                  |

## Introduction

The best way to learn deeply about bitcoin is to avoid GUIs (even bitcoin-qt), and instead learn it from the command line.

## Requirements

This tutorial assumes that you have some minimal background of how to use the command line interface. If not, there are many tutorials available, and I have one for Mac users at https://github.com/ChristopherA/intro-mac-command-line

## Files

* [0.0: Introduction to Programming with Bitcoin Core and Lightning](00_0_Introduction.md)

**PART ONE: PREPARING FOR BITCOIN**

* [1.0: Introducing Bitcoin](01_0_Introducing_Bitcoin.md)
* [2.0: Setting Up a Bitcoin-Core VPS](02_0_Setting_Up_a_Bitcoin-Core_VPS.md)
  * [2.1: Setting Up a Bitcoin-Core VPS by Hand](02_1_Setting_Up_a_Bitcoin-Core_VPS_by_Hand.md)
  * [2.2: Setting Up a Bitcoin-Core VPS with StackScript.md](02_2_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md)
    * [Linode_Bitcoin-Core_VPS_Setup.stackscript](02_2__Script_Linode_Setup.stackscript)
  * [2.3: Setting Up a Bitcoin-Core Machine via Other Means](02_3_Setting_Up_Bitcoin_Core_Other.md)

**PART TWO: BITCOIN-CLI**

* [3.0: Understanding Your Bitcoin Setup](03_0_Understanding_Your_Bitcoin_Setup.md)
  * [3.1: Verifying Your Bitcoin Setup](03_1_Verifying_Your_Bitcoin_Setup.md)
  * [3.2: Knowing Your Bitcoin Setup](03_2_Knowing_Your_Bitcoin_Setup.md)
  * [3.3: Setting Up Your Wallet](03_3_Setting_Up_Your_Wallet.md)
    * [Interlude: Using Command-Line Variables](03_3__Interlude_Using_Command-Line_Variables.md)
  * [3.4: Receiving a Transaction](03_4_Receiving_a_Transaction.md)
* [4.0: Sending Bitcoin Transactions](04_0_Sending_Bitcoin_Transactions.md)
  * [4.1: Sending Coins the Easy Way](04_1_Sending_Coins_The_Easy_Way.md)
  * [4.2: Creating a Raw Transaction](04_2_Creating_a_Raw_Transaction.md)
     * [Interlude: Using JQ](04_2__Interlude_Using_JQ.md)
  * [4.3: Creating a Raw Transaction with Named Arguments](04_3_Creating_a_Raw_Transaction_with_Named_Arguments.md)
  * [4.4: Sending Coins with Raw Transactions](04_4_Sending_Coins_with_a_Raw_Transaction.md)
     * [Interlude: Using Curl](04_4__Interlude_Using_Curl.md) — Needs Rewrite for New Section
  * [4.5: Sending Coins with Automated Raw Transactions](04_5_Sending_Coins_with_Automated_Raw_Transactions.md)
  * [4.6: Creating a Segwit Transaction](04_6_Creating_a_Segwit_Transaction) — Awaiting Better Integration of Segwit into CLI
* [5.0: Controlling Bitcoin Transactions](05_0_Controlling_Bitcoin_Transactions.md)
  * [5.1 Watching for Stuck Transactions](05_1_Watching_for_Stuck_Transactions.md)
  * [5.2: Resending a Transaction with RBF](05_2_Resending_a_Transaction_with_RBF.md)
  * [5.3: Funding a Transaction with CPFP](05_3_Funding_a_Transaction_with_CPFP.md)
* [6.0: Expanding Bitcoin Transactions](06_0_Expanding_Bitcoin_Transactions.md)
  * [6.1: Sending a Transaction with a Multsig](06_1_Sending_a_Transaction_to_a_Multisig.md)
  * [6.2: Spending a Transaction with a Multsig](06_2_Spending_a_Transaction_to_a_Multisig.md)
  * [6.3: Sending & Spending an Automated Multisig](06_3_Sending_an_Automated_Multisig.md)
  * [6.4: Sending a Transaction with a Locktime](06_4_Sending_a_Transaction_with_a_Locktime.md)
  * [6.5: Sending a Transaction with Data](06_5_Sending_a_Transaction_with_Data.md)

**PART THREE: BITCOIN SCRIPTING**

* [7.0: Introducing Bitcoin Scripts](07_0_Introducing_Bitcoin_Scripts.md)
  * [7.1: Understanding the Foundation of Transactions](07_1_Understanding_the_Foundation_of_Transactions.md)
  * [7.2: Running a Bitcoin Script](07_2_Running_a_Bitcoin_Script.md)
  * [7.3: Scripting a P2PKH](07_3_Scripting_a_P2PKH.md)
  * [7.4: Testing a Bitcoin Script](07_4_Testing_a_Bitcoin_Script.md)
* [8.0: Embedding Bitcoin Scripts in P2SH Transactions](08_0_Embedding_Bitcoin_Scripts_in_P2SH_Transactions.md)
  * [8.1: Understanding the Foundation of P2SH](08_1_Understanding_the_Foundation_of_P2SH.md)
  * [8.2: Building the Structure of P2SH](08_2_Building_the_Structure_of_P2SH.md)
  * [8.3: Running a Bitcoin Script with P2SH](08_3_Running_a_Bitcoin_Script_with_P2SH.md)
  * [8.4: Scripting a Multisig](08_4_Scripting_a_Multisig.md)
  * [8.5: Spending a P2SH Transaction](08_5_Spending_a_P2SH_Transaction.md) — Awaiting API Work
* [9.0: Empowering Timelock with Bitcoin Scripts](09_0_Empowering_Timelock_with_Bitcoin_Scripts.md)
  * [9.1: Understanding Timelock Options](09_1_Understanding_Timelock_Options.md)
  * [9.2: Using CLTV in Scripts](09_2_Using_CLTV_in_Scripts.md)
  * [9.3: Using CSV in Scripts](09_3_Using_CSV_in_Scripts.md)
* [10.0: Expanding Bitcoin Scripts](10_0_Expanding_Bitcoin_Scripts.md)
  * [10.1: Using Script Conditionals](10_1_Using_Script_Conditionals.md)
  * [10.2: Using Other Script Commands](10_2_Using_Other_Script_Commands.md)
* [11.0: Designing Real Bitcoin Scripts](11_0_Designing_Real_Bitcoin_Scripts.md)
  * [11.1: Writing Puzzles Scripts](11_1_Writing_Puzzle_Scripts.md)
  * [11.2: Writing Complex Multisig Scripts](11_2_Writing_Complex_Multisig_Scripts.md)
  * [11.3: Empowering Bitcoin with Scripts](11_3_Empowering_Bitcoin_with_Scripts.md)

**PART FOUR: LIGHTNING-CLI**

_This section is unwritten currently, and will probably involve chapter 14.0 expanding into at least two chapters._

* 12.0: Setting Up TOR
* 13.0: Setting Up Lightning
* 14.0: Using Lightning

Some good docs from one of the developers are here: https://diyhpl.us/wiki/transcripts/blockstream-webinars/2019-07-31-rusty-russell-getting-started-with-c-lightning/

**PART FIVE: BITCOIN & LIGHTNING PROGRAMMING**

_This section is currently a messy set of older writings which are being reorganized as listed below. We're not going to work through this section until we've got chapters 1-11 updated per our current notes, then 12-14 written. But, if you'd like to see what we have to date on programming in correlation with Bitcoin Core, please feel free._

* [15.0: Talking to Bitcoind with C](15_0_Talking_to_Bitcoind.md) — Needs Rewrite + Editing
  * [15.1: Accessing Bitcoind with C](15_1_Accessing_Bitcoind_with_C.md) — Needs Rewrite
  * [15.2: Programming Bitcoind with C](15_2_Programming_Bitcoind_with_C.md) — Needs Rewrite + Editing
  * [15.3: Receiving Bitcoind Notifications with C](15_3_Receiving_Bitcoind_Notifications_with_C.md) — Pending
* 16.0: Talking to Lightningd with C
* 17.0: Programming with LibWally
   * 17.1: Using secp256k1 Functions
   * 17.2: Writing Wallet Functions
   * 17.3: Integrating Libwally and Scripts
* 18.0: Talking to Bitcoind with Other Languages
  * [18.1: Accessing Bitcoind with_Node_JS](18_1_Accessing_Bitcoind_with_Other_Languages.md)  — Needs Rewrite + Editing
  * [18.2: Accessing Bitcoind with Python](18_2_Accessing_Bitcoind_with_Python.md)  — Needs Info form PR + Expansion + Editing
  * [18.3: Accessing Bitcoind with Java](18_3_Accessing_Bitcoind_with_Java.md) — Needs Rewrite + Editing

**APPENDICES**

Appendix I: Compiling Bitcoin

Appendix II: Using Bitcoin Regtest

* [A2.0: Setting Up a Bitcoin Regtest](A2_0_Setting_Up_a_Bitcoin_Regtest.md) — Needs Rewrite + Editing
   * [A2.1: Starting the Regtest](A2_1_Starting_the_Regtest.md) — Needs Rewrite + Editing
   * [A2.2: Mining with Regtest](A2_2_Mining_with_Regtest.md) — Needs Rewrite + Editing
   * [A2.3: Testing with Regtest](A2_3_Testing_with_Regtest.md) — Needs Rewrite + Editing
   * [why is Regtest important if you're compiling it yourself?]
   
## TODOs

The empty chapters above show some of our current plans for future work. Other plans, mainly drawn from old issues, can be found in [TODO.md](TODO.md)
