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

## Write Your Transaction Software

We're now going to take that plan step by step

### 1. Request an Address and an Amount

Inputting the information is easy enough via command line arguments:
```
if (argc != 3) {

  printf("ERROR: Only %i arguments! Correct usage is '%s [recipient] [amount]'\n",argc-1,argv[0]);
  exit(-1);

}

char *tx_recipient = argv[1];
float tx_amount = atof(argv[2]);

printf("Sending %4.8f BTC to %s\n",tx_amount,tx_recipient);
```

> **WARNING:** A real program would need much better sanitization of these variables. 

### 2. Set an Arbitrary Fee

We're just setting the 0.0005 BTC fee that we've reguarly used to ensure that our test transactions go through quickly:
```
float	tx_fee = 0.0005;
float	tx_total =	tx_amount + tx_fee;
```

> **WARNING:** A real program would calculate a fee that minimized cost while ensuring the speed was sufficient for the sender. 

### X. Prepare Your RPC

### 3. Find a UTXO

