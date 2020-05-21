# AFAIRE Interlude : Utilisation des variables de la ligne de commande

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
$ NEW_ADDRESS_1=$(bitcoin-cli getnewaddress)
```
These commands clear the NEW_ADDRESS_1 variable, just to be sure, then fill it with the results of the `bitcoin-cli getnewaddress` command.

You can then use your shell's `echo` command to look at your (new) address:
```
$ echo $NEW_ADDRESS_1
n4cqjJE6fqcmeWpftygwPoKMMDva6BpyHf
```
Because you have your address in a variable, you can now easily sign a message for that address, without worrying about mistyping the address. You'll of course save that signature into a variable too!
```
$ NEW_SIG_1=$(bitcoin-cli signmessage $NEW_ADDRESS_1 "Hello, World")
```
The rest of this tutorial will use this style of saving information to variables when it's practical.

_When is it not practical to use command-line variables?_ Command-line variables aren't practical if you need to use the information somewhere other than on the command line. For example, saving your signature may not actually be useful if you're just going to have to send it to someone else in an email. In addition, some future commands will output JSON objects instead of simple information, and variables can't be used to capture that information ... at least not without a _little_ more work.

## Summary: Using Command-Line Variables

Shell variables can be used to hold long Bitcoin strings, minimizing the chances of mistakes.

## What's Next?

Continue "Understanding Your Bitcoin Setup" with [§3.4: Receiving a Transaction](03_4_Receiving_a_Transaction.md).
