# 12.2: Accessing Bitcoind with C

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Though command-line `curl` is the easiest way to access the `bitcoind` directly, there are [many other options](https://en.bitcoin.it/wiki/API_reference_(JSON-RPC)) for doing so and most of them support more fully featured programming languages. The best package for doing so in C is currently [libbitcoinrpc](https://github.com/gitmarek/libbitcoinrpc/blob/master/README.md). It uses a `curl` library for accessing the data and it uses the somewhat clunky `jansson` library for decoding JSON. The advantage of a full programming language over command-line `curl` for writing more complex code should be obvious.

## Set Up libbitcoinrpc

To use `libbitcoinrpc`, you need to install a basic C setup and the dependent packages, `libcurl`, `libjansson`, and `libuuid`. The following will do so on a Ubuntu system:
```
$ sudo apt-get install make gcc libcurl4-openssl-dev libjansson-dev uuid-dev
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
For a Ubunto system, you'll also want to adjust the `INSTALL_LIBPATH` in the `Makefile` to install to `/usr/lib` instead of `/usr/local/lib`:
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

## Write Code in C

`libbitcoinrpc` has well-structured and simple methods for connecting to your `bitcoind`, executing RPC calls, and decoding the response.

### Setup Your Code

Make sure that your code files include the appropriate headers:
```
#include <jansson.h>
#include <bitcoinrpc.h>
```
You'll also need to link in the appropriate libraries whenever you compile:
```
$ cc mybitcoinclient.c -lbitcoinrpc -ljansson -o rpcclient
```

### Build Your Connection

There are just a few steps involved with building the connection to your `bitcoind` server. 

First, you initialize the library:
```
bitcoinrpc_global_init();
```
Then you connect to your `bitcoind`. The four arguments for `bitcoinrpc_cl_init_params` are username, password, IP address, and port. As usual, you should extract the user and password from `~/.bitcoin/bitcoin.conf`, while IP address 127.0.0.1 and port 18332 should be correct for the standard testnet setup described in this documents.
```
bitcoinrpc_cl_t *rpc_client;
rpc_client = bitcoinrpc_cl_init_params ("bitcoinrpc", "d8340efbcd34e312044c8431c59c792c", "127.0.0.1", 18332);
```

> **MAINNET VS TESTNET:** The port would be 8332 for a mainnet setup.

If `rpc_client` is successful, then you can go. 

When you're all done with everything, you should close your connection:
```
bitcoinrpc_global_cleanup();
```
#### Test Your Connection

Here's the complete code for a test of the connection to `bitcoind`.
```
file: testbitcoin.c

#include <jansson.h>
#include <bitcoinrpc.h>

int main(void) {
  
  bitcoinrpc_global_init();

  bitcoinrpc_cl_t *rpc_client;
  rpc_client = bitcoinrpc_cl_init_params ("bitcoinrpc", "d8340efbcd34e312044c843
1c59c792c", "127.0.0.1", 18332);

  if (rpc_client) {

    printf("Successfully connected to server!\n");
  
  } else {

    printf("Failed to connect to server!\n");

  }

  bitcoinrpc_global_cleanup();

}
```
Test it out to make sure everything is working:
```
$ cc testbitcoin.c -lbitcoinrpc -ljansson -o testbitcoin
$ ./testbitcoin 
Successfully connected to server!
```
### Make an RPC Call

In order to you an RPC method in `bitcoinrpc`, you must initialize a variable of type `bitcoinrpc_method_t`. You do so with the appropriate value for the method you want to use, all of which are listed in the [bitcoinrpc Reference](https://github.com/gitmarek/libbitcoinrpc/blob/master/doc/reference.md), which includes all of the `bitcoinrpc` constants, including RPC calls and error messages.
```
bitcoinrpc_method_t *getmininginfo  = NULL;
getmininginfo = bitcoinrpc_method_init(BITCOINRPC_METHOD_GETMININGINFO);
```
You must next input the parameters for your methods, which is done by creating a JSON array, then (if necessary) filling it. This is done with the `jansson` library; see the [jansson Reference](http://jansson.readthedocs.io/en/2.8/apiref.html) for more information on its specific function calls. 

In this case, we just input an empty array, because we're not sending any arguments to `getmininginfo`. We'll have some increasingly complex examples of parameter submission later on.
```
json_t *params = NULL;
params = json_array();
```
The parameters are input to the method as follows. After inputting the parameters into the method, they can be neatly cleared away.
```
bitcoinrpc_method_set_params(getmininginfo, params);
json_decref(params);
```
Two more objects are required, a "response object" and an "error object". They're created via standard `bitcoinrpc` function calls:
```
bitcoinrpc_resp_t *btcresponse  = NULL;
btcresponse = bitcoinrpc_resp_init();

bitcoinrpc_err_t btcerror;
```
And now you can put it all together to make a `getmininginfo` RPC call:
```
bitcoinrpc_call (rpc_client, getmininginfo, btcresponse, &btcerror);
```
[[AND, I JUST GET LITERALLY INDECIPHERABLE ERROR MESSAGES AT THIS POINT, AND THE SAMPLE CODE DOESN'T WORK EITHER. I'M NOT SURE IF THIS LIBRARY STILL WORKS.]]
