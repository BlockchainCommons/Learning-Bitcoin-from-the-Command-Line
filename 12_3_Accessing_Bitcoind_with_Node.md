# 12.3: Accessing Bitcoind with Node

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Accessing the `bitcoind` doesn't require `curl`. All you need is simple RPC functionality. That's what's provided by [BCRPC](https://github.com/dgarage/bcrpc) by kallewoof of Digital Garage.

## Setting Up Node.js

BCRPC is built on node.js. Thus, you'll first need to install the `node.js` and `npm` (node package manager) packages for your system. 

If you're using a Ubuntu machine, you can run the following commands to get a new version of `node.js` (as opposed to the horribly out-of-date version in the Ubuntu package system).
```
curl -sL https://deb.nodesource.com/setup_8.x | sudo -E bash -
sudo apt-get install -y nodejs
sudo apt-get install mocha -g
```
### Setting Up BCRPC

You should next download BCRPC from the [BCRPC Repository](https://github.com/dgarage/bcrpc). Clone it or download it, as you prefer.

Once you've done that, you can finish setting it up:
```
$ unzip bcrpc-master.zip
$ cd bcrpc-master
$ npm install
```

### Testing BCRPC

