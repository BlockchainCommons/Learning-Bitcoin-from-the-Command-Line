# Learning Bitcoin from the Command-Line #

> NOTE: This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

The best way to learn to learn deeply about bitcoin is to avoid GUIs (even bitcoin-qt), and instead learn it from the command line. This tutorial assumes that you have some minimal background of how to use a command line. If not, there are many tutorials available, and I have one for Mac users at https://github.com/ChristopherA/intro-mac-command-line

## Installing a PRUNED Bitcoin Node on a VPS ##

A pruned bitcoin node is a full node (in particular, a pruned node is NOT an SPV node), but it is smaller as doesn’t have all the history.

In addition, the bitcoin.conf settings suggested here will minimalize the initial and ongoing bandwidth requirements, allow a $5 a month VPS to be great for learning and testing with bitcoin.

This info works with both Linode and Digital Ocean, so should work with more VPSs and local virtual machines.

> NOTE: Don’t use a VPS for a bitcoin wallet with significant real funds— see http://blog.thestateofme.com/2012/03/03/lessons-to-be-learned-from-the-linode-bitcoin-incident/ — it is just very nice to be able experiment with real bitcoin transactions on a live full node without tying up a self-hosted server on a home network. I’ve also found it useful to be able to use an iPhone or iPad to communicate via SSH to my VPS to do some simple bitcoin tasks.
>

If you don’t have a Linode or DG account, signup using these codes will give you roughly a month of free time.

* Linode Referral Code: https://www.linode.com/?r=3c7fa15a78407c9a3d4aefb027539db2557b3765
* Digital Ocean: http://www.digitalocean.com/?refcode=a6060686b88a

I slightly prefer Linode, because there is a scripting capability called a "StackScript" for creating VPS'es that I plan to use to automate some of these installation functions.

> IMPORTANT: First, you’ll should to copy the httpS URL for most recent bitcoin linux distribution from https://bitcoin.org/en/download as well the most recently httpS URL for the signatures for that release, and you will need the URL for the bitcoin signing keys for versionpast  v0.11.+ . I have included what they were as of writing this tutorial, but you should not rely on them — always verify your bitcoin distribution!

On the the VPS, create the minimal (~$5/m) VPS with Debian 8. Startup the VPS and use the IP address that your VPS dashboard shows you.

```
ssh root@162.243.130.224

uname -a # Should be "Linux debian", and give distribution release "4.8.6-x86_64-linode78"
lsb_release -a #Should report 8.6 or better (this script tested on debian 8.6)


hostnamectl set-hostname bitcoincore-pruned.local # You may not need to do this on Digital Ocean

nano /etc/hosts #add "127.0.0.1  bitcoincore-pruned.local" to hosts file.

dpkg-reconfigure tzdata

date # confirm that this shows your correct time zone

# we should update debian to latest security fixes

apt-get update; apt-get upgrade; apt-get dist-upgrade

# keep debian updates
echo "unattended-upgrades unattended-upgrades/enable_auto_updates boolean true" | debconf-set-selections
apt-get -y install unattended-upgrades

# we need to update iptables to use bitcoin ports

iptables -A INPUT -p tcp --dport 22 -m conntrack --ctstate NEW,ESTABLISHED -j ACCEPT
iptables -A INPUT -p tcp --dport 8333 -j ACCEPT
iptables -A INPUT -m conntrack --ctstate ESTABLISHED -j ACCEPT
iptables -A INPUT -i lo -j ACCEPT
iptables -P INPUT DROP
iptables -P FORWARD DROP

ip6tables -A INPUT -p tcp --dport 22 -m conntrack --ctstate NEW,ESTABLISHED -j ACCEPT
ip6tables -A INPUT -p tcp --dport 8333 -j ACCEPT
ip6tables -A INPUT -m conntrack --ctstate ESTABLISHED -j ACCEPT
ip6tables -A INPUT -i lo -j ACCEPT
ip6tables -P INPUT DROP
ip6tables -P FORWARD DROP

echo "iptables-persistent iptables-persistent/autosave_v4 boolean true" | debconf-set-selections
echo "iptables-persistent iptables-persistent/autosave_v6 boolean true" | debconf-set-selections
apt-get -y install iptables-persistent

adduser user1

adduser user1 sudo

reboot

ssh user1@45.33.46.147

sudo apt-get install haveged # Installs random number tools — otherwise gpg will not have enough randomness on a VPS

## TODO: rng-tools may be installed by haveged -- if working remove these
#sudo apt-get install rng-tools
#/etc/init.d/rng-tools start
#sudo /usr/sbin/rngd -r /dev/urandom # Initialize randomness pool
#cat /dev/urandom | rngtest -c 1000 # check randomness pool

## TODO: make sure haveged is properly initialized, and that it will be started on boot

gpg --gen-key # create a key for this VPS (I don't the name of this machine as the email address — all other questions I press return
    #TODO: I'm not absolutely sure that we need to do this — it is only required for fully qualified --verify if an --lsign is done of the import.

# get current values for these URLs from https://bitcoin.org/en/download — make sure that they are HTTPS not HTTP urls.

wget https://bitcoin.org/bin/bitcoin-core-0.13.2/bitcoin-0.13.2-x86_64-linux-gnu.tar.gz
wget https://bitcoin.org/bin/bitcoin-core-0.13.2/SHA256SUMS.asc
wget https://bitcoin.org/laanwj-releases.asc
    ## TODO: validate the release key, for instance, it is the same as in the keyserver i.e. gpg --recv-keys 0x01EA5486DE18A882D4C2684590C8019E36C2E964
gpg --import laanwj-releases.asc
    gpg --list-keys
    gpg --lsign 36C2E964   # laanwj's bitcoin release key just imported ##
gpg --verify SHA256SUMS.asc
# You should see a 'good signature', for example:
#   gpg: Signature made Tue 03 Jan 2017 12:20:59 AM PST using RSA key ID 36C2E964
#   gpg: Good signature from "Wladimir J. van der Laan (Bitcoin Core binary release signing key) <laanwj@gmail.com>"

cat SHA256SUMS.asc
sha256sum bitcoin-0.13.2-x86_64-linux-gnu.tar.gz
# SHA256 hashes should match

tar xzf bitcoin-0.13.2-x86_64-linux-gnu.tar.gz
sudo install -m 0755 -o root -g root -t /usr/local/bin bitcoin-0.13.2/bin/*
rm bitcoin-0.13.2-x86_64-linux-gnu.tar.gz
rm -rf bitcoin-0.13.2/

mkdir ~/.bitcoin
echo -e "server=1\ndbcache=1536\npar=1\nblocksonly=1\nprune=550\nmaxuploadtarget=137\nmaxconnections=16\nrpcuser=bitcoinrpc\nrpcpassword=$(xxd -l 16 -p /dev/urandom)" > ~/.bitcoin/bitcoin.conf
chmod 600 ~/.bitcoin/bitcoin.conf
more ~/.bitcoin/bitcoin.conf

bitcoind -daemon #start bitcoin

```
Note: it may take up to several minutes for Bitcoin Core to start, during which it will display the following message whenever you use bitcoin-cli:
  `error: {"code":-28,"message":"Verifying blocks..."}`

Also useful is this command, the same number (your local getblockcount = the remote blockchain.info's getblockcount) then your pruned node is ready (about a day).

```
echo `bitcoin-cli getblockcount 2>&1`/`wget -O - http://blockchain.info/q/getblockcount 2>/dev/null`
```

### Useful aliases to add to .bash_profile

```
alias btcdir="cd ~/.bitcoin/" #linux default bitcoind path
        # alias btcdir="cd ~/Library/Application\ Support/Bitcoin/" #mac default bitcoind path
alias bc="bitcoin-cli"
alias bd="bitcoind"
alias btcinfo='bitcoin-cli getinfo | egrep "\"version\"|balance|blocks|connections|errors"'
alias btcblock="echo `bitcoin-cli getblockcount 2>&1`/`wget -O - http://blockchain.info/q/getblockcount 2>/dev/null`"
```

### Useful commands

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
