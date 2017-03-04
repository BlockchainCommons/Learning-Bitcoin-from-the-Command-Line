# 2.2: Setting Up a Bitcoin-Core VPS with StackScript

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

This document explains how to set up a VPS (Virtual Private Sever) to run a Bitcoin node on Linode.com, installed using an automated StackScript. You just need to enter a few commands and boot your VPS. Then, after you go and get yourself an espresso, you'll be able to come back and find your new Bitcoin node happily downloading blocks.

> **WARNING:** Don’t use a VPS for a bitcoin wallet with significant real funds; see http://blog.thestateofme.com/2012/03/03/lessons-to-be-learned-from-the-linode-bitcoin-incident/ . It is  very nice to be able experiment with real bitcoin transactions on a live node without tying up a self-hosted server on a local network. I’ve also found it useful to be able to use an iPhone or iPad to communicate via SSH to my VPS to do some simple bitcoin tasks. But a higher level of safety is required for significant funds.

If you want to instead do all the setup by hand, please read the parallel HOWTO file, [2.1: Setting up a Bitcoin-Core VPS by Hand](./2_1_Setting_Up_a_Bitcoin-Core_VPS_by_Hand.md).

If you already have a Bitcoin node running, instead read the next HOWTO file, [3.0: Playing with bitcoin-cli](3_0_Playing_with_Bitcoin-CLI.md).

## Getting Started with Linode

Linode is a Cloud Hosting service that offers quick, cheap Linux servers with SSD storage. We use them for this tutorial primarily because their BASH-driven StackScripts offer an easy way to automatically set up a Bitcoin node with no fuss and no muss.

### Set Up a Linode Account

You can create a Linode account by going here:

```
https://www.linode.com
```

If you prefer, the following referral code will give you about a month worse of free usage, great for learning Bitcoin:

```
https://www.linode.com/?r=3c7fa15a78407c9a3d4aefb027539db2557b3765
```

You'll need to provide an email address and later preload money from a credit card or PayPal for future costs.

When you're done, you should land on https://manager.linode.com

### Consider Two-Factor Authentication

Your server security won't be complete if people can break into your Linode account, so consider setting up Two-Factor Authentication for it. You can find this setting on your [My Profile: Password & Authentication page](https://manager.linode.com/profile/auth). If you don't do this now, make a TODO item to come back and do it later.

## Creating the Linode Image using a StackScript

### Load the StackScript

There's a copy of our Bitcoin VPS Setup StackScript in [the repo here](Linode-Bitcoin-VpS-Setup.stackscript). This script basically automates the Bitcoin VPS setup instructions from [2A - Setting up a Bitcoin-Core VPS by Hand](./2A_Setting_Up_a_Bitcoin-Core_VPS_by_Hand.md). If you want to be particulary prudent, read it over carefully. If you are satisfied, you can copy that StackScript into your own account by going to [Manage StackScripts](https://manager.linode.com/stackscripts/index), or clicking the link under your list of Linodes. Click "Add a new StackScript", give it a good name (we use `Bitcoin VPS Setup`), then copy and paste the script.

### Add a Linode

You'll next need to click to your Linodes tab and  select "Add a Linode".

A Linode 2048 will suffice for most setups, including: Pruned Mainnet, Pruned Testnet, and even non-Pruned Testnet. They all use less than 30G of storage and they can barely get by with 2G of memory. This is the setup we suggest.

If you want to instead have a non-Pruned Mainnet in a VPS, you'll need to install a Linode with a disk in excess of 120G, which is currently the Linode 12288, which has 192G of storage and 12G of memory and costs approximately $80 per month. We do _not_ suggest this.

The following chart shows minimum requirements

| Setup | Memory | Storage | Linnode |
|-------|--------|---------|---------|
| Mainnet | 2G | 120G | L 12288 |
| Pruned Mainnet | 2G | ~5G | L 2048 |
| Testnet | 2G | ~15G | L 2048 |
| Pruned Testnet | 2G | ~5G | L 2048 |
| Regtest | 2G | ~ | L 2048 |

Just choose your Linode type, choose a Location that's geographically as close to you as possible, and click "Add your Linode!".

_Be aware that the requirements might change over time as the blockchain continues to grow. Watch for "Out of Memory" or "Disk Space is Low!" errors. Either one indicates that you should migrate to the next larger machine!_

### Configure Your Linode

You should now be back on your list of Linodes page, with your VPS listed with a status of "Brand New".

Click on this new Linode to go to its Dashboard.

You will see that no disks are installed yet.

Make sure that Lindeo has completed the initial configuration, which just takes a minute or two. Just look for "Linode Initial Configuration" in the "Host Job Queue" and ensure that it has a little green "Success" buttton.

### Run the StackScript

You're now ready to run your StackScript!

Click on "Deploy an Image" then chooose "Deploying using StackScripts".

If you already added the StackScript to your account, it should be on the list of "Your StackScripts"; select it.

### Enter Stackscript Options

To make sure that you can set up your VPS to your specifications, the StackScript has a few options. Here's what you should enter into them.

_This setup may not work if you do not enter all of this mandatory information:_

**Installation Type.** See _Appendix I_ for more on these Bitcoin installation types. If you're planning to get on the main Bitcoin network, you'll probably want to choose "Pruned Mainnet". If you're wanting to play with Bitcoin Core and learn more about how it works, you'll probably want to choose "Pruned Testnet" or "Private Regtest".

**Short Hostname.** Pick a name for your VPS. For example, "mybtctest"

**Fully Qualified Hostname.** If you're going to include this VPS as part of a network with full DNS records, type in the hostname with its domain. For example, "mybtctest.mydomain.com". Otherwise, just repeat the short hostname and add ".local", for example "mybtctest.local".

**User Password.** Bitcoin will be setup in an account called "user1". This is the password for that account.

_You can freely choose to enter this optional information or skip it:_

**SSH Key.** Copy your local computer's SSH key here; this allows you be able to automatically login in via SSH to your user1 account. If you haven't setup an SSH key on your local computer yet, there are good instructions for it on [Github](https://help.github.com/articles/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent/).  You may also want to add your SSH key into your Linode LISH (Linode Interactive Shell) by going to your "Linode Home Page / My Preferences / LISH Settings /  LISH Keys". Using an SSH key will give you a simpler and safer way to log in to your server.

**SSH-Allowed IPs.** This is a comma-separated list of IPs that will be allowed to SSH into the VPS. For example "192.168.1.15,192.168.1.16". If you do not enter any IPs, _your VPS will not be very secure_. It will constantly be bombarded by attackers trying to find their way in, and they may very well succeed. 

*The remaining questions all have to do with the mechanics of the VPS deployment and should be left as they are with one exception: bump the Swap Disk from 256MB to 512MB, to ensure that you have enough memory to download the blockchain._

Finally, you'll need to fill in a root password, which will be the password used for the root account.

Click "Deploy" to initialize your disks and to prepare your VPS. The whole queue should run in less than a minute. When it's done you should see in the "Host Job Queue", green "Success" buttons stating "Disk Create from StackScript - Setting password for root… done." and "Create Filesystem - 256MB Swap Image".

You may now want to change your Linode VPS's name from the default `linodexxxxxxxx`.  Go to the Settings tab, and change the label to be more useful, such as your VPS's short hostname. For instance I have renamed mine to `bitcoin-testnet-pruned` to differentiate it from other VPSs in my account.

## Boot Your VPS

Your Linode VPS is now ready to boot. If you are not at your new VPS's Dashboard, click on it.

Now select the button "Boot". As soon as you see the green button "Success: System Boot" you can login.

First, you'll need the IP address. Click on the "Linodes" tab and you should see a listing of your VPS, the fact that it's running, its "plan", its IP address, and some other information.

Go to your local console and login to the user1 account using that address:

```
ssh user1@[IP-ADDRESS]
```

For example:

```
ssh user1@192.168.33.11
```

If you configured your VPS to use an SSH key, the login should be automatic (possibly requiring your SSH password to unlock your key). If you didn't configure a SSH key, then you'll need to type in the user1 password.

### Get an Espresso

Here's the big catch: _your StackScript is running right now_. The BASH script gets executed the first time the VPS is booted. That means your VPS isn't ready yet.

So, go take a break, get an espresso, or otherwise relax for a few minutes. There are two parts of the script that take a while: the updating of all the Debian packages; and the downloading of the Bitcoin code. They shouldn't take more than 5 minutes each, which means if you come back in 10 minutes, you'll probably be ready to go.

If you're impatient you can jump ahead and `sudo tail -f ~root/stackscript.log` which will display the current progress of installation, as described in the next section.

## Verify Your Installation

You will know the StackScripts are done when a BITCOIN-IS-READY file appears in the user1 home directory. At that point, your home directory should look like this:

```
$ ls
bitcoin-0.13.2-x86_64-linux-gnu.tar.gz    laanwj-releases.asc
BITCOIN-IS-READY            SHA256SUMS.asc
```

Alongside the BITCOIN-IS-READY file are the various files that were used to install Bitcoin on your VPS. _None_ of them are necessary. We've just left them in case you want to do any additional verification. Otherwise, you can delete them:

```
$ rm *
```

### Verify the Bitcoin Setup

In order to ensure that the downloaded Bitcoin release is valid, the StackScript checks both the signature and the SHA checksum. You should verify that both of those tests came back right:

```
$ sudo grep VERIFICATION ~root/stackscript.log
```

If you see something like the following, all should be well:

```
VERIFICATION SUCCESS / SIG: gpg: Good signature from &quot;Wladimir J. van der Laan (Bitcoin Core binary release signing key) &lt;laanwj@gmail.com&gt;&quot;
VERIFICATION SUCCESS / SHA: 29215a7fe7430224da52fc257686d2d387546eb8acd573a949128696e8761149
```

However, if either of those two checks instead reads "VERIFICATION ERROR", then there's a problem. Since this is all scripted, it's possible that there's just been a minor change that has caused the script's checks not to work right. But, it's also possible that someone is trying to encourage you to run a fake copy of the Bitcoin daemon. So, _be very sure you know what happened before you make use of Bitcoin!_

### Read the Logs

You may also want to read through all of the setup log files, to make sure that nothing unexpected happened during the installation.

It's best to look through the standard StackScript log file, which has all of the output, including errors:

`$ sudo more ~root/stackscript.log`

Note that it is totally normal to see _some_ errors, particularly when running the very noisy gpg software and when various things try to access the non-existant /dev/tty device.

If you want instead to look at a smaller set of info, all of the errors should be in:

`$ sudo more ~root/stackscript.err`

It still has a fair amount of information that isn't errors, but it's a quicker read.

If all look good, congratulations, you have a functioning Bitcoin node using Linode!

## What We Have Wrought

Although the default Debian 8 image that we are using for your VPS has been modified by Linode to be relatively secure, your Bitcoin node as installed through the Linode StackScript is set up with an even high level of security. You may find this limiting, or be unable to do things that you expect. Here are a few notes on that:

### Protected Services

Your Bitcoin VPS installation is minimal and allows almost no communication. This is managed through Part 3 of the StackScript, which blocks access to most of the ports on the VPS.

Two files control these restrictions, one for IPv4 and one for IPv6:

```
/etc/iptables.firewall.rules
/etc/ip6tables.firewall.rules
```

If you look at the rules, you'll see they allow all outbound traffic and all local loopback traffic. However, for inbound traffic, the only connections accepted are Bitcoin, Ping, and SSH. If you want to accept other types of traffic, you will need to open up additional ports ("dports") using the same methodology as shown in the Bitcoin port 8333 connection. For example, to allow connections to port 25 (mail), would require adding the following to the iptables and the ip6tables:

```
-A INPUT -p tcp --dport 25 -j ACCEPT
-A INPUT -p udp --dport 25 -j ACCEPT
```

This example opens up access to port 25 for TCP and UDP connections. Other popular ports are 80 (HTTP), 443 (HTTPS), 53 (DNS), 110 (POP), and 143 (IMAP).  Be sure any such changes are put above the logs and defaults that appear at the end of the iptables.firewall.rules files.

You can immediately incorporate your changes by running the following script:

```
$ /etc/network/if-pre-up.d/firewall
```

### Protected Shells

If you provided an IP access for SSH control, SSH (and SCP) access to the server is severely restricted. /etc/hosts.deny disallows anyone from logging in. _We do not suggest changing this_. /etc/hosts.allow then allows specific IP addresses. Just add more IP addresses in a comma-separated list if you need to offer more access.

For example:

```
sshd: 127.0.0.1, 192.128.23.1
```

### Automated Upgrades

Debian is also set up to automatically upgrade itself, to ensure that it remains abreast of the newest security patches.

If for some reason you wanted to change this (_we don't suggest it_), you can do this:

```
echo "unattended-upgrades unattended-upgrades/enable_auto_updates boolean false" | debconf-set-selections
```

## Playing with Bitcoin

So now you probably want to play with Bitcoin!

But wait, your Bitcoin daemon is probably still downloading blocks. This alias, from your .bash configuration will tell you how things are going:

```
$ btcblock
```

If you choose one of the pruned options, it'll probably take a day to download everything.

So, it might be time for a few more espressos.

But, when you're ready to go, continue on with [3.0: Playing with bitcoin-cli](3_0_Playing_with_Bitcoin-CLI.md), where we'll talk about the files and how you can start experimenting.

## Appendix I: Bitcoin Installation Types

**Mainnet.** This will download the entirety of the Bitnet blockchain. That's 120G of data (and getting more every day).

**Pruned Mainnet.** This will cut the blockchain you're storing down to just the last 550 blocks. If you're not mining or running some other Bitcoin service, this should be plenty for validation.

**Testnet.** This gives you access to an alternative Bitcoin blockchain where the Bitcoins don't actually have value. It's intended for experimentation and testing.

**Pruned Testnet.** This is just the last 550 blocks of Testnet ... because the Testnet blockchain is pretty big now too.

**Private Regtest.** This is Regression Testing Mode, which lets you run a totally local Bitcoin server. It allows for even more in-depth testing. There's no pruning needed here, because you'll be starting from scratch.
