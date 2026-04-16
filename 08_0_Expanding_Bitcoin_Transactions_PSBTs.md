# Chapter Eight: Expanding Bitcoin Transactions with PSBTs

The previous chapter discussed how to use multisigs to collaboratively
determine consent among multiple parties. But we didn't get as far as
actually spending those coins! That's because the general topic of
creating collaborative transactions has been taken up by a newer
specification: partially signed bitcoin transactions (PSBTs).

This chapter will examine the usage of PSBTs, starting with that
multisig you created in the last chapter, and moving on to three more
real-world use cases for their usage:

1. Spending multisig funds (§8.1)
2. Pooling funds (§8.3)
3. Exchanging funds in a CoinJoin (§8.3)
4. Signing with a hardware wallet (§8.5)

Each use case requires collaboration. Most require trustlessness. All
can be done with PSBTs.

## Objectives for This Section

After working through this chapter, a developer will be able to:

   * Create Transactions with PSBTs
   * Use Command Line Tools to Complete PSBTs
   * Use HWI to Interact with a Hardware Wallet
   
Supporting objectives include the ability to:

   * Understand How PSBTs Differ from Multisigs
   * Understand the Full Workflow of Working with PSBTs
   * Plan for the Power of PSBTs
   * Understand The Use of a Hardware Wallet
   
## Table of Contents

   * [Section 1: Spending a Multisig with a PSBT](08_1_Spending_a_Multisig_with_a_PSBT.md)
   * [Section 2: Understanding the PSBT](08_2_Understanding_the_PSBT.md)
   * [Section 3: Using a Partially Signed Bitcoin Transaction](08_3_Using_a_Partially_Signed_Bitcoin_Transaction.md)
   * [Section 4: Creating Animated QR Codes](08_4_Creating_Animated_QR_Codes.md)
   * [Section 5: Integrating with Hardware Wallets](08_5_Integrating_with_Hardware_Wallets.md)
