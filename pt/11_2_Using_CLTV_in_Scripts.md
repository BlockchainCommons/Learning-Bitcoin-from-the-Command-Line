# 11.2: Usando o CLTV nos Scripts

O ```OP_CHECKLOCKTIMEVERIFY``` (ou CLTV) é o complemento natural para o ```nLockTime```. Ele muda a ideia de bloquear transações por um tempo absoluto ou altura de bloco para o âmbito dos opcodes, permitindo o bloqueio das UTXOs individuais.

> :warning: **AVISO DE VERSÃO:** O CLTV ficou disponível no Bitcoin Core 0.11.2, mas deve ser amplamente implementado neste momento.

## Lembrando do nLockTime

Antes de nos aprofundarmos no CLTV, devemos primeiro lembrar do funcionamento do ```nLockTime```.

Conforme detalhado na seção [§8.1:Enviando uma transação com Locktime](08_1_Sending_a_Transaction_with_a_Locktime.md), o locktime é habilitado pela configuração de duas variáveis, o ```nLockTime``` e o ```nSequence```. O ```nSequence``` deve ser definido para ser menor do que 0xffffffff (geralmente: 0xffffffff-1), então o ```nLockTime``` será interpretado da seguinte forma:

* Se o ```nLockTime``` for inferior a 500 milhões, será interpretado como uma altura de bloco;
* Se o ```nLockTime``` for 500 milhões ou mais, será interpretado como um carimbo de data/hora UNIX.

Uma transação com ```nLockTime``` definida não pode ser gasta (ou mesmo colocada na blockchain) até que a altura do bloco ou a data/hora sejam alcançados. Neste meio tempo, a transação pode ser cancelada, ao se utilizar qualquer uma das UTXOs que constituem a transação.

## Compreendendo o Opcode CLTV

O ```OP_CHECKLOCKTIMEVERIFY``` funciona dentro do mesmo paradigma de altura de blocos ou tempos UNIX, ambos absolutos, mas é executado como parte de um Script do Bitcoin. Ele lê um argumento, que pode ser os dois já mencionados. Por meio de uma metodologia um tanto complicada, ele compara esse argumento ao tempo atual. Se for muito cedo, o script irá falhar, porém, se a condição de tempo for atendida, o script continua.

Como o CLTV é apenas parte de um script (e presumivelmente parte de uma transação P2SH), uma transação CLTV não é mantida fora da mempool como uma transação ```nLockTime```. Logo, assim que for verificado, ele vai para a blockchain e os fundos são considerados gastos. O truque é que todas as saídas que foram bloqueadas com o CLTV não estão disponíveis para _serem gastas_ até que o CLTV permita.

### Compreendendo um CLTV de tempo absoluto

É assim que o ```OP_CHECKLOCKTIMEVERIFY``` seria utilizado para verificar o locktime de 24 de maio de 2017:
```
1495652013 OP_CHECKLOCKTIMEVERIFY
```
Mas geralmente vamos descrever isso em uma abstração:
```
<May24> OP_CHECKLOCKTIMEVERIFY
```
Ou assim:
```
<AbsoluteTime> OP_CHECKLOCKTIMEVERIFY
```

### Compreendendo um CLTV de altura de bloco absoluta

É assim que o ```OPCHECKLOCKTIMEVERIFY``` compararia a uma altura de bloqueio alcançada no dia 24 de maio de 2017:
```
467951 OP_CHECKLOCKTIMEVERIFY
```
Mas geralmente vamos abstrair assim:
```
<AbsoluteBlockHeight> OP_CHECKLOCKTIMEVERIFY
```

### Entendendo como o CLTV realmente funciona

A explicação acima é suficiente para usar e entender o CLTV. No entanto, o [BIP 65](https://github.com/bitcoin/bips/blob/master/bip-0065.mediawiki) apresenta todos os seguintes detalhes.

Um script de bloqueio só permitirá que uma transação reenvie uma UTXO bloqueado com um CLTV se o ```OP_CHECKLOCKTIMEVALUE``` verificar todas as seguintes condições:

* O campo ```nSequence``` deve ser definido como sendo menor do que 0xffffffff, geralmente 0xffffffff-1 para evitar conflitos com os timelocks relativos;
* CLTV deve retirar um operando da pilha e deve ser 0 ou maior.
* Tanto o operando da pilha quanto o ```nLockTime``` devem estar acima ou abaixo de 500 milhões, para representar o mesmo tipo de locktime absoluto;
* O valor ```nLockTime``` deve ser maior ou igual ao operando da pilha.

Portanto, a primeira coisa a se notar aqui é que o ```nLockTime``` ainda é utilizado com o CLTV. Para ser mais preciso, ele é necessário na transação que tenta _gastar novamente_ uma UTXO com o temporizador CLTV. Isso significa que não faz parte dos requisitos do script. É apenas o cronômetro que é usado para liberar os fundos, _como definido no script_.

Isso é gerenciado por meio de um entendimento inteligente de como o ```nLockTime``` funciona: Um valor para o ```nLockTime``` deve sempre ser escolhido sendo menor ou igual ao tempo presente (ou altura do bloco), de modo que a transação de gasto possa ser colocada na blockchain. Porém, devido aos requisitos do CLTV, também deve ser escolhido um valor maior ou igual ao operando do CLTV. A união desses dois conjuntos é ```NULL``` até que o tempo presente corresponda ao operando CLTV. Posteriormente, qualquer valor pode ser escolhido entre o operando do CLTV e o tempo atual. Normalmente, apenas iríamos configurar para a hora atual (ou para o bloco atual).

## Escrevendo um Script CLTV

O ```OP_CHECKLOCKTIMEVERIFY``` inclui um ```OP_VERIFY```, o que significa que ele irá parar imediatamente o script se nossa verificação não for bem-sucedida. Ele tem uma outra peculiaridade: Ao contrário da maioria dos comandos de "verificação", ele deixa o que está sendo testado na pilha (apenas no caso de querermos fazer qualquer outra verificação no tempo). Isso significa que um ```OP_CHECKLOCKTIMEVERIFY``` geralmente é seguido por um ```OP_DROP``` para limpar a pilha.

O seguinte script de bloqueio simples pode ser usado para transformar uma saída P2PKH em uma transação P2PKH bloqueada por tempo:
```
<NextYear> OP_CHECKLOCKTIMEVERIFY OP_DROP OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
```

### Codificando um script CLTV

Obviamente, como acontece com quaisquer scripts Bitcoin complexos, este script CLTV seria realmente codificado em um script P2SH, conforme explicado na seção [§10.1: Entendendo a Fundação do P2SH](10_1_Understanding_the_Foundation_of_P2SH.md) e na [§10.2: Construindo a Estrutura de P2SH](10_2_Building_the_Structure_of_P2SH.md).

Supondo que o ```<NextYear>``` fosse o número inteiro "1546288031" (little-endian hex: 0x9f7b2a5c) e o ```<pubKeyHash>``` fosse "371c20fb2e9899338ce5e99908e64fd30b789313", este ```redeemScript``` seria construído assim:
```
OP_PUSHDATA (4 bytes) 0x9f7b2a5c OP_CHECKLOCKTIMEVERIFY OP_DROP OP_DUP OP_HASH160 OP_PUSHDATA (20 bytes) 0x371c20fb2e9899338ce5e99908e64fd30b789313 OP_EQUALVERIFY OP_CHECKSIG
```
O que se traduz em hexadecimal como sendo:
```
04 9f7b2a5c b1 75 76 a9 14 371c20fb2e9899338ce5e99908e64fd30b789313 88 ac
```
Ou se preferir:
```
$ btcc 0x9f7b2a5c OP_CHECKLOCKTIMEVERIFY OP_DROP OP_DUP OP_HASH160 0x371c20fb2e9899338ce5e99908e64fd30b789313 OP_EQUALVERIFY OP_CHECKSIG
049f7b2a5cb17576a914371c20fb2e9899338ce5e99908e64fd30b78931388ac
```
O RPC ```decodescript``` pode verificar se acertamos a decodificação:
```
{
  "asm": "1546288031 OP_CHECKLOCKTIMEVERIFY OP_DROP OP_DUP OP_HASH160 371c20fb2e9899338ce5e99908e64fd30b789313 OP_EQUALVERIFY OP_CHECKSIG",
  "type": "nonstandard",
  "p2sh": "2MxANZMPo1b2jGaeKTv9rwcBEiXcXYCc3x9",
  "segwit": {
    "asm": "0 07e55bf1eaedf43ec52af57b77ad7330506c209a70d17fa2e1853304aa8e4e5b",
    "hex": "002007e55bf1eaedf43ec52af57b77ad7330506c209a70d17fa2e1853304aa8e4e5b",
    "reqSigs": 1,
    "type": "witness_v0_scripthash",
    "addresses": [
      "tb1qqlj4hu02ah6ra3f274ah0ttnxpgxcgy6wrghlghps5esf25wfedse4yw4w"
    ],
    "p2sh-segwit": "2N4HTwMjVdm38bdaQ5h3X3VktLY74D2qBoK"
  }
}
```

Não vamos mostrar continuamente como todos os Scripts do Bitcoin são codificados em transações P2SH, ao invés disso, ofereceremos estes atalhos: Quando descrevemos um script, ele será um ```redeemScript```, que normalmente seria serializado e codificado em um script de bloqueio e serializado no script de desbloqueio. Quando mostramos um procedimento de desbloqueio, será a segunda rodada de validação, seguindo a confirmação do hash do script de bloqueio.

## Gastando uma UTXO do CLTV

No caso do exemplo acima, o script de desbloqueio abaixo seria suficiente, desde que o ```nLockTime``` fosse definido em algum lugar antes da data ```<NextYear>```, e desde que o momento atual fosse, de fato, pelo menos ```<NextYear>```:
```
<signature> <pubKey>
```

### Executando um script CLTV

Para executar o Script, primeiro devemos concatenar os scripts de desbloqueio e bloqueio:
```
Script: <signature> <pubKey> <NextYear> OP_CHECKLOCKTIMEVERIFY OP_DROP OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Stack: [ ]
```
As três constantes seriam colocadas na pilha:
```
Script: OP_CHECKLOCKTIMEVERIFY OP_DROP OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Stack: [ <signature> <pubKey> <NextYear> ]
```
Então, o ```OP_CHECKLOCKTIMEVERIFY``` é executado. Ele encontra algo na pilha e verifica se o ```nSequence``` não é 0xffffffff. Finalmente, ele compara o ```<NextYear>``` com o ```nLockTime```. Se ambos são o mesmo tipo de representação e se ```nLockTime ≥ <NextYear>```, então ele é processado com sucesso (caso contrário, termina o script):
```
Script: OP_DROP OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Running: <NextYear> OP_CHECKLOCKTIMEVERIFY
Stack: [ <signature> <pubKey> <NextYear> ]
```
Então, o ```OP_DROP``` se livra daquele ```<NextYear>``` restante:
```
Script: OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG
Running: <NextYear> OP_DROP
Stack: [ <signature> <pubKey> ]
```
Finalmente, o restante do script é executado, que é uma verificação normal de uma assinatura e chave pública.

## Resumo: Usando o CLTV nos Scripts

O ```OP-CHECKLOCKTIMEVERIFY``` é um opcode simples que olha para um único argumento, o interpreta como uma altura de bloco ou timestamp UNIX, e só permite que a UTXO seja desbloqueada se àquela altura de bloco ou timestamp UNIX estiver no passado. Definir o ```nLockTime``` na transação de gastos é o que permite ao Bitcoin fazer este cálculo.

> :fire: ***Qual é o poder do CLTV?*** Já vimos que os tempos de bloqueio simples eram uma das bases dos Contratos Inteligentes. O CLTV dá o próximo passo. Agora podemos garantir que uma UTXO não possa ser gasta antes de um certo tempo _e_ podemos garantir que ela também não será gasta. Em sua forma mais simples, isso poderia ser usado para criar um fundo que alguém só poderia ter acesso aos 18 anos ou um fundo de aposentadoria que só poderia ser acessado quando fizesse 50 anos. No entanto, o verdadeiro poder vem quando combinado com condicionais, onde apenas o CLTV apenas é ativado em certas situações.

## O Que Vem Depois?

Vamos continuar "Aumentando o poder do timelock com scripts do Bitcoin" na seção [§11.3: Usando o CSV nos Scripts](11_3_Using_CSV_in_Scripts.md).