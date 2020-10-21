# Chapter One: Introduction to Learning Bitcoin Core (& Lightning) from the Command Line

## Introduction

The ways that we make payments for goods and services has been changing dramatically over the last several decades. Where once all transactions were conducted through cash or checks, now various electronic payment methods are the norm. However, most of these electronic payments still occur through centralized systems, where credit card companies, banks, or even internet-based financial institutions like Paypal keep long, individually correlated lists of transactions and have the power to censor transactions that they don't like.

These centralization risks were some of the prime catalysts behind the creation of cryptocurrencies, the first and most successful of which is Bitcoin. Bitcoin offers pseudonymity; it makes it difficult to correlate transactions; and it makes censorship by individual entities all but impossible. These advantages have made it one of the quickest growing currencies in the world. That growth in turn has made Bitcoin into a going concern among entrepreneurs and developers, eager to create new services for the Bitcoin community.

If you're one of those entrepreneurs or developers, then this course is for you, because it's all about learning to program Bitcoin. It's an introductory course that explains all the nuances and features of Bitcoin as it goes. It also takes a very specific tack, by offering lessons in how to work _directly_ with Bitcoin Core and with the c-lightning server using their RPC interfaces.

Why not use some of the more fully featured libraries found in various programming languages? Why not create your own from scratch? It's because working with cryptocurrency is dangerous. There are no safety nets. If you accidentally overpay your fees or lose a signing key or create an invalid transaction or make any number of potential mistakes, then your cryptocurrency will been gone forever. Much of that responsibility will, of course, lie with you as a cryptocurrency programmer, but it can be minimized by working with the most robust, secure, and safe cryptocurrency interfaces around, the ones created by the cryptocurrency programming teams themselves: ``bitcoind`` and ``lightningd``.

Much of this book thus discusses how to script Bitcoin (and Lightning) directly from the command line. Some later chapters deals with more sophisticated programming languages, but again they continue to interact directly with the ``bitcoind`` and ``lightningd`` daemons by using RPC or by interacting with the files they create. This allows you to stand on the shoulders of giants and use their trusted technology to learn how to create your own trusted systems.

## Overview of Topics

This book is broadly divided into the following sections:

   * **Part One: Preparing for Bitcoin** - Understanding the basics of Bitcoin and setting up a server for use.
   * **Part Two: Using Bitcoin-CLI** - Using the Bitcoin-CLI for creating transactions.
   * **Part Three: Bitcoin Scripting** - Expanding your Bitcoin work with scripts.
   * **Part Four: Using Tor** - Improving your node security with Tor
   * **Part Five: Programming with RPC** - Accessing RPC from C and other languages.
   * **Part Six: Using Lightning-CLI** - Using the Lightning-CLI for creating transactions.
   * **Appendices** - Utilizing less common Bitcoin setups.

## How To Read This Book

So where do you start? This book is primarily intended to be read sequentially. Just follow the "What's Next?" Links at the end of each section and/or click through the individual section links on each chapter page. You'll achieve the best understanding from this course if you actually build yourself a Bitcoin server (per Chapter 2) and then run through all the examples over the course of the book: trying out examples is an excellent learning methodology.

If you have different levels of skill or want to learn different things, you might skip to different parts of the book:

* If you've already got a Bitcoin environment ready to be used, jump to [Chapter Three: Understanding Your Bitcoin Setup](03_0_Understanding_Your_Bitcoin_Setup.md).
* If you only care about Bitcoin scripting, jump to [Chapter Nine: Introducing Bitcoin Scripts](09_0_Introducing_Bitcoin_Scripts.md).
* If you just want to read about using programming languages, jump to [Chapter Fifteen: Talking to Bitcoin](15_0_Talking_to_Bitcoind.md).
* If you are only interested in Lightning, zap over to [Chapter Eighteen: Understanding Your Lightning Setup](18_0_Understanding_Your_Lightning_Setup.md).
* If you want to read the major new content added for v2 of the course (2020), following on v1 (2016), read [§3.5: Understanding the Descriptor](03_5_Understanding_the_Descriptor.md), [§4.6: Creating a SegWit Transaction](04_6_Creating_a_Segwit_Transaction.md), [Chapter 7: Expanding Bitcoin with PSBTs](07_0_Expanding_Bitcoin_Transactions_PSBTs.md), [§9.5: Scripting a P2WPKH](09_5_Scripting_a_P2WPKH.md), [§10.5: Scripting a SegWit Script](10_5_Scripting_a_Segwit_Script.md), [Chapter 14: Using Tor](14_0_Using_Tor.md), [Chapter 15: Talking to Bitcoind with C](15_0_Talking_to_Bitcoind.md), [Chapter 16: Programming with Libwally](16_0_Programming_with_Libwally.md), [Chapter Seventeen: Talking to Bitcoind with Other Languages](17_0_Talking_to_Bitcoind_Other.md), [Chapter Eighteen: Understanding ](18_0_Understanding_Your_Lightning_Setup.md), and [Chapter Nineteen: Using Lightning](19_0_Using_Lightning.md).

## What's Next?

If you'd like a basic introduction to Bitcoin, public-key cryptography, ECC, blockchains, and Lightning, read the [Introducing Bitcoin](01_1_Introducing_Bitcoin.md) interlude. 

Otherwise, if you're ready to dive into the course, go to [Setting Up a Bitcoin-Core VPS](02_0_Setting_Up_a_Bitcoin-Core_VPS.md).
