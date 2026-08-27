# Chapter One: Introduction to Learning Bitcoin Core from the Command Line

## Introduction

The ways that we make payments for goods and services has been
changing dramatically over the last several decades. Where once all
transactions were conducted through cash or checks, now various
electronic payment methods are the norm. However, most of these
electronic payments still occur through centralized systems, where
credit card companies, banks, or even internet-based financial
institutions like Paypal keep long, individually correlated lists of
transactions and have the power to censor transactions that they don't
like.

These centralization risks were some of the prime catalysts behind the
creation of cryptocurrencies, the first and most successful of which
is Bitcoin. Bitcoin offers pseudonymity; it makes it more difficult to
correlate transactions; and it makes censorship by individual entities
all but impossible. These advantages have made it one of the quickest
growing currencies in the world. That growth in turn has made Bitcoin
into a going concern among entrepreneurs and developers, eager to
create new services for the Bitcoin community.

If you're one of those entrepreneurs or developers, then this course
is for you, because it's all about learning to program Bitcoin. It's
an introductory course that explains all the nuances and features of
Bitcoin as it goes. It also takes a very specific tack, by offering
lessons in how to work _directly_ with Bitcoin Core using its RPC
interfaces.

Why not use some of the more fully featured libraries found in various
programming languages? That's because the goal here is to create a
visceral, hands-on experience that makes it easy to begin directly
working with `bitcoind`. By typing in the commands, seeing the results
yourself, and perhaps even exploring them beyond the specific walls of
this course, you should gain a deep and intuitive understanding of how
Bitcoin works, and you should be able to do so without the rigamarole
of having to program, compile, and run code. You probably won't be
using the command line long-term, but we think it's perfect as a
learning tool, which is why we've create a whole sequence of
digital-asset and digital-identity courses around that precept.

Even if you later move beyond `bitcoin-cli`, we suggest you cleave to
other well-tested programming libraries rather than rolling one from
scratch. Sticking with `bitcoind` and its RPC interface offers a
particularly stable (yet up to date) methodology for working with
Bitcoin. That's important because working with cryptocurrency is
dangerous. There are no safety nets. If you accidentally overpay your
fees or lose a signing key or create an invalid transaction or make
any number of potential mistakes, then your cryptocurrency will be
gone forever. Much of that responsibility will, of course, lie with
you as a cryptocurrency programmer, but it can be minimized by working
with the most robust, secure, and safe cryptocurrency interfaces
around.

## Required Skill Level

You do not need to be particularly technical for the majority of this
course. All you need is the confidence to run basic commands on the
UNIX command line. If you're familiar with things like `ssh`, `cd`,
and `ls`, the course will supply you with the rest.

## Overview of Topics

This book is broadly divided into the following sections:

| Part | Description | Skills |
|-------|---------|---------|
| **Part One: Preparing for Bitcoin** | Understanding the basics of Bitcoin and setting up a server for use. | Command Line | 
| **Part Two: Sending Transactions** | Using the Bitcoin-CLI for creating transactions. | Command Line |
| **Part Three: Expanding Bitcoin** | Using multisigs, PSBTs, locktimes, and data | Command Line |
| **Part Four: Exploring the Ecosystem** | Storing your foundational secrets securely | Command Line |
| **Part Five: Scripting with Bitcoin** | Expanding your Bitcoin work with scripts. | Programming Concepts |
| **Part Six: Using Taproot** | Working with Bitcoin's cutting edge. | Command Line |
| **Part Seven: Maintaing Privacy** | Improving privacy (and why it's important) | Command Line |
| **Appendices** | Utilizing less common Bitcoin setups. | Command Line |

## How To Use This Course

So where do you start? This book is primarily intended to be read
sequentially. Just follow the "What's Next?" Links at the end of each
section and/or click through the individual section links on each
chapter page. You'll achieve the best understanding from this course
if you actually build yourself a Bitcoin server (per Chapter 2) and
then run through all the examples over the course of the book: trying
out examples is an excellent learning methodology.

If you have different levels of skill or want to learn different
things, you might skip to different parts of the book:

* If you've already got a Bitcoin environment ready to be used, jump to [Chapter Three: Preparing Your Bitcoin Wallet](03_0_Preparing_Your_Bitcoin_Wallet.md )
* If you only care about Bitcoin scripting, jump to [Chapter Eleven: Introducing Bitcoin Scripts](11_0_Introducing_Bitcoin_Scripts.md)
* If you conversely don't want to do any programming, consider skipping chapters 11-16 on Bitcoin Script. The rest of the course should still make sense without them.
* If you want to read the major new content added for v3 of the course (2026), read [§3.4: Understanding the Descriptor Wallet](03_4_Understanding_the_Descriptor_Wallet.md), [§4.1: Understanding the Address](04_1_Understanding_the_Address.md), [§4.3: Creating QR Codes for Addresses](04_3_Creating_QR_Codes_for_Addresses.md), [§5.6: Sending Coins to Other Addresses](05_6_Sending_Coins_to_Other_Addresses.md), [§7.2: Creating Multisigs from Descriptors](07_2_Creating_Multisig_Descriptors/), and [§8.4: Creating Animated QR Codes](08_4_Creating_Animated_QR_Codes.md) as well as all of [Chapter 10: Working with Secrets](https://learningbitcoin.blockchaincommons.com/10_0_Working_with_Secrets/), [Chapter 17: Introducing Taproot](17_0_Introducing_Taproot.md), [Chapter 18: Using Schnorr](18_0_Using_Schnorr.md), and [Chapter 19: Using Tapscript](19_0_Using_Tapscript.md). However, the entire course has been updated. For example, the Scripts chapters now talk about P2WSH and there's much more on descriptors everywhere.

Past versions of this course included details on Lightning and on
working with code, but they've been removed to return the course to
its original course. (We are currently seeking funding to turn
Lightning into its own course; if you're interested in supporting
that, [let us know](mailto:team@blockchaincommons.com).

## Why to Use this Course

Obviously, you're working through this course because you're
interested in Bitcoin. Besides imparting basic knowledge, it's also
helped readers to join (or create) open-source projects and to get
entry-level jobs in Bitcoin programming. A number of Blockchain
Commons' interns learned about Bitcoin from this course, as have some
members of our programming team.

## How to Support this Course

* Please use [Issues](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/issues) for any questions. Blockchain Commons does not have an active support team, and so we can't address individual problems or queries, but we will look over them in time, and use them to improve future iterations of the course.
* Please use [PRs](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/pulls) for any fixes of typos or incorrect (or changed) commands. For command-line or technical changes, it's very helpful if you also use the PR comments to explain why you did what you did, so that we don't have to research it.
* Please Use Our [Community discussions area](https://github.com/BlockchainCommons/Community/discussions) for talking about careers and skills. Blockchain Commons occasionally offers internships, as discussed in our Community repo.
* Please [become a patron](https://github.com/sponsors/BlockchainCommons) if you find this course helpful or if you want to help educate the next generation of blockchain programmers.

## What's Next?

If you'd like a basic introduction to Bitcoin, public-key
cryptography, ECC, and blockchains, read the [Introducing
Bitcoin](01_1_Introducing_Bitcoin.md) interlude.

Otherwise, if you're ready to dive into the course, go to [Chapter
Two: Setting Up a Bitcoin-Core
VPS](02_0_Setting_Up_a_Bitcoin-Core_VPS.md).
