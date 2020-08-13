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

  struct wally_tx *gtx;
  lw_response = wally_tx_init_alloc(0,0,1,1,&gtx);

  if (lw_response) {

    printf("Error: Wally_tx_init_alloc failed: %d\n",lw_response);
    exit(-1);
    
  }

  lw_response = wally_psbt_set_global_tx(psbt,gtx);
  
  if (lw_response) {

    printf("Error: Wally_psbt_set_global_tx failed: %d\n",lw_response);
    exit(-1);
    
  }
  
  wally_psbt_to_base64(psbt,0,&psbt_64);
  printf("%s\n",psbt_64);

  wally_tx_free(gtx);
  wally_psbt_free(psbt);
	
  wally_cleanup(0);	
  
}
