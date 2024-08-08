# 5.1: Guardare le transazioni ferme

A volte una transazione Bitcoin può fermarsi. Di solito è perché non c'erano commissioni di transazione sufficienti, ma può anche essere a causa di un problema tecnico della rete o del software.

## Controllare le transazioni

Dovresti _sempre_ vigilare per assicurarti che le tue transazioni vadano a buon fine. `bitcoin-cli listtransactions` mostrerà tutte le transazioni in entrata e in uscita, mentre `bitcoin-cli gettransaction` con un txid mostrerà una transazione specifica.

Di seguito viene mostrata una transazione che non è stata inserita in un blocco. Puoi dirlo perché non ha conferme.

```
$ bitcoin-cli -named gettransaction txid=fa2ddf84a4a632586d435e10880a2921db6310dfbd6f0f8f583aa0feacb74c8e
{
  "amount": -0.00020000,
  "fee": -0.00001000,
  "confirmations": 0,
  "trusted": true,
  "txid": "fa2ddf84a4a632586d435e10880a2921db6310dfbd6f0f8f583aa0feacb74c8e",
  "walletconflicts": [
  ],
  "time": 1592953220,
  "timereceived": 1592953220,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "tb1qw508d6qejxtdg4y5r3zarvary0c5xw7kxpjzsx",
      "category": "send",
      "amount": -0.00020000,
      "vout": 0,
      "fee": -0.00001000,
      "abandoned": false
    }
  ],
  "hex": "02000000014cda1f42a1bd39d8d0ff5958a804bc2bc548b71d7ceadbde53ea15aeaf1e2691000000006a473044022016a7a9f045a0f6a52129f48adb7da35c2f54a0741d6614e9d55b8a3bc3e1490a0220391e9085a3697bc790e94bb924d5310e16f23489d9c600864a32674e871f523c01210278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132ffffffff02204e000000000000160014751e76e8199196d454941c45d1b3a323f1433bd6e8030000000000001600146c45d3afa8762086c4bd76d8a71ac7c976e1919600000000"
```
Una transazione può essere considerata bloccata o ferma se rimane in questo stato per un periodo di tempo prolungato. Non molti anni fa, potevi essere sicuro che ogni transazione sarebbe andata a buon fine _una volta..._. Ma non è più così a causa del crescente utilizzo di Bitcoin. Ora, se una transazione rimane bloccata troppo a lungo, uscirà dalla mempool e verrà persa dalla rete Bitcoin.

> :book: ***Cos'è la mempool?*** Mempool (o Memory Pool) è una piscina (pool in inglese) che contiene tutte le transazioni non confermate su un nodo bitcoin. Sono le transazioni che un nodo ha ricevuto dalla rete p2p e che non sono ancora incluse in un blocco. Ogni nodo bitcoin può avere un insieme di transazioni leggermente diverso nel suo mempool: transazioni diverse potrebbero essersi propagate a un nodo specifico. Ciò dipende da quando il nodo è stato avviato l'ultima volta e anche dai suoi limiti su quanto è disposto a memorizzare. Quando un minatore effettua un blocco, utilizza le transazioni dal suo mempool. Quindi, quando un blocco viene verificato, tutti i minatori rimuovono le transazioni in esso contenute dai propri pool. A partire da Bitcoin 0.12, anche le transazioni non confermate possono scadere dai mempool se sono abbastanza vecchie, in genere 72 ore, e a partire dalla versione 0.14.0 il tempo di sfratto è stato aumentato a 2 settimane. I pool minerari potrebbero avere i propri meccanismi di gestione della mempool.

Questo elenco di tutte le [transazioni non confermate](https://blockchain.info/unconfirmed-transactions) potrebbe non corrispondere al mempool di nessuna singola macchina, ma dovrebbe (principalmente) essere un superset di essi.

## Decidere cosa fare

Se la transazione rimane bloccata più a lungo del previsto, in genere puoi fare una di queste tre cose:

**1. Attendi finché non viene cancellato.** Se hai inviato la transazione con una commissione bassa o media, alla fine dovrebbe andare a buon fine. Come mostrato su [Mempool Space](https://mempool.space), quelli con tariffe inferiori subiranno ritardi. (Dai un'occhiata alla transazione più a sinistra e guarda quanto tempo è rimasta in attesa e quanto ha pagato per la sua commissione.)

**2. Attendi la scadenza.** Se hai inviato accidentalmente senza alcuna commissione di transazione o se vengono soddisfatti alcuni numeri o altre condizioni, la transazione potrebbe non andare mai a buon fine. Tuttavia, le tue monete non andranno perse. Finché non disponi di un portafoglio che rinvia intenzionalmente transazioni non confermate, dovrebbe essere cancellato dal mempool entro tre giorni circa, quindi puoi riprovare.

**3. Come mitente utilizza RBF.** Se sei il mittente della transazione e hai aderito a RBF (Replace-By-Fee), puo riprovare con una commissione più alta. Vedere [Capitolo 5.2: Rinviare le Transazioni con RBF](05_2_Rinviare_le_Transazioni_con_RBF.md).

**4. Come destinatario utilizza CPFP.** In alternativa, se sei il destinatario della transazione, puoi utilizzare CPFP (Child-pays-for-parent) per utilizzare la transazione non confermata come input per una nuova transazione. Vedi [Capitolo 5.3: 05_3 Pagare una Transaccion con CPFP](05_3_Pagare_una_Transaccion_con_CPFP.md).

## Riepilogo: Guardare le transazioni ferme

Questa è un'introduzione al potere delle transazioni Bitcoin. Se sai che una transazione è ferma, puoi decidere di liberarla con funzionalità come RBF o CPFP.

## Cosa viene dopo?

Continua "Controllo delle transazioni Bitcoin" con [Capitolo 5.2 Rinviare le Transazioni con RBF](05_2_Rinviare_le_Transazioni_con_RBF.md).
