# 16.3 Ricevere Notifiche in C con Librerie ZMQ

> :information_source: **NOTA:** Questa sezione è stata recentemente aggiunta al corso ed è una bozza iniziale che potrebbe essere ancora in attesa di revisione. Lettore avvisato.

Nel [Capitolo 16.1](16_1_Accedere_a_Bitcoind_con_Librerie_RPC.md) e nel [Capitolo 16.2](16_2_Programare_Bitcoind_in_C_con_Librerie_RPC.md) hanno introdotto le librerie RPC e JSON per C, e in tal modo hanno mostrato uno dei vantaggi di accedere ai comandi RPC di Bitcoin attraverso un linguaggio di programmazione: la possibilità di creare ragionevolmente programmi molto più complessi. Questo capitolo introduce una terza libreria, per [ZMQ](http://zeromq.org/), e in tal modo rivela un altro vantaggio: la possibilità di monitorare le notifiche. Lo utilizzeremo per codificare un ascoltatore di blockchain.

> :book: ***Che cos'è ZMQ?*** ZeroMQ (ZMQ) è una libreria di messaggistica asincrona ad alte prestazioni che fornisce una coda di messaggi. ZeroMQ supporta modelli di messaggistica comuni (pub/sub, request/reply, client/server e altri) su una varietà di trasporti (TCP, in-process, inter-process, multicast, WebSocket e altro), rendendo la messaggistica inter-processo semplice come la messaggistica inter-thread. Puoi trovare ulteriori dettagli sulle notifiche ZMQ e altri tipi di messaggi in [questo repository](https://github.com/Actinium-project/ChainTools/blob/master/docs/chainlistener.md).

## Configura ZMQ

Prima di poter creare un ascoltatore di blockchain, dovrai configurare `bitcoind` per consentire le notifiche ZMQ e poi dovrai installare una libreria ZMQ per sfruttare queste notifiche.

### Configurare `bitcoind` per ZMQ

Bitcoin Core è pronto per ZMQ, ma devi specificare gli endpoint ZMQ. I socket di pubblicazione di ZeroMQ premettono a ogni elemento di dati un prefisso di argomento arbitrario che consente ai client abbonati di richiedere solo quegli elementi con un prefisso corrispondente. Attualmente ci sono quattro argomenti supportati da `bitcoind`:
:
```
$ bitcoind --help | grep zmq | grep address
  -zmqpubhashblock=<address>
  -zmqpubhashtx=<address>
  -zmqpubrawblock=<address>
  -zmqpubrawtx=<address>
```
Puoi eseguire `bitcoind` con argomenti da riga di comando per gli endpoint ZMQ, come mostrato sopra, ma puoi anche rendere accessibile un endpoint aggiungendo le righe appropriate al tuo file `~/.bitcoin/bitcoin.conf` e riavviando il tuo demone.    


```
zmqpubrawblock=tcp://127.0.0.1:28332
zmqpubrawtx=tcp://127.0.0.1:28333
```
Puoi quindi testare se i tuoi endpoint stanno funzionando utilizzando l'RPC `getzmqnotifications`:


```
$ bitcoin-cli getzmqnotifications
[
  {
    "type": "pubrawblock",
    "address": "tcp://127.0.0.1:28332",
    "hwm": 1000
  },
  {
    "type": "pubrawtx",
    "address": "tcp://127.0.0.1:28333",
    "hwm": 1000
  }
]
```
Il tuo `bitcoind` ora emetterà notifiche ZMQ.

### Installare ZMQ

Per sfruttare queste notifiche, hai bisogno di una libreria ZMQ da utilizzare con C; in questa sezione useremo quindi una nuova libreria ZMQ invece della libreria `libbitcoinrpc`, ma quando sperimenterai in futuro, potrai ovviamente combinarle. 

Fortunatamente, le librerie ZMQ sono disponibili tramite pacchetti standard Debian:

```
$ sudo apt-get install libzmq3-dev
$ sudo apt-get install libczmq-dev
```
Ora sei pronto per programmare!

## Scrivi il Tuo Programma di Notifica

Il seguente programma C è un semplice client che si abbona a un punto di connessione ZMQ servito da `bitcoind` e legge i messaggi in arrivo.  

Il programma richiede due parametri: il primo parametro è il "server", che è il punto di connessione TCP esposto da `bitcoind`; e il secondo è l'argomento, che attualmente è `zmqpubhashblock`, `zmqpubhashtx`, `zmqpubrawblock` o `zmqpubrawtx`. L'argomento deve essere supportato tramite il `bitcoin.conf` e l'indirizzo IP e la porta del server devono corrispondere a quanto definito lì.


``` c
#include <czmq.h>
int main(int argc, char ** argv) {

  char *zmqserver;
  char *topic;

  if (argc < 3) {
    printf("\nUSAGE:\nchainlistener <tcp://localhost:port> <topic>\n\n");
    return 0;
  } else {
    zmqserver = argv[1];
    topic = argv[2];
  }
```

Aprirai un socket ZMQ al server definito per l'argomento definito:
``` c
  zsock_t *socket = zsock_new_sub(zmqserver, topic);
  assert(socket);
```
Dopodiché, aspetti:
```
  while(1) {
    zmsg_t *msg;
    int rc = zsock_recv(socket, "m", &msg);
    assert(rc == 0);

    char *header = zmsg_popstr(msg);
    zframe_t *zdata = zmsg_pop(msg);
    unsigned int *no = (unsigned int*)zmsg_popstr(msg);

    char *data = zframe_strhex(zdata);
    int len = zframe_size(zdata);
    printf("Size: %d\n", len);
    printf("Data: %s", data);
    printf("\nNo: %d\n", *no);

    free(header);
    free(data);
    free(no);
    free(zdata);
    zmsg_destroy(&msg);
    sleep(1);
 }
```
Durante l'attesa, guardi i messaggi sul socket ZMQ. Ogni volta che ricevi un messaggio, lo estrai dallo stack e riporti il suo numero, la sua lunghezza e, soprattutto, i dati.

Questo è tutto!

Ovviamente, quando hai finito, dovresti pulire:
```
  zsock_destroy(&socket);
  return 0;
}
```
### Testare il Codice di Notifica

Il codice sorgente è nella [directory src](src/16_3_chainlistener.c) come al solito. Dovresti compilarlo:

```
$ cc -o chainlistener chainlistener.c -I/usr/local/include -L/usr/local/lib -lzmq -lczmq
```
Afterward, you can run it with the topics and addresses that you defined in your `bitcoin.conf`:
```
$ ./chainlistener tcp://127.0.0.1:28333 rawtx
Size: 250
Data: 02000000000101F5BD2032E5A9E6650D4E411AD272E391F26AFC3C9102B7C0C7444F8F74AE86010000000017160014AE9D51ADEEE8F46ED2017F41CD631D210F2ED9C5FEFFFFFF0203A732000000000017A9147231060F1CDF34B522E9DB650F44EDC6C0714E4C8710270000000000001976A914262437B129CF8592AB2EDC59C07D19C57729F72888AC02483045022100AE316D5F21657E3525271DE39EB285D8A0E89A20AB6413824E88CE47DCD0EFE702202F61E10C2A8F4A7125D5EB63AEF883D8E3584A0ECED0D349283AABB6CA5E066D0121035A77FE575A9005E3D3FF0682E189E753E82FA8BFF0A20F8C45F06DC6EBE3421079111B00
No: 67
Size: 249
Data: 0200000000010165C986992F7DAD22BBCE3FCF0BF546EDBC3C599618B04CFA22D9E64EF0CE4C030000000017160014B58E0A5CD68B249F1C407E9AAE9CD0332AAA3067FEFFFFFF02637932000000000017A914CCC47261489036CB6B9AA610857793FF5752E5378710270000000000001976A914262437B129CF8592AB2EDC59C07D19C57729F72888AC0247304402206CCC3F3B4BE01D4E532A01C2DC6BC3B53E4FFB6B494C8B87DD603EFC648A159902201653841E8B16A814DC375129189BB7CF01CFF7D269E91178645B6A97F5C7F4F10121030E20F3D2F172281B8DC747F007DF24B352248AC09E48CA64016942A8F01D317079111B00
No: 68
Size: 250
Data: 02000000000101E889CFC1FFE127BA49F6C1011388606A194109AE1EDAAB9BEE215E123C14A7920000000017160014577B0B3C2BF91B33B5BD70AE9E8BD8144F4B87E7FEFFFFFF02C34B32000000000017A914A9F1440402B46235822639C4FD2F78A31E8D269E8710270000000000001976A914262437B129CF8592AB2EDC59C07D19C57729F72888AC02483045022100B46318F53E1DCE63E7109DB4FA54AF40AADFC2FEB0E08263756BC3B7A6A744CB02200851982AF87DBABDC3DFC3362016ECE96AECFF50E24D9DCF264AE8966A5646FE0121039C90FCB46AEA1530E5667F8FF15CB36169D2AD81247472F236E3A3022F39917079111B00
No: 69
Size: 250
Data: 0200000000010137527957C9AD6CFF0C9A74597E6EFCD7E1EBD53E942AB2FA34A831046CA11488000000001716001429BFF05B3CD79E9CCEFDB5AE82139F72EB3E9DB0FEFFFFFF0210270000000000001976A914262437B129CF8592AB2EDC59C07D19C57729F72888AC231E32000000000017A9146C8D5FE29BFDDABCED0D6F4D8E82DCBFD9D34A8B8702483045022100F259846BAE29EB2C7A4AD711A3BC6109DE69AE91E35B14CA2742157894DD9760022021464E09C00ABA486AEAA0C49FEE12D2850DC03F57F04A1A9E2CC4D0F4F1459C012102899F24A9D60132F4DD1A5BA6DCD1E4E4B6C728927BA482C2C4E511679F60CA5779111B00
No: 70
.......
```
Dopodiché, puoi eseguirlo con gli argomenti e gli indirizzi che hai definito nel tuo `bitcoin.conf`:

--------------------------

### Riepilogo: Ricevere Notifiche di Bitcoind con C

Utilizzando il framework ZMQ, puoi facilmente ricevere notifiche collegandoti a un punto di connessione esposto da `bitcoind` attraverso il suo file di configurazione.

> :fire: ***Qual è il Potere delle Notifiche?*** Con le notifiche, non dipendi più completamente dagli utenti per emettere comandi. Invece, puoi creare programmi che monitorano la blockchain di Bitcoin e prendono le azioni appropriate quando si verificano determinate cose. Questo potrebbe a sua volta essere integrato con i comandi RPC che hai programmato nelle sezioni precedenti. Questo è anche un grande passo avanti rispetto a ciò che potresti fare con gli script di shell: certamente, puoi creare script di shell ascoltatori con loop infiniti, ma i linguaggi di programmazione tendono a essere uno strumento migliore per quel compito.

## Cosa Succede Dopo?

Scopri di più su "Programmazione con RPC" in [Capitolo 17: Programmare Bitcoin con Libwally](17_0_Programmare_Bitcoin_con_Libwally.md).




