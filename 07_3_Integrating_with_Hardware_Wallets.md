# 7.3: Integrating with Hardware Wallets

> :information_source: **NOTE:** This section has been recently added to the course and is an early draft that may still be awaiting review. Caveat reader.

One of the greatest powers of PSBTs is the ability to hand transactions off to hardware wallets. This will be a great development tool for you if you continue to program with Bitcoin. However, you can't test it out now if you're using one of the configurations we suggest for this course — a VM on Linode per [§2.1](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/02_1_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md) or an even more farflung option such as AWS per [§2.2](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/02_2_Setting_Up_Bitcoin_Core_Other.md) — because obviously you won't have any way to hook a hardware wallet up to your remote, virtual machine.

> :book: ***What is a Hardware Wallet?*** A hardware wallet is an electronic device that improves the security of a cryptocurrency by maintaining all the private keys on the device, rather than ever putting them on a computer directly connected to the internet. Hardware wallets have specific protocols for providing online interactions, usually managed by a program talking to the device through a USB port. In this chapter, we'll be managing a hardware wallet with `bitcoin-cli` and the `hwy.py` program.

You have three options for moving through this chapter on hardware wallets: (1) read along without testing the code; (2) install Bitcoin on a local machine to fully test these commands; or (3) skip straight ahead to [Chapter 8: Expanding Bitcoin Transactions in Other Ways](08_0_Expanding_Bitcoin_Transactions_Other.md). We suggest option #1, but if you really want to get your hands dirty we'll also give some support for #2 by talking about using a Macintosh (a hardware-platform supported by [Bitcoin Standup](https://github.com/BlockchainCommons/Bitcoin-Standup)) for testing.

> :warning: **VERSION WARNING:** PSBTs are an innovation from Bitcoin Core v 0.17.0. Earlier versions of Bitcoin Core will not be able to work with the PSBT while it is in progress (though they will still be able to recognize the final transaction). The HWI interface appeared in Bitcoin Core v 0.18.0, but as long as you are using our suggested setup with Bitcoin Standup, it should work.

The methodology described in this chapter for integrating with a hardware wallet depends on the [Bitcoin Hardware Wallet Interface](https://github.com/bitcoin-core/HWI) released through Bitcoin Core and builds on the [installation](https://github.com/bitcoin-core/HWI/blob/master/README.md) and [usage](https://hwi.readthedocs.io) instructions found there.

> :warning: **FRESHNESS WARNING:** The HWI interface is very new and raw around the edges as of Bitcoin Core v 0.20.0. It may be hard to install correctly, and it may have unintuitive errors. What follows is a description of a working setup, but it took several tries to get it right, and your setup may vary.

## Install Bitcoin Core on a Local Machine

_If you just plan to read over this section and not test out these commands until some future date when you have a local development environment, you can skip this subsection, which is about creating a Bitcoin Core installation on a local machine such as a Mac or Linux machine._

There are alternate versions of the Bitcoin Standup script that you used to create your VM that will install on a MacOS or on a non-Linode Linux machine.

If you have MacOS, you can install [Bitcoin Standup MacOS](https://github.com/BlockchainCommons/Bitcoin-Standup-MacOS/blob/master/README.md).

If you have a local Linux machine, you can install [Bitcoin Standup Linux Scripts](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts/blob/master/README.md).

Once you've gotten Bitcoin Standup running on your local machine, you'll want to sync the "Testnet" blockchain, assuming that you're continuing to follow the standard methodlogy of this course.

We will be using a Macintosh and Testnet for the examples in this section.

### Create an Alias for Bitcoin-CLI

Create an alias that runs `bitcoin-cli` from the correct directory with any appropriate flags.

Here's an example alias from a Mac:
```
$ alias bitcoin-cli="~/StandUp/BitcoinCore/bitcoin-0.20.0/bin/bitcoin-cli -testnet"
```
You'll note it not only gives us the full path, but also ensures that we stay on Testnet.

## Install HWI on a Local Machine

_The following instructions again assume a Mac, and you can again skip this subsection if you're just reading through this chapter._

HWI is a Bitcoin Core program available in python that can be used to interact with hardware wallets.

### Install Python

Because HWI  is written in `python`, you'll need to install that as well as a few auxilliary programs.

If you don't already have the `xcode` command line tools, you'll need them:
```
$ xcode-select --install
```

If you don't already have the Homebrew package manager, you should install that too. Current instructions are available at the [Homebrew site](https://brew.sh/). As of this writing, you simply need to:
```
$ /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"
```

For a first-time installation, you should also make sure your `/usr/local/Frameworks` directory is created correctly:
```
$ sudo mkdir /usr/local/Frameworks
$ sudo chown $(whoami):admin /usr/local/Frameworks
```

If you've got all of that in place, you can finally install Python:
```
$ brew install python
$ brew install libusb
```

### Install HWI

You're now ready to install HWI, which requires cloning a GitHub repo and running an install script.

If you don't have `git` already installed on your Mac, you can do so just by trying to run it: `git --version`.

You can then clone the HWI repo:
```
$ cd ~/StandUp
$ git clone https://github.com/bitcoin-core/HWI.git
```
Afterward, you need to install the package and its dependencies:
```
$ cd HWI
HWI$ python3 setup.py install
```

### Create an Alias for HWI

You'll want to create an alias here too, varied by your actual install location:
```
$ alias hwi="~/Standup/HWI/hwi.py --chain test"
```
Again, we've included a reference to testnet in this alias.

## Prepare Your Ledger

_We had to choose a hardware-wallet platform too, for this HWI demonstration. Our choice was the Ledger, which has long been our testbed for hardware wallets. Please see [HWI's device support info](https://github.com/bitcoin-core/HWI/blob/master/README.md#device-support) for a list of other supported devices. If you use a device other than a Ledger, you'll need to assess your own solutions for preparing it for usage on the Testnet, but otherwise you should be able to continue with the course as written._

If you are working with Bitcoins on your Ledger, you probably won't need to do anything. (But we don't suggest that for use with this course). 

To work with Testnet coins, as suggested by this course, you'll need to make a few updates:

1. Go to Settings on your Ledger Live app (it's the gear), go to the "Experimental Features" tab, and turn on "Developer Mode".
2. Go to the "Manager" and install "Bitcoin Test". The current version requires that you have "Bitcoin" installed first.
3. Go to the "Manager", scroll to your new "Bitcoin Test", and "Add Account"

## Link to a Ledger

In order for a Ledger to be accessible, you must login with your PIN and then call up the app that you want to use, in this case the "Bitcoin Test" app. You may need to repeat this from time to time if your Ledger falls asleep.

Once you've done that, you can ask for HWI to access the Ledger with the `enumerate` command:
```
$ hwi enumerate
[{"type": "ledger", "model": "ledger_nano_s", "path": "IOService:/AppleACPIPlatformExpert/PCI0@0/AppleACPIPCI/XHC1@14/XHC1@14000000/HS05@14100000/Nano S@14100000/Nano S@0/IOUSBHostHIDDevice@14100000,0", "fingerprint": "9a1d520b", "needs_pin_sent": false, "needs_passphrase_sent": false}]
```
If you receive information on your device, you're set! As you can see, it verifies your hardware-wallet type, provides other identifying information, and tells you how to communicate with the device. The `fingerprint` (`9a1d520b`) is what you should pay particular attention to, because all interactions with your hardware wallet will require it.

If you instead got `[]`, then either (1) you didn't get your Ledger device ready by entering your PIN and choosing the correct application, or (2) there's something wrong with your Python setup, probably a missing dependency: you should consider `uninstall`ing it and trying from scratch.

## Import Addresses

Interacting with a hardware wallet usually comes in two parts: watching for funds and spending funds. 

You can watch for funds by importing addresses from your hardware wallet to your full node, using HWI and `bitcoin-cli`.

### Create a Wallet

To use your hardware wallet with `bitcoin-cli`, you'll want to create a specific named wallet in Bitcoin Core, using the `createwallet` RPC, which is a command we haven't previously discussed.
```
$ bitcoin-cli --named createwallet wallet_name="ledger" disable_private_keys="true"
{
  "name": "ledger",
  "warning": ""
}
```
In this case, you are creating a new wallet `ledger` without private keys (since those will be over on the Ledger device).

> :book: ***Why Name Wallets?*** To date, this course has used the default ("") wallet in Bitcoin Core. This is fine for many purposes, but is inadequate if you have a more complex situation, such as when you're watching keys from a hardware wallet. Here, we want to be able to differentiate from the locally owned keys (which are kept in the "" wallet) and the remotely owned keys (which are kept in the "ledger" wallet).

You can now see that the new wallet is in your wallets list:
```
$ bitcoin-cli listwallets
[
  "",
  "ledger"
]
```
Because you've created a second wallet, some commands will now require a `-rpcwallet=` flag, to specify which one you're using

### Import the Keys

You now have to import a watch-list of addresses from the hardware wallet. This is done with HWI's `getkeypool` command:
```
$ hwi -f 9a1d520b getkeypool 0 1000
[{"desc": "wpkh([9a1d520b/84h/1h/0h]tpubDD7KTtoGzK9GuWUQcr1uTJazsAkqoXhdrwGXWVix6nPpNZmSbagZWD4QSaMsyK8YohAirGDPrWdRiEpKzTFB7DrTrqfzHCn7yi5EsqeR93S/0/*)#qttxy592", "range": [0, 1000], "timestamp": "now", "internal": false, "keypool": true, "active": true, "watchonly": true}, {"desc": "wpkh([9a1d520b/84h/1h/0h]tpubDD7KTtoGzK9GuWUQcr1uTJazsAkqoXhdrwGXWVix6nPpNZmSbagZWD4QSaMsyK8YohAirGDPrWdRiEpKzTFB7DrTrqfzHCn7yi5EsqeR93S/1/*)#3lw8ep4j", "range": [0, 1000], "timestamp": "now", "internal": true, "keypool": true, "active": true, "watchonly": true}]
```
We address HWI with the `fingerprint` and ask for the first 1000 addresses. The WPKH (native Segwit) address type is used as a default. In return, we receive two descriptors for the key pool: one for receiving addresses and one for change addresses.

> :book: ***What is a key pool?*** A key pool is a group of pregenerated keys. Modern HD wallets create key pools by continuing to determine new hierarchical addresses based on the original seed. The idea of key pools was originally implemented to ease the backup requirements of wallets. This allowed a user to generate a keypool and then backup the wallet immediately, rather than requiring backups after every new address was created. The concept has also proven very useful in the modern day since it allows the importing of a whole set of future addresses from one device to another.

The values returned by `getkeypool` are the same sorts of descriptors that we learned about in [§3.5: Understanding the Descriptor](03_5_Understanding_the_Descriptor.md). At the time, we said that they were most useful for moving addresses among different machines. Here's the real-life example: moving addresses from a hardware wallet to Bitcoin Core node, so that our net-connected machine will be able to watch over the keys owned by the offline hardware wallet.

Just as you learned in [§3.5](03_5_Understanding_the_Descriptor.md), you can examine these descriptors with the `getdescriptorinfo` RPC:
```
$ bitcoin-cli getdescriptorinfo "wpkh([9a1d520b/84h/1h/0h]tpubDD7KTtoGzK9GuWUQcr1uTJazsAkqoXhdrwGXWVix6nPpNZmSbagZWD4QSaMsyK8YohAirGDPrWdRiEpKzTFB7DrTrqfzHCn7yi5EsqeR93S/0/*)#qttxy592"
{
  "descriptor": "wpkh([9a1d520b/84'/1'/0']tpubDD7KTtoGzK9GuWUQcr1uTJazsAkqoXhdrwGXWVix6nPpNZmSbagZWD4QSaMsyK8YohAirGDPrWdRiEpKzTFB7DrTrqfzHCn7yi5EsqeR93S/0/*)#n65e7wjf",
  "checksum": "qttxy592",
  "isrange": true,
  "issolvable": true,
  "hasprivatekeys": false
}
```
As you'd expect, we do _not_ have `privatekeys`, because hardware wallets hold on to those.

With the descriptors in hand, you can import the keys into your new `ledger` wallet using the `importmulti` RPC that you also met in [§3.5](03_5_Understanding_the_Descriptor.md). In this case, just put the entire response you got back from HWI in `'`s.
```
$ bitcoin-cli -rpcwallet=ledger importmulti '[{"desc": "wpkh([9a1d520b/84h/1h/0h]tpubDD7KTtoGzK9GuWUQcr1uTJazsAkqoXhdrwGXWVix6nPpNZmSbagZWD4QSaMsyK8YohAirGDPrWdRiEpKzTFB7DrTrqfzHCn7yi5EsqeR93S/0/*)#qttxy592", "range": [0, 1000], "timestamp": "now", "internal": false, "keypool": true, "active": true, "watchonly": true}, {"desc": "wpkh([9a1d520b/84h/1h/0h]tpubDD7KTtoGzK9GuWUQcr1uTJazsAkqoXhdrwGXWVix6nPpNZmSbagZWD4QSaMsyK8YohAirGDPrWdRiEpKzTFB7DrTrqfzHCn7yi5EsqeR93S/1/*)#3lw8ep4j", "range": [0, 1000], "timestamp": "now", "internal": true, "keypool": true, "active": true, "watchonly": true}]'
[
  {
    "success": true
  },
  {
    "success": true
  }
]
```
(Note that HWI helpfully output the derivation path with `h`s to show hardened derivations rather than `'`s, and calculated its checksum accordingly, so that we don't have to do massive quoting like we did in §3.5.)

You _could_ now list all of the watch-only addresses that you received using the `getaddressesbylabel` command. All 1000 of the receive addresses are right there, in the `ledger` wallet!
```
$ bitcoin-cli -rpcwallet=ledger getaddressesbylabel "" | more
{
  "tb1qqqvnezljtmc9d7x52udpc0m9zgl9leugd2ur7y": {
    "purpose": "receive"
  },
  "tb1qqzvrm6hujdt93qctuuev5qc4499tq9fdk0prwf": {
    "purpose": "receive"
  },
...
}
```
## Receive a Transaction

Obviously, receiving a transaction is simple. You use `getnewaddress` to request one of those imported addresses:
```
$ bitcoin-cli -rpcwallet=ledger getnewaddress
tb1qqqvnezljtmc9d7x52udpc0m9zgl9leugd2ur7y
```
Then you send money to it.

The power of HWI is that you can watch the payments from your Bitcoin Core node, rather than having to plug in your hardware wallet and query it.
```
$ bitcoin-cli -rpcwallet=ledger listunspent
[
  {
    "txid": "c733533eb1c052242f9ed89cd8927aedb41852156e684634ee7c74028774e595",
    "vout": 1,
    "address": "tb1q948388a23pfsf52kz6skd5k4z4627jja2evztr",
    "label": "",
    "scriptPubKey": "00142d4f139faa885304d15616a166d2d51574af4a5d",
    "amount": 0.01000000,
    "confirmations": 12,
    "spendable": false,
    "solvable": true,
    "desc": "wpkh([9a1d520b/84'/1'/0'/0/0]02a013cf9c4b5f5689d9253036a3e477cf98689626f7814c94f092726f11b741ab)#9za8hlvk",
    "safe": true
  },
  {
    "txid": "5b3c4aeb811f9a119fd633b12a6927415cc61b8654628df58e9141cab804bab8",
    "vout": 0,
    "address": "tb1qqqvnezljtmc9d7x52udpc0m9zgl9leugd2ur7y",
    "label": "",
    "scriptPubKey": "001400193c8bf25ef056f8d4571a1c3f65123e5fe788",
    "amount": 0.01000000,
    "confirmations": 1,
    "spendable": false,
    "solvable": true,
    "desc": "wpkh([9a1d520b/84'/1'/0'/0/569]030168d9482e2b02d7027fb4a89edc54adaa1adf709334f647d0a1b0533828aec5)#sx9haake",
    "safe": true
  }
]
```
## Create a Transaction with PSBT

Watching and receiving payments is just half the battle. You may also want to make payments using accounts held by your hardware wallet. This is the fourth real-life example for using PSBTs, per the process outlined in [§7.1: Creating a Partially Signed Bitcoin Transaction](7_1_Creating_a_Partially_Signed_Bitcoin_Transaction.md). 

The commands work exactly the same. In this case, use `walletcreatefundedpsbt` to form your PSBT because this is a situation where you don't care what UTXOs are used:
```
$ bitcoin-cli -named -rpcwallet=ledger walletcreatefundedpsbt inputs='''[]''' outputs='''[{"tb1qcaedd724gts3aug73m78c7nfsv9d8zs9q6h2kd":0.015}]'''
{
  "psbt": "cHNidP8BAJoCAAAAAri6BLjKQZGO9Y1iVIYbxlxBJ2kqsTPWnxGaH4HrSjxbAAAAAAD+////leV0hwJ0fO40RmhuFVIYtO16ktic2J4vJFLAsT5TM8cBAAAAAP7///8CYOMWAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBU+gBwAAAAAAFgAU9Ojd5ds3CJi1fIRWbj92CYhQgX0AAAAAAAEBH0BCDwAAAAAAFgAUABk8i/Je8Fb41FcaHD9lEj5f54giBgMBaNlILisC1wJ/tKie3FStqhrfcJM09kfQobBTOCiuxRiaHVILVAAAgAEAAIAAAACAAAAAADkCAAAAAQEfQEIPAAAAAAAWABQtTxOfqohTBNFWFqFm0tUVdK9KXSIGAqATz5xLX1aJ2SUwNqPkd8+YaJYm94FMlPCScm8Rt0GrGJodUgtUAACAAQAAgAAAAIAAAAAAAAAAAAAAIgID2UK1nupSfXC81nmB65XZ+pYlJp/W6wNk5FLt5ZCSx6kYmh1SC1QAAIABAACAAAAAgAEAAAABAAAAAA==",
  "fee": 0.00000209,
  "changepos": 1
}
```

You can take a look at the PSBT and verify that it looks rational:
```
$ psbt="cHNidP8BAJoCAAAAAri6BLjKQZGO9Y1iVIYbxlxBJ2kqsTPWnxGaH4HrSjxbAAAAAAD+////leV0hwJ0fO40RmhuFVIYtO16ktic2J4vJFLAsT5TM8cBAAAAAP7///8CYOMWAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBU+gBwAAAAAAFgAU9Ojd5ds3CJi1fIRWbj92CYhQgX0AAAAAAAEBH0BCDwAAAAAAFgAUABk8i/Je8Fb41FcaHD9lEj5f54giBgMBaNlILisC1wJ/tKie3FStqhrfcJM09kfQobBTOCiuxRiaHVILVAAAgAEAAIAAAACAAAAAADkCAAAAAQEfQEIPAAAAAAAWABQtTxOfqohTBNFWFqFm0tUVdK9KXSIGAqATz5xLX1aJ2SUwNqPkd8+YaJYm94FMlPCScm8Rt0GrGJodUgtUAACAAQAAgAAAAIAAAAAAAAAAAAAAIgID2UK1nupSfXC81nmB65XZ+pYlJp/W6wNk5FLt5ZCSx6kYmh1SC1QAAIABAACAAAAAgAEAAAABAAAAAA=="

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
And as usual, `analyzepsbt` will show how far you've gotten:
```
$ bitcoin-cli analyzepsbt $psbt
{
  "inputs": [
    {
      "has_utxo": true,
      "is_final": false,
      "next": "signer",
      "missing": {
        "signatures": [
          "00193c8bf25ef056f8d4571a1c3f65123e5fe788"
        ]
      }
    },
    {
      "has_utxo": true,
      "is_final": false,
      "next": "signer",
      "missing": {
        "signatures": [
          "2d4f139faa885304d15616a166d2d51574af4a5d"
        ]
      }
    }
  ],
  "estimated_vsize": 208,
  "estimated_feerate": 0.00001004,
  "fee": 0.00000209,
  "next": "signer"
}
```
Because you imported that keypool, `bitcoin-cli` has all the information it needs to fill in the inputs, it just can't sign because the private keys are held on the hardware wallet.

That's where HWI comes in, with the `signtx` command. You just send along the PSBT:
```
$ hwi -f 9a1d520b signtx $psbt
```
Expect to have to do some fiddling with your hardware wallet at this point. The device will probably ask you to confirm the inputs, the outputs, and the fee. When you're done, it should return a new PSBT.

```
{"psbt": "cHNidP8BAJoCAAAAAri6BLjKQZGO9Y1iVIYbxlxBJ2kqsTPWnxGaH4HrSjxbAAAAAAD+////leV0hwJ0fO40RmhuFVIYtO16ktic2J4vJFLAsT5TM8cBAAAAAP7///8CYOMWAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBU+gBwAAAAAAFgAU9Ojd5ds3CJi1fIRWbj92CYhQgX0AAAAAAAEBH0BCDwAAAAAAFgAUABk8i/Je8Fb41FcaHD9lEj5f54giAgMBaNlILisC1wJ/tKie3FStqhrfcJM09kfQobBTOCiuxUcwRAIgAxkQlk2fqEMxvP54WWyiFhlfSul9sd4GzKDhfGpmlewCIHYej3zXWWMgWI6rixxQw9yzGozDaFPqQNNIvcFPk+lfASIGAwFo2UguKwLXAn+0qJ7cVK2qGt9wkzT2R9ChsFM4KK7FGJodUgtUAACAAQAAgAAAAIAAAAAAOQIAAAABAR9AQg8AAAAAABYAFC1PE5+qiFME0VYWoWbS1RV0r0pdIgICoBPPnEtfVonZJTA2o+R3z5holib3gUyU8JJybxG3QatHMEQCIH5t6T2yufUP7glYZ8YH0/PhDFpotSmjgZUhvj6GbCFIAiBcgXzyYl7IjYuaF3pJ7AgW1rLYkjeCJJ2M9pVUrq5vFwEiBgKgE8+cS19WidklMDaj5HfPmGiWJveBTJTwknJvEbdBqxiaHVILVAAAgAEAAIAAAACAAAAAAAAAAAAAACICA9lCtZ7qUn1wvNZ5geuV2fqWJSaf1usDZORS7eWQksepGJodUgtUAACAAQAAgAAAAIABAAAAAQAAAAA="}
$ psbt_f="cHNidP8BAJoCAAAAAri6BLjKQZGO9Y1iVIYbxlxBJ2kqsTPWnxGaH4HrSjxbAAAAAAD+////leV0hwJ0fO40RmhuFVIYtO16ktic2J4vJFLAsT5TM8cBAAAAAP7///8CYOMWAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBU+gBwAAAAAAFgAU9Ojd5ds3CJi1fIRWbj92CYhQgX0AAAAAAAEBH0BCDwAAAAAAFgAUABk8i/Je8Fb41FcaHD9lEj5f54giAgMBaNlILisC1wJ/tKie3FStqhrfcJM09kfQobBTOCiuxUcwRAIgAxkQlk2fqEMxvP54WWyiFhlfSul9sd4GzKDhfGpmlewCIHYej3zXWWMgWI6rixxQw9yzGozDaFPqQNNIvcFPk+lfASIGAwFo2UguKwLXAn+0qJ7cVK2qGt9wkzT2R9ChsFM4KK7FGJodUgtUAACAAQAAgAAAAIAAAAAAOQIAAAABAR9AQg8AAAAAABYAFC1PE5+qiFME0VYWoWbS1RV0r0pdIgICoBPPnEtfVonZJTA2o+R3z5holib3gUyU8JJybxG3QatHMEQCIH5t6T2yufUP7glYZ8YH0/PhDFpotSmjgZUhvj6GbCFIAiBcgXzyYl7IjYuaF3pJ7AgW1rLYkjeCJJ2M9pVUrq5vFwEiBgKgE8+cS19WidklMDaj5HfPmGiWJveBTJTwknJvEbdBqxiaHVILVAAAgAEAAIAAAACAAAAAAAAAAAAAACICA9lCtZ7qUn1wvNZ5geuV2fqWJSaf1usDZORS7eWQksepGJodUgtUAACAAQAAgAAAAIABAAAAAQAAAAA="
```

When you analyze this, you'll see that it's ready to be finalized:
```
$ bitcoin-cli analyzepsbt $psbt_f
{
  "inputs": [
    {
      "has_utxo": true,
      "is_final": false,
      "next": "finalizer"
    },
    {
      "has_utxo": true,
      "is_final": false,
      "next": "finalizer"
    }
  ],
  "estimated_vsize": 208,
  "estimated_feerate": 0.00001004,
  "fee": 0.00000209,
  "next": "finalizer"
}
```

At this point you're back into standard territory:
```
$ bitcoin-cli finalizepsbt $psbt_f
{
  "hex": "02000000000102b8ba04b8ca41918ef58d6254861bc65c4127692ab133d69f119a1f81eb4a3c5b0000000000feffffff95e5748702747cee3446686e155218b4ed7a92d89cd89e2f2452c0b13e5333c70100000000feffffff0260e3160000000000160014c772d6f95542e11ef11e8efc7c7a69830ad38a054fa0070000000000160014f4e8dde5db370898b57c84566e3f76098850817d024730440220031910964d9fa84331bcfe78596ca216195f4ae97db1de06cca0e17c6a6695ec0220761e8f7cd7596320588eab8b1c50c3dcb31a8cc36853ea40d348bdc14f93e95f0121030168d9482e2b02d7027fb4a89edc54adaa1adf709334f647d0a1b0533828aec50247304402207e6de93db2b9f50fee095867c607d3f3e10c5a68b529a3819521be3e866c214802205c817cf2625ec88d8b9a177a49ec0816d6b2d8923782249d8cf69554aeae6f17012102a013cf9c4b5f5689d9253036a3e477cf98689626f7814c94f092726f11b741ab00000000",
  "complete": true
}
$ hex=02000000000102b8ba04b8ca41918ef58d6254861bc65c4127692ab133d69f119a1f81eb4a3c5b0000000000feffffff95e5748702747cee3446686e155218b4ed7a92d89cd89e2f2452c0b13e5333c70100000000feffffff0260e3160000000000160014c772d6f95542e11ef11e8efc7c7a69830ad38a054fa0070000000000160014f4e8dde5db370898b57c84566e3f76098850817d024730440220031910964d9fa84331bcfe78596ca216195f4ae97db1de06cca0e17c6a6695ec0220761e8f7cd7596320588eab8b1c50c3dcb31a8cc36853ea40d348bdc14f93e95f0121030168d9482e2b02d7027fb4a89edc54adaa1adf709334f647d0a1b0533828aec50247304402207e6de93db2b9f50fee095867c607d3f3e10c5a68b529a3819521be3e866c214802205c817cf2625ec88d8b9a177a49ec0816d6b2d8923782249d8cf69554aeae6f17012102a013cf9c4b5f5689d9253036a3e477cf98689626f7814c94f092726f11b741ab00000000
$ bitcoin-cli sendrawtransaction $hex
45f996d4ff8c9e9ab162f611c5b6ad752479ede9780f9903bdc80cd96619676d
```
You've successfully sent funds using the private keys held on your hardware wallet!

## Learn Other HWI Commands

There are a variety of other commands available for use with HWI. At the time of this writing, they are:
```
numerate,getmasterxpub,signtx,getxpub,signmessage,getkeypool,getdescriptors,displayaddress,setup,wipe,restore,backup,promptpin,togglepassphrase,sendpin
```

## Summary: Integrating with Hardware Wallets

Hardware wallets can offer better protection by keeping your private keys offline, protected in the hardware. Fortunately, there's still a way to interact with them using `bitcoin-cli`. You just install HWI and it will then allow you to (1) import public keys to watch them; and (2) sign transactions using your hardware wallet.

> :fire: ***What's the power of HWI?*** HWI lets you interact with hardware wallets using all of the commands of `bitcoin-cli` that you've learned to date. You can make raw transactions of any sort, then send PSBTs to  hardware wallets for signing. Thus, you have all of the power of Bitcoin Core, but you also have the security of a hardware device.

## What's Next?

Expand Bitcoin Transactions more with [Chapter Eight: Expanding Bitcoin Transactions in Other Ways](08_0_Expanding_Bitcoin_Transactions_Other.md).
