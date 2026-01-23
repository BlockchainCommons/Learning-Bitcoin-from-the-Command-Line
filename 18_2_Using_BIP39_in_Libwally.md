# 17.2: Using BIP39 in Libwally

> :information_source: **NOTE:** This section has been recently added to the course and is an early draft that may still be awaiting review. Caveat reader.

One of Libwally's greatest powers is that it can lay bare the underlying work of generating seeds, private keys, and ultimately addresses. To start with, it supports [BIP39](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki), which is the BIP that defines mnemonic codes for Bitcoin — something that's entirely unsupported, to date, by Bitcoin Core.

> :book: ***What is a Mnemonic Code?*** Bitcoin addresses (and their corresponding private keys and underlying seeds) are long, unintelligible lists of characters and numbers, which are not only impossible to remember, but also easy to mistype. Mnemonic codes are a solution for this that allow users to record 12 (or 24) words in their language — something that's much less prone to mistakes. These codes can then be used to fully restore a BIP32 seed that's the basis of an HD wallet.

> :book: ***What is a Seed?*** We briefly touched on seeds in [§3.5: Understanding the Descriptor](03_5_Understanding_the_Descriptor.md). It's the random number that's used to generate a whole sequence of private keys (and thus addresses) in an HD wallet. We'll return to seeds in the next section, which is all about HD wallets and Libwally. For now, just know that a BIP39 mnemonic code corresponds to the seed for a BIP32 hierarchical deterministic wallet.

## Create Mnemonic Codes

All Bitcoin keys start with entropy. This first use of Libwally, and its BIP39 mnemonics, thus shows how to generate entropy and to get a mnemonic code from that.

> :book: ***What is Entropy?*** Entropy is a fancy way of saying randomness, but it's a carefully measured randomness that's used as the foundation of a true-random-number generated (TRG). Its measured in "bits", with more bits of entropy resulting in more randomness (and thus more protection for what's being generated). For Bitcoin, entropy is the foundation of your seed, which in an HD wallet generates all of your addresses.

You'll always start work with Libwally by initializing the library and testing the results, as first demonstrated in [§17.1](17_1_Setting_Up_Libwally.md):
```
  int lw_response;

  lw_response = wally_init(0);

  if (lw_response) {

    printf("Error: Wally_init failed: %d\n",lw_response);
    exit(-1);
    
  }
```
Now you're ready to entropize.

### Create Entropy

Using `libsodium`, you can create entropy with the `randombytes_buf` command:
```
  unsigned char entropy[16];  
  randombytes_buf(entropy, 16);
```
This example, which will be the only way we use the `libsodium` library, creates 16 bytes of entropy. Generally, to create a secure mnemonic code, you should use between 128 and 256 bits of entropy, which is 16 to 32 bytes.

>:warning: **WARNING:** Again, be very certain that you're very comfortable with your method for entropy generation before you use it in a real-world program.

### Translate into a Mnemonic

16 bytes of entropy is sufficient to create a 12-character Mnemonic code, which is done with Libwally's `bip39_mnemonic_from_bytes` function:
```
  char *mnem = NULL;
  lw_response = bip39_mnemonic_from_bytes(NULL,entropy,16,&mnem);
```
Note that you have to pass along the byte size, so if you were to increase the size of your entropy, to generate a longer mnemonic phrase, you'd also need to increase the value in this function.

> **NOTE:** There are mnemonic word lists for different languages! The default is to use the English-language list, which is the `NULL` variable in these Libwally mnemonic commands, but you can alternatively request a different language!

That's it! You've created a mnemonic phrase!

>:book: ***How is the Mnemonic Phrase Created?*** You can learn about that in [BIP39](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki), but if you prefer, Greg Walker has an [excellent example](https://learnmeabitcoin.com/technical/mnemonic): basically, you add a checksum, then you covert each set of 11 bits into a word from the word list. You can do this with the commands `bip39_get_wordlist` and `bip39_get_word` if you don't trust the `bip39_mnemonic_from_bytes` command.

### Translate into a Seed

There are some functions, such as `bip32_key_from_seed` (which we'll meet in the next section) that require you to have the seed rather than the Mnemonic. The two things are functionally identical: if you have the seed, you can generate the mnemonic, and vice-versa.

If you need to generate the seed from your mnemonic, you just use the `bip39_mnemonic_to_seed` command:
```
  unsigned char seed[BIP39_SEED_LEN_512];
  size_t seed_len;
  
  lw_response = bip39_mnemonic_to_seed(mnem,NULL,seed,BIP39_SEED_LEN_512,&seed_len);
```
Note that all BIP39 seeds are current 512 bytes; nonetheless you have to set the size of your variable appropriately, and pass along that size to `bip39_mnemonic_to_seed`.

### Print Your Seed

If you want to see what your seed looks like in hex, you can use the `wally_hex_from_bytes` function to turn your seed into a readable (but not-great-for-people) hex code:
```
  char *seed_hex;
  wally_hex_from_bytes(seed,sizeof(seed),&seed_hex);
  printf("Seed: %s\n",seed_hex);
```
If you've done everything right, you should get back a 64-byte seed. (That's the `BIP39_SEED_LEN_512` variable you've been throwing around, which defines a default seed length as 512 bits or 64 bytes.)

> :warning: **WARNING:** You definitely should test that your seed length is 64 bytes in some way, because it's easy to mess up, for example by using the wrong variable type when you run `bip39_mnemonic_to_seed`.

## Test Mnemonic Code

The full code for generating entropy, generating a BIP39 mnemonic, validating the mnemonic, and generating a seed can be found in the [src directory](src/17_2_genmnemonic.c). Download it and compile:
```
$ cc genmnemonic.c -lwallycore -lsodium -o genmnemonic
```
Then you can run the test:
```
Mnemonic: parent wasp flight sweet miracle inject lemon matter label column canyon trend
Mnemonic validated!
Seed: 47b04cfb5d8fd43d371497f8555a27a25ca0a04aafeb6859dd4cbf37f6664b0600c4685c1efac29c082b1df29081f7a46f94a26f618fc6fd38d8bc7b6cd344c7
```

## Summary: Using BIP39 in Libwally

BIP39 allows you generate a set of 12-24 Mnemonic words from a seed (and the Libwally library also allows you to validate it!).

> :fire: ***What is the power of BIP39?*** Bitcoin seeds and private keys are prone to all sorts of lossage. You mistype a single digit, and your money is gone forever. Mnemonic Words are a much more user-friendly way of representing the same data, but because they're words in the language of the user's choice, they're less prone to mistakes. The power of BIP39 is thus to improve the accessibility, usability, and safety of Bitcoin.

> :fire: ***What is the power of BIP39 in Libwally?*** Bitcoind doesn't currently support mnemonic words, so using Libwally can allow you to generate mnemonic words in conjunction with addresses held by `bitcoind` (though as we'll see in §17.7, it requires a bit of a work-around at present to import your keys into Bitcoin Core).

## What's Next?

Learn more about "Programming Bitcoin with Libwally" in [17.3: Using BIP32 in Libwally](17_3_Using_BIP32_in_Libwally.md).
