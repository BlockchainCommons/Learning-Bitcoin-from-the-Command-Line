 # 13.1: Writing Puzzle Scripts

Bitcoin Scripts _don't_ actually have to depend on the knowledge of a secret key. They can instead be puzzles of any sort. 

## Write Simple Algebra Scripts

Our first real Script, from [§9.2: Running a Bitcoin Script](09_2_Running_a_Bitcoin_Script.md) was an alegbraic puzzle. That Bitcoin Script, `OP_ADD 99 OP_EQUAL`, could have been alternatively described as `x + y = 99`.

This sort of Script doesn't have a lot of applicability in the real world, as it's too easy to claim the funds. But, a puzzle-solving contest giving out Bitcoin dust might offer it as a fun entertainment.

More notably, creating alegebraic puzzles gives you a nice understanding of how the arithmetic functions in Bitcoin Script work.

### Write a Multiplier Script

Bitcoin Script has a number of opcodes that were disabled to maintain the security of the system. One of the is `OP_MUL`, which would have allowed multiplication ... but is instead disabled.

So, how would you write an algebraic function like `3x + 7 = 13`?

The most obvious answer is to `OP_DUP` the number input from the locking script twice. Then you can push the `7` and keep adding until you get your total. The full locking script would look like this: `OP_DUP OP_DUP 7 OP_ADD OP_ADD OP_ADD 13 OP_EQUAL`.

Here's how it would run if executed with the correct unlocking script of `2`:
```
Script: 2 OP_DUP OP_DUP 7 OP_ADD OP_ADD OP_ADD 13 OP_EQUAL
Stack: [ ]

Script: OP_DUP OP_DUP 7 OP_ADD OP_ADD OP_ADD 13 OP_EQUAL
Stack: [ 2 ]

Script: OP_DUP 7 OP_ADD OP_ADD OP_ADD 13 OP_EQUAL
Running: 2 OP_DUP
Stack: [ 2 2 ]

Script: 7 OP_ADD OP_ADD OP_ADD 13 OP_EQUAL
Running: 2 OP_DUP
Stack: [ 2 2 2 ]

Script: OP_ADD OP_ADD OP_ADD 13 OP_EQUAL
Stack: [ 2 2 2 7 ]

Script: OP_ADD OP_ADD 13 OP_EQUAL
Running: 2 7 OP_ADD
Stack: [ 2 2 9 ]

Script: OP_ADD 13 OP_EQUAL
Running: 2 9 OP_ADD
Stack: [ 2 11 ]

Script: 13 OP_EQUAL
Running: 2 11 OP_ADD
Stack: [ 13 ]

Script: OP_EQUAL
Stack: [ 13 13 ]

Script: 
Running: 13 13 OP_EQUAL
Stack: [ True ]
```
Or if you prefer `btcdeb`:
```
$ btcdeb '[2 OP_DUP OP_DUP 7 OP_ADD OP_ADD OP_ADD 13 OP_EQUAL]'
btcdeb 0.2.19 -- type `btcdeb -h` for start up options
valid script
9 op script loaded. type `help` for usage information
script   |  stack 
---------+--------
2        | 
OP_DUP   | 
OP_DUP   | 
7        | 
OP_ADD   | 
OP_ADD   | 
OP_ADD   | 
13       | 
OP_EQUAL | 

#0000 2
btcdeb> step
		<> PUSH stack 02
script   |  stack 
---------+--------
OP_DUP   | 02
OP_DUP   | 
7        | 
OP_ADD   | 
OP_ADD   | 
OP_ADD   | 
13       | 
OP_EQUAL | 

#0001 OP_DUP
btcdeb> step
		<> PUSH stack 02
script   |  stack 
---------+--------
OP_DUP   |      02
7        | 02
OP_ADD   | 
OP_ADD   | 
OP_ADD   | 
13       | 
OP_EQUAL | 

#0002 OP_DUP
btcdeb> step
		<> PUSH stack 02
script   |  stack 
---------+--------
7        |      02
OP_ADD   |      02
OP_ADD   | 02
OP_ADD   | 
13       | 
OP_EQUAL | 

#0003 7
btcdeb> step
		<> PUSH stack 07
script   |  stack 
---------+--------
OP_ADD   |      07
OP_ADD   |      02
OP_ADD   |      02
13       | 02
OP_EQUAL | 

#0004 OP_ADD
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 09
script   |  stack 
---------+--------
OP_ADD   |      09
OP_ADD   |      02
13       | 02
OP_EQUAL | 

#0005 OP_ADD
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 0b
script   |  stack 
---------+--------
OP_ADD   |      0b
13       | 02
OP_EQUAL | 

#0006 OP_ADD
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 0d
script   |  stack 
---------+--------
13       | 0d
OP_EQUAL | 
#0007 13
btcdeb> step
		<> PUSH stack 0d
script   |  stack 
---------+--------
OP_EQUAL |      0d
         | 0d

#0008 OP_EQUAL
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 01
script   |  stack 
---------+--------
         | 01
```
### Write an Equation System

What if you wanted to instead write an equation system, such as `x + y = 3`, `y + z = 5`, and `x + z = 4`? A bit of algebra tells you that the answers come out to `x = 1`, `y = 2`, and `z = 3`. But, how do you script it?

Most obviously, after the redeemer inputs the three numbers, you're going to need two copies of each number, since each number goes into two different equations. `OP_3DUP` takes care of that and results in `x y z x y z` being on the stack. Popping off two items at a time will give you `y z`, `z x`, and `x y`. Voila! That's the three equations, so you just need to add them up and test them in the right order! Here's the full script: `OP_3DUP OP_ADD 5 OP_EQUALVERIFY OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL`.

Here's how it runs with the correct unlocking script of `1 2 3`:
```
Script: 1 2 3 OP_3DUP OP_ADD 5 OP_EQUALVERIFY OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Stack: [ ]

Script: OP_3DUP OP_ADD 5 OP_EQUALVERIFY OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Stack: [ 1 2 3 ]

Script: OP_ADD 5 OP_EQUALVERIFY OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Running: 1 2 3 OP_3DUP
Stack: [ 1 2 3 1 2 3 ]

Script: 5 OP_EQUALVERIFY OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Running: 2 3 OP_ADD
Stack: [ 1 2 3 1 5 ]

Script: OP_EQUALVERIFY OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Stack: [ 1 2 3 1 5 5 ]

Script: OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Running: 5 5 OP_EQUALVERIFY
Stack: [ 1 2 3 1 ] — Does Not Exit

Script: 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Running: 3 1 OP_ADD
Stack: [ 1 2 4 ]

Script: OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Stack: [ 1 2 4 4 ]

Script: OP_ADD 3 OP_EQUAL
Running: 4 4 OP_EQUALVERIFY
Stack: [ 1 2 ] — Does Not Exit

Script: 3 OP_EQUAL
Running: 1 2 OP_ADD
Stack: [ 3 ]

Script: OP_EQUAL
Stack: [ 3 3 ]

Script: 
Running: 3 3 OP_EQUAL
Stack: [ True ]
```
Here it is in `btcdeb`:
```
$ btcdeb '[1 2 3 OP_3DUP OP_ADD 5 OP_EQUALVERIFY OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL]'
btcdeb 0.2.19 -- type `btcdeb -h` for start up options
valid script
13 op script loaded. type `help` for usage information
script         |  stack 
---------------+--------
1              | 
2              | 
3              | 
OP_3DUP        | 
OP_ADD         | 
5              | 
OP_EQUALVERIFY | 
OP_ADD         | 
4              | 
OP_EQUALVERIFY | 
OP_ADD         | 
3              | 
OP_EQUAL       | 

#0000 1
btcdeb> step
		<> PUSH stack 01
script         |  stack 
---------------+--------
2              | 01
3              | 
OP_3DUP        | 
OP_ADD         | 
5              | 
OP_EQUALVERIFY | 
OP_ADD         | 
4              | 
OP_EQUALVERIFY | 
OP_ADD         | 
3              | 
OP_EQUAL       | 

#0001 2
btcdeb> step
		<> PUSH stack 02
script         |  stack 
---------------+--------
3              |      02
OP_3DUP        | 01
OP_ADD         | 
5              | 
OP_EQUALVERIFY | 
OP_ADD         | 
4              | 
OP_EQUALVERIFY | 
OP_ADD         | 
3              | 
OP_EQUAL       | 

#0002 3
btcdeb> step
		<> PUSH stack 03
script         |  stack 
---------------+--------
OP_3DUP        |      03
OP_ADD         |      02
5              | 01
OP_EQUALVERIFY | 
OP_ADD         | 
4              | 
OP_EQUALVERIFY | 
OP_ADD         | 
3              | 
OP_EQUAL       | 

#0003 OP_3DUP
btcdeb> step
		<> PUSH stack 01
		<> PUSH stack 02
		<> PUSH stack 03
script         |  stack 
---------------+--------
OP_ADD         |      03
5              |      02
OP_EQUALVERIFY |      01
OP_ADD         |      03
4              |      02
OP_EQUALVERIFY | 01
OP_ADD         | 
3              | 
OP_EQUAL       | 

#0004 OP_ADD
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 05
script         |  stack 
---------------+--------
5              |      05
OP_EQUALVERIFY |      01
OP_ADD         |      03
4              |      02
OP_EQUALVERIFY | 01
OP_ADD         | 
3              | 
OP_EQUAL       | 

#0005 5
btcdeb> step
		<> PUSH stack 05
script         |  stack 
---------------+--------
OP_EQUALVERIFY |      05
OP_ADD         |      05
4              |      01
OP_EQUALVERIFY |      03
OP_ADD         |      02
3              | 01
OP_EQUAL       | 

#0006 OP_EQUALVERIFY
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 01
		<> POP  stack
script         |  stack 
---------------+--------
OP_ADD         |      01
4              |      03
OP_EQUALVERIFY |      02
OP_ADD         | 01
3              | 
OP_EQUAL       | 

#0007 OP_ADD
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 04
script         |  stack 
---------------+--------
4              |      04
OP_EQUALVERIFY |      02
OP_ADD         | 01
3              | 
OP_EQUAL       | 

#0008 4
btcdeb> step
		<> PUSH stack 04
script         |  stack 
---------------+--------
OP_EQUALVERIFY |      04
OP_ADD         |      04
3              |      02
OP_EQUAL       | 01

#0009 OP_EQUALVERIFY
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 01
		<> POP  stack
script         |  stack 
---------------+--------
OP_ADD         |      02
3              | 01
OP_EQUAL       | 

#0010 OP_ADD
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 03
script         |  stack 
---------------+--------
3              | 03
OP_EQUAL       | 

#0011 3
btcdeb> step
		<> PUSH stack 03
script         |  stack 
---------------+--------
OP_EQUAL       |      03
               | 03

#0012 OP_EQUAL
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 01
script         |  stack 
---------------+--------
               | 01
```

> :warning: **WARNING** `btcdeb` isn't just useful for providing visualization of these scripts, but to also double-check the results. Sure enough, we got this one wrong the first time, testing the equations in the wrong order. That's how easy it is to make a financially fatal mistake in a Bitcoin Script, and that's why every script must be tested.

## Write Simple Computational Scripts

Though puzzle scripts are trivial, they can actually have real-world usefulness if you want to crowdsource a computation. You simply create a script that requires the answer to the computation and you send funds to the P2SH address as a reward. It'll stay there until someone comes up with the answer.

For example, Peter Todd [offered rewards](https://bitcointalk.org/index.php?topic=293382.0) for solving equations that demonstrate collisions for standard cryptographic algorithms. Here was his script for confirming a SHA1 collision: `OP_2DUP OP_EQUAL OP_NOT OP_VERIFY OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL`. It requires two inputs, which will be the two numbers that collide.

Here's how it runs with correct answers.

First, we fill in our stack:
```
Script: <numA> <numB> OP_2DUP OP_EQUAL OP_NOT OP_VERIFY OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL
Stack: [ ]

Script: OP_2DUP OP_EQUAL OP_NOT OP_VERIFY OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL
Stack: [ <numA> <numB> ]

Script: OP_EQUAL OP_NOT OP_VERIFY OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL
Running: <numA> <numB> OP_2DUP
Stack: [ <numA> <numB> <numA> <numB> ]
```
Then, we make sure the two numbers aren't equal, exiting if they are:
```
Script: OP_NOT OP_VERIFY OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL
Running: <numA> <numB> OP_EQUAL
Stack: [ <numA> <numB> False ]

Script: OP_VERIFY OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL
Running: False OP_NOT
Stack: [ <numA> <numB> True ]

Script: OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL
Running: True OP_VERIFY
Stack: [ <numA> <numB> ] — Does Not Exit
```
We now create two SHAs:
```
Script: OP_SWAP OP_SHA1 OP_EQUAL
Running: <numB> OP_SHA1
Stack: [ <numA> <hashB> ]

Script: OP_SHA1 OP_EQUAL
Running: <numA> <hashB> OP_SWAP
Stack: [ <hashB> <numA> ]

Script: OP_EQUAL
Running: <numA> OP_SHA1
Stack: [ <hashB> <hashA> ]
```
Finally, we see if they match.
```
Script: 
Running: <hashB> <hashA> OP_EQUAL
Stack: [ True ]
```
This is a nice script because it shows careful use of logic (with the `OP_NOT` and the `OP_VERIFY`) and good use of stack functions (with the `OP_SWAP`). It's all around a great example of a real-world function. And it is very real-world.  When [SHA-1 was broken](https://shattered.io/), 2.48 BTC were quickly liberated from the address, with a total value of about $3,000 at the time.

`btcdeb` can be run to prove the collision (and the script):
```
$ btcdeb '[255044462d312e330a25e2e3cfd30a0a0a312030206f626a0a3c3c2f57696474682032203020522f4865696768742033203020522f547970652034203020522f537562747970652035203020522f46696c7465722036203020522f436f6c6f7253706163652037203020522f4c656e6774682038203020522f42697473506572436f6d706f6e656e7420383e3e0a73747265616d0affd8fffe00245348412d3120697320646561642121212121852fec092339759c39b1a1c63c4c97e1fffe017f46dc93a6b67e013b029aaa1db2560b45ca67d688c7f84b8c4c791fe02b3df614f86db1690901c56b45c1530afedfb76038e972722fe7ad728f0e4904e046c230570fe9d41398abe12ef5bc942be33542a4802d98b5d70f2a332ec37fac3514e74ddc0f2cc1a874cd0c78305a21566461309789606bd0bf3f98cda8044629a1 255044462d312e330a25e2e3cfd30a0a0a312030206f626a0a3c3c2f57696474682032203020522f4865696768742033203020522f547970652034203020522f537562747970652035203020522f46696c7465722036203020522f436f6c6f7253706163652037203020522f4c656e6774682038203020522f42697473506572436f6d706f6e656e7420383e3e0a73747265616d0affd8fffe00245348412d3120697320646561642121212121852fec092339759c39b1a1c63c4c97e1fffe017346dc9166b67e118f029ab621b2560ff9ca67cca8c7f85ba84c79030c2b3de218f86db3a90901d5df45c14f26fedfb3dc38e96ac22fe7bd728f0e45bce046d23c570feb141398bb552ef5a0a82be331fea48037b8b5d71f0e332edf93ac3500eb4ddc0decc1a864790c782c76215660dd309791d06bd0af3f98cda4bc4629b1 OP_2DUP OP_EQUAL OP_NOT OP_VERIFY OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL]'
```

Peter Todd's other [bounties](https://bitcointalk.org/index.php?topic=293382.0) remain unclaimed at the time of this writing. They're all written in the same manner as the SHA-1 example above.

## Understand the Limitations of Puzzle Scripts

Puzzle scripts are great to further examine Bitcoin Scripting, but you'll only see them in real-world use if they're holding small amounts of funds or if they're intended for redemption by very skilled users. There's a reason for this: they aren't secure. 

Here's where the security falls down:

First, anyone can redeem them without knowing much of a secret. They do have to have the `redeemScript`, which offers some protection, but once they do, that's probably the only secret that's necessary — unless your puzzle is _really_ tough, such as a computational puzzle.

Second, the actual redemption isn't secure. Normally, a Bitcoin transaction is protected by the signature. Because the signature covers the transaction, no one on the network can rewrite that transaction to instead send to their address without invalidating the signature (and thus the transaction). That isn't true with a transactions whose inputs are just numbers. Anyone could grab the transaction and rewrite it to allow them to steal the funds. If they can get their transaction into a block before yours, they win, and you don't get the puzzle money. There are solutions for this, but they involve mining the block yourself or having a trusted pool mine it, and neither of those options is rational for an average Bitcoin user. 

Yet, Peter Todd's cryptographic bounties prove that puzzle scripts do have some real-world application.

## Summary: Writing Puzzle Scripts

Puzzles scripts are a great introduction to more realistic and complex Bitcoin Scripts. They demonstrate the power of the mathematical and stack functions in Bitcoin Script and how they can be carefully combined to create questions that require very specific answers. However, their real-world usage is also limited by the security issues inherent in non-signed Bitcoin transactions.

> :fire: ***What is the power of puzzle script?*** Despite their limitations, puzzles scripts have been used in the real world as the prizes for computational bounties. Anyone who can figure out a complex puzzle, whose solution presumably has some real-world impact, can win the bounty. Whether they get to actually keep it is another question.

## What's Next?

Continue "Designing Real Bitcoin Scripts" with [§13.2: Writing Complex Multisig Scripts](13_2_Writing_Complex_Multisig_Scripts.md).
