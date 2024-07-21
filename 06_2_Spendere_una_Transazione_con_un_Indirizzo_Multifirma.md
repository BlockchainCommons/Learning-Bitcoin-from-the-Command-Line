# 6.2: Spendere una transazione Multifirma

Il modo classico e complesso di spendere i fondi inviati ad un indirizzo multifirma utilizzando "bitcoin-cli" richiede molto lavoro manuale.

## Trova i tuoi fondi

Per cominciare, devi trovare i tuoi fondi; il tuo computer non sa cercarli, perché non sono associati ad alcun indirizzo nel tuo portafoglio. Puoi avvisare `bitcoind` di farlo utilizzando il comando `importaddress`:
```
$ bitcoin-cli -named importaddress address=2NAGfA4nW6nrZkD5je8tSiAcYB9xL2xYMCz
```
Se hai un nodo podato (e probabilmente lo fai), dovrai invece dirgli di non ripetere la scansione:
```
$ bitcoin-cli -named importaddress address=2NAGfA4nW6nrZkD5je8tSiAcYB9xL2xYMCz rescan="false"
```
Se preferisci, puoi importare l'indirizzo utilizzando il suo descrittore (e questa è generalmente la risposta migliore e più standardizzata):
```
$ bitcoin-cli importmulti '[{"desc": "sh(multi(2,02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191,02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3))#0pazcr4y", "timestamp": "now", "watchonly": true}]'
[
  {
    "success": true
  }
]
```
Successivamente i fondi dovrebbero apparire quando esegui `listunspend`... ma non sono ancora facilmente spendibili. (In effetti, il tuo portafoglio potrebbe affermare che non sono affatto "spendibili"!)

Se per qualche motivo non riesci a incorporare l'indirizzo nel tuo portafoglio, puoi utilizzare `gettransaction` per ottenere informazioni (o guardare un block explorer).
```
$ bitcoin-cli -named gettransaction txid=b164388854f9701051809eed166d9f6cedba92327e4296bf8a265a5da94f6521 verbose=true
{
  "amount": -0.00006500,
  "fee": -0.00001000,
  "confirmations": 3,
  "blockhash": "0000000000000165b5f602920088a7e36b11214161d6aaebf5229e3ed4f10adc",
  "blockheight": 1773282,
  "blockindex": 9,
  "blocktime": 1592959320,
  "txid": "b164388854f9701051809eed166d9f6cedba92327e4296bf8a265a5da94f6521",
  "walletconflicts": [
  ],
  "time": 1592958753,
  "timereceived": 1592958753,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr",
      "category": "send",
      "amount": -0.00006500,
      "vout": 0,
      "fee": -0.00001000,
      "abandoned": false
    }
  ],
  "hex": "020000000001011b95a6055174ec64b82ef05b6aefc38f34d0e57197e40281ecd8287b4260dec60000000000ffffffff01641900000000000017a914a5d106eb8ee51b23cf60d8bd98bc285695f233f38702473044022070275f81ac4129e1d167ef7e700739f2899ea4c7f1adef3a4da29436f14fb97e02207310d4ec449eba49f0fa404ae45b9c82431d883490c7a0ed882ad0b5d7a623d0012102883bb5463e37d55252d8b3d5c2141b007b37c8a7db6211f75c955acc5ea325eb00000000",
  "decoded": {
    "txid": "b164388854f9701051809eed166d9f6cedba92327e4296bf8a265a5da94f6521",
    "hash": "bdf4e3bc5d354a5dfa5528f172480976321d989d7e5806ac14f1fe9b0b1c093a",
    "version": 2,
    "size": 192,
    "vsize": 111,
    "weight": 441,
    "locktime": 0,
    "vin": [
      {
        "txid": "c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b",
        "vout": 0,
        "scriptSig": {
          "asm": "",
          "hex": ""
        },
        "txinwitness": [
          "3044022070275f81ac4129e1d167ef7e700739f2899ea4c7f1adef3a4da29436f14fb97e02207310d4ec449eba49f0fa404ae45b9c82431d883490c7a0ed882ad0b5d7a623d001",
          "02883bb5463e37d55252d8b3d5c2141b007b37c8a7db6211f75c955acc5ea325eb"
        ],
        "sequence": 4294967295
      }
    ],
    "vout": [
      {
        "value": 0.00006500,
        "n": 0,
        "scriptPubKey": {
          "asm": "OP_HASH160 a5d106eb8ee51b23cf60d8bd98bc285695f233f3 OP_EQUAL",
          "hex": "a914a5d106eb8ee51b23cf60d8bd98bc285695f233f387",
          "reqSigs": 1,
          "type": "scripthash",
          "addresses": [
            "2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr"
          ]
        }
      }
    ]
  }
}
```

## Imposta le tue variabili

Quando sarai pronto a spendere i fondi ricevuti da un indirizzo multifirma, avrai bisogno di raccogliere _molti_ dati: molto più di quelli di cui hai bisogno quando spendi un normale P2PKH o SegWit UTXO. Ciò è in parte dovuto al fatto che le informazioni sull'indirizzo multisig non sono nel tuo portafoglio e in parte perché stai spendendo denaro inviato a un indirizzo P2SH (pay-to-script-hash), e questo è molto più impegnativo.

In totale, dovrai raccogliere tre cose: informazioni estese sull'UTXO; il redeemScript; e tutte le chiavi private coinvolte. Ovviamente avrai bisogno anche di un nuovo indirizzo del destinatario. Le chiavi private devono attendere la fase di firma, ma tutto il resto può essere fatto ora.

### Accedi alle informazioni dell' UTXO

Per cominciare, prendi `txid` e `vout` per la transazione che vuoi spendere, come al solito. In questo caso, è stato recuperato dalle informazioni `gettransaction` sopra:
```
$ utxo_txid=b164388854f9701051809eed166d9f6cedba92327e4296bf8a265a5da94f6521
$utxo_vout=0
```
Tuttavia, devi accedere anche a una terza informazione sull'UTXO, il suo `scriptPubKey`/`hex`, che è lo script che ha bloccato la transazione. Ancora una volta, probabilmente lo stai facendo guardando i dettagli della transazione:
```
$ utxo_spk=a914a5d106eb8ee51b23cf60d8bd98bc285695f233f387
```

### Registra lo redeemScript

Si spera che tu abbia salvato il `redeemScript`. Ora dovresti registrarlo in una variabile.

Questo è stato tratto dalla nostra creazione dell'indirizzo nella sezione precedente.

```
redeem_script="522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae"
```
### Decidi il tuo destinatario

Rimanderemo semplicemente i soldi a noi stessi. Questo è utile perché libera i fondi dal multisig, convertendoli in una normale transazione P2PKH che può poi essere confermata da un'unica chiave privata:

```
$ recipient=$(bitcoin-cli getrawchangeaddress)
```
## Crea la tua transazione

Ora puoi creare la tua transazione. Questo non è diverso dal solito.

```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.00005}''')
$ echo $rawtxhex
020000000121654fa95d5a268abf96427e3292baed6c9f6d16ed9e80511070f954883864b10000000000ffffffff0188130000000000001600142c48d3401f6abed74f52df3f795c644b4398844600000000
```

## Firma la tua transazione

Ora sei pronto per firmare la tua transazione. Questo è un processo in più fasi perché dovrai farlo su più macchine, ognuna delle quali fornirà le proprie chiavi private.

### Scarica la tua prima chiave privata

Poiché questa transazione non utilizza completamente il tuo portafoglio, dovrai accedere direttamente alle tue chiavi private. Inizia su "machine1", dove dovresti recuperare tutte le chiavi private di quell'utente coinvolte nel multisig:
```
machine1$ bitcoin-cli -named dumpprivkey address=$address1
cNPhhGjatADfhLD5gLfrR2JZKDE99Mn26NCbERsvnr24B3PcSbtR
```
> :warning: **ATTENZIONE:** L'accesso diretto alle chiavi private dalla shell è un comportamento molto pericoloso e dovrebbe essere fatto con estrema cautela se si utilizzano soldi veri. Per lo meno, non salvare le informazioni in una variabile a cui è possibile accedere dal tuo computer. Rimuovere la cronologia della shell è un altro ottimo passo. Al massimo non farlo.

### Crea la tua prima firma

Ora puoi creare la tua prima firma con il comando `signrawtransactionwithkey`. Qui è dove le cose sono diverse: dovrai istruire il comando su come firmare. Puoi farlo aggiungendo le seguenti nuove informazioni:

* Includi un argomento `prevtxs` che includa `txid`, `vout`, `scriptPubKey` e `redeemScript` che hai registrato, ciascuno dei quali è una singola coppia chiave-valore nell'oggetto JSON.
* Includi un argomento `privkeys` che elenca le chiavi private che hai scaricato su questa macchina.

```
machine1$ bitcoin-cli -named signrawtransactionwithkey hexstring=$rawtxhex prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cNPhhGjatADfhLD5gLfrR2JZKDE99Mn26NCbERsvnr24B3PcSbtR"]'
{
  "hex": "020000000121654fa95d5a268abf96427e3292baed6c9f6d16ed9e80511070f954883864b100000000920047304402201c97b48215f261055e41b765ab025e77a849b349698ed742b305f2c845c69b3f022013a5142ef61db1ff425fbdcdeb3ea370aaff5265eee0956cff9aa97ad9a357e3010047522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352aeffffffff0188130000000000001600142c48d3401f6abed74f52df3f795c644b4398844600000000",
  "complete": false,
  "errors": [
    {
      "txid": "b164388854f9701051809eed166d9f6cedba92327e4296bf8a265a5da94f6521",
      "vout": 0,
      "witness": [
      ],
      "scriptSig": "0047304402201c97b48215f261055e41b765ab025e77a849b349698ed742b305f2c845c69b3f022013a5142ef61db1ff425fbdcdeb3ea370aaff5265eee0956cff9aa97ad9a357e3010047522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae",
      "sequence": 4294967295,
      "error": "CHECK(MULTI)SIG failing with non-zero signature (possibly need more signatures)"
    }
  ]
}

```
Ciò produce errori spaventosi e dice che sta "fallendo". Va tutto bene. Puoi vedere che la firma è stata parzialmente riuscita perché l'`hex` è diventato più lungo. Sebbene la transazione sia stata parzialmente firmata, non viene eseguita perché necessita di più firme.

### Ripetere per gli altri firmatari

Ora puoi trasferire la transazione, affinché venga firmata nuovamente da chiunque altro sia richiesto per il multisig. Lo fanno eseguendo lo stesso comando di firma che hai eseguito tu ma: (1) con il codice `hex` più lungo da cui ottieni l'output: (`bitcoin-cli -named signrawtransactionwithkey hexstring=$rawtxhex prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cMgb3KM8hPATCtgMKarKMiFesLft6eEw3DY6BB8d97fkeXeqQagw"]' | jq -r '.hex'`); e (2) con le loro chiavi private.

> :information_source: **NOTA — M-di-N VS N-di-N:** Ovviamente, se hai una firma N-of-N (come la multifirma 2-of-2 in questo esempio), allora tutti devono firmare, ma se hai una multifirma M-di-N dove "M < N", la firma sarà completa quando solo alcuni ("M") dei firmatari avranno firmato.

Per farlo accedono prima alle loro chiavi private:
```
machine2$ bitcoin-cli -named dumpprivkey address=$address2
cVhqpKhx2jgfLUWmyR22JnichoctJCHPtPERm11a2yxnVFKWEKyz
```
In secondo luogo, firmano il nuovo `hex` utilizzando tutti gli stessi valori `prevtxs`:
```
machine1$  bitcoin-cli -named signrawtransactionwithkey hexstring=020000000121654fa95d5a268abf96427e3292baed6c9f6d16ed9e80511070f954883864b100000000920047304402201c97b48215f261055e41b765ab025e77a849b349698ed742b305f2c845c69b3f022013a5142ef61db1ff425fbdcdeb3ea370aaff5265eee0956cff9aa97ad9a357e3010047522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352aeffffffff0188130000000000001600142c48d3401f6abed74f52df3f795c644b4398844600000000 prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cVhqpKhx2jgfLUWmyR22JnichoctJCHPtPERm11a2yxnVFKWEKyz"]'
{
  "hex": "020000000121654fa95d5a268abf96427e3292baed6c9f6d16ed9e80511070f954883864b100000000d90047304402201c97b48215f261055e41b765ab025e77a849b349698ed742b305f2c845c69b3f022013a5142ef61db1ff425fbdcdeb3ea370aaff5265eee0956cff9aa97ad9a357e301473044022000a402ec4549a65799688dd531d7b18b03c6379416cc8c29b92011987084e9f402205470e24781509c70e2410aaa6d827aa133d6df2c578e96a496b885584fb039200147522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352aeffffffff0188130000000000001600142c48d3401f6abed74f52df3f795c644b4398844600000000",
  "complete": true
}
```
In terzo luogo, potrebbe essere necessario inviare la "stringa esadecimale" `hexstring` ancora più lunga prodotta ad altri firmatari.

Ma in questo caso ora vediamo che la firma è `complete`!

## Invia la tua transazione

Una volta terminato, dovresti ricorrere alla metodologia JQ standard per salvare la tua `hexstring` e quindi inviarla:
```
$ signedtx=$(bitcoin-cli -named signrawtransactionwithkey hexstring=020000000121654fa95d5a268abf96427e3292baed6c9f6d16ed9e80511070f954883864b100000000920047304402201c97b48215f261055e41b765ab025e77a849b349698ed742b305f2c845c69b3f022013a5142ef61db1ff425fbdcdeb3ea370aaff5265eee0956cff9aa97ad9a357e3010047522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352aeffffffff0188130000000000001600142c48d3401f6abed74f52df3f795c644b4398844600000000 prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cVhqpKhx2jgfLUWmyR22JnichoctJCHPtPERm11a2yxnVFKWEKyz"]' | jq -r .hex)
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
99d2b5717fed8875a1ed3b2827dd60ae3089f9caa7c7c23d47635f6f5b397c04
```

## Comprendere l'importanza di questa metodologia di firma ampliata

Ciò ha richiesto un po' di lavoro e, come imparerai presto, la stupidità con le chiavi private, con lo script di riscatto e con lo scriptpubkey non è effettivamente necessaria per riscattare da indirizzi multifirma utilizzando versioni più recenti di Bitcoin Core. Quindi, qual'era il punto?

Questa metodologia di riscatto mostra un modo standard per firmare e riutilizzare le transazioni P2SH. In breve, per riscattare i fondi P2SH, una `signrawtransactionwithkey` deve:

1. Includere `scriptPubKey`, che spiega il puzzle crittografico P2SH.
2. Includere `redeemScript`, che risolve il puzzle crittografico P2SH e introduce un nuovo puzzle tutto suo.
3. Essere eseguito su ogni macchina che contiene le chiavi private richieste.
4. Includere le firme pertinenti, che risolvono il puzzle di riscattoScript.

Qui abbiamo visto questa metodologia utilizzata per riscattare i fondi multisig. In futuro potrai utilizzarlo anche per riscattare i fondi bloccati con altri script P2SH più complessi, come spiegato a partire dal capitolo 9.

## Riepilogo: spendere una transazione con un indirizzo Multisig

Si scopre che spendere denaro inviato a un indirizzo multisig può richiedere un bel po' di lavoro. Ma finché hai i tuoi indirizzi originali e il tuo `redeemScript`, puoi farlo firmando una transazione grezza con ogni indirizzo diverso e fornendo ulteriori informazioni lungo il percorso.

## Qual è il prossimo argomento?

Continua la "Espansione delle transazioni Bitcoin" con [Capitolo 6.3: Inviare e Ricevere una Multifirma Automatizzata](06_3_Inviare_e_Ricevere_una_Multifirma_Automatizzata.md).
