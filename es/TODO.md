## Translatable word counts by chapter

We can use this table to track progress in translation and better see what's left to be done. As of right now I am a little uncertain as to how to update the translation branch chapters to the latest changes in the English master. Need to better understand and document the Releases feature.

As of right now I am uncertain how to track our current progress towards release 2.0.1 versus updates to the latest master. Enough chapters were translated based on master, which has changes beyond release 2.0.1 that it would be difficult, with chapter insertions and more, to revert back to the original stationary target. Maybe a new release of 2.0.2 and then strict enforcing of this would help.

So for example, this table was created at the same time as 2.0.1, before the new chapter 15 was inserted, and thus 16-20 were 15-19. I am now inserting chapter 15 into the table since it has been translated so. See issue #438

Initial merge as of <> commit...?

TODO:
- [ ] Complete initial translation and merge of chapters into spanish-translation branch
- [ ] Ensure all chapter names and links are in spanish and that links are working
- [ ] Update chapters from original translation commit to latest master or latest release (2.0.2?)
- [ ] Better document the release process and how to translate to a release

Unfortunately checkboxes don't work in the table markdown. If anyone knows how to put checkboxes in a table please do! For now we will mark things as:

Blank when no one has announced the translation is in progress
PENDING when someone currently selected chapter to translate. Maybe a github username can be added here to better track who's working on what
REVIEW when PR created and review pending or accepted and waiting merge
DONE when review accepted and merged into spanish-translation

Chapter|Word Count|Initial translation complete + merged|Chapter names and links updated to spanish and verified working|Updated to latest master/release?
---|---|---|---|---
01_0_Introduction.md|1144|DONE||
01_1_Introducing_Bitcoin.md|2735|DONE||
02_0_Setting_Up_a_Bitcoin-Core_VPS.md|226|REVIEW||
02_1_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md|2746|DONE||
02_2_Setting_Up_Bitcoin_Core_Other.md|254|DONE||
03_0_Understanding_Your_Bitcoin_Setup.md|248|REVIEW||
03_1_Verifying_Your_Bitcoin_Setup.md|773|REVIEW||
03_2_Knowing_Your_Bitcoin_Setup.md|517|REVIEW||
03_3_Setting_Up_Your_Wallet.md|1699|REVIEW||
03_3__Interlude_Using_Command-Line_Variables.md|347|DONE||
03_4_Receiving_a_Transaction.md|1479|REVIEW||
03_5_Understanding_the_Descriptor.md|1349|REVIEW||
04_0_Sending_Bitcoin_Transactions.md|176|REVIEW||
04_1_Sending_Coins_The_Easy_Way.md|1195|REVIEW||
04_2_Creating_a_Raw_Transaction.md|1720|REVIEW||
04_2__Interlude_Using_JQ.md|1956|REVIEW||
04_3_Creating_a_Raw_Transaction_with_Named_Arguments.md|413|REVIEW||
04_4_Sending_Coins_with_a_Raw_Transaction.md|1024|REVIEW||
04_4__Interlude_Using_Curl.md|1643|REVIEW||
04_5_Sending_Coins_with_Automated_Raw_Transactions.md|614|REVIEW||
04_6_Creating_a_Segwit_Transaction.md|1172|REVIEW||
05_0_Controlling_Bitcoin_Transactions.md|149|DONE||
05_1_Watching_for_Stuck_Transactions.md|595|DONE||
05_2_Resending_a_Transaction_with_RBF.md|1372|DONE||
05_3_Funding_a_Transaction_with_CPFP.md|827|DONE||
06_0_Expanding_Bitcoin_Transactions_Multisigs.md|155|||
06_1_Sending_a_Transaction_to_a_Multisig.md|1764|||
06_2_Spending_a_Transaction_to_a_Multisig.md|1079|||
06_3_Sending_an_Automated_Multisig.md|613|||
07_0_Expanding_Bitcoin_Transactions_PSBTs.md|169|||
07_1_Creating_a_Partially_Signed_Bitcoin_Transaction.md|1470|||
07_2_Using_a_Partially_Signed_Bitcoin_Transaction.md|1393|||
07_3_Integrating_with_Hardware_Wallets.md|2150|||
08_0_Expanding_Bitcoin_Transactions_Other.md|139|REVIEW||
08_1_Sending_a_Transaction_with_a_Locktime.md|1483|DONE||
08_2_Sending_a_Transaction_with_Data.md|580|REVIEW||
09_0_Introducing_Bitcoin_Scripts.md|196|CONFLICT||
09_1_Understanding_the_Foundation_of_Transactions.md|989|REVIEW||
09_2_Running_a_Bitcoin_Script.md|863|REVIEW||
09_3_Testing_a_Bitcoin_Script.md|1000|REVIEW||
09_4_Scripting_a_P2PKH.md|838|REVIEW||
09_5_Scripting_a_P2WPKH.md|845|REVIEW||
10_0_Embedding_Bitcoin_Scripts_in_P2SH_Transactions.md|170|DONE||
10_1_Understanding_the_Foundation_of_P2SH.md|1164|DONE||
10_2_Building_the_Structure_of_P2SH.md|1284|DONE||
10_3_Running_a_Bitcoin_Script_with_P2SH.md|323|DONE||
10_4_Scripting_a_Multisig.md|1016|DONE||
10_5_Scripting_a_Segwit_Script.md|750|DONE||
10_6_Spending_a_P2SH_Transaction.md|384|DONE||
11_0_Empowering_Timelock_with_Bitcoin_Scripts.md|108|DONE||
11_1_Understanding_Timelock_Options.md|557|DONE||
11_2_Using_CLTV_in_Scripts.md|1197|DONE||
11_3_Using_CSV_in_Scripts.md|1470|DONE||
12_0_Expanding_Bitcoin_Scripts.md|99|REVIEW||
12_1_Using_Script_Conditionals.md|1120|REVIEW||
12_2_Using_Other_Script_Commands.md|407|REVIEW||
13_0_Designing_Real_Bitcoin_Scripts.md|116|REVIEW||
13_1_Writing_Puzzle_Scripts.md|998|REVIEW||
13_2_Writing_Complex_Multisig_Scripts.md|996|REVIEW||
13_3_Empowering_Bitcoin_with_Scripts.md|1467|REVIEW||
14_0_Using_Tor.md|116|DONE||
14_1_Verifying_Your_Tor_Setup.md|1568|DONE||
14_2_Changing_Your_Bitcoin_Hidden_Services.md|434|DONE||
14_3_Adding_SSH_Hidden_Services.md|330|REVIEW||
15_0_Using_i2p.md|?|DONE||
15_1_i2p_service.md|?|DONE||
16_0_Talking_to_Bitcoind.md|254|DONE||
16_1_Accessing_Bitcoind_with_C.md|1238|DONE||
16_2_Programming_Bitcoind_with_C.md|1427|DONE||
16_3_Receiving_Bitcoind_Notifications_with_C.md|650|DONE||
17_0_Programming_with_Libwally.md|333|DONE||
17_1_Setting_Up_Libwally.md|559|DONE||
17_2_Using_BIP39_in_Libwally.md|939|DONE||
17_3_Using_BIP32_in_Libwally.md|959|REVIEW||
17_4_Using_PSBTs_in_Libwally.md|989|DONE||
17_5_Using_Scripts_in_Libwally.md|785|DONE||
17_6_Using_Other_Functions_in_Libwally.md|655|DONE||
17_7_Integrating_Libwally_and_Bitcoin-CLI.md|1380|REVIEW||
18_0_Talking_to_Bitcoind_Other.md|286|DONE||
18_1_Accessing_Bitcoind_with_Go.md|547|DONE||
18_2_Accessing_Bitcoind_with_Java.md|821|DONE||
18_3_Accessing_Bitcoind_with_NodeJS.md|393|DONE||
18_4_Accessing_Bitcoind_with_Python.md|1158|DONE||
18_5_Accessing_Bitcoind_with_Rust.md|829|REVIEW||
18_6_Accessing_Bitcoind_with_Swift.md|1503|DONE||
19_0_Understanding_Your_Lightning_Setup.md|192|||
19_1_Verifying_Your_Lightning_Setup.md|1294|||
19_2_Knowing_Your_lightning_Setup.md|399|||
19_2__Interlude_Accessing_a_Second_Lightning_Node.md|886|||
19_3_Setting_Up_a_Channel.md|1173|||
20_0_Using_Lightning.md|146|||
20_1_Generate_a_Payment_Request.md|968|||
20_2_Paying_a_Invoice.md|604|||
20_3_Closing_a_Channel.md|848|||
20_4_Lightning_Network_Review.md|626|||
A0_Appendices.md|112|REVIEW||
A1_0_Understanding_Bitcoin_Standup.m|420|REVIEW||
A2_0_Compiling_Bitcoin_from_Source.md|412|REVIEW||
A3_0_Using_Bitcoin_Regtest.md|980|REVIEW||
CLA.md|495|||
CONTRIBUTING.md|529|||
LICENSE-CC-BY-4.0.md|2716|||
README.md|1705|DONE||
TRANSLATING.md|686|||
TOTAL|89069|||
