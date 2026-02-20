# 3.6: Creating QR Codes for Addresses

To date we've been displaying addresses as little bits of bech32- and base58-encoded data. That's great if you can cut and paste them or if you can store them in shell variables. But what if you're trying to transmit an address between devices that aren't on the same network, or that don't have a direct connection even if they are? That's where QR Codes come into play.

## The Power of Airgaps

When you have two devices that don't directly talk to each other you have an [airgap](https://developer.blockchaincommons.com/airgap/). It could be created purposefully, it could be a result of a device's limitations, or it could be part of a designed architecture. Here are some common situations that involve airgaps:

* _Your mobile device tends to be airgapped from other devices._ Sure, it can talk to the internet, and it's been taught to talk to your printers, but the ways in which you can talk to your computer or your tablet tend to be extremely restricted.
* _Your mobile device tends to be airgapped from other peoples' mobile devices._ Again, there may be a few ways they can talk, such as exchanging contact information, but they're limited.
* _Hardware wallets tend to be airgapped from everything._ Some older wallets allowed you to connect up to a computer with a single cord, but many newer wallets have done away with even that.

This isn't just an annoyance (though it often is!). It's also a powerful security feature. Airgaps constrain transmission, they eliminate interactivity, and they highlight user agency. These are advantages that make it much harder for a device to be attacked by a virus or an active adversary. For that reason, many self-sovereign wallets, which allow users to safely hold their own funds rather than trusting an exchange, are airgapped. 

## The Power of QR Codes

The practice of airgapping raise an obvious question: how do you get data to and from an airgapped device? Traditionally this was done with "sneakernet", which meant copying the data onto a floppy disk and carrying it across the airgap. More recently, the same effect can be accomplished with an SD card or an NFC card.

QR Codes can also be used to transmit content across airgaps. A QR Code, or Quick Response code, is simply a two-dimensional bar code. It contains data in its lines and dots and has three squares at three of the corners, which cameras can use to figure out the edges of the QR Code and flatten it out, so that it can be read correctly. These QR Codes can contain a surprising amount of information: 2,953 bytes that can be freely input. You put all of that together and you have an excellent mechanism for transmitting Bitcoin information across airgaps: you just show a QR Code on one device, and you read that code with a camera on the other.

## Set Up qrencode

One of the simplest methods for encoding data as a QR Code is the `qrencode` program, which is available for a variety of playforms from the internet. 

If you set up your machine with the Bitcoin Standup Scripts, as we suggest, you already have it.
```
$ which qrencode
/usr/bin/qrencode
```

> 🍎 **OTHER OSES.** Mac and Windows both have access to `qrencode` as well. If you're using a Mac, you can install [Homebrew](https://brew.sh/) and then brew up `qrencode`. If you're using Windows, versions are available for download.

## Encode Addresses with qrencode

One of the most common uses of QR Coins in Bitcoin is to encode addresses. This can be as simple as outputting your `getnewaddress` command to `qrencode` and then outputting (`-o`) that to a file:
```
$ bitcoin-cli getnewaddress | qrencode -o newaddress.jpg
$ ls -la newaddress.jpg 
-rw-r--r-- 1 standup sudo 404 Feb 19 16:04 newaddress.jpg
```
Here's what that code looks like:

![](/images/03-qraddress.jpg)

## Test Every QR

Working with digital currency means never trusting anything. That's because there are no take-backs. If you make a mistake, no one can correct it for you. Often that means that you lose funds. For that reason, you should always be sure of any QR code you're given.

First, decode it and see what it really says. This can often be done with a mobile device: scan the code and most modern devices should show you what it says:

<a href="/images/03-qraddress-photo.jpg"><img src="/images/03-qraddress-photo.jpg" width=250></a>

Second, check the text of the QR with the person who sent it to you. For addresses, checking the first several and last several addresses can be enough. In this case, if asked, the QR sender could verify the address against their wallet with `getaddressesbylabel` and see that `tb1qguyxx...93xk3nhu` is indeed in there.

```
$ bitcoin-cli getaddressesbylabel ""
{
  "mueEcK86JTvMrge7qibsRdMes6ET5oAGgd": {
    "purpose": "receive"
  },
  "2NAzFNuopaor2YnqVA2QC4KYme6HxWTk7jq": {
    "purpose": "receive"
  },
  "tb1qguyxx0e50kvsc8a73dh2tpatk4kqlg93xk3nhu": {
    "purpose": "receive"
  },
  "tb1quqv4shd6ts0t6ew5l8kcqvvxgvr8drtr7ajew2": {
    "purpose": "receive"
  },
  "tb1pps29f3evn5lscg82a2wceyl8p6s0wckjvp3zllw2myn6xg6xxvxqcekkxd": {
    "purpose": "receive"
  }
}
```

## Other Uses for QRs

QRs can have other uses in Bitcoin work. 

The [Quick Connect](https://developer.blockchaincommons.com/quickconnect/) protocol allows a network coordinator (that's the `bitcoin-cli` or other server that actually talks to the Bitcoin network) to securely link to a Seed Vault (that's the airgapped hardware device where seeds or keys are actually stored). Though we'll be doing a lot of work in this course where we're just passing variables around on the command line, a real-world Bitcoin setup requires much more sophisticated separation of data of this sort for security.

QRs can also be used to pass complex transactions called PSBTs among multiple members of a multisig. However, PSBTs are very large, so they require a different sort of QR called [animated QRs](https://developer.blockchaincommons.com/animated-qrs/). There's more on PSBTs and animated QRs in [chapter 7](07_0_Expanding_Bitcoin_Transactions_PSBTs.md).

Fundamentally, any time you have non-human-readable data that you want to transmit between two devices without a direct connection, a QR Code is a great option. (But, it's one that we largely won't see again in this course, because we're of course working on the _command line_.)

## Summary: Creating QR Code for Addresses

You won't be creating more QR Codes in this course (except when we briefly touch upon Animated QR Codes). However, QR Codes are a vital part of the larger Bitcoin ecosystem because they allow you to transmit data across airgaps, which could be important for distributing addresses to people sending you money or for collecting together signatures for a PSBT. And, as it happens, you can create them right from the command line with the `qrencode` tool.

## What's Next?

Conclude "Understanding Your Bitcoin Setup" with [§3.7: Receiving a Transaction](03_7_Receiving_a_Transaction.md).



