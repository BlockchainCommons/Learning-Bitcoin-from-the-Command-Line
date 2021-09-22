# 3.3: Setting Up Your Wallet

You're now ready to start working with Bitcoin. To begin with, you'll need to create a wallet for sending and receiving funds.

## Create a Wallet

> :warning: **VERSION WARNING:** Newer versions of Bitcoin Core, starting with v0.21.0, will no longer automatically create a default wallet on startup. So, you will need to manually create one. But if you're running an older version of Bitcoin Core, a new wallet has already been created for you, in which case you can skip ahead to [Create an Address](#create-an-address).

The first thing you need to do is create a new wallet, which can be done with the `bitcoin-cli createwallet` command. By creating a new wallet, you'll be creating your public-private key pair. Your public key is the source from which your addresses will be created, and your private key is what will allow you to spend any funds you receive into your addresses. Bitcoin Core will automatically save that information into a `wallet.dat` file in your `~/.bitcoin/testnet3/wallets` directory.

If you check your `wallets` directory, you'll see that it's currently empty.
```
$ ls ~/.bitcoin/testnet3/wallets
$
```

Although Bitcoin Core won't create a new wallet for you, it will still load a top-level unnamed ("") wallet on startup by default. You can take advantage of this by creating a new unnamed wallet.
```
$ bitcoin-cli createwallet ""
{
  "name": "",
  "warning": ""
}
```

Now, your `wallets` directory should be populated.
```
$ ls ~/.bitcoin/testnet3/wallets
database  db.log  wallet.dat
```

> :book: ***What is a Bitcoin wallet?*** A Bitcoin wallet is the digital equivalent of a physical wallet on the Bitcoin network. It stores information on the amount of bitcoins you have and where it's located (addresses), as well as the ways you can use to spend it. Spending physical money is intuitive, but to spend bitcoins users need to provide the correct _private key_. We will explain this in more detail throughout the course, but what you should know for now is that this public-private key dynamic is part of what makes Bitcoin secure and trustless. Your key pair information is saved in the `wallet.dat` file, in addition to data about preferences and transactions. For the most part, you won't have to worry about that private key: `bitcoind` will use it when it's needed. However, this makes the `wallet.dat` file extremely important: if you lose it, you lose your private keys, and if you lose your private keys, you lose your funds!

Sweet, now you have a Bitcoin wallet. But a wallet will be of little use for receiving bitcoins if you don't create an address first.

## Create an Address

The next thing you need to do is create an address for receiving payments. This is done with the `bitcoin-cli getnewaddress` command. Remember that if you want more information on this command, you should type `bitcoin-cli help getnewaddress`. Currently, there are three types of addresses: `legacy` and the two types of SegWit address, `p2sh-segwit` and `bech32`. If you do not otherwise specify, you'll get the default, which is currently `bech32`.

However, for the next few sections we're instead going to be using `legacy` addresses, both because `bitcoin-cli` had some teething problems with its early versions of SegWit addresses, and because other people might not be able to send to `bech32` addresses. This is all unlikely to be a problem for you now, but for the moment we want to get your started with transaction examples that are (mostly) guaranteed to work.

First, restart `bitcoind` so your new unnamed wallet is set as default and automatically loaded.
```
$ bitcoin-cli stop
Bitcoin Core stopping # wait a minute so it stops completely
$ bitcoind -daemon
Bitcoin Core starting # wait a minute so it starts completely
```

You can now create an address. You can require `legacy` address either with the second argument to `getnewaddress` or with the named `addresstype` argument.
```
$ bitcoin-cli getnewaddress -addresstype legacy
moKVV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B
```
Note that this address begins with an "m" (or sometimes an "n") to signify a testnet Legacy address. It would be a "2" for a P2SH address or a "tb1" for a Bech32 address.

> :link: **TESTNET vs MAINNET:** The equivalent mainnet address would start with a "1" (for Legacy), "3" (for P2SH), or "bc1" (for Bech32).

Take careful note of the address. You'll need to give it to whomever will be sending you funds.

> :book: ***What is a Bitcoin address?*** A Bitcoin address is literally where you receive money. It's like an email address, but for funds. Technically, it's a public key, though different address schemes adjust that in different ways. However unlike an email address, a Bitcoin address should be considered single use: use it to receive funds just _once_. When you want to receive funds from someone else or at some other time, generate a new address. This is suggested in large part to improve your privacy. The whole blockchain is immutable, which means that explorers can look at long chains of transactions over time, making it possible to statistically determine who you and your contacts are, no matter how careful you are. However, if you keep reusing the same address, then this becomes even easier. By creating your first Bitcoin address, you've also begun to fill in your Bitcoin wallet. More precisely, you've begun to fill the `wallet.dat` file in your `~/.bitcoin/testnet3 /wallets` directory.

With a single address in hand, you could jump straight to the next section and begin receiving funds. However, before we get there, we're going to briefly discuss the other sorts of addresses that you'll meet in the future and talk about a few other wallet commands that you might want to use in the future.

### Knowing Your Bitcoin Addresses

There are three types of Bitcoin addresses that you can create with the `getnewaddress` RPC command. You'll be using a `legacy` (P2PKH) address here, while you'll move over to a SegWit (P2SH-SegWit) or Bech32 address in [§4.6: Creating a Segwit Transaction](04_6_Creating_a_Segwit_Transaction.md).

As noted above, the foundation of a Bitcoin address is a public key: someone sends funds to your public key, and then you use your private key to redeem it. Easy? Except putting your public key out there isn't entirely secure. At the moment, if someone has your public key, then they can't retrieve your private key (and thus your funds); that's the basis of cryptography, which uses a trap-door function to ensure that you can only go from private to public key, and not vice-versa. But the problem is that we don't know what the future might bring. Except we do know that cryptography systems eventually get broken by the relentless advance of technology, so it's better not to put raw public keys on the 'net, to future-proof your transactions.

Classic Bitcoin transactions created P2PKH addresses that added an additional cryptographic step to protect public keys.

> :book: ***What is a Legacy (P2PKH) address?*** This is a Legacy address of the sort used by the early Bitcoin network. We'll be using it in examples for the next few sections. It's called a Pay to PubKey Hash (or P2PKH) address because the address is a 160-bit hash of a public key. Using a hash of your public key as your address creates a two-step process where to spend funds you need to reveal both the private key and the public key, and it increases future security accordingly. This sort of address remains important for receiving funds from people with out-of-date wallet software.

As described more fully in [§4.6: Creating a Segwit Transaction](04_6_Creating_a_Segwit_Transaction.md), the Block Size Wars of the late '10s resulted in a new sort of address: SegWit. This is the preferred sort of address currently, and should be fully integrated into Bitcoin-Core at this point, but nonetheless we're saving it for §4.6.

SegWit simply means "segregated witness" and it's a way of separating the transaction signatures out from the rest of the transaction to reduce transaction size. Some SegWit addresses will sneak into some of our examples prior to §4.6 as change addresses, which you'll see as addresses that begin with "tb". This is fine because the `bitcoin-cli` entirely supports their usage. But we won't use them otherwise.

There are two addresses of this sort:

> :book: ***What is a P2SH-SegWit (aka Nested SegWit) address?*** This is the first generation of SegWit. It wraps the SegWit address in a Script hash to ensure backward compatibility. The result creates transactions that are about 25%+ smaller (with corresponding reductions in transaction fees). 

> :book: ***What is a Bech32 (aka Native SegWit, aka P2WPKH) address?*** This is the second generation of SegWit. It's fully described in [BIP 173](https://en.bitcoin.it/wiki/BIP_0173). It creates transactions that are even smaller but more notably also has some advantages in creating addresses that are less prone to human error and have some implicit error-correction beyond that. It is *not* backward compatible like P2SH-SegWit was, and so some people may not be able to send to it.

There are other sorts of Bitcoin addresses, such as P2PK (which paid to a bare public key, and is deprecated because of its future insecurity) and P2SH (which pays to a Script Hash, and which is used by the first-generation Nested SegWit addresses; we'll meet it more fully in a few chapters).

## Optional: Sign a Message

Sometimes you'll need to prove that you control a Bitcoin address (or rather, that you control its private key). This is important because it lets people know that they're sending funds to the right person. This can be done by creating a signature with the `bitcoin-cli signmessage` command, in the form `bitcoin-cli signmessage [address] [message]`. For example:
```
$ bitcoin-cli signmessage "moKVV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B" "Hello, World"
HyIP0nzdcH12aNbQ2s2rUxLwzG832HxiO1vt8S/jw+W4Ia29lw6hyyaqYOsliYdxne70C6SZ5Utma6QY/trHZBI=
```
You'll get the signature as a return.

> :book: ***What is a signature?*** A digital signature is a combination of a message and a private key that can then be unlocked with a public key. Since there's a one-to-one correspendence between the elements of a keypair, unlocking with a public key proves that the signer controlled the corresponding private key.

Another person can then use the `bitcoin-cli verifymessage` command to verify the signature. He inputs the address in question, the signature, and the message:
```
$ bitcoin-cli verifymessage "moKVV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B" "HyIP0nzdcH12aNbQ2s2rUxLwzG832HxiO1vt8S/jw+W4Ia29lw6hyyaqYOsliYdxne70C6SZ5Utma6QY/trHZBI=" "Hello, World"
true
```
If they all match up, then the other person knows that he can safely transfer funds to the person who signed the message by sending to the address.

If some black hat was making up signatures, this would instead produce a negative result:
```
$ bitcoin-cli verifymessage "FAKEV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B" "HyIP0nzdcH12aNbQ2s2rUxLwzG832HxiO1vt8S/jw+W4Ia29lw6hyyaqYOsliYdxne70C6SZ5Utma6QY/trHZBI=" "Hello, World"
error code: -3
error message:
Invalid address
```

## Optional: Dump Your Wallet

It might seem dangerous having all of your irreplaceable private keys in a single file. That's what `bitcoin-cli dumpwallet` is for. It lets you make a copy of your wallet.dat:
```
$ bitcoin-cli dumpwallet ~/mywallet.txt
```
The `mywallet.txt` file in your home directory will have a long list of private keys, addresses, and other information. Mind you, you'd never want to put this data out in a plain text file on a Bitcoin setup with real funds!

You can then recover it with `bitcoin-cli importwallet`.
```
$ bitcoin-cli importwallet ~/mywallet.txt
```
But note this requires an unpruned node!
```
$ bitcoin-cli importwallet ~/mywallet.txt
error code: -4
error message:
Importing wallets is disabled when blocks are pruned
```

## Optional: View Your Private Keys

Sometimes, you might want to actually look at the private keys associated with your Bitcoin addresses. Perhaps you want to be able to sign a message or spend bitcoins from a different machine. Perhaps you just want to back up certain important private keys. You can also do this with your dump file, since it's human readable.
```
$ bitcoin-cli dumpwallet ~/mywallet.txt
{
  "filename": "/home/standup/mywallet.txt"
}
```
More likely, you just want to look at the private key associated with a specific address. This can be done with the `bitcoin-cli dumpprivkey` command.
```
$ bitcoin-cli dumpprivkey "moKVV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B"
cTv75T4B3NsG92tdSxSfzhuaGrzrmc1rJjLKscoQZXqNRs5tpYhH
```
You can then save that key somewhere safe, preferably somewhere not connected to the internet.

You can also import any private key, from a wallet dump or an individual key dump, as follows:
```
$ bitcoin-cli importprivkey cW4s4MdW7BkUmqiKgYzSJdmvnzq8QDrf6gszPMC7eLmfcdoRHtHh
```
Again, expect this to require an unpruned node. Expect this to take a while, as `bitcoind` needs to reread all past transactions, to see if there are any new ones that it should pay attention to.

> :information_source: **NOTE:** Many modern wallets prefer [mnemonic codes](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki) to generate the seeds necessary to create the private keys. This methodology is not used `bitcoin-cli`, so you won't be able to generate handy word lists to remember your private keys.

_You've been typing that Bitcoin address you generated a _lot_, while you were signing messages and now dumping keys. If you think it's a pain, we agree. It's also prone to errors, a topic that we'll address in the very next section._

## Summary: Setting Up Your Wallet

You need to create an address to receive funds. Your address is stored in a wallet, which you can back up. You can also do lots more with an address, like dumping its private key or using it to sign messages. But really, creating that address is _all_ you need to do in order to receive Bitcoin funds.

## What's Next?

Step back from "Understanding Your Bitcoin Setup" with [Interlude: Using Command-Line Variables](03_3__Interlude_Using_Command-Line_Variables.md).
