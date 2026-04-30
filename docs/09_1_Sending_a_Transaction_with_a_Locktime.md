# 9.1: Sending a Transaction with a Locktime

The previous chapters showed how to vary up transactions by sending
them from multiple machines and to multiple recipients gathered under
a single address.  But there are two other ways to fundamentally
change basic transactions. The first of these is to vary time by
choosing a locktime. This gives you the ability to send raw
transactions at some time in the future.

## Understand How Locktime Works

When you create a locktime transaction, you lock it with a number that
represents either a block height (if it's a small number) or a UNIX
timestamp (if it's a big number). This tells the Bitcoin network that
the transaction may not be put into a block until either the specified
time has arrived or the blockchain has reached the specified height.

> 📖 **_What is block height?_** It's the total count of blocks in the
chain, going back to the genesis block for Bitcoin.

When a locktime transaction is waiting to go into a block, it can be
cancelled. This means that it is far, far from finalized. In fact, the
ability to cancel is one of the main points of a Locktime transaction.

> 📖 **_What is nLockTime?_** It's the same thing as locktime. More
specifically, it's what locktime is called internal to the Bitcoin
Core source code.

> :book: **_What is a timelock?_** Locktime is just one way to lock
Bitcoin transactions until some point in the future; collectively
these methods are called timelocks. Locktime is the most basic
timelock method. It locks an entire transaction with an absolute time,
and it's available through `bitcoin-cli` (which is why it's the only
timelock covered in this section). A parallel method, which locks a
transaction with a relative time, is defined in [BIP
68](https://github.com/bitcoin/bips/blob/master/bip-0068.mediawiki)
and covered in [§11.3: Using CSV in
Scripts](11_3_Using_CSV_in_Scripts.md).

> Bitcoin Script further empowers both sorts of timelocks, allowing
for the locking of individual outputs instead of entire
transactions. Absolute timelocks do so with the Script opcode
OP_CHECKLOCKTIMEVERIFY, which is defined in [BIP
65](https://github.com/bitcoin/bips/blob/master/bip-0065.mediawiki)
and covered in [§11.2: Using CLTV in
Scripts](11_2_Using_CLTV_in_Scripts.md), while relative timelocks aare
linked to the Script opcode OP_CHECKSEQUENCEVERIFY, which is defined
in [BIP
112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki)
and also covered in [§11.3](11_3_Using_CSV_in_Scripts.md).

## Create a Locktime Transaction

In order to create a locktime transaction, you need to first determine
what you will set the locktime to.

### Figure Out Your Locktime By UNIX Timestamp

Most frequently you will set the locktime to a UNIX timestamp
representing a specific date and time. You can calculate a UNIX
timestamp at a web site like [UNIX Time
Stamp](http://www.unixtimestamp.com/) or [Epoch
Convertor](https://www.epochconverter.com/). However, it would be
better to [write your own
script](https://www.epochconverter.com/#code) on your local machine,
so that you know the UNIX timestamp you receive is accurate. If you
don't do that, at least double check on two different sites.

> 📖 **_Why Would I Use a UNIX Timestamp?_** Using a UNIX timestamp
makes it easy to definitively link a transaction to a specific time,
without worrying about whether the speed of block creation might
change at some point. Particularly if you're creating a locktime
that's far in the future, it's the safer thing to do. But, beyond
that, it's just more intuitive because it creates a direct correlation
between some calendar date and the time when the transaction can be
mined.

> ⚠️ **WARNING:** Locktime with UNIX timestamps has a bit of wriggle
room: the release of blocks isn't regular and block times can be two
hours ahead of real time, so a locktime actually means "within a few
hours of this time, plus or minus".

### Figure Out Your Locktime By Block Height

Alternatively, you can set the locktime to a smaller number
representing a block height. To calculate your future block height,
you need to first know what the current block height is. `bitcoin-cli
getblockcount` will tell you what your local machine thinks the block
height is. You may want to double-check with a Bitcoin explorer.

```
$ bitcoin-cli getblockcount
302373
```

Once you've figured out the current height, you can decide how far in
the future to set your locktime to. Remember that on average a new
block will be created every 10 minutes. So, for example, if you wanted
to set the locktime to a week in the future, you'd choose a block
height that is 6 x 24 x 7 = 1,008 blocks in advance of the current
one.

> 📖 **_Why Would I Use a Blockheight?_** Unlike with timestamps,
there's no fuzziness for blockheights. If you set a blockheight of
120,000 for your locktime, then there's absolutely no way for it to go
into block 119,999. This can make it easier to algorithmically control
your locktimed transaction. The downside is that you can't be as sure
of when precisely the locktime will be.

> ⚠️ **WARNING:** If you want to set a block-height locktime, you must
set the locktime to less than 500 million. If you set it to 500
million or over, your number will instead be interpreted as a
timestamp. Since the UNIX timestamp of 500 million was November 5,
1985, that probably means that your transaction will be put into a
block at the miners' first opportunity.

## Write Your Transaction

Once you have figured out your locktime, all you need to do is write
up a typical raw transaction, with a third variable for `locktime`:

```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.001, "'$changeaddress'": 0.013 }''')
```

Note that this usage of `locktime` is under 500 million, which means
that it defines a block height. In this case, it's just a few blocks
past the current block height at the time of this writing, meant to
exemplify how locktime works without sitting around for a long time to
wait and see what happens.

Here's what the created transaction looks like:
```
$ bitcoin-cli -named decoderawtransaction hexstring=$rawtxhex 
{
  "txid": "adb314f5e60e5e38a53a506e668eb7204638d36234fff437eb24711f13117330",
  "hash": "adb314f5e60e5e38a53a506e668eb7204638d36234fff437eb24711f13117330",
  "version": 2,
  "size": 113,
  "vsize": 113,
  "weight": 452,
  "locktime": 302378,
  "vin": [
    {
      "txid": "9c1f8e701638cf58c8430f8ffd30c5235948268ecabb6af07d0dc378fff29d9e",
      "vout": 478,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967293
    }
  ],
  "vout": [
    {
      "value": 0.00100000,
      "n": 0,
      "scriptPubKey": {
        "asm": "0 102accd89423260c16a9149e50601cd5eff36ad8",
        "desc": "addr(tb1qzq4veky5yvnqc94fzj09qcqu6hhlx6kcazvs7g)#trtezash",
        "hex": "0014102accd89423260c16a9149e50601cd5eff36ad8",
        "address": "tb1qzq4veky5yvnqc94fzj09qcqu6hhlx6kcazvs7g",
        "type": "witness_v0_keyhash"
      }
    },
    {
      "value": 0.01300000,
      "n": 1,
      "scriptPubKey": {
        "asm": "0 513ed4ed3a8657646fa72c70d6194b85ccab1d38",
        "desc": "addr(tb1q2yldfmf6setkgma893cdvx2tshx2k8fckwaggv)#m55ayk3a",
        "hex": "0014513ed4ed3a8657646fa72c70d6194b85ccab1d38",
        "address": "tb1q2yldfmf6setkgma893cdvx2tshx2k8fckwaggv",
        "type": "witness_v0_keyhash"
      }
    }
  ]
}
```

Note that the sequence number (`4294967293`) is less than
`0xffffffff`. This is necessary signalling to show that the
transaction includes a locktime. It's also done automatically by
`bitcoin-cli`. If the sequence number is instead set to `0xffffffff`,
your locktime will be ignored.

> ℹ️ **Sequence Usage #2.** This is the second use of the `nSequence`
value in Bitcoin. As with RBF, `nSequence` is again used as an opt-in,
this time for the use of locktime. 0xffffffff-2 (4294967293) is now
the preferred value for signalling locktime because it purposefully
allows the use of both RBF (which requires `nSequence < 0xffffffff-1`)
and absolute locktime but disallows relative timelock (which requires
`nSequence < 0xf0000000`), the other use of the `nSequence` value. If
you set `nSequence` lower than `0xf0000000`, then you will activate
relative timelocks your transaction, which is probably not what you
want.

> ⚠️ **WARNING:** If you are creating a locktime raw transaction by
some other means than `bitcoin-cli`, you will have to set the sequence
to less than `0xffffffff` by hand.

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

That's what's expected!  You're not allowed to place the timelocked
transaction into the mempool until its lock has expired. Instead, hold
the transaction, occasionally resending it to the Bitcoin network
until it's accepted into the mempool.

Alternatively, you could send the signed transaction (`$signedtx`) to
the recipient, so that _they_ could place it in the mempool when the
locktime has expired.

Once the locktime is past, anyone can send that signed transaction to
the network, and the recipient will receive the money as intended
... provided that the transaction hasn't been cancelled.

## Cancel a Locktime Transaction

Cancelling a locktime transaction is _very_ simple: you send a new
transaction using at least one of the same UTXOs.

## Summary: Sending a Transaction with a Locktime

Locktime offers a way to create a transaction that _should_ not be
relayable to the network and that _will_ not be accepted into a block
until the appropriate time has arrived. In the meantime, it can be
cancelled simply by reusing a UTXO.

> 🔥 **_What is the Power of Locktime?_** The power of locktime may
not be immediately obvious because of the ability to cancel it so
easily. However, it's another of the bases of Smart Contracts: it has
a lot of utility in a variety of custodial or contractual
applications. For example, consider a situation where a third party is
holding your bitcoins. In order to guarantee the return of your
bitcoins if the custodian ever disappeared, they could produce a
timelock transaction to return the coins to you, then update that
every once in a while with a new one, further in the future. If they
ever failed to update, then the coins would return to you when the
current timelock expired. Locktime could similarly be applied to a
payment network, where the network holds coins while they're being
exchanged by network participants. Finally, a will offers an example
of a more complex contract, where payments are sent out to a number of
people. These payments would be built on locktime transactions, and
would be continually updated as long as the owner continues to show
signs of life. (The unifying factor of all of these applications is,
of course, _trust_. Simple locktime transactions only work if the
holder of the coins can be trusted to send them out under the
appropriate conditions.)

## What's Next?

Complete your work on "Expanding Bitcoin Transactions" with [§9.2: Sending a
Transaction with Data](09_2_Sending_a_Transaction_with_Data.md).
