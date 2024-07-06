# 12.1: Using Script Conditionals

There's one final aspect of Bitcoin Scripting that's crucial to unlocking its true power: conditionals allow you create various paths of execution.

## Understand Verify

You've already seen one conditional in scripts: `OP_VERIFY` (0x69). It pops the top item on the stack and sees if it's true; if not _it ends execution of the script_. 

Verify is usually incorporated into other opcodes. You've already seen `OP_EQUALVERIFY` (0xad), `OP_CHECKLOCKTIMEVERIFY` (0xb1), and `OP_CHECKSEQUENCEVERIFY` (0xb2). Each of these opcodes does its core action (equal, checklocktime, or checksequence) and then does a verify afterward. The other verify opcodes that you haven't seen are: `OP_NUMEQUALVERIFY` (0x9d), `OP_CHECKSIGVERIFY` (0xad), and `OP_CHECKMULTISIGVERIFY` (0xaf).

So how is `OP_VERIFY` a conditional? It's the most powerful sort of conditional. Using `OP_VERIFY`, _if_ a condition is true, the Script continues executing, _else_ the Script exits. This is how you check conditions that are absolutely required for a Script to succeed. For example, the P2PKH script (`OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG`) has two required conditions: (1) that the supplied public key match the public-key hash; and (2) that the supplied signature match that public key. An `OP_EQUALVERIFY` is used for the comparison of the hashed public key and the public-key hash because it's an absolutely required condition. You don't _want_ the script to continue on if that fails.

You may notice there's no `OP_VERIFY` at the end of this (or most any) script, despite the final condition being required as well. That's because Bitcoin effectively does an `OP_VERIFY` at the very end of each Script, to ensure that the final stack result is true. You don't _want_ to do an `OP_VERIFY` before the end of the script, because you need to leave something on the stack to be tested!

## Understand If/Then

The other major conditional in Bitcoin Script is the classic `OP_IF` (0x63) / `OP_ELSE` (0x67) / `OP_ENDIF` (0x68). This is typical flow control: if `OP_IF` detects a true statement, it executes the block under it; otherwise, if there's an `OP_ELSE`, it executes that; and `OP_ENDIF` marks the end of the final block.

> :warning: **WARNING:** These conditionals are technically opcodes too, but as with small numbers, we're going to leave the `OP_` prefix off for brevity and clarity. Thus we'll write `IF`, `ELSE`, and `ENDIF` instead of `OP_IF`, `OP_ELSE`, and `OP_ENDIF`.

### Understand If/Then Ordering

There are two big catches to conditionals. They make it harder to read and assess scripts if you're not careful.

First, the `IF` conditional checks the truth of what's _before it_ (which is to say what's in the stack), not what's after it. 

Second, the `IF` conditional tends to be in the locking script and what it's checking tends to be in the unlocking script.

Of course, you might say, that's how Bitcoin Script works. Conditionals use reverse Polish notation and they adopt the standard unlocking/locking paradigm, just like _everything else_ in Bitcoin Scripting. That's all true, but it also goes contrary to the standard way we read IF/ELSE conditionals in other programming languages; thus, it's easy to unconsciously read Bitcoin conditionals wrong.

Consider the following code: `IF OP_DUP OP_HASH160 <pubKeyHashA> ELSE OP_DUP OP_HASH160 <pubKeyHashB> ENDIF OP_EQUALVERIFY OP_CHECKSIG `. 

Looking at conditionals in prefix notation might lead you to read this as:
```
IF (OP_DUP) THEN

    OP_HASH160 
    OP_PUSHDATA <pubKeyHashA> 

ELSE 

    OP_DUP 
    OP_HASH160 
    OP_PUSHDATA <pubKeyHashB> 

ENDIF 
 
 OP_EQUALVERIFY 
 OP_CHECKSIG
```
So, you might think, if the `OP_DUP` is successful, then we get to do the first block, else the second. But that doesn't make any sense! Why wouldn't the `OP_DUP` succeed?!

And, indeed, it doesn't make any sense, because we accidentally read the statement using the wrong notation. The correct reading of this is:
```
IF 
  
    OP_DUP
    OP_HASH160 
    OP_PUSHDATA <pubKeyHashA> 

ELSE 

    OP_DUP 
    OP_HASH160 
    OP_PUSHDATA <pubKeyHashB> 

ENDIF 
 
 OP_EQUALVERIFY 
 OP_CHECKSIG
```
The statement that will evaluate to `True` or `False` is placed on the stack _prior_ to running the `IF`, then the correct block of code is run based on that result.

This particular example code is intended as a poor man's 1-of-2 multisignature. The owner of `<privKeyA>` would put `<signatureA> <pubKeyA> True` in his unlocking script, while the owner of `<privKeyB>` would put `<signatureB> <pubKeyB> False` in her unlocking script. That trailing `True` or `False` is what's checked by the `IF`/`ELSE` statement.  It tells the script which public-key hash to check against, then the `OP_EQUALVERIFY` and the `OP_CHECKSIG` at the end do the real work. 

### Run an If/Then Multisig

With a core understanding of Bitcoin conditionals in hand, we're now ready to run through a script. We're going to do so by creating a slight variant of our poor man's 1-of-2 multisignature where our users don't have to remember if they're `True` or `False`. Instead, if need be, the script checks both public-key hashes, just requiring one success:
```
OP_DUP OP_HASH160 <pubKeyHashA> OP_EQUAL
IF

    OP_CHECKSIG

ELSE

    OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG
    
ENDIF

```
Remember your reverse Polish notation! That `IF` statement if referring to the `OP_EQUAL` before it, not the `OP_CHECKSIG` after it!

#### Run the True Branch

Here's how it actally runs if unlocked with `<signatureA> <pubKeyA>`:
```
Script: <signatureA> <pubKeyA> OP_DUP OP_HASH160 <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Stack: [ ]
```
First, we put constants on the stack:
```
Script: OP_DUP OP_HASH160 <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Stack: [ <signatureA> <pubKeyA> ]
```
Then we run the first few, obvious commands, `OP_DUP` and `OP_HASH160` and push another constant:
```
Script: OP_HASH160 <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Running: <pubKeyA> OP_DUP
Stack: [ <signatureA> <pubKeyA> <pubKeyA> ]

Script: <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Running: <pubKeyA> OP_HASH160
Stack: [ <signatureA> <pubKeyA> <pubKeyHashA> ]

Script: OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Stack: [ <signatureA> <pubKeyA> <pubKeyHashA> <pubKeyHashA> ]
```
Next we run the `OP_EQUAL`, which is what's going to feed the `IF`:
```
Script: IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Running: <pubKeyHashA> <pubKeyHashA> OP_EQUAL
Stack: [ <signatureA> <pubKeyA> True ]
```
Now the `IF` runs, and since there's a `True`, it only runs the first block, eliminating all the rest:
```
Script: OP_CHECKSIG
Running: True IF
Stack: [ <signatureA> <pubKeyA> ]
```
And the `OP_CHECKSIG` will end up `True` as well:
```
Script: 
Running: <signatureA> <pubKeyA> OP_CHECKSIG
Stack: [ True ]
```
#### Run the False Branch

Here's how it actally runs if unlocked with `<signatureB> <pubKeyB>`:
```
Script: <signatureB> <pubKeyB> OP_DUP OP_HASH160 <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Stack: [ ]
```
First, we put constants on the stack:
```
Script: OP_DUP OP_HASH160 <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Stack: [ <signatureB> <pubKeyB> ]
```
Then we run the first few, obvious commands, `OP_DUP` and `OP_HASH160` and push another constant:
```
Script: OP_HASH160 <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Running: <pubKeyB> OP_DUP
Stack: [ <signatureB> <pubKeyB> <pubKeyB> ]

Script: <pubKeyHashA> OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Running: <pubKeyB> OP_HASH160
Stack: [ <signatureB> <pubKeyB> <pubKeyHashB> ]

Script: OP_EQUAL IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Stack: [ <signatureB> <pubKeyB> <pubKeyHashB> <pubKeyHashA> ]
```
Next we run the `OP_EQUAL`, which is what's going to feed the `IF`:
```
Script: IF OP_CHECKSIG ELSE OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG ENDIF
Running: <pubKeyHashB> <pubKeyHashA> OP_EQUAL
Stack: [ <signatureB> <pubKeyB> False ]
```
Whoop! The result was `False` because `<pubKeyHashB>` does not equal `<pubKeyHashA>`. Now when the `IF` runs, it collapses down to just the `ELSE` statement:
```
Script: OP_DUP OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG
Running: False IF
Stack: [ <signatureB> <pubKeyB> ]
```
Afterward, we go through the whole rigamarole again, starting with another `OP_DUP`, but eventually testing against the other `pubKeyHash`:
```
Script: OP_HASH160 <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG
Running: <pubKeyB> OP_DUP
Stack: [ <signatureB> <pubKeyB> <pubKeyB> ]

Script: <pubKeyHashB> OP_EQUALVERIFY OP_CHECKSIG
Running: <pubKeyB> OP_HASH160
Stack: [ <signatureB> <pubKeyB> <pubKeyHashB> ]

Script: OP_EQUALVERIFY OP_CHECKSIG
Stack: [ <signatureB> <pubKeyB> <pubKeyHashB> <pubKeyHashB> ]

Script:OP_CHECKSIG
Running: <pubKeyHashB> <pubKeyHashB> OP_EQUALVERIFY 
Stack: [ <signatureB> <pubKeyB> ]

Script: 
Running: <signatureB> <pubKeyB> OP_CHECKSIG
Stack: [ True ]
```
This probably isn't nearly as efficient as a true Bitcoin multisig, but it's a good example of how results pushed onto the stack by previous tests can be used to feed future conditionals. In this case, it's the failure of the first signature which tells the conditional that it should go check the second one. 

## Understand Other Conditionals

There are a few other conditionals of note. The big one is `OP_NOTIF` (0x64), which is the opposite of `OP_IF`: it executes the following block if the top item is `False`. An `ELSE` can be placed with it, which as usual is executed if the first block is not executed. You still end with `OP_ENDIF`.

There's also an `OP_IFDUP` (0x73), which duplicates the top stack item only if it's not 0.

These options are used much less often than the main `IF`/`ELSE`/`ENDIF` construction.

## Summary: Using Script Conditionals

Conditionals in Bitcoin Script allow you to halt the script (using `OP_VERIFY`) or to choose different branches of execution (using `OP_IF`). However, reading `OP_IF` can be a bit tricky. Remember that it's the item pushed onto the stack _before_ the `OP_IF` is run that controls its execution; that item will typically be part of the unlocking script (or else a direct result of items in the unlocking script).

> :fire: ***What is the power of conditionals?*** Script Conditionals are the final major building block in Bitcoin Script. They're what are required to turn simple, static Bitcoin Scripts into complex, dynamic Bitcoin Scripts that can evaluate differently based on different times, different circumstances, or different user inputs. In other words, they're the final basis of smart contracts.

## What's Next?

Continue "Expanding Bitcoin Scripts" with [§12.2: Using Other Script Commands](12_2_Using_Other_Script_Commands.md).
