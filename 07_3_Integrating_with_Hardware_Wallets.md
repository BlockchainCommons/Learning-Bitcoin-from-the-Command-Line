# 7.3: Integrating with Hardware Wallets

> :information_source: **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

One of the greatest powers of PSBTs is the ability to hand transactions off to hardware devices. This will be a great development tool for you if you continue to program with Bitcoin. However, you can't test it out if you're using one of the configurations we suggest for this course — a VM on Linode per [§2.2](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/02_2_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md) or an even more farflung option such an AWS per [§2.3](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/02_3_Setting_Up_Bitcoin_Core_Other.md) — because obviously you won't  have any way to hook a hardware device up to your remote, virtual machine.

>: ***What is a Hardware Wallet?*** A hardware wallet is an electronic device that improves the security of cryptocurrency by maintaing all the private keys on the device, rather than ever putting them on a device directly connected to the internet. Wallets have specific protocols for providing online interactions, usually managed by a program through a USB port. In this case, we'll be managing them with `bitcoin-cli` and the `hwy.py` program.

You have three options for moving through this chapter on hardware wallets: (1) read along without testing the code; (2) install Bitcoin on a local machine to fully test these commands; or (3) skip straight ahead to [Chapter 8: Expanding Bitcoin Transactions in Other Ways](08_0_Expanding_Bitcoin_Transactions_Other.md). We suggest option #1, but if you really want to test these commands out we'll also give some support for #2 by talking about using a Macintosh for testing (a hardware-platform supported by [Bitcoin Standup](https://github.com/BlockchainCommons/Bitcoin-Standup)).

> :warning: **VERSION WARNING:** PSBTs are an innovation from Bitcoin Core v 0.17.0. Earlier versions of Bitcoin Core will not be able to work with the PSBT while it is in process (though they will still be able to recognize the final transaction). The HWI interface appeared in Bitcoin Core v 0.18.0, but as long as you are using our suggested setup with Bitcoin Standup, it should all be fully integrated.

> :warning **FRESHNESS WARNING:** The HWI interface is very new. It may be hard to install correctly and it may have unintuive errors. What follows if a description of a working setup, but it took several tries to get right, and your setup may vary.

The methodology for integrating with a Hardware Wallet that is described in this chapter depends on the [Bitcoin Hardware Wallet Interface](https://github.com/bitcoin-core/HWI) released through Bitcoin Core and builds on the [installation](https://github.com/bitcoin-core/HWI/blob/master/README.md) and [usage](https://github.com/bitcoin-core/HWI/blob/master/docs/bitcoin-core-usage.md) instructions found there.

## Install Bitcoin Core on a Local Machine

_If you just plan to read over this section and not test out these commands until some future date when you have a local development environment set up, you can skip this instruction, which is about creating a Bitcoin Core installation on a local machine such as a Mac or UNIX machine._

There are alternate versions of the Bitcoin Standup script that you used to create your VM that will install on a MacOS or on a non-Linode UNIX.

If you have MacOS, you can install [Bitcoin Standup MacOS](https://github.com/BlockchainCommons/Bitcoin-Standup-MacOS/blob/master/README.md).

If you have a local UNIX machine, you can install [Bitcoin Standup Linux Scripts](https://github.com/BlockchainCommons/Bitcoin-Standup-MacOS/blob/master/README.md).

Once you've gotten Bitcoin Standup running on your local machine, you'll want to sync the "Testnet" blockchain, assuming that you're continuing to follow the standard methodlogy of this course.

We will be using a Macintosh and Testnet for the examples in this section.

### Creating an Alias for Bitcoin-CLI

Create an alias that causes `bitcoin-cli` to run the correct command from the correct directory with any appropriate flags.

Here's an example alias from a Mac:
```
$ alias bitcoin-cli="~/StandUp/BitcoinCore/bitcoin-0.20.0/bin/bitcoin-cli -testnet"
```
You'll note it not only gives us the full path, but also ensures that we stay on Testnet.

## Installing HWI on a Local Machine

_The following instructions again assume a Mac._

HWI is a Bitcoin Core program available in python.

### Installing Python

Because HWI program is written in `python`, you'll need to install that first, if you haven't already.

If you don't already have the `xcode` command line tools, you'll need them:
```
$ xcode-select --install
```

If you don't already have the Homebrew package manager, you should install that too. Current instructions are available at the [Homebrew site](https://brew.sh/). As of this writing, you simple need to:
```
$ /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"
```

For a first time install, you should also make sure your `/usr/local/Frameworks` directory is created correctly:
```
sudo mkdir /usr/local/Frameworks
sudo chown $(whoami):admin /usr/local/Frameworks
```

If you have `brew` installed, then you can then easily install `python`:
```
$ brew install python
$ brew install libusb
```

### Installing HWI

You're now ready to install HWI, which requires cloning a GitHub repo and compiling.

If you don't have `git` already installed on your Mac, you can do so just by trying to run it: `git --version`.

You can then clone the HWI repo:
```
$ cd ~/StandUp
$ git clone https://github.com/bitcoin-core/HWI.git
```
Afterward, you need to install the package and its dependencies:
```
$ cd HWI
HWI$ python3 setup.py install
```

### Creating an Alias for HWI

You'll want to create an alias here too, varied by yhour actual install location:
```
$ alias hwi="~/Standup/HWI/hwi.py --testnet"
```
Again, we've included a reference to testnet in this alias.

## Prepare Your Ledger

_We had to choose a hardware-wallet platform too, for this HWI demonstration. Our choice was Ledger, which has long been our testbed for hardware wallets. Please see [HWI's device support info](https://github.com/bitcoin-core/HWI/blob/master/README.md#device-support) for a list of other supported devices. If you use a device other than a Ledger, you'll need to assess your own solutions for preparing it for usage on the Testnet, but otherwise you should be able to continue with the course as written._

If you are working with Bitcoins on your Ledger, you probably won't need to do anything. (But we don't suggest that for use with this course). 

To work with Testnet, as suggested by this course, you'll need to make a few updates:

1. Go to Settings on your Ledger Live app (it's the gear), go to the "Experimental Features" tab, and turn on "Developer Mode"
2. Go to the "Manager" and install "Bitcoin Test". The current version requires that you have "Bitcoin" and "Etereum" installed first.
3. Go to the "Manager", scroll to your new "Bitcoin Test", and "Add Account"

To fully test, you will also want to request some Testnet coins from a faucet as described in [§3.4: Receiving a Transaction](03_4_Receiving_a_Transaction.md).

## Link to a Ledger

In order for a Ledger to be accessible, you must login with your PIN and then call up the app that you want to use, in this case the "Bitcoin Test" app. Once you've done that, you can ask for HWI to access the Ledger:
```
$ hwi enumerate
[{"type": "ledger", "model": "ledger_nano_s", "path": "IOService:/AppleACPIPlatformExpert/PCI0@0/AppleACPIPCI/XHC1@14/XHC1@14000000/HS05@14100000/Nano S@14100000/Nano S@0/IOUSBHostHIDDevice@14100000,0", "fingerprint": "9a1d520b", "needs_pin_sent": false, "needs_passphrase_sent": false}]
```
If you got information on your device, you're set! As you can see, it verifies your hardware-wallet type, provides other identifying information, and tells you how you must communicate with the device.

If you instead got `[]`, then either (1) you didn't get your Ledger device ready by entering your PIN and choosing the correct application, or (2) there's something wrong with your Python setup, probably a missing dependency: you should consider `uninstall`ing it and trying from scratch.

## Import Addresses

Interacting with a hardware wallet usually comes in two major steps: watching for funds and spending funds. 

You can watch funds by importing addresses from your hardware wallet to your full node, using `bitcoin-cli`.

### Creating a Wallet

To use your hardware wallet with `bitcoin-cli`, you'll want to create a specific named wallet, using the `createwallet` RPC, which is a new command.
```
$ bitcoin-cli --named createwallet wallet_name="ledger" disable_private_keys="true"
{
  "name": "ledger",
  "warning": ""
}
```
In this case, we are creating a new wallet `ledger` without private keys (since those will be over on the Ledger device).

> :book: ***Why Name Wallets?*** To date, this course has used the default ("") wallet in Bitcoin Core. This is fine for many purposes, but is inadequate if you have a more complex situation, such as when you're watching keys from a hardware wallet. Here, we want to be able to differentiate from the locally owned keys (which are kept in the "" wallet) and the remotely owned keys (which are kept in the "ledger" wallet).

You can now see that the new wallet is in your list:
```
$ bitcoin-cli listwallets
[
  "",
  "ledger"
]
```
Because you've created a second wallet, some commands will now require a `-rpcwallet=` flag, to specify which wallet you're using

### Importing the Keys

We now have to import a list of addresses from the hardware wallet to watch. This is done with HWI's `getkeypool` command:
```
$ hwi -f 9a1d520b getkeypool --wpkh 0 1000
[{"desc": "wpkh([9a1d520b/84h/1h/0h]tpubDD7KTtoGzK9GuWUQcr1uTJazsAkqoXhdrwGXWVix6nPpNZmSbagZWD4QSaMsyK8YohAirGDPrWdRiEpKzTFB7DrTrqfzHCn7yi5EsqeR93S/0/*)#qttxy592", "range": [0, 1000], "timestamp": "now", "internal": false, "keypool": true, "active": true, "watchonly": true}, {"desc": "wpkh([9a1d520b/84h/1h/0h]tpubDD7KTtoGzK9GuWUQcr1uTJazsAkqoXhdrwGXWVix6nPpNZmSbagZWD4QSaMsyK8YohAirGDPrWdRiEpKzTFB7DrTrqfzHCn7yi5EsqeR93S/1/*)#3lw8ep4j", "range": [0, 1000], "timestamp": "now", "internal": true, "keypool": true, "active": true, "watchonly": true}]
```
We ask for the first 1000 WPKH (native Segwit) addresses and receive two descriptors for a key pool: one for receiving addresses and one for change addresses.

> :book: ***What is a key pool?*** A key pool is a group of pregenerated keys. Modern HD wallets generate key pools by continuing to to determine new hierarchical addresses based on the original seed. The idea of key pools was originally implemented to ease the backup requirements of wallets. This allowed a keypool to be generated, and then the wallet to be backed up, rather than requiring backups after every new address was created. The concept has also proven very useful in the modern day since it allows the importing of a whole set of future addresses from one device to another.

These are the same sorts of descriptors that we learned about in [§3.5: Understanding the Descriptor](03_5_Understanding_the_Descriptor.md). At the time, we said that they were most useful for moving addresses among different machines. Here's the real-life example: moving addresses from a hardware wallet to Bitcoin Core node, so that our net-connected machine will be able to watch over them.

Just as we learned in [§3.5](03_5_Understanding_the_Descriptor.md), you can examine these descriptors with the `getdescriptorinfo` RPC:
```
$ bitcoin-cli getdescriptorinfo "wpkh([9a1d520b/84h/1h/0h]tpubDD7KTtoGzK9GuWUQcr1uTJazsAkqoXhdrwGXWVix6nPpNZmSbagZWD4QSaMsyK8YohAirGDPrWdRiEpKzTFB7DrTrqfzHCn7yi5EsqeR93S/0/*)#qttxy592"
{
  "descriptor": "wpkh([9a1d520b/84'/1'/0']tpubDD7KTtoGzK9GuWUQcr1uTJazsAkqoXhdrwGXWVix6nPpNZmSbagZWD4QSaMsyK8YohAirGDPrWdRiEpKzTFB7DrTrqfzHCn7yi5EsqeR93S/0/*)#n65e7wjf",
  "checksum": "qttxy592",
  "isrange": true,
  "issolvable": true,
  "hasprivatekeys": false
}
```
As you'd expect, who do _not_ `hasprivatekeys`, because hardware wallets hold on to those.

You can import the keys into your new `ledger` wallet using the `importmulti` RPC that you also met in [§3.5](03_5_Understanding_the_Descriptor.md). In this case, just put the entire response you got back from HWI in `'`s.
```
$ bitcoin-cli -rpcwallet=ledger importmulti '[{"desc": "wpkh([9a1d520b/84h/1h/0h]tpubDD7KTtoGzK9GuWUQcr1uTJazsAkqoXhdrwGXWVix6nPpNZmSbagZWD4QSaMsyK8YohAirGDPrWdRiEpKzTFB7DrTrqfzHCn7yi5EsqeR93S/0/*)#qttxy592", "range": [0, 1000], "timestamp": "now", "internal": false, "keypool": true, "active": true, "watchonly": true}, {"desc": "wpkh([9a1d520b/84h/1h/0h]tpubDD7KTtoGzK9GuWUQcr1uTJazsAkqoXhdrwGXWVix6nPpNZmSbagZWD4QSaMsyK8YohAirGDPrWdRiEpKzTFB7DrTrqfzHCn7yi5EsqeR93S/1/*)#3lw8ep4j", "range": [0, 1000], "timestamp": "now", "internal": true, "keypool": true, "active": true, "watchonly": true}]'
[
  {
    "success": true
  },
  {
    "success": true
  }
]
```
You can now view the watch-only addresse that you received using the `getaddressesbylabel` command. All 1000 of the receive addresses are right there, in the `ledger` wallet!
```
$ bitcoin-cli -rpcwallet=ledger getaddressesbylabel "" | more
{
  "tb1qqqvnezljtmc9d7x52udpc0m9zgl9leugd2ur7y": {
    "purpose": "receive"
  },
  "tb1qqzvrm6hujdt93qctuuev5qc4499tq9fdk0prwf": {
    "purpose": "receive"
  },
...
}
```
## Create a Transaction with PSBT

## Summary: Integrating with Hardware Wallets

> :fire: ***What's the power of HWI?***

## What's Next?

Move on to "Bitcoin Scripting" with [Chapter Seven: Introducing Bitcoin Scripts](07_0_Introducing_Bitcoin_Scripts.md).
