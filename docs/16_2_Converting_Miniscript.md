# 16.2: Converting Policy & Miniscript

There are currently no command-line functions available to convert
Policy into Miniscript. However, a convertor at Peter Wuille's website
is easy to use and as official as you get in the Bitcoin world. In
addition, some wallets allow the direct use of scripting policy and
some APIs exist for conversion.

## Compile Policy on the Web

The [bitcoin.sipa.be miniscript
site](https://bitcoin.sipa.be/miniscript/) contains the most
accessible "Policy to Miniscript compiler":

[![](/images/16_2_miniscript-site-1.png)](/images/16_2_miniscript-site-1.png)

Enter a policy and click "Compile" and it will show the output,
including the Miniscript conversion, analysis of transaction costs,
and even the Bitcoin Script that the Miniscript runs as.

[![](/images/16_2_miniscript-site-2.png)](/images/16_2_miniscript-site-2.png)

> ⚠️ **Duplicate Names Not Allowed.** The sipa.be website doesn't like
it when you use the same name for multiple variables (e.g., "buyer" or
"escrow"). If you receive an error, change the names of any duplicated
keys.

## Encode Miniscript on the Web

The "Analyze a Miniscript" section of [the
website](https://bitcoin.sipa.be/miniscript/) similarly allows you to
encode Miniscript into Bitcoin Script, which you could then use to
form an address as described in the previous chapters' discussion of
P2SH and P2WSH.

[![](/images/16_2_miniscript-site-3.png)](/images/16_2_miniscript-site-3.png)

## Examine Policy to Bitcoin Script Conversions

Try using the sipa.be website to examine a few of the Bitcoin Scripts
from chapter 15, especially the ones we already converted in the
previous section. 

For example, here's the policy for our president or two vice-presidents script:

```
or(pk(president),thresh(2,pk(vp1),pk(vp2),pk(vp3)))
```

In [§15.2](15_2_Writing_Complex_Multisig_Scripts/) we wrote it as follows:

```
OP_DEPTH
1
OP_EQUAL
OP_IF
  <pubKeyPres>
  OP_CHECKSIGNATURE
ELSE
  2
  <pubKeyVPA>
  <pubKeyVPB>
  <pubKeyVPC>
  3
  OP_CHECKMULTISIG
OP_ENDIF
```

The compiler instead produces:

```
<pubKeyPres>
OP_CHECKSIG
OP_IFDUP
OP_NOTIF
  2
  <pubKeyVPA>
  <pubKeyVPB>
  <pubKeyVPC>
  3
  OP_CHECKMULTISIG
OP_ENDIF
```

This does a great job of showing two of the benefits of
Miniscript. First, it demonstrates how different Bitcoin Scripts can
produce the same results. The abstraction of Miniscript cuts out some
of that variation. Second, it demonstrates how Miniscript is built to
produce more compact results: it produces code using 10 elements vs 14
in our original. That likely means that it's fewer bytes and so
slightly cheaper to put in a block.

(Is it more or less readable? That's a different question, but with
Miniscript and especially Policy there, readability becomes less of an
issue.)

## Convert Policy & Miniscript with APIs

Conversion software is available through various APIs.

* [miniscript-policies](https://github.com/bitcoinerlab/miniscript-policies), which exposes C++ code from the sipa website
* [rust-miniscript](https://github.com/rust-bitcoin/rust-miniscript/tree/master), which includes a policy compiler. It's also the basis of BDK, which we'll meet in [§16.4](16_4_Using_BDK.md).
* [miniscript](https://github.com/bitcoinerlab/miniscript), a Miniscript analyzer in Typescript

## Summary: Converting Policy & Miniscript

Miniscript and Policy are great new tools for creating Bitcoin
Scripts, but at the moment you can't convert between the various forms
on the command line. Nonetheless, the [bitcoin.sipa.be miniscript
site](https://bitcoin.sipa.be/miniscript/) offers great tools that
will allow you to do the conversion, and then return to the command
line with whatever scripting version you prefer.

## What's Next?

Continue "Using Miniscript" with [§16.3: Using Miniscript with Descriptors]
(16_3_Using_Miniscript_with_Descriptors.md).
