# 16.3: Using BIP32 in Libwally

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

In [§16.2](16_2_Using_BIP39_in_Libwally.md), you were able to use entropy to generate a seed and its related mnemonic. As you may recall from [§3.5: Understanding the Descriptor](03_5_Understanding_the_Descriptor.md), a seed is the basis of a Hierchical Deterministic (HD) Wallet, where that single seed can be used to generate many addresses. So how do you get from the seed to actual addresses? That's where [BIP32](https://en.bitcoin.it/wiki/BIP_0032) comes in.

## Creating HD Addresses

To create a HD address requires starting with a seed, and then walking down the hierarchy until the point that you create addresses.

That starts off easily enough, you just generate a seed, which you already did in the previous section:
```
  unsigned char entropy[16];  
  randombytes_buf(entropy, 16);

  char *mnem = NULL;
  lw_response = bip39_mnemonic_from_bytes(NULL,entropy,16,&mnem);
  
  unsigned char seed[BIP39_SEED_LEN_512];
  size_t seed_len;  
  lw_response = bip39_mnemonic_to_seed(mnem,NULL,seed,BIP39_SEED_LEN_512,&seed_len);
```
### Generating a Root Key

With a seed in hand, you can then generate a master extended key with the `bip32_key_from_seed_alloc` function (or alternatively the `bip32_key_from_seed`, which doesn't do the `alloc`):
```
  struct ext_key *key_root;  
  lw_response = bip32_key_from_seed_alloc(seed,sizeof(seed),BIP32_VER_TEST_PRIVATE,0,&key_root);
```
As you can see, you'll need to tell it what version of the key to return, in this case `BIP32_VER_TEST_PRIVATE`, a private testnet key.

> :link: **TESTNET vs MAINNET:** On mainnet, you'd instead ask for `BIP32_VER_TEST_PRIVATE`.

### Generating xpub & xprv

Whenever you have a key in hand, you can turn it into xpub or xprv keys for distribution with the `bip32_key_to_base58` command. You just tell it whether you want a `PRIVATE` (xprv) or `PUBLIC` (xpub) key:
```
  char *xprv;
  lw_response = bip32_key_to_base58(key_root, BIP32_FLAG_KEY_PRIVATE, &xprv);

  char *xpub;  
  lw_response = bip32_key_to_base58(key_root, BIP32_FLAG_KEY_PUBLIC, &xpub);
```

### Understanding the Hierarchy

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

  printf("[84'/0'/0'/0/0]%s\n",segwit);
					       
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
