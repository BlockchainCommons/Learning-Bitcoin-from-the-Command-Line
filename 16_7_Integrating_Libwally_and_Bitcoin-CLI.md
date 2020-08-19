# 16.7: Integrate Libwally and Bitcoin-CLI

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Libwally is ultimately limited. It's about creating and accessing wallets, with some additional functions related to transactions and PSBTs that might be useful for services that are not connected to full nodes on the internet. Ultimately, however, you're going to need full node services to take advantage of Libwally.

This final section will offer some examples of using Libwally programs to complement a `bitcoin-cli` environment. Though we will suggest that these services are all on the same machine, they may become even more powerful if the `bitcoin-cli` service is directly connected to the internet and the Libwally service is not.

## Creating Recovery Words

One of the big limitations of Bitcoin Core is that it creates a BIP32 HD wallet, but it doesn't provide any way to back that up with BIP39 mnemonic words. You can now do that yourself.

You can dump your wallet with the `dumpwallet` RPC command:
```
$ bitcoin-cli dumpwallet seed
{
  "filename": "/home/standup/.bitcoin/seed"
}
```
If you read the file you created, you should see your seed toward the start:
```
# Wallet dump created by Bitcoin v0.20.0
# * Created on 2020-08-19T01:26:20Z
# * Best block at time of backup was 1807738 (00000000bf0ec16a551727230a56ba5c37aae9158114e6f906c1df9655bf97ed),
#   mined on 2020-08-19T01:18:26Z

# extended private masterkey: tprv8ZgxMBicQKsPf1YVXBNApPnJZSt7hYkvBvkzKygbKSF7Bbnvha1ZwRnzJRCXyXkzSkJpggPcvPBroUmhFKYM8F4cxJCjFnmE2MTqrH8ywYf
```
(Under that will be keys for all of your addresses.)

The following command will currently work to extract that seed, though this type of command depending on file formatting is always prone to breaking as file formats change:
```
$ seed=$(more ~/.bitcoin/seed  | grep "extended" | awk -F': ' '{print $2}')
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
