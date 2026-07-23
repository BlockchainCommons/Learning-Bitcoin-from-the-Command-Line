# 16.1: Understanding Miniscript & Policy

In recent years a subset of Bitcoin Scripts has appeared called
Miniscript, courtesy of [BIP
379](https://github.com/bitcoin/bips/blob/master/bip-0379.md). Miniscript
is more structured than Bitcoin Script, allowing "analysis,
composition, generic signing and more." There's also an even more
abstract version of Miniscript called Policy, which is better for
design.

## Understand the Purpose of Miniscript

Miniscript just does what Bitcoin Script does. Or rather, it does just
what a subset of Bitcoin Script does, and in a more constrained way.

So, why have Miniscript at all? It works around the open design of
Bitcoin Script to create something that is more analyzable and
composable, allowing for the simpler creation of predictable Bitcoin
Scripts. [BIP
379](https://github.com/bitcoin/bips/blob/master/bip-0379.md) offers a
number of additional reasons for the creation of a second layer of
Bitcoin Scripting languages.

Miniscript is meant to:

* Create the most efficient Bitcoin Scripts.
* Easily composable multiple spending conditions (like the variable multisigs of [§15.2](15_2_Writing_Complex_Multisig_Scripts.md).
* Allow analysis of Bitcoin Scripts.
* Determine whether resource limitations might prevent spending a Bitcoin Script.

As BIP379 explains: "Miniscript functions as a representation for
scripts ... It has a structure that allows composition. It is very
easy to statically analyze for various properties (spending
conditions, correctness, security properties, malleability, ...). It
can be targeted by spending policy compilers. Finally, compatible
scripts can easily be converted to Miniscript form - avoiding the need
for additional metadata for e.g. signing devices that support it."

Though Miniscript can be compiled independently into Bitcoin Script,
it's also built to be placed directly in P2WSH (Segwit) and P2TR
(Taproot) transactions, allowing for smaller, more efficient, and more
reliable transactions.

## Understand Miniscript & Policy

Miniscripts are composed of functions called "fragments", which appear
in the form "function(arg1, arg2, ...)". Each fragment has a
one-to-one correlation with some chunk of Bitcoin Script code (hence
the name, "fragment"). For example the fragment `pk_h(key)`
corresponds to the Bitcoin Script `DUP HASH160 <HASH160(key)>
EQUALVERIFY` while `or_i(X,Z)` corresponds to `IF [X] ELSE [Z] ENDIF`.

Generally, Miniscript can recognize the following types of Bitcoin Script functions:

* Single-key Signatures
* Multisigs
* Relative Timelocks
* Absolute Timelocks
* Hashlocks (e.g., you hold the prehash object, or "preimage")
* And Statements
* Or Statements
* If/Else Statements
* Thresholds of Other Functions (e.g. k-of-n conditions are met)

Miniscript also includes types (referring to whether fragments verify
their results, set up a key for verification, or do something else
with the stack); and wrappers (which convert between types and do
stack-based manipulation), and which are designated by a letter
followed by a colon in Miniscript, such as `s:pk(key)` .

Miniscript is meant to be a composable and analyzable version of
Bitcoin Script, but not necessarily a design language. For example
`or_b(pk(key1),s:pk(key2))` is a common Miniscript function. It checks
the first key's signature (`pk(key1)`), swaps that result down in the
stack (`s:`), checks the second key's signature (`pk(key2)`) and then
boolean ORs the two results. That's great for being able to compose
this set of of Bitcoin Script commands (`<key1> OP_CHECKSIG OP_SWAP
<key2> OP_CHECKSIG OP_BOOLOR`), but it's not great for designing the
spending requirement. For that, a designer really wants to be able to
say something like `or(pk(key1),pk(key2))`.

That's where Miniscript Policy (or just Policy) comes in. It's a
further abstraction and simplification of Miniscript that is compiled
into Miniscript before _that_ is compiled into Bitcoin Script.

_Though Miniscript is necessary to achieve the goal of structured and
analyzable code, Policy allows human users to easily design using
Miniscript._

The standard procedure is therefore:

1. User writes spending policy for coins.
2. User or app compiles policy into Miniscript.
3. App encodes Miniscript into Bitcoin Script for use  on address.

## Know Policy

Unlike Miniscript, Policy is not fully specified. However, it's
generally understood to include the following abstractions of
Miniscript. (Bitcoin representations are included for those policy
functions that always compile into the Miniscript code.)

| Policy | Representation | Notes |
|---------|------------|-----|
| pk(key) | | Require key signature |
| after(num) | `<num> CHECKLOCKTIMEVERIFY` | Enforce absolute timelock |
| older(num) | `<num> CHECKSEQUENCEVERIFY` | Enforce relative timelock |
| hash160(hex) | `SIZE <20> EQUALVERIFY HASH160 <h> EQUAL` | Require prehash object |
| hash256(hex) | `SIZE <20> EQUALVERIFY HASH256 <h> EQUAL` | Require prehash object |
| ripemd160(hex) | `SIZE <20> EQUALVERIFY RIPEMD160 <h> EQUAL` | Require prehash object |
| sha256(hex) | `SIZE <20> EQUALVERIFY SHA256 <h> EQUAL | Require prehash object |
| and(x,y) | | Require X and Y |
| or(x,y) | | Require X or Y |
| thresh(k,policy1,policy2,...) | | Require k-of-n policies |

A spending policy then requires some combination of these.

Policy also generally supports a "likelihood" prefix of the number
"#@", for example "9@pk(key)", which states how likely one element is
to happen as compared to others and influences how the ultimate
Bitcoin Script is composed.

Here are some examples of spending policies:

**You must have the key.**

   ```
   pk(key)
   ```

**One of two keys may sign.**

   ```
   or(pk(key1),pk(key2))
   ```

**One of two keys may sign, but it'll probably be the first one.**

   ```
   or(9@pk(key1),pk(key2))
   ```

   _All of the more complex multisig examples from
   [§15.2](15_2_Writing_Complex_Multisig_Scripts.md) can similarly be
   created:_

**A multisig with a single signer or two-out-of three co-signers.**

   ```
   or(pk(president),thresh(2,pk(vp1),pk(vp2),pk(vp3)))
   ```

**A two-of-three multisig with a required signer.**

   ```
   and(pk(required),or(pk(opt1),pk(opt2)))
   ```

**A required escrow agent with timelock protection.**

   ```
   or(and(pk(escrow),or(pk(buyer),pk(seller))),thresh(3,after(1783637422),pk(buyer),pk(seller)))
   ```

**An open escrow with buyer protection.**

   ```
   or(thresh(2,pk(escrow),pk(buyer),pk(seller)),and(after(1783637422),pk(buyer)))
   ```

## Know Miniscript

Miniscript looks a lot like Policy with three big differences.

First, the ands and ors are minutely differentiated based on what
inputs they expect, what outputs they offer, and how they should be
represented in Bitcoin Script:

| Miniscript | Representation | Notes |
|------------|----------------|-------|
| `andor` | `[X] NOTIF [Z] ELSE [Y] ENDIF` | 
| `and_b` | `[X] [Y] BOOLAND` | boolean output |
| `and_n` | `[X] NOTIF 0 ELSE [Y] ENDIF` | andor(X,Y,0) |
| `and_v` | `[X] [Y]` | varied output |
| `or_b` | `[X] [Y] BOOLOR` | boolean output |
| `or_c` | `[X] NOTIF [Y] ENDIF` | verify or stop |
| `or_d` | `[X] IFDUP NOTIF [Y] ENDIF` | boolean output |
| `or_i` | `IF [X] ELSE [Y] ENDIF` | varied output |

Some of these differences are related to what's on the stack, some to
what input is expected, some to what output is expected, and some to
how the Script could be most efficiently composed. It's important to
be able to read these, but not necessarily to know how to compose
them. (That's what Policy is for!)

Second, the generic `thresh` is differentiated into `thresh` (for
general use), `multi` (for multisigs in P2WSH), and `multi_a` (for
multisigs in P2TR). In addition, `pk` is differentiated into `pk_h`,
which is a hash that instead requires both the public key and the
signature.

| Miniscript | Representation | Notes |
|------------|----------------|-------|
| `multi` | `<k> <key1> ... <keyn> <n> CHECKMULTISIG` | multisig |
| `pk_h` | `DUP HASH160 <HASH160(key)> EQUALVERIFY` | P2PKH |

Third, wrappers are added. The most common are:

| Miniscript | Representation | Notes |
|------------|----------------|-------|
| c: | [X] CHECKSIG | check signature |
| s: | SWAP [X] | swap the top of the stack |
| v: | [X] VERIFY | verify or stop |

A [complete reference](https://bitcoin.sipa.be/miniscript/) is available at sipa.be.

Here's how the above examples convert from Policy to Miniscript:

**You must have the key.**

   This stays just the same:

   ```
   pk(key)
   ```

**One of two keys may sign.**

   The `or` is turned into boolean or (`or_b`) and a swap (`s:`) is
   added that's needed for the script to work.
   
   ```
   or(pk(key1),pk(key2))
   
   ⬇️
   
   or_b(pk(key1),s:pk(key2))
   ```

**One of two keys may sign, but it'll probably be the first one.**

   The weighting turns the `or` into an `or_d`, which uses `IFDUP
   NOTIF` to check the more likely condition (`key1`) and on failure
   check the least likely condition (`key2`), which also requires the
   use of `pkh` instead of `pk` for the second key.
   
   ```
   or(9@pk(key1),pk(key2))

   ⬇️

   or_d(pk(key1),pkh(key2))
   ```


**A multisig with a single signer or two-out-of three co-signers.**

   Again, an `or_d` is used, presuming the single-sig is more likely,
   while the `thresh` is turned into a `multi`.
   
   ```
   or(pk(president),thresh(2,pk(vp1),pk(vp2),pk(vp3)))

   ⬇️

   or_d(pk(president),multi(2,vp1,vp2,vp3))
   ```

**A two-of-three multisig with a required signer.**

   The two non-required signers are checked with an `or_c`: if one
   fails, then the other is checked. The second one is verified with a
   `v:` (if both fail, then quit there). Then,
   that result and the required signature are checked with `and_v`.
   
   ```
   and(pk(required),or(pk(opt1),pk(opt2)))

   ⬇️

   and_v(or_c(pk(opt1),v:pk(opt2)),pk(required))
   ```

**A required escrow agent with timelock protection.**

   The `andor` says to either check the escrow agent's public key and
   the second condition, or else run the third condition. The second
   condition is a signature check (`c:`) of either the buyer or
   seller's key (`or_i`). The alternative condition is a pair of
   `and_v`, which together make up the three-of-three threshold laid
   out for the buyer key, seller key, and timelock, with each of the
   signature checks having a verify (`v:`) to show that if they fail,
   the whole thing fails.
   
   ```
   or(and(pk(escrow),or(pk(buyer),pk(seller))),thresh(3,after(1783637422),pk(buyer),pk(seller)))

   ⬇️

   andor(pk(escrow),c:or_i(pk_h(buyer),pk_h(seller)),and_v(and_v(v:pk(buyer),v:pk(seller)),after(1783637422)))
   ```

**An open escrow with buyer protection.**

   This last example similarly using an `andor` to say: either the
   buyer's key and after a timelock, or a multisig. The `thresh` is
   again turned into a `multi`.
   
   ```
   or(thresh(2,pk(escrow),pk(buyer),pk(seller)),and(after(1783637422),pk(buyer)))

   ⬇️

   andor(pk(buyer2),after(1783637422),multi(2,escrow,buyer,seller))
   ```

Again, you shouldn't worry about knowing how to compose these
Miniscript functions, but you should be able to reasonably read them
and suss out what they're doing, so that if you're handed Miniscript
in the future, you can understand it.

## Summary: Understanding Miniscript

Miniscript offers a regularized, interopable way to represent Bitcoin
Script that can be analyzed and optimized. Policy abstracts that
further to make it easy to design spending rules. You can create at
either level, convert, then use enter the Miniscript or converted
Bitcoin Script to create an address, depending on the functionality of
the wallet you're using.

> 🔥 ***What is the power of miniscript and policy?*** Miniscript
makes it easy for programs to analyze and optimize Bitcoin
Script. Policy makes it easy for users to design spending policy.

## What's Next?

Continue "Using Miniscript" with [§16.2: Converting
Miniscript & Policy](16_2_Converting_Miniscript.md).
