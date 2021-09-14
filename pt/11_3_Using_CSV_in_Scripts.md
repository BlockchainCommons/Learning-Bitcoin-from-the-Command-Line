# 11.3: Usando o CSV nos Scripts

O ```nLockTime``` e o ```OP_CHECKLOCKTIMEVERIFY``` (ou CLTV) são apenas um lado da equação do timelock. Do outro lado estão o ```nSequence``` e o ```OP_CHECKSEQUENCEVERIFY```, que podem ser usados ​​para verificar tempos relativos ao invés de tempos absolutos.

> :warning: **AVISO DE VERSÃO:** O CSV está disponível no Bitcoin Core 0.12.1.

## Compreendendo o nSequence

Cada entrada em uma transação tem um valor ```nSequence``` (ou se preferirmos ```sequence```). Tem sido uma ferramenta principal para melhorias do Bitcoin, conforme discutido anteriormente na seção [§5.2: Reenviando uma transação com o RBF](05_2_Resending_a_Transaction_with_RBF.md) e na [§8.1 Enviando uma transação com Locktime](08_1_Sending_a_Transaction_with_a_Locktime.md), onde o usamos para sinalizar o RBF e o ```nLockTime```, respectivamente. No entanto, há mais um uso para o ```nSequence```, descrito no [BIP 68](https://github.com/bitcoin/bips/blob/master/bip-0068.mediawiki): Podemos usá-lo para criar um timelock relativo em uma transação.

Um timelock relativo é um bloqueio colocado em uma entrada específica de uma transação e que é calculado em relação à data de mineração da UTXO que está sendo usado na entrada. Por exemplo, se uma UTXO foi minerada no bloco #468260 e uma transação foi criada onde a entrada para aquela UTXO foi uma ```nSequence``` de 100, então a nova transação não poderia ser minerada até o bloco #468360.

Simples assim!

> :information_source: **NOTA - SEQUÊNCIA:** Este é o terceiro uso do ```nSequence``` no Bitcoin. Qualquer valor no ```nSequence``` sem o conjunto de 32 bits (1 << 31), ou seja, do 0x00000001 a 0x7ffffffff, será interpretado como um bloqueio de tempo relativo se a ```nVersion ≥ 2``` (que é o padrão do Bitcoin Core depois da versão 0.14.0). Devemos ter cuidado para garantir que os timelocks relativos não entrem em conflito com os outros dois usos de ```nSequence```, que é para sinalizar o ```nTimeLock``` e o RBF. O ```nTimeLock``` geralmente define um valor como sendo 0xffffffff-1, onde um timelock relativo não é permitido e, o RBF geralmente define um valor de "1", onde um bloqueio de tempo relativo é irrelevante, porque define um timelock de 1 bloco.

> De maneira geral, podemos lembrar do seguinte: Com um valor ```nVersion``` de 2, um valor ```nSequence``` que vai de 0x00000001 a 0x7fffffff permite um timelock relativo, o RBF e o ```nTimeLock```. Um valor ```nSequence``` de 0x7fffffff a 0xffffffff-2 permite o RBF e o ```nTimeLock```. Um valor ```nSequence``` de 0xffffffff-1 permite apenas o ```nTimeLock```. Um valor ```nSequence``` de 0xffffffff não permite nenhum dos três. Um ```nVersion``` pode ser definido como 1 para não permitir bloqueios de tempo relativos para qualquer valor do ```nSequence```. Ufa!

### Criando um CSV de Tempo de Bloco Relativo

O formato para usar o ```nSequence``` para representar bloqueios de tempo relativos é definido no [BIP 68](https://github.com/bitcoin/bips/blob/master/bip-0068.mediawiki) e é um pouco mais complexo do que apenas inserir um número qualquer, como fizemos no ```nTimeLock```. Ao invés disso, as especificações do BIP dividem o número de quatro bytes em três partes:

* Os primeiros dois bytes são usados ​​para especificar um timelock relativo;
* O 23º bit é usado para sinalizar positivamente se o bloqueio se refere a um tempo ou a uma altura de bloco;
* O 32º bit é usado para sinalizar positivamente se os timelocks relativos estão desativados.

Com isso dito, a construção de um timelock relativo baseado em bloco é bem fácil, porque os dois bits sinalizados são definidos como ```0```, então apenas precisamos definir o ```nSequence``` para um valor entre 1 e 0xffff (65535). A nova transação pode ser extraída naquele número de blocos depois que o UTXO associado foi extraído.

### Criando um CSV de Tempo Relativo

Podemos definir o ```nSequence``` como um tempo relativo, onde o bloqueio dura 512 segundos vezes o valor do ```nSequence```.

A fim de fazer isso, precisamos:

1. Decidir o quanto no futuro definiremos o nosso bloqueio de tempo relativo;
2. Converter isso para segundos;
3. Dividir por 512;
4. Arredondar esse valor para cima ou para baixo e defini-lo como ```nSequence```;
5. Definir o 23º bit como verdadeiro.

Para definir um período de 6 meses no futuro, devemos primeiro calcular da seguinte forma:
```
$ seconds=$((6*30*24*60*60))
$ nvalue=$(($seconds/512))
```
Em seguida, transformar isso em hexadecimal:
```
$ hexvalue=$(printf '%x\n' $nvalue)
```
Finalmente, definimos o 23º bit do valor hexadecimal que criamos:
```
$ relativevalue=$(printf '%x\n' $((0x$hexvalue | 0x400000)))
$ echo $relativevalue
4224679
```
Se convertermos de volta, teremos o valor de 4224679 = 10000000111011010100111. O 23º dígito é definido como "1", enquanto os primeiros 2 bytes, 0111011010100111, são convertidos em 76A7 em hexadecimal ou 30375 em decimal. Multiplicando isso por 512, teremos 15,55 milhões de segundos, o que de fato é 180 dias.

## Criando uma transação com um timelock relativo

Então desejamos criar uma transação simples com um timelock relativo? Tudo que precisamos fazer é emitir uma transação onde o ```nSequence``` de uma entrada é definido como mostramos acima: Com o ```nSequence``` para essa entrada definido de forma que os primeiros dois bytes definam o timelock, o 23º bit define o tipo do timelock, e o 32º bit é definido como sendo falso.

Vamos enviar a transação e veremos que ela não pode ser legalmente minerada até que blocos ou tempo suficientes tenham passado além do tempo que a UTXO foi minerada.

Exceto que praticamente ninguém faz isso. As definições do [BIP 68](https://github.com/bitcoin/bips/blob/master/bip-0068.mediawiki) para ```nSequence``` foram incorporadas ao Bitcoin Core ao mesmo tempo que o [BIP 112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki), que descreve o opcode CSV e o ```nSequence``` equivalente ao opcode CLTV. Assim como o CLTV, o CSV oferece recursos aprimorados. Portanto, quase todo o uso dos timelocks relativos foi com o opcode CSV, não com o valor ```nSequence``` bruto por si só.

|                      | Absolute Timelock | Relative Timelock |
|:--------------------:|-------------------|-------------------|
| **Transaction Bloqueada** | nTimeLock         | nSequence         |
| **Saída do Bloqueio**      | OP_CHECKLOCKTIMEVERIFY| OP_CHECKSEQUENCEVERIFY |

## Compreendendo o Opcode CSV

O ```OP_SEQUENCEVERIFY``` nos Scripts do Bitcoin funciona quase como o ```OP_LOCKTIMEVERIFY```.

Podemos exigir que uma UTXO seja mantida por cem blocos após sua mineração:
```
100 OP_CHECKSEQUENCEVERIFY
```
Ou podemos fazer um cálculo mais complexo para exigir que uma UTXO seja retida daqui seis meses, neste caso acabaremos com um número mais complexo:
```
4224679 OP_CHECKSEQUENCEVERIFY
```
Neste caso, usaremos uma abreviatura:
```
<+6Months> OP_CHECKSEQUENCEVERIFY
```

> :warning: **ATENÇÃO:** Lembre-se de que um timelock relativo é um intervalo de tempo desde a mineração da UTXO usada como uma entrada. _Não_ é um intervalo de tempo após a criação da transação. Se usarmos uma UTXO que já foi confirmada cem vezes, e colocarmos um timelock relativo de 100 blocos nela, ela será elegível para mineração imediatamente. Os timelocks relativos têm alguns usos muito específicos, mas provavelmente não se aplicam se nosso único objetivo for determinar algum tempo definido no futuro.

### Entendendo como o CSV realmente funciona

O CSV tem muitas das mesmas sutilezas de uso que CLTV:

* O campo ```nVersion``` deve ser definido como 2 ou mais;
* O campo ```nSequence``` deve ser definido como sendo menor do que 0x80000000;
* Quando CSV é executado, deve haver um operando na pilha que esteja entre 0 e 0xf0000000-1;
* Tanto o operando da pilha quanto o ```nSequence``` devem ter o mesmo valor no 23º bit;
* O ```nSequence``` deve ser maior ou igual ao operando da pilha.

Assim como no CLTV, quando estiver usando uma UTXO com um CSV em condições de bloqueio, devemos definir o ```nSequence``` para habilitar a transação. Normalmente, o configuraremos com o valor exato no script de bloqueio.

## Escrevendo um script CSV

Assim como o ```OP_CHECKLOCKTIMEVERIFY```, o ```OP_CHECKSEQUENCEVERIFY``` inclui um ```OP_VERIFY``` implícito e deixa os argumentos na pilha, exigindo um ```OP_DROP``` quando finalizar tudo.

Um script que bloquearia fundos por até seis meses após a mineração e que exigiria uma assinatura no estilo P2PKH padrão teria a seguinte aparência:
```
<+6Months> OP_CHECKSEQUENCEVERIFY OP_DROP OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
```

### Codificando um script CSV

Ao codificar um script CSV, precisamos tomar cuidado ao codificar o valor inteiro para o timelock relativo. Deve ser passado como um número inteiro de 3 bytes, o que significa que iremos ignorar o maior byte, o que pode desativar o timelock relativo. Como é um número inteiro, precisamos nos certificar de convertê-lo para little-endian.

Isso pode ser feito com o script de shell ```integer2lehex.sh``` do capítulo anterior.

Para um tempo relativo de 100 blocos:
```
$ ./integer2lehex.sh 100
Integer: 100
LE Hex: 64
Length: 1 bytes
Hexcode: 0164
```
Embora deva ser preenchido como ```000064```, exigindo um código de ```03000064```.

Por um período relativo de 6 meses:
```
$ ./integer2lehex.sh 4224679
Integer: 4224679
LE Hex: a77640
Length: 3 bytes
Hexcode: 03a77640
```

## Gastando uma UTXO do CSV

Para gastar uma UTXO bloqueado com um script CSV, devemos definir o ```nSequence``` dessa entrada para um valor maior que o requerido no script, mas menor que o tempo entre a UTXO e o bloco atual. Isso mesmo, isso significa que precisamos saber o requisito exato no script de bloqueio. Mas temos uma cópia do ```redeemScript```, então se não conhecermos os requisitos, podemos desserializá-lo e, em seguida, definir o ```nSequence``` como sendo o número que é mostrado lá.

## Resumo: Usando o CSV nos Scripts

O ```nSequence``` e o CSV oferecem uma alternativa para o ```nLockTime``` e o CLTV onde bloqueamos uma transação com base em um tempo relativo desde que a entrada foi extraída, ao invés de basear o bloqueio em um tempo definido no futuro. Eles funcionam quase de forma idêntica, exceto pelo fato de que o valor do ```nSequence``` é codificado de forma ligeiramente diferente do valor do ```nLockTime```, com bits específicos significando coisas específicas.

> :fire: ***Qual é o poder do CSV?*** O CSV não é apenas uma maneira preguiçosa de bloquear uma transação, quando não queremos calcular um tempo no futuro. Ele é um paradigma totalmente diferente, um bloqueio que usaríamos se fosse importante criar uma duração mínima específica entre o momento em que uma transação é minerada e o momento em que os fundos podem ser gastos. O uso mais óbvio é (mais uma vez) para uma conta de garantia, quando desejamos um tempo preciso entre a entrada dos fundos e a saída. No entanto, ele tem possibilidades muito mais poderosas em transações fora da rede, incluindo canais de pagamento. Esses aplicativos são, por definição, construídos em transações que não são realmente colocadas na blockchain, o que significa que, se forem posteriormente colocados em um bloco, um período de tempo pode ser muito útil. Os [Hashed Timelock Contracts](https://en.bitcoin.it/wiki/Hashed_Timelock_Contracts) foram uma dessas implementações, dando a base para a rede de pagamento Lightning. Eles serão discutidos na seção [§13.3: Expandindo os Scripts do Bitcoin](13_3_Empowering_Bitcoin_with_Scripts.md).

## O Que Vem Depois?

Vamos avançar "Criando Scripts do Bitcoin" no capítulo [12: Expandindo os Scripts do Bitcoin](12_0_Expanding_Bitcoin_Scripts.md).