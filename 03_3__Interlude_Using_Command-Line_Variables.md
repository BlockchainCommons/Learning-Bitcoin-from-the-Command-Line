# Interlude: Using Command-Line Variables

> :information_source: **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

The previous section demonstrated a number of command-line commands used without obfuscation or interference. However, that's often not the best way to run Bitcoin from the command line. Because you're dealing with long, complex, and unreadable variables, it's easy to make a mistake if you're copying those variables around (or, satoshi forfend, if you're typing them in by hand). Because those variables can mean the difference between receiving and losing real money, you don't _want_ to make mistakes. For these reasons, we strongly suggest using command-line variables to save addresses, signatures, or other long strings of information whenever it's reasonable to do so.

If you're using `bash`, you can save information to a variable like this:
```
$ VARIABLE=$(command)
```
That's a simple command substitution, the equivalent to ``VARIABLE=`command` ``. The command inside the parentheses is run, then assigned to the VARIABLE.

To create a new address would then look like this:
```
$ unset NEW_ADDRESS_1
$ NEW_ADDRESS_1=$(bitcoin-cli getnewaddress "" legacy)
```
These commands clear the NEW_ADDRESS_1 variable, just to be sure, then fill it with the results of the `bitcoin-cli getnewaddress` command.

You can then use your shell's `echo` command to look at your (new) address:
```
$ echo $NEW_ADDRESS_1
mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE
```
Because you have your address in a variable, you can now easily sign a message for that address, without worrying about mistyping the address. You'll of course save that signature into a variable too!
```
$ NEW_SIG_1=$(bitcoin-cli signmessage $NEW_ADDRESS_1 "Hello, World")
$ echo $NEW_SIG_1
IPYIzgj+Rg4bxDwCyoPiFiNNcxWHYxgVcklhmN8aB2XRRJqV731Xu9XkfZ6oxj+QGCRmTe80X81EpXtmGUpXOM4=
```
The rest of this tutorial will use this style of saving information to variables when it's practical.

> :book: ***When is it not practical to use command-line variables?*** Command-line variables aren't practical if you need to use the information somewhere other than on the command line. For example, saving your signature may not actually be useful if you're just going to have to send it to someone else in an email. In addition, some future commands will output JSON objects instead of simple information, and variables can't be used to capture that information ... at least not without a _little_ more work.

A quick productivity trick using shebangs or `!!`

You will commonly find yourself experimenting with commands, adding a quote here, escaping a special character there. Most commands will not correctly execute the first time, so it will be an iterative process. 

Once you find the right command and wish to chain it to another command, the use of `!!` will be very helpfull.

```
$ bitcoin-cli listunspent | jq -r '.[0] | txid' # bad syntax for jq - missing . to access txid
jq: error: txid/0 is not defined at <top-level>, line 1:
.[0] | txid
jq: 1 compile error
$  bitcoin-cli listunspent | jq -r '.[0] | .txid' 
95fc0f7def910f3569598a0188047c21d0a0b7082447d4c8ea680230e1958a45
$ bitcoin-cli $(!!) #!! will expand to the previous command
$ bitcoin-cli $(bitcoin-cli listunspent | jq -r '.[0] | .txid')
{
  "amount": 0.00999890,
  "confirmations": 291,
  "blockhash": "0000000000000136f7d9a85b9d1d851e1e227d9e73a102dcf1a4ede85792006f",
  "blockheight": 1807512,
  "blockindex": 45,
  "blocktime": 1597615054,
  "txid": "95fc0f7def910f3569598a0188047c21d0a0b7082447d4c8ea680230e1958a45",
  "walletconflicts": [
  ],
  "time": 1597614258,
  "timereceived": 1597614258,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "tb1qfurf3wlee5xtt2jagudc59ly80uqsgzhcfj75w",
      "category": "receive",
      "amount": 0.00999890,
      "label": "-addresstype",
      "vout": 0
    }
  ],
  "hex": "020000000001010a3064498faf474b881a2693e21f061be5d33397eaed67af335e420bb375a95b01000000000100000001d2410f00000000001600144f0698bbf9cd0cb5aa5d471b8a17e43bf80820570247304402205414305ed6f7934f2d7f143b33938fa4b8882d20f30ce594f2ab60a8249003b7022060152efe2c9a1fc3cf62bcbb33708d46d1c2c6be4db0ff3e503473701af65445012102827c1fa77b51645884fd6797ed2fd2c6a603c275054b0f99115f25729a1d165c00000000"
}
```

Another common use case of `!!` is when you are trying to access a resource but dont have the right permissions.

```
$ apt install cowsay # apt needs sudo 
$ sudo !! # expands
$ sudo apt install cowsay
```


## Summary: Using Command-Line Variables

Shell variables can be used to hold long Bitcoin strings, minimizing the chances of mistakes.

## What's Next?

Continue "Understanding Your Bitcoin Setup" with [§3.4: Receiving a Transaction](03_4_Receiving_a_Transaction.md).
