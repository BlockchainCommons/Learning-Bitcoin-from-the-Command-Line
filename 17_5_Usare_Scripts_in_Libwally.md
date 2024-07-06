# 17.5: Using Scripts in Libwally

> :information_source: **NOTE:** This section has been recently added to the course and is an early draft that may still be awaiting review. Caveat reader.

Way back in Part 3, while introducing Scripts, we said that you were likely to actually create transactions using scripts with an API, and marked it as a topic for the future. Well, the future has now arrived.

## Create the Script

Creating the script is the _easiest_ thing to do in Libwally. Take the following example, a simple [Puzzle Script](/13_1_Writing_Puzzle_Scripts.md) that we've returned to from time to time:
```
OP_ADD 99 OP_EQUAL
```
Using `btcc`, we can serialize that. 
```
$ btcc OP_ADD 99 OP_EQUAL
warning: ambiguous input 99 is interpreted as a numeric value; use 0x99 to force into hexadecimal interpretation
93016387
```
Previously we built the standard P2SH script by hand, but Libwally can actually do that for you.

First, Libwally has to convert the hex into bytes, since bytes are most of what it works with:
```
  int script_length = strlen(script)/2;
  unsigned char bscript[script_length];
    
  lw_response = wally_hex_to_bytes(script,bscript,script_length,&written);
```
Then, you run `wally_scriptpubkey_p2sh_from_bytes` with your bytes, telling Libwally to also `HASH160` it for you:
```
  unsigned char p2sh[WALLY_SCRIPTPUBKEY_P2SH_LEN];
  			  
  lw_response = wally_scriptpubkey_p2sh_from_bytes(bscript,sizeof(bscript),WALLY_SCRIPT_HASH160,p2sh,WALLY_SCRIPTPUBKEY_P2SH_LEN,&written);
```
If you looked at the results of `p2sh`, you'd see it was:
```
a9143f58b4f7b14847a9083694b9b3b52a4cea2569ed87
```
Which [you may recall](10_2_Building_the_Structure_of_P2SH.md) breaks apart to:
```
a9 / 14 / 3f58b4f7b14847a9083694b9b3b52a4cea2569ed / 87
```
That's our old friend `OP_HASH160 3f58b4f7b14847a9083694b9b3b52a4cea2569ed OP_EQUAL`.

Basically, Libwally took your serialized redeem script, hashed it for you with SHA-256 and RIPEMD-160, and the applied the standard framing to turn it into a proper P2SH; You did similar work in [§10.2](10_2_Building_the_Structure_of_P2SH.md), but with an excess of shell commands.

In fact, you can double-check your work using the same commands from §10.2:
```
$ redeemScript="93016387"
$ echo -n $redeemScript | xxd -r -p | openssl dgst -sha256 -binary | openssl dgst -rmd160
(stdin)= 3f58b4f7b14847a9083694b9b3b52a4cea2569ed
```

## Create a Transaction

In order to make use of that `pubScriptKey` that you just created, you need to create a transaction and embed the `pubScriptKey` within (and this is the big change from `bitcoin-cli`: you can actually hand create a transaction with a P2SH script).

The process of creating a transaction in Libwally is very intensive, just like the process for creating a PSBT, and so we're just going to outline it, taking one major shortcut, and then leave a method without shortcuts for future investigation.

Creating a transaction itself is easy enough: you just need to tell `wally_tx_init_alloc` your version number, your locktime, and your number of inputs and outputs:
```
  struct wally_tx *tx;
  lw_response = wally_tx_init_alloc(2,0,1,1,&tx);
```

Filling in those inputs and outputs is where things get tricky!

### Create a Transaction Output

To create an output, you tell `wally_tx_output_init_alloc` how many satoshis you're spending and you hand it the locking script:
```
  struct wally_tx_output *tx_output;
  lw_response = wally_tx_output_init_alloc(95000,p2sh,sizeof(p2sh),&tx_output);
```
That part actually wasn't hard at all, and it allowed you to at long-last embed a P2SH in a `vout`.

One more command adds it to your transaction:
```
  lw_response = wally_tx_add_output(tx,tx_output);
```

### Create a Transaction Input

Creating the input is much harder because you have to pile information into the creation routines, not all of which is intuitively accessible when you're using Libwally. So, rather than going that deep into the weeds, here's where we take our shortcut. We write our code so that it's passed the hex code for a transaction that's already been created, and then we just reuse the input.

The conversion from the hex code is done with `wally_tx_from_hex`:
```
  struct wally_tx *utxo;
  lw_response = wally_tx_from_hex(utxo_hex,0,&utxo);
```
Then you can plunder the inputs from your hexcode to create an input with Libwally:
```
  struct wally_tx_input *tx_input;
  lw_response = wally_tx_input_init_alloc(utxo->inputs[0].txhash,sizeof(utxo->inputs[0].txhash),utxo->inputs[0].index,0,utxo->inputs[0].script,utxo->inputs[0].script_len,utxo->inputs[0].witness,&tx_input);
  assert(lw_response == WALLY_OK);						
```
As you might expect, you then add that input to your transaction:
```
  lw_response = wally_tx_add_input(tx,tx_input);
```

> **NOTE** Obviously, you'll want to be able to create your own inputs if you're using Libwally for real applications, but this is intended as a first step. And, it can actually be useful for integrating with `bitcoin-cli`, as we'll see in [§16.7](17_7_Integrating_Libwally_and_Bitcoin-CLI.md).

### Print a Transaction

You theoretically could sign and send this transaction from your C program built on Libwally, but in keeping with the idea that we're just using a simple C program to substitute in a P2SH, we're going to print out the new hex. This is done with the help of `wally_tx_to_hex`:
```
  char *tx_hex;
  lw_response = wally_tx_to_hex(tx,0, &tx_hex);

  printf("%s\n",tx_hex);
```
We'll show how to make use of that in §16.7.

## Test Your Replacement Script

You can grab the test code from the [src directory](src/17_5_replacewithscript.c) and compile it:
```
$  cc replacewithscript.c -lwallycore -o replacewithscript
```
Afterward, prepare a hex transaction and a serialized hex script:
```
hex=020000000001019527cebb072524a7961b1ba1e58fc18dd7c6fc58cd6c1c45d7e1d8fc690b006e0000000017160014cc6e8522f0287b87b7d0a83629049c2f2b0e972dfeffffff026f8460000000000017a914ba421212a629a840492acb2324b497ab95da7d1e87306f0100000000001976a914a2a68c5f9b8e25fdd1213c38d952ab2be2e271be88ac02463043021f757054fa61cfb75b64b17230b041b6d73f25ff9c018457cf95c9490d173fb4022075970f786f24502290e8a5ed0f0a85a9a6776d3730287935fb23aa817791c01701210293fef93f52e6ce8be581db62229baf116714fcb24419042ffccc762acc958294e6921b00

script=93016387
```
You can then run the replacement program:
```
$ ./replacewithscript $hex $script
02000000019527cebb072524a7961b1ba1e58fc18dd7c6fc58cd6c1c45d7e1d8fc690b006e0000000017160014cc6e8522f0287b87b7d0a83629049c2f2b0e972d0000000001187301000000000017a9143f58b4f7b14847a9083694b9b3b52a4cea2569ed8700000000
```
You can then see the results with `bitcoin-cli`:
```
$ bitcoin-cli decoderawtransaction $newhex
{
  "txid": "f4e7dbab45e759a7ac6e2fb0f10720cd29d047efad89fe1b569f5f4ba61fd8e6",
  "hash": "f4e7dbab45e759a7ac6e2fb0f10720cd29d047efad89fe1b569f5f4ba61fd8e6",
  "version": 2,
  "size": 106,
  "vsize": 106,
  "weight": 424,
  "locktime": 0,
  "vin": [
    {
      "txid": "6e000b69fcd8e1d7451c6ccd58fcc6d78dc18fe5a11b1b96a7242507bbce2795",
      "vout": 0,
      "scriptSig": {
        "asm": "0014cc6e8522f0287b87b7d0a83629049c2f2b0e972d",
        "hex": "160014cc6e8522f0287b87b7d0a83629049c2f2b0e972d"
      },
      "sequence": 0
    }
  ],
  "vout": [
    {
      "value": 0.00095000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_HASH160 3f58b4f7b14847a9083694b9b3b52a4cea2569ed OP_EQUAL",
        "hex": "a9143f58b4f7b14847a9083694b9b3b52a4cea2569ed87",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2My2ApqGcoNXYceZC4d7fipBu4GodkbefHD"
        ]
      }
    }
  ]
}
```
The `vin` should just match the input you substituted in, but it's the `vout` that's exciting: you've created a transaction with a `scripthash`!

## Summary: Using Scripts in Libwally

Creating transactions in Libwally is another topic that could take up a whole chapter, but the great thing is that once you make this leap, you can introduce a P2SH `scriptPubKey`, and that part alone is pretty easy. Though the methodology detailed in this chapter requires you to have a transaction hex already in hand (probably created with `bitcoin-cli`) if you dig further into Libwally, you can do it all yourself.

> :fire: ***What is the Power of Scripts in Libwally?*** Quite simply, you can do something you couldn't before: create a transaction locked with an arbitrary P2SH.

## What's Next?

Learn more about "Programming Bitcoin with Libwally" in [§17.6: Using Other Functions in Libwally](17_6_Using_Other_Functions_in_Libwally.md).
