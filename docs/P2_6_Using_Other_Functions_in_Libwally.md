# 17.6: Using Other Functions in Libwally

> :information_source: **NOTE:** This section has been recently added to the course and is an early draft that may still be awaiting review. Caveat reader.

Libwally is an extensive library that provides a considerable amount of wallet-related functionality, much of it not available through `bitcoin-cli`. Following is an overview of some functionality not previously covered in this chapter.

## Use Cryptographic Functions

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

## Use Address Functions

Libwally contains a number of functions that can be used to import, export, and translate Bitcoin addresses.

Some convert back and forth between addresses and `scriptPubKey` bytes:

   * `wally_addr_segwit_from_bytes` — Convert a witness program (in bytes) into a Segwit address
   * `wally_addr_segwit_to_bytes` — Convert a Segwit address into a `scriptPubKey` (in bytes)
   * `wally_address_to_scriptpubkey` — Convert a legacy address into a `scriptPubKey`(in bytes)
   * `wally_scriptpubkey_to_address` — Convert a `scriptPubKey` (in bytes) into a legacy address
   
Some relate to the wallet import format (WIF):

   * `wally_wif_from_bytes` — Convert a private key (in bytes) to a WIF
   * `wally_wif_is_uncompressed` — Determines if a WIF is uncompressed
   * `wally_wif_to_address` — Derive a P2PKH address from a WIF
   * `wally_wif_to_bytes` — Convert a WIF to a private key (in bytes)
   * `wally_wif_to_public_key` — Derive a public key (in bytes) from a WIF
   
## Use BIP32 Functions

There are additional BIP32 HD-wallet functions, beyond what was covered in [§17.3: Using BIP32 in Libwally](17_3_Using_BIP32_in_Libwally.md).

   * `bip32_key_get_fingerprint` — Generate a BIP32 fingerprint for an extended key
   * `bip32_key_serialize` — Transform an extended key into serialized bytes
   * `bip32_key_strip_private_key` — Convert an extended private key to an extended public key
   * `bip32_key_unserialize` — Transform serialized bytes into an extended key

There are also numerous various depending on whether you want to allocate memory or have Libwally do the `_alloc` for you.

## Use BIP38 Functions

[BIP38](https://github.com/bitcoin/bips/blob/master/bip-0038.mediawiki) allows for the creation of password-protected private key. We do not teach it because we consider inserting this sort of human factor into key management dangerous. See [#SmartCustody](https://www.smartcustody.com/index.html).

The main functions are:

   * `bip38_from_private_key` — Encode a private key using BIP38
   * `bip38_to_private_key` — Decode a private key using BIP38
   
## Use BIP39 Functions

A few BIP39 mnemonic-word functions were just overviewed in [§17.2: Using BIP39 in Libwally](17_2_Using_BIP39_in_Libwally.md):

   * `bip39_get_languages` — See a list of supported languages
   * `bit39_get_word` — Retrieve a specific word from a language's word list
   * `bip39_get_wordlist` — See a list of words for a language
   
## Use PSBT Functions

Listings of most PSBT functions can be found in [17.4: Using PSBTs in Libwally](17_4_Using_PSBTs_in_Libwally.md).

## Use Script Functions

[§17.5: Using Scripts in Libwally](17_5_Using_Scripts_in_Libwally.md) just barely touched upon Libwally's Scripts functions.

There's another function that lets you determine the sort of script found in a transaction:

   * `wally_scriptpubkey_get_type` — Determine a transaction's script type.

Then there are a slew of functions that create `scriptPubKey` from bytes, `scriptSig` from signatures, and Witnesses from bytes or signatures.

   * `wally_script_push_from_bytes`
   * `wally_scriptpubkey_csv_2of2_then_1_from_bytes`
   * `wally_scriptpubkey_csv_2of3_then_2_from_bytes`
   * `wally_scriptpubkey_multisig_from_bytes`
   * `wally_scriptpubkey_op_return_from_bytes`
   * `wally_scriptpubkey_p2pkh_from_bytes`
   * `wally_scriptpubkey_p2sh_from_bytes`
   * `wally_scriptsig_multisig_from_bytes`
   * `wally_scriptsig_p2pkh_from_der`
   * `wally_scriptsig_p2pkh_from_sig`
   * `wally_witness_multisig_from_bytes`
   * `wally_witness_p2wpkh_from_der`
   * `wally_witness_p2wpkh_from_sig`
   * `wally_witness_program_from_bytes`

## Use Transaction Functions

We also just barely touched upon the functions that can be used to create and convert functions in [§17.5](17_5_Using_Scripts_in_Libwally.md).

There are numerous informational functions, some of the more interesting of which are:

   * `wally_tx_get_length`
   * `wally_tx_get_total_output_satoshi`
   * `wally_tx_get_weight`
   
There also are functions that affect a `wally_tx`, a `wally_tx_input`, a `wally_tx_output`, or a `wally_tx_witness_stack` and that create signatures.

## Use Elements Functions

Libwally can be compiled to be used with Blockstream's Elements, which includes access to its assets functions.

## Summary: Using Other Functions in Libwally

There is much more that you can do with Libwally, more than can be covered in this chapter or even listed in this section. Notably, you can perform cryptographic functions, encode private keys, build complete transactions, and use Elements. The [Libwally docs](https://wally.readthedocs.io/en/latest/) are the place to go for more information, though as of this writing they are both limited and out-of-date. The Libwally header files are a backup if the docs are incomplete or wrong.

## What's Next?

Finish learning about "Programming Bitcoin with Libwally" in [§17.7: Integrating Libwally and Bitcoin-CLI](17_7_Integrating_Libwally_and_Bitcoin-CLI.md).  


