# 10.1: Using Script Conditionals

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

There's one more aspect of Bitcoin Scripting that's crucial to unlocking its true power: conditionals allow you create various paths of execution.

## Understand Verify

You've already seen one conditional in scripts: `OP_VERIFY` (0x69). It pops the top item on the stack and sees if it's true, and if it's not _it ends execution of the script_. 

Verify is usually incorporated into other opcodes. You've already seen `OP_EQUALVERIFY` (0xad), `OP_CHECKLOCKTIMEVERIFY` (0xb1), and `OP_CHECKSEQUENCEVERIFY` (0xb2). Each of these opcodes does its core action (equal, checklocktime, or checksequence) and then does a verify afterward. The other verify opcodes that you haven't seen are: `OP_NUMEQUALVERIFY` (0x9d), `OP_CHECKSIGVERIFY` (0xad), and `OP_CHECKMULTISIGVERIFY` (0xaf).

So how is verify a conditional? It's the most powerful sort of conditional. Using `OP_VERIFY`, _if_ a condition is true, the Script continues executing, _else_ the Script exits. This is how you check conditions that are absolutely required for a Script to succeed. For example, the P2PKH script (`OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG`) has two required conditions: (1) that the supplied public key match the public-key hash; and (2) that the supplied signature match that public key. An `OP_EQUALVERIFY` is used for the check of the public key and the public-key hash because it's an absolutely required condition. You don't _want_ the script to continue on.

You may notice there's no `OP_VERIFY` at the end of this (or most any) script, despite the final condition being required as well. That's because Bitcoin effectively does an `OP_VERIFY` at the very end of each Script, to ensure that the final stack result is true.

## Understand If/Then

The other major conditional in Bitcoin Script is the classic OP_IF (0x63) / OP_ELSE (0x67) / OP_ENDIF (0x68). This is typical flow control: if `OP_IF` detects a true statement, it executes the block under it; otherwise, if there's an `OP_ELSE`, it executes that; and `OP_ENDIF` marks the end of the final block.

> **WARNING:** These conditionals are technically opcodes too, but as with small numbers, we're going to leave the `OP_` prefix off for brevity and clarity. 

* Verify
      * If
         * DOn't know inputs (in unlocking!)
