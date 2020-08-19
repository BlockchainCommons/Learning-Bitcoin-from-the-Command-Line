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
   
## Using BIP32 Functions

There are additional BIP32 HD-wallet functions, beyond what was covered in [§16.3: Using BIP32 in Libwally](16_3_Using_BIP32_in_Libwally.md).

   * `bip32_key_get_fingerprint` — Generate a BIP32 fingerprint for an extended key
   * `bip32_key_serialize` — Transform an extended key into serialized bytes
   * `bip32_key_strip_private_key` — Convert an extended private key to an extended public key
   * `bip32_key_unserialize` — Transform serialized bytes into an extended key

There are also numerous various depending on you want to allocate memory of have Libwally doing the `_alloc` for you.

## Using BIP38 Functions

[BIP38](https://github.com/bitcoin/bips/blob/master/bip-0038.mediawiki) allows for the creation of password-protected private key. We do not teach it because we consider inserting this sort of human factor into key management dangerous. See [#SmartCustody](https://www.smartcustody.com/index.html).

The main functions are:

   * `bip38_from_private_key` — Encode a private key using BIP38
   * `bip38_to_private_key` — Decode a private key using BIP38
   


## Using Elements Function2

Libwally can be compiled to be used with Blockstream's Elements, which includes access to its assets functions.
