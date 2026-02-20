# Interlude: Using Command-Line Variables

This chapter demonstrated a number of command-line commands used without obfuscation or interference. However, that's often not the best way to run Bitcoin from the command line. Because you're dealing with long, complex, non-human-readable variables, it's easy to make a mistake if you're copying those variables around (or, satoshi forfend, if you're typing them in by hand). Because those variables can mean the difference between receiving and losing real money, you don't _want_ to make mistakes. For these reasons, we strongly suggest using command-line variables to save addresses, signatures, or other long strings of information whenever it's reasonable to do so.

If you're using `bash`, you can save information to a variable like this:
```
$ VARIABLE=$(command)
```
That's a simple command substitution, the equivalent to ``VARIABLE=`command` ``. The command inside the parentheses is run, then assigned to the VARIABLE.

To create a new address would then look like this:
```
$ NEW_ADDRESS_1=$(bitcoin-cli getnewaddress)
```
This commands fills the NEW_ADDRESS_1 variable with the results of the `bitcoin-cli getnewaddress` command.

You can then use your shell's `echo` command to look at your (new) address:
```
$ echo $NEW_ADDRESS_1
tb1qhed3ausdjr6crntypcqjv6l2n2vtv2qv7dednu
```
You can then use this variable in other commands which require the address (or do similar for any other complex output). The rest of this tutorial will use this style of saving information to variables when it's practical.

> :book: ***When is it not practical to use command-line variables?*** Command-line variables aren't practical if you need to use the information somewhere other than on the command line. For example, saving an address may not actually be useful if you're just going to have to send it to someone else in an email. In addition, some future commands will output JSON objects instead of simple information, and variables can't be used to capture that information ... at least not without a _little_ more work.

## Summary: Using Command-Line Variables

Shell variables can be used to hold long Bitcoin strings, minimizing the chances of mistakes.

## What's Next?

Continue on to [Chapter Four: Sending Bitcoin Transactions](04_0_Sending_Bitcoin_Transactions.md).
