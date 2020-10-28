# Todo List for v2.0 of Learning Bitcoin

This is a list of potential (or planned) TODO items for "Learning Bitcoin from the Command Line". Many are drawn from issues found at the previous locale of this repo.

**Updating Work Begun:** 6/10
**Updating Work Done:** 10/27
**Pending Issues:** Third-party edit of whole course

Please also see [TODO-30.md](the v3.0 TODO) for the next iteration of the course.

## 1. Integrate with Bitcoin-Standup

Replace our current scripts with Bitcoin Standup (which was based on those scripts)

1. <strike>Fix Verification errors in Bitcoin Standup Scripts</strike> **6/10**
   * <strike>Fix IPADDR error in Bitcoin Standup Scripts</strike> **6/10**
2. <strike>Upgrade Bitcoin Standup Scripts to Debian 10</strike> **6/16**
3. <strike>Upgrade Bitcoin Standup Scripts to Bitcoin 0.20</strike> **6/16**
4. <strike>Ensure Bitcoin Standup covers everything else in previous scripts</strike> **6/16**
5. <strike>Rewrite the StackScript chapter</strike> **6/16**
6. <strike>Rewrite the "by-hand" chapter to match Bitcoin Standup</strike> **7/27**
   * <strike>Write New Appendix 1</strike> **7/27**
   * <strike>Revise All Chapter Ordering for Cut §2.1</strike> **7/27**
7. <strike>Reintroduce aliases after setup</strike> **6/19**
8. <strike>Figure out what to do about the old `btcblock` on testnet, where there seems to be no CLI way to determine blockheight.</strike> **No Known Solution on Testnet**

## 2. Upgrade to 0.20

9. <strike>Walk through chapters 1-6, making sure all commands work with Bitcoin Core 0.20 (and changing them if not). Redo the output of all examples.</strike> **6/19**, **6/23**, **6/30**
   * <strike>Edit Chapter 0</strike> **6/19**
   * <strike>Edit Chapter 1</strike> **6/19**
   * <strike>Re-edit Chapter 2</strike> **6/19**
   * <strike>Edit & Check Chapter 3</strike> **6/19**
   * <strike>Edit & Check Chapter 4</strike> **6/23**
      * <strike>Double-check fee calculator in 4.2I with a more complex example</strike> **6/19**
      * <strike>Integrate older Curl Interlude</strike> **6/19**
   * <strike>Edit & Check Chapter 5</strike> **6/23**
   * <strike>Edit & Check Chapter 6</strike> **6/30**
      * <strike>Update for change in importaddress</strike> **7/7**
   
## 3. Add BTCDEB Support

10. Update Scripting & Integrate `btcdeb`
   * <strike>Edit & Integrate Chapter 9</strike> **7/15**
      * <strike>Fix P2PKH Example</strike> **7/15**
      * <strike>Write P2WPKH Chapter</strike> **7/15**
   * <strike>Edit & Integrate Chapter 10</strike> **7/15, 7/21**
      * <strike>Write P2WSH Chapter</strike> **7/21**
   * <strike>Edit & Integrate Chapter 11</strike> **7/21**
   * <strike>Edit & Integrate Chapter 12</strike> **7/21**
   * <strike>Edit & Integrate Chapter 13</strike> **7/21**
   * Turn off verbose info on btcdeb?
   * Get sigs working right on btcdeb?
   
## 4. Add New Concepts

Add and document the following new concepts:

11. <strike>Add SegWit Transactions. The majority of Bitcoin transactions now use this signing methodology, so it needs to be fully explained and incorporated, alongside its newer bech32 addresses.</strike> **6/23**
   * <strike>Add definitions of Segwit and bech32 addresses to 3.3</strike> **6/23**
   * <strike>Do we still have to use "bitcoin-cli getnewaddress "" legacy" on CLI? If not, run back through chapters that use legacy in their examples, starting in 3.3</strike> **Gonna leave it for now. A future version might shift to P2SH-SegWit as default.**
   * <strike>Integrate discussions of SegWit into early parts of chapter 4.</strike> **6/23**
   * <strike>Write chapter 4.6</strike> **6/23**
12. <strike>Add PSBT. Partially Signed Bitcoin Transactions are a powerful new interchange format that should be fully included in our discussions of multi-sigs and escrows (which instead fell back on older methodologies in v1.0).</strike> **7/2**, **7/7**, **7/10**
   * <strike>Research PSBTs</strike> **7/2**
   * <strike>Update Outlines</strike> **7/2**
   * <strike>Write 7.1: Creating a PSBT</strike> **7/7**
      * <strike>Create Illustrations for 7.1</strike> **7/7**
   * <strike>Write 7.2: Using a PSBT</strike> **7/7**, **7/10**
      * <strike>Multisigs</strike> **7/7**
         * <strike>Redo with correct math</strike> **7/10**
      * <strike>Fund Pooling</strike> **7/10**
      * <strike>CoinJoins</strike> **7/10**
      
13. <strike>Add Wallet Updates.</strike> **6/30**, **7/2**, **7/7**
   * <strike>Bitcoin Descriptors</strike> **6/30**
      * <strike>Show what a SegWit descriptor looks like</strike> **7/2**
      * <strike>Show what a multi-sig descriptor looks like</strike> **7/2**
      * <strike>Revise based on comments</strike> **7/7**
   * <strike>Key Ordering (sortedmulti)</strike> **7/2** **Partially Supported in 0.20**
14. <strike>Consider HWI.</strike> **7/14**
  * <strike>Write 7.3: Using HWI</strike> **7/14**
  * <strike>Edit All of Chapter 6-8</strike> **7/14**
15. <strike>Consider splitting up Chapter 6 (definitely if we get an HWI chapter)</strike>
   * <strike>Break apart Chapter 6</strike> **7/2**
   * <strike>Rewrite all section links in early chapters (1-5, 6, 8)</strike> **7/7**
   * <strike>Update Script Chapters</strike> **7/15**
   * <strike>Update Tor Chapter</strike> **7/22**
   * <strike>Update Programming Chapters</strike>
   
## 5. Finish Later Chapters

16. <strike>Edit & Integrate Tor Chapter. </strike> **7/22**
17. <strike>Edit & Integrate all "C" work</strike> **8/5** - **8/26**
   * <strike>Edit & Update Chapter 15</strike> **8/5**
   * <strike>Write the Libwally section</strike> **8/5** - **8/26**
      * <strike>16.1: Intro</strike> **8/5**
      * <strike>16.2: BIP39</strike> **8/11**
      * <strike>16.3: BIP32</strike> **8/11**
      * <strike>16.4: PSBTs</strike> **8/12**
      * <strike>16.5: Scripts</strike> **8/12**
      * <strike>16.6: Other Functions</strike> **8/18**
      * <strike>16.7: Integration</strike> **8/25**, **8/26**
18. <strike>Edit & Integrate all "Other Languages" work</strike> **9/1**-**9/30**
   * <strike>Edit 17.1: Go</strike> **9/1**
   * <strike>Edit 17.2: Java</strike> **9/2**
   * <strike>Edit 17.3: NodeJS</strike> **9/2**
   * <strike>Edit 17.4: Python</strike> **9/8**
   * <strike>Edit 17.5: Rust</strike> **9/8**
   * <strike>Write 17.6: Swift</strike> **9/29**
      * <strike>Setup Swift Environment</strike> **9/9**, **9/15**
      * <strike>Document Swift Environment</strike> **9/23**
      * <strike>Do Basic RPC Commands</strike> **9/29**
      * <strike>Look at Wallets</strike> **9/29**
      * <strike>Create Transaction</strike> **9/29**
      * <strike>First edit of Swift</strike> **9/30**
   * <strike>Edit All of 17</strike> **9/30**
      * <strike>Finalize Naming of Src</strike> **9/30**
      * <strike>Full Edit of 17.1-17.6</strike> **9/30**
19. <strike>Edit & Integrate Appendices work</strike> **7/27**, **8/4**
   * <strike>Edit & Integrate A2: Compiling</strike> **7/27**
   * <strike>Edit & Integrate A3: RegTest</strike> **8/4**
20. <strike>Edit & Integrate Lightning Chapters</strike> **10/13**, **10/14**
   * <strike> Figure out how/if to set up a pruned c-lightning</strike> **10/13**

## 6. Finalize Book

21. <strike>Take out warnings for all old chapters</strike> **10/20**
22. <strike>Add new first chapter, indicating places to start for people of different levels</strike> **10/20**
23. <strike>Add other Bitcoin standup options to Ch. 2</strike> **10/27**
24. <strike>Move an coding appendices (e.g., JQ interlude) to src code directory </strike> **10/20**
25. <strike>Write letter about new edition</strike> **10/27**
26. <strike>Write tweetstorm</strike> **10/27**

## 7. Additional Support

27. Full edit of book (intern?)
