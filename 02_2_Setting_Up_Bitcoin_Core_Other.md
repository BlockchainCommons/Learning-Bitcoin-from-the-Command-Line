# 2.2: Setting Up a Bitcoin-Core Machine via Other Means

> :information_source: **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

The previous section, [§2.1: Setting Up a Bitcoin-Core VPS with Bitcoin Standup](02_1_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md), presumed that you would be creating a full node on a VPS using a Linode Stackscript. However, you can actually create a Bitcoin-Core instance via any methodology of your choice and still follow along with the later steps of this tutorial.

Following are other setup methodologies that we are aware of:

* *[Using GordianNode-macOS](https://github.com/BlockchainCommons/GordianNode-macOS).* If you have a modern Mac, you can use Blockchain Commons' *GordianNode* app, powered by *BitocinStandup*, to install a full node on your Mac.
* *[Using Other Bitcoin Standup Scripts](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts).* Blockchain Commons also offers a version of the Linode script that you used that can be run from the command line on any Debian or Ubuntu machine.
* *[Setting Up a Bitcoin Node on AWS](https://wolfmcnally.com/115/developer-notes-setting-up-a-bitcoin-node-on-aws/).* @wolfmcnally has written a step-by-step tutorial for setting up Bitcoin-Core with Amazon Web Services (AWS).
* *[Setting Up a Bitcoin Node on a Raspberry Pi 3](https://medium.com/@meeDamian/bitcoin-full-node-on-rbp3-revised-88bb7c8ef1d1).* Damian Mee explains how to set up a headless full node on a Raspberry Pi 3.
* *[Compiling Bitcoin From Source](https://jonatack.github.io/articles/how-to-compile-bitcoin-core-and-run-the-tests
).* Jon Atack explains, step-by-step, how to compile Bitcoin Core from source and how to run the functional and unit tests - TOR not included
* *[https://launchpad.net/~luke-jr/+archive/ubuntu/bitcoincore](Luke Dash Jr repository for Bitcoin Core for Ubuntu and Debian).* Bitcoin Core can be installed via apt but not from the included repo

## What's Next?

Unless you want to return to one of the other methodologies for creating a Bitcoin-Core node, you should:

   * Move on to "bitcoin-cli" with [Chapter Three: Understanding Your Bitcoin Setup](03_0_Understanding_Your_Bitcoin_Setup.md).
