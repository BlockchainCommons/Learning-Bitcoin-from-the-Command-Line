> **WARNING:** This section is of limited utility due to a lack of good test sites and test suites.

# 8.5: Testing a Bitcoin Script

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

Bitcoin Scripting allows for considerable additional control over Bitcoin transactions, but it's also somewhat dangerous. As long as you produce a legal P2SH lock, your transaction will be accepted ... but that's no guarantee that the associated Bitcoin Script with either run or produce a `True` result. So, you need to thoroughly test your Scripts before you put your money into that script.

## Test a Script Online

Web simulators seem like they should be a great option for Bitcoin Script testing. Unfortunately, at the time of this writing, online simulators are few, and the ones that exist are almost fatally buggy and also out-of-date for recent features such as CSV. A few are included here mainly for completeness sake: because they used to work pretty well and perhaps might again. Someday. For now, they're not particularly recommended though.

### The Script Playground

Charlie Marsh has built an extensive [Script Playground](http://www.crmarsh.com/script-playground/). Just put together your unlocking script and your locking script and run them. The Playground will play an `OP_VERIFY` at the end, and tell you with a green checkmark or a red X whether the transaction was unlocked or not.

Type in the test from [§7.2: Running a Bitcoin Script.md](7_2_Running_a_Bitcoin_Script.md) of `1 98 OP_ADD 99 OP_EQUAL` and watch it verify; change it to `1 97 OP_ADD 99 OP_EQUAL` and see it fail.

If you prefer, you can grab the [JavaScript code](https://github.com/crm416/script/) and run it on your own machine.

#### Bugs & Challenges

_Signature Problems._ To really test Scripts you need to be able to verify hashes and signatures. The Playground tried to make that easy with buttons that can be used to cut-and-paste public keys and signatures, but they haven't kept up with Flash security upgrades and no longer work reliably. This makes any testing extremely cumbersome, as you have to dump addresses, public keys, and private keys from `bitcoin-cli`. Signing is simplified by just checking the signature of a nonce of "Secure", but given that `bitcoin-cli` produces a base64 signature instead of a hex signature, you're deep in the weeds. As a result of all of this, unless you're deeply comfortable with Bitcoin signatures, you're unlikely to be able to use Script Playground for anything but tests of flow control.

_Number Problems_. The Script Playground doesn't recognize integers of 100 or more.

_If the signature buttons worked right, the Playground would be great, but until that happens, it's not very useful._

### Web BTC Script Debugger

WebBTC has a [Script Debugger](https://webbtc.com/script) that not only shows you the execution of a script, but also explains its parts and shows you the stack. The following example is similar to the add-to-99 script of previous chapters: [Add to 15 Script](https://webbtc.com/script/1%2014/OP_ADD%2015%20OP_EQUAL/).

#### Bugs & Challenges

_Web Problems._ The web submit form no longer works, requiring you to type all script into a URL(!) of the form `https://webbtc.com/script/[unlocking-script]/[locking-script]/`. Each script is a series of opcodes or constants separated by `%20`s. For example `1%2014` is `1 14` and `OP_ADD%2015%20OP_EQUAL` is `OP_ADD 15 OP_EQUAL`.

_Visual Problems._ The stack is upside down.

_Signature Problems._ Allegedly, all signatures are assumed valid if a scripthash is not provided. This does not seem to be the case. Given that you're not signing actual transactions, and there's no discussion of how to sign a nonce, it's unclear how this even would work — again majorly impacting the utility of this site.

_Number Problems_. The BTC debugger doesn't recognize integers of 17 or more. This is not unusual, as 1 to 16 translate to OP_1 to OP_16, while going beyond that requires an OP_PUSHDATA. However, if you're not insulated from this sort of complexity, you might as well be translating to hexcode yourself.

_If signatures were indeed considered valid, this would be a great resource, but between that not working and having to annoyingly type in code as a URL, this is another online script tester that's barely usable currently. The one saving grace is that real transactions can be examined, down to the Bitcoin script ... but the site hasn't been recording new transactions since December 2016.

## Test a Script with Software Packages

Software packages may be a possible future for the testing of Bitcoin Scripts. A Python package called [Hashmal](https://github.com/mazaclub/hashmal) is very promising, but it was still in Alpha at the time of this writing. 

## Test a Script with Bitcoin

Someday web sites and software packages _might_ offer great opportunities for testing Bitcoin Scripts. However, they'll never be the real thing, because you can't guarantee that they follow the consensus rules. For example, the Script Playground explicitly says that it ignores the extra-pop multisig bug. This means that any multisig code that you successfully test on the Script Playground will break in the real world.

So the only way to _really_ test Bitcoin Scripts is to try them out on Testnet. You need to use an API to generate a transaction with your script, then verify that you can redeem coins using the script _before_ you put it on Mainnet. Don't trust that your code is right; don't just eyeball it. Doing so is another great way to lose funds on Bitcoin.

## Summary: Testing a Bitcoin Script

It turns out that there aren't currently any great tools for testing Bitcoin Scripts, except perhaps for some very simple flow control testing. We hope that some of the options in this section will mature (or get fixed) in the future, but for now you're going to need to jump ahead to APIs when you really need to do some real-world testing.
