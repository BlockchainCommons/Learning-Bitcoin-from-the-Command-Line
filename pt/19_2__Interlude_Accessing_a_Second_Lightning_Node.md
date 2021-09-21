# Prefácio: Acessando um Segundo Node Lightning

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho inicial que ainda pode estar aguardando revisão.

Quando estávamos brincando com o Bitcoin, estávamos acessando uma rede existente, e isso torna tudo relativamente fácil para se trabalhar. Apenas ligávamos o `bitcoind` e estávamos imediatamente interagindo com a rede. Agora, a Lightning funciona da seguinte maneira: é fundamentalmente uma rede ponto a ponto, construída a partir das conexões entre dois nodes individuais. Em outras palavras, para interagir com a Lightning Network, precisaremos primeiro encontrar um node ao qual podemos nos conectar.

Existem quatro maneiras de fazermos isso (das quais as três primeiras são possíveis para a nossa primeira conexão):

## Pedindo Informações Sobre um Node

Se outra pessoa já tiver um node da Lightning Network na rede que escolhemos, podemos pedir o ID dele.

Se estiverem executando a c-lightning, eles só precisam usar o comando `getinfo`:
```
$ lightning-cli getinfo
lightning-cli: WARNING: default network changing in 2020: please set network=testnet in config!
   "id": "03240a4878a9a64aea6c3921a434e573845267b86e89ab19003b0c910a86d17687",
   "alias": "VIOLETGLEE",
   "color": "03240a",
   "num_peers": 0,
   "num_pending_channels": 0,
   "num_active_channels": 0,
   "num_inactive_channels": 0,
   "address": [
      {
         "type": "ipv4",
         "address": "74.207.240.32",
         "port": 9735
      }
   ],
   "binding": [
      {
         "type": "ipv6",
         "address": "::",
         "port": 9735
      },
      {
         "type": "ipv4",
         "address": "0.0.0.0",
         "port": 9735
      }
   ],
   "version": "v0.9.1-96-g6f870df",
   "blockheight": 1862854,
   "network": "testnet",
   "msatoshi_fees_collected": 0,
   "fees_collected_msat": "0msat",
   "lightning-dir": "/home/standup/.lightning/testnet"
}
```
Eles poderão então nos dizer o `ID` deles (`03240a4878a9a64aea6c3921a434e573845267b86e89ab19003b0c910a86d17687`). Eles também precisarão informar o endereço IP (`74.207.240.32`) e porta (`9735`).

## Criando um Novo Node c-lightning

No entanto, para fins de teste, provavelmente iremos desejar ter um segundo node sob nosso próprio controle. A maneira mais fácil de fazer isso é criar um segundo node c-lightning em uma nova máquina, usando Bitcoin Standup, de acordo com a seção [§2.1](02_1_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md) ou compilando-o manualmente, de acordo com a seção [§19.1](19_1_Verifying_Your_Lightning_Setup.md).

Depois de ter nosso node em execução, podemos executar o `getinfo` para recuperar nossas informações, como mostrado acima.

## Criando um Novo Node LND

No entanto, para os exemplos do próximo capítulo, vamos criar um node LND. Isso nos permitirá demonstrar um pouco da profundidade do ecossistema Lightning, mostrando como comandos semelhantes funcionam nas duas plataformas diferentes.

Uma maneira de criar um node LND é executar os scripts Bitcoin Standup novamente em uma nova máquina, mas desta vez escolher a LND, de acordo com a seção [§2.1](2_1_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md).

Outra forma é compilar o LND a partir do código-fonte em uma máquina em que já estejamos executando um node Bitcoin, como falaremos abaixo.

### Compilando o Código-Fonte do LND

Primeiro, precisaremos baixar e instalar o Go:
```
$ wget --progress=bar:force https://dl.google.com/go/"go1.14.4"."linux"-"amd64".tar.gz -O ~standup/"go1.14.4"."linux"-"amd64".tar.gz
$ /bin/tar xzf ~standup/"go1.14.4"."linux"-"amd64".tar.gz -C ~standup
$ sudo mv ~standup/go /usr/local
```
Depois, precisamos nos certificar de que a versão Go é a mais atualizada (atualmente é a `go1.14.4`), e que a plataforma e arquitetura são adequadas para nossa máquina. O item acima funcionará para o Debian.

Vamos atualizar nosso PATH:
```
$ export GOPATH=~standup/gocode
$ export PATH="$PATH":/usr/local/go/bin:"$GOPATH"/bin
```
Em seguida, vamos nos certificar de que o `go` funciona:
```
$ go version
go version go1.14.4 linux/amd64
```
Também precisaremos do `git` e do `make`:
```
$ sudo apt-get install git
$ sudo apt-get install build-essential
```
Agora estamos prontos para recuperar o LND. Certifique-se de obter a versão atual (no momento, é a `v0.11.0-beta.rc4`).
```
$ go get -d github.com/lightningnetwork/lnd
```
E agora podemos compilar:
```
$ cd "$GOPATH"/src/github.com/lightningnetwork/lnd
$ git checkout v0.11.0-beta.rc4
$ make
$ make install
```
Os comandos acima irão instalar o Go na pasta `~/gocode/bin`, que é o `$GOPATH/bin`.

Devemos alterá-la para os diretórios globais:
```
$ sudo cp $GOPATH/bin/lnd $GOPATH/bin/lncli /usr/bin
```

### Criando um Arquivo de Configuração do LND

Ao contrário da c-lightning, precisaremos criar um arquivo de configuração padrão para o LND.

No entanto, primeiro, iremos precisar habilitar o ZMQ em nosso Bitcoind, se ainda não o fizemos na seção [§16.3](16_3_Receiving_Bitcoind_Notifications_with_C.md).

Isso requer adicionar o seguinte ao nosso arquivo `~/.bitcoin/bitcoin.conf`, se ainda não estiver lá:
```
zmqpubrawblock=tcp://127.0.0.1:28332
zmqpubrawtx=tcp://127.0.0.1:28333
```

Se estivermos usando um arquivo de configuração Bitcoin do Standup ou algum outro `conf` especializado, precisamos nos certificar de colocar nossos novos comandos na seção correta. Idealmente, devemos chegar perto do topo do arquivo, caso contrário, na seção `[test]` (assumindo, como de costume, que estamos usando a testnet).

Devemos então reiniciar o Bitcoin (ou apenas reiniciar nossa máquina). Podemos testar se está tudo funcionando da seguinte maneira:
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
Agora estamos prontos para criar um arquivo de configuração.

Primeiro, precisamos recuperar nosso rpcuser e rpcpassword. A seguinte é uma maneira automatizada de fazer isso:
```
$ BITCOINRPC_USER=$(cat ~standup/.bitcoin/bitcoin.conf | grep rpcuser | awk -F = '{print $2}')
$ BITCOINRPC_PASS=$(cat ~standup/.bitcoin/bitcoin.conf | grep rpcpassword | awk -F = '{print $2}')
```

> :warning: **AVISO:** Obviamente, nunca iremos armazenar nossa senha RPC em uma variável shell em um ambiente de produção.

Em seguida, podemos gravar o arquivo:
```
$ mkdir ~/.lnd
$ cat > ~/.lnd/lnd.conf << EOF
[Application Options]
maxlogfiles=3
maxlogfilesize=10
#externalip=1.1.1.1 # change to your public IP address if required.
alias=StandUp
listen=0.0.0.0:9735
debuglevel=debug
[Bitcoin]
bitcoin.active=1
bitcoin.node=bitcoind
bitcoin.testnet=true
[Bitcoind]
bitcoind.rpchost=localhost
bitcoind.rpcuser=$BITCOINRPC_USER
bitcoind.rpcpass=$BITCOINRPC_PASS
bitcoind.zmqpubrawblock=tcp://127.0.0.1:28332
bitcoind.zmqpubrawtx=tcp://127.0.0.1:28333
EOF
```

### Criando um Serviço LND

Finalmente, podemos criar um serviço LND para executar automaticamente o `lnd`:
```
$ cat > ~/lnd.service << EOF
# It is not recommended to modify this file in-place, because it will
# be overwritten during package upgrades. If you want to add further
# options or overwrite existing ones then use
# $ systemctl edit lnd.service
# See "man systemd.service" for details.
# Note that almost all daemon options could be specified in
# /etc/lnd/lnd.conf, except for those explicitly specified as arguments
# in ExecStart=
[Unit]
Description=LND Lightning Network Daemon
Requires=bitcoind.service
After=bitcoind.service
[Service]
ExecStart=/usr/bin/lnd
ExecStop=/usr/bin/lncli --lnddir /var/lib/lnd stop
PIDFile=/run/lnd/lnd.pid
User=standup
Type=simple
KillMode=process
TimeoutStartSec=60
TimeoutStopSec=60
Restart=always
RestartSec=60
[Install]
WantedBy=multi-user.target
EOF
```
Em seguida, precisaremos instalar aquilo e iniciar as coisas:
```
$ sudo cp ~/lnd.service /etc/systemd/system
$ sudo systemctl enable lnd
$ sudo systemctl start lnd
```
(É esperado que a primeira vez leve um certo tempo.)

### Habilitando Conexões Remotas

Assim como na c-lightning, precisaremos tornar o LND acessível a outros nodes. Veja como fazer isso se usarmos o `ufw`, de acordo com as configurações do Bitcoin Standup:
```
$ sudo ufw allow 9735
```

### Criando uma Carteira

Na primeira vez que executamos o LND, devemos criar uma carteira:
```
$ lncli --network=testnet create
```
O LND pedirá uma senha e, em seguida, nos perguntará se desejaremos inserir um mnemônico existente (basta pressionar `n` para o último).

Agora devemos ter um `lnd` funcionando, que pode ser verificado com o comando `getinfo`:
```
$ lncli --network=testnet getinfo
{
    "version": "0.11.0-beta.rc4 commit=v0.11.0-beta.rc4",
    "commit_hash": "fc12656a1a62e5d69430bba6e4feb8cfbaf21542",
    "identity_pubkey": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
    "alias": "StandUp",
    "color": "#3399ff",
    "num_pending_channels": 0,
    "num_active_channels": 0,
    "num_inactive_channels": 0,
    "num_peers": 2,
    "block_height": 1862848,
    "block_hash": "000000000000000ecb6fd95e1f486283d48683aa3111b6c23144a2056f5a1532",
    "best_header_timestamp": "1602632294",
    "synced_to_chain": true,
    "synced_to_graph": false,
    "testnet": true,
    "chains": [
        {
            "chain": "bitcoin",
            "network": "testnet"
        }
    ],
    "uris": [
    ],
    "features": {
        "0": {
            "name": "data-loss-protect",
            "is_required": true,
            "is_known": true
        },
        "5": {
            "name": "upfront-shutdown-script",
            "is_required": false,
            "is_known": true
        },
        "7": {
            "name": "gossip-queries",
            "is_required": false,
            "is_known": true
        },
        "9": {
            "name": "tlv-onion",
            "is_required": false,
            "is_known": true
        },
        "13": {
            "name": "static-remote-key",
            "is_required": false,
            "is_known": true
        },
        "15": {
            "name": "payment-addr",
            "is_required": false,
            "is_known": true
        },
        "17": {
            "name": "multi-path-payments",
            "is_required": false,
            "is_known": true
        }
    }
}
```
O ID deste node é `032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543`. Embora este comando não nos mostre o endereço IP e a porta, eles devem ser o endereço IP da nossa máquina e a porta `9735`.

## Escute a Fofoca

Se já estivéssemos conectados à Lightning Network, e nosso node já estivesse "fofocando" com seus pares, também poderíamos ser capazes de encontrar informações sobre os pares automaticamente, por meio do comando `listpeers`:
```       
c$ lightning-cli --network=testnet listpeers
{
   "peers": [
      {
         "id": "0302d48972ba7eef8b40696102ad114090fd4c146e381f18c7932a2a1d73566f84",
         "connected": true,
         "netaddr": [
            "127.0.0.1:9736"
         ],
         "features": "02a2a1",
         "channels": []
      }
   ]
}
```   
No entanto, este definitivamente não será o caso na nossa primeira interação com a Lightning Network.

## Resumo: Prefácio: Acessando um Segundo Node Lightning

Sempre precisaremos de dois nodes Lightning para formar um canal. Se não tivermos outra pessoa que está testando as coisas conosco, precisaremos criar um segundo node, usanda c-lightning ou (como faremos em nossos exemplos) usando a LND.

## O Que Vem Depois?

Embora possivelmente tenhamos criado um LND, a c-lightning permanecerá no centro dos nossos exemplos até que precisemos começar a usar os dois, no [Capítulo 20](20_0_Using_Lightning.md).

Vamos continuar "Compreendendo Nossa Configuração da Lightning" na seção [§19.3: Criando um Canal Lightning](19_3_Setting_Up_a_Channel.md).
