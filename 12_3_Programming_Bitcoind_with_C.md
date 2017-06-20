# 12.3: Programming Bitcoind with C

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

[§12.2](12_2_Accessing_Bitcoind_with_C.md) laid out the methodology for creating C programs based on RPC calls to `bitcoind`. We're now going to show the potential of that C programming by laying out a simplistic, first cut of an actual Bitcoin program.

## Plan for Your Code

We're going to program a simplistic first cut version of `sendtoaddress`, which will allow a user to send money to an address as long as he has a big enough UTXO. Here's what we need to do:

  1. Request an address and an amount
  2. Set an arbitrary fee
  3. Find a UTXO that's large enough for the amount + the fee
  4. Create a change address
  5. Create a raw transaction that sends from the UTXO to the address and the change address
  6. Sign the transaction
  7. Send the transaction
  
### Plan for Your Future

Since this is our first functional C program, we're going to try and keep it simple (KISS). If we were producing an actual production program, we'd at least want to do the following:

   1. Test and/or sanitize the inputs
   2. Calculate a fee automatically
   3. Combine multiple UTXOs if necessary
   
If you want to continue to expand this example, these would be an excellent place to start, especially the latter points, which will approve your understanding and usage of actual RPC commands.
