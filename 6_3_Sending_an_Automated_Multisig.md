 # 6.3: Sending & Spending an Automated Multisig

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

The standard technique for creating multisignature addresses and for spending their funds is complex, but it's a worthwhile exercise for understanding a bit more about how they work, and how you can manipulate them at a relatively low level. However, Bitcoin Core has made multisigs a little bit easier in new releases. 

> **VERSION WARNING:** The `addmultisigaddress` command is available in Bitcoin Core v 0.10 or higher.

## Create a Multisig Address in Your Wallet

In order to make funds sent to multisig addresses easier to spend, you need to have done some prep using the `addmultisigaddress` command. It's probably not what you'd want to do if you were writing multisig wallet programs, but if you were just trying to receive some funds by hand, it might save you some hair-pulling.

### Collect the Keys

You start off creating P2PKH addresses and retrieving public keys as usual, for each user who will be part of the multisig:
```
machine1$ address3=$(bitcoin-cli getnewaddress)
machine1$ echo $address3
mkMkhbUzcSPdEHUoRQkBKHe8otP1SzWWeb
machine1$ bitcoin-cli -named validateaddress address=$address3 | jq -r '. | .pubkey'
02e7356952f4bb1daf475c04b95a2f7e0d9a12cf5b5c48a25b2303783d91849ba4

machine2$ address4=$(bitcoin-cli getnewaddress)
$ echo $address4
mkyeUBPDoeyFrfLE4V5oAQfee99pT2W1E3
$ bitcoin-cli -named validateaddress address=$address4 | jq -r '. | .pubkey'
030186d2b55de166389aefe209f508ce1fbd79966d9ac417adef74b7c1b5e07776
```

### Create the Multisig Address Everywhere

Next you create the multisig on _each machine that contributes signatures_ using a new command, `addmultisigaddress`, instead of `createmultisig`. This new command saves some of the information into your wallet, making it a lot easier to spend the money afterward.
```
machine1$ bitcoin-cli -named addmultisigaddress nrequired=2 keys='''["'$address3'","030186d2b55de166389aefe209f508ce1fbd79966d9ac417adef74b7c1b5e07776"]'''
2Mzw7WBvh9RAQ4ssKqxyNyP7L9NAojLqSW8

machine2$ bitcoin-cli -named addmultisigaddress nrequired=2 keys='''["02e7356952f4bb1daf475c04b95a2f7e0d9a12cf5b5c48a25b2303783d91849ba4","'$address4'"]'''
2Mzw7WBvh9RAQ4ssKqxyNyP7L9NAojLqSW8
```
As noted in the previous section, it doesn't matter whether you use addresses or public keys: you'll get the same multisig address. However, you must use the same order. Thus, it's best for the members of the multisig to check amongst themselves to make sure they all got the same result.

### Watch for Funds

Afterward, the members of the multisig will still need to run `importaddress` to watch for funds received on the multisig address:
```
machine1$ bitcoin-cli -named importaddress address=2Mzw7WBvh9RAQ4ssKqxyNyP7L9NAojLqSW8

machine2$ bitcoin-cli -named importaddress address=2Mzw7WBvh9RAQ4ssKqxyNyP7L9NAojLqSW8
```

## Respend with an Automated Transaction

Afterward, you will be able to receive funds on the multisiganture address as normal. The use of `addmultisigaddress` is simply a bureaucratic issue on the part of the recipients: a bit of bookkeeping to make life easier for them.

But, it makes life a lot easier. Because everything is in the wallet, the signers will be able to respend the funds sent to the multisignature address exactly the same as any other address ... other than the need to sign on multiple machines.

You start by collecting your variables, but you no longer need to worry about `scriptPubKey` or `redeemScript`:
```
machine1$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[1] | .txid') 
machine1$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[1] | .vout') 
machine1$ recipient=$(bitcoin-cli getrawchangeaddress)
```
You create a raw transaction:
```
machine1$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 1.2995}''')
```
Then you sign it:
```
machine1$ bitcoin-cli -named signrawtransaction hexstring=$rawtxhex
{
  "hex": "02000000014ecda61c45f488e35c613a7c4ae26335a8d7bfd0a942f026d0fb1050e744a67d000000009100473044022025decef887fe2e3eb1c4b3edaa155e5755102d1570716f1467bb0b518b777ddf022017e97f8853af8acab4853ccf502213b7ff4cc3bd9502941369905371545de28d0147522102e7356952f4bb1daf475c04b95a2f7e0d9a12cf5b5c48a25b2303783d91849ba421030186d2b55de166389aefe209f508ce1fbd79966d9ac417adef74b7c1b5e0777652aeffffffff0130e1be07000000001976a9148dfbf103e48df7d1993448aa387dc31a2ebd522d88ac00000000",
  "complete": false,
  "errors": [
    {
      "txid": "7da644e75010fbd026f042a9d0bfd7a83563e24a7c3a615ce388f4451ca6cd4e",
      "vout": 0,
      "scriptSig": "00473044022025decef887fe2e3eb1c4b3edaa155e5755102d1570716f1467bb0b518b777ddf022017e97f8853af8acab4853ccf502213b7ff4cc3bd9502941369905371545de28d0147522102e7356952f4bb1daf475c04b95a2f7e0d9a12cf5b5c48a25b2303783d91849ba421030186d2b55de166389aefe209f508ce1fbd79966d9ac417adef74b7c1b5e0777652ae",
      "sequence": 4294967295,
      "error": "Operation not valid with the current stack size"
    }
  ]
}
```
Note that you no longer had to give `signrawtransaction` extra help, because all of that extra information was already in your wallet. Most importantly, you didn't make your private keys vulnerable by directly manipulating them. Instead the process was _exactly_ the same as respending a normal UTXO, except that the transaction wasn't fully signed at the end.

### Sign It On Other Machines

The final step is exporting the partially signed `hex` to any other machines and signing it again:
```
$ signedtx=$(bitcoin-cli -named signrawtransaction hexstring=02000000014ecda61c45f488e35c613a7c4ae26335a8d7bfd0a942f026d0fb1050e744a67d000000009100473044022025decef887fe2e3eb1c4b3edaa155e5755102d1570716f1467bb0b518b777ddf022017e97f8853af8acab4853ccf502213b7ff4cc3bd9502941369905371545de28d0147522102e7356952f4bb1daf475c04b95a2f7e0d9a12cf5b5c48a25b2303783d91849ba421030186d2b55de166389aefe209f508ce1fbd79966d9ac417adef74b7c1b5e0777652aeffffffff0130e1be07000000001976a9148dfbf103e48df7d1993448aa387dc31a2ebd522d88ac00000000 | jq -r '.hex')
```
When everyone that's required has signed, you're off to the races:
```
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
3ce88839ac6165aeadcfb188c490e1b850468eff571b4ca78fac64342751510d
```
As with the shortcut demonstrated in [§4.5: Sending Coints with Automated Raw Transactions](4_5_Sending_Coins_with_Automated_Raw_Transactions.md), the result is a lot easier, but you lose some control in the process.

## Summary: Sending & Spending an Automated Multisig

There's an easier way to resepend funds sent to multisig addresses that simply requires use of the `addmultisigaddress` command when you create your address. It doesn't demonstrate the intricacies of P2SH respending, and it doesn't give you expansive control, but if you just want to get your money, this is the way to go.
