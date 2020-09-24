# 17.6: Accessing Bitcoind with Swift

> :information_source: **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

This section explains how to interact with `bitcoind` using the Swift programming language.

## Setting Up Swift on Your Mac

To date, you've built all of your alternative programming language development environments on your Debian virtual node. However, that's not the best platform for Swift. Though there is a version of Swift available for Ubuntu platforms, it's not fully featured, and it works somewhat differently from the Mac-native Swift. A "Variant" at the bottom of this chapter explains how to set it up, but be warned that you'll be in uncharted territory.

As for setting up Swift on your Mac, it's pretty easy. Generally, you'll just call up the App Store and install "Xcode" on your Mac. But, if you prefer

1. Call up the App Store and Select Xcode

Still on Mojave?
[may be safer methodology too]

Download the xip for 10.3.1:
https://developer.apple.com/download/more/

Click on XIP to extract
Move to Apps

2. Install GordianServer

[...]

3. Find GordianServer Info

Click on gear, should show data directory

By default:  Library/Application Support/Bitcoin/
bitcoin.conf

rpcuser=oIjA53JC2u
rpcpassword=ebVCeSyyM0LurvgQyi0exWTqm4oU0rZU


## Variant: Deploying Swift on Ubuntu

If you prefer to deploy Swift on Ubuntu, you can do so, though the functionality isn't the same. Some of the code in this chapter will likely generate errors that you'll need to resolve, and you'll also need to do more work to link in libraries.

To get started, install some required libraries:
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
For things to work, you should update your `PATH` in your `.bashrc`:
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

