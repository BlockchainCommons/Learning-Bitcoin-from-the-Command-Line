
# 18.1: Verifying Your Lightning Setup

> :information_source: **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

In this section, you'll install and verify c-lightning, your utility for accessing the Lightning Network.

> :book: ***What is Lightning Network?*** The Lightning Network is a decentralized network that uses the smart contract functionality of the Bitcoin blockchain to enable instant payments across a network of participants. Lightning is built as a layer-2 protocol that interacts with Bitcoin to allow users to exchange their bitcoins "off-chain". 

> :book: ***What is a layer-2 protocol?*** Layer 2 refers to a secondary protocol built on top of the Bitcoin blockchain system. The main goal of these protocols is to solve the transaction speed and scaling difficulties that are present in Bitcoin: Bitcoin is not able to process thousands of transactions per second (TPS), so layer-2 protocols have been created to solve the blockchain scalability problem. These solutions are also known as "off-chain" scaling solutions.

## Installing C-Lightning

If you used the [Bitcoin Standup Scripts](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts), you may have already installed Lightning at the beginning of this course. You can test this by seeing if `lightningd` is running:
```
$ ps auxww | grep -i lightning
standup  31213  0.0  0.2  24144 10424 pts/0    S    15:38   0:00 lightningd --testnet
standup  31214  0.0  0.1  22716  7444 pts/0    S    15:38   0:00 /usr/local/bin/../libexec/c-lightning/plugins/autoclean
standup  31215  0.0  0.2  22992  8248 pts/0    S    15:38   0:00 /usr/local/bin/../libexec/c-lightning/plugins/bcli
standup  31216  0.0  0.1  22756  7604 pts/0    S    15:38   0:00 /usr/local/bin/../libexec/c-lightning/plugins/keysend
standup  31217  0.0  0.1  22776  7648 pts/0    S    15:38   0:00 /usr/local/bin/../libexec/c-lightning/plugins/pay
standup  31218  0.0  0.1  22720  7652 pts/0    S    15:38   0:00 /usr/local/bin/../libexec/c-lightning/plugins/txprepare
standup  31219  0.0  0.1  22744  7716 pts/0    S    15:38   0:00 /usr/local/bin/../libexec/c-lightning/plugins/spenderp
standup  31227  0.0  0.1  22748  7384 pts/0    SL   15:38   0:00 /usr/local/libexec/c-lightning/lightning_hsmd
standup  31228  0.0  0.2  23044  8192 pts/0    S    15:38   0:00 /usr/local/libexec/c-lightning/lightning_connectd
standup  31229  0.0  0.1  22860  7556 pts/0    S    15:38   0:00 /usr/local/libexec/c-lightning/lightning_gossipd
standup  32072  0.0  0.0   6208   888 pts/0    S+   15:50   0:00 grep -i lightning
```
If not, you'll need to install it by hand. Unfortunately, if you're using Debian the simplest methods don't work, and you'll need to install by hand (which should still be pretty simple if you follow our instructions. If you happen to be on a standard Ubuntu system, instead try [Installing from Ubuntu ppa]() and you can always attempt [Installing Pre-compiled Binaries]().

> :book: ***What is c-lightning?*** There are three different implementations of Lightning at present: C-Lightning, LND, and Eclair. They should all be functionally compatible, based on the same [BOLT RFCs](https://github.com/lightningnetwork/lightning-rfc/blob/master/00-introduction.md), but their implementation details may be different. We've chosen c-lightning as the basis of our course in large part because it's also part of the same [Elements Project](https://github.com/ElementsProject) that also contains Libwally.

### Compiling the Source Code 

Installing Lightning from source code should actually be pretty simple if you follow these instructions.

You also _probably_ want to do this on an unpruned node, as working with pruned nodes on Lightning may cause issues with installation and usage. If you set up your node way back at the start of this course to be pruned, you may wish to replace it with an unpruned node now. (If you're using testnet, you should be able to use the same size machine as you did for your pruned node.)

With that, you're ready to install Lightning:

First you're going to need to install dependencies, including development requirements.
```
$ sudo apt-get install -y \
   autoconf automake build-essential git libtool libgmp-dev \
   libsqlite3-dev python3 python3-mako net-tools zlib1g-dev libsodium-dev \
   gettext
$ sudo apt-get install -y valgrind python3-pip libpq-dev
```
These can take a while, because there are a number of them, and some are large.

Second, clone the Lightning repo:
```
$ cd ~
$ git clone https://github.com/ElementsProject/lightning.git
$ cd lightning
```
You can now use the `pip3` you installed to install additional requirements for the compilation, and configure it all:
```
$ pip3 install -r requirements.txt
$ ./configure
```
You can now compile. This may also take some time depending your machine.
```
$ make
```
Afterward, all you need to do is install:
```
$ sudo make install
```

## Checking Your installation

You can confirm you have installed lightningd correctly using the `help` parameter:

```
$ lightningd --help
lightningd: WARNING: default network changing in 2020: please set network=testnet in config!
Usage: lightningd 
A bitcoin lightning daemon (default values shown for network: testnet).
--conf=<file>                        Specify configuration file
--lightning-dir=<dir>                Set base directory: network-specific
                                     subdirectory is under here
                                      (default: "/home/javier/.lightning")
--network <arg>                      Select the network parameters (bitcoin,
                                     testnet, regtest, litecoin or
                                     litecoin-testnet) (default: testnet)
--testnet                            Alias for --network=testnet
--signet                             Alias for --network=signet
--mainnet                            Alias for --network=bitcoin

```

## Running lightningd

You'll begin your exploration of the Lightning network with the `lightning-cli` command. However,`lightningd` _must_ be running to use `lightning-cli`, as `lightning-cli` sends JSON-RPC commands to the lightningd (all just as with `bitcoin-cli` and `bitcoind`). If you installed `c-lightning by hand, you'll now need to start it:
```
$ nohup lightningd --testnet &
```
(If you installed c-lightning using a Bitcoin Standup Script you'll already have a linux service that will start the `lightningd` daemon automatically whenever you reboot.)

## Verifying your node

You should have an output like this indicating your node is ready if blockheight  shows a height value that match with your most recent number getblockcount `bitcoin-cli getblockcount`  command output.

```
$ lightning-cli --network=testnet getinfo
{
   "id": "03fce2a20393a65b9d6cab5425f4cd33ddc621ade458efd69d652917e2b5eaf59c",
   "alias": "VIOLENTIRON",
   "color": "03fce2",
   "num_peers": 0,
   "num_pending_channels": 0,
   "num_active_channels": 0,
   "num_inactive_channels": 0,
   "address": [],
   "binding": [
      {
         "type": "ipv6",
         "address": "::",
         "port": 9735
      },
      {
         "type": "ipv4",
         "address": "0.0.0.0",
         "port": 9735
      }
   ],
   "version": "v0.8.2-398-g869fa08",
   "blockheight": 54959,
   "network": "testnet",
   "msatoshi_fees_collected": 0,
   "fees_collected_msat": "0msat",
   "lightning-dir": "/home/user/.lightning/testnet"
}
```

If node is still sycing with bitcoin network you should see a message like this.

```
"warning_bitcoind_sync": "Bitcoind is not up-to-date with network."
```
If your lightning daemon is not up-to-date, you'll get a message to you `getinfo command` like this:
```
"warning_lightningd_sync": "Still loading latest blocks from bitcoind."
```
If you tried to run on a pruned blockchain, you'll get error messages in your log like this:
```
bitcoin-cli -testnet getblock 0000000000000559febee77ab6e0be1b8d0bef0f971c7a4bee9785393ecef451 0 exited with status 1
```




## Create Your Aliases

We suggest creating some aliases to make it easier to use c-lightning.

You can do so by putting them in your `.bash_profile`.
```
cat >> ~/.bash_profile <<EOF
alias lndir="cd ~/.lightning/" #linux default c-lightning path
alias lnc="lightning-cli"
alias lnd="lightningd"
alias lninfo='lightning-cli getinfo'
EOF
```
After you enter these aliases you can either `source .bash_profile` to input them or just log out and back in.

Note that these aliases includes shortcuts for running `lightning-cli`, for running `lightningd`, and for going to the c-lightning directory. These aliases are mainly meant to make your life easier. We suggest you create other aliases to ease your use of frequent commands (and arguments) and to minimize errors. Aliases of this sort can be even more useful if you have a complex setup where you regularly run commands associated with Mainnet, with Testnet, _and_ with Regtest, as explained further below.

With that said, use of these aliases in _this_ document might accidentally obscure the core lessons being taught about c-lightning, so the only alias directly used here is `lninfo` because it encapsulatea  much longer and more complex command. Otherwise, we show the full commands; adjust for your own use as appropriate.
## Optional: Know Your Server Types

> **TESTNET vs MAINNET:** When you set up your node, you choose to create it as either a Mainnet, Testnet, or Regtest node. Though this document presumes a testnet setup, it's worth understanding how you might access and use the other setup types — even all on the same machine! But, if you're a first-time user, skip on past this, as it's not necessary for a basic setup.

When lightningd starts up it usually reads a general configuration file located depending on the network you are using (default: $HOME/.lightning/testnet/config). This can be changed: see –conf and –lightning-dir.

The type of setup is mainly controlled through the ~/.lightning/config file. If you're running testnet, it probably will be located in ~/.lightning/testnet/config.   In next section we will explain how to manage your lightning daemon options or general options.

```
~/.lightning/testnet$ ls -la config
-rw-rw-r-- 1 user user 267 jul 12 17:08 config
:~/.lightning/testnet$ 

```
If you want to run several different sorts of nodes simultaneously, you must leave the testnet (or regtest) flag out of your configuration file. You should then choose whether you're using the mainnet, the testnet, or your regtest every time you run lightningd or lightning-cli.

## Summary: Verifying your Lightning setup

Before you start playing with lightning, you should make sure that your aliases are set up, your lightningd is running, and your node is synced. You may also want to set up some access to alternative lightning setups, if you're an advanced user.

## What's Next?

Continue "Understanding Your Lightning Setup" with [§18.2: Knowing Your Lightning Setup](18_2_Knowing_Your_lightning_Setup.md).

### Variant: Installing from Ubuntu ppa

If you are using a Ubuntu version other than Debian, you can install c-lightning using [Ubuntu ppa](https://launchpad.net/~lightningnetwork/+archive/ubuntu/ppa):

```
$ sudo apt-get install -y software-properties-common
$ sudo add-apt-repository -u ppa:lightningnetwork/ppa
$ sudo apt-get install lightningd
```

### Variant: Installing Pre-compiled Binaries

Currently, the easiest way to install c-lightning on Debian is to use the precompiled binaries on the [Github repo](https://github.com/ElementsProject/lightning/releases). Choose the newest tarball, such as `clightning-v0.9.1-Ubuntu-20.04.tar.xz`.

$ sudo apt-get install libpq5
sudo apt-get install libsodium23

After downloading it, you need to move to root directory and unpackage it:
```
$ cd /
$ sudo tar xf ~/clightning-v0.9.1-Ubuntu-20.04.tar.xz 
```



