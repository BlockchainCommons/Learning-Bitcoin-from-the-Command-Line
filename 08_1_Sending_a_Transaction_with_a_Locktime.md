# 8.1: Sending a Transaction with a Locktime

The previous chapters showed two different ways to send funds from multiple machines and to multiple recipients. But there are two other ways to fundamentally change basic transactions. The first of these is to vary time by choosing a locktime. This gives you the ability to send raw transactions at some time in the future.

## Understand How Locktime Works

When you create a locktime transaction, you lock it with a number that represents either a block height (if it's a small number) or a UNIX timestamp (if it's a big number). This tells the Bitcoin network that the transaction may not be put into a block until either the specified time has arrived or the blockchain has reached the specified height. 

> :book: _What is block height?_ It's the total count of blocks in the chain, going back to the genesis block for Bitcoin. 

When a locktime transaction is waiting to go into a block, it can be cancelled. This means that it is far, far from finalized. In fact, the ability to cancel is the whole purpose of a locktime transaction.

> :book: _What is nLockTime?_ It's the same thing as locktime. More specifically, it's what locktime is called internal to the Bitcoin Core source code.

> :book: _What is Timelock?_ Locktime is just one way to lock Bitcoin transactions until some point in the future; collectively these methods are called timelocks. Locktime is the most basic timelock method. It locks an entire transaction with an absolute time, and it's available through `bitcoin-cli` (which is why it's the only timelock covered in this section). A parallel method, which locks a transaction with a relative time, is defined in [BIP 68](https://github.com/bitcoin/bips/blob/master/bip-0068.mediawiki) and covered in [§11.3: Using CSV in Scripts](11_3_Using_CSV_in_Scripts.md). 

> Bitcoin Script further empowers both sorts of timelocks, allowing for the locking of individual outputs instead of entire transactions. Absolute timelocks (such as Locktime) are linked to the Script opcode OP_CHECKLOCKTIMEVERIFY, which is defined in [BIP 65](https://github.com/bitcoin/bips/blob/master/bip-0065.mediawiki) and covered in [§11.2: Using CLTV in Scripts](11_2_Using_CLTV_in_Scripts.md), while relative timelocks (such as Timelock) are linked to the Script opcode OP_CHECKSEQUENCEVERIFY, which is defined in [BIP 112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki) and also covered in [§11.3](11_3_Using_CSV_in_Scripts.md).

## Create a Locktime Transaction

In order to create a locktime transaction, you need to first determine what you will set the locktime to.

### Figure Out Your Locktime By UNIX Timestamp

Most frequently you will set the locktime to a UNIX timestamp representing a specific date and time. You can calculate a UNIX timestamp at a web site like [UNIX Time Stamp](http://www.unixtimestamp.com/) or [Epoch Convertor](https://www.epochconverter.com/). However, it would be better to [write your own script](https://www.epochconverter.com/#code) on your local machine, so that you know the UNIX timestamp you receive is accurate. If you don't do that, at least double check on two different sites.

> :book: _Why Would I Use a UNIX Timestamp?_ Using a UNIX timestamp makes it easy to definitively link a transaction to a specific time, without worrying about whether the speed of block creation might change at some point. Particularly if you're creating a locktime that's far in the future, it's the safer thing to do. But, beyond that, it's just more intuitive, creating a direct correlation between some calendar date and the time when the transaction can be mined.

> :warning: **WARNING:** Locktime with UNIX timestamps has a bit of wriggle room: the release of blocks isn't regular and block times can be two hours ahead of real time, so a locktime actually means "within a few hours of this time, plus or minus".

### Figure Out Your Locktime By Block Height

Alternatively, you can set the locktime to a smaller number representing a block height. To calculate your future block height, you need to first know what the current block height is. `bitcoin-cli getblockcount` will tell you what your local machine thinks the block height is. You may want to double-check with a Bitcoin explorer.

Once you've figured out the current height, you can decide how far in the future to set your locktime to. Remember that on average a new block will be created every 10 minutes. So, for example, if you wanted to set the locktime to a week in the future, you'd choose a block height that is 6 x 24 x 7 = 1,008 blocks in advance of the current one.

> :book: _Why Would I Use a Blockheight?_ Unlike with timestamps, there's no fuzziness for blockheights. If you set a blockheight of 120,000 for your locktime, then there's absolutely no way for it to go into block 119,999. This can make it easier to algorithmically control your locktimed transaction. The downside is that you can't be as sure of when precisely the locktime will be.

> :warning: **WARNING:** If you want to set a block-height locktime, you must set the locktime to less than 500 million. If you set it to 500 million or over, your number will instead be interpreted as a timestamp. Since the UNIX timestamp of 500 million was November 5, 1985, that probably means that your transaction will be put into a block at the miners' first opportunity.

## Write Your Transaction

Once you have figured out your locktime, all you need to do is write up a typical raw transaction, with a third variable for `locktime`:
```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.001, "'$changeaddress'": 0.00095 }''' locktime=1774650)
```
Note that this usage of `locktime` is under 500 million, which means that it defines a block height. In this case, it's just a few blocks past the current block height at the time of this writing, meant to exemplify how locktime works without sitting around for a long time to wait and see what happens.

Here's what the created transaction looks like:
```
$ bitcoin-cli -named decoderawtransaction hexstring=$rawtxhex 
{
  "txid": "ba440b1dd87a7ccb6a200f087d2265992588284eed0ae455d0672aeb918cf71e",
  "hash": "ba440b1dd87a7ccb6a200f087d2265992588284eed0ae455d0672aeb918cf71e",
  "version": 2,
  "size": 113,
  "vsize": 113,
  "weight": 452,
  "locktime": 1774650,
  "vin": [
    {
      "txid": "0ad9fb6992dfe4ea90236b69852b3605c0175633b32996a486dcd0b2e739e385",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967294
    }
  ],
  "vout": [
    {
      "value": 0.00100000,
      "n": 0,
      "scriptPubKey": {
        "asm": "0 f333554cc0830d03a9c1f26758e2e7e0f155539f",
        "hex": "0014f333554cc0830d03a9c1f26758e2e7e0f155539f",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1q7ve42nxqsvxs82wp7fn43ch8urc425ul5um4un"
        ]
      }
    },
    {
      "value": 0.00095000,
      "n": 1,
      "scriptPubKey": {
        "asm": "0 a37718a3510958112b6a766e0023ff251b6c2bfb",
        "hex": "0014a37718a3510958112b6a766e0023ff251b6c2bfb",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1q5dm33g63p9vpz2m2wehqqglly5dkc2lmtmr98d"
        ]
      }
    }
  ]
}
```
Note that the sequence number (`4294967294`) is less than `0xffffffff`. This is necessary signalling to show that the transaction includes a locktime. It's also done automatically by `bitcoin-cli`. If the sequence number is instead set to `0xffffffff`, your locktime will be ignored.

> :information_source: **NOTE — SEQUENCE:** This is the second use of the `nSequence` value in Bitcoin. As with RBF, `nSequence` is again used as an opt-in, this time for the use of locktime. 0xffffffff-1 (4294967294) is the preferred value for signalling locktime because it purposefully disallows the use of both RBF (which requires `nSequence < 0xffffffff-1`) and relative timelock (which requires `nSequence < 0xf0000000`), the other two uses of the `nSequence` value. If you set `nSequence` lower than `0xf0000000`, then you will also relative timelock your transaction, which is probably not what you want.

> :warning: **WARNING:** If you are creating a locktime raw transaction by some other means than `bitcoin-cli`, you will have to set the sequence to less than `0xffffffff` by hand.

## Send Your Transaction

By now you're probably well familiar with finishing things up:
```
$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
error code: -26
error message:
non-final
```
Whoop! What's that error!?

Since 2013, you generally can't place the timelocked transaction into the mempool until its lock has expired. However, you can still hold the transaction, occasionally resending it to the Bitcoin network until it's accepted into the mempool. Alternatively, you could send the signed transaction (`$signedtx`) to the recipient, so that he could place it in the mempool when the locktime has expired. 

Once the locktime is past, anyone can send that signed transaction to the network, and the recipient will receive the money as intended ... provided that the transaction hasn't been cancelled.

## Cancel a Locktime Transaction

Cancelling a locktime transaction is _very_ simple: you send a new transactions using at least one of the same UTXOs.

## Summary: Sending a Transaction with a Locktime

Locktime offers a way to create a transaction that _should_ not be relayable to the network and that _will_ not be accepted into a block until the appropriate time has arrived. In the meantime, it can be cancelled simply by reusing a UTXO.

> :fire: _What is the Power of Locktime?_ The power of locktime may not be immediately obvious because of the ability to cancel it so easily. However, it's another of the bases of Smart Contracts: it has a lot of utility in a variety of custodial or contractual applications. For example, consider a situation where a third party is holding your bitcoins. In order to guarantee the return of your bitcoins if the custodian ever disappeared, they could produce a timelock transaction to return the coins to you, then update that every once in a while with a new one, further in the future. If they ever failed to update, then the coins would return to you when the current timelock expired. Locktime could similarly be applied to a payment network, where the network holds coins while they're being exchanged by network participants. Finally, a will offers an example of a more complex contract, where payments are sent out to a number of people. These payments would be built on locktime transactions, and would be continually updated as long as the owner continues to show signs of life. (The unifying factor of all of these applications is, of course, _trust_. Simple locktime transactions only work if the holder of the coins can be trusted to send them out under the appropriate conditions.)

## What's Next?

Continue "Expanding Bitcoin Transactions" with [§8.2: Sending a Transaction with Data](08_2_Sending_a_Transaction_with_Data.md).
