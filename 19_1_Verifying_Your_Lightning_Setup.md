# 19.1: Creating a c-lightning Setup

> :information_source: **NOTE:** This section has been recently added to the course and is an early draft that may still be awaiting review. Caveat reader.

In this section, you'll install and verify c-lightning, your utility for accessing the Lightning Network.

> :book: ***What is the Lightning Network?*** The Lightning Network is a decentralized network that uses the smart contract functionality of the Bitcoin blockchain to enable instant payments across a network of participants. Lightning is built as a layer-2 protocol that interacts with Bitcoin to allow users to exchange their bitcoins "off-chain". 

> :book: ***What is a layer-2 protocol?*** Layer 2 refers to a secondary protocol built on top of the Bitcoin blockchain system. The main goal of these protocols is to solve the transaction speed and scaling difficulties that are present in Bitcoin: Bitcoin is not able to process thousands of transactions per second (TPS), so layer-2 protocols have been created to solve the blockchain scalability problem. These solutions are also known as "off-chain" scaling solutions.

## Install C-Lightning

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
If not, you'll need to install it now. Unfortunately, if you're using Debian you'll need to install it by hand, by compiling the source code — but it should still be pretty simple if you follow these instructions. If you happen to be on a standard Ubuntu system, instead try [Installing from Ubuntu ppa](#variant-install-from-ubuntu-ppa), and you can always attempt [Installing Pre-compiled Binaries](#variant-install-pre-compiled-binaries).

> :book: ***What is c-lightning?*** There are three different implementations of Lightning at present: c-lightning, LND, and Eclair. They should all be functionally compatible, based on the same [BOLT RFCs](https://github.com/lightningnetwork/lightning-rfc/blob/master/00-introduction.md), but their implementation details may be different. We've chosen c-lightning as the basis of our course because it's also part of the same [Elements Project](https://github.com/ElementsProject) that also contains Libwally.

### Compile the c-lightning Source Code 

Installing Lightning from source code should actually be pretty simple if you follow these instructions.

You _probably_ want to do this on an unpruned node, as working with pruned nodes on Lightning may cause issues with installation and usage. If you set up your node way back at the start of this course to be pruned, you may wish to replace it with an unpruned node now. (If you're using testnet, you should be able to use the same type of machine as you did for your pruned node.)

> :warning: **WARNING:** You actually can run c-lightning on a pruned node. However, as the [Lightning repo](https://github.com/ElementsProject/lightning#pruning) notes, there may be issues. To make it work you have to ensure that your Lightning node is only ever trying to update info on blocks that your Bitcoin node has not pruned. To do so you must make sure (1) that your Bitcoin node is fully up to date before you start your Lightning node for the first time; and (2) that your Lightning node never falls too far behind your Bitcoin node (for a standard 550-block pruning, it can never be turned off for 4 or more days). So, you can do it, but it does introduce some danger, which isn't a good idea if you're running a production service.

With that, you're ready to install Lightning:

First, install dependencies, including development requirements.
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
Now, compile. This may also take some time depending your machine.
```
$ make
```
Afterward, all you need to do is install:
```
$ sudo make install
```

## Check Your installation

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

## Run lightningd

You'll begin your exploration of the Lightning network with the `lightning-cli` command. However,`lightningd` _must_ be running to use `lightning-cli`, as `lightning-cli` sends JSON-RPC commands to the `lightningd` (all just as with `bitcoin-cli` and `bitcoind`). 

If you installed `c-lightning` by hand, you'll now need to start it:
```
$ nohup lightningd --testnet &
```

### Run lightningd as a service

If you prefer, you can install `lightningd` as a service that will run every time you restart your machine. The following will do so, and start it running immediately:

```
$ cat > ~/lightningd.service << EOF
# It is not recommended to modify this file in-place, because it will
# be overwritten during package upgrades. If you want to add further
# options or overwrite existing ones then use
# $ systemctl edit bitcoind.service
# See "man systemd.service" for details.
# Note that almost all daemon options could be specified in
# /etc/lightning/config, except for those explicitly specified as arguments
# in ExecStart=
[Unit]
Description=c-lightning daemon
[Service]
ExecStart=/usr/local/bin/lightningd --testnet
# Process management
####################
Type=simple
PIDFile=/run/lightning/lightningd.pid
Restart=on-failure
# Directory creation and permissions
####################################
# Run as standup
User=standup
# /run/lightningd
RuntimeDirectory=lightningd
RuntimeDirectoryMode=0710
# Hardening measures
####################
# Provide a private /tmp and /var/tmp.
PrivateTmp=true
# Mount /usr, /boot/ and /etc read-only for the process.
ProtectSystem=full
# Disallow the process and all of its children to gain
# new privileges through execve().
NoNewPrivileges=true
# Use a new /dev namespace only populated with API pseudo devices
# such as /dev/null, /dev/zero and /dev/random.
PrivateDevices=true
# Deny the creation of writable and executable memory mappings.
MemoryDenyWriteExecute=true
[Install]
WantedBy=multi-user.target
EOF
$ sudo cp ~/lightningd.service /etc/systemd/system
$ sudo systemctl enable lightningd.service
$ sudo systemctl start lightningd.service
```

### Enable Remote Connections

If you have some sort of firewall, you'll need to open up port 9735, to allow other Lightning nodes to talk to you.

If you use `ufw` from Bitcoin Standup, this is done as follows:
```
$ sudo ufw allow 9735
```

## Verify your Node

You can check if your Lightning node is ready to go by comparing the output of `bitcoin-cli getblockcount` with the `blockheight` result from `lightning-cli getinfo`.

```
$ bitcoin-cli -testnet getblockcount
1838587
$ lightning-cli --testnet getinfo
{
   "id": "03d4592f1244cd6b5a8bb7fba6a55f8a91591d79d3ea29bf8e3c3a405d15db7bf9",
   "alias": "HOPPINGNET",
   "color": "03d459",
   "num_peers": 0,
   "num_pending_channels": 0,
   "num_active_channels": 0,
   "num_inactive_channels": 0,
   "address": [
      {
         "type": "ipv4",
         "address": "74.207.240.32",
         "port": 9735
      },
      {
         "type": "ipv6",
         "address": "2600:3c01::f03c:92ff:fe48:9ddd",
         "port": 9735
      }
   ],
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
   "version": "v0.9.1-96-g6f870df",
   "blockheight": 1838587,
   "network": "testnet",
   "msatoshi_fees_collected": 0,
   "fees_collected_msat": "0msat",
   "lightning-dir": "/home/standup/.lightning/testnet"
}
```
In this case, the `blockheight` is shown as `1838587` by both commends.

You may instead get an error, depending on the precise situation.

If the Bitcoin node is still sycing with bitcoin network you should see a message like this:
```
"warning_bitcoind_sync": "Bitcoind is not up-to-date with network."
```
If your lightning daemon is not up-to-date, you'll get a message like this:
```
"warning_lightningd_sync": "Still loading latest blocks from bitcoind."
```
If you tried to run on a pruned blockchain where the Bitcoin node wasn't up to date when you started the Lightning node, you'll get error messages in your log like this:
```
bitcoin-cli -testnet getblock 0000000000000559febee77ab6e0be1b8d0bef0f971c7a4bee9785393ecef451 0 exited with status 1
```

## Create Aliases

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
After you enter these aliases you can either `source ~/.bash_profile` to input them or just log out and back in.

Note that these aliases include shortcuts for running `lightning-cli`, for running `lightningd`, and for going to the c-lightning directory. These aliases are mainly meant to make your life easier. We suggest you create other aliases to ease your use of frequent commands (and arguments) and to minimize errors. Aliases of this sort can be even more useful if you have a complex setup where you regularly run commands associated with Mainnet, with Testnet, _and_ with Regtest, as explained further below.

With that said, use of these aliases in _this_ document might accidentally obscure the core lessons being taught about c-lightning, so we'll continue to show the full commands; adjust for your own use as appropriate.

##  Optional: Modify Your Server Types

> :link: **TESTNET vs MAINNET:** When you set up your node, you choose to create it as either a Mainnet, Testnet, or Regtest node. Though this document presumes a testnet setup, it's worth understanding how you might access and use the other setup types — even all on the same machine! But, if you're a first-time user, skip on past this, as it's not necessary for a basic setup.

When lightningd starts up, it usually reads a configuration file whose location is dependent on the network you are using (default: `~/.lightning/testnet/config`). This can be changed with the `–conf` and `–lightning-dir` flags.

```
~/.lightning/testnet$ ls -la config
-rw-rw-r-- 1 user user 267 jul 12 17:08 config
```
There is also a general configuration file (default: `~/.lightning/config`). If you want to run several different sorts of nodes simultaneously, you must leave the testnet (or regtest) flag out of this configuration file. You should then choose whether you're using the mainnet, the testnet, or your regtest every time you run `lightningd` or `lightning-cli`.

Your setup may not actually have any config files: c-lightning will run with a good default setup without them.

## Summary: Verifying your Lightning setup

Before you start playing with lightning, you should make sure that your aliases are set up, your  `lightningd` is running, and your node is synced. You may also want to set up some access to alternative lightning setups, on other networks.

## What's Next?

Continue "Understanding Your Lightning Setup" with [§19.2: Knowing Your Lightning Setup](19_2_Knowing_Your_lightning_Setup.md).

## Variant: Install from Ubuntu ppa

If you are using a Ubuntu version other than Debian, you can install c-lightning using [Ubuntu ppa](https://launchpad.net/~lightningnetwork/+archive/ubuntu/ppa):

```
$ sudo apt-get install -y software-properties-common
$ sudo add-apt-repository -u ppa:lightningnetwork/ppa
$ sudo apt-get install lightningd
```

## Variant: Install Pre-compiled Binaries

Another method for installing Lightning is to use the precompiled binaries on the [Github repo](https://github.com/ElementsProject/lightning/releases). Choose the newest tarball, such as `clightning-v0.9.1-Ubuntu-20.04.tar.xz`.

After downloading it, you need to move to root directory and unpackage it:
```
$ cd /
$ sudo tar xf ~/clightning-v0.9.1-Ubuntu-20.04.tar.xz 
```
Warning: this will require you to have the precise same libraries as were used to create the binary. It's often easier to just recompile.
