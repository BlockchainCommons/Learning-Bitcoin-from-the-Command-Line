# 17.3: Using BIP32 in Libwally

> :information_source: **NOTE:** This section has been recently added to the course and is an early draft that may still be awaiting review. Caveat reader.

In [§17.2](17_2_Using_BIP39_in_Libwally.md), you were able to use entropy to generate a seed and its related mnemonic. As you may recall from [§3.5: Understanding the Descriptor](03_5_Understanding_the_Descriptor.md), a seed is the basis of a Hierchical Deterministic (HD) Wallet, where that single seed can be used to generate many addresses. So how do you get from the seed to actual addresses? That's where [BIP32](https://en.bitcoin.it/wiki/BIP_0032) comes in.

## Create an HD Root

To create a HD address requires starting with a seed, and then walking down the hierarchy until the point that you create addresses.

That starts off easily enough, you just generate a seed, which you already did in the previous section:
```c
  unsigned char entropy[16];  
  randombytes_buf(entropy, 16);

  char *mnem = NULL;
  lw_response = bip39_mnemonic_from_bytes(NULL,entropy,16,&mnem);
  
  unsigned char seed[BIP39_SEED_LEN_512];
  size_t seed_len;  
  lw_response = bip39_mnemonic_to_seed(mnem,NULL,seed,BIP39_SEED_LEN_512,&seed_len);
```
### Generate a Root Key

With a seed in hand, you can then generate a master extended key with the `bip32_key_from_seed_alloc` function (or alternatively the `bip32_key_from_seed`, which doesn't do the `alloc`):
```c
  struct ext_key *key_root;  
  lw_response = bip32_key_from_seed_alloc(seed,sizeof(seed),BIP32_VER_TEST_PRIVATE,0,&key_root);
```
As you can see, you'll need to tell it what version of the key to return, in this case `BIP32_VER_TEST_PRIVATE`, a private testnet key.

> :link: **TESTNET vs MAINNET:** On mainnet, you'd instead ask for `BIP32_VER_MAIN_PRIVATE`.

### Generate xpub & xprv

Whenever you have a key in hand, you can turn it into xpub or xprv keys for distribution with the `bip32_key_to_base58` command. You just tell it whether you want a `PRIVATE` (xprv) or `PUBLIC` (xpub) key:
```c
  char *xprv;
  lw_response = bip32_key_to_base58(key_root, BIP32_FLAG_KEY_PRIVATE, &xprv);

  char *xpub;  
  lw_response = bip32_key_to_base58(key_root, BIP32_FLAG_KEY_PUBLIC, &xpub);
```

## Understand the Hierarchy

Before going further, you need to understand how the hierarchy of an HD wallet works. As discussed in [§3.5](03_5_Understanding_the_Descriptor.md), a derivation path describes the tree that you follow to get to a hierarchical key, so `[0/1/0]` is the 0th child of the 1st child of the 0th child of a root key. Sometimes part of that derivation are marked with `'`s or `h`s to show hardened derivations, which increase security: `[0'/1'/0']`.

However, for HD wallets, each of those levels of the hierachy is used in a very specific way. This was originally defined in [BIP44](https://github.com/bitcoin/bips/blob/master/bip-0044.mediawiki) and was later updated for Segwit in [BIP84].

Altogether, a BIP32 derivation path is defined to have five levels:

1. **Purpose.** This is usually set to `44'` or `84'`, depending on the BIP that is being followed.
2. **Coin.** For Mainnet bitcoins, this is `0'`, for testnet it's `1'`.
3. **Account.** A wallet can contain multiple, discrete accounts, starting with `0'`.
4. **Change.** External addresses (for distribution) are set to `0`, while internal addresses (for change) are set to `1`.
5. **Index.** The nth address for the hierarchy, starting with `0`.

So on testnet, the zeroth adddress for an external address for the zeroth account for testnet coins using the BIP84 standards is `[m/84'/1'/0'/0/0]`. That's the address you'll be creating momentarily.

> :link: **TESTNET vs MAINNET:** For mainnet, that'd be `[m/84'/0'/0'/0/0]`

### Understand the Hierarchary in Bitcoin Core

We'll be using the above hierarchy for all HD keys in Libwally, but note that this standard isn't used by Bitcoin Core's `bitcoin-cli`, which instead uses `[m/0'/0'/0']` for the 0th external address and `[m/0'/1'/0']` for the 0th change address.

## Generate an Address

To generate an address, you thus have to dig down through the whole hierarchy.

### Generate an Account Key

One way to do this is to use the `bip32_key_from_parent_path_alloc` function to drop down several levels of a hierarchy. You embed the levels in an array:
```c
  uint32_t path_account[] = {BIP32_INITIAL_HARDENED_CHILD+84, BIP32_INITIAL_HARDENED_CHILD+1, BIP32_INITIAL_HARDENED_CHILD};
```
Here we'll be looking at the zeroth hardened child (that's the account) or the first hardened child (that's testnet coins) of the 84th hardened child (that's the BIP84 standard): `[m/84'/1'/0']`.

You can then use that path to generate a new key from your old key:
```c
  struct ext_key *key_account;
  lw_response = bip32_key_from_parent_path_alloc(key_root,path_account,sizeof(path_account),BIP32_FLAG_KEY_PRIVATE,&key_account);
```
Every time you have a new key, you can use that to generate new xprv and xpub keys, if you desire:
```c
  lw_response = bip32_key_to_base58(key_account, BIP32_FLAG_KEY_PRIVATE, &a_xprv);
  lw_response = bip32_key_to_base58(key_account, BIP32_FLAG_KEY_PUBLIC, &a_xpub);
```

### Generate an Address Key

Alternatively, you can use the `bip32_key_from_parent_alloc` function, which just drops down one level of the hierarchy at a time. The following example drops down to the 0th child of the account key (which is the external address) and then the 0th child of that. This would be useful because then you could continue generating the 1st address, the 2nd address, and so on from that external key:
```c
  struct ext_key *key_external;  
  lw_response = bip32_key_from_parent_alloc(key_account,0,BIP32_FLAG_KEY_PRIVATE,&key_external);

  struct ext_key *key_address;  
  lw_response = bip32_key_from_parent_alloc(key_external,0,BIP32_FLAG_KEY_PRIVATE,&key_address);
```
> :warning: **WARNING:** At some point in this hierarchy, you might decide to generate `BIP32_FLAG_KEY_PUBLIC` instead of `BIP32_FLAG_KEY_PRIVATE`. Obviously this decision will be based on your security and your needs, but remember that you only need a public key to generate the actual address.

### Generate an Address

Finally, you're ready to generate an address from your final key. All you do is run `wally_bip32_to_addr_segwit` using your final key and a description of what sort of address this is.
```c
  char *segwit;
  lw_response = wally_bip32_key_to_addr_segwit(key_address,"tb",0,&segwit);

  printf("[m/84'/1'/0'/0/0]: %s\n",segwit);
```  

> :link: **TESTNET vs MAINNET:** The `tb` argument defines a testnet address. For mainnet instead use `bc`.

There is also a `wally_bip32_key_to_address` function, which can be used to generate a legacy address or a nested Segwit address.

## Test HD Code

The code for these HD example can, as usual, be found in the [src directory](src/17_3_genhd.c).

You can compile and test it:
```
$ cc genhd.c -lwallycore -lsodium -o genhd
$ ./genhd
Mnemonic: behind mirror pond finish borrow wood park foam guess mail regular reflect
Root xprv key: tprv8ZgxMBicQKsPdLFXmZ6VegTxcmeieNpRUq8J2ahXxSaK2aF7CGqAc14ZADLjdHJdCr8oR2Zng9YH1x1A7EBaajQLVGNtxc4YpFejdE3wyj8
Root xpub key: tpubD6NzVbkrYhZ4WoHKfCm64685BoAeoi1L48j5K6jqNiNhs4VspfeknVgRLLiQJ3RkXiA9VxguUjmEwobtmrXNbhXsPHfm9W5HJR9DKRGaGJ2
Account xprv key: tprv8yZN7h6SPvJXrhAk56z6cwHQE6qZBRreB9fqqZJ1Xd1nLci3Rw8HTmqNkpFNgf3eZx8hYzhFWafUhHSt3HgF13aHvCE6kveS7gZAyfQwMDi
Account xpub key: tpubDWFQG78gYHzCkACXxkeh2LwWo8MVLm3YkTGd85LJwtpBB6xp4KwseGTEvxjeZNhnCNPdfZqRcgcZZAka4tD3xGS2J53WKHPMRhG357VKsqT
[m/84'/1'/0'/0/0]: tb1q0knqq26ek59pfl7nukzqr28m2zl5wn2f0ldvwu
```

## Summary: Using BIP32 in Libwally

An HD wallet allows you to generate a vast number of keys from a single seed. You now know how those keys are organized under BIP44, BIP84, and Bitcoin Core and how to derive them, starting with either a seed or mnemonic words.

> :fire: ***What is the power of BIP32?*** Keys are the most difficult (and most dangerous) element of most cryptographic operations. If you lose them, you lose whatever the key protected. BIP32 ensures that you just need to know one key, the seed, rather than a huge number of different keys for different addresses.

> :fire: ***What is the power of BIP32 in Libwally?*** Bitcoind already does HD-based address creation for you, which means you don't usually have to worry about deriving addresses in this way. However, using the BIP32 functions of Libwally can be very useful if you have an offline machine where you need to derive addresses, possibly based on a seed passed out of `bitcoind` to your offline device (or vice-versa).

## What's Next?

Learn more about "Programming Bitcoin with Libwally" in [17.4: Using PSBTs in Libwally](17_4_Using_PSBTs_in_Libwally.md).
