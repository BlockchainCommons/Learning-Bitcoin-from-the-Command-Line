# 16.2: Using BIP39 in Libwally

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

One of Libwally's greatest powers is that it can lay bare the underlying work of creating addresses. To start with, it supports [BIP39](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki), which is the BIP that supports Mnemonic codes for Bitcoin addresses.

> :book: ***What is a Mnemonic Code?*** Bitcoin addresses (and their corresponding private keys) are long, untintelligible lists of characters and numbers, which are not only impossible to remember, but also easy to mistype. Mnemonic codes are a solution for this that allow users to record 12 (or 24) words in their language, which is much less prone to mistakes. These codes can then be used to fully restore a BIP32 seed that's the basis of an HD wallet.

## Creating Mnemonic Codes

All Bitcoin addresses start with entropy. This first use of Libwally, and its BIP39 addresses, thus shows how to generate entropy and to get a mnemonic code frm that.

> :book: ***What is Entropy?*** It's a fancy way of saying randomness, but it's a carefully measured randomness that's used as the foundation of a true-random-number generated (TRG). That measurement occurs in "bits", with more bits of entropy resulting in more randomness (and thus more protection for what's being generated). For Bitcoin, entropy is the foundation of your seed, which in an HD wallet generates all of your addresses.

You'll always start work with Libwally by initializing the library and testing the results, as first demonstrated in [§16.1](16_1_Setting_Up_Libwally.md):
```
  int lw_response;

  lw_response = wally_init(0);

  if (lw_response) {

    printf("Error: Wally_init failed: %d\n",lw_response);
    exit(-1);
    
  }
```
Now you're ready to entropize.

### Creating Entropy

Using `libsodium`, you can create entropy with the `randombytes_buf` command:
```
  unsigned char entropy[16];  
  randombytes_buf(entropy, 16);
```
This example, which will be the only way we use the `libsodium` library, creates 16 bytes of entropy. Generally, to create a secure mnemonic code, you should use between 128 and 256 bits of entropy, which is 16 to 32 bytes.

>:warning: **WARNING:** Again, be very certain that you're very comfortable with your method for entropy generation before you use it for a real-world program.

### Translating into a Mnemonic

16 bytes of entropy is sufficient to create a 12-character Mnemonic code, which is done with Libwally's `bip39_mnemonic_from_bytes` function:
```
  char *mnem = NULL;
  lw_response = bip39_mnemonic_from_bytes(NULL,entropy,16,&mnem);
```
Note that you have to pass along the byte size, so if you were to increase the size of your entropy, to generate a longer mnemonic phrase, you'd also need to increase the value in this function.

> :note: **NOTE:** There are mnemonic word lists for different languages! The default is to use the English-language list, which is the `NULL` variable in these various commands, but you can alternatively request a different language!

That's it! You've created a mnemonic phrase!

>:book: ***How is the Mnemonic Phrase Created?*** You can learn about that in [BIP39](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki), but if you prefer, Greg Walker has an [excellent example](https://learnmeabitcoin.com/technical/mnemonic): basically, you add a checksum, then you covert each set of 11 bits into a word from the word list. You can do this with the commands `bip39_get_wordlist` and `bip39_get_word` if you don't trust the `bip39_mnemonic_from_bytes` command.

### Translating into a Seed

There are some functions, such as `bip32_key_from_seed` (which we'll meet in the next section) that require you to have the Seed rather than the Mnemonic. The two things are functionally identical: if you have the Seed, you can generate the Mnemonic, and vice-versa.

If you need to generate the Seed from your Mnemonic, you just use the `bip39_mnemonic_to_seed` command:
```
size_t seed_len;
unsigned char *seed;

lw_response = bip39_mnemonic_to_seed(mnem,NULL,seed,BIP39_SEED_LEN_512,&seed_len);
```
## Testing Mnemonic Code

The full code for generating entropy, generating a BIP39 Mnemonic, validating the Mnemonic, and generating a seed can be found in the [src directory](src/16_2_genmnemonic.c). Download it and compile:
```
$ cc genmnemonic.c -lwallycore -lsodium -o genmnemonic
```
Then you can run the test:
```
$ ./genmnemonic 
Mnemonic: point envelope vital weekend avoid cost rice recipe cradle travel armed rich
Mnemonic validated!
```

## Summary: Using BIP39 in Libwally

BIP39 allows you generate a set of 12-24 Mnemonic words from a seed (and the Libwally library also allows you to validate it!).

> :fire: ***What is the power of BIP39?*** Bitcoin seeds and private keys are prone to all sorts of lossage. You mistype a single digit, and your money is gone forever. Mnemonic Words are a much more user-friendly way of representing the same data, but because they're words in the language of the user's choice, they're less prone to mistakes. The power of BIP39 is thus to improve the accessibility, usability, and safety of Bitcoin.

## What's Next?

Learn more about "Programming Bitcoin with Libwally" in [16.3: Using BIP32 in Libwally](16_3_Using_BIP39_in_Libwally.md).
