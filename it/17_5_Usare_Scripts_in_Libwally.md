# 17.5: Utilizzare gli Script in Libwally

> :information_source: **NOTA:** Questa sezione è stata recentemente aggiunta al corso ed è una bozza preliminare che potrebbe essere ancora in attesa di revisione. Attenzione lettore.

Molto tempo fa, nella Parte 3, mentre introducevamo gli Script, abbiamo detto che era probabile che tu creassi effettivamente transazioni utilizzando script con un'API, e l'avevamo segnato come un argomento per il futuro. Ebbene, il futuro è arrivato.

## Crea lo Script

Creare lo script è la _cosa più facile_ da fare in Libwally. Prendi il seguente esempio, un semplice [Puzzle Script](/13_1_Writing_Puzzle_Scripts.md) al quale siamo tornati di tanto in tanto:

```
OP_ADD 99 OP_EQUAL
```
Utilizzando `btcc`, possiamo serializzarlo. 
```
$ btcc OP_ADD 99 OP_EQUAL
warning: ambiguous input 99 is interpreted as a numeric value; use 0x99 to force into hexadecimal interpretation
93016387
```
In precedenza avevamo costruito lo script standard P2SH a mano, ma Libwally può effettivamente farlo per te.

Innanzitutto, Libwally deve convertire l'hex in byte, poiché i byte sono la maggior parte di quello con cui lavora:
```
  int script_length = strlen(script)/2;
  unsigned char bscript[script_length];
    
  lw_response = wally_hex_to_bytes(script,bscript,script_length,&written);
```
Poi, esegui `wally_scriptpubkey_p2sh_from_bytes` con i tuoi byte, dicendo a Libwally di fare anche `HASH160` per te:
```
  unsigned char p2sh[WALLY_SCRIPTPUBKEY_P2SH_LEN];
  			  
  lw_response = wally_scriptpubkey_p2sh_from_bytes(bscript,sizeof(bscript),WALLY_SCRIPT_HASH160,p2sh,WALLY_SCRIPTPUBKEY_P2SH_LEN,&written);
```
Se guardi i risultati di `p2sh`, vedrai che era:
```
a9143f58b4f7b14847a9083694b9b3b52a4cea2569ed87
```

[Potresti ricordare](10_2_Construire_la_Struttura_di_P2SH.md) che si divide cosi:

```
a9 / 14 / 3f58b4f7b14847a9083694b9b3b52a4cea2569ed / 87
```
Questo è il nostro vecchio amico `OP_HASH160 3f58b4f7b14847a9083694b9b3b52a4cea2569ed OP_EQUAL`.

Fondamentalmente, Libwally ha preso il tuo script di rimborso serializzato, l'ha hashato per te con SHA-256 e RIPEMD-160, e poi ha applicato il framing standard per trasformarlo in un vero P2SH; Hai fatto un lavoro simile nel [Capitolo 10.2](10_2_Construire_la_Struttura_di_P2SH.md), ma con un eccesso di comandi shell.

Infatti, puoi ricontrollare il tuo lavoro utilizzando gli stessi comandi di §10.2:

```
$ redeemScript="93016387"
$ echo -n $redeemScript | xxd -r -p | openssl dgst -sha256 -binary | openssl dgst -rmd160
(stdin)= 3f58b4f7b14847a9083694b9b3b52a4cea2569ed
```


## Crea una Transazione

Per utilizzare quel `pubScriptKey` che hai appena creato, devi creare una transazione e incorporare il `pubScriptKey` al suo interno (e questo è il grande cambiamento rispetto a `bitcoin-cli`: puoi effettivamente creare manualmente una transazione con uno script P2SH).

Il processo di creazione di una transazione in Libwally è molto intenso, proprio come il processo per creare un PSBT, quindi lo delineeremo solo, prendendo una scorciatoia principale, e poi lasceremo un metodo senza scorciatoie per ulteriori indagini.

Creare una transazione stessa è abbastanza semplice: devi solo dire a `wally_tx_init_alloc` il tuo numero di versione, il tuo locktime e il numero di input e output:

```
  struct wally_tx *tx;
  lw_response = wally_tx_init_alloc(2,0,1,1,&tx);
```


Compilare quegli input e output è dove le cose diventano complicate!

### Crea un Output della Transazione

Per creare un output, dici a `wally_tx_output_init_alloc` quanti satoshi stai spendendo e gli fornisci lo script di blocco:
```
  struct wally_tx_output *tx_output;
  lw_response = wally_tx_output_init_alloc(95000,p2sh,sizeof(p2sh),&tx_output);
```
Quella parte in realtà non è stata affatto difficile, e ti ha permesso finalmente di incorporare un P2SH in un `vout`.

Un comando in più lo aggiunge alla tua transazione:

```
  lw_response = wally_tx_add_output(tx,tx_output);
```


### Crea un Input della Transazione

Creare l'input è molto più difficile perché devi accumulare informazioni nelle routine di creazione, non tutte delle quali sono intuitivamente accessibili quando usi Libwally. Quindi, piuttosto che entrare così a fondo nei dettagli, ecco dove prendiamo la nostra scorciatoia. Scriviamo il nostro codice in modo che venga passato il codice hex di una transazione già creata, e poi riutilizziamo semplicemente l'input.

La conversione dal codice hex è fatta con `wally_tx_from_hex`:

```
  struct wally_tx *utxo;
  lw_response = wally_tx_from_hex(utxo_hex,0,&utxo);
```
Poi puoi saccheggiare gli input dal tuo codice hex per creare un input con Libwally:
```
  struct wally_tx_input *tx_input;
  lw_response = wally_tx_input_init_alloc(utxo->inputs[0].txhash,sizeof(utxo->inputs[0].txhash),utxo->inputs[0].index,0,utxo->inputs[0].script,utxo->inputs[0].script_len,utxo->inputs[0].witness,&tx_input);
  assert(lw_response == WALLY_OK);						
```
Come ti aspetti, poi aggiungi quell'input alla tua transazione:
```
  lw_response = wally_tx_add_input(tx,tx_input);
```


> **NOTA** Ovviamente, vorrai essere in grado di creare i tuoi input se stai usando Libwally per applicazioni reali, ma questo è inteso come un primo passo. E, può essere effettivamente utile per integrarsi con `bitcoin-cli`, come vedremo Nel prossimo [Capitolo 17.7](17_7_Integrare_Libwally_e_Bitcoin-CLI.md).

### Stampa una Transazione

Teoricamente potresti firmare e inviare questa transazione dal tuo programma C costruito su Libwally, ma in linea con l'idea che stiamo solo utilizzando un semplice programma C per sostituire un P2SH, stamperemo il nuovo hex. Questo è fatto con l'aiuto di `wally_tx_to_hex`:

```
  char *tx_hex;
  lw_response = wally_tx_to_hex(tx,0, &tx_hex);

  printf("%s\n",tx_hex);
```
Mostreremo come utilizzarlo in §16.7.

## Testa il Tuo Script di Sostituzione

Puoi prendere il codice di test dalla [directory src](src/17_5_replacewithscript.c) e compilarlo:

```
$  cc replacewithscript.c -lwallycore -o replacewithscript
```
Successivamente, prepara una transazione hex e uno script hex serializzato:
```
hex=020000000001019527cebb072524a7961b1ba1e58fc18dd7c6fc58cd6c1c45d7e1d8fc690b006e0000000017160014cc6e8522f0287b87b7d0a83629049c2f2b0e972dfeffffff026f8460000000000017a914ba421212a629a840492acb2324b497ab95da7d1e87306f0100000000001976a914a2a68c5f9b8e25fdd1213c38d952ab2be2e271be88ac02463043021f757054fa61cfb75b64b17230b041b6d73f25ff9c018457cf95c9490d173fb4022075970f786f24502290e8a5ed0f0a85a9a6776d3730287935fb23aa817791c01701210293fef93f52e6ce8be581db62229baf116714fcb24419042ffccc762acc958294e6921b00

script=93016387
```
Poi puoi eseguire il programma di sostituzione:
```
$ ./replacewithscript $hex $script
02000000019527cebb072524a7961b1ba1e58fc18dd7c6fc58cd6c1c45d7e1d8fc690b006e0000000017160014cc6e8522f0287b87b7d0a83629049c2f2b0e972d0000000001187301000000000017a9143f58b4f7b14847a9083694b9b3b52a4cea2569ed8700000000
```
Puoi poi vedere i risultati con `bitcoin-cli`:
```
$ bitcoin-cli decoderawtransaction $newhex
{
  "txid": "f4e7dbab45e759a7ac6e2fb0f10720cd29d047efad89fe1b569f5f4ba61fd8e6",
  "hash": "f4e7dbab45e759a7ac6e2fb0f10720cd29d047efad89fe1b569f5f4ba61fd8e6",
  "version": 2,
  "size": 106,
  "vsize": 106,
  "weight": 424,
  "locktime": 0,
  "vin": [
    {
      "txid": "6e000b69fcd8e1d7451c6ccd58fcc6d78dc18fe5a11b1b96a7242507bbce2795",
      "vout": 0,
      "scriptSig": {
        "asm": "0014cc6e8522f0287b87b7d0a83629049c2f2b0e972d",
        "hex": "160014cc6e8522f0287b87b7d0a83629049c2f2b0e972d"
      },
      "sequence": 0
    }
  ],
  "vout": [
    {
      "value": 0.00095000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_HASH160 3f58b4f7b14847a9083694b9b3b52a4cea2569ed OP_EQUAL",
        "hex": "a9143f58b4f7b14847a9083694b9b3b52a4cea2569ed87",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2My2ApqGcoNXYceZC4d7fipBu4GodkbefHD"
        ]
      }
    }
  ]
}
```
Il `vin` dovrebbe semplicemente corrispondere all'input che hai sostituito, ma è il `vout` che è entusiasmante: hai creato una transazione con uno `scripthash`!

## Sommario: Utilizzare gli Script in Libwally

Creare transazioni in Libwally è un altro argomento che potrebbe occupare un intero capitolo, ma la cosa fantastica è che una volta fatto questo salto, puoi introdurre uno `scriptPubKey` P2SH, e quella parte da sola è abbastanza semplice. Anche se la metodologia dettagliata in questo capitolo richiede di avere già in mano un hex di transazione (probabilmente creato con `bitcoin-cli`) se esplori ulteriormente Libwally, puoi fare tutto da solo.

> :fire: ***Qual è il Potere degli Script in Libwally?*** Semplicemente, puoi fare qualcosa che non potevi fare prima: creare una transazione bloccata con un arbitrario P2SH.

## Cosa c'è dopo?

Scopri di più su "Programmazione Bitcoin con Libwally" nel [Capitolo 17.6: Usare Altre Funzioni in Libwally](17_6_Usare_Altre_Funzioni_in_Libwally.md).
