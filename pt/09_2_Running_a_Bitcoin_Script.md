# 9.2: Executando um Script do Bitcoin

Os scripts de Bitcoin podem não parecer tão intuitivos no começo, mas sua execução é bastante simples usando a notação polonesa reversa e uma pilha (_stack_).

## Compreendendo a Linguagem de Programação

Um script Bitcoin possui três partes: uma linha de entrada; uma pilha para o armazenamento; e comandos específicos para execução.

### Compreendendo a Ordem

Os scripts do Bitcoin são executados da esquerda para a direita. Para nós é fácil, porque é a mesma ordem na qual lemos. No entanto, pode na verdade ser o elemento menos intuitivo do Bitcoin Script, porque significa que as funções não têm a aparência que esperaríamos. Ao invés disso, _os operandos vêm antes do operador._

Por exemplo, se estivéssemos somando "1" e "2", nosso script do Bitcoin ficaria ```1 2 OP_ADD```, e _não_"1 + 2". Como sabemos que o operador OP_ADD recebe duas entradas, sabemos que as duas entradas anteriores são os operandos.

> :warning: **ATENÇÃO:** Tecnicamente, tudo em Bitcoin Script é um opcode, portanto, seria mais apropriado registrar o exemplo acima como ```OP_1 OP_2 OP_ADD```. Em nossos exemplos, não nos preocupamos sobre como as constantes serão avaliadas, pois esse é um tópico de tradução, como é explicado em [§10.2: Construindo a Estrutura do P2SH](10_2_Building_the_Structure_of_P2SH.md). Alguns escritores preferem também retirar o prefixo "OP" de todos os operadores, mas nós optamos por não fazê-lo.

### Compreendendo a Pilha

Na verdade, não é correto dizer que um operador se aplica às entradas anteriores. Na verdade, um operador se aplica às entradas principais na pilha do Bitcoin.

> :book: ***O que é uma pilha?*** Uma pilha é uma estrutura de dados LIFO (last in, first out, ou no português, primeiro a entrar, último a sair). Possui duas funções de acesso: push e pop. Empurrar (push) coloca um novo objeto no topo da pilha, empurrando para baixo tudo que está abaixo dele. Retirar (pop) remove o objeto superior da pilha.

Sempre que o script do Bitcoin encontrar uma constante, ele a coloca na pilha. Portanto, o exemplo acima de ```1 2 OP_ADD``` seria realmente parecido com o que foi processado:
```
Script: 1 2 OP_ADD
Stack: [ ]

Script: 2 OP_ADD
Stack: [ 1 ]

Script: OP_ADD
Stack: [ 1 2 ]
```
_Observe que, neste e nos exemplos seguintes, o topo da pilha está à direita e a parte inferior à esquerda._

### Compreendendo os Opcodes

Quando um script do Bitcoin encontra um operador, ele o avalia. Cada operador retira zero ou mais elementos da pilha como entradas, geralmente um ou dois. Em seguida, ele os processa de uma maneira específica antes de colocar zero ou mais elementos de volta na pilha, geralmente um ou dois.

> :book: ***O que é um Opcode?*** O Opcode significa "código de operação" (no inglês, "_operation code_"). É normalmente associado a código de linguagem de máquina e é uma função simples (ou "operador").

OP_ADD retira dois itens da pilha (no nosso caso: 2 depois 1), adiciona-os e coloca o resultado de volta na pilha (no exemplo: 3).
```
Script:
Running: 1 2 OP_ADD
Stack: [ 3 ]
```

## Construindo Complexidade

Scripts mais complexos são criados executando mais comandos em ordem. Eles precisam ser avaliados cuidadosamente da esquerda para a direita, para que possamos entender o estado da pilha conforme cada novo comando é executado. Ele mudará constantemente, como resultado de operadores anteriores:
```
Script: 3 2 OP_ADD 4 OP_SUB
Stack: [ ]

Script: 2 OP_ADD 4 OP_SUB
Stack: [ 3 ]

Script: OP_ADD 4 OP_SUB
Stack: [ 3 2 ]

Script: 4 OP_SUB
Running: 3 2 OP_ADD
Stack: [ 5 ]

Script: OP_SUB
Stack: [ 5 4 ]

Script: 
Running: 5 4 OP_SUB
Stack: [ 1 ]
```

## Compreendendo o Uso do Bitcoin Script

É basicamente programar no Bitcoin ... além de algumas complexidades de como essa linguagem de programação interage com o próprio Bitcoin.

### Compreendendo scriptSig e scriptPubKey

Como vimos anteriormente, cada entrada de uma transação no Bitcoin contém um ```scriptSig``` que é usado para desbloquear o ```scriptPubKey``` para o UTXO associado. Eles são _efetivamente_ concatenados, o que significa que o ```scriptSig``` e o ```scriptPubKey``` são executados juntos, nessa ordem.

Então, presuma que um UTXO foi bloqueado com um ```scriptPubKey``` que leia ```OP_ADD 99 OP_EQUAL```, exigindo como entrada dois números que somam noventa e nove, e presuma que o ```scriptSig``` de ```1 98``` foi executado para desbloqueá-lo. Os dois scripts seriam efetivamente executados em ordem como ```1 98 OP_ADD 99 OP_EQUAL```.

Avaliando o resultado:
```
Script: 1 98 OP_ADD 99 OP_EQUAL
Stack: []

Script: 98 OP_ADD 99 OP_EQUAL
Stack: [ 1 ]

Script: OP_ADD 99 OP_EQUAL
Stack: [ 1 98 ]

Script: 99 OP_EQUAL
Running: 1 98 OP_ADD
Stack: [ 99 ]

Script: OP_EQUAL
Stack: [ 99 99 ]

Script: 
Running: 99 99 OP_EQUAL
Stack: [ True ]
```
Esta abstração não é muito precisa: por razões de segurança, o ```scriptSig``` é executado, então o conteúdo da pilha é transferido para o ```scriptPubKey``` para ser executado, mas é precisa o suficiente para entender como a chave do ```scriptSig``` se encaixa no cadeado de ```scriptPubKey```.

> :warning: **AVISO:** O exemplo acima é um tipo de transação não-padrão. Na verdade, não seria aceito por nós que executamos o Bitcoin Core com as configurações padrões. Na seção [§10.1: Compreendendo a Base do P2SH](10_1_Understanding_the_Foundation_of_P2SH.md) iremos discutir como realmente _poderíamos_ executar um Script Bitcoin como este, usando o poder do P2SH.

### Obtendo os Resultados

O Bitcoin irá verificar uma transação e permitir que o UTXO possa ser gasto novamente se dois critérios forem atendidos ao executar o ```scriptSig``` e o ```scriptPubKey```:

   1. A execução não foi marcada como inválida em nenhum ponto, por exemplo, com um OP_VERIFY com falha ou com o uso de um opcode desativado;
   2. O item no topo da pilha no final da execução é verdadeiro (diferente de zero).

No exemplo acima, a transação seria bem-sucedida porque a pilha tem um ```True``` no final. Mas, seria igualmente permitido terminar com uma pilha completa e o número ```42``` no topo (os leitores do Guia do Mochileiro das Galáxias pegaram a referência).

## Resumo: Executando um Script do Bitcoin

Para processar um script no Bitcoin, um ```scriptSig``` é executado seguido pelo ```scriptPubKey``` que ele está desbloqueando. Esses comandos são executados em ordem, da esquerda para a direita, com constantes sendo colocadas em uma pilha e os operadores retirando elementos dessa pilha e, em seguida, enviando os resultados de volta para ela. Se o script não parar no meio e se o item no topo da pilha no final for diferente de zero, então o UTXO será desbloqueado.

## O Que Vem Depois?

Vamos continuar "Apresentando Scripts no Bitcoin" na seção [§9.3: Testando um Script no Bitcoin](09_3_Testing_a_Bitcoin_Script.md).