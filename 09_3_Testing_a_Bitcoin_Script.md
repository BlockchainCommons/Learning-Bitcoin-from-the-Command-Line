# 9.3: Testing a Bitcoin Script

Bitcoin Scripting allows for considerable additional control over Bitcoin transactions, but it's also somewhat dangerous. As we'll describe in [§10.1](10_1_Understanding_the_Foundation_of_P2SH.md), the actual Scripts are somewhat isolated from the Bitcoin network, which means that it's possible to write a script and have it accepted by the network even if it's impossible to redeem from that script! So, you need to thoroughly test your Scripts before you put your money into them.

This chapter thus describes a prime method for testing Bitcoin Scripts, which we'll also be using for occasional examples throughout the rest of this section.

## Install btcdeb

The Bitcoin Script Debugger (`btcdeb`) by @kallewoof is one of the most reliable methods we've found for debugging Bitcoin Scripts. It does, however, require setting up C++ and a few other accessories on your machine, so we'll also offer a few other options toward the end of this chapter.

First, you need to clone the `btcdeb` GitHub repository, which will also require installing `git` if you don't yet have it.
```
$ sudo apt-get install git
$ git clone https://github.com/bitcoin-core/btcdeb.git
```
Note that when you run `git clone` it will copy `btcdeb` into your current directory. We've chosen to do so in our `~standup` directory.
```
$ ls
bitcoin-0.20.0-x86_64-linux-gnu.tar.gz  btcdeb  laanwj-releases.asc  SHA256SUMS.asc
```
Afterward, you must install required C++ and other packages.
```
$ sudo apt-get install autoconf libtool g++ pkg-config make
```
You should also install readline, as this makes the debugger a lot easier to use by supporting history using up/down arrows, left-right movement, autocompletion using tab, and other good user interfaces. 
```
$ sudo apt-get install libreadline-dev
```
You're now ready to compile and install `btcdeb`:
```
$ cd btcdeb
$ ./autogen.sh
$ ./configure
$ make
$ sudo make install
```
After all of that, you should have a copy of `btcdeb`:
```
$ which btcdeb
/usr/local/bin/btcdeb
```

## Use btcdeb

`btcdeb` works like a standard debugger. It takes a script (as well as any number of stack entries) as a startup argument. You then can `step` through the script.

If you instead start it up with no arguments, you simply get an interpreter where you may issue `exec [opcode]` commands to perform actions directly.

### Use btcdeb for an Addition Example

The following example shows the use of `btcdeb` for the addition example from the previous section, `1 2 OP_ADD`
```
$ btcdeb '[1 2 OP_ADD]' 
btcdeb 0.2.19 -- type `btcdeb -h` for start up options
warning: ambiguous input 1 is interpreted as a numeric value; use OP_1 to force into opcode
warning: ambiguous input 2 is interpreted as a numeric value; use OP_2 to force into opcode
miniscript failed to parse script; miniscript support disabled
valid script
3 op script loaded. type `help` for usage information
script  |  stack 
--------+--------
1       | 
2       | 
OP_ADD  | 
#0000 1
```
It shows our initial script, running top to bottom, and also shows what will be executed next in the script.

We type `step` and it advances one step by taking the first item in the script and pushing it onto the stack:
```
btcdeb> step
		<> PUSH stack 01
script  |  stack 
--------+--------
2       | 01
OP_ADD  | 
#0001 2
```
And again:
```
btcdeb> step
		<> PUSH stack 02
script  |  stack 
--------+--------
OP_ADD  |      02
        | 01
#0002 OP_ADD
```
Now we execute the the `OP_ADD` and there's great excitement because that opcode pops the first two items off the stack, adds them together, then pushes their sum onto the stack.
```
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 03
script  |  stack 
--------+--------
        | 03
```
And that's where our script ends, with nothing more to execute and a `03` sitting on top of our stack as the result of the Script.

> **NOTE:** `btcdeb` allows you to repeat the previous command by hitting enter. We will be doing this in subsequent examples, so don't be surprised about `btcdeb>` prompts with nothing as input. It is simply repeating the previous (often `step`) command.

### Use btcdeb for a Subtraction Example

The previous section also included a slightly more complex subtraction example of Scripting: `3 2 OP_ADD 4 OP_SUB`. Here's what that looks like:

```
$ btcdeb '[3 2 OP_ADD 4 OP_SUB]'
btcdeb 0.2.19 -- type `btcdeb -h` for start up options
warning: ambiguous input 3 is interpreted as a numeric value; use OP_3 to force into opcode
warning: ambiguous input 2 is interpreted as a numeric value; use OP_2 to force into opcode
warning: ambiguous input 4 is interpreted as a numeric value; use OP_4 to force into opcode
miniscript failed to parse script; miniscript support disabled
valid script
5 op script loaded. type `help` for usage information
script  |  stack 
--------+--------
3       | 
2       | 
OP_ADD  | 
4       | 
OP_SUB  | 
#0000 3
btcdeb> step
		<> PUSH stack 03
script  |  stack 
--------+--------
2       | 03
OP_ADD  | 
4       | 
OP_SUB  | 
#0001 2
btcdeb> 
		<> PUSH stack 02
script  |  stack 
--------+--------
OP_ADD  |      02
4       | 03
OP_SUB  | 
#0002 OP_ADD
btcdeb> 
		<> POP  stack
		<> POP  stack
		<> PUSH stack 05
script  |  stack 
--------+--------
4       | 05
OP_SUB  | 
#0003 4
btcdeb> 
		<> PUSH stack 04
script  |  stack 
--------+--------
OP_SUB  |      04
        | 05
#0004 OP_SUB
btcdeb> 
		<> POP  stack
		<> POP  stack
		<> PUSH stack 01
script  |  stack 
--------+--------
        | 01
```
We'll be returning to `btcdeb` from time to time, and it will remain an excellent tool for testing your own scripts.

### Use the Power of btcdeb

`btcdeb` also has a few more powerful functions, such as `print` and `stack`, which show you the script and the stack at any time.

For example, in the above script, once you've advanced to the `OP_ADD` command, you can see the following:
```
btcdeb> print
    #0000 3
    #0001 2
 -> #0002 OP_ADD
    #0003 4
    #0004 OP_SUB
btcdeb> stack
<01>	02	(top)
<02>	03
```
Using these commands can make it easier to see what's going on and where you are.

## Test a Script Online

There are also a few web simulators that you can use to test scripts online. They can be superior to a command-line tool by offering a more graphical output, but we also find that they tend to have shortcomings.

In the past we've tried to give extensive guidelines on using sites such as the [Script Playground](http://www.crmarsh.com/script-playground/) or the [Bitcoin Online Script Debugger](https://bitcoin-script-debugger.visvirial.com/), but they become out of date and/or disappeared too quickly to keep up with them.

Assume that these debuggers have the nice advantage of showing things visually and explicitly telling you whether a script succeeds (unlocks) or fails (stays locked). Assume that they have disadvantages with signatures, where many of them either always return `true` for signature tests or else have very cumbersome mechanisms for incorporating them.

## Test a Script with Bitcoin

Even with a great tool like `btcdeb` or transient resources like the various online script testers, you're not working with the real thing. You can't guarantee that they follow Bitcoin's consensus rules, which means you can't guarantee their results. For example, the Script Playground explicitly says that it ignores a bug that's implicit in Bitcoin multisignatures. This means that any multisig code that you successfully test on the Script Playground will break in the real world.

So the only way to _really_ test Bitcoin Scripts is to try them out on Testnet.

And how do you do that? As it happens that's the topic of [chapter 10](10_0_Embedding_Bitcoin_Scripts_in_P2SH_Transactions.md), which looks into introducing these abstract scripts to the real world of Bitcoin by embedding them in P2SH transactions. (But even then, you will probably need an API to push your P2SH transaction onto the Bitcoin network, so full testing will still be a ways in the future.)

_Whatever_ other testing methods you've used, testing a script on Testnet should be your final test _before_ you put your Script on Mainnet. Don't trust that your code is right; don't just eyeball it. Don't even trust whatever simulators or debuggers you've been using. Doing so is another great way to lose funds on Bitcoin.

## Summary: Testing a Bitcoin Script

You should install `btcdeb` as a command-line tool to test out your Bitcoin Scripts. As of this writing, it produces accurate results that can step through the entire scripting process. You can also look at some online sites for a more visual representation. When you're all done, you're going to need to go to testnet to make sure things are working accurately, before you deploy more generally.

## What's Next?

Continue "Introducing Bitcoin Scripts" with our first real-life example: [§9.4: Scripting a P2PKH](09_4_Scripting_a_P2PKH.md).
