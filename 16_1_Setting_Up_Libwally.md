# 16.1: Setting Up Libwally

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

This first section will explain how to download Libwally and how to get it working.

> :book: ***What is Libwally?*** Libwally is a library of primitives helpful for the creation of wallets that is cross-platform and cross-language, so that the same functions can be used everywhere. There are [extensive docs](https://wally.readthedocs.io/en/release_0.7.8/). It's made available as part of Blockstream's [Elements Project](https://github.com/ElementsProject).

## Installing Libwally

As usual, you'll need some packages on your system:
```
$ sudo apt-get install git
$ sudo apt-get install dh-autoreconf
```

You can download Libwally from its Git repo:
```
$ git clone https://github.com/ElementsProject/libwally-core
```
You can then get things installed:
```
$ ./tools/autogen.sh
```
As with `libbitcoinrpc`, you may wish to install this in `/usr/include` and `/usr/lib` for ease of usage. Just modify the appropriate line in the `configure` program:
```
< ac_default_prefix=/usr
---
> ac_default_prefix=/usr/local
```
Afterward you can finish your prep:
```
$ ./configure
$ make
```
You can then verify that tests are work:
```
$ make check
Making check in src
make[1]: Entering directory '/home/standup/libwally-core/src'
Making check in secp256k1
make[2]: Entering directory '/home/standup/libwally-core/src/secp256k1'
make  check-TESTS
make[3]: Entering directory '/home/standup/libwally-core/src/secp256k1'
make[4]: Entering directory '/home/standup/libwally-core/src/secp256k1'
============================================================================
Testsuite summary for libsecp256k1 0.1
============================================================================
# TOTAL: 0
# PASS:  0
# SKIP:  0
# XFAIL: 0
# FAIL:  0
# XPASS: 0
# ERROR: 0
============================================================================
make[4]: Leaving directory '/home/standup/libwally-core/src/secp256k1'
make[3]: Leaving directory '/home/standup/libwally-core/src/secp256k1'
make[2]: Leaving directory '/home/standup/libwally-core/src/secp256k1'
make[2]: Entering directory '/home/standup/libwally-core/src'
make  check-TESTS check-local
make[3]: Entering directory '/home/standup/libwally-core/src'
make[4]: Entering directory '/home/standup/libwally-core/src'
PASS: test_bech32
PASS: test_psbt
PASS: test_psbt_limits
PASS: test_tx
============================================================================
Testsuite summary for libwallycore 0.7.8
============================================================================
# TOTAL: 4
# PASS:  4
# SKIP:  0
# XFAIL: 0
# FAIL:  0
# XPASS: 0
# ERROR: 0
============================================================================
make[4]: Leaving directory '/home/standup/libwally-core/src'
make[3]: Nothing to be done for 'check-local'.
make[3]: Leaving directory '/home/standup/libwally-core/src'
make[2]: Leaving directory '/home/standup/libwally-core/src'
make[1]: Leaving directory '/home/standup/libwally-core/src'
make[1]: Entering directory '/home/standup/libwally-core'
make[1]: Nothing to be done for 'check-am'.
make[1]: Leaving directory '/home/standup/libwally-core'
```
Finally, you can install:
```
$ sudo make install
```

