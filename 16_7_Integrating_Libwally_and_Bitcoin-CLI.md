# 16.7: Integrate Libwally and Bitcoin-CLI

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Libwally is limited. It's about creating and accessing wallets, with some additional functions related to transactions and PSBTs that might be useful for services that are not connected to full nodes on the internet. Ultimately, however, you're going to need full node services to take advantage of Libwally.

This final section will offer some examples of using Libwally programs to complement a `bitcoin-cli` environment. Though these examples imply that these services are all on the same machine, they may become even more powerful if the `bitcoin-cli` service is directly connected to the internet and the Libwally service is not.

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

## Importing Addresses

## Passing Around a Transaction

[per 16.5]

[also mention PSBTs]
