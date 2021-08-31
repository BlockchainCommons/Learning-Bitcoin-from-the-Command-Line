# 17.1: Setting Up Libwally

> :information_source: **NOTE:** This section has been recently added to the course and is an early draft that may still be awaiting review. Caveat reader.

This first section will explain how to download the Libwally C Library and get it working.

> :book: ***What is Libwally?*** Libwally is a library of primitives helpful for the creation of wallets that is cross-platform and cross-language, so that the same functions can be used everywhere. There are [online docs](https://wally.readthedocs.io/en/latest/). Libwally is made available as part of Blockstream's [Elements Project](https://github.com/ElementsProject).

## Install Libwally

As usual, you'll need some packages on your system:
```
$ sudo apt-get install git
$ sudo apt-get install dh-autoreconf
```
You can then download Libwally from its Git repo:
```
$ git clone https://github.com/ElementsProject/libwally-core
```
Afterward, you can begin the configuration process:
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
You can then verify that tests are working:
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

## Prepare for Libwally

So how do you use Libwally in a program? As usual, you'll need to include appropriate files and link appropriate libraries for your code.

### Include the Files

There are a considerable number of possible include files:

```
$ ls /usr/include/wally*
/usr/include/wally_address.h  /usr/include/wally_bip39.h   /usr/include/wally_elements.h  /usr/include/wally_script.h
/usr/include/wally_bip32.h    /usr/include/wally_core.h    /usr/include/wally.hpp	  /usr/include/wally_symmetric.h
/usr/include/wally_bip38.h    /usr/include/wally_crypto.h  /usr/include/wally_psbt.h	  /usr/include/wally_transaction.h
```
Fortunately, the file names largely match the sections in the [docs](https://wally.readthedocs.io/en/latest/), so you should be able to include the correct files based on what you're doing, after including the ubiquitous `wally_core.h`.

### Link the Libraries

You also will need to link appropriate libraries:
```
$ ls /usr/lib/libsecp* /usr/lib/libwally*
/usr/lib/libsecp256k1.a   /usr/lib/libwallycore.la  /usr/lib/libwallycore.so.0
/usr/lib/libsecp256k1.la  /usr/lib/libwallycore.so  /usr/lib/libwallycore.so.0.0.0
```
Mostly, you'll be using `libwallycore`.

## Set Up a Libwally Program

Compared to some of the previous libraries, Libwally is ridiculously easy to initialize:
```
lw_response = wally_init(0);
```
And then when you're done, there's a handy function to clean up any allocated memory:
```
wally_cleanup(0);
```
In both cases, the argument is for flags, but is currently set to `0`.

## Test a Test Libwally Program

The src directory contains [testwally.c](src/17_1_testwally.c), which just shows how the initialize and cleanup functions work.

You can compile it as follows:
```
$ cc testwally.c -lwallycore -o testwally
```
Afterward you can run it:
```
$ ./testwally
Startup: 0
```
The "Startup" value is the return from `wally_init`. The `0` value may initially appear discouraging, but it's's what you want to see:
```
include/wally_core.h:#define WALLY_OK      0 /** Success */
```

## Install Libsodium

You should also install Libsodium to get access to a high quality random number generator for testing purposes.

> :warning: **WARNING:** The generation of random numbers can be one of the greatest points of vulnerability in any Bitcoin software. If you do it wrong, you expose your users to attacks because they end up with insecure Bitcoin keys, and this isn't a [theoretical problem](https://github.com/BlockchainCommons/SmartCustodyBook/blob/master/manuscript/03-adversaries.md#adversary-systemic-key-compromise). BlockchainInfo once incorrectly generated 0.0002% of their keys, which resulted in the temporary loss of 250 Bitcoins. Bottom line: make sure you're totally comfortable with your random number generation. It might be Libsodium, or it might be an even more robust TRNG method.

You can download a [Libsodium tarball](https://download.libsodium.org/libsodium/releases/) and then follow the instructions at [Libsodium installation](https://doc.libsodium.org/installation) to install.

First, untar:
```
$ tar xzfv /tmp/libsodium-1.0.18-stable.tar.gz 
```
Then, adjust the `configure` file exactly as you have the other libraries to date:
```
< ac_default_prefix=/usr
---
> ac_default_prefix=/usr/local
```
Finally, `make`, `check`, and `install`:
```
$ make
$ make check
...
============================================================================
Testsuite summary for libsodium 1.0.18
============================================================================
# TOTAL: 77
# PASS:  77
# SKIP:  0
# XFAIL: 0
# FAIL:  0
# XPASS: 0
# ERROR: 0
============================================================================
...
$ sudo make install
```
This course will only use `libsodium` for one small (but crucial!) bit of entropy generation, but watch for it in the next section.

## Summary: Setting Up Libwally

By installing the Libwally (and Libsodium) includes and libraries, you gain access to a number of cryptographic and wallet functions, which can complement your RPC and ZMQ libraries (or your command-line `bitcoin-cli`).

So what precisely can you do now? That's what the rest of this chapter is about.

## What's Next?

Learn more about "Programming Bitcoin with Libwally" in [17.2: Using BIP39 in Libwally](17_2_Using_BIP39_in_Libwally.md).


