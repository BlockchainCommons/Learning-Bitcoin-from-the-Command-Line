# 10.1: Creating Secrets from the Command Line

Bitcoin addresses are built on a hierarchy of secrets. In
an HD wallet, a seed leads to a master key which leads to an account
key which leads to address keys. However, in Bitcoin Core, you never
see the seed, even though it's the fundamental unit of interoperability
for the wider Bitcoin ecosystem. This section shows you how to create
a seed, which we'll import into Bitcoin Core over the course of this
chapter.

## Understand How Seeds Work

A seed is a secret. It's a large number. As discussed in
[§3.4](03_4_Understanding_the_Descriptor_Wallet.md), it's also the
foundation of a hierarchy of secrets:

* A master private key is derived from the seed.
* Account private keys are derived from the master private key and an
account derivation path (e.g., `84h/0h/0h` for a P2WPKH account).
* Address private keys are derived from an account private key and an
address derivation path (e.g., `0/0` for the first external
address).
* Public keys are derived from the associated private keys.
* Addresses are derived from address public keys.

The descriptors that we first encountered in §3.4 are made up of the lower-level secrets in this hierarcy:
```
| wpkh([e18dae20/84h/1h/0h]tpubDC4ujMbsd9REzpGk3gnTjkrfJFw1NnvCpx6QBbLj3CHBzcLmVzssTVP8meRAM1WW4pZnK6SCCPGyzi9eMfzSXoeFMNprqtgxG71VRXTmetu/0/*)#3658f8sn"
```

Here:

* `e18dae20` is a fingerprint of the **master public key** (but we don't actually see it).
* `/84h/1h/0h` is the **account derivation path**.
* `tpubDC4ujMbsd9REzpGk3gnTjkrfJFw1NnvCpx6QBbLj3CHBzcLmVzssTVP8meRAM1WW4pZnK6SCCPGyzi9eMfzSXoe` is the **account public key**.
* `/0/*` is a **ranged address derivation path**.

That's sufficient to regenerate all the external P2WPKH addresses in
this wallet, but it hides the seed and the master private key, which
lowers the ineroperability: we'd have to import each of our eight or
more descriptors independently into another wallet (including four
different private keys), rather than being able to just load up the
seed and tell it what derivation paths to use.

The seed is great, because it's ultimately the only secret you need to
recover your funds (though it might be a lot of work if you don't know
the proper derivation paths, and you could lose funds if you only had
the seed and you used non-standard derivation paths). That's why in
this chapter we're going to step outside of the Bitcoin Core paradigm
to create a seed, then import that into Bitcoin Core by hand.

> 📖 **What is a seed?** A seed is a "top-level" secret in
Bitcoin. It's what's used to generate all the keys that you use to
secure your Bitcoin funds. Technically, if you're using a BIP-32
hierarchical wallet, the seed generates a master key, which generates
account keys, which generates address keys, which generates addresses.

## Install Your Own Seed Generator

We have to go outside of Bitcoin Core to generate seeds (or at least
to generate seeds that we can actually see). To accomplish this (and
later to transform and archive seeds), we're going to use a variety of
command-line tools created by [Blockchain
Commons](https://www.blockchaincommons.com/). The first one, which
generates seeds, is
[`seedtool`](https://github.com/BlockchainCommons/seedtool-cli).

### Install Rust

`seedtool` is a Rust tool, which means that it can easily be installed
with `cargo`, but you have to have Rust installed first. If you
already have Rust installed, skip ahead, otherwise continue on.

If want to install Rust on a Debian machine, such as the Linode that
we suggest, follow a three-step process.

First, install the C compiler:

```
sudo apt-get install build-essential
```

Second, use `curl` to install Rust. The following will install it into
your current account. Choose the "Proceed with standard installation" option when you're
asked:

```
curl https://sh.rustup.rs -sSf | sh
```

Third, reset your environment:

```
. "$HOME/.cargo/env"
```

For Mac, follow a similar procedure, but skip the `apt-get` and
instead `xcode-select --install` if you don't have a C compiler
installed.

For Windows, see Rust's [other installation
methods](https://forge.rust-lang.org/infra/other-installation-methods.html).

### Install Seedtool

Once you have Rust installed, installing Rust packages is easy. The
following will install `seedtool`:

```
cargo install seedtool-cli
```

> ⚠️ **Memory Pressure.** We've had our little 4G Debian Linode get
stuck when we asked it to install multiple Rust crates back to
back. If a `cargo install` seems to get stuck, with "memory pressure"
warnings, reboot the machine and try again (continuing with whatever package you were installing).

## Use Seedtool

Using seedtool is also ridiculously easy. You run `seedtool` and it generates a seed.

```
SEED=$(seedtool)
echo $SEED

| f6cfffce024b5b1a8d8925aa6903d039
```

You've now got a seed that you can use to generate entire hierarchies of Bitcoin addresses!

## Backup Your Seed

Hexadecimal seeds like `f6cfffce024b5b1a8d8925aa6903d039` can be a
little unwieldly. You're not going to remember them, and it would be
easy to corrupt a seed by losing or changing a digit.

Seedtool allows you to translate a seed into a friendlier format by
reading in your hex with a `-i hex` flag and then outputting it in
your favorite format with `-o [type]`. This includes a variety of
output formats that are great for backup.

> ⚠️ **Don't Forget the -i.** If you omit the `-i` flag, `seedtool`
will instead generate a new seed and then convert that. That's almost
definitely not what you want!

`seedtool --help` lists all the possible output (and input)
formats. Three are discussed below: BIP-39 (`bip39`), Bytewords
(`btw`), and SSKR (`sskr`).

[§10.5](10_5_Storing_Secrets_with_Envelope.md) will discuss how to use
Gordian Envelope for even more powerful and resilient storage of
secrets. But these are a great start.

### Backup as BIP-39

[BIP-39](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki)
invented the idea of a mnemonic seed phrase. It allows a seed to be converted
into a list of words that are less prone to errors when you store and
recover them. Choosing `-o bip39` will output your seed as an
English-language BIP-39 word list:

```
seedtool -i hex $SEED -o bip39

| walnut lend vicious afraid remember minute curtain caution price elite village inhale
```

This word list can then be stored somewhere safe. The [#SmartCustody
course](https://www.smartcustody.com/) suggests inscribing it into
metal and locking it in a vault or safe. This should make your seed much less prone to loss (while still protecting it from compromise).

> 📖 **What is a seed phase?** A collection of 12 or 24 ordered
mnemonic words can define a seed.  Though seed phrases are not
currently used by Bitcoin Core, they are in wide use in the larger
Bitcoin ecosystem.

### Backup as Bytewords

Blockchain Commons has its own set of mnemonic words,
[Bytewords](https://developer.blockchaincommons.com/bytewords/), which
are more regular that BIP-39 words and chosen to be easy to remember
and hard to confuse. (They're also the foundation of the
[UR](https://developer.blockchaincommons.com/ur/) and [Animated
QR](https://developer.blockchaincommons.com/animated-qrs/) technology
that you used in [§8.4](08_4_Creating_Animated_QR_Codes.md).)

You can output as Bytewords with `-o btw`

```
seedtool -i hex $SEED -o btw

| yawn task zoom taco also gear help city lung loud data peck iron apex taxi eyes kept miss ramp huts
```

### Backup as SSKR

Backups tend to increase resilience (the odds that your secret is
lost) but also increase the chance of compromise (the odds that your
secret is stolen). The more copies of something you make, the better
your chance of recovering that secret, but the more likely someone
else steals it.

Shamir's Secret Sharing resolves this issue by letting you shard a
secret, distribute the shares created, and then reconstruct your
secret from some fraction (a "threshold") of those shares. It's common
to create shares with a 2-of-3 threshold (there are three shares, but
you can reconstruct from any two) or a 3-of-5 threshold (there are
five shares, but you can reconstruct from any three).

[SSKR](https://developer.blockchaincommons.com/sskr/) is Blockchain
Commons' expansion of Shamir's Secret Sharing, which is built into
reference applications such as `seedtool` and `envelope`.

SSKR shares can be created through `-o sskr` and a `--groups` variable
that defines the threshold. The output is all done in Blockchain
Commons specified formats. The following outputs SSKR shares as URs:

```
seedtool -i hex $SEED -o sskr --groups 2-of-3 --sskr-format ur

| ur:sskr/gocfaaaeadaenskelybeetesvytptnehrytaemsnetjemhpanlcp
| ur:sskr/gocfaaaeadadpdpscknbmtensncnbbdaaasgaholrpsortuywpsp
| ur:sskr/gocfaaaeadaowkstoxjelbdirhechlcftyzmgucwfheetdeooxvl
```

`seedtool -i sskr` can then be used to input any two of these shares
and regenerate the seed:

```
seedtool -i sskr
ur:sskr/gocfaaaeadaenskelybeetesvytptnehrytaemsnetjemhpanlcp
ur:sskr/gocfaaaeadaowkstoxjelbdirhechlcftyzmgucwfheetdeooxvl
^D

| f6cfffce024b5b1a8d8925aa6903d039
```

[#SmartCustody](https://github.com/BlockchainCommons/SmartCustody/blob/master/Docs/SSKR-Sharing.md)
contains more information on how to design a scenario for how many
shares to create, where to store them, and what threshold to use.

## Summary: Creating Secrets from the Command Line

This section offered two simple lessons: how to create a seed and why
you'd want to. Doing so allows the ultimate portability: you can use
that same seed to create the same keys and addresses in different
wallets.

> 🔥 **_What is the Power of Seeds?_** Seeds are as far as you can get
from a bag of keys. They are a singular secret that you can use to
create many different addresses in many different accounts. They're
the intended root of a HD wallet, even if they're not visible in
Bitcoin Core.

## What's Next?

Continue "Exploring the Ecosystem" with [§10.2: Converting Secrets with
Keytool](10_2_Converting_Secrets_with_Keytool.md).


