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
