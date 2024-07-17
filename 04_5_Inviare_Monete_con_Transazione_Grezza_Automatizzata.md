# 4.5: Inviare monete con transazioni grezze automatizzate

Questo capitolo illustra tre modi per inviare fondi tramite l'interfaccia CLI di Bitcoin. [Capitolo 4.1](04_1_Inviare_Monete_Modo_Semplice.md) ha descritto come farlo con un semplice comando e [Capitolo 4.4](04_4_Inviare_Monete_con_Transazione_Grezza.md) ha spiegato in dettaglio come utilizzare una transazione grezza più pericolosa. Questa sezione finale risolve la differenza mostrando come rendere le transazioni grezze più semplici e sicure.

## Lascia che Bitcoin calcoli per te

La metodologia per le transazioni grezze automatizzate è semplice: crei una transazione grezza, ma usi il comando "fundrawtransaction" per chiedere a bitcoind di eseguire i calcoli per te.

Per utilizzare questo comando, dovrai assicurarti che il tuo file ~/.bitcoin/bitcoin.conf contenga variabili razionali per il calcolo delle commissioni di transazione. Consulta [Capitolo 4.1: Inviare Monete Modo Semplice](04_1_Inviare_Monete_Modo_Semplice.md) per ulteriori informazioni a riguardo.

Per numeri molto conservativi, abbiamo suggerito di aggiungere quanto segue a `bitcoin.conf`:
```
mintxfee=0.0001
txconfirmtarget=6
```
Per far avanzare il tutorial (e più in generale per spostare velocemente il denaro) abbiamo suggerito quanto segue:
```
mintxfee=0.001
txconfirmtarget=1
```

## Crea una transazione grezza semplice semplice

Per utilizzare `fundrawtransaction` devi prima creare una transazione semplice che elenchi _no_ input e _no_ cambi di indirizzo. Elencherai semplicemente il destinatario e l'importo che desideri inviargli, in questo caso `$recipient` e `0.00020000` BTC.
```
$ recipient=n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi
$ unfinishedtx=$(bitcoin-cli -named createrawtransaction inputs='''[]''' outputs='''{ "'$recipient'": 0.0002 }''')
```

## Finanzia la tua transazione semplice

Quindi dici a 'bitcoin-cli' di finanziare quella transazione semplice:

```
$ bitcoin-cli -named fundrawtransaction hexstring=$unfinishedtx
{
  "hex": "02000000012db87641c6a21e5a68b20c226428544978e6ac44964d5d8060d7388000c584eb0100000000feffffff02204e0000000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac781e0000000000001600140cc9cdcf45d4ea17f5227a7ead52367aad10a88400000000",
  "fee": 0.00022200,
  "changepos": 1
}
```
Ciò fornisce molte informazioni utili, ma una volta che sei sicuro di come funziona, ti consigliamo di utilizzare JQ per salvare il tuo esadecimale in una variabile, come al solito:
```
$ rawtxhex3=$(bitcoin-cli -named fundrawtransaction hexstring=$unfinishedtx | jq -r '.hex')
```
## Verifica la tua transazione finanziata

Sembra una magia, quindi le prime volte che usi `fundrawtransaction`, probabilmente vorrai verificarlo.

L'esecuzione di `decoderawtransaction` mostrerà che la transazione grezza è ora strutturata correttamente, utilizzando uno o più dei tuoi UTXO e inviando i fondi in eccesso a un indirizzo di resto:

```
$ bitcoin-cli -named decoderawtransaction hexstring=$rawtxhex3
{
  "txid": "b3b4c2057dbfbef6690e975ede92fde805ddea13c730f58401939a52c9ac1b99",
  "hash": "b3b4c2057dbfbef6690e975ede92fde805ddea13c730f58401939a52c9ac1b99",
  "version": 2,
  "size": 116,
  "vsize": 116,
  "weight": 464,
  "locktime": 0,
  "vin": [
    {
      "txid": "eb84c5008038d760805d4d9644ace67849542864220cb2685a1ea2c64176b82d",
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
      "value": 0.00020000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 e7c1345fc8f87c68170b3aa798a956c2fe6a9eff OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi"
        ]
      }
    },
    {
      "value": 0.00007800,
      "n": 1,
      "scriptPubKey": {
        "asm": "0 a782f4c6e1e75a5b24f3d675d6f11b5ebf3b2142",
        "hex": "0014a782f4c6e1e75a5b24f3d675d6f11b5ebf3b2142",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1q57p0f3hpuad9kf8n6e6adugmt6lnkg2zzr592r"
        ]
      }
    }
  ]
}
```
La cosa interessante qui è l'indirizzo di resto, che è il secondo "vout". Nota che è un indirizzo "tb1", il che significa che è Bech32; quando abbiamo dato a Bitcoin Core la totale capacità di gestire il nostro resto, lo ha fatto utilizzando il suo tipo di indirizzo predefinito, Bech32, e ha funzionato bene. Ecco perché il nostro indirizzo di resto non è un grosso problema, come vedrai nel [Capitolo 4.6](04_6_Creare_una_Transazione_Segwit.md) , ma ci sono alcuni trucchi per un utilizzo più ampio, di cui parleremo lì.

Sebbene abbiamo visto la commissione nell'output di `fundrawtransaction`, non è visibile qui. Tuttavia, puoi verificarlo con lo script JQ `txfee-calc.sh` creato in [JQ Interludio](04_2_Intermezzo_Usare_JQ.md):
```
$ ~/txfee-calc.sh $rawtxhex3
.000222
```
Infine, puoi utilizzare `getaddressinfo` per vedere che l'indirizzo di modifica generato appartiene davvero a te:

```
$ bitcoin-cli -named getaddressinfo address=tb1q57p0f3hpuad9kf8n6e6adugmt6lnkg2zzr592r
{
  "address": "tb1q57p0f3hpuad9kf8n6e6adugmt6lnkg2zzr592r",
  "scriptPubKey": "0014a782f4c6e1e75a5b24f3d675d6f11b5ebf3b2142",
  "ismine": true,
  "solvable": true,
  "desc": "wpkh([d6043800/0'/1'/10']038a2702938e548eaec28feb92c7e4722042cfd1ea16bec9fc274640dc5be05ec5)#zpv26nar",
  "iswatchonly": false,
  "isscript": false,
  "iswitness": true,
  "witness_version": 0,
  "witness_program": "a782f4c6e1e75a5b24f3d675d6f11b5ebf3b2142",
  "pubkey": "038a2702938e548eaec28feb92c7e4722042cfd1ea16bec9fc274640dc5be05ec5",
  "ischange": true,
  "timestamp": 1592335137,
  "hdkeypath": "m/0'/1'/10'",
  "hdseedid": "fdea8e2630f00d29a9d6ff2af7bf5b358d061078",
  "hdmasterfingerprint": "d6043800",
  "labels": [
  ]
}
```
Nota i risultati di `ismine`.

## Invia la tua transazione finanziata

A questo punto potrai firmare e inviare la transazione come di consueto.

```
$ signedtx3=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex3 | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx3
8b9dd66c999966462a3d88d6ac9405d09e2aa409c0aa830bdd08dbcbd34a36fa
```
In pochi minuti avrai indietro il resto:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "8b9dd66c999966462a3d88d6ac9405d09e2aa409c0aa830bdd08dbcbd34a36fa",
    "vout": 1,
    "address": "tb1q57p0f3hpuad9kf8n6e6adugmt6lnkg2zzr592r",
    "scriptPubKey": "0014a782f4c6e1e75a5b24f3d675d6f11b5ebf3b2142",
    "amount": 0.00007800,
    "confirmations": 1,
    "spendable": true,
    "solvable": true,
    "desc": "wpkh([d6043800/0'/1'/10']038a2702938e548eaec28feb92c7e4722042cfd1ea16bec9fc274640dc5be05ec5)#zpv26nar",
    "safe": true
  }
]
```

## Riepilogo: invio di monete con transazioni grezze automatizzate

Se devi inviare fondi con transazioni grezze, allora "fundrawtransaction" ti offre una bella alternativa in cui commissioni, input e output vengono calcolati per te, in modo da non perdere accidentalmente un sacco di soldi.

> :fire: ***Qual è il potere di inviare monete con transazioni grezze automatizzate?***

> _I vantaggi._ Fornisce un buon equilibrio. Se invii fondi manualmente e "sendtoaddress" non offre un controllo sufficiente per qualsiasi motivo, puoi ottenere alcuni dei vantaggi delle transazioni grezze senza i pericoli. Questa metodologia dovrebbe essere utilizzata quando possibile se si inviano transazioni grezze manualmente.

> _Gli svantaggi._ È un miscuglio. Sebbene esistano alcune opzioni aggiuntive per il comando "fundrawtransaction" che non sono state menzionate qui, il tuo controllo è comunque limitato. Probabilmente non vorrai mai utilizzare questo metodo se stessi scrivendo un programma in cui l'obiettivo è sapere esattamente cosa sta succedendo.

## Qual è il prossimo argomento?

Completa il tuo "Invio di transazioni Bitcoin" con [Capitolo 4.6: Creare una Transazione Segwit](04_6_Creare_una_Transazione_Segwit.md).

