# 10.6: Spendere una Transazione P2SH

Prima di concludere questa panoramica sulle transazioni P2SH, toccheremo come spenderle. Questa sezione è principalmente una panoramica, facendo riferimento a una sezione precedente in cui abbiamo _già_ speso una transazione P2SH.

## Usa il Redeem Script

Come abbiamo visto in [Capitolo 6.2: Spendere una Transazione con un Indirizzo Multifirma](06_2_Spendere_una_Transazione_con_un_Indirizzo_Multifirma.md), spendere una transazione P2SH riguarda tutto l'avere quella versione serializzata dello script di blocco, il cosiddetto _redeemScript_. Quindi, il primo passo per poter spendere una transazione P2SH è assicurarsi di salvare il _redeemScript_ prima di distribuire l'indirizzo P2SH a tutti.

### Raccogli le Tue Variabili

Poiché gli indirizzi P2SH, diversi dagli indirizzi multisig speciali e dagli indirizzi Segwit annidati, non sono integrati in `bitcoin-cli`, non ci saranno scorciatoie per spendere P2SH come hai visto in [Capitolo 6.3: Inviare e Ricevere una Multifirma Automatizzata](06_3_Inviare_e_Ricevere_una_Multifirma_Automatizzata.md). Dovrai raccogliere tutte le variabili più complesse da solo!

Questo significa che devi raccogliere:

   * L'`hex` dello `scriptPubKey` per la transazione che stai spendendo
   * Il `redeemScript` serializzato
   * Qualsiasi chiave privata, poiché firmerai manualmente
   * Tutti i normali `txids`, `vouts` e `addresses` di cui avresti bisogno

## Creare la Transazione

Come abbiamo visto in §6.2, la creazione di una transazione è abbastanza standard:

```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.00005}''')
$ echo $rawtxhex
020000000121654fa95d5a268abf96427e3292baed6c9f6d16ed9e80511070f954883864b10000000000ffffffff0188130000000000001600142c48d3401f6abed74f52df3f795c644b4398844600000000
```
Tuttavia, firmare richiede l'inserimento di informazioni extra per (1) `scriptPubKey`; (2) il `redeemScript`; e (3) qualsiasi chiave privata necessaria.

Ecco l'esempio di farlo per quel multisig incorporato in P2SH nel Capitolo6.2:
```
$ bitcoin-cli -named signrawtransactionwithkey hexstring=$rawtxhex prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cNPhhGjatADfhLD5gLfrR2JZKDE99Mn26NCbERsvnr24B3PcSbtR"]'
```
Con qualsiasi altro tipo di P2SH includerai un diverso `redeemscript`, ma per il resto la pratica è esattamente la stessa. L'unica differenza è che dopo due capitoli di lavoro sugli Script ora capisci cos'è lo `scriptPubKey` e cos'è il `redeemScript`, quindi spero che quelli che erano elementi misteriosi quattro capitoli fa siano ora familiari.

## Riepilogo: Spendere una Transazione P2SH

Hai già speso una P2SH nel Capitolo 6, quando hai reinviato una transazione multisig nel modo difficile, che richiedeva di allineare le informazioni `scriptPubKey` e `redeemScript`. Ora sai che lo `scriptPubKey` è uno script di blocco P2SH standardizzato, mentre il `redeemScript` corrisponde a un hash in quello script di blocco e che devi essere in grado di eseguirlo con le variabili appropriate per ottenere un risultato `True`. Ma oltre a sapere di più, non c'è nulla di nuovo nello spendere una transazione P2SH, perché lo hai già fatto!

## Cosa c'è Dopo?

Avanza attraverso "Bitcoin Scripting" con [Capitolo 11: Potenziare Blocchi Temporali con Scripts di Bitcoin](11_0_Potenziare_Blocchi_Temporali_con_Scripts_di_Bitcoin.md). 
