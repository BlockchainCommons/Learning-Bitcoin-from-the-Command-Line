# Learning Bitcoin (and Lightning) from the Command Line

Learning Bitcoin from the Command Line is a tutorial for working with Bitcoin (and Lightning) that teaches direct interaction with the servers themselves, as the most robust and secure way to begin cryptocurrency work.

> NOTE: This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for use.

_This tutorial assumes that you have some minimal background of how to use the command line interface. If not, there are many tutorials available, and I have one for Mac users at https://github.com/ChristopherA/intro-mac-command-line._

## Table of Contents

* [0.0: Introduction to Programming with Bitcoin Core and Lightning](00_0_Introduction.md)

### PART ONE: PREPARING FOR BITCOIN

**Status:** May need new concepts from 0.20.

* [1.0: Introducing Bitcoin](01_0_Introducing_Bitcoin.md)
* [2.0: Setting Up a Bitcoin-Core VPS](02_0_Setting_Up_a_Bitcoin-Core_VPS.md)
  * [2.1: Setting Up a Bitcoin-Core VPS by Hand](02_1_Setting_Up_a_Bitcoin-Core_VPS_by_Hand.md)
  * [2.2: Setting Up a Bitcoin-Core VPS with Bitcoin Standup](02_2_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md)
  * [2.3: Setting Up a Bitcoin-Core Machine via Other Means](02_3_Setting_Up_Bitcoin_Core_Other.md)

### PART TWO: USING BITCOIN-CLI

**Status:** Requires editing; requires check that it works in 0.20; requires concepts brought up to 0.20.

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
     * [Interlude: Using Curl](04_4__Interlude_Using_Curl.md)
  * [4.5: Sending Coins with Automated Raw Transactions](04_5_Sending_Coins_with_Automated_Raw_Transactions.md)
  * [4.6: Creating a Segwit Transaction](04_6_Creating_a_Segwit_Transaction.md)
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

## PART THREE: BITCOIN SCRIPTING

**Status:** Requires editing; requires check that it works in 0.20; requires concepts brought up to 0.20.

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

### PART FOUR: USING TOR

**Status:** Unwritten.

* [12.0: Using Tor](12_0_Using_Tor.md)
  * 12.1: Verifying Your Tor Setup
  * 12.2: Changing Your Bitcoin Hidden Services
  * 12.3: Adding SSH Hiddne Services
  
### PART FIVE: USING LIGHTNING-CLI

**Status:** Unwritten. Chapter 14 may expand into multiple chapters.

(At this point, I'm assuming that Lightning will be integrated into Standup, at which point we just need to tech how to use it at a pretty basic level.)

* 13.0: Understanding Lightning
  * 13.1: Verifying Your Lightning Setup
  * 13.2: Setting up a Channel
  * 13.3: Receiving a Transaction
* 14.0: Using Lightning
  * 14.1: Sending a Transaction
  * 14.X: Closing a Channel

> _Some good docs from one of the developers are here: https://diyhpl.us/wiki/transcripts/blockstream-webinars/2019-07-31-rusty-russell-getting-started-with-c-lightning/._

### PART SIX: PROGRAMMING WITH RPC

**Status:** This section is currently a messy set of older writings which are being reorganized as listed below. We're not going to work through this section until we've got chapters 1-11 updated per our current notes, then 12-14 written. But, if you'd like to see what we have to date on programming in correlation with Bitcoin Core, please feel free.

* [15.0: Talking to Bitcoind with C](15_0_Talking_to_Bitcoind.md) — Needs Rewrite + Editing
  * [15.1: Accessing Bitcoind with C](15_1_Accessing_Bitcoind_with_C.md) — Needs Rewrite
  * [15.2: Programming Bitcoind with C](15_2_Programming_Bitcoind_with_C.md) — Needs Rewrite + Editing
  * [15.3: Receiving Bitcoind Notifications with C](15_3_Receiving_Bitcoind_Notifications_with_C.md) — Pending
* 16.0: Programming with LibWally
   * 17.1: Getting ready for LibWally
   * 17.2: Using secp256k1 Functions
   * 17.3: Writing Wallet Functions
   * 17.4: Integrating Libwally and Scripts
* 17.0: Talking to Lightningd with C
* 18.0: Talking to Bitcoind with Other Languages
  * [18.1: Accessing Bitcoind with Go] — Unwritten
  * [18.2: Accessing Bitcoind with Java](18_2_Accessing_Bitcoind_with_Java.md)
  * [18.3: Accessing Bitcoind with_Node_JS](18_3_Accessing_Bitcoind_with_NodeJS.md)  — Needs Rewrite + Editing
  * [18.4: Accessing Bitcoind with Python](18_4_Accessing_Bitcoind_with_Python.md)
  * [18.5: Accessing Bitcoind with Rust](18_5_Accessing_Bitcoind_with_Rust.md)
  * [18.6: Accessing Bitcoind with Swift] — Unwritten
  
### APPENDICES

**Status:** Varied. Appendix I is recent that just needs editing for style; Appendix II needs a rewrite.

* [Appendix I: Compiling Bitcoin from Source](A1_0_Compiling_Bitcoin_from_Source.md)
* [Appendix II: Using Bitcoin Regtest](A2_0_Setting_Up_a_Bitcoin_Regtest.md) — Needs Rewrite + Editing
   * [A2.1: Starting the Regtest](A2_1_Starting_the_Regtest.md) — Needs Rewrite + Editing
   * [A2.2: Mining with Regtest](A2_2_Mining_with_Regtest.md) — Needs Rewrite + Editing
   * [A2.3: Testing with Regtest](A2_3_Testing_with_Regtest.md) — Needs Rewrite + Editing
   * [why is Regtest important if you're compiling it yourself?]
   
## Status - Work in Progress

Learning Bitcoin from the Command Line  is currently under active development and its writing in progress. Current chapters are functional, but need to be updated to more modern versions of Bitcoin-Core. Additional chapters also need to be written to fill out our intended scope.

The empty chapters above show some of our current plans for future work. Other plans, mainly drawn from old issues, can be found in [TODO.md](TODO.md)

Obviously, this work in progress should not be used for production tasks until it is completed and has had further testing and auditing.

## Origin, Authors, Copyright & Licenses

Unless otherwise noted (either in this [/README.md](./README.md) or in the file's header comments) the contents of this repository are Copyright © 2020 by Blockchain Commons, LLC, and are licensed under  [CC-BY](./LICENSE-CC-BY-4.0.md).

## Financial Support

*Learning Bitcoin from the Command Line* is a project of [Blockchain Commons](https://www.blockchaincommons.com/). We are proudly a "not-for-profit" social benefit corporation committed to open source & open development. Our work is funded entirely by donations and collaborative partnerships with people like you. Every contribution will be spent on building open tools, technologies, and techniques that sustain and advance blockchain and internet security infrastructure and promote an open web.

To financially support further development of `$projectname` and other projects, please consider becoming a Patron of Blockchain Commons through ongoing monthly patronage as a [GitHub Sponsor](https://github.com/sponsors/BlockchainCommons). You can also support Blockchain Commons with bitcoins at our [BTCPay Server](https://btcpay.blockchaincommons.com/).

## Contributing

We encourage public contributions through issues and pull requests! Please review [CONTRIBUTING.md](./CONTRIBUTING.md) for details on our development process. All contributions to this repository require a GPG signed [Contributor License Agreement](./CLA.md).

### Questions & Support

As an open-source, open-development community, Blockchain Commons does not have the resources to provide direct support of our projects. If you have questions or problems, please use this repository's [issues](./issues) feature. Unfortunately, we can not make any promises on response time.

If your company requires support to use our projects, please feel free to contact us directly about options. We may be able to offer you a contract for support from one of our contributors, or we might be able to point you to another entity who can offer the contractual support that you need.

### Credits

The following people directly contributed to this repository. You can add your name here by getting involved. The first step is learning how to contribute from our [CONTRIBUTING.md](./CONTRIBUTING.md) documentation.


| Name              | Role                | Github                                            | Email                                 | GPG Fingerprint                                    |
| ----------------- | ------------------- | ------------------------------------------------- | ------------------------------------- | -------------------------------------------------- |
| Christopher Allen | Lead Author | [@ChristopherA](https://github.com/ChristopherA) | \<ChristopherA@LifeWithAlacrity.com\> | FDFE 14A5 4ECB 30FC 5D22  74EF F8D3 6C91 3574 05ED |
| Shannon Appelcline |  Lead Author | [@shannona](https://github.com/shannona) | \<shannon.appelcline@gmail.com\> | 7EC6 B928 606F 27AD | 


Additional contributions are listed below:

| Role                | Names                                    |
| ------------------- | ---------------------------------------- |
| ***Contributors:*** | [Javier Vargas](https://github.com/javiervargas) (Java secction), [jodobear](https://github.com/jodobear) (Appendix I, Python section)                                   |
| ***Reviewers:***    | Glen Willem [@gwillem](https://github.com/gwillem) |
| ***Sponsors:***     | Blockstream Corporation                  |


## Responsible Disclosure

We want to keep all of our software safe for everyone. If you have discovered a security vulnerability, we appreciate your help in disclosing it to us in a responsible manner. We are unfortunately not able to offer bug bounties at this time.

We do ask that you offer us good faith and use best efforts not to leak information or harm any user, their data, or our developer community. Please give us a reasonable amount of time to fix the issue before you publish it. Do not defraud our users or us in the process of discovery. We promise not to bring legal action against researchers who point out a problem provided they do their best to follow the these guidelines.

### Reporting a Vulnerability

Please report suspected security vulnerabilities in private via email to ChristopherA@BlockchainCommons.com (do not use this email for support). Please do NOT create publicly viewable issues for suspected security vulnerabilities.

The following keys may be used to communicate sensitive information to developers:

| Name              | Fingerprint                                        |
| ----------------- | -------------------------------------------------- |
| Christopher Allen | FDFE 14A5 4ECB 30FC 5D22  74EF F8D3 6C91 3574 05ED |

You can import a key by running the following command with that individual’s fingerprint: `gpg --recv-keys "<fingerprint>"` Ensure that you put quotes around fingerprints that contain spaces.
