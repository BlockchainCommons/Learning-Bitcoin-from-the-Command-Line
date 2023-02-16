# 18.6: Accessing Bitcoind with Swift

> :information_source: **NOTE:** This section has been recently added to the course and is an early draft that may still be awaiting review. Caveat reader.

This section explains how to interact with `bitcoind` using the Swift programming language and your own RPC client.

## Set Up Swift on Your Mac

To date, you've built all of your alternative-programming-language development environments on your Debian virtual node. However, that's not the best platform for Swift. Though there is a version of Swift available for Ubuntu platforms, it's not fully featured, and it works somewhat differently from the Mac-native Swift. A "variant" at the bottom of this section explains how to set it up, but be warned that you'll be in uncharted territory.

Instead, we suggest creating an optimal Swift environment on a Mac. There are four major steps in doing so.

### 1. Install Xcode

You're going to need `Xcode`, the integrated development enviroment for Swift and Objective-C. That can be easily installed by going to the Mac App Store and `Get`ting Xcode.

#### Alternative: Install by Hand

Some people advise against an App Store install because it's somewhat all-or-nothing; it also won't work if you're still using Mojave because you want to avoid Catalina's incompatibilities. In that case you can download directly from the [Developer Area](https://developer.apple.com/download/more/) at Apple.

If you're using Mojave, you'll need the `xip` file for Xcode 10.3.1. Otherwise, get the newest one.

Once it's downloaded, you can click on the `xip` to extract it, then move the Xcode app to your Applications folder.

(Either way, you should have Xcode installed in your Applications folder at the end of this step.)

### 2. Install the Gordian Server

You're also going to need a Bitcoin node on your Mac, so that you can communicate with it. Technically, you could use a remote node and access it with the RPC login and password over the net. However, we suggest you instead install a full node directly on your Mac, because that's the safest and cleanest setup, ensuring that none of your communications leave your machine.

To easily install a full node on your Mac, use Blockchain Commons' [GordianServer for MacOS](https://github.com/BlockchainCommons/GordianServer-macOS). See the [installation instructions](https://github.com/BlockchainCommons/GordianServer-macOS#installation-instructions) in the README, but generally all you have to do is download the current `dmg` file, open it, and install that app in your Applications directory too.

Afterward, run the GordianServer App, and tell it to `Start` Testnet. 

> :link: **TESTNET vs. MAINNET:** Or `Start` Mainnet.

#### 3. Make Your Gordian bitcoin-cli Accessible

When you want to access the `bitcoin-cli` created by GordianServer on your local Mac, you can find it at  `~/.standup/BitcoinCore/bitcoin-VERSION/bin/bitcoin-cli`, for example  `~/.standup/BitcoinCore/bitcoin-0.20.1/bin/bitcoin-cli`. 

You may wish to create an alias for that:
```
alias bitcoin-cli="~/.standup/BitcoinCore/bitcoin-0.20.1/bin/bitcoin-cli -testnet"
```

> :link: **TESTNET vs. MAINNET:** Obviously, the `-testnet` parameter is only required if you're running on testnet.

### 4. Find Your GordianServer Info

Finally, you'll need your `rpcuser` and `rpcpassword` information. That's in `~/Library/Application Support/Bitcoin/bitcoin.conf` by default under Gordian.
```
$ grep rpc ~/Library/Application\ Support/Bitcoin/bitcoin.conf
rpcuser=oIjA53JC2u
rpcpassword=ebVCeSyyM0LurvgQyi0exWTqm4oU0rZU
...
```
## Build Your Connection by Hand

At the time of this writing, there isn't an up-to-date, simple-to-use Bitcoin RPC Library that's specific for Swift, something that you can drop in and immediately start using. Thus, you're're going to do something you're never done before: build an RPC connection by hand.

### Write the RPC Transmitter

This just requires writing a function that passes RPC commands on to `bitcoind` in the correct format:
```
func makeCommand(method: String, param: Any, completionHandler: @escaping (Any?) -> Void) -> Void {
```

RPC connections to `bitcoind` use the HTML protocol, which means that you need to do three things: create a URL; make a URLRequest; and initiate a URLSession.

#### 1. Create a URL

Within the function, you need to create a URL from your IP, port, `rpcuser`, `rpcpassword`, and wallet:
```
    let testnetRpcPort = "18332"
    let nodeIp = "127.0.0.1:\(testnetRpcPort)"
    let rpcusername = "oIjA53JC2u"
    let rpcpassword = "ebVCeSyyM0LurvgQyi0exWTqm4oU0rZU"
    let walletName = ""
```
The actual RPC connection to Bitcoin Core is built using a URL of the format "http://rpcusername:rpcpassword@nodeIp/walletName":
```
    let walletUrl = "http://\(rpcusername):\(rpcpassword)@\(nodeIp)/\(walletName)"

    let url = URL(string: walletUrl)
```
This means that your sample variables result in the following URL:
```
http://oIjA53JC2u:ebVCeSyyM0LurvgQyi0exWTqm4oU0rZU@127.0.0.1:18332/
```
Which should look a lot like the URL used in some of the previous sections for RPC connections.

#### 2. Create a URLRequest

With that URL in you hand, you can now create a URLRequest, with the `POST` method and the `text/plain` content type. The HTTP body is then the familiar JSON object that you've been sending whenever you connect directly to Bitcoin Core's RPC ports, as first demonstrated when using Curl in [§4.4](04_4__Interlude_Using_Curl.md).
```
    var request = URLRequest(url: url!)
    request.httpMethod = "POST"
    request.setValue("text/plain", forHTTPHeaderField: "Content-Type")
    request.httpBody = "{\"jsonrpc\":\"1.0\",\"id\":\"curltest\",\"method\":\"\(method)\",\"params\":[\(param)]}".data(using: .utf8)
```

#### 3. Create a URLSession

Finally, you're ready to build a URLSession around your URLRequest.
```
    let session = URLSession(configuration: .default)
    let task = session.dataTask(with: request as URLRequest) { data, response, error in
```
The completion handler for `dataTask` needs to check for errors:
```
        do {

            if error != nil {

                    //Handle the error
                    
            } else {
```
And then parse the data that you're receiving. Here, you're pulling the JSON results into an `NSDictionary`:
```
                if let urlContent = data {
                        
                    do {
                            
                        let json = try JSONSerialization.jsonObject(with: urlContent, options: JSONSerialization.ReadingOptions.mutableLeaves) as! NSDictionary
```
After that, there's more error handling and more error handling and then you can eventually return the dictionary `result` using the `completionHandler` that you defined for the new `makeCommand` function:
```
                        if let errorCheck = json["error"] as? NSDictionary {
                                                                
                            if let errorMessage = errorCheck["message"] as? String {

                                print("FAILED")
                                print(errorMessage)

                            }
                                
                        } else {
                                
                            let result = json["result"]
                            completionHandler(result)
                            
                        }
                            
                    } catch {
                            
                            //Handle error here
                            
                    }
```
Of course you eventually have to tell the `task` to start:
```
    task.resume()
```
And that's "all" there is to doing that RPC interaction by hand using a programming language such as Swift.

> :pray: **THANKS:** Thanks to @Fonta1n3 who provided the [main code](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/issues/137) for our RPC Transmitter.

### Make An RPC Call

Having written the `makeCommand` RPC function, you can send an RPC call by running it. Here's `getblockchaininfo`:
```
let method = "getblockchaininfo"
let param = ""

makeCommand(method: method,param: param) { result in
    
    print(result!)

}
```
### Make an RPC Call with Arguments

You could similarly grab the current block count from that info and use that to (reduntantly) get the hash of the current block, by using the `param` parameter:
```
let method = "getblockchaininfo"
let param = ""

makeCommand(method: method,param: param) { result in
    
    let blockinfo = result as! NSDictionary
    let block = blockinfo["blocks"] as! NSNumber
    
    let method = "getblockhash"
    makeCommand(method: method,param: block) { result in
        print("Blockhash for \(block) is \(result!)")
    }
    
}
```

### Run Your Code

The complete code is available in the [src directory](src/18_6_getinfo.playground). Load it into your Xcode playground and then "Editor -> Run Playground" and you should get results like:
```
{
    bestblockhash = 00000000000000069725608ebc5b59e520572a8088cbc57ffa5ba87b7f300ac7;
    blocks = 1836745;
    chain = test;
    chainwork = 0000000000000000000000000000000000000000000001cc3e9f8e0bc6b71196;
    difficulty = "16508683.81195478";
    headers = 1836745;
    initialblockdownload = 0;
    mediantime = 1601416765;
    pruned = 0;
    "size_on_disk" = 28205538354;
    softforks =     {
        bip34 =         {
            active = 1;
            height = 21111;
            type = buried;
        };
        bip65 =         {
            active = 1;
            height = 581885;
            type = buried;
        };
        bip66 =         {
            active = 1;
            height = 330776;
            type = buried;
        };
        csv =         {
            active = 1;
            height = 770112;
            type = buried;
        };
        segwit =         {
            active = 1;
            height = 834624;
            type = buried;
        };
    };
    verificationprogress = "0.999999907191804";
    warnings = "Warning: unknown new rules activated (versionbit 28)";
}
Blockhash for 1836745 is 00000000000000069725608ebc5b59e520572a8088cbc57ffa5ba87b7f300ac7
```

## Look Up Funds

With your new `makeCommand` for RPC functions, you can similarly run a command like `getwalletinfo` or `getbalance`:
```
var method = "getwalletinfo"
var param = ""

makeCommand(method: method,param: param) { result in
    
    print(result!)
    
}

method = "getbalance"
makeCommand(method: method,param: param) { result in

    let balance = result as! NSNumber
    print("Balance is \(balance)")

}
```
Which returns:
```
Balance is 0.01
{
    "avoid_reuse" = 0;
    balance = "0.01";
    hdseedid = bf493318f548df8e25c390d6a7f70758fd6b3668;
    "immature_balance" = 0;
    keypoololdest = 1599723938;
    keypoolsize = 999;
    "keypoolsize_hd_internal" = 1000;
    paytxfee = 0;
    "private_keys_enabled" = 1;
    scanning = 0;
    txcount = 1;
    "unconfirmed_balance" = 0;
    walletname = "";
    walletversion = 169900;
}
```
## Create an Address

Creating an address is simple enough, but what about creating a legacy address with a specific label? That requires two parameters in your RPC call.

Since the simplistic `makeCommand` function in this section just passes on its `param`s as the guts of a JSON Object, all you have to do is correctly format those guts. Here's one way to do so:
```
method = "getnewaddress"
param = "\"learning-bitcoin\", \"legacy\""

makeCommand(method: method,param: param) { result in

    let address = result as! NSString
    print(address)
}
```
Running this in the Xcode playground produces a result:
```
mt3ZRsmXHVMMqYQPJ8M74QjF78bmqrdHZF
```
That result is obviously a Legacy address; its label can then be checked from the command line:
```
$ bitcoin-cli getaddressesbylabel "learning-bitcoin"
{
  "mt3ZRsmXHVMMqYQPJ8M74QjF78bmqrdHZF": {
    "purpose": "receive"
  }
}
```
Success!

> :information_source: **NOTE:** As we often say in these coding examples, a real-world program would be much more sophisticated. In particular, you'd want to be able to send an actual JSON Object as a parameter, and then have your `makeCommand` program parse it and input it to the URLSession appropriately. What we have here maximizes readability and simplicity without focusing on ease of use.

## Send a Transaction

As usual, sending a transaction (the hard way) is a multi-step process:

0. Generate or receive a receiving address
1. Find an unspent UTXO
2. Create a raw transaction
3. Sign the raw transaction
4. Send the raw transaction

You'll use the `address` that you generated in the previous step as your recipient.

### 1. Find an Unspent UTXO

The `listunspent` RPC lets you find your UTXO:
```
    method = "listunspent"
    param = ""
    
    makeCommand(method: method,param: param) { result in

        let unspent = result as! NSArray
        let utxo = unspent[0] as! NSDictionary
        
        let txid = utxo["txid"] as! NSString
        let vout = utxo["vout"] as! NSInteger
        let amount = utxo["amount"] as! NSNumber
        let new_amount = amount.floatValue - 0.0001
```
As in other examples, you're going to arbitrarily grab the 0th UTXO, and pull the `txid`, `vout`, and `amount` from it.

> :information_source **NOTE:** Once again, a real-life program would be much more sophisticated.

### 2. Create a Raw Transaction

Creating a raw transaction is the trickiest thing because you need to get all of your JSON objects, arrays, and quotes right. Here's how to do so in Swift, using the transmitter's very basic `param` formatting:
```
        method = "createrawtransaction"
        param="[ { \"txid\": \"\(txid)\", \"vout\": \(vout) } ], { \"\(address)\": \(new_amount)}"
        makeCommand(method: method,param: param) { result in

            let hex = result as! NSString
```
### 3. Sign the Raw Transaction

Signing your transaction just requires you to run the `signrawtransactionwithwallet` RPC, using your new `hex`:
```
            method = "signrawtransactionwithwallet"
            param = "\"\(hex)\""
            
            makeCommand(method: method,param: param) { result in

                let signedhexinfo = result as! NSDictionary
                let signedhex = signedhexinfo["hex"] as! NSString
```

### 4. Send the Raw Transaction

Sending your transaction is equally simple:
```
                method = "sendrawtransaction"
                param = "\"\(signedhex)\""

                makeCommand(method: method,param: param) { result in

                    let new_txid = result as! NSString
                    print("TXID: \(new_txid)")
                    
                }
            }         
        }
    }
}
```

The code for this transaction sender can be found in the [src directory](src/18_6_sendtx.playground).

## Use Swift in Other Ways

That covers our usual discussions of programming Bitcoin RPC in a language, but Swift is a particularly important language since it can be deployed on mobile devices, one of the prime venues for wallets. As such, you may wish to consider a few other libraries:

* The Blockchain Commons [ios-Bitcoin framework](https://github.com/BlockchainCommons/iOS-Bitcoin) converts the Libbitcoin library from C++ to Swift
* [Libwally Swift](https://github.com/blockchain/libwally-swift) is a Swift wrapper for Libwally

## Summary: Accessing Bitcoind with Swift

Swift is a robust modern programming language that unfortunately doesn't yet have any easy-to-use RPC libraries ... which just gave us the opportunity to write an RPC-access function of our own. With that in hand, you can interact with `bitcoind` on a Mac or build companion applications over on an iPhone, which is a perfect combination for airgapped Bitcoin work.

## What's Next?

Learn about Lightning in [Chapter 19: Understanding Your Lightning Setup](19_0_Understanding_Your_Lightning_Setup.md).

## Variant: Deploy Swift on Ubuntu

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

### Create a Project
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

