# 3.3: Configurando sua carteira

Agora você está pronto para começar a trabalhar com Bitcoin. Para começar, você precisará criar um endereço para receber fundos.

## Crie um endereço

A primeira coisa que você precisa fazer é criar um endereço para recebimento de pagamentos. Isso é feito com o comando `bitcoin-cli getnewaddress`. Lembre-se que se você quiser mais informações sobre este comando, deve digitar `bitcoin-cli help getnewaddress`. Atualmente, existem três tipos de endereços: `legacy` e os dois tipos de endereço SegWit,` p2sh-segwit` e `bech32`. Se você não especificar de outra forma, você obterá o padrão, que atualmente é `bech32`.

No entanto, para as próximas seções, em vez disso, usaremos endereços `legacy`, tanto porque `bitcoin-cli` teve alguns problemas iniciais com suas versões anteriores de endereços SegWit, e porque outras pessoas podem não ser capazes de enviar para endereços `bech32`. É improvável que tudo isso seja um problema para você agora, mas no momento queremos começar com exemplos de transações que (na maioria) têm garantia de funcionamento.

Você pode exigir o endereço `legacy` como segundo argumento para `getnewaddress` ou com o argumento denominado `addresstype`.

```
$ bitcoin-cli getnewaddress -addresstype legacy
moKVV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B
```

Observe que este endereço começa com um "m" (ou às vezes um "n") para significar um endereço legacy testnet. Seria um "2" para um endereço P2SH ou um "tb1" para um endereço Bech32.

> :link: **TESTNET vs MAINNET:** O endereço mainnet equivalente começaria com "1" (para Legacy), "3" (para P2SH) ou "bc1" (para Bech32).

Anote cuidadosamente o endereço. Você precisará entregá-lo a quem enviará os fundos.

> :book: ***O que é um endereço Bitcoin?*** Um endereço Bitcoin é literalmente onde você recebe dinheiro. É como um endereço de e-mail, mas para fundos. Tecnicamente, é uma chave pública. No entanto, ao contrário de um endereço de e-mail, um endereço de Bitcoin deve ser considerado de uso único: use-o para receber fundos apenas _uma vez_. Quando você quiser receber fundos de outra pessoa ou em algum outro momento, gere um novo endereço. Isso é sugerido em grande parte para melhorar sua privacidade. Todo o blockchain é imutável, o que significa que os exploradores podem observar longas cadeias de transações ao longo do tempo, tornando possível determinar estatisticamente quem é você e quem são seus contatos, não importa o quão cuidadoso você seja. No entanto, se você continuar reutilizando o mesmo endereço, isso se tornará ainda mais fácil.

> :book: ***O que é uma carteira Bitcoin?*** Ao criar seu primeiro endereço Bitcoin, você também começou a preencher sua carteira Bitcoin. Mais precisamente, você começou a preencher o arquivo `wallet.dat` em seu diretório `~/.bitcoin/testnet3/wallets`. O arquivo `wallet.dat` contém dados sobre preferências e transações, mas mais importante, contém todos os pares de chaves que você criou: a chave pública (que é a fonte do endereço onde você recebe fundos) e a chave privada (que é como você gasta esses fundos). Na maior parte, você não terá que se preocupar com a chave privada: `bitcoind` irá usá-la quando for necessário. No entanto, isso torna o arquivo `wallet.dat` extremamente importante: se você o perder, perderá suas chaves privadas e, se perder suas chaves privadas, perderá seus fundos!

Com um único endereço em mãos, você pode pular direto para a próxima seção e começar a receber fundos. No entanto, antes de chegarmos lá, vamos discutir brevemente os outros tipos de endereços que você encontrará no futuro e falar sobre alguns outros comandos de carteira que você pode querer usar no futuro.

### Conhecendo seus endereços de Bitcoin

Existem três tipos de endereços Bitcoin que você pode criar com o comando RPC `getnewaddress`. Você usará um endereço `legacy` (P2PKH) aqui, enquanto se moverá para um endereço SegWit (P2SH-SegWit) ou Bech32 em [4.6: Criação de uma transação Segwit](04_6_Creating_a_Segwit_Transaction.md).

Conforme observado acima, a base de um endereço de Bitcoin é uma chave pública: alguém envia fundos para sua chave pública e você usa sua chave privada para resgatá-la. Fácil? Exceto que colocar sua chave pública lá não é totalmente seguro. No momento, se alguém tiver sua chave pública, não poderá recuperar sua chave privada (e, portanto, seus fundos); essa é a base da criptografia, que usa uma função de trapdoor para garantir que você só possa passar da chave privada para a pública, e não vice-versa. Mas o problema é que não sabemos o que o futuro pode trazer. Exceto que sabemos que os sistemas de criptografia eventualmente são quebrados pelo avanço implacável da tecnologia, então é melhor não colocar chaves públicas brutas na rede, para preparar suas transações para o futuro.

As transações clássicas de Bitcoin criaram endereços P2PKH que adicionaram uma etapa criptográfica adicional para proteger as chaves públicas.

> :book: ***O que é um endereço legacy (P2PKH)?*** Este é um endereço legado do tipo usado pela antiga rede Bitcoin. Iremos usá-lo em exemplos nas próximas seções. É chamado de endereço Pay to PubKey Hash (ou P2PKH) porque o endereço é um hash de 160 bits de uma chave pública. Usar um hash de sua chave pública como seu endereço cria um processo de duas etapas onde gastar os fundos você precisa revelar a chave privada e a chave pública, e aumenta a segurança futura de acordo. Esse tipo de endereço continua sendo importante para receber fundos de pessoas com software de carteira desatualizado.

Conforme descrito mais detalhadamente em [4.6: Criação de uma transação Segwit](04_6_Creating_a_Segwit_Transaction.md), a Block-Size Wars do final dos anos 10 do Bitcoin resultaram em um novo tipo de endereço: SegWit. Este é o tipo de endereço preferido atualmente e deve estar totalmente integrado ao Bitcoin-Core neste exato momento.

SegWit significa simplesmente "testemunha segregada" e é uma maneira de separar as assinaturas da transação do resto da transação para reduzir o tamanho da transação. Alguns endereços SegWit entrarão em alguns de nossos exemplos anteriores a 4.6 como endereços de troco, que você verá como endereços que começam com "tb". Isso é bom porque o `bitcoin-cli` suporta inteiramente seu uso. 

Existem dois endereços desse tipo:

> :book: ***O que é um endereço P2SH-SegWit (também conhecido como Nested SegWit)?*** Esta é a primeira geração do SegWit. Ele envolve o endereço SegWit em um hash de script para garantir a compatibilidade com versões anteriores. O resultado cria transações que são cerca de 25% + menores (com reduções correspondentes nas taxas de transação).

> :book: ***O que é um endereço Bech32 (também conhecido como SegWit nativo, também conhecido como P2WPKH)?*** Esta é a segunda geração do SegWit. Está totalmente descrito em [BIP 173] (https://en.bitcoin.it/wiki/BIP_0173). Ele cria transações que são ainda menores, mas mais notavelmente também tem algumas vantagens na criação de endereços que são menos propensos a erro humano e têm alguma correção de erro implícita além disso. Ele * não * é compatível com versões anteriores como o P2SH-SegWit era e, portanto, algumas pessoas podem não ser capazes de enviar para ele.

Existem outros tipos de endereços de Bitcoin, como P2PK (que paga a uma chave pública simples e está obsoleto devido à sua insegurança futura) e P2SH (que paga a um Hash de script e que é usado pelo SegWit e esta aninhado com a primeira geração endereços; vamos conhecê-lo mais detalhadamente em alguns capítulos).

## Opcional: assine uma mensagem

Às vezes, você precisará provar que controla um endereço Bitcoin (ou melhor, que controla sua chave privada). Isso é importante porque permite que as pessoas saibam que estão enviando fundos para a pessoa certa. Isso pode ser feito criando uma assinatura com o comando `bitcoin-cli signmessage`, na forma `bitcoin-cli signmessage [endereço] [mensagem]`. Por exemplo:

```
$ bitcoin-cli signmessage "moKVV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B" "Hello, World"
HyIP0nzdcH12aNbQ2s2rUxLwzG832HxiO1vt8S/jw+W4Ia29lw6hyyaqYOsliYdxne70C6SZ5Utma6QY/trHZBI=
```
Você receberá a assinatura como um retorno.

> :book: ***What is a signature?*** A digital signature is a combination of a message and a private key that can then be unlocked with a public key. Since there's a one-to-one correspendence between the elements of a keypair, unlocking with a public key proves that the signer controlled the corresponding private key.

Another person can then use the `bitcoin-cli verifymessage` command to verify the signature. He inputs the address in question, the signature, and the message:
```
$ bitcoin-cli verifymessage "moKVV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B" "HyIP0nzdcH12aNbQ2s2rUxLwzG832HxiO1vt8S/jw+W4Ia29lw6hyyaqYOsliYdxne70C6SZ5Utma6QY/trHZBI=" "Hello, World"
true
```
Se todos eles corresponderem, a outra pessoa saberá que pode transferir fundos com segurança para a pessoa que assinou a mensagem enviando para o endereço.

Se algum golpista estivesse criando assinaturas, isso produziria um invalido.
```
$ bitcoin-cli verifymessage "FAKEV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B" "HyIP0nzdcH12aNbQ2s2rUxLwzG832HxiO1vt8S/jw+W4Ia29lw6hyyaqYOsliYdxne70C6SZ5Utma6QY/trHZBI=" "Hello, World"
error code: -3
error message:
Invalid address
```

## Opcional: descarregar sua carteira

Pode parecer perigoso ter todas as suas chaves privadas insubstituíveis em um único arquivo. É para isso que serve `bitcoin-cli dumpwallet`. Ele permite que você faça uma cópia de seu wallet.dat:
```
$ bitcoin-cli dumpwallet ~/mywallet.txt
```
O arquivo `mywallet.txt` em seu diretório home terá uma longa lista de chaves privadas, endereços e outras informações. Lembre-se, você não gostaria de colocar esses dados em um arquivo de texto simples, em uma configuração com fundos reais!

Você pode então recuperá-lo com `bitcoin-cli importwallet`.
```
$ bitcoin-cli importwallet ~/mywallet.txt
```
Mas observe que isso requer um node não prunado.
```
$ bitcoin-cli importwallet ~/mywallet.txt
error code: -4
error message:
Importing wallets is disabled when blocks are pruned
```

## Opcional: Visualize suas chaves privadas

Às vezes, você pode querer realmente olhar para as chaves privadas associadas aos seus endereços Bitcoin. Talvez você queira assinar uma mensagem ou gastar bitcoins em uma máquina diferente. Talvez você só queira fazer backup de algumas chaves privadas importantes. Você também pode fazer isso com seu arquivo de descarregado, já que ele pode ser lido por humanos.
```
$ bitcoin-cli dumpwallet ~/mywallet.txt
{
  "filename": "/home/standup/mywallet.txt"
}
```
Mais provavelmente, você deseja apenas examinar a chave privada associada a um endereço específico. Isso pode ser feito com o comando `bitcoin-cli dumpprivkey`.
```
$ bitcoin-cli dumpprivkey "moKVV6XEhfrBCE3QCYq6ppT7AaMF8KsZ1B"
cTv75T4B3NsG92tdSxSfzhuaGrzrmc1rJjLKscoQZXqNRs5tpYhH
```
Você pode salvar essa chave em um local seguro, de preferência em algum lugar sem conexão com a Internet.

Você também pode importar qualquer chave privada, de um despejo de carteira ou um despejo de chave individual, da seguinte maneira:
```
$ bitcoin-cli importprivkey cW4s4MdW7BkUmqiKgYzSJdmvnzq8QDrf6gszPMC7eLmfcdoRHtHh
```
Novamente, espere que isso exija um node não prunado. Espere que isso demore um pouco, já que o `bitcoind` precisa reler todas as transações anteriores, para ver se há alguma nova.

> :information_source: **NOTA:** Muitas carteiras modernas preferem [códigos mnemônicos](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki) para gerar as sementes necessárias para criar as chaves privadas. Esta metodologia não é usada `bitcoin-cli`, então você não será capaz de gerar listas de palavras úteis para lembrar suas chaves privadas.

_Você digitou aquele endereço Bitcoin que gerou, enquanto assinava uma mensagens e agora esta descarregando as chaves. Se você acha que é muito complicado, concordamos. Também está sujeito a erros, um tópico que abordaremos na próxima seção._

## Resumo: Configurando sua carteira

Você precisa criar um endereço para receber fundos. Seu endereço é armazenado em uma carteira, da qual você pode fazer backup. Você também pode fazer muito mais com um endereço, como descartar sua chave privada ou usá-la para assinar mensagens. Mas, realmente, criar esse endereço é _tudo_ que você precisa fazer para receber os fundos.

## Mas oque vem a seguir?

Um passo para trás em "Compreendendo a configuração do seu node Bitcoin" com [Usando variáveis de linha de comandos](03_3__Interlude_Using_Command-Line_Variables.md).