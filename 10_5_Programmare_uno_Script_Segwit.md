# 10.5: Programmare uno script Segwit

> :information_source: **NOTA:** Questa sezione è stata recentemente aggiunta al corso ed è una bozza iniziale che potrebbe essere ancora in attesa di revisione. Lettore avvisato.

Segwit ha introdotto una serie di nuove opzioni per i tipi di indirizzi (e quindi di script). [Capitolo 9.5: Programmare una P2WPKH](09_5_Programmare_una_P2WPKH.md) ha spiegato come il nuovo tipo di indirizzo Bech32 abbia variato gli script standard trovati nella maggior parte delle transazioni tradizionali. Questo capitolo esamina gli altri tre tipi di script introdotti dall'aggiornamento Segwit: il P2SH-Segwit (che era l'indirizzo di transizione "nested Segwit", quando Segwit è stato introdotto), il P2WSH (che è l'equivalente Segwit dell'indirizzo P2SH, proprio come P2WPKH è l'equivalente Segwit dell'indirizzo P2PKH) e l'indirizzo P2WSH annidato.

Questa è un'altra situazione in cui non dovrai davvero preoccuparti di queste sfumature mentre lavori con `bitcoin-cli`, ma è utile sapere come funziona tutto.

## Comprendere uno Script P2SH-Segwit

L'indirizzo P2SH-Segwit è una razza morente. Era fondamentalmente una misura provvisoria mentre Bitcoin stava passando a Segwit, che permetteva a un utente di creare un indirizzo Segwit e poi fare in modo che qualcuno con uno scambio o un portafoglio non abilitato a Segwit finanziasse quell'indirizzo.

Se mai dovessi usarne uno, c'è un'opzione per creare un indirizzo P2SH-Segwit usando `getnewaddress`:
```
$ bitcoin-cli getnewaddress -addresstype p2sh-segwit
2NEzBvokxh4ME4ahdT18NuSSoYvvhS7EnMU
```
L'indirizzo inizia con un `2` (o un `3`), rivelandosi come uno script

> :book: ***Perché i vecchi nodi non possono inviare agli indirizzi Segwit nativi?*** [Capitolo 10.1](10_1_Comprendere_la_Base_di_P2SH.md) ha notato che c'erano un numero stabilito di transazioni Bitcoin "standard". Non puoi effettivamente bloccare una transazione con uno script che non sia uno di quei tipi standard. Segwit è ora riconosciuto come uno di quegli standard, ma un vecchio nodo non lo saprà, e quindi si rifiuterà di inviare su una tale transazione per protezione del mittente. Avvolgere un indirizzo Segwit all'interno di un hash di script risolve il problema.

Quando guardi un UTXO inviato a quell'indirizzo, puoi vedere che il `desc` è diverso, rivelando un indirizzo WPKH avvolto in uno script:

```
$ bitcoin-cli listunspent
  {
    "txid": "ed752673bfd4338ccf0995983086da846ad652ae0f28280baf87f9fd44b3c45f",
    "vout": 1,
    "address": "2NEzBvokxh4ME4ahdT18NuSSoYvvhS7EnMU",
    "redeemScript": "001443ab2a09a1a5f2feb6c799b5ab345069a96e1a0a",
    "scriptPubKey": "a914ee7aceea0865a05a29a28d379cf438ac5b6cd9c687",
    "amount": 0.00095000,
    "confirmations": 1,
    "spendable": true,
    "solvable": true,
    "desc": "sh(wpkh([f004311c/0'/0'/3']03bb469e961e9a9cd4c23db8442d640d9b0b11702dc0126462ac9eb88b64a4dd48))#p29e839h",
    "safe": true
  }
```
Più importante, c'è un `redeemScript`, che si decodifica in `OP_0 OP_PUSHDATA (20 bytes) 3ab2a09a1a5f2feb6c799b5ab345069a96e1a0a`. Questo dovrebbe sembrare familiare, perché è un `OP_0` seguito da un codice esadecimale a 20 byte di un hash di chiave pubblica. In altre parole, un P2SH-SegWit è solo uno `scriptPubKey` SegWit inserito in uno script. Questo è tutto. Corrisponde esattamente a come i multisig moderni sono un multisig inserito in un P2SH, come discusso in [§10.4: Scripting di un Multisig](10_4_Scripting_a_Multisig.md).

Al contrario, quando spendiamo questa transazione, sembra esattamente come un P2SH:

```
$ bitcoin-cli getrawtransaction ed752673bfd4338ccf0995983086da846ad652ae0f28280baf87f9fd44b3c45f 1
{
  "txid": "ed752673bfd4338ccf0995983086da846ad652ae0f28280baf87f9fd44b3c45f",
  "hash": "aa4b1c2bde86ea446c9a9db2f77e27421316f26a8d88869f5b195f03b1ac4f23",
  "version": 2,
  "size": 247,
  "vsize": 166,
  "weight": 661,
  "locktime": 1781316,
  "vin": [
    {
      "txid": "59178b02cfcbdee51742a4b2658df35b63b51115a53cf802bc6674fd94fa593a",
      "vout": 1,
      "scriptSig": {
        "asm": "00149ef51fb1f5adb44e20eff758d34ae64fa781fa4f",
        "hex": "1600149ef51fb1f5adb44e20eff758d34ae64fa781fa4f"
      },
      "txinwitness": [
        "3044022069a23fcfc421b44c622d93b7639a2152f941dbfd031970b8cef69e6f8e97bd46022026cb801f38a1313cf32a8685749546a5825b1c332ee4409db82f9dc85d99086401",
        "030aec1384ae0ef264718b8efc1ef4318c513403d849ea8466ef2e4acb3c5ccce6"
      ],
      "sequence": 4294967294
    }
  ],
  "vout": [
    {
      "value": 8.49029534,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_HASH160 b4b656f4c4b14ee0d098299d1d6eb42d2e22adcd OP_EQUAL",
        "hex": "a914b4b656f4c4b14ee0d098299d1d6eb42d2e22adcd87",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2N9ik3zihJ91VGNF55sZFe9GiCAXh2cVKKW"
        ]
      }
    },
    {
      "value": 0.00095000,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_HASH160 ee7aceea0865a05a29a28d379cf438ac5b6cd9c6 OP_EQUAL",
        "hex": "a914ee7aceea0865a05a29a28d379cf438ac5b6cd9c687",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2NEzBvokxh4ME4ahdT18NuSSoYvvhS7EnMU"
        ]
      }
    }
  ],
  "hex": "020000000001013a59fa94fd7466bc02f83ca51511b5635bf38d65b2a44217e5decbcf028b175901000000171600149ef51fb1f5adb44e20eff758d34ae64fa781fa4ffeffffff029e299b320000000017a914b4b656f4c4b14ee0d098299d1d6eb42d2e22adcd87187301000000000017a914ee7aceea0865a05a29a28d379cf438ac5b6cd9c68702473044022069a23fcfc421b44c622d93b7639a2152f941dbfd031970b8cef69e6f8e97bd46022026cb801f38a1313cf32a8685749546a5825b1c332ee4409db82f9dc85d9908640121030aec1384ae0ef264718b8efc1ef4318c513403d849ea8466ef2e4acb3c5ccce6442e1b00",
  "blockhash": "0000000069cbe44925fab2d472870608c7e1e241a1590fd78be10c63388ed6ee",
  "confirmations": 282952,
  "time": 1595360859,
  "blocktime": 1595360859
}
```
Ogni `vout` è della forma `OP_HASH160 <HASH> OP_EQUAL`. Questo è un normale P2SH come si vede nel [Capitolo 10.2](10_2_Construire_la_Struttura_di_P2SH.md), il che significa che è solo quando viene eseguito lo script di riscatto che avviene la magia. Proprio come con un P2WPKH, un vecchio nodo vedrà `OP_0 OP_PUSHDATA (20 bytes) 3ab2a09a1a5f2feb6c799b5ab345069a96e1a0a` nello script di riscatto e lo verificherà automaticamente, mentre un nuovo nodo vedrà quello, saprà che è un P2WPKH e quindi andrà a cercare nei `witnesses`. Vedi [Capitolo 9.5: Programmare una P2WPKH](09_5_Programmare_una_P2WPKH.md).

> :book: ***Quali sono gli svantaggi delle transazioni Segwit annidate?*** Sono più grandi delle transazioni Segwit native, quindi ottieni alcuni vantaggi del Segwit, ma non tutti.

## Comprendere uno Script P2WSH

Al contrario, le transazioni P2WSH dovrebbero essere sempre più utilizzate, poiché sono la sostituzione Segwit nativa per P2SH, offrendo tutti gli stessi vantaggi di dimensione del blocco creati con le transazioni P2WPKH Segwit native.

Questo è un esempio di indirizzo P2WSH:
[https://blockstream.info/testnet/address/tb1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3q0sl5k7](https://blockstream.info/testnet/address/tb1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3q0sl5k7)

I dettagli mostrano che un UTXO inviato a questo indirizzo è bloccato con uno `scriptPubKey` come questo:

```
OP_0 OP_PUSHDATA (32 bytes) 1863143c14c5166804bd19203356da136c985678cd4d27a1b8c6329604903262
```
Funziona proprio come un indirizzo P2WPKH, l'unica differenza è che invece di un hash di chiave pubblica a 20 byte, l'UTXO include un hash di script a 32 byte. Proprio come con un P2WPKH, i vecchi nodi lo verificano, mentre i nuovi nodi riconoscono che è un P2WSH e quindi verificano internamente lo script come descritto nelle sezioni precedenti, ma utilizzando i dati `witness`, che ora includono lo script di riscatto.

C'è anche un'altra variante, uno script P2WSH incorporato in uno script P2SH, che funziona molto come il P2SH-Segwit descritto sopra, ma per gli script P2WSH annidati. (Uff!)

## Riepilogo: Scripting di un Segwit Script

Ci sono due tipi di script P2SH che riguardano Segwit.

L'indirizzo P2SH-Segwit è un indirizzo Segwit annidato che incorpora il semplice `scriptPubkey` Segwit all'interno di uno Script, proprio come i multisig sono incorporati negli script oggigiorno: la chiave in stile Segwit viene srotolata e quindi analizzata normalmente su una macchina che comprende Segwit. Lo scopo è la compatibilità all'indietro con i vecchi nodi che altrimenti potrebbero non essere in grado di inviare a indirizzi Segwit nativi.

L'indirizzo P2WSH è una variante Segwit di P2SH, proprio come P2WPKH è una variante Segwit di P2WSH. Funziona con la stessa logica ed è identificato dall'avere un hash a 32 byte invece di un hash a 20 byte. Lo scopo è estendere i vantaggi di Segwit ad altri tipi di script.

## Cosa c'è Dopo?

Continua "Incorporare Bitcoin Scripts" con [Capitolo 10.6: Spendere una Transazione P2SH](10_6_Spendere_una_Transazione_P2SH.md).
