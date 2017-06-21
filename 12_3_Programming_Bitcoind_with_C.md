# 12.3: Programming Bitcoind with C

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

[§12.2](12_2_Accessing_Bitcoind_with_C.md) laid out the methodology for creating C programs based on RPC calls to `bitcoind`. We're now going to show the potential of that C programming by laying out a simplistic, first cut of an actual Bitcoin program.

## Plan for Your Code

We're going to program a simplistic first cut version of `sendtoaddress`, which will allow a user to send money to an address as long as he has a big enough UTXO. Here's what we need to do:

  1. Request an address and an amount
  2. Set an arbitrary fee
  3. Find a UTXO that's large enough for the amount + the fee
  4. Create a change address
  5. Create a raw transaction that sends from the UTXO to the address and the change address
  6. Sign the transaction
  7. Send the transaction
  
### Plan for Your Future

Since this is our first functional C program, we're going to try and keep it simple (KISS). If we were producing an actual production program, we'd at least want to do the following:

   1. Test and/or sanitize the inputs
   2. Calculate a fee automatically
   3. Combine multiple UTXOs if necessary
   
If you want to continue to expand this example, these would be an excellent place to start, especially the latter points, which will approve your understanding and usage of actual RPC commands.

## Write Your Transaction Software

We're now going to take that plan step by step

### 1. Request an Address and an Amount

Inputting the information is easy enough via command line arguments:
```
if (argc != 3) {

  printf("ERROR: Only %i arguments! Correct usage is '%s [recipient] [amount]'\n",argc-1,argv[0]);
  exit(-1);

}

char *tx_recipient = argv[1];
float tx_amount = atof(argv[2]);

printf("Sending %4.8f BTC to %s\n",tx_amount,tx_recipient);
```

> **WARNING:** A real program would need much better sanitization of these variables. 

### 2. Set an Arbitrary Fee

We're just setting the 0.0005 BTC fee that we've reguarly used to ensure that our test transactions go through quickly:
```
float tx_fee = 0.0005;
float tx_total = tx_amount + tx_fee;
```

> **WARNING:** A real program would calculate a fee that minimized cost while ensuring the speed was sufficient for the sender. 

### X. Prepare Your RPC 

Obviously, you're going to need to get all of your variables ready again, as discussed in [§12.2: Accessing Bitcoind with C](12_2_Accessing_Bitcoind_with_C.md). You also need to initialize your library, connect your RPC client, and prepare your response object:
```
bitcoinrpc_global_init();
rpc_client = bitcoinrpc_cl_init_params ("bitcoinrpc", "73bd45ba60ab8f9ff9846b6404769487", "127.0.0.1", 18332);
btcresponse = bitcoinrpc_resp_init();
```

### 3. Find a UTXO

To find a UTXO you must call the `listunspent` RPC:
```
rpc_method = bitcoinrpc_method_init(BITCOINRPC_METHOD_LISTUNSPENT);
bitcoinrpc_call(rpc_client, rpc_method, btcresponse, &btcerror);
```
However, the real work comes in decoding the response. We said that the `jansson` library was "somewhat clunky" and this is why: you have to create (and clear) a very large set of `json_t` objects in order to dig down to what you want.

First, you must retrieve the `result` field from JSON:
```
json_t *lu_response = NULL;
json_t *lu_result = NULL;

lu_response = bitcoinrpc_resp_get (btcresponse);
lu_result = json_object_get(lu_response,"result");
```
Then, you go into a loop, examining each unspent transaction, which appears as an element in your JSON result array:
```
int i;

const char *tx_id = 0;
int tx_vout = 0;
double tx_amount = 0.0;

for (i = 0 ; i < json_array_size(lu_result) ; i++) {

  json_t *lu_data = NULL;
  lu_data = json_array_get(lu_result, i);
  
  json_t *lu_amount = NULL;
  lu_amount = json_object_get(lu_data,"amount");
  tx_amount = json_real_value(lu_amount);
```
Is the UTXO large enough to pay out your transaction? If so, grab it!
```
  if (tx_amount > tx_total) {

    json_t *lu_txid = NULL;
    lu_txid = json_object_get(lu_data,"txid");
    tx_id = strdup(json_string_value(lu_txid));

    json_t *lu_vout = NULL;
	  lu_vout = json_object_get(lu_data,"vout");
    tx_vout = json_integer_value(lu_vout);

    json_decref(lu_amount);
	  json_decref(lu_txid);
    json_decref(lu_vout);
	  json_decref(lu_data);
    break;

  } 
```
Make sure to clean things up as you go through failed loops:
```
  json_decref(lu_amount);
  json_decref(lu_data);

}

json_decref(lu_result);
json_decref(lu_response);
```      
If you didn't find any large-enough UTXOs, you'll have to report that sad fact to the user ... and perhaps suggest that they should use a better sending program that will correctly merge UTXOs.
```
if (!tx_id) {

  printf("Very Sad: You don't have any UTXOs larger than %f",tx_total);
  exit(-1);
}

printf("We are using txid %s with vout %i\n",tx_id,tx_vout);
```

> **WARNING:** A real program would use subroutines for this sort of lookup, so that you could confidentally call various RPCs from a library of C functions. We're just going to blob it all into `main` as part of our KISS philosophy of simple examples.

### 4. Create a Change Address

Repeat this methodology to get a change address:
```
rpc_method = bitcoinrpc_method_init(BITCOINRPC_METHOD_GETRAWCHANGEADDRESS);

if (!rpc_method) {

  printf("ERROR: Unable to initialize listunspent method!\n");
  exit(-1);

}

bitcoinrpc_call(rpc_client, rpc_method, btcresponse, &btcerror);

if (btcerror.code != BITCOINRPCE_OK) {

printf("Error: listunspent error code %d [%s]\n", btcerror.code,btcerror.msg);

  exit(-1);

}

lu_response = bitcoinrpc_resp_get (btcresponse);
lu_result = json_object_get(lu_response,"result");
char *changeaddress = strdup(json_string_value(lu_result));
```
The only difference is what in particular we extract from our JSON object.

> **WARNING:** Here's another place that a subroutine would be really nice: to abstract out the whole RPC method initialization and call.

### 5. Create a Raw Transaction
