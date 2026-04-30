# 9.2: Running a Bitcoin Script

Bitcoin Scripts may not initially seem that intuitive, but their execution is quite simple, using reverse Polish notation and a stack.

## Understand the Scripting Language

A Bitcoin Script has three parts: it has a line of input; it has a stack for storage; and it has specific commands for execution.

### Understand the Ordering

Bitcoin Scripts are run from left to right. That sounds easy enough, because it's the same way you read. However, it might actually be the most non-intuitive element of Bitcoin Script, because it means that functions don't look like you'd expect. Instead, _the operands go before the operator._

For example, if you were adding together "1" and "2", your Bitcoin Script for that would be `1 2 OP_ADD`, _not_ "1 + 2". Since we know that OP_ADD operator takes two inputs, we know that the two inputs before it are its operands.

> :warning: **WARNING:** Technically, everything in Bitcoin Script is an opcode, thus it would be most appropriate to record the above example as `OP_1 OP_2 OP_ADD`. In our examples, we don't worry about how the constants will be evaluated, as that's a topic of translation, as is explained in [§10.2: Building the Structure of P2SH](10_2_Building_the_Structure_of_P2SH.md). Some writers prefer to also leave the "OP" prefix off all operators, but we have opted not to.

### Understand the Stack

It's actually not quite correct to say that an operator applies to the inputs before it. Really, an operator applies to the top inputs in Bitcoin's stack.

> :book: ***What is a stack?*** A stack is a LIFO (last-in-first-out) data structure. It has two access functions: push and pop. Push places a new object on top of the stack, pushing down everything below it. Pop removes the top object from the stack.

Whenever Bitcoin Script encounters a constant, it pushes it on the stack. So the above example of `1 2 OP_ADD` would actually look like this as it was processed:
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

When a Bitcoin Script encounters an operator, it evaluates it. Each operator pops zero or more elements off the stack as inputs, usually one or two. It then processes them in a specific way before pushing zero or more elements back on the stack, usually one or two.

> :book: ***What is an Opcode?*** Opcode stands for "operation code". It's typically associated with machine-language code, and is a simple function (or "operator").

OP_ADD pops two items off the stack (here: 2 then 1), adds then together, and pushes the result back on the stack (here: 3).
```
Script:
Running: 1 2 OP_ADD
Stack: [ 3 ]
```

## Build Up Complexity

More complex scripts are created by running more commands in order. They need to be carefully evaluated from left to right, so that you can understand the state of the stack as each new command is run. It will constantly change, as a result of previous operators:
```
Script: 3 2 OP_ADD 4 OP_SUB
Stack: [ ]

Script: 2 OP_ADD 4 OP_SUB
Stack: [ 3 ]

Script: OP_ADD 4 OP_SUB
Stack: [ 3 2 ]

Script: 4 OP_SUB
Running: 3 2 OP_ADD
Stack: [ 5 ]

Script: OP_SUB
Stack: [ 5 4 ]

Script: 
Running: 5 4 OP_SUB
Stack: [ 1 ]
```

## Understand the Usage of Bitcoin Script

That's pretty much Bitcoin Scripting ... other than a few intricacies for how this scripting language interacts with Bitcoin itself.

### Understand scriptSig and scriptPubKey

As we've seen, every input for a Bitcoin transaction contains a `scriptSig` that is used to unlock the `scriptPubKey` for the associated UTXO. They are _effectively_ concatenated together, meaning that `scriptSig` and `scriptPubKey` are run together, in that order.

So, presume that a UTXO were locked with a `scriptPubKey` that read `OP_ADD 99 OP_EQUAL`, requiring as input two numbers that add up to ninety-nine, and presume that the `scriptSig` of `1 98` were run to unlock it. The two scripts would effectively be run in order as `1 98 OP_ADD 99 OP_EQUAL`.

Evaluate the result:
```
Script: 1 98 OP_ADD 99 OP_EQUAL
Stack: []

Script: 98 OP_ADD 99 OP_EQUAL
Stack: [ 1 ]

Script: OP_ADD 99 OP_EQUAL
Stack: [ 1 98 ]

Script: 99 OP_EQUAL
Running: 1 98 OP_ADD
Stack: [ 99 ]

Script: OP_EQUAL
Stack: [ 99 99 ]

Script: 
Running: 99 99 OP_EQUAL
Stack: [ True ]
```
This abstraction isn't quite accurate: for security reasons, the `scriptSig` is run, then the contents of the stack are transferred for the `scriptPubKey` to run, but it's accurate enough for understanding how the key of `scriptSig` fits into the lock of `scriptPubKey`.

> :warning: **WARNING** The above is a non-standard transaction type. It would not actually be accepted by nodes running Bitcoin Core with the standard settings. [§10.1: Building a Bitcoin Script with P2SH](10_1_Understanding_the_Foundation_of_P2SH.md) discusses how you actually _could_ run a Bitcoin Script like this, using the power of P2SH.

### Get the Results

Bitcoin will verify a transaction and allow the UTXO to be respent if two criteria are met when running `scriptSig` and `scriptPubKey`:

   1. The execution did not get marked as invalid at any point, for example with a failed OP_VERIFY or the usage of a disabled opcode.
   2. The top item in the stack at the end of execution is true (non-zero).
   
In the above example, the transaction would succeed because the stack has a `True` at its top. But, it would be just as permissible to end with a full stack and the number `42` on top.

## Summary: Running a Bitcoin Script

To process a Bitcoin Script, a `scriptSig` is run followed by the `scriptPubKey` that it's unlocking. These commands are run in order, from left to right, with constants being pushed onto a stack and operators popping elements off that stack, then pushing results back onto it. If the Script doesn't halt in the middle and if the item on top of the stack at the end is non-zero, then the UTXO is unlocked.

## What's Next?

Continue "Introducing Bitcoin Scripts" with [§9.3: Testing a Bitcoin Script](09_3_Testing_a_Bitcoin_Script.md).
