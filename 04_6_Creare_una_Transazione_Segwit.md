# 4.6: Creazione di una transazione SegWit

> :information_source: **NOTA:** Questa sezione è stata aggiunta di recente al corso ed è una bozza iniziale che potrebbe essere ancora in attesa di revisione. Lettore avvertito.

C'era una volta, i cieli dei Bitcoin tremavano a causa delle guerre dei blocchi. Le tariffe erano alle stelle e gli utenti erano preoccupati per il ridimensionamento. Gli sviluppatori di Bitcoin Core erano riluttanti ad aumentare semplicemente la dimensione dei blocchi, ma sono giunti a un compromesso: SegWit, the Segregated Witness. Testimone Segregato è un modo elegante per dire "Firma Separata". Crea nuovi tipi di transazioni che rimuovono le firme alla fine della transazione. Combinando questo con dimensioni di blocchi aumentate che sono visibili solo ai nodi aggiornati, SegWit ha risolto i problemi di ridimensionamento per Bitcoin in quel momento (e ha anche risolto un brutto bug di malleabilità che in precedenza aveva reso impraticabile un ridimensionamento ancora migliore con protocolli di livello 2 come Lightning).

L'inghippo? SegWit utilizza indirizzi diversi, alcuni dei quali sono compatibili con i nodi più vecchi e altri no.

> :warning: **AVVERTIMENTO VERSIONE:** SegWit è stato introdotto in BitCoin 0.16.0 con quello che all'epoca veniva descritto come "supporto completo". Detto questo, all'epoca c'erano alcuni difetti nella sua integrazione con "bitcoin-cli" che impedivano il corretto funzionamento della firma sui nuovi indirizzi P2SH-SegWit. L'indirizzo Bech32 non compatibile con le versioni precedenti è stato introdotto anche in Bitcoin 0.16.0 ed è stato reso il tipo di indirizzo predefinito in Bitcoin 0.19.0. Tutte queste funzionalità ora dovrebbero funzionare completamente per quanto riguarda le funzioni `bitcoin-cli` (e quindi questo tutorial).

> Il problema sta nell'interagire con il mondo esterno. Tutti dovrebbero essere in grado di inviare a un indirizzo P2SH-SegWit perché è stato appositamente creato per supportare la compatibilità con le versioni precedenti racchiudendo la funzionalità SegWit in uno script Bitcoin. Lo stesso non vale per gli indirizzi Bech32: se qualcuno ti dice che non è in grado di inviare al tuo indirizzo Bech32, ecco perché, e devi generare un indirizzo `legacy` o P2SH-SegWit per il loro utilizzo. (Molti siti, in particolare gli exchange, non possono nemmeno generare o ricevere indirizzi SegWit, in particolare indirizzi Bech32, ma questo è un problema completamente diverso e non influisce sul tuo utilizzo.)

## Comprendere una transazione SegWit

Nelle transazioni classiche, le informazioni sulla firma (testimone) venivano archiviate verso la metà della transazione, mentre nelle transazioni SegWit si trovano in fondo. Ciò va di pari passo con l'aumento delle dimensioni dei blocchi introdotti nell'aggiornamento SegWit. La dimensione del blocco è stata aumentata da 1M a un importo variabile in base al numero di transazioni SegWit presenti in un blocco, partendo da 1M (nessuna transazione SegWit) e arrivando fino a 4M (tutte le transazioni SegWit). Questa dimensione variabile è stata creata per accogliere i nodi classici, in modo che tutto rimanga compatibile con le versioni precedenti. Se un nodo classico vede una transazione SegWit, elimina le informazioni testimone (risultando in un blocco di dimensioni più piccole, sotto il vecchio limite di 1 milione), mentre se un nuovo nodo vede una transazione SegWit, mantiene le informazioni testimone (risultando in una transazione SegWit più grande blocco, fino al nuovo limite di 4M).

Ecco il cosa e come delle transazioni SegWit. Non che tu abbia bisogno di sapere tutto cio per usarle. La maggior parte delle transazioni sulla rete Bitcoin ora sono SegWit. Sono ciò che utilizzerai nativamente per inviare e ricevere denaro. I dettagli a questo punto non sono più rilevanti dei particolari su come funziona la maggior parte di Bitcoin.


## Crea un indirizzo SegWit

Puoi creare un indirizzo SegWit allo stesso modo di qualsiasi altro indirizzo, con i comandi `getnewaddress` e `getrawchangeaddress`.

Se devi creare un indirizzo per qualcuno che non può inviare ai nuovi indirizzi Bech32, utilizza il tipo di indirizzo `p2sh-segwit`:

```
$ bitcoin-cli -named getnewaddress address_type=p2sh-segwit
2N5h2r4karVqN7uFtpcn8xnA3t5cbpszgyN
```
Se vedi un indirizzo con il prefisso "2" significa che hai fatto bene.

> :link: **TESTNET vs MAINNET:** "3" per Mainnet.

Tuttavia, se la persona con cui stai interagendo ha un cliente completo e aggiornato, sarà in grado di inviare a un indirizzo Bech32, che creerai utilizzando i comandi nel modo predefinito:

```
$ bitcoin-cli getnewaddress
tb1q5gnwrh7ss5mmqt0qfan85jdagmumnatcscwpk6
```
Come abbiamo già visto, gli indirizzi di modifica generati da `bitcoin-cli` interagiscono bene con gli indirizzi Bech32, quindi non ha senso usare il flag `legacy` neanche lì:
```
$ bitcoin-cli getrawchangeaddress
tb1q05wx5tyadm8qe83exdqdyqvqqzjt3m38vfu8ff
```
Qui, nota che il prefisso univoco "tb1" denotava Bech32.

> :link: **TESTNET vs MAINNET:** "bc1" per mainnet.

A `bitcoin-cli` non importa quale tipo di indirizzo stai utilizzando. Puoi eseguire un comando come `listaddressgroupings` e mescolerà liberamente indirizzi di diversi tipi:
```
$ bitcoin-cli listaddressgroupings
[
  [
    [
      "mfsiRhxbQxcD7HLS4PiAim99oeGyb9QY7m",
      0.01000000,
      ""
    ]
  ],
  [
    [
      "mi25UrzHnvn3bpEfFCNqJhPWJn5b77a5NE",
      0.00000000,
      ""
    ],
    [
      "tb1q6dak4e9fz77vsulk89t5z92l2e0zm37yvre4gt",
      0.00000000
    ]
  ],
  [
    [
      "mjehC2KHzXcBDcwTd4LhZ2GzyzrZ3Kd3ff",
      0.00022000,
      ""
    ]
  ],
  [
    [
      "mk9ry5VVy8mrA8SygxSQQUDNSSXyGFot6h",
      0.00000000
    ],
    [
      "mqjrdY5raxKzXQf5t2VvVvzhvFAgersu9B",
      0.00000000
    ],
    [
      "mwJL7cRiW2bUnY81r1thSu3D4jtMmwyU6d",
      0.00000000,
      ""
    ],
    [
      "tb1q57p0f3hpuad9kf8n6e6adugmt6lnkg2zzr592r",
      0.00007800
    ]
  ],
  [
    [
      "mpVLL7iqPr4d7BJkEG54mcdm7WmrAhaW6q",
      0.01000000,
      ""
    ]
  ],
  [
    [
      "tb1q5gnwrh7ss5mmqt0qfan85jdagmumnatcscwpk6",
      0.01000000,
      ""
    ]
  ]
]
```

## Invia una transazione SegWit nel modo più semplice

Allora come si invia una transazione Segwit? Esattamente come qualsiasi altra transazione. Non importa se l'UTXO è SegWit, l'indirizzo è SegWit o una combinazione di questi. Puoi aspettarti che `bitcoin-cli` faccia la cosa giusta. Anche se puoi notare le differenze tramite gli indirizzi, non sono importanti per l'interazione con le cose a livello di "bitcoin-cli" o RPC. (E questo è uno dei vantaggi dell'utilizzo della riga di comando e dell'interfaccia RPC, come suggerito in questo tutorial: gli esperti hanno già svolto il duro lavoro per te, comprese cose come come inviare sia agli indirizzi legacy che a quelli Bech32. Utilizza tale funzionalità a proprio vantaggio.)

Ecco un esempio di invio a un indirizzo SegWit, nel modo più semplice:

```
$ bitcoin-cli sendtoaddress tb1qw508d6qejxtdg4y5r3zarvary0c5xw7kxpjzsx 0.005
854a833b667049ac811b4cf1cad40fa7f8dce8b0f4c1018a58b84559b6e05f42
```
Se guardi la tua transazione, puoi vedere l'uso dell'indirizzo Bech32:
```
$ bitcoin-cli -named gettransaction txid="854a833b667049ac811b4cf1cad40fa7f8dce8b0f4c1018a58b84559b6e05f42" verbose=true
{
  "amount": -0.00500000,
  "fee": -0.00036600,
  "confirmations": 0,
  "trusted": true,
  "txid": "854a833b667049ac811b4cf1cad40fa7f8dce8b0f4c1018a58b84559b6e05f42",
  "walletconflicts": [
  ],
  "time": 1592948795,
  "timereceived": 1592948795,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "tb1qw508d6qejxtdg4y5r3zarvary0c5xw7kxpjzsx",
      "category": "send",
      "amount": -0.00500000,
      "vout": 1,
      "fee": -0.00036600,
      "abandoned": false
    }
  ],
  "hex": "0200000002114d5a4c3b847bc796b2dc166ca7120607b874aa6904d4a43dd5f9e0ea79d4ba010000006a47304402200a3cc08b9778e7b616340d4cf7841180321d2fa019e43f25e7f710d9a628b55c02200541fc200a07f2eb073ad8554357777d5f1364c5a96afe5e77c6185d66a40fa7012103ee18c598bafc5fbea72d345329803a40ebfcf34014d0e96aac4f504d54e7042dfeffffffa71321e81ef039af490251379143f7247ad91613c26c8f3e3404184218361733000000006a47304402200dd80206b57beb5fa38a3c3578f4b0e40d56d4079116fd2a6fe28e5b8ece72310220298a8c3a1193ea805b27608ff67a2d8b01e347e33a4222edfba499bb1b64a31601210339c001b00dd607eeafd4c117cfcf86be8efbb0ca0a33700cffc0ae0c6ee69d7efeffffff026854160000000000160014d591091b8074a2375ed9985a9c4b18efecfd416520a1070000000000160014751e76e8199196d454941c45d1b3a323f1433bd6c60e1b00",
  "decoded": {
    "txid": "854a833b667049ac811b4cf1cad40fa7f8dce8b0f4c1018a58b84559b6e05f42",
    "hash": "854a833b667049ac811b4cf1cad40fa7f8dce8b0f4c1018a58b84559b6e05f42",
    "version": 2,
    "size": 366,
    "vsize": 366,
    "weight": 1464,
    "locktime": 1773254,
    "vin": [
      {
        "txid": "bad479eae0f9d53da4d40469aa74b8070612a76c16dcb296c77b843b4c5a4d11",
        "vout": 1,
        "scriptSig": {
          "asm": "304402200a3cc08b9778e7b616340d4cf7841180321d2fa019e43f25e7f710d9a628b55c02200541fc200a07f2eb073ad8554357777d5f1364c5a96afe5e77c6185d66a40fa7[ALL] 03ee18c598bafc5fbea72d345329803a40ebfcf34014d0e96aac4f504d54e7042d",
          "hex": "47304402200a3cc08b9778e7b616340d4cf7841180321d2fa019e43f25e7f710d9a628b55c02200541fc200a07f2eb073ad8554357777d5f1364c5a96afe5e77c6185d66a40fa7012103ee18c598bafc5fbea72d345329803a40ebfcf34014d0e96aac4f504d54e7042d"
        },
        "sequence": 4294967294
      },
      {
        "txid": "33173618421804343e8f6cc21316d97a24f7439137510249af39f01ee82113a7",
        "vout": 0,
        "scriptSig": {
          "asm": "304402200dd80206b57beb5fa38a3c3578f4b0e40d56d4079116fd2a6fe28e5b8ece72310220298a8c3a1193ea805b27608ff67a2d8b01e347e33a4222edfba499bb1b64a316[ALL] 0339c001b00dd607eeafd4c117cfcf86be8efbb0ca0a33700cffc0ae0c6ee69d7e",
          "hex": "47304402200dd80206b57beb5fa38a3c3578f4b0e40d56d4079116fd2a6fe28e5b8ece72310220298a8c3a1193ea805b27608ff67a2d8b01e347e33a4222edfba499bb1b64a31601210339c001b00dd607eeafd4c117cfcf86be8efbb0ca0a33700cffc0ae0c6ee69d7e"
        },
        "sequence": 4294967294
      }
    ],
    "vout": [
      {
        "value": 0.01463400,
        "n": 0,
        "scriptPubKey": {
          "asm": "0 d591091b8074a2375ed9985a9c4b18efecfd4165",
          "hex": "0014d591091b8074a2375ed9985a9c4b18efecfd4165",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1q6kgsjxuqwj3rwhkenpdfcjccalk06st9z0k0kh"
          ]
        }
      },
      {
        "value": 0.00500000,
        "n": 1,
        "scriptPubKey": {
          "asm": "0 751e76e8199196d454941c45d1b3a323f1433bd6",
          "hex": "0014751e76e8199196d454941c45d1b3a323f1433bd6",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1qw508d6qejxtdg4y5r3zarvary0c5xw7kxpjzsx"
          ]
        }
      }
    ]
  }
}
```
Infatti, entrambi i `vouts` utilizzano indirizzi Bech32: Il destinatario e l'indirizzo di resto generato automaticamente.

Ma quando torniamo indietro al nostro "vin", scopriamo che proviene da un indirizzo `legacy`. Ma perche non importa:
```
$ bitcoin-cli -named gettransaction txid="33173618421804343e8f6cc21316d97a24f7439137510249af39f01ee82113a7"
{
  "amount": 0.01000000,
  "confirmations": 43,
  "blockhash": "00000000000000e2365d2f814d1774b063d9a04356f482010cdfdd537b1a24bb",
  "blockheight": 1773212,
  "blockindex": 103,
  "blocktime": 1592937103,
  "txid": "33173618421804343e8f6cc21316d97a24f7439137510249af39f01ee82113a7",
  "walletconflicts": [
  ],
  "time": 1592936845,
  "timereceived": 1592936845,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "mpVLL7iqPr4d7BJkEG54mcdm7WmrAhaW6q",
      "category": "receive",
      "amount": 0.01000000,
      "label": "",
      "vout": 0
    }
  ],
  "hex": "020000000001016a66efa334f06e2c54963e48d049a35d7a1bda44633b7464621cae302f35174a0100000017160014f17b16c6404e85165af6f123173e0705ba31ec25feffffff0240420f00000000001976a914626ab1ca41d98f597d18d1ff8151e31a40d4967288acd2125d000000000017a914d5e76abfe5362704ff6bbb000db9cdfa43cd2881870247304402203b3ba83f51c1895b5f639e9bfc40124715e2495ef2c79d4e49c0f8f70fbf2feb02203d50710abe3cf37df4d2a73680dadf3cecbe4f2b5d0b276dbe7711d0c2fa971a012102e64f83ee1c6548bcf44cb965ffdb803f30224459bd2e57a5df97cb41ba476b119b0e1b00"
}
```

## Invia una transazione SegWit nel modo più difficile

Allo stesso modo puoi finanziare una transazione con un indirizzo Bech32 senza alcuna differenza rispetto alle tecniche che hai imparato finora. Ecco come farlo esattamente con una transazione grezza completa:

```
$ changeaddress=$(bitcoin-cli getrawchangeaddress)
$ echo $changeaddress
tb1q4xje3mx9xn7f8khv7p69ekfn0q72kfs8x3ay4j
$ bitcoin-cli listunspent
[
...
  {
    "txid": "003bfdca5578c0045a76768281f05d5e6f57774be399a76f387e2a0e99e4e452",
    "vout": 0,
    "address": "tb1q5gnwrh7ss5mmqt0qfan85jdagmumnatcscwpk6",
    "label": "",
    "scriptPubKey": "0014a226e1dfd08537b02de04f667a49bd46f9b9f578",
    "amount": 0.01000000,
    "confirmations": 5,
    "spendable": true,
    "solvable": true,
    "desc": "wpkh([d6043800/0'/0'/5']0327dbe2d58d9ed2dbeca28cd26e18f48aa94c127fa6fb4b60e4188f6360317640)#hd66hknp",
    "safe": true
  }
]
$ recipient=tb1qw508d6qejxtdg4y5r3zarvary0c5xw7kxpjzsx
$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[2] | .txid')
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[2] | .vout')
$ echo $utxo_txid $utxo_vout
003bfdca5578c0045a76768281f05d5e6f57774be399a76f387e2a0e99e4e452 0
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.002, "'$changeaddress'": 0.007 }''')
$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
e02568b706b21bcb56fcf9c4bb7ba63fdbdec1cf2866168c4f50bc0ad693f26c
```
Funziona tutto esattamente come gli altri tipi di transazioni!

### Riconoscere il nuovo descrittore

Se guardi il campo `desc`, noterai che l'indirizzo SegWit ha un descrittore di stile diverso da quelli incontrati in [Capitolo 3.5: Comprendere il Descriptor](03_5_Comprendere_il_Descriptor.md). 

Il descrittore legacy descritto nell [Capitolo 3.5] assomigliava a questo:

`pkh([d6043800/0'/0'/18']03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388)#4ahsl9pk`.

Il nostro nuovo descrittore SegWit invece si presenta così:

`wpkh([d6043800/0'/0'/5']0327dbe2d58d9ed2dbeca28cd26e18f48aa94c127fa6fb4b60e4188f6360317640)#hd66hknp"`.

La cosa importante da notare è che la funzione è cambiata. In precedenza era `pkh`, che è un indirizzo a P2PKH hashed public-key standard. L'indirizzo SegWit è invece `wpkh`, il che significa che è un indirizzo SegWit nativo P2WPKH. Ciò sottolinea il :fuoco: ***potere dei descrittori***. Descrivono come creare un indirizzo da una chiave o altre informazioni, con le funzioni che definiscono in modo inequivocabile come creare l'indirizzo in base al suo tipo.

## Riepilogo: creare una transazione SegWit

Non c'è davvero alcuna complessità nella creazione di transazioni SegWit. Internamente sono strutturate in modo diverso rispetto alle transazioni legacy, ma dalla riga di comando non c'è differenza: basta utilizzare un indirizzo con un prefisso diverso. L'unica cosa a cui prestare attenzione è che alcune persone potrebbero non essere in grado di inviare a un indirizzo Bech32 se utilizzano software obsoleto.

> :fire: ***Qual è il potere di inviare monete con SegWit?***

> _I vantaggi._ Le transazioni SegWit sono più piccole e quindi saranno più economiche da inviare rispetto alle transazioni tradizionali a causa delle commissioni inferiori. Bech32 raddoppia questo vantaggio e crea anche indirizzi che sono più difficili da sbagliare durante la trascrizione, e questo è piuttosto importante, dato che l'errore dell'utente è uno dei modi più probabili per perdere i tuoi bitcoin.

> _Gli svantaggi._ Gli indirizzi SegWit potrebbero non essere supportati dal software Bitcoin obsoleto. In particolare, le persone potrebbero non essere in grado di inviare sats al tuo indirizzo Bech32.

## Qual è il prossimo tema?

Prosegui verso "bitcoin-cli" con il [Capitolo 5: Controllare Transazioni Bitcoin](05_0_Controllare_Transazioni_Bitcoin.md).
