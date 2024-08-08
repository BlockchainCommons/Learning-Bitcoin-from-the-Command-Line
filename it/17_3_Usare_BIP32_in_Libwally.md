# 17.3: Utilizzo di BIP32 in Libwally

> :information_source: **NOTA:** Questa sezione è stata recentemente aggiunta al corso ed è una bozza iniziale che potrebbe essere ancora in attesa di revisione. Attenzione lettore.

Nel [Capitolo 17.2](17_2_Usare_BIP39_in_Libwally.md), sei stato in grado di utilizzare l'entropia per generare un seed e il suo relativo mnemonico. Come ricorderai dal [Capitolo 3.5: Comprendere il Descriptor](03_5_Comprendere_il_Descriptor.md), un seed è la base di un Wallet Gerarchico Deterministico (HD), dove quel singolo seed può essere utilizzato per generare molti indirizzi. Quindi, come si passa dal seed agli indirizzi effettivi? È qui che entra in gioco [BIP32](https://en.bitcoin.it/wiki/BIP_0032).

## Creare una Radice HD

Per creare un indirizzo HD è necessario iniziare con un seed e poi scendere lungo la gerarchia fino al punto in cui si creano gli indirizzi.

Questo inizia abbastanza facilmente, basta generare un seed, cosa che hai già fatto nella sezione precedente:

```
  unsigned char entropy[16];  
  randombytes_buf(entropy, 16);

  char *mnem = NULL;
  lw_response = bip39_mnemonic_from_bytes(NULL,entropy,16,&mnem);
  
  unsigned char seed[BIP39_SEED_LEN_512];
  size_t seed_len;  
  lw_response = bip39_mnemonic_to_seed(mnem,NULL,seed,BIP39_SEED_LEN_512,&seed_len);
```
### Generare una Chiave Radice

Con un seed a portata di mano, puoi quindi generare una chiave master estesa con la funzione `bip32_key_from_seed_alloc` (o alternativamente `bip32_key_from_seed`, che non fa l'allocazione):

```
  struct ext_key *key_root;  
  lw_response = bip32_key_from_seed_alloc(seed,sizeof(seed),BIP32_VER_TEST_PRIVATE,0,&key_root);
```
Come puoi vedere, dovrai specificare quale versione della chiave restituire, in questo caso `BIP32_VER_TEST_PRIVATE`, una chiave privata per testnet.

> :link: **TESTNET vs MAINNET:** Su mainnet, invece, dovresti chiedere `BIP32_VER_MAIN_PRIVATE`.

### Generare xpub & xprv

Ogni volta che hai una chiave a portata di mano, puoi trasformarla in chiavi xpub o xprv per la distribuzione con il comando `bip32_key_to_base58`. Basta indicare se desideri una chiave `PRIVATE` (xprv) o `PUBLIC` (xpub):

```
  char *xprv;
  lw_response = bip32_key_to_base58(key_root, BIP32_FLAG_KEY_PRIVATE, &xprv);

  char *xpub;  
  lw_response = bip32_key_to_base58(key_root, BIP32_FLAG_KEY_PUBLIC, &xpub);
```

## Comprendere la Gerarchia

Prima di procedere, devi capire come funziona la gerarchia di un wallet HD. Come discusso in [§3.5](03_5_Understanding_the_Descriptor.md), un percorso di derivazione descrive l'albero che segui per arrivare a una chiave gerarchica, quindi `[0/1/0]` è il 0° figlio del 1° figlio del 0° figlio di una chiave radice. A volte parte di quella derivazione è contrassegnata con `'` o `h` per mostrare derivazioni hardened, che aumentano la sicurezza: `[0'/1'/0']`.

Tuttavia, per i wallet HD, ciascuno di questi livelli della gerarchia è utilizzato in un modo molto specifico. Questo è stato originariamente definito in [BIP44](https://github.com/bitcoin/bips/blob/master/bip-0044.mediawiki) ed è stato successivamente aggiornato per Segwit in [BIP84].

Nel complesso, un percorso di derivazione BIP32 è definito per avere cinque livelli:

1. **Scopo.** Questo è solitamente impostato su `44'` o `84'`, a seconda del BIP che viene seguito.
2. **Moneta.** Per i bitcoin su Mainnet, questo è `0'`, per testnet è `1'`.
3. **Account.** Un wallet può contenere più account distinti, a partire da `0'`.
4. **Cambio.** Gli indirizzi esterni (per la distribuzione) sono impostati su `0`, mentre gli indirizzi interni (per il cambio) sono impostati su `1`.
5. **Indice.** Il n° indirizzo per la gerarchia, a partire da `0`.

Quindi su testnet, il primo indirizzo per un indirizzo esterno per il primo account per le monete di testnet utilizzando gli standard BIP84 è `[m/84'/1'/0'/0/0]`. Questo è l'indirizzo che creerai tra poco.

> :link: **TESTNET vs MAINNET:** Per mainnet, sarebbe `[m/84'/0'/0'/0/0]`

### Comprendere la Gerarchia in Bitcoin Core

Utilizzeremo la gerarchia sopra per tutte le chiavi HD in Libwally, ma nota che questo standard non è utilizzato dal `bitcoin-cli` di Bitcoin Core, che invece utilizza `[m/0'/0'/0']` per il 0° indirizzo esterno e `[m/0'/1'/0']` per il 0° indirizzo di cambio.

## Generare un Indirizzo

Per generare un indirizzo, devi quindi scendere attraverso l'intera gerarchia.

### Generare una Chiave Account

Un modo per farlo è utilizzare la funzione `bip32_key_from_parent_path_alloc` per scendere diversi livelli di una gerarchia. Incapsuli i livelli in un array:
```
  uint32_t path_account[] = {BIP32_INITIAL_HARDENED_CHILD+84, BIP32_INITIAL_HARDENED_CHILD+1, BIP32_INITIAL_HARDENED_CHILD};
```
Qui guarderemo al primo figlio hardened (cioè l'account) o al secondo figlio hardened (cioè le monete di testnet) del 84° figlio hardened (cioè lo standard BIP84): `[m/84'/1'/0']`.

Puoi quindi utilizzare quel percorso per generare una nuova chiave dalla tua vecchia chiave:

```
  struct ext_key *key_account;
  lw_response = bip32_key_from_parent_path_alloc(key_root,path_account,sizeof(path_account),BIP32_FLAG_KEY_PRIVATE,&key_account);
```
Ogni volta che hai una nuova chiave, puoi usarla per generare nuove chiavi xprv e xpub, se lo desideri:
```
  lw_response = bip32_key_to_base58(key_account, BIP32_FLAG_KEY_PRIVATE, &a_xprv);
  lw_response = bip32_key_to_base58(key_account, BIP32_FLAG_KEY_PUBLIC, &a_xpub);
```


### Generare una Chiave di Indirizzo

In alternativa, puoi utilizzare la funzione `bip32_key_from_parent_alloc`, che scende solo di un livello della gerarchia alla volta. Il seguente esempio scende al 0° figlio della chiave account (che è l'indirizzo esterno) e poi al 0° figlio di quello. Questo sarebbe utile perché poi potresti continuare a generare il 1° indirizzo, il 2° indirizzo e così via da quella chiave esterna:

```
  struct ext_key *key_external;  
  lw_response = bip32_key_from_parent_alloc(key_account,0,BIP32_FLAG_KEY_PRIVATE,&key_external);

  struct ext_key *key_address;  
  lw_response = bip32_key_from_parent_alloc(key_external,0,BIP32_FLAG_KEY_PRIVATE,&key_address);
```
> :warning: **AVVISO:** A un certo punto in questa gerarchia, potresti decidere di generare `BIP32_FLAG_KEY_PUBLIC` invece di `BIP32_FLAG_KEY_PRIVATE`. Ovviamente questa decisione sarà basata sulla tua sicurezza e le tue necessità, ma ricorda che hai bisogno solo di una chiave pubblica per generare l'indirizzo effettivo.

### Generare un Indirizzo

Finalmente, sei pronto per generare un indirizzo dalla tua chiave finale. Tutto quello che devi fare è eseguire `wally_bip32_to_addr_segwit` usando la tua chiave finale e una descrizione del tipo di indirizzo che è.
.
```
  char *segwit;
  lw_response = wally_bip32_key_to_addr_segwit(key_address,"tb",0,&segwit);

  printf("[m/84'/1'/0'/0/0]: %s\n",segwit);
```  


> :link: **TESTNET vs MAINNET:** L'argomento `tb` definisce un indirizzo di testnet. Per mainnet invece usa `bc`.

Esiste anche una funzione `wally_bip32_key_to_address`, che può essere utilizzata per generare un indirizzo legacy o un indirizzo Segwit annidato.

## Testare il Codice HD

Il codice per questi esempi HD può, come al solito, essere trovato nella [directory src](src/17_3_genhd.c).

Puoi compilarlo e testarlo:

```
$ cc genhd.c -lwallycore -lsodium -o genhd
$ ./genhd
Mnemonic: behind mirror pond finish borrow wood park foam guess mail regular reflect
Root xprv key: tprv8ZgxMBicQKsPdLFXmZ6VegTxcmeieNpRUq8J2ahXxSaK2aF7CGqAc14ZADLjdHJdCr8oR2Zng9YH1x1A7EBaajQLVGNtxc4YpFejdE3wyj8
Root xpub key: tpubD6NzVbkrYhZ4WoHKfCm64685BoAeoi1L48j5K6jqNiNhs4VspfeknVgRLLiQJ3RkXiA9VxguUjmEwobtmrXNbhXsPHfm9W5HJR9DKRGaGJ2
Account xprv key: tprv8yZN7h6SPvJXrhAk56z6cwHQE6qZBRreB9fqqZJ1Xd1nLci3Rw8HTmqNkpFNgf3eZx8hYzhFWafUhHSt3HgF13aHvCE6kveS7gZAyfQwMDi
Account xpub key: tpubDWFQG78gYHzCkACXxkeh2LwWo8MVLm3YkTGd85LJwtpBB6xp4KwseGTEvxjeZNhnCNPdfZqRcgcZZAka4tD3xGS2J53WKHPMRhG357VKsqT
[m/84'/1'/0'/0/0]: tb1q0knqq26ek59pfl7nukzqr28m2zl5wn2f0ldvwu
```


## Riepilogo: Utilizzo di BIP32 in Libwally

Un wallet HD ti consente di generare un numero enorme di chiavi da un singolo seed. Ora sai come sono organizzate quelle chiavi sotto BIP44, BIP84 e Bitcoin Core e come derivarle, partendo da un seed o da parole mnemoniche.

> :fire: ***Qual è il potere di BIP32?*** Le chiavi sono l'elemento più difficile (e più pericoloso) della maggior parte delle operazioni crittografiche. Se le perdi, perdi ciò che la chiave proteggeva. BIP32 garantisce che devi conoscere solo una chiave, il seed, piuttosto che un enorme numero di chiavi diverse per indirizzi diversi.

> :fire: ***Qual è il potere di BIP32 in Libwally?*** Bitcoind già crea indirizzi basati su HD per te, il che significa che di solito non devi preoccuparti di derivare indirizzi in questo modo. Tuttavia, l'utilizzo delle funzioni BIP32 di Libwally può essere molto utile se hai una macchina offline dove devi derivare indirizzi, possibilmente basati su un seed passato da `bitcoind` al tuo dispositivo offline (o viceversa).

## Cosa c'è dopo?

Scopri di più su "Programming Bitcoin with Libwally" nel [Capitolo 17.4: Usare PSBTs in Libwally](17_4_Usare_PSBTs_in_Libwally.md).
