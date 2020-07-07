# 15.3 Receiving Bitcoind Notifications with C

> **NOTE:** This is a draft in progress, so that I can get some feedback from early reviewers. It is not yet ready for learning.

[§15.3](15_3_Receiving_Bitcoind_Notifications_with_C.md) In this chapter we will show how to create a simple listener for bitcoin blockchain to receive notifications using ZMQ-based (http://zeromq.org/) notification interface, which is a highly scalable networking library written in C that acts like a concurrency framework.   Bitcoin allows connection points where clients can subscribe to get notified about blockchain events like raw transactions or raw blocks. 

## Install ZMQ

To create a blockchain listener in C you need to install ZMQ following this steps.

  1. Install ZMQ
  2. Create C program.
  3. Compile C program.
  4. Configure bitcoind to allow ZMQ notifications
  5. Execute listener.

### 1. Install ZMQ

```
sudo apt-get install libzmq3-dev
```
Output

```
Reading package lists... Done
Building dependency tree       
Reading state information... Done
The following NEW packages will be installed:
  libzmq3-dev
0 upgraded, 1 newly installed, 0 to remove and 18 not upgraded.
Need to get 400 kB of archives.
After this operation, 2.227 kB of additional disk space will be used.
Get:1 http://es.archive.ubuntu.com/ubuntu bionic-updates/universe amd64 libzmq3-dev amd64 4.2.5-1ubuntu0.2 [400 kB]
Fetched 400 kB in 0s (1.114 kB/s)   
Selecting previously unselected package libzmq3-dev:amd64.
(Reading database ... 313982 files and directories currently installed.)
Preparing to unpack .../libzmq3-dev_4.2.5-1ubuntu0.2_amd64.deb ...
Unpacking libzmq3-dev:amd64 (4.2.5-1ubuntu0.2) ...
Setting up libzmq3-dev:amd64 (4.2.5-1ubuntu0.2) ...
Processing triggers for man-db (2.8.3-2ubuntu0.1) ...
```
Then 

```
sudo apt-get install libczmq-dev
```
Output:

```
Reading package lists... Done
Building dependency tree       
Reading state information... Done
The following additional packages will be installed:
  libczmq4
The following NEW packages will be installed:
  libczmq-dev libczmq4
0 upgraded, 2 newly installed, 0 to remove and 18 not upgraded.
Need to get 496 kB of archives.
After this operation, 2.148 kB of additional disk space will be used.
Do you want to continue? [Y/n] y
Get:1 http://es.archive.ubuntu.com/ubuntu bionic/universe amd64 libczmq4 amd64 4.1.0-2 [145 kB]
Get:2 http://es.archive.ubuntu.com/ubuntu bionic/universe amd64 libczmq-dev amd64 4.1.0-2 [351 kB]
Fetched 496 kB in 0s (1.287 kB/s)    
Selecting previously unselected package libczmq4:amd64.
(Reading database ... 314055 files and directories currently installed.)
Preparing to unpack .../libczmq4_4.1.0-2_amd64.deb ...
Unpacking libczmq4:amd64 (4.1.0-2) ...
Selecting previously unselected package libczmq-dev:amd64.
Preparing to unpack .../libczmq-dev_4.1.0-2_amd64.deb ...
Unpacking libczmq-dev:amd64 (4.1.0-2) ...
Setting up libczmq4:amd64 (4.1.0-2) ...
Setting up libczmq-dev:amd64 (4.1.0-2) ...
Processing triggers for man-db (2.8.3-2ubuntu0.1) ...
Processing triggers for libc-bin (2.27-3ubuntu1) ...
```

### 2. Create C Program

Now we've installed ZMQ and we can compile our C program using it's notifications.  This C program it's a simple client that subscribes to a connection point served by bitcoind and ZMQ interface and reads incoming messages.  
The program use czmq.h library and receives two parameters as follows,  first param is the point exposed by bitcoind that should be tcp connection point depending second param that could be one of these:

zmqpubrawblock
zmqpubrawtx
zmqpubhashtx
zmqpubhashblock


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

  zsock_t *socket = zsock_new_sub(zmqserver, topic);
  assert(socket);

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
  zsock_destroy(&socket);
  return 0;
}
```

### 3. Compile C program

To compile this C program you have to do it with clang or gcc compiler and test zmq and czmq libraries.

```
gcc -o chainlistener chainlistener.c -I/usr/local/include -L/usr/local/lib -lzmq -lczmq
```

### 4. Configure ZMQ on bitcoind

#### ZMQ

ZeroMQ is a high-performance asynchronous messaging library that provides a message queue.  ZeroMQ supports common messaging patterns (pub/sub, request/reply, client/server and others) over a variety of transports (TCP, in-process, inter-process, multicast, WebSocket and more), making inter-process messaging as simple as inter-thread messaging.   As the purpose of this chapter is to show how to receive bitcoind notifications in the C language, ZMQ will be used for it.

Currently, the ZeroMQ facility only needs to have the ZeroMQ endpoint specified.  ZeroMQ publish sockets prepend each data item with an arbitrary topic
prefix that allows subscriber clients to request only those items with a matching prefix.

Topics.

```
zmqpubrawblock=tcp://127.0.0.1:28332
zmqpubrawtx=tcp://127.0.0.1:28333
zmqpubhashtx=tcp://127.0.0.1:28334
zmqpubhashblock=tcp://127.0.0.1:28335
```

In this example we'll use raw tx that is the topic that notifies about new transactions in raw format.

Add this lines to bitcoin.conf file and restart daemon.    

```
zmqpubrawblock=tcp://127.0.0.1:28332
zmqpubrawtx=tcp://127.0.0.1:28333
```
Then test it's working using this command

```
$ bitcoin-cli --testnet getzmqnotifications
```
Output:
```
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
### 5. Execute listener:

When you execute chainlistener and passes these params you'll get an output like this:

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
The first param it's bitcoind connection point where ZMQ will notifies all about transactions in raw format.   It corresponds to this line in bitcoin.conf file zmqpubrawtx=tcp://127.0.0.1:28333

The second param is the topic selected rawtx explained above.

### For More Information

In this repository you can browse find more details about ZMQ notifications and others kind of messages.  (https://github.com/Actinium-project/ChainTools/blob/master/docs/chainlistener.md)

### Summary Receiving Bitcoind Notifications with C.md

By using ZMQ framework, you can easily receive notifications by subscribing to a connection point exposed by bitcoind changing configuration file.







