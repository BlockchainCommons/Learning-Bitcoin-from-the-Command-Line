# 17.4: Using PSBTs in Libwally

> :information_source: **NOTE:** This section has been recently added to the course and is an early draft that may still be awaiting review. Caveat reader.

You learned all about Partially Signed Bitcoin Transactions (PSBTs) in [§7.1](07_1_Creating_a_Partially_Signed_Bitcoin_Transaction.md) and [§7.2](07_2_Using_a_Partially_Signed_Bitcoin_Transaction.md), and as you saw in [§7.3: Integrating with Hardware Wallets](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/07_3_Integrating_with_Hardware_Wallets.md), one of their prime advantages is being able to integrate with offline nodes, such as Hardware Wallets. HWI allowed you to pass commands to a Hardware Wallet, but what does the wallet itself use to manage the PSBTs? As it happens, it can use something like Libwally, as this section demonstrates.

Basically, Libwally has all of the PSBT functionality, so if there's something you could do with `bitcoind`, you could also do it using Libwally, even if your device is offline. What follows is the barest introduction to what's a very complex topic.

## Convert a PSBT

Converting a PSBT into Libwally's internal structure is incredibly easy, you just run `wally_psbt_from_base64` with a base64 PSBT — which are the outputs produced by `bitcoin-cli`, such as:

`cHNidP8BAJoCAAAAAri6BLjKQZGO9Y1iVIYbxlxBJ2kqsTPWnxGaH4HrSjxbAAAAAAD+////leV0hwJ0fO40RmhuFVIYtO16ktic2J4vJFLAsT5TM8cBAAAAAP7///8CYOMWAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBU+gBwAAAAAAFgAU9Ojd5ds3CJi1fIRWbj92CYhQgX0AAAAAAAEBH0BCDwAAAAAAFgAUABk8i/Je8Fb41FcaHD9lEj5f54giBgMBaNlILisC1wJ/tKie3FStqhrfcJM09kfQobBTOCiuxRiaHVILVAAAgAEAAIAAAACAAAAAADkCAAAAAQEfQEIPAAAAAAAWABQtTxOfqohTBNFWFqFm0tUVdK9KXSIGAqATz5xLX1aJ2SUwNqPkd8+YaJYm94FMlPCScm8Rt0GrGJodUgtUAACAAQAAgAAAAIAAAAAAAAAAAAAAIgID2UK1nupSfXC81nmB65XZ+pYlJp/W6wNk5FLt5ZCSx6kYmh1SC1QAAIABAACAAAAAgAEAAAABAAAAAA==`

However, it's a bit harder to deal with the result, because Libwally converts it into a very complex `wally_psbt` structure.

Here's how it's defined in `/usr/include/wally_psbt.h`:
```
struct wally_psbt {
    unsigned char magic[5];
    struct wally_tx *tx;
    struct wally_psbt_input *inputs;
    size_t num_inputs;
    size_t inputs_allocation_len;
    struct wally_psbt_output *outputs;
    size_t num_outputs;
    size_t outputs_allocation_len;
    struct wally_unknowns_map *unknowns;
    uint32_t version;
};

struct wally_psbt_input {
    struct wally_tx *non_witness_utxo;
    struct wally_tx_output *witness_utxo;
    unsigned char *redeem_script;
    size_t redeem_script_len;
    unsigned char *witness_script;
    size_t witness_script_len;
    unsigned char *final_script_sig;
    size_t final_script_sig_len;
    struct wally_tx_witness_stack *final_witness;
    struct wally_keypath_map *keypaths;
    struct wally_partial_sigs_map *partial_sigs;
    struct wally_unknowns_map *unknowns;
    uint32_t sighash_type;
};

struct wally_psbt_output {
    unsigned char *redeem_script;
    size_t redeem_script_len;
    unsigned char *witness_script;
    size_t witness_script_len;
    struct wally_keypath_map *keypaths;
    struct wally_unknowns_map *unknowns;
};
```
These in turn use some transaction structures defined in `/usr/include/wally_transaction.h`:
```
struct wally_tx {
    uint32_t version;
    uint32_t locktime;
    struct wally_tx_input *inputs;
    size_t num_inputs;
    size_t inputs_allocation_len;
    struct wally_tx_output *outputs;
    size_t num_outputs;
    size_t outputs_allocation_len;
};

struct wally_tx_output {
    uint64_t satoshi;
    unsigned char *script;
    size_t script_len;
    uint8_t features;
};
```
There's a lot there! Though much of this should be familiar from pervious chapters, it's a bit overwhelming to see it all laid out in C structures.

## Read a Converted PSBT

Obviously, you can read anything out of a PSBT structure by calling up the individual elements from the various substructures. The following is a brief overview showing how to grab a few of the elements. 

Here's an example of retrieving the values and `scriptPubKeys` of the inputs:
```
  int inputs = psbt->num_inputs;
  printf("TOTAL INPUTS: %i\n",inputs);

  for (int i = 0 ; i < inputs ; i++) {
    printf("\nINPUT #%i: %i satoshis\n",i, psbt->inputs[i].witness_utxo->satoshi);

    char *script_hex;
    wally_hex_from_bytes(psbt->inputs[i].witness_utxo->script,psbt->inputs[i].witness_utxo->script_len,&script_hex);
    printf("scriptPubKey: %s\n",script_hex);
    wally_free_string(script_hex);
    
  }
```
This programming pattern will be used on many parts of the PSBT. You look at the size of the inputs array, then you step through it, retrieving what you want to see (in this case, satoshis and scripts).

Here's a similar example for the outputs:
```
  int outputs = psbt->num_outputs;
  printf("\nTOTAL OUTPUTS: %i\n",outputs);
  for (int i = 0 ; i < outputs ; i++) {

    char *pubkey_hex;
    wally_hex_from_bytes(psbt->tx->outputs[i].script,psbt->tx->outputs[i].script_len,&pubkey_hex);
    printf("\nINPUT #%i\n",i);
    printf("scriptPubKey: %s\n",pubkey_hex);
    wally_free_string(pubkey_hex);    
  }
```
Obviously, there's a lot more you could look at in the PSBTs. In fact, looking is the main point of a PSBT: you can verify inputs and outputs from an offline computer.

> :warning: **WARNING:** These reading functions are _very_ rudimentary and will not work properly for extremly normal situations like an input or output that's still empty or that includes a `non_witness_utxo`. They will segfault if they aren't delivered a precisely expected PSBT. A real reader would need to be considerably more robust, to cover all possible situations, but that's left as an exercise for the reader.

### Test Your PSBT Reader

Again, the code for this (extremely rudimentary and specific) PSBT reader is in the [src directory](src/17_4_examinepsbt.c). 

You can compile it as normal:
```
$ cc examinepsbt.c -lwallycore -o examinepsbt
```
The following PSBT from [§7.3](07_3_Integrating_with_Hardware_Wallets.md) can be used for testing, as it matches the very narrow criteria required by this limited implementation:
```
psbt=cHNidP8BAJoCAAAAAri6BLjKQZGO9Y1iVIYbxlxBJ2kqsTPWnxGaH4HrSjxbAAAAAAD+////leV0hwJ0fO40RmhuFVIYtO16ktic2J4vJFLAsT5TM8cBAAAAAP7///8CYOMWAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBU+gBwAAAAAAFgAU9Ojd5ds3CJi1fIRWbj92CYhQgX0AAAAAAAEBH0BCDwAAAAAAFgAUABk8i/Je8Fb41FcaHD9lEj5f54giBgMBaNlILisC1wJ/tKie3FStqhrfcJM09kfQobBTOCiuxRiaHVILVAAAgAEAAIAAAACAAAAAADkCAAAAAQEfQEIPAAAAAAAWABQtTxOfqohTBNFWFqFm0tUVdK9KXSIGAqATz5xLX1aJ2SUwNqPkd8+YaJYm94FMlPCScm8Rt0GrGJodUgtUAACAAQAAgAAAAIAAAAAAAAAAAAAAIgID2UK1nupSfXC81nmB65XZ+pYlJp/W6wNk5FLt5ZCSx6kYmh1SC1QAAIABAACAAAAAgAEAAAABAAAAAA==
```
Run `examinepsbt` with that PSBT, and you should see the scripts on the inputs and the outputs:
```
$ ./examinepsbt $psbt
TOTAL INPUTS: 2

INPUT #0: 1000000 satoshis
scriptPubKey: 001400193c8bf25ef056f8d4571a1c3f65123e5fe788

INPUT #1: 1000000 satoshis
scriptPubKey: 00142d4f139faa885304d15616a166d2d51574af4a5d

TOTAL OUTPUTS: 2

INPUT #0
scriptPubKey: 0014c772d6f95542e11ef11e8efc7c7a69830ad38a05

INPUT #1
scriptPubKey: 0014f4e8dde5db370898b57c84566e3f76098850817d
```
And of course, you can check this with the `decodepsbt` RPC command for `bitcoin-cli`:
```
$ bitcoin-cli decodepsbt $psbt
{
  "tx": {
    "txid": "45f996d4ff8c9e9ab162f611c5b6ad752479ede9780f9903bdc80cd96619676d",
    "hash": "45f996d4ff8c9e9ab162f611c5b6ad752479ede9780f9903bdc80cd96619676d",
    "version": 2,
    "size": 154,
    "vsize": 154,
    "weight": 616,
    "locktime": 0,
    "vin": [
      {
        "txid": "5b3c4aeb811f9a119fd633b12a6927415cc61b8654628df58e9141cab804bab8",
        "vout": 0,
        "scriptSig": {
          "asm": "",
          "hex": ""
        },
        "sequence": 4294967294
      },
      {
        "txid": "c733533eb1c052242f9ed89cd8927aedb41852156e684634ee7c74028774e595",
        "vout": 1,
        "scriptSig": {
          "asm": "",
          "hex": ""
        },
        "sequence": 4294967294
      }
    ],
    "vout": [
      {
        "value": 0.01500000,
        "n": 0,
        "scriptPubKey": {
          "asm": "0 c772d6f95542e11ef11e8efc7c7a69830ad38a05",
          "hex": "0014c772d6f95542e11ef11e8efc7c7a69830ad38a05",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1qcaedd724gts3aug73m78c7nfsv9d8zs9q6h2kd"
          ]
        }
      },
      {
        "value": 0.00499791,
        "n": 1,
        "scriptPubKey": {
          "asm": "0 f4e8dde5db370898b57c84566e3f76098850817d",
          "hex": "0014f4e8dde5db370898b57c84566e3f76098850817d",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1q7n5dmewmxuyf3dtus3txu0mkpxy9pqtacuprak"
          ]
        }
      }
    ]
  },
  "unknown": {
  },
  "inputs": [
    {
      "witness_utxo": {
        "amount": 0.01000000,
        "scriptPubKey": {
          "asm": "0 00193c8bf25ef056f8d4571a1c3f65123e5fe788",
          "hex": "001400193c8bf25ef056f8d4571a1c3f65123e5fe788",
          "type": "witness_v0_keyhash",
          "address": "tb1qqqvnezljtmc9d7x52udpc0m9zgl9leugd2ur7y"
        }
      },
      "bip32_derivs": [
        {
          "pubkey": "030168d9482e2b02d7027fb4a89edc54adaa1adf709334f647d0a1b0533828aec5",
          "master_fingerprint": "9a1d520b",
          "path": "m/84'/1'/0'/0/569"
        }
      ]
    },
    {
      "witness_utxo": {
        "amount": 0.01000000,
        "scriptPubKey": {
          "asm": "0 2d4f139faa885304d15616a166d2d51574af4a5d",
          "hex": "00142d4f139faa885304d15616a166d2d51574af4a5d",
          "type": "witness_v0_keyhash",
          "address": "tb1q948388a23pfsf52kz6skd5k4z4627jja2evztr"
        }
      },
      "bip32_derivs": [
        {
          "pubkey": "02a013cf9c4b5f5689d9253036a3e477cf98689626f7814c94f092726f11b741ab",
          "master_fingerprint": "9a1d520b",
          "path": "m/84'/1'/0'/0/0"
        }
      ]
    }
  ],
  "outputs": [
    {
    },
    {
      "bip32_derivs": [
        {
          "pubkey": "03d942b59eea527d70bcd67981eb95d9fa9625269fd6eb0364e452ede59092c7a9",
          "master_fingerprint": "9a1d520b",
          "path": "m/84'/1'/0'/1/1"
        }
      ]
    }
  ],
  "fee": 0.00000209
}
```
You can see the input satoshis and `scriptPubKey` clearly listed in the `inputs` and the new `scriptPubKey`s in the `tx`'s `vout`. 

So, it's all there for your gathering!

## Create a PSBT

As noted at the head of this section, all of the functions needed to create and process PSBTs are available in Libwally. Actually running through the process of doing so is complex enough that it's beyond the scope of this section, but here's a quick run-down of the functions required. Note that the [documents](https://wally.readthedocs.io/en/latest/psbt/) are out of date for PSBTs, so you'll need to consult `/usr/include/wally_psbt.h` for full information.

As discussed in [§7.1](07_1_Creating_a_Partially_Signed_Bitcoin_Transaction.md) there are several roles involved in creating PSBTs

### Take the Creator Role

The creator role is tasked with creating a PSBT with at least one input.

A PSBT is created with a simple use of `wally_psbt_init_alloc`, telling it how many inputs and outputs you will eventually add:
```
  struct wally_psbt *psbt;
  lw_response = wally_psbt_init_alloc(0,1,1,0,&psbt);
```
But what you have is not yet a legal PSBT, because of the lack of inputs. You can create those by creating a transaction and setting it as the global transaction in the PSBT, which updates all the inputs and outputs:
```
  struct wally_tx *gtx;
  lw_response = wally_tx_init_alloc(0,0,1,1,&gtx);
  lw_response = wally_psbt_set_global_tx(psbt,gtx);
```
### Test Your PSBT Creation

At this point, you should have an empty, but working PSBT, which you can see by compiling and running [the program](src/17_4_createemptypsbt.c).
```
$ cc createemptypsbt.c -lwallycore -o createemptypsbt
$ ./createemptypsbt 
cHNidP8BAAoAAAAAAAAAAAAAAA==
```
You can even use `bitcoin-cli` to test the result:
```
$ psbt=$(./createpsbt)
$ bitcoin-cli decodepsbt $psbt
{
  "tx": {
    "txid": "f702453dd03b0f055e5437d76128141803984fb10acb85fc3b2184fae2f3fa78",
    "hash": "f702453dd03b0f055e5437d76128141803984fb10acb85fc3b2184fae2f3fa78",
    "version": 0,
    "size": 10,
    "vsize": 10,
    "weight": 40,
    "locktime": 0,
    "vin": [
    ],
    "vout": [
    ]
  },
  "unknown": {
  },
  "inputs": [
  ],
  "outputs": [
  ],
  "fee": 0.00000000
}
```
## Take the Rest of the Roles

As with PSBT reading, we are introducing the concept of PSBT creation, and then leaving the rest as an exercise for the reader.

Following is a rough listing of functions for every roles; more functions will be needed to create some of the elements that are added to PSBTs.

**Creator:**

* wally_psbt_init_alloc
* wally_psbt_set_global_tx

**Updater:**

* wally_psbt_input_set_non_witness_utxo
* wally_psbt_input_set_witness_utxo
* wally_psbt_input_set_redeem_script
* wally_psbt_input_set_witness_script
* wally_psbt_input_set_keypaths
* wally_psbt_input_set_unknowns
* wally_psbt_output_set_redeem_script
* wally_psbt_output_set_witness_script
* wally_psbt_output_set_keypaths
* wally_psbt_output_set_unknowns

**Signer:**

* wally_psbt_input_set_partial_sigs
* wally_psbt_input_set_sighash_type
* wally_psbt_sign

**Combiner:**

* wally_psbt_combine

**Finalizer:**
* wally_psbt_finalize
* wally_psbt_input_set_final_script_sig
* wally_psbt_input_set_final_witness

**Extracter:**

* wally_psbt_extract

## Summary: Using PSBTs in Libwally

This section could be an entire chapter, as working with PSBTs at a low level is very intensive work that requires much more intensive manipulating of inputs and outputs than was the case in [Chapter 7](07_0_Expanding_Bitcoin_Transactions_PSBTs.md). Instead this section shows the basics: how to extract information from a PSBT, and how to begin creating one. 

> :fire: ***What is the Power of PSBTs in Libwally?*** Obviously, you can already do all of this in `bitcoin-cli`, and it's simpler because Bitcoin Core manages a lot of the drudgery. The advantage of using Libwally is that it can be run offline, so it could be Libwally that's sitting on the other side of a hardware device that your `bitcoin-cli` is communicating to with HWI. This is, in fact, one of the major points of PSBTs: to be able to manipulate partially signed transactions without needing a full node. Libwally enables it.

## What's Next?

Learn more about "Programming Bitcoin with Libwally" in [17.5: Using Scripts in Libwally](17_5_Using_Scripts_in_Libwally.md).
