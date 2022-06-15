# Updates for v2.3 of LBTCftCL

V2.2 of LBTCftCL was drafted in the summer of 2021 and up to date with V0.20 of Bitcoin Core. Bitcoin Core is now up to v23.0, and the course needs to be updated to best address these recent changes. What follows is a rough listing of updates that are likely to require changes to the course. They will all require investigation, and in some cases it might be determined that there's nothing to be done. Some of the main questions that will determine whether material should be included in the course are listed, as our ideas for where material might go in the course.

## Legacy Updates

* **Segwit**
   * Segwit is now old enough that we should teach it as the default. That means that sections 3.1-4.5 should be rewritten to use Segwit as the default and 4.6 should be removed (with perhaps a bit of the information about the different types of addresses being preserved).
   * It's _possible_ that the same should occur with 10.5, but it should first be reviewed to see if it's a meaningful building block in the scripting process (or not).
* **Fees**
   * There was some question of if `mintxfee` is still current, or if `paytxfee` should be used. I haven't seen any evidence of obsolence, but it'd be good to check this and make sure we're still on the best practices.
   * This is discussed in [4.1](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/04_1_Sending_Coins_The_Easy_Way.md)
  
## 23.0 Updates

See also [#575](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/issues/575) and of course the [Bitcoin Core 23.0 release notes](https://github.com/bitcoin/bitcoin/blob/master/doc/release-notes/release-notes-23.0.md).

* **Schnorr & Taproot**
   * Obviously, the biggest update. We'll need to teach the basics of both and why they're exciting. The main question here is: what commands actually take advantage of Schnorr & Taproot, and how can we show this off.
   * If there's just a little bit of functionality right now, it can go into chapter 8, probably in two new sections: 8.3 & 8.4, on Schnorr and Taproot. If there's already a lot of functionality, it should go into its own chapter, and 8 (and everything beyond it) should be shifted back.
* **Descriptor Wallets**
   * Descriptor wallets are now the default. There's some unfortunate lack of integration with multisigs, but we should otherwise give them more attention. What can we do new about inputting and outputting descriptor wallets? Are there any other functions of note?
   * This will likely go in [3.5](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/03_5_Understanding_the_Descriptor.md), though it's possible it go split into two chapters: Understanding the Descriptor and Understanding the Descriptor Wallet.
* **Freezing Coins**
   * This is an interesting new ability that allows you to prevent UTXOs from being automatically selected when transactions are created. 
   * This would probably fit well into 4.5 as a final header-section.
* **CJDNS Network** 
   * I'm not familiar with CJDNS, but it sounds like a privacy option that would fit in with Tor and i2p
   * If that's correctly, this would be a good 15.2, with the 15.0 chapter renamed "Using Other Privacy Options" or something like that
* **RPC Changes**
   * The following RPC commands have had changes to their output and any examples should be rerun. If they are part of a sequence of commands (e.g., building out a transaction), then the whole sequence should be rerun.
   * `createmultisig`, `addmultisigaddress`, `listunspent`, `getblockchaininfo`
