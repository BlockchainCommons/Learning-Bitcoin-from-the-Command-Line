# Appendix II: Compiling Bitcoin from Source

This course presumes that you will use a script to create a Bitcoin environment, either using Bitcoin Standup for Linode per [§2.1](02_1_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md), or via some other means per [§2.2](02_2_Setting_Up_Bitcoin_Core_Other.md). However, you may prefer to compile Bitcoin by hand.

This has the following benefits:

1. You will always be up-to-date with the latest release. Caveat: Being always updated is not necessary for Bitcoin Core since the software is always backwards compatible, meaning an old version of Bitcoin Core will still be able to participate in the Bitcoin network, though you may not have the latest features. You should always check the features of a new release before updating.
2. You won't need to depend on pre-compiled Bitcoin Core binaries. This requires less trust. Even though the maintainers of Bitcoin Core do a great job of maintaining the integrity of the code, a pre-compiled binary is a few steps removed from source code. When you compile from the source code, the code can be inspected before compilation.
3. You can customize the build, doing things such as disabling the wallet or the GUI.

## Prepare your Environment

This tutorial uses Debian 10.4.kv0 OS on an amd64 architecture(64-bit computers), but you can use this tutorial on any Debian based system (e.g. Ubuntu, Mint, etc.). For other Linux systems, you can adapt the following steps with the package manager for that system.

You can have basic or no familiarity with the command line as long as you have enthusiasm. The terminal is your most powerful ally, not something to be feared. You can simply copy and paste the following commands to compile bitcoin. (A command with a "$" is a normal user command, and one with a "#" is a super-user/root command.)

If your user is not in the sudoers list then do the following:

```
$ su root
<enter root passwd>
# apt-get install sudo
# usermod -aG sudo <username>
# reboot
```

## Install Bitcoin

### Step 1: Update Your System

First, update the system using:
```
$ sudo apt-get update
```

### Step 2: Install Git and Dependencies

Install `git`, which will allow you to download the source code, and `build-essential`, which compiles the code:
```
$ sudo apt-get install git build-essential -y
```

Afterward, install remaining dependencies:
```
$ sudo apt-get install libtool autotools-dev automake pkg-config bsdmainutils python3 libssl-dev libevent-dev libboost-system-dev libboost-filesystem-dev libboost-chrono-dev libboost-test-dev libboost-thread-dev libminiupnpc-dev libzmq3-dev libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools libprotobuf-dev protobuf-compiler ccache -y
```

### Step 3: Download the Source Code

Once the dependencies are installed, download the repository (repo) containing the Bitcoin source code from github:
```
$ git clone https://github.com/bitcoin/bitcoin.git
```
Check the contents of the repo:
```
$ ls bitcoin
````
It should approximately match the following contents:

![cloning the repo](./public/LBftCLI-compiling_bitcoin-git.png)

### Step 4: Install Berkley DB v4.8

1. Enter `contrib` directory: `$ cd bitcoin/contrib/`
2. Run the following command: ```$ ./install_db4.sh `pwd` ```

Once it's downloaded you should see the following output. Take note of the output, you will use it to configure bitcoin while building:

![db4](./public/LBftCLI-compiling_bitcoin-db4.png)

### Step 5: Compile Bitcoin Core

It is recommended that you compile from a tagged branch, which is more stable, unless you want to try the bleeding edge of bitcoin development. Run the following command to get the list of tags, ordered from the most recent:
```
$ git tag -n | sort -V
```
Then choose a tag such as `v0.20.0`:
```
$ git checkout <TAG>
```

Once you've selected a tag branch, execute the following from inside the `bitcoin` directory. The `<PATH-TO>` should be the output of the `install_db4.sh` script.

```
$ ./autogen.sh
$ export BDB_PREFIX='<PATH-TO>/db4'
$ ./configure BDB_LIBS="-L${BDB_PREFIX}/lib -ldb_cxx-4.8" BDB_CFLAGS="-I${BDB_PREFIX}/include"
$ make  # build bitcoin core
```

### Step 6: Test the Build

If you want to check your build (which is a good idea), run the following tests:

1. `$ make check` will run Unit Tests, which should all return `PASS`.
2. `$ test/functional/test_runner.py --extended` will run extended functional tests. Omit `--extended` flag if you want to skip a few tests. This will take a while.

### Step 7: Run or Install Bitcoin Core

Now that you have compiled Bitcoin Core from source, you can start using it or install it for global availability.

#### Run Bitcoin Core without Installing

To just run Bitcoin Core:

`$ src/qt/bitcoin-qt` to launch the GUI.
`$ src/bitcoind` to run bitcoin on the command line.

### Install Bitcoin Core

To install:

`$ sudo make install` will install bitcoin core globally. Once installed you can then run bitcoin from anywhere in the command line, just as any other software like so: `$ bitcoin-qt` for the GUI or `bitcoind` and then `bitcoin-cli` for command line.

## Finalize Your System

By compiling Bitcoin from source, you've increased the trustlessness of your setup. However, you're far short of all the additional security provided by a Bitcoin Standup setup. To resolve this, you may want to walk through the entire [Linode Stackscript](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts/blob/master/Scripts/LinodeStandUp.sh) and step-by-step run all the commands. The only place you need to be careful is in Step 6, which installs Bitcoin. Skip just past where you've verified your binaries, and continue from there.

## Summary: Compiling Bitcoin from Source

If you wanted the increased security of installing Bitcoin from source, you should now have that. Hopefully, you've also gone through the Linode Stackscript to set up a more secure server.

## What's Next?

If you were in the process of creating a Bitcoin node for use in this course, you should continue on with [Chapter 3: Understanding Your Bitcoin Setup](03_0_Understanding_Your_Bitcoin_Setup.md).

If you are reading through the appendices, continue with [Appendix III: Using Bitcoin Regtest](A3_0_Using_Bitcoin_Regtest.md).

