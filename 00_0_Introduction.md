# Introduction to Programming Bitcoin Core & Lightning

The ways that we make payments for goods and services has been changing dramatically over the last several decades. Where once all transactions were conducted through cash or checks, now various electronic payment methods are the norm. However, most of these electronic payments still occur through centralized systems, where credit card companies, banks, or even internet-based financial institutions like Paypal keep long, individually correlated lists of transactions and have the power to censor transactions that they don't like.

These centralization risks were some of the prime catalysts behind the creation of cryptocurrencies, the first and most successful of which is Bitcoin. Bitcoin offers pseudonymity, it makes it difficult to correlate transactions, and it makes censorship by individual entities all but impossible. These advantages have made it one of the quickest growing currencies in the world. That growth in turn has made Bitcoin into a going concern among entrepreneurs and developers, eager to create new services for the Bitcoin community.

If you're one of those entrepreneurs or developers, then this book is for you, because it's all about learning to program Bitcoin. It's an introductory book that explains all the nuances and features of Bitcoin as it goes. It also takes a very specific tack, by offering lessons in how to work _directly_ with Bitcoin Core and with the c-lightning server using their RPC interfaces.

Why not use some of the more fully featured libraries found in various programming languages? Why not create your own from scratch? It's because working with cryptocurrency is dangerous. There are no safety nets. If you accidentally overpay your fees or lose a signing key or create an invalid transaction or make any number of potential mistakes, then your cryptocurrency will been gone forever. Much of that responsibility will, of course, lie with you as a cryptocurrency programmer, but it can be minimized by working with the most robust, secure, and safe cryptocurrency interfaces around, the ones created by the cryptocurrency programming teams themselves: ``bitcoind`` and ``lightningd``.

Much of this book thus discusses how to script Bitcoin (and Lightning) directly from the command line. Some later chapters deals with more sophisticated programming languages, but again they continue to interact directly with the ``bitcoind`` and ``lightningd`` daemons by using RPC or by interacting with the files they create. This allows you to stand on the shoulders of giants and use their trusted technology to learn how to create your own trusted systems.

## Overview of Topics

This book is broadly divided into the following sections:

   * **Part One: Preparing for Bitcoin** - Understanding the basics of Bitcoin and setting up a server for use.
   * **Part Two: Using Bitcoin-CLI** - Using the Bitcoin-CLI for creating transactions.
   * **Part Three: Bitcoin Scripting** - Expanding your Bitcoin work with scripts.
   * **Part Four: Using Tor** - Improving your node security with Tor
   * **Part Five: Using Lightning-CLI** - Using the Lightning-CLI for creating transactions.
   * **Part Six: Programming with RPC** - Accessing RPC from C and other languages.
   * **Appendices** - Utilizing less common Bitcoin setups.

## What's Next?

Learn the basics of Bitcoin in [Chapter One: Introducing Bitcoin](01_0_Introducing_Bitcoin.md).
