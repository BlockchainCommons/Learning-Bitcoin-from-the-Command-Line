# 3.3: Configurando Nossa carteira

Agora estamos prontos para começar a brincar com o Bitcoin. Para começar, precisaremos criar um endereço para receber fundos.

## Criando um Endereço

A primeira coisa que precisamos fazer é criar um endereço para recebimento de pagamentos. Podemos fazer isso usando o comando `bitcoin-cli getnewaddress`. Temos que lembrar que se quisermos mais informações sobre este comando, podemos digitar `bitcoin-cli help getnewaddress`. Atualmente, existem três tipos de endereços: Os `legacy` e os dois tipos de endereço SegWit, `p2sh-segwit` e `bech32`. Se não especificarmos qual queremos criar, sempre teremos por padrão o `bech32`.

No entanto, nas próximas sessões, usaremos endereços `legacy`, porque o `bitcoin-cli` teve alguns problemas com suas versões anteriores usando os endereços SegWit e, porque algumas pessoas podem não ser capazes de enviar seus saldos para endereços `bech32`. É improvável que tudo isso seja um problema para nós neste exato momento, mas vamos começar com exemplos de transações que (na maioria das vezes) temos a garantia que irá funcionar.

Podemos exigir um endereço `legacy` como segundo argumento para `getnewaddress` ou com o argumento denominado `addresstype`.

```
$ bitcoin-cli getnewaddress -addresstype legacy
moKVV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B
```

Observe que este endereço começa com "m" (ou às vezes um "n") para significar um endereço legacy na rede Testnet. Seria um "2" para um endereço P2SH ou um "tb1" para um endereço Bech32.

> :link: **TESTNET vs MAINNET:** O endereço Mainnet equivalente começaria com "1" (para Legacy), "3" (para P2SH) ou "bc1" (para Bech32).

Anote cuidadosamente o endereço. Precisaremos utilizá-lo para quando recebermos bitcoins.

> :book: ***O que é um endereço Bitcoin?*** Um endereço Bitcoin é literalmente onde receberemos as moedas. É como um endereço de e-mail, mas para bitcoin. Tecnicamente, é uma chave pública. No entanto, ao contrário de um endereço de e-mail, um endereço Bitcoin deve ser considerado de uso único: Vamos usar para receber fundos apenas _uma vez_. Quando quisermos receber fundos de outra pessoa ou em algum outro momento, precisaremos gerar um novo endereço. Isso é sugerido em grande parte para melhorar nossa privacidade. Toda a blockchain é imutável, o que significa que os exploradores podem observar longas cadeias de transações ao longo do tempo, tornando possível determinar estatisticamente quem é você e quem são os nossos contatos, não importa o quão cuidadoso sejamos. No entanto, se continuarmos reutilizando o mesmo endereço, isso se tornará ainda mais fácil.

> :book: ***O que é uma carteira Bitcoin?*** Ao criar nosso primeiro endereço Bitcoin, também começamos a preencher nossa carteira Bitcoin. Mais precisamente, começamos a preencher o arquivo `wallet.dat` no nosso diretório `~/.bitcoin/testnet3/wallets`. O arquivo `wallet.dat` contém dados sobre preferências e transações, mas mais importante, contém todos os pares de chaves que criamos: A chave pública (que é a fonte do endereço onde receberemos as moedas) e a chave privada (que é como gastamos esses fundos). Na maior parte, não teremos que nos preocupar com a chave privada: O `bitcoind` irá usá-la quando for necessário. No entanto, isso torna o arquivo `wallet.dat` extremamente importante: Se o perdermos, perderemos nossas chaves privadas e, se perdermos as chaves privadas, perderemos nossos fundos!

Com um único endereço em mãos, podemos pular direto para a próxima seção e começar a receber alguns satoshinhos. No entanto, antes de chegarmos lá, vamos discutir brevemente os outros tipos de endereços que encontraremos no futuro e falar sobre alguns outros comandos de carteira que podemos querer usar mais pra frente.

### Conhecendo os Endereços do Bitcoin

Existem três tipos de endereços Bitcoin que podemos criar com o comando RPC `getnewaddress`. Usaremos um endereço `legacy` (P2PKH) aqui, mas iremos utilizar um endereço SegWit (P2SH-SegWit) ou Bech32 na sessão [4.6: Criação de uma transação Segwit](04_6_Creating_a_Segwit_Transaction.md).

Conforme observado acima, a base de um endereço de Bitcoin é uma chave pública: Alguém envia fundos para nossa chave pública e usamos a nossa chave privada para resgatá-la. Simples, não? Exceto que colocar nossa chave pública lá não é algo seguro. No momento, se alguém tiver nossa chave pública, não poderemos recuperar nossa chave privada (e, portanto, nossos fundos). Essa é a base da criptografia, que usa uma função de _trapdoor_ para garantir que só possamos passar da chave privada para a pública, e não vice-versa. Mas o problema é que não sabemos o que o futuro pode nos trazer. Exceto que sabemos que os sistemas de criptografia eventualmente são quebrados pelo avanço implacável da tecnologia, então é melhor não colocar chaves públicas brutas na rede, para prepararmos nossas transações para o futuro.

As transações clássicas do Bitcoin criaram endereços P2PKH que adicionaram uma etapa criptográfica adicional para proteger as chaves públicas.

> :book: ***O que é um endereço legacy (P2PKH)?*** Este é um endereço legado do tipo usado pela antiga rede Bitcoin. Iremos usá-lo em exemplos nas próximas seções. É chamado de endereço Pay to PubKey Hash (ou P2PKH) porque o endereço é um hash de 160 bits de uma chave pública. Usar um hash de sua chave pública como endereço cria um processo de duas etapas onde para gastar os fundos precisamos revelar a chave privada e a chave pública, aumentando assim a segurança futura. Esse tipo de endereço continua sendo importante para receber fundos de pessoas com softwares desatualizados.

Conforme descrito mais detalhadamente na sessão [4.6: Criando uma transação Segwit](04_6_Creating_a_Segwit_Transaction.md), a Guerra pelo Tamanho dos Blocos do final dos anos 10 do Bitcoin resultaram em um novo tipo de endereço: O SegWit. Este é o tipo de endereço preferido atualmente e deve estar totalmente integrado ao Bitcoin-Core neste exato momento.

O SegWit significa simplesmente "Segregated Witness" e é uma maneira de separar as assinaturas da transação do resto dela para reduzir o tamanho da mesma. Alguns endereços SegWit entrarão em alguns de nossos exemplos como endereços de troco, que veremos como endereços que começam com "tb". Isso é bom porque o `bitcoin-cli` suporta inteiramente o seu uso. 

Existem dois endereços desse tipo:

> :book: ***O que é um endereço P2SH-SegWit (também conhecido como Nested SegWit)?*** Esta é a primeira geração do SegWit. Ele envolve o endereço SegWit em um hash de script para garantir a compatibilidade com versões anteriores. O resultado cria transações que são cerca de 25% menores (com reduções correspondentes nas taxas de transação).

> :book: ***O que é um endereço Bech32 (também conhecido como SegWit nativo ou como P2WPKH)?*** Esta é a segunda geração do SegWit. Está totalmente descrito em [BIP 173](https://en.bitcoin.it/wiki/BIP_0173). Ele cria transações que são ainda menores, mas mais notavelmente também tem algumas vantagens na criação de endereços que são menos propensos a erro humano e têm algumas correções de erro implícita. Ele _não_ é compatível com versões anteriores como o P2SH-SegWit era e, portanto, algumas pessoas podem não ser capazes de enviar bitcoins para ele.

Existem outros tipos de endereços de Bitcoin, como P2PK (que paga a uma chave pública simples e está obsoleto devido à sua insegurança futura) e P2SH (que paga a um Hash de script e que é usado pelo SegWit e esta aninhado com a primeira geração endereços. Vamos conhecê-lo mais detalhadamente nos próximos capítulos).

## Opcional: Assinando uma Mensagem

Às vezes, precisamos provar que controlamos um endereço Bitcoin (ou melhor, que controlamos a chave privada). Isso é importante porque permite que as pessoas saibam que estão enviando fundos para a pessoa certa. Isso pode ser feito criando uma assinatura com o comando `bitcoin-cli signmessage`, na forma `bitcoin-cli signmessage [endereço] [mensagem]`. Por exemplo:

```
$ bitcoin-cli signmessage "moKVV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B" "Hello, World"
HyIP0nzdcH12aNbQ2s2rUxLwzG832HxiO1vt8S/jw+W4Ia29lw6hyyaqYOsliYdxne70C6SZ5Utma6QY/trHZBI=
```
O resultado é uma assinatura como um retorno.

> :book: ***O que é uma assinatura?*** Uma assinatura digital é uma combinação de uma mensagem e uma chave privada que pode ser desbloqueada com uma chave pública. Como há uma correspondência um-para-um entre os elementos de um par de chaves, o desbloqueio com uma chave pública prova que o assinante controlou a chave privada correspondente.

Outra pessoa pode usar o comando `bitcoin-cli verifymessage` para verificar a assinatura. Ela insere o endereço em questão, a assinatura e a mensagem:
```
$ bitcoin-cli verifymessage "moKVV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B" "HyIP0nzdcH12aNbQ2s2rUxLwzG832HxiO1vt8S/jw+W4Ia29lw6hyyaqYOsliYdxne70C6SZ5Utma6QY/trHZBI=" "Hello, World"
true
```
Se todos eles corresponderem, a outra pessoa saberá que pode transferir fundos com segurança para a pessoa que assinou a mensagem enviando para o endereço.

Se algum golpista estivesse criando assinaturas, isso produziria um erro.
```
$ bitcoin-cli verifymessage "FAKEV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B" "HyIP0nzdcH12aNbQ2s2rUxLwzG832HxiO1vt8S/jw+W4Ia29lw6hyyaqYOsliYdxne70C6SZ5Utma6QY/trHZBI=" "Hello, World"
error code: -3
error message:
Invalid address
```

## Opcional: Fazendo o Dump da Nossa Carteira

Pode parecer perigoso ter todas as chaves privadas insubstituíveis em um único arquivo. É para isso que serve o comando `bitcoin-cli dumpwallet`. Ele permite que façamos uma cópia do nosso arquivo `wallet.dat`:
```
$ bitcoin-cli dumpwallet ~/mywallet.txt
```
O arquivo `mywallet.txt` em nosso diretório home terá uma longa lista de chaves privadas, endereços e outras informações. Lembre-se, ninguém gostaria de colocar esses dados em um arquivo de texto simples, em uma configuração com saldos reais!

Podemos então recuperá-los com o `bitcoin-cli importwallet`.
```
$ bitcoin-cli importwallet ~/mywallet.txt
```
É importante observar que isso requer um node não prunado.
```
$ bitcoin-cli importwallet ~/mywallet.txt
error code: -4
error message:
Importing wallets is disabled when blocks are pruned
```

## Opcional: Visualizando as Chaves Privadas

Às vezes, podemos querer realmente olhar para as chaves privadas associadas aos nossos endereços Bitcoin. Talvez queremos assinar uma mensagem ou gastar bitcoins em uma máquina diferente. Talvez só estamos querendo fazer backup de algumas chaves privadas importantes. Também pode fazer isso com nosso arquivo criado acima, já que ele pode ser lido por humanos.
```
$ bitcoin-cli dumpwallet ~/mywallet.txt
{
  "filename": "/home/standup/mywallet.txt"
}
```
Mais provavelmente, desejamos apenas examinar a chave privada associada a um endereço específico. Isso pode ser feito com o comando `bitcoin-cli dumpprivkey`.
```
$ bitcoin-cli dumpprivkey "moKVV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B"
cTv75T4B3NsG92tdSxSfzhuaGrzrmc1rJjLKscoQZXqNRs5tpYhH
```
Podemos salvar essa chave em um local seguro, de preferência em algum lugar sem conexão com a Internet.

Também podemos importar qualquer chave privada, de um dump de carteira ou um dump da chave individual, da seguinte maneira:
```
$ bitcoin-cli importprivkey cW4s4MdW7BkUmqiKgYzSJdmvnzq8QDrf6gszPMC7eLmfcdoRHtHh
```
Novamente, é esperado que isso exija um node não prunado. Isso provavelmente vai demorar um pouco, já que o `bitcoind` precisa reler todas as transações anteriores, para ver se há alguma nova.

> :information_source: **NOTA:** Muitas carteiras modernas preferem [códigos mnemônicos](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki) para gerar as seeds necessárias para criar as chaves privadas. Esta metodologia não é usada pelo `bitcoin-cli`, então não seremos capazes de gerar listas de palavras para lembrar das nossas chaves privadas.

_Nós digitamos aquele endereço Bitcoin que gerou, enquanto assinávamos uma mensagem e agora estamos fazendo o dump as chaves. Se por acaso achar que isso é muito complicado, os autores e tradutores também concordam com isso. Também estamos sujeitos a erros, um tópico que abordaremos na próxima sessão._

## Resumo: Configurando Nossa carteira

Precisamos criar um endereço para receber fundos. Nosso endereço é armazenado em uma carteira, da qual podemos fazer o backup. Podemos fazer muito coisas com nosso endereço, como fazer o dump da nossa chave privada ou usá-la para assinar mensagens. Mas, realmente, criar esse endereço é _tudo_ que precisaremos fazer para receber alguns satoshinhos.

## O Que Vem Depois?

Vamos dar uma pausa no capítulo "Compreendendo Nossa Configuração do Bitcoin" com o [Prefácio: Usando Variáveis de Linha de Comando](03_3__Interlude_Using_Command-Line_Variables.md).