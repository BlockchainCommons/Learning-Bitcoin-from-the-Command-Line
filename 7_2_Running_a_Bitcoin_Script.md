# 7.2: Running a Bitcoin Script

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Bitcoin Scripts are very simple executed, using reverse Polish notation and a stack.

## Understand the Scripting Language

A Bitcoin Script has three parts: it has a line of input; it has stack for storage; and it has specific commands for execution.

### Understand the Ordering

Bitcoin Scripts are run from left to right. That sounds easy enough, as it's the same way you read. However, this might be the most intimidating element of Bitcoin Script, because it means that mathematical operators don't look like you'd expect. Instead, _the operands go before the operator._

That means that if you were adding together "1" and "2", your Bitcoin Script for that would be `1 2 OP_ADD`, _not_ "1 + 2". Since we know that OP_ADD operator takes two inputs, we know that the two inputs before it are its operands. At least that's true at the most basical level of Scripting.

> **WARNING:** Technically, everything in Bitcoin Script is an opcode, thus it would be most appropriate to record the above example as `OP_1 OP_2 OP_ADD`. We leave the OP_ prefix off constants, but include it on all actual operators. Some writers prefer to also leave the OP_ prefix off operators, but we have opted not to.

### Understand the Stack

It's actually not quite correct to say that an operator applies to the inputs before it. Instead, an operator actually applies to the top inputs in Bitcoin's stack.

_What is a stack?_ A stack is a LIFO (last-in-first-out) data structure. It has two access functions: push and pop. Push places a new object on top of the stack, pushing down everything below it. Pop removes the top object from the stack.

Whenever Bitcoin Script encounters a constant, it's pushed onto the Stack. So the above example of `1 2 OP_ADD1` would actually look like this as it was processed:
```
Script: 1 2 OP_ADD
Stack: [ ]

Script: 2 OP_ADD
Stack: [ 1 ]

Script: OP_ADD
Stack: [ 1 2 ]
```
_Note that in this and in following examples the top of the stack is to the right and the bottom is to the left._

### Understand the Opcodes

So in this paradigm, what do opcodes do? They pop zero or more elements off the stack as inputs, usually one or two. They process their operator. Then, they push zero or more elements back on the stack, usually one or two.

OP_ADD pops two items off the stack (here: 2 then 1), adds then, and then pushes the resul (here: 3).
```
Script:
Stack: [ 3 ]
```

## Run a Simple Script

Here's an example of a more complex script. It shows how operators continue to interact with the Stack, not just with the operands right before them:
```
Script: 3 2 OP_ADD 4 OP_SUB
Stack: [ ]

Script: 2 OP_ADD 4 OP_SUB
Stack: [ 3 ]

Script: OP_ADD 4 OP_SUB
Stack: [ 3 2 ]

Script: 4 OP_SUB
Stack: [ 5 ]

Script: OP_SUB
Stack: [ 5 4 ]

Script: 
Stack: [ 1 ]
```

## Understand the Usage of Bitcoin Script

That's pretty much Bitcoin Scripting ... other than a few intricacies for how this Scripting language interacts with Bitcoin itself.

### Understand scriptSig and scriptPubKey

As we've seen, every input for a Bitcoin transaction conttains a `scriptSig` which is used to unlock the `scriptPubKey` for its UTXO. The easy way to think of this is that scriptSig is run, then the scriptPubKey is run.

So, presume that a UTXO were locked with a `scriptPubKey` that read `100 OP_EQUAL` and that the `scriptSig` `1 99 OP_ADD` were run to unlock it. The two scripts would efectively be run in order as `1 99 OP_ADD 100 OP_EQUAL` and the following were occur:

```
Script: 1 99 OP_ADD 100 OP_EQUAL
Stack: []

Script: 99 OP_ADD 100 OP_EQUAL
Stack: [1]

Script: OP_ADD 100 OP_EQUAL
Stack: [1 99]

Script: 100 OP_EQUAL
Stack: [100]

Script: OP_EQUAL
Stack: [100 100]

Script: 
Stack: [TRUE]
```
This abstraction isn't quite true. For security reasons, the `scriptSig` is run, then the contents of the stack are transferred for the `scriptPubKey` to run, but it's correct enough for understanding how the key of `scriptSig` fits into the lock of `scriptPubKey`.

> **WARNING** The above is a non-standard transaction type. It would not actually be accepted by nodes running Bitcoin Core with its standard settings. It's included solely for pedagogical purposes. The only sorts of unlocking scripts that are likely to be widely accepted are pay-to-public-key-hash (P2PKH), the obsolete pay-to-public-key (P2PK), pay-to-script-hash (P2SH), multisigs, and null data (OP_RETURN). That probably sounds limiting: the good news is that you can put complex Bitcoin scripts into P2SH addresses, as we'll first examine in Chapte r8.

### Get the Results

Bitcoin will verify a transaction, and allow the UTXO to be respent, if two criteria are met when running `scriptSig` and `scriptPubKey`:

   1. The execution did not get marked as invalid at any point, for example with a failed OP_VERIFY or the usage of a disabled opcode.
   2. The top item in the stack at the end of execution is true (non-zero).
   
In the above example, the transaction would succeed because the stack has a `TRUE` at its top.

## Summary: Running a Bitcoin Script

To process a Bitcoin Script, a `scriptSig` is run followed by the `scriptPubKey` that it's unlocking. These commands are run in order, from left to right, with constants being pushed onto a stack and operators popping elements off that stack, then pushing results back on. If the Script doesn't halt in the middle and if the item on top of the stack at the end if `TRUE`, then the UTXO is unlocked.
