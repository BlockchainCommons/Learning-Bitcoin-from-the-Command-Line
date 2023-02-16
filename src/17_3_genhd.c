#include <stdio.h>
#include "sodium.h"
#include "wally_core.h"
#include "wally_bip39.h"
#include "wally_bip32.h"
#include "wally_address.h"

int main(void) {

  int lw_response;

  /* 1. Initialize Libwally */
  
  lw_response = wally_init(0);

  if (lw_response) {

    printf("Error: Wally_init failed: %d\n",lw_response);
    exit(-1);
    
  }

  /* 2. Generate Entropy + Mnemonic + Seed */

  unsigned char entropy[16];  
  randombytes_buf(entropy, 16);

  char *mnem = NULL;
  lw_response = bip39_mnemonic_from_bytes(NULL,entropy,16,&mnem);

  if (lw_response) {

    printf("Error: bip39_mnemonic_from_bytes failed: %d\n",lw_response);
    exit(-1);
    
  }

  printf("Mnemonic: %s\n",mnem);
  
  unsigned char seed[BIP39_SEED_LEN_512];
  size_t seed_len;
  
  lw_response = bip39_mnemonic_to_seed(mnem,NULL,seed,BIP39_SEED_LEN_512,&seed_len);

  if (lw_response) {

    printf("Error: bip39_mnemonic_to_seed failed: %d\n",lw_response);
    exit(-1);
    
  }

  /* 3. Generate root */

  struct ext_key *key_root;
  
  lw_response = bip32_key_from_seed_alloc(seed,sizeof(seed),BIP32_VER_TEST_PRIVATE,0,&key_root);

  if (lw_response) {

    printf("Error: bip32_key_from_seed failed: %d\n",lw_response);
    exit(-1);
    
  }

  /* 4. Generate master xprv + xpub */

  char *xprv;
  lw_response = bip32_key_to_base58(key_root, BIP32_FLAG_KEY_PRIVATE, &xprv);

  if (lw_response) {

    printf("Error: root bip32_key_to_base58 failed: %d\n",lw_response);
    exit(-1);
    
  }
  
  printf("Root xprv key: %s\r\n", xprv);

  char *xpub;  
  lw_response = bip32_key_to_base58(key_root, BIP32_FLAG_KEY_PUBLIC, &xpub);

  if (lw_response) {

    printf("Error: root bip32_key_to_base58 failed: %d\n",lw_response);
    exit(-1);
    
  }

  printf("Root xpub key: %s\r\n", xpub);

  /* 5. Generate key for account */

  struct ext_key *key_account;
  
  uint32_t path_account[] = {BIP32_INITIAL_HARDENED_CHILD+84, BIP32_INITIAL_HARDENED_CHILD+1, BIP32_INITIAL_HARDENED_CHILD};

  lw_response = bip32_key_from_parent_path_alloc(key_root,path_account,sizeof(path_account),BIP32_FLAG_KEY_PRIVATE,&key_account);

  if (lw_response) {

    printf("Error: bip32_key_from_parent_path_alloc failed: %d\n",lw_response);
    exit(-1);
    
  }

  char *a_xprv;
  lw_response = bip32_key_to_base58(key_account, BIP32_FLAG_KEY_PRIVATE, &a_xprv);

  if (lw_response) {

    printf("Error: account bip32_key_to_base58 failed: %d\n",lw_response);
    exit(-1);
    
  }
  
  printf("Account xprv key: %s\r\n", a_xprv);

  char *a_xpub;  
  lw_response = bip32_key_to_base58(key_account, BIP32_FLAG_KEY_PUBLIC, &a_xpub);

  if (lw_response) {

    printf("Error: account bip32_key_to_base58 failed: %d\n",lw_response);
    exit(-1);
    
  }

  printf("Account xpub key: %s\r\n", a_xpub);

  /* 6. Generate External Adress Key */

  struct ext_key *key_external;  

  lw_response = bip32_key_from_parent_alloc(key_account,0,BIP32_FLAG_KEY_PRIVATE,&key_external);

  if (lw_response) {

    printf("Error: root bip32_key_from_parent_alloc Level #1 failed: %d\n",lw_response);
    exit(-1);
    
  }

  struct ext_key *key_address;  

  lw_response = bip32_key_from_parent_alloc(key_external,0,BIP32_FLAG_KEY_PRIVATE,&key_address);

  if (lw_response) {

    printf("Error: root bip32_key_from_parent_alloc Level #2 failed: %d\n",lw_response);
    exit(-1);
    
  }

  /* 7. Generate Address */

  char *segwit;
  lw_response = wally_bip32_key_to_addr_segwit(key_address,"tb",0,&segwit);

  printf("[m/84'/1'/0'/0/0]: %s\n",segwit);
					       
  /* Cleanup! */

  bip32_key_free(key_address);      
  bip32_key_free(key_external);    
  bip32_key_free(key_account);  
  bip32_key_free(key_root);

  wally_free_string(xprv);
  wally_free_string(xpub);  
  wally_free_string(a_xprv);
  wally_free_string(a_xpub);

  wally_free_string(mnem);

  wally_cleanup(0);
    
} 
