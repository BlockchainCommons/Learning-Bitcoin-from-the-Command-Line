# 12.2: Accessing Bitcoind with C

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Though command-line `curl` is the easiest way to access the `bitcoind` directly, there are [many other options](https://en.bitcoin.it/wiki/API_reference_(JSON-RPC)) for doing so and most of them support more fully featured programming languages. The best package for doing so in C is currently [libbitcoinrpc](https://github.com/gitmarek/libbitcoinrpc/blob/master/README.md). It uses a `curl` library for accessing the data and it uses the somewhat clunky `jansson` library for decoding the JSON. 

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

Before you can compile and install the package, you'll probably need to adjust your `$PATH`, so that you can access `/sbin/ldconfig`:
```
$ PATH="/sbin:$PATH"
```
For a Ubunto system, you'll also want to adjust the `INSTALL_LIBPATH` in the `libbitcoinrpc` `Makefile` to install to `/usr/lib` instead of `/usr/local/lib`:
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

To use `libbitcoinrpc`, make sure that your code files include the appropriate headers:
```
#include <jansson.h>
#include <bitcoinrpc.h>
```
You'll also need to link in the appropriate libraries whenever you compile:
```
$ cc mybitcoinclient.c -lbitcoinrpc -ljansson -o rpcclient
```

### Build Your Connection

Building the connection to your `bitcoind` server takes a few simple steps.

First, initialize the library:
```
bitcoinrpc_global_init();
```
Then connect to your `bitcoind`. The four arguments for `bitcoinrpc_cl_init_params` are username, password, IP address, and port. As usual, you should extract the user and password from `~/.bitcoin/bitcoin.conf`, while IP address 127.0.0.1 and port 18332 should be correct for the standard testnet setup described in this documents.
```
bitcoinrpc_cl_t *rpc_client;
rpc_client = bitcoinrpc_cl_init_params ("bitcoinrpc", "d8340efbcd34e312044c8431c59c792c", "127.0.0.1", 18332);
```

> **MAINNET VS TESTNET:** The port would be 8332 for a mainnet setup.

If `rpc_client` is successful, then you can go. 

Later, when you're all done with your `bitcoind` connection, you should close it:
```
bitcoinrpc_global_cleanup();
```

Appendix I shows the complete code for a test of a `bitcoind` connection.

### Make an RPC Call

In order to use an RPC method in `bitcoinrpc`, you must initialize a variable of type `bitcoinrpc_method_t`. You do so with the appropriate value for the method you want to use, all of which are listed in the [bitcoinrpc Reference](https://github.com/gitmarek/libbitcoinrpc/blob/master/doc/reference.md).
```
bitcoinrpc_method_t *getmininginfo  = NULL;
getmininginfo = bitcoinrpc_method_init(BITCOINRPC_METHOD_GETMININGINFO);
```
Usually you would set parameters here, but `getmininginfo` requires no parameters, so you can skip that for now.

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
### Output Your Response

Retrieve the output of your call as a JSON object with `bitcoinrpc_resp_get`.
```
json_t *jsonresponse = NULL;
jsonresponse = bitcoinrpc_resp_get (btcresponse);
```
If you want to output the complete JSON results of the RPC call, you can do so with a simple invocation of `json_dumps`, from the `jansson` library:
```
printf ("%s\n", json_dumps (j, JSON_INDENT(2)));
```
However since your now writing complete programs, you're probably going to want to do more subtle work, such as pulling out individual JSON values for specific usage. The [jansson Reference](https://jansson.readthedocs.io/en/2.10/apiref.html) details how to do so.

You can drill down to the `result` JSON object:
```
json_t *jsonresult = NULL;
jsonresult = json_object_get(jsonresponse,"result");
printf ("%s\n", json_dumps (jsonresult, JSON_INDENT(2)));
```
Alternatively, you can drill down to an individual item like `blocks`:
```
json_t *jsonblocks = NULL;
jsonresult = json_object_get(jsonresult,"blocks");

int blocks;
blocks = json_integer_value(jsonresult);
printf("Block Count: %d\n",blocks);
```
Appendix II has an example of this complete code, with the variable initiatialization all rearranged to the top, and with cleanup of the JSON objects.

[[SUMMARY]]

## Appendix I: Testing a Bitcoind Connection

Here's the complete code for a test of the connection to `bitcoind`.
```
file: testbitcoin.c

#include <jansson.h>
#include <bitcoinrpc.h>

int main(void) {
  
  bitcoinrpc_global_init();

  bitcoinrpc_cl_t *rpc_client;
  rpc_client = bitcoinrpc_cl_init_params ("bitcoinrpc", "d8340efbcd34e312044c8431c59c792c", "127.0.0.1", 18332);

  if (rpc_client) {

    printf("Successfully connected to server!\n");
  
  } else {

    printf("Failed to connect to server!\n");

  }

  bitcoinrpc_global_cleanup();

}
```
You can compile and run this as follows:
```
$ cc testbitcoin.c -lbitcoinrpc -ljansson -o testbitcoin
$ ./testbitcoin 
Successfully connected to server!
```
## Appendix II: Getting Mining Info

[[APPENDIX 2: First Code]]
   [[clean up organizing of initialization of variables]]
   [[FREE up the JSON objects]]

[[BREAK OUT TWO MORE CHAPTERS ON GETTING INFO + PUTTING TRANSACTION TOGETHER; OR 1?]]
