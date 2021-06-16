#include <stdio.h>
#include "wally_core.h"
#include "wally_psbt.h"

int main(int argc, char *argv[]) {

  /* 1. Request a PSBT */
  
  if (argc != 2) {

    printf("ERROR: Only %i arguments! Correct usage is '%s [psbt]'\n",argc-1,argv[0]);
    exit(-1);
    
  }
  char *psbt_64 = argv[1];

  /* 2. Convert the PSBT */
  
  int lw_response;

  struct wally_psbt *psbt;
  
  lw_response = wally_psbt_from_base64(psbt_64,&psbt);

  if (lw_response) {

    printf("Error: Wally_psbt_from_base64 failed: %d\n",lw_response);
    exit(-1);
    
  }
  
  /* 3. Read the Inputs */

  int inputs = psbt->num_inputs;
  printf("TOTAL INPUTS: %i\n",inputs);

  /* This will crash if the inputs are not filled in, or if there are non_witness_utxos */
  
  for (int i = 0 ; i < inputs ; i++) {
    printf("\nINPUT #%i: %i satoshis\n",i, psbt->inputs[i].witness_utxo->satoshi);

    char *script_hex;
    wally_hex_from_bytes(psbt->inputs[i].witness_utxo->script,psbt->inputs[i].witness_utxo->script_len,&script_hex);
    printf("scriptPubKey: %s\n",script_hex);
    wally_free_string(script_hex);
    
  }
  
  /* 4. Read the Outputs */

  int outputs = psbt->num_outputs;
  printf("\nTOTAL OUTPUTS: %i\n",outputs);
  
  /* This will crash if the outputs are not filled in */
  
  for (int i = 0 ; i < outputs ; i++) {

    char *pubkey_hex;
    wally_hex_from_bytes(psbt->tx->outputs[i].script,psbt->tx->outputs[i].script_len,&pubkey_hex);
    printf("\nINPUT #%i\n",i);
    printf("scriptPubKey: %s\n",pubkey_hex);
    wally_free_string(pubkey_hex);    
  }

  wally_cleanup(0);  
}
