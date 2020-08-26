# 16.7: Integrate Libwally and Bitcoin-CLI

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Libwally is limited. It's about creating and accessing wallets, with some additional functions related to transactions and PSBTs that might be useful for services that are not connected to full nodes on the internet. Ultimately, however, you're going to need full node services to take advantage of Libwally.

This final section will offer some examples of using Libwally programs to complement a `bitcoin-cli` environment. Though these examples imply that these services are all on the same machine, they may become even more powerful if the `bitcoin-cli` service is directly connected to the internet and the Libwally service is not.

NEW OUTLINE:

1. Sharing a Transaction
   * Sharing a PSBT
2. Importing & Exporting BIP39 Seeds
   * What It Doesn't Work
3. Importing & Exporting Private Keys
   * What It Doesn't Work
4. Importing an Address

## Importing & Experting BIP39 Seeds

It would be nice if you could either export an HD seed from `bitcoin-cli`, to generate the mnemonic phrase with Libwally, or generate a seed from a mneomnic phrase on Libwally, and then import it into `bitcoin-cli`. Unfortunately, neither of these is possible at this time. A seed is mneomnic phrase is translated into a seed using HMAC-SHA512, which means the result in 512 bits. However, `bitcoin-cli` exports HD seeds (using `dumpwallet`) and imports HD seeds (using `sethdseed`) with a length of 256 bits. Until that is changed, never the twain shall meet (and when that does change, there will be another notable improvement in the integration between `bitcoin-cli` and Libwally).

> :book: ***What's the Difference Between Entropy & a Seed?*** Libwally says that it creates its mnemonic phrases from entropy. That's essentially the same thing as a seed: they're both large, randomized numbers. So, if `bitcoin-cli` was compatible with 512-bit mnemonic-phrase seeds, you could use one to generate the mneomnic phrases, and get the results that you'd expect.

> :book: ***What's the difference between Entropy & Raw Entropy?*** However, not all entropy is the same. When you input entropy into a command that creates a mnemonic seed, it has to a specific, well-understand length. Changing raw entropy into entropy requires massaging the raw entropy until it's the right length and format, and at that point you could reuse the entropy to always recreate the same mnemonics (which is why it's effectively the same thing as a seed at that point, but raw entropy isn't).


## Importing Addresses


## Passing Around a Transaction

[per 16.5]

[also mention PSBTs]

[This is backburnered for the moment]

## Creating Recovery Words

One of the big limitations of Bitcoin Core is that it creates a BIP32 HD wallet, but it doesn't provide any way to back that up with BIP39 mnemonic words. With Libwally, you can now do that yourself.

You can dump your wallet with the `dumpwallet` RPC command:
```
$ bitcoin-cli dumpwallet seed
{
  "filename": "/home/standup/.bitcoin/seed"
}
```
If you read the file you created, you should see your seed with the line `hdseed=1`
```
standup@btctest:~/.bitcoin$ more seed | grep hdseed
cV2ofwMK2EWH7PduPGTU3mKkKsQRhAddWNzMLHqVgnvD8RgkHE97 2020-08-04T19:04:02Z hdseed=1 # addr=tb1qtuk0khv6qmwq6xl0llk9r8ht35z3kkk6qsaazw
```

The following command will currently work to extract that seed, though this type of command depending on file formatting is always prone to breaking as file formats change:
```
seed=$(cat seed | grep hdseed=1 | awk '{print $1}')
```
Now, you just need to import that into a simple Libwally program.

[write the libwally program]
   * bip32_key_serialize
   * bip39_mnemonic_from_bytes
[write a script that (1) dumps; (2) extracts the key; (3) runs it through libwally; (4) outputs the mnemonic words]
