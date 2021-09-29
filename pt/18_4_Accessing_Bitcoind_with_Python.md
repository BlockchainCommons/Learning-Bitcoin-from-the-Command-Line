# 18.4: Acessando o Bitcoind com Python

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho inicial que ainda pode estar aguardando revisão.

Esta seção explica como interagir com o `bitcoind` usando a linguagem de programação Python e o [Python-BitcoinRPC](https://github.com/jgarzik/python-bitcoinrpc).

## Configurando o Python

Se já temos o Bitcoin Core instalado, provavelmente temos o Python 3 disponível.
Podemos verificar isso executando:

```$ python3 --version```

Se ele retornar um número de versão (por exemplo, `3.7.3` ou `3.8.3`), então temos o python3 instalado.

No entanto, se não tivermos o Python instalado, precisaremos compilá-lo a partir do código-fonte. Devemos consultar como fazer isso em ["Construindo Python da Fonte"](18_4_Accessing_Bitcoind_with_Python.md#variant-build-python-from-source) antes de continuarmos.

### Configurando o BitcoinRPC

Quer tenhamos usado um Python existente ou compilado a partir da fonte, agora estamos prontos para instalar a biblioteca `python-bitcoinrpc`:

```
$ pip3 install python-bitcoinrpc
```
Se não instalamos o `pip`, precisaremos executar o seguinte:
```
$ sudo apt install python3-pip
```
Em seguida, vamos repetir o comando `pip3 install python-bitcoinrpc`.

### Criando um Projeto BitcoinRPC

Geralmente, precisaremos incluir declarações apropriadas do `bitcoinrpc` em nosso projetos Bitcoin usando o Python. O seguinte fornecerá acesso aos comandos baseados em RPC:
```py
from bitcoinrpc.authproxy import AuthServiceProxy, JSONRPCException
```
Também podemos achar o seguinte comando útil:
```py
from pprint import pprint
import logging
```
O `pprint` irá imprimir a resposta `json` do `bitcoind`.

O `logging` irá imprimir a chamada que fizemos para o respose do `bitcoind` e o próprio `bitcoind`, o que é útil quando fazemos um monte de chamadas juntas. Se não quisermos um retorno muito grande no terminal, podemos comentar o bloco `logging`.

## Construindo a Nossa Conexão

Agora estamos prontos para começar a interagir com o `bitcoind` estabelecendo uma conexão. Vamos criar um arquivo chamado `btcrpc.py` e digitar o seguinte:

```py
logging.basicConfig()
logging.getLogger("BitcoinRPC").setLevel(logging.DEBUG)
# rpc_user e rpc_password estão configurados no arquivo bitcoin.conf
rpc_user = "StandUp"
rpc_pass = "6305f1b2dbb3bc5a16cd0f4aac7e1eba"
rpc_host = "127.0.0.1"
rpc_client = AuthServiceProxy(f"http://{rpc_user}:{rpc_pass}@{rpc_host}:18332", timeout=120)
```

Os argumentos na URL são `<rpc_username>:<rpc_password>@<host_IP_address>:<port>`. Como de costume, o `user` e o `pass` são encontrados no arquivo `~/.bitcoin/bitcoin.conf`, enquanto o `host` é o nosso localhost, e a porta é `18332` para Testnet. O argumento `timeout` é especificado desde o tempo limite dos sockets sob grande demanda na rede principal. Se obtivermos a resposta `socket.timeout: timed out`, precisamos ser pacientes e aumentar o` timeout`.

> :link: **MAINNET VS TESTNET:** A porta seria 8332 para uma configuração na Mainnet.

### Fazendo uma Chamada RPC

Se o `rpc_client` for inicializado com sucesso, seremos capazes de enviar comandos RPC para o nosso node do Bitcoin.

Para usar um método RPC do `python-bitcoinrpc`, usaremos o objeto `rpc_client` que criamos, que fornece a maior parte da funcionalidade que pode ser acessada através do `bitcoin-cli`, usando os mesmos nomes do método.

Por exemplo, o comando a seguir irá recuperar a contagem de blocos do nosso node:

```py
block_count = rpc_client.getblockcount()
print("---------------------------------------------------------------")
print("Block Count:", block_count)
print("---------------------------------------------------------------\n")
```

Devemos obter a seguinte resposta com o `logging` habilitado:


```sh
DEBUG:BitcoinRPC:-3-> getblockcount []
DEBUG:BitcoinRPC:<-3- 1773020
---------------------------------------------------------------
Block Count: 1773020
---------------------------------------------------------------
```

### Fazendo uma Chamada RPC com Argumentos

Podemos usar esse blockcount como um argumento para recuperar o blockhash de um bloco e também para recuperar os detalhes do bloco.

Isso é feito enviando nossos comandos do objeto `rpc_client` com um argumento:
```py
blockhash = rpc_client.getblockhash(block_count)
block = rpc_client.getblock(blockhash)
```

O `getblockhash` retornará um único valor, enquanto o `getblock` retornará um array associativo de informações sobre o bloco, que inclui um array em `block['tx']` fornecendo detalhes sobre cada transação dentro do bloco:

```py
nTx = block['nTx']
if nTx > 10:
    it_txs = 10
    list_tx_heading = "First 10 transactions: "
else:
    it_txs = nTx
    list_tx_heading = f"All the {it_txs} transactions: "
print("---------------------------------------------------------------")
print("BLOCK: ", block_count)
print("-------------")
print("Block Hash...: ", blockhash)
print("Merkle Root..: ", block['merkleroot'])
print("Block Size...: ", block['size'])
print("Block Weight.: ", block['weight'])
print("Nonce........: ", block['nonce'])
print("Difficulty...: ", block['difficulty'])
print("Number of Tx.: ", nTx)
print(list_tx_heading)
print("---------------------")
i = 0
while i < it_txs:
    print(i, ":", block['tx'][i])
    i += 1
print("---------------------------------------------------------------\n")
```

### Executando nosso código

Podemos usar [o código que está no src/](../src/18_4_getinfo.py) e executá-lo com `python3`:

```
$ python3 getinfo.py
---------------------------------------------------------------
Block Count: 1831106
---------------------------------------------------------------
---------------------------------------------------------------
BLOCK:  1831106
-------------
Block Hash...:  00000000000003b2ea7c2cdfffd86156ad1f5606ab58e128940a2534d1348b04
Merkle Root..:  056a547fe59208167eef86fa694263728fb684119254b340c1f86bdd423a8082
Block Size...:  52079
Block Weight.:  128594
Nonce........:  1775583700
Difficulty...:  4194304
Number of Tx.:  155
First 10 transactions: 
---------------------
0 : d228d55112e3aa26265b0118cfdc98345c229d20fe074b9afb87107c03ce11b5
1 : 92822e8e34fafb472b87c99ea3f3e16440452b3f361ed86c6fa62175173fb750
2 : fa7c67600c14d4aa350a9674688f1429577954f4a6c5e4639d06c8964824f647
3 : 3a91d1527e308e5603dafde7ab17824f441a73a779d2571d073466dc9e8451b2
4 : 30fd0e5527b1522e7b26a4818b9edac80fe47c0c39fc34705478a49e684708d0
5 : 24c5372b38c78cbaf5b0b305925502a491bc0c1b5758f50c0bd335abb6ae85f5
6 : be70e125a5793efc5e32051fecba0668df971bdf371138c8261201c2a46b2d38
7 : 41ebf52c847a59ba0aeb4425c74e89a01e91defa86a82785ff53ed4668054561
8 : dc8211b4ce122f87692e7c203672e3eb1ffc44c0a307eafcc560323fcc5fae78
9 : 59e2d8e11cad287eacf3207e64a373f65059286b803ef0981510193ae29cbc8c
---------------------------------------------------------------
```

## Pesquisando Fundos

Da mesma forma, podemos recuperar as informações da nossa carteira com o RPC `getwalletinfo`:

```py
wallet_info = rpc_client.getwalletinfo()
print("---------------------------------------------------------------")
print("Wallet Info:")
print("-----------")
pprint(wallet_info)
print("---------------------------------------------------------------\n")
```

Devemos obter um retorno semelhante ao que tivemos abaixo com o `logging` desabilitado:

```sh
---------------------------------------------------------------
Wallet Info:
-----------
{'avoid_reuse': False,
 'balance': Decimal('0.07160443'),
 'hdseedid': '6dko666b1cc0d69b7eb0539l89eba7b6390kdj02',
 'immature_balance': Decimal('0E-8'),
 'keypoololdest': 1542245729,
 'keypoolsize': 999,
 'keypoolsize_hd_internal': 1000,
 'paytxfee': Decimal('0E-8'),
 'private_keys_enabled': True,
 'scanning': False,
 'txcount': 9,
 'unconfirmed_balance': Decimal('0E-8'),
 'walletname': '',
 'walletversion': 169900}
---------------------------------------------------------------
```
Outros comandos informativos como `getblockchaininfo`, `getnetworkinfo`, `getpeerinfo` e `getblockchaininfo` funcionarão de forma semelhante.

Outros comandos podem fornecer informações específicas sobre elementos selecionados na sua carteira.

### Recuperando um Array

O RPC `listtransactions` permite que observemos as 10 transações mais recentes do nosso sistema (ou algum conjunto arbitrário de transações usando os argumentos `count` e `skip`). Ele mostra como um comando RPC pode retornar uma matriz simples de ser manipulada:

```py
tx_list = rpc_client.listtransactions()
pprint(tx_list)
```

### Explorando um UTXO

Da mesma forma, podemos usar o `listunspent` para obter uma matriz de UTXOs:

```py
print("Exploring UTXOs")
## List UTXOs
utxos = rpc_client.listunspent()
print("Utxos: ")
print("-----")
pprint(utxos)
print("------------------------------------------\n")
```

Para manipular uma matriz como a retornada de `listtransactions` ou` listunpsent`, você apenas pega o item apropriado do elemento apropriado da matriz:
```
## Select a UTXO - first one selected here
utxo_txid = utxos[0]['txid']
```

Para o `listunspent`, obtemos um `txid`. Podemos recuperar as informações sobre ele com o comando `gettransaction` e, em seguida, decodificá-lo com um `decoderawtransaction`:


```
utxo_hex = rpc_client.gettransaction(utxo_txid)['hex']

utxo_tx_details = rpc_client.decoderawtransaction(utxo_hex)

print("Details of Utxo with txid:", utxo_txid)
print("---------------------------------------------------------------")
print("UTXO Details:")
print("------------")
pprint(utxo_tx_details)
print("---------------------------------------------------------------\n")
```

Este código está disponível no arquivo [walletinfo.py](../src/18_4_walletinfo.py).
```
$ python3 walletinfo.py 
---------------------------------------------------------------
Wallet Info:
-----------
{'avoid_reuse': False,
 'balance': Decimal('0.01031734'),
 'hdseedid': 'da5a1b058deb9e51ecffef1b0ddc069a5dfb2c5f',
 'immature_balance': Decimal('0E-8'),
 'keypoololdest': 1596567843,
 'keypoolsize': 1000,
 'keypoolsize_hd_internal': 999,
 'paytxfee': Decimal('0E-8'),
 'private_keys_enabled': True,
 'scanning': False,
 'txcount': 6,
 'unconfirmed_balance': Decimal('0E-8'),
 'walletname': '',
 'walletversion': 169900}
---------------------------------------------------------------

Utxos: 
-----
[{'address': 'mv9cjEnS2o1EygBMdrz99LzhM8KeEMoXDg',
  'amount': Decimal('0.00001000'),
  'confirmations': 1180,
  'desc': "pkh([ce0c7e14/0'/0'/25']02d0541b9211aecd25913f7fdecfc1b469215fa326d52067b1b3f7efbd12316472)#n06pq9q5",
  'label': '-addresstype',
  'safe': True,
  'scriptPubKey': '76a914a080d1a10f5e7a02d0a291f118982ed19e8cfcd788ac',
  'solvable': True,
  'spendable': True,
  'txid': '84207ffec658ae29ad1fdd330d8a13613303c3cf281ce628fadeb7636ffb535e',
  'vout': 0},
 {'address': 'tb1qrcf8c29966tvqxhwrtd2se3rj6jeqtll3r46a4',
  'amount': Decimal('0.01029734'),
  'confirmations': 1180,
  'desc': "wpkh([ce0c7e14/0'/1'/26']02c581259ba7e6aef6d7ea23adb08f7c7f10c4c678f2e097a4074639e7685d4805)#j3pctfhf",
  'safe': True,
  'scriptPubKey': '00141e127c28a5d696c01aee1adaa8662396a5902fff',
  'solvable': True,
  'spendable': True,
  'txid': '84207ffec658ae29ad1fdd330d8a13613303c3cf281ce628fadeb7636ffb535e',
  'vout': 1},
 {'address': 'mzDxbtYY3LBBBJ6HhaBAtnHv6c51BRBTLE',
  'amount': Decimal('0.00001000'),
  'confirmations': 1181,
  'desc': "pkh([ce0c7e14/0'/0'/23']0377bdd176f985b4af2f6bdbb22c2925b6007b6c07ba171f75e65990c002615e98)#3y6ef6vu",
  'label': '-addresstype',
  'safe': True,
  'scriptPubKey': '76a914cd339342b06042bb986a45e73d56db46acc1e01488ac',
  'solvable': True,
  'spendable': True,
  'txid': '1679bee019c61608340b79810377be2798efd4d2ec3ace0f00a1967af70666b9',
  'vout': 1}]
------------------------------------------

Details of Utxo with txid: 84207ffec658ae29ad1fdd330d8a13613303c3cf281ce628fadeb7636ffb535e
---------------------------------------------------------------
UTXO Details:
------------
{'hash': '0c6c27f58f122329bbc53a91f290b35ce23bd2708706b21a04cdc387dc8e2fd9',
 'locktime': 1831103,
 'size': 225,
 'txid': '84207ffec658ae29ad1fdd330d8a13613303c3cf281ce628fadeb7636ffb535e',
 'version': 2,
 'vin': [{'scriptSig': {'asm': '', 'hex': ''},
          'sequence': 4294967294,
          'txid': '1679bee019c61608340b79810377be2798efd4d2ec3ace0f00a1967af70666b9',
          'txinwitness': ['3044022014b3e2359fb46d8cbc4cd30fa991b455edfa4b419a4c64a53fcdfc79e3ca89db022010cefc3268bc252d55f1982c426328b709b47d02332def9e2efb3b12de2cf0d301',
                          '0351b470e87b44e8e9607acf09b8d4543c51c93c17dc741176319e60202091f2be'],
          'vout': 0}],
 'vout': [{'n': 0,
           'scriptPubKey': {'addresses': ['mv9cjEnS2o1EygBMdrz99LzhM8KeEMoXDg'],
                            'asm': 'OP_DUP OP_HASH160 '
                                   'a080d1a10f5e7a02d0a291f118982ed19e8cfcd7 '
                                   'OP_EQUALVERIFY OP_CHECKSIG',
                            'hex': '76a914a080d1a10f5e7a02d0a291f118982ed19e8cfcd788ac',
                            'reqSigs': 1,
                            'type': 'pubkeyhash'},
           'value': Decimal('0.00001000')},
          {'n': 1,
           'scriptPubKey': {'addresses': ['tb1qrcf8c29966tvqxhwrtd2se3rj6jeqtll3r46a4'],
                            'asm': '0 1e127c28a5d696c01aee1adaa8662396a5902fff',
                            'hex': '00141e127c28a5d696c01aee1adaa8662396a5902fff',
                            'reqSigs': 1,
                            'type': 'witness_v0_keyhash'},
           'value': Decimal('0.01029734')}],
 'vsize': 144,
 'weight': 573}
---------------------------------------------------------------
```

## Crindo um Endereço

Criar um novo endereço com Python 3 requer apenas o uso de um RPC como `getnewaddress` ou `getrawchangeaddress`.
```
new_address = rpc_client.getnewaddress("Learning-Bitcoin-from-the-Command-Line")
new_change_address = rpc_client.getrawchangeaddress()
```
Neste exemplo, usamos o comando `getnewaddress` junto com um argumento: o rótulo `Learning-Bitcoin-from-the-Command-Line`.

## Enviando uma Transação

A criação de uma transação no Python 3 requer a combinação de alguns dos exemplos anteriores, de criação de endereços e de recuperação de UTXOs, com alguns comandos novos do RPC para criar, assinar e enviar uma transação, da mesma forma que fizemos anteriormente na linha de comando.

Existem cinco etapas:

0. Criar dois endereços, um que funcionará como destinatário e outro para o troco;
1. Selecionar um UTXO e definir os detalhes da transação;
2. Criar uma transação bruta;
3. Assinar a transação bruta com a chave privada do UTXO;
4. Transmitir a transação na Testnet do bitcoin.

### 1. Selecionando o UTXO e Definindo os Detalhes da Transação

No trecho de código a seguir, primeiro selecionaremos o UTXO que gostaríamos de gastar. Em seguida, iremos obter o endereço, id da transação e o índice vetorial da saída.

```py
utxos = rpc_client.listunspent()
selected_utxo = utxos[0]  # novamente, selecionando o primeiro utxo aqui
utxo_address = selected_utxo['address']
utxo_txid = selected_utxo['txid']
utxo_vout = selected_utxo['vout']
utxo_amt = float(selected_utxo['amount'])
```
Em seguida, também recuperamos o endereço do destinatário para o qual desejamos enviar os bitcoins, calculamos a quantidade de bitcoins que desejamos enviar e calculamos a taxa de minerador e o valor do troco. Aqui, o valor é dividido arbitrariamente em dois e uma taxa de minerador é definida arbitrariamente.

```py
recipient_address = new_address
recipient_amt = utxo_amt / 2  # enviando metade das moedas para o recebedor
miner_fee = 0.00000300        # escolha uma taxa apropriada baseada no tamanho da sua transação
change_address = new_change_address
change_amt = float('%.8f'%((utxo_amt - recipient_amt) - miner_fee))
```

> :warning: **AVISO:** Obviamente, um programa real faria escolhas mais sofisticadas sobre qual UTXO usar, o que fazer com os fundos e qual taxa de mineração pagar.

### 2. Criando uma Transação Bruta

Agora temos todas as informações para enviar uma transação, mas antes de enviá-la, devemos criá-la.

```py
txids_vouts = [{"txid": utxo_txid, "vout": utxo_vout}]
addresses_amts = {f"{recipient_address}": recipient_amt, f"{change_address}": change_amt}
unsigned_tx_hex = rpc_client.createrawtransaction(txids_vouts, addresses_amts)
```
Lembre-se de que o formato do comando `createrawtransaction` é:

```$ bitcoin-cli createrawtransaction '[{"txid": <utxo_txid>, "vout": <vector_id>}]' '{"<address>": <amount>}'```

O `txids_vouts` é, portanto, uma lista, e o `address_amts` é um dicionário python, para combinar com o formato de `createrawtransaction`.

Se quisermos ver mais sobre os detalhes da transação que criamos, podemos usar `decoderawtransaction`, tanto no Python 3 quanto com `bitcoin-cli`.

### 3. Assinar Transação Bruta

Assinar uma transação geralmente é a parte mais complicada do envio de uma transação, quando olhamos a parte de programação. Aqui recuperamos uma chave privada de um endereço com `dumpprivkey` e a colocamos em uma matriz:
```py
address_priv_key = []  # list of priv keys of each utxo
address_priv_key.append(rpc_client.dumpprivkey(utxo_address))
```
Depois, usamos esse array, que deve conter as chaves privadas de cada UTXO que está sendo gasto, para assinar nosso `unsigned_tx_hex`:
```py
signed_tx = rpc_client.signrawtransactionwithkey(unsigned_tx_hex, address_priv_key)
```
Isso retornará um objeto JSON com o hex da transação assinada e se foi assinado completamente ou não:

### 4. Transmitindo a Transação

Finalmente, estamos prontos para transmitir a transação assinada na rede Bitcoin:

```py
send_tx = rpc_client.sendrawtransaction(signed_tx['hex'])
```
### Executando Nosso Código

[Este código](../src/18_4_sendtx.py) está cheio de instruções com `print` para demonstrar todos os dados disponíveis a cada momento:
```
$ python3 sendtx.py 
Creating a Transaction
---------------------------------------------------------------
Transaction Details:
-------------------
UTXO Address.......:  mv9cjEnS2o1EygBMdrz99LzhM8KeEMoXDg
UTXO Txid..........:  84207ffec658ae29ad1fdd330d8a13613303c3cf281ce628fadeb7636ffb535e
Vector ID of Output:  0
UTXO Amount........:  1e-05
Tx Amount..........:  5e-06
Recipient Address..:  tb1qca0elxxqzw5xc0s3yq5qhapzzj90ka0zartu6y
Change Address.....:  tb1qrveukqrvqm9h6fua99xvcxgnvdx507dg8e8hrt
Miner Fee..........:  3e-06
Change Amount......:  2e-06
---------------------------------------------------------------

---------------------------------------------------------------
Unsigned Transaction Hex:  02000000015e53fb6f63b7defa28e61c28cfc3033361138a0d33dd1fad29ae58c6fe7f20840000000000ffffffff02f401000000000000160014c75f9f98c013a86c3e1120280bf422148afb75e2c8000000000000001600141b33cb006c06cb7d279d294ccc1913634d47f9a800000000
---------------------------------------------------------------

---------------------------------------------------------------
Signed Transaction: 
----------------------
{'complete': True,
 'hex': '02000000015e53fb6f63b7defa28e61c28cfc3033361138a0d33dd1fad29ae58c6fe7f2084000000006a47304402205da9b2234ea057c9ef3b7794958db6c650c72dedff1a90d2915147a5f6413f2802203756552aba0dd8ebd71b0f28341becc01b28d8b28af063d7c8ce89f9c69167f8012102d0541b9211aecd25913f7fdecfc1b469215fa326d52067b1b3f7efbd12316472ffffffff02f401000000000000160014c75f9f98c013a86c3e1120280bf422148afb75e2c8000000000000001600141b33cb006c06cb7d279d294ccc1913634d47f9a800000000'}
---------------------------------------------------------------

---------------------------------------------------------------
TXID of sent transaction:  187f8baa222f9f37841d966b6bad59b8131cfacca861cbe9bfc8656bd16a44cc
```

## Resumo: Acessando o Bitcoind com Python

Acessar Bitcoind com Python é muito fácil usando a biblioteca `python-bitcoinrpc`. A primeira coisa a se fazer é estabelecer uma conexão com nossa instância `bitcoind`, então podemos fazer todas as chamadas da API do Bitcoin conforme descrito nos documentos do Bitcoin Core. Isso torna mais fácil criar programas pequenos ou grandes para gerenciar nosso próprio node, verificando saldos ou criando aplicações interessantes, conforme acessamos todo o poder do `bitcoin-cli`.

## O Que Vem Depois?

Vamos aprender mais sobre "Conversando com o Bitcoind com Outras Linguagens" na seção [§18.5: Acessando o Bitcoind com Rust](18_5_Accessing_Bitcoind_with_Rust.md).

## Variante: Construindo Python da Fonte

Se precisarmos instalar o Python 3 a partir do código-fonte, precisaremos seguir estas instruções, e então continuar com ["Criando um projeto BitcoinRPC"](18_4_Accessing_Bitcoind_with_Python.md#creating-a-bitcoinrpc-project).

### 1. Instalando as Dependências
```sh
$ sudo apt-get install build-essential checkinstall
$ sudo apt-get install libreadline-gplv2-dev libncursesw5-dev libssl-dev libsqlite3-dev tk-dev libgdbm-dev libc6-dev libbz2-dev libffi-dev zlib1g-dev
```

### 2. Baixando e Extraindo o Python
```sh
$ wget https://www.python.org/ftp/python/3.8.3/Python-3.8.3.tgz
$ tar -xzf Python-3.8.3.tgz
```

### 3. Compilando o Código-Fonte Python e Verificando a Instalação:

```sh
$ cd Python-3.8.3
$ sudo ./configure --enable-optimizations
$ sudo make -j 8  # coloque o número de núcleos que você quer usar do seu sistema para acelerar o processo
$ sudo make altinstall
$ python3.8 --version
```

Depois de obter o retorno da versão, podemos remover o arquivo de instalação:
```sh
$ rm Python-3.8.3.tgz
```