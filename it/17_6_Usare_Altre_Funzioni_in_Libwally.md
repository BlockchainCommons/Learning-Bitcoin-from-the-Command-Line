# 17.6: Usare Altre Funzioni in Libwally

> :information_source: **NOTA:** Questa sezione è stata recentemente aggiunta al corso ed è una bozza preliminare che potrebbe essere ancora in attesa di revisione. Attenzione lettore.

Libwally è una libreria estesa che offre una notevole quantità di funzionalità relative ai wallet, molte delle quali non sono disponibili tramite `bitcoin-cli`. Di seguito è fornita una panoramica di alcune funzionalità non trattate precedentemente in questo capitolo.

## Utilizzare Funzioni Criptografiche

Un certo numero di funzioni crittografiche possono essere accessibili direttamente da Libwally:

   * `wally_aes` — Utilizzare la crittografia o decrittografia AES
   * `wally_aes_cbc` — Utilizzare la crittografia o decrittografia AES in modalità CBC
   * `wally_hash160` — Utilizzare l'hash RIPEMD-160(SHA-256)
   * `wally_scrypt` — Utilizzare la derivazione della chiave Scrypt
   * `wally_sha256` — Utilizzare l'hash SHA256
   * `wally_sha256_midstate` — Utilizzare SHA256 per fare hash solo del primo blocco di dati
   * `wally_sha256d` — Effettuare un doppio hash SHA256
   * `wally_sha512` — Utilizzare l'hash SHA512

Ci sono anche funzioni HMAC per gli hash SHA, utilizzate per generare codici di autenticazione dei messaggi basati sugli hash. Sono usate in [BIP32](https://en.bitcoin.it/wiki/BIP_0032), tra le altre cose.

   * `wally_hmac_sha256`
   * `wally_hmac_sha512`
   
Funzioni aggiuntive coprono la derivazione della chiave PBKDF2 e la matematica delle curve ellittiche.

## Utilizzare Funzioni per gli Indirizzi

Libwally contiene un certo numero di funzioni che possono essere utilizzate per importare, esportare e tradurre gli indirizzi Bitcoin.

Alcune convertono tra indirizzi e bytes `scriptPubKey`:

   * `wally_addr_segwit_from_bytes` — Convertire un programma witness (in bytes) in un indirizzo Segwit
   * `wally_addr_segwit_to_bytes` — Convertire un indirizzo Segwit in un `scriptPubKey` (in bytes)
   * `wally_address_to_scriptpubkey` — Convertire un indirizzo legacy in un `scriptPubKey` (in bytes)
   * `wally_scriptpubkey_to_address` — Convertire un `scriptPubKey` (in bytes) in un indirizzo legacy
   
Alcuni riguardano il formato di importazione del wallet (WIF):

   * `wally_wif_from_bytes` — Convertire una chiave privata (in bytes) in un WIF
   * `wally_wif_is_uncompressed` — Determina se un WIF è non compresso
   * `wally_wif_to_address` — Derivare un indirizzo P2PKH da un WIF
   * `wally_wif_to_bytes` — Convertire un WIF in una chiave privata (in bytes)
   * `wally_wif_to_public_key` — Derivare una chiave pubblica (in bytes) da un WIF
   
## Utilizzare Funzioni BIP32

Ci sono ulteriori funzioni HD-wallet BIP32, oltre a quelle trattate nel [Capitolo 17.3: Usare BIP32 in Libwally](17_3_Usare_BIP32_in_Libwally.md).

   * `bip32_key_get_fingerprint` — Generare un'impronta BIP32 per una chiave estesa
   * `bip32_key_serialize` — Trasformare una chiave estesa in bytes serializzati
   * `bip32_key_strip_private_key` — Convertire una chiave privata estesa in una chiave pubblica estesa
   * `bip32_key_unserialize` — Trasformare bytes serializzati in una chiave estesa

Ci sono anche numerose altre a seconda che tu voglia allocare memoria o far fare a Libwally l'_alloc_ per te.

## Utilizzare Funzioni BIP38

[BIP38](https://github.com/bitcoin/bips/blob/master/bip-0038.mediawiki) consente la creazione di chiavi private protette da password. Non lo insegniamo perché consideriamo pericoloso inserire questo tipo di fattore umano nella gestione delle chiavi. Vedi [#SmartCustody](https://www.smartcustody.com/index.html).

Le principali funzioni sono:

   * `bip38_from_private_key` — Codificare una chiave privata utilizzando BIP38
   * `bip38_to_private_key` — Decodificare una chiave privata utilizzando BIP38
   
## Utilizzare Funzioni BIP39

Alcune funzioni mnemoniche BIP39 sono state appena accennate nel [Capitolo 17.2: Usare BIP39 in Libwally](17_2_Usare_BIP39_in_Libwally.md):

   * `bip39_get_languages` — Vedere un elenco delle lingue supportate
   * `bit39_get_word` — Recuperare una parola specifica dall'elenco delle parole di una lingua
   * `bip39_get_wordlist` — Vedere un elenco di parole per una lingua
   
## Utilizzare Funzioni PSBT

L'elenco della maggior parte delle funzioni PSBT può essere trovato nel [Capitolo 17.4: Usare PSBTs in Libwally](17_4_Using_PSBTs_in_Libwally.md).

## Utilizzare Funzioni per gli Script

[Capitolo 17.5: Usare Scripts in Libwally](17_5_Usare_Scripts_in_Libwally.md) ha appena accennato alle funzioni degli Script di Libwally.

C'è un'altra funzione che ti permette di determinare il tipo di script trovato in una transazione:

   * `wally_scriptpubkey_get_type` — Determinare il tipo di script di una transazione.

Poi ci sono una serie di funzioni che creano `scriptPubKey` da bytes, `scriptSig` da firme e Witnesses da bytes o firme.

   * `wally_script_push_from_bytes`
   * `wally_scriptpubkey_csv_2of2_then_1_from_bytes`
   * `wally_scriptpubkey_csv_2of3_then_2_from_bytes`
   * `wally_scriptpubkey_multisig_from_bytes`
   * `wally_scriptpubkey_op_return_from_bytes`
   * `wally_scriptpubkey_p2pkh_from_bytes`
   * `wally_scriptpubkey_p2sh_from_bytes`
   * `wally_scriptsig_multisig_from_bytes`
   * `wally_scriptsig_p2pkh_from_der`
   * `wally_scriptsig_p2pkh_from_sig`
   * `wally_witness_multisig_from_bytes`
   * `wally_witness_p2wpkh_from_der`
   * `wally_witness_p2wpkh_from_sig`
   * `wally_witness_program_from_bytes`

## Utilizzare Funzioni per le Transazioni

Abbiamo appena accennato alle funzioni che possono essere utilizzate per creare e convertire transazioni nel [Capitolo 17.5: Usare Scripts in Libwally](17_5_Usare_Scripts_in_Libwally.md).

Ci sono numerose funzioni informative, alcune delle più interessanti sono:

   * `wally_tx_get_length`
   * `wally_tx_get_total_output_satoshi`
   * `wally_tx_get_weight`
   
Ci sono anche funzioni che influenzano un `wally_tx`, un `wally_tx_input`, un `wally_tx_output`, o uno `stack di testimoni` e che creano firme.

## Utilizzare Funzioni Elements

Libwally può essere compilato per essere utilizzato con Elements di Blockstream, che include l'accesso alle sue funzioni per gli asset.

## Riepilogo: Utilizzare Altre Funzioni in Libwally

C'è molto di più che puoi fare con Libwally, più di quanto possa essere trattato in questo capitolo o persino elencato in questa sezione. In particolare, puoi eseguire funzioni crittografiche, codificare chiavi private, costruire transazioni complete e utilizzare Elements. La [documentazione di Libwally](https://wally.readthedocs.io/en/latest/) è il posto giusto per ulteriori informazioni, anche se al momento della scrittura sono sia limitate che obsolete. I file di intestazione di Libwally sono una riserva se la documentazione è incompleta o errata.

## Cosa c'è dopo?

Continua a imparare su "Programmazione Bitcoin con Libwally" nel [Capitolo 17.7: Integrare Libwally e Bitcoin-CLI](17_7_Integrare_Libwally_e_Bitcoin-CLI.md)
