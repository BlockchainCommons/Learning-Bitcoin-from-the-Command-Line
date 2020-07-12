
# 13.1: Verifying Your Lightning Setup

> :information_source: **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Before you start playing with c-lightning, you should ensure that everything is setup correctly.

## Create Your Aliases

We suggest creating some aliases to make it easier to use c-lightning.

You can do so by putting them in your `.bash_profile`.
```
cat >> ~/.bash_profile <<EOF
alias lndir="cd ~/.lightningn/" #linux default c-lightning path
alias lnc="lightning-cli"
alias lnd="lightningd"
alias lninfo='bitcoin-cli getwalletinfo | egrep "\"balance\""; bitcoin-cli getnetworkinfo | egrep "\"version\"|connections"; bitcoin-cli getmininginfo | egrep "\"blocks\"|errors"'
EOF
```
After you enter these aliases you can either `source .bash_profile` to input them or just log out and back in.

Note that these aliases includes shortcuts for running `lightning-cli`, for running `lightningd`, and for going to the c-lightning directory. These aliases are mainly meant to make your life easier. We suggest you create other aliases to ease your use of frequent commands (and arguments) and to minimize errors. Aliases of this sort can be even more useful if you have a complex setup where you regularly run commands associated with Mainnet, with Testnet, _and_ with Regtest, as explained further below.

With that said, use of these aliases in _this_ document might accidentally obscure the core lessons being taught about c-lightning, so the only alias directly used here is `lninfo` because it encapsulatea  much longer and more complex command. Otherwise, we show the full commands; adjust for your own use as appropriate.

## Run lightningd

You'll begin your exploration of the Lightning network with the `lightning-cli` command. However, lightningd _must_ be running to use lightning-cli, as lightning-cli sends JSON-RPC commands to the lightningd. If you used our standard setup, lightningd should already be up and running. You can double check by looking at the process table.
```
$ ps auxww | grep lightningd
standup   7817  1.1  0.2  95280  8964 pts/18   D+   14:13   0:00 lightningd --network=testnet
```
If it's not running, you'll want to run `lightningd --network=testnet` by hand.

## Verify your node

You should have the whole blockchain downloaded before you start playing. Just run the `bitcoin-cli getblockcount` alias to see if it's all loaded. 
```
$ lightning-cli --network=testnet getinfo
{
   "id": "03fce2a20393a65b9d6cab5425f4cd33ddc621ade458efd69d652917e2b5eaf59c",
   "alias": "VIOLENTIRON",
   "color": "03fce2",
   "num_peers": 0,
   "num_pending_channels": 0,
   "num_active_channels": 0,
   "num_inactive_channels": 0,
   "address": [],
   "binding": [
      {
         "type": "ipv6",
         "address": "::",
         "port": 9735
      },
      {
         "type": "ipv4",
         "address": "0.0.0.0",
         "port": 9735
      }
   ],
   "version": "v0.8.2-398-g869fa08",
   "blockheight": 54959,
   "network": "testnet",
   "msatoshi_fees_collected": 0,
   "fees_collected_msat": "0msat",
   "lightning-dir": "/home/user/.lightning/testnet"
}
```

## Summary: Verifying your Lightning setup

Before you start playing with lightning, you should make sure that your aliases are set up, your lightinnd is running, and your node is synced. You may also want to set up some access to alternative lightning setups, if you're an advanced user.

## What's Next?

Continue "Understanding Your Lightning Setup" with [§13.2: Knowing Your Lightning Setup](13_2_Knowing_Your_lightning_Setup.md).
