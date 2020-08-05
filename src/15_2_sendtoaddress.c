#include <jansson.h>
#include <bitcoinrpc.h>
#include <string.h>

int main(int argc, char *argv[]) {

  /* 1. Request an Address and an Amount */
  
  if (argc != 3) {

    printf("ERROR: Only %i arguments! Correct usage is '%s [recipient] [amount]'\n",argc-1,argv[0]);
    exit(-1);
    
  }
  char *tx_recipient = argv[1];
  double tx_amount = atof(argv[2]);
		   
  /* 2. Set an Arbitrary Fee */
  
  double tx_fee = 0.0005;
  double tx_total = tx_amount + tx_fee;

  /* 3. Prepare Your RPC */
  
  bitcoinrpc_cl_t *rpc_client;
  bitcoinrpc_method_t *rpc_method  = NULL;
  bitcoinrpc_resp_t *btcresponse  = NULL;
  bitcoinrpc_err_t btcerror;

  json_t *lu_response = NULL;
  json_t *lu_result = NULL;

  bitcoinrpc_global_init();

  rpc_client = bitcoinrpc_cl_init_params ("bitcoinrpc", "YOUR-RPC-PASSWD", "127.0.0.1", 18332);

  if (rpc_client) {

    /* 4. Find a UTXO */
    
    rpc_method = bitcoinrpc_method_init(BITCOINRPC_METHOD_LISTUNSPENT);

    if (!rpc_method) {

      printf("ERROR: Unable to initialize listunspent method!\n");
      exit(-1);

    }

    btcresponse = bitcoinrpc_resp_init();
    if (!btcresponse) {

      printf("Error: Cannot initialize response object!");
      exit(-1);

    }
    
    bitcoinrpc_call(rpc_client, rpc_method, btcresponse, &btcerror);

    if (btcerror.code != BITCOINRPCE_OK) {

      printf("Error: listunspent error code %d [%s]\n", btcerror.code,btcerror.msg);
      exit(-1);

    }

    lu_response = bitcoinrpc_resp_get (btcresponse);
    lu_result = json_object_get(lu_response,"result");

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
      
    }

    json_decref(lu_result);
    json_decref(lu_response);
		
    if (!tx_id) {

      printf("Very Sad: You don't have any UTXOs larger than %f\n",tx_total);
      exit(-1);
    }

    bitcoinrpc_method_free(rpc_method);
    
    /* 5. Create a Change Address */
 
    rpc_method = bitcoinrpc_method_init(BITCOINRPC_METHOD_GETRAWCHANGEADDRESS);

    if (!rpc_method) {

      printf("ERROR: Unable to initialize createrawchangeaddress method!\n");
      exit(-1);

    }

    bitcoinrpc_call(rpc_client, rpc_method, btcresponse, &btcerror);

    if (btcerror.code != BITCOINRPCE_OK) {

      printf("Error: createrawchangeaddress error code %d [%s]\n", btcerror.code, btcerror.msg);
      exit(-1);

    }

    lu_response = bitcoinrpc_resp_get (btcresponse);
    lu_result = json_object_get(lu_response,"result");
    char *changeaddress = strdup(json_string_value(lu_result));

    json_decref(lu_result);
    json_decref(lu_response);
    bitcoinrpc_method_free(rpc_method);
    
    /* 6. Create a Raw Transaction */

    /* 6.1. Create the Input Parameters */
    
    json_t *inputtxid = NULL;
    inputtxid = json_object();

    json_object_set_new(inputtxid,"txid",json_string(tx_id));
    json_object_set_new(inputtxid,"vout",json_integer(tx_vout));

    json_t *inputparams = NULL;
    inputparams = json_array();
    json_array_append(inputparams,inputtxid);

    /* 6.2 Create the Output Parameters */
    
     json_t *outputparams = NULL;
    outputparams = json_object();
    
    char tx_amount_string[32];
    sprintf(tx_amount_string,"%.8f",tx_amount);
    char tx_change_string[32];
    sprintf(tx_change_string,"%.8f",tx_value - tx_total);
    
    json_object_set(outputparams,
			tx_recipient,
			json_string(tx_amount_string));
    json_object_set(outputparams,
			changeaddress,
			json_string(tx_change_string));

    /* 6.3 Create the Parameter Array */
    
    json_t *params = NULL;
    params = json_array();
    json_array_append(params,inputparams);
    json_array_append(params,outputparams);

    /* 6.4 Make the RPC Call */
    
    rpc_method = bitcoinrpc_method_init(BITCOINRPC_METHOD_CREATERAWTRANSACTION);

    if (!rpc_method) {

      printf("ERROR: Unable to initialize createrawtransaction method!\n");
      exit(-1);

    }


    if (bitcoinrpc_method_set_params(rpc_method, params) != BITCOINRPCE_OK) {

      fprintf (stderr, "Error: Could not set params for createrawtransaction");
      
    }

    json_decref(inputtxid);
    json_decref(inputparams);
    json_decref(outputparams);
    json_decref(params);
		
    bitcoinrpc_call(rpc_client, rpc_method, btcresponse, &btcerror);    

    if (btcerror.code != BITCOINRPCE_OK) {

      printf("Error: createrawtransaction error code %d [%s]\n", btcerror.code,btcerror.msg);
      exit(-1);

    }

    lu_response = bitcoinrpc_resp_get(btcresponse);
    lu_result = json_object_get(lu_response,"result");

    char *tx_rawhex = strdup(json_string_value(lu_result));

    json_decref(lu_result);
    json_decref(lu_response);
    bitcoinrpc_method_free(rpc_method);

    /* 7. Sign the Transaction */

    params = json_array();
    json_array_append_new(params,json_string(tx_rawhex));

    rpc_method = bitcoinrpc_method_init(BITCOINRPC_METHOD_SIGNRAWTRANSACTION);

    if (!rpc_method) {

      printf("ERROR: Unable to initialize signrawtransaction method!\n");
      exit(-1);

    }

    if (bitcoinrpc_method_set_params(rpc_method, params) != BITCOINRPCE_OK) {

      fprintf (stderr, "Error: Could not set params for signrawtransaction");
      
    }

    json_decref(params);

    bitcoinrpc_call(rpc_client, rpc_method, btcresponse, &btcerror);    

    if (btcerror.code != BITCOINRPCE_OK) {

      printf("Error: signrawtransaction error code %d [%s]\n", btcerror.code,btcerror.msg);
      exit(-1);

    }

    lu_response = bitcoinrpc_resp_get(btcresponse);
    lu_result = json_object_get(lu_response,"result");

    json_t *lu_signature = json_object_get(lu_result,"hex");
    char *tx_signrawhex = strdup(json_string_value(lu_signature));
    json_decref(lu_signature);

    json_decref(lu_result);
    json_decref(lu_response);
    bitcoinrpc_method_free(rpc_method);

    /* 8. Send the Transaction */

    params = json_array();
    json_array_append_new(params,json_string(tx_signrawhex));

    rpc_method = bitcoinrpc_method_init(BITCOINRPC_METHOD_SENDRAWTRANSACTION);

    if (!rpc_method) {

      printf("ERROR: Unable to initialize sendrawtransaction method!\n");
      exit(-1);

    }

    if (bitcoinrpc_method_set_params(rpc_method, params) != BITCOINRPCE_OK) {

      fprintf (stderr, "Error: Could not set params for sendrawtransaction");
      
    }

    json_decref(params);

    bitcoinrpc_call(rpc_client, rpc_method, btcresponse, &btcerror);    

    if (btcerror.code != BITCOINRPCE_OK) {

      printf("Error: endrawtransaction error code %d [%s]\n", btcerror.code,btcerror.msg);
      exit(-1);

    }

    lu_response = bitcoinrpc_resp_get(btcresponse);
    lu_result = json_object_get(lu_response,"result");

    char *tx_newid = strdup(json_string_value(lu_result));

    printf("Txid: %s\n",tx_newid);
    
    json_decref(lu_result);
    json_decref(lu_response);
    bitcoinrpc_method_free(rpc_method);
    
  } else {

    printf("ERROR: Failed to connect to server!\n");

  }

  bitcoinrpc_cl_free(rpc_client);
  bitcoinrpc_global_cleanup();

}
