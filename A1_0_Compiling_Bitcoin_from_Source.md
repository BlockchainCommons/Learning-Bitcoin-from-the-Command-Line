# Appendix I - Compiling Bitcoin from Source

In this tutorial we will learn how to compile Bitcoin Core from source on a Debian based system. It is not necessary to compile bitcoin from source but, it has many benefits:

1. You are always update to the latest release. Caveat: Being always updated is not necessary for Bitcoin Core since the software is always backwards compatible, meaning an old version of Bitcoin Core will still be able to participate in the Bitcoin network, though you may not have the latest features. You should always check the features of a new release before updating.
2. You don't need to depend on pre-compiled Bitcoin Core binaries. This requires less trust. Even though the maintainers of Bitcoin Core do a great job of maintaining the integrity of the code, a pre-compiled binary is a few steps removed from source code. When you compile from the source code, the code is inspected before compilation.
3. You can customize the build, like disabling the wallet or the GUI, etc.

## Environment

This tutorial uses Debian 10.4.0 OS on amd64 architecture(64 bit computers), but you can use this tutorial on any Debian based system e.g. Ubuntu, Mint, etc. On other linux systems, you can adapt the following steps with the package manager for that system.

## Legend

- Commands starting with `$` are executed as the user.
- Commands starting with `#` are executed as root.
- Text starting with `#` on the same line as the command are comments.

Type the commands without `$` or `#`

## Prerequisites

- Basic or no familiarity with the command line and enthusiasm. The terminal is your most powerful ally, not something to be feared. You can simply copy and paste the following commands to compile bitcoin.
- Admin previlages. If your user is not in the sudoers list then do the following otherwise skip this.

```
$ su -  # enter root. It will ask you for the root password.
# apt-get install sudo  # check if sudo is installed.
# usermod -aG <username> sudo  # add user to the sudoers list
# reboot  # reboot computer
```

## Steps

### 1. Update

First thing to do is always update the system using:

`$ sudo apt-get update`

### 2. Install Git and Dependencies

First we install `git` to download the source code and `build-essential` which compiles the code, do:

`$ sudo apt-get install git build-essential -y`

Then we install remaining dependencies:

`$ sudo apt-get install libtool autotools-dev automake pkg-config bsdmainutils python3 libssl-dev libevent-dev libboost-system-dev libboost-filesystem-dev libboost-chrono-dev libboost-test-dev libboost-thread-dev libminiupnpc-dev libzmq3-dev libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools libprotobuf-dev protobuf-compiler ccache -y`

### 3. Download the Source Code

Once the dependencies are installed, we download the repository(repo) containing the source code from github:

`$ git clone https://github.com/bitcoin/bitcoin.git`

Check the contents of the repo:

`$ ls bitcoin`

![cloning the repo](/res/LBftCLI-compiling_bitcoin-git.png)

### 4. Install Berkley DB v4.8

1. Enter `contrib` directory: `$ cd bitcoin/contrib/`
2. Run the following command: ```$ ./install_db4.sh `pwd` ```

Once it's downloaded you should see the following output. Take note of the output, we will use it to configure bitcoin while building:

![db4](res/LBftCLI-compiling_bitcoin-db4.png)

### 5. Compile Bitcoin Core

It is recommended that you compile from a tagged branch which is more stable unless you want to try the bleeding edge of bitcoin development. Run the following commands to get the list of tags:

```bash
$ git tag -n | sort -V  # get tags odered from most recent.
$ git checkout <TAG>  # type the relevant TAG you want to compile e.g. git checkout v0.20.0
```

- Once in the relevant tag branch execute the following from inside the `bitcoin` directory:

```bash
$ ./autogen.sh
$ export BDB_PREFIX='<PATH-TO>/db4'  # enter relevant PATH-TO from the output of the install_db4.sh script
$ ./configure BDB_LIBS="-L${BDB_PREFIX}/lib -ldb_cxx-4.8" BDB_CFLAGS="-I${BDB_PREFIX}/include"
$ make  # build bitcoin core
```

### 6. Test the Build

If you want to check your build(good idea), run the following tests:

1. `$ make check` will run Unit Tests which should all return `PASS`.
2. `$ test/functional/test_runner.py --extended` will run extended functional tests. Omit `--extended` flag if you want to skip a few tests. This will take a while.

### 7. Run or Install Bitcoin Core

Now that you have compiled Bitcoin Core from source, you can start using it or install it for global availability.

#### Run Bitcoin Core without Installing

`$ src/qt/bitcoin-qt` to launch the GUI.
`$ src/bitcoind` to run bitcoin on the command line.

### Install Bitcoin Core

`$ sudo make install` will install bitcoin core globally. Once installed you can then run bitcoin from anywhere in the command just as any other software like so: `$ bitcoin-qt` for the GUI or `$bitcoind` for command line.

