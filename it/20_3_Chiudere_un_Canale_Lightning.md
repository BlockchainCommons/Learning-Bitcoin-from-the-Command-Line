# 20.3: Chiudere un Canale Lightning

> :information_source: **NOTE:** Questa sezione è stata recentemente aggiunta al corso ed è una bozza iniziale che potrebbe essere ancora in attesa di revisione. Attenzione lettore.

In questo capitolo imparerai come chiudere un canale utilizzando l'interfaccia a riga di comando `lightning-cli close`. Chiudere un canale significa che tu e la tua controparte invierete il saldo concordato del canale alla blockchain, per cui dovrai pagare le commissioni di transazione della blockchain e aspettare che la transazione venga minata. Una chiusura può essere cooperativa o non cooperativa, ma funziona in entrambi i casi.

Per chiudere un canale, devi prima conoscere l'ID del nodo remoto; puoi recuperarlo in uno dei due modi.

## Trova i tuoi Canali per Fondi

Puoi usare il comando `lightning-cli listfunds` per vedere i tuoi canali. Questo comando RPC visualizza tutti i fondi disponibili, sia in `outputs` non spesi (UTXO) nel wallet interno sia bloccati in canali attualmente aperti.

```
c$ lightning-cli --testnet listfunds
{
   "outputs": [
      {
         "txid": "66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d",
         "output": 1,
         "value": 99847,
         "amount_msat": "99847000msat",
         "scriptpubkey": "00142fe02e5be9283e8c5bcb93ae61421baf8cb64f9c",
         "address": "tb1q9lszuklf9qlgck7tjwhxzssm47xtvnuu4jslf8",
         "status": "confirmed",
         "blockheight": 1862856,
         "reserved": false
      }
   ],
   "channels": [
      {
         "peer_id": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
         "connected": true,
         "state": "CHANNELD_NORMAL",
         "short_channel_id": "1862856x29x0",
         "channel_sat": 89987,
         "our_amount_msat": "89987000msat",
         "channel_total_sat": 100000,
         "amount_msat": "100000000msat",
         "funding_txid": "66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d",
         "funding_output": 0
      }
   ]
}

  "message_flags": 1,
  "channel_flags": 2,
  "active": true,
  "last_update": 1595508075,
  "base_fee_millisatoshi": 1000,
  "fee_per_millionth": 1,
  "delay": 40,
  "htlc_minimum_msat": "1000msat",
  "htlc_maximum_msat": "280000000msat",
  "features": ""
}
```

Puoi recuperare l'ID del primo canale in una variabile in questo modo:

```
c$ nodeidremote=$(lightning-cli --testnet listfunds | jq '.channels[0] | .peer_id')
```


## Trova i tuoi Canali con JQ

L'altro modo per trovare i canali da chiudere è utilizzare il comando `listchannels`. Restituisce dati sui canali noti al nodo. Poiché i canali possono essere bidirezionali, fino a due nodi verranno restituiti per ogni canale (uno per ogni direzione).

Tuttavia, la rete di gossip di Lightning è molto efficace, e quindi in breve tempo conoscerai migliaia di canali. Questo è ottimo per inviare pagamenti attraverso la rete Lightning, ma meno utile per scoprire i tuoi canali. Per farlo è necessario un po' di lavoro con `jq`.

Prima di tutto, devi conoscere il tuo ID nodo, che può essere recuperato con `getinfo`:

```
c$ nodeid=$(lightning-cli --testnet getinfo | jq .id)
c$ echo $nodeid
"03240a4878a9a64aea6c3921a434e573845267b86e89ab19003b0c910a86d17687"
c$
```
Puoi quindi usarlo per cercare in `listchannels` tutti i canali in cui il tuo nodo è sia la sorgente che la destinazione:

```
c$ lightning-cli --testnet listchannels | jq '.channels[] | select(.source == '$nodeid' or .destination == '$nodeid')'
{
  "source": "03240a4878a9a64aea6c3921a434e573845267b86e89ab19003b0c910a86d17687",
  "destination": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
  "short_channel_id": "1862856x29x0",
  "public": true,
  "satoshis": 100000,
  "amount_msat": "100000000msat",
  "message_flags": 1,
  "channel_flags": 0,
  "active": true,
  "last_update": 1602639570,
  "base_fee_millisatoshi": 1,
  "fee_per_millionth": 10,
  "delay": 6,
  "htlc_minimum_msat": "1msat",
  "htlc_maximum_msat": "99000000msat",
  "features": ""
}
```
Ecco di nuovo il nostro vecchio amico `032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543`, come destinazione.

Una volta che sai cosa hai, puoi memorizzarlo in una variabile:

```
c$ nodeidremote=$(lightning-cli --testnet listchannels | jq '.channels[] | select(.source == '$nodeid' or .destination == '$nodeid') | .destination')
```


## Chiudi un Canale

Ora che hai un ID nodo remoto, sei pronto per usare il comando `lightning-cli close` per chiudere un canale. Per impostazione predefinita, tenterà di chiudere il canale cooperativamente con il peer; se vuoi chiuderlo unilateralmente, imposta l'argomento `unilateraltimeout` con il numero di secondi di attesa. (Se lo imposti a 0 e il peer è online, verrà comunque tentata una chiusura mutuale). In questo esempio, tenterai una chiusura mutuale.



```
c$ lightning-cli --testnet close $nodeidremote 0
{
   "tx": "02000000011d3cf2126ae36e12be3aee893b385ed6a2e19b1da7f4e579e3ef15ca234d69660000000000ffffffff021c27000000000000160014d39feb57a663803da116402d6cb0ac050bf051d9cc5e01000000000016001451c88b44420940c52a384bd8a03888e3676c150900000000",
   "txid": "f68de52d80a1076e36c677ef640539c50e3d03f77f9f9db4f13048519489593f",
   "type": "mutual"
}
```
La transazione di chiusura on-chain è [f68de52d80a1076e36c677ef640539c50e3d03f77f9f9db4f13048519489593f](https://blockstream.info/testnet/tx/f68de52d80a1076e36c677ef640539c50e3d03f77f9f9db4f13048519489593f).

È questa transazione di chiusura che effettivamente distribuisce i fondi scambiati tramite transazioni Lightning. Questo può essere visto esaminando la transazione:

```
$ bitcoin-cli --named getrawtransaction txid=f68de52d80a1076e36c677ef640539c50e3d03f77f9f9db4f13048519489593f verbose=1
{
  "txid": "f68de52d80a1076e36c677ef640539c50e3d03f77f9f9db4f13048519489593f",
  "hash": "3a6b3994932ae781bab80e159314bad06fc55d3d33453a1d663f9f9415c9719c",
  "version": 2,
  "size": 334,
  "vsize": 169,
  "weight": 673,
  "locktime": 0,
  "vin": [
    {
      "txid": "66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d",
      "vout": 0,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "txinwitness": [
        "",
        "304402207f8048e29192ec86019bc83be8b4cac5d1fc682374538bed0707f58192d41c390220512ebcde122d53747feedd70c09153a40c56d09a5fec02e47642afdbb20aa2ac01",
        "3045022100d686a16084b60800fa0f6b14c25dca1c13d10a55c5fb7c6a3eb1c5f4a2fb20360220555f5b6e672cf9ef82941f7d46ee03dd52e0e848b9f094a41ff299deb8207cab01",
        "522102f7589fd8366252cdbb37827dff65e3304abd5d17bbab57460eff71a9e32bc00b210343b980dff4f2723e0db99ac72d0841aad934b51cbe556ce3a1b257b34059a17052ae"
      ],
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 0.00010012,
      "n": 0,
      "scriptPubKey": {
        "asm": "0 d39feb57a663803da116402d6cb0ac050bf051d9",
        "hex": "0014d39feb57a663803da116402d6cb0ac050bf051d9",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1q6w07k4axvwqrmggkgqkkev9vq59lq5we5fcrzn"
        ]
      }
    },
    {
      "value": 0.00089804,
      "n": 1,
      "scriptPubKey": {
        "asm": "0 51c88b44420940c52a384bd8a03888e3676c1509",
        "hex": "001451c88b44420940c52a384bd8a03888e3676c1509",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1q28ygk3zzp9qv223cf0v2qwygudnkc9gfp30ud4"
        ]
      }
    }
  ],
  "hex": "020000000001011d3cf2126ae36e12be3aee893b385ed6a2e19b1da7f4e579e3ef15ca234d69660000000000ffffffff021c27000000000000160014d39feb57a663803da116402d6cb0ac050bf051d9cc5e01000000000016001451c88b44420940c52a384bd8a03888e3676c1509040047304402207f8048e29192ec86019bc83be8b4cac5d1fc682374538bed0707f58192d41c390220512ebcde122d53747feedd70c09153a40c56d09a5fec02e47642afdbb20aa2ac01483045022100d686a16084b60800fa0f6b14c25dca1c13d10a55c5fb7c6a3eb1c5f4a2fb20360220555f5b6e672cf9ef82941f7d46ee03dd52e0e848b9f094a41ff299deb8207cab0147522102f7589fd8366252cdbb37827dff65e3304abd5d17bbab57460eff71a9e32bc00b210343b980dff4f2723e0db99ac72d0841aad934b51cbe556ce3a1b257b34059a17052ae00000000",
  "blockhash": "000000000000002a214b1ffc3a67c64deda838dd24d12154c15d3a6f1137e94d",
  "confirmations": 1,
  "time": 1602713519,
  "blocktime": 1602713519
}
```
L'input della transazione è `66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d`, che era la transazione di finanziamento nel [Capitolo 19.3](19_3_Creare_un_Canale_in_Lightning.md). La transazione ha quindi due output, uno per il nodo remoto e l'altro per il wallet locale c-lightning. L'output all'indice 0 corrisponde al nodo remoto con un valore di 0.00010012 BTC; e l'output all'indice 1 corrisponde al nodo locale con un valore di 0.00089804 BTC.

Lightning mostrerà allo stesso modo 89804 satoshi restituiti come un nuovo UTXO nel suo wallet:

```
$ lightning-cli --network=testnet listfunds
{
   "outputs": [
      {
         "txid": "66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d",
         "output": 1,
         "value": 99847,
         "amount_msat": "99847000msat",
         "scriptpubkey": "00142fe02e5be9283e8c5bcb93ae61421baf8cb64f9c",
         "address": "tb1q9lszuklf9qlgck7tjwhxzssm47xtvnuu4jslf8",
         "status": "confirmed",
         "blockheight": 1862856,
         "reserved": false
      },
      {
         "txid": "f68de52d80a1076e36c677ef640539c50e3d03f77f9f9db4f13048519489593f",
         "output": 1,
         "value": 89804,
         "amount_msat": "89804000msat",
         "scriptpubkey": "001451c88b44420940c52a384bd8a03888e3676c1509",
         "address": "tb1q28ygk3zzp9qv223cf0v2qwygudnkc9gfp30ud4",
         "status": "confirmed",
         "blockheight": 1863006,
         "reserved": false
      }
   ],
   "channels": [
      {
         "peer_id": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
         "connected": false,
         "state": "ONCHAIN",
         "short_channel_id": "1862856x29x0",
         "channel_sat": 89987,
         "our_amount_msat": "89987000msat",
         "channel_total_sat": 100000,
         "amount_msat": "100000000msat",
         "funding_txid": "66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d",
         "funding_output": 0
      }
   ]
}

```

### Comprendere i Tipi di Chiusura dei Canali

Il comando `close` RPC tenta di chiudere un canale cooperativamente con il suo peer o unilateralmente dopo che l'argomento `unilateraltimeout` è scaduto. Questo merita una discussione aggiuntiva, poiché va al cuore del design senza fiducia di Lightning:

Ogni partecipante di un canale è in grado di creare quante più transazioni Lightning al proprio controparte consentite dai loro fondi. La maggior parte delle volte non ci saranno disaccordi tra i partecipanti, quindi ci saranno solo due transazioni on-chain, una di apertura e l'altra di chiusura del canale. Tuttavia, potrebbero esserci scenari in cui un peer non è online o non è d'accordo con lo stato finale del canale o dove qualcuno cerca di rubare fondi all'altra parte. Questo è il motivo per cui ci sono sia chiusure cooperative che forzate.

#### Chiusura Cooperativa

Nel caso di una chiusura cooperativa, entrambi i partecipanti al canale concordano di chiudere il canale e regolare lo stato finale sulla blockchain. Entrambi i partecipanti devono essere online; la chiusura viene eseguita trasmettendo una spesa incondizionata della transazione di finanziamento con un output per ciascun peer.

#### Chiusura Forzata

Nel caso di una chiusura forzata, solo un partecipante è online o i partecipanti non sono d'accordo sullo stato finale del canale. In questa situazione, un peer può eseguire una chiusura unilaterale del canale senza la cooperazione dell'altro nodo. Viene eseguita trasmettendo una transazione di impegno che si impegna a uno stato precedente del canale concordato da entrambe le parti. Questa transazione di impegno contiene lo stato del canale diviso in due parti: il saldo per ciascun partecipante e tutti i pagamenti pendenti (HTLC).

Per eseguire questo tipo di chiusura, devi specificare un argomento `unilateraltimeout`. Se questo valore non è zero, il comando di chiusura chiuderà unilateralmente il canale quando viene raggiunto quel numero di secondi:

```
c$ lightning-cli --network=testnet close $newidremote 60
{
   "tx": "0200000001a1091f727e6041cc93fead2ea46b8402133f53e6ab89ab106b49638c11f27cba00000000006a40aa8001df85010000000000160014d22818913daf3b4f86e0bcb302a5a812d1ef6b91c6772d20",
   "txid": "02cc4c647eb3e06f37fcbde39871ebae4333b7581954ea86b27b85ced6a5c4f7",
   "type": "unilateral"
}

```
## Sommario: Chiudere un Canale

Quando chiudi un canale esegui una transazione on-chain terminando il tuo rapporto finanziario con il nodo remoto. Per chiudere un canale, devi tenere conto del suo stato e del tipo di chiusura che vuoi eseguire.

## Cosa Succede Dopo?

Continua a "Using Lightning" col [Capitolo 20.4:Espandere la Rete Lightning](20_4_Espandere_la_Rete_Lightning.md).

