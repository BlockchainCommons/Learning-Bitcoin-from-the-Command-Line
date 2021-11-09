# 13.1: Escrevendo Scripts de Quebra-Cabeças

Os scripts de Bitcoin realmente _não_ precisam depender do conhecimento de uma chave secreta. Ao invés disso, eles podem ser quebra-cabeças de qualquer tipo.

## Escrevendo Scripts de Álgebra Simples

Nosso primeiro script real, da seção [§9.2: Executando um Script no Bitcoin](09_2_Running_a_Bitcoin_Script.md) foi um quebra-cabeça simples. Esse script Bitcoin, ```OP_ADD 99 OP_EQUAL```, poderia ter sido descrito da seguinte forma: ```x + y = 99```.

Esse tipo de script não tem muita aplicabilidade no mundo real, pois é muito fácil reivindicar os fundos. Mas, um quebra-cabeça que distribui pequenos valores em Bitcoin pode ser um entretenimento divertido.

Mais notavelmente, a criação de quebra-cabeças algébricos fornece uma boa compreensão de como funcionam as funções aritméticas no Bitcoin Script.

### Escrevendo um Script Multiplicador

O Bitcoin Script possui vários opcodes que foram desativados para manter a segurança do sistema. Um deles é o ```OP_MUL```, que teria permitido a multiplicação, mas, está desabilitado.

Então, como escreveríamos uma função algébrica como ```3x + 7 = 13```?

A resposta mais óbvia é usando o ```OP_DUP``` no número inserido no script de bloqueio duas vezes. Então podemos colocar o ```7``` e continuar adicionando até obter o total. O script de bloqueio completo ficaria assim: ```OP_DUP OP_DUP 7 OP_ADD OP_ADD OP_ADD 13 OP_EQUAL```.

Veja como seria executado o script de desbloqueio correto do ```2```:
```
Script: 2 OP_DUP OP_DUP 7 OP_ADD OP_ADD OP_ADD 13 OP_EQUAL
Stack: [ ]

Script: OP_DUP OP_DUP 7 OP_ADD OP_ADD OP_ADD 13 OP_EQUAL
Stack: [ 2 ]

Script: OP_DUP 7 OP_ADD OP_ADD OP_ADD 13 OP_EQUAL
Running: 2 OP_DUP
Stack: [ 2 2 ]

Script: 7 OP_ADD OP_ADD OP_ADD 13 OP_EQUAL
Running: 2 OP_DUP
Stack: [ 2 2 2 ]

Script: OP_ADD OP_ADD OP_ADD 13 OP_EQUAL
Stack: [ 2 2 2 7 ]

Script: OP_ADD OP_ADD 13 OP_EQUAL
Running: 2 7 OP_ADD
Stack: [ 2 2 9 ]

Script: OP_ADD 13 OP_EQUAL
Running: 2 9 OP_ADD
Stack: [ 2 11 ]

Script: 13 OP_EQUAL
Running: 2 11 OP_ADD
Stack: [ 13 ]

Script: OP_EQUAL
Stack: [ 13 13 ]

Script: 
Running: 13 13 OP_EQUAL
Stack: [ True ]
```
Ou se você preferir usar o `btcdeb`:
```
$ btcdeb '[2 OP_DUP OP_DUP 7 OP_ADD OP_ADD OP_ADD 13 OP_EQUAL]'
btcdeb 0.2.19 -- type `btcdeb -h` for start up options
valid script
9 op script loaded. type `help` for usage information
script   |  stack 
---------+--------
2        | 
OP_DUP   | 
OP_DUP   | 
7        | 
OP_ADD   | 
OP_ADD   | 
OP_ADD   | 
13       | 
OP_EQUAL | 

#0000 2
btcdeb> step
		<> PUSH stack 02
script   |  stack 
---------+--------
OP_DUP   | 02
OP_DUP   | 
7        | 
OP_ADD   | 
OP_ADD   | 
OP_ADD   | 
13       | 
OP_EQUAL | 

#0001 OP_DUP
btcdeb> step
		<> PUSH stack 02
script   |  stack 
---------+--------
OP_DUP   |      02
7        | 02
OP_ADD   | 
OP_ADD   | 
OP_ADD   | 
13       | 
OP_EQUAL | 

#0002 OP_DUP
btcdeb> step
		<> PUSH stack 02
script   |  stack 
---------+--------
7        |      02
OP_ADD   |      02
OP_ADD   | 02
OP_ADD   | 
13       | 
OP_EQUAL | 

#0003 7
btcdeb> step
		<> PUSH stack 07
script   |  stack 
---------+--------
OP_ADD   |      07
OP_ADD   |      02
OP_ADD   |      02
13       | 02
OP_EQUAL | 

#0004 OP_ADD
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 09
script   |  stack 
---------+--------
OP_ADD   |      09
OP_ADD   |      02
13       | 02
OP_EQUAL | 

#0005 OP_ADD
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 0b
script   |  stack 
---------+--------
OP_ADD   |      0b
13       | 02
OP_EQUAL | 

#0006 OP_ADD
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 0d
script   |  stack 
---------+--------
13       | 0d
OP_EQUAL | 
#0007 13
btcdeb> step
		<> PUSH stack 0d
script   |  stack 
---------+--------
OP_EQUAL |      0d
         | 0d

#0008 OP_EQUAL
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 01
script   |  stack 
---------+--------
         | 01
```
### Escrevendo um Sistema de Equações

E se quiséssemos escrever um sistema de equações, como ```x + y = 3```,```y + z = 5``` e ```x + z = 4```? A álgebra diz que as respostas resultariam em ```x = 1```,```y = 2``` e ```z = 3```. Mas, como programamos isso?

Obviamente, depois que o remetente inserir os três números, precisaremos de duas cópias de cada número, uma vez que cada número entra em duas equações diferentes. O ```OP_3DUP``` cuida disso e resulta em```x y z x y z``` estando na pilha. Tirar dois itens de cada vez resultará em ```y z```,```z x``` e ```x y```. Pronto! Essas são as três equações, então só precisamos adicioná-las e testá-las na ordem correta! Aqui está o script completo: ```OP_3DUP OP_ADD 5 OP_EQUALVERIFY OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL```.

Veja como funciona com o script de desbloqueio correto de ```1 2 3```:
```
Script: 1 2 3 OP_3DUP OP_ADD 5 OP_EQUALVERIFY OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Stack: [ ]

Script: OP_3DUP OP_ADD 5 OP_EQUALVERIFY OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Stack: [ 1 2 3 ]

Script: OP_ADD 5 OP_EQUALVERIFY OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Running: 1 2 3 OP_3DUP
Stack: [ 1 2 3 1 2 3 ]

Script: 5 OP_EQUALVERIFY OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Running: 2 3 OP_ADD
Stack: [ 1 2 3 1 5 ]

Script: OP_EQUALVERIFY OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Stack: [ 1 2 3 1 5 5 ]

Script: OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Running: 5 5 OP_EQUALVERIFY
Stack: [ 1 2 3 1 ] — Does Not Exit

Script: 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Running: 3 1 OP_ADD
Stack: [ 1 2 4 ]

Script: OP_EQUALVERIFY OP_ADD 3 OP_EQUAL
Stack: [ 1 2 4 4 ]

Script: OP_ADD 3 OP_EQUAL
Running: 4 4 OP_EQUALVERIFY
Stack: [ 1 2 ] — Does Not Exit

Script: 3 OP_EQUAL
Running: 1 2 OP_ADD
Stack: [ 3 ]

Script: OP_EQUAL
Stack: [ 3 3 ]

Script: 
Running: 3 3 OP_EQUAL
Stack: [ True ]
```
Aqui está usando o `btcdeb`:
```
$ btcdeb '[1 2 3 OP_3DUP OP_ADD 5 OP_EQUALVERIFY OP_ADD 4 OP_EQUALVERIFY OP_ADD 3 OP_EQUAL]'
btcdeb 0.2.19 -- type `btcdeb -h` for start up options
valid script
13 op script loaded. type `help` for usage information
script         |  stack 
---------------+--------
1              | 
2              | 
3              | 
OP_3DUP        | 
OP_ADD         | 
5              | 
OP_EQUALVERIFY | 
OP_ADD         | 
4              | 
OP_EQUALVERIFY | 
OP_ADD         | 
3              | 
OP_EQUAL       | 

#0000 1
btcdeb> step
		<> PUSH stack 01
script         |  stack 
---------------+--------
2              | 01
3              | 
OP_3DUP        | 
OP_ADD         | 
5              | 
OP_EQUALVERIFY | 
OP_ADD         | 
4              | 
OP_EQUALVERIFY | 
OP_ADD         | 
3              | 
OP_EQUAL       | 

#0001 2
btcdeb> step
		<> PUSH stack 02
script         |  stack 
---------------+--------
3              |      02
OP_3DUP        | 01
OP_ADD         | 
5              | 
OP_EQUALVERIFY | 
OP_ADD         | 
4              | 
OP_EQUALVERIFY | 
OP_ADD         | 
3              | 
OP_EQUAL       | 

#0002 3
btcdeb> step
		<> PUSH stack 03
script         |  stack 
---------------+--------
OP_3DUP        |      03
OP_ADD         |      02
5              | 01
OP_EQUALVERIFY | 
OP_ADD         | 
4              | 
OP_EQUALVERIFY | 
OP_ADD         | 
3              | 
OP_EQUAL       | 

#0003 OP_3DUP
btcdeb> step
		<> PUSH stack 01
		<> PUSH stack 02
		<> PUSH stack 03
script         |  stack 
---------------+--------
OP_ADD         |      03
5              |      02
OP_EQUALVERIFY |      01
OP_ADD         |      03
4              |      02
OP_EQUALVERIFY | 01
OP_ADD         | 
3              | 
OP_EQUAL       | 

#0004 OP_ADD
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 05
script         |  stack 
---------------+--------
5              |      05
OP_EQUALVERIFY |      01
OP_ADD         |      03
4              |      02
OP_EQUALVERIFY | 01
OP_ADD         | 
3              | 
OP_EQUAL       | 

#0005 5
btcdeb> step
		<> PUSH stack 05
script         |  stack 
---------------+--------
OP_EQUALVERIFY |      05
OP_ADD         |      05
4              |      01
OP_EQUALVERIFY |      03
OP_ADD         |      02
3              | 01
OP_EQUAL       | 

#0006 OP_EQUALVERIFY
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 01
		<> POP  stack
script         |  stack 
---------------+--------
OP_ADD         |      01
4              |      03
OP_EQUALVERIFY |      02
OP_ADD         | 01
3              | 
OP_EQUAL       | 

#0007 OP_ADD
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 04
script         |  stack 
---------------+--------
4              |      04
OP_EQUALVERIFY |      02
OP_ADD         | 01
3              | 
OP_EQUAL       | 

#0008 4
btcdeb> step
		<> PUSH stack 04
script         |  stack 
---------------+--------
OP_EQUALVERIFY |      04
OP_ADD         |      04
3              |      02
OP_EQUAL       | 01

#0009 OP_EQUALVERIFY
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 01
		<> POP  stack
script         |  stack 
---------------+--------
OP_ADD         |      02
3              | 01
OP_EQUAL       | 

#0010 OP_ADD
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 03
script         |  stack 
---------------+--------
3              | 03
OP_EQUAL       | 

#0011 3
btcdeb> step
		<> PUSH stack 03
script         |  stack 
---------------+--------
OP_EQUAL       |      03
               | 03

#0012 OP_EQUAL
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 01
script         |  stack 
---------------+--------
               | 01
```

> :warning: **ATENÇÃO** O ```btcdeb``` não é útil apenas para fornecer a visualização desses scripts, mas também para verificar os resultados. Com certeza, erramos na primeira vez, testando as equações na ordem errada. É tão fácil cometer um erro financeiramente fatal em um script do Bitcoin, e é por isso que todo script deve ser testado exaustivamente.

## Escrevendo Scripts Computacionais Simples

Embora os scripts de quebra-cabeça sejam triviais, podem realmente ter utilidade no mundo real se quisermos terceirizar uma computação. Podemos simplesmente criar um script que requer a resposta do cálculo e enviar fundos para o endereço P2SH como recompensa. Assim, a recompensa ficará lá até que alguém dê a resposta.

Por exemplo, Peter Todd [ofereceu recompensas](https://bitcointalk.org/index.php?topic=293382.0) para resolver equações que demonstram colisões para algoritmos criptográficos padrão. Aqui estava o script para confirmar uma colisão SHA1: ```OP_2DUP OP_EQUAL OP_NOT OP_VERIFY OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL```. Requer duas entradas, que serão os dois números que colidem.

Veja como funciona com as respostas corretas.

Primeiro, preenchemos nossa pilha:
```
Script: <numA> <numB> OP_2DUP OP_EQUAL OP_NOT OP_VERIFY OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL
Stack: [ ]

Script: OP_2DUP OP_EQUAL OP_NOT OP_VERIFY OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL
Stack: [ <numA> <numB> ]

Script: OP_EQUAL OP_NOT OP_VERIFY OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL
Running: <numA> <numB> OP_2DUP
Stack: [ <numA> <numB> <numA> <numB> ]
```
Em seguida, nos certificamos de que os dois números não são iguais, saindo se forem:
```
Script: OP_NOT OP_VERIFY OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL
Running: <numA> <numB> OP_EQUAL
Stack: [ <numA> <numB> False ]

Script: OP_VERIFY OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL
Running: False OP_NOT
Stack: [ <numA> <numB> True ]

Script: OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL
Running: True OP_VERIFY
Stack: [ <numA> <numB> ] — Does Not Exit
```
Agora criamos dois SHAs:
```
Script: OP_SWAP OP_SHA1 OP_EQUAL
Running: <numB> OP_SHA1
Stack: [ <numA> <hashB> ]

Script: OP_SHA1 OP_EQUAL
Running: <numA> <hashB> OP_SWAP
Stack: [ <hashB> <numA> ]

Script: OP_EQUAL
Running: <numA> OP_SHA1
Stack: [ <hashB> <hashA> ]
```
Finalmente, vemos se eles combinam.
```
Script: 
Running: <hashB> <hashA> OP_EQUAL
Stack: [ True ]
```
Este é um bom script porque mostra o uso cuidadoso da lógica (com o ```OP_NOT``` e o ```OP_VERIFY```) e bom uso das funções de pilha (com o ```OP_SWAP```). Trata-se de um ótimo exemplo de uma função do mundo real. E isso é muito real. Quando o [SHA-1 foi quebrado](https://shattered.io/), 2,48 BTC foram rapidamente liberados do endereço, com um valor total de cerca de $3.000 na época.

O ```btcdeb``` pode ser executado para provar a colisão (e o script):
```
btcdeb $ btcdeb '[255044462d312e330a25e2e3cfd30a0a0a312030206f626a0a3c3c2f57696474682032203020522f4865696768742033203020522f547970652034203020522f537562747970652035203020522f46696c7465722036203020522f436f6c6f7253706163652037203020522f4c656e6774682038203020522f42697473506572436f6d706f6e656e7420383e3e0a73747265616d0affd8fffe00245348412d3120697320646561642121212121852fec092339759c39b1a1c63c4c97e1fffe017f46dc93a6b67e013b029aaa1db2560b45ca67d688c7f84b8c4c791fe02b3df614f86db1690901c56b45c1530afedfb76038e972722fe7ad728f0e4904e046c230570fe9d41398abe12ef5bc942be33542a4802d98b5d70f2a332ec37fac3514e74ddc0f2cc1a874cd0c78305a21566461309789606bd0bf3f98cda8044629a1 255044462d312e330a25e2e3cfd30a0a0a312030206f626a0a3c3c2f57696474682032203020522f4865696768742033203020522f547970652034203020522f537562747970652035203020522f46696c7465722036203020522f436f6c6f7253706163652037203020522f4c656e6774682038203020522f42697473506572436f6d706f6e656e7420383e3e0a73747265616d0affd8fffe00245348412d3120697320646561642121212121852fec092339759c39b1a1c63c4c97e1fffe017346dc9166b67e118f029ab621b2560ff9ca67cca8c7f85ba84c79030c2b3de218f86db3a90901d5df45c14f26fedfb3dc38e96ac22fe7bd728f0e45bce046d23c570feb141398bb552ef5a0a82be331fea48037b8b5d71f0e332edf93ac3500eb4ddc0decc1a864790c782c76215660dd309791d06bd0af3f98cda4bc4629b1 OP_2DUP OP_EQUAL OP_NOT OP_VERIFY OP_SHA1 OP_SWAP OP_SHA1 OP_EQUAL']
```

As outras [recompensas](https://bitcointalk.org/index.php?topic=293382.0) de Peter Todd permanecem intocadas, pelo menos até o momento da redação deste artigo. Elas são todas escritas da mesma maneira que o exemplo SHA-1 acima.

## Compreendendo as Limitações dos Scripts de Quebra-Cabeça

Os scripts de quebra-cabeça são ótimos para examinar mais detalhadamente a programação no Bitcoin, mas só os veremos em uso no mundo real se estiverem mantendo pequenas quantias de fundos ou se forem destinados ao resgate por usuários muito habilidosos. Há uma razão para isso: eles não são seguros.

É aqui que a segurança cai:

Primeiro, qualquer um pode resgatá-los sem saber muito sobre um segredo. Eles precisam ter o ```redeemScript```, sim, que oferece alguma proteção, mas uma vez que consigam, esse é provavelmente o único segredo necessário, a menos que nosso quebra-cabeça seja _realmente_ difícil, como um quebra-cabeça computacional.

Em segundo lugar, o resgate real não é seguro. Normalmente, uma transação de Bitcoin é protegida pela assinatura. Como a assinatura cobre a transação, ninguém na rede pode reescrever essa transação para enviar para o endereço sem invalidar a assinatura (e, portanto, a transação). Isso não é verdade com transações cujas entradas são apenas números. Qualquer um poderia pegar a transação e reescrevê-la para permitir o roubo dos fundos. Se eles conseguirem colocar a transação em um bloco antes da nossa transação, eles ganham e nós não receberemos o dinheiro do quebra-cabeça. Existem soluções para isso, mas elas envolvem a mineração do bloco por conta própria ou um pool confiável para minerá-lo, e nenhuma dessas opções é racional para um usuário comum do Bitcoin.

Ainda assim, as recompensas criptográficas de Peter Todd provam que os scripts de quebra-cabeça têm sim alguma aplicação no mundo real.

## Resumo: Escrevendo Quebra-Cabeças com Scripts

Os scripts de quebra-cabeça são uma ótima introdução aos scripts do Bitcoin mais realistas e complexos. Eles demonstram o poder das funções matemáticas e de pilha no Bitcoin Script e como podem ser cuidadosamente combinadas para criar perguntas que requerem respostas muito específicas. No entanto, o uso no mundo real também é limitado pelos problemas de segurança inerentes às transações no Bitcoin não assinadas.

> :fire: ***Qual é o poder do script de quebra-cabeça?*** Apesar das limitações, os scripts de quebra-cabeça têm sido usados ​​no mundo real como prêmios para recompensas computacionais. Qualquer pessoa que consiga descobrir um quebra-cabeça complexo, cuja solução provavelmente tenha algum impacto no mundo real, pode ganhar a recompensa. Se vão conseguir sacar para seus endereços, é outra questão.

## O Que Vem Depois?

Vamos continuar "Projetando Scripts Reais no Bitcoin" na seção [§13.2: Escrevendo Scripts Complexos de Multisig](13_2_Writing_Complex_Multisig_Scripts.md).