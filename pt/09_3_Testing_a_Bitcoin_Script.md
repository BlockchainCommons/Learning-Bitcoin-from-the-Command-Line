# 9.3: Testando um Script do Bitcoin

Programar no Bitcoin nos permite um controle adicional considerável sobre as transações no Bitcoin, mas também é um tanto quanto perigoso. Como descreveremos na seção [§10.1](10_1_Understanding_the_Foundation_of_P2SH.md), os scripts reais são um tanto isolados da rede Bitcoin, o que significa que é possível escrever um script e a rede aceitá-lo, mesmo que seja impossível resgatar daquele script! Portanto, precisamos testar exaustivamente nossos scripts antes de colocarmos nosso dinheiro neles.

Este capítulo, portanto, descreve um método primário para testar os scripts do Bitcoin, que também usaremos para os exemplos ocasionais que ocorrerem durante esta seção.

## Instalando o btcdeb

O Bitcoin Script Debugger (```btcdeb```) criado por @kallewoof é um dos métodos mais confiáveis ​​que encontramos para depurar os scripts no Bitcoin. No entanto, ele requer a configuração de C++ e algumas coisas a mais em nossa máquina, portanto, também ofereceremos algumas outras opções no final do capítulo.

Primeiro, precisamos clonar o repositório do ```btcdeb``` do GitHub, que também exigirá a instalação do ```git``` caso ainda não o tenhamos em nossa máquina.
```
$ sudo apt-get install git
$ git clone https://github.com/bitcoin-core/btcdeb.git
```
Note que quando executamos o ```git clone```, ele irá copiar o ```btcdeb``` para o nosso diretório atual. Vamos fazer isso no diretório ```~standup```.
```
$ ls
bitcoin-0.20.0-x86_64-linux-gnu.tar.gz  btcdeb  laanwj-releases.asc  SHA256SUMS.asc
```
Posteriormente, devemos instalar o C++ e os demais pacotes.
```
$ sudo apt-get install autoconf libtool g++ pkg-config make
```
Também precisaremos instalar o readline, pois isso torna o depurador muito mais fácil de ser usado, suportando a utilização do histórico usando as setas para cima e para baixo, movimento da esquerda para a direita, além do preenchimento automático usando tab e outras interfaces boas para o usuário.
```
$ sudo apt-get install libreadline-dev
```
Agora estamos prontos para compilar e instalar o ```btcdeb```:
```
$ cd btcdeb
$ ./autogen.sh
$ ./configure
$ make
$ sudo make install
```
Depois de tudo isso, devemos ter uma cópia do ```btcdeb``` em nossa máquina:
```
$ which btcdeb
/usr/local/bin/btcdeb
```

## Usando o btcdeb

O ```btcdeb``` funciona como um depurador padrão. Ele pega um script (bem como qualquer número de entradas da pilha) como um argumento de inicialização. Em seguida, podemos percorrer o script usando o ```step```.

Se ao invés disso nós quisermos inicializá-lo sem argumentos, iremos obter simplesmente um interpretador onde podemos emitir comandos ```exec [opcode]``` para realizar ações diretas.

### Usando o btcdeb com um Exemplo de Adição

O exemplo a seguir mostra o uso do ```btcdeb``` usando como exemplo a adição da seção anterior, ```1 2 OP_ADD```
```
$ btcdeb '[1 2 OP_ADD]' 
btcdeb 0.2.19 -- type `btcdeb -h` for start up options
warning: ambiguous input 1 is interpreted as a numeric value; use OP_1 to force into opcode
warning: ambiguous input 2 is interpreted as a numeric value; use OP_2 to force into opcode
miniscript failed to parse script; miniscript support disabled
valid script
3 op script loaded. type `help` for usage information
script  |  stack 
--------+--------
1       | 
2       | 
OP_ADD  | 
#0000 1
```
Ele mostra nosso script inicial, rodando de cima para baixo, e também mostra o que será executado a seguir no script.

Nós precisamos digitar ```step``` para ele avançar um passo pegando o primeiro item no script e colocando-o na pilha:
```
btcdeb> step
		<> PUSH stack 01
script  |  stack 
--------+--------
2       | 01
OP_ADD  | 
#0001 2
```
E novamente:
```
btcdeb> step
		<> PUSH stack 02
script  |  stack 
--------+--------
OP_ADD  |      02
        | 01
#0002 OP_ADD
```
Agora executamos o ```OP_ADD``` e há uma grande empolgação porque esse opcode tira os primeiros dois itens da pilha, faz a soma e, em seguida, coloca o resultado na pilha.
```
btcdeb> step
		<> POP  stack
		<> POP  stack
		<> PUSH stack 03
script  |  stack 
--------+--------
        | 03
```
E é aí que nosso script termina, sem mais nada para executar e com um ```03``` no topo da pilha como resultado do script.

> **NOTA:** O ```btcdeb``` nos permite repetir o comando anterior pressionando apenas a tecla enter. Faremos isso em exemplos subsequentes, então não fique assustado com os prompts ```btcdeb>``` se não houver nenhum comando. Isto está apenas repetindo o comando anterior (geralmente o ```step```).

### Usando o btcdeb com um Exemplo de Subtração

A seção anterior também incluiu um exemplo de subtração um pouco mais complexo na criação do script: ```3 2 OP_ADD 4 OP_SUB```. Isso é o resultado:

```
$ btcdeb '[3 2 OP_ADD 4 OP_SUB]'
btcdeb 0.2.19 -- type `btcdeb -h` for start up options
warning: ambiguous input 3 is interpreted as a numeric value; use OP_3 to force into opcode
warning: ambiguous input 2 is interpreted as a numeric value; use OP_2 to force into opcode
warning: ambiguous input 4 is interpreted as a numeric value; use OP_4 to force into opcode
miniscript failed to parse script; miniscript support disabled
valid script
5 op script loaded. type `help` for usage information
script  |  stack 
--------+--------
3       | 
2       | 
OP_ADD  | 
4       | 
OP_SUB  | 
#0000 3
btcdeb> step
		<> PUSH stack 03
script  |  stack 
--------+--------
2       | 03
OP_ADD  | 
4       | 
OP_SUB  | 
#0001 2
btcdeb> 
		<> PUSH stack 02
script  |  stack 
--------+--------
OP_ADD  |      02
4       | 03
OP_SUB  | 
#0002 OP_ADD
btcdeb> 
		<> POP  stack
		<> POP  stack
		<> PUSH stack 05
script  |  stack 
--------+--------
4       | 05
OP_SUB  | 
#0003 4
btcdeb> 
		<> PUSH stack 04
script  |  stack 
--------+--------
OP_SUB  |      04
        | 05
#0004 OP_SUB
btcdeb> 
		<> POP  stack
		<> POP  stack
		<> PUSH stack 01
script  |  stack 
--------+--------
        | 01
```
Voltaremos ao ```btcdeb``` de tempos em tempos, e ele continuará sendo uma excelente ferramenta para testarmos nossos scripts.

### Usando o Poder do btcdeb

O ```btcdeb``` também tem algumas funções mais poderosas, como ```print``` e ```stack```, que mostram o script e a pilha a qualquer momento da execução.

Por exemplo, no script acima, depois de avançar para o comando ```OP_ADD```, podemos fazer o seguinte:
```
btcdeb> print
    #0000 3
    #0001 2
 -> #0002 OP_ADD
    #0003 4
    #0004 OP_SUB
btcdeb> stack
<01>	02	(top)
<02>	03
```
Usar esses comandos pode tornar mais fácil observarmos o que está acontecendo e onde estamos no processo.

## Testando um Script Online

Existem também alguns simuladores na web que podemos usar para testar os scripts online. Eles podem ser superiores a uma ferramenta de linha de comando, oferecendo uma saída gráfica melhor, mas também descobrimos que eles tendem a ter certas deficiências.

No passado, tentamos fornecer diretrizes abrangentes sobre o uso de sites como o [Script Playground](http://www.crmarsh.com/script-playground/) ou o [Bitcoin Online Script Debugger](https: // bitcoin-script-debugger.visvirial.com/), mas eles estão desatualizados e/ou sumiram e não podemos mais acompanhá-los.

O que podemos garantir é que esses depuradores possuem a vantagem de mostrar coisas visualmente e explicitamente informando se um script foi bem-sucedido (desbloqueado) ou que houve falha (permanece bloqueado). Porém eles possuem algumas desvantagens relacionadas as assinaturas, onde muitos deles sempre retornam ```true``` para testes de assinatura ou então possuem mecanismos muito complicados para incorporá-las.

## Testando um Script com Bitcoin

Mesmo com uma ótima ferramenta como o ```btcdeb``` ou recursos transitórios como os diversos testadores de script online, não estamos trabalhando com a coisa real. Não podemos garantir que elas seguem as regras de consenso do Bitcoin, o que significa que não podemos garantir os resultados. Por exemplo, o Script Playground diz explicitamente que ignora um bug que está implícito quando usando o multisig no Bitcoin. Isso significa que qualquer código multisig que testarmos com sucesso no Script Playground dará erro no mundo real.

Portanto, a única maneira de _realmente_ testar os scripts do Bitcoin é testá-los usando a Testnet.

E como podemos faz isso? Acontece que esse é o tópico do [capítulo 10](10_0_Embedding_Bitcoin_Scripts_in_P2SH_Transactions.md), que examina a introdução desses scripts abstratos no mundo real do Bitcoin incorporando-os em transações P2SH. (Mas, mesmo assim, provavelmente precisaremos de uma API para enviar nossa transação P2SH para a rede Bitcoin, então os testes completos ainda serão produzidos no futuro).

_Quaisquer que sejam_ os outros métodos de teste que usamos, testar um script na Testnet deve ser nosso teste final _antes_ de colocar nosso script em Mainnet. Não podemos confiar que nosso código está correto. Não podemos fazer isso apenas olhando para ele. Nem mesmo podemos confiar em quaisquer simuladores ou depuradores que estivermos utilizando. Fazer isso é outra ótima maneira de perder seu saldo no Bitcoin.

## Resumo: Testando um Script do Bitcoin

Você deve instalar o ```btcdeb``` como uma ferramenta de linha de comando para testar os scripts do Bitcoin. No momento em que este livro foi escrito, produzimos resultados precisos que podem percorrer todo o processo do script. Também podemos procurar em alguns sites online uma representação mais visual. Quando estivermos com tudo pronto, vamos precisar usar a testnet para ter certeza de que as coisas estão funcionando com precisão, antes de implantarmos na Mainnet.

## O Que Vem Depois?

Vamos continuar "Apresentando Scripts no Bitcoin" com nosso primeiro exemplo real na seção [§9.4: Programando um P2PKH](09_4_Scripting_a_P2PKH.md).