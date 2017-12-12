# 12.6: Accessing Bitcoind with Python

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Though command-line `curl` is the easiest way to access the `bitcoind` directly, there are many other options for doing so and most of them support more fully featured programming languages. One package for doing so in Python is [Bitcoin authproxy](https://github.com/bitcoin/bitcoin/blob/master/test/functional/test_framework/authproxy.py). It was originally [written by Jeff Garzik](https://github.com/jgarzik/python-bitcoinrpc) but is now maintained by Bitcoin Core as the primary interface for the QA testing suite. It uses an http client module to access the bitcoind RPC server.

## Set Up Bitcoin AuthProxy

The latest Bitcoin Core version of `authproxy` requires Python 3, but you can still use jgarzik's original script to run in Python 2. Download the script to your working directory, start python, and import the module.
```
$ python
>>> from authproxy import AuthServiceProxy, JSONRPCException
```
Instantiate an `AuthServiceProxy` object with the same localhost IP address, port, username and password you set in bitcoin.conf. For exmaple if these are the lines in bitcoin.conf:
```
rpcuser=bitcoinrpc
rpcpassword=1d446e2e45e96aaba6dea515871cc497
```
your python command would be:
```
>>> rpc_connection = AuthServiceProxy("http://%s:%s@127.0.0.1:8332"%("bitcoinrpc","1d446e2e45e96aaba6dea515871cc497"))

```

### Make an RPC Call

Now that we have our RPC connection set up via the proxy, RPC calls as easy to make as if you were just calling `bitcoin-cli` form the command line. AuthProxy returns python dict's that may contain decimals, so they are not super easy to convert to JSON. We'll make a few generic calls to bitcoind to get some network statistics:
```
>>> rpc_connection.getmempoolinfo()
{'usage': 42593984, 'bytes': 21357852, 'size': 17809, 'mempoolminfee': Decimal('0.00078966'), 'maxmempool': 50000000}
>>> rpc_connection.getbestblockhash()
'00000000000000000033a05c84da138f2a6779f34671082cfd4b9f586054c65c'
```
You can even use this interface to run wallet commands, and add paramaters to the RPC call. Here we ask bitcoind for a new address, then send that address back to bitcoind to get the associated private key. (In this example, the wallet is not encrypted!):
```
>>> rpc_connection.getnewaddress()
'1BFcCbjDPBoCwR76PCFEeTRZEVfKAwP11v'
>>> rpc_connection.dumpprivkey('1BFcCbjDPBoCwR76PCFEeTRZEVfKAwP11v')
'L4dnpfiZ69PKsKyym5o7HvoftvHjw1z2QkojQBwYRPUuaWgzTnNS'
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

Here's the complete code for the `getmininginfo` command, with organized variable initiatialization, error checking, and variable cleanup.
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

  rpc_client = bitcoinrpc_cl_init_params ("bitcoinrpc", "73bd45ba60ab8f9ff9846b6404769487", "127.0.0.1", 18332);

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
  "id": "03406237-cd8f-466d-ac31-86711ea9d1db",
  "result": {
    "blocks": 1147154,
    "errors": "Warning: unknown new rules activated (versionbit 28)",
    "pooledtx": 0,
    "currentblocksize": 0,
    "currentblockweight": 0,
    "currentblocktx": 0,
    "difficulty": 313525.08513550513,
    "networkhashps": 3958339463617.417,
    "chain": "test"
  },
  "error": null
}

Just the Result: {
  "blocks": 1147154,
  "errors": "Warning: unknown new rules activated (versionbit 28)",
  "pooledtx": 0,
  "currentblocksize": 0,
  "currentblockweight": 0,
  "currentblocktx": 0,
  "difficulty": 313525.08513550513,
  "networkhashps": 3958339463617.417,
  "chain": "test"
}

Block Count: 1147154
```
