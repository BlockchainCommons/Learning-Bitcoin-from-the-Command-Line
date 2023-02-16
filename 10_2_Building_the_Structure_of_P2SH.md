# 10.2: Building the Structure of P2SH

In the previous section we overviewed the theory of how to create P2SH transactions to hold Bitcoin Scripts. The actual practice of doing so is _much more difficult_, but for the sake of completeness, we're going to look at it here. This is probably not something you'd ever do without an API, so if it gets too intimidating, be aware that we'll be returning to pristine, high-level Scripts in a moment.

## Create a Locking Script

Any P2SH transaction starts with a locking script. This is the subject of chapters 9 and 11-12. You can use any of the Bitcoin Script methods described therein to create any sort of locking script, as long as the resulting serialized `redeemScript` is 520 bytes or less. 

> :book: ***Why are P2SH scripts limited to 520 bytes?*** As with many things in Bitcoin, the answer is backward compatibility: new functionality has to constantly be built within the old constraints of the system. In this case, 520 bytes is the maximum that can be pushed onto the stack at once. Since the whole redeemScript is pushed onto the stack as part of the redemption process, it hits that limit.

## Serialize a Locking Script the Hard Way

After you create a locking script, you need to serialize it before it can be input into Bitcoin. This is a two-part process. First, you must turn it into hexcode, then you must transform that hex into binary.

### Create the Hex Code

Creating the hexcode that is necessary to serialize a script is both a simple translation and something that is complex enough that it goes beyond any shell script that you're likely to write. This step is one of the main reasons that you need an API to create P2SH transactions.

You create hexcode by stepping through your locking script and turning each element into one-byte hex command, possibly followed by additional data, per the guide at the [Bitcoin Wiki Script page](https://en.bitcoin.it/wiki/Script):

* Operators are translated to the matching byte for that opcode
* The constants 1-16 are translated to opcodes 0x51 to 0x61 (OP_1 to OP_16)
* The constant -1 is translate to opcode 0x4f (OP_1NEGATE)
* Other constants are preceded by opcodes 0x01 to 0x4e (OP_PUSHDATA, with the number specifying how many bytes to push)
   * Integers are translated into hex using little-endian signed-magnitude notation

### Translate Integers

The integers are the most troublesome part of a locking-script translation.

First, you should verify that your number falls between -2147483647 and 2147483647, the range of four-byte integers when the most significant byte is used for signing.

Second, you need to translate the decimal value into hexadecimal and pad it out to an even number of digits. This can be done with the `printf` command:
```
$ integer=1546288031
$ hex=$(printf '%08x\n' $integer | sed 's/^\(00\)*//')
$ echo $hex
5c2a7b9f
```
Third, you need to add an additional leading byte of `00` if the top digit is "8" or greater, so that the number is not interpreted as negative.
```
$ hexfirst=$(echo $hex | cut -c1)
$ [[ 0x$hexfirst -gt 0x7 ]] && hex="00"$hex
```
Fourth, you need to translate the hex from big-endian (least significant byte last) to little-endian (least significant byte first). You can do this with the `tac` command:
```
$ lehex=$(echo $hex | tac -rs .. | echo "$(tr -d '\n')")
$ echo $lehex
9f7b2a5c
```
In addition, you always need to know the size of any data that you put on the stack, so that you can precede it with the proper opcode. You can just remember that every two hexadecimal characters is one byte. Or, you can use `echo -n` piped to `wc -c`, and divide that in half:
```
$ echo -n $lehex | wc -c | awk '{print $1/2}'
4
```
With that whole rigamarole, you'd know that you could translate the integer 1546288031 into an `04` opcode (to push four bytes onto the stack) followed by `9f7b2a5c` (the little-endian hex representation of 1546288031).

If you instead had a negative number, you would need to (1) do your calculations on the absolute value of the number, then (2) bitwise-or 0x80 to your final, little-endian result. For example, `9f7b2a5c`, which is 1546288031, would become `9f7b2adc`, which is -1546288031:
```
$ neglehex=$(printf '%x\n' $((0x$lehex | 0x80)))
$ echo $neglehex
9f7b2adc
```
### Transform the Hex to Binary

To complete your serialization, you translate the hexcode into binary. On the command line, this just requires a simple invocation of `xxd -r -p`. However, you probably want to do that as part of a a single pipe that will also hash the script ...

## Run The Integer Conversion Script

A complete script for changing an integer between -2147483647 and 2147483647 to a little-endian signed-magnitude representation in hex can be found in the [src code directory](src/10_2_integer2lehex.sh). You can download it as `integer2lehex.sh`.

> :warning: **WARNING:** This script has not been robustly checked. If you are going to use it to create real locking scripts you should make sure to double-check and test your results.

Be sure the permissions on the script are right:
```
$ chmod 755 integer2lehex.sh
```
You can then run the script as follows:
```
$ ./integer2lehex.sh 1546288031
Integer: 1546288031
LE Hex: 9f7b2a5c
Length: 4 bytes
Hexcode: 049f7b2a5c

$ ./integer2lehex.sh -1546288031
Integer: -1546288031
LE Hex: 9f7b2adc
Length: 4 bytes
Hexcode: 049f7b2adc
```

## Analyze a P2SH Multisig

To better understand this process, we will reverse-engineer the P2SH multisig that we created in [§6.1: Sending a Transaction to a Multisig](06_1_Sending_a_Transaction_to_a_Multisig.md). Take a look at the `redeemScript` that you used, which you now know is the hex-serialized version of the locking script:
```
522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae
```
You can translate this back to Script by hand using the [Bitcoin Wiki Script page](https://en.bitcoin.it/wiki/Script) as a reference. Just look at one byte (two hex characters) of data at a time, unless you're told to look at more by an OP_PUSHDATA command (an opcode in the range of 0x01 to 0x4e).

The whole Script will break apart as follows:
```
52 / 21 / 02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191 / 21 / 02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3 / 52 / ae
```
Here's what the individual parts mean:

* 0x52 = OP_2
* 0x21 = OP_PUSHDATA 33 bytes (hex: 0x21)
* 0x02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191 = the next 33 bytes (public-key hash)
* 0x21 = OP_PUSHDATA 33 bytes (hex: 0x21)
* 0x02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3 = the next 33 bytes (public-key hash)
* 0x52 = OP_2
* 0xae = OP_CHECKMULTISIG

In other words, that `redeemScript` was a translation of of `2 02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191 02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3 2 OP_CHECKMULTISIG`. We'll return to this script in [§10.4: Scripting a Multisig](10_4_Scripting_a_Multisig.md) when we detail exactly how multisigs work within the P2SH paradigm.

If you'd like a mechanical hand with this sort of translation in the future, you can use `bitcoin-cli decodescript`:
```
$ bitcoin-cli -named decodescript hexstring=522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae
{
  "asm": "2 02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191 02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3 2 OP_CHECKMULTISIG",
  "reqSigs": 2,
  "type": "multisig",
  "addresses": [
    "mmC2x2FoYwBnVHMPRUAzPYg6WDA31F1ot2",
    "mhwZFJUnWqTqy4Y7pXVum88qFtUnVG1keM"
  ],
  "p2sh": "2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr",
  "segwit": {
    "asm": "0 6fe9f451ccedb8e4090b822dcad973d0388a37b4c89fd1aed485110adecab2a9",
    "hex": "00206fe9f451ccedb8e4090b822dcad973d0388a37b4c89fd1aed485110adecab2a9",
    "reqSigs": 1,
    "type": "witness_v0_scripthash",
    "addresses": [
      "tb1qdl5lg5wvakuwgzgtsgku4ktn6qug5da5ez0artk5s5gs4hk2k25szvjky9"
    ],
    "p2sh-segwit": "2NByn92W1vH5oQC1daY69F5sU7PEStKKQBR"
  }
}
```
It's especially helpful for checking your work when you're serializing.

## Serialize a Locking Script the Easy Way

When you installed `btcdeb` in [§9.3](09_3_Testing_a_Bitcoin_Script.md) you also installed `btcc` which can be used to serialize Bitcoin scripts:
```
$ btcc 2 02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191 02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3 2 OP_CHECKMULTISIG
warning: ambiguous input 2 is interpreted as a numeric value; use OP_2 to force into opcode
warning: ambiguous input 2 is interpreted as a numeric value; use OP_2 to force into opcode
522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae
```
That's a lot easier than figuring that out by hand!

Also consider the Python [Transaction Script Compiler](https://github.com/Kefkius/txsc), which translates back and forth.

## Hash a Serialized Script

After you've created a locking script and serialized it, the third step in creating a P2SH transaction is to hash the locking script. As previously noted, a 20-byte OP_HASH160 hash is created through a combination of a SHA-256 hash and a RIPEMD-160 hash. Hashing a serialized script thus takes two commands: `openssl dgst -sha256 -binary` does the SHA-256 hash and outputs a binary to be sent through the pipe, then `openssl dgst -rmd160` takes that binary stream, does a RIPEMD-160 hash, and finally outputs a human-readable hexcode.

Here's the whole pipe, including the previous transformation of the hex-serialized script into binary:
```
$ redeemScript="522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae"
$ echo -n $redeemScript | xxd -r -p | openssl dgst -sha256 -binary | openssl dgst -rmd160
(stdin)= a5d106eb8ee51b23cf60d8bd98bc285695f233f3
```
## Create a P2SH Transaction

Creating your 20-byte hash just gives you the hash at the center of a P2SH locking script. You still need to put it together with the other opcodes that create a standard P2SH transaction: `OP_HASH160 a5d106eb8ee51b23cf60d8bd98bc285695f233f3 OP_EQUAL`.

Depending on your API, you might be able to enter this as an `asm`-style `scriptPubKey` for your transaction, or you might have to translate it to `hex` code as well. If you have to translate, use the same methods described above for "Creating the Hex Code" (or use `btcc`), resulting in `a914a5d106eb8ee51b23cf60d8bd98bc285695f233f387`.

Note that the `hex scriptPubKey` for P2SH Script transaction will _always_ start with an `a914`, which is the `OP_HASH160` followed by an `OP_PUSHDATA` of 20 bytes (hex: `0x14`); and it will _always_ end with a `87`, which is an `OP_EQUAL`. So all you have to do is put your hashed redeem script in between those numbers.

## Summary: Building the Structure of P2SH

Actually creating the P2SH locking script dives further into the guts of Bitcoin than you've ever gone before. Though it's helpful to know how all of this works at a very low level, it's most likely that you'll have an API taking care of all of the heavy-lifting for you. Your task will simply be to create the Bitcoin Script to do the locking ... which is the main topic of chapters 9 and 11-12.

## What's Next?

Continue "Embedding Bitcoin Scripts" with [§10.3: Running a Bitcoin Script with P2SH](10_3_Running_a_Bitcoin_Script_with_P2SH.md).
