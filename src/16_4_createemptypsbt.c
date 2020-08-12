#include <stdio.h>
#include <stdlib.h>
#include "wally_core.h"
#include "wally_psbt.h"
#include "wally_transaction.h"

int main(void) {

  int lw_response;
  struct wally_psbt *psbt;
					     
  char *psbt_64;
  
  lw_response = wally_psbt_init_alloc(0,1,1,0,&psbt);

  if (lw_response) {

    printf("Error: Wally_psbt_init_alloc failed: %d\n",lw_response);
    exit(-1);
    
  }

  struct wally_tx *wtx;
  lw_response = wally_tx_init_alloc(0,0,1,1,&wtx);

  if (lw_response) {

    printf("Error: Wally_tx_init_alloc failed: %d\n",lw_response);
    exit(-1);
    
  }
  
  lw_response = wally_tx_clone(wtx, 0, &psbt->tx);
  
  if (lw_response) {

    printf("Error: Wally_tx_clone failed: %d\n",lw_response);
    exit(-1);
    
  }
  
  psbt->num_inputs = wtx->num_inputs;
  psbt->num_outputs = wtx->num_outputs;
  wally_tx_free(wtx);
  
  wally_psbt_to_base64(psbt,0,&psbt_64);
  printf("%s\n",psbt_64);

  wally_psbt_free(psbt);
  
}
