# Learning Bitcoin from the Command Line 2.1.0
### _by Christopher Allen and Shannon Appelcline_

![](https://www.blockchaincommons.com/images/projects/lbtc-screen.png)

Learning Bitcoin from the Command Line is a tutorial for working with Bitcoin (and Lightning) that teaches direct interaction with the servers themselves, as the most robust and secure way to begin cryptocurrency work.

> NOTE: This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for use.

_This tutorial assumes that you have some minimal background of how to use the command line interface. If not, there are many tutorials available, and I have one for Mac users at https://github.com/ChristopherA/intro-mac-command-line._

## Translations

* [Portuguese](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/tree/portuguese-translation/pt/README.md) — v2.0.1 translation
* [Spanish](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/tree/spanish-translation/es/README.md) - v2.0.1 translation

If you'd like to make your own translation, please see [Contributing](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/tree/master#contributing), below.

## Table of Contents

### PART ONE: PREPARING FOR BITCOIN

**Status:** Finished. Updated for 0.20.

* [1.0: Introduction to Programming with Bitcoin Core and Lightning](01_0_Introduction.md)
    * [Interlude: Introducing Bitcoin](01_1_Introducing_Bitcoin.md)
* [2.0: Setting Up a Bitcoin-Core VPS](02_0_Setting_Up_a_Bitcoin-Core_VPS.md)
  * [2.1: Setting Up a Bitcoin-Core VPS with Bitcoin Standup](02_1_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md)
  * [2.2: Setting Up a Bitcoin-Core Machine via Other Means](02_2_Setting_Up_Bitcoin_Core_Other.md)

### PART TWO: USING BITCOIN-CLI

**Status:** Finished. Updated for 0.20.

* [3.0: Understanding Your Bitcoin Setup](03_0_Understanding_Your_Bitcoin_Setup.md)
  * [3.1: Verifying Your Bitcoin Setup](03_1_Verifying_Your_Bitcoin_Setup.md)
  * [3.2: Knowing Your Bitcoin Setup](03_2_Knowing_Your_Bitcoin_Setup.md)
  * [3.3: Setting Up Your Wallet](03_3_Setting_Up_Your_Wallet.md)
    * [Interlude: Using Command-Line Variables](03_3__Interlude_Using_Command-Line_Variables.md)
  * [3.4: Receiving a Transaction](03_4_Receiving_a_Transaction.md)
  * [3.5: Understanding the Descriptor](03_5_Understanding_the_Descriptor.md)
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
* [6.0: Expanding Bitcoin Transactions with Multisigs](06_0_Expanding_Bitcoin_Transactions_Multisigs.md)
  * [6.1: Sending a Transaction with a Multisig](06_1_Sending_a_Transaction_to_a_Multisig.md)
  * [6.2: Spending a Transaction with a Multisig](06_2_Spending_a_Transaction_to_a_Multisig.md)
  * [6.3: Sending & Spending an Automated Multisig](06_3_Sending_an_Automated_Multisig.md)
* [7.0: Expanding Bitcoin Transactions with PSBTs](07_0_Expanding_Bitcoin_Transactions_PSBTs.md)
  * [7.1: Creating a Partially Signed Bitcoin Transaction](07_1_Creating_a_Partially_Signed_Bitcoin_Transaction.md)
  * [7.2: Using a Partially Signed Bitcoin Transaction](07_2_Using_a_Partially_Signed_Bitcoin_Transaction.md)
  * [7.3: Integrating with Hardware Wallets](07_3_Integrating_with_Hardware_Wallets.md)
* [8.0: Expanding Bitcoin Transactions in Other Ways](08_0_Expanding_Bitcoin_Transactions_Other.md)
  * [8.1: Sending a Transaction with a Locktime](08_1_Sending_a_Transaction_with_a_Locktime.md)
  * [8.2: Sending a Transaction with Data](08_2_Sending_a_Transaction_with_Data.md)

### PART THREE: BITCOIN SCRIPTING

**Status:** Finished. Updated for 0.20 and btcdeb.

* [9.0: Introducing Bitcoin Scripts](09_0_Introducing_Bitcoin_Scripts.md)
  * [9.1: Understanding the Foundation of Transactions](09_1_Understanding_the_Foundation_of_Transactions.md)
  * [9.2: Running a Bitcoin Script](09_2_Running_a_Bitcoin_Script.md)
  * [9.3: Testing a Bitcoin Script](09_3_Testing_a_Bitcoin_Script.md)
  * [9.4: Scripting a P2PKH](09_4_Scripting_a_P2PKH.md)
  * [9.5: Scripting a P2WPKH](09_5_Scripting_a_P2WPKH.md)
* [10.0: Embedding Bitcoin Scripts in P2SH Transactions](10_0_Embedding_Bitcoin_Scripts_in_P2SH_Transactions.md)
  * [10.1: Understanding the Foundation of P2SH](10_1_Understanding_the_Foundation_of_P2SH.md)
  * [10.2: Building the Structure of P2SH](10_2_Building_the_Structure_of_P2SH.md)
  * [10.3: Running a Bitcoin Script with P2SH](10_3_Running_a_Bitcoin_Script_with_P2SH.md)
  * [10.4: Scripting a Multisig](10_4_Scripting_a_Multisig.md)
  * [10.5: Scripting a Segwit Script](10_5_Scripting_a_Segwit_Script.md)
  * [10.6: Spending a P2SH Transaction](10_6_Spending_a_P2SH_Transaction.md)
* [11.0: Empowering Timelock with Bitcoin Scripts](11_0_Empowering_Timelock_with_Bitcoin_Scripts.md)
  * [11.1: Understanding Timelock Options](11_1_Understanding_Timelock_Options.md)
  * [11.2: Using CLTV in Scripts](11_2_Using_CLTV_in_Scripts.md)
  * [11.3: Using CSV in Scripts](11_3_Using_CSV_in_Scripts.md)
* [12.0: Expanding Bitcoin Scripts](12_0_Expanding_Bitcoin_Scripts.md)
  * [12.1: Using Script Conditionals](12_1_Using_Script_Conditionals.md)
  * [12.2: Using Other Script Commands](12_2_Using_Other_Script_Commands.md)
* [13.0: Designing Real Bitcoin Scripts](13_0_Designing_Real_Bitcoin_Scripts.md)
  * [13.1: Writing Puzzles Scripts](13_1_Writing_Puzzle_Scripts.md)
  * [13.2: Writing Complex Multisig Scripts](13_2_Writing_Complex_Multisig_Scripts.md)
  * [13.3: Empowering Bitcoin with Scripts](13_3_Empowering_Bitcoin_with_Scripts.md)

### PART FOUR: PRIVACY

**Status:** Finished.

* [14.0: Using Tor](14_0_Using_Tor.md)
  * [14.1: Verifying Your Tor Setup](14_1_Verifying_Your_Tor_Setup.md)
  * [14.2: Changing Your Bitcoin Hidden Services](14_2_Changing_Your_Bitcoin_Hidden_Services.md)
  * [14.3: Adding SSH Hidden Services](14_3_Adding_SSH_Hidden_Services.md)

* [15.0: Using i2p](15_0_Using_i2p.md)
  * [15.1: Bitcoin Core as an I2P (Invisible Internet Project) service](15_1_i2p_service.md)

### PART FIVE: PROGRAMMING WITH RPC

**Status:** Finished.

* [16.0: Talking to Bitcoind with C](16_0_Talking_to_Bitcoind.md)
  * [16.1: Accessing Bitcoind in C with RPC Libraries](16_1_Accessing_Bitcoind_with_C.md)
  * [16.2: Programming Bitcoind in C with RPC Libraries](16_2_Programming_Bitcoind_with_C.md)
  * [16.3: Receiving Notifications in C with ZMQ Libraries](16_3_Receiving_Bitcoind_Notifications_with_C.md)
* [17.0: Programming Bitcoin with Libwally](17_0_Programming_with_Libwally.md)
   * [17.1: Setting Up Libwally](17_1_Setting_Up_Libwally.md)
   * [17.2: Using BIP39 in Libwally](17_2_Using_BIP39_in_Libwally.md)
   * [17.3: Using BIP32 in Libwally](17_3_Using_BIP32_in_Libwally.md)
   * [17.4: Using PSBTs in Libwally](17_4_Using_PSBTs_in_Libwally.md)
   * [17.5: Using Scripts in Libwally](17_5_Using_Scripts_in_Libwally.md)
   * [17.6: Using Other Functions in Libwally](17_6_Using_Other_Functions_in_Libwally.md)
   * [17.7: Integrating Libwally and Bitcoin-CLI](17_7_Integrating_Libwally_and_Bitcoin-CLI.md)
* [18.0: Talking to Bitcoind with Other Languages](18_0_Talking_to_Bitcoind_Other.md)
  * [18.1: Accessing Bitcoind with Go](18_1_Accessing_Bitcoind_with_Go.md)
  * [18.2: Accessing Bitcoind with Java](18_2_Accessing_Bitcoind_with_Java.md)
  * [18.3: Accessing Bitcoind with Node JS](18_3_Accessing_Bitcoind_with_NodeJS.md)
  * [18.4: Accessing Bitcoind with Python](18_4_Accessing_Bitcoind_with_Python.md)
  * [18.5: Accessing Bitcoind with Rust](18_5_Accessing_Bitcoind_with_Rust.md)
  * [18.6: Accessing Bitcoind with Swift](18_6_Accessing_Bitcoind_with_Swift.md)

### PART SIX: USING LIGHTNING-CLI

**Status:** Finished.

* [19.0: Understanding Your Lightning Setup](19_0_Understanding_Your_Lightning_Setup.md)
  * [19.1: Verifying Your c-lightning Setup](19_1_Verifying_Your_Lightning_Setup.md)
  * [19.2: Knowing Your c-lightning Setup](19_2_Knowing_Your_lightning_Setup.md)
     * [Interlude: Accessing a Second Lightning Node](19_2__Interlude_Accessing_a_Second_Lightning_Node.md)
  * [19.3: Creating a Lightning Channel](19_3_Setting_Up_a_Channel.md)
* [20.0: Using Lightning](20_0_Using_Lightning.md)
  * [20.1: Generating a Payment Request](20_1_Generate_a_Payment_Request.md)
  * [20.2: Paying an Invoice](20_2_Paying_a_Invoice.md)
  * [20.3: Closing a Lighnting Channel]((20_3_Closing_a_Channel.md))
  * [20.4: Expanding the Lightning Network](20_4_Lightning_Network_Review.md)

### APPENDICES

**Status:** Finished.

* [Appendices](A0_Appendices.md)
  * [Appendix I: Understanding Bitcoin Standup](A1_0_Understanding_Bitcoin_Standup.md)
  * [Appendix II: Compiling Bitcoin from Source](A2_0_Compiling_Bitcoin_from_Source.md)
  * [Appendix III: Using Bitcoin Regtest](A3_0_Using_Bitcoin_Regtest.md)

## Status - Beta

v2.1.0 of **Learning Bitcoin from the Command Line** is feature complete and has undergone full editing and integration. It is ready for learning

We are also tentatively considering what we could include in a [v3.0](TODO-30.md) of the course. If you'd like to support work of that sort, become a [GitHub Sponsor](https://github.com/sponsors/BlockchainCommons) or support us at our [BTCPay Server](https://btcpay.blockchaincommons.com/), and let us know that **Learning Bitcoin** was the reason why.
### Version History

#### 2.1.0 (October 12, 2021)

* New chapter 15  (i2p).
* Added fixes to wallet usage from 0.21
* Updated install to new Bitcoin Scripts for 22.0
* Incorporated numerous fixes revealed by first translation projects

#### 2.0.1 (June 15, 2021)

* Numerous small patches following 2.0
* Intended as baseline for translations (2.2.0), but 2.1.0 material on i2p slipped in

#### 2.0.0 (November 3, 2020)

* Second major release of Learning Bitcoin
* Added material on Segwit, Tor, hardware wallets, lightning, regtest
* Completely previous unfinished work on programming using a variety of languages

#### 1.0.0 (pre-2020)

* Original, extensive but incomplete version
* Contained comprehensive sections on setup, bitcoin-cli, and scripting

## Origin, Authors, Copyright & Licenses

Unless otherwise noted (either in this [/README.md](./README.md) or in the file's header comments) the contents of this repository are Copyright © 2020 by Blockchain Commons, LLC, and are licensed under  [CC-BY](./LICENSE-CC-BY-4.0.md).

## Financial Support

*Learning Bitcoin from the Command Line* is a project of [Blockchain Commons](https://www.blockchaincommons.com/). We are proudly a "not-for-profit" social benefit corporation committed to open source & open development. Our work is funded entirely by donations and collaborative partnerships with people like you. Every contribution will be spent on building open tools, technologies, and techniques that sustain and advance blockchain and internet security infrastructure and promote an open web.

To financially support further development of *Learning Bitcoin from the Command Line* and other projects, please consider becoming a Patron of Blockchain Commons through ongoing monthly patronage as a [GitHub Sponsor](https://github.com/sponsors/BlockchainCommons). You can also support Blockchain Commons with bitcoins at our [BTCPay Server](https://btcpay.blockchaincommons.com/).

## Contributing

We encourage public contributions through issues and pull requests! Please review [CONTRIBUTING.md](./CONTRIBUTING.md) for details on our development process. All contributions to this repository require a GPG signed [Contributor License Agreement](./CLA.md).

if you would like to provide a translation of Learning Bitcoin into another language, please additionally see [TRANSLATING.md](./TRANSLATING.md).

### Discussions

The best place to talk about Blockchain Commons and its projects is in our GitHub Discussions areas.

[**Blockchain Commons Discussions**](https://github.com/BlockchainCommons/Community/discussions). For developers, interns, and patrons of Blockchain Commons, please use the discussions area of the [Community repo](https://github.com/BlockchainCommons/Community) to talk about general Blockchain Commons issues, the intern program, or topics other than the [Gordian System](https://github.com/BlockchainCommons/Gordian/discussions) or the [wallet standards](https://github.com/BlockchainCommons/AirgappedSigning/discussions), each of which have their own discussion areas.

### Other Questions & Problems

As an open-source, open-development community, Blockchain Commons does not have the resources to provide direct support of our projects. Please consider the discussions area as a locale where you might get answers to questions. Alternatively, please use this repository's [issues](./issues) feature. Unfortunately, we can not make any promises on response time.

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
| ***Contributors:*** | [gg2001](https://github.com/gg2001) (Go, Node.js sections), [gorazdko](https://github.com/gorazdko) (Rust section), [Javier Vargas](https://github.com/javiervargas) (C, Java, Lightning, Tor sections), [jodobear](https://github.com/jodobear) (Appendix: Compiling Bitcoin, Python section), [Prayank](    https://github.com/prayank23) (i2p sections)                               |
| ***Reviewers:***    | Glen Willem [@gwillem](https://github.com/gwillem) |
| ***Sponsors:***     | Blockstream Corporation                  |

### Translation Credits

Thanks to the volunteers who spent extensive time writing and reviewing other-language translations of the original English-language course.

#### Portuguese Translation

| Name              | Role                | Github                                            | 
| ----------------- | ------------------- | ------------------------------------------------- | 
| Namcios | Translator & Reviewer | [@namcios](https://github.com/namcios) | 
| Korea | Translator & Reviewer | [@KoreaComK](https://github.com/KoreaComK) | 
| Luke Pavsky | Translator & Reviewer | [@lukedevj](https://github.com/lukedevj) | 
| hgrams | Translator & Reviewer | [@hgrams](https://github.com/hgrams) | 

#### Spanish Translation

 Name | Role | GitHub |
| ---------- | -------- | ------------ |
| Ian Culp | Translator & Reviewer | [@icculp](https://github.com/icculp) |
| Maxi Goyheneche | Translator | [@maxcrowar](https://github.com/maxcrowar) |
| Said Rahal | Translator | [@srahalh](https://github.com/srahalh) |
| César A. Vallero | Translator & Reviewer | [@csralvall](https://github.com/csralvall) |
| Javier Vargas | Translator & Reviewer | [@javiervargas](https://github.com/javiervargas) |

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
