# 10.5: Storing Secrets with Envelope

Properly storing secrets is more than just sticking your secret in a
file. You also want to (1) include metadata, so you know what the
secret is; and (2) protect the secret from casual spying or loss. This
section explains how to do it using Blockchain Commons' [Gordian
Envelope](https://developer.blockchaincommons.com/envelope/).

## Install Gordian Envelope

You can install Gordian Envelope using Rust's `cargo` utility. If you
already [installed
Rust](https://learningbitcoin.blockchaincommons.com/10_1_Creating_Secrets_from_the_Command_Line/#install-rust)
in §10.1, all you need to do is install `bc-envelope-cli` with `cargo`:

```
cargo install bc-envelope-cli
```

> ⚠️ **Memory Pressure.** This is where we've most often had problems
with `cargo` locking up: when installing `bc-envelope-cli` after
installing `seedtool-cli` earlier in the same session. Just be ready
to reboot if necessary and you should be able to do a clean install
afterward.

## Understand Gordian Envelope

Gordian Envelope is a structured "smart document" system that allows
for the storage of semantic triplets in the form of
subject/predicate/object. That's a subject followed by an assertion
about the subject that itself is made up of a predicate and an object.

Alice (subject) / knows (predicate) / Bob (object) is a simple
example. More usefully we might have "f6cfffce024b5b1a8d8925aa6903d039
(subject) / 'isA' (predicate) / 'seed' (object)", which not only
archives the seed, but defines what it is.

In this section, we're going to be building envelopes of secrets and
assertions about those secrets, with the metadata (the assertions)
meant to aid in the recovery of assets based on those secrets.

Envelope is recursive, meaning that any part of an envelope can have
assertions placed on them, to an infinite depth. Envelope also allows
for signing, encryption, and redaction of individual parts of the
envelope. We're going to keep things simple, only displaying the
encryption capabilities, but be aware that there are many more
possibilities. The [Envelope developers
page](https://developer.blockchaincommons.com/envelope/) is a good
place to get started in exploring those possibilities.

## Store a Seed

The first thing you're going to do is store the seed we generated in
[§10.1](10_1_Creating_Secrets_from_the_Command_Line.md), which is
hopefully still stored in `$SEED`.

We'll do so with the standard `envelope-cli` commands that we're going
to use throughout this section: `envelope subject` which creates a
subject for an envelope, then `envelope assertion add pred-obj`, which
adds an assertion containing a predicate and object to an envelope.

In each case we'll need to define the type of content we're adding to
the envelope. It will usually be a `string` unless there's a
pre-defined `known` value as defined in the [Blockchain Commons
registry](https://github.com/BlockchainCommons/Research/blob/master/known-value-assignments/markdown/0_blockchain_commons_registry.md).

You can create a basic seed envelope, like the one we described above,
as follows:

```
SEED_ENVELOPE=$(envelope subject type string "$SEED")
SEED_ENVELOPE=$(envelope assertion add pred-obj known 'isA' known 'Seed' "$SEED_ENVELOPE")
```

Envelope will store your data as a `ur:envelope`:

```
echo $SEED_ENVELOPE

| ur:envelope/lftpsokscxiyeniaiyiyiyiaihdyeyeeidecidehhsetieeteseyechshsenesdyeoiedyeoesoyadcsspcmceuosn
```

You can always look at your envelope with the `envelope format` command:

```
$ envelope format $SEED_ENVELOPE
"f6cfffce024b5b1a8d8925aa6903d039" [
    'isA': 'Seed'
]
```

We're going to add a bit more information to this:

* Where the seed was created (in case there's ever a zero-day exploit
for that software).
* Where the seed was used.
* What derivation paths were used.

```
SEED_ENVELOPE=$(envelope assertion add pred-obj string "createdBy" string "`seedtool -V`" "$SEED_ENVELOPE")
SEED_ENVELOPE=$(envelope assertion add pred-obj string "usedBy" string "`bitcoin-cli --version | head -1`" "$SEED_ENVELOPE")
SEED_ENVELOPE=$(envelope assertion add pred-obj known 'DerivationPath' string "m/44h/0h/0h" "$SEED_ENVELOPE")
SEED_ENVELOPE=$(envelope assertion add pred-obj known 'DerivationPath' string "m/84h/0h/0h" "$SEED_ENVELOPE")
```

The goal of metadata is to ensure that you have enough data to
actually make use of any secret that you recover. This looks like a
pretty nice collection of information for doing so:

```
envelope format $SEED_ENVELOPE

| "f6cfffce024b5b1a8d8925aa6903d039" [
|     'isA': 'Seed'
|     "createdBy": "seedtool-cli 0.4.0"
|     "usedBy": "Bitcoin Core RPC client version v30.2.0"
|     'DerivationPath': "m/44h/0h/0h"
|     'DerivationPath': "m/84h/0h/0h"
| ]
```

## Store a Master Private Key

In [§10.4](10_4_Exporting_Secrets_from_Bitcoin_Core.md), you extracted
a master private key and descriptors from Bitcoin Core. You can create
a similar Gordian Envelope to hold each of these. Here's a seed for the key:

```
KEY_ENVELOPE=$(envelope subject type string "$MP_KEY")
KEY_ENVELOPE=$(envelope assertion add pred-obj known 'isA' known 'MasterKey' "$KEY_ENVELOPE")
KEY_ENVELOPE=$(envelope assertion add pred-obj string "createdBy" string "`bitcoin-cli --version | head -1`" "$KEY_ENVELOPE")
KEY_ENVELOPE=$(envelope assertion add pred-obj string "usedBy" string "`bitcoin-cli --version | head -1`" "$KEY_ENVELOPE")
KEY_ENVELOPE=$(envelope assertion add pred-obj known 'DerivationPath' string "m/44h/0h/0h" "$KEY_ENVELOPE")
KEY_ENVELOPE=$(envelope assertion add pred-obj known 'DerivationPath' string "m/84h/0h/0h" "$KEY_ENVELOPE")
KEY_ENVELOPE=$(envelope assertion add pred-obj known 'DerivationPath' string "m/49h/0h/0h" "$KEY_ENVELOPE")
KEY_ENVELOPE=$(envelope assertion add pred-obj known 'DerivationPath' string "m/86h/0h/0h" "$KEY_ENVELOPE")
```

Here's what it looks like:

```
envelope format $KEY_ENVELOPE

| "xprv9s21ZrQH143K4YGw3kaAmh3ezx1Xo4AanFmnX8TbaGghuNxXP9MDt2rPrLDyouX5kg9fiDJn7BELPnWRKwtVFn6aUBaNYJC9Mu6s39DnAo4" [
|     'isA': 'MasterKey'
|     "createdBy": "Bitcoin Core RPC client version v30.2.0"
|     "usedBy": "Bitcoin Core RPC client version v30.2.0"
|     'DerivationPath': "m/44h/0h/0h"
|     'DerivationPath': "m/49h/0h/0h"
|     'DerivationPath': "m/84h/0h/0h"
|     'DerivationPath': "m/86h/0h/0h"
| ]
```

Note that we've changed the `isA` to describe the master key rather
than a seed. We've also shown that it was created by Bitcoin Core, and
we've listed out the whole set of address derivations that are used by
Bitcoin Core.

## Store Descriptors

Finally, if we want, we can store what we'd have to call a
"bag-of-descriptors".

This puts all of the descriptors from §10.4 in a single envelope:

```
DESC_ENVELOPE_1=$(envelope subject type string "descriptors-for-bitcoin")
DESC_ENVELOPE_1=$(envelope assertion add pred-obj known 'isA' string "collectionOfDescriptors" "$DESC_ENVELOPE_1")
DESC_ENVELOPE_1=$(envelope assertion add pred-obj string "createdBy" string "`bitcoin-cli --version | head -1`" "$DESC_ENVELOPE_1")
DESC_ENVELOPE_1=$(envelope assertion add pred-obj string "usedBy" string "`bitcoin-cli --version | head -1`" "$DESC_ENVELOPE_1")
DESC_ENVELOPE_1=$(envelope assertion add pred-obj known 'OutputDescriptor' string "${DESC_ARRAY[0]}" "$DESC_ENVELOPE_1")
DESC_ENVELOPE_1=$(envelope assertion add pred-obj known 'OutputDescriptor' string "${DESC_ARRAY[1]}" "$DESC_ENVELOPE_1")
DESC_ENVELOPE_1=$(envelope assertion add pred-obj known 'OutputDescriptor' string "${DESC_ARRAY[2]}" "$DESC_ENVELOPE_1")
DESC_ENVELOPE_1=$(envelope assertion add pred-obj known 'OutputDescriptor' string "${DESC_ARRAY[3]}" "$DESC_ENVELOPE_1")
DESC_ENVELOPE_1=$(envelope assertion add pred-obj known 'OutputDescriptor' string "${DESC_ARRAY[4]}" "$DESC_ENVELOPE_1")
DESC_ENVELOPE_1=$(envelope assertion add pred-obj known 'OutputDescriptor' string "${DESC_ARRAY[5]}" "$DESC_ENVELOPE_1")
DESC_ENVELOPE_1=$(envelope assertion add pred-obj known 'OutputDescriptor' string "${DESC_ARRAY[6]}" "$DESC_ENVELOPE_1")
DESC_ENVELOPE_1=$(envelope assertion add pred-obj known 'OutputDescriptor' string "${DESC_ARRAY[7]}" "$DESC_ENVELOPE_1")
```

Again, here's what that looks like:

```
envelope format $DESC_ENVELOPE_1

| "descriptors-for-bitcoin" [
|     'isA': "collectionOfDescriptors"
|     "createdBy": "Bitcoin Core RPC client version v30.2.0"
|     "usedBy": "Bitcoin Core RPC client version v30.2.0"
|     'OutputDescriptor': "pkh(xprv9s21ZrQH143K4YGw3kaAmh3ezx1Xo4AanFmnX8TbaGghuNxXP9MDt2rPrLDyouX5kg9fiDJn7BELPnWRKwtVFn6aUBaNYJC9Mu6s39DnAo4/44h/0h/0h/0/*)#e0uydrgt"
|     'OutputDescriptor': "pkh(xprv9s21ZrQH143K4YGw3kaAmh3ezx1Xo4AanFmnX8TbaGghuNxXP9MDt2rPrLDyouX5kg9fiDJn7BELPnWRKwtVFn6aUBaNYJC9Mu6s39DnAo4/44h/0h/0h/1/*)#gme9skcn"
|     'OutputDescriptor': "sh(wpkh(xprv9s21ZrQH143K4YGw3kaAmh3ezx1Xo4AanFmnX8TbaGghuNxXP9MDt2rPrLDyouX5kg9fiDJn7BELPnWRKwtVFn6aUBaNYJC9Mu6s39DnAo4/49h/0h/0h/0/*))#pfqdkhdr"
|     'OutputDescriptor': "sh(wpkh(xprv9s21ZrQH143K4YGw3kaAmh3ezx1Xo4AanFmnX8TbaGghuNxXP9MDt2rPrLDyouX5kg9fiDJn7BELPnWRKwtVFn6aUBaNYJC9Mu6s39DnAo4/49h/0h/0h/1/*))#82ggd6xh"
|     'OutputDescriptor': "tr(xprv9s21ZrQH143K4YGw3kaAmh3ezx1Xo4AanFmnX8TbaGghuNxXP9MDt2rPrLDyouX5kg9fiDJn7BELPnWRKwtVFn6aUBaNYJC9Mu6s39DnAo4/86h/0h/0h/0/*)#q58cmp0l"
|     'OutputDescriptor': "tr(xprv9s21ZrQH143K4YGw3kaAmh3ezx1Xo4AanFmnX8TbaGghuNxXP9MDt2rPrLDyouX5kg9fiDJn7BELPnWRKwtVFn6aUBaNYJC9Mu6s39DnAo4/86h/0h/0h/1/*)#3qzex5l8"
|     'OutputDescriptor': "wpkh(xprv9s21ZrQH143K4YGw3kaAmh3ezx1Xo4AanFmnX8TbaGghuNxXP9MDt2rPrLDyouX5kg9fiDJn7BELPnWRKwtVFn6aUBaNYJC9Mu6s39DnAo4/84h/0h/0h/0/*)#u8jwrsql"
|     'OutputDescriptor': "wpkh(xprv9s21ZrQH143K4YGw3kaAmh3ezx1Xo4AanFmnX8TbaGghuNxXP9MDt2rPrLDyouX5kg9fiDJn7BELPnWRKwtVFn6aUBaNYJC9Mu6s39DnAo4/84h/0h/0h/1/*)#dnh079s8"
| ]
```

Although this isn't quite as neat as building an envelope around a
seed or a master key and then describing it, it's very utilitarian, as
it's the complete list of output descriptors needed to rebuild a
Bitcoin Core wallet, without having to depend on the wallet.dat/SQLite
format.

## Store Individually Secured Descriptions

In the last section we noted that using the master private keys in the
descriptors increased the chances of compromise. That doesn't matter
if we're putting everything in the same bag, as above. But, if you
wanted to store a descriptor individually, you might want to decrease
correlation by using the account private key rather than the master
private key in the descriptor.

If you installed `keytool` in
[§10.2](10_2_Converting_Secrets_with_Keytool.md), you can use it to
convert your master key into a specific account key

The following `keytool` command outputs both the account private key
and the public output descriptor based on the master key:

```
$ keytool --master-key $MP_KEY --account-derivation-path "m/44h/0h/0h" account-key-base58 output-descriptor

| xprv9xy8w97WS4kmg1BsEYGpDSeePDe5EZJBtSQbNBdmx3Rr53T452BBNR3o9mwpDzqm6zMfbjbiTEvGrtJ9y9u1rUjb8CYMHBzuzbJvzifv1i4
| wpkh([44h/0h/0h]xpub6BxVLeeQGSK4tVGLLZopaabNwFUZe223FfLCAa3PWNxpwqnCcZVRvDNH13fKh4Ea5w785U9a7u1LX5Tu9m9gpdi2i5PEE6aMNvXGi2LuKdV/0/0)
```

You just substitute the private key into the descriptor and you have a
private account descriptor:

```
ACCOUNT_KEY_44H="wpkh([44h/0h/0h]xprv9xy8w97WS4kmg1BsEYGpDSeePDe5EZJBtSQbNBdmx3Rr53T452BBNR3o9mwpDzqm6zMfbjbiTEvGrtJ9y9u1rUjb8CYMHBzuzbJvzifv1i4/0/0)"
```

You can then build an individual descriptor envelope:

```
DESC_ENVELOPE_2=$(envelope subject type string "$ACCOUNT_KEY_44H")
DESC_ENVELOPE_2=$(envelope assertion add pred-obj known 'isA' known 'outputDescriptor' "$DESC_ENVELOPE_2")
DESC_ENVELOPE_2=$(envelope assertion add pred-obj string "createdBy" string "`bitcoin-cli --version | head -1`" "$DESC_ENVELOPE_2")
DESC_ENVELOPE_2=$(envelope assertion add pred-obj string "usedBy" string "`bitcoin-cli --version | head -1`" "$DESC_ENVELOPE_2")
```

Here's what this last envelope looks like:

```
envelope format $DESC_ENVELOPE_2

| "wpkh([44h/0h/0h]xprv9xy8w97WS4kmg1BsEYGpDSeePDe5EZJBtSQbNBdmx3Rr53T452BBNR3o9mwpDzqm6zMfbjbiTEvGrtJ9y9u1rUjb8CYMHBzuzbJvzifv1i4/0/0)" [
|     'isA': 'outputDescriptor'
|     "createdBy": "Bitcoin Core RPC client version v30.2.0"
|     "usedBy": "Bitcoin Core RPC client version v30.2.0"
| ]
```

You'd presumably build one of these for each descriptor account that
was actually in use and contained funds. The advantage is that if one
of these envelopes is compromised, the rest of the accounts are not,
where they would be if you instead using the original descriptor from
Bitcoin Core, which contains the master private key.

## Secure Envelopes

The envelopes you've created to date are great for making sure you
don't lose the secrets underlying your Bitcoin assets. But, they're
awful for security. Though they're encoded in CBOR, they're stored as
URs, and one of the points of URs is that they're
self-describing. People _should_ be able to figure out what to do with
an envelope when they receive the encoded UR string.

That means that you probably want to provide some better security for
your secrets. Envelope offers two ways to do so: encryption or SSKR.

### Encrypt an Envelope

Encrypting an envelope offers very strong protection, but the catch is
that you have to have the decryption key to recover the data. That can
just transfer the potential single-point-of-failure from the Bitcoin
secret to the encryption key.

The best solution to this is probably to use a stretched password for
encryption rather than an encryption key. It's certainly not as
secure, but it can give you something that you can actually remember
(or pass on to loved ones).

An envelope can be encrypted with the `envelope encrypt` command,
using the `--password` argument to stretch a password:

```
KEY_ENVELOPE_ENC=$(envelope encrypt --password my-simple-pass "$KEY_ENVELOPE")
```

When you look at your envelope you'll see that only the subject is
encrypted:

```
envelope format $KEY_ENVELOPE_ENC

| ENCRYPTED [
|     'isA': 'MasterKey'
|     "createdBy": "Bitcoin Core RPC client version v30.2.0"
|     "usedBy": "Bitcoin Core RPC client version v30.2.0"
|     'DerivationPath': "m/44h/0h/0h"
|     'DerivationPath': "m/49h/0h/0h"
|     'DerivationPath': "m/84h/0h/0h"
|     'DerivationPath': "m/86h/0h/0h"
|     'hasSecret': EncryptedKey(Argon2id)
| ]
```

This is intentional: just like assertions are applied to the subject
of an envelope, encryption is applied to the subject of the
envelope. The advantage of this is that you can even add assertions
defining how to recover the password!

```
KEY_ENVELOPE_ENC=$(envelope assertion add pred-obj string "passwordClue" string "located on 1Pass as 'simple password'" "$KEY_ENVELOPE_ENC")
```

Now, you or someone else can return to this envelope sometime in the
far future and hopefully figure out what it is and how to access its
content. (That's the power of metadata!)

Most of our examples were built with the secret as the subject,
because we knew we were going to encrypt them later. But, sometimes
that's not possible, as with the bag-of-descriptors envelope.

No problem. In this case you need to "wrap" the envelope, which puts
the entire envelope into the subject, and then you encrypt that.

```
DESC_ENVELOPE_WRAPPED=$(envelope subject type wrapped "$DESC_ENVELOPE_1")
DESC_ENVELOPE_ENC=$(envelope encrypt --password my-simple-pass "$DESC_ENVELOPE_WRAPPED")
```

You can still add new assertions after the wrapping (and encrypting),
for example if you want to add in that password clue:

```
DESC_ENVELOPE_ENC=$(envelope assertion add pred-obj string "passwordClue" string "located on 1Pass as 'simple password'" "$DESC_ENVELOPE_ENC")
```

You might choose to add other metadata outside the encryption, but if
not, here's what you have:

```
envelope format $DESC_ENVELOPE_ENC

| ENCRYPTED [
|     "passwordClue": "located on 1Pass as 'simple password'"
|     'hasSecret': EncryptedKey(Argon2id)
| ]
```

### Shard an Envelope

An alternative option is to shard an envelope using SSKR. You met the
technology in [§10.1](10_1_Creating_Secrets_from_the_Command_Line.md)
when you sharded a seed. Sharding an envelope works just the same,
except you use the `envelope sskr split` command and the `--group`
argument`. The latter determins how many shares to create, and what
the threshold is (meaning: how many shares you need to recover the
complete envelope).

```
KEY_SHARES=$(envelope sskr split --group "2-of-3" $KEY_ENVELOPE)
KEY_ARRAY=($KEY_SHARES)
```

This put three shards in ${KEY_ARRAY[0]}, ${KEY_ARRAY[1]}, and
${KEY_ARRAY[2]}. You'd want to divide them up into safe, separate,
geographically distinct areas. Any two could then restore the original
envelope:

```
RESTORED_KEY=$(envelope sskr join "${KEY_ARRAY[0]}" "${KEY_ARRAY[1]}")

envelope format $RESTORED_KEY

| "xprv9s21ZrQH143K4YGw3kaAmh3ezx1Xo4AanFmnX8TbaGghuNxXP9MDt2rPrLDyouX5kg9fiDJn7BELPnWRKwtVFn6aUBaNYJC9Mu6s39DnAo4" [
|     'isA': 'MasterKey'
|     "createdBy": "Bitcoin Core RPC client version v30.2.0"
|     "usedBy": "Bitcoin Core RPC client version v30.2.0"
|     'DerivationPath': "m/44h/0h/0h"
|     'DerivationPath': "m/49h/0h/0h"
|     'DerivationPath': "m/84h/0h/0h"
|     'DerivationPath': "m/86h/0h/0h"
|]
```

In many ways, this is the most powerful of the storage methods described here.

* Unlike when you store a bare file, you don't have to worry about compromise (as long as you carefully separate your shares).
* Unlike when you store an encrypted file, you don't have to worry about loss (as long as you don't lose multiple shares).

## Summary: Storing Secrets with Envelope

You can store the seed you created in §10.1 or the master key or
descriptors you extracted from Bitcoin Core in §10.4 in a Gordian
Envelope. By adding metadata, you can create enough context to recover
your digital assets at a later time. By encrypting or sharding the
envelope, you can keep it safe from compromise or loss.

## What's Next?

Move on to "Bitcoin Scripting" with [Chapter Elven: Introducing
Bitcoin Scripts](11_0_Introducing_Bitcoin_Scripts.md).