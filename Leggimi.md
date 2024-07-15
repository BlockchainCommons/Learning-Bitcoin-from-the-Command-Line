# Imparare Bitcoin dalla riga di comando 2.2.0
### _by Christopher Allen and Shannon Appelcline_

![](https://www.blockchaincommons.com/images/projects/lbtc-screen.png)

Learning Bitcoin from the Command Line è un tutorial per lavorare con Bitcoin (e Lightning) che insegna l'interazione diretta con i server stessi, come il modo più robusto e sicuro per iniziare a lavorare con le criptovalute.

> NOTA: questa è una bozza in lavorazione, in modo da poter ricevere feedback dai primi revisori. Non è ancora pronta per l'uso.

_Questo tutorial presuppone che tu abbia una minima conoscenza di base su come usare l'interfaccia della riga di comando. In caso contrario, sono disponibili molti tutorial e ne ho uno per utenti Mac su https://github.com/ChristopherA/intro-mac-command-line._

## Traduzioni
* [English](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line) - Traduzione v2.0.1
* [Portuguese](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/tree/portuguese-translation/pt/README.md) - Traduzione v2.0.1 
* [Spanish](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/tree/spanish-translation/es/README.md) - Traduzione v2.0.1 
* [Italiano](https://github.com/lutxoitalico/Learning-Bitcoin-from-the-Command-Line/blob/master/Leggimi.md) - Traduzione da v2.2.0

Se desideri effettuare la tua traduzione, guarda qui [Contribuire](CONTRIBUIRE.md).

## Indice di contenuti

### PRIMA PARTE: PREPARATI PER BITCOIN

**Status:** Prima traduzione, da revisionare.

* [01_0: Introduzione alla programmazione con Bitcoin Core e Lightning](01_0_Introduzione.md)
  * [01_1 Introduzione a Bitcoin](01_1_Introduzione_a_Bitcoin.md)
* [02_0 Configurare Bitcoin-Core VPS](02_0_Configurare_Bitcoin-Core_VPS.md)
  * [02_1 Configurare Bitcoin-Core_VPS in un colpo con StackScript](02_1_Configurare_Bitcoin-Core_VPS_con_StackScript.md)
  * [02_2 Configurare Bitcoin Core con Altri Mezzi](02_2_Configurare_Bitcoin_Core_Altri_Mezzi.md)

### SECONDA PARTE: USARE BITCOIN-CLI

**Status:** Prima traduzione parziale, da revisionare.

* [3_0 Comprendere la Configurazione di Bitcoin](03_0_Comprendere_la_Configurazione_di_Bitcoin.md)
  * [03_1 Verificare la configurazione Bitcoin](03_1_Verificare_la_configurazione_Bitcoin.md)
  * [03_2 Capire la Configurazione di Bitcoin](03_2_Capire_la_configurazione_di_Bitcoin.md)
  * [03_3 Configurare la Wallet](03_3_Configurare_la_Wallet.md)
    * [03_3 Intermezzo Utilizzare Variabili nella Linea di Comando ](03_3_Intermezzo_Utilizzare_Variabili_nella_Linea_di_Comando.md)
  * [03_4 Ricevere una Transazione](03_4_Ricevere_una_Transazione.md)
  * [03_5 Comprendere il Descriptor](003_5_Comprendere_il_Descriptor.md)
* [04_0 Inviare Transazioni Bitcoin](04_0_Inviare_Transazioni_Bitcoin.md)
  * [04_1 Inviare Monete Modo Semplice](04_1_Inviare_Monete_Modo_Semplice.md)
  * [04_2 Creare una Transazione Grezza](04_2_Creare_una_Transazione_Grezza.md)
     * [04_2 Intermezzo Usare JQ](04_2_Intermezzo_Usare_JQ.md)
  * [04_3 Creare una Transazione Grezza con Alias ](04_3_Creare_una_Transazione_Grezza_con_Alias.md)
  * [04_4_Inviare_Monete_con_Transazione_Grezza](04_4_Inviare_Monete_con_Transazione_Grezza.md)
     * [04_4_Intermezzo_Usare_Curl.md](04_4_Intermezzo_Usare_Curl.md)
  * [04_5_Inviare_Monete_con_Transazione_Grezza_Automatizzata](04_5_Inviare_Monete_con_Transazione_Grezza_Automatizzata)
  * [04_6_Creare_una_Transazione_Segwit.md](04_6_Creare_una_Transazione_Segwit.md)
* [5.0: Controlling Bitcoin Transactions](05_0_Controlling_Bitcoin_Transactions.md)
  * [5.1 Watching for Stuck Transactions](05_1_Watching_for_Stuck_Transactions.md)
  * [5.2: Resending a Transaction with RBF](05_2_Resending_a_Transaction_with_RBF.md)
  * [5.3: Funding a Transaction with CPFP](05_3_Funding_a_Transaction_with_CPFP.md)
* [6.0: Expanding Bitcoin Transactions with Multisigs](06_0_Expanding_Bitcoin_Transactions_Multisigs.md)
  * [6.1: Sending a Transaction with a Multisig](06_1_Sending_a_Transaction_to_a_Multisig.md)
  * [6.2: Spending a Transaction with a Multisig](06_2_Spending_a_Transaction_to_a_Multisig.md)
  * [6.3: Sending & Spending an Automated Multisig](06_3_Sending_an_Automated_Multisig.md)
* [7.0: Expanding Bitcoin Transactions with PSBTs](07_0_Expanding_Bitcoin_Transactions_PSBTs.md)
  * [7.1: Creating a Partially Signed Bitcoin Transaction](07_1_Creating_a_Partially_Signed_Bitcoin_Transaction.md)
  * [7.2: Using a Partially Signed Bitcoin Transaction](07_2_Using_a_Partially_Signed_Bitcoin_Transaction.md)
  * [7.3: Integrating with Hardware Wallets](07_3_Integrating_with_Hardware_Wallets.md)
* [8.0: Expanding Bitcoin Transactions in Other Ways](08_0_Expanding_Bitcoin_Transactions_Other.md)
  * [8.1: Sending a Transaction with a Locktime](08_1_Sending_a_Transaction_with_a_Locktime.md)
  * [8.2: Sending a Transaction with Data](08_2_Sending_a_Transaction_with_Data.md)

### TERZA PARTE: BITCOIN SCRIPTING

**Status:** Da fare...

* [9.0: Introducing Bitcoin Scripts](09_0_Introducing_Bitcoin_Scripts.md)
  * [9.1: Understanding the Foundation of Transactions](09_1_Understanding_the_Foundation_of_Transactions.md)
  * [9.2: Running a Bitcoin Script](09_2_Running_a_Bitcoin_Script.md)
  * [9.3: Testing a Bitcoin Script](09_3_Testing_a_Bitcoin_Script.md)
  * [9.4: Scripting a P2PKH](09_4_Scripting_a_P2PKH.md)
  * [9.5: Scripting a P2WPKH](09_5_Scripting_a_P2WPKH.md)
* [10.0: Embedding Bitcoin Scripts in P2SH Transactions](10_0_Embedding_Bitcoin_Scripts_in_P2SH_Transactions.md)
  * [10.1: Understanding the Foundation of P2SH](10_1_Understanding_the_Foundation_of_P2SH.md)
  * [10.2: Building the Structure of P2SH](10_2_Building_the_Structure_of_P2SH.md)
  * [10.3: Running a Bitcoin Script with P2SH](10_3_Running_a_Bitcoin_Script_with_P2SH.md)
  * [10.4: Scripting a Multisig](10_4_Scripting_a_Multisig.md)
  * [10.5: Scripting a Segwit Script](10_5_Scripting_a_Segwit_Script.md)
  * [10.6: Spending a P2SH Transaction](10_6_Spending_a_P2SH_Transaction.md)
* [11.0: Empowering Timelock with Bitcoin Scripts](11_0_Empowering_Timelock_with_Bitcoin_Scripts.md)
  * [11.1: Understanding Timelock Options](11_1_Understanding_Timelock_Options.md)
  * [11.2: Using CLTV in Scripts](11_2_Using_CLTV_in_Scripts.md)
  * [11.3: Using CSV in Scripts](11_3_Using_CSV_in_Scripts.md)
* [12.0: Expanding Bitcoin Scripts](12_0_Expanding_Bitcoin_Scripts.md)
  * [12.1: Using Script Conditionals](12_1_Using_Script_Conditionals.md)
  * [12.2: Using Other Script Commands](12_2_Using_Other_Script_Commands.md)
* [13.0: Designing Real Bitcoin Scripts](13_0_Designing_Real_Bitcoin_Scripts.md)
  * [13.1: Writing Puzzles Scripts](13_1_Writing_Puzzle_Scripts.md)
  * [13.2: Writing Complex Multisig Scripts](13_2_Writing_Complex_Multisig_Scripts.md)
  * [13.3: Empowering Bitcoin with Scripts](13_3_Empowering_Bitcoin_with_Scripts.md)

### QUARTA PARTE: RISERVATEZZA

**Status:** Da fare...

* [14.0: Using Tor](14_0_Using_Tor.md)
  * [14.1: Verifying Your Tor Setup](14_1_Verifying_Your_Tor_Setup.md)
  * [14.2: Changing Your Bitcoin Hidden Services](14_2_Changing_Your_Bitcoin_Hidden_Services.md)
  * [14.3: Adding SSH Hidden Services](14_3_Adding_SSH_Hidden_Services.md)

* [15.0: Using i2p](15_0_Using_i2p.md)
  * [15.1: Bitcoin Core as an I2P (Invisible Internet Project) service](15_1_i2p_service.md)

### QUINTA PARTE: PROGRAMMARE CON RPC

**Status:** Da fare...

* [16.0: Talking to Bitcoind with C](16_0_Talking_to_Bitcoind.md)
  * [16.1: Accessing Bitcoind in C with RPC Libraries](16_1_Accessing_Bitcoind_with_C.md)
  * [16.2: Programming Bitcoind in C with RPC Libraries](16_2_Programming_Bitcoind_with_C.md)
  * [16.3: Receiving Notifications in C with ZMQ Libraries](16_3_Receiving_Bitcoind_Notifications_with_C.md)
* [17.0: Programming Bitcoin with Libwally](17_0_Programming_with_Libwally.md)
   * [17.1: Setting Up Libwally](17_1_Setting_Up_Libwally.md)
   * [17.2: Using BIP39 in Libwally](17_2_Using_BIP39_in_Libwally.md)
   * [17.3: Using BIP32 in Libwally](17_3_Using_BIP32_in_Libwally.md)
   * [17.4: Using PSBTs in Libwally](17_4_Using_PSBTs_in_Libwally.md)
   * [17.5: Using Scripts in Libwally](17_5_Using_Scripts_in_Libwally.md)
   * [17.6: Using Other Functions in Libwally](17_6_Using_Other_Functions_in_Libwally.md)
   * [17.7: Integrating Libwally and Bitcoin-CLI](17_7_Integrating_Libwally_and_Bitcoin-CLI.md)
* [18.0: Talking to Bitcoind with Other Languages](18_0_Talking_to_Bitcoind_Other.md)
  * [18.1: Accessing Bitcoind with Go](18_1_Accessing_Bitcoind_with_Go.md)
  * [18.2: Accessing Bitcoind with Java](18_2_Accessing_Bitcoind_with_Java.md)
  * [18.3: Accessing Bitcoind with Node JS](18_3_Accessing_Bitcoind_with_NodeJS.md)
  * [18.4: Accessing Bitcoind with Python](18_4_Accessing_Bitcoind_with_Python.md)
  * [18.5: Accessing Bitcoind with Rust](18_5_Accessing_Bitcoind_with_Rust.md)
  * [18.6: Accessing Bitcoind with Swift](18_6_Accessing_Bitcoind_with_Swift.md)

### SESTA PARTE: USARE LIGHTNING-CLI

**Status:** Da Fare...

* [19.0: Understanding Your Lightning Setup](19_0_Understanding_Your_Lightning_Setup.md)
  * [19.1: Verifying Your c-lightning Setup](19_1_Verifying_Your_Lightning_Setup.md)
  * [19.2: Knowing Your c-lightning Setup](19_2_Knowing_Your_lightning_Setup.md)
     * [Interlude: Accessing a Second Lightning Node](19_2__Interlude_Accessing_a_Second_Lightning_Node.md)
  * [19.3: Creating a Lightning Channel](19_3_Setting_Up_a_Channel.md)
* [20.0: Using Lightning](20_0_Using_Lightning.md)
  * [20.1: Generating a Payment Request](20_1_Generate_a_Payment_Request.md)
  * [20.2: Paying an Invoice](20_2_Paying_a_Invoice.md)
  * [20.3: Closing a Lighnting Channel]((20_3_Closing_a_Channel.md))
  * [20.4: Expanding the Lightning Network](20_4_Lightning_Network_Review.md)

### APENDICE

**Status:** Da fare

* [Appendices](A0_Appendices.md)
  * [Appendix I: Understanding Bitcoin Standup](A1_0_Understanding_Bitcoin_Standup.md)
  * [Appendix II: Compiling Bitcoin from Source](A2_0_Compiling_Bitcoin_from_Source.md)
  * [Appendix III: Using Bitcoin Regtest](A3_0_Using_Bitcoin_Regtest.md)

## Status - Beta

La versione v2.1.0 di **Learning Bitcoin from the Command Line** è completa e ha subito modifiche e integrazioni. È pronta per l'uso

Stiamo anche valutando in via provvisoria cosa potremmo includere in una [v3.0](TODO-30.md) del corso. Se desideri supportare un lavoro di questo tipo, diventa uno [sponsor GitHub](https://github.com/sponsors/BlockchainCommons) o supportaci sul nostro [BTCPay Server](https://btcpay.blockchaincommons.com/) e facci sapere che **Learning Bitcoin** è stato il motivo.

### Lo storico delle revisioni

#### 2.2.0 (Luglio 2024)

#### 2.2.0 (17 Novembre, 2021)

* [Traduzione in Portogese](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/tree/master/pt)
* [Traduzione in Spagnolo](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/tree/master/es)

#### 2.1.0 (12 Ottobre, 2021)

* Nuovo capitolo 15 (i2p).
* Aggiunte correzioni all'utilizzo del portafoglio da 0.21
* Aggiornata l'installazione ai nuovi script Bitcoin per 22.0
* Incorporate numerose correzioni rivelate dai primi progetti di traduzione

#### 2.0.1 (15 Giugno, 2021)

* Numerose piccole correzioni successive alla 2.0  
* Pensate come base per le traduzioni (2.2.0), ma è scivolato dentro anche il materiale 2.1.0 su i2p 

#### 2.0.0 (3 novembre 2020)

* Seconda versione importante di Learning Bitcoin
* Aggiunto materiale su Segwit, Tor, portafogli hardware, lightning, regtest
* Lavoro completamente incompiuto precedente sulla programmazione utilizzando una varietà di linguaggi

#### 1.0.0 (pre-2020)

* Versione originale, estesa ma incompleta
* Conteneva sezioni complete su setup, bitcoin-cli e scripting

## Origine, autori, copyright e licenze
Salvo diversa indicazione (in questo file [Leggimi](Leggimi.md) o nei commenti dell'intestazione del file), i contenuti di questo repository sono coperti da Copyright © 2020 di Blockchain Commons, LLC e sono concessi in licenza con [CC-BY](./LICENSE-CC-BY-4.0.md).

## Supporto finanziario

*Learning Bitcoin from the Command Line* è un progetto di [Blockchain Commons](https://www.blockchaincommons.com/). Siamo orgogliosamente una società di beneficenza sociale "senza scopo di lucro" impegnata nell'open source e nello sviluppo aperto. Il nostro lavoro è finanziato interamente da donazioni e partnership collaborative con persone come te. Ogni contributo verrà speso per creare strumenti, tecnologie e tecniche aperte che sostengano e facciano progredire l'infrastruttura di sicurezza blockchain e Internet e promuovano un web aperto.

Per supportare finanziariamente l'ulteriore sviluppo di *Learning Bitcoin from the Command Line* e altri progetti, ti preghiamo di considerare di diventare un Patreon di Blockchain Commons tramite un patrocinio mensile continuo come [Sponsor GitHub](https://github.com/sponsors/BlockchainCommons). Puoi anche supportare Blockchain Commons con bitcoin sul nostro [BTCPayServer](https://btcpay.blockchaincommons.com/).

## Contribuire

Incoraggiamo i contributi pubblici tramite issues e pull request! Si prega di rivedere [CONTRIBUIRE.md](CONTRIBUIRE.md) per i dettagli sul nostro processo di sviluppo. Tutti i contributi a questo repository richiedono un [Contributor License Agreement](./CLA.md) firmato GPG.

se si desidera fornire una traduzione di Learning Bitcoin in un'altra lingua, si prega di consultare anche [TRADURRE.md](TRADURRE.md).

### Discussioni

Il posto migliore per parlare di Blockchain Commons e dei suoi progetti è nelle nostre aree di discussione GitHub.

[**Discussioni su Blockchain Commons**](https://github.com/BlockchainCommons/Community/discussions). Per sviluppatori, stagisti e utenti di Blockchain Commons, si prega di utilizzare l'area discussioni del [repository della community](https://github.com/BlockchainCommons/Community) per parlare di problemi generali di Blockchain Commons, del programma di tirocinio o di argomenti diversi da quelli trattati dalla [Gordian Developer Community](https://github.com/BlockchainCommons/Gordian-Developer-Community/discussions) o dalla
[Gordian User Community](https://github.com/BlockchainCommons/Gordian/discussions).'

### Altre domande e problemi

In quanto community open source e open-development, Blockchain Commons non ha le risorse per fornire supporto diretto ai nostri progetti. Si prega di considerare l'area discussioni come un luogo in cui è possibile ottenere risposte alle domande. In alternativa, si prega di utilizzare la funzionalità [issues](../../issues) di questo repository. Purtroppo, non possiamo fare promesse sui tempi di risposta.

Se la tua azienda ha bisogno di supporto per utilizzare i nostri progetti, non esitare a contattarci direttamente per conoscere le opzioni. Potremmo essere in grado di offrirti un contratto per il supporto da uno dei nostri collaboratori, oppure potremmo essere in grado di indirizzarti a un'altra entità che può offrirti il ​​supporto contrattuale di cui hai bisogno.

### Credits

The following people directly contributed to this repository. You can add your name here by getting involved. The first step is learning how to contribute from our [CONTRIBUTING.md](./CONTRIBUTING.md) documentation.


| Name              | Role                | Github                                            | Email                                 | GPG Fingerprint                                    |
| ----------------- | ------------------- | ------------------------------------------------- | ------------------------------------- | -------------------------------------------------- |
| Christopher Allen | Lead Author | [@ChristopherA](https://github.com/ChristopherA) | \<ChristopherA@LifeWithAlacrity.com\> | FDFE 14A5 4ECB 30FC 5D22  74EF F8D3 6C91 3574 05ED |
| Shannon Appelcline |  Lead Author | [@shannona](https://github.com/shannona) | \<shannon.appelcline@gmail.com\> | 7EC6 B928 606F 27AD |


Additional contributions are listed below:

| Role                | Names                                    |
| ------------------- | ---------------------------------------- |
| ***Contributors:*** | [gg2001](https://github.com/gg2001) (Go, Node.js sections), [gorazdko](https://github.com/gorazdko) (Rust section), [Javier Vargas](https://github.com/javiervargas) (C, Java, Lightning, Tor sections), [jodobear](https://github.com/jodobear) (Appendix: Compiling Bitcoin, Python section), [Prayank](    https://github.com/prayank23) (i2p sections)                               |
| ***Reviewers:***    | Glen Willem [@gwillem](https://github.com/gwillem) |
| ***Sponsors:***     | Blockstream Corporation                  |

### Translation Credits 

Thanks to the volunteers who spent extensive time writing and reviewing other-language translations of the original English-language course.

#### Portuguese Translation

| Name              | Role                | Github                                            | 
| ----------------- | ------------------- | ------------------------------------------------- | 
| Namcios | Translator & Reviewer | [@namcios](https://github.com/namcios) | 
| Korea | Translator & Reviewer | [@KoreaComK](https://github.com/KoreaComK) | 
| Luke Pavsky | Translator & Reviewer | [@lukedevj](https://github.com/lukedevj) | 
| hgrams | Translator & Reviewer | [@hgrams](https://github.com/hgrams) | 

#### Spanish Translation

 Name | Role | GitHub |
| ---------- | -------- | ------------ |
| Ian Culp | Translator & Reviewer | [@icculp](https://github.com/icculp) |
| Maxi Goyheneche | Translator | [@maxcrowar](https://github.com/maxcrowar) |
| Said Rahal | Translator | [@srahalh](https://github.com/srahalh) |
| César A. Vallero | Translator & Reviewer | [@csralvall](https://github.com/csralvall) |
| Javier Vargas | Translator & Reviewer | [@javiervargas](https://github.com/javiervargas) |

## Responsible Disclosure

We want to keep all of our software safe for everyone. If you have discovered a security vulnerability, we appreciate your help in disclosing it to us in a responsible manner. We are unfortunately not able to offer bug bounties at this time.

We do ask that you offer us good faith and use best efforts not to leak information or harm any user, their data, or our developer community. Please give us a reasonable amount of time to fix the issue before you publish it. Do not defraud our users or us in the process of discovery. We promise not to bring legal action against researchers who point out a problem provided they do their best to follow the these guidelines.

### Reporting a Vulnerability

Please report suspected security vulnerabilities in private via email to ChristopherA@BlockchainCommons.com (do not use this email for support). Please do NOT create publicly viewable issues for suspected security vulnerabilities.

The following keys may be used to communicate sensitive information to developers:

| Name              | Fingerprint                                        |
| ----------------- | -------------------------------------------------- |
| Christopher Allen | FDFE 14A5 4ECB 30FC 5D22  74EF F8D3 6C91 3574 05ED |

You can import a key by running the following command with that individual’s fingerprint: `gpg --recv-keys "<fingerprint>"` Ensure that you put quotes around fingerprints that contain spaces.
