# 4.3 Creating a Raw Transaction with Named Arguments

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

It can sometimes be daunting to figure out the right order for the arguments to a bitcoin-cli command. Fortunately, you can use _named arguments_ as an alternative.

> **VERSION WARNING:** This is an innovation from Bitcoin Core v 0.14.0. If you used our setup scripts, that's what you should have, but double-check your version if you have any problems.

## Create a Named Argument Alias

To use a named argument you must run `bitcoin-cli` with the `-named` argument. If you plan to do this regularly, you'll probably want to create an alias:
```
alias bitcoin-cli="bitcoin-cli -named"
```
As usual, that's for your ease of use, but we'll continue using the whole commands, to maintain clarity.

## Test Out a Named Argument

To learn what the names are for the arguments of a command, just look at `bitcoin-cli help`. It will list the arguments in their proper order, but will now also give names for each of them.

For example, `bitcoin-cli getbalance` lists the following arguments:

   1. account
   2. minconf
   3. include watchonly
   
The following shows a traditional, unintuitive usage of `getbalance` with `minconf`:
```
$ bitcoin-cli getbalance "*" 1
```
With named arguments you can now call it like this, which improves clarities and minimizes mistakes:
```
$ bitcoin-cli -named getbalance account="*" minconf=1
```

## Test Out a Raw Transaction

Here's what all the commands for sending a raw transaction would look like with named arguments:
