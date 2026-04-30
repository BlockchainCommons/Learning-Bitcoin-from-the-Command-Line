# 16.2: Programming Bitcoind in C with RPC Libraries

> :information_source: **NOTE:** This section has been recently added to the course and is an early draft that may still be awaiting review. Caveat reader.

[§16.1](16_1_Accessing_Bitcoind_with_C.md) laid out the methodology for creating C programs using RPC and JSON libraries. We're now going to show the potential of those C libraries by laying out a simplistic, first cut of an actual Bitcoin program.

## Plan for Your Code

This section will create a simplistic first cut version of `sendtoaddress`, which will allow a user to send money to an address as long as he has a big enough UTXO. Here's what we need to do:

  1. Request an address and an amount
  2. Set an arbitrary fee
  3. Prepare Your RPC
  4. Find a UTXO that's large enough for the amount + the fee
  5. Create a change address
  6. Create a raw transaction that sends from the UTXO to the address and the change address
  7. Sign the transaction
  8. Send the transaction
  
### Plan for Your Future

Since this is your first functional C program, we're going to try and keep it simple (KISS). If we were producing an actual production program, we'd at least want to do the following:

   1. Test and/or sanitize the inputs
   2. Calculate a fee automatically
   3. Think logically about which valid UTXO to use
   4. Combine multiple UTXOs if necessary
   5. Watch for more errors in the `libbitcoinrpc` or `jansson` commands
   6. Watch for errors in the RPC responses
   
If you want to continue to expand this example, addressing the inadequacies of this example program would be a great place to start.

## Write Your Transaction Software

Your now ready to undertake that plan step by step

### Step 1: Request an Address and an Amount

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

> :warning: **WARNING:** A real program would need much better sanitization of these variables. 

### Step 2: Set an Arbitrary Fee

This example just an arbitrary 0.0005 BTC fee to ensure that the test transactions goes through quickly:
```
float tx_fee = 0.0005;
float tx_total = tx_amount + tx_fee;
```

> :warning: **WARNING:** A real program would calculate a fee that minimized cost while ensuring the speed was sufficient for the sender. 

### Step 3: Prepare Your RPC 

Obviously, you're going to need to get all of your variables ready again, as discussed in [§16.1: Accessing Bitcoind with C](16_1_Accessing_Bitcoind_with_C.md). You also need to initialize your library, connect your RPC client, and prepare your response object:
```
bitcoinrpc_global_init();
rpc_client = bitcoinrpc_cl_init_params ("bitcoinrpc", "YOUR-RPC-PASSWD", "127.0.0.1", 18332);
btcresponse = bitcoinrpc_resp_init();
```

### Step 4: Find a UTXO

To find a UTXO you must call the `listunspent` RPC:
```
rpc_method = bitcoinrpc_method_init(BITCOINRPC_METHOD_LISTUNSPENT);
bitcoinrpc_call(rpc_client, rpc_method, btcresponse, &btcerror);
```
However, the real work comes in decoding the response. The previous section noted that the `jansson` library was "somewhat clunky" and this is why: you have to create (and clear) a very large set of `json_t` objects in order to dig down to what you want.

First, you must retrieve the `result` field from JSON:
```
json_t *lu_response = NULL;
json_t *lu_result = NULL;

lu_response = bitcoinrpc_resp_get (btcresponse);
lu_result = json_object_get(lu_response,"result");
```

> :warning: **WARNING:** You only get a result if there wasn't an error. Here's another place for better error checking for production code.

Then, you go into a loop, examining each unspent transaction, which appears as an element in your JSON result array:
```
int i;

const char *tx_id = 0;
int tx_vout = 0;
double tx_value = 0.0;

for (i = 0 ; i < json_array_size(lu_result) ; i++) {

  json_t *lu_data = NULL;
  lu_data = json_array_get(lu_result, i);
  
  json_t *lu_value = NULL;
  lu_value = json_object_get(lu_data,"amount");
  tx_value = json_real_value(lu_value);
```
Is the UTXO large enough to pay out your transaction? If so, grab it!

> :warning: **WARNING:** A real-world program would think more carefully about which UTXO to grab, based on size and other factors. It probably wouldn't just grab the first thing it saw that worked.

```
  if (tx_value > tx_total) {

    json_t *lu_txid = NULL;
    lu_txid = json_object_get(lu_data,"txid");
    tx_id = strdup(json_string_value(lu_txid));

    json_t *lu_vout = NULL;
    lu_vout = json_object_get(lu_data,"vout");
    tx_vout = json_integer_value(lu_vout);

    json_decref(lu_value);
    json_decref(lu_txid);
    json_decref(lu_vout);
    json_decref(lu_data);
    break;

  } 
```
You should clear your main JSON elements as well:
```
}

json_decref(lu_result);
json_decref(lu_response);
```      

> :warning: **WARNING:** A real-world program would also make sure the UTXOs were `spendable`.

If you didn't find any large-enough UTXOs, you'll have to report that sad fact to the user ... and perhaps suggest that they should use a better program that will correctly merge UTXOs.
```
if (!tx_id) {

  printf("Very Sad: You don't have any UTXOs larger than %f\n",tx_total);
  exit(-1);
}
```

> **WARNING:** A real program would use subroutines for this sort of lookup, so that you could confidentally call various RPCs from a library of C functions. We're just going to blob it all into `main` as part of our KISS philosophy of simple examples.

### Step 5: Create a Change Address

Repeat the standard RPC-lookup methodology to get a change address:
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
The only difference is in what particular information is extracted from the JSON object.

> :warning: **WARNING:** Here's a place that a subroutine would be really nice: to abstract out the whole RPC method initialization and call.

### Step 6: Create a Raw Transaction

Creating the actual raw transaction is the other tricky part of programming your `sendtoaddress` replacement. That's because it requires the creation of a complex JSON object as a paramter.

To correctly create these parameters, you'll need to review what the `createrawtransaction` RPC expects. Fortunately, this is easy to determine using the `bitcoin-cli help` functionality:
```
$ bitcoin-cli help createrawtransaction
createrawtransaction [{"txid":"id","vout":n},...] {"address":amount,"data":"hex",...} ( locktime )
```
To review, your inputs will be a JSON array containing one JSON object for each UTXO. Then the outputs will all be in one JSON object. It's easiest to create these JSON elements from the inside out, using `jansson` commands. 

#### Step 6.1: Create the Input Parameters

To create the input object for your UTXO, use `json_object`, then fill it with key-values using either `json_object_set_new` (for newly created references) or `json_object_set` (for existing references):
```
json_t *inputtxid = NULL;
inputtxid = json_object();

json_object_set_new(inputtxid,"txid",json_string(tx_id));
json_object_set_new(inputtxid,"vout",json_integer(tx_vout));
```
You'll note that you again have to translate each C variable type into a JSON variable type using the appropriate function, such as `json_string` or `json_integer`.

To create the overall input array for all your UTXOs, use `json_array`, then fill it up with objects using `json_array_append`:
```
json_t *inputparams = NULL;
inputparams = json_array();
json_array_append(inputparams,inputtxid);
```

#### Step 6.2: Create the Output Parameters

To create the output array for your transaction, follow the same format, creating a JSON object with `json_object`, then filling it with `json_object_set`:
```
json_t *outputparams = NULL;
outputparams = json_object();

char tx_amount_string[32];
sprintf(tx_amount_string,"%.8f",tx_amount);
char tx_change_string[32];
sprintf(tx_change_string,"%.8f",tx_value - tx_total);

json_object_set(outputparams, tx_recipient, json_string(tx_amount_string));
json_object_set(outputparams, changeaddress, json_string(tx_change_string));
```

> :warning: **WARNING:** You might expect to input your Bitcoin values as numbers, using `json_real`. Unfortunately, this exposes one of the major problems with integrating the `jansson` library and Bitcoin. Bitcoin is only valid to eight significant digits past the decimal point. You might recall that .00000001 BTC is a satoshi, and that's the smallest possible division of a Bitcoin. Doubles in C offer more significant digits than that, though they're often imprecise out past eight decimals. If you try to convert straight from your double value in C (or a float value, for that matter) to a Bitcoin value, the imprecision will often create a Bitcoin value with more than eight significant digits. Before Bitcoin Core 0.12 this appears to work, and you could use `json_real`. But as of Bitcoin Core 0.12, if you try to give `createrawtransaction` a Bitcoin value with too many significant digits, you'll instead get an error and the transaction will not be created. As a result, if the Bitcoin value has _ever_ become a double or float, you must reformat it to eight significant digits past the digit before feeding it in as a string. This is obviously a kludge, so you should make sure it continues to work in future versions of Bitcoin Core.

#### Step 6.3: Create the Parameter Array

To finish creating your parameters, simply bundle them all up in a JSON array:
```
json_t *params = NULL;
params = json_array();
json_array_append(params,inputparams);
json_array_append(params,outputparams);
```
#### Step 6.4 Make the RPC Call

Use the normal method to create your RPC call:
```
rpc_method = bitcoinrpc_method_init(BITCOINRPC_METHOD_CREATERAWTRANSACTION);
```
However, now you must feed it your parameters. This simply done with `bitcoinrpc_method_set_params`:
```
if (bitcoinrpc_method_set_params(rpc_method, params) != BITCOINRPCE_OK) {

  fprintf (stderr, "Error: Could not set params for createrawtransaction");

}
```
Afterward, run the RPC and get the results as usual:
```
bitcoinrpc_call(rpc_client, rpc_method, btcresponse, &btcerror);

lu_response = bitcoinrpc_resp_get(btcresponse);
lu_result = json_object_get(lu_response,"result");

char *tx_rawhex = strdup(json_string_value(lu_result));
```
### Step 7. Sign the Transaction

It's a lot easier to assign a simple parameter to a function. You just create a JSON array, then assign the parameter to the array:
```
params = json_array();
json_array_append_new(params,json_string(tx_rawhex));
```
Sign the transaction by following the typical rigamarole for creating an RPC call:
```
rpc_method = bitcoinrpc_method_init(BITCOINRPC_METHOD_SIGNRAWTRANSACTION);
if (bitcoinrpc_method_set_params(rpc_method, params) != BITCOINRPCE_OK) {

  fprintf (stderr, "Error: Could not set params for signrawtransaction");

}

json_decref(params);

bitcoinrpc_call(rpc_client, rpc_method, btcresponse, &btcerror);
lu_response = bitcoinrpc_resp_get(btcresponse);
```
Again, using `jansson` to access the output can be a little tricky. Here you have to remember that `hex` is part of a JSON object, not a standalone result, as was the case when you created the raw transaction. Of course, you can always access this information from command line help: `bitcoin-cli help signrawtransaction`:
```
lu_result = json_object_get(lu_response,"result");
json_t *lu_signature = json_object_get(lu_result,"hex");
char *tx_signrawhex = strdup(json_string_value(lu_signature));
json_decref(lu_signature);
```

> :warning: ***WARNING:*** A real-world program would obviously carefully test the response of every RPC command to make sure there were no errors. That's especially true for `signrawtransaction`, because you might end up with a partially signed transaction. Worse, if you don't check the errors in the JSON object, you'll just see the `hex` and not realize that it's either unsigned or partially signed. 

### Step 8. Send the Transaction

You can now send your transaction, using all of the previous techniques:
```
params = json_array();
json_array_append_new(params,json_string(tx_signrawhex));

rpc_method = bitcoinrpc_method_init(BITCOINRPC_METHOD_SENDRAWTRANSACTION);

if (bitcoinrpc_method_set_params(rpc_method, params) != BITCOINRPCE_OK) {

  fprintf (stderr, "Error: Could not set params for sendrawtransaction");

}

json_decref(params);

bitcoinrpc_call(rpc_client, rpc_method, btcresponse, &btcerror);
lu_response = bitcoinrpc_resp_get(btcresponse);
lu_result = json_object_get(lu_response,"result");

char *tx_newid = strdup(json_string_value(lu_result));

printf("Txid: %s\n",tx_newid);
```
The entire code, with a _little_ more error-checking appears in the Appendix.

## Test Your Code

The complete code can be found in the [src directory](src/16_2_sendtoaddress.c).

Compile this as usual:
```
$ cc sendtoaddress.c -lbitcoinrpc -ljansson -o sendtoaddress
```
You can then use it to send funds to an address:

```
./sendtoaddress tb1qynx7f8ulv4sxj3zw5gqpe56wxleh5dp9kts7ns .001
Txid: b93b19396f8baa37f5f701c7ca59d3128144c943af5294aeb48e3eb4c30fa9d2
```
You can see information on this transaction that we sent [here](https://live.blockcypher.com/btc-testnet/tx/b93b19396f8baa37f5f701c7ca59d3128144c943af5294aeb48e3eb4c30fa9d2/). 

## Summary: Programming Bitcoind with C

With access to a C library, you can create much more fully featured programs than it was reasonable to do so with shell scripts. But, it can take a lot of work! Even at 316 lines of code, `sendtoaddress.c` doesn't cover nearly all of the intricacies requires to safely and intelligently transact bitcoins.

## What's Next?

Learn more about "Talking to Bitcoind with C" in [16.3: Receiving Notifications in C with ZMQ Libraries](16_3_Receiving_Bitcoind_Notifications_with_C.md).
