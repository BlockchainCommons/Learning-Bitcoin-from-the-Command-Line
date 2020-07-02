# 15.1: Accessing Bitcoind with C

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Interacting with the bitcoind directly and using command-line curl can get simple if you understand how it works, thus in this section we'll show how to use a good package for doing so in C called libbitcoinrpc that provides the functionality to access JSON-RPC bitcoind API.  It uses a curl library for accessing the data and it uses the jansson library for encoding and decoding the JSON.

## Set Up libbitcoinrpc

To use `libbitcoinrpc`, you need to install a basic C setup and the dependent packages `libcurl`, `libjansson`, and `libuuid`. The following will do so on a Ubuntu system:
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
$ sudo apt-get unzip
$ unzip libbitcoinrpc-master.zip 
unzip libbitcoinrpc-master.zip 
Archive:  libbitcoinrpc-master.zip
a2285e8f221185cd0afdcfaf1bc8c78988fce09a
   creating: libbitcoinrpc-master/
  inflating: libbitcoinrpc-master/.gitignore  
  inflating: libbitcoinrpc-master/.travis.yml  
  inflating: libbitcoinrpc-master/CREDITS  
  inflating: libbitcoinrpc-master/Changelog.md  
  inflating: libbitcoinrpc-master/LICENSE  
  inflating: libbitcoinrpc-master/Makefile  
  inflating: libbitcoinrpc-master/README.md  
$ cd libbitcoinrpc-master/
```

### Compile libbitcoinrpc

Before you can compile and install the package, you'll probably need to adjust your `$PATH`, so that you can access `/sbin/ldconfig`:
```
$ PATH="/sbin:$PATH"
```
For an Ubuntu system, you'll also want to adjust the `INSTALL_LIBPATH` in the `libbitcoinrpc` `Makefile` to install to `/usr/lib` instead of `/usr/local/lib`:
```
INSTALL_LIBPATH    := $(INSTALL_PREFIX)/usr/lib
```
(If you prefer not to sully your `/usr/lib`, the alternative is to change your `etc/ld.so.conf` or its dependent files appropriately ... but for a test setup on a test machine, this is probably fine.)

Likewise, you'll also want to adjust the `INSTALL_HEADERPATH` in the `libbitcoinrpc` `Makefile` to install to `/usr/include` instead of `/usr/local/include`:
```
INSTALL_HEADERPATH    := $(INSTALL_PREFIX)/usr/include
```

Then you can compile:
```
$ ~/libbitcoinrpc-master$ make

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
~/libbitcoinrpc-master$ sudo make install
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
~/libbitcoinrpc-master$

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
$ cc mybitcoinclient.c -lbitcoinrpc -ljansson -o mybitcoinclient
```

### Build Your Connection

Building the connection to your `bitcoind` server takes a few simple steps.

First, initialize the library:
```
bitcoinrpc_global_init();
```
Then connect to your `bitcoind` with `bitcoinrpc_cl_init_params`. The four arguments for `bitcoinrpc_cl_init_params` are username, password, IP address, and port. You should already know all of this information from your work with `curl`. As you'll recall, the IP address 127.0.0.1 and port 18332 should be correct for the standard testnet setup described in this documents, while you can extract the user and password from `~/.bitcoin/bitcoin.conf`.
```
bitcoinrpc_cl_t *rpc_client;
rpc_client = bitcoinrpc_cl_init_params("bitcoinrpc", "d8340efbcd34e312044c8431c59c792c", "127.0.0.1", 18332);
```

> **MAINNET VS TESTNET:** The port would be 8332 for a mainnet setup.

If `rpc_client` is successfully initialized, you'll be able to send off RPC commands.

Later, when you're all done with your `bitcoind` connection, you should close it:
```
bitcoinrpc_global_cleanup();
```

Appendix I shows the complete code for a test of a `bitcoind` connection.

### Make an RPC Call

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
With your four standard variables in hand, you can make a `getmininginfo` RPC call using `bitcoinrpc_call`:
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
However, since you're now writing complete programs, you probablywant to do more subtle work, such as pulling out individual JSON values for specific usage. The [jansson Reference](https://jansson.readthedocs.io/en/2.10/apiref.html) details how to do so.

Just as when you were using `curl`, you'll find that RPC returns a JSON object containing an `id`, an `error`, and most importantly a JSON object of the `result`. 

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

> **WARNING:** It's extremely easy to segfault your C code when working with `jansson` objects if you get confused with what type of object you're retrieving. Make careful use of `bitcoin-cli help` to know what you should expect, and if you experience a segmentation fault, first look at your JSON retrieval functions.

Appendix II has an example of this complete code for accessing mining information.

### Make an RPC Call with Arguments

But what if your RPC call _did_ have arguments? 

#### Create a JSON Array

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

This simple methodology will serve for the majority of RPC commands with parameters, but some RPC commands require more complex inputs. In these cases you may need to create subsidiary JSON objects or JSON arrays, which you will then append to the parameters array as usual. The next section contains an example of doing so using `createrawtransaction`, which contains a JSON array of JSON objects for the inputs, a JSON object for the outputs, and the `locktime` parameter. 

#### Assign the Parameters

When you've created your parameters JSON array, you simply assign it after you've initialized your RPC method, as follows:
```
bitcoinrpc_method_set_params(rpc_method, params)
```
This section doesn't include a full example of this more complex methodology, but we'll see it in action multiple times in our first comprehensive RPC-based C program, in the next section.

## Summary: Accessing Bitcoind with C

By linking to the `bitcoinrpc` and `jansson` libraries, you can easily access `bitcoind` via RPC calls from a C library. To do so, you create an RPC connection, then make individual RPC calls, some of them with parameters. `jansson` then allows you to decode the JSON responses.

_What is the power of C?_ C allows you to take the next step beyond shell-scripting, permitting the creation of more comprehensive and robust programs. A few examples appear in the next two sections.

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

Here's the complete code for the `getmininginfo` command, with organized variable initiatialization, error checking, and variable cleanup.   For this example we use a testnet network and show it's output.
```
file: getmininginfo.c

#include <jansson.h>
#include <bitcoinrpc.h>

int main(void) {

  bitcoinrpc_cl_t *rpc_client;
  bitcoinrpc_method_t *getmininginfo  = NULL;
  bitcoinrpc_resp_t *btcresponse  = NULL;
  bitcoinrpc_err_t btcerror;

  json_t *jsonresponse = NULL;
  json_t *jsonresult = NULL;
  json_t *jsonblocks = NULL;
  int blocks;

  bitcoinrpc_global_init();

  rpc_client = bitcoinrpc_cl_init_params ("bitcoinrpc", "73bd45ba60ab8f9ff9846b6404769487", "127.0.0.1", 18443);

  if (rpc_client) {
    getmininginfo = bitcoinrpc_method_init(BITCOINRPC_METHOD_GETMININGINFO);

    if (!getmininginfo) {

      printf("ERROR: Unable to initialize getmininginfo method!\n");
      exit(-1);

    }

    btcresponse = bitcoinrpc_resp_init();
    if (!btcresponse) {

      printf("Error: Cannot initialize response object!\n");
      exit(-1);

    }

    bitcoinrpc_call(rpc_client, getmininginfo, btcresponse, &btcerror);
    
    if (btcerror.code != BITCOINRPCE_OK) {


      printf("Error: getmininginfo error code %d [%s]\n", btcerror.code,btcerror.msg);
      exit(-1);

    }

    printf("Full Response: ");
    jsonresponse = bitcoinrpc_resp_get (btcresponse);
    printf ("%s\n", json_dumps (jsonresponse, JSON_INDENT(2)));

    printf("\nJust the Result: ");
    jsonresult = json_object_get(jsonresponse,"result");
    printf ("%s\n", json_dumps (jsonresult, JSON_INDENT(2)));

    jsonblocks = json_object_get(jsonresult,"blocks");
    blocks = json_integer_value(jsonblocks);
    printf("\nBlock Count: %d\n",blocks);

    json_decref(jsonblocks);
    json_decref(jsonresult);
    json_decref(jsonresponse);

  } else {

    printf("ERROR: Failed to connect to server!\n");

  }

  bitcoinrpc_cl_free(rpc_client);
  bitcoinrpc_global_cleanup();

}
```
As usual, you can compile and run as follows:
```
$ cc getmininginfo.c -lbitcoinrpc -ljansson -o getmininginfo
$ ./getmininginfo 
Full Response: {
  "result": {
    "blocks": 1773353,
    "difficulty": 10178811.406987719,
    "networkhashps": 129510207940932.2,
    "pooledtx": 9,
    "chain": "test",
    "warnings": "Warning: unknown new rules activated (versionbit 28)"
  },
  "error": null,
  "id": "6e502927-b065-486a-8182-bc1acd843bae"
}

Just the Result: {
  "blocks": 1773353,
  "difficulty": 10178811.406987719,
  "networkhashps": 129510207940932.2,
  "pooledtx": 9,
  "chain": "test",
  "warnings": "Warning: unknown new rules activated (versionbit 28)"
}

Block Count: 1773353
```
