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

#### 2.1 Make Your Gordian bitcoin-cli Accessible

Note that when you want to access the `bitcoin-cli` on your local Mac, you can find it at  `~/.standup/BitcoinCore/bitcoin-VERSION/bin/bitcoin-cli`, for example  `~/.standup/BitcoinCore/bitcoin-0.20.1/bin/bitcoin-cli`. 

You may wish to create an alias for that:
```
alias bitcoin-cli="~/.standup/BitcoinCore/bitcoin-0.20.1/bin/bitcoin-cli -testnet"
```

> :link: **TESTNET vs. MAINNET:** Obviously, the `-testnet` parameter is only required if you're running on testnet.

### 3. Find Your GordianServer Info

As usual, you'll need the RPC login and password. That's in `~/Library/Application Support/Bitcoin/bitcoin.conf` by default under Gordian.

Pull out the `rpcuser` and `rpcpassword` information; you'll need it to build your RPC connection:
```
$ grep rpc ~/Library/Application\ Support/Bitcoin/bitcoin.conf
rpcuser=oIjA53JC2u
rpcpassword=ebVCeSyyM0LurvgQyi0exWTqm4oU0rZU
...
```
## Building Your Connection by Hand

At the time of this writing, there isn't a simple and easy Bitcoin RPC Library for Swift, which you can drop in and immediately start using. Thus, we're going to do something we've never done before: build an RPC connection by hand.

It just requires writing a function:
```
func makeCommand(method: String, param: Any, completionHandler: @escaping (Any?) -> Void) -> Void {
```

### 1. Create a URL

Within the funciton, you need to create a URL from your IP, port, `rpcuser`, `rpcpassword`, and wallet:
```
    let testnetRpcPort = "18332"
    let nodeIp = "127.0.0.1:\(testnetRpcPort)"
    let rpcusername = "oIjA53JC2u"
    let rpcpassword = "ebVCeSyyM0LurvgQyi0exWTqm4oU0rZU"
    let walletName = ""
    let walletUrl = "http://\(rpcusername):\(rpcpassword)@\(nodeIp)/\(walletName)"

    let url = URL(string: walletUrl)
```
An RPC connection to Bitcoin Core is built using a URL of the format "http://\(rpcusername):\(rpcpassword)@\(nodeIp)/\(walletName)". This means that your sample variables result in the following URL:
```
http://oIjA53JC2u:ebVCeSyyM0LurvgQyi0exWTqm4oU0rZU@127.0.0.1:18332/
```

### 2. Create a URLRequest

With that URL in you hand, you can now create a URLRequest, with the `POST` method and the `text/plain` content type. The HTTP body is then the familiar JSON object that you've been sending whenever you connect directly to Bitcoin Core's RPC ports, as first demonstrated when using Curl in [§4.4](04_4__Interlude_Using_Curl.md).
```
    var request = URLRequest(url: url!)
    request.httpMethod = "POST"
    request.setValue("text/plain", forHTTPHeaderField: "Content-Type")
    request.httpBody = "{\"jsonrpc\":\"1.0\",\"id\":\"curltest\",\"method\":\"\(method)\",\"params\":[\(param)]}".data(using: .utf8)
```

### 3. Create a URLSession

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
And then parse the data. Here, you're pulling the JSON results into an `NSDictionary`:
```
                if let urlContent = data {
                        
                    do {
                            
                        let json = try JSONSerialization.jsonObject(with: urlContent, options: JSONSerialization.ReadingOptions.mutableLeaves) as! NSDictionary
```
After that, there's more error handling and more error handling and then you can eventually return the JSON `result` using the `completionHandler` that you included for the new `makeCommand` function:
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
And that's "all" there is to doing that RPC interaction by hand using a program language such as Swift.

### Making An RPC Call

Having written the `makeCommand` RPC function, you can send an RPC call by running it. Here's `getblockchaininfo`:
```
let method = "getblockchaininfo"
// Your rpc commands parameters (none needed for getblockchaininfo)
let param = ""

makeCommand(method: method,param: param) { result in
    
    print(result!)

}
```
### Making an RPC Call with Arguments

You could similarly grab the current block count from that info and use it to get the hash of the current block, by using the `param` parameter:
```
let method = "getblockchaininfo"
// Your rpc commands parameters (none needed for getblockchaininfo)
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

### Running Your Code

The complete code is available in the [src directory](src/17_6_getinfo.playground). Load it into your Xcode playground and then "Editor -> Run Playground" and you should get results like:
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

## Looking Up Funds

With your new `makeCommand` for RPC functions, you can equally run a command like `getwalletinfo` or `getbalance`:
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
## Creating an Address

Creating an address is simple enough, but what about creating a legacy address with a specific label? That requires two parameters in your RPC call.

Since the simplistic `makeCommand` function in this section just passes on its `param`s as the guts of a JSON object, all you have to do is correctly format those guts. Here's one way to do so:
```
method = "getnewaddress"
param = "\"learning-bitcoin\", \"legacy\""

makeCommand(method: method,param: param) { result in

    let address = result as! String
    print(address!)
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

> :information_source: **NOTE:** As we often say in these coding examples, a real program would be much more sophisticated. In particular, you'd want to be able to send an actual JSON object as a parameter, and then have your `makeCommand` program parse it and input it to the URLSession appropriately.







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

