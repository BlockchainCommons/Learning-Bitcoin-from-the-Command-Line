# 18.2: Accessing Bitcoind with Java

> :information_source: **NOTE:** This section has been recently added to the course and is an early draft that may still be awaiting review. Caveat reader.

This section explains how to interact with `bitcoind` using the Java programming language and the [JavaBitcoindRpcClient](https://github.com/Polve/JavaBitcoindRpcClient).

## Set Up Java

You can install Java on your server, using the `apt-get` command. You will also install [Apache Maven](http://maven.apache.org/) to manage the dependencies.
```
$ sudo apt-get install openjdk-11-jre-headless maven
```

You can verify your Java installation:
```
$ java -version
openjdk version "11.0.8" 2020-07-14
OpenJDK Runtime Environment (build 11.0.8+10-post-Debian-1deb10u1)
OpenJDK 64-Bit Server VM (build 11.0.8+10-post-Debian-1deb10u1, mixed mode, sharing)
```

### Create a Maven Project

In order to program with Bitcoin using java, you will create a Maven project:
```
$ mvn archetype:generate -DgroupId=com.blockchaincommons.lbtc -DartifactId=java-project -DarchetypeArtifactId=maven-archetype-quickstart -DinteractiveMode=false
```
This will download some dependencies
```
Downloading: https://repo.maven.apache.org/maven2/org/apache/maven/plugins/maven-clean-plugin/2.5/maven-clean-plugin-2.5.pom
Downloaded: https://repo.maven.apache.org/maven2/org/apache/maven/plugins/maven-clean-plugin/2.5/maven-clean-plugin-2.5.pom (4 KB at 4.2 KB/sec)
Downloading: https://repo.maven.apache.org/maven2/org/apache/maven/plugins/maven-plugins/22/maven-plugins-22.pom
Downloaded: https://repo.maven.apache.org/maven2/org/apache/maven/plugins/maven-plugins/22/maven-plugins-22.pom (13 KB at 385.9 KB/sec)
Downloading: https://repo.maven.apache.org/maven2/org/apache/maven/maven-parent/21/maven-parent-21.pom
Downloaded: https://repo.maven.apache.org/maven2/org/apache/maven/maven-parent/21/maven-parent-21.pom (26 KB at 559.6 KB/sec)
Downloading: https://repo.maven.apache.org/maven2/org/apache/apache/10/apache-10.pom
..............
```
It will also create a configuration file `pom.xml`:

```
$ cd java-project
$ ls -lagh
total 16K
drwxr-xr-x  3 sudo 4.0K Sep  1 13:58 .
drwxr-xr-x 15 sudo 4.0K Sep  1 13:58 ..
-rw-r--r--  1 sudo  663 Sep  1 13:58 pom.xml
drwxr-xr-x  4 sudo 4.0K Sep  1 13:58 src
```

In order to include `JavaBitcoindRpcClient`, you must add its dependency to `<dependendencies>` in the `pom.xml` file

```xml
      <dependency>
        <groupId>wf.bitcoin</groupId>
        <artifactId>bitcoin-rpc-client</artifactId>
        <version>1.2.1</version>
      </dependency>
```
You also need to add compiler properties to indicate what JDK version will compile the source code.

```
  <properties>
    <!-- https://maven.apache.org/general.html#encoding-warning -->
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
    <maven.compiler.source>11</maven.compiler.source>
    <maven.compiler.target>11</maven.compiler.target>
  </properties>
```

Whenever you add source code to your classes, you'll be able to test it with:
```
$ mvn compile
```
You can also execute with `exec:java`
```
$ mvn exec:java -Dexec.mainClass=com.blockchaincommons.lbtc.App
```

### Create Alternative Projects

If you use [Gradle]((https://gradle.org/releases/), you can instead run:
```groovy
compile 'wf.bitcoin:JavaBitcoindRpcClient:1.2.1'
```

If you want a sample project and some instructions on how to run it on the server that you just created, you can refer to the [Bitcoind Java Sample Project](https://github.com/brunocvcunha/bitcoind-java-client-sample/). You can also browse all souce code for bitcoin-rpc-client (https://github.com/Polve/bitcoin-rpc-client).

## Build Your Connection

To use `JavaBitcoindRpcClient`, you need to create a `BitcoindRpcClient` instance. You do this by creating a URL with arguments of username, password, IP address and port. As you'll recall, the IP address `127.0.0.1` and port `18332` should be correct for the standard testnet setup described in this course, while you can extract the user and password from `~/.bitcoin/bitcoin.conf`.

```java
       BitcoindRpcClient rpcClient = new BitcoinJSONRPCClient("http://StandUp:6305f1b2dbb3bc5a16cd0f4aac7e1eba@localhost:18332");
```
Note that you'll also need to import the appropriate information:
```
import wf.bitcoin.javabitcoindrpcclient.BitcoinJSONRPCClient;
import wf.bitcoin.javabitcoindrpcclient.BitcoindRpcClient;
```

> **MAINNET VS TESTNET:** The port would be 8332 for a mainnet setup.

If `rpcClient` is successfully initialized, you'll be able to send RPC commands.

Later, when you're all done with your `bitcoind` connection, you should close it:
```
rpcClient.stop();
```

### Make an RPC Call

You'll find that the `BitcoindRpcClient` provides most of the functionality that can be accessed through `bitcoin-cli` or other RPC methods, using the same method names, but in camelCase.

For example, to execute the `getmininginfo` command to get the block information and the difficulty on the network, you should use the `getMiningInfo()` method:
```java
MiningInfo info = rpcClient.getMiningInfo();
System.out.println("Mining Information");
System.out.println("------------------");
System.out.println("Chain......: " + info.chain());
System.out.println("Blocks.....: " + info.blocks());
System.out.println("Difficulty.: " + info.difficulty());
System.out.println("Hash Power.: " + info.networkHashps());
```
The output for this line should be similar to this:
```
Mining Information
------------------
Chain......: test
Blocks.....: 1830905
Difficulty.: 4194304
Hash Power.: 40367401348837.41
```
### Make an RPC Call with Arguments

You can look up addresses on your wallet by passing the address as an argument to `getAddressInfo`:

```java
	String addr1 = "mvLyH7Rs45c16FG2dfV7uuTKV6pL92kWxo";

	AddressInfo addr1Info = rpcClient.getAddressInfo(addr1);
	System.out.println("Address: " + addr1Info.address());
	System.out.println("MasterFingerPrint: " + addr1Info.hdMasterFingerprint());
	System.out.println("HdKeyPath: " + addr1Info.hdKeyPath());
	System.out.println("PubKey: " + addr1Info.pubKey());
```
The output will look something like this:
```
Address: mvLyH7Rs45c16FG2dfV7uuTKV6pL92kWxo
MasterFingerPrint: ce0c7e14
HdKeyPath: m/0'/0'/5'
PubKey: 0368d0fffa651783524f8b934d24d03b32bf8ff2c0808943a556b3d74b2e5c7d65
```

### Run Your Code

The code for these examples can be found in [the src directory](src/18_2_App-getinfo.java) and should be installed into the standard directory structure created here as `~/java-project/src/main/java/com/blockchaincommons/lbtc/App.java`. It can then be compiled and run.

```
$ mvn compile
$ mvn exec:java -Dexec.mainClass=com.blockchaincommons.lbtc.App
Chain......: test
Blocks.....: 1831079
Difficulty.: 4194304
Hash Power.: 38112849943221.16
Address: mvLyH7Rs45c16FG2dfV7uuTKV6pL92kWxo
MasterFingerPrint: ce0c7e14
HdKeyPath: m/0'/0'/5'
PubKey: 0368d0fffa651783524f8b934d24d03b32bf8ff2c0808943a556b3d74b2e5c7d65
```
(You'll also see lots more information about the compilation, of course.)

## Look up Funds

Retrieving the balance for a whole account is equally easy:
```
        System.out.println("Balance: " + rpcClient.getBalance());
```

## Create an Address

You can create a new address on your wallet, attach a specific label to it, and even dump its private key.

```java
  String address = rpcClient.getNewAddress("Learning-Bitcoin-from-the-Command-Line");
  System.out.println("New Address: " + address);

  String privKey = rpcClient.dumpPrivKey(address);
  System.out.println("Priv Key: " + privKey);
```
Output:
```
New Address: mpsFtZ8qTJPRGZy1gaaUw37fHeUSPLkzzs
Priv Key: cTy2AnmAALsHokYzJzTdsUBSqBtypmWfmSNYgG6qQH43euUZgqic
```

## Send a Transaction

The JavaBitcoindRpcClient library has some good tools that make it easy to create a transaction from scratch.

### Create a Transaction

You can create a raw transaction using the `createRawTransaction` method, passing as arguments two ArrayList objects containing inputs and outputs to be used.

First you set up your new addresses, here an existing address on your system and a new address on your system.
```
        String addr1 = "tb1qdqkc3430rexxlgnma6p7clly33s6jjgay5q8np";
        System.out.println("Used address addr1: " + addr1);

        String addr2 = rpcClient.getNewAddress();
        System.out.println("Created address addr2: " + addr2);
```
Then, you can use the `listUnspent` RPC to find UTXOs for the existing address.
```
        List<Unspent> utxos = rpcClient.listUnspent(0, Integer.MAX_VALUE, addr1);
        System.out.println("Found " + utxos.size() + " UTXOs (unspent transaction outputs) belonging to addr1");
```
Here's an output of all the information:
```java
System.out.println("Created address addr1: " + addr1);
String addr2 = rpcClient.getNewAddress();
System.out.println("Created address addr2: " + addr2);
List<String> generatedBlocksHashes = rpcClient.generateToAddress(110, addr1);
System.out.println("Generated " + generatedBlocksHashes.size() + " blocks for addr1");
List<Unspent> utxos = rpcClient.listUnspent(0, Integer.MAX_VALUE, addr1);
System.out.println("Found " + utxos.size() + " UTXOs (unspent transaction outputs) belonging to addr1");
```

Transactions are built with `BitcoinRawTxBuilder`:
```
        BitcoinRawTxBuilder txb = new BitcoinRawTxBuilder(rpcClient);
```
First you fill the inputs with the UTXOs you're spending:
```
        TxInput in = utxos.get(0);
        txb.in(in);
```

> :warning: **WARNING:** Obviously in a real program you'd intelligently select a UTXO; here, we just grab the 0th one, a tactic that we'll use throughout this chapter.

Second, you fill the ouputs each with an amount and an address:
```
	BigDecimal estimatedFee = BigDecimal.valueOf(0.00000200);
      	BigDecimal txToAddr2Amount = utxos.get(0).amount().subtract(estimatedFee);
	txb.out(addr2, txToAddr2Amount);

	System.out.println("unsignedRawTx in amount: " + utxos.get(0).amount());
        System.out.println("unsignedRawTx out amount: " + txToAddr2Amount);
```

You're now ready to actually create the transaction:
```
	String unsignedRawTxHex = txb.create();
	System.out.println("Created unsignedRawTx from addr1 to addr2: " + unsignedRawTxHex);
```

### Sign a Transactions

You now can sign transaction with the method `signRawTransactionWithKey`. This method receives as parameters an unsigned raw string transaction, the private key of the sending address, and the TxInput object.

```java
	SignedRawTransaction srTx = rpcClient.signRawTransactionWithKey(
					unsignedRawTxHex,
					Arrays.asList(rpcClient.dumpPrivKey(addr1)), //
					Arrays.asList(in),
					null);
	System.out.println("signedRawTx hex: " + srTx.hex());
	System.out.println("signedRawTx complete: " + srTx.complete());
```

### Send a Transactiong

Finally, sending requires the `sendRawTransaction` command:
```java
String sentRawTransactionID = rpcClient.sendRawTransaction(srTx.hex());
System.out.println("Sent signedRawTx (txID): " + sentRawTransactionID);
```

### Run Your Code

You can now run [the transaction code](src/18_2_App-sendtx.java) as `~/java-project/src/main/java/com/blockchaincommons/lbtc/App.java`.

```
$ mvn compile
$ mvn exec:java -Dexec.mainClass=com.blockchaincommons.lbtc.App
Used address addr1: tb1qdqkc3430rexxlgnma6p7clly33s6jjgay5q8np
Created address addr2: tb1q04q2wzlhfqlrnz95ynfj7gp4t3yynrj0542smv
Found 1 UTXOs (unspent transaction outputs) belonging to addr1
unsignedRawTx in amount: 0.00850000
unsignedRawTx out amount: 0.00849800
Created unsignedRawTx from addr1 to addr2: 0200000001d2a90fc3b43e8eb4ae9452af43c9448112d359cac701f7f537aa8b6f39193bb90100000000ffffffff0188f70c00000000001600147d40a70bf7483e3988b424d32f20355c48498e4f00000000
signedRawTx hex: 02000000000101d2a90fc3b43e8eb4ae9452af43c9448112d359cac701f7f537aa8b6f39193bb90100000000ffffffff0188f70c00000000001600147d40a70bf7483e3988b424d32f20355c48498e4f024730440220495fb64d8cf9dee9daa8535b8867709ac8d3763d693fd8c9111ce610645c76c90220286f39a626a940c3d9f8614524d67dd6594d9ee93818927df4698c1c8b8f622d01210333877967ac52c0d0ec96aca446ceb3f51863de906e702584cc4da2780d360aae00000000
signedRawTx complete: true
Sent signedRawTx (txID): 82032c07e0ed91780c3369a1943ea8abf49c9e11855ffedd935374ecbc789c45
```

## Listen to Transactions or Blocks

As with [C and its ZMQ libraries](15_3_Receiving_Bitcoind_Notifications_with_C.md), there are easy ways to use Java to listen to the blockchain — and to execute specific code when something happens, such as a transaction that involves an address in your wallet, or even the generation of a new block in the network.

To do this, use `JavaBitcoindRpcClient`'s `BitcoinAcceptor` class, which allows you to attach listeners in the network.

```java
        String blockHash = rpcClient.getBestBlockHash();
	BitcoinAcceptor acceptor = new BitcoinAcceptor(rpcClient, blockHash, 6, new BitcoinPaymentListener() {

	      @Override
	      public void transaction(Transaction tx) {
	      	System.out.println("Transaction: " + tx);
	      }

	      @Override
	      public void block(String block) {
	      	System.out.println("Block: " + block);
	      }
	});
  acceptor.run();
```

See [the src directory](src/18_2_App-listen.java) for the complete code. Every time a transaction is sent or a new block is generated, you should see output on your console:
```
Transaction: {account=Tests, address=mhopuJzgmTwhGfpNLCJ9CRknugY691oXp1, category=receive, amount=5.0E-4, label=Tests, vout=1, confirmations=0, trusted=false, txid=361e8fcff243b74ebf396e595a007636654f67c3c7b55fd2860a3d37772155eb, walletconflicts=[], time=1513132887, timereceived=1513132887, bip125-replaceable=unknown}

Block: 000000004564adfee3738314549f7ca35d96c4da0afc6b232183917086b6d971
```

### Summary Accessing Bitcoind with Java

By using the javabitcoinrpc library, you can easily access bitcoind via RPC calls from Java. You'll also have access to nice additional features, like the `bitcoinAcceptor` listening service.

## What's Next?

Learn more about "Talking to Bitcoin in Other Languages" in [18.3: Accessing Bitcoin with NodeJS](18_3_Accessing_Bitcoind_with_NodeJS.md).

