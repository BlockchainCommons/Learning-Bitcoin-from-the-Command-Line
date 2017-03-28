# Interlude: Using Command-Line Variables 

The previous section demonstrated a number of command-line commands used without obfuscation or interference. However, that's often not the best way to run Bitcoin from the command line. Because you're dealing with long, complex, and unreadable variables, it's easy to make a mistake if you're copying those variables around (or, satoshi forfend, if you're typing them in by hand). Because those variables can mean the difference between receiving and losing real money, you don't _want_ to make mistakes. For these reasons, we suggest using command-line variables to save addresses, signatures, or other long strings of information whenever it's reasonable to do so.

If you're using `bash`, you can save information to a variable like this:
```
$ VARIABLE=$(command)
```
That's a simple command substitution, and the equivalent to `VARIABLE=\`command\``. The command inside the parentheses is run, then assigned to the VARIABLE.

To create a new address would then look like this:
```
$ unset NEW_ADDRESS_1
$ NEW_ADDRESS_1=$(bitcoin-cli getnewaddress)
```
These commands clear the NEW_ADDRESS_1 variable, just to be sure, then fill it with the results of the `bitcoin-cli getnewaddress` command.

You can then use your shell's `echo` command to look at your (new) address:
```
$ echo $NEW_ADDRESS_1
n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf
```

You could similarly save a signature like this:
```
$ NEW_SIG_1=$(bitcoin-cli signmessage $NEW_ADDRESS_1 "Hello, World")
```
Just put _everything_ inside the parentheses!

The rest of this tutorial will use this style of saving information to variables when it's practical.

(When is it not practical? Saving your signature may not actually be practical if you're just going to have to send it to someone else, not use it yourself. In addition, in the future, some commands will output JSON objects instead of just a line of information, and variables can't be used to capture that information ... though there are alternatives.)
