# 12.2: Using Other Script Commands

You may already have in hand most of the Bitcoin Script opcodes that you'll be using in most scripts. However, Bitcoin Script offers a lot more options, which might be exactly what you need to create the financial instrument of your dreams.

You should consult the [Bitcoin Script page](https://en.bitcoin.it/wiki/Script) for a more thorough look at all of these and many other commands. This section only highlights the most notable opcodes.

## Understand Arithmetic Opcodes

Arithmetic opcodes manipulate or test numbers.

Manipulate one number:

* OP_1ADD	(0x8b) — Increment by one
* OP_1SUB	(0x8c)	 — Decrement by one
* OP_NEGATE	(0x8f)	— Flip the sign of the number
* OP_ABS	(0x90)	— Make the number positive
* OP_NOT (0x91) — Flips 1 and 0, else 0

Also see: `OP_0NOTEQUAL` (0x92)

Manipulate two numbers mathematically:

* OP_ADD (0x93) — Add two numbers
* OP_SUB (0x94) — Subtract two numbers
* OP_MIN (0xa3) — Return the smaller of two numbers
* OP_MAX (0xa4) — Return the larger of two numbers

Manipulate two numbers logically:

* OP_BOOLAND (0x9a)	— 1 if both numbers are not 0, else 0
* OP_BOOLOR	(0x9b) — 1 if either number is not 0, else 0

Test two numbers:

* OP_NUMEQUAL	(0x9c) — 1 if both numbers are equal, else 0
* OP_LESSTHAN	(0x9f) — 1 if first number is less than second, else 0
* OP_GREATERTHAN (0xa0) — 1 if first number is greater than second, else 0
* OP_LESSTHANOREQUAL (0xa1) — 1 if first number is less than or equal to second, else 0
* OP_GREATERTHANOREQUAL (0xa2) — 1 if first number is greater than or equal to second, else 0

Also see: `OP_NUMEQUALVERIFY` (0x9d), `OP_NUMNOTEQUAL` (0x9e)

Test three numbers:

* OP_WITHIN	(0xa5) — 1 if a number is in the range of two other numbers

## Understand Stack Opcodes

There are a shocking number of stack opcodes, but other than `OP_DROP`, `OP_DUP`, and sometimes `OP_SWAP` they're generally not necessary if you're careful about stack ordering. Nonetheless, here are a few of the more interesting ones:

* OP_DEPTH (0x74) — Pushes the size of the stack
* OP_DROP	(0x75) — Pops the top stack item
* OP_DUP	(0x76) — Duplicates the top stack item
* OP_PICK (0x79) — Duplicates the nth stack item as the top of the stack
* OP_ROLL (0x7a) — Moves the nth stack item to the top of the stack
* OP_SWAP (0x7c) — Swaps the top two stack items

Also see: `OP_TOALTSTACK` (0x6b), `OP_FROMALTSTACK` (0x6c), `OP_2DROP` (0x6d), `OP_2DUP` (0x6e), `OP_3DUP` (0x6f), `OP_2OVER` (0x70), `OP_2ROT` (0x71), `OP_2SWAP` (0x72), `OP_IFDUP` (0x73), `OP_NIP` (0x77), `OP_OVER` (0x78), `OP_ROT` (0x7b), and `OP_TUCK` (0x7d).

## Understand Cryptographic Opcodes

Finally, a variety of opcodes support hashing and signature checking:

Hash:

* OP_RIPEMD160 (0xa6) — RIPEMD-160
* OP_SHA1	(0xa7)	— SHA-1
* OP_SHA256	(0xa8)	- SHA-256
* OP_HASH160	(0xa9)	— SHA-256 + RIPEMD-160
* OP_HASH256	(0xaa)	— SHA-256 + SHA-256

Check Signatures:

* OP_CHECKSIG (0xac) — Check a signature
* OP_CHECKMULTISIG (0xae) — Check a m-of-n multisig

Also see: `OP_CODESEPARATOR` (0xab), `OP_CHECKSIGVERIFY` (0xad), and `OP_CHECKMULTISIGVERIFY` (0xaf).

## Summary: Using Other Script Commands

Bitcoin Script includes a wide array of arithemetic, stack, and cryptographic opcodes. Most of these additional opcodes are probably not as common as the ones discussed in previous sections, but nonetheless they're available if they're just what you need to write your Script!

## What's Next?

Advance through "Bitcoin Scripting" with [Chapter Thirteen: Designing Real Bitcoin Scripts](13_0_Designing_Real_Bitcoin_Scripts.md).
