# 19.1: Verificando Nossa Configuração da c-lightning

>: information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho inicial que ainda pode estar aguardando revisão.

Nesta seção, instalaremos e verificaremos a c-lightning, nosso utilitário para acessar a Lightning Network.

> :book: ***O que é a Lightning Network?*** A Rede Lightning é uma rede descentralizada que usa a funcionalidade de contrato inteligente da blockchain do Bitcoin para permitir pagamentos instantâneos em uma rede de participantes. A Lightning é construída como um protocolo de segunda camada que interage com o Bitcoin para permitir que os usuários troquem seus bitcoins "fora da blockchain" (ou o jargão em inglês, "off-chain").

> :book: ***O que é um protocolo de segunda camada?*** A segunda camada refere-se a um protocolo secundário criado em cima do sistema de blockchain do Bitcoin. O objetivo principal desses protocolos é resolver a velocidade de transação e as dificuldades de escala que estão presentes no Bitcoin. O Bitcoin não é capaz de processar milhares de transações por segundo (TPS), então protocolos de segunda camada foram criados para resolver o problema de escalabilidade da blockchain. Essas soluções também são conhecidas como soluções de dimensionamento "off-chain".

## Instalando a c-lightning

Se já usamos os [Bitcoin Standup Scripts](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts), talvez já o tenhamos instalado no início deste curso. Podemos testar isto verificando se o `lightningd` está em execução:
```
$ ps auxww | grep -i lightning
standup  31213  0.0  0.2  24144 10424 pts/0    S    15:38   0:00 lightningd --testnet
standup  31214  0.0  0.1  22716  7444 pts/0    S    15:38   0:00 /usr/local/bin/../libexec/c-lightning/plugins/autoclean
standup  31215  0.0  0.2  22992  8248 pts/0    S    15:38   0:00 /usr/local/bin/../libexec/c-lightning/plugins/bcli
standup  31216  0.0  0.1  22756  7604 pts/0    S    15:38   0:00 /usr/local/bin/../libexec/c-lightning/plugins/keysend
standup  31217  0.0  0.1  22776  7648 pts/0    S    15:38   0:00 /usr/local/bin/../libexec/c-lightning/plugins/pay
standup  31218  0.0  0.1  22720  7652 pts/0    S    15:38   0:00 /usr/local/bin/../libexec/c-lightning/plugins/txprepare
standup  31219  0.0  0.1  22744  7716 pts/0    S    15:38   0:00 /usr/local/bin/../libexec/c-lightning/plugins/spenderp
standup  31227  0.0  0.1  22748  7384 pts/0    SL   15:38   0:00 /usr/local/libexec/c-lightning/lightning_hsmd
standup  31228  0.0  0.2  23044  8192 pts/0    S    15:38   0:00 /usr/local/libexec/c-lightning/lightning_connectd
standup  31229  0.0  0.1  22860  7556 pts/0    S    15:38   0:00 /usr/local/libexec/c-lightning/lightning_gossipd
standup  32072  0.0  0.0   6208   888 pts/0    S+   15:50   0:00 grep -i lightning
```
Caso contrário, precisaremos instalá-lo agora. Infelizmente, se estivermos usando o Debian, precisaremos instalá-lo manualmente, compilando o código-fonte, mas ainda assim deve ser muito simples se seguirmos estas instruções. Se acontecer de estarmos em um sistema Ubuntu padrão, podemos tentar [Instalar a partir do Ubuntu ppa](#variant-install-from-ubuntu-ppa), e sempre podemos tentar [Instalar os binários pré-compilados](#variant-install-pre-compiled-binaries).

> :book: ***O que é a c-lightning?*** Existem três implementações diferentes da Lightning no momento: C-lightning, LND e Eclair. Todos devem ser funcionalmente compatíveis, com base nas mesmas [RFCs do BOLT](https://github.com/lightningnetwork/lightning-rfc/blob/master/00-introduction.md), mas os detalhes de implementação podem ser diferentes. Escolhemos a c-lightning como base do curso porque ela também faz parte do [projeto Elements](https://github.com/ElementsProject), que contém a Libwally.

### Compilando o Código-Fonte da c-lightning

A instalação da Lightning a partir do código-fonte deve ser bem simples se seguirmos estas instruções.

_Provavelmente_ desejaremos fazer isso em um node não prunado, pois trabalhar com nodes prunados na Lightning pode causar problemas de instalação e uso. Se, no início deste curso, configuramos nosso node para ser prunado, podemos querer substituí-lo por um full node agora. Se estivermos usando a testnet, provavelmente conseguiremos usar o mesmo tipo de máquina que usamos para o node prunado.

> :warning: **AVISO:** Realmente podemos executar a c-lightning em um node prunado. No entanto, conforme observamos no [repositório Lightning](https://github.com/ElementsProject/lightning#pruning), pode haver uma série de problemas. Para fazer isso funcionar, devemos garantir que o node da Lightning sempre tente atualizar informações sobre os blocos que o node do Bitcoin não excluiu. Para fazermos isso, devemos nos certificar de que (1) nosso node de Bitcoin está totalmente atualizado antes de iniciar nosso node da Lightning pela primeira vez e; (2) nosso node Lightning nunca fique defasado do node do Bitcoin (para um node prunado em 550 blocos padrão, ele nunca pode ser desligado por 4 dias ou mais). Portanto, podemos usar o node assim, mas apresenta algum perigo, o que não é uma boa ideia se estivermos executando um serviço em produção.

Dito isso, estamos prontos para instalar a Lightning:

Primeiro, vamos instalar as dependências, incluindo requisitos de desenvolvimento.
```
$ sudo apt-get install -y \
   autoconf automake build-essential git libtool libgmp-dev \
   libsqlite3-dev python3 python3-mako net-tools zlib1g-dev libsodium-dev \
   gettext
$ sudo apt-get install -y valgrind python3-pip libpq-dev
```
Isso pode demorar um pouco, porque há várias dependências e algumas são bem grandes.

Em segundo lugar, vamos clonar o repositório Lightning:
```
$ cd ~
$ git clone https://github.com/ElementsProject/lightning.git
$ cd lightning
```
Agora podemos usar o `pip3` que instalamos para instalar requisitos adicionais para a compilação e posteriomente configurar tudo:
```
$ pip3 install -r requirements.txt
$ ./configure
```
Agora, vamos compilar. Isso pode levar algum tempo também, dependendo do processamento da nossa máquina.
```
$ make
```
Depois disso, o que precisamos fazer é instalar:
```
$ sudo make install
```

## Verificando Nossa Instalação

Podemos confirmar que o lightningd foi instalado corretamente usando o parâmetro `help`:

```
$ lightningd --help
lightningd: WARNING: default network changing in 2020: please set network=testnet in config!
Usage: lightningd 
A bitcoin lightning daemon (default values shown for network: testnet).
--conf=<file>                        Specify configuration file
--lightning-dir=<dir>                Set base directory: network-specific
                                     subdirectory is under here
                                      (default: "/home/javier/.lightning")
--network <arg>                      Select the network parameters (bitcoin,
                                     testnet, regtest, litecoin or
                                     litecoin-testnet) (default: testnet)
--testnet                            Alias for --network=testnet
--signet                             Alias for --network=signet
--mainnet                            Alias for --network=bitcoin
```

## Executando lightningd

Começaremos a explorar a Lightning Network com o comando `lightning-cli`. No entanto, `lightningd` _deve_ estar rodando para podermos usar o `lightning-cli`, já que `lightning-cli` envia comandos JSON-RPC para o `lightningd` (tudo exatamente como o `bitcoin-cli` e o `bitcoind`).

Se instalamos a `c-lightning` manualmente, precisaremos iniciá-la:
```
$ nohup lightningd --testnet &
```

### Executando o lightningd como um serviço

Se preferirmos, podemos instalar o `lightningd` como um serviço que será executado toda vez que reiniciarmos nossa máquina. Os comandos seguintes farão isso e ele começará a funcionar imediatamente:

```
$ cat > ~/lightningd.service << EOF
# It is not recommended to modify this file in-place, because it will
# be overwritten during package upgrades. If you want to add further
# options or overwrite existing ones then use
# $ systemctl edit bitcoind.service
# See "man systemd.service" for details.
# Note that almost all daemon options could be specified in
# /etc/lightning/config, except for those explicitly specified as arguments
# in ExecStart=
[Unit]
Description=c-lightning daemon
[Service]
ExecStart=/usr/local/bin/lightningd --testnet
# Process management
####################
Type=simple
PIDFile=/run/lightning/lightningd.pid
Restart=on-failure
# Directory creation and permissions
####################################
# Run as standup
User=standup
# /run/lightningd
RuntimeDirectory=lightningd
RuntimeDirectoryMode=0710
# Hardening measures
####################
# Provide a private /tmp and /var/tmp.
PrivateTmp=true
# Mount /usr, /boot/ and /etc read-only for the process.
ProtectSystem=full
# Disallow the process and all of its children to gain
# new privileges through execve().
NoNewPrivileges=true
# Use a new /dev namespace only populated with API pseudo devices
# such as /dev/null, /dev/zero and /dev/random.
PrivateDevices=true
# Deny the creation of writable and executable memory mappings.
MemoryDenyWriteExecute=true
[Install]
WantedBy=multi-user.target
EOF
$ sudo cp ~/lightningd.service /etc/systemd/system
$ sudo systemctl enable lightningd.service
$ sudo systemctl start lightningd.service
```

### Habilitando Conexões Remotas

Se tivermos algum tipo de firewall, precisaremos abrir a porta 9735 para permitir que outros nodes da Lightning interajam conosco.

Se usarmos o `ufw` do Bitcoin Standup, podemos fazer da seguinte maneira:
```
$ sudo ufw allow 9735
```

## Verificando o Nosso Node

Podemos verificar se o nosso node Lightning está pronto para funcionar comparando a saída de `bitcoin-cli getblockcount` com o resultado de `blockheight` do `lightning-cli getinfo`.

```
$ bitcoin-cli -testnet getblockcount
1838587
$ lightning-cli --testnet getinfo
{
   "id": "03d4592f1244cd6b5a8bb7fba6a55f8a91591d79d3ea29bf8e3c3a405d15db7bf9",
   "alias": "HOPPINGNET",
   "color": "03d459",
   "num_peers": 0,
   "num_pending_channels": 0,
   "num_active_channels": 0,
   "num_inactive_channels": 0,
   "address": [
      {
         "type": "ipv4",
         "address": "74.207.240.32",
         "port": 9735
      },
      {
         "type": "ipv6",
         "address": "2600:3c01::f03c:92ff:fe48:9ddd",
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
   "blockheight": 1838587,
   "network": "testnet",
   "msatoshi_fees_collected": 0,
   "fees_collected_msat": "0msat",
   "lightning-dir": "/home/standup/.lightning/testnet"
}
```
Neste caso, o `blockheight` é mostrado como `1838587` por ambos os comandos.

Em vez disso, podemos obter um erro, dependendo da situação.

Se o node Bitcoin ainda estiver sincronizando com a rede, devemos ver uma mensagem como esta:
```
"warning_bitcoind_sync": "Bitcoind is not up-to-date with network."
```
Se o nosso node Lightning não estiver atualizado, receberemos uma mensagem como esta:
```
"warning_lightningd_sync": "Still loading latest blocks from bitcoind."
```
Se tentarmos executar em uma blockchain prunada cujo node Bitcoin não estava atualizado quando iniciamos o node Lightning, receberemos mensagens de erro em nosso log parecidas com esta:
```
bitcoin-cli -testnet getblock 0000000000000559febee77ab6e0be1b8d0bef0f971c7a4bee9785393ecef451 0 exited with status 1
```

## Criando Aliases

Sugerimos a criação de alguns aliases (apelidos de comandos) para facilitar o uso da c-lightning.

Podemos fazer isso colocando-os em nosso arquivo `.bash_profile`.
```
cat >> ~/.bash_profile <<EOF
alias lndir="cd ~/.lightning/" #linux default c-lightning path
alias lnc="lightning-cli"
alias lnd="lightningd"
alias lninfo='lightning-cli getinfo'
EOF
```
Depois de inserir esses aliases, podemos executar o comando `source ~/.bash_profile` para inseri-los, ou apenas efetuar logout e login novamente.

Podemos observar que esses aliases incluem atalhos para executar o `lightning-cli`, para executar o `lightningd` e para ir para o diretório c-lightning. Esses aliases têm como objetivo principal tornar nossa vida mais fácil. Sugerimos criar outros apelidos para facilitar o uso de comandos frequentes (e seus argumentos) e para minimizar erros. Os aliases desse tipo podem ser ainda mais úteis se tivermos uma configuração complexa onde regularmente executamos comandos associados a Mainnet, com Testnet _e_ com a Regtest, conforme explicado mais adiante.

Dito isso, o uso desses aliases _neste_ livro pode acidentalmente obscurecer as lições principais que estão sendo ensinadas sobre a c-lightning, portanto, continuaremos a mostrar os comandos completos. Podemos ajustá-los para nosso próprio uso conforme apropriado.

## Opcional: Modificando Nossos Tipos de Servidor

> :link: **TESTNET vs MAINNET:** Ao configurar nosso node, escolhemos criá-lo como um node Mainnet, Testnet ou Regtest. Embora este documento presuma uma configuração no Testenet, vale a pena entender como podemos acessar e usar os outros tipos de configuração, mesmo todos estando na mesma máquina! Mas, se você for um usuário iniciante, pode pular esta parte, pois não é necessária para uma configuração básica.

Quando o lightningd é inicializado, geralmente ele lê um arquivo de configuração cuja localização depende da rede que estamos usando (o padrão é `~/.lightning/testnet/config`). Isso pode ser alterado com os sinalizadores `–conf` e `–lightning-dir`.

```
~/.lightning/testnet$ ls -la config
-rw-rw-r-- 1 user user 267 jul 12 17:08 config
```
Também existe um arquivo de configuração geral (o padrão é `~/.lightning/config`). Se desejarmos executar vários tipos diferentes de nodes simultaneamente, devemos deixar o sinalizador testnet (ou regtest) fora deste arquivo de configuração. Devemos então escolher se estamos usando a mainnet, a testnet ou a regtest toda vez que executarmos o `lightningd` ou o `lightning-cli`.

Nossa configuração pode não ter nenhum arquivo de configuração: a c-lightning será executada com uma boa configuração padrão, sem eles.

## Resumo: Verificando Nossa Configuração da c-lightning

Antes de começar a brincar com a lightning, devemos nos certificar de que nossos aliases estão configurados, nosso `lightningd` está rodando e nosso node está sincronizado. Também podemos querer configurar algum acesso a configurações alternativas da Lightning, em outras redes.

## O Que Vem Depois?

Vamos continuar "Compreendendo a Configuração da Lightning" na seção [§19.2: Conhecendo Nossa Configuração da c-lightning](19_2_Knowing_Your_lightning_Setup.md).

## Variante: Instalando do Ubuntu ppa

Se estivermos usando uma versão do Ubuntu diferente do Debian, podemos instalar a c-lightning usando [Ubuntu ppa](https://launchpad.net/~lightningnetwork/+archive/ubuntu/ppa):

```
$ sudo apt-get install -y software-properties-common
$ sudo add-apt-repository -u ppa:lightningnetwork/ppa
$ sudo apt-get install lightningd
```

## Variante: Instalando Binários Pré-Compilados

Outro método para instalar a Lightning é usar os binários pré-compilados no [repositório Github](https://github.com/ElementsProject/lightning/releases). Vamos escolher o arquivo mais recente, como `clightning-v0.9.1-Ubuntu-20.04.tar.xz`.

Depois de baixá-lo, precisaremos ir para o diretório raiz e descompactá-lo:
```
$ cd /
$ sudo tar xf ~/clightning-v0.9.1-Ubuntu-20.04.tar.xz 
```
Aviso: Isso exigirá que tenhamos exatamente as mesmas bibliotecas que foram usadas para criar o binário. Geralmente é mais fácil apenas recompilar.