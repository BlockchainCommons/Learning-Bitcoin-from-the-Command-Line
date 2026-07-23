# 16.3: Using Miniscript with Descriptors

Miniscript and Policy are great for designing scripts outside of
Bitcoin Core that can then be converted into Bitcoin Script and turned
into addresses.

Some wallets offer deeper usage of Miniscript, but Bitcoin Core
integration currently remains limited: to date you can only use
Miniscript with Bitcoin Core to create read-only P2WSH
addresses. Doing so requires a combination of two things you've
already learned: first you convert from Policy to Miniscript; then you
create a descriptor to read into Bitcoin Core. (We call the result a
"minidescriptor", but that's not a formal term.)

## Create a P2WSH Minidescriptor

Creating a minidescriptor for import into Bitcoin Core is a simple
five-step process.

### 1. Create a Spending Policy

We're going to use our president & vice-president policy from
previous chapters:

```
or(pk(president),thresh(2,pk(vp1),pk(vp2),pk(vp3)))
```
   
### 2. Convert the Policy to Miniscript

Compiling our Policy at
[bitcoin.sipa.be](https://bitcoin.sipa.be/miniscript/) gives us our
Miniscript:

```
or_d(pk(president),multi(2,vp1,vp2,vp3))
```

### 3. Gather the Keys

To enter a minidescriptor into Bitcoin Core requires that you fill
in the keys that should be used using base58 format. Since these
Miniscripts are currently read-only, that means entering the xpubs,

Various people involved in a Minidescriptor might have created their
keys in a variety of ways. Our executives used `seedtool` and
`keytool` from [chapter 10](10_0_Working_with_Secrets.md) to create
their secrets as follows:

```
SEEDP=$(seedtool)
KEYP=$(keytool --seed $SEEDP --account-derivation-path m/84h/1h/0h/0h --network testnet account-pub-key-base58)
echo $KEYP

| tpubDFhGpPUEJaipdEeJtXZAUwb3Hz34hRCCqhvNgRwzrnbsGfXhCNEroYbGVHaVAyTAmJK4HXxLGCTXjP9HjwQRnG3oW7hCFY96JZkmo6kkCdt

SEEDVP1=$(seedtool)
KEYVP1=$(keytool --seed $SEEDVP1 --account-derivation-path m/84h/1h/0h/0h --network testnet account-pub-key-base58)
echo $KEYVP1

|tpubDE1YvUwRqg2N2JfU5LKDGuuv3AM2d2CgMYQauAxgoWnGMVaHETz8n2Cdz3VGi9bbfFeY9dRrydeshyQzqPBLPwSEV8hRBZj5JMEqV3rTkc6

SEEDVP2=$(seedtool)
KEYVP2=$(keytool --seed $SEEDVP2 --account-derivation-path m/84h/1h/0h/0h --network testnet account-pub-key-base58)
echo $KEYVP2

| tpubDE8xY8eWp6CZcdeVQFYG7c2H12GeUpN5j5WLpGEYxurygx5HfjkcbN28vjMmTdYHRNvPauKRsKdgwuJ4hiyJ93gnXPadBoiZeUUJsQ2sP37

SEEDVP3=$(seedtool)
KEYVP3=$(keytool --seed $SEEDVP3 --account-derivation-path m/84h/1h/0h/0h --network testnet account-pub-key-base58)
echo $KEYVP3
   
| tpubDE4kssfB4wqobfE3GWmYFkyV7giscXNG9HbXu2NykHoF1z9Tf4osozgPX4VZ8MnWxytKfeXAa89oZi9NUNauoLYcY51gFghUia34aAuj6sb
   ```

### 4. Place the Keys in the Minidescriptor

We gathered `84h/1h/0h/0h` keys. They're intended to support coins
that are Segwit (`84h`), testnet (`1h`), first account (`0h`),
non-change (`0h`). To allow for a range of addresses, we then
supplement each key with a `/*` when we place it in our minscript.
   
(⚠️ We would have liked to collect `84h/1h/0h` keys and then defined
them as `0h/*`, which would be more normative use, but Bitcoin Core
currently flags that as "duplicate keys" if you try, even though
they're not.)

```
EXEC_MINID=$(echo "or_d(pk($KEYP/*),multi(2,$KEYVP1/*,$KEYVP2/*,$KEYVP3/*))")
echo $EXEC_MINID

| or_d(pk(tpubDFhGpPUEJaipdEeJtXZAUwb3Hz34hRCCqhvNgRwzrnbsGfXhCNEroYbGVHaVAyTAmJK4HXxLGCTXjP9HjwQRnG3oW7hCFY96JZkmo6kkCdt/*),multi(2,tpubDE1YvUwRqg2N2JfU5LKDGuuv3AM2d2CgMYQauAxgoWnGMVaHETz8n2Cdz3VGi9bbfFeY9dRrydeshyQzqPBLPwSEV8hRBZj5JMEqV3rTkc6/*,tpubDE8xY8eWp6CZcdeVQFYG7c2H12GeUpN5j5WLpGEYxurygx5HfjkcbN28vjMmTdYHRNvPauKRsKdgwuJ4hiyJ93gnXPadBoiZeUUJsQ2sP37/*,tpubDE4kssfB4wqobfE3GWmYFkyV7giscXNG9HbXu2NykHoF1z9Tf4osozgPX4VZ8MnWxytKfeXAa89oZi9NUNauoLYcY51gFghUia34aAuj6sb/*))
```

5. **Make a Wish**

To turn our Miniscript into a descriptor (a minidescriptor)
requires one more thing. It has to be defined with a normal
descriptor function.

Minidescriptors are currently only readable in Bitcoin Core as P2WSH
addresses (not P2TR addresses). That requires a final step of
embedding the Minidescriptor in `wsh(...)` to create a legal
descriptor for Bitcoin Core to read.

```
EXEC_MINID=$(echo "wsh($EXEC_MINID)")
echo $EXEC_MINID

| wsh(or_d(pk(tpubDFhGpPUEJaipdEeJtXZAUwb3Hz34hRCCqhvNgRwzrnbsGfXhCNEroYbGVHaVAyTAmJK4HXxLGCTXjP9HjwQRnG3oW7hCFY96JZkmo6kkCdt/*),multi(2,tpubDE1YvUwRqg2N2JfU5LKDGuuv3AM2d2CgMYQauAxgoWnGMVaHETz8n2Cdz3VGi9bbfFeY9dRrydeshyQzqPBLPwSEV8hRBZj5JMEqV3rTkc6/*,tpubDE8xY8eWp6CZcdeVQFYG7c2H12GeUpN5j5WLpGEYxurygx5HfjkcbN28vjMmTdYHRNvPauKRsKdgwuJ4hiyJ93gnXPadBoiZeUUJsQ2sP37/*,tpubDE4kssfB4wqobfE3GWmYFkyV7giscXNG9HbXu2NykHoF1z9Tf4osozgPX4VZ8MnWxytKfeXAa89oZi9NUNauoLYcY51gFghUia34aAuj6sb/*)))
```

## Import a P2WSH Minidescriptor

The Minidescriptor that you've created should theoretically be
readable anywhere that supports Miniscript descriptors. To incorporate
it specifically into Bitcoin Core requires the following additional
steps:

### 6. Add the Checksum

As you'll recall, you always need a checksum for Bitcoin Core
descriptors.

```
EXEC_MINID_CS=$(bitcoin-cli getdescriptorinfo $EXEC_MINID | jq -r '.checksum')
EXEC_MINID=$(echo $EXEC_MINID#$EXEC_MINID_CS)
echo $EXEC_MINID

| wsh(or_d(pk(tpubDFhGpPUEJaipdEeJtXZAUwb3Hz34hRCCqhvNgRwzrnbsGfXhCNEroYbGVHaVAyTAmJK4HXxLGCTXjP9HjwQRnG3oW7hCFY96JZkmo6kkCdt/*),multi(2,tpubDE1YvUwRqg2N2JfU5LKDGuuv3AM2d2CgMYQauAxgoWnGMVaHETz8n2Cdz3VGi9bbfFeY9dRrydeshyQzqPBLPwSEV8hRBZj5JMEqV3rTkc6/*,tpubDE8xY8eWp6CZcdeVQFYG7c2H12GeUpN5j5WLpGEYxurygx5HfjkcbN28vjMmTdYHRNvPauKRsKdgwuJ4hiyJ93gnXPadBoiZeUUJsQ2sP37/*,tpubDE4kssfB4wqobfE3GWmYFkyV7giscXNG9HbXu2NykHoF1z9Tf4osozgPX4VZ8MnWxytKfeXAa89oZi9NUNauoLYcY51gFghUia34aAuj6sb/*)))#fhq2j999
```

### 7. Double-check Your Addresses

Before you import your Minidescriptor it's best practice to make sure
you have the right one. You can check this by verifying the addresses
it creates with the `deriveaddresses` command.

```
bitcoin-cli deriveaddresses $EXEC_MINID 2

| [
|   "tb1q5wj3hm4vask5zk2xzgdgw9slcpufml37yjnh0jhwf2c3sguuymhqsxc2gd",
|   "tb1qyxz7dv9859evxy5uutn3ueer978l73fw9ew0r2ajnj2lp5xltzpsrarv0q",
|   "tb1qy2s9l6jkhpnwfeyrsl88jj7e88a9k0dw0c8fyudeqv5lnqapm86q69zuue"
| ]
```

Presumably you've got another version of this miniscripted account
somewhere else, on the wallet you're spending from. You should check
the range of addresses you derived with Bitcoin Core against the first
few addresses on that other wallet and make sure they are the same.

### 8. Import Your Minidescriptor

Finally, you're ready to import your minidescriptor. You want to do
so into a brand-new wallet:

```
bitcoin-cli -named createwallet wallet_name="watchmini" disable_private_keys=true blank=true
```

Like when we imported other watch-only addresses in
[§7.1](07_1_Creating_Multisig_Public_Keys.md), we create the wallet
without any private keys.

Now you can import:
   
```
bitcoin-cli -rpcwallet=watchmini importdescriptors '''[{ "desc":
"'$EXEC_MINID'", "timestamp": 1790329126 }]'''
   
| [
|   {
|     "success": true,
|     "warnings": [
|       "Range not given, using default keypool range"
|     ]
|   }
| ]
```

You now have a watch-only wallet that you can use to monitor a range
of addresses that you defined through Miniscript!

## Spend Your Funds

Obviously, because this is a watch-only address, you can't spend the
funds from Bitcoin Core. We've set it up this way because this is the
only limited, trial functionality that Bitcoin Core has for
minidescriptor importing at the time of this writing.

As noted above, you should already be using this set of addresses
somewhere else, in a wallet that you can actually spend from. But,
that wallet may be offline; having a watch-only version will allow you
to monitor its funds without exposing the private keys to the dangers
of being on the net.

## Summary: Using Miniscript with Descriptors

Though you can use Bitcoin Scripts that you converted from Miniscript
(or Policy) in Bitcoin Core, currently there's only limited ability to
use Miniscript directly.

The one thing you can do is read in a Miniscript descriptor built with
public keys to create a read-only wallet. Doing so is a simple
combination of the process you've already learned to convert Policy
into Miniscript with the process you've already learned to read a
descriptor into Bitcoin Core.

## What's Next?

Continue "Using Miniscript" with [§16.4: Using BDK](16_4_Using_BDK.md).
