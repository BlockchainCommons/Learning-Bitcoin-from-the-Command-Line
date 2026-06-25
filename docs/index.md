# Learning Bitcoin from the Command Line 3.0
### _by Christopher Allen and Shannon Appelcline_
#### A [Blockchain Commons Course](https://developer.blockchaincommons.com/courses/)

![](https://www.blockchaincommons.com/images/projects/lbtc-screen.png)

<i>This work is currently <b><font color="red">in process</font></b>,
but the files linked here have been revised and are in a fairly
complete state.</i>

<div style="float: right">
  <p>
    <a href="https://developer.blockchaincommons.com/resources/">
      <img src="https://developer.blockchaincommons.com/assets/badges/cat-resources.png" width=150px>
    </a>
  </p>
  <p>
    <a href=".">
      <img src="https://developer.blockchaincommons.com/assets/badges/learning-bitcoin.png" width=150px>
    </a>
  </p>
</div>


## Part One: Preparing for Bitcoin

* [**1.0: Introduction to Programming with Bitcoin Core and Lightning**](01_0_Introduction.md)
    * [Interlude: Introducing Bitcoin](01_1_Introducing_Bitcoin.md)
* [**2.0: Setting Up a Bitcoin-Core VPS**](02_0_Setting_Up_a_Bitcoin-Core_VPS.md)
    * [2.1: Setting Up a Bitcoin-Core VPS with Bitcoin Standup](02_1_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md)
    * [2.2: Setting Up a Bitcoin-Core Machine via Other Means](02_2_Setting_Up_Bitcoin_Core_Other.md)

## Part Two: Sending Transactions

* [**3.0: Preparing Your Bitcoin Wallet**](03_0_Preparing_Your_Bitcoin_Wallet.md)
    * [3.1: Verifying Your Bitcoin Setup](03_1_Verifying_Your_Bitcoin_Setup.md)
    * [3.2: Knowing Your Bitcoin Setup](03_2_Knowing_Your_Bitcoin_Setup.md)
    * [3.3: Setting Up Your Wallet](03_3_Setting_Up_Your_Wallet.md) 
    * [3.4: Understanding the Descriptor Wallet](03_4_Understanding_the_Descriptor_Wallet.md)
* [**4.0: Preparing Your Bitcoin Addresses**](04_0_Preparing_Your_Bitcoin_Addresses.md)
    * [4.1: Understanding the Address](04_1_Understanding_the_Address.md)
    * [4.2: Integrating Addresses and Descriptors](04_2_Integrating_Addresses_Descriptors.md)
    * [4.3: Creating QR Codes for Addresses](04_3_Creating_QR_Codes_for_Addresses.md)
    * [4.4: Receiving a Transaction](04_4_Receiving_a_Transaction.md)
        * [Interlude: Using Command-Line Variables](04_4a_Interlude_Using_Command-Line_Variables.md)
* [**5.0: Sending Bitcoin Transactions**](05_0_Sending_Bitcoin_Transactions.md) 
    * [5.1: Sending Coins the Easy Way](05_1_Sending_Coins_The_Easy_Way.md) 
    * [5.2: Creating a Raw Transaction](05_2_Creating_a_Raw_Transaction.md) 
        * [Interlude: Using JQ](05_2a_Interlude_Using_JQ.md) 
    * [5.3: Creating a Raw Transaction with Named Arguments](05_3_Creating_a_Raw_Transaction_with_Named_Arguments.md)
    * [5.4: Sending Coins with Raw Transactions](05_4_Sending_Coins_with_a_Raw_Transaction.md)
        * [Interlude: Using Curl](05_4a_Interlude_Using_Curl.md)
    * [5.5: Sending Coins with Automated Raw Transactions](05_5_Sending_Coins_with_Automated_Raw_Transactions.md)
    * [5.6: Sending Other Types of Transactions](05_6_Sending_Coins_to_Other_Addresses.md) 
* [**6.0: Controlling Bitcoin Transactions**](06_0_Controlling_Bitcoin_Transactions.md)
    * [6.1 Watching for Stuck Transactions](06_1_Watching_for_Stuck_Transactions.md)
    * [6.2: Resending a Transaction with RBF](06_2_Resending_a_Transaction_with_RBF.md)
    * [6.3: Funding a Transaction with CPFP](06_3_Funding_a_Transaction_with_CPFP.md)

## Part Three: Expanding Bitcoin

* [**7.0: Expanding Bitcoin Transactions with Multisigs**](07_0_Expanding_Bitcoin_Transactions_Multisigs.md)
    * [7.1: Creating a Multisig with Public Keys](07_1_Creating_Multisig_Public_Keys.md)
    * [7.2: Creating a Multisig with Descriptors](07_2_Creating_Multisig_Descriptors.md)
    * [7.3: Spending a Transaction with a Multisig](07_3_Spending_a_Transaction_to_a_Multisig.md)
* [**8.0: Expanding Bitcoin Transactions with PSBTs**](08_0_Expanding_Bitcoin_Transactions_PSBTs.md)
    * [8.1: Spending a Multisig with a PSBT](08_1_Spending_a_Multisig_with_a_PSBT.md)
    * [8.2: Understanding the PSBT](08_2_Understanding_the_PSBT.md)
    * [8.3: Using a Partially Signed Bitcoin Transaction](08_3_Using_a_Partially_Signed_Bitcoin_Transaction.md)
    * [8.4: Creating Animated QR Codes](08_4_Creating_Animated_QR_Codes.md)
* [**9.0: Expanding Bitcoin Transactions in Other Ways**](09_0_Expanding_Bitcoin_Transactions_Other.md)
    * [9.1: Sending a Transaction with a Locktime](09_1_Sending_a_Transaction_with_a_Locktime.md)
    * [9.2: Sending a Transaction with Data](09_2_Sending_a_Transaction_with_Data.md)

## Part Four: Exploring the Ecosystem

* [**10.0: Working with Secrets**](10_0_Working_with_Secrets.md )
    * [10.1: Creating Secrets from the Command Line](10_1_Creating_Secrets_from_the_Command_Line.md)
    * [10.2: Converting Secrets with Keytool](10_2_Converting_Secrets_with_Keytool.md)
    * [10.3: Importing Secrets to Bitcoin Core](10_3_Importing_Secrets_to_Bitcoin_Core.md)
    * [10.4: Exporting Secrets from Bitcoin Core](10_4_Exporting_Secrets_from_Bitcoin_Core.md)
    * [10.5: Storing Secrets with Envelope](10_5_Storing_Secrets_with_Envelope.md)

## Part Five: Scripting with Bitcoin

* [**11.0: Introducing Bitcoin Scripts**](11_0_Introducing_Bitcoin_Scripts.md)
    * [11.1: Understanding the Foundation of Transactions](11_1_Understanding_the_Foundation_of_Transactions.md)
    * [11.2: Running a Bitcoin Script](11_2_Running_a_Bitcoin_Script.md)
    * [11.3: Testing a Bitcoin Script](11_3_Testing_a_Bitcoin_Script.md)
    * [11.4: Scripting a P2PKH](11_4_Scripting_a_P2PKH.md)
    * [11.5: Scripting a P2WPKH](11_5_Scripting_a_P2WPKH.md)
* [**12.0: Embedding Bitcoin Scripts in P2SH & P2SH Transactions**](12_0_Embedding_Bitcoin_Scripts_in_P2WSH_Transactions.md)
    * [12.1: Understanding the Foundation of P2SH & P2WSH](12_1_Understanding_the_Foundation_of_P2WSH.md)
    * [12.2: Building the Structure of P2SH & P2WSH](12_2_Building_the_Structure_of_P2WSH.md)  
    * [12.3: Running a Bitcoin Script with P2SH or P2WSH](12_3_Running_a_Bitcoin_Script_with_P2WSH.md)
    * [12.4: Scripting a Multisig](12_4_Scripting_a_Multisig.md)
    * [12.5: Spending a P2SH or P2WSH Transaction](12_5_Spending_a_P2WSH_Transaction.md)

---

> _Learning Bitcoin v1.0 was created in part under the sponsorship of [Blockstream](https://blockstream.com/). Learning Bitcoin v2.0 was self-funded by [Blockchain Commons](https://www.blockchaincommons.com/). Learning Bitcoin v3.0 was funded by a grant from [HRF](https://hrf.org/)._

---
