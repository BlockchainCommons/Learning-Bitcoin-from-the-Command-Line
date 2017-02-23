# 2A - Setting Up a Bitcoin-Core VPS by Hand

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

This document explains how to set up a VPS (Virtual Private Sever) to run a Bitcoin node on cloud computer by hand. You'll need to set up your computer yourself, but then this document will provide you with important commands to secure your VPS and to get it running Bitcoin.

> **WARNING:** Don’t use a VPS for a bitcoin wallet with significant real funds; see http://blog.thestateofme.com/2012/03/03/lessons-to-be-learned-from-the-linode-bitcoin-incident/ . It is  very nice to be able experiment with real bitcoin transactions on a live node without tying up a self-hosted server on a local network. I’ve also found it useful to be able to use an iPhone or iPad to communicate via SSH to my VPS to do some simple bitcoin tasks. But a higher level of safety is required for significant funds.

If you want to instead have a script do the setup for you, specifically at Linode.com, read the parallel HOWTO file, [2A - Setting up a Bitcoin-Core VPS with StackScript](./2B_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md).

If you already have a Bitcoin node running, instead read the next HOWTO file, [3 - Playing with Bitcoin Core](—link—).

## Choosing Your Bitcoin Setup

Before you start, you should choose between five possible Bitcoin setups.

You'll actually encode this choice at the end, when you start up Bitcoin, but in the meantime your choice will impact a few commands.

Your options are:

**Mainnet.** This will download the entirety of the Bitnet blockchain. That's 120G of data (and getting more every day).

**Pruned Mainnet.** This will cut the blockchain you're storing down to just the last 550 blocks. If you're not mining or running some other Bitcoin service, this should be plenty for validation.

**Testnet.** This gives you access to an alternative Bitcoin blockchain where the Bitcoins don't actually have value. It's intended for experimentation and testing.

**Pruned Testnet.** This is just the last 550 blocks of Testnet ... because the Testnet blockchain is pretty big now too.

**Private Regtest.** This is Regression Testing Mode, which lets you run a totally local Bitcoin server. It allows for even more in-depth testing. There's no pruning needed here, because you'll be starting from scratch.

## Getting Started at a Cloud Provider

First you'll need to choose a cloud provider. The commands in this document are derived from the script we use at linode.com, and they should also work at digitalocean.com.

### Set Up an Account

After you've selected your cloud provider, you'll need to setup an account, if you don't already have one. If you wish, the following referral codes can be used to earn about a month of free time:

```
Linode Referral Code: https://www.linode.com/?r=3c7fa15a78407c9a3d4aefb027539db2557b3765
Digital Ocean: http://www.digitalocean.com/?refcode=a6060686b88a
```

### Consider Two-Factor Authentication

If your cloud provider offers two-factor authentication for their web tools, we highly suggest using it, to ensure that your machine remains secure.

### Create a Machine

You should now create your Bitcoin VPS. If you want to install an unPruned Mainnet setup, you'll need more than 120G of disk space. If you instead plan to use a Pruned setup, a much smaller amount will do: this currently takes up about 15G of storage, so make sure you have 24G or 30G to be sure. You'll also need at least 2G of memory to run bitcoind.

We suggest Pruned setup unless you're planning to set up a miner or a complex Bitcoin service.

We also suggest you choose a Debian 8 image when you're creating your machine. These commands were all tested on Debian 8.7 (jessie). The further you get away from that, the less likely things will work as laid out here. So, another Debian is probably OK and maybe ubuntu, but we've tried these out on a Mac, and we know that it's missing some commands like "wget". So, installer beware!

Afterward, boot your VPS.

## Configuring Your VPS

You're now ready to log. You'll need to lookup the IP address of your new machine, and then you should be able to SSH in:

```
$ ssh root@192.168.1.52
```

You'll need to do some bog-standard configuration, then do some work to improve the security of your machine.

### Set Up Your Hostname

Choose a hostname for your machine (e.g. "mybtc") and enter it into your hostname file; afterward, run a few commands to propagate that new info:

```
$ echo "mybtc" > /etc/hostname
$ /etc/init.d/hostname.sh start
$ /bin/hostname $HOSTNAME
```

Also enter the info into your /etc/hosts file. Note that you should also enter a fully-qualified hostname into your hosts file, as shown below. If you're not making the machine part of a name, just choose a ".local" suffix.

```
$ echo "127.0.0.1    localhost" > /etc/hosts
$ echo "127.0.1.1 mybtc.local mybtc" >> /etc/hosts
```

### Update Your Timezone

Make sure your timezone is set correctly.

The following example sets your machine to the American west coast timezone:

```
$ echo "America/Los_Angeles" > /etc/timezone
$ cp /usr/share/zoneinfo/America/Los_Angeles /etc/localtime
```

## Protect Your VPS

Though you're not putting much real value on this server, you should still make sure it's as absolutely secure as possible.

### Create Firewall Rules

To start with, create a firewall rules file. For all instructions that look like this, you should just be able to cut from the "cat" all the way down to the EOF, and everything will be placed into the appropriate file.

```
$ cat > /etc/iptables.firewall.rules <<EOF
*filter

#  Allow all loopback (lo0) traffic and drop all traffic to 127/8 that doesn't use lo0
-A INPUT -i lo -j ACCEPT
-A INPUT -d 127.0.0.0/8 -j REJECT

#  Accept all established inbound connections
-A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT

#  Allow all outbound traffic - you can modify this to only allow certain traffic
-A OUTPUT -j ACCEPT

# If you want HTTP and HTTPS, uncomment these

#  Allow SSH connections
#
#  The -dport number should be the same port number you set in sshd_config
#
-A INPUT -p tcp -m state --state NEW --dport 22 -j ACCEPT

#  Allow ping
-A INPUT -p icmp -j ACCEPT

# Allow Bitcoin connections
-A INPUT -p tcp --dport 22 -m conntrack --ctstate NEW,ESTABLISHED -j ACCEPT
-A INPUT -p tcp --dport 8333 -j ACCEPT
-A INPUT -p tcp --dport 18333 -j ACCEPT
-A INPUT -m conntrack --ctstate ESTABLISHED -j ACCEPT

#  Log iptables denied calls
-A INPUT -m limit --limit 5/min -j LOG --log-prefix "iptables denied: " --log-level 7

#  Drop all other inbound - default deny unless explicitly allowed policy
-A INPUT -j DROP
-A FORWARD -j DROP

COMMIT
EOF
```

Note that 8333 is the Mainnet Port and 18333 is the Testnet port. If you want to be adventurous, you can delete the one you're not using.

Most work is still done on IPv4 networks, but the following will establish the same rules for IPv6:

```
$ cat /etc/iptables.firewall.rules | sed 's/127.0.0.0\/8/::1\/128/' > /etc/ip6tables.firewall.rules
```

Afterward, you need to create a file that will run both of these on startup:

```
$ cat > /etc/network/if-pre-up.d/firewall <<EOF
#!/bin/sh
/sbin/iptables-restore < /etc/iptables.firewall.rules
/sbin/ip6tables-restore < /etc/ip6tables.firewall.rules
EOF

$ chmod a+x /etc/network/if-pre-up.d/firewall
```

Finally, you should immediately run that:

```
$ /etc/network/if-pre-up.d/firewall
```

Please note that this will only allow access to your machine for only SSH, Ping, and Bitcoin services. If you want to accept other types of traffic, you will need to open up additional ports ("dports") using the same methodology as shown in the Bitcoin port 8333 connections. For example, to allow connections to port 25 (mail), would require adding the following to the iptables and the ip6tables:

```
-A INPUT -p tcp --dport 25 -j ACCEPT
-A INPUT -p udp --dport 25 -j ACCEPT
```

This example opens up access to port 25 for TCP and UDP connections. Other popular ports are 80 (HTTP), 443 (HTTPS), 53 (DNS), 110 (POP), and 143 (IMAP).  Be sure any such changes are put above the logs and defaults that appear at the end of the firewall files.

You can immediately incorporate your changes by running the following script:

```
$ /etc/network/if-pre-up.d/firewall
```

### Lock Down Your SSH

If you know your fixed IP address for home, we _highly_ suggest that you lock down your SSH, so that your server can only be logged into from that IP. You can also enter multiple IP addresses if you comma separate them.

To do so, just replace the following IP address with your own:

```
$ echo "sshd: 192.168.1.22" >> /etc/hosts.allow
$ echo "sshd: ALL" >> /etc/hosts.deny
```

## Setting Up a User

It's always best to do your work with a user other than root. The following creates a user account for 'user1'

```
/usr/sbin/useradd -m -g sudo -s /bin/bash user1
/usr/bin/passwd user1
/usr/sbin/adduser user1 sudo
```

You'll be asked for a password after the second command.

### Set Up a SSH Key

Though it's not required, we suggest copying your SSH key to your user1 account, to simplify access and make it more secure.

Make the SSH directory:

```
$ mkdir ~user1/.ssh
```

Then past your SSH key into ~user1/.ssh/authorized_keys

Afterward give user1 the permissions:

```
$ chown -R user1 ~user1/.ssh
```

 If you haven't setup an SSH key on your local computer yet, there are good instructions for it on [Github](https://help.github.com/articles/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent/).

### Create Bitcoin Aliases

We find a number of Bash aliases helpful to make it easier to use Bitcoin.

If you are using a Testnet setup, we suggest the following:

```
$ sudo -u user1 cat >> ~user1/.bash_profile <<EOF
alias btcdir="cd ~/.bitcoin/" #linux default bitcoind path
        # alias btcdir="cd ~/Library/Application\ Support/Bitcoin/" #mac default bitcoind path
alias bc="bitcoin-cli"
alias bd="bitcoind"
alias btcinfo='bitcoin-cli getinfo | egrep "\"version\"|balance|blocks|connections|errors"'
alias btcblock="echo \\\`bitcoin-cli getblockcount 2>&1\\\`/\\\`wget -O - http://blockexplorer.com/testnet/q/getblockcount 2> /dev/null | cut -d : -f2 | rev | cut -c 2- | rev\\\`"
EOF
```
Otherwise, we suggest the following:
```
$ cat >> ~user1/.bash_profile <<EOF
alias btcdir="cd ~/.bitcoin/" #linux default bitcoind path
        # alias btcdir="cd ~/Library/Application\ Support/Bitcoin/" #mac default bitcoind path
alias bc="bitcoin-cli"
alias bd="bitcoind"
alias btcinfo='bitcoin-cli getinfo | egrep "\"version\"|balance|blocks|connections|errors"'
alias btcblock="echo \\\`bitcoin-cli getblockcount 2>&1\\\`/\\\`wget -O - http://blockchain.info/q/getblockcount 2>/dev/null\\\`"
EOF
```
The only difference between the two is in the 'btcblock' line, which looks up the block count in different places for Mainnet and Testnet.

> **WARNING:** The btcblock alias will not work correctly if you try to place it in your .bash_profile by hand, rather than using the "cat" command as suggested. To enter it by hand, you need to adjust the number of backslashes (usually from three each to one each), so make sure you know what you're doing if you aren't entering the commands exactly as shown.

As usual, give your user permission:

```
$ /bin/chown user1 ~user1/.bash_profile
```

## Updating Debian

An up-to-date Debian is a safe Debian.

Before you install Bitcoin, you should run the following commands to get all the latest patches:

```
$ export DEBIAN_FRONTEND=noninteractive
$ apt-get update
$ apt-get upgrade -y
$ apt-get dist-upgrade -y
```

This process will take several minutes. Take a break. Have an espresso.

Afterward, you also need to install a random number generator:

```
$ apt-get install haveged -y
```

Finally, we highly suggest that you set upgrades to be automatic, so that you stay up to date on updates in the future:

```
$ echo "unattended-upgrades unattended-upgrades/enable_auto_updates boolean true" | debconf-set-selections
$ apt-get -y install unattended-upgrades
```

## Installing Bitcoin

At last, the moment of truth, you're ready to install Bitcoin!

You will want to do this all in the user1 account, so switch over:

```
$ su user1
```

### Setup Variables

First, we suggest setting up two variables to make this installation more automatic.

The first variable, $BITCOIN, should be set to the current version of Bitcoin. It was 0.13.2 when we wrote this. The second will then automatically generate a truncated form used by some of the files.

```
$ export BITCOIN=bitcoin-core-0.13.2
$ export BITCOINPLAIN=`echo $BITCOIN | sed 's/bitcoin-core/bitcoin/'`
```

### Download Files

Grab the relevant files from bitcoin.org:

```
$ wget https://bitcoin.org/bin/$BITCOIN/$BITCOINPLAIN-x86_64-linux-gnu.tar.gz -O ~user1/$BITCOINPLAIN-x86_64-linux-gnu.tar.gz
$ wget https://bitcoin.org/bin/$BITCOIN/SHA256SUMS.asc -O ~user1/SHA256SUMS.asc
$ wget https://bitcoin.org/laanwj-releases.asc -O ~user1/laanwj-releases.asc
```

This is the other step of the process that takes five minutes or so. It might be time to go have a nice walk in the sun.

### Verify Bitcoin Signature

You want to make extra sure that your Bitcoin setup is authenticate and hasn't been messed with. The first way to do that is to look at the signature:

```
$ /usr/bin/gpg --import ~user1/laanwj-releases.asc
$ /usr/bin/gpg --lsign `sudo -u user1 /usr/bin/gpg --list-keys | grep pub | awk '{print $2}' | awk -F/ '{print $2}'`
$ /usr/bin/gpg --verify ~user1/SHA256SUMS.asc
```

Amongst the info you get back from the last command should be a line telling you that you have a "Good signature".

### Verify Bitcoin SHA

Next, you should verify the Hash for the Bitcoin tar file against the expected Hash:

```
$ /usr/bin/sha256sum ~user1/$BITCOINPLAIN-x86_64-linux-gnu.tar.gz | awk '{print $1}'
$ cat ~user1/SHA256SUMS.asc | grep $BITCOINPLAIN-x86_64-linux-gnu.tar.gz | awk '{print $1}'
```

If those both produce the same number, it's OK.

### Install Bitcoin

If both of your verification tests succeeded, you can now install Bitcoin. (If they didn't, you need to start looking into what's going on!)

```
$ /bin/tar xzf ~user1/$BITCOINPLAIN-x86_64-linux-gnu.tar.gz -C ~user1
$ /usr/bin/install -m 0755 -o root -g root -t /usr/local/bin ~user1/$BITCOINPLAIN/bin/*
$ /bin/rm -rf ~user1/$BITCOINPLAIN/
```

### Create Bitcoin Configuration

Finally, you should set up a bitcoin configuration file.

First, create the directory:

```
$ sudo -u user1 /bin/mkdir ~user1/.bitcoin
```

This is the core file, which is appropriate for a Mainnet or Testnet setup:
```
$ cat >> ~user1/.bitcoin/bitcoin.conf << EOF
server=1
dbcache=1536
par=1
blocksonly=1
maxuploadtarget=137
maxconnections=16
rpcuser=bitcoinrpc
rpcpassword=$(xxd -l 16 -p /dev/urandom)
EOF
```

If you want a pruned copy of the chain (and you probably should), add the following:

```
$ cat >> ~user1/.bitcoin/bitcoin.conf << EOF
prune=550
EOF
```

Otherwise, if you are _not_ pruning add the following:

```
$ cat >> ~user1/.bitcoin/bitcoin.conf << EOF
txindex=1
EOF
```

(txindex gives the benefit of a complete transaction index, but is not compatible with pruning, so you choose one or the other.)

Finally, if you want to use Testnet instead of Mainnet, add the following:

```
$ cat >> ~user1/.bitcoin/bitcoin.conf << EOF
testnet=1
EOF
```

So, for example, a pruned testnet, which is our favored setup for playing with bitcoin, would look like this:
```
server=1
dbcache=1536
par=1
blocksonly=1
maxuploadtarget=137
maxconnections=16
rpcuser=bitcoinrpc
rpcpassword=$(xxd -l 16 -p /dev/urandom)
prune=550
testnet=1
```

_Please note that this setup does not yet support a Private Regtest. That will require a very different setup TBD._

Finally, limit permissions to your configuration file:

```
$ /bin/chmod 600 ~user1/.bitcoin/bitcoin.conf
```

### Start the Daemon!

After all of that, starting the Bitcoin daemon is anticlimatically simple:

```
$ /usr/local/bin/bitcoind -daemon
```

You should also add a crontab entry , so that the bitcoin daemon starts up whenever your VPS restarts:

```
$ ( /usr/bin/crontab -l -u user1 2>/dev/null; echo "@reboot /usr/local/bin/bitcoind -daemon" ) | /usr/bin/crontab -u user1 -
```

## Playing with Bitcoin

So now you probably want to play with Bitcoin!

But wait, your Bitcoin daemon is probably still downloading blocks. This alias, from your .bash configuration will tell you how things are going:

```
$ btcblock
```

If you choose of the pruned mainnet, it will probably take a little over a day to download everything. 

So, it might be time for a few more espressos.

But, when you're ready to go, continue on with [Playing with Bitcoin](-link-), where we'll talk about the files and how you can start experimenting.### Useful commands

```
bc help
bc getblockchaininfo
bc getnetworkinfo
bc getnettotals
bc getwalletinfo
bc stop
```


### Some tutorials once you've got bitcoin installed and up-to-date

- Bitcoin.org's developer examples https://bitcoin.org/en/developer-examples#transactions
- Jonas Nick's "How to Run a Bitcoin Node" https://github.com/jonasnick/bitcoin-node
- David DeRosa's "A Developer Oriented Series about Bitcoin" http://davidederosa.com/basic-blockchain-programming/

Videos

- Bitcoin JSON-RPC Tutorial 1 https://www.youtube.com/watch?v=ygNit44dQHA
- Bitcoin JSON-RPC Tutorial 2 - VPS Setup https://www.youtube.com/watch?v=ygNit44dQHA
- Bitcoin JSON-RPC Tutorial 3 - bitcoin.conf https://www.youtube.com/watch?v=W54aRszkEOI&t=65s
- Bitcoin JSON-RPC Tutorial 4 - Command Line Interface https://www.youtube.com/watch?v=vmdYD7vutTI
- Bitcoin JSON-RPC Tutorial 5 - Your First Calls https://www.youtube.com/watch?v=ARL_PvDEBtU
