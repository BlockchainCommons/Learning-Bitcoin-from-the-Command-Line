# 17.4: Utilizzo di PSBTs in Libwally

> :information_source: **NOTA:** Questa sezione è stata recentemente aggiunta al corso ed è una bozza iniziale che potrebbe essere ancora in attesa di revisione. Attenzione lettore.

Hai imparato tutto sulle Transazioni Bitcoin Parzialmente Firmate (PSBT) in [§7.1](07_1_Creating_a_Partially_Signed_Bitcoin_Transaction.md) e [§7.2](07_2_Using_a_Partially_Signed_Bitcoin_Transaction.md), e come hai visto in [§7.3: Integrating with Hardware Wallets](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/07_3_Integrating_with_Hardware_Wallets.md), uno dei loro principali vantaggi è la possibilità di integrarsi con nodi offline, come i Portafogli Hardware. HWI ti permetteva di inviare comandi a un Portafoglio Hardware, ma cosa utilizza il portafoglio stesso per gestire le PSBT? Come si scopre, può utilizzare qualcosa come Libwally, come dimostra questa sezione.

Fondamentalmente, Libwally ha tutta la funzionalità delle PSBT, quindi se c'è qualcosa che potresti fare con `bitcoind`, potresti farlo anche usando Libwally, anche se il tuo dispositivo è offline. Quello che segue è la più semplice introduzione a un argomento molto complesso.

## Convertire una PSBT

Convertire una PSBT nella struttura interna di Libwally è incredibilmente facile, basta eseguire `wally_psbt_from_base64` con una PSBT in base64 — che sono gli output prodotti da `bitcoin-cli`, come:

`cHNidP8BAJoCAAAAAri6BLjKQZGO9Y1iVIYbxlxBJ2kqsTPWnxGaH4HrSjxbAAAAAAD+////leV0hwJ0fO40RmhuFVIYtO16ktic2J4vJFLAsT5TM8cBAAAAAP7///8CYOMWAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBU+gBwAAAAAAFgAU9Ojd5ds3CJi1fIRWbj92CYhQgX0AAAAAAAEBH0BCDwAAAAAAFgAUABk8i/Je8Fb41FcaHD9lEj5f54giBgMBaNlILisC1wJ/tKie3FStqhrfcJM09kfQobBTOCiuxRiaHVILVAAAgAEAAIAAAACAAAAAADkCAAAAAQEfQEIPAAAAAAAWABQtTxOfqohTBNFWFqFm0tUVdK9KXSIGAqATz5xLX1aJ2SUwNqPkd8+YaJYm94FMlPCScm8Rt0GrGJodUgtUAACAAQAAgAAAAIAAAAAAAAAAAAAAIgID2UK1nupSfXC81nmB65XZ+pYlJp/W6wNk5FLt5ZCSx6kYmh1SC1QAAIABAACAAAAAgAEAAAABAAAAAA==`

Tuttavia, è un po' più difficile gestire il risultato, perché Libwally lo converte in una struttura `wally_psbt` molto complessa.

Ecco come è definita in `/usr/include/wally_psbt.h`:

```
struct wally_psbt {
    unsigned char magic[5];
    struct wally_tx *tx;
    struct wally_psbt_input *inputs;
    size_t num_inputs;
    size_t inputs_allocation_len;
    struct wally_psbt_output *outputs;
    size_t num_outputs;
    size_t outputs_allocation_len;
    struct wally_unknowns_map *unknowns;
    uint32_t version;
};

struct wally_psbt_input {
    struct wally_tx *non_witness_utxo;
    struct wally_tx_output *witness_utxo;
    unsigned char *redeem_script;
    size_t redeem_script_len;
    unsigned char *witness_script;
    size_t witness_script_len;
    unsigned char *final_script_sig;
    size_t final_script_sig_len;
    struct wally_tx_witness_stack *final_witness;
    struct wally_keypath_map *keypaths;
    struct wally_partial_sigs_map *partial_sigs;
    struct wally_unknowns_map *unknowns;
    uint32_t sighash_type;
};

struct wally_psbt_output {
    unsigned char *redeem_script;
    size_t redeem_script_len;
    unsigned char *witness_script;
    size_t witness_script_len;
    struct wally_keypath_map *keypaths;
    struct wally_unknowns_map *unknowns;
};
```
Questi a loro volta utilizzano alcune strutture di transazione definite in `/usr/include/wally_transaction.h`:
```
struct wally_tx {
    uint32_t version;
    uint32_t locktime;
    struct wally_tx_input *inputs;
    size_t num_inputs;
    size_t inputs_allocation_len;
    struct wally_tx_output *outputs;
    size_t num_outputs;
    size_t outputs_allocation_len;
};

struct wally_tx_output {
    uint64_t satoshi;
    unsigned char *script;
    size_t script_len;
    uint8_t features;
};
```
C'è molto! Anche se gran parte di questo dovrebbe essere familiare dai capitoli precedenti, è un po' travolgente vederlo tutto disposto in strutture C.

## Leggere una PSBT Convertita

Ovviamente, puoi leggere qualsiasi cosa da una struttura PSBT richiamando i singoli elementi dalle varie sotto-strutture. Quanto segue è una breve panoramica che mostra come recuperare alcuni degli elementi.

Ecco un esempio di recupero dei valori e `scriptPubKeys` degli input:

```
  int inputs = psbt->num_inputs;
  printf("TOTAL INPUTS: %i\n",inputs);

  for (int i = 0 ; i < inputs ; i++) {
    printf("\nINPUT #%i: %i satoshis\n",i, psbt->inputs[i].witness_utxo->satoshi);

    char *script_hex;
    wally_hex_from_bytes(psbt->inputs[i].witness_utxo->script,psbt->inputs[i].witness_utxo->script_len,&script_hex);
    printf("scriptPubKey: %s\n",script_hex);
    wally_free_string(script_hex);
  }
```
Questo schema di programmazione sarà utilizzato su molte parti della PSBT. Guardi la dimensione dell'array degli input, poi lo attraversi, recuperando ciò che vuoi vedere (in questo caso, satoshi e script).

Ecco un esempio simile per gli output:

```
  int outputs = psbt->num_outputs;
  printf("\nTOTAL OUTPUTS: %i\n",outputs);
  for (int i = 0 ; i < outputs ; i++) {
    char *pubkey_hex;
    wally_hex_from_bytes(psbt->tx->outputs[i].script,psbt->tx->outputs[i].script_len,&pubkey_hex);
    printf("\nINPUT #%i\n",i);
    printf("scriptPubKey: %s\n",pubkey_hex);
    wally_free_string(pubkey_hex);    
  }
```
Ovviamente, c'è molto altro che potresti guardare nelle PSBT. Infatti, guardare è il punto principale di una PSBT: puoi verificare input e output da un computer offline.

> :warning: **AVVISO:** Queste funzioni di lettura sono _molto_ rudimentali e non funzioneranno correttamente in situazioni estremamente normali come un input o output che è ancora vuoto o che include un `non_witness_utxo`. Si verificheranno errori se non viene fornita una PSBT precisamente come previsto. Un lettore reale dovrebbe essere considerevolmente più robusto, per coprire tutte le possibili situazioni, ma questo è lasciato come esercizio per il lettore.

### Testare il Tuo Lettore PSBT

Ancora una volta, il codice per questo (estremamente rudimentale e specifico) lettore PSBT è nella [directory src](src/17_4_examinepsbt.c).

Puoi compilarlo come al solito:

```
$ cc examinepsbt.c -lwallycore -o examinepsbt
```
La seguente PSBT dal [Capitolo: 7.3](07_3_Integrazione_con_Hardware_Wallets.md) può essere utilizzata per i test, poiché corrisponde ai criteri molto ristretti richiesti da questa implementazione limitata:
```
psbt=cHNidP8BAJoCAAAAAri6BLjKQZGO9Y1iVIYbxlxBJ2kqsTPWnxGaH4HrSjxbAAAAAAD+////leV0hwJ0fO40RmhuFVIYtO16ktic2J4vJFLAsT5TM8cBAAAAAP7///8CYOMWAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBU+gBwAAAAAAFgAU9Ojd5ds3CJi1fIRWbj92CYhQgX0AAAAAAAEBH0BCDwAAAAAAFgAUABk8i/Je8Fb41FcaHD9lEj5f54giBgMBaNlILisC1wJ/tKie3FStqhrfcJM09kfQobBTOCiuxRiaHVILVAAAgAEAAIAAAACAAAAAADkCAAAAAQEfQEIPAAAAAAAWABQtTxOfqohTBNFWFqFm0tUVdK9KXSIGAqATz5xLX1aJ2SUwNqPkd8+YaJYm94FMlPCScm8Rt0GrGJodUgtUAACAAQAAgAAAAIAAAAAAAAAAAAAAIgID2UK1nupSfXC81nmB65XZ+pYlJp/W6wNk5FLt5ZCSx6kYmh1SC1QAAIABAACAAAAAgAEAAAABAAAAAA==
```
Esegui `examinepsbt` con quella PSBT, e dovresti vedere gli script sugli input e gli output:
```
$ ./examinepsbt $psbt
TOTAL INPUTS: 2

INPUT #0: 1000000 satoshis
scriptPubKey: 001400193c8bf25ef056f8d4571a1c3f65123e5fe788

INPUT #1: 1000000 satoshis
scriptPubKey: 00142d4f139faa885304d15616a166d2d51574af4a5d

TOTAL OUTPUTS: 2

INPUT #0
scriptPubKey: 0014c772d6f95542e11ef11e8efc7c7a69830ad38a05

INPUT #1
scriptPubKey: 0014f4e8dde5db370898b57c84566e3f76098850817d
```
E ovviamente, puoi verificare questo con il comando RPC `decodepsbt` per `bitcoin-cli`:
```
$ bitcoin-cli decodepsbt $psbt
{
  "tx": {
    "txid": "45f996d4ff8c9e9ab162f611c5b6ad752479ede9780f9903bdc80cd96619676d",
    "hash": "45f996d4ff8c9e9ab162f611c5b6ad752479ede9780f9903bdc80cd96619676d",
    "version": 2,
    "size": 154,
    "vsize": 154,
    "weight": 616,
    "locktime": 0,
    "vin": [
      {
        "txid": "5b3c4aeb811f9a119fd633b12a6927415cc61b8654628df58e9141cab804bab8",
        "vout": 0,
        "scriptSig": {
          "asm": "",
          "hex": ""
        },
        "sequence": 4294967294
      },
      {
        "txid": "c733533eb1c052242f9ed89cd8927aedb41852156e684634ee7c74028774e595",
        "vout": 1,
        "scriptSig": {
          "asm": "",
          "hex": ""
        },
        "sequence": 4294967294
      }
    ],
    "vout": [
      {
        "value": 0.01500000,
        "n": 0,
        "scriptPubKey": {
          "asm": "0 c772d6f95542e11ef11e8efc7c7a69830ad38a05",
          "hex": "0014c772d6f95542e11ef11e8efc7c7a69830ad38a05",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1qcaedd724gts3aug73m78c7nfsv9d8zs9q6h2kd"
          ]
        }
      },
      {
        "value": 0.00499791,
        "n": 1,
        "scriptPubKey": {
          "asm": "0 f4e8dde5db370898b57c84566e3f76098850817d",
          "hex": "0014f4e8dde5db370898b57c84566e3f76098850817d",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1q7n5dmewmxuyf3dtus3txu0mkpxy9pqtacuprak"
          ]
        }
      }
    ]
  },
  "unknown": {
  },
  "inputs": [
    {
      "witness_utxo": {
        "amount": 0.01000000,
        "scriptPubKey": {
          "asm": "0 00193c8bf25ef056f8d4571a1c3f65123e5fe788",
          "hex": "001400193c8bf25ef056f8d4571a1c3f65123e5fe788",
          "type": "witness_v0_keyhash",
          "address": "tb1qqqvnezljtmc9d7x52udpc0m9zgl9leugd2ur7y"
        }
      },
      "bip32_derivs": [
        {
          "pubkey": "030168d9482e2b02d7027fb4a89edc54adaa1adf709334f647d0a1b0533828aec5",
          "master_fingerprint": "9a1d520b",
          "path": "m/84'/1'/0'/0/569"
        }
      ]
    },
    {
      "witness_utxo": {
        "amount": 0.01000000,
        "scriptPubKey": {
          "asm": "0 2d4f139faa885304d15616a166d2d51574af4a5d",
          "hex": "00142d4f139faa885304d15616a166d2d51574af4a5d",
          "type": "witness_v0_keyhash",
          "address": "tb1q948388a23pfsf52kz6skd5k4z4627jja2evztr"
        }
      },
      "bip32_derivs": [
        {
          "pubkey": "02a013cf9c4b5f5689d9253036a3e477cf98689626f7814c94f092726f11b741ab",
          "master_fingerprint": "9a1d520b",
          "path": "m/84'/1'/0'/0/0"
        }
      ]
    }
  ],
  "outputs": [
    {
    },
    {
      "bip32_derivs": [
        {
          "pubkey": "03d942b59eea527d70bcd67981eb95d9fa9625269fd6eb0364e452ede59092c7a9",
          "master_fingerprint": "9a1d520b",
          "path": "m/84'/1'/0'/1/1"
        }
      ]
    }
  ],
  "fee": 0.00000209
}
```
Puoi vedere chiaramente i satoshi di input e `scriptPubKey` elencati negli `input` e i nuovi `scriptPubKey` nei `vout` del `tx`.

Quindi, è tutto lì per il tuo recupero!

## Creare una PSBT

Come notato all'inizio di questa sezione, tutte le funzioni necessarie per creare e processare PSBT sono disponibili in Libwally. In realtà, eseguire il processo di creazione è così complesso che va oltre lo scopo di questa sezione, ma ecco una rapida panoramica delle funzioni necessarie. Nota che i [documenti](https://wally.readthedocs.io/en/latest/psbt/) sono obsoleti per PSBT, quindi dovrai consultare `/usr/include/wally_psbt.h` per informazioni complete.

Come discusso nel [Capitolo 7.1](07_1_Creare_una_Transazione_Bitcoin_Parzialmente_Firmata.md), ci sono diversi ruoli coinvolti nella creazione delle PSBT

### Assumere il Ruolo del Creatore

Il ruolo del creatore è incaricato di creare una PSBT con almeno un input.

Una PSBT è creata con un semplice utilizzo di `wally_psbt_init_alloc`, dicendo quante input e output aggiungerai alla fine:
:
```
  struct wally_psbt *psbt;
  lw_response = wally_psbt_init_alloc(0,1,1,0,&psbt);
```

Ma ciò che hai non è ancora una PSBT legale, a causa della mancanza di input. Puoi crearli creando una transazione e impostandola come transazione globale nella PSBT, che aggiorna tutti gli input e output:

```
  struct wally_tx *gtx;
  lw_response = wally_tx_init_alloc(0,0,1,1,&gtx);
  lw_response = wally_psbt_set_global_tx(psbt,gtx);
```
### Testare la Creazione della Tua PSBT

A questo punto, dovresti avere una PSBT vuota, ma funzionante, che puoi vedere compilando ed eseguendo [il programma](src/17_4_createemptypsbt.c).

```
$ cc createemptypsbt.c -lwallycore -o createemptypsbt
$ ./createemptypsbt 
cHNidP8BAAoAAAAAAAAAAAAAAA==
```
Puoi persino utilizzare `bitcoin-cli` per testare il risultato:
```
$ psbt=$(./createpsbt)
$ bitcoin-cli decodepsbt $psbt
{
  "tx": {
    "txid": "f702453dd03b0f055e5437d76128141803984fb10acb85fc3b2184fae2f3fa78",
    "hash": "f702453dd03b0f055e5437d76128141803984fb10acb85fc3b2184fae2f3fa78",
    "version": 0,
    "size": 10,
    "vsize": 10,
    "weight": 40,
    "locktime": 0,
    "vin": [
    ],
    "vout": [
    ]
  },
  "unknown": {
  },
  "inputs": [
  ],
  "outputs": [
  ],
  "fee": 0.00000000
}
```
## Assumere gli Altri Ruoli

Come con la lettura delle PSBT, stiamo introducendo il concetto di creazione delle PSBT, e poi lasciando il resto come esercizio per il lettore.

Quanto segue è un elenco approssimativo di funzioni per ogni ruolo; saranno necessarie ulteriori funzioni per creare alcuni degli elementi che vengono aggiunti alle PSBT.

**Creatore:**

* wally_psbt_init_alloc
* wally_psbt_set_global_tx

**Aggiornante:**

* wally_psbt_input_set_non_witness_utxo
* wally_psbt_input_set_witness_utxo
* wally_psbt_input_set_redeem_script
* wally_psbt_input_set_witness_script
* wally_psbt_input_set_keypaths
* wally_psbt_input_set_unknowns
* wally_psbt_output_set_redeem_script
* wally_psbt_output_set_witness_script
* wally_psbt_output_set_keypaths
* wally_psbt_output_set_unknowns

**Firmante:**

* wally_psbt_input_set_partial_sigs
* wally_psbt_input_set_sighash_type
* wally_psbt_sign

**Combinatore:**

* wally_psbt_combine

**Finalizzatore:**
* wally_psbt_finalize
* wally_psbt_input_set_final_script_sig
* wally_psbt_input_set_final_witness

**Estrattore:**

* wally_psbt_extract

## Riepilogo: Utilizzo di PSBT in Libwally

Questa sezione potrebbe essere un intero capitolo, poiché lavorare con le PSBT a un livello basso è un lavoro molto intenso che richiede una manipolazione degli input e degli output molto più intensiva rispetto a quanto avvenuto nel [Capitolo 7](07_0_Ampliare_le_Transazioni_Bitcoin_con_PSBTs.md). Invece, questa sezione mostra le basi: come estrarre informazioni da una PSBT e come iniziare a crearne una.

> :fire: ***Qual è il potere delle PSBT in Libwally?*** Ovviamente, puoi già fare tutto questo in `bitcoin-cli`, ed è più semplice perché Bitcoin Core gestisce molte delle operazioni di routine. Il vantaggio di usare Libwally è che può essere eseguito offline, quindi potrebbe essere Libwally a trovarsi dall'altra parte di un dispositivo hardware con cui il tuo `bitcoin-cli` sta comunicando tramite HWI. Questo è, infatti, uno dei principali punti delle PSBT: poter manipolare transazioni parzialmente firmate senza bisogno di un nodo completo. Libwally lo rende possibile.

## Cosa c'è dopo?

Scopri di più su "Programming Bitcoin with Libwally" nel [Capitolo 17.5: Usare Scripts in Libwally](17_5_Usare_Scripts_in_Libwally.md).

