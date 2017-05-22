> **WARNING:** The lack of any code Script interpretors is currently halting this section.

# 8.3: Testing a Bitcoin Script

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Bitcoin Scripting allows for considerable new control over Bitcoin transactions, but it's all somewhat abstract. How do you actually turn that theoretical Bitcoin Script into functional Bitcoin code? The first answer is to test it ... but unfortunately that answer isn't as easy as it seems.

## Get the Code Right

The object of testing your Bitcoin Scripts is to get the code right. Don't worry about the whole rigamarole of P2SH scripting (yet). Just worry about producing the Bitcoin Script code itself and making sure it runs and does what you expect.

This is a critically important aspect of any Bitcoin Scripting, because there's _no other_ check on your Script. Bitcoin will often protect you from trying to send bad transactions, but it _won't_ protect you from writing bad locking scripts. That's because the P2SH methodology actually keeps Bitcoin from knowing about the actual Script until it tries to unlock it ... and if your script doesn't run at that point, your funds are gone.

So the onus is on you to get that code right.

## Test a Script Online

It would seem like it should be easy to test a Bitcoin Script online in a web simulator. Unfortunately, as of the time of this writing, online simulators are few, and the ones that exist are almost fatally buggy.

### The Script Playground

Charlie Marsh has built an excellent [Script Playground](http://www.crmarsh.com/script-playground/). Just put together your unlocking script and your locking script and run them. The Playground was run an `OP_VERIFY` at the end, and tell you with a green checkmark or a red X whether the transaction was unlocked or not.

Type in the test from [§7.2: Running a Bitcoin Script.md](7_2_Running_a_Bitcoin_Script.md) of `1 98 OP_ADD 99 OP_EQUAL` and watch it verify; change it to `1 97 OP_ADD 99 OP_EQUAL` and see it fail.

#### Bugs & Challenges

_Signature Problems._ To really test Scripts you need to be able to verify hashes and signatures. The Playground tried to make that easy with buttons that can be used to cut-and-paste public keys and signatures, but they haven't kept up with Flash security ugprades and no longer work reliably. This makes any testing extremely cumbersome, as you have to dump addresses, public keys, and private keys from `bitcoin-cli`. Signing is simplified by just checking the signature of a nonce of "Secure", but given that `bitcoin-cli` produces a base64 signature instead of a hex signature, you're deep in the weeds. As a result of all of this, unless you're deeply comfortable with Bitcoin signatures, you're unlikely to be able to use Script Playground for anything but tests of flow control.

_Number Problems_. The Script Playground doesn't recognize numbers of 100 or more.

_If the signature buttons worked right, the Playground would be great, but until that happens, it's not very useful._

### Web BTC Script Debugger

WebBTC has a [Script Debugger](https://webbtc.com/script) that not only shows you the execution of a script, but also explains its parts and shows you the stack. The following example is similar to the add-to-99 script of previous chapters: [Add to 15 Script](https://webbtc.com/script/1%2014/OP_ADD%2015%20OP_EQUAL/).

#### Bugs & Challenges

_Web Problems._ The web submit form no longer works, requiring you to type all script into the URL in the form `https://webbtc.com/script/[unlocking-script]/[locking-script]`. Each script is a series of operators separated by `%20`s. For example `1%2014` is `1 14` and `OP_ADD%2015%20OP_EQUAL` is `OP_ADD 15 OP_EQUAL`.

_Signature Problems._ Allegedly, all signatures are assumed valid if a scripthash is not provided. This does not seem to be the case. Examining [existing transactions](https://webbtc.com/script/d0d92c5ae1e70d9825ddb32cb08b51ef2bac443d84cbb100380e8a1e659f6964:0) demonstrates how their signatures were processed, but checking examples does not work, again majorly impacting the utility of this site.

_Number Problems_. The BTC debugger doesn't recognize numbers of 17 or more. (This is not unusual: 1 to 16 become OP_1 to OP_16, but beyond that, extra work is required for numbers, work that you're usually insulated from.)

_If signatures were indeed considered valid, this would be a great resource, but between that not working and having to annoyingly type in code as a URL, this is another online script tester that's barely usable currently. The one saving grace is that transactions can be examined down to the Bitcoin script ... but the site hasn't been recording new transactions since December 2016.

## Test a Script with Java

## Test a Script with Bitcoin

## Summary: Testing a Bitcoin Script
