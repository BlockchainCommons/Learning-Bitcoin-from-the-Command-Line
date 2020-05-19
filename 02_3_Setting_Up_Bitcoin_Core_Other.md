# 2.3: Setting Up a Bitcoin-Core Machine via Other Means

> :information_source: **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

The previous sections presume that you will be [§2.1: Setting up a Bitcoin-Core VPS by Hand](02_1_Setting_Up_a_Bitcoin-Core_VPS_by_Hand.md) or [§2.2: Setting Up a Bitcoin-Core VPS with StackScript](02_2_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md) (as a Linode). However, you can actually create a Bitcoin-Core instance via any methodology of your choice and still follow along with the later steps of this tutorial.

Following are other setup methodologies that we are aware of:

* *[Bitcoin Standup](https://github.com/BlockchainCommons/Bitcoin-Standup).* A completely mechanical setup from Blockchain Commons that will not only install Bitcoin-Core, but will also install other programs such as Tor. It is available as a [Mac App](https://github.com/BlockchainCommons/Bitcoin-StandUp-MacOS) or as [Linux Scripts](https://github.com/BlockchainCommons/Bitcoin-StandUp-MacOS) (based on our own setup in §2.2, but available for any flavor of Debian). 
* *[Setting Up a Bitcoin Node on AWS](https://wolfmcnally.com/115/developer-notes-setting-up-a-bitcoin-node-on-aws/).* A step-by-step tutorial by @wolfmcnally on setting on Bitcoin-Core with Amazon Web Services (aws).

Note that if you use one of these alternative setups, you will need to create the special Bitcoin aliases, as explained in [§3.1: Verifying Your Bitcoin Setup](03_1_Verifying_Your_Bitcoin_Setup.md).

## What's Next?

Unless you want to return to one of the other methodologies for creating a Bitcoin-Core node, you should:

   * Move on to "bitcoin-cli" with [Chapter Three: Understanding Your Bitcoin Setup](03_0_Understanding_Your_Bitcoin_Setup.md).
