# Apêndice 2: Compilando Bitcoin na Fonte

Este curso presume que usará um script para criar um ambiente Bitcoin, seja usando o Bitcoin Standup do Linode de acordo com o especificado na seção [§2.1](02_1_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md), ou por algum outro meio de acordo com a seção [§2.2](02_2_Setting_Up_Bitcoin_Core_Other.md). No entanto, podemos querer compilar o Bitcoin manualmente.

Isso tem os seguintes benefícios:

1. Sempre estará atualizados com a versão mais recente. Mas atenção! Estar sempre atualizado não é necessário para o Bitcoin Core, pois o software é sempre compatível com versões anteriores, o que significa que uma versão antiga do Bitcoin Core ainda poderá participar da rede Bitcoin, embora possamos não ter os recursos mais recentes. Devemos sempre verificar os recursos de uma nova versão antes de atualizarmos.
2. Não precisaremos depender de binários do Bitcoin Core pré-compilados. Isso requer menos confiança. Mesmo que os mantenedores do Bitcoin Core façam um ótimo trabalho em manter a integridade do código, um binário pré-compilado é removido algumas etapas do código-fonte. Quando compilamos a partir do código-fonte, o código pode ser inspecionado antes da compilação.
3. Podemos personalizar a construção, fazendo coisas como desabilitar a carteira ou a GUI.

## Preparando nosso ambiente

Este tutorial usa o sistema operacional Debian 10.4.kv0 em uma arquitetura amd64 (computadores de 64 bits), mas podemos usar este tutorial em qualquer sistema baseado no Debian (por exemplo, Ubuntu, Mint, etc). Para outros sistemas Linux, podemos adaptar as etapas a seguir com o gerenciador de pacotes do sistema.

Podemos ter pouca familiaridade ou nenhuma com a linha de comando, desde que tenhamos vontade de aprender. O terminal é o nosso aliado mais poderoso, não algo a ser temido. Podemos simplesmente copiar e colar os seguintes comandos para compilar Bitcoin. Um comando com um "$" é um comando do usuário normal e um com um "#" é um comando de super usuário/root.

Se o nosso usuário não estiver na lista dos super usuários, podemos o seguinte:

```
$ su root
<enter root passwd>
# apt-get install sudo
# usermod -aG sudo <username>
# reboot
```

## Instalando o Bitcoin

### Etapa 1: atualizando nosso sistema

Primeiro, vamos atualizar o sistema usando:
```
$ sudo apt-get update
```

### Etapa 2: Instalando o Git e as dependências

Vamos instalar o `git`, que permitirá que baixemos o código-fonte, e o `build-essential`, que compila o código:
```
$ sudo apt-get install git build-essential -y
```

Depois, vamos instalar as dependências restantes:
```
$ sudo apt-get install libtool autotools-dev automake pkg-config bsdmainutils python3 libssl-dev libevent-dev libboost-system-dev libboost-filesystem-dev libboost-chrono-dev libboost-test-dev libboost-thread-dev libminiupnpc-dev libzmq3-dev libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools libprotobuf-dev protobuf-compiler ccache -y
```

### Etapa 3: Baixando o código-fonte

Assim que as dependências forem instaladas, baixamos o repositório (repo) que contém o código-fonte do Bitcoin no github:
```
$ git clone https://github.com/bitcoin/bitcoin.git
```
Vamos verificar o conteúdo do repo:
```
$ ls bitcoin
```
Devemos corresponder aproximadamente ao seguinte conteúdo:

![clonando o repo](./public/LBftCLI-compiling_bitcoin-git.png)

### Etapa 4: Instalando o Berkley DB v4.8

1. Vamos entrar no diretório `contrib`:`$ cd bitcoin/contrib/`;
2. Vamos executar o seguinte comando: ```$ ./install_db4.sh `pwd` ```.

Depois de fazer o download, veremos o seguinte retorno. Podemos observar o retorno, e o usaremos para configurar o bitcoin durante o build:

![db4](./public/LBftCLI-compiling_bitcoin-db4.png)

### Etapa 5: Compilando o Bitcoin Core

É recomendado que compilemos a partir de um branch com tag, que é mais estável, a menos que queiramos experimentar as últimas novidades do desenvolvimento do bitcoin. Vamos executar o seguinte comando para obter a lista de tags, ordenada a partir das mais recentes:
```
$ git tag -n | sort -V
```
Em seguida, vamos escolha uma tag, por exemplo a `v0.20.0`:
```
$ git checkout <TAG>
```

Depois de selecionar um ramo da tag, vamos executar o seguinte, dentro do diretório `bitcoin`. O `<PATH-TO>` deve ser o retorno do script `install_db4.sh`.

```
$ ./autogen.sh
$ export BDB_PREFIX='<PATH-TO>/db4'
$ ./configure BDB_LIBS="-L${BDB_PREFIX}/lib -ldb_cxx-4.8" BDB_CFLAGS="-I${BDB_PREFIX}/include"
$ make  # build bitcoin core
```

### Etapa 6: Testando o build

Se quisermos verificar nossa compilação (o que é uma boa ideia), podemos executar os seguintes testes:

1. `$ make check` irá executar os Testes de Unidade, que devem retornar `PASS`;
2. `$ test/functional/test_runner.py --extended` executará os testes funcionais estendidos. Podemos retirar a sinalização `--extended` se quisermos pular alguns testes. Isso vai demorar um pouco.

### Etapa 7: Executando ou instalando o Bitcoin Core

Agora que compilou o Bitcoin Core a partir da fonte, podemos começar a usá-lo ou instalá-lo.

#### Executando o Bitcoin Core sem instalar

Para executar o Bitcoin Core:

`$ src/qt/bitcoin-qt` para iniciar a GUI;
`$ src/bitcoind` para executar bitcoin na linha de comando.

### Instalando Bitcoin Core

Para instalar:

`$ sudo make install` irá instalar o Bitcoin Core globalmente. Uma vez instalado, podemos executar o bitcoin de qualquer lugar usando a linha de comando, assim como qualquer outro software, como: `$ bitcoin-qt` para a GUI ou `bitcoind` e então `bitcoin-cli` para a linha de comando.

## Finalizando Nosso Sistema

Ao compilar Bitcoin a partir da fonte, aumentamos a confiança que temos em nossa configuração. No entanto, estamos muito aquém de toda a segurança adicional fornecida por uma configuração Bitcoin Standup. Para resolver isso, podemos percorrer todo o [Linode Stackscript](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts/blob/master/Scripts/LinodeStandUp.sh) e executar passo a passo todos os comandos. O único lugar que precisamos ter cuidado é na Etapa 6, que instala o Bitcoin. Pulando o ponto onde já verificamos os binários e continuemos a partir daí.

## Resumo: Compilando Bitcoin na Fonte

Se quisermos aumentar a segurança da instalação do Bitcoin a partir da fonte, agora podemos fazer ter isso. Felizmente, também passamos pelo Linode Stackscript para configurar um servidor mais seguro.

## O Que Vem Depois?

Se estávamos no processo de criação de um node de Bitcoin para usar neste curso, devemos continuar com o [Capítulo 3: Compreendendo a configuração do node Bitcoin](03_0_Understanding_Your_Bitcoin_Setup.md).

Se estivermos lendo os apêndices, vamos continuar com o [Apêndice 3: Usando o Bitcoin Regtest](A3_0_Using_Bitcoin_Regtest.md).