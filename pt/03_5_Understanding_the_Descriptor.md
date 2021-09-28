# 3.5: Compreendendo o Descritor

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho inicial que ainda pode estar aguardando revisão. Cuidado, leitor.

Você deve ter notado o estranho campo `desc:` no comando `listunspent` da seção anterior. Aqui está o que está acontecendo (e como ele pode ser usado para transferir endereços).

> :warning: **AVISO DE VERSÃO:** Esta é uma inovação do Bitcoin Core v0.17.0 que continuou a ser expandida através do Bitcoin Core 0.20.0. A maioria dos comandos nesta seção são de 0.17.0, mas o `importmulti` atualizado que suporta descritores é de 0.18.0. 

## Entendendo Sobre a Transferência de Endereços

A maior parte deste curso presume que estejamos trabalhando inteiramente a partir de um único node onde gerenciamos nossa própria carteira, enviando e recebendo pagamentos com os endereços criados por essa carteira. No entanto, não é necessariamente assim que funciona o ecossistema Bitcoin por completo. Nesse caso, é mais provável que movamos endereços entre carteiras e até mesmo configuremos carteiras para vigiar fundos controlados por carteiras diferentes.

É aí que entram os descritores. Eles são mais úteis se estivermos interagindo com _outro_ software que não seja o Bitcoin Core, e se realmente precisamos do apoio deste tipo de função de compatibilidade: podemos consultar a seção [§6.1](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/pt/06_1_Sending_a_Transaction_to_a_Multisig.md) para um exemplo do mundo real de como usar a capacidade total dos descritores é fundamental.

Mover endereços entre carteiras costumava focar em `xpub` e `xprv`, e eles ainda possuem suporte.

> :book: ***O que é xprv?*** Uma chave privada estendida (_xtended private key_). Esta é a combinação de uma chave privada e um código de cadeia. É uma chave privada da qual uma sequência inteira de chaves privadas filhas pode ser derivada.

> :book: ***O que é xpub?*** Uma chave pública estendida (_xtended public key_). Esta é a combinação de uma chave pública e um código de cadeia. É uma chave pública da qual uma sequência inteira de chaves públicas filhas pode ser derivada.

O fato de que podemos ter uma "sequência inteira de chaves filhas" revela o fato de que "xpub" e "xprv" não são chaves padrão como falamos até agora. Em vez disso, são chaves hierárquicas que podem ser usadas para criar molhos inteiros de chaves, baseadas na ideia de carteiras HD.

> :book: ***O que é uma Carteira HD?*** A maioria das carteiras modernas é construída com base no [BIP32: Hierarchical Deterministic Wallets](https://github.com/bitcoin/bips/blob/master/bip-0032.mediawiki). Este é um design hierárquico em que uma única seed pode ser usada para gerar uma sequência completa de chaves. A carteira inteira pode então ser restaurada a partir dessa seed, em vez de exigir a restauração de cada chave privada.

> :book: ***O que é um caminho de derivação?*** Quando temos chaves hierárquicas, precisamos ser capazes de definir chaves individuais como descendentes de uma semente. Por exemplo, `[0]` é a 0ª chave, `[0/1]` é o primeiro filho da 0ª chave, `[1/0/1]` é o primeiro neto do filho zero da 1ª chave. Algumas chaves também contêm um `'` após o número, para mostrar que estão protegidas, o que as protege de um ataque específico que pode ser usado para derivar uma `xprv` de uma `xpub`. Não precisamos nos preocupar com os detalhes, a não ser o fato de que esses `'`s causarão problemas de formatação ao trabalhar na linha de comando.

> :information_source: **NOTA:** um caminho de derivação define uma chave, o que significa que uma chave representa um caminho de derivação. Eles são equivalentes. No caso de um descritor, o caminho de derivação permite ao `bitcoind` saber de onde veio a chave que segue no descritor! 

`xpubs` e `xprvs` provaram ser insuficientes quando os tipos de chaves públicas multiplicadas sob a [expansão SegWit](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/04_6_Creating_a_Segwit_Transaction.md), portanto essa é a necessidade de "descritores de saída".

> :book: ***O que é um descritor de saída?*** Uma descrição precisa de como derivar um endereço Bitcoin de uma combinação de uma função com uma ou mais entradas para essa função.

A introdução de funções nos descritores é o que os torna poderosos, porque eles podem ser usados para transferir todos os tipos de endereços, desde os endereços legados com os quais estamos trabalhando agora até os endereços Segwit e multisig que encontraremos no futuro. Uma função individual corresponde a um determinado tipo de endereço e se correlaciona com regras específicas para gerar esse endereço.

## Capturando um Descritor

Descritores são visíveis em vários comandos, como por exemplo `listunspent` e `getaddressinfo`:
```
$ bitcoin-cli getaddressinfo ms7ruzvL4atCu77n47dStMb3of6iScS8kZ
{
  "address": "ms7ruzvL4atCu77n47dStMb3of6iScS8kZ",
  "scriptPubKey": "76a9147f437379bcc66c40745edc1891ea6b3830e1975d88ac",
  "ismine": true,
  "solvable": true,
  "desc": "pkh([d6043800/0'/0'/18']03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388)#4ahsl9pk",
  "iswatchonly": false,
  "isscript": false,
  "iswitness": false,
  "pubkey": "03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388",
  "iscompressed": true,
  "ischange": false,
  "timestamp": 1592335136,
  "hdkeypath": "m/0'/0'/18'",
  "hdseedid": "fdea8e2630f00d29a9d6ff2af7bf5b358d061078",
  "hdmasterfingerprint": "d6043800",
  "labels": [
    ""
  ]
}
```
Neste caso, o descritor é `pkh([d6043800/0'/0'/18']03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388)#4ahsl9pk`.

## Compreendendo um Descritor

Um descritor é composto de várias partes:
```
função([caminho-de-derivação]chave)#checksum
```
Vamos explicar o que tudo isso significa:
* **Função.** A função que é usada para criar um endereço a partir dessa chave. Nestes casos, é `pkh`, que é o endereço padrão P2PKH legado que encontramos em [§3.3: Configurando Nossa Carteira](03_3_Setting_Up_Your_Wallet.md). Da mesma forma, um endereço P2WSH SegWit usaria `wsh` e um endereço P2WPKH usaria `wpkh`;
* **Caminho de Derivação.** Descreve qual parte de uma carteira HD está sendo exportada. Neste caso, é uma seed com a impressão digital `d6043800` e então o 18º filho do 0º filho do 0º filho (`0 '/ 0' / 18'`) dessa seed. Também pode haver uma derivação adicional após a chave: `função([caminho-de-derivação]chave/mais-derivação)#checksum`.
   * É importante notar aqui que, se algum dia recebermos um caminho de derivação sem uma impressão digital, poderemos inventá-lo. Acontece que, se houver um existente, devemos usá-lo, porque se algum dia voltarmos para o dispositivo que criou a impressão digital, precisaremos ter o mesmo;
* **Chave**. A chave ou as chaves que estão sendo transferidas. Isso pode ser algo tradicional como uma `xpub` ou `xprv`, pode ser apenas uma chave pública para um endereço, como neste caso, pode ser um conjunto de endereços para uma multi-assinatura, ou pode ser outra coisa. Estes são os dados principais: a função explica o que fazer com eles;
* **Checksum**. Os descritores são feitos para poderem ser transferidos por humanos. Esta soma de verificação (checksum) garante que o fizemos corretamente;

Podemos ver [Informações do Suporte do Bitcoin Core ao Descritor](https://github.com/bitcoin/bitcoin/blob/master/doc/descriptors.md) para maiores informações.

## Examinando um Descritor

Podemos examinar um descritor com o comando RPC `getdescriptorinfo`:
```
$ bitcoin-cli getdescriptorinfo "pkh([d6043800/0'/0'/18']03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388)#4ahsl9pk"
{
  "descriptor": "pkh([d6043800/0'/0'/18']03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388)#4ahsl9pk",
  "checksum": "4ahsl9pk",
  "isrange": false,
  "issolvable": true,
  "hasprivatekeys": false
}
```
Observe que ele retorna um checksum. Se algum dia recebermos um descritor sem um checksum, podemos encontrá-lo com este comando:
```
$ bitcoin-cli getdescriptorinfo "pkh([d6043800/0'/0'/18']03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388)"
{
  "descriptor": "pkh([d6043800/0'/0'/18']03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388)#4ahsl9pk",
  "checksum": "4ahsl9pk",
  "isrange": false,
  "issolvable": true,
  "hasprivatekeys": false
}
```
Além de nos dar o checksum, esse comando também verifica a validade do descritor e nos dá informações úteis, como se o descritor contém chaves privadas.

Um dos poderes de um descritor é a capacidade de derivar um endereço de forma padrão. Isto é feito com o RPC `deriveaddresses`.
```
$ bitcoin-cli deriveaddresses "pkh([d6043800/0'/0'/18']03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388)#4ahsl9pk"
[
  "ms7ruzvL4atCu77n47dStMb3of6iScS8kZ"
]
```
Podemos observar que ele nos retorna o endereço inicial (como deveria).

## Importando um Descritor

Mas a coisa realmente importante sobre um descritor é que podemos levá-lo a outra máquina (remota) e importá-lo. Isto é feito com o RPC `importmulti` usando a opção `desc`:
```
remote$ bitcoin-cli importmulti '[{"desc": "pkh([d6043800/0'"'"'/0'"'"'/18'"'"']03efdee34c0009fd175f3b20b5e5a5517fd5d16746f2e635b44617adafeaebc388)#4ahsl9pk", "timestamp": "now", "watchonly": true}]'
[
  {
    "success": true
  }
]
```
Primeiro, podemos notar nosso primeiro uso muito feio de aspas. Toda `'` no caminho de derivação pode ser substituído com `'"'"'`. Devemos esperar fazer isso se estivermos manipulando um descritor que contenha um caminho de derivação. (Outra opção é trocar o `'` com um `h` de "hardened", mas isto irá alterar nosso checksum, então caso tenhamos essa preferência pela facilidade de uso, precisaremos de um novo checksum com o `getdescriptorinfo`.)

Segundo, podemos notar que o marcamos como `watchonly`. Isso ocorre porque sabemos que é uma chave pública, então não podemos usá-la para gastar fundos. Se tivéssemos esquecido de colocar esta sinalização, `importmulti` teria dito algo como o seguinte: `Some private keys are missing, outputs will be considered watchonly. If this is intentional, specify the watchonly flag.`.

> :book: ***O que é um endereço watch-only?*** Um endereço watch-only nos permite atentar a transações de certo endereço (ou para toda uma família de endereços se tivermos usado uma `xpub`), mas não nos permite gastarmos fundos destes endereços.

Usando `getaddressbylabel`, podemos ver que nosso endereço foi importado corretamente na nossa máquina remota!
```
remote$ bitcoin-cli getaddressesbylabel ""
{
  "ms7ruzvL4atCu77n47dStMb3of6iScS8kZ": {
    "purpose": "receive"
  }
}
```
## Resumo: Compreendendo o Descritor

Descritores nos permitem passar chaves públicas e privadas entre carteiras, mas mais do que isso, nos permitem definir endereços de forma correta e precisa, além de derivar endereços de tipos diversos a partir de um formato descritivo e padronizado.

> :fire: ***Qual é o poder dos descritores?*** Descritores nos permitem importar e exportar chaves e seeds. O que é excelente se queremos movê-las entre carteiras diferentes. Como um desenvolvedor, eles também nos permitem construir o tipo preciso de endereços que queremos criar. Por exemplo, nós os utilizamos na [FullyNoded 2](https://github.com/BlockchainCommons/FullyNoded-2/blob/master/Docs/How-it-works.md) para gerarmos um multisig de três seeds.

Faremos uso real dos descritores na seção [§7.3](07_3_Integrating_with_Hardware_Wallets.md), quando estivermos importando endereços de uma hardware wallet.

## O Que Vem Depois?

Vamos avançar no "bitcoin-cli" com o [Capítulo Quatro: Enviando Transações no Bitcoin](04_0_Sending_Bitcoin_Transactions.md).
