# Appendix I: Understanding Bitcoin Standup

[§2.1: Setting Up a Bitcoin Core VPS with StackScript](02_1_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md) explains the process of creating a Bitcoin node using [Bitcoin-Standup-Scripts](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts). The following appendix explains what the major sections of the script do. You may wish to follow along in [Linode Standup](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts/blob/master/Scripts/LinodeStandUp.sh) in another window.

## Step 1: Hostname

Your host's name is stored in `/etc/hostname` and set with the `hostname` command. It also appears in `/etc/hosts`.

## Step 2: Timezone

Your host's timezone is stored in `/etc/timezone`, then an appropriate file from `/usr/share/zoneinfo/` is copied to `/etc/localtime`

## Step 3: Updating Debian

The `apt-get` package manager is used to bring your machine up to date and to install `gnupg`, the random-number generator `haveged`, and the uncomplicated firewall `ufw`. 

Your machine is setup to automatically stay up to date with `echo "unattended-upgrades unattended-upgrades/enable_auto_updates boolean true" | debconf-set-selections`.

## Step 4: Setting Up a User

A `standup` user is created, which will be used for your Bitcoin applications. It also has `sudo` permissions, allowing you to take privileged actions with this account.

If you supplied an SSH key, it will allow you access to this account (otherwise, you must use the password you created in setup).

If you supplied an IP address, `ssh` access will be limited to that address, per `/etc/hosts.allow`.

## Step 5: Setting Up Tor

Tor is installed to provide protected (hidden) services to access Bitcoin's RPC commands through your server. See [§14.1: Verifying Your Tor Setup](14_1_Verifying_Your_Tor_Setup.md) for more information on your Tor Setup.

If you supplied an authorized client for the hidden services, access will be limited to that key, per `/var/lib/tor/standup/authorized_clients`. If you did not, [§14.2](14_2_Changing_Your_Bitcoin_Hidden_Services.md) explains how to do so at a later date.

## Step 6: Installing Bitcoin

Bitcoin is installed in `~standup/.bitcoin`. Your configuration is stored in `~standup/.bitcoin/bitcoin.conf`.

Be sure that the checksums verified per [§2.1](02_1_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md), otherwise you could be exposed to a supply-chain attack.

## Step 7: Installing QR Encoder

To keep everything compatible with [GordianSystem](https://github.com/BlockchainCommons/GordianSystem) a QR code is created at `/qrcode.png`. This can be read from a QuickConnect client such as [GordianWallet](https://github.com/BlockchainCommons/GordianWallet-iOS).

## Conclusion — Understanding Bitcoin Standup

Bitcoin Standup uses scripts to try and match much of the functionality of a [GordianNode](https://github.com/BlockchainCommons/GordianNode-macOS). It should provide you with a secure Bitcoin environment built on a foundation of Bitcoin Core and Tor for RPC communications.

## What's Next?

If you were in the process of creating a Bitcoin node for use in this course, you should return to [§2.1](02_1_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md).

If you are reading through the appendices, continue with [Appendix II: Compiling Bitcoin from Source](A2_0_Compiling_Bitcoin_from_Source.md).
