# 16.1: Accessing Bitcoind in C with RPC Libraries

> :information_source: **NOTE:** This section has been recently added to the course and is an early draft that may still be awaiting review. Caveat reader.

You've already seen one alternative way to access the Bitcoind's RPC ports: `curl`, which was covered in a [Chapter 4 Interlude](04_4__Interlude_Using_Curl.md). Interacting with `bitcoind` through an RPC library in C is no different than that, you just need some good libraries to help you out. This section introduces a package called `libbitcoinrpc`, which allows you to access JSON-RPC `bitcoind` port.  It uses a `curl` library for accessing the data and it uses the `jansson` library for encoding and decoding the JSON.

## Set Up libbitcoinrpc

To use `libbitcoinrpc`, you need to install a basic C setup and the dependent packages `libcurl`, `libjansson`, and `libuuid`. The following will do so on your Bitcoin Standup server (or any other Ubuntu server).
```
$ sudo apt-get install make gcc libcurl4-openssl-dev libjansson-dev uuid-dev
Suggested packages:
  libcurl4-doc libidn11-dev libkrb5-dev libldap2-dev librtmp-dev libssh2-1-dev
The following NEW packages will be installed:
  libcurl4-openssl-dev libjansson-dev uuid-dev
0 upgraded, 3 newly installed, 0 to remove and 4 not upgraded.
Need to get 358 kB of archives.
After this operation, 1.696 kB of additional disk space will be used.
Do you want to continue? [Y/n] y
```
You can then download [libbitcoinrpc from Github](https://github.com/gitmarek/libbitcoinrpc/blob/master/README.md). Clone it or grab a zip file, as you prefer.
```
$ sudo apt-get install git
$ git clone https://github.com/gitmarek/libbitcoinrpc
```

> :warning: **WARNING** A change in the "signrawtransaction" RPC caused signing with `libbitcoinrpc` to segfault for Bitcoin 0.17 or higher. A [PR has been submitted](https://github.com/gitmarek/libbitcoinrpc/pull/1/commits) to resolve the problem, but if it hasn't yet been merged, you can just make the one simple change in the source code to `src/bitcoinrpc_method.c` before compiling.

### Compiling libbitcoinrpc

Before you can compile and install the package, you'll probably need to adjust your `$PATH`, so that you can access `/sbin/ldconfig`:
```
$ PATH="/sbin:$PATH"
```
For an Ubuntu system, you'll also want to adjust the `INSTALL_LIBPATH` in the `libbitcoinrpc` `Makefile` to install to `/usr/lib` instead of `/usr/local/lib`:
```
$ emacs ~/libbitcoinrpc/Makefile 
...
INSTALL_LIBPATH    := $(INSTALL_PREFIX)/usr/lib
```
(If you prefer not to sully your `/usr/lib`, the alternative is to change your `etc/ld.so.conf` or its dependent files appropriately ... but for a test setup on a test machine, this is probably fine.)

Likewise, you'll also want to adjust the `INSTALL_HEADERPATH` in the `libbitcoinrpc` `Makefile` to install to `/usr/include` instead of `/usr/local/include`:
```
...
INSTALL_HEADERPATH    := $(INSTALL_PREFIX)/usr/include
```

Then you can compile:
```
$ cd libbitcoinrpc
~/libbitcoinrpc$ make

gcc -fPIC -O3 -g -Wall -Werror -Wextra -std=c99 -D VERSION=\"0.2\" -o src/bitcoinrpc_err.o -c src/bitcoinrpc_err.c
gcc -fPIC -O3 -g -Wall -Werror -Wextra -std=c99 -D VERSION=\"0.2\" -o src/bitcoinrpc_global.o -c src/bitcoinrpc_global.c
gcc -fPIC -O3 -g -Wall -Werror -Wextra -std=c99 -D VERSION=\"0.2\" -o src/bitcoinrpc.o -c src/bitcoinrpc.c
gcc -fPIC -O3 -g -Wall -Werror -Wextra -std=c99 -D VERSION=\"0.2\" -o src/bitcoinrpc_resp.o -c src/bitcoinrpc_resp.c
gcc -fPIC -O3 -g -Wall -Werror -Wextra -std=c99 -D VERSION=\"0.2\" -o src/bitcoinrpc_cl.o -c src/bitcoinrpc_cl.c
gcc -fPIC -O3 -g -Wall -Werror -Wextra -std=c99 -D VERSION=\"0.2\" -o src/bitcoinrpc_method.o -c src/bitcoinrpc_method.c
gcc -fPIC -O3 -g -Wall -Werror -Wextra -std=c99 -D VERSION=\"0.2\" -shared -Wl,-soname,libbitcoinrpc.so.0 \
src/bitcoinrpc_err.o src/bitcoinrpc_global.o src/bitcoinrpc.o src/bitcoinrpc_resp.o src/bitcoinrpc_cl.o src/bitcoinrpc_method.o \
-o .lib/libbitcoinrpc.so.0.2 \
-Wl,--copy-dt-needed-entries -luuid -ljansson -lcurl
ldconfig -v -n .lib
.lib:
	libbitcoinrpc.so.0 -> libbitcoinrpc.so.0.2 (changed)
ln -fs libbitcoinrpc.so.0 .lib/libbitcoinrpc.so
```
If that works, you can install the package:
```
$ sudo make install
Installing to 
install .lib/libbitcoinrpc.so.0.2 /usr/local/lib
ldconfig  -n /usr/local/lib
ln -fs libbitcoinrpc.so.0 /usr/local/lib/libbitcoinrpc.so
install -m 644 src/bitcoinrpc.h /usr/local/include
Installing docs to /usr/share/doc/bitcoinrpc
mkdir -p /usr/share/doc/bitcoinrpc
install -m 644 doc/*.md /usr/share/doc/bitcoinrpc
install -m 644 CREDITS /usr/share/doc/bitcoinrpc
install -m 644 LICENSE /usr/share/doc/bitcoinrpc
install -m 644 Changelog.md /usr/share/doc/bitcoinrpc
Installing man pages
install -m 644 doc/man3/bitcoinrpc*.gz /usr/local/man/man3
```

## Prepare Your Code

`libbitcoinrpc` has well-structured and simple methods for connecting to your `bitcoind`, executing RPC calls, and decoding the response.

To use `libbitcoinrpc`, make sure that your code files include the appropriate headers:
```
#include <jansson.h>
#include <bitcoinrpc.h>
```
You'll also need to link in the appropriate libraries whenever you compile:
```
$ cc yourcode.c -lbitcoinrpc -ljansson -o yourcode
```

## Build Your Connection

Building the connection to your `bitcoind` server takes a few simple steps.

First, initialize the library:
```
bitcoinrpc_global_init();
```
Then connect to your `bitcoind` with `bitcoinrpc_cl_init_params`. The four arguments for `bitcoinrpc_cl_init_params` are username, password, IP address, and port. You should already know all of this information from your work with [Curl](04_4__Interlude_Using_Curl.md). As you'll recall, the IP address 127.0.0.1 and port 18332 should be correct for the standard testnet setup described in these documents, while you can extract the user and password from `~/.bitcoin/bitcoin.conf`.
```
$ cat bitcoin.conf 
server=1
dbcache=1536
par=1
maxuploadtarget=137
maxconnections=16
rpcuser=StandUp
rpcpassword=6305f1b2dbb3bc5a16cd0f4aac7e1eba
rpcallowip=127.0.0.1
debug=tor
prune=550
testnet=1
[test]
rpcbind=127.0.0.1
rpcport=18332
[main]
rpcbind=127.0.0.1
rpcport=8332
[regtest]
rpcbind=127.0.0.1
rpcport=18443
```
Which you then place in the `bitcoinrpc_cl_init_params`:
```
bitcoinrpc_cl_t *rpc_client;
rpc_client = bitcoinrpc_cl_init_params("StandUp", "6305f1b2dbb3bc5a16cd0f4aac7e1eba", "127.0.0.1", 18332);
```

> **MAINNET VS TESTNET:** The port would be 8332 for a mainnet setup.

If `rpc_client` is successfully initialized, you'll be able to send off RPC commands.

Later, when you're all done with your `bitcoind` connection, you should close it:
```
bitcoinrpc_global_cleanup();
```

### Test the Test Code

Test code can be found at [16_1_testbitcoin.c in the src directory](src/16_1_testbitcoin.c). Download it to your testnet machine, then insert the correct RPC password (and change the RPC user if you didn't create your server with StandUp).

You can compile and run this as follows:
```
$ cc testbitcoin.c -lbitcoinrpc -ljansson -o testbitcoin
$ ./testbitcoin 
Successfully connected to server!
```

> :warning: **WARNING:** If you forget to enter your RPC password in this or any other code samples that depend on RPC, you will receive a mysterious `ERROR CODE 5`.

## Make an RPC Call

In order to use an RPC method using `libbitcoinrpc`, you must initialize a variable of type `bitcoinrpc_method_t`. You do so with the appropriate value for the method you want to use, all of which are listed in the [bitcoinrpc Reference](https://github.com/gitmarek/libbitcoinrpc/blob/master/doc/reference.md).
```
bitcoinrpc_method_t *getmininginfo  = NULL;
getmininginfo = bitcoinrpc_method_init(BITCOINRPC_METHOD_GETMININGINFO);
```
Usually you would set parameters next, but `getmininginfo` requires no parameters, so you can skip that for now.

You must also create two other objects, a "response object" and an "error object". They can be initialized as follows:
```
bitcoinrpc_resp_t *btcresponse  = NULL;
btcresponse = bitcoinrpc_resp_init();

bitcoinrpc_err_t btcerror;
```
You use the `rpc_client` variable that you already learned about in the previous test, and add on your `getmininginfo` method and the two other objects:
```
bitcoinrpc_call(rpc_client, getmininginfo, btcresponse, &btcerror);
```
### Output Your Response

You'll want to know what the RPC call returned. To do so, retrieve the output of your call as a JSON object with `bitcoinrpc_resp_get` and save it into a standard `jansson` object, of type `json_t`:
```
json_t *jsonresponse = NULL;
jsonresponse = bitcoinrpc_resp_get(btcresponse);
```
If you want to output the complete JSON results of the RPC call, you can do so with a simple invocation of `json_dumps`, also from the `jansson` library:
```
printf ("%s\n", json_dumps(j, JSON_INDENT(2)));
```
However, since you're now writing complete programs, you probably want to do more subtle work, such as pulling out individual JSON values for specific usage. The [jansson Reference](https://jansson.readthedocs.io/en/2.10/apiref.html) details how to do so.

Just as when you were using [Curl](04_4__Interlude_Using_Curl.md), you'll find that RPC returns a JSON object containing an `id`, an `error`, and most importantly a JSON object of the `result`. 

The `json_object_get` function will let you retrieve a value (such as the `result`) from a JSON object by key:
```
json_t *jsonresult = NULL;
jsonresult = json_object_get(jsonresponse,"result");
printf ("%s\n", json_dumps (jsonresult, JSON_INDENT(2)));
```
However, you probably want to drill down further, to get a specific variable. Once you've retrieved the appropriate value, you will need to convert it to a standard C object by using the appropriate `json_*_value` function. For example, accessing an integer uses `json_integer_value`:
```
json_t *jsonblocks = NULL;
jsonblocks = json_object_get(jsonresult,"blocks");

int blocks;
blocks = json_integer_value(jsonblocks);
printf("Block Count: %d\n",blocks);
```

> :warning: **WARNING:** It's extremely easy to segfault your C code when working with `jansson` objects if you get confused with what type of object you're retrieving. Make careful use of `bitcoin-cli help` to know what you should expect, and if you experience a segmentation fault, first look at your JSON retrieval functions.

### Test the Info Code

Retrieve the test code from [the src directory](src/16_1_getmininginfo.c).
```
$ cc getmininginfo.c -lbitcoinrpc -ljansson -o getmininginfo
$ ./getmininginfo 
Full Response: {
  "result": {
    "blocks": 1804406,
    "difficulty": 4194304,
    "networkhashps": 54842097951591.781,
    "pooledtx": 127,
    "chain": "test",
    "warnings": "Warning: unknown new rules activated (versionbit 28)"
  },
  "error": null,
  "id": "474ccddd-ef8c-4e3f-93f7-fde72fc08154"
}

Just the Result: {
  "blocks": 1804406,
  "difficulty": 4194304,
  "networkhashps": 54842097951591.781,
  "pooledtx": 127,
  "chain": "test",
  "warnings": "Warning: unknown new rules activated (versionbit 28)"
}

Block Count: 1804406
```
## Make an RPC Call with Arguments

But what if your RPC call _did_ have arguments? 

### Create a JSON Array

To send parameters to your RPC call using `libbitcoinrpc` you have to wrap them in a JSON array. Since an array is just a simple listing of values, all you have to do is encode the parameters as ordered elements in the array. 

Create the JSON array using the `json_array` function from `jansson`:
```
json_t *params = NULL;
params = json_array();
```
You'll then reverse the procedure that you followed to access JSON values: you'll convert C-typed objects to JSON-typed objects using the `json_*` functions. Afterward, you'll append them to the array:
```
json_array_append_new(params,json_string(tx_rawhex));
```
Note that there are two variants to the append command: `json_array_append_new`, which appends a newly created variable, and `json_array_append`, which appends an existing variable.

This simple `json_array_append_new` methodology will serve for the majority of RPC commands with parameters, but some RPC commands require more complex inputs. In these cases you may need to create subsidiary JSON objects or JSON arrays, which you will then append to the parameters array as usual. The next section contains an example of doing so using `createrawtransaction`, which contains a JSON array of JSON objects for the inputs, a JSON object for the outputs, and the `locktime` parameter. 

### Assign the Parameters

When you've created your parameters JSON array, you simply assign it after you've initialized your RPC method, as follows:
```
bitcoinrpc_method_set_params(rpc_method, params)
```
This section doesn't include a full example of this more complex methodology, but we'll see it in action multiple times in our first comprehensive RPC-based C program, in the next section.

## Summary: Accessing Bitcoind with C

By linking to the `bitcoinrpc` RPC and `jansson` JSON libraries, you can easily access `bitcoind` via RPC calls from a C library. To do so, you create an RPC connection, then make individual RPC calls, some of them with parameters. `jansson` then allows you to decode the JSON responses. The next section will demonstrate how this can be used for a pragmatic, real-world program.

* :fire: ***What is the power of C?*** C allows you to take the next step beyond shell-scripting, permitting the creation of more comprehensive and robust programs.

## What's Next?

Learn more about "Talking to Bitcoind with C" in [16.2: Programming Bitcoind in C with RPC Libraries](16_2_Programming_Bitcoind_with_C.md).
