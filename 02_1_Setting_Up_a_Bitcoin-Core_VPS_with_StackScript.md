# 2.1: Setting Up a Bitcoin-Core VPS with Bitcoin Standup

This document explains how to set up a VPS (Virtual Private Sever) to run a Bitcoin node on Linode.com, installed using an automated StackScript from the [Bitcoin Standup project](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts). You just need to enter a few commands and boot your VPS. Almost immediately after you boot, you'll find your new Bitcoin node happily downloading blocks.

> :warning: **WARNING:** Don’t use a VPS for a bitcoin wallet with significant real funds; see http://blog.thestateofme.com/2012/03/03/lessons-to-be-learned-from-the-linode-bitcoin-incident/ . It is very nice to be able experiment with real bitcoin transactions on a live node without tying up a self-hosted server on a local network. It's also useful to be able to use an iPhone or iPad to communicate via SSH to your VPS to do some simple bitcoin tasks. But a higher level of safety is required for significant funds.

* If you want to understand what this setup does, read [Appendix I: Understanding Bitcoin Standup](A1_0_Understanding_Bitcoin_Standup.md) as you install.
* If you want to instead setup on a machine other than a Linode VPS, such as an AWS machine or a Mac, goto [§2.2: Setting Up a Bitcoin-Core via Other Means](02_2_Setting_Up_Bitcoin_Core_Other.md)
* If you already have a Bitcoin node running, goto [Chapter Three: Understanding Your Bitcoin Setup](03_0_Understanding_Your_Bitcoin_Setup.md).

## Getting Started with Linode

Linode is a Cloud Hosting service that offers quick, cheap Linux servers with SSD storage. We use them for this tutorial primarily because their BASH-driven StackScripts offer an easy way to automatically set up a Bitcoin node with no fuss and no muss.

### Set Up a Linode Account

You can create a Linode account by going here:

```
https://www.linode.com
```

If you prefer, the following referral code will give you two months worth of free usage (up to $100), great for learning Bitcoin:

[https://www.linode.com/?r=3c7fa15a78407c9a3d4aefb027539db2557b3765](https://www.linode.com/?r=3c7fa15a78407c9a3d4aefb027539db2557b3765)

You'll need to provide an email address and later preload money from a credit card or PayPal for future costs.

When you're done, you should land on [https://cloud.linode.com/dashboard](https://cloud.linode.com/dashboard).

### Consider Two-Factor Authentication

Your server security won't be complete if people can break into your Linode account, so consider setting up Two-Factor Authentication for it. You can find this setting on your [My Profile: Password & Authentication page](https://manager.linode.com/profile/auth). If you don't do this now, make a TODO item to come back and do it later.

## Creating the Linode Image using a StackScript

### Load the StackScript

Download the [Linode Standup Script](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts/blob/master/Scripts/LinodeStandUp.sh) from the [Bitcoin Standup Scripts repo](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts). This script basically automates all Bitcoin VPS setup instructions. If you want to be particulary prudent, read it over carefully. If you are satisfied, you can copy that StackScript into your own account by going to the [Stackscripts page](https://cloud.linode.com/stackscripts?type=account) on your Linode account and selecting to [Create New Stackscript](https://cloud.linode.com/stackscripts/create). Give it a good name (we use `Bitcoin Standup`), then copy and paste the script. Choose Debian 11 for your target image and "Save" it.

### Do the Initial Setup

You're now ready to create a node based on the Stackscript.

1. On the [Stackscripts page](https://cloud.linode.com/stackscripts?type=account), click on the "..." to the right of your new script and choose "Deploy New Linode".
2. Fill in a short and a fully qualified hostname
   * **Short Hostname.** Pick a name for your VPS. For example, "mybtctest".
   * **Fully Qualified Hostname.** If you're going to include this VPS as part of a network with full DNS records, type in the hostname with its domain. For example, "mybtctest.mydomain.com". Otherwise, just repeat the short hostname and add ".local", for example "mybtctest.local".
3. Enter the password for the "standup" user.
4. Fill in the appropriate advanced options. 
   * **X25519 Public Key.** This is a public key to add to Tor's list of authorized clients. If you don't use it, anyone who gets the QR code for your node can access it. You'll get this public key from whichever client you're using to connect to your node. For example, if you use [FullyNoded 2](https://github.com/BlockchainCommons/FullyNoded-2), you can go to its settings and "Export Tor V3 Authentication Public Key" for use here.
   * **Installation Type.** This is likely "Mainnet" or "Pruned Mainnet" if you are setting up a node for usage and "Testnet" or "Pruned Testnet" if you're just playing around. The bulk of this tutorial will assume you chose "Pruned Testnet", but you should still be able to follow along with other types. See the [Synopsis](#synopsis-bitcoin-installation-types) for more information on these options. (Note that if you plan to try out the Lightning chapters, you'll probably want to use an Unpruned node, as working with Pruned nodes on Lightning is iffy. See [§18.1](18_1_Verifying_Your_Lightning_Setup.md#compiling-the-source-code) for the specifics.)
   * **SSH Key.** Copy your local computer's SSH key here; this allows you be able to automatically login in via SSH to the standup account. If you haven't setup an SSH key on your local computer yet, there are good instructions for it on [Github](https://help.github.com/articles/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent/).  You may also want to add your SSH key into your Linode LISH (Linode Interactive Shell) by going to your "Linode Home Page / My Preferences / LISH Settings /  LISH Keys". Using an SSH key will give you a simpler and safer way to log in to your server.
   * **SSH-Allowed IPs.** This is a comma-separated list of IPs that will be allowed to SSH into the VPS. For example "192.168.1.15,192.168.1.16". If you do not enter any IPs, _your VPS will not be very secure_. It will constantly be bombarded by attackers trying to find their way in, and they may very well succeed.
5. Select an Image
   * **Target Image.** If you followed the instructions, this will only allow you to select "Debian 11" (though previous versions of this Stackscript worked with Debian 9 or 10and might still).
6. Choose a region for where the Linode will be located.

*The remaining questions all have to do with the mechanics of the VPS deployment and should be left as they are with one exception: bump the Swap Disk from 256MB to 512MB, to ensure that you have enough memory to download the blockchain.*

### Choose Other Standup Options

Blockchain Commons is currently in the process of expanding its Bitcoin Standup Scripts with options to install Lightning and other Bitcoin apps of note. Take a look at any extra options, and see if they're things that you'd like to play with. In particular, if Lightning is an option, we suggest installing it, because it will make [Chapter 18](18_0_Understanding_Your_Lightning_Setup.md) and [Chapter 19](19_0_Using_Lightning.md) much easier. 

### Choose a Linode Plan

You'll next to choose a Linode plan.

Linode will default to Dedicated-CPU plans, but you can select the more cost-efficient Shared-CPU instead. A Shared-CPU Linode 4GB will suffice for most setups, including: Pruned Mainnet, Pruned Testnet, and even non-Pruned Testnet. They all use less than 50G of storage and 4GB is a comfortable amount of memory. This is the setup we suggest. It runs $20 per month.

If you want to instead have a non-Pruned Mainnet in a VPS, you'll need to install a Linode with a disk in excess of 280G(!), which is currently the Linode 16GB, which has 320G of storage and 16G of memory and costs approximately $80 per month. We do _not_ suggest this.

The following chart shows minimum requirements

| Setup | Memory | Storage | Linnode |
|-------|--------|---------|---------|
| Mainnet | 2G | 280G | Linode 16GB |
| Pruned Mainnet | 2G | ~5G | Linode 4GB |
| Testnet | 2G | ~15G | Linode 4GB |
| Pruned Testnet | 2G | ~5G | Linode 4GB |
| Regtest | 2G | ~ | Linode 4GB |

Note, there may be ways to reduce both costs.

* For the machines we suggest as **Linode 4GB**, you may be able to reduce that to a Linode 2GB. Some versions of Bitcoin Core have worked well at that size, some have occasionally run out of memory and then recovered, and some have continuously run out of memory. Remember to up that swap space to maximize the odds of this working. Use at your own risk.
* For the Unpruned Mainnet, which we suggest as a **Linode 16GB**, you can probably get by with a Linode 4GB, but add [Block Storage](https://cloud.linode.com/volumes) sufficient to store the blockchain. This is certainly a better long-term solution since the Bitcoin blockchain's storage requirements continuously increase if you don't prune, while the CPU requirements don't (or don't to the same degree). A 320 GibiByte storage would be $32 a month, which combined with a Linode 4GB is $52 a month, instead of $80, and more importantly you can keep growing it. We don't fully document this setup for two reasons (1) we don't suggest the unpruned mainnet setup, and so we suspect it's a much less common setup; and (2) we haven't tested how Linodes volumes compare to their intrinic SSDs for performance and usage. But there's full documentation on the Block Storage page. You'd need to set up the Linode, run its stackscript, but then interrupt it to move the blockchain storage overly to a newly commissioned volume before continuing.

If you are running a deployment that will be transacting real Bitcoins, you may want to alternatively consider a Dedicated-CPU Linode, which tends to run 50% more expensive than the Shared-CPU Linode. We've generally found the Shared CPUs to be entirely sufficient, but for a wide deployment, you may wish to consider higher levels of reliability.

### Do the Final Setup

The last thing you need to do is enter a root password. (If you missed anything, you'll be told so now!)

Click "Deploy" to initialize your disks and to prepare your VPS. The whole queue should run in less than a minute. When it's done you should see in the "Host Job Queue", green "Success" buttons stating "Disk Create from StackScript - Setting password for root… done." and "Create Filesystem - 256MB Swap Image".

You may now want to change your Linode VPS's name from the default `linodexxxxxxxx`.  Go to the Settings tab, and change the label to be more useful, such as your VPS's short hostname. For instance you might name it `bitcoin-testnet-pruned` to differentiate it from other VPSs in your account.

## Login to Your VPS

If you watch your Linode control panel, you should see the new computer spin up. When the job has reached 100%, you'll be able to login.

First, you'll need the IP address. Click on the "Linodes" tab and you should see a listing of your VPS, the fact that it's running, its "plan", its IP address, and some other information.

Go to your local console and login to the `standup` account using that address:

```
ssh standup@[IP-ADDRESS]
```

For example:

```
ssh standup@192.168.33.11
```

If you configured your VPS to use an SSH key, the login should be automatic (possibly requiring your SSH password to unlock your key). If you didn't configure a SSH key, then you'll need to type in the user1 password.

### Wait a Few Minutes

Here's a little catch: _your StackScript is running right now_. The BASH script gets executed the first time the VPS is booted. That means your VPS isn't ready yet.

The total run time is about 10 minutes. So, go take a break, get an espresso, or otherwise relax for a few minutes. There are two parts of the script that take a while: the updating of all the Debian packages; and the downloading of the Bitcoin code. They shouldn't take more than 5 minutes each, which means if you come back in 10 minutes, you'll probably be ready to go.

If you're impatient you can jump ahead and `sudo tail -f /standup.log` which will display the current progress of installation, as described in the next section.

## Verify Your Installation

You'll know that stackscrpit is done when the `tail` of the `standup.log` says something like the following:
```
/root/StackScript - Bitcoin is setup as a service and will automatically start if your VPS reboots and so is Tor
/root/StackScript - You can manually stop Bitcoin with: sudo systemctl stop bitcoind.service
/root/StackScript - You can manually start Bitcoin with: sudo systemctl start bitcoind.service
```
At that point, your home directory should look like this:

```
$ ls
bitcoin-22.0-x86_64-linux-gnu.tar.gz  keys.txt  SHA256SUMS  SHA256SUMS.asc
```

These are the various files that were used to install Bitcoin on your VPS. _None_ of them are necessary. We've just left them in case you want to do any additional verification. Otherwise, you can delete them:

```
$ rm *
```

### Verify the Bitcoin Setup

In order to ensure that the downloaded Bitcoin release is valid, the StackScript checks both the signature and the SHA checksum. You should verify that both of those tests came back right:

```
$ sudo grep VERIFICATION /standup.log
```

If you see something like the following, all should be well:

```
./standup.sh - SIG VERIFICATION SUCCESS: 9 GOOD SIGNATURES FOUND.
./standup.sh - SHA VERIFICATION SUCCESS / SHA: bitcoin-22.0-x86_64-linux-gnu.tar.gz: OK
```
If either of those two checks instead reads "VERIFICATION ERROR", then there's a problem.

The log also contains more information on the Signatures, if you want to make sure you know _who_ signed the Bitcoin release:
```
$ sudo grep -i good /standup.log
./standup.sh - SIG VERIFICATION SUCCESS: 9 GOOD SIGNATURES FOUND.
gpg: Good signature from "Andrew Chow (Official New Key) <achow101@gmail.com>" [unknown]
gpg: Good signature from "Ben Carman <benthecarman@live.com>" [unknown]
gpg: Good signature from "Antoine Poinsot <darosior@protonmail.com>" [unknown]
gpg: Good signature from "Stephan Oeste (it) <it@oeste.de>" [unknown]
gpg: Good signature from "Michael Ford (bitcoin-otc) <fanquake@gmail.com>" [unknown]
gpg: Good signature from "Oliver Gugger <gugger@gmail.com>" [unknown]
gpg: Good signature from "Hennadii Stepanov (hebasto) <hebasto@gmail.com>" [unknown]
gpg: Good signature from "Jon Atack <jon@atack.com>" [unknown]
gpg: Good signature from "Wladimir J. van der Laan <laanwj@visucore.com>" [unknown]
```
Since this is all scripted, it's possible that there's just been a minor change that has caused the script's checks not to work right. (This has happened a few times over the existence of the script that became Standup.) But, it's also possible that someone is trying to encourage you to run a fake copy of the Bitcoin daemon. So, _be very sure you know what happened before you make use of Bitcoin!_

### Read the Logs

You may also want to read through all of the setup log files, to make sure that nothing unexpected happened during the installation.

It's best to look through the standard StackScript log file, which has all of the output, including errors:

`$ sudo more /standup.log`

Note that it is totally normal to see _some_ errors, particularly when running the very noisy gpg software and when various things try to access the non-existant `/dev/tty` device.

If you want instead to look at a smaller set of info, all of the errors should be in:

`$ sudo more /standup.err`

It still has a fair amount of information that isn't errors, but it's a quicker read.

If all look good, congratulations, you have a functioning Bitcoin node using Linode!

## What We Have Wrought

Although the default Debian 11 image that we are using for your VPS has been modified by Linode to be relatively secure, your Bitcoin node as installed through the Linode StackScript is set up with an even higher level of security. You may find this limiting, or be unable to do things that you expect. Here are a few notes on that:

### Protected Services

Your Bitcoin VPS installation is minimal and allows almost no communication. This is done through the uncomplicated firewall (`ufw`), which blocks everything except SSH connections. There's also some additional security possible for your RFC ports, thanks to the hidden services installed by Tor.

**Adjusting UFW.** You should probably leave UFW in its super-protected stage! You don't want to use a Bitcoin machine for other services, because everyone increases your vulnerability! If you decide otherwise, there are several [guides to UFW](https://www.digitalocean.com/community/tutorials/ufw-essentials-common-firewall-rules-and-commands) that will allow you to add services. As advertised, it's uncomplicated. For example adding mail services would just require opening the mail port: `sudo ufw allow 25`. But don't do that.

**Adjusting Tor.** You might want to better protect services like SSH. See [Chapter 14: Using Tor](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/14_0_Using_Tor.md) for more on Tor.

### Protected Shells

If you defined "SSH-allowed IPs", SSH (and SCP) access to the server is severely restricted. `/etc/hosts.deny` disallows anyone from logging in. _We do not suggest changing this_. `/etc/hosts.allow` then allows specific IP addresses. Just add more IP addresses in a comma-separated list if you need to offer more access.

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

_If you'd like to know more about what the Bitcoin Standup stackscript does, please see [Appendix I: Understanding Bitcoin Standup](A1_0_Understanding_Bitcoin_Standup.md)._

## Playing with Bitcoin

So now you probably want to play with Bitcoin!

But wait, your Bitcoin daemon is probably still downloading blocks. The `bitcoin-cli getblockcount` will tell you how you're currently doing:
```
$ bitcoin-cli getblockcount
1771352
```
If it's different every time you type the command, you need to wait before working with Bitcoin. This takes 1-6 hours currently for a  pruned setup, depending on your precise machine. 

But, once it settles at a number, you're ready to continue!

Still, it might be time for a few more espressos. But soon enough, your system will be ready to go, and you'll be read to start experimenting.

## Summary: Setting Up a Bitcoin-Core VPS by Hand

Creating a Bitcoin-Core VPS with the Standup scripts made the whole process quick, simple and (hopefully) painless.

## What's Next?

You have a few options for what's next:

   * Read the [StackScript](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts/blob/master/Scripts/LinodeStandUp.sh) to understand your setup.
   * Read what the StackScript does in [Appendix I](A1_0_Understanding_Bitcoin_Standup.md).
   * Choose an entirely alternate methodology in [§2.2: Setting Up a Bitcoin-Core Machine via Other Means](02_2_Setting_Up_Bitcoin_Core_Other.md).
   * Move on to "bitcoin-cli" with [Chapter Three: Understanding Your Bitcoin Setup](03_0_Understanding_Your_Bitcoin_Setup.md).

## Synopsis: Bitcoin Installation Types

**Mainnet.** This will download the entirety of the Bitnet blockchain. That's 280G of data (and getting more every day). 

**Pruned Mainnet.** This will cut the blockchain you're storing down to just the last 550 blocks. If you're not mining or running some other Bitcoin service, this should be plenty for validation.

**Testnet.** This gives you access to an alternative Bitcoin blockchain where the Bitcoins don't actually have value. It's intended for experimentation and testing.

**Pruned Testnet.** This is just the last 550 blocks of Testnet ... because the Testnet blockchain is pretty big now too.

**Private Regtest.** This is Regression Testing Mode, which lets you run a totally local Bitcoin server. It allows for even more in-depth testing. There's no pruning needed here, because you'll be starting from scratch. This is a very different setup, and so is covered in [Appendix 3](A3_0_Using_Bitcoin_Regtest.md).
