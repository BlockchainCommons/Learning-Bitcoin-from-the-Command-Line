# 17.2: Updating to Taproot

You probably don't need to update to Taproot, but if you do, here's how.

## Check for Taproot

This course generally assume that you installed a brand-new Bitcoin
node in [chapter
2](https://learningbitcoin.blockchaincommons.com/02_0_Setting_Up_a_Bitcoin-Core_VPS/),
using an up-to-date version of Bitcoin. If that's so, you're already
ready to use Taproot.

If you're not sure, you can verify with `listdescriptors`

```sh
bitcoin-cli listdescriptors | jq -r '.descriptors[].desc'

| pkh([1f7ddd8d/44h/1h/0h]tpubDDjV4QxW5nqaYRjy8i4okDjZruRRouFLnmVSu1qpVVyD7usAKfv5z9bs2mnFtmFo8wQGi4ydzUH4gv5s31Um99daWboM78QMWA1KCvQ2nu5/0/*)#nf7xv0aj
| pkh([1f7ddd8d/44h/1h/0h]tpubDDjV4QxW5nqaYRjy8i4okDjZruRRouFLnmVSu1qpVVyD7usAKfv5z9bs2mnFtmFo8wQGi4ydzUH4gv5s31Um99daWboM78QMWA1KCvQ2nu5/1/*)#zam836d2
| sh(wpkh([1f7ddd8d/49h/1h/0h]tpubDCdQP2kjQwGUNad1EyeTC7hZbDFKHCBfrwzn3gntgw82zig6thuWfG49MeqqTUzK8TxDDtNYyBG7n6PvrSgkdgEFhyh4g4TeQtLaUrdk4bX/0/*))#fh4xr6nn
| sh(wpkh([1f7ddd8d/49h/1h/0h]tpubDCdQP2kjQwGUNad1EyeTC7hZbDFKHCBfrwzn3gntgw82zig6thuWfG49MeqqTUzK8TxDDtNYyBG7n6PvrSgkdgEFhyh4g4TeQtLaUrdk4bX/1/*))#ukmsm9xv
| tr([1f7ddd8d/86h/1h/0h]tpubDD5YYFNXU51S6XzqwNBbZtvTxZxtH8C2c4HCPvccaAVjiD9eY263XSrD4aEVXmVfz8GwDaxtPDHGw81HKJmxK71N1Y9BQR9qwd7CrdwoRgz/0/*)#vcee90vh
| tr([1f7ddd8d/86h/1h/0h]tpubDD5YYFNXU51S6XzqwNBbZtvTxZxtH8C2c4HCPvccaAVjiD9eY263XSrD4aEVXmVfz8GwDaxtPDHGw81HKJmxK71N1Y9BQR9qwd7CrdwoRgz/1/*)#avucc6u0
| wpkh([1f7ddd8d/84h/1h/0h]tpubDD8B4TJhEgVZ62qtR6xm1L7xorbJnpxs9YSr9qdjzHAyV4hD8b8d4Bv9ArVFKzUkaPYJht7acwDtJq34Yabi8ocehVgfRy16Dry3iJDqbQj/0/*)#f3qxhd2n
| wpkh([1f7ddd8d/84h/1h/0h]tpubDD8B4TJhEgVZ62qtR6xm1L7xorbJnpxs9YSr9qdjzHAyV4hD8b8d4Bv9ArVFKzUkaPYJht7acwDtJq34Yabi8ocehVgfRy16Dry3iJDqbQj/1/*)#c9982c6t
```

If you've got `tr(...)` descriptors with a derivation path starting
with `86h`, that's a Taproot descriptor.

Here's a single line that will verify it:

```sh
if (bitcoin-cli listdescriptors | jq -r '.descriptors[].desc' | grep "86h" | grep "tr") >& /dev/null; then echo "✅ Taproot descriptors are present"; fi

| ✅ Taproot descriptors are present
```

## Update to Taproot

If your wallet is missing the Taproot descriptors (or is missing any
future descriptor types), you can use the `createwalletdescriptor`
command to add them by requesting the appropriate address type. In
this case, `bech32m` is the address type used by P2TR/Taproot
addresses.

```sh
bitcoin-cli createwalletdescriptor bech32m

| {
|   "descs": [
|     "tr([3f3521a6/86h/1h/0h]tpubDCW7vJKTJGE1e5WTX7mWDmafQmsEMpaVbqFkZih8uQympr8a1FZkw7paeNXkVgE28jdZd13aexRzuCzF3NfuzQNFgAqmSU9p1DDrYeRUUoE/0/*)#e7freggd",
|     "tr([3f3521a6/86h/1h/0h]tpubDCW7vJKTJGE1e5WTX7mWDmafQmsEMpaVbqFkZih8uQympr8a1FZkw7paeNXkVgE28jdZd13aexRzuCzF3NfuzQNFgAqmSU9p1DDrYeRUUoE/1/*)#g2vzyac4"
|   ]
| }
```

Afterward, your Taproot descriptors will be part of your `listdescriptors` listing:

```sh
if (bitcoin-cli listdescriptors | jq -r '.descriptors[].desc' | grep "86h" | grep "tr") >& /dev/null; then echo "✅ Taproot descriptors are present"; fi
✅ Taproot descriptors are present
```

If you make a mistake and try to create Taproot descriptors that already exist, no problem:

```sh
bitcoin-cli createwalletdescriptor bech32m

| error code: -4
| error message:
| Descriptor already exists
```

## Summary: Using BDK with Miniscript

Your wallet is likely already set up for Taproot. If it's not, just
use `createwalletdescriptor bech32m` to do so.

## What's Next?

Continue "Using Taproot" with [Chapter Eightteen: Using
Schnorr](18_0_Using_Schnorr.md).

