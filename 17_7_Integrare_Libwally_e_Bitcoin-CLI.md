# 17.7: Integrazione di Libwally e Bitcoin-CLI

> :information_source: **NOTA:** Questa sezione è stata recentemente aggiunta al corso ed è una bozza preliminare che potrebbe essere ancora in fase di revisione. Avvertenza per il lettore.

Libwally è limitato. Si occupa della manipolazione di semi, chiavi, indirizzi e altri elementi dei portafogli, con alcune funzioni aggiuntive relative alle transazioni e ai PSBT che potrebbero essere utili per servizi non collegati ai nodi completi su Internet. Tuttavia, alla fine avrai bisogno di servizi di nodi completi per sfruttare appieno Libwally.

Questa sezione finale offrirà alcuni esempi di utilizzo dei programmi Libwally per completare un ambiente `bitcoin-cli`. Anche se questi esempi implicano che questi servizi siano tutti sulla stessa macchina, potrebbero diventare ancora più potenti se il servizio `bitcoin-cli` è direttamente connesso a Internet e il servizio Libwally no.

## Condividere una Transazione

[Capitolo 17.5: Usare Scripts in Libwally](17_5_Usare_Scripts_in_Libwally.md) ha dettagliato come Libwally potrebbe essere utilizzato per riscrivere una transazione esistente, per fare qualcosa che `bitcoin-cli` non può: produrre una transazione che contiene uno P2SH unico. Ovviamente, questo è un elemento fondamentale; se decidi di approfondire Libwally, creerai transazioni complete da solo. Ma, anche questa metodologia abbreviata ha il suo utilizzo: mostra come le transazioni possono essere passate avanti e indietro tra `bitcoin-cli` e Libwally, dimostrando un primo esempio di utilizzo complementare.

Per dimostrare completamente questa metodologia, creerai una transazione con `bitcoin-cli`, utilizzando questo UTXO:

```
  {
    "txid": "c0a110a7a84399b98052c6545018873b13ee3128fa74f7a697779174a36ea33a",
    "vout": 1,
    "address": "mvLyH7Rs45c16FG2dfV7uuTKV6pL92kWxo",
    "label": "",
    "scriptPubKey": "76a914a2a68c5f9b8e25fdd1213c38d952ab2be2e271be88ac",
    "amount": 0.00094000,
    "confirmations": 17375,
    "spendable": true,
    "solvable": true,
    "desc": "pkh([ce0c7e14/0'/0'/5']0368d0fffa651783524f8b934d24d03b32bf8ff2c0808943a556b3d74b2e5c7d65)#qldtsl65",
    "safe": true
  }
```
Ormai sai come impostare una transazione con `bitcoin-cli`:
```
$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[0] | .txid') 
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
$ recipient=tb1qycsmq3jas5wkhf8xrfn8k7438cm5pc8h9ae2k0
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.0009 }''')
```
Anche se hai inserito un destinatario e un importo nell'output, non è rilevante, perché riscriverai quei dati. Un codice più sofisticato potrebbe leggere le informazioni `vout` esistenti prima della riscrittura, ma noi manteniamo tutto molto vicino al nostro [codice originale](src/17_5_replacewithscript.c).

Ecco la sola modifica necessaria, per permetterti di specificare il satoshi `vout`, senza doverlo codificare a mano, come nel caso originale:

```
...
  int satoshis = atoi(argv[3]);
...
  lw_response = wally_tx_output_init_alloc(satoshis,p2sh,sizeof(p2sh),&tx_output);
...
```
Poi esegui le operazioni come prima:
```
$ newtxhex=$(./replacewithscript $rawtxhex $script 9000)
```
Ecco come appariva la transazione originale:
```
$ bitcoin-cli decoderawtransaction $rawtxhex
{
  "txid": "438d50edd7abeaf656c5abe856a00a20af5ff08939df8fdb9f8bfbfb96234fcb",
  "hash": "438d50edd7abeaf656c5abe856a00a20af5ff08939df8fdb9f8bfbfb96234fcb",
  "version": 2,
  "size": 82,
  "vsize": 82,
  "weight": 328,
  "locktime": 0,
  "vin": [
    {
      "txid": "c0a110a7a84399b98052c6545018873b13ee3128fa74f7a697779174a36ea33a",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 0.00090000,
      "n": 0,
      "scriptPubKey": {
        "asm": "0 2621b0465d851d6ba4e61a667b7ab13e3740e0f7",
        "hex": "00142621b0465d851d6ba4e61a667b7ab13e3740e0f7",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1qycsmq3jas5wkhf8xrfn8k7438cm5pc8h9ae2k0"
        ]
      }
    }
  ]
}
```
Ecco la transazione riscritta da Libwally per utilizzare uno P2SH:
```
standup@btctest:~/c$ bitcoin-cli decoderawtransaction $newtxhex
{
  "txid": "badb57622ab5fe029fc1a71ace9f7b76c695f933bceb0d38a155c2e5c984f4e9",
  "hash": "badb57622ab5fe029fc1a71ace9f7b76c695f933bceb0d38a155c2e5c984f4e9",
  "version": 2,
  "size": 83,
  "vsize": 83,
  "weight": 332,
  "locktime": 0,
  "vin": [
    {
      "txid": "c0a110a7a84399b98052c6545018873b13ee3128fa74f7a697779174a36ea33a",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 0
    }
  ],
  "vout": [
    {
      "value": 0.00090000,
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
Successivamente puoi firmarla come al solito con `bitcoin-cli`:
```
$ signedtx=$(bitcoin-cli signrawtransactionwithwallet $newtxhex | jq -r '.hex')
```
E come puoi vedere, il risultato è una transazione legittima pronta per essere inviata alla rete Bitcoin:
```
$ bitcoin-cli decoderawtransaction $signedtx
{
  "txid": "3061ca8c01d029c0086adbf8b7d4280280c8aee151500bab7c4f783bbc8e75e6",
  "hash": "3061ca8c01d029c0086adbf8b7d4280280c8aee151500bab7c4f783bbc8e75e6",
  "version": 2,
  "size": 189,
  "vsize": 189,
  "weight": 756,
  "locktime": 0,
  "vin": [
    {
      "txid": "c0a110a7a84399b98052c6545018873b13ee3128fa74f7a697779174a36ea33a",
      "vout": 1,
      "scriptSig": {
        "asm": "3044022026c81b6ff4a15135d10c7f4b1ae6e44ac4fdb25c4a3c03161b17b8ab8d04850502200b448d070f418de1ca07e76943d23d447bc95c7c5e0322bcc153cadb5d9befe0[ALL] 0368d0fffa651783524f8b934d24d03b32bf8ff2c0808943a556b3d74b2e5c7d65",
        "hex": "473044022026c81b6ff4a15135d10c7f4b1ae6e44ac4fdb25c4a3c03161b17b8ab8d04850502200b448d070f418de1ca07e76943d23d447bc95c7c5e0322bcc153cadb5d9befe001210368d0fffa651783524f8b934d24d03b32bf8ff2c0808943a556b3d74b2e5c7d65"
      },
      "sequence": 0
    }
  ],
  "vout": [
    {
      "value": 0.00090000,
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
Voila! Questa è la potenza de Libwally con `bitcoin-cli`.

Ovviamente, puoi anche passare un PSBT utilizzando le funzioni descritte nel [Capitolo17.4](17_4_Usare_PSBTs_in_Libwally.md) e questa è una metodologia più aggiornata per l'uso moderno di Bitcoin, ma in entrambi gli esempi, il concetto di passare le transazioni tra `bitcoin-cli` e il codice di Libwally e viceversa dovrebbe essere simile.

## Import & Export BIP39 Seeds

Purtroppo, non tutte le interazioni tra Libwally e `bitcoin-cli` sono così fluide. Ad esempio, sarebbe utile poter esportare un seme HD da `bitcoin-cli` per generare la frase mnemonica con Libwally, o generare un seme da una frase mnemonica utilizzando Libwally, e poi importarlo in `bitcoin-cli`. Sfortunatamente, nessuna di queste operazioni è possibile al momento. Una frase mnemonica viene tradotta in un seme utilizzando HMAC-SHA512, il che significa che il risultato è di 512 bit. Tuttavia, `bitcoin-cli` esporta semi HD (utilizzando `dumpwallet`) e importa semi HD (utilizzando `sethdseed`) con una lunghezza di 256 bit. Finché non verrà modificato, non ci sarà incontro tra i due.

> :book: ***Qual è la differenza tra Entropia e un Seme?*** Libwally afferma di creare le sue frasi mnemoniche a partire dall'entropia. Questo è essenzialmente la stessa cosa di un seme: entrambi sono numeri grandi e casuali. Quindi, se `bitcoin-cli` fosse compatibile con i semi di frase mnemonica a 512 bit, potresti usare uno di essi per generare le frasi mnemoniche e ottenere i risultati che ti aspetti.

> :book: ***Qual è la differenza tra Entropia e Entropia Grezza?*** Non tutta l'entropia è uguale. Quando inserisci l'entropia in un comando che crea un seme mnemonico, deve avere una lunghezza e un formato specifici. Trasformare l'entropia grezza in entropia richiede la regolazione dell'entropia grezza fino a ottenere la lunghezza e il formato giusti, e a quel punto puoi riutilizzare quell'entropia (non grezza) per ricreare sempre le stesse mnemoniche (il che è il motivo per cui l'entropia è effettivamente la stessa cosa di un seme a quel punto, ma l'entropia grezza no).

## Importare Chiavi Private

Fortunatamente, puoi fare quasi la stessa cosa importando una chiave privata generata in Libwally. Dai un'occhiata a [genhd-for-import.c](src/17_7_genhd_for_import.c), una versione semplificata del programma `genhd` del [Capitolo 17.3](17_3_Usare_BIP32_in_Libwally.md) che utilizza anche la libreria `jansson` nel [Capitolo 16.1](116_1_Accedere_a_Bitcoind_con_Librerie_RPC.md) per un'uscita regolarizzata.

Il codice aggiornato contiene anche una modifica importante: richiede un'impronta (fingerprint) da Libwally in modo da poter creare correttamente un percorso di derivazione:

```
  char account_fingerprint[BIP32_KEY_FINGERPRINT_LEN];
  lw_response = bip32_key_get_fingerprint(key_account,account_fingerprint,BIP32_KEY_FINGERPRINT_LEN);

  char *fp_hex;
  lw_response = wally_hex_from_bytes(account_fingerprint,BIP32_KEY_FINGERPRINT_LEN,&fp_hex);
```


> :warning: **ATTENZIONE:** Ricorda che l'impronta nel percorso di derivazione è arbitraria. Poiché Libwally ne fornisce una, la stiamo usando, ma se non ne avessi una, potresti aggiungere un codice esadecimale arbitrario di 4 byte come impronta al tuo percorso di derivazione.

Assicurati di compilare il nuovo codice con la libreria `jansson`, dopo averla installata (se necessario) come indicato nel [Capitolo 16.1](116_1_Accedere_a_Bitcoind_con_Librerie_RPC.md).

```
$ cc genhd-for-import.c -lwallycore -lsodium -ljansson -o genhd-for-import
```
Quando esegui il nuovo programma, otterrai un elenco ben formattato di tutto:
```
$ ./genhd-for-import 
{
  "mnemonic": "physical renew say quit enjoy eager topic remind riot concert refuse chair",
  "account-xprv": "tprv8yxn8iFNgsLktEPkWKQpMqb7bcx5ViFQEbJMtqrGi8LEgvy8es6YeJvyJKrbYEPKMw8JbU3RFhNRQ4F2pataAtTNokS1JXBZVg62xfd5HCn",
  "address": "tb1q9lhru6k0ymwrtr5w98w35n3lz22upml23h753n",
  "derivation": "[d1280779/84h/1h/0h]"
}
```
Hai la `mnemonic` da cui puoi recuperare, un `account-xprv` che puoi importare, una `derivation` da usare per l'importazione e un `address` di esempio, che puoi usare per testare l'importazione.

Ora puoi ricorrere alle lezioni apprese nel [Capitolo 3.5](03_5_Comprendere_il_Descriptor.md) su come trasformare quel xprv in un descrittore e importarlo.

Prima, devi determinare il checksum:

```
$ xprv=tprv8yxn8iFNgsLktEPkWKQpMqb7bcx5ViFQEbJMtqrGi8LEgvy8es6YeJvyJKrbYEPKMw8JbU3RFhNRQ4F2pataAtTNokS1JXBZVg62xfd5HCn
$ dp=[d1280779/84h/1h/0h]
$ bitcoin-cli getdescriptorinfo "wpkh($dp$xprv/0/*)"
{
  "descriptor": "wpkh([d1280779/84'/1'/0']tpubDWepH8HcqF2RmhRYPy5QmFFEAeU1f3SJotu9BMta8Q8dXRDuHFv8poYqUUtEiWftBjtKn1aNhi9Qg2P4NdzF66dShYvB92z78WJbYeHTLTz/0/*)#f8rmqc0z",
  "checksum": "46c00dk5",
  "isrange": true,
  "issolvable": true,
  "hasprivatekeys": true
}
```


Ci sono tre cose da notare qui:

1. Usiamo `wpkh` come funzione nel nostro percorso di derivazione. Questo perché vogliamo generare indirizzi Segwit moderni, non indirizzi legacy. Questo corrisponde al nostro utilizzo nella funzione `wally_bip32_key_to_addr_segwit` di Libwally. La cosa più importante, tuttavia, è avere le stesse aspettative con Libwally e `bitcoin-cli` (e il tuo descrittore) su che tipo di indirizzo stai generando, in modo che tutto corrisponda!
2. Utilizziamo il percorso `/0/*` perché volevamo gli indirizzi esterni per questo account. Se invece volessimo gli indirizzi di cambio, useremmo `/1/*`.
3. Non useremo la riga `descriptor` restituita, poiché è per un indirizzo `xpub`. Invece, applicheremo il `checksum` restituito al `xprv` che già possediamo.

```
$ cs=$(bitcoin-cli getdescriptorinfo "wpkh($dp$xprv/0/*)" | jq -r ' .checksum')
```
Poi lo inserisci in `importmulti` per importare questa chiave in `bitcoin-cli`:
```
$ bitcoin-cli importmulti '''[{ "desc": "wpkh('$dp''$xprv'/0/*)#'$cs'", "timestamp": "now", "range": 10, "watchonly": false, "label": "LibwallyImports", "keypool": false, "rescan": false }]'''
[
  {
    "success": true
  }
]

```
Qui hai importato/generato i primi dieci indirizzi per la chiave privata.

Esaminando la nuova etichetta `LibwallyImported`, li mostra:

```
$ bitcoin-cli getaddressesbylabel "LibwallyImports"
{
  "tb1qzeqrrt77xhvazq5g8sc9th0lzjwstknan8gzq7": {
    "purpose": "receive"
  },
  "tb1q9lhru6k0ymwrtr5w98w35n3lz22upml23h753n": {
    "purpose": "receive"
  },
  "tb1q8fsgxt0z9r9hfl5mst5ylxka2yljjxlxlvaf8j": {
    "purpose": "receive"
  },
  "tb1qg6dayhdk4qc6guutxvdweh6pctc9dpguu6awqc": {
    "purpose": "receive"
  },
  "tb1qdphaj0exvemxhgfpyh4p99wn84e2533u7p96l6": {
    "purpose": "receive"
  },
  "tb1qwv9mdqkpx6trtmvgw3l95npq8gk9pgllucvata": {
    "purpose": "receive"
  },
  "tb1qwh92pkrv6sps62udnmez65vfxe9n5ceuya56xz": {
    "purpose": "receive"
  },
  "tb1q4e98ln8xlym64qjzy3k8zyfyt5q60dgcn39d90": {
    "purpose": "receive"
  },
  "tb1qhzje887fyl65j4mulqv9ysmntwn95zpgmgvtqd": {
    "purpose": "receive"
  },
  "tb1q62xf9ec8zcfkh2qy5qnq4qcxrx8l0jm27dd8ru": {
    "purpose": "receive"
  },
  "tb1qlw85usfk446ssxejm9dmxsfn40kzsqce77aq20": {
    "purpose": "receive"
  }
}

```
Il secondo sulla tua lista corrisponde effettivamente al tuo esempio (`tb1q9lhru6k0ymwrtr5w98w35n3lz22upml23h753n`). L'importazione di questa chiave privata e la derivazione di dieci indirizzi è stata effettuata con successo.

Se guardi di nuovo il [Capitolo 7.3](07_3_Integrazione_con_Hardware_Wallets.md), vedrai che questo è lo stesso metodo che abbiamo usato per importare indirizzi da un Hardware Wallet (anche se in questo caso abbiamo importato anche la chiave privata come prova di concetto). La principale differenza è che in precedenza le informazioni erano create da una black box (letteralmente: era un dispositivo Ledger), e questa volta hai creato le informazioni tu stesso utilizzando Libwally, dimostrando come puoi fare questo tipo di lavoro su dispositivi airgapped o altri dispositivi più remoti, per poi portarli su `bitcoin-cli`.

## Importare Indirizzi

Ovviamente, se puoi importare chiavi private, puoi importare anche indirizzi — il che di solito significa importare indirizzi in sola visualizzazione _senza_ le chiavi private.

Un modo per farlo è utilizzare la metodologia `importmulti` sopra, ma utilizzare l'indirizzo xpub fornito (`wpkh([d1280779/84'/1'/0']tpubDWepH8HcqF2RmhRYPy5QmFFEAeU1f3SJotu9BMta8Q8dXRDuHFv8poYqUUtEiWftBjtKn1aNhi9Qg2P4NdzF66dShYvB92z78WJbYeHTLTz/0/*)#f8rmqc0z`) piuttosto che l'xprv originale. Questo è il modo migliore per importare una sequenza intera di indirizzi in sola visualizzazione.

In alternativa, puoi importare indirizzi individuali. Ad esempio, considera l'indirizzo di esempio restituito attualmente dal programma `genhd-for-import`:

```
$ ./genhd-for-import 
{
  "mnemonic": "finish lady crucial walk illegal ride hamster strategy desert file twin nature",
  "account-xprv": "tprv8xRujYeVN7CwBHxLoTHRdmzwdW7dKUzDfruSo56GqqfRW9QXtnxnaRG8ke7j65uNjxmCVfcagz5uuaMi2vVJ8jpiGZvLwahmNB8F3SHUSyv",
  "address": "tb1qtvcchgyklp6cyleth85c7pfr4j72z2vyuwuj3d",
  "derivation": "[6214ecff/84h/1h/0h]"
}
```
Puoi importare quello come indirizzo in sola visualizzazione con `importaddress`:
```
$ bitcoin-cli -named importaddress address=tb1qtvcchgyklp6cyleth85c7pfr4j72z2vyuwuj3d label=LibwallyWO rescan=false
$ bitcoin-cli getaddressesbylabel "LibwallyWO"
{
  "tb1qtvcchgyklp6cyleth85c7pfr4j72z2vyuwuj3d": {
    "purpose": "receive"
  }
}
}
```

## Riepilogo: Integrazione di Libwally e Bitcoin-CLI

Con una conoscenza fondamentale di Libwally, puoi ora completare tutto il lavoro delle lezioni precedenti. Trasferire indirizzi, chiavi, transazioni e PSBT è solo alcuni dei modi in cui potresti utilizzare questi due potenti metodi di programmazione Bitcoin insieme. C'è anche molta più profondità se vuoi approfondire la vasta libreria di funzioni di Libwally.

> :fire: ***Qual è il Potere dell'Integrazione tra Libwally e Bitcoin-CLI?*** Uno dei maggiori vantaggi di Libwally è che ha molte funzioni che possono essere utilizzate offline. In confronto, Bitcoin Core è un programma connesso alla rete. Questo può aiutarti ad aumentare la sicurezza passando chiavi, indirizzi, transazioni o PSBT a una fonte offline (che eseguirà programmi Libwally). Inoltre, Libwally può fare cose che Bitcoin Core attualmente non può, come generare un seme da una frase mnemonica BIP39 (e anche se attualmente non puoi importare il seme in Bitcoin Core, puoi ancora importare la chiave principale per l'account, come mostrato qui).

## Cosa c'è dopo?

Scopri altri tipi di programmazione nel [Capitolo 18: Parlare a Bitcoind in Altri Linguaggi](18_0_Parlare_a_Bitcoind_in_Altri_Linguaggi.md).
