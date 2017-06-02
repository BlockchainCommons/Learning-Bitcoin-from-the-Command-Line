# 11.2: Writing Complex Multisig Scripts

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

To date, the multisigs described in these documents have been entirely simple, of the m-of-n or n-of-n form. However, you might desire more complex multisigs, signers might need different cosigners or where different options might become available over time. 

## Write a Variable Multisig

A variable multisig requires different numbers of people to sign depending on who is signing.

### Write a Multisig with a Single Signer or Co-Signers

Imagine a corporation where either the president or two-out-of-three vice presidents could agree to the usage of funds.

We could write this by creating an IF/ELSE/ENDIF statement that has two blocks, one for the president and his one-of-one signature and one for the vice-presidents and their two-of-three signatures. We could then determine which block is being used by how many signatures were in the unlocking script. Using `OP_DEPTH 1 OP_EQUAL` will tell us if there is one item on the stack, and we can go from there.

The full locking script would be `OP_DEPTH 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF`

If run by the president, it would look like this:
```
Script: <sigPres> OP_DEPTH 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Stack: [ ]

Script: OP_DEPTH 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Stack: [ <sigPres> ]

Script: 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Running: <SigPres> OP_DEPTH
Stack: [ <sigPres> 1 ]

Script: OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Stack: [ <sigPres> 1 1 ]

Script: IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Running: 1 1 OP_EQUAL
Stack: [ <sigPres> True ]
```
Because the result is `True`, the Script now collapses to the `IF` statement:
```
Script: <pubKeyPres> OP_CHECKSIGNATURE
Running: True IF
Stack: [ <sigPres> ]

Script: OP_CHECKSIGNATURE
Stack: [ <sigPres> <pubKeyPres> ]

Script: 
Running: <sigPres> <pubKeyPres> OP_CHECKSIGNATURE
Stack: [ True ]
```
If run by two vice-presidents, it would look like this:
```
Script: 0 <sigVPA> <sigVPB> OP_DEPTH 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Stack: [ ]

Script: OP_DEPTH 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Stack: [ 0 <sigVPA> <sigVPB> ]

Script: 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Running: 0 <sigVPA> <sigVPB> OP_DEPTH
Stack: [ 0 <sigVPA> <sigVPB> 3 ]

Script: OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Stack: [ 0 <sigVPA> <sigVPB> 3 1 ]

Script: IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Running: 3 1 OP_EQUAL
Stack: [ 0 <sigVPA> <sigVPB> False ]
```
Because the result is `False`, the Script now collapses to the `IF` statement:
```
Script: 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG
Running: False IF
Stack: [ 0 <sigVPA> <sigVPB> ]

Script: OP_CHECKMULTISIG
Stack: [ 0 <sigVPA> <sigVPB> 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 ]

Script: 
Running: 0 <sigVPA> <sigVPB> 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG
Stack: [ ]
```
You might notice that our `OP_CHECKSIGNATURE` is different because we included the public key in the locking script, rather than requiring a hash. This is done so that all of the submissions of signatures are the same. It should be secure, as the public key only appears in the hashed `redeemScript` in the original transaction.

The one possible catch is what to do if the President is absent-minded and accidentally signs a transaction with one of his VPs, because he remembers this being a 2-of-3 multisig. One option is to decide that's an acceptable failure condition, because the President is using the multsig incorrectly. Another option is to turn the 2-of-3 multisig into a 2-of-4 multisig, just in case the President doesn't tolerate failure: `OP_DEPTH 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> <pubKeyPres> 4 OP_CHECKMULTISIG ENDIF`. This would allow the President to mistakenly sign with any Vice President, but wouldn't impact things if two Vice Presidents wanted to (correctly) sign.

### Write a Multisig with a Required Signer

Another multisig possibility includes have a m-of-n multisig were one of the signers is required. This can usually be managed by breaking the multisig down into multiple m of n-1 multisigs. For example, a 2-of-3 multisig where one of the signers is required would actually be two 2-of-2 multisigs, each including the required signer.

Here's a simple way to script that:
```
OP_3DUP
2 <pubKeyRequired> <pubKeyA> 2  OP_CHECKMULTISIG
NOTIF

  2 <pubKeyRequired> <pubKeyB> 2  OP_CHECKMULTISIG

ENDIF
```
The unlocking script would be either `0 <pubKeyRequired> <pubKeyA>` or `<pubKeyRequired> <pubKeyB>`.

First the Script would check against `<pubKeyRequired> <pubKeyA>`. If that fails, it would check against `<pubKeyRequired> <pubKeyB>`.

The result of the final `OP_CHECKMULTISIG` that was run will be left on the top of the stack (though there will be cruft below it if the first one succeeded).

## Write an Escrow Multisig

We've talked many times about esscrows. Complex multisigs combined with timelocks offer an automated way to create them in a robust manner.

Imagine home buyer Alice and home seller Bob who are working with an escrow agent The easy way to script this would be as a multisig where any two of the three parties could release the money: either the seller and buyer agree or the escrow agent takes over and agrees with one of the parties: `2 <pubKeyA> <pubKeyB> <pubKeyEscrow> 3 OP_CHECKMULTISG`.

However, this weakens the power of the escrow agent and allows our seller and buyer to accidentally make a bad decision among themselves — which is one of the things an escrow system is designed to avoid. So it could be that what we really want is the system that we just laid out, where the escrow agent is a required party in the 2-of-3 multisig: `OP_3DUP 2 <pubKeyEscrow> <pubKeyA> 2  OP_CHECKMULTISIG NOTIF 2 <pubKeyEscrow> <pubKeyB> 2  OP_CHECKMULTISIG ENDIF`.

However, this doesn't pass the walk-in-front-of-a-bus test. If our escrow agent dies or flees to the Bahamas during the escrow, the buyer and seller are out a lot of money. This is where a timelock comes in. We can open an additional branch of the test that only becomes available if we're passed the end of our escrow period:
```
OP_3DUP
2 <pubKeyRequired> <pubKeyA> 2  OP_CHECKMULTISIG
NOTIF

  2 <pubKeyRequired> <pubKeyB> 2  OP_CHECKMULTISIG

  NOTIF

    <+31Days> OP_CHECKSEQUENCEVERIFY OP_DROP
    2 <pubKeyA> <pubKeyB> 2  OP_CHECKMULTISIG
    
  ENDIF
ENDIF
```
First we test a signature for the buyer and the escrow agent, then a signature for the seller and the escrow agent. If both of those fail and 31 days have passed, then we also allow a signature for the buyer and seller.

### Write a Simpler Escrow Multisig

[BIP 112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki#Escrow_with_Timeout) actually offers an easier example of this sort of escrow that doesn't have the extra protections:
```
IF

    2 <pubKeyA> <pubKeyB> <pubKeyEscrow> 3 OP_CHECKMULTISIG 

ELSE

    <+31Days> OP_CHECKSEQUENCEVERIFY OP_DROP
    <pubKeyA> OP_CHECKSIG

ENDIF
```
Here, any two of the three signers can release the money at any time, but after 30 days, Alice can retrieve her money on her own.

Note that this Script requires a `True` or `False` to be passed in to identify which branch is being used. This is a simpler, less computationally intensive way to do thing that is relatively common.

Early on, the following `sigScript` would be allowed: `0 <signer1> <signer2> True`. Later on, also could produce a `sigScript` like this: `<sigA> False`.

## Summary: Writing Complex Multisig Scripts

More complex multisignatures can typically be created by combining various signatures or multisignatures with conditionals and tests. The resulting multisigs can be variable requiring different numbers of signers based on who they are and when they're signing. 

_What is the power of complex multisig scripts?_ More than anything we've seen to date, complex multisig scripts are truly smart contracts. They can be very precise in who is allowed to sign and when. Multi-level corporations, partnerships, and escrows alike can be supported. Using other powerful features like timelocks can further protect these funds, allowing them to be released or even returned at certain times.
