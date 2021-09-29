# 10.2: Construindo a Estrutura do P2SH

Na seção anterior, apresentamos uma visão geral da teoria de como criar as transações P2SH para armazenar os scripts de Bitcoin. Na prática, fazer isso é _muito mais difícil_, mas por uma questão de integridade, vamos examinar minuciosamente todos os pontos. Provavelmente, isso não é algo que faríamos sem uma API, então, se ficar muito complicado, esteja ciente de que retornaremos aos scripts originais de alto nível mais pra frente.

## Criando um Script de Bloqueio

Qualquer transação P2SH começa com um script de bloqueio. Esse é o assunto dos capítulos 9, 11 e 12. Podemos usar qualquer um dos métodos de script do Bitcoin descritos nestes capítulos para criar qualquer tipo de script de bloqueio, desde que o ```redeemScript``` serializado resultante tenha 520 bytes ou menos.

> :book: ***Por que os scripts P2SH são limitados a 520 bytes?*** Como muitas coisas no Bitcoin, a resposta é a compatibilidade com as versões anteriores: novas funcionalidades devem ser constantemente criadas dentro das antigas restrições do sistema. Nesse caso, 520 bytes é o máximo que pode ser colocado na pilha de uma vez. Como todo o redeemScript é colocado na pilha como parte do processo de resgate, ele está limitado à essa quantidade.

## Serializando um Script de Bloqueio da Maneira Difícil

Depois de criar um script de bloqueio, precisamos serializá-lo antes que possam ser inseridos no Bitcoin. Este é um processo de duas partes. Primeiro, devemos transformá-lo em um hexcode, para então transformar esse hex em binário.

### Criando o Código Hexadecimal

Criar o hexcode necessário para serializar um script é uma simples tradução, mas ao mesmo tempo, algo complexo o suficiente para ir além de qualquer script shell que provavelmente escreveremos. Esta etapa é um dos principais motivos pelos quais precisamos de uma API para criar as transações P2SH.

Podemos criar um hexcode percorrendo nosso script de bloqueio e transformando cada elemento em um comando hexadecimal de um byte, possivelmente seguido por dados adicionais. De acordo com o guia da [página Wiki do Bitcoin Script](https://en.bitcoin.it/wiki/Script):

* Os operadores são traduzidos para o byte correspondente para esse opcode;
* As constantes 1-16 são convertidas para opcodes 0x51 a 0x61 (OP_1 a OP_16);
* A constante -1 é traduzida para opcode 0x4f (OP_1NEGATE);
* Outras constantes são precedidas por opcodes 0x01 a 0x4e (OP_PUSHDATA, com o número especificando de quantos bytes adicionar);
   * Os inteiros são traduzidos em hexadecimal usando a notação de magnitude com sinal _little-endian_.

### Traduzindo os Números Inteiros

Os inteiros são a parte mais problemática de uma tradução de script de bloqueio.

Primeiro, devemos verificar se o nosso número está entre -2147483647 e 2147483647, o intervalo de inteiros de quatro bytes quando o byte mais significativo é usado para assinatura.

Em segundo lugar, precisamos traduzir o valor decimal em hexadecimal e preenchê-lo com um número par de dígitos. Isso pode ser feito com o comando ```printf```:
```
$ integer=1546288031
$ hex=$(printf '%08x\n' $integer | sed 's/^\(00\)*//')
$ echo $hex
5c2a7b9f
```
Terceiro, precisamos adicionar um byte inicial adicional de ```00``` se o dígito superior for "8" ou maior, para que o número não seja interpretado como negativo.
```
$ hexfirst=$(echo $hex | cut -c1)
$ [[ 0x$hexfirst -gt 0x7 ]] && hex="00"$hex
```
Quarto, precisamos traduzir o hexa em _big-endian_ (byte menos significativo por último) para _little-endian_ (byte menos significativo primeiro). Podemos fazer isso com o comando ```tac```:
```
$ lehex=$(echo $hex | tac -rs .. | echo "$(tr -d '\n')")
$ echo $lehex
9f7b2a5c
```
Além disso, sempre precisaremos saber o tamanho dos dados que colocamos na pilha, para que possamos precedê-los com o opcode adequado. Podemos apenas lembrar que cada dois caracteres hexadecimais é um byte. Ou podemos usar o comando ```echo -n``` com o pipe para ```wc -c``` e dividi-lo ao meio:
```
$ echo -n $lehex | wc -c | awk '{print $1/2}'
4
```
Com todo esse trabalho, iríamos saber que poderíamos traduzir o número inteiro 1546288031 em um opcode ```04``` (para colocar quatro bytes na pilha) seguido por ```9f7b2a5c``` (a representação hexadecimal do tipo _little-endian_ de 1546288031).

Se, ao invés disso, tivéssemos um número negativo, precisaríamos (1) fazer os cálculos no valor absoluto do número e, em seguida, (2) bit a bit - ou colocar 0x80 para nosso resultado _little-endian_ final. Por exemplo, ```9f7b2a5c```, que é 1546288031, se tornaria ```9f7b2adc```, que é -1546288031:
```
$ neglehex=$(printf '%x\n' $((0x$lehex | 0x80)))
$ echo $neglehex
9f7b2adc
```

### Transformando o Hex em Binário

Para completar nossa serialização, traduzimos o código hexadecimal em binário. Na linha de comando, isso requer apenas uma invocação simples do ```xxd -r -p```. No entanto, provavelmente desejamos fazer isso tudo junto, para também fazer o hash do script...

## Executando o Script de Conversão de Inteiros

Um script completo para alterar um número inteiro entre -2147483647 e 2147483647 para uma representação de magnitude assinada do tipo _little-endian_ em hexadecimal pode ser encontrado no [diretório de código src](../src/10_2_integer2lehex.sh). Podemos baixar o ```integeer2lehex.sh```.

> :warning: **AVISO:** Este script não foi verificado de forma consistente. Se formos utilizá-lo para criar scripts de bloqueio reais, precisamos nos certificar de verificar e testar os resultados.

Precisamos nos certificar de que as permissões no script estão corretas:
```
$ chmod 755 integer2lehex.sh
```
Podemos então executar o script da seguinte maneira:
```
$ ./integer2lehex.sh 1546288031
Integer: 1546288031
LE Hex: 9f7b2a5c
Length: 4 bytes
Hexcode: 049f7b2a5c

$ ./integer2lehex.sh -1546288031
Integer: -1546288031
LE Hex: 9f7b2adc
Length: 4 bytes
Hexcode: 049f7b2adc
```

## Analisando um Multisig P2SH

Para entender melhor o processo, faremos a engenharia reversa do multisig P2SH que criamos na seção [§6.1: Enviando uma Transação com Multisig](06_1_Sending_a_Transaction_to_a_Multisig.md). Dê uma olhada no ```redeemScript``` que usamos, que agora sabemos que é a versão hexadecimal do script de bloqueio:
```
522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae
```
Podemos traduzir isso de volta para o script manualmente usando a [página Wiki do Bitcoin Script](https://en.bitcoin.it/wiki/Script) como uma referência. Basta olhar para um byte (dois caracteres hexadecimais) de dados por vez, a menos que nos seja dito para olhar pra mais bytes usando OP_PUSHDATA (um opcode no intervalo de 0x01 a 0x4e).

Todo o Script será dividido da seguinte forma:
```
52 / 21 / 02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191 / 21 / 02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3 / 52 / ae
```
Aqui está o que cada parte individual significa:

* 0x52 = OP_2
* 0x21 = OP_PUSHDATA 33 bytes (hex: 0x21)
* 0x02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191 = os próximos 33 bytes (hash de chave pública)
* 0x21 = OP_PUSHDATA 33 bytes (hex: 0x21)
* 0x02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3 = os próximos 33 bytes (hash de chave pública)
* 0x52 = OP_2
* 0xae = OP_CHECKMULTISIG

Em outras palavras, esse ```redeemScript``` era uma tradução de ```2 02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191 02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3 2 OP_CHECKMULTISIG```. Voltaremos a este script na seção [§10.4: Programando um Multisig](10_4_Scripting_a_Multisig.md) quando detalharmos exatamente como os multisigs funcionam dentro do paradigma P2SH.

Se gostarmos de fazer o trabalho manual com esse tipo de tradução no futuro, podemos usar o ```decodescript bitcoin-cli```:
```
$ bitcoin-cli -named decodescript hexstring=522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae
{
  "asm": "2 02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191 02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3 2 OP_CHECKMULTISIG",
  "reqSigs": 2,
  "type": "multisig",
  "addresses": [
    "mmC2x2FoYwBnVHMPRUAzPYg6WDA31F1ot2",
    "mhwZFJUnWqTqy4Y7pXVum88qFtUnVG1keM"
  ],
  "p2sh": "2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr",
  "segwit": {
    "asm": "0 6fe9f451ccedb8e4090b822dcad973d0388a37b4c89fd1aed485110adecab2a9",
    "hex": "00206fe9f451ccedb8e4090b822dcad973d0388a37b4c89fd1aed485110adecab2a9",
    "reqSigs": 1,
    "type": "witness_v0_scripthash",
    "addresses": [
      "tb1qdl5lg5wvakuwgzgtsgku4ktn6qug5da5ez0artk5s5gs4hk2k25szvjky9"
    ],
    "p2sh-segwit": "2NByn92W1vH5oQC1daY69F5sU7PEStKKQBR"
  }
}
```
É especialmente útil para verificar nosso trabalho durante a serialização.

## Serializando um Script de Bloqueio da Maneira Fácil

Quando instalamos o ```btcdeb``` na seção [§9.3](09_3_Testing_a_Bitcoin_Script.md) também instalamos o ```btcc``` que pode ser usado para serializar scripts do Bitcoin:
```
$ btcc 2 02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191 02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3 2 OP_CHECKMULTISIG
warning: ambiguous input 2 is interpreted as a numeric value; use OP_2 to force into opcode
warning: ambiguous input 2 is interpreted as a numeric value; use OP_2 to force into opcode
522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae
```
Isso é muito mais fácil do que fazer tudo na mão!

Considere também o compilador em Python, [Transaction Script Compiler](https://github.com/Kefkius/txsc), que traduz de trás pra frente também.

## Fazendo o Hash de um Script Serializado

Depois de criar um script de bloqueio e serializá-lo, a terceira etapa na criação de uma transação P2SH é fazer o hash do script de bloqueio. Conforme observado anteriormente, um hash OP_HASH160 de 20 bytes é criado por meio de uma combinação de um hash SHA-256 e um hash RIPEMD-160. O hash de um script serializado, portanto, requer dois comandos: ```openssl dgst -sha256 -binary``` que faz o hash SHA-256 e produz um binário a ser enviado no pipe, então o ```openssl dgst -rmd160``` pega o fluxo do binário, faz um RIPEMD- 160 hash e, finalmente, gera um código hexadecimal legível.

Aqui está todo o processo, incluindo a transformação anterior do script hexadecimal em binário:
```
$ redeemScript="522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae"
$ echo -n $redeemScript | xxd -r -p | openssl dgst -sha256 -binary | openssl dgst -rmd160
(stdin)= a5d106eb8ee51b23cf60d8bd98bc285695f233f3
```

## Criando uma Transação P2SH

Criar o hash de 20 bytes apenas fornece o hash no centro de um script de bloqueio P2SH. Ainda precisamos colocá-lo junto com os outros opcodes que criam uma transação P2SH padrão: ```OP_HASH160 a5d106eb8ee51b23cf60d8bd98bc285695f233f3 OP_EQUAL```.

Dependendo de nossa API, podemos inserir isso como um ```scriptPubKey``` no estilo ```asm``` para nossa transação, ou podemos ter que traduzi-lo para o código ```hex``` também. Se tivermos que traduzir, podemos usar os mesmos métodos descritos acima para "Criar o código hexadecimal" (ou usar o ```btcc```), resultando em ```a914a5d106eb8ee51b23cf60d8bd98bc285695f233f387```.

Podemos observar que o ```hex scriptPubKey``` para a transação P2SH Script irá _sempre_ começar com um ```a914```, que é o ```OP_HASH160``` seguido por um ```OP_PUSHDATA``` de 20 bytes (hex: ```0x14```); e _sempre_ terminará com um ```87```, que é um ```OP_EQUAL```. Portanto, tudo o que precisamos fazer é colocar o script de resgate em hash entre esses números.

## Resumo: Construindo a Estrutura do P2SH

Na verdade, a criação do script de bloqueio P2SH entra ainda mais nas entranhas do Bitcoin. Embora seja útil saber como tudo isso funciona em um nível muito baixo, é mais provável que tenhamos uma API cuidando de todo o trabalho pesado para nós. Nossa tarefa será simplesmente criar o Script Bitcoin para fazer o bloqueio... que é o tópico principal dos capítulos 9, 11 e 12.

## O Que Vem Depois?

Vamos continuar "Incorporando Scripts em Transações P2SH no Bitcoin" na seção [§10.3: Executando um Script no Bitcoin com P2SH](10_3_Running_a_Bitcoin_Script_with_P2SH.md).