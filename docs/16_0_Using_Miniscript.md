# Chapter 16: Using Miniscript

Bitcoin Script is somewhat unwieldly and it's definitely not
regularized. Different people can write the same thing in dramatically
different ways. That created two needs: first, to create a subset of
Bitcoin Script that could be analyzed an regularized; and second, to
create a abstraction of that to allow for the easy design of spending
policies. Miniscript was created to meet the first need, and
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
  * [Section Two: Converting Policy](16_2_Converting_Miniscript.md)
  * [Section Three: Converting Miniscript](16_3_Converting_Miniscript.md)
  * [Section Four: Using Miniscript with Descriptors](16_4_Using_Miniscript_with_Descriptors.md)
  * [Section Five: Using Bitcoin Dev Kit (BDK)](16_5_Using_BDK.md)
