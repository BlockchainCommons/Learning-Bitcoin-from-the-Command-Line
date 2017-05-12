# 7.3: Scripting a Pay to Public Key Hash

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

With basic understanding of Bitcoin Scripting in hand, you can now easily analyze the functioning of a standard P2PKH script.

## Understand the Unlocking Script

We've long said that when funds are sent to a Bitcoin address, they're locked to the private address associated with the address. This is all managed through `scriptPubKey` in a P2PKH transaction, which is set up so that it requires two secret bits of information: the private key and the public key associated with the Bitcoin address, which is itself a public key hash.

The example in §6.2 has a `scriptSig` unlocking script that is `3045022100c4ef5b531061a184404e84ab46beee94e51e8ae15ce98d2f3e10ae7774772ffd02203c546c399c4dc1d6eea692f73bb3fff490ea2e98fe300ac6a11840c7d52b6166[ALL] 0319cd3f2485e3d47552617b03c693b7f92916ac374644e22b07420c8812501cfb`. This is just `<signature> <pubKey>`.

## Understand the Locking Script

The `scriptPubKey` locking script from §6.2 is `OP_DUP OP_HASH160 371c20fb2e9899338ce5e99908e64fd30b789313 OP_EQUALVERIFY OP_CHECKSIG`, which is the standard methodology for a P2PKH address. That long chain string in the middle is a `<pubKeyHash>`.

## Run a P2PKH Script

When you run a P2PKH unlocking script with the P2PKH in the UTXO, the following is (effectively) generated:

```
Script: <signature> <pubKey> OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
```
First, the script puts the initial constants on the stack and makes a duplicate of the pubKey:
```
Script: <pubKey> OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Stack: [ <signature> ]

Script: OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Stack: [ <signature> <pubKey> ]

Script: OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Stack: [ <signature> <pubKey> <pubKey> ]
```
Why the duplicate? Because that's what's required by the script!

Next `OP_HASH160` pops the `<pubKey>` off the stack, hashes it, and puts the result back on the stack.
```
Script: <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Stack: [ <signature> <pubKey> <pubKeyHash> ]
```
The next constant follows it:
```
Script: OP_EQUALVERIFY OP_CHECKSIG
Stack: [ <signature> <pubKey> <pubKeyHash> <pubKeyHash> ]
```
`OP_EQUALVERIFY` is really two opcodes: `OP_EQUAL`, which pops the two two elements and pushes true or false depending on if they're equal; and verify which pops the result and immediately marks the transaction as invalid if it's false.

Assuming the two `<pubKeyHash>es` are equal, we now have the following situation:
```
Script: OP_CHECKSIG
Stack: [ <signature> <pubKey> ]
```
At this point we've proven that the `<pubKey>` supplied in the `scriptSig` matches the one associated with the Bitcoin address.

Finally, `OP_CHECKSIG` verifies that the unlocking script's signature proves ownership of the private key associated with that private key:
```
Script:
Stack: [ True ]
```
At this point, the Script ends and the transaction is allowed to respend the UTXO in question.

## Summary: Scripting a Pay to Public Key Hash

Sending to a P2PKH address was relativel easy when you were just using `bitcoin-cli`. Examining the Bitcoin Script underlying it lays bare the cryptographic functions that were implicit in funding that transaction: how UTXOs were unlocked with a signature and a public key; and how the new transaction output was in turn locked with a new public-key hash.


