# 16.6: Using Other Functions in Libwally

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Libwally is an extensive library that provides a considerable amount of wallet-related functionality, much of it not available through `bitcoin-cli`. Following is an overview of some functionality not otherwise covered in this chapter.

## Using Cryptographic Functions

A number of cryptographic functions can be directly accessed from Libwally:

   * `wally_aes` — Use AES encryption or decryption
   * `wally_aes_cbc` — Use AES encryption or decryption in CBC mode
   * `wally_hash160` — Use RIPEMD-160(SHA-256) hash
   * `wally_scrypt` — Use Scrypt key derivation
   * `wally_sha256` — Use SHA256 hash
   * `wally_sha256_midstate` — Use SHA256 to hash only the first chunk of data
   * `wally_sha256d` — Conduct a SHA256 double-hash
   * `wally_sha512` — Use SHA512 hash

There are also HMAC functions for the two SHA hashes, which are used generate message-authentication-codes based on the hashes. They're used in [BIP32](https://en.bitcoin.it/wiki/BIP_0032), among other places.

   * `wally_hmac_sha256`
   * `wally_hmac_sha512`
   
Additional functions cover PBKDF2 key derivation and elliptic-curve math.

## Using Address Functions

Libwally contains a number of functions that can be used to import, export, and translate Bitcoin addresses.

Some convert back and forth between addresses and `scriptPubKey` bytes:

   * `wally_addr_segwit_from_bytes` — Convert a witness program (in bytes) into a Segwit address
   * `wally_addr_segwit_to_bytes` — Convert a Segwit address into a `scriptPubKey` (in bytes)
   * `wally_address_to_scriptpubkey` — Convert a legacy address into a `scriptPubKey`(in bytes)
   * `wally_scriptpubkey_to_address` — Convert a `scriptPubKey` (in bytes) into a legacy address
   
Some relate to the wallet import format (WIF):

   * `wally_wif_from_bytes` — Convert a private key (in bytes) to a WIF
   * `wally_wif_is_uncompressed` — Determines if a WIF is uncompressed
   * `wally_wif_to_address` — Derivate a P2PKH address from a WIF
   * `wally_wif_to_bytes` — Convert a WIF to a private key (in bytes)
   * `wally_wif_to_public_key` — Derive a public key (in bytes) from a WIF
   
Some have to do with the BIP32 HD-wallet format:

   * `wally_bip32_key_to_address` — Derive a legacy address from a BIP32 key
   * `wally_bip32_key_to_addr_segwit` — Derive a Segwit address from a BIP32 key
   
## Using Elements Function

Libwally can be compiled to be used with Blockstream's Elements, which includes access to its assets functions.
