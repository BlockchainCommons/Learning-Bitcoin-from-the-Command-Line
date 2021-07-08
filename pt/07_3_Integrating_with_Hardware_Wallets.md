# 7.3: Integração com hardware wallets

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho inicial que ainda pode estar aguardando revisão. Leitor de advertência.

Uma das principais vantagens dos PSBTs é a capacidade de transferir transações para hardware wallets. Esta será uma ótima ferramenta de desenvolvimento para nós continuarmos a programar usando o Bitcoin. No entanto, não podemos testá-la agora se estivermos usando uma das configurações que sugerimos para este curso, uma VM no Linode de acordo com a seção [§2.1](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/02_1_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md) ou uma opção mais expansiva ainda como um AWS de acordo com a seção [§2.2](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/02_2_Setting_Up_Bitcoin_Core_Other.md), isso porque obviamente não teremos como conectar uma hardware wallet à nossa máquina virtual remota.

> :book: ***O que é uma hardware wallet?*** Uma hardware wallet é um dispositivo eletrônico que melhora a segurança da criptomoeda mantendo todas as chaves privadas no dispositivo, ao invés de colocá-las em um computador conectado diretamente a internet. As carteiras de hardware têm protocolos específicos para fornecer interações online, geralmente gerenciadas por um programa que se comunica com o dispositivo por meio de uma porta USB. Neste capítulo, gerenciaremos uma carteira de hardware com o ```bitcoin-cli``` e o programa ```hwy.py```.

Existem três opções de como passar por este capítulo: (1) Ler sem testar o código; (2) Instalar o Bitcoin em uma máquina local para testar todos esses comandos ou; (3) Pular direto para o [Capítulo 8: Expandindo as transações de Bitcoin usando outras maneiras](08_0_Expanding_Bitcoin_Transactions_Other.md). Sugerimos a primeira opção, mas se quisermos colocar a mão na massa, iremos dar o suporte necessário para a segunda opção falando sobre o uso de um Macintosh (uma plataforma hardware que o [Bitcoin Standup](https://github.com/BlockchainCommons/Bitcoin-Standup) dá suporte) para teste.

> :warning: **AVISO DE VERSÃO:** Os PSBTs são uma inovação do Bitcoin Core v0.17.0. As versões anteriores do Bitcoin Core não funcionarão com o PSBT enquanto ele estiver em andamento (embora ainda consigam reconhecer a transação final). A interface HWI apareceu no Bitcoin Core v 0.18.0, mas, desde que estejamos usando nossa configuração sugerida com o Bitcoin Standup, ela deve funcionar.

A metodologia descrita neste capítulo para integração com uma hardware wallet depende do [Bitcoin Hardware Wallet Interface](https://github.com/bitcoin-core/HWI) lançada através do Bitcoin Core e que se baseia na [instalação](https://github.com/bitcoin-core/HWI/blob/master/README.md) e [uso](https://hwi.readthedocs.io) das instruções contidas nele.

> :warning: **AVISO DE NOVIDADE:** A interface HWI é muito nova e precisa de alguns ajustes ainda, mesmo depois usando a v0.20.0 do Bitcoin Core. Pode ser difícil instalá-la corretamente e pode conter erros não intuitivos. O que se segue é uma descrição de uma configuração de trabalho, mas foram necessárias várias tentativas para ter sucesso e sua configuração pode variar.

## Instalando o Bitcoin Core em uma máquina local

_Se pretendemos apenas ler esta seção e não testar os comandos, podemos pular esta subseção, que basicamente explicará como criar uma instalação Bitcoin Core em uma máquina local, como um Mac ou Máquina Linux._

Existem versões alternativas do script Bitcoin Standup que usamos para criar nossa VM que será instalada em um MacOS ou em uma máquina Linux que não seja o Linode.

Se tivermos um MacOS, podemos instalar o [Bitcoin Standup MacOS](https://github.com/BlockchainCommons/Bitcoin-Standup-MacOS/blob/master/README.md).

Se tivermos uma máquina Linux local, podemos instalar o [Bitcoin Standup Linux Scripts](https://github.com/BlockchainCommons/Bitcoin-Standup-MacOS/blob/master/README.md).

Depois de colocar o Bitcoin Standup em execução em nossa máquina local, vamos desejar sincronizar o blockchain "Testnet", assumindo que já estamos seguindo o método padrão deste curso.

Estaremos usando um Macintosh e um Testnet para os exemplos desta seção.

### Criando um alias para o Bitcoin-CLI

Criando um alias que execute o ```bitcoin-cli``` no diretório correto com quaisquer flags apropriadas.

Aqui está um exemplo de um alias de um Mac:
```
$ alias bitcoin-cli="~/StandUp/BitcoinCore/bitcoin-0.20.0/bin/bitcoin-cli -testnet"
```
Podemos notar que ele nos dá não apenas o caminho completo, mas também garante que permaneçamos na Testnet.

## Instalando o HWI em uma máquina local

_As instruções a seguir novamente presumem um Mac e podemos pular novamente esta subseção se estivermos apenas lendo o capítulo._

O HWI é um programa do Bitcoin Core disponível em python que podemos usar para interagir com as hardware wallets.

### Instalando o Python

Como o HWI é escrito em ```python```, precisaremos instalá-lo, bem como alguns programas auxiliares.

Se ainda não temos as ferramentas de linha de comando ```xcode```, iremos precisar delas:
```
$ xcode-select --install
```
Se ainda não temos o gerenciador de pacotes Homebrew, precisamos instalá-lo também. As instruções atuais estão disponíveis no [site do Homebrew](https://brew.sh/). No momento em que este livro foi escrito, basta utilizarmos o comando abaixo:
```
$ /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"
```

Para uma instalação pela primeira vez, também devemos nos certificar de que nosso diretório ```/usr/local/Frameworks``` foi criado corretamente:
```
$ sudo mkdir /usr/local/Frameworks
$ sudo chown $(whoami):admin /usr/local/Frameworks
```
Se já fizemos tudo isso, podemos finalmente instalar o Python:
```
$ brew install python
$ brew install libusb
```

### Instalando o HWI

Agora estamos prontos para instalar o HWI, o que requer a clonagem de um repositório GitHub e a execução de um script de instalação.

Se ainda não temos o ```git``` instalado em nosso Mac, podemos fazer isso apenas executando o comando: ```git --version```.

Podemos então clonar o repositório HWI:
```
$ cd ~/StandUp
$ git clone https://github.com/bitcoin-core/HWI.git
```
Depois, precisaremos instalar o pacote e as dependências:
```
$ cd HWI
HWI$ python3 setup.py install
```
### Criando um alias para o HWI

Iremos querer criar um alias aqui também, variando de acordo com o local de instalação real:
```
$ alias hwi="~/Standup/HWI/hwi.py --testnet"
```
Novamente, incluímos uma referência a testnet ao alias.

## Preparando nossa Ledger

_Precisamos escolher uma plataforma de hardware wallet para a demonstração de HWI. Nossa escolha foi a Ledger, que há muito tempo é onde fazemos os testes nestes casos. Podemos consultar as [informações de suporte do dispositivo do HWI](https://github.com/bitcoin-core/HWI/blob/master/README.md#device-support) para obter uma lista de outros dispositivos compatíveis. Se usarmos um dispositivo diferente de uma Ledger, precisaremos avaliar as nossas soluções para prepará-la para uso na Testnet, mas caso contrário, devemos  ser capazes de continuar com o curso conforme escrito aqui._

Se estiver trabalhando com Bitcoins usando a Ledger, provavelmente precisaremos fazer nada. (Mas não sugerimos isso neste curso).

Para trabalhar com moedas usando a Testnet, conforme sugerido aqui, precisaremos fazer algumas atualizações:

1. Vamos em Configurações em nosso aplicativo Ledger Live (clicando na engrenagem), depois na guia "Recursos Experimentais" e vamos ativar o "Modo de Desenvolvedor";
2. Vamos no "Gerenciar" para instalar o "Bitcoin Test". A versão atual requer que tenhamos o "Bitcoin" e o "Ethereum" instalados primeiro;
3. Vamos no "Gerenciar", para instalar o nosso novo "Bitcoin Test" e "Adicionar uma conta".

## Acoplando na Ledger

Para que um Razão esteja acessível, devemos fazer o login com o nosso PIN e, em seguida, acessar o aplicativo que desejamos usar, que no caso é o aplicativo "Bitcoin Test". Pode ser necessário repetir isso algumas vezes quando nossa Ledger ficar em stand-by.

Depois, podemos solicitar que o HWI acesse a Ledger com o comando `enumerate`:
```
$ hwi enumerate
[{"type": "ledger", "model": "ledger_nano_s", "path": "IOService:/AppleACPIPlatformExpert/PCI0@0/AppleACPIPCI/XHC1@14/XHC1@14000000/HS05@14100000/Nano S@14100000/Nano S@0/IOUSBHostHIDDevice@14100000,0", "fingerprint": "9a1d520b", "needs_pin_sent": false, "needs_passphrase_sent": false}]
```
Se recebermos algumas informações no nosso dispositivo, está tudo certo! Como podemos observar, ele verifica o tipo da hardware wallet, fornecendo outras informações de identificação e também informa como se comunicar com o dispositivo. A ```fingerprint``` (```9a1d520b```) é o que devemos prestar atenção especial, porque todas as interações com nossa hardware wallet irá exigir essa informação.

Se, ao invés disso, obtivermos como resultado ```[]```, então (1) Não deixamos nosso dispositivo Ledger habilitado com o PIN correto e não escolhemos o aplicativo correto, ou (2) há algo errado com nossa configuração Python, provavelmente uma dependência esteja faltando: Devemos considerar usar o comando ```uninstall``` e tentar do zero novamente.

## Importando os endereços

A interação com uma hardware wallet geralmente ocorre em duas partes: Observar os saldos e gastá-los.

Podemos monitorar os fundos importando os endereços da nossa hardware wallet para o full node, usando o HWI e o ```bitcoin-cli```.

### Criando uma carteira

Para usar nossa hardware wallet com o ```bitcoin-cli```, precisaremos criar uma carteira com nome específico no Bitcoin Core, usando o RPC ```createwallet```, que é um comando que não discutimos anteriormente.
```
$ bitcoin-cli --named createwallet wallet_name="ledger" disable_private_keys="true"
{
  "name": "ledger",
  "warning": ""
}
```
Neste caso, estamos criando uma carteira chamada `ledger` sem chaves privadas (já que estas estarão no dispositivo da Ledger).

> :book: ***Por que nomear as carteiras?*** Até agora, este curso usou uma carteira padrão ("") no Bitcoin Core. Isso é bom para muitos propósitos, mas é inadequado se tivermos uma situação mais complexa, como quando estamos usando as chaves de uma hardware wallet. Aqui, queremos ser capazes de diferenciar as chaves de locais (que são mantidas na carteira "") e as chaves da Ledger (que são mantidas na carteira "ledger").

Agora podemos observar que a nova carteira está em nossa lista de carteiras:
```
$ bitcoin-cli listwallets
[
  "",
  "ledger"
]
```
Como criamos uma segunda carteira, alguns comandos exigirão um sinalizador ```-rpcwallet =```, para especificar qual delas estamos utilizando.

### Importando as chaves

Agora temos que importar uma lista de endereços de observação da hardware wallet. Isso é feito com o comando ```getkeypool``` do HWI:
```
$ hwi -f 9a1d520b getkeypool --wpkh 0 1000
[{"desc": "wpkh([9a1d520b/84h/1h/0h]tpubDD7KTtoGzK9GuWUQcr1uTJazsAkqoXhdrwGXWVix6nPpNZmSbagZWD4QSaMsyK8YohAirGDPrWdRiEpKzTFB7DrTrqfzHCn7yi5EsqeR93S/0/*)#qttxy592", "range": [0, 1000], "timestamp": "now", "internal": false, "keypool": true, "active": true, "watchonly": true}, {"desc": "wpkh([9a1d520b/84h/1h/0h]tpubDD7KTtoGzK9GuWUQcr1uTJazsAkqoXhdrwGXWVix6nPpNZmSbagZWD4QSaMsyK8YohAirGDPrWdRiEpKzTFB7DrTrqfzHCn7yi5EsqeR93S/1/*)#3lw8ep4j", "range": [0, 1000], "timestamp": "now", "internal": true, "keypool": true, "active": true, "watchonly": true}]
```
Endereçamos o HWI com a ```fingerprint``` e solicitamos os primeiros 1000 endereços WPKH (Segwit nativo). Em troca, recebemos dois descritores para o conjunto de chaves: Um para receber endereços e outro para alterar endereços.

> :book: ***O que é um conjunto de chaves?*** Um conjunto de chaves é um grupo de chaves pré-geradas. As hardware wallets modernas criam molhos de chaves usando novos endereços hierárquicos com base na seed original. A ideia destes molhos de chaves é facilitar os requisitos de backup das carteiras. Isso permitia que um usuário gerasse um conjunto de chaves e fizesse backup da carteira imediatamente, ao invés de exigir backups após a criação de cada novo endereço. O conceito também tem se mostrado muito útil na atualidade, pois permite a importação de todo um conjunto de endereços de um dispositivo para outro.

Os valores retornados pelo comando ```getkeypool``` são os mesmos tipos de descritores que aprendemos na seção [§3.5: Compreendendo o descritor](03_5_Understanding_the_Descriptor.md). Na época, dissemos que eram úteis para mover endereços entre máquinas diferentes. Aqui está o exemplo da vida real: Mover endereços de uma hardware wallet para o node do Bitcoin Core, de modo que nossa máquina conectada à rede possa vigiar as chaves pertencentes à carteira da hardware wallet.

Assim como aprendemos na seção [§3.5](03_5_Understanding_the_Descriptor.md), podemos examinar esses descritores com o comando RPC ```getdescriptorinfo```:
```
$ bitcoin-cli getdescriptorinfo "wpkh([9a1d520b/84h/1h/0h]tpubDD7KTtoGzK9GuWUQcr1uTJazsAkqoXhdrwGXWVix6nPpNZmSbagZWD4QSaMsyK8YohAirGDPrWdRiEpKzTFB7DrTrqfzHCn7yi5EsqeR93S/0/*)#qttxy592"
{
  "descriptor": "wpkh([9a1d520b/84'/1'/0']tpubDD7KTtoGzK9GuWUQcr1uTJazsAkqoXhdrwGXWVix6nPpNZmSbagZWD4QSaMsyK8YohAirGDPrWdRiEpKzTFB7DrTrqfzHCn7yi5EsqeR93S/0/*)#n65e7wjf",
  "checksum": "qttxy592",
  "isrange": true,
  "issolvable": true,
  "hasprivatekeys": false
}
```
Como seria de esperar, _não_ temos ```privatekeys```, porque as hardware wallets ficam com a posse.

Com os descritores em mãos, podemos importar as chaves para nossa nova carteira ```ledger``` usando o comando RPC ```importmulti``` que já usamos na seção [§3.5](03_5_Understanding_the_Descriptor.md). Nesse caso, basta colocar toda a resposta que recebemos do HWI entre ```'``` s.
```
$ bitcoin-cli -rpcwallet=ledger importmulti '[{"desc": "wpkh([9a1d520b/84h/1h/0h]tpubDD7KTtoGzK9GuWUQcr1uTJazsAkqoXhdrwGXWVix6nPpNZmSbagZWD4QSaMsyK8YohAirGDPrWdRiEpKzTFB7DrTrqfzHCn7yi5EsqeR93S/0/*)#qttxy592", "range": [0, 1000], "timestamp": "now", "internal": false, "keypool": true, "active": true, "watchonly": true}, {"desc": "wpkh([9a1d520b/84h/1h/0h]tpubDD7KTtoGzK9GuWUQcr1uTJazsAkqoXhdrwGXWVix6nPpNZmSbagZWD4QSaMsyK8YohAirGDPrWdRiEpKzTFB7DrTrqfzHCn7yi5EsqeR93S/1/*)#3lw8ep4j", "range": [0, 1000], "timestamp": "now", "internal": true, "keypool": true, "active": true, "watchonly": true}]'
[
  {
    "success": true
  },
  {
    "success": true
  }
]
```
(Observe que HWI aproveita o caminho de derivação com ```h```s para mostrar as derivações ao invés dos ```'```s, além de ter calculado a soma da verificação corretamente, para que não tenhamos que fazer citações massivas como fizemos na seção §3.5.)

Agora, _podemos_ listar todos os endereços apenas para observar que recebemos o valor, usando o comando ```getaddressesbylabel```. Todos os 1000 endereços de recebimento estão ali, na carteira ```ledger```!
```
$ bitcoin-cli -rpcwallet=ledger getaddressesbylabel "" | more
{
  "tb1qqqvnezljtmc9d7x52udpc0m9zgl9leugd2ur7y": {
    "purpose": "receive"
  },
  "tb1qqzvrm6hujdt93qctuuev5qc4499tq9fdk0prwf": {
    "purpose": "receive"
  },
...
}
```
## Recebendo uma transação

Obviamente, receber uma transação é bem simples. Usamos o ```getnewaddress``` para solicitar um desses endereços importados:
```
$ bitcoin-cli -rpcwallet=ledger getnewaddress
tb1qqqvnezljtmc9d7x52udpc0m9zgl9leugd2ur7y
```
Então enviamos o dinheiro para ele.

O poder do HWI é que podemos observar os pagamentos do nosso node Bitcoin Core, ao invés de ter que conectar nossa hardware wallet e consultá-la.
```
$ bitcoin-cli -rpcwallet=ledger listunspent
[
  {
    "txid": "c733533eb1c052242f9ed89cd8927aedb41852156e684634ee7c74028774e595",
    "vout": 1,
    "address": "tb1q948388a23pfsf52kz6skd5k4z4627jja2evztr",
    "label": "",
    "scriptPubKey": "00142d4f139faa885304d15616a166d2d51574af4a5d",
    "amount": 0.01000000,
    "confirmations": 12,
    "spendable": false,
    "solvable": true,
    "desc": "wpkh([9a1d520b/84'/1'/0'/0/0]02a013cf9c4b5f5689d9253036a3e477cf98689626f7814c94f092726f11b741ab)#9za8hlvk",
    "safe": true
  },
  {
    "txid": "5b3c4aeb811f9a119fd633b12a6927415cc61b8654628df58e9141cab804bab8",
    "vout": 0,
    "address": "tb1qqqvnezljtmc9d7x52udpc0m9zgl9leugd2ur7y",
    "label": "",
    "scriptPubKey": "001400193c8bf25ef056f8d4571a1c3f65123e5fe788",
    "amount": 0.01000000,
    "confirmations": 1,
    "spendable": false,
    "solvable": true,
    "desc": "wpkh([9a1d520b/84'/1'/0'/0/569]030168d9482e2b02d7027fb4a89edc54adaa1adf709334f647d0a1b0533828aec5)#sx9haake",
    "safe": true
  }
]
```
## Criando uma transação com o PSBT

Observar e receber os pagamentos é apenas metade da batalha. Também podemos fazer pagamentos usando contas mantidas em nossa hardware wallet. Este é o quarto exemplo da vida real usando os PSBTs, de acordo com o processo descrito na seção [§7.1: Criando uma Transação Bitcoin Parcialmente Assinada](7_1_Creating_a_Partially_Signed_Bitcoin_Transaction.md).

Os comandos funcionam exatamente da mesma forma. Nesse caso, usamos o ```walletcreatefundedpsbt``` para formar nosso PSBT porque estamos em uma situação em que não nos importamos com quais UTXOs são usados:
```
$ bitcoin-cli -named -rpcwallet=ledger walletcreatefundedpsbt inputs='''[]''' outputs='''[{"tb1qcaedd724gts3aug73m78c7nfsv9d8zs9q6h2kd":0.015}]'''
{
  "psbt": "cHNidP8BAJoCAAAAAri6BLjKQZGO9Y1iVIYbxlxBJ2kqsTPWnxGaH4HrSjxbAAAAAAD+////leV0hwJ0fO40RmhuFVIYtO16ktic2J4vJFLAsT5TM8cBAAAAAP7///8CYOMWAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBU+gBwAAAAAAFgAU9Ojd5ds3CJi1fIRWbj92CYhQgX0AAAAAAAEBH0BCDwAAAAAAFgAUABk8i/Je8Fb41FcaHD9lEj5f54giBgMBaNlILisC1wJ/tKie3FStqhrfcJM09kfQobBTOCiuxRiaHVILVAAAgAEAAIAAAACAAAAAADkCAAAAAQEfQEIPAAAAAAAWABQtTxOfqohTBNFWFqFm0tUVdK9KXSIGAqATz5xLX1aJ2SUwNqPkd8+YaJYm94FMlPCScm8Rt0GrGJodUgtUAACAAQAAgAAAAIAAAAAAAAAAAAAAIgID2UK1nupSfXC81nmB65XZ+pYlJp/W6wNk5FLt5ZCSx6kYmh1SC1QAAIABAACAAAAAgAEAAAABAAAAAA==",
  "fee": 0.00000209,
  "changepos": 1
}
```
Podemos observar os PSBT e verificar se ele parece correto:
```
$ psbt="cHNidP8BAJoCAAAAAri6BLjKQZGO9Y1iVIYbxlxBJ2kqsTPWnxGaH4HrSjxbAAAAAAD+////leV0hwJ0fO40RmhuFVIYtO16ktic2J4vJFLAsT5TM8cBAAAAAP7///8CYOMWAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBU+gBwAAAAAAFgAU9Ojd5ds3CJi1fIRWbj92CYhQgX0AAAAAAAEBH0BCDwAAAAAAFgAUABk8i/Je8Fb41FcaHD9lEj5f54giBgMBaNlILisC1wJ/tKie3FStqhrfcJM09kfQobBTOCiuxRiaHVILVAAAgAEAAIAAAACAAAAAADkCAAAAAQEfQEIPAAAAAAAWABQtTxOfqohTBNFWFqFm0tUVdK9KXSIGAqATz5xLX1aJ2SUwNqPkd8+YaJYm94FMlPCScm8Rt0GrGJodUgtUAACAAQAAgAAAAIAAAAAAAAAAAAAAIgID2UK1nupSfXC81nmB65XZ+pYlJp/W6wNk5FLt5ZCSx6kYmh1SC1QAAIABAACAAAAAgAEAAAABAAAAAA=="

$ bitcoin-cli decodepsbt $psbt
{
  "tx": {
    "txid": "45f996d4ff8c9e9ab162f611c5b6ad752479ede9780f9903bdc80cd96619676d",
    "hash": "45f996d4ff8c9e9ab162f611c5b6ad752479ede9780f9903bdc80cd96619676d",
    "version": 2,
    "size": 154,
    "vsize": 154,
    "weight": 616,
    "locktime": 0,
    "vin": [
      {
        "txid": "5b3c4aeb811f9a119fd633b12a6927415cc61b8654628df58e9141cab804bab8",
        "vout": 0,
        "scriptSig": {
          "asm": "",
          "hex": ""
        },
        "sequence": 4294967294
      },
      {
        "txid": "c733533eb1c052242f9ed89cd8927aedb41852156e684634ee7c74028774e595",
        "vout": 1,
        "scriptSig": {
          "asm": "",
          "hex": ""
        },
        "sequence": 4294967294
      }
    ],
    "vout": [
      {
        "value": 0.01500000,
        "n": 0,
        "scriptPubKey": {
          "asm": "0 c772d6f95542e11ef11e8efc7c7a69830ad38a05",
          "hex": "0014c772d6f95542e11ef11e8efc7c7a69830ad38a05",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1qcaedd724gts3aug73m78c7nfsv9d8zs9q6h2kd"
          ]
        }
      },
      {
        "value": 0.00499791,
        "n": 1,
        "scriptPubKey": {
          "asm": "0 f4e8dde5db370898b57c84566e3f76098850817d",
          "hex": "0014f4e8dde5db370898b57c84566e3f76098850817d",
          "reqSigs": 1,
          "type": "witness_v0_keyhash",
          "addresses": [
            "tb1q7n5dmewmxuyf3dtus3txu0mkpxy9pqtacuprak"
          ]
        }
      }
    ]
  },
  "unknown": {
  },
  "inputs": [
    {
      "witness_utxo": {
        "amount": 0.01000000,
        "scriptPubKey": {
          "asm": "0 00193c8bf25ef056f8d4571a1c3f65123e5fe788",
          "hex": "001400193c8bf25ef056f8d4571a1c3f65123e5fe788",
          "type": "witness_v0_keyhash",
          "address": "tb1qqqvnezljtmc9d7x52udpc0m9zgl9leugd2ur7y"
        }
      },
      "bip32_derivs": [
        {
          "pubkey": "030168d9482e2b02d7027fb4a89edc54adaa1adf709334f647d0a1b0533828aec5",
          "master_fingerprint": "9a1d520b",
          "path": "m/84'/1'/0'/0/569"
        }
      ]
    },
    {
      "witness_utxo": {
        "amount": 0.01000000,
        "scriptPubKey": {
          "asm": "0 2d4f139faa885304d15616a166d2d51574af4a5d",
          "hex": "00142d4f139faa885304d15616a166d2d51574af4a5d",
          "type": "witness_v0_keyhash",
          "address": "tb1q948388a23pfsf52kz6skd5k4z4627jja2evztr"
        }
      },
      "bip32_derivs": [
        {
          "pubkey": "02a013cf9c4b5f5689d9253036a3e477cf98689626f7814c94f092726f11b741ab",
          "master_fingerprint": "9a1d520b",
          "path": "m/84'/1'/0'/0/0"
        }
      ]
    }
  ],
  "outputs": [
    {
    },
    {
      "bip32_derivs": [
        {
          "pubkey": "03d942b59eea527d70bcd67981eb95d9fa9625269fd6eb0364e452ede59092c7a9",
          "master_fingerprint": "9a1d520b",
          "path": "m/84'/1'/0'/1/1"
        }
      ]
    }
  ],
  "fee": 0.00000209
}
```
And as usual, `analyzepsbt` will show how far you've gotten:
E, como de costume, `analisepsbt` mostrará o quão longe você chegou:
```
$ bitcoin-cli analyzepsbt $psbt
{
  "inputs": [
    {
      "has_utxo": true,
      "is_final": false,
      "next": "signer",
      "missing": {
        "signatures": [
          "00193c8bf25ef056f8d4571a1c3f65123e5fe788"
        ]
      }
    },
    {
      "has_utxo": true,
      "is_final": false,
      "next": "signer",
      "missing": {
        "signatures": [
          "2d4f139faa885304d15616a166d2d51574af4a5d"
        ]
      }
    }
  ],
  "estimated_vsize": 208,
  "estimated_feerate": 0.00001004,
  "fee": 0.00000209,
  "next": "signer"
}
```
Como importamos aquele conjunto de chaves, o ```bitcoin-cli``` tem todas as informações de que precisa para preencher as entradas, porém, ele não pode assinar porque as chaves privadas são mantidas na hardware wallet.

É aí que entra o HWI, com o comando ```signtx```. Basta que enviemos junto com o PSBT:
```
$ hwi -f 9a1d520b signtx $psbt
```
É provável  que tenhamos de mexer em nossa hardware wallet neste momento. O dispositivo provavelmente pedirá a confirmação das entradas, as saídas e a taxa. Quando terminar, ele deve retornar um novo PSBT.

```
{"psbt": "cHNidP8BAJoCAAAAAri6BLjKQZGO9Y1iVIYbxlxBJ2kqsTPWnxGaH4HrSjxbAAAAAAD+////leV0hwJ0fO40RmhuFVIYtO16ktic2J4vJFLAsT5TM8cBAAAAAP7///8CYOMWAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBU+gBwAAAAAAFgAU9Ojd5ds3CJi1fIRWbj92CYhQgX0AAAAAAAEBH0BCDwAAAAAAFgAUABk8i/Je8Fb41FcaHD9lEj5f54giAgMBaNlILisC1wJ/tKie3FStqhrfcJM09kfQobBTOCiuxUcwRAIgAxkQlk2fqEMxvP54WWyiFhlfSul9sd4GzKDhfGpmlewCIHYej3zXWWMgWI6rixxQw9yzGozDaFPqQNNIvcFPk+lfASIGAwFo2UguKwLXAn+0qJ7cVK2qGt9wkzT2R9ChsFM4KK7FGJodUgtUAACAAQAAgAAAAIAAAAAAOQIAAAABAR9AQg8AAAAAABYAFC1PE5+qiFME0VYWoWbS1RV0r0pdIgICoBPPnEtfVonZJTA2o+R3z5holib3gUyU8JJybxG3QatHMEQCIH5t6T2yufUP7glYZ8YH0/PhDFpotSmjgZUhvj6GbCFIAiBcgXzyYl7IjYuaF3pJ7AgW1rLYkjeCJJ2M9pVUrq5vFwEiBgKgE8+cS19WidklMDaj5HfPmGiWJveBTJTwknJvEbdBqxiaHVILVAAAgAEAAIAAAACAAAAAAAAAAAAAACICA9lCtZ7qUn1wvNZ5geuV2fqWJSaf1usDZORS7eWQksepGJodUgtUAACAAQAAgAAAAIABAAAAAQAAAAA="}
$ psbt_f="cHNidP8BAJoCAAAAAri6BLjKQZGO9Y1iVIYbxlxBJ2kqsTPWnxGaH4HrSjxbAAAAAAD+////leV0hwJ0fO40RmhuFVIYtO16ktic2J4vJFLAsT5TM8cBAAAAAP7///8CYOMWAAAAAAAWABTHctb5VULhHvEejvx8emmDCtOKBU+gBwAAAAAAFgAU9Ojd5ds3CJi1fIRWbj92CYhQgX0AAAAAAAEBH0BCDwAAAAAAFgAUABk8i/Je8Fb41FcaHD9lEj5f54giAgMBaNlILisC1wJ/tKie3FStqhrfcJM09kfQobBTOCiuxUcwRAIgAxkQlk2fqEMxvP54WWyiFhlfSul9sd4GzKDhfGpmlewCIHYej3zXWWMgWI6rixxQw9yzGozDaFPqQNNIvcFPk+lfASIGAwFo2UguKwLXAn+0qJ7cVK2qGt9wkzT2R9ChsFM4KK7FGJodUgtUAACAAQAAgAAAAIAAAAAAOQIAAAABAR9AQg8AAAAAABYAFC1PE5+qiFME0VYWoWbS1RV0r0pdIgICoBPPnEtfVonZJTA2o+R3z5holib3gUyU8JJybxG3QatHMEQCIH5t6T2yufUP7glYZ8YH0/PhDFpotSmjgZUhvj6GbCFIAiBcgXzyYl7IjYuaF3pJ7AgW1rLYkjeCJJ2M9pVUrq5vFwEiBgKgE8+cS19WidklMDaj5HfPmGiWJveBTJTwknJvEbdBqxiaHVILVAAAgAEAAIAAAACAAAAAAAAAAAAAACICA9lCtZ7qUn1wvNZ5geuV2fqWJSaf1usDZORS7eWQksepGJodUgtUAACAAQAAgAAAAIABAAAAAQAAAAA="
```
Ao analisá-lo, veremos que estamos prontos para finalizá-lo:
```
$ bitcoin-cli analyzepsbt $psbt_f
{
  "inputs": [
    {
      "has_utxo": true,
      "is_final": false,
      "next": "finalizer"
    },
    {
      "has_utxo": true,
      "is_final": false,
      "next": "finalizer"
    }
  ],
  "estimated_vsize": 208,
  "estimated_feerate": 0.00001004,
  "fee": 0.00000209,
  "next": "finalizer"
}
```
Neste momento, estamos de volta a terras já conhecidas:
```
$ bitcoin-cli finalizepsbt $psbt_f
{
  "hex": "02000000000102b8ba04b8ca41918ef58d6254861bc65c4127692ab133d69f119a1f81eb4a3c5b0000000000feffffff95e5748702747cee3446686e155218b4ed7a92d89cd89e2f2452c0b13e5333c70100000000feffffff0260e3160000000000160014c772d6f95542e11ef11e8efc7c7a69830ad38a054fa0070000000000160014f4e8dde5db370898b57c84566e3f76098850817d024730440220031910964d9fa84331bcfe78596ca216195f4ae97db1de06cca0e17c6a6695ec0220761e8f7cd7596320588eab8b1c50c3dcb31a8cc36853ea40d348bdc14f93e95f0121030168d9482e2b02d7027fb4a89edc54adaa1adf709334f647d0a1b0533828aec50247304402207e6de93db2b9f50fee095867c607d3f3e10c5a68b529a3819521be3e866c214802205c817cf2625ec88d8b9a177a49ec0816d6b2d8923782249d8cf69554aeae6f17012102a013cf9c4b5f5689d9253036a3e477cf98689626f7814c94f092726f11b741ab00000000",
  "complete": true
}
$ hex=02000000000102b8ba04b8ca41918ef58d6254861bc65c4127692ab133d69f119a1f81eb4a3c5b0000000000feffffff95e5748702747cee3446686e155218b4ed7a92d89cd89e2f2452c0b13e5333c70100000000feffffff0260e3160000000000160014c772d6f95542e11ef11e8efc7c7a69830ad38a054fa0070000000000160014f4e8dde5db370898b57c84566e3f76098850817d024730440220031910964d9fa84331bcfe78596ca216195f4ae97db1de06cca0e17c6a6695ec0220761e8f7cd7596320588eab8b1c50c3dcb31a8cc36853ea40d348bdc14f93e95f0121030168d9482e2b02d7027fb4a89edc54adaa1adf709334f647d0a1b0533828aec50247304402207e6de93db2b9f50fee095867c607d3f3e10c5a68b529a3819521be3e866c214802205c817cf2625ec88d8b9a177a49ec0816d6b2d8923782249d8cf69554aeae6f17012102a013cf9c4b5f5689d9253036a3e477cf98689626f7814c94f092726f11b741ab00000000
$ bitcoin-cli sendrawtransaction $hex
45f996d4ff8c9e9ab162f611c5b6ad752479ede9780f9903bdc80cd96619676d
```
Pronto, enviamos um saldo com sucesso usando as chaves privadas de nossa hardware wallet!

## Aprendendo outros comandos HWI

Existem vários outros comandos disponíveis para usar com o HWI. No momento em que este artigo foi escrito, são eles:
```
numerate,getmasterxpub,signtx,getxpub,signmessage,getkeypool,getdescriptors,displayaddress,setup,wipe,restore,backup,promptpin,togglepassphrase,sendpin
```
## Resumo: Integração com hardware wallets

As hardware wallets podem oferecer melhor proteção, mantendo nossas chaves privadas offline, protegidas no hardware. Felizmente, ainda há uma maneira de interagir com elas usando o ```bitcoin-cli```. Basta instalar o HWI e ele permitirá (1) Importar as chaves públicas para observarmos e; (2) Assinar transações usando nossa hardware wallet.

> :fire: ***Qual é o poder do HWI?*** O HWI permite que possamos interagir com hardware wallets usando todos os comandos de ```bitcoin-cli``` que aprendemos até agora. Podemos fazer as transações brutas de qualquer tipo e enviar PSBTs para hardware wallets para serem assinadas. Assim, temos todo o poder do Bitcoin Core, mas também temos a segurança de um dispositivo hardware.

## O Que Vem Depois?

Vamos expandir ainda mais as transações de Bitcoin com o [Capítulo Oito: Expandindo transações de Bitcoin de outras maneiras](08_0_Expanding_Bitcoin_Transactions_Other.md).