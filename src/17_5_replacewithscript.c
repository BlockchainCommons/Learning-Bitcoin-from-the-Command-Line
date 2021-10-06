#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "wally_core.h"
#include "wally_transaction.h"
#include "wally_script.h"

int main(int argc, char *argv[]) {

  if (argc != 3) {

    printf("ERROR: Only %i arguments! Correct usage is '%s [transaction hex] [serialized locking script]'\n",argc-1,argv[0]);
    exit(-1);
    
  }
  char *utxo_hex = argv[1];
  char *script = argv[2];

  int lw_response;
  size_t written;

  /* 1. Create the P2SH Script */

  int script_length = strlen(script)/2;
  unsigned char bscript[script_length];
    
  lw_response = wally_hex_to_bytes(script,bscript,script_length,&written);
  assert(lw_response == WALLY_OK);

  unsigned char p2sh[WALLY_SCRIPTPUBKEY_P2SH_LEN];
  			  
  lw_response = wally_scriptpubkey_p2sh_from_bytes(bscript,sizeof(bscript),WALLY_SCRIPT_HASH160,p2sh,WALLY_SCRIPTPUBKEY_P2SH_LEN,&written);
  assert(lw_response == WALLY_OK);

  char *pubkey;
  lw_response = wally_hex_from_bytes(p2sh,sizeof(p2sh),&pubkey);

  /* 2. Create the Transaction */
  
  struct wally_tx *tx;
  lw_response = wally_tx_init_alloc(2,0,1,1,&tx);
  assert(lw_response == WALLY_OK);

  /* 3. Create the Outputs with the P2SH */
  
  struct wally_tx_output *tx_output;
  lw_response = wally_tx_output_init_alloc(95000,p2sh,sizeof(p2sh),&tx_output);
  assert(lw_response == WALLY_OK);

  lw_response = wally_tx_add_output(tx,tx_output);
  assert(lw_response == WALLY_OK);

  /* 4. Create the Inputs based on Hex */
  
  struct wally_tx *utxo;
  lw_response = wally_tx_from_hex(utxo_hex,0,&utxo);
  assert(lw_response == WALLY_OK);

  struct wally_tx_input *tx_input;
  lw_response = wally_tx_input_init_alloc(utxo->inputs[0].txhash,sizeof(utxo->inputs[0].txhash),utxo->inputs[0].index,0,utxo->inputs[0].script,utxo->inputs[0].script_len,utxo->inputs[0].witness,&tx_input);
  assert(lw_response == WALLY_OK);						  
  lw_response = wally_tx_add_input(tx,tx_input);
  assert(lw_response == WALLY_OK);

  /* 5. Print It */
  
  char *tx_hex;
  lw_response = wally_tx_to_hex(tx,0, &tx_hex);
  assert(lw_response == WALLY_OK);

  printf("%s\n",tx_hex);

  wally_free_string(tx_hex);
  wally_tx_free(tx);
  wally_tx_input_free(tx_input);
  wally_tx_output_free(tx_output);
  wally_tx_free(utxo);
  
  wally_cleanup(0);  

}
