# 2.2: Setting Up a Bitcoin-Core Machine via Other Means

The previous section, [§2.1: Setting Up a Bitcoin-Core VPS with Bitcoin Standup](02_1_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md), presumed that you would be creating a full node on a VPS using a Linode Stackscript. However, you can actually create a Bitcoin-Core instance via any methodology of your choice and still follow along with the later steps of this tutorial.

Following are other setup methodologies that we are aware of:

* *[Compiling from Source](A2_0_Compiling_Bitcoin_from_Source.md).* If you prefer to compile Bitcoin Core by hand, that's covered in Appendix 2.
* *[Using GordianNode-macOS](https://github.com/BlockchainCommons/GordianNode-macOS).* If you have a modern Mac, you can use Blockchain Commons' *GordianNode* app, powered by *BitcoinStandup*, to install a full node on your Mac.
* *[Using Other Bitcoin Standup Scripts](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts).* Blockchain Commons also offers a version of the Linode script that you used that can be run from the command line on any Debian or Ubuntu machine. This tends to be the leading-edge script, which means that it's more likely to feature new functions, like Lightning installation.
* *[Setting Up a Bitcoin Node on AWS](https://wolfmcnally.com/115/developer-notes-setting-up-a-bitcoin-node-on-aws/).* @wolfmcnally has written a step-by-step tutorial for setting up Bitcoin-Core with Amazon Web Services (AWS).
* *[Setting Up a Bitcoin Node on a Raspberry Pi 3](https://medium.com/@meeDamian/bitcoin-full-node-on-rbp3-revised-88bb7c8ef1d1).* Damian Mee explains how to set up a headless full node on a Raspberry Pi 3.

## What's Next?

Unless you want to return to one of the other methodologies for creating a Bitcoin-Core node, you should:

   * Move on to "bitcoin-cli" with [Chapter Three: Understanding Your Bitcoin Setup](03_0_Understanding_Your_Bitcoin_Setup.md).
