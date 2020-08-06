# 16.2: Using BIP39 in Libwally

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

One of Libwally's greatest powers is that it can lay bare the underlying work of creating addresses. To start with, it supports [BIP39](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki), which is the BIP that supports Mnemonic codes for Bitocin addresses.

> :book: ***What is a Mnemonic Code?*** Bitcoin addresses (and their corresponding private keys) are long, untintelligible lists of characters and numbers, which are not only impossible to remember, but also easy to mess up. Mnemonic codes were a solution for this that allow users to remember 12 (or 24) words in their language. These codes can then be used to fully restore a BIP32 seed that's the basis of an HD wallet.


