# 7.3: Integrating with Hardware Wallets

> :information_source: **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

One of the greatest powers of PSBTs is the ability to hand transactions off to hardware devices. This will be a great development tool for you if you continue to program with Bitcoin. However, you can't test it out if you're using one of the configurations we suggest for this course — a VM on Linode per [§2.2](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/02_2_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md) or an even more farflung option such an AWS per [§2.3](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/02_3_Setting_Up_Bitcoin_Core_Other.md) — because obviously you won't  have any way to hook a hardware device up to your remote, virtual machine

So, you have three options for moving through this chapter on hardware wallets: (1) read along without testing the code; (2) install Bitcoin on a local machine to fully test these commands; or (3) skip straight ahead to [Chapter 8: Expanding Bitcoin Transactions in Other Ways](08_0_Expanding_Bitcoin_Transactions_Other.md). We suggest option #1, but if you really want to test these commands out we'll also give some support for #2 by talking about using a Macintosh for testing (a hardware-platform supported by [Bitcoin Standup](https://github.com/BlockchainCommons/Bitcoin-Standup)).

> :warning: **VERSION WARNING:** This is an innovation from Bitcoin Core v 0.17.0. Earlier versions of Bitcoin Core will not be able to work with the PSBT while it is in process (though they will still be able to recognize the final transaction).

The methodology for integrating with a Hardware Wallet that is described in this chapter depends on the [Bitcoin Hardware Wallet Interface](https://github.com/bitcoin-core/HWI) released through Bitcoin Core and builds on the [installation](https://github.com/bitcoin-core/HWI/blob/master/README.md) and [usage](https://github.com/bitcoin-core/HWI/blob/master/docs/bitcoin-core-usage.md) instructions found there.

## Install Bitcoin Core on a Local Machine

_If you just plan to read over this section and not test out these commands until some future date when you have a local development environment set up, you can skip this instruction, which is about creating a Bitcoin Core installation on a local machine such as a Mac or UNIX machine._

There are alternate versions of the Bitcoin Standup script that you used to create your VM that will install on a MacOS or on a non-Linode UNIX.

If you have MacOS, you can install [Bitcoin Standup MacOS](https://github.com/BlockchainCommons/Bitcoin-Standup-MacOS/blob/master/README.md).

If you have a local UNIX machine, you can install [Bitcoin Standup Linux Scripts](https://github.com/BlockchainCommons/Bitcoin-Standup-MacOS/blob/master/README.md).

We will be using a Macintosh for the examples in this section.

### Create an Alias for Bitcoin-CLI

Create an alias that causes `bitcoin-cli` to run the correct command from the correct directory with any appropriate flags.

Here's an example alias from a Mac:
```
Mac$ $ alias bitcoin-cli="~/StandUp/BitcoinCore/bitcoin-0.20.0/bin/bitcoin-cli -testnet"
```
## Install HWI on a Local Machine

_The following instructions again assume a Mac._

HWI is a Bitcoin Core program available in python.

### Install Python

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
$ brew install libusb
$ brew install poetry
```


### Install HWI

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
$ poetry install
Creating virtualenv hwi-Qw5zZLjb-py3.8 in /Users/shannona/Library/Caches/pypoetry/virtualenvs
Installing dependencies from lock file


Package operations: 16 installs, 0 updates, 0 removals

  - Installing altgraph (0.17)
  - Installing dis3 (0.1.3)
  - Installing mccabe (0.6.1)
  - Installing pbkdf2 (1.3)
  - Installing pycodestyle (2.6.0)
  - Installing pyflakes (2.2.0)
  - Installing autopep8 (1.5.2)
  - Installing ecdsa (0.13.3)
  - Installing flake8 (3.8.1)
  - Installing hidapi (0.7.99.post21)
  - Installing libusb1 (1.8)
  - Installing macholib (1.14)
  - Installing mnemonic (0.18)
  - Installing pyaes (1.6.1)
  - Installing pyinstaller (3.6)
  - Installing typing-extensions (3.7.4.2)
  - Installing hwi (1.1.2)
```

## Prepare Your Ledger

_We had to choose a hardware-wallet platform too, for this HWI demonstration. Our choice was Ledger, which has long been our testbed for hardware wallets. Please see [HWI's device support info](https://github.com/bitcoin-core/HWI/blob/master/README.md#device-support) for a list of other supported devices. If you use a device other than a Ledger, you'll need to assess your own solutions for preparing it for usage on the Testnet, but otherwise you should be able to continue with the course as written._

If you are working with Bitcoins on your Ledger, you probably won't need to do anything. (But we don't suggest that for use with this course). 

To work with Testnet, as suggested by this course, you'll need to make a few updates:

1. Go to Settings on your Ledger Live app (it's the gear), go to the "Experimental Features" tab, and turn on "Developer Mode"
2. Go to the "Manager" and install "Bitcoin Test". The current version requires that you have "Bitcoin" and "Etereum" installed first.
3. Go to the "Manager", scroll to your new "Bitcoin Test", and "Add Account"

TO fully test, you will also want to request some Testnet coins from a faucet as described in [§3.4: Receiving a Transaction](03_4_Receiving_a_Transaction.md).

## Link to a Ledger

## Import Addresses

## Create a Transaction with PSBT

## Summary: Integrating with Hardware Wallets

> :fire: ***What's the power of HWI?***

## What's Next?

Move on to "Bitcoin Scripting" with [Chapter Seven: Introducing Bitcoin Scripts](07_0_Introducing_Bitcoin_Scripts.md).
