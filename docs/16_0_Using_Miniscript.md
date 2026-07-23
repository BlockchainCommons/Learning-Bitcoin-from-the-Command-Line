# Chapter 16: Using Miniscript

Bitcoin Script is somewhat unwieldly and it's definitely not
regularized. Different people can write the same thing in dramatically
different ways. That resulted in two needs: first, to design a subset
of Bitcoin Script that could be analyzed and regularized; and second,
to create a abstraction of that to allow for the easy description of
spending policies. Miniscript was created to meet the first need, and
Miniscript policy (or just Policy) was created for the second. This
chapter runs through how they work and how they can currently be used
in Bitcoin Core and other tools

## Objectives for This Chapter

After working through this chapter, a developer will be able to:

   * Create spending policies
   * Transform spending policies into Bitcoin Script
   * Transform spending policies into read-only addresses
   
Supporting objectives include the ability to:

   * Understand Miniscript
   * Know of BDK
   
## Table of Contents

  * [Section One: Understanding Miniscript & Policy](16_1_Understanding_Miniscript.md)
  * [Section Two: Converting Miniscript & Policy](16_2_Converting_Miniscript.md)
  * [Section Three: Using Miniscript with Descriptors](16_3_Using_Miniscript_with_Descriptors.md)
  * [Section Four: Using Bitcoin Dev Kit (BDK)](16_4_Using_BDK.md)
