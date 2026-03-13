# 11.1: Understanding Timelock Options

In [§8.1: Sending a Transaction with a Locktime](08_1_Sending_a_Transaction_with_a_Locktime.md), `nLocktime` offered a great first option for locking transactions so that they couldn't be spent until some point in the future — based either on time or blockheight. But, that's not the only way to put a timelock on a transaction.

## Understand the Limitations of nLockTime

`nLockTime` is a simple and powerful way to lock a transaction, but it has some limitations:

1. **No Divisions.** `nLocktime` locks the entire transaction.
2. **No Networking.** Most modern nodes won't accept a `nLockTime` into the mempool until it's almost ready to finalize.
3. **No Scripts.** The original, simple use of `nLockTime` didn't allow it to be used in Scripts.
4. **No Protection.** `nLockTime` allows the funds to be spent with a different, non-locked transaction.

The last item was often the dealbreaker for `nLockTime`. It prevented a transaction from being spent, but it didn't prevent the funds from being used in a different transaction. So, it had uses, but they all depended on trust.

## Understand the Possibilities of Timelock Scripts

In more recent years, Bitcoin Core has expanded to allow the manipulation of timelocks at the opcode level with _OP_CHECKLOCKTIMEVERIFY_ (CLTV) and _OP_CHECKSEQUENCEVERIFY_ (CSV). These both work under a new methodology that further empowers Bitcoin.

_They Are Opcodes._ Because they're opcodes, CLTV and CSV can be used as part of more complex redemption conditions. Most often they're linked with the conditionals described in the next chapter.

_They Lock Outputs._ Because they're opcodes that are included in transactions as part of a `scriptPubKey`, they just lock that single output. That means that the transactions are accepted onto the Bitcoin network and that the UTXOs used to fund those transactions are spent. There's no going back on a transaction timelocked with CLTV or CSV like there is with a bare `nLockTime`. Respending the resultant UTXO then requires that the timelock conditions be met.

Here's one catch for using timelocks: _They're one-way locks._ Timelocks are designed so that they unlock funds at a certain time. They cannot then relock a fund: once a timelocked fund is available to spend, it remains available to spend.

### Understand the Possibilities of CLTV

_OP_CHECKLOCKTIMEVERIFY_ or CLTV is a match for the classic `nLockTime` feature, but in the new opcode-based paradigm. It allows a UTXO to become accessible at a certain time or at a certain blockheight. 

CLTV was first detailed in [BIP 65](https://github.com/bitcoin/bips/blob/master/bip-0065.mediawiki).

### Understand the Possibilities of CSV

_OP_CHECKSEQUENCEVERIFY_ or CSV depends on a new sort of "relative locktime", which is set in the transaction's _nSequence_ field. As usual, it can be set as either a time or a blockheight. If it's set as a time, "n", then a relative-timelocked transaction is spendable "n x 512" seconds after its UTXO was mined, and if it's set as a block, "n", then a relative-timelocked transaction is spendable "n" blocks after its UTXO was mined.

The use of `nSequence` for a relative timelock was first detailed in [BIP 68](https://github.com/bitcoin/bips/blob/master/bip-0068.mediawiki), then the CSV opcode was added in [BIP 112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki).

## Summary: Understanding Timelock Options

You now have four options for Timelock:

* `nLockTime` to keep a transaction off the blockchain until a specific time.
* `nSequence` to keep a transaction off the blockchain until a relative time.
* CLTV to make a UTXO unspendable until a specific time.
* CSV to make a UTXO unspendable until a relative time.

## What's Next?

Continue "Empowering Timelock" with [§11.2: Using CLTV in Scripts](11_2_Using_CLTV_in_Scripts.md). 
