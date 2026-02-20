# 3.6: Creating QR Codes for Addresses

To date we've been displaying addresses as little bits of bech32- and base58-encoded data. That's great if you can cut and paste them or if you can store them in shell variables. But what if you're trying to transmit an address between devices that aren't on the same network, or that don't have a direct connection even if they are. That's where QR Codes come into play.

## The Power of Airgaps

When you have two devices that don't directly talk to each other you have an [airgap](https://developer.blockchaincommons.com/airgap/). It can be created purposefully, it could be a result of a device's limitations, or it could be part of a designed architecture. Here are some common situations that involve airgaps:

* _Your mobile device tends to be airgapped from other devices._ Sure, it can talk to the internet, and it's been taught to talk to your printers, but the ways in which you can talk to your computer or your tablet tend to be extremely restricted.
* _Your mobile device tends to be airgapped from other peoples' mobile devices._ Again, there may be a few ways they can talk, such as exchanging contact information, but they're limited.
* _Hardware wallets tend to be airgapped from everything._ Some older wallets allowed you to connect up to a computer with a single cord, but many newer wallets have done away with even that.

This isn't just an annoyance (though it often is!). It's also a powerful security feature. Airgaps constrain transmission, they eliminate interactivity, and they highlight user agency. These are advantages that make it much harder for a device to be attacked by a virus or an active adversary across an Airgap. For that reason many self-sovereign wallets, which allow users to safely hold their own funds rather than trusting an exchange, are airgapped. 

## The Power of QR Codes

The advantages of airgapping raise an obvious question: how do you get data to and from an airgapped device? Traditionally this was done with "sneakernet", which meant copying the data onto a floppy disk and carrying across the airgap. More recently, the same effect can be accomplished with an SD card or an NFC card.

QR Codes can also be used to carry content across airgaps. A QR Code, or Quick Response code, is simply a two-dimensional bar code. It contains data in its lines and dots and has three squares at three of the corners, which cameras can use to figure out the edges of the QR Code and flatten it out so that it can be read correctly. And QR Codes can contain a surprising amount of information: 2,953 bytes that can be freely input. You put all of that together and you have an excellent mechanism for transmitting Bitcoin information across airgaps: you just show a QR Code on one device, and you read that code with a camera on the other.

## Setup qrencode

One of the simplest methods for encoding data as a QR Code is the `qrencode` program, which is available for a variety of playforms on the internet. 

If you setup your machine with the Bitcoin Standup Scripts, as we suggest, you already have it.
```
$ which qrencode
/usr/bin/qrencode
```

> 🍎 **OTHER OSES.** Mac and Windows both have access to `qrencode` as well. If you're using a Mac, you can install [Homebrew](https://brew.sh/) and then brew up `qrencode`. If you're using Windows, versions are available for download.

## Encode Addresses with qrencode

One of the most common uses of QR Coins in Bitcoin is to encode addresses. This is as simple as outputting your `getnewaddress` command to `qrencode` and then outputting (`-o`) that to a file:
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

![](/images/03-qraddress-photo.jpg)



