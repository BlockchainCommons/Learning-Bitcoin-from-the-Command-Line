# 4.6: Creating a SegWit Transaction

> :information_source: **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Once upon a time, the Bitcoin heavens shook with the blocksize wars. Users were worried about scaling, fees were skyrocketing. The Bitcoin Core developers were reluctant to simply increase the blocksize, so they arrived upon another solution: SegWit, the Segregated Witness. Segregated Witness is a fancy way of saying "Separated Signature". It creates new sorts of addresses that remove signatures to the end of the transaction. By combining this with increased block sizes that only are visible to upgraded nodes, SegWit resolved the scaling problems for Bitcoin at the time (and also resolved a nasty malleability bug that made improved scaling with layer-2 protocols like Lightning unfeasible). 

The catch? SegWit uses different addresses, some of which are compatible with older nodes, of some of which are not.

> :warning: **VERSION WARNING:** SegWit was introduced in BitCoin 0.16.0 with what was described at the time as "full support". With that said, there were some flaws in its integration with `bitcoin-cli` which prevented signing from working correctly on new P2SH-SegWit addresses. The non-backward-compatible Bech32 address was also introduced in Bitcoin 0.16.0 and made the default addresstype in Bitcoin 0.19.0. All of this functionality should now fully work with regard to `bitcoin-cli` functions (and thus this tutorial). The catch comes in interacting with the wider world. Everyone should be able to send to a P2SH-SegWit address, because it was purposefully built to support backward compatibility by wrapping the SegWit functionality in a Bitcoin Script. The same isn't true for Bech32 addresses: if someone tells you that they're unable to send to your Bech32 address, this is why, and you need to generate a `legacy` or P2SH-SegWit address for their usage. (Many sites, particularly exchanges, can also not generate or receive on SegWit addresses, particularly Bech32 addresses, but that's a whole different issue and doesn't affect your usage of them).

## Understand a SegWit Transaction

In classic transactions, signature (witness) information was stored toward the middle of the transaction, while in SegWit transactions, it's at the bottom. This goes hand-in-hand with the blocksize increases that were introduced in the SegWit upgrade. The blocksize was increased from 1M to a variable amount based on how many SegWit transactions are in a block, starting as low as 1M (no SegWit transactions) and going as high as 4M (all SegWit transactions). This variable sized was created to accomodate classic nodes, so that everything remains backward compatible. If a classic node sees a SegWit tranaction, it throws out the witness information (resulting in a smaller sized block, under the old 1M limit), while if a new node sees a SegWit transaction, it keeps the witness information (resulting in a larger sizde block, up to the new 4M limit).

So that's the what and how og SegWit transactions. Not that you need to know any of it to use them. Most transactions on the BitCoin network are now SegWit. They're what you're going to natively use for more transactions and receipts of money. The details are no more relevant at this point than the details of how most of Bitcoin works.

## Create a SegWit Address

You create a SegWit address the same way as any other address, with the `getnewaddress` and the `getrawchangeaddress` commands.

If you need to create an address for someone who can't send to the newer Bech32 addresses, then use the `p2sh-segwit` addresstype:
```
$ bitcoin-cli getnewaddress -addresstype p2sh-segwit
2N5h2r4karVqN7uFtpcn8xnA3t5cbpszgyN
```
Seeing an address with a "2" prefix means that you did it right.

> :link: **TESTNET vs MAINNET:** "3" for Mainnet.

However, if the person you're interacting with has a fully mature client, they'll be able to send to a Bech32 address, which you create using the commands in the default way:
```
$ bitcoin-cli getnewaddress
tb1q5gnwrh7ss5mmqt0qfan85jdagmumnatcscwpk6
```

As we've already seen, change addresses generated from within `bitcoin-cli` interact fine with Bech32 addresses, so there's no point in using the `legacy` flag there either:
```
$ bitcoin-cli getrawchangeaddress
tb1q05wx5tyadm8qe83exdqdyqvqqzjt3m38vfu8ff
```

There are 2 different Segwit address formats. They are P2SH (starting with a "3") and bech32 (starting "bc1")
P2SH can be sent to by people using older Bitcoin software with no Segwit support. This supports backwards compatibility
People sending from newer Bitcoin software that has Segwit and bech32 can send to the new address type that starts "bc1"
People sending from older Bitcoin software that has no Segwit probably cannot send to the new address type that starts "bc1"

So, if people send you BTC from old software, give them the Segwit addresses that start with "3". If people send you BTC using new Segwit Bitcoin software, give them the Segwit addresses that start with "bc1".

```
$ bitcoin-cli getnewaddress
tb1q5gnwrh7ss5mmqt0qfan85jdagmumnatcscwpk6
```
And Bitcoin-cli doesn't care. You can run a command like `listaddressgroupings` and it will freely mix addresses of the different types:
```
$ bitcoin-cli listaddressgroupings
[
  [
    [
      "mfsiRhxbQxcD7HLS4PiAim99oeGyb9QY7m",
      0.01000000,
      ""
    ]
  ],
  [
    [
      "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
      0.00000000,
      ""
    ],
    [
      "tb1q6dak4e9fz77vsulk89t5z92l2e0zm37yvre4gt",
      0.00000000
    ]
  ],
  [
    [
      "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
      0.00022000,
      ""
    ]
  ],
  [
    [
      "mk9ry5VVy8mrA8SygxSQQUDNSSXyGFot6h",
      0.00000000
    ],
    [
      "mqjrdY5raxKzXQf5t2VvVvzhvFAgersu9B",
      0.00000000
    ],
    [
      "mwJL7cRiW2bUnY81r1thSu3D4jtMmwyU6d",
      0.00000000,
      ""
    ],
    [
      "tb1q57p0f3hpuad9kf8n6e6adugmt6lnkg2zzr592r",
      0.00007800
    ]
  ],
  [
    [
      "mpVLL7iqPr4d7BJkEG54mcdm7WmrAhaW6q",
      0.01000000,
      ""
    ]
  ],
  [
    [
      "tb1q5gnwrh7ss5mmqt0qfan85jdagmumnatcscwpk6",
      0.01000000,
      ""
    ]
  ]
]
```

## Send a SegWit Transaction

So how do you send a Bitcoin transaction? Exactly like any other transaction. It doesn't matter if the UTXO is SegWit, the address is SegWit, or some combination thereof. You can expect `bitcoin-cli` to do the right thing. Though you can tell the differences via the addresses, they don't matter. (And this is one of the advantages of using the command line and the RPC interface, as suggested in this tutorial: because experts have already done the hard work for you, including things how to send to both legacy and Bech32 addresses. You just get to use that functionality to your own advantage.)

Here's an example of sending to a SegWit address, the easy way:
```
$ bitcoin-cli sendtoaddress address=tb1qw508d6qejxtdg4y5r3zarvary0c5xw7kxpjzsx 0.005
854a833b667049ac811b4cf1cad40fa7f8dce8b0f4c1018a58b84559b6e05f42
```
If you look at your transaction, you can see use of the Bech32 address:
```
$ bitcoin-cli gettransaction txid="854a833b667049ac811b4cf1cad40fa7f8dce8b0f4c1018a58b84559b6e05f42" verbose=true
{
  "amount": -0.00500000,
  "fee": -0.00036600,
  "confirmations": 0,
  "trusted": true,
  "txid": "854a833b667049ac811b4cf1cad40fa7f8dce8b0f4c1018a58b84559b6e05f42",
  "walletconflicts": [
  ],
  "time": 1592948795,
  "timereceived": 1592948795,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "tb1qw508d6qejxtdg4y5r3zarvary0c5xw7kxpjzsx",
      "category": "send",
      "amount": -0.00500000,
      "vout": 1,
      "fee": -0.00036600,
      "abandoned": false
    }
  ],
  "hex": "0200000002114d5a4c3b847bc796b2dc166ca7120607b874aa6904d4a43dd5f9e0ea79d4ba010000006a47304402200a3cc08b9778e7b616340d4cf7841180321d2fa019e43f25e7f710d9a628b55c02200541fc200a07f2eb073ad8554357777d5f1364c5a96afe5e77c6185d66a40fa7012103ee18c598bafc5fbea72d345329803a40ebfcf34014d0e96aac4f504d54e7042dfeffffffa71321e81ef039af490251379143f7247ad91613c26c8f3e3404184218361733000000006a47304402200dd80206b57beb5fa38a3c3578f4b0e40d56d4079116fd2a6fe28e5b8ece72310220298a8c3a1193ea805b27608ff67a2d8b01e347e33a4222edfba499bb1b64a31601210339c001b00dd607eeafd4c117cfcf86be8efbb0ca0a33700cffc0ae0c6ee69d7efeffffff026854160000000000160014d591091b8074a2375ed9985a9c4b18efecfd416520a1070000000000160014751e76e8199196d454941c45d1b3a323f1433bd6c60e1b00",
  "decoded": {
    "txid": "854a833b667049ac811b4cf1cad40fa7f8dce8b0f4c1018a58b84559b6e05f42",
    "hash": "854a833b667049ac811b4cf1cad40fa7f8dce8b0f4c1018a58b84559b6e05f42",
    "version": 2,
    "size": 366,
    "vsize": 366,
    "weight": 1464,
    "locktime": 1773254,
    "vin": [
      {
        "txid": "bad479eae0f9d53da4d40469aa74b8070612a76c16dcb296c77b843b4c5a4d11",
        "vout": 1,
        "scriptSig": {
          "asm": "304402200a3cc08b9778e7b616340d4cf7841180321d2fa019e43f25e7f710d9a628b55c02200541fc200a07f2eb073ad8554357777d5f1364c5a96afe5e77c6185d66a40fa7[ALL] 03ee18c598bafc5fbea72d345329803a40ebfcf34014d0e96aac4f504d54e7042d",
          "hex": "47304402200a3cc08b9778e7b616340d4cf7841180321d2fa019e43f25e7f710d9a628b55c02200541fc200a07f2eb073ad8554357777d5f1364c5a96afe5e77c6185d66a40fa7012103ee18c598bafc5fbea72d345329803a40ebfcf34014d0e96aac4f504d54e7042d"
        },
        "sequence": 4294967294
      },
      {
        "txid": "33173618421804343e8f6cc21316d97a24f7439137510249af39f01ee82113a7",
        "vout": 0,
        "scriptSig": {
          "asm": "304402200dd80206b57beb5fa38a3c3578f4b0e40d56d4079116fd2a6fe28e5b8ece72310220298a8c3a1193ea805b27608ff67a2d8b01e347e33a4222edfba499bb1b64a316[ALL] 0339c001b00dd607eeafd4c117cfcf86be8efbb0ca0a33700cffc0ae0c6ee69d7e",
          "hex": "47304402200dd80206b57beb5fa38a3c3578f4b0e40d56d4079116fd2a6fe28e5b8ece72310220298a8c3a1193ea805b27608ff67a2d8b01e347e33a4222edfba499bb1b64a31601210339c001b00dd607eeafd4c117cfcf86be8efbb0ca0a33700cffc0ae0c6ee69d7e"
        },
        "sequence": 4294967294
      }
    ],
    "vout": [
      {
        "value": 0.01463400,
        "n": 0,
        "scriptPubKey": {
          "asm": "0 d591091b8074a2375ed9985a9c4b18efecfd4165",
          "hex": "0014d591091b8074a2375ed9985a9c4b18efecfd4165",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1q6kgsjxuqwj3rwhkenpdfcjccalk06st9z0k0kh"
          ]
        }
      },
      {
        "value": 0.00500000,
        "n": 1,
        "scriptPubKey": {
          "asm": "0 751e76e8199196d454941c45d1b3a323f1433bd6",
          "hex": "0014751e76e8199196d454941c45d1b3a323f1433bd6",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1qw508d6qejxtdg4y5r3zarvary0c5xw7kxpjzsx"
          ]
        }
      }
    ]
  }
}
```
In fact, both of the `vouts` use Bech32 addresses.

But when we backtrack our `vin`, we discover that came from a legacy address. Because it doesn't matter:
```
$ bitcoin-cli -named gettransaction txid="33173618421804343e8f6cc21316d97a24f7439137510249af39f01ee82113a7"
{
  "amount": 0.01000000,
  "confirmations": 43,
  "blockhash": "00000000000000e2365d2f814d1774b063d9a04356f482010cdfdd537b1a24bb",
  "blockheight": 1773212,
  "blockindex": 103,
  "blocktime": 1592937103,
  "txid": "33173618421804343e8f6cc21316d97a24f7439137510249af39f01ee82113a7",
  "walletconflicts": [
  ],
  "time": 1592936845,
  "timereceived": 1592936845,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "mpVLL7iqPr4d7BJkEG54mcdm7WmrAhaW6q",
      "category": "receive",
      "amount": 0.01000000,
      "label": "",
      "vout": 0
    }
  ],
  "hex": "020000000001016a66efa334f06e2c54963e48d049a35d7a1bda44633b7464621cae302f35174a0100000017160014f17b16c6404e85165af6f123173e0705ba31ec25feffffff0240420f00000000001976a914626ab1ca41d98f597d18d1ff8151e31a40d4967288acd2125d000000000017a914d5e76abfe5362704ff6bbb000db9cdfa43cd2881870247304402203b3ba83f51c1895b5f639e9bfc40124715e2495ef2c79d4e49c0f8f70fbf2feb02203d50710abe3cf37df4d2a73680dadf3cecbe4f2b5d0b276dbe7711d0c2fa971a012102e64f83ee1c6548bcf44cb965ffdb803f30224459bd2e57a5df97cb41ba476b119b0e1b00"
}
```
You can similarly fund a transaction with a Bech32 address with no difference to the techniques you've learned so far:
```
$ changeaddress=$(bitcoin-cli getrawchangeaddress)
$ echo $changeaddress
tb1q4xje3mx9xn7f8khv7p69ekfn0q72kfs8x3ay4j
$ bitcoin-cli listunspent
[
...
  {
    "txid": "003bfdca5578c0045a76768281f05d5e6f57774be399a76f387e2a0e99e4e452",
    "vout": 0,
    "address": "tb1q5gnwrh7ss5mmqt0qfan85jdagmumnatcscwpk6",
    "label": "",
    "scriptPubKey": "0014a226e1dfd08537b02de04f667a49bd46f9b9f578",
    "amount": 0.01000000,
    "confirmations": 5,
    "spendable": true,
    "solvable": true,
    "desc": "wpkh([d6043800/0'/0'/5']0327dbe2d58d9ed2dbeca28cd26e18f48aa94c127fa6fb4b60e4188f6360317640)#hd66hknp",
    "safe": true
  }
]
$ recipient=tb1qw508d6qejxtdg4y5r3zarvary0c5xw7kxpjzsx
$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[2] | .txid')
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[2] | .vout')
$ echo $utxo_txid $utxo_vout
003bfdca5578c0045a76768281f05d5e6f57774be399a76f387e2a0e99e4e452 0
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.002, "'$changeaddress'": 0.007 }''')
$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r '.hex')
standup@btctest20:~$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
e02568b706b21bcb56fcf9c4bb7ba63fdbdec1cf2866168c4f50bc0ad693f26c
```
It all works exactly the same as other sorts of transactions!

## Summary: Creating a SegWit Transaction

There's really no complexity to creating SegWit transactions. Internally, they're structured differently from legacy transactions, but from the command line there's no difference: you just use an address with a different prefix. The only thing to watch for is that some people may not be able to send to a Bech32 address, if they're using obsolete software.

> :fire: ***What the power of sending coins with SegWit?***

> _The Advantages._ SegWit transactions are smaller, and so will be cheaper to send than legacy transactions due to lower fees. Bech32 doubles down on this advantage, and also creates addresses that are hard to make mistakes when transcribing — and that's pretty important, given that user error is one of the most likely ways to lose your Bitcoins.

> _The Disadvantages._ SegWit transactions may not be supported by obsolete Bitcoin software. In particular, people may not be able to send to your Bech32 address.

## What's Next?

Advance through "bitcoin-cli" with [Chapter Five: Controlling Bitcoin Transactions](05_0_Controlling_Bitcoin_Transactions.md).
