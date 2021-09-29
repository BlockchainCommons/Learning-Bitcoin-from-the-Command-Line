# 18.3: Acessando o Bitcoind com NodeJS

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho inicial que ainda pode estar aguardando revisão.

Esta seção explica como interagir com o `bitcoind` usando a linguagem de programação NodeJS e o [pacote BCRPC](https://github.com/dgarage/bcrpc).

## Configurando o Node.js

O BCRPC é construído em node.js. Portanto, primeiro precisamos instalar os pacotes `node.js` e o `npm` (o gerenciador de pacotes do node) em nosso sistema.

Se estiver usando uma máquina Ubuntu, podemos executar os seguintes comandos para obter uma nova versão do `node.js`, ao invés da versão terrivelmente desatualizada no sistema de pacotes do Ubuntu.

```
$ curl -sL https://deb.nodesource.com/setup_14.x | sudo bash -
$ sudo apt-get install -y nodejs
$ sudo npm install mocha -g
```

### Configurando o BCRPC

Agora podemos clonar o pacote BCRPC do GitHub e instalar as dependências.

```
$ git clone https://github.com/dgarage/bcrpc.git
$ cd bcrpc
$ npm install
```

Para testar o pacote BCRPC, devemos primeiro definir as variáveis de ambiente para o rpcuser e rpcpassword. Como de costume, elas vêm do `~/.bitcoin/bitcoin.conf`. Também devemos definir a porta RPC como sendo 18332, que deve ser a correta para a configuração Testnet padrão descrita nos documentos.

```
$ export BITCOIND_USER=StandUp
$ export BITCOIND_PASS=d8340efbcd34e312044c8431c59c792c
$ export BITCOIND_PORT=18332
```

> :warning: **AVISO:** Obviamente, você nunca colocaria sua senha em uma variável de ambiente em um ambiente de produção.

> :link: **MAINNET VS TESTNET:** A porta seria 8332 para uma configuração na Mainnet.

Agora podemos verificar se tudo está funcionando corretamente:

```
$ npm test

> bcrpc@0.2.2 test /home/user1/bcrpc
> mocha tests.js

  BitcoinD
    ✓ is running

  bcrpc
    ✓ can get info

  2 passing (36ms)
```
Parabéns, agora temos um wrapper RPC pronto para ser usado no Bitcoin com o Node.js e que está funcionando com as nossas configurações do Bitcoin.

### Criando um Projeto BCRPC

Agora podemos criar um novo projeto Node.js e instalar o BCRPC via npm.

```
$ cd ~
$ mkdir myproject
$ cd myproject
$ npm init
  [continue with default options]
$ npm install bcrpc
```
## Construindo Nossa Conexão

Em nosso diretório `myproject`, criamos um arquivo `.js` onde nosso código JavaScript será executado.

Podemos iniciar uma conexão RPC criando um `RpcAgent`:
```
const RpcAgent = require('bcrpc');
agent = new RpcAgent({port: 18332, user: 'StandUp', pass: 'd8340efbcd34e312044c8431c59c792c'});
```
Obviamente, nosso `user` e `pass` devem coincidir novamente com o que está em nosso `~/.bitcoin/bitcoin.conf`, e usamos a `port 18332` se estivermos na Testnet.

### Fazendo uma chamada RPC

Usando o BCRPC, podemos usar os mesmos comandos RPC que normalmente usaríamos via `bitcoin-cli` com nosso `RpcAgent`, exceto que eles precisam estar em camelCase. Por exemplo, `getblockhash` seria `getBlockHash`.

Para imprimir o número do bloco mais recente, basta chamar o `getBlockCount` através do nosso `RpcAgent`:

```
agent.getBlockCount(function (err, blockCount) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(blockCount.result);
});
```

### Fazendo uma chamada RPC com argumentos

As funções BCRPC podem aceitar argumentos. Por exemplo, o `getBlockHash` recebe o `blockCount.result` como uma entrada.

```  
  agent.getBlockHash(blockCount.result, function (err, hash) {
    if (err)
      throw Error(JSON.stringify(err));
    console.log(hash.result);
  })
```

O resultado das funções BCRPC é um objeto JSON contendo informações sobre quaisquer erros e o id da solicitação. Ao acessar nosso resultado, adicionamos o `.result` no final dele para especificar que estamos interessados no resultado real, não em informações sobre os erros.

### Executando Nosso Código

Podemos encontrar o código `getinfo` no [diretório src/](../src/18_3_getinfo.js).
```
$ node getinfo.js
1831094
00000000000002bf8b522a830180ad3a93b8eed33121f54b3842d8838580a53c
```
Isso é com o que a saída do exemplo acima se pareceria se substituíssemos o `console.log(blockCount.result);` e o `console.log(hash.result);` por `console.log(blockCount);` e `console.log (hash);`, respectivamente:

```
{ result: 1774686, error: null, id: null }
{
  result: '00000000000000d980c495a2b7addf09bb0a9c78b5b199c8e965ee54753fa5da',
  error: null,
  id: null
}
```

## Pesquisando por Fundos

É útil, ao aceitar Bitcoin, verificar o Bitcoin recebido em um endereço específico em nossa carteira. Por exemplo, se administrássemos uma loja online que aceita Bitcoin, para cada pagamento de um cliente, geraríamos um novo endereço, mostraríamos esse endereço ao cliente e, em seguida, verificaríamos o saldo do endereço após algum tempo, para certificar-se de que o montante foi recebido:

```
agent.getReceivedByAddress('mpGpCMX6SuUimDZKiVViuhd7EGyVxkNnha', function (err, addressInfo) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(addressInfo.result);
});
```

> :information_source: **NOTA:** Obviamente, precisaremos inserir um endereço reconhecido por nossa máquina.

Por padrão, esta função verifica as transações que foram confirmadas uma vez, no entanto, podemos aumentar para um número maior, como 6:

```
agent.getReceivedByAddress('mpGpCMX6SuUimDZKiVViuhd7EGyVxkNnha', 6, function (err, addressInfo) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(addressInfo.result);
});
```

### Pesquisando Informações da Carteira

Também podemos procurar informações adicionais sobre nossa carteira e visualizar nosso saldo, contagem de transações etc.

```
agent.getWalletInfo(function (err, walletInfo) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(walletInfo.result);
});
```

O código está disponível como [walletinfo.js](../src/18_3_walletinfo.js).
```
$ node walletinfo.js
0.008498
{
  walletname: '',
  walletversion: 169900,
  balance: 0.010438,
  unconfirmed_balance: 0,
  immature_balance: 0,
  txcount: 4,
  keypoololdest: 1596567843,
  keypoolsize: 999,
  hdseedid: 'da5a1b058deb9e51ecffef1b0ddc069a5dfb2c5f',
  keypoolsize_hd_internal: 1000,
  paytxfee: 0,
  private_keys_enabled: true,
  avoid_reuse: false,
  scanning: false
}
```
Ao invés de imprimirmos todos os detalhes associados à nossa carteira, podemos imprimir informações específicas, como nosso saldo. Como um objeto JSON está sendo acessado, podemos fazer isso alterando a linha `console.log(walletInfo.result);` para `console.log(walletInfo.result.balance);`:

## Criando um Endereço

Também podemos passar argumentos adicionais para os comandos RPC. Por exemplo, o seguinte gera um novo endereço legado, com o sinalizador `-addresstype`.

```
agent.getNewAddress('-addresstype', 'legacy', function (err, newAddress) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(newAddress.result);
});
```
Isso é o mesmo que executar o seguinte na linha de comando:
```
$ bitcoin-cli getnewaddress -addresstype legacy
mtGPcBvRPZFEHo2YX8un9qqPBydhG82uuZ
```

No BCRPC, geralmente podemos usar os mesmos sinalizadores que no `bitcoin-cli` no BCRPC. Embora usamos o camelCase (`getNewAddress`) para os métodos, os sinalizadores, que normalmente são separados por espaços na linha de comando, são colocados em strings e separados por vírgulas.

## Enviando uma Transação

Podemos enviar saldos para um endereço muito facilmente, usando a função `sendToAddress`:

```
agent.sendToAddress(newAddress.result, 0.00001, function(err, txid) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(txid.result);
});
```

Isso deve retornar o txid da transação:

```
1679bee019c61608340b79810377be2798efd4d2ec3ace0f00a1967af70666b9
```

### Pesquisando uma Transação

Agora podemos desejar visualizar uma transação, como a que acabamos de enviar.
```
agent.getTransaction(txid.result, function (err, transaction) {
  if (err)
    throw Error(JSON.stringify(err));
  console.log(transaction.result);
});
```

Devemos obter uma saída semelhante a esta:

```
{
  amount: 0.001,
  confirmations: 4776,
  blockhash: '000000006628870b0a8a66abea9cf0d4e815c491f079e3fa9e658a87b5dc863a',
  blockindex: 117,
  blocktime: 1591857418,
  txid: '1661ce322c128e053b8ea8fcc22d17df680d2052983980e2281d692b9b4ab7df',
  walletconflicts: [],
  time: 1591857343,
  timereceived: 1591857343,
  'bip125-replaceable': 'no',
  details: [
    {
      address: 'mpGpCMX6SuUimDZKiVViuhd7EGyVxkNnha',
      category: 'receive',
      amount: 0.001,
      label: '',
      vout: 0
    }
  ],
  hex: '02000000000101e9e8c3bd057d54e73baadc60c166860163b0e7aa60cab33a03e89fb44321f8d5010000001716001435c2aa3fc09ea53c3e23925c5b2e93b9119b2568feffffff02a0860100000000001976a914600c8c6a4abb0a502ea4de01681fe4fa1ca7800688ac65ec1c000000000017a91425b920efb2fde1a0277d3df11d0fd7249e17cf8587024730440220403a863d312946aae3f3ef0a57206197bc67f71536fb5f4b9ca71a7e226b6dc50220329646cf786cfef79d60de3ef54f702ab1073694022f0618731902d926918c3e012103e6feac9d7a8ad1ac6b36fb4c91c1c9f7fff1e7f63f0340e5253a0e4478b7b13f41fd1a00'
}
```

O código completo está disponível no [sendtx.js](../src/18_3_sendtx.js).

## Resumo: Acessando o Bitcoind com NodeJS

Com o BCRPC podemos acessar todos os comandos RPC disponíveis através do `bitcoin-cli`, usando o JavaScript. O [BCRPC README](https://github.com/dgarage/bcrpc) tem alguns exemplos que usam promises (os exemplos neste livro usam callbacks). O [JavaScript por trás dele](https://github.com/dgarage/bcrpc/blob/master/index.js) é curto e bem legível.

Com base nesses exemplos, devemos ser capazes de incorporar Bitcoin em um projeto Node.js e fazer coisas como enviar e receber fundos.

## O Que Vem Depois?

Vamos aprender mais sobre "Conversando com o Bitcoind com Outras Linguagens" na seção [§18.4: Acessando o Bitcoind com Python](18_4_Accessing_Bitcoind_with_Python.md).