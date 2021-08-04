# 12.2: Usando outros comandos no script

Já temos em mãos a maioria dos opcodes do script do Bitcoin que iremos usar na maioria dos nossos scripts. No entanto, o script do Bitcoin oferece muito mais opções, o que pode vir a ser exatamente o que precisamos para criar o nosso instrumento financeiro dos nossos sonhos.

Devemos consultar a [página do script  do Bitcoin](https://en.bitcoin.it/wiki/script) para termos uma lista mais completa de todos esses e muitos outros comandos. Esta seção destaca apenas os opcodes mais notáveis.

## Entendendo os Opcodes Aritméticos

Os opcodes aritméticos manipulam ou testam números.

Manipulam um número:

* OP_1ADD (0x8b) - Adiciona uma unidade;
* OP_1SUB (0x8C) - Subtrai uma unidade;
* OP_NEGATE (0x8f) - Inverte o sinal do número;
* OP_ABS (0x90) - O número fica positivo;
* OP_NOT (0x91) - Troca 1 por 0, senão 0.

É legal saber sobre o ```OP_0NOTEQUAL``` (0x92).

Manipulam dois números matematicamente:

* OP_ADD (0x93) - Adiciona dois números;
* OP_SUB (0x94) - Subtrai dois números;
* OP_MIN (0xA3) - Retorna o menor entre dois números;
* OP_MAX (0xA4) - Retorna o maior entre dois números.

Manipulam dois números logicamente:

* OP_BOOLAND (0x9a) - 1 se ambos os números não são 0, senão 0;
* OP_BOOLOR	(0x9B) - 1 se qualquer número não é 0, senão 0.

Testa dois números:

* OP_NUMEQUAL (0x9C) - 1 se ambos os números forem iguais, senão 0;
* OP_LESSTHAN (0x9f) - 1 se o primeiro número for menor que o segundo, senão 0;
* OP_GREATERTHAN (0xA0) - 1 se o primeiro número for maior que o segundo, senão 0;
* OP_LESSTHANOREQUAL (0xA1) - 1 se o primeiro número for menor ou igual ao segundo, senão 0;
* OP_GREATERTHANOREQUAL (0xA2) - 1 se o primeiro número for maior ou igual a segundo, senão 0.

É legal saber também: O ```OP_NUMEQUALVERIFY``` (0x9d) e o ```OP_NUMNOTEQUAL``` (0x9e)

Testa três números:

* OP_WITHIN (0xA5) - 1 se um número estiver no intervalo de dois outros números.

## Entendendo a Opcodes de Pilha

Há um número chocante de opcodes de pilha, mas além de ```OP_DROP```, ```OP_DUP```, e às vezes, ```OP_SWAP``` geralmente não serão necessários se tivermos cuidado com a ordem da pilha. No entanto, aqui estão alguns dos mais interessantes:

* OP_DEPTH (0x74) - Aumenta o tamanho da pilha;
* OP_DROP (0x75) - Retira o item superior da pilha;
* OP_DUP (0x76) - Duplica o item superior da pilha;
* OP_PICK (0x79) - Duplica o enésimo item da pilha começando pelo topo;
* OP_ROLL (0x7a) - Move o enésimo item para o topo da pilha;
* OP_SWAP(0x7C) - Troca os dois principais itens da pilha.

É legal saber também: O `OP_TOALTSTACK` (0x6b), `OP_FROMALTSTACK` (0x6c), `OP_2DROP` (0x6d), `OP_2DUP` (0x6e), `OP_3DUP` (0x6f), `OP_2OVER` (0x70), `OP_2ROT` (0x71), `OP_2SWAP` (0x72), `OP_IFDUP` (0x73), `OP_NIP` (0x77), `OP_OVER` (0x78), `OP_ROT` (0x7b), e o `OP_TUCK` (0x7d).

## Entendendo os Opcodes Criptográficos

Finalmente, uma variedade de opcodes para dar suporte ao hashing e a verificação da assinatura:

Hash:

* OP_RIPEMD160 (0xa6) — RIPEMD-160;
* OP_SHA1	(0xa7)	— SHA-1;
* OP_SHA256	(0xa8)	- SHA-256;
* OP_HASH160	(0xa9)	— SHA-256 + RIPEMD-160;
* OP_HASH256	(0xaa)	— SHA-256 + SHA-256.

Verifica as assinaturas:

* Op_checksig (0xac) - Verifica uma assinatura;
* Op_checkmultisig (0xae) - Verifica uma multisig M-DE-N.

É legal saber também: O `OP_CODESEPARATOR` (0xab), `OP_CHECKSIGVERIFY` (0xad), e o `OP_CHECKMULTISIGVERIFY` (0xaf).

## Resumo: Usando outros comandos no script

O script do Bitcoin inclui uma ampla gama de opcodes aritméticos, de pilha e criptográficos. A maioria desses opcodes adicionais provavelmente não serão tão comuns quanto os discutidos nas seções anteriores, mas, no entanto, estão disponíveis, para sabermos que existem caso precisemos utilizá-los para escrever nosso script!

## O Que Vem Depois?

Vamos avançar do "Expandindo os Scripts do Bitcoin" para o [Capítulo 13: Projetando Scripts Reais no Bitcoin](13_0_Designing_real_bitcoin_scripts.md).