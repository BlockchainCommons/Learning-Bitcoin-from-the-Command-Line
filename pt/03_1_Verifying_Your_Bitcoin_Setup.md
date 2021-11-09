# 3.1: Verificando Nossa Configuração do Bitcoin

Antes de começarmos a brincar com Bitcoin, devemos nos certificar de que tudo está configurado corretamente.

## Crie os Aliases

Sugerimos a criação de alguns aliases (um tipo de atalho) para facilitar o uso do Bitcoin.

Podemos fazer isso colocando-os em nosso `.bash_profile`, `.bashrc` ou no `.profile`.
```
cat >> ~/.bash_profile <<EOF
alias btcdir="cd ~/.bitcoin/" #linux default bitcoind path
alias bc="bitcoin-cli"
alias bd="bitcoind"
alias btcinfo='bitcoin-cli getwalletinfo | egrep "\"balance\""; bitcoin-cli getnetworkinfo | egrep "\"version\"|connections"; bitcoin-cli getmininginfo | egrep "\"blocks\"|errors"'
EOF
```
Depois de inserir os aliases, podemos usar o comando `source .bash_profile` para inseri-los ou apenas efetuar logout/login novamente.

Observe que os aliases incluem atalhos para executar o `bitcoin-cli`, o `bitcoind` e para ir ao diretório Bitcoin. Esses aliases têm como objetivo principal tornar a nossa vida mais fácil, por isso que damos a sugestão de criar outros apelidos para facilitar o uso de comandos frequentes (e argumentos) e também para minimizar erros. Os aliases desse tipo podem ser ainda mais úteis se tivermos uma configuração complexa onde regularmente executamos comandos associados a Mainnet, com Testnet, _e_ com Regtest, conforme explicaremos mais à frente.

Dito isso, o uso desses aliases _neste_ documento pode acidentalmente deixar algumas das lições principais onde ensinamos sobre o Bitcoin no escuro, então o único alias que usaremos diretamente aqui é `btcinfo` porque ele encapsula um comando muito mais longo e complexo. Caso contrário, iremos mostrar os comandos completos, por isso, podemos ajustar os aliases para nosso próprio uso conforme apropriado.

## Executando o Bitcoind

Começaremos nossa exploração da rede Bitcoin com o comando `bitcoin-cli`. No entanto, o `bitcoind` _deve_ estar em execução para conseguirmos usar o bitcoin-cli, pois ele envia comandos JSON-RPC para o bitcoind. Se estivermos usando a configuração padrão, o bitcoind já deve estar instalado e funcionando. Podemos verificar olhando a tabela de processos.
```
$ ps auxww | grep bitcoind
standup    455  1.3 34.4 3387536 1392904 ?     SLsl Jun16  59:30 /usr/local/bin/bitcoind -conf=/home/standup/.bitcoin/bitcoin.conf
```

Se ele não estiver listado, podemos executar o `/usr/local/bin/bitcoind -daemon` manualmente e também colocá-lo em crontab.

## Verificando os Blocos

Vamos partir do pressuposto que já tenhamos baixado todo o blockchain antes de começarmos a brincar. Basta executar o comando `bitcoin-cli getblockcount` para ver tudo está carregado corretamente.
```
$ bitcoin-cli getblockcount
1772384
```
Isso indica que está carregado. Precisamos verificar isso em um serviço online que informa a altura atual do bloco.

> :book: ***O que é a altura do bloco?*** A altura do bloco é a distância que um bloco particular está do bloco de gênese. A altura do bloco atual é a altura do bloco mais recente adicionado a blockchain.

Podemos fazer isso olhando um explorador, como o [Mempool Space Explorer](https://mempool.space/pt/testnet). É o número mais recente corresponde ao `getblockcount`? Se sim, nossa blockchain está atualizada.

Se quisermos que um alias veja tudo de uma vez, o código abaixo funciona normalmente no Testnet, mas pode desaparecer em algum momento no futuro:
```
$ cat >> ~/.bash_profile << EOF
alias btcblock="echo \\\`bitcoin-cli getblockcount 2>&1\\\`/\\\`wget -O - https://blockstream.info/testnet/api/blocks/tip/height 2> /dev/null | cut -d : -f2 | rev | cut -c 1- | rev\\\`"
EOF
$ source .bash_profile 
$ btcblock
1804372/1804372
```

> :link: **TESTNET vs MAINNET:** Lembre-se de que este tutorial geralmente assume que estamos usando a Testnet. Se estivermos usando a mainnet, podemos recuperar a altura do bloco atual usando a seguinte linha de comando: `wget -O - https://mempool.space/testnet/api/blocks/tip/height 2> /dev/ null`. Podemos substituir a última metade do alias `btblock` (após `/`) por isso.

Se a nossa blockchain não estiver atualizada, mas nosso `getblockcount` estiver aumentando, não há problema. O tempo total de download pode levar de uma a várias horas, dependendo da configuração.

## Opcional: Conhecendo os Tipos de Servidores

> **TESTNET vs MAINNET:** Ao configurar o node Bitcoin, precisamos escolher se vamos criá-lo como sendo um Mainnet, Testnet ou Regtest. Embora este documento presuma uma configuração de Testnet, vale a pena entender como podemos acessar e usar os outros tipos de configuração, inclusive, como ter todos na mesma máquina! Mas, se formos iniciantes, podemos pular isso, pois não é necessário para uma configuração básica.

O tipo de configuração é controlado principalmente por meio do arquivo `~/.bitcoin/bitcoin.conf`. Se  estivermos executando o Testnet, provavelmente teremos a seguinte linha:
```
testnet=1
```
Se estivermos executando a Regtest, provavelmente teremos essa linha:
```
regtest=1
```
No entanto, se desejarmos executar vários tipos diferentes de nós simultaneamente, devemos deixar o um sinalizador Testnet (ou Regtest) fora do nosso arquivo de configuração. Podemos então escolher se estamos usando a Mainnet, Testnet ou Regtest toda vez que executarmos o bitcoind ou o bitcoin-cli.

Aqui está um conjunto de aliases que tornariam isso mais fácil, criando um alias específico para iniciar e parar o bitcoind, para ir para o diretório bitcoin e para executar o bitcoin-cli, na Mainnet (que não tem sinalizadores extras), no Testnet (que é -testnet), ou no nosso Regtest (que é -regtest).
```
cat >> ~/.bash_profile <<EOF
alias bcstart="bitcoind -daemon"
alias btstart="bitcoind -testnet -daemon"
alias brstart="bitcoind -regtest -daemon"

alias bcstop="bitcoin-cli stop"
alias btstop="bitcoin-cli -testnet stop"
alias brstop="bitcoin-cli -regtest stop"

alias bcdir="cd ~/.bitcoin/" #linux default bitcoin path
alias btdir="cd ~/.bitcoin/testnet" #linux default bitcoin testnet path
alias brdir="cd ~/.bitcoin/regtest" #linux default bitcoin regtest path

alias bc="bitcoin-cli"
alias bt="bitcoin-cli -testnet"
alias br="bitcoin-cli -regtest"
EOF
```
Para ainda mais complexidade, podemos fazer com que cada um dos aliases 'start' use o sinalizador -conf para carregar a configuração de um arquivo diferente. Isso vai muito além do escopo deste tutorial, mas o oferecemos como um ponto de partida para quando nossas explorações de Bitcoin atingirem o próximo nível.

## Resumo: Verificando Nossa Configuração do Bitcoin

Antes de começar a brincar com o Bitcoin, devemos nos certificar de que nossos aliases estão configurados, nosso bitcoind está rodando e os blocos foram baixados. Também podemos querer configurar algum acesso a configurações alternativas de Bitcoin, se formos usuários mais avançados.

## O Que Vem Depois?

Continue "Compreendendo Nossa Configuração do Bitcoin" com [3.2: Conhecendo Nossa Configuração do Bitcoin](03_2_Knowing_Your_Bitcoin_Setup.md).