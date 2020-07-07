# 18.2: Accessing Bitcoind with Java

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Interacting with the `bitcoind` directly and using command-line `curl` can get simple if you understand how it works, but there's a project [JavaBitcoindRpcClient](https://github.com/Polve/JavaBitcoindRpcClient) that provides the functionality in a Java-API level, making it even easier to interact with your Bitcoin Server.


## Setup Java

To install Java on the VPS Server, you are able to use the `apt-get` command. We will also use [Apache Maven](http://maven.apache.org/) to manage the dependencies, so we will install it together.  In this project we will create a maven project and indicate minimum configuration about Gradle (https://gradle.org/releases/)

```
$ apt-get install openjdk-11-jre-headless maven
```

You can verify your Java installation:
```
$ java -version
openjdk version "11.0.7" 2020-04-14
OpenJDK Runtime Environment (build 11.0.7+10-post-Ubuntu-2ubuntu218.04)
OpenJDK 64-Bit Server VM (build 11.0.7+10-post-Ubuntu-2ubuntu218.04, mixed mode, sharing)
```

## Create maven project

```
mvn archetype:generate -DgroupId=com.blockchaincommons.lbtc -DartifactId=java-project -DarchetypeArtifactId=maven-archetype-quickstart -DinteractiveMode=false
```
It will download some dependencies

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
It will create a configuration file pom.xml

```
user@machine:~/BitcoinRpcClient/java-project$ ll
total 16
drwxr-xr-x 3 user user 4096 Jun 17 16:47 ./
drwxr-xr-x 3 user user 4096 Jun 17 15:02 ../
-rw-r--r-- 1 user user 1175 Jun 17 16:34 pom.xml
drwxr-xr-x 4 user user 4096 Jun 17 15:02 src/
user@machine:~/BitcoinRpcClient/java-project$ 
```

This project uses JavaBitcoindRpcClient, so you need include the dependency editing pom.xml file

```xml
<dependency>
  <groupId>wf.bitcoin</groupId>
  <artifactId>JavaBitcoindRpcClient</artifactId>
  <version>1.1.0</version>
</dependency>
```
You need to add compiler properties to indicate what JDK version will compile the source code.    

```
    <properties>
	<!-- https://maven.apache.org/general.html#encoding-warning -->
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
        <maven.compiler.source>1.11</maven.compiler.source>
        <maven.compiler.target>1.11</maven.compiler.target>
    </properties>
```
Finally add source code to java classes and execute

```
user@machine:~/BitcoinRpcClient/java-project$ mvn package
[INFO] Scanning for projects...
[INFO]
[INFO] ------------------< com.blockchaincommons.lbtc:java-project >-------------------
[INFO] Building java-project 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
......
-------------------------------------------------------
 T E S T S
-------------------------------------------------------
Running com.blockchaincommons.lbtc.AppTest
Tests run: 2, Failures: 0, Errors: 0, Skipped: 0, Time elapsed: 0.067 sec

Results :

Tests run: 2, Failures: 0, Errors: 0, Skipped: 0

[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ java-project ---
[INFO] Building jar: /home/user/BitcoinRpcClient/java-project/target/java-project-1.0-SNAPSHOT.jar
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time: 1.956 s
[INFO] Finished at: 2020-06-1716T12:21:18+31:00
[INFO] ------------------------------------------------------------------------```

```
Or if you use Gradle:
```groovy
compile 'wf.bitcoin:JavaBitcoindRpcClient:1.1.0'
```

If you want a sample project and some instructions on how to run it on the server that we just created, you can refer to the [Bitcoind Java Sample Project](https://github.com/brunocvcunha/bitcoind-java-client-sample/).

## Build Your Connection

### Make an RPC Call

To use `JavaBitcoindRpcClient`, you need to create a `BitcoindRpcClient` instance. The arguments in the URL are username, password, IP address and port. You should know this information from your work with `curl` . As you'll recall, the IP address 127.0.0.1 and port 18332 should be correct for the standard testnet setup described in this documents, while you can extract the user and password from `~/.bitcoin/bitcoin.conf`.

```java
  BitcoindRpcClient rpcClient = new BitcoinJSONRPCClient("http://bitcoinrpc:d8340efbcd34e312044c8431c59c792c@127.0.0.1:18332");
```

> **MAINNET VS TESTNET:** The port would be 8332 for a mainnet setup.

If `rpcClient` is successfully initialized, you'll be able to send off RPC commands.

Later, when you're all done with your `bitcoind` connection, you should close it:
```
rpcClient.stop();
```

### Making your first RPC Call

In order to use an RPC method using `JavaBitcoindRpcClient`, you'll find that the `BitcoindRpcClient` provides most of the functionality that can be accessed through `bitcoin-cli` or `curl`, using the same method names. For more details about the commands that you are able to execute and what to expect back, you should refer to [3.2: Knowing Your Bitcoin Setup](03_2_Knowing_Your_Bitcoin_Setup.md).

For example, to execute the `getmininginfo` command to get the block information and the difficulty on the network, you should use the `getMiningInfo()` method:
```java
MiningInfo info = rpcClient.getMiningInfo();
System.out.println("Mining Information");
System.out.println("------------------");
System.out.println("Chain......: " + info.chain());
System.out.println("Blocks.....: " + info.blocks());
System.out.println("Difficulty.: " + info.difficulty());
System.out.println("Hash Power.: " + new BigDecimal(info.networkHashps()).toPlainString());
```
The output for this line should be similar to this:
```
Mining Information
------------------
Chain......: test
Blocks.....: 1254920
Difficulty.: 1.0
Hash Power.: 6585163152453.466796875
```
### Making an RPC Call with Arguments

When you make a RPC call with arguments you need to setup parameters depending method or type of object you use.   

### Look up Address

You can look up addresses on your wallet passing it as an argument.   In this case we use getAddressInfo method to obtain some information about an address.

```java
public AddressInfo getAddressInfo(String address) throws GenericRpcException {
    return new AddressInfoMapWrapper((Map<String, ?>) query("getaddressinfo", address));
  }

String addr1 = "bcrt1qs4ylwj2v5v0gq7eqzp9k9vxazdrkexhkghxpyp";   //regtest
AddressInfo addr1Info = rpcClient.getAddressInfo(addr1);
System.out.println("Address: " + addr1Info.address());
System.out.println("HdKeyPath: " + addr1Info.hdKeyPath());
System.out.println("PubKey: " + addr1Info.pubKey());
System.out.println("MasterFingerPrint: " + addr1Info.hdMasterFingerprint());
```
Output:
```
Jun 19, 2020 8:01:56 PM wf.bitcoin.javabitcoindrpcclient.BitcoinJSONRPCClient <clinit>
WARNING: Currently relying on rpcuser / rpcpassword for authentication. This will soon be deprecated in bitcoind. To use newer auth mechanism based on a temporary password, remove properties rpcuser / rpcpassword from bitcoin.conf
Address: bcrt1qs4ylwj2v5v0gq7eqzp9k9vxazdrkexhkghxpyp
HdKeyPath: m/0'/0'/16'
PubKey: 03cf852403abbcf0431e8c82b414b0c805f5e1b863989cbc9adb3a316510e0d1f5
MasterFingerPrint: 91cfb0fc

```

### Look up Funds

You can look up your balance.

```java
public BigDecimal getBalance(String account) throws GenericRpcException {
    return (BigDecimal) query("getbalance");
  }
  String balance = rpcClient.getBalance(account);
  System.out.println("Balance: " + balance);
  
```
Output: 

```
Balance: 14701.56249600
```

### Creating an Address

You can create a new address on your wallet attaching a specific label to it, as well as dump the private key for a specific address.
For more information about the wallet setup, you can check [3.3: Setting Up Your Wallet](03_3_Setting_Up_Your_Wallet.md).

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

### Create a Transaction

You can create a raw transaction using createRawTransaction method passing as arguments two ArrayList objects containing inputs and outputs to be used.  In this example we will create two addresses and we will use generateToAddress method in regtest to mine some bitcoin.

```java
public String create() throws GenericRpcException {
 return bitcoin.createRawTransaction(new ArrayList<>(inputs), outputs);
}
```

#### generatetoaddress

The generatetoaddress RPC mines blocks immediately to a specified address.  It receives numBlocks param as the number of blocks to generate.  The address to send the newly generated bitcoin to and param maxTries as the maximum number of iterations that are tried to create the requested number of blocks.

We use generateToAddress method to mine some bitcoin and we use listUnspent (https://github.com/Polve/bitcoin-rpc-client/blob/master/src/main/java/wf/bitcoin/javabitcoindrpcclient/BitcoinJSONRPCClient.java#L756) method to load object utxos with coins associated with address object addr1.

```java
System.out.println("Created address addr1: " + addr1);
String addr2 = rpcClient.getNewAddress();
System.out.println("Created address addr2: " + addr2);
List<String> generatedBlocksHashes = rpcClient.generateToAddress(110, addr1);
System.out.println("Generated " + generatedBlocksHashes.size() + " blocks for addr1");
List<Unspent> utxos = rpcClient.listUnspent(0, Integer.MAX_VALUE, addr1);
System.out.println("Found " + utxos.size() + " UTXOs (unspent transaction outputs) belonging to addr1");
```

debug.log
```
2020-06-19T18:22:30Z [default wallet] AddToWallet 0bed0fc1b6190b85b93bda6fe752c7596234bea8399827d27e347f35ca68d59f  new
2020-06-19T18:22:30Z CreateNewBlock(): block weight: 892 txs: 0 fees: 0 sigops 400
2020-06-19T18:22:30Z UpdateTip: new best=6125a1648f84e11d9d8ee1b003056c20142e9f1e54376f5d117554785957aadf height=1100 version=0x20000000 log2_work=11.104599 tx=1103 date='2020-06-19T18:22:48Z' progress=1.000000 cache=0.0MiB(114txo)
2020-06-19T18:22:30Z [default wallet] AddToWallet a2690eb4c50b1140dfc77f95db9c8065e8d7e88b2cfbd9a75e9b2dd157857afd  new
```
Output
```
Created address addr1: bcrt1qs4ylwj2v5v0gq7eqzp9k9vxazdrkexhkghxpyp
Created address addr2: bcrt1qdp6fut9pmchwacpr28vfszdp5qayza8jkq5t3v
Generated 110 blocks for addr1
Found 118 UTXOs (unspent transaction outputs) belonging to addr1
```
Now we have created UXTO's we can create a transaction,  to perform this we will use  three objects,  TxInput, TxOutput and Transactions Builder.    With this code we got inputs and outputs for our transaction.   Object uxto is a list with all UXTO's belonging to addr1.    We will choose uxto in position zero on the list and add it to txb object as input.   Then we add addr2 object as the output and set fee subtracting estimatedFee value.

```java
BitcoinRawTxBuilder txb = new BitcoinRawTxBuilder(rpcClient);
BigDecimal estimatedFee = BigDecimal.valueOf(0.00000200);
TxInput in = utxos.get(0);
txb.in(in);
		
txToAddr2Amount = utxos.get(0).amount().subtract(estimatedFee); 
txb.out(addr2, txToAddr2Amount);
		
System.out.println("unsignedRawTx in amount: " + utxos.get(0).amount());
System.out.println("unsignedRawTx out amount: " + txToAddr2Amount);
```

Output
```
unsignedRawTx in amount: 0.78125000
unsignedRawTx out amount: 0.78124800
```

### Sending Transactions

Before send a transaction we need to create and sign it.   To create you can use create method of Builder Transaction object.   This method returns a unsigned string transaction in hexadecimal format.

```java
String unsignedRawTxHex = txb.create();
System.out.println("Created unsignedRawTx from addr1 to addr2: " + unsignedRawTxHex);
```
Output:
```
Created unsignedRawTx from addr1 to addr2: 020000000101f08cabf817b8fb076501f04b69df0aae59d61d94edfb40c2c41b1b2bd16a3f0000000000ffffffff010017a8040000000016001468749e2ca1de2eeee02351d89809a1a03a4174f200000000
```
Later you should sign transaction with method signRawTransactionWithKey.   This method receives as parameters a unsigned raw string transaction, the private key of address and TxInput object.

```java
SignedRawTransaction srTx = rpcClient.signRawTransactionWithKey(
				unsignedRawTxHex,
				Arrays.asList(rpcClient.dumpPrivKey(addr1)), // 
				Arrays.asList(in),
				null);
System.out.println("signedRawTx hex: " + srTx.hex());
System.out.println("signedRawTx complete: " + srTx.complete());
```
Output
```
signedRawTx hex: 0200000000010101f08cabf817b8fb076501f04b69df0aae59d61d94edfb40c2c41b1b2bd16a3f0000000000ffffffff010017a8040000000016001468749e2ca1de2eeee02351d89809a1a03a4174f20247304402204ed1ce8ea7e36cd53ba78beaccaf3ef62b094c29413a451e3abae99548520f7f02206b606c21cd38cc4e61d84c229d42ce69a01cb3a0ed360fced2a6f5b5d8dbe951012103cf852403abbcf0431e8c82b414b0c805f5e1b863989cbc9adb3a316510e0d1f500000000
signedRawTx complete: true
```

Finally you can send signed transaction using 

```java
String sentRawTransactionID = rpcClient.sendRawTransaction(srTx.hex());
System.out.println("Sent signedRawTx (txID): " + sentRawTransactionID);```
```
This program will output a transaction id:

```
Sent signedRawTx (txID): 03b2327117264837f449a718e5aeedb07f90d435892a33c3c2772d4c3b40111f
```
debug.log.

```
2020-06-19T18:22:31Z [default wallet] AddToWallet 03b2327117264837f449a718e5aeedb07f90d435892a33c3c2772d4c3b40111f  new
```

For more information about sending transactions, you can check [4: Sending Bitcoin Transactions](04_0_Sending_Bitcoin_Transactions.md).

### Listening to Transactions or Blocks

You may want to write applications that keep listening the Blockchain, and execute a specific code when something happens, such as a transaction that involves an address in your wallet, or even the generation of a new block in the network.
To do that, `JavaBitcoindRpcClient` provides support to `BitcoinAcceptor`, where you can attach listeners in the network.

Example:
```java
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

Every time some transaction is sent, or a new block is generated, you should see a similar output in your console:
```
Transaction: {account=Tests, address=mhopuJzgmTwhGfpNLCJ9CRknugY691oXp1, category=receive, amount=5.0E-4, label=Tests, vout=1, confirmations=0, trusted=false, txid=361e8fcff243b74ebf396e595a007636654f67c3c7b55fd2860a3d37772155eb, walletconflicts=[], time=1513132887, timereceived=1513132887, bip125-replaceable=unknown}

Block: 000000004564adfee3738314549f7ca35d96c4da0afc6b232183917086b6d971
```

### For More Information

In this repository you can browse all souce code for bitcoin-rpc-client (https://github.com/Polve/bitcoin-rpc-client)

### Summary Accessing Bitcoind with Java

By using javabitcoinrpc library, you can easily access bitcoind via RPC calls from a Java. To do so, you should create a client, an RPC connection, then create objects and classes to interact with individual RPC calls, some of them with parameters. You can easily create address, sign transactions, query balances and others RPC methods.
