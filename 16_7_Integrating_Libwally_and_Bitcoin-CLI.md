# 16.7: Integrate Libwally and Bitcoin-CLI

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Libwally is limited. It's about creating and accessing wallets, with some additional functions related to transactions and PSBTs that might be useful for services that are not connected to full nodes on the internet. Ultimately, however, you're going to need full node services to take advantage of Libwally.

This final section will offer some examples of using Libwally programs to complement a `bitcoin-cli` environment. Though these examples imply that these services are all on the same machine, they may become even more powerful if the `bitcoin-cli` service is directly connected to the internet and the Libwally service is not.

## Sharing a Transaction

[§16.5: Using Scripts in Libwally](16_5_Using_Scripts_in_Libwally.md) detailed how Libwally could be used to rewrite an existing transaction, to do something that `bitcoin-cli` can't: produce a transaction that contains a unique P2SH. Obviously, this is a building block; if you dig further into Libwally you'll want to create entire transactions on your own. But, this abbreviated methodology also has its own usage: it shows how transactions can be passed back and forth between `bitcoin-cli` and Libwally, demonstrating a first example of using them in a complementary fashion.

We're going to start out with this UTXO:
```
  {
    "txid": "c0a110a7a84399b98052c6545018873b13ee3128fa74f7a697779174a36ea33a",
    "vout": 1,
    "address": "mvLyH7Rs45c16FG2dfV7uuTKV6pL92kWxo",
    "label": "",
    "scriptPubKey": "76a914a2a68c5f9b8e25fdd1213c38d952ab2be2e271be88ac",
    "amount": 0.00094000,
    "confirmations": 17375,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([ce0c7e14/0'/0'/5']0368d0fffa651783524f8b934d24d03b32bf8ff2c0808943a556b3d74b2e5c7d65)#qldtsl65",
    "safe": true
  }
```
By now, you know how to set up a transaction with `bitcoin-cli`:
```
$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[0] | .txid') 
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
$ recipient=tb1qycsmq3jas5wkhf8xrfn8k7438cm5pc8h9ae2k0
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.0009 }''')
```
The output doesn't matter, because that's what we're going to be rewriting.

We also slightly vary our old `replacewithscript` code to take a satoshi argument. A fancier bit of code could read the existing `vout` info before rewriting, but we're keeping things very close to our [original code](src/16_5_replacewithscript.c).
```
...
  int satoshis = atoi(argv[3]);
...
  lw_response = wally_tx_output_init_alloc(satoshis,p2sh,sizeof(p2sh),&tx_output);
...
```
Then we just run things like before:
```
$ newtxhex=$(./replacewithscript $rawtxhex $script 9000)
```
Here's what the original transaction looked like:
```
$ bitcoin-cli decoderawtransaction $rawtxhex
{
  "txid": "438d50edd7abeaf656c5abe856a00a20af5ff08939df8fdb9f8bfbfb96234fcb",
  "hash": "438d50edd7abeaf656c5abe856a00a20af5ff08939df8fdb9f8bfbfb96234fcb",
  "version": 2,
  "size": 82,
  "vsize": 82,
  "weight": 328,
  "locktime": 0,
  "vin": [
    {
      "txid": "c0a110a7a84399b98052c6545018873b13ee3128fa74f7a697779174a36ea33a",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 0.00090000,
      "n": 0,
      "scriptPubKey": {
        "asm": "0 2621b0465d851d6ba4e61a667b7ab13e3740e0f7",
        "hex": "00142621b0465d851d6ba4e61a667b7ab13e3740e0f7",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1qycsmq3jas5wkhf8xrfn8k7438cm5pc8h9ae2k0"
        ]
      }
    }
  ]
}
```
And here's the transaction rewritten by Libwally to use a P2SH:
```
standup@btctest:~/c$ bitcoin-cli decoderawtransaction $newtxhex
{
  "txid": "badb57622ab5fe029fc1a71ace9f7b76c695f933bceb0d38a155c2e5c984f4e9",
  "hash": "badb57622ab5fe029fc1a71ace9f7b76c695f933bceb0d38a155c2e5c984f4e9",
  "version": 2,
  "size": 83,
  "vsize": 83,
  "weight": 332,
  "locktime": 0,
  "vin": [
    {
      "txid": "c0a110a7a84399b98052c6545018873b13ee3128fa74f7a697779174a36ea33a",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 0
    }
  ],
  "vout": [
    {
      "value": 0.00090000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_HASH160 3f58b4f7b14847a9083694b9b3b52a4cea2569ed OP_EQUAL",
        "hex": "a9143f58b4f7b14847a9083694b9b3b52a4cea2569ed87",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2My2ApqGcoNXYceZC4d7fipBu4GodkbefHD"
        ]
      }
    }
  ]
}
```
Afterward you can sign it as usual with `bitcoin-cli`:
```
$ signedtx=$(bitcoin-cli signrawtransactionwithwallet $newtxhex | jq -r '.hex')
```
And as you can see, the result is a legitimate transaction ready to go out to the Bitcoin network:
```
$ bitcoin-cli decoderawtransaction $signedtx
{
  "txid": "3061ca8c01d029c0086adbf8b7d4280280c8aee151500bab7c4f783bbc8e75e6",
  "hash": "3061ca8c01d029c0086adbf8b7d4280280c8aee151500bab7c4f783bbc8e75e6",
  "version": 2,
  "size": 189,
  "vsize": 189,
  "weight": 756,
  "locktime": 0,
  "vin": [
    {
      "txid": "c0a110a7a84399b98052c6545018873b13ee3128fa74f7a697779174a36ea33a",
      "vout": 1,
      "scriptSig": {
        "asm": "3044022026c81b6ff4a15135d10c7f4b1ae6e44ac4fdb25c4a3c03161b17b8ab8d04850502200b448d070f418de1ca07e76943d23d447bc95c7c5e0322bcc153cadb5d9befe0[ALL] 0368d0fffa651783524f8b934d24d03b32bf8ff2c0808943a556b3d74b2e5c7d65",
        "hex": "473044022026c81b6ff4a15135d10c7f4b1ae6e44ac4fdb25c4a3c03161b17b8ab8d04850502200b448d070f418de1ca07e76943d23d447bc95c7c5e0322bcc153cadb5d9befe001210368d0fffa651783524f8b934d24d03b32bf8ff2c0808943a556b3d74b2e5c7d65"
      },
      "sequence": 0
    }
  ],
  "vout": [
    {
      "value": 0.00090000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_HASH160 3f58b4f7b14847a9083694b9b3b52a4cea2569ed OP_EQUAL",
        "hex": "a9143f58b4f7b14847a9083694b9b3b52a4cea2569ed87",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2My2ApqGcoNXYceZC4d7fipBu4GodkbefHD"
        ]
      }
    }
  ]
}
```
Voila! That's the power of Libwally with `bitcoin-cli`.

Obviously, you can also pass around a PSBT using the functions described in [§16.4](16_4_Using_PSBTs_in_Libwally.md) and that's a more sustainable methodlogy in the modern-day usage of Bitcoin, but in either example, the methodology of passing transactions from `bitcoin-cli` to Libwally code and back should be similarly.

## Importing & Exporting BIP39 Seeds

Unfortunately, not all interactions go as smoothly. For example, it would be nice if you could either export an HD seed from `bitcoin-cli`, to generate the mnemonic phrase with Libwally, or to generate a seed from a mneomnic phrase on Libwally, and then import it into `bitcoin-cli`. Unfortunately, neither of these is possible at this time. A mneomnic phrase is translated into a seed using HMAC-SHA512, which means the result is 512 bits. However, `bitcoin-cli` exports HD seeds (using `dumpwallet`) and imports HD seeds (using `sethdseed`) with a length of 256 bits. Until that is changed, never the twain shall meet (and when that does change, there will be another notable improvement in the integration between `bitcoin-cli` and Libwally).

> :book: ***What's the Difference Between Entropy & a Seed?*** Libwally says that it creates its mnemonic phrases from entropy. That's essentially the same thing as a seed: they're both large, randomized numbers. So, if `bitcoin-cli` was compatible with 512-bit mnemonic-phrase seeds, you could use one to generate the mneomnic phrases, and get the results that you'd expect.

> :book: ***What's the difference between Entropy & Raw Entropy?*** Not all entropy is the same. When you input entropy into a command that creates a mnemonic seed, it has to a specific, well-understand length. Changing raw entropy into entropy requires massaging the raw entropy until it's the right length and format, and at that point you could reuse that (non-raw) entropy to always recreate the same mnemonics (which is why it's effectively the same thing as a seed at that point, but raw entropy isn't).

# Importing Private Keys

Fortunately, you can do much the same thing by importing a private key generated in Libwally. Take a look at [genhd-for-import.c](src/16_7_genhd_for_import.c), a simplified version of the `genhd` program from [§16.3](16_3_Using_BIP32_in_Libwally.md) that also uses the `jansson` library from [§15.1](15_1_Accessing_Bitcoind_with_C.md).

Be sure to compile the new code with the `jansson` library, and installing it per §15.1.
```
$ cc genhd-for-import.c -lwallycore -lsodium -ljansson -o genhd-for-import
```
When you run the new program, it'll give you a nicely output list of everything:
```
$ ./genhd-for-import 
{
  "mnemonic": "rotate proof kitten age exit master old verb course sea mean address",
  "account-xprv": "tprv8xf6jjUuuKPG4p6erw6TUumrSJPsGH8ZowoAYXFahj3RkZ7za9aW1E2nW2Sj2GsT6JwmPsXaR4jLBZ1XrbMJvNfN6Ruk3tR9bP3hsABFqWD",
  "address": "tb1q88sdux8hvt0jf3xhy0c3l3pgyc34m2kq5kylaa",
  "derivation": "[m/84h/1h/0h/0/0]"
}
```
You have the `mnemonic` that you can recover from, an `account-xprv` that you can import, a `derivation` to use for the import, and a sample `address, that you can use for testing the import.

You can now fall back on lessons learned from [§3.5](03_5_Understanding_the_Descriptor.md) on how to turn that xprv into a descriptor and import it.

First, you need to figure out the checksum:
```
$ xprv=tprv8xf6jjUuuKPG4p6erw6TUumrSJPsGH8ZowoAYXFahj3RkZ7za9aW1E2nW2Sj2GsT6JwmPsXaR4jLBZ1XrbMJvNfN6Ruk3tR9bP3hsABFqWD
$  bitcoin-cli getdescriptorinfo "wpkh([d34db33f/84h/1h/0h]$xprv/0/*)"
{
  "descriptor": "wpkh([d34db33f/84'/1'/0']tpubDVM8t9XA3h4vxH8Skam3tKRy1KuoRcKUPFPwq3Ht7zqpb3NmCYQ6Bieeg8XptKHSnKTWWb2MGdZD1tYAWwKBH6H1wissCE38WHcWJKSrTzJ/0/*)#mvvxgk0j",
  "checksum": "wgknk6u4",
  "isrange": true,
  "issolvable": true,
  "hasprivatekeys": true
}
```
Two things to note here:

1. We made up a fingerprint (`d34db33f`). You can do that, as long as you're consistent (and as long as the original source doesn't have its own fingerprint).
2. We are not going to use the returned `descriptor` line, as that's for a `xpub` address. Instead we'll apply the returned `checksum` to the `xprv` that we already have.

We then plug that into `importmulti` to import this key into `bitcoin-cli`:
```
$ cs=wgknk6u4
$ bitcoin-cli importmulti '''[{ "desc": "wpkh([d34db33f/84h/1h/0h]'$xprv'/0/*)#'$cs'", "timestamp": "now", "range": 10, "watchonly": false, "label": "LibwallyImported", "keypool": false, "rescan": false }]'''
[
  {
    "success": true
  }
]
```
Note here that the descriptor here included the path `/0/*` because we wanted the external addresses for this account. We derived the first ten. If we instead wanted the change addresses, we'd use `/1/*`.

Examining the new `LibwallyImported` label shows ten addresses:
```
$ bitcoin-cli getaddressesbylabel "LibwallyImported"
{
  "tb1qpgt0wukrm8wz6gdu7jc9ltmrxzagdtflg4huvt": {
    "purpose": "receive"
  },
  "tb1qyds9q02a03vveu25gewxxssy2uszm0q2v04hhx": {
    "purpose": "receive"
  },
  "tb1q88sdux8hvt0jf3xhy0c3l3pgyc34m2kq5kylaa": {
    "purpose": "receive"
  },
  "tb1qg9a7xnm9lkketnfvzkcc7gfwvc9lt4dcsp0zrh": {
    "purpose": "receive"
  },
  "tb1qfzr0nqcvs73fhkgy7n8zavjzqxvewfzpsn9zkm": {
    "purpose": "receive"
  },
  "tb1qdlj7n66kzce5sw67twe6zsylcddnn3q7v85uqn": {
    "purpose": "receive"
  },
  "tb1q39dxeawe4w275tpye8p5tt74g965c534ux0yfc": {
    "purpose": "receive"
  },
  "tb1qnq2r00894jxucpnkdl2r72arjvj4th8eeysp7j": {
    "purpose": "receive"
  },
  "tb1q4h399mcz0m7fjz92nhvk5dyww09fpedusr6n7n": {
    "purpose": "receive"
  },
  "tb1q6squxmxqmca9rqqndp6wz5hng8h6pc6ajpgz8j": {
    "purpose": "receive"
  },
  "tb1qu23p72l60tc3jktjfdx0zr6tlqtkjfk9xzkrkl": {
    "purpose": "receive"
  }
}
```
And the third one on our list indeed matches our sample (`tb1q88sdux8hvt0jf3xhy0c3l3pgyc34m2kq5kylaa`). The import of this private key and the derivation of ten addresses was successful.

Now, if you look back at [§7.3](07_3_Integrating_with_Hardware_Wallets.md), you'll see this was the same methodology we used to import addresses from a Hardware Wallet (though this time we also imported the private key as proof of concept). The biggest difference is that before the information was created by a black box (literally, a Ledger device), and this time you created the information yourself using Libwally, showing how you can do this sort of work on airgapped or other more remote devices, then bring it over to `bitcoin-cli`.

## Importing Addresses

3. Importing & Exporting Private Keys
   * What It Doesn't Work
4. Importing an Address


