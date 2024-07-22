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

**Status:** Prima traduzione completa, da revisionare.

* [01_0: Introduzione alla programmazione con Bitcoin Core e Lightning](01_0_Introduzione.md)
  * [01_1 Introduzione a Bitcoin](01_1_Introduzione_a_Bitcoin.md)
* [02_0 Configurare Bitcoin-Core VPS](02_0_Configurare_Bitcoin-Core_VPS.md)
  * [02_1 Configurare Bitcoin-Core_VPS in un colpo con StackScript](02_1_Configurare_Bitcoin-Core_VPS_con_StackScript.md)
  * [02_2 Configurare Bitcoin Core con Altri Mezzi](02_2_Configurare_Bitcoin_Core_Altri_Mezzi.md)

### SECONDA PARTE: USARE BITCOIN-CLI

**Status:** Prima traduzione completa, da revisionare.

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
  * [04_4 Inviare Monete con Transazione Grezza](04_4_Inviare_Monete_con_Transazione_Grezza.md)
  * [04_4 Intermezzo_Usare Curl](04_4_Intermezzo_Usare_Curl.md)
  * [04_5 Inviare Monete con Transazione Grezza Automatizzata](04_5_Inviare_Monete_con_Transazione_Grezza_Automatizzata.md)
  * [04_6 Creare una Transazione Segwit](04_6_Creare_una_Transazione_Segwit.md)
* [05_0 Controllare Transazioni Bitcoin](05_0_Controllare_Transazioni_Bitcoin.md)
  * [05_1 Guardare le Transazioni Ferme](05_1_Guardare_le_Transazioni_Ferme.md)
  * [05_2 Rinviare le Transazioni con RBF](05_2_Rinviare_le_Transazioni_con_RBF.md)
  * [05_3 Pagare una Transaccion con CPFP](05_3_Pagare_una_Transaccion_con_CPFP.md)
* [06_0 Ampliare le Transazioni Bitcoin con Multifirme](06_0_Ampliare_le_Transazioni_Bitcoin_con_Multifirme.md)
  * [06_1 Inviare una Transazione a un Indirizzo Multifirma](06_1_Inviare_una_Transazione_a_un_Indirizzo_Multifirma.md)
  * [06_2 Spendere una Transazione con un Indirizzo Multifirma](06_2_Spendere_una_Transazione_con_un_Indirizzo_Multifirma.md)
  * [06_3 Inviare e Ricevere una Multifirma Automatizzata](06_3_Inviare_e_Ricevere_una_Multifirma_Automatizzata.md)
* [07_0 Ampliare le Transazioni Bitcoin con PSBTs](07_0_Ampliare_le_Transazioni_Bitcoin_con_PSBTs.md)
  * [07_1 Creare una Transazione Bitcoin Parzialmente Firmata](07_1_Creare_una_Transazione_Bitcoin_Parzialmente_Firmata.md)
  * [07_2 Usare una Transazione Bitcoin Parzialmente Firmata](07_2_Usare_una_Transazione_Bitcoin_Parzialmente_Firmata.md)
  * [07_3 Integrazione con Hardware Wallets](07_3_Integrazione_con_Hardware_Wallets.md)
* [08_0 Ampliare Altre Transazioni Bitcoin](08_0_Ampliare_Altre_Transazioni_Bitcoin.md)
  * [08_1 Inviare una Transazione con Blocco temporale](08_1_Inviare_una_Transazione_con_Blocco_temporale.md)
  * [08_2 Inviare una Transazione con Dati](08_2_Inviare_una_Transazione_con_Dati.md)
  
### TERZA PARTE: BITCOIN SCRIPTING

**Status:** Prima traduzione parziale, da revisionare...

* 09_0_Introduzione_script_di_Bitcoin.md
  * 09_1_Le_basi_delle_transazioni.md
  * 09_2_Eseguire_uno_Script_di_Bitcoin.md
  * 09_3_Provare_uno_Script_Bitcoin.md
  * 09_4_Programmare_una_P2PKH.md
  * 09_5_Programmare_una_P2WPKH.md
* 10_0_Inserire_Scripts_di_Bitcoin_in_Transazioni_P2SH.md
  * 10_1_Comprendere_la_Base_di_P2SH.md
  * 10_2_Construire_la_Struttura_di_P2SH.md
  * 10_3_Eseguire_uno_Script_Bitcoin_con_P2SH.md
  * 10_4_Programmare_una_Multifirma.md
  * 10_5_Programmare_uno_Script_Segwit.md
  * 10_6_Spendere_una_Transazione_P2SH.md
* 11_0_Potenziare_Blocchi_Temporali_con_Scripts_di_Bitcoin.md
  * 11_1_Comprendere_le_Opzioni_dei_Blocchi_Temporali.md
  * 11_2_Usare_CLTV_negli_Scripts.md
  * 11_3_Usare_CSV_negli_Scripts.md
* 12_0_Ampliando_Scripts_di_Bitcoin.md
  * 12_1_Usare_Script_Condizionati.md
  * 12_2_Usare_Altri_Comandi_di_Scripting.md
* 13_0_Disegnare_Scripts_di_Bitcoin_Reali.md
  * 13_1_Scrivere_Scripts_Puzzle.md
  * 13_2_Scrivere_Scripts_Multifirma_Complessi.md
  * 13_3_Potenziare_Bitcoin_con_Scripts.md    

### QUARTA PARTE: RISERVATEZZA

**Status:** Da fare...

* 14_0_Usare_Tor.md
  * 14_1_Verificare_la_Configurazione_Tor.md
  * 14_2_Cambiare_Bitcoin_Hidden_Services.md
  * 14_3_Aggiungere_SSH_Hidden_Services.md
* 15_0_Usare_i2p.md
  * 15_1_Servizi_i2p.md

### QUINTA PARTE: PROGRAMMARE CON RPC

**Status:** Da fare...

* 16_0_Parlare_con_Bitcoind.md
  * 16_1_Accedere_a_Bitcoind_con_Librerie_RPC.md
  * 16_2_Programare_Bitcoind_in_C_con_Librerie_RPC.md
  * 16_3_Ricevere_Notifiche_di_Bitcoind_in_C_tramite_Librerie_ZMQ.md
* 17_0_Programmare_Bitcoin_con_Libwally.md
  * 17_1_Configurare_Libwally.md
  * 17_2_Usare_BIP39_in_Libwally.md
  * 17_3_Usare_BIP32_in_Libwally.md
  * 17_4_Usare_PSBTs_in_Libwally.md
  * 17_5_Usare_Scripts_in_Libwally.md
  * 17_6_Usare_Altre_Funzioni_in_Libwally.md
  * 17_7_Integrare_Libwally_e_Bitcoin-CLI.md
* 18_0_Parlare_a_Bitcoind_in_Altri_Linguaggi.md
  * 18_1_Accedere_a_Bitcoind_con_Go.md
  * 18_2_Accedere_a_Bitcoind_con_Java.md
  * 18_3_Accedere_a_Bitcoind_con_NodeJS.md
  * 18_4_Accedere_a_Bitcoind_con_Python.md
  * 18_5_Accedere_a_Bitcoind_con_Rust.md
  * 18_6_Accedere_a_Bitcoind_con_Swift.md

### SESTA PARTE: USARE LIGHTNING-CLI

**Status:** Da Fare...

* 19_0_Comprendere_La_Configurazione_Lightning.md
  * 19_1_Verificare_la_Configurazione_Lightning.md
  * 19_2_Comprendere_la_Configurazione_Lightning.md
  * 19_2_Intermezzo_Accedere_ad_un_Secondo_Nodo_Lightning.md
  * 19_3_Creare_un_Canale_in_Lightning.md
* 20_0_Usare_Lightning.md
  * 20_1_Generare_una_Richiesta_di_Pagamento.md
  * 20_2_Pagare_una_Fattura.md
  * 20_3_Chiudere_un_Canale_Lightning.md
  * 20_4_Espandere_la_Rete_Lightning.md

### APENDICE

**Status:** Da fare

* A0_Appendici.md
  * A1_0_Comprendere_Bitcoin_Standup.md
  * A2_0_Compilare_Bitcoin_dal_Codice_Fonte.md
  * A3_0_Usare_Bitcoin_Regtest.md

## Status - Beta

La versione v2.1.0 di **Learning Bitcoin from the Command Line** è completa e ha subito modifiche e integrazioni. È pronta per l'uso

Stiamo anche valutando in via provvisoria cosa potremmo includere in una [v3.0](TODO-30.md) del corso. Se desideri supportare un lavoro di questo tipo, diventa uno [sponsor GitHub](https://github.com/sponsors/BlockchainCommons) o supportaci sul nostro [BTCPay Server](https://btcpay.blockchaincommons.com/) e facci sapere che **Learning Bitcoin** è stato il motivo.

### Lo storico delle revisioni

#### 2.2.0 (Luglio 2024)
* [Traduzione in Italiano, lavori in corso](https://github.com/lutxoitalico/Learning-Bitcoin-from-the-Command-Line)
  
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

### Crediti

Le seguenti persone hanno contribuito direttamente a questo repository. Puoi aggiungere il tuo nome qui partecipando. Il primo passo è imparare come contribuire dalla nostra documentazione [CONTRIBUTING.md](./CONTRIBUTING.md).


| Name              | Role                | Github                                            | Email                                 | GPG Fingerprint                                    |
| ----------------- | ------------------- | ------------------------------------------------- | ------------------------------------- | -------------------------------------------------- |
| Christopher Allen | Lead Author | [@ChristopherA](https://github.com/ChristopherA) | \<ChristopherA@LifeWithAlacrity.com\> | FDFE 14A5 4ECB 30FC 5D22  74EF F8D3 6C91 3574 05ED |
| Shannon Appelcline |  Lead Author | [@shannona](https://github.com/shannona) | \<shannon.appelcline@gmail.com\> | 7EC6 B928 606F 27AD |


Di seguito sono elencati altri collaboratori:

| Role                | Names                                    |
| ------------------- | ---------------------------------------- |
| ***Contributors:*** | [gg2001](https://github.com/gg2001) (Go, Node.js sections), [gorazdko](https://github.com/gorazdko) (Rust section), [Javier Vargas](https://github.com/javiervargas) (C, Java, Lightning, Tor sections), [jodobear](https://github.com/jodobear) (Appendix: Compiling Bitcoin, Python section), [Prayank](    https://github.com/prayank23) (i2p sections)                               |
| ***Reviewers:***    | Glen Willem [@gwillem](https://github.com/gwillem) |
| ***Sponsors:***     | Blockstream Corporation                  |

### Crediti di traduzione

Grazie ai volontari che hanno dedicato molto tempo alla scrittura e alla revisione delle traduzioni in altre lingue del corso originale in lingua inglese.

#### Traduzione in portoghese

| Name              | Role                | Github                                            |
| ----------------- | ------------------- | ------------------------------------------------- | 
| Namcios | Translator & Reviewer | [@namcios](https://github.com/namcios) | 
| Korea | Translator & Reviewer | [@KoreaComK](https://github.com/KoreaComK) | 
| Luke Pavsky | Translator & Reviewer | [@lukedevj](https://github.com/lukedevj) | 
| hgrams | Translator & Reviewer | [@hgrams](https://github.com/hgrams) | 

#### Traduzione in Spagnolo

| Name              | Role                | Github                                            | 
| ---------- | -------- | ------------ |
| Ian Culp | Translator & Reviewer | [@icculp](https://github.com/icculp) |
| Maxi Goyheneche | Translator | [@maxcrowar](https://github.com/maxcrowar) |
| Said Rahal | Translator | [@srahalh](https://github.com/srahalh) |
| César A. Vallero | Translator & Reviewer | [@csralvall](https://github.com/csralvall) |
| Javier Vargas | Translator & Reviewer | [@javiervargas](https://github.com/javiervargas) |

#### Traduzione in Italiano

| Name              | Role                | Github                                            | 
| ---------- | -------- | ------------ |
| Lutxo | Translator | [@lutxoitalico](https://github.com/lutxoitalico) |

## Divulgazione responsabile

Vogliamo mantenere tutti i nostri software sicuri per tutti. Se hai scoperto una vulnerabilità di sicurezza, apprezziamo il tuo aiuto nel segnalarcela in modo responsabile. Purtroppo al momento non siamo in grado di offrire bug bounty.

Ti chiediamo di offrirci buona fede e di fare del tuo meglio per non far trapelare informazioni o danneggiare alcun utente, i suoi dati o la nostra community di sviluppatori. Ti preghiamo di darci un ragionevole lasso di tempo per risolvere il problema prima di pubblicarlo. Non frodare i nostri utenti o noi nel processo di scoperta. Ti promettiamo di non intraprendere azioni legali contro i ricercatori che segnalano un problema, a condizione che facciano del loro meglio per seguire queste linee guida.

### Segnalazione di una vulnerabilità

Segnala sospette vulnerabilità di sicurezza in privato tramite e-mail a ChristopherA@BlockchainCommons.com (non utilizzare questa e-mail per supporto). Ti preghiamo di NON creare problemi visibili pubblicamente per sospette vulnerabilità di sicurezza.

Le seguenti chiavi possono essere utilizzate per comunicare informazioni sensibili agli sviluppatori:

| Name              | Fingerprint                                        |
| ----------------- | -------------------------------------------------- |
| Christopher Allen | FDFE 14A5 4ECB 30FC 5D22  74EF F8D3 6C91 3574 05ED |

Puoi importare una chiave eseguendo il seguente comando con l'impronta digitale individuale: `gpg --recv-keys "<fingerprint>"` Assicurati di mettere le virgolette attorno alle impronte digitali che contengono spazi.
