# 5.3: Pagare una transazione con CPFP

Se la tua transazione Bitcoin è bloccata e sei il _destinatario_, puoi compensarla usando CPFP (child-pays-for-parent). Questa è un'alternativa alla possibilità del _mittente_ di farlo con RBF.

> :warning: **AVVISO DI VERSIONE:** Questa è un'innovazione di Bitcoin Core v 0.13.0, il che significa che la maggior parte delle persone dovrebbe già usarla.

## Capire come funziona CPFP

RBF era tutto incentrato sul mittente. Ha sbagliato e ha dovuto aumentare la commissione, oppure voleva essere intelligente e combinare le transazioni per una serie di motivi. Era una potente funzionalità orientata al mittente. In un certo senso, CPFP è l'opposto di RBF, perché dà potere al destinatario che sa che i suoi soldi non sono ancora arrivati ​​e vuole accelerare il processo. Tuttavia, è anche una funzionalità molto più semplice, con un'applicabilità meno ampia.

Fondamentalmente, l'idea di CPFP è che un destinatario ha una transazione che vuole spendere e non è stata confermata in un blocco. Quindi, include quella transazione non confermata in una nuova transazione e paga una commissione sufficientemente alta da incoraggiare un miner a includere sia la transazione originale (madre) che la nuova transazione (figlia) in un blocco. Di conseguenza, le transazioni madre e figlia vengono cancellate simultaneamente.

Va notato che CPFP non è una nuova funzionalità del protocollo, come RBF. È solo un nuovo schema di incentivazione che può essere utilizzato per la selezione delle transazioni da parte dei miner. Ciò significa anche che non è affidabile come una modifica del protocollo come RBF: potrebbero esserci motivi per cui la figlia non viene selezionata per essere inserita in un blocco e ciò impedirà alla madre di essere inserita in un blocco.

## Spendi UTXO non confermati

Finanziare di una transazione con CPFP è un processo molto semplice che utilizza i metodi con cui hai già familiarità:

   1. Trova `txid` e `vout` della transazione non confermata. Questa potrebbe essere la parte più complicata, poiché `bitcoin-cli` in genere cerca di proteggerti dalle transazioni non confermate. Il mittente potrebbe essere in grado di inviarti queste informazioni; anche solo con il `txid`, dovresti essere in grado di capire il `vout` in un blockchain explorer.

Hai un'altra opzione: usa `bitcoin-cli getrawmempool`, che può essere usato per elencare il contenuto dell'intero mempool, dove si troveranno le transazioni non confermate. Potresti doverle esaminare più volte se il mempool è particolarmente occupato. Puoi quindi ottenere maggiori informazioni su una transazione specifica con `bitcoin-cli getrawtransaction` con il flag verbose impostato su `true`:
   
   ```
$ bitcoin-cli getrawmempool
[
  "95d51e813daeb9a861b2dcdddf1da8c198d06452bbbecfd827447881ff79e061"
]

$ bitcoin-cli getrawtransaction 95d51e813daeb9a861b2dcdddf1da8c198d06452bbbecfd827447881ff79e061 true
{
  "txid": "95d51e813daeb9a861b2dcdddf1da8c198d06452bbbecfd827447881ff79e061",
  "hash": "9729e47b8aee776112a82cec46df7638d112ca51856c53e238a9b1f7af3be4ce",
  "version": 2,
  "size": 247,
  "vsize": 166,
  "weight": 661,
  "locktime": 1773277,
  "vin": [
    {
      "txid": "7a0178472300247d423ac4a04ff9165fa5b944104f6d6f9ebc557c6d207e7524",
      "vout": 0,
      "scriptSig": {
        "asm": "0014334f3a112df0f22e743ad97eec8195a00faa59a0",
        "hex": "160014334f3a112df0f22e743ad97eec8195a00faa59a0"
      },
      "txinwitness": [
        "304402207966aa87db340841d76d3c3596d8b4858e02aed1c02d87098dcedbc60721d8940220218aac9d728c9a485820b074804a8c5936fa3145ce68e24dcf477024b19e88ae01",
        "03574b1328a5dc2d648498fc12523cdf708efd091c28722a422d122f8a0db8daa9"
      ],
      "sequence": 4294967294
    }
  ],
  "vout": [
    {
      "value": 0.01000000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_HASH160 f079f77f2ef0ef1187093379d128ec28d0b4bf76 OP_EQUAL",
        "hex": "a914f079f77f2ef0ef1187093379d128ec28d0b4bf7687",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2NFAkGiwnp8wvCodRBx3smJwxncuG3hndn5"
        ]
      }
    },
    {
      "value": 0.02598722,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_HASH160 8799be12fb9eae6644659d95b9602ddfbb4b2aff OP_EQUAL",
        "hex": "a9148799be12fb9eae6644659d95b9602ddfbb4b2aff87",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2N5cDPPuCTtYq13oXw8RfpY9dHJW8sL64U2"
        ]
      }
    }
  ],
  "hex": "0200000000010124757e206d7c55bc9e6f6d4f1044b9a55f16f94fa0c43a427d2400234778017a0000000017160014334f3a112df0f22e743ad97eec8195a00faa59a0feffffff0240420f000000000017a914f079f77f2ef0ef1187093379d128ec28d0b4bf768742a727000000000017a9148799be12fb9eae6644659d95b9602ddfbb4b2aff870247304402207966aa87db340841d76d3c3596d8b4858e02aed1c02d87098dcedbc60721d8940220218aac9d728c9a485820b074804a8c5936fa3145ce68e24dcf477024b19e88ae012103574b1328a5dc2d648498fc12523cdf708efd091c28722a422d122f8a0db8daa9dd0e1b00"
}
```
Guarda l'array `vout`. Trova l'oggetto che corrisponde al tuo indirizzo. (Qui, è l'unico.) Il valore `n` è il tuo `vout`. Ora hai tutto ciò che ti serve per creare una nuova transazione CPFP.

```
$ utxo_txid=2NFAkGiwnp8wvCodRBx3smJwxncuG3hndn5
$ utxo_vout=0
$ recipient2=$(bitcoin-cli getrawchangeaddress)
```
   2. Crea una transazione raw usando la tua transazione non confermata come input.

   3. Raddoppia le commissioni di transazione (o di più).

Quando esegui questi passaggi, il tutto dovrebbe apparire normale, nonostante tu stia lavorando con una transazione non confermata. Per verificare che tutto andasse bene, abbiamo persino esaminato i risultati della nostra firma prima di salvare le informazioni in una variabile:

```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient2'": 0.03597 }''')

$ bitcoin-cli -named signrawtransaction hexstring=$rawtxhex | jq -r '.hex'
02000000012b137ef780666ba214842ff6ea2c3a0b36711bcaba839c3710f763e3d9687fed000000006a473044022003ca1f6797d781ef121ba7c2d1d41d763a815e9dad52aa8bc5ea61e4d521f68e022036b992e8e6bf2c44748219ca6e0056a88e8250f6fd0794dc69f79a2e8993671601210317b163ab8c8862e09c71767112b828abd3852e315441893fa0f535de4fa39b8dffffffff01905abd07000000001976a91450b1d90a130c4f3f1e5fbfa7320fd36b7265db0488ac00000000

$ signedtx=$(bitcoin-cli -named signrawtransaction hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
6a184a2f07fa30189f4831d6f041d52653a103b3883d2bec2f79187331fd7f0e
```

   4. Non è necessario incrociare le dita. Hai verificato che i tuoi dati siano corretti. Da questo punto in poi, le cose sono fuori dal tuo controllo.

Le tue transazioni potrebbero essere eseguite rapidamente. Potrebbero non esserlo. Tutto dipende dal fatto che i minatori che generano casualmente i blocchi correnti abbiano o meno la patch CPFP. Ma hai dato alle tue transazioni la migliore possibilità.

Questo è tutto.

### Fai attenzione alle sfumature

Sebbene CPFP sia solitamente descritto come un destinatario che utilizza una nuova transazione per pagare una vecchia transazione che non è stata confermata, c'è una sfumatura in questo.

Un _mittente_ potrebbe utilizzare CPFP per liberare una transazione se ricevesse il resto da essa. Utilizzerebbe semplicemente quel resto come input e l'uso risultante di CPFP libererebbe l'intera transazione. Tieni presente che farebbe meglio a utilizzare RBF finché fosse abilitato, poiché le commissioni totali sarebbero quindi inferiori.

Un _destinatario_ potrebbe usare CPFP anche se non ha intenzione di spendere immediatamente i soldi, ad esempio se teme che i fondi non possano essere rispediti se la transazione scade. In questo caso, crea semplicemente una transazione secondaria che invia tutti i soldi (meno una commissione di transazione) a un indirizzo di resto. È quello che abbiamo fatto sopra nel nostro esempio.

## Riepilogo: Pagare una transazione con CPFP

Puoi sfruttare gli incentivi CPFP per liberare i fondi che ti sono stati inviati ma non sono stati confermati. Usa semplicemente la transazione non confermata come UTXO e paga una commissione di transazione superiore alla media.

> :fire: ***Qual è il potere di CPFP?*** Principalmente, CPFP è utile solo per sbloccare i fondi quando sei il destinatario e il mittente non puo aiutare per qualsiasi motivo. Non ha le possibilità più potenti di RBF, ma è un modo alternativo per esercitare il controllo su una transazione dopo che è stata inserita nel mempool, ma prima che venga confermata in un blocco.

## Cosa c'è dopo?

Vai attraverso "bitcoin-cli" con [Capitolo 6: Ampliare le Transazioni Bitcoin con Multifirme](06_0_Ampliare_le_Transazioni_Bitcoin_con_Multifirme.md).
