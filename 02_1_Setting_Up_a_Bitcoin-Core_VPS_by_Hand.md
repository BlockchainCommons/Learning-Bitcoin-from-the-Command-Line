# 2.1: Setting Up a Bitcoin-Core VPS by Hand

> :information_source: **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

This document explains how to set up a VPS (Virtual Private Server) by hand to run a Bitcoin node on a cloud computer. You'll need to set up your computer yourself, but then this document will provide you with important commands to secure your VPS and to get it running Bitcoin. The setup should all be done in my session, so you don't lose the variables `export`ed at the start.

> :warning: **WARNING:** Don’t use a VPS for a bitcoin wallet with significant real funds; see http://blog.thestateofme.com/2012/03/03/lessons-to-be-learned-from-the-linode-bitcoin-incident/ . It is  very nice to be able experiment with real bitcoin transactions on a live node without tying up a self-hosted server on a local network. I’ve also found it useful to be able to use an iPhone or iPad to communicate via SSH to my VPS to do some simple bitcoin tasks. But a higher level of safety is required for significant funds.

If you want to instead have a script do the setup for you, specifically at linode.com, read the parallel HOWTO file, [§2.2: Setting up a Bitcoin-Core VPS with StackScript](02_2_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md).

If you already have a Bitcoin node running, instead read the next HOWTO file, [Chapter Three: Understanding Your Bitcoin Setup](03_0_Understanding_Your_Bitcoin_Setup.md).

## Choosing Your Bitcoin Setup

Before you start, you should choose between five possible Bitcoin setups, as discussed in Appendix I. We're going to make it simple: _Choose the Unpruned Testnet_ setup. This is the perfect setup for playing with Bitcoin, and we're going to assume it's the default from here on out, though we'll have some sidebars where we explain other options.

## Getting Started at a Cloud Provider

First you'll need to choose a cloud provider. The commands in this document are derived from the script we use at linode.com; they should also work at digitalocean.com.

### Set Up an Account

After you've selected your cloud provider, you'll need to setup an account, if you don't already have one. If you wish, you can take a look if some discount is available:

```
Linode: https://www.linode.com/lp/price-performance/ (50$ free for 60 days as of now)
Digital Ocean: https://m.do.co/c/de06f00a961e (100$ free for 60 days)
```

### Optional: Consider Two-Factor Authentication

If your cloud provider offers two-factor authentication for their web tools, we highly suggest using it, to ensure that your machine remains secure.

### Create a Machine

You should now create your Bitcoin VPS. For an Unpruned Testnet setup, you should have at least 2-3G of memory and at least 19G of storage.

> :warning: **WARNING:** We've occasionally had machines run out of memory after running `bitcoind` for a few days when they only had 2G. Coming back, we find that `bitcoind` has stopped, leaving the message "Error: Out of memory. Terminating." in the `debug.log` file. This simply requires a restart of `bitcoind` and ten or fifteen minutes to get the blockchain resynced. Be generous with your memory if you want to avoid this annoyance, but don't worry too much if you hit it.

We also suggest that you choose a Debian 9 image when you're creating your machine. These commands were all tested on Debian 9.5 (stretch). The further you get away from that, the less likely things will work as described here. So, another Debian is probably OK and maybe a different Ubuntu, but we've tried these out on a Mac, and we know that it's missing some commands like "wget". So, installer beware!

Afterward, boot your VPS.

> :link: **TESTNET vs MAINNET:** The various possible setups require 5-19G of storage and 2-3G of memory. The non-Pruned Mainnet is the only setup that requires considerably more: about 170G of storage to hold the current blockchain.
>
> Following are suggestions for machine requirements:
>
> | Setup | Memory | Storage |
> |-------|--------|---------|
> | Mainnet | 2-3G | 170G |
> | Pruned Mainnet | 2-3G | ~5G |
> | Testnet | 2-3G | ~19G |
> | Pruned Testnet | 2-3G | ~5G |
> | Regtest | 2-3G | ~ |

## Configuring Your VPS

You'll need to look up the IP address of your new machine, and then you should be able to SSH in:

```
$ ssh root@192.168.1.52
```

Now, you'll need to do some bog-standard configuration, then some work to improve the security of your machine.

_If you already have your own techniques for setting up a machine, go ahead and follow them, then jump ahead to "Setting Up a User", then "Installing Bitcoin". Otherwise,continue on!_


### Prettify your terminal
This is optional but if you want you can customize your terminal so that is more readable and usable.

Just open your _~/.bash_profile_, or create it if the file is not existing yet and insert the following commands:
```
export PS1="\[\033[36m\]\u\[\033[m\]@\[\033[32m\]\h:\[\033[33;1m\]\w\[\033[m\]\$ "
export CLICOLOR=1
export LSCOLORS=ExFxBxDxCxegedabagacad
alias ls='ls -GFh'
```

Then to apply the changes, run this command from the terminal:
```
$ source ~/.bash_profile
```

Feel free to play with the settings until you are satisfied!


### Set Up Your Hostname

Choose a hostname for your machine (e.g. "mybtc") and enter it into your hostname file; afterward, run a few commands to propagate that new info:

```
$ echo "mybtc" > /etc/hostname
$ /etc/init.d/hostname.sh start
$ /bin/hostname "mybtc"
```

Otherwise, if you have _systemd_ installed, you can use just the following command:
```
$ hostnamectl set-hostname mybtc
```
This setting will be applied once you restart your machine.

Enter the IP and hostname info into your /etc/hosts file. Note that you should also enter a fully-qualified hostname, as shown below. If you're not making the machine part of a domain, just choose a ".local" suffix.

```
$ echo "127.0.0.1    localhost" > /etc/hosts
$ echo "127.0.1.1 mybtc.local mybtc" >> /etc/hosts
```

### Update Your Timezone

Make sure your timezone is correct.

First, take a look at the regions available in the following directory:
```
$ /usr/share/zoneinfo
```

The following example sets your machine to the American west coast timezone:

```
$ sudo rm /etc/localtime
$ sudo ln -s /usr/share/zoneinfo/America/Los_Angeles /etc/localtime
```

Just run
```
$ date
```
in your terminal to check that everything is correct.

## Protecting Your VPS

Though you're not putting much real value on this server, you should still make sure it's as absolutely secure as possible.

### Create Firewall Rules

To start with, create a firewall rules file.

_For all instructions that look like this, you should just be able to cut from the "cat" all the way down to the EOF, and everything will be placed into the appropriate file._
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
-A INPUT -p tcp --dport 9735 -j ACCEPT
-A INPUT -m conntrack --ctstate ESTABLISHED -j ACCEPT

#  Log iptables denied calls
-A INPUT -m limit --limit 5/min -j LOG --log-prefix "iptables denied: " --log-level 7

#  Drop all other inbound - default deny unless explicitly allowed policy
-A INPUT -j DROP
-A FORWARD -j DROP

COMMIT
EOF
```

> :link: **TESTNET vs MAINNET:** Note that 8333 is the Bitcoin Mainnet Port, 18333 is the Bitcoin Testnet port, and 9735 is the Lightning port. If you want to be adventurous, you can delete the one you're not using.

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

At this point, if you run on your terminal
```
$ iptables -L
```

you should see the following output:

```
Chain INPUT (policy ACCEPT)
target     prot opt source               destination         
ACCEPT     all  --  anywhere             anywhere            
REJECT     all  --  anywhere             loopback/8           reject-with icmp-port-unreachable
ACCEPT     all  --  anywhere             anywhere             state RELATED,ESTABLISHED
ACCEPT     tcp  --  anywhere             anywhere             state NEW tcp dpt:ssh
ACCEPT     icmp --  anywhere             anywhere            
ACCEPT     tcp  --  anywhere             anywhere             tcp dpt:ssh ctstate NEW,ESTABLISHED
ACCEPT     tcp  --  anywhere             anywhere             tcp dpt:8333
ACCEPT     tcp  --  anywhere             anywhere             tcp dpt:18333
ACCEPT     tcp  --  anywhere             anywhere             tcp dpt:9735
ACCEPT     all  --  anywhere             anywhere             ctstate ESTABLISHED
LOG        all  --  anywhere             anywhere             limit: avg 5/min burst 5 LOG level debug prefix "iptables denied: "
DROP       all  --  anywhere             anywhere            

Chain FORWARD (policy ACCEPT)
target     prot opt source               destination         
DROP       all  --  anywhere             anywhere            

Chain OUTPUT (policy ACCEPT)
target     prot opt source               destination         
ACCEPT     all  --  anywhere             anywhere  
```

#### Optional: Add More Firewall Rules

Please note that this will only allow access to your machine for SSH, Ping, and Bitcoin services. If you want to accept other types of traffic, you will need to open up additional ports ("dports") using the same methodology as shown in the Bitcoin port 8333 connection. For example, to allow connections to port 25 (mail), would require adding the following to the iptables and the ip6tables:

```
-A INPUT -p tcp --dport 25 -j ACCEPT
-A INPUT -p udp --dport 25 -j ACCEPT
```

This example opens up access to port 25 for TCP and UDP connections. Other popular ports are 80 (HTTP), 443 (HTTPS), 53 (DNS), 110 (POP), and 143 (IMAP).  Be sure any such changes are put above the logs and defaults that appear at the end of the iptables.firewall.rules files.

If you made changes, you can immediately incorporate them by again running the firewall script that you created.

```
$ /etc/network/if-pre-up.d/firewall
```

### Lock Down Your SSH

If you know your fixed IP address for home, we _highly_ suggest that you lock down your SSH, so that your server can only be accessed from that IP. You can also enter multiple IP addresses if you comma separate them.

To do so, just replace $YOUR_HOME_IP with your own:
```
$ echo "sshd: $YOUR_HOME_IP" >> /etc/hosts.allow
$ echo "sshd: ALL" >> /etc/hosts.deny
```

### Update Your Debian

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
## Setting Up a User

It's always best to do your work with a user other than root. The following creates a user account for 'user1'
```
$ /usr/sbin/useradd -m -g sudo -s /bin/bash user1
$ /usr/bin/passwd user1
$ /usr/sbin/adduser user1 sudo
```
You'll be asked for a password for user1 after the second command.

### Optional: Set Up a SSH Key

Though it's not required, we suggest copying your SSH key to your user1 account, to simplify access and to make it more secure.

Make the SSH directory:
```
$ mkdir ~user1/.ssh
```

Then paste your SSH key into ~user1/.ssh/authorized_keys .

Afterward give user1 access to the directory:
```
$ chown -R user1 ~user1/.ssh
```
 If you haven't set up an SSH key on your local computer yet, there are good instructions for it on [Github](https://help.github.com/articles/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent/).

## Installing Bitcoin

You're now ready to get to the bitcoin-specific part of this tutorial!

### Create Bitcoin Aliases

We find a number of Bash aliases helpful to make it easier to use Bitcoin.
```
$ sudo -u user1 cat >> ~user1/.bash_profile <<EOF
alias btcdir="cd ~/.bitcoin/" #linux default bitcoind path
alias bc="bitcoin-cli"
alias bd="bitcoind"
alias btcinfo='bitcoin-cli getwalletinfo | egrep "\"balance\""; bitcoin-cli getnetworkinginfo | egrep "\"version\"|connections"; bitcoin-cli getmininginfo | egrep "\"blocks\"|errors"'
alias btcblock='bitcoin-cli getblockcount'
alias latesttestnetblock='wget -O - https://testnet.blockexplorer.com/api/status?q=getBlockCount 2> /dev/null | cut -d : -f5 | cut -f1 -d","'
EOF
```

> :link: **TESTNET vs MAINNET:** If you are using a mainnet or pruned mainnet setup, we instead suggest the following. The only difference between the two is in the 'btcblock' line, which looks up the block count in different places for mainnet and testnet.
```
$ sudo -u user1 cat >> ~user1/.bash_profile <<EOF
alias btcdir="cd ~/.bitcoin/" #linux default bitcoind path
alias bc="bitcoin-cli"
alias bd="bitcoind"
alias btcinfo='bitcoin-cli getwalletinfo | egrep "\"balance\""; bitcoin-cli getnetworkinginfo | egrep "\"version\"|connections"; bitcoin-cli getmininginfo | egrep "\"blocks\"|errors"'
alias btcblock='bitcoin-cli getblockcount'
alias latesttestnetblock='wget -O - https://blockexplorer.com/api/status?q=getBlockCount 2> /dev/null | cut -d : -f5 | cut -f1 -d","'
EOF
```

As usual, give your user permission:
```
$ /bin/chown user1 ~user1/.bash_profile
```
And put a copy in `.bashrc` for use with interactive non-login shells:
```
$ cp ~user1/.bash_profile ~user1/.bashrc
$ /bin/chown user1 ~user1/.bashrc
```

### Login as Your Unprivileged User

You now want to switch over to the user1 account for the actual install:
```
$ su user1
$ cd
$ source ~/.bash_profile
```

### Setup Variables

We suggest setting up two variables to make this installation more automatic.

The first variable, $BITCOIN, should be set to the current version of Bitcoin. It was 0.19.0.1 when we wrote this. The second will then automatically generate a truncated form used by some of the files.
Insert this in the _~/.bash_profile_ file to make it persistent.
```
export BITCOIN=bitcoin-core-0.19.0.1
export BITCOINPLAIN=`echo $BITCOIN | sed 's/bitcoin-core/bitcoin/'`
```
Then reload your configuration by running:
```
$ source ~/.bash_profile
```

### Download Files

Grab the relevant files from bitcoin.org:
```
$ wget https://bitcoin.org/bin/$BITCOIN/$BITCOINPLAIN-x86_64-linux-gnu.tar.gz -O ~user1/$BITCOINPLAIN-x86_64-linux-gnu.tar.gz
$ wget https://bitcoin.org/bin/$BITCOIN/SHA256SUMS.asc -O ~user1/SHA256SUMS.asc
$ wget https://bitcoin.org/laanwj-releases.asc -O ~user1/laanwj-releases.asc
```
This is the other step of the setup process that takes five minutes or so. It might be time to go have a nice walk in the sun.

### Verify Bitcoin Signature

You want to make extra sure that your Bitcoin setup is authentic and hasn't been messed with. The first way to do that is to look at the signature:
```
$ /usr/bin/gpg --import ~user1/laanwj-releases.asc
$ /usr/bin/gpg --verify ~user1/SHA256SUMS.asc
```
Amongst the info you get back from the last command should be a line telling you that you have a "Good signature". (Don't worry about the warning.)

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
$ /bin/tar xzf ~user1/bitcoin-0.19.0.1-x86_64-linux-gnu.tar.gz -C ~user1
$ sudo /usr/bin/install -m 0755 -o root -g root -t /usr/local/bin ~user1/$BITCOINPLAIN/bin/*
$ /bin/rm -rf ~user1/$BITCOINPLAIN/
```

### Create Bitcoin Configuration

Finally, you should set up a bitcoin configuration file.

First, create the directory:
```
$ /bin/mkdir ~user1/.bitcoin
```
This is the core bitcoin.conf file, which is appropriate for an unpruned testnet setup:
```
$ cat >> ~user1/.bitcoin/bitcoin.conf << EOF
# Accept command line and JSON-RPC commands
server=1

# Set database cache size in megabytes (4 to 16384, default: 450)
dbcache=1536

# Set the number of script verification threads (-6 to 16, 0 = auto, <0 = leave that many cores free, default: 0)
par=1

# Set to blocksonly mode, sends and receives no lose transactions, instead handles only complete blocks
blocksonly=1

# Tries to keep outbound traffic under the given target (in MiB per 24h), 0 = no limit (default: 0)
maxuploadtarget=137

# Maintain at most <n> connections to peers (default: 125)
maxconnections=16

# Username for JSON-RPC connections
rpcuser=bitcoinrpc

# Password for JSON-RPC connections
rpcpassword=$(xxd -l 16 -p /dev/urandom)

# Allow JSON-RPC connections from, by default only localhost are allowed
rpcallowip=127.0.0.1

# Use the test chain
testnet=1

# Maintain a full transaction index, used by the getrawtransaction rpc call (default: 0)
txindex=1

# Make the wallet broadcast transactions (default: 1)
walletbroadcast=1
EOF
```
> :link: **TESTNET vs MAINNET:** If you want to use mainnet instead of testnet, just omit the "testnet=1" line; easy!

> :link: **PRUNED vs UNPRUNED:** If you want to use a pruned copy of the blockchain instead of an unpruned copy, to minimize storage requirements and loading time, do _not_ include the "txindex=1" line, but instead add a "prune=550" line. txindex gives the benefit of a complete transaction index, but is not compatible with pruning, so you choose one or the other

To end, limit permissions to your configuration file:
```
$ /bin/chmod 600 ~user1/.bitcoin/bitcoin.conf
```

### Start the Daemon!

After all of that, starting the Bitcoin daemon is anticlimatically simple:
```
$ /usr/local/bin/bitcoind -daemon
```
If everything is fine you should see the following message:
```
Bitcoin Core starting
```

You should also add a crontab entry, so that the bitcoin daemon starts up whenever your VPS restarts:
```
$ ( /usr/bin/crontab -l -u user1 2>/dev/null; echo "@reboot /usr/local/bin/bitcoind -daemon" ) | /usr/bin/crontab -u user1 -
```

## Playing with Bitcoin

So now you probably want to play with Bitcoin!

But wait, your Bitcoin daemon is probably still downloading blocks. This alias, from your .bash configuration will tell you which block you are downloading:
```
$ btcblock
```

You can compare that with the value of the actual latest testnet block by launching this previously set alias:
```
$ latesttestnetblock
```
When these values are equal then the downloading is finished.

0.19.0.1 is quite fast to download blocks, but it might still take an hour to download the unpruned testnet. It might be time for a few more espressos.

> :link: **TESTNET vs MAINNET:** An unpruned mainnet will take hours longer.

But, when you're ready to go, continue on with [Chapter Three: Understanding Your Bitcoin Setup](03_0_Understanding_Your_Bitcoin_Setup.md), where we'll talk about the files and how you can start experimenting.

## Summary: Setting Up a Bitcoin-Core VPS by Hand

By setting up a Bitcoin-Core VPS by hand you've simultaneously created your playform for learning Bitcoin while also ensuring that you know how it all works.

## What's Next?

See the other method for setting up a VPS with [§2.2: Setting up a Bitcoin-Core VPS with StackScript](02_2_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md). Or, move on to "bitcoin-cli" with [Chapter Three: Understanding Your Bitcoin Setup](03_0_Understanding_Your_Bitcoin_Setup.md).

## Appendix: Bitcoin Installation Types

**Mainnet.** This will download the entirety of the Bitnet blockchain. That's 120G of data (and getting more every day).

**Pruned Mainnet.** This will cut the blockchain you're storing down to just the last 550 blocks. If you're not mining or running some other Bitcoin service, this should be plenty for validation.

**Testnet.** This gives you access to an alternative Bitcoin blockchain where the bitcoins don't actually have value. It's intended for experimentation and testing.

**Pruned Testnet.** This is just the last 550 blocks of Testnet ... because the Testnet blockchain is pretty big now too.

**Private Regtest.** This is Regression Testing Mode, which lets you run a totally local Bitcoin server. It allows for even more in-depth testing. There's no pruning needed here, because you'll be starting from scratch. This is a very different setup, and so is covered near the end of this tutorial.
