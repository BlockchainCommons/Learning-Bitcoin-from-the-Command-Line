# 16.3 Receiving Notifications in C with ZMQ  Libraries

> :information_source: **NOTE:** This section has been recently added to the course and is an early draft that may still be awaiting review. Caveat reader.

[§16.1](16_1_Accessing_Bitcoind_with_C.md) and [§16.2](16_2_Programming_Bitcoind_with_C.md) introduced RPC and JSON libraries for C, and in doing so showed one of the advantages of accessing Bitcoin's RPC commands through a programming language: the ability to reasonably create much more complex programs. This chapter introduces a third library, for [ZMQ](http://zeromq.org/), and in doing so reveals another advantage: the ability to monitor for notifications. It will use that for coding a blockchain listener.

> :book: ***What is ZMQ?*** ZeroMQ (ZMQ)is a high-performance asynchronous messaging library that provides a message queue.  ZeroMQ supports common messaging patterns (pub/sub, request/reply, client/server, and others) over a variety of transports (TCP, in-process, inter-process, multicast, WebSocket, and more), making inter-process messaging as simple as inter-thread messaging. You can find more details about ZMQ notifications and others kind of messages in [this repo](https://github.com/Actinium-project/ChainTools/blob/master/docs/chainlistener.md).

## Set Up ZMQ

Before you can create a blockchain listener, you will need to configure `bitcoind` to allow ZMQ notifications, and then you'll need to install a ZMQ library to take advantage of those notifications.

### Configure `bitcoind` for ZMQ

Bitcoin Core is ZMQ-ready, but you must specify ZMQ endpoints. ZeroMQ publish-sockets prepend each data item with an arbitrary topic
prefix that allows subscriber clients to request only those items with a matching prefix. There are currently four topics supported by `bitcoind`:
```
$ bitcoind --help | grep zmq | grep address
  -zmqpubhashblock=<address>
  -zmqpubhashtx=<address>
  -zmqpubrawblock=<address>
  -zmqpubrawtx=<address>
```
You can run `bitcoind` with command-line arguments for ZMQ endpoints, as shown above, but you can also make an endpoint accessible by adding appropriate lines to your `~/.bitcoin/bitcoin.conf` file and restarting your daemon.    

```
zmqpubrawblock=tcp://127.0.0.1:28332
zmqpubrawtx=tcp://127.0.0.1:28333
```
You can then test your endpoints are working using the `getzmqnotifications` RPC:

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
Your `bitcoind` will now issue ZMQ notifications

### Install ZMQ

To take advantage of those notifications, you need a ZMQ library to go with C; we'll thus be using a new ZMQ library instead of the `libbitcoinrpc` library in this section, but when you're experimenting in the future, you'll of course be able to combine them. 

Fortunately, ZMQ libraries are available through standard Debian packages:
```
$ sudo apt-get install libzmq3-dev
$ sudo apt-get install libczmq-dev
```
You're now ready to code!

## Write Your Notification Program

The following C program is a simple client that subscribes to a ZMQ connection point served by `bitcoind` and reads incoming messages.  

The program requires two parameters: the first parameter is the "server", which is the TCP connection point exposed by `bitcoind`; and the second is the "topic", which is currently `zmqpubhashblock`, `zmqpubhashtx`, `zmqpubrawblock`, or `zmqpubrawtx`. The topic must be supported through the `bitcoin.conf` and the server's IP address and port must match what's defined there.

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
You will open a ZMQ socket to the defined server for the defined topic:
``` c
  zsock_t *socket = zsock_new_sub(zmqserver, topic);
  assert(socket);
```
After that, you wait:
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
While, waiting, you watch for messages on the ZMQ socket. Whenever you receive a message, you will `pop` it off the stack and report out its number, its length, and most importantly the data.

That's it!

Of course when you're done, you should clean up:
```
  zsock_destroy(&socket);
  return 0;
}
```

### Test the Notification Code

The source code is in the [src directory](src/16_3_chainlistener.c) as usual. You should compile it:
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

### Summary Receiving Bitcoind Notifications with C.md

By using the ZMQ framework, you can easily receive notifications by subscribing to a connection point exposed by `bitcoind` through its configuration file.

> :fire: ***What is the Power of Notifications?*** With notifications, you're no longer entirely dependent upon users to issue commands. Instead, you can create programs that monitor the Bitcoin blockchain and take appropriate actions when certain things occur. This in turn could be merged with the RPC commands that you programmed in previous sections. This is also a big step beyond what you could do with shell scripts: certainly, you can create infinite-loop listener shell scripts, but programming languages tend to be a better tool for that task.

## What's Next?

Learn more about "Programming with RPC" in [Chapter 17: Programming Bitcoin with Libwally](17_0_Programming_with_Libwally.md).





