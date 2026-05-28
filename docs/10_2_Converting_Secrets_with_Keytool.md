# 10.2: Converting Secrets with Keytool

Seeds are great for portability and interoperability ... except that
Bitcoin Core won't accept them as input. Instead, you need to generate
keys for import into Bitcoin Core.

## Understand How the Key Hierarchy Works

[§10.1](10_1_Creating_Secrets_from_the_Command_Line.md) overviewed the
hierarchy of seed, keys, and addresses used in Bitcoin. We're going to
be talking about three of those elements in this section.

* **Seed.** This is the foundation of a BIP-32 HD wallet. Bitcoin Core doesn't expose them.
* **Master Key.** A key derived directly from the seed. The root of a BIP-32 HD wallet. This has been exposed in some versions of Bitcoin Core, but currently you can only see the fingerprint of the master public key.
* **Account Key.** A key derived from the master key and a specific account deriviation path. It's included in both the external and internal descriptors for a specific address type. This is what you see when you `listdescriptors` in Bitcoin Core.

## Install Your Own Seed Convertor

To convert seeds you're going to use another Blockchain Commons tool,
`keytool`. It's an older tool that hasn't yet been converted to Rust,
and as a result you're going to need to install a variety of Python
and C++ libraries.

### Install C++ and Python

If you're using a Debian Linux machine, the following listing of packages will get you everything you need:
```
sudo apt-get install llvm clang lsb-release wget git apt-transport-https pkg-config autoconf libtool libc++-dev libc++abi-dev python3 python3-setuptools
```

Afterward you can install LLVM for Python and link up `python` so that `keytool` can find it:
```
sudo bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"
sudo ln -s /usr/bin/python3 /usr/bin/python
```

Blockchain Commons has alternative instructions for installing on
[MacOS with Homebrew](https://github.com/BlockchainCommons/keytool-cli/tree/master#macos).

Once you've got a robust Python environment, you can clone the repo and compile it:

```
cd ~
git clone https://github.com/BlockchainCommons/keytool-cli.git
cd keytool-cli/
autoreconf -i
export CC="clang" && export CXX="clang++" && ./build.sh
sudo make install
```

Python installs unfortunately aren't nearly as robust as Rust
installs, but this has been tested on both MacOS and Debian as of this
writing.

## Output Seed & Fingerprint

You presumably have a seed stored in your `$SEED` variable from last
chapter. If not, see
[§10.1](10_1_Creating_Secrets_from_the_Command_Line/#use-seedtool) for
how to do so.

To start with, you should fingerprint the master key derived from that
seed. Though this isn't technically required for Bitcoin Core
descriptors, it nonetheless can be used in the future as metadata that
will tell us what seed was used to create certain descriptors.

When using `keytool` you're going to input your seed with `--seed` and then
ask for a certain type of output, in this case,
`master-key-fingerprint`.

```
FINGERPRINT=$(keytool --seed $SEED master-key-fingerprint)
```

Here's what it looks like:

```
echo $FINGERPRINT

| 35dad980
```

You'll note that you skipped right past the step of creating the master private key and deriving the master public key before finger printing it. That's
because `keytool` is smart and does all of the intermediary
calculations without asking you. So here it goes from the seed through
the private master key through the public master key to the
fingerprint, and you only have to worry about the first and last
steps.

(We're actually never going to output those master keys because we only care about things further down the hierarchy.)

## Convert to an Account Key

You now need to create account keys for each type of address that you
want to support. This is done by again inputting your `$SEED` with
`--seed`, but this time adding an `-account-derivation-path` defining
the `m/purpose'/coin_type'/account'/` and then asking for an output of
either `account-key` or `account-key-base58`.

The following creates a WPKH account key:
```
ADP_WPKH="m/84h/0h/0h"
AKEY_WPKH=$(keytool --seed $SEED --account-derivation-path $ADP_WPKH account-key)
AKEY58_WPKH=$(keytool --seed $SEED --account-derivation-path $ADP_WPKH account-key-base58)
```

We've stored that key in two forms.

The `$AKEY` variable contains a UR format that will be handy for use with other Blockchain Commons CLIs.

```
echo $AKEY_WPKH

| ur:crypto-hdkey/onaoykaxhdclaedwfyeekpzoeepluebbtblnkksafedwmkssclfmbyidylskdwuelyfypkbakibehsaahdcxrdkenlhkqznbntgrhhmsmtbzmohljypmhllozorsesqdsomngseccfwdetbdmycxamoeadlncsghykaeykaeykaocyectntalaaycysktytardvlnlihie
```

The `$AKEY58` is base 58 format, which is needed for use with Bitcoin Core.

```
echo $AKEY58_WPKH

| xprv9z7kiySAUeGtnrbQpzqFsE8uDQufoofkLqxW2kPRcLppKx6kthUEfp4mTjyQijpcfQ5iXgYVH9EzASguJ1PNTU3YnztKGmmtBJciRU9iFre
```

You could use this same methodology to create an account key for any
path. For example, the following would create a legacy PKH account:

```
ADP_PKH="m/44h/0h/0h"
AKEY_PKH=$(keytool --seed $SEED --account-derivation-path $ADP_PKH account-key)
AKEY58_PKH=$(keytool --seed $SEED --account-derivation-path $ADP_PKH account-key-base58)
```

## Get Your Data Together

At this point you should have three bits of data:

* Master key fingerprint
* Derivation path for each address type
* Base 58 private key for each address type

This is almost everything you need to import an account into Bitcoin
Core. (The last bit, a checksum, is something that you'll get from
Bitcoin Core itself).

## Summary: Converting Secrets to Keytool

To get from a seed to an address takes a few steps. Fortunately,
`keytool` can take input in dozens of forms and can output in dozens
of forms. Here we generated a fingerprint (hash) from the seed for
identification purposes, and then used a derivation path to generate
an account key, which is what we'll use for import into Bitcoin Core.

## What's Next?

Continue "Exploring the Ecosystem" with
[§10.3: Imprting Secrets to Bitcoin Core](10_3_Importing_Secrets_to_Bitcoin_Core.md).


