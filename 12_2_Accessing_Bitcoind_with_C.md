# 12.2: Accessing Bitcoind with C

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Though command-line `curl` is the easiest way to access the `bitcoind` directly, there are [many other options](https://en.bitcoin.it/wiki/API_reference_(JSON-RPC)) for doing so. The best package for doing so in C is currently [libbitcoinrpc](https://github.com/gitmarek/libbitcoinrpc/blob/master/README.md). It uses a `curl` library for accessing the data and it uses the somewhat clunky `jansson` library for decoding JSON. The advantage of a full programming language over command-line `curl` for writing more complex code should be obvious.

## Set Up libbitcoinrpc

First, you need to install a basic C setup and the dependent packages, `libcurl`, `libjansson`, and `libuuid`. The following will do so on a Ubuntu system:
```
$ sudo apt-get install make
$ sudo apt-get install gcc
$ sudo apt-get install libcurl4-openssl-dev libjansson-dev uuid-dev
```
You can then download [libbitcoinrpc from Github](https://github.com/gitmarek/libbitcoinrpc/blob/master/README.md). Clone it or grab a zip file, as you prefer.
```
$ sudo apt-get unzip
$ unzip libbitcoinrpc-master.zip 
$ cd libbitcoinrpc-master/
```

### Compile libbitcoinrpc

You'll probably need to adjust your `$PATH`, so that you can access `/sbin/ldconfig`:
```
$ PATH="/sbin:$PATH"
```
You'll also want to adjust the `INSTALL_LIBPATH` in the `Makefile` to install to `/usr/lib` instead of `/usr/local/lib`:
```
INSTALL_LIBPATH    := $(INSTALL_PREFIX)/usr/lib
```
(If you prefer not to sully your `/usr/lib`, the alternative is to change your `etc/ld.so.conf` or its dependent files appropriately ... but for a test setup on a test machine, this is probably fine.)

Then you can compile:
```
$ make
```
If that works, you can install the package:
```
$ sudo make install
```
