# 2.2: Setting Up a Bitcoin-Core Machine via Other Means

The previous section, [§2.1: Setting Up a Bitcoin-Core VPS with Bitcoin Standup](02_1_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md), presumed that you would be creating a full node on a VPS using a Linode Stackscript. However, you can actually create a Bitcoin-Core instance via any methodology of your choice and still follow along with the later steps of this tutorial.

Following are other setup methodologies that we are aware of:

* *[Compiling from Source](A2_0_Compiling_Bitcoin_from_Source.md).* If you prefer to compile Bitcoin Core by hand, that's covered in Appendix 2.
* *[Using Fully Noded Server](https://github.com/Fonta1n3/FullyNoded-Server).* If you have a **Mac**, you can install Bitcoin using Fontaine's Fully Noded Server. The main gotcha is that it hides the `bitcoin-cli` binary, but you can create an alias for it using something like `alias bitcoin-cli="~/.fullynoded/BitcoinCore/bitcoin-30.2/bin/bitcoin-cli -signet"` and see [§3.1](03_1_Verifying_Your_Bitcoin_Setup.md) for more on creating aliases. Do note that Mac's zsh and our default bash shells work slightly differently so the syntax of piping, shell variables, and other similar functions might be different.
* *[Using Other Bitcoin Standup Scripts](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts).* Blockchain Commons also offers a version of the Linode script that you used that can be run from the command line on any Debian or Ubuntu machine.
* *[Setting Up a Bitcoin Node on AWS](https://wolfmcnally.com/115/developer-notes-setting-up-a-bitcoin-node-on-aws/).* @wolfmcnally has written a step-by-step tutorial for setting up Bitcoin-Core with Amazon Web Services (AWS).
* *[Setting Up a Bitcoin Node on a Raspberry Pi 3 (Revised)](https://medium.com/@meeDamian/bitcoin-full-node-on-rbp3-revised-88bb7c8ef1d1).* Damian Mee explains how to set up a headless full node on a Raspberry Pi 3.

Be sure that you are installing on a current version of your OS, to avoid problems down the line. As of this writing, this course is tested on Debian 13.

## What's Next?

Unless you want to return to one of the other methodologies for creating a Bitcoin-Core node, you should:

   * Move on to "bitcoin-cli" with [Chapter Three: Preparing Your Bitcoin Wallet](03_0_Preparing_Your_Bitcoin_Wallet.md)
