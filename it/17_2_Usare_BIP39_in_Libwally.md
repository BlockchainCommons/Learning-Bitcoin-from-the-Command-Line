# 17.2: Utilizzare BIP39 in Libwally

> :information_source: **NOTA:** Questa sezione è stata recentemente aggiunta al corso ed è una bozza iniziale che potrebbe ancora essere in attesa di revisione. Lettore avvisato.

Uno dei maggiori poteri di Libwally è che può mettere a nudo il lavoro sottostante di generazione di semi, chiavi private e, in ultima analisi, indirizzi. Per cominciare, supporta [BIP39](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki), che è il BIP che definisce i codici mnemonici per Bitcoin — qualcosa che finora non è supportato da Bitcoin Core.

> :book: ***Cos'è un Codice Mnemonico?*** Gli indirizzi Bitcoin (e le relative chiavi private e semi sottostanti) sono lunghe liste incomprensibili di caratteri e numeri, che non solo sono impossibili da ricordare, ma anche facili da digitare erroneamente. I codici mnemonici sono una soluzione a questo problema che consente agli utenti di registrare 12 (o 24) parole nella loro lingua — qualcosa di molto meno soggetto a errori. Questi codici possono quindi essere utilizzati per ripristinare completamente un seme BIP32 che è la base di un portafoglio HD.

> :book: ***Cos'è un Seme?*** Abbiamo toccato brevemente l'argomento seme nel [Capitolo 3.5: Comprendere il Descriptor](03_5_Comprendere_il_Descriptor.md). È il numero casuale che viene utilizzato per generare un'intera sequenza di chiavi private (e quindi indirizzi) in un portafoglio HD. Torneremo sui semi nella sezione successiva, che riguarda i portafogli HD e Libwally. Per ora, sappi solo che un codice mnemonico BIP39 corrisponde al seme per un portafoglio deterministico gerarchico BIP32.

## Creare Codici Mnemonici

Tutte le chiavi Bitcoin iniziano con l'entropia. Questo primo utilizzo di Libwally e dei suoi mnemonici BIP39 mostra quindi come generare entropia e ottenere un codice mnemonico da essa.

> :book: ***Cos'è l'Entropia?*** L'entropia è un modo sofisticato di dire casualità, ma è una casualità attentamente misurata che viene utilizzata come base di un generatore di numeri veramente casuali (TRG). È misurata in "bit", con più bit di entropia che risultano in maggiore casualità (e quindi maggiore protezione per ciò che viene generato). Per Bitcoin, l'entropia è la base del tuo seme, che in un portafoglio HD genera tutti i tuoi indirizzi.

Inizierai sempre il lavoro con Libwally inizializzando la libreria e testando i risultati, come dimostrato per la prima volta nel [Capitolo 17.1](17_1_Configurare_Libwally.md):

```
  int lw_response;

  lw_response = wally_init(0);

  if (lw_response) {

    printf("Error: Wally_init failed: %d\n",lw_response);
    exit(-1);
    
  }
```
Ora sei pronto per entropizzare.

### Creare Entropia

Usando `libsodium`, puoi creare entropia con il comando `randombytes_buf`:

```
  unsigned char entropy[16];  
  randombytes_buf(entropy, 16);
```
Questo esempio, che sarà l'unico modo in cui utilizzeremo la libreria `libsodium`, crea 16 byte di entropia. In generale, per creare un codice mnemonico sicuro, dovresti usare tra 128 e 256 bit di entropia, ovvero 16-32 byte.

>:warning: **ATTENZIONE:** Ancora una volta, assicurati di essere molto a tuo agio con il tuo metodo di generazione dell'entropia prima di usarlo in un programma reale.

### Tradurre in un Mnemonico

16 byte di entropia sono sufficienti per creare un codice mnemonico a 12 caratteri, che viene fatto con la funzione `bip39_mnemonic_from_bytes` di Libwally::
```
  char *mnem = NULL;
  lw_response = bip39_mnemonic_from_bytes(NULL,entropy,16,&mnem);
```
Nota che devi passare la dimensione del byte, quindi se dovessi aumentare la dimensione della tua entropia, per generare una frase mnemonica più lunga, dovresti anche aumentare il valore in questa funzione.

> **NOTA:** Esistono elenchi di parole mnemoniche per diverse lingue! Il valore predefinito è utilizzare l'elenco in lingua inglese, che è la variabile `NULL` in questi comandi mnemonici di Libwally, ma puoi richiedere alternativamente una lingua diversa!

Ecco fatto! Hai creato una frase mnemonica!

>:book: ***Come viene creata la Frase Mnemonica?*** Puoi impararlo in [BIP39](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki), ma se preferisci, Greg Walker ha un [ottimo esempio](https://learnmeabitcoin.com/technical/mnemonic): in pratica, aggiungi un checksum, poi converti ogni serie di 11 bit in una parola dall'elenco delle parole. Puoi farlo con i comandi `bip39_get_wordlist` e `bip39_get_word` se non ti fidi del comando `bip39_mnemonic_from_bytes`.

### Tradurre in un Seme

Ci sono alcune funzioni, come `bip32_key_from_seed` (che incontreremo nella sezione successiva) che richiedono di avere il seme piuttosto che il Mnemonico. Le due cose sono funzionalmente identiche: se hai il seme, puoi generare il mnemonico e viceversa.

Se hai bisogno di generare il seme dal tuo mnemonico, devi solo usare il comando `bip39_mnemonic_to_seed`:

```
  unsigned char seed[BIP39_SEED_LEN_512];
  size_t seed_len;
  
  lw_response = bip39_mnemonic_to_seed(mnem,NULL,seed,BIP39_SEED_LEN_512,&seed_len);
```
Nota che tutti i semi BIP39 sono attualmente 512 byte; tuttavia, devi impostare correttamente la dimensione della tua variabile e passare tale dimensione a `bip39_mnemonic_to_seed`.

### Stampare il Tuo Seme

Se vuoi vedere come appare il tuo seme in esadecimale, puoi usare la funzione `wally_hex_from_bytes` per trasformare il tuo seme in un codice esadecimale leggibile (ma non ottimale per le persone):
```
  char *seed_hex;
  wally_hex_from_bytes(seed,sizeof(seed),&seed_hex);
  printf("Seed: %s\n",seed_hex);
```
Se hai fatto tutto correttamente, dovresti ottenere un seme di 64 byte. (Questa è la variabile `BIP39_SEED_LEN_512` che hai usato, che definisce una lunghezza predefinita del seme di 512 bit o 64 byte.)

> :warning: **ATTENZIONE:** Dovresti assolutamente verificare che la lunghezza del tuo seme sia di 64 byte in qualche modo, perché è facile sbagliare, ad esempio utilizzando il tipo di variabile sbagliato quando esegui `bip39_mnemonic_to_seed`.

## Testare il Codice Mnemonico

Il codice completo per generare entropia, generare un mnemonico BIP39, convalidare il mnemonico e generare un seme può essere trovato nella [directory src](src/17_2_genmnemonic.c). Scaricalo e compila:

```
$ cc genmnemonic.c -lwallycore -lsodium -o genmnemonic
```
Poi puoi eseguire il test:
```
Mnemonic: parent wasp flight sweet miracle inject lemon matter label column canyon trend
Mnemonic validated!
Seed: 47b04cfb5d8fd43d371497f8555a27a25ca0a04aafeb6859dd4cbf37f6664b0600c4685c1efac29c082b1df29081f7a46f94a26f618fc6fd38d8bc7b6cd344c7
```

## Riepilogo: Utilizzare BIP39 in Libwally

BIP39 ti consente di generare un insieme di 12-24 parole mnemoniche da un seme (e la libreria Libwally ti consente anche di convalidarlo!).

> :fire: ***Qual è la potenza di BIP39?*** I semi e le chiavi private di Bitcoin sono soggetti a tutti i tipi di perdite. Se sbagli una sola cifra, i tuoi soldi sono persi per sempre. Le parole mnemoniche sono un modo molto più user-friendly di rappresentare gli stessi dati, ma poiché sono parole nella lingua scelta dall'utente, sono meno soggette a errori. Il potere di BIP39 è quindi migliorare l'accessibilità, l'usabilità e la sicurezza di Bitcoin.

> :fire: ***Qual è la potenza di BIP39 in Libwally?*** Bitcoind attualmente non supporta le parole mnemoniche, quindi usare Libwally può permetterti di generare parole mnemoniche in combinazione con gli indirizzi detenuti da `bitcoind` (anche se come vedremo nel §17.7, richiede un po' di lavoro al momento per importare le tue chiavi in Bitcoin Core).

## Cosa c'è dopo?

Scopri di più su "Programmare Bitcoin con Libwally" nel [Capitolo 17.3: Usare BIP32 in Libwally](17_3_Usare_BIP32_in_Libwally.md).
