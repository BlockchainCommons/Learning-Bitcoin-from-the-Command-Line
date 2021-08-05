# 11.1: Compreendendo as Opções de Timelocks

Na seção [§8.1: Enviando uma transação com Locktime](08_1_Sending_a_Transaction_with_a_Locktime.md), o ```nLocktime``` ofereceu uma ótima opção inicial para bloquear as transações para que não pudessem ser gastas até algum ponto no futuro, com base no tempo (data/hora) ou na altura do bloco. Mas, essa não é a única maneira de colocar um timelock em uma transação.

## Compreendendo as limitações do nLockTime

O ```nLockTime``` é uma maneira simples e poderosa de bloquear uma transação, mas possui algumas limitações:

1. **Sem divisões.** O `nLocktime` bloqueia toda a transação;
2. **Sem rede.** A maioria dos nodes modernos não aceita um ```nLockTime``` na mempool até que esteja na hora ou próximo para ser finalizada;
3. **Sem Scripts.** O uso simples e original do ```nLockTime``` não permitia que fosse usado em Scripts;
4. **Sem proteção.** O ```nLockTime``` permite que os fundos sejam gastos com uma transação diferente e não bloqueada.

O último item costumava ser o _dealbreaker_ para o ```nLockTime```. Isso evitou que uma transação fosse gasta, mas não impediu que os fundos fossem usados em uma transação diferente. Então, havia certos usos, mas todos dependiam de confiança.

## Compreendendo as possibilidades dos scripts de Timelock

Nos últimos anos, o Bitcoin Core foi expandido para permitir a manipulação dos timelocks no nível do opcode com os _OP_CHECKLOCKTIMEVERIFY_ (CLTV) e _OP_CHECKSEQUENCEVERIFY_ (CSV). Ambos trabalham sob uma nova metodologia que fortalece ainda mais o Bitcoin.

_Eles são opcodes._ Por serem opcodes, o CLTV e o CSV podem ser usados ​​como parte de condições de resgate mais complexas. Na maioria das vezes, eles estão vinculados às condicionais que ireimos descrever no próximo capítulo.

_Eles bloqueiam as saídas._ Por serem opcodes incluídos nas transações como parte de uma ```sigPubKey```, eles apenas bloqueiam aquela saída única. Isso significa que as transações são aceitas na rede Bitcoin e que as UTXOs usadas ​​para financiar essas transações são gastos. Não há como voltar atrás em uma transação bloqueada por tempo com o CLTV ou o CSV como acontece com um ```nLockTime``` vazio. Gastar novamente a UTXO resultante requer que as condições do timelock sejam atendidas.

Aqui está um ponto importante sobre a utilização dos timelocks: _Eles são bloqueios de mão única._ Os bloqueios de tempo são projetados para desbloquear fundos em um determinado momento. Eles não podem bloquear novamente um fundo: Uma vez que um fundo bloqueado por tempo está disponível, ele ficará disponível para ser gasto.

### Compreendendo as possibilidades do CLTV

O _OP_CHECKLOCKTIMEVERIFY_ ou CLTV é compativel com o clássico recurso ```nLockTime```, mas no novo paradigma baseado em opcode. Ele permite que uma UTXO se torne acessível em um determinado momento ou em uma determinada altura de bloco.

O CLTV foi detalhado pela primeira vez no [BIP 65](https://github.com/bitcoin/bips/blob/master/bip-0065.mediawiki).

### Compreendendo as possibilidades do CSV

O _OP_CHECKSEQUENCEVERIFY_ ou CSV depende de um novo tipo de "locktime relativo", que é definido no campo _nSequence_ da transação. Como de costume, ele pode ser definido como uma data/hora ou uma altura de bloco. Se for definido como um tempo "n", então uma transação bloqueada em um tempo relativo pode ser gasta "n x 512" segundos depois que a UTXO foi minerada, e se for definido como um bloco "n", então uma transação bloqueada em tempo relativo pode ser gasta em "n" blocos depois que a UTXO foi minerada.

O uso do ```nSequence``` para um bloqueio de tempo relativo foi detalhado primeiramente no [BIP 68](https://github.com/bitcoin/bips/blob/master/bip-0068.mediawiki), e o opcode CSV foi adicionado no [BIP 112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki).

## Resumo: Compreendendo as Opções de Timelocks

Agora possuímos quatro opções de Timelocks:

* ```nLockTime``` para manter uma transação fora do blockchain até um dado momento específico;
* ```nSequence``` para manter uma transação fora do blockchain até um dado momento relativo;
* CLTV para tornar uma UTXO impossível de ser gasto até uma data/hora específica;
* CSV para tornar uma UTXO impossível de ser gasto até uma data/hora ou altura do bloco relativa.

## O Que Vem Depois?

Vamos continuar "Aumentando o poder do timelock com scripts do Bitcoin" na seção [§11.2: Usando o CLTV nos Scripts](11_2_Using_CLTV_in_Scripts.md).