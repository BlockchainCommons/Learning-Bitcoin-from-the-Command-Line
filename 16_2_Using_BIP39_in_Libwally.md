# 16.2: Using BIP39 in Libwally

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

One of Libwally's greatest powers is that it can lay bare the underlying work of creating addresses. To start with, it supports [BIP39](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki), which is the BIP that supports Mnemonic codes for Bitocin addresses.

> :book: ***What is a Mnemonic Code?*** Bitcoin addresses (and their corresponding private keys) are long, untintelligible lists of characters and numbers, which are not only impossible to remember, but also easy to mess up. Mnemonic codes were a solution for this that allow users to remember 12 (or 24) words in their language. These codes can then be used to fully restore a BIP32 seed that's the basis of an HD wallet.

_In process_

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

  /* 4. Translate into Seed */

  size_t seed_len;
  unsigned char seed[BIP39_SEED_LEN_512];
  
  lw_response = bip39_mnemonic_to_seed(mnem,NULL,&seed,BIP39_SEED_LEN_512,&seed_len);

  printf("Seed: %x\n",seed);
  
} 

