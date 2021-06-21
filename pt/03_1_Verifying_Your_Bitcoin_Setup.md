# 3.1: Verificando a configuração do seu node Bitcoin

Antes de começar a brincar com Bitcoin, você deve se certificar de que tudo está configurado corretamente.

## Crie seus aliases

Sugerimos a criação de alguns aliases para facilitar o uso do Bitcoin

Você pode fazer isso colocando-os em seu `.bash_profile`,` .bashrc` ou `.profile`.
```
cat >> ~/.bash_profile <<EOF
alias btcdir="cd ~/.bitcoin/" #linux default bitcoind path
alias bc="bitcoin-cli"
alias bd="bitcoind"
alias btcinfo='bitcoin-cli getwalletinfo | egrep "\"balance\""; bitcoin-cli getnetworkinfo | egrep "\"version\"|connections"; bitcoin-cli getmininginfo | egrep "\"blocks\"|errors"'
EOF
```
Depois de inserir esses apelidos, você pode usar `source .bash_profile` para inseri-los ou apenas efetuar logout e logar novamente.

Observe que esses apelidos incluem atalhos para executar `bitcoin-cli`, para executar` bitcoind` e para ir para o diretório Bitcoin. Esses apelidos têm como objetivo principal tornar sua vida mais fácil. Sugerimos que você crie outros apelidos para facilitar o uso de comandos frequentes (e argumentos) e para minimizar erros. Aliases desse tipo podem ser ainda mais úteis se você tiver uma configuração complexa onde regularmente executa comandos associados a Mainnet, com Testnet, _e_ com Regtest, conforme explicado mais adiante.

Dito isso, o uso desses aliases _este_ documento pode acidentalmente obscurecer as lições centrais sendo ensinadas sobre Bitcoin, então o único alias usado diretamente aqui é `btcinfo` porque encapsula um comando muito mais longo e complexo. Caso contrário, mostramos os comandos completos; ajuste para seu próprio uso conforme apropriado.

## Rodando o Bitcoind

Você começará sua exploração da rede Bitcoin com o comando `bitcoin-cli`. No entanto, bitcoind _deve_ estar em execução para usar bitcoin-cli, pois bitcoin-cli envia comandos JSON-RPC para o bitcoind. Se você usou nossa configuração padrão, o bitcoind já deve estar instalado e funcionando. Você pode verificar olhando a tabela de processos.
```
$ ps auxww | grep bitcoind
standup    455  1.3 34.4 3387536 1392904 ?     SLsl Jun16  59:30 /usr/local/bin/bitcoind -conf=/home/standup/.bitcoin/bitcoin.conf
```

Se não estiver rodando, você pode rodar `/usr/local/bin/bitcoind -daemon` manualmente e também colocá-lo em seu crontab.

## Verifique seus blocos

Você deve ter baixado todo o blockchain antes de começar a brincar. Basta executar o comando `bitcoin-cli getblockcount` para ver se está tudo carregado.
```
$ bitcoin-cli getblockcount
1772384
```
Isso indica que está carregado; você precisará verificar isso em um serviço online que informa a altura atual do bloco.

> :book: ***O que é a altura do bloco?*** A altura do bloco é a distância que um bloco particular está do bloco de gênese. A altura do bloco atual é a altura do bloco mais recente adicionado a um blockchain.

Você pode fazer isso olhando para um explorador, como [Mempool Space Explorer](https://mempool.space/pt/testnet). O número mais recente corresponde ao `getblockcount`? Se sim, você está atualizado.

Se você quiser que um alias veja tudo de uma vez, o seguinte funciona atualmente para Testnet, mas pode desaparecer em algum momento no futuro:
```
$ cat >> ~/.bash_profile << EOF
alias btcblock="echo \\\`bitcoin-cli getblockcount 2>&1\\\`/\\\`wget -O - https://blockstream.info/testnet/api/blocks/tip/height 2> /dev/null | cut -d : -f2 | rev | cut -c 1- | rev\\\`"
EOF
$ source .bash_profile 
$ btcblock
1804372/1804372
```

> :link: **TESTNET vs MAINNET:** Lembre-se de que este tutorial geralmente assume que você está usando testnet. Se você estiver usando a mainnet, pode recuperar a altura do bloco atual com: `wget -O - https://mempool.space/testnet/api/blocks/tip/height 2> /dev/ null`. Você pode substituir a última metade do alias `btblock` (após `/`) por isso.

Se você não está atualizado, mas seu `getblockcount` está aumentando, não há problema. O tempo total de download pode levar de uma hora a várias horas, dependendo da configuração.

## Opcional: Conheça os tipos de servidores

> **TESTNET vs MAINNET:** Ao configurar seu node Bitcoin, você escolhe criá-lo como um Mainnet, Testnet ou Regtest. Embora este documento presuma uma configuração de rede de teste, vale a pena entender como você pode acessar e usar os outros tipos de configuração - mesmo todos na mesma máquina! Mas, se você for um usuário iniciante, pule isso, pois não é necessário para uma configuração básica.

O tipo de configuração é controlado principalmente por meio do arquivo `~/.bitcoin/bitcoin.conf`. Se você estiver executando o testnet, provavelmente contém esta linha:
```
testnet=1
```
Se você estiver executando o regtest, provavelmente contém esta linha:
```
regtest=1
```
No entanto, se você deseja executar vários tipos diferentes de nós simultaneamente, deve deixar o sinalizador testnet (ou regtest) fora de seu arquivo de configuração. Você pode então escolher se está usando mainnet, testnet ou regtest toda vez que executar bitcoind ou bitcoin-cli.

Aqui está um conjunto de aliases que tornariam isso mais fácil, criando um alias específico para iniciar e parar o bitcoind, para ir para o diretório bitcoin e para executar bitcoin-cli, para cada mainnet (que não tem sinalizadores extras), o testnet (que é -testnet), ou seu regtest (que é -regtest).
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
Para ainda mais complexidade, você pode fazer com que cada um de seus aliases 'start' use o sinalizador -conf para carregar a configuração de um arquivo diferente. Isso vai muito além do escopo deste tutorial, mas o oferecemos como um ponto de partida para quando suas explorações de Bitcoin atingirem o próximo nível.

## Resumo: Verificando a configuração do seu node Bitcoin

Antes de começar a brincar com bitcoin, você deve se certificar de que seus aliases estão configurados, seu bitcoind está rodando e seus blocos foram baixados. Você também pode querer configurar algum acesso a configurações alternativas de Bitcoin, se você for um usuário avançado.

## Oque vem a seguir?

Continue "Understanding Your Bitcoin Setup" com [3.2: Knowing Your Bitcoin Setup](03_2_Knowing_Your_Bitcoin_Setup.md).