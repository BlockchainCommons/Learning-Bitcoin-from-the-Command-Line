# 17.6: Accessing Bitcoind with Swift

> :information_source: **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

This section explains how to interact with `bitcoind` using the Swift programming language.

## Setting Up Swift on Your Mac

To date, you've built all of your alternative programming language development environments on your Debian virtual node. However, that's not the best platform for Swift. Though there is a version of Swift available for Ubuntu platforms, it's not fully featured, and it works somewhat differently from the Mac-native Swift. A "variant" at the bottom of this section explains how to set it up, but be warned that you'll be in uncharted territory.

Instead. we suggest an optimal Swift environment on a Mac. There are three major steps in setting that up.

### 1. Install Xcode

You're going to need `Xcode`, the integrated development enviroment for Swift and Objective-C. That can be easily installed by going to the Mac App Store and `Get`ting Xcode.

#### Alternative: Install by Hand

Some people advise against an App Store install because it's somewhat all-or-nothing; it also won't work if you're still using Mojave because you want to avoid Catalina's incompatibilities. In that case you can download directly from the [Developer Area](https://developer.apple.com/download/more/) at Apple.

If you're using Mojave, you'll need the xip for Xcode 10.3.1. Otherwise, get the newest one.

Once it's downloaded, you can click on the XIP to extract it, then move the Xcode app to your Applications folder.

(EIther way, you should have Xcode installed in your Applications folder at the end of this step.)

### 2. Install the Gordian Server

You're also going to need a Bitcoin node on your Mac, so that you can communicate with it. Technically, you could use a remote node, and access it with the RPC login and password over the net. However, we suggest you instead install a full node directly on your Mac, because that's the safest and cleanest setup, ensuring that none of your communications leave your machine.

To easily install a full node on your Mac, use Blockchain Commons' [GordianServer for MacOS](https://github.com/BlockchainCommons/GordianServer-macOS). See the [installation instructions](https://github.com/BlockchainCommons/GordianServer-macOS#installation-instructions) in the README, but generally all you have to do is download the current DMG, open it, and install that app in your Applications too.

Afterward, run the GordianServer App, and tell it to `Start` Testnet. 

> :link: **TESTNET vs. MAINNET:** Or `Start` Mainnet.

### 3. Find Your GordianServer Info

As usual, you'll need the RPC login and password. That's in `~/Library/Application Support/Bitcoin/bitcoin.conf` by default under Gordian.

Pull out the `rpcuser` and `rpcpassword` information; you'll need it to build your RPC connection:
```
$ grep rpc ~/Library/Application\ Support/Bitcoin/bitcoin.conf
rpcuser=oIjA53JC2u
rpcpassword=ebVCeSyyM0LurvgQyi0exWTqm4oU0rZU
...
```

## Variant: Deploying Swift on Ubuntu

If you prefer to deploy Swift on Ubuntu, you can do so, though the functionality isn't the same. Some of the code in this chapter will likely generate errors that you'll need to resolve, and you'll also need to do more work to link in C libraries.

To get started, install some required Debian libraries:
```
$ sudo apt-get install clang
$ sudo apt-get install libcurl4 libpython2.7 libpython2.7-dev
```
If you're using Debian 10 or higher (and you really should be), you'll also need to backdate a few libraries to get older versions:
```
$ sudo apt-get install libtinfo5 libncurses5
```
Afteward you can download and install Swift:
```
$ wget https://swift.org/builds/swift-5.1.3-release/ubuntu1804/swift-5.1.3-RELEASE/swift-5.1.3-RELEASE-ubuntu18.04.tar.gz
$ tar xzfv swift-5.1.3-RELEASE-ubuntu18.04.tar.gz 
$ sudo mv swift-5.1.3-RELEASE-ubuntu18.04 /usr/share/swift
```
To be able to use your new Swift setup, you need to update your `PATH` in your `.bashrc`:
```
$ echo "export PATH=/usr/share/swift/usr/bin:$PATH" >> ~/.bashrc
$ source ~/.bashrc
```
You can now test Swift out with the `--version` argument:
```
$ swift --version
Swift version 5.1.3 (swift-5.1.3-RELEASE)
Target: x86_64-unknown-linux-gnu
```

### Creating a Project
Once you've installed Swift on your Ubuntu machine, you can create projects with the `package init` command:
```
$ mkdir swift-project
$ cd swift-project/
/swift-project$ swift package init --type executable
Creating executable package: swift-project
Creating Package.swift
Creating README.md
Creating .gitignore
Creating Sources/
Creating Sources/swift-project/main.swift
Creating Tests/
Creating Tests/LinuxMain.swift
Creating Tests/swift-projectTests/
Creating Tests/swift-projectTests/swift_projectTests.swift
Creating Tests/swift-projectTests/XCTestManifests.swift
```
You'll then edit `Sources/.../main.swift` and when you're ready to compile, you can use the `build` command:
```
$ swift build
[4/4] Linking swift-project
```
Finally, you'll be able to run the program from the `.build/debug` directory:
```
$ .build/debug/swift-project
Hello, world!
```
Good luck!

