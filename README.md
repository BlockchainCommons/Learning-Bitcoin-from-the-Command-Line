# Learning Bitcoin from the Command Line 3.0 (in process)
### _by Christopher Allen and Shannon Appelcline_

![](https://www.blockchaincommons.com/images/projects/lbtc-screen.png)

---

***In 2026, Learning Bitcoin from the Command Line is being brought up to date with a new 3.0 edition. If you want to see the work in progress, visit our [`lbtcftcl-v3.0` branch](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/tree/lbtcftcl-v3.0), which is already much more up-to-date than v2.2.2. But be aware it is being worked on through the end of the year, and so will not be complete or entirely coherent before then.***

---

Learning Bitcoin from the Command Line is a tutorial for working with Bitcoin (and Lightning) that teaches direct interaction with the servers themselves, as the most robust and secure way to begin cryptocurrency work.

_This tutorial assumes that you have some minimal background of how to use the command line interface. If not, there are many tutorials available, and I have one for Mac users at https://github.com/ChristopherA/intro-mac-command-line._

_You may also be interested in our newer [Learning FROST from the Command Line course](https://learningfrost.blockchaincommons.com/)._

## Translations

* [Portuguese](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/tree/portuguese-translation/pt/README.md) — v2.0.1 translation
* [Spanish](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/tree/spanish-translation/es/README.md) - v2.0.1 translation

If you'd like to make your own translation, please see [Contributing](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/tree/master#contributing), below.

## Table of Contents

### PART ONE: PREPARING FOR BITCOIN

* [1.0: Introduction to Programming with Bitcoin Core and Lightning](docs/01_0_Introduction.md)
    * [Interlude: Introducing Bitcoin](docs/01_1_Introducing_Bitcoin.md)
* [2.0: Setting Up a Bitcoin-Core VPS](docs/02_0_Setting_Up_a_Bitcoin-Core_VPS.md)
  * [2.1: Setting Up a Bitcoin-Core VPS with Bitcoin Standup](docs/02_1_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md)
  * [2.2: Setting Up a Bitcoin-Core Machine via Other Means](docs/02_2_Setting_Up_Bitcoin_Core_Other.md)

### PART TWO: SENDING TRANSACTIONS

* [3.0: Preparing Your Bitcoin Wallet](docs/03_0_Preparing_Your_Bitcoin_Wallet.md)
  * [3.1: Verifying Your Bitcoin Setup](docs/03_1_Verifying_Your_Bitcoin_Setup.md)
  * [3.2: Knowing Your Bitcoin Setup](docs/03_2_Knowing_Your_Bitcoin_Setup.md)
  * [3.3: Setting Up Your Wallet](docs/03_3_Setting_Up_Your_Wallet.md) 
  * [3.4: Understanding the Descriptor Wallet](docs/03_4_Understanding_the_Descriptor_Wallet.md)
* [4.0: Preparing Your Bitcoin Addresses](docs/04_0_Preparing_Your_Bitcoin_Addresses.md)
  * [4.1: Understanding the Address](docs/04_1_Understanding_the_Address.md)
  * [4.2: Integrating Addresses and Descriptors](docs/04_2_Integrating_Addresses_Descriptors.md)
  * [4.3: Creating QR Codes for Addresses](docs/04_3Creating_QR_Codes_for_Addresses.md)
  * [4.4: Receiving a Transaction](docs/04_4_Receiving_a_Transaction.md)
    * [Interlude: Using Command-Line Variables](docs/04_4a_Interlude_Using_Command-Line_Variables.md)
* [5.0: Sending Bitcoin Transactions](docs/05_0_Sending_Bitcoin_Transactions.md) 
  * [5.1: Sending Coins the Easy Way](docs/05_1_Sending_Coins_The_Easy_Way.md) 
  * [5.2: Creating a Raw Transaction](docs/05_2_Creating_a_Raw_Transaction.md) 
     * [Interlude: Using JQ](docs/05_2a_Interlude_Using_JQ.md) 
  * [5.3: Creating a Raw Transaction with Named Arguments](docs/05_3_Creating_a_Raw_Transaction_with_Named_Arguments.md)
  * [5.4: Sending Coins with Raw Transactions](docs/05_4_Sending_Coins_with_a_Raw_Transaction.md)
     * [Interlude: Using Curl](docs/05_4a_Interlude_Using_Curl.md)
  * [5.5: Sending Coins with Automated Raw Transactions](docs/05_5_Sending_Coins_with_Automated_Raw_Transactions.md)
  * [5.6: Sending Other Types of Transactions](docs/05_6_Sending_Coins_to_Other_Addresses.md) 
* [6.0: Controlling Bitcoin Transactions](docs/06_0_Controlling_Bitcoin_Transactions.md)
  * [6.1 Watching for Stuck Transactions](docs/06_1_Watching_for_Stuck_Transactions.md)
  * [6.2: Resending a Transaction with RBF](docs/06_2_Resending_a_Transaction_with_RBF.md)
  * [6.3: Funding a Transaction with CPFP](docs/06_3_Funding_a_Transaction_with_CPFP.md)

### PART THREE: EXPANDING BITCOIN

* [7.0: Expanding Bitcoin Transactions with Multisigs](docs/07_0_Expanding_Bitcoin_Transactions_Multisigs.md)
  * [7.1: Creating a Multisig with Public Keys](docs/07_1_Creating_Multisig_Public_Keys.md)
  * [7.2: Creating a Multisig with Descriptors](docs/07_2_Creating_Multisig_Descriptors.md)
  * [7.3: Spending a Transaction with a Multisig](docs/07_3_Spending_a_Transaction_to_a_Multisig.md)
* [8.0: Expanding Bitcoin Transactions with PSBTs](docs/08_0_Expanding_Bitcoin_Transactions_PSBTs.md)
  * [8.1: Spending a Multisig with a PSBT[(08_1_Spending_a_Multisig_with_a_PSBT.md)
  * [8.2: Understanding the PSBT](docs/08_2_Understanding_the_PSBT.md)
  * [8.3: Using a Partially Signed Bitcoin Transaction](docs/08_3_Using_a_Partially_Signed_Bitcoin_Transaction.md)
  * [8.4: Creating Animated QR Codes](docs/08_4_Creating_Animated_QR_Codes.md)

[in progress]

* [8.0: Expanding Bitcoin Transactions in Other Ways](docs/08_0_Expanding_Bitcoin_Transactions_Other.md)
  * [8.1: Sending a Transaction with a Locktime](docs/08_1_Sending_a_Transaction_with_a_Locktime.md)
  * [8.2: Sending a Transaction with Data](docs/08_2_Sending_a_Transaction_with_Data.md) [**TO UPDATE**]

### PART FOUR: IMPORTING & EXPORTING YOUR SECRETS

### PART FIVE: SCRIPTING WITH BITCOIN

* [11.0: Introducing Bitcoin Scripts](docs/11_0_Introducing_Bitcoin_Scripts.md)
  * [11.1: Understanding the Foundation of Transactions](docs/11_1_Understanding_the_Foundation_of_Transactions.md)
  * [11.2: Running a Bitcoin Script](docs/11_2_Running_a_Bitcoin_Script.md)
  * [11.3: Testing a Bitcoin Script](docs/11_3_Testing_a_Bitcoin_Script.md)
  * [11.4: Scripting a P2PKH](docs/11_4_Scripting_a_P2PKH.md)
  * [11.5: Scripting a P2WPKH](docs/11_5_Scripting_a_P2WPKH.md)
* [12.0: Embedding Bitcoin Scripts in P2SH Transactions](docs/12_0_Embedding_Bitcoin_Scripts_in_P2SH_Transactions.md)
  * [12.1: Understanding the Foundation of P2SH](docs/12_1_Understanding_the_Foundation_of_P2SH.md)
  * [12.2: Building the Structure of P2SH](docs/12_2_Building_the_Structure_of_P2SH.md)
  * [12.3: Running a Bitcoin Script with P2SH](docs/12_3_Running_a_Bitcoin_Script_with_P2SH.md)
  * [12.4: Scripting a Multisig](docs/12_4_Scripting_a_Multisig.md)
  * [12.5: Scripting a Segwit Script](docs/12_5_Scripting_a_Segwit_Script.md)
  * [12.6: Spending a P2SH Transaction](docs/12_6_Spending_a_P2SH_Transaction.md)
* [13.0: Empowering Timelock with Bitcoin Scripts](docs/13_0_Empowering_Timelock_with_Bitcoin_Scripts.md)
  * [13.1: Understanding Timelock Options](docs/13_1_Understanding_Timelock_Options.md) [**TO UPDATE**]
  * [13.2: Using CLTV in Scripts](docs/13_2_Using_CLTV_in_Scripts.md)
  * [13.3: Using CSV in Scripts](docs/13_3_Using_CSV_in_Scripts.md)
* [14.0: Expanding Bitcoin Scripts](docs/14_0_Expanding_Bitcoin_Scripts.md)
  * [14.1: Using Script Conditionals](docs/14_1_Using_Script_Conditionals.md)
  * [14.2: Using Other Script Commands](docs/14_2_Using_Other_Script_Commands.md)
* [15.0: Improving Bitcoin Scripts with Taproot](docs/15_0_Improving_Bitcoin_Scripts_with_Taproot.md) [**TO WRITE**]
   * SECTIONS TBD, BASED ON WHAT'S IN NEW CHAPTERS 9 + 10
* [16.0: Designing Real Bitcoin Scripts](docs/16_0_Designing_Real_Bitcoin_Scripts.md)
  * [16.1: Writing Puzzles Scripts](docs/16_1_Writing_Puzzle_Scripts.md)
  * [16.2: Writing Complex Multisig Scripts](docs/16_2_Writing_Complex_Multisig_Scripts.md)
  * [16.3: Empowering Bitcoin with Scripts](docs/16_3_Empowering_Bitcoin_with_Scripts.md)

### PART SIX: UPGRADING TO TAPROOT

* [9.0: Expanding Bitcoin Transactions with Schnorr](docs/09_0_Expanding_Bitcoin_Transactions_with_Schnorr.md) [**TO WRITE**]
  * [9.1: Understanding Schnorr & Taproot](docs/09_1_Understanding_Schnorr_Taproot.md) [**TO WRITE**]
  * [9.2: Sending Schnorr Signed Transactions](docs/09_2_Sending_Schnorr_Signed_Transactions.md) [**TO WRITE**]
  * [9.3: Sending a Schnorr Multisig](docs/09_3_Sending_a_Schnorr_Multisig.md) [**TO WRITE**]    
  * [9.4: Sending FROST Signed Transactions](docs/09_4_Sending_FROST_Signed_Transactions.md) [**TO WRITE**]
* [10.0: Expanding Bitcoin Transactions with Taproot](docs/10_0_Expanding_Bitcoin_Transactions_with_Taproot.md) [**TO WRITE**]
  * [10.1: Sending Basic Taproot Transactions](docs/10_1_Sending_Basic_Taproot_Transactions.md) [**TO WRITE**]
  * [10.2: Sending Advanced Taproot Transactions](docs/10_2_Sending_Advanced_Taproot_Transactions.md) [**TO WRITE**]

### PART SEVEN: MAINTAINING PRIVACY

* [17.0: Using Tor](docs/16_0_Using_Tor.md)
  * [17.1: Verifying Your Tor Setup](docs/17_1_Verifying_Your_Tor_Setup.md)
  * [17.2: Changing Your Bitcoin Hidden Services](docs/17_2_Changing_Your_Bitcoin_Hidden_Services.md)
  * [17.3: Adding SSH Hidden Services](docs/17_3_Adding_SSH_Hidden_Services.md)

* [18.0: Using i2p](docs/18_0_Using_i2p.md)
  * [18.1: Bitcoin Core as an I2P (Invisible Internet Project) service](docs/18_1_i2p_service.md)

[TODO: CONSIDER IF CJDNS Network FITS IN HERE, possibly as 18.2, with the section renamed "Other Privacy Options" or something]

---

_Everything over this line is on the backburner currently; files are being labeled P1, P2, P3, and L1, L2 and we'll figure out what to do with them later._

### PART FIVE: PROGRAMMING WITH RPC

**Status:** Finished.

* [P1.0: Talking to Bitcoind with C](docs/P1_0_Talking_to_Bitcoind.md)
  * [P1.1: Accessing Bitcoind in C with RPC Libraries](docs/P1_1_Accessing_Bitcoind_with_C.md)
  * [P1.2: Programming Bitcoind in C with RPC Libraries](docs/P1_2_Programming_Bitcoind_with_C.md)
  * [P1.3: Receiving Notifications in C with ZMQ Libraries](docs/P1_3_Receiving_Bitcoind_Notifications_with_C.md)
* [P2.0: Programming Bitcoin with Libwally](docs/P2_0_Programming_with_Libwally.md)
   * [P2.1: Setting Up Libwally](docs/P2_1_Setting_Up_Libwally.md)
   * [P2.2: Using BIP39 in Libwally](docs/P2_2_Using_BIP39_in_Libwally.md)
   * [P2.3: Using BIP32 in Libwally](docs/P2_3_Using_BIP32_in_Libwally.md)
   * [P2.4: Using PSBTs in Libwally](docs/P2_4_Using_PSBTs_in_Libwally.md)
   * [P2.5: Using Scripts in Libwally](docs/P2_5_Using_Scripts_in_Libwally.md)
   * [P2.6: Using Other Functions in Libwally](docs/P2_6_Using_Other_Functions_in_Libwally.md)
   * [P2.7: Integrating Libwally and Bitcoin-CLI](docs/P2_7_Integrating_Libwally_and_Bitcoin-CLI.md)
* [P3.0: Talking to Bitcoind with Other Languages](docs/P3_0_Talking_to_Bitcoind_Other.md)
  * [P3.1: Accessing Bitcoind with Go](docs/P3_1_Accessing_Bitcoind_with_Go.md)
  * [P3.2: Accessing Bitcoind with Java](docs/P3_2_Accessing_Bitcoind_with_Java.md)
  * [P3.3: Accessing Bitcoind with Node JS](docs/P3_3_Accessing_Bitcoind_with_NodeJS.md)
  * [P3.4: Accessing Bitcoind with Python](docs/P3_4_Accessing_Bitcoind_with_Python.md)
  * [P3.5: Accessing Bitcoind with Rust](docs/P3_5_Accessing_Bitcoind_with_Rust.md)
  * [P3.6: Accessing Bitcoind with Swift](docs/P3_6_Accessing_Bitcoind_with_Swift.md)

### PART SIX: USING LIGHTNING-CLI

**Status:** Finished.

* [L1.0: Understanding Your Lightning Setup](docs/L1_0_Understanding_Your_Lightning_Setup.md)
  * [L1.1: Verifying Your core lightning Setup](docs/L1_1_Verifying_Your_Lightning_Setup.md)
  * [L1.2: Knowing Your core lightning Setup](docs/L1_2_Knowing_Your_lightning_Setup.md)
     * [Interlude: Accessing a Second Lightning Node](docs/L1_2a_Interlude_Accessing_a_Second_Lightning_Node.md)
  * [L1.3: Creating a Lightning Channel](docs/L1_3_Setting_Up_a_Channel.md)
* [L2.0: Using Lightning](docs/L2_0_Using_Lightning.md)
  * [L2.1: Generating a Payment Request](docs/L2_1_Generate_a_Payment_Request.md)
  * [L2.2: Paying an Invoice](docs/L2_2_Paying_a_Invoice.md)
  * [L2.3: Closing a Lighnting Channel](docs/(L2_3_Closing_a_Channel.md))
  * [L2.4: Expanding the Lightning Network](docs/L2_4_Lightning_Network_Review.md)

### APPENDICES

**Status:** Finished.

* [Appendices](docs/A0_Appendices.md)
  * [Appendix I: Understanding Bitcoin Standup](docs/A1_0_Understanding_Bitcoin_Standup.md)
  * [Appendix II: Compiling Bitcoin from Source](docs/A2_0_Compiling_Bitcoin_from_Source.md)
  * [Appendix III: Using Bitcoin Regtest](docs/A3_0_Using_Bitcoin_Regtest.md)

## Status - Beta

v2.2.2 of **Learning Bitcoin from the Command Line** is feature complete and has undergone full editing and integration. It is ready for learning. However, it is out-of-date, focused on Bitcoin Core 22.0. We are currently working on [v3.0](docs/TODO.md) of the course, for release toward the end of 2026. If you'd like to support work of that sort, become a [GitHub Sponsor](https://github.com/sponsors/BlockchainCommons) or support us at our [BTCPay Server](https://btcpay.blockchaincommons.com/), and let us know that **Learning Bitcoin** was the reason why.

### Version History

#### 2.2.2 (January 22, 2026)

* Updated §2.1 and Appendix 1 to accomodate recent updates to [Bitcoin-Standup-Scripts](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts) as well as changes to the Linode interface.

#### 2.2.1 (January 15, 2026)

* Incorporation of PRs and Issues, to close out previous iteration of course, in prep for big 3.0 update.

#### 2.2.0 (November 17, 2021)

* [Portuguese translation](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/tree/master/pt)
* [Spanish translation](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/tree/master/es)

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

[**Blockchain Commons Discussions**](https://github.com/BlockchainCommons/Community/discussions). For developers, interns, and patrons of Blockchain Commons, please use the discussions area of the [Community repo](https://github.com/BlockchainCommons/Community) to talk about general Blockchain Commons issues, the intern program, or topics other than those covered by the [Gordian Developer Community](https://github.com/BlockchainCommons/Gordian-Developer-Community/discussions) or the 
[Gordian User Community](https://github.com/BlockchainCommons/Gordian/discussions).'

### Other Questions & Problems

As an open-source, open-development community, Blockchain Commons does not have the resources to provide direct support of our projects. Please consider the discussions area as a locale where you might get answers to questions. Alternatively, please use this repository's [issues](../../issues) feature. Unfortunately, we can not make any promises on response time.

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
| ***Contributors:*** | [gg2001](https://github.com/gg2001) (Go, Node.js sections), [gorazdko](https://github.com/gorazdko) (Rust section), [Javier Vargas](https://github.com/javiervargas) (C, Java, Lightning, Tor sections), [jodobear](https://github.com/jodobear) (Appendix: Compiling Bitcoin, Python section), [Prayank](docs/    https://github.com/prayank23) (i2p sections)                               |
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
