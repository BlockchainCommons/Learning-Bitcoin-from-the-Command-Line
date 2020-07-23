# Chapter 14.2: Changing Your Bitcoin Hidden Services

> :information_source:  **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

You've got a working Tor service, but over time you may wish to reset or otherwise adjust it.

## Reset Your `bitcoind` Onion Address

If you ever want to reset your onion address for `bitcoind`, just remove the `onion_private_key` in your data directory, such as `~/.bitcoin/testnet`:
```
$ cd ~/.bitcoin/testnet
$ rm onion_private_key 
```
When you restart, a new onion address will be generated:
```
2020-07-22T23:52:27Z tor: Got service ID pyrtqyiqbwb3rhe7, advertising service pyrtqyiqbwb3rhe7.onion:18333
2020-07-22T23:52:27Z tor: Cached service private key to /home/standup/.bitcoin/testnet3/onion_private_key
```

## Reset Your RPC Onion Address

If you want to reset your onion address for RPC access, you similarly delete the appropriate `HiddenServiceDirectory` and restart Tor:
```
$ sudo rm -rf /var/lib/tor/standup/
$ sudo /etc/init.d/tor restart
```

> :warning: **WARNING:** Reseting your RPC onion address will disconnect any mobile wallets or other services that you've connected using the Quicklink API. Do this with extreme caution.

## Forcing `bitcoind` to Use Tor

Finally, you can force `bitcoind` to use onion by adding the following to your `bitcoin.conf`:
```
proxy=127.0.0.1:9050
listen=1
bind=127.0.0.1
onlynet=onion
```
You will then need to add onion-based seed nodes or other nodes to your setup, once more by editing the `bitcoin.conf`:
```
seednode=address.onion
seednode=address.onion
seednode=address.onion
seednode=address.onion
addnode=address.onion
addnode=address.onion
addnode=address.onion
addnode=address.onion
```
Afterward, restart `tor` and `bitcoind`.

You should now be communicating exlusively on Tor. But, unless you are in a hostile state, this level of anonymity is probably not required. It also is not particularly recommended: you might greatly decrease your number of potential peers, inviting problems of censorship or even correlation. You may also see lag. And, this setup may give you a false sense of anonymity that really doesn't exist on the Bitcoin network.

> :warning: **WARNING:** This setup is untested! Use at your own risk!

## Summary: Changing Your Bitcoin Hidden Services

You probably won't need to fool with your Onion services once you've verified them, but in case you do, here's how to reset a Tor address that has become compromised or to move over to exclusive-Tor use for your `bitcoind`.

## What's Next?

Continue "Understanding Tor" with [14.3: Adding SSH Hidden Services](14_3_Adding_SSH_Hidden_Services.md).
