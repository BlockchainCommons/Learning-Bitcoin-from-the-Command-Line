
# 18.1: Verifying Your Lightning Setup

> :information_source: **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

> :book: ***What is Lightning Network?*** 

Lightning Network is a decentralized network using smart contract functionality in the bitcoin blockchain to enable instant payments across a network of participants.   Lightning is builded like a layer-2 protocol that interacts with Bitcoin to allow users to exchange their bitcoins "off-chain". 

> :book: ***What is a layer-2 protocol?*** 

Layer 2 refers to a secondary protocol builded on top of bitcoin blockchain system. The main goal of these protocols is to solve the transaction speed and scaling difficulties that are present on the bitcoin system.   Bitcoin is not able to process thousands of transactions per second (TPS),  so 2 layer protocols are proposed to solve the blockchain scalability problem.   This solutions are known as “off-chain” scaling solutions.

## C-lightning installation

As we said previous chapter you should have c-lightning installed courtesy BitcoinStandupScript,  if not we describe here three options and steps to install it.

For this and future chapters, we presume that you have a VPS with c-lightning installed, running `lightningd` courtersy of [Bitcoin Standup Scripts](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts) on Testnet.

### Ubuntu ppa

The first and fastest option is to use [Ubuntu ppa](https://launchpad.net/~lightningnetwork/+archive/ubuntu/ppa).

```
sudo apt-get install -y software-properties-common
sudo add-apt-repository -u ppa:lightningnetwork/ppa
sudo apt-get install lightningd
```
### Install pre-compiled binaries

Download and install binaries from [c-lightning](https://github.com/ElementsProject/lightning/releases) github repository.

### Compiling the source code 

In this section we'll show you steps to compile c-lightning source code and running it.

* Get dependencies
```
c$ sudo apt-get install -y \
>   autoconf automake build-essential git libtool libgmp-dev \
>   libsqlite3-dev python3 python3-mako net-tools zlib1g-dev libsodium-dev \
>   gettext
```

```
Reading package lists... Done
Building dependency tree       
Reading state information... Done
build-essential is already the newest version (12.1ubuntu2).
net-tools is already the newest version (1.60-26ubuntu1).
python3 is already the newest version (3.5.1-3).
python3-mako is already the newest version (1.0.3+ds1-1ubuntu1).
gettext is already the newest version (0.19.7-2ubuntu3.1).
The following packages were automatically installed and are no longer required:
  glade2script glade2script-python3 pastebinit snapd-login-service
Use 'sudo apt autoremove' to remove them.
The following additional packages will be installed:
  autotools-dev git-man liberror-perl libgmpxx4ldbl libltdl-dev m4
  .......
```
* Clone repository

```
c$ git clone https://github.com/ElementsProject/lightning.git
c$ cd lightning
```

* Get additional dependencies for development or testing.

```
c$ sudo apt-get install -y valgrind python3-pip libpq-dev
```

```
Reading package lists... Done
Building dependency tree       
Reading state information... Done
The following packages were automatically installed and are no longer required:
  glade2script glade2script-python3 pastebinit snapd-login-service
Use 'sudo apt autoremove' to remove them.
The following additional packages will be installed:
  comerr-dev krb5-multidev libexpat1-dev libgssrpc4 libkadm5clnt-mit9 libkadm5srv-mit9 libkdb5-8 libpq5 libpython3-dev libpython3.5-dev libssl-dev libssl-doc
  python-pip-whl python3-dev python3-setuptools python3-wheel python3.5-dev
Suggested packages:
  krb5-doc krb5-user postgresql-doc-9.5 python-setuptools-doc valgrind-dbg kcachegrind alleyoop valkyrie
The following NEW packages will be installed:
  comerr-dev krb5-multidev libexpat1-dev libgssrpc4 libkadm5clnt-mit9 libkadm5srv-mit9 libkdb5-8 libpq-dev libpq5 libpython3-dev libpython3.5-dev libssl-dev libssl-doc
  python-pip-whl python3-dev python3-pip python3-setuptools python3-wheel python3.5-dev valgrind
0 upgraded, 20 newly installed, 0 to remove and 0 not upgraded.
Need to get 53,4 MB of archives.
After this operation, 137 MB of additional disk space will be used.
Get:1 http://es.archive.ubuntu.com/ubuntu xenial-updates/main amd64 libgssrpc4 amd64 1.13.2+dfsg-5ubuntu2.1 [54,5 kB]
Get:2 http://es.archive.ubuntu.com/ubuntu xenial-updates/main amd64 libkdb5-8 amd64 1.13.2+dfsg-5ubuntu2.1 [37,0 kB]
Get:3 http://es.archive.ubuntu.com/ubuntu xenial-updates/main amd64 libkadm5srv-mit9 amd64 1.13.2+dfsg-5ubuntu2.1 [51,3 kB]
.......
```
* Install requirements

```
c$ pip3 install -r requirements.txt
```

```
Collecting Flask==1.1.1 (from -r requirements.txt (line 1))
  Downloading https://files.pythonhosted.org/packages/9b/93/628509b8d5dc749656a9641f4caf13540e2cdec85276964ff8f43bbb1d3b/Flask-1.1.1-py2.py3-none-any.whl (94kB)
    100% |████████████████████████████████| 102kB 1.8MB/s 
Collecting Sphinx==1.8.4 (from -r requirements.txt (line 2))
  Downloading https://files.pythonhosted.org/packages/75/25/905f901fb749e45cb201f1d946e82f59d84730c430287ea7980b0bcbd2dc/Sphinx-1.8.4-py2.py3-none-any.whl (3.1MB)
    100% |████████████████████████████████| 3.1MB 372kB/s 
......
  Running setup.py install for mako ... done
  Running setup.py install for mistune-contrib ... done
  Running setup.py install for py-cpuinfo ... done
Successfully installed Flask-1.1.1 Jinja2-2.11.2 Pygments-2.6.1 Sphinx-1.8.4 Werkzeug-1.0.1 alabaster-0.7.12 apipkg-1.5 attrs-19.3.0 babel-2.8.0 cheroot-8.2.1 click-7.1.2 commonmark-0.8.1 docutils-0.16 entrypoints-0.3 ephemeral-port-reserve-1.1.1 execnet-1.7.1 flake8-3.7.8 flaky-3.6.1 future-0.18.2 imagesize-1.2.0 importlib-metadata-1.7.0 itsdangerous-1.1.0 jaraco.functools-3.0.1 m2r-0.2.1 mako-1.0.14 mccabe-0.6.1 mistune-0.8.4 mistune-contrib-0.1 more-itertools-8.4.0 mrkd-0.1.6 packaging-20.4 pathlib2-2.3.5 plac-1.2.0 pluggy-0.13.1 psycopg2-binary-2.8.4 py-1.9.0 py-cpuinfo-7.0.0 pycodestyle-2.5.0 pyflakes-2.1.1 pytest-5.3.1 pytest-benchmark-3.2.2 pytest-forked-1.0.2 pytest-timeout-1.3.3 pytest-xdist-1.29.0 python-bitcoinlib-0.10.2 pytz-2020.1 recommonmark-0.5.0 snowballstemmer-2.0.0 sphinx-rtd-theme-0.4.2 sphinxcontrib-websupport-1.1.0 tqdm-4.32.2 wcwidth-0.2.5 zipp-3.1.0
You are using pip version 8.1.1, however version 20.1.1 is available.
```

* Build lightning
```
c$ ./configure
```

```
Compiling ccan/tools/configurator/configurator...done
checking for python3-mako... found
Making autoconf users comfortable... yes
checking for off_t is 32 bits... no
checking for __alignof__ support... yes
checking for asprintf() declaration... yes
checking for __attribute__((cold)) support... yes
checking for __attribute__((const)) support... yes
checking for __attribute__((deprecated)) support... no
checking for __attribute__((nonnull)) support... yes
checking for __attribute__((sentinel)) support... yes
checking for __attribute__((pure)) support... yes
checking for __attribute__((may_alias)) support... yes
checking for __attribute__((noreturn)) support... yes
checking for __attribute__ format printf support... yes
.......
Setting COMPAT... 1
Setting PYTEST... pytest
Setting STATIC... 0
Setting ASAN... 0
Setting TEST_NETWORK... regtest
c$
```
* Make

It will take some time depending your machine.

```
c$ make
```

```
CC: cc -DBINTOPKGLIBEXECDIR="../libexec/c-lightning" -Wall -Wundef -Wmissing-prototypes -Wmissing-declarations -Wstrict-prototypes -Wold-style-definition -Werror -Wno-maybe-uninitialized -std=gnu11 -g -fstack-protector -Og -I ccan -I external/libwally-core/include/ -I external/libwally-core/src/secp256k1/include/ -I external/jsmn/ -I external/libbacktrace/ -I external/x86_64-linux-gnu/libbacktrace-build -I external/libsodium/src/libsodium/include -I external/libsodium/src/libsodium/include/sodium -I external/x86_64-linux-gnu/libsodium-build/src/libsodium/include -I . -I/usr/local/include    -DSHACHAIN_BITS=48 -DJSMN_PARENT_LINKS  -DCOMPAT_V052=1 -DCOMPAT_V060=1 -DCOMPAT_V061=1 -DCOMPAT_V062=1 -DCOMPAT_V070=1 -DCOMPAT_V072=1 -DCOMPAT_V073=1 -DCOMPAT_V080=1 -DCOMPAT_V081=1 -DCOMPAT_V082=1 -DCOMPAT_V090=1 -DBUILD_ELEMENTS=1 -c -o
LD: cc   -Og    -Lexternal/x86_64-linux-gnu -lwallycore -lsecp256k1 -ljsmn -lbacktrace -lsodium -L/usr/local/lib -lm -lgmp -lsqlite3 -lz  -lpq -o
cc ccan/ccan/cdump/tools/cdump-enumstr.c
cc ccan/ccan/cdump/cdump.c
cc ccan/ccan/strmap/strmap.c
cc ccan/ccan/asort/asort.c
cc ccan/ccan/autodata/autodata.c
cc ccan/ccan/bitmap/bitmap.c
cc ccan/ccan/bitops/bitops.c
cc ccan/ccan/breakpoint/breakpoint.c
cc ccan/ccan/crc32c/crc32c.c
cc ccan/ccan/crypto/hmac_sha256/hmac_sha256.c
cc ccan/ccan/crypto/hkdf_sha256/hkdf_sha256.c
......
cc common/channel_id.c
cc common/daemon.c
cc common/daemon_conn.c
cc common/derive_basepoints.c
cc common/funding_tx.c
tools/generate-wire.py --page impl common/gen_status_wire.h status < common/status_wire.csv > common/gen_status_wire.c
cc common/gen_status_wire.c
cc common/hash_u5.c
cc common/key_derive.c
cc common/memleak.c
cc common/msg_queue.c
cc common/node_id.c
cc common/permute_tx.c
cc common/setup.c
cc common/status.c
cc common/status_wire.c
cc common/subdaemon.c
cc common/type_to_string.c
cc common/utils.c
cc common/utxo.c
cc common/version.c
cc common/withdraw_tx.c
cc bitcoin/base58.c
cc bitcoin/block.c
cc bitcoin/chainparams.c
cc bitcoin/feerate.c
cc bitcoin/locktime.c
cc bitcoin/preimage.c
cc bitcoin/privkey.c
cc bitcoin/psbt.c
cc bitcoin/pubkey.c
cc bitcoin/script.c
cc bitcoin/shadouble.c
cc bitcoin/short_channel_id.c
cc bitcoin/signature.c
cc bitcoin/tx.c
cc bitcoin/tx_parts.c
cc bitcoin/varint.c
cc wire/wire_sync.c
.......
cc lightningd/test/run-invoice-select-inchan.c
ld lightningd/test/run-invoice-select-inchan
cc lightningd/test/run-jsonrpc.c
ld lightningd/test/run-jsonrpc
cc cli/test/run-remove-hint.c
ld cli/test/run-remove-hint
cc cli/test/run-large-input.c
ld cli/test/run-large-input
if [ x"`git log doc/lightningd-config.5 | head -n1`" != x"`git log doc/lightningd-config.5.md | head -n1`" -o x"`git diff doc/lightningd-config.5.md`" != x"" ]; then mrkd doc/lightningd-config.5.md doc/lightningd-config.5; else touch doc/lightningd-config.5; fi
if [ x"`git log doc/lightning-hsmtool.8 | head -n1`" != x"`git log doc/lightning-hsmtool.8.md | head -n1`" -o x"`git diff doc/lightning-hsmtool.8.md`" != x"" ]; then mrkd doc/lightning-hsmtool.8.md doc/lightning-hsmtool.8; else touch doc/lightning-hsmtool.8; fi
(grep -v '^   lightning.*\.[0-9]\.md>$' doc/index.rst; for m in $(cd doc && ls lightningd*.[0-9].md lightning-*.[0-9].md); do echo "   ${m%.[0-9].md} <$m>"; done |LC_ALL=C sort) > doc/index.rst.tmp.$$ && mv doc/index.rst.tmp.$$ doc/index.rst
rm external/x86_64-linux-gnu/libwally-core-build/src/secp256k1/libsecp256k1.la
c$
```
* Make Install

```
$ sudo make install
```

```
mkdir -p /usr/local/bin
mkdir -p /usr/local/libexec/c-lightning
mkdir -p /usr/local/libexec/c-lightning/plugins
mkdir -p /usr/local/share/man/man1
mkdir -p /usr/local/share/man/man5
mkdir -p /usr/local/share/man/man7
mkdir -p /usr/local/share/man/man8
mkdir -p /usr/local/share/doc/c-lightning
cp tools/hsmtool tools/lightning-hsmtool
install cli/lightning-cli lightningd/lightningd tools/lightning-hsmtool /usr/local/bin
install lightningd/lightning_channeld lightningd/lightning_closingd lightningd/lightning_connectd lightningd/lightning_gossipd lightningd/lightning_hsmd lightningd/lightning_onchaind lightningd/lightning_openingd /usr/local/libexec/c-lightning
[ -z "plugins/autoclean plugins/bcli plugins/fundchannel plugins/keysend plugins/pay" ] || install plugins/autoclean plugins/bcli plugins/fundchannel plugins/keysend plugins/pay /usr/local/libexec/c-lightning/plugins
install -m 644 doc/lightning-cli.1 /usr/local/share/man/man1
install -m 644 doc/lightningd-config.5 /usr/local/share/man/man5
install -m 644 doc/lightning-autocleaninvoice.7 doc/lightning-check.7 doc/lightning-checkmessage.7 doc/lightning-close.7 doc/lightning-connect.7 doc/lightning-createonion.7 doc/lightning-decodepay.7 doc/lightning-delexpiredinvoice.7 doc/lightning-delinvoice.7 doc/lightning-dev-sendcustommsg.7 doc/lightning-disconnect.7 doc/lightning-fundchannel.7 doc/lightning-fundchannel_start.7 doc/lightning-fundchannel_complete.7 doc/lightning-fundchannel_cancel.7 doc/lightning-getroute.7 doc/lightning-getsharedsecret.7 doc/lightning-invoice.7 doc/lightning-listchannels.7 doc/lightning-listforwards.7 doc/lightning-listfunds.7 doc/lightning-listinvoices.7 doc/lightning-listpays.7 doc/lightning-listpeers.7 doc/lightning-listsendpays.7 doc/lightning-newaddr.7 doc/lightning-pay.7 doc/lightning-plugin.7 doc/lightning-reserveinputs.7 doc/lightning-sendonion.7 doc/lightning-sendpay.7 doc/lightning-setchannelfee.7 doc/lightning-signmessage.7 doc/lightning-txprepare.7 doc/lightning-txdiscard.7 doc/lightning-txsend.7 doc/lightning-unreserveinputs.7 doc/lightning-waitinvoice.7 doc/lightning-waitanyinvoice.7 doc/lightning-waitblockheight.7 doc/lightning-waitsendpay.7 doc/lightning-withdraw.7 /usr/local/share/man/man7
install -m 644 doc/lightningd.8 doc/lightning-hsmtool.8 /usr/local/share/man/man8
install -m 644 README.md doc/INSTALL.md doc/HACKING.md LICENSE /usr/local/share/doc/c-lightning
```

## Check your installation

You can confirm you have installed correclty lightningd using this parameter:

```
c$ lightningd --help
```

```
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

### Run lightningd

You'll begin your exploration of the Lightning network with the `lightning-cli` command. However, lightningd _must_ be running to use lightning-cli, as lightning-cli sends JSON-RPC commands to the lightningd. If you used our standard setup, lightningd should already be up and running. You can double check by looking at the process table.
```
$ ps auxww | grep lightningd
standup   7817  1.1  0.2  95280  8964 pts/18   D+   14:13   0:00 lightningd --network=testnet
```
If it's not running, you'll want to run `lightningd --network=testnet` by hand.    If you installed c-lightning using Bitcoin Standup Script you'll have a linux service that will start lightningd daemon each start.

### Verify your node

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
