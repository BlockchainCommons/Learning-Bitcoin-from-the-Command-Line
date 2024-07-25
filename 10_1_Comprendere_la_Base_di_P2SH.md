# 10.1: Comprendere le Basi di P2SH

Sai che gli Script di Bitcoin possono essere utilizzati per controllare il riscatto degli UTXO. Il prossimo passo è creare i tuoi Script... ma ciò richiede una tecnica molto specifica.

## Conoscere gli Standard di Bitcoin

Ecco la questione per l'uso degli Script di Bitcoin: per motivi di sicurezza, la maggior parte dei nodi Bitcoin accetterà solo sei tipi di transazioni Bitcoin "standard".

* __Pay to Public Key (P2PK)__ — Una transazione più vecchia, deprecata (`<pubKey> OP_CHECKSIG`) che è stata sostituita dalla maggiore sicurezza di P2PKH.
* __Pay to Public Key Hash (P2PKH)__ — Una transazione standard (`OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG`) che paga l'hash di una chiave pubblica.
* __Pay to Witness Public Key hash (P2WPKH)__ — Il più recente tipo di transazione a chiave pubblica. È semplicemente (`OP_0 <pubKeyHash>`) perché dipende dal consenso dei minatori per funzionare, come descritto nel [Capitolo 9.5](09_5_Programmare_una_P2WPKH.md).
* __Multisig__ — Una transazione per un gruppo di chiavi, come spiegato più dettagliatamente nel [Capitolo 10.4](10_4_Programmare_una_Multifirma.md).
* __Null Data__ — Una transazione non spendibile (`OP_RETURN Data`).
* __Pay to Script Hash (P2SH)__ — Una transazione che paga a uno script specifico, come spiegato più dettagliatamente qui.

Quindi come si scrive uno Script Bitcoin più complesso? La risposta è in quell'ultimo tipo di transazione standard, il P2SH. Puoi inserire qualsiasi tipo di script lungo e complesso in una transazione P2SH, e finché segui le regole standard per incorporare il tuo script e per riscattare i fondi, otterrai tutti i benefici degli Script Bitcoin.

> :warning: **AVVISO SULLA VERSIONE:** Gli script P2SH arbitrari sono diventati standard solo a partire da Bitcoin Core 0.10.0. Prima di allora, erano consentiti solo i P2SH Multisig.

## Comprendere lo Script P2SH

Hai già visto una transazione P2SH quando hai creato un multisig nel [Capitolo 6.1: Inviare una Transazione a un Indirizzo Multifirma](06_1_Inviare_una_Transazione_a_un_Indirizzo_Multifirma.md). Sebbene il multisig sia uno dei tipi di transazione standard, `bitcoin-cli` semplifica l'uso dei suoi multisig incorporandoli in transazioni P2SH, come descritto più dettagliatamente nel [Capitolo 10.4: Programmare una Multifirma](10_4_Programmare_una_Multifirma.md).

Quindi, guardiamo ancora una volta lo `scriptPubKey` di quel multisig P2SH:
```
  "scriptPubKey": {
    "asm": "OP_HASH160 a5d106eb8ee51b23cf60d8bd98bc285695f233f3 OP_EQUAL",
    "hex": "a914a5d106eb8ee51b23cf60d8bd98bc285695f233f387",
    "reqSigs": 1,
    "type": "scripthash",
    "addresses": [
      "2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr"
    ]
  }
```
  Lo script di blocco è piuttosto semplice: `OP_HASH160 a5d106eb8ee51b23cf60d8bd98bc285695f233f3 OP_EQUAL`. Come al solito, c'è un grosso pezzo di dati nel mezzo. Questo è un hash di un altro script di blocco nascosto (`redeemScript`) che sarà rivelato solo quando i fondi saranno riscattati. In altre parole, lo script di blocco standard per un indirizzo P2SH è: `OP_HASH160 <redeemScriptHash> OP_EQUAL`.

> :book: ***Cos'è un redeemScript?*** Ogni transazione P2SH porta con sé l'impronta di uno script di blocco nascosto sotto forma di un hash di 20 byte. Quando una transazione P2SH viene riscattata, il `redeemScript` completo (non hashato) è incluso come parte del `scriptSig`. Bitcoin verificherà che il `redeemScript` corrisponda all'hash; poi eseguirà effettivamente il `redeemScript` per vedere se i fondi possono essere spesi (o no).

Uno degli elementi interessanti delle transazioni P2SH è che né il mittente né la Blockchain sanno effettivamente cosa sia il `redeemScript`! Un mittente invia semplicemente a un indirizzo P2SH standardizzato contrassegnato con un prefisso "2" e non si preoccupa di come il destinatario recupererà i fondi alla fine.

> :link: **TESTNET vs MAINNET:** su testnet, il prefisso per gli indirizzi P2SH è `2`, mentre su mainnet è `3`.

## Comprendere Come Costruire uno Script P2SH

Poiché lo script di blocco visibile per una transazione P2SH è così semplice, creare una transazione di questo tipo è piuttosto semplice anche. In teoria. Tutto quello che devi fare è creare una transazione il cui script di blocco includa un hash di 20 byte del `redeemScript`. Questo hashing viene effettuato con il `OP_HASH160` standard di Bitcoin.

> :book: ***Cos'è OP_HASH160?*** L'operazione hash standard per Bitcoin esegue un hash SHA-256, poi un hash RIPEMD-160.

In generale, sono necessari quattro passaggi:

1. Crea uno script di blocco arbitrario con Bitcoin Script.
2. Crea una versione serializzata di quello script di blocco.
3. Esegui un hash SHA-256 su quei byte serializzati.
4. Esegui un hash RIPEMD-160 sui risultati di quell'hash SHA-256.

Ciascuno di questi passaggi ovviamente richiede del lavoro a sé, e alcuni di essi possono essere piuttosto intricati. La buona notizia è che non devi davvero preoccuparti di essi, perché sono sufficientemente complessi da far sì che di solito un'API si occupi di tutto per te.

Quindi, per ora, ti forniremo solo una panoramica, in modo che tu possa comprendere la metodologia generale. In [Capitolo 10.2: Construire la Struttura di P2SH](10_2_Construire_la_Struttura_di_P2SH.md) forniremo uno sguardo più approfondito alla creazione di script, nel caso tu voglia mai capire i dettagli di questo processo.

## Comprendere Come Inviare una Transazione Script P2SH

Quindi, come invii effettivamente la tua transazione P2SH? Di nuovo, la teoria è molto semplice:

1. Incorpora il tuo hash in uno script `OP_HASH160 <redeemScriptHash> OP_EQUAL`.
2. Traduci questo in codice esadecimale.
3. Usa quel codice esadecimale come tuo `scriptPubKey`. 
4. Crea il resto della transazione.

Purtroppo, questo è un altro caso in cui dovrai fare affidamento sulle API, in gran parte perché `bitcoin-cli` non fornisce alcun supporto per la creazione di transazioni P2SH. (Può riscattarle senza problemi.)

## Comprendere Come Sbloccare una Transazione Script P2SH

Il trucco per riscattare una transazione P2SH è che il destinatario deve aver salvato lo script di blocco serializzato segreto che è stato hashato per creare l'indirizzo P2SH. Questo è chiamato `redeemScript` perché è ciò che il destinatario deve avere per riscattare i suoi fondi.

Uno `scriptSig` di sblocco per una transazione P2SH è formato come: `... dati ... <redeemScript>`. I `dati` devono essere _solamente_ dati che vengono spinti nello stack, non operatori. ([BIP 16](https://github.com/bitcoin/bips/blob/master/bip-0016.mediawiki) li chiama firme, ma non è un requisito reale.)

> :warning: **AVVERTENZA:** Sebbene le firme non siano un requisito, uno script P2SH in realtà non è molto sicuro se non richiede almeno una firma nei suoi input. I motivi di ciò sono descritti in [§13.1: Scrivere Script Enigmi](13_1_Writing_Puzzle_Scripts.md).

Quando un UTXO viene riscattato, viene eseguito in due round di verifica:

1. Innanzitutto, il `redeemScript` nel `scriptSig` viene hashato e confrontato con lo script hashato nel `scriptPubKey`. 
2. Se corrispondono, inizia un secondo round di verifica.
3. In secondo luogo, il `redeemScript` viene eseguito utilizzando i dati precedenti che sono stati spinti nello stack. 
4. Se anche quel secondo round di verifica ha successo, l'UTXO viene sbloccato.

Mentre non puoi facilmente creare una transazione P2SH senza un'API, dovresti essere in grado di riscattare facilmente una transazione P2SH con `bitcoin-cli`. In effetti, lo hai già fatto in [Capiotolo 6.2:Spendere una Transazione con un Indirizzo Multifirma](06_2_Spendere_una_Transazione_con_un_Indirizzo_Multifirma.md). Il processo esatto è descritto nel [Capitolo 10.6: Spendere una Transazione P2SH](10_6_Spendere_una_Transazione_P2SH.md), dopo che avremo finito con tutte le complessità della creazione di transazioni P2SH.

> :warning: **AVVISO:** Puoi creare una transazione perfettamente valida con un redeemScript hashato correttamente, ma se il redeemScript non viene eseguito, o non viene eseguito correttamente, i tuoi fondi sono persi per sempre. Ecco perché è così importante testare i tuoi Script, come discusso nel [Capitlo 9.3: Provare uno Script Bitcoin](09_3_Provare_uno_Script_Bitcoin.md).

## Riepilogo: Comprendere le Basi di P2SH

Gli Script Bitcoin arbitrari non sono standard in Bitcoin. Tuttavia, puoi incorporarli in transazioni standard utilizzando il tipo di indirizzo P2SH. Basta hashare il tuo script come parte dello script di blocco, poi rivelarlo ed eseguirlo come parte dello script di sblocco. Finché puoi anche soddisfare il `redeemScript`, l'UTXO può essere speso.

> :fire: ***Qual è il potere di P2SH?*** Conosci già il potere dello Script Bitcoin, che ti consente di creare contratti intelligenti più complessi di ogni tipo. P2SH è ciò che sblocca effettivamente quel potere permettendoti di includere Script Bitcoin arbitrari nelle transazioni Bitcoin standard.

## Cosa Succede Dopo?

Continua a "Incorporare gli Script Bitcoin" col [Capitolo 10.2: Construire la Struttura di P2SH](10_2_Construire_la_Struttura_di_P2SH.md).
