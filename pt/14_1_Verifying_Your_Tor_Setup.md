# 14.1: Verificando Nossa Configuração do Tor

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho inicial que ainda pode estar aguardando revisão.

Se fizéssemos uma instalação padrão com o [Bitcoin Standup](https://github.com/BlockchainCommons/Bitcoin-Standup), provavelmente já teríamos o Tor configurado como parte do nosso node Bitcoin. O Tor está instalado e já criou os serviços ocultos para as portas RPC do Bitcoin, enquanto um endereço Onion também foi criado para o ```bitcoind```. Nesta seção iremos discutir tudo isso e o que fazer com essas informações e ferramentas.

> :book: ***O que é o Tor?*** O Tor é uma rede de sobreposição e anonimato de baixa latência baseada no design de roteamento e construção de caminhos para permitir a comunicação anônima. É um software gratuito e open source com o nome derivado da sigla do nome do projeto de software original: "The Onion Router".

> :book: ***Por que usar o Tor com o Bitcoin?*** A rede Bitcoin é uma rede peer-to-peer que escuta as transações e as propaga usando um endereço IP público. Ao conectar-se à rede sem o Tor, iríamos compartilhar nosso endereço IP, o que poderia expor nossa localização, o tempo de atividade e outros detalhes a terceiros, o que é uma prática de privacidade indesejável. Para nos protegermos virtualmente, devemos usar ferramentas como o Tor para ocultar os detalhes da nossa conexão. O Tor permite melhorar nossa privacidade online, pois os dados são codificados criptograficamente e passam por nodes diferentes, cada um decodificando uma única camada (por isso a metáfora da cebola).

## Entendendo o Tor

Então, como o Tor funciona?

Quando um usuário deseja se conectar a um servidor na Internet, o Tor tenta construir um caminho formado por pelo menos três retransmissores de nodes do Tor, chamados de Guarda, Meio e Saída. Durante a construção desse caminho, as chaves de criptografia simétricas são negociadas. Quando uma mensagem se move ao longo do caminho, cada retransmissão remove sua camada de criptografia. Dessa forma, a mensagem chega ao destino final na forma original, e cada parte conhece apenas o salto anterior e o próximo e não pode determinar a origem ou o destino.

Esta é a aparência de uma conexão sem o Tor:
```
20:58:03.804787 IP bitcoin.36300 > lb-140-82-114-25-iad.github.com.443: Flags [P.], seq 1:30, ack 25, win 501, options [nop,nop,TS val 3087919981 ecr 802303366], length 29
```
Agora, com o Tor, muito menos informações sobre as máquinas reais são transmitidas:
```
21:06:52.744602 IP bitcoin.58776 > 195-xxx-xxx-x.rev.pxxxxxm.eu.9999: Flags [P.], seq 264139:265189, ack 3519373, win 3410, options [nop,nop,TS val 209009853 ecr 3018177498], length 1050
21:06:52.776968 IP 195-xxx-xxx-x.rev.pxxxxxm.eu.9999 > bitcoin.58776: Flags [.], ack 265189, win 501, options [nop,nop,TS val 3018177533 ecr 209009853], length 0
```
Resumindo: o Tor criptografa nossos dados de forma a ocultar nossa origem, destino e quais serviços estamos usando, enquanto um protocolo de criptografia padrão como TLS *apenas* protege o conteúdo dos nossos dados.

### Compreendendo a Arquitetura da Rede Tor

A arquitetura básica da rede Tor é composta pelos seguintes componentes:

* **Cliente Tor (OP ou Onion Proxy).** Um cliente Tor instala o software local que atua como um proxy onion. Ele empacota os dados do aplicativo em células do mesmo tamanho (512 bytes), que então envia para a rede Tor. Uma célula é a unidade básica de transmissão do Tor.
* **Node Onion (OR ou Onion Router).** Um Node Onion transmite células provenientes do cliente Tor e de servidores online. Existem três tipos de Nodes Onion: nodes de entrada (Guarda), nodes intermediários (Meio) e nodes de saída (Saída).
* **Servidor de Diretório.** Um servidor de diretório armazena informações sobre roteadores e servidores onion (serviços ocultos), como as suas chaves públicas.
* **Onion Server (Servidor Oculto).** Um servidor onion oferece suporte a aplicativos TCP, como páginas da web ou IRC, como serviços.

### Compreendendo as Limitações do Tor

O Tor não é uma ferramenta perfeita. Como as informações da rede Tor são descriptografadas nos nodes de saída antes de serem enviadas aos destinos finais, teoricamente um observador poderia coletar metadados suficientes para comprometer o anonimato e potencialmente identificar os usuários.

Também há estudos que sugerem que possíveis explorações da proteção anti-DoS do Bitcoin podem permitir que um invasor force outros usuários que usam o Tor a se conectar exclusivamente por meio dos nodes de saída do Tor ou dos seus pares Bitcoin, isolando o cliente do resto da rede Bitcoin e expondo-o a censura, correlação e demais ataques.

Da mesma forma, os usuários do Bitcoin Tor podem ser atacados por impressão digital (fingerprinting) definindo um cookie de endereço nos nodes. Isso também permitiria a correlação e, portanto, acabando com o anonimato.

Enquanto isso, mesmo no Tor, o Bitcoin é apenas um serviço pseudo-anônimo devido aos muitos perigos de correlação que se originam do próprio livro-razão permanente. Isso significa que o uso do Bitcoin sobre o Tor tem mais probabilidade de _perder o anonimato_ do que outros serviços (e pode levar ao anonimato de outras atividades).

Com isso dito, o Tor é geralmente considerado mais seguro do que a alternativa, que é a navegação não anônima.

## Verificando a Nossa Configuração do Tor

Então, como verificamos se o Tor está ativado? Se o instalamos com o Bitcoin Standup, podemos verificar se o Tor está sendo executado em nosso sistema:
```
$ sudo -u debian-tor tor --verify-config
```

Se o Tor estiver instalado corretamente, devemos obter a seguinte resposta:
```
Jun 26 21:52:09.230 [notice] Tor 0.4.3.5 running on Linux with Libevent 2.0.21-stable, OpenSSL 1.0.2n, Zlib 1.2.11, Liblzma 5.2.2, and Libzstd N/A.
Jun 26 21:52:09.230 [notice] Tor can't help you if you use it wrong! Learn how to be safe at https://www.torproject.org/download/download#warning
Jun 26 21:52:09.230 [notice] Read configuration file "/etc/tor/torrc".
Configuration was valid
```
> :warning: **AVISO:** Isso significa apenas que o Tor está sendo executado, não que está sendo usado para todas (ou algumas) conexões.

### Verificando a Nossa Configuração do Tor para o RPC

O propósito mais importante do Tor, conforme instalado pelo Bitcoin Standup, é oferecer serviços ocultos para as portas RPC que são usadas para enviar comandos para o ```bitcoind```.

> :book: ***O que é um serviço oculto do Tor?*** Um serviço oculto (também conhecido como "um onion service") é um serviço que pode ser acessado via Tor. A conexão feita a esse serviço _usando a Onion Network_ será anônima.

O arquivo de configuração do Tor pode ser encontrado em ```/etc/tor/torrc```. Se olharmos para ele, veremos os seguintes serviços que protegem nossas portas do RPC:
```
HiddenServiceDir /var/lib/tor/standup/
HiddenServiceVersion 3
HiddenServicePort 1309 127.0.0.1:18332
HiddenServicePort 1309 127.0.0.1:18443
HiddenServicePort 1309 127.0.0.1:8332
```
> :link: **TESTNET vs MAINNET:** Na Mainnet o RPC é executado na porta 8332, já na testnet, na porta 18332.

> :information_source: **NOTA:** O ```HiddenServiceDir``` é onde todos os arquivos são mantidos para este serviço em particular. Se precisarmos pesquisar nosso endereço onion, chaves de acesso ou adicionar clientes autorizados, é aqui que iremos alterar!

A maneira fácil de testar o nosso Serviço Oculto RPC é usar o [QuickConnect API](https://github.com/BlockchainCommons/Bitcoin-Standup/blob/master/Docs/Quick-Connect-API.md) integrado ao Bitcoin Standup. Basta baixar o código QR que encontramos em ```/qrcode.png``` e digitalizá-lo usando uma carteira ou node que suporte o QuickConnect, como [a Gordian Wallet](https://github.com/BlockchainCommons/FullyNoded-2). Ao escanear o QR, devemos ver a carteira sincronizada com o nosso node. Estaremos fazendo isso usando os serviços ocultos do RPC.

A maneira difícil de testar nosso Serviço Oculto RPC é enviar um comando ```bitcoin-cli``` com o ```torify```, que permite traduzir um comando UNIX normal para um comando protegido por Tor. É algo difícil porque precisamos obter três informações.

1. **Nossa porta de serviço oculta.** Isso pode ser obtido em ```/etc/tor/torrc/```. Por padrão, é a porta 1309.
2. **Nosso endereço Tor.** Essa informação encontra-se no arquivo ```hostname``` no diretório ```HiddenServiceDir``` definido em ```/etc/tor/torrc```. Por padrão, o arquivo pode ser encontrado em ```/var/lib/tor/standup/hostname```. Ele está protegido, então precisaremos usar o ```sudo``` para acessá-lo:
```
$ sudo more /var/lib/tor/standup/hostname
mgcym6je63k44b3i5uachhsndayzx7xi4ldmwrm7in7yvc766rykz6yd.onion
```
3. **Nossa senha do RPC.** Que está no arquivo ```~/.bitcoin/bitcoin.conf```

Quando tivermos todas essas informações, podemos emitir um comando ```bitcoin-cli``` usando o ```torify``` e especificando o ```-rpcconnect``` como nosso endereço onion, o ```-rpcport``` como nossa porta de serviço oculta, e o ```-rpcpassword``` como sendo nossa senha:
```
$ torify bitcoin-cli -rpcconnect=mgcym6je63k44b3i5uachhsndayzx7xi4ldmwrm7in7yvc766rykz6yd.onion -rpcport=1309 -rpcuser=StandUp -rpcpassword=685316cc239c24ba71fd0969fa55634f getblockcount
```

### Verificando Nossa Configuração do Tor para o Bitcoind

O Bitcoin Standup também garante que nosso ```bitcoind``` esteja configurado para se comunicar opcionalmente em um endereço onion.

Podemos verificar a configuração inicial do Tor para o ```bitcoind``` executando grep para "tor" no ```debug.log``` em nosso diretório de dados:
```
$ grep "tor:" ~/.bitcoin/testnet3/debug.log
2021-06-09T14:07:04Z tor: ADD_ONION successful
2021-06-09T14:07:04Z tor: Got service ID vazr3k6bgnfafmdpcmbegoe5ju5kqyz4tk7hhntgaqscam2qupdtk2yd, advertising service vazr3k6bgnfafmdpcmbegoe5ju5kqyz4tk7hhntgaqscam2qupdtk2yd.onion:18333
2021-06-09T14:07:04Z tor: Cached service private key to /home/standup/.bitcoin/testnet3/onion_v3_private_key
```
> :information_source: **NOTA:** O Bitcoin Core não suporta mais endereços v2. O suporte ao Tor v2 foi removido no Pull Request [#22050](https://github.com/bitcoin/bitcoin/pull/22050)

> **TESTNET vs MAINNET:** Na Mainnet o ```bitcoind``` responde usando a porta 8333, na testnet, na porta 18333.

Podemos verificar se um serviço oculto Tor foi criado para o Bitcoin usando o comando RPC ```getnetworkinfo```:

```
$ bitcoin-cli getnetworkinfo
...
 "localaddresses": [
    {
      "address": "173.255.245.83",
      "port": 18333,
      "score": 1
    },
    {
      "address": "2600:3c01::f03c:92ff:fe86:f26",
      "port": 18333,
      "score": 1
    },
    {
      "address": "vazr3k6bgnfafmdpcmbegoe5ju5kqyz4tk7hhntgaqscam2qupdtk2yd.onion",
      "port": 18333,
      "score": 4
    }
  ],
...
```
Isso mostra três endereços para acessarmos nosso servidor Bitcoin, um endereço IPv4 (```173.255.245.83```), um endereço IPv6 (```2600:3c01::f03c:92ff:fe86:f26```), e um endereço Tor (```vazr3k6bgnfafmdpcmbegoe5ju5kqyz4tk7htgtgaq2qudsc```).

> :warning: **AVISO:** Obviamente, nunca podemos revelar seu endereço Tor de uma forma que esteja associada ao nosso nome ou a outras PII ("personably identifiable information", ou informações de identificação pessoal)!

Podemos ver algumas informações semelhantes com o ```getnetworkinfo```.
```
 bitcoin-cli getnetworkinfo
{
  "version": 200000,
  "subversion": "/Satoshi:0.20.0/",
  "protocolversion": 70015,
  "localservices": "0000000000000408",
  "localservicesnames": [
    "WITNESS",
    "NETWORK_LIMITED"
  ],
  "localrelay": true,
  "timeoffset": 0,
  "networkactive": true,
  "connections": 10,
  "networks": [
    {
      "name": "ipv4",
      "limited": false,
      "reachable": true,
      "proxy": "",
      "proxy_randomize_credentials": false
    },
    {
      "name": "ipv6",
      "limited": false,
      "reachable": true,
      "proxy": "",
      "proxy_randomize_credentials": false
    },
    {
      "name": "onion",
      "limited": false,
      "reachable": true,
      "proxy": "127.0.0.1:9050",
      "proxy_randomize_credentials": true
    }
  ],
  "relayfee": 0.00001000,
  "incrementalfee": 0.00001000,
  "localaddresses": [
    {
      "address": "173.255.245.83",
      "port": 18333,
      "score": 1
    },
    {
      "address": "2600:3c01::f03c:92ff:fe86:f26",
      "port": 18333,
      "score": 1
    },
    {
      "address": "vazr3k6bgnfafmdpcmbegoe5ju5kqyz4tk7hhntgaqscam2qupdtk2yd.onion",
      "port": 18333,
      "score": 4
    }
  ],
  "warnings": ""
}
```
Este serviço oculto permitirá conexões anônimas com o nosso ```bitcoind``` através da rede Bitcoin.

> :warning: **AVISO:** Executar o Tor e ter um serviço oculto do Tor não nos obriga a usar o Tor.

### Verificando Nossa Configuração do Tor para os Peers

Usando o comando RPC ```getpeerinfo```, podemos ver quais nodes estão conectados ao nosso node e verificar se eles estão conectados ao Tor.

```
$ bitcoin-cli getpeerinfo
```
Alguns podem estar conectados via Tor:
```
...
{
    "id": 9,
    "addr": "nkv.......xxx.onion:8333",
    "addrbind": "127.0.0.1:51716",
    "services": "000000000000040d",
    "servicesnames": [
      "NETWORK",
      "BLOOM",
      "WITNESS",
      "NETWORK_LIMITED"
    ],
    "relaytxes": true,
    "lastsend": 1593981053,
    "lastrecv": 1593981057,
    "bytessent": 1748,
    "bytesrecv": 41376,
    "conntime": 1593980917,
    "timeoffset": -38,
    "pingwait": 81.649295,
    "version": 70015,
    "subver": "/Satoshi:0.20.0/",
    "inbound": false,
    "addnode": false,
    "startingheight": 637875,
    "banscore": 0,
    "synced_headers": -1,
    "synced_blocks": -1,
    "inflight": [
    ],
    "whitelisted": false,
    "permissions": [
    ],
    "minfeefilter": 0.00000000,
    "bytessent_per_msg": {
      "addr": 55,
      "feefilter": 32,
      "getaddr": 24,
      "getheaders": 1053,
      "inv": 280,
      "ping": 32,
      "pong": 32,
      "sendcmpct": 66,
      "sendheaders": 24,
      "verack": 24,
      "version": 126
    },
    "bytesrecv_per_msg": {
      "addr": 30082,
      "feefilter": 32,
      "getdata": 280,
      "getheaders": 1053,
      "headers": 106,
      "inv": 9519,
      "ping": 32,
      "pong": 32,
      "sendcmpct": 66,
      "sendheaders": 24,
      "verack": 24,
      "version": 126
    }
  }
...
```
Alguns podem não estar, como esta conexão IPv6:
```
...
  {
    "id": 17,
    "addr": "[2001:638:a000:4140::ffff:191]:18333",
    "addrlocal": "[2600:3c01::f03c:92ff:fe86:f26]:36344",
    "addrbind": "[2600:3c01::f03c:92ff:fe86:f26]:36344",
    "services": "0000000000000409",
    "servicesnames": [
      "NETWORK",
      "WITNESS",
      "NETWORK_LIMITED"
    ],
    "relaytxes": true,
    "lastsend": 1595447081,
    "lastrecv": 1595447067,
    "bytessent": 12250453,
    "bytesrecv": 2298711417,
    "conntime": 1594836414,
    "timeoffset": -1,
    "pingtime": 0.165518,
    "minping": 0.156638,
    "version": 70015,
    "subver": "/Satoshi:0.20.0/",
    "inbound": false,
    "addnode": false,
    "startingheight": 1780784,
    "banscore": 0,
    "synced_headers": 1781391,
    "synced_blocks": 1781391,
    "inflight": [
    ],
    "whitelisted": false,
    "permissions": [
    ],
    "minfeefilter": 0.00001000,
    "bytessent_per_msg": {
      "addr": 4760,
      "feefilter": 32,
      "getaddr": 24,
      "getdata": 8151183,
      "getheaders": 1085,
      "headers": 62858,
      "inv": 3559475,
      "ping": 162816,
      "pong": 162816,
      "sendcmpct": 132,
      "sendheaders": 24,
      "tx": 145098,
      "verack": 24,
      "version": 126
    },
    "bytesrecv_per_msg": {
      "addr": 33877,
      "block": 2291124374,
      "feefilter": 32,
      "getdata": 9430,
      "getheaders": 1085,
      "headers": 60950,
      "inv": 2019175,
      "ping": 162816,
      "pong": 162816,
      "sendcmpct": 66,
      "sendheaders": 24,
      "tx": 5136622,
      "verack": 24,
      "version": 126
    }
  }
...
```
Ter um endereço Tor para nosso ```bitcoind``` é provavelmente um pouco menos útil do que ter um endereço Tor para nossas conexões RPC. Isso é verdade porque não é recomendado tentar enviar todas as nossas conexões Bitcoin via Tor, e em parte porque proteger nossos comandos RPC é o que realmente importa. É muito mais provável que façamos isso remotamente, a partir de uma carteira de software como a Gordian Wallet, enquanto o nosso próprio servidor tem mais probabilidade de estar no escritório, no porão ou em um bunker.

No entanto, existem maneiras de fazer o ```bitcoind``` usar mais o Tor, conforme iremos discutir na próxima seção.

## Resumo: Verificando Nossa Configuração do Tor

O Tor é um pacote de software instalado como parte do Bitcoin Standup que permite a troca de comunicações anonimamente. Ele protegerá nossas portas RPC (8332 ou 18332) e nossas portas do ```bitcoind``` (8333 ou 18333), porém, teremos que nos conectar ativamente ao endereço onion para utilizá-las! O Tor é um pilar fundamental da privacidade e segurança para a nossa configuração do Bitcoin, e podemos verificar se ele está disponível e vinculado ao Bitcoin com alguns comandos simples.

> :fire: ***Qual é o poder do Tor?*** Muitos ataques a usuários de Bitcoin dependem da informação de quem é a vítima e que ela possui Bitcoins. O Tor pode nos proteger disso, escondendo onde estamos e o que estamos fazendo. É particularmente importante se desejamos nos conectar ao nosso próprio node remotamente por meio de uma carteira de software, e pode ser crucial se fizermos isso em algum país onde o uso de Bitcoin é mal visto. Se precisarmos acessar nossos serviços Bitcoin remotamente, precisamos nos certificar de que nossa carteira seja totalmente compatível com o Tor e envie todos os comandos RPC ao nosso servidor usando esse protocolo.

## O Que Vem Depois?

Vamos continuar "Usando o Tor" na seção [§14.2: Mudando Nossos Serviços Ocultos do Bitcoin](14_2_Changing_Your_Bitcoin_Hidden_Services.md).