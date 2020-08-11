#include <stdio.h>
#include "sodium.h"
#include "wally_core.h"
#include "wally_bip39.h"

int main(void) {

  int lw_response;

  /* 1. Initialize Libwally */
  
  lw_response = wally_init(0);

  if (lw_response) {

    printf("Error: Wally_init failed: %d\n",lw_response);
    exit(-1);
    
  }

  /* 2. Generate Entropy */

  unsigned char entropy[16];  
  randombytes_buf(entropy, 16);
  
  /* 3. Translate into Mnemonic */

  char *mnem = NULL;
  lw_response = bip39_mnemonic_from_bytes(NULL,entropy,16,&mnem);

  if (lw_response) {

    printf("Error: bip39_mnemonic_from_bytes failed: %d\n",lw_response);
    exit(-1);
    
  }
  
  printf("Mnemonic: %s\n",mnem);

  /* 4. Validate the Mnemonic */

  lw_response = bip39_mnemonic_validate(NULL,mnem);
  
  if (lw_response) {

    printf("Error: Mnemonic did not validate: %d\n",lw_response);
    exit(-1);

  } else {

    printf("Mnemonic validated!\n");

  }
      
  /* 5. Translate into Seed */

  size_t seed_len;
  unsigned char *seed;
  
  lw_response = bip39_mnemonic_to_seed(mnem,NULL,seed,BIP39_SEED_LEN_512,&seed_len);

    if (lw_response) {

    printf("Error: bip39_mnemonic_to_seed failed: %d\n",lw_response);
    exit(-1);
    
  }

  
} 
