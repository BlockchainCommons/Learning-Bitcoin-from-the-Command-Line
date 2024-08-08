# 19.3: Creating a Lightning Channel

> :information_source: **NOTE:** Questa sezione è stata recentemente aggiunta al corso ed è una bozza iniziale che potrebbe essere ancora in attesa di revisione. Attenzione lettore.

Ora comprendi le basi della tua configurazione Lightning e, si spera, hai creato o ricevuto informazioni su un secondo nodo Lightning. Sei pronto per creare il tuo primo canale della rete Lightning. Ovviamente, dovrai capire cos'è e come viene creato utilizzando c-lightning.

> :book: ***Cos'è un Canale Lightning?*** In poche parole, un canale lightning è un tubo di denaro che consente trasferimenti di denaro veloci, economici e privati senza inviare transazioni alla blockchain. Più tecnicamente, un canale è una transazione Bitcoin multisignatura 2-of-2 on-chain che stabilisce una relazione finanziaria senza fiducia tra due persone o due agenti. Una certa quantità di denaro viene depositata nel canale, che poi mantiene un database locale con il saldo bitcoin per entrambe le parti, tenendo traccia di quanto denaro ciascuno ha dall'importo iniziale. I due utenti possono quindi scambiarsi bitcoin attraverso il loro canale Lightning senza mai scrivere sulla blockchain di Bitcoin. Solo quando vogliono chiudere il loro canale regolano i loro bitcoin sulla blockchain, in base alla divisione finale delle monete.

> :book: ***Come Creano i Canali Lightning una Rete Lightning?*** Anche se un canale Lightning consente il pagamento solo tra due utenti, i canali possono essere collegati insieme per formare una rete che consente pagamenti tra membri che non hanno un canale diretto tra loro. Questo crea una rete tra più persone costruita da connessioni a coppie.

In questa sezione, continueremo a utilizzare la nostra configurazione c-lightning come nodo principale.

## Creare un Canale

Creare un canale Lightning richiede i seguenti passaggi:

* Finanziare il tuo wallet c-lightning con alcuni satoshi.
* Connettersi a un nodo remoto come peer.
* Aprire un canale.

### Finanziare il Tuo Wallet c-lightning

Per spostare fondi su un canale Lightning è necessario prima finanziare il tuo wallet c-lightning.

> :book: ***Cos'è un wallet c-lightning?*** L'implementazione standard di c-lightning viene fornita con un wallet Bitcoin integrato che ti consente di inviare e ricevere transazioni bitcoin on-chain. Questo wallet verrà utilizzato per creare nuovi canali.

La prima cosa che devi fare è inviare alcuni satoshi al tuo wallet c-lightning. Puoi creare un nuovo indirizzo usando il comando `lightning-cli newaddr`. Questo genera un nuovo indirizzo che può successivamente essere utilizzato per finanziare i canali gestiti dal nodo c-lightning. Puoi specificare il tipo di indirizzo desiderato; se non specificato, l'indirizzo generato sarà un bech32.



```
$ lightning-cli --testnet newaddr
{
   "address": "tb1qefule33u7ukfuzkmxpz02kwejl8j8dt5jpgtu6",
   "bech32": "tb1qefule33u7ukfuzkmxpz02kwejl8j8dt5jpgtu6"
}
```

Puoi quindi inviare fondi a questo indirizzo utilizzando `bitcoin-cli sendtoaddress` (o qualsiasi altra metodologia preferita). Per questo esempio, lo abbiamo fatto nella transazione [11094bb9ac29ce5af9f1e5a0e4aac2066ae132f25b72bff90fcddf64bf2feb02](https://blockstream.info/testnet/tx/11094bb9ac29ce5af9f1e5a0e4aac2066ae132f25b72bff90fcddf64bf2feb02).

Questa transazione è chiamata [transazione di finanziamento](https://github.com/lightningnetwork/lightning-rfc/blob/master/03-transactions.md#funding-transaction-output), e deve essere confermata prima che i fondi possano essere utilizzati.  

> :book: ***Cos'è una Transazione di Finanziamento?*** Una transazione di finanziamento è una transazione Bitcoin che deposita denaro in un canale Lightning. Può essere finanziata singolarmente (da un partecipante) o congiuntamente (da entrambi). Da lì in poi, le transazioni Lightning riguardano tutte il riassegnare la proprietà della transazione di finanziamento, ma si regolano sulla blockchain solo quando il canale viene chiuso.

Per controllare il tuo saldo locale dovresti usare il comando `lightning-cli listfunds`:



```       
c$ lightning-cli --testnet listfunds
{
   "outputs": [],
   "channels": []
}
```       


Poiché i fondi non hanno ancora sei conferme, non c'è alcun saldo disponibile. Dopo sei conferme dovresti vedere un saldo:

```       
c$ lightning-cli --testnet listfunds
{
   "outputs": [
      {
         "txid": "11094bb9ac29ce5af9f1e5a0e4aac2066ae132f25b72bff90fcddf64bf2feb02",
         "output": 0,
         "value": 300000,
         "amount_msat": "300000000msat",
         "scriptpubkey": "0014ca79fcc63cf72c9e0adb3044f559d997cf23b574",
         "address": "tb1qefule33u7ukfuzkmxpz02kwejl8j8dt5jpgtu6",
         "status": "confirmed",
         "blockheight": 1780680,
         "reserved": false
      }
   ],
   "channels": []
}

```      
Nota che il valore è elencato in satoshi o microsatoshi, non Bitcoin! 

> :book: ***Cosa sono i satoshi e gli msat?*** Hai già incontrato i satoshi molto tempo fa nel [Capitolo 3.4](03_4_Ricevere_una_Transazione.md). Un satoshi è un centomilionesimo di bitcoin, quindi 300.000 satoshi = 0,003 BTC. Un satoshi è la più piccola unità di valuta sulla rete Bitcoin. Ma, la rete Lightning può andare oltre, quindi 1.000 msat, o millisatoshi, equivalgono a un satoshi. Ciò significa che 1 msat è un centomiliardesimo di bitcoin, e 300.000.000 msat = 0,003 BTC.

Ora che hai finanziato il tuo wallet c-lightning avrai bisogno di informazioni su un nodo remoto per iniziare il processo di creazione del canale. 

### Connettersi a un Nodo Remoto

La prossima cosa che devi fare è connettere il tuo nodo a un peer. Questo viene fatto con il comando `lightning-cli connect`. Ricorda che se vuoi maggiori informazioni su questo comando, dovresti digitare `lightning-cli help connect`.

Per connettere il tuo nodo a un peer remoto hai bisogno del suo id, che rappresenta la chiave pubblica del nodo di destinazione. Per comodità, `id` può essere della forma `id@host` o `id@host:port`. Potresti averlo recuperato con `lightning-cli getinfo` (su c-lightning) o `lncli --network=testnet getinfo` (su LND) come discusso nel [precedente interludio](19_2_Intermezzo_Accedere_ad_un_Secondo_Nodo_Lightning.md). 

Abbiamo selezionato il nodo LND, `032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543`, che si trova all'indirizzo IP `45.33.35.151`, a cui ci connetteremo dal nostro nodo c-lightning:



```       
$ lightning-cli --network=testnet connect 032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543@45.33.35.151
{
   "id": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
   "features": "02a2a1"
}
```     


### Aprire un Canale

Il comando RPC fundchannel apre un canale di pagamento con un peer impegnando una transazione di finanziamento sulla blockchain. Dovresti usare il comando `lightning-cli fundchannel` per farlo, con i seguenti parametri:

* **id** è l'id del peer restituito dalla connessione.
* **amount** è l'importo in satoshi preso dal wallet interno per finanziare il canale. Il valore non può essere inferiore al limite di polvere, attualmente impostato a 546, né superiore a 16.777.215 satoshi (a meno che i canali grandi non siano stati negoziati con il peer).
* **feerate** è una tariffa facoltativa utilizzata per la transazione di apertura e come tariffa iniziale per le transazioni di impegno e HTLC. 
* **announce** è un flag facoltativo che determina se annunciare o meno questo canale. Il valore predefinito è true. Se desideri creare un canale privato non annunciato, impostalo su false.
* **minconf** specifica il numero minimo di conferme che devono avere gli output utilizzati nel processo di apertura del canale. Il valore predefinito è 1.
* **utxos** specifica gli utxos da utilizzare per finanziare il canale, come array di “txid:vout”.

Ora puoi aprire il canale in questo modo:



```
$ lightning-cli --testnet fundchannel 032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543 100000 urgent true 1
{
   "tx": "0200000000010193dc3337837f091718f47b71f2eae8b745ec307231471f6a6aab953c3ea0e3b50100000000fdffffff02a0860100000000002200202e30365fe321a435e5f66962492163302f118c13e215ea8928de88cc46666c1d07860100000000001600142fe02e5be9283e8c5bcb93ae61421baf8cb64f9c024730440220668a7c253c9fd83fc1b45e4a52823fb6bc5fad30da36240d4604f0d6981a6f4502202aeb1da5fbbc8790791ef72b3378005fe98d485d22ffeb35e54a6fbc73178fb2012103b3efe051712e9fa6d90008186e96320491cfe1ef1922d74af5bc6d3307843327c76c1c00",
   "txid": "66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d",
   "channel_id": "1d3cf2126ae36e12be3aee893b385ed6a2e19b1da7f4e579e3ef15ca234d6966",
   "outnum": 0
}
```
Per confermare lo stato del canale usa il comando `lightning-cli listfunds`:


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
         "status": "unconfirmed",
         "reserved": false
      },
      {
         "txid": "b5e3a03e3c95ab6a6a1f47317230ec45b7e8eaf2717bf41817097f833733dc93",
         "output": 1,
         "value": 200000,
         "amount_msat": "200000000msat",
         "scriptpubkey": "0014ed54b65eae3da99b23a48bf8827c9acd78079469",
         "address": "tb1qa42tvh4w8k5ekgay30ugyly6e4uq09rfpqf9md",
         "status": "confirmed",
         "blockheight": 1862831,
         "reserved": true
      }
   ],
   "channels": [
      {
         "peer_id": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
         "connected": true,
         "state": "CHANNELD_AWAITING_LOCKIN",
         "channel_sat": 100000,
         "our_amount_msat": "100000000msat",
         "channel_total_sat": 100000,
         "amount_msat": "100000000msat",
         "funding_txid": "66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d",
         "funding_output": 0
      }
   ]
}

```
Mentre questo nuovo canale con 100.000 satoshi è non confermato, il suo stato sarà `CHANNELD_AWAITING_LOCKIN`. Nota che anche il cambio non confermato di `99847` satoshi appare come una nuova transazione nel wallet. Dopo che tutte le sei conferme sono completate, il canale cambierà allo stato `CHANNELD_NORMAL`, che sarà il suo stato permanente. A questo punto, apparirà anche un `short_channel_id`, come:

```
         "short_channel_id": "1862856x29x0",
```

Questi valori indicano dove si trova la transazione di finanziamento sulla blockchain. Si presenta nella forma `block x txid x vout`.

In questo caso, `1862856x29x0` significa:

* Creato nel blocco 1862856;
* con un `txid` di 29; e
* un `vout` di 0.

Potresti aver bisogno di utilizzare questo `short_channel_id` per determinati comandi in Lightning.

Questa transazione di finanziamento può anche essere trovata onchain su [66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d](https://blockstream.info/testnet/tx/66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d)

> :book: ***Cos'è la Capacità del Canale?*** In un canale Lightning, entrambe le parti del canale possiedono una parte della sua capacità. L'importo sul tuo lato del canale è chiamato *saldo locale* e l'importo sul lato del tuo peer è chiamato *saldo remoto*. Entrambi i saldi possono essere aggiornati molte volte senza chiudere il canale (quando il saldo finale viene inviato alla blockchain), ma la capacità del canale non può cambiare senza chiuderlo o modificarlo. La capacità totale di un canale è la somma del saldo detenuto da ciascun partecipante nel canale.

## Sommario: Creazione di un Canale

Devi creare un canale con un nodo remoto per poter ricevere e inviare denaro sulla rete Lightning.   

## Cosa Succede Dopo?

Sei pronto per partire! Continua col [Capitolo 20: Usare Lightning](20_0_Usare_Lightning.md).

