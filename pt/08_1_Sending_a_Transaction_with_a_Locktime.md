# 8.1: Enviando uma Transação com um Locktime

Os capítulos anteriores mostraram duas maneiras diferentes de enviar fundos de várias máquinas e para vários destinatários. Mas, existem duas outras maneiras de alterar fundamentalmente as transações básicas. A primeira delas é variar o tempo, escolhendo um tempo de bloqueio ou mais conhecido pela expressão em inglês locktime. Isso nos dá a capacidade de enviar transações brutas em algum momento no futuro.

## Entendendo Como o Locktime Funciona

Ao criar uma transação do tipo locktime, a bloqueamos com um número que representa a altura do bloco (se for um número pequeno) ou um carimbo de data/hora usando o padrão UNIX (se for um número grande). Isso informa à rede Bitcoin que a transação não pode ser colocada no bloco até que o tempo especificado chegue ou a blockchain tenha atingido a altura especificada.

> :book: _O que é a altura do bloco?_ É a contagem total de blocos na blockchain, contado desde o bloco gênese até o último bloco minerado da blockchain do Bitcoin.

Quando uma transação do tipo locktime está esperando para entrar em um bloco, ela pode ser cancelada. Isso significa que ainda não foi atingido a altura ou a data necessária. Na verdade, a capacidade de cancelar é todo o propósito de uma transação do tipo locktime.

> :book: _O que é nLockTime?_ É a mesma coisa que o locktime. Mais especificamente, é como o locktime é chamado internamente no código-fonte do Bitcoin Core.

> :book: _O que é o Timelock?_ O Locktime é apenas uma maneira de bloquear transações Bitcoin até algum ponto no futuro; o coletivo desses métodos são chamados de timelocks. O locktime é o método de timelock mais básico. Ele bloqueia uma transação inteira com um tempo absoluto e está disponível através do ```bitcoin-cli``` (é por isso que é o único timelock coberto nesta seção). Um método paralelo, que bloqueia uma transação com um tempo relativo, é definido no [BIP 68](https://github.com/bitcoin/bips/blob/master/bip-0068.mediawiki) e abordado na seção [§11.3: Usando CSV em Scripts](11_3_Using_CSV_in_Scripts.md).

> O Bitcoin Script capacita ainda mais os dois tipos de timelocks, permitindo o bloqueio de saídas individuais ao invés de toda a transação. Os timelocks absolutos (como Locktime) estão vinculados ao opcode Script OP_CHECKLOCKTIMEVERIFY, que é definido no [BIP 65](https://github.com/bitcoin/bips/blob/master/bip-0065.mediawiki) e que será estudado na seção [§11.2: Usando CLTV em Scripts](11_2_Using_CLTV_in_Scripts.md), enquanto os timelocks relativos (como o Timelock) estão vinculados ao opcode de Script OP_CHECKSEQUENCEVERIFY, que é definido no [BIP 112](https://github.com/bitcoin/bips /blob/master/bip-0112.mediawiki) e que também será abordado na seção [§11.3](11_3_Using_CSV_in_Scripts.md).

## Criando uma Transação com Locktime

Para criar uma transação com locktime, precisamos primeiro determinar como definiremos o tempo.

### Descobrindo Nosso Locktime Usando o Carimbo de Data/Hora do UNIX

Provavelmente, iremos definir com muito mais frequência o nosso tempo de bloqueio usando um carimbo de data/hora do tipo UNIX que representa uma data e hora específica. Podemos calcular um carimbo de data/hora UNIX usando sites que fazem essa conversão, como o [UNIX Time Stamp](http://www.unixtimestamp.com/) ou o [Epoch Convertor](https://www.epochconverter.com/). No entanto, seria melhor [escrevermos nosso próprio script](https://www.epochconverter.com/#code) em nossa máquina local, para que saibamos que o carimbo de data/hora UNIX que recebemos está correto (lembre-se, não confie, verifique). Se não fizermos isso, pelo menos seria interessante verificar em dois sites diferentes para ter a certeza que tudo está correto.

> :book: _Por que iríamos usar um carimbo de data/hora do UNIX?_ O uso de um carimbo de data/hora do UNIX facilita a vinculação definitiva de uma transação a um horário específico, sem se preocupar se a velocidade de criação do bloco mudara drasticamente até lá. Principalmente se estivermos criando um locktime em um futuro distante, é a coisa mais segura a ser feita. Entretanto, utilizar esse método é mais intuitivo, criando uma correlação direta entre alguma data do calendário e a hora em que a transação pode ser colocada na blockchain do Bitcoin.

> :warning: **AVISO:** O Locktime com carimbos de data/hora tem menos flexibilidade, pois a liberação de blocos não é regular e os tempos de bloqueio podem ser duas horas antes do tempo real, então um locktime significa "mais ou menos nesta data e nesta hora".

### Descobrindo Nossoo Locktime Usando a Altura do Bloco

Como alternativa, podemos definir o locktime usando um número menor, que representa a altura de um bloco. Para calcular a altura do bloco futuro, precisamos primeiro saber qual é a altura do bloco atual. O ```bitcoin-cli getblockcount``` nos dirá a altura do bloco atual baseado na blockchain da máquina local. Podemos verificar um explorador do Bitcoin como a [mempool.space](https://mempool.space/).

Depois de descobrir a altura atual, podemos decidir até que ponto no futuro iremos configurar o locktime. É importante lembrar que, em média, um novo bloco será criado a cada 10 minutos. Então, por exemplo, se quisermos definir o tempo de bloqueio para uma semana no futuro, devemos escolher uma altura de bloco que é _6 x 24 x 7 = 1.008 blocos_ depois do atual.

> :book: _Por que eu usaria a altura do bloco?_ Ao contrário dos carimbos de data/hora, não existe confusão nas alturas de bloco. Se definirmos uma altura de bloco de 120.000 para o nosso locktime, não haverá absolutamente nenhuma maneira dele entrar no bloco 119.999. Isso pode tornar o controle no código mais fácil ao manipular nossa transação com locktime. A desvantagem é que não podemos ter certeza de quando exatamente será o locktime.

> :warning: **AVISO:** Se desejamos definir um locktime usando a altura do bloco, devemos definir o tempo de bloqueio para menos de 500 milhões. Se definirmos como sendo 500 milhões ou mais, nosso número será interpretado como um carimbo de data/hora. Como o carimbo de data/hora usando o padrão UNIX com valor de 500 milhões era dia 5 de novembro de 1985, provavelmente significa que nossa transação será permitida a qualquer momento a entrar na blockchain.

## Escrevendo a Transação

Depois de descobrir o locktime, tudo o que precisamos fazer é escrever uma transação bruta padrão, com uma terceira variável para o ```locktime```:
```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.001, "'$changeaddress'": 0.00095 }''' locktime=1774650)
```
Podemos observar que o uso do ```locktime``` está abaixo dos 500 milhões, o que significa que ele define a altura do bloco. Nesse caso, são apenas alguns blocos além da altura do bloco atual no momento em que este livro foi escrito, com o objetivo de exemplificar como o locktime funciona sem ficar sentado por muito tempo para esperar para ver o que acontece.

A transação criada tem mais ou menos essa aparência:
```
$ bitcoin-cli -named decoderawtransaction hexstring=$rawtxhex 
{
  "txid": "ba440b1dd87a7ccb6a200f087d2265992588284eed0ae455d0672aeb918cf71e",
  "hash": "ba440b1dd87a7ccb6a200f087d2265992588284eed0ae455d0672aeb918cf71e",
  "version": 2,
  "size": 113,
  "vsize": 113,
  "weight": 452,
  "locktime": 1774650,
  "vin": [
    {
      "txid": "0ad9fb6992dfe4ea90236b69852b3605c0175633b32996a486dcd0b2e739e385",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967294
    }
  ],
  "vout": [
    {
      "value": 0.00100000,
      "n": 0,
      "scriptPubKey": {
        "asm": "0 f333554cc0830d03a9c1f26758e2e7e0f155539f",
        "hex": "0014f333554cc0830d03a9c1f26758e2e7e0f155539f",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1q7ve42nxqsvxs82wp7fn43ch8urc425ul5um4un"
        ]
      }
    },
    {
      "value": 0.00095000,
      "n": 1,
      "scriptPubKey": {
        "asm": "0 a37718a3510958112b6a766e0023ff251b6c2bfb",
        "hex": "0014a37718a3510958112b6a766e0023ff251b6c2bfb",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1q5dm33g63p9vpz2m2wehqqglly5dkc2lmtmr98d"
        ]
      }
    }
  ]
}
```
Observe que o número de sequência (```4294967294```) é menor que ```0xffffffff```. Esta é uma sinalização necessária para mostrar que a transação inclui um locktime. Também é feito automaticamente pelo ```bitcoin-cli```. Se o número da sequência for definido como ```0xffffffff```, nosso locktime será ignorado.

> :information_source: **NOTA - SEQUÊNCIA:** Este é o segundo uso do valor ```nSequence``` em Bitcoin. Tal como acontece com o RBF, o ```nSequence``` é novamente usado como um opt-in, desta vez para o uso do locktime. O 0xffffffff-1 (4294967294) é o valor para sinalizar o tempo de bloqueio porque proíbe propositadamente o uso do RBF (que requer uma ```nSequence < 0xffffffff-1```) e do timelock relativo (que requer ```nSequence < 0xf0000000```), os outros dois usos do ```nSequence```. Se definirmos o ```nSequence``` abaixo de ```0xf0000000```, então também teremos um timelock relativo em nossa transação, o que provavelmente não é o desejado.

> :warning: **ATENÇÃO:** Se estivermos criando uma transação bruta com locktime por algum outro meio que não seja o ```bitcoin-cli```, teremos que definir a sequência para um valor menor que ```0xffffffff``` manualmente.

## Enviando a Transação

Agora, já estamos familiarizados com a parte final, que é enviar a transação para a rede:
```
$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
error code: -26
error message:
non-final
```
Eita! Que erro é esse!?

Desde 2013, geralmente não podemos colocar a transação usando um timelock na mempool até que nosso bloqueio tenha expirado. No entanto, ainda podemos guardar nossa transação, ocasionalmente reenviando-a para a rede Bitcoin até que ela seja aceita na mempool. Uma outra opção é enviarmos a transação assinada (```$signedtx```) ao destinatário, para que ele possa colocá-la na mempool quando o tempo de bloqueio expirar.

Assim que o bloqueio de tempo expirar, qualquer pessoa pode enviar a transação assinada para a rede, e o destinatário receberá o saldo como esperado... Desde que a transação não tenha sido cancelada.

## Cancelando uma Transação com Locktime

Cancelar uma transação de tempo de bloqueio é _muito_ simples: precisamos apenas enviar uma nova transação usando pelo menos um dos UTXOs.

## Resumo: Enviando uma Transação com um Locktime

O Locktime oferece uma maneira de criar uma transação que _precisa_ esperar e que _não será aceita_ até que um determinado bloco chegue ou um momento pré-definido passe. Enquanto isso, ela pode ser cancelada simplesmente reutilizando um dos UTXO da transação.

> :fire: _Qual é o poder do locktime?_ O poder do locktime pode não ser imediatamente óbvio devido à capacidade de cancelá-lo com facilidade. No entanto, é outra das bases dos Smart Contracts, pois tem muita utilidade em uma variedade de aplicações de custódia ou contratuais. Por exemplo, considere uma situação em que um terceiro está fazendo o hold das nossas moedas. Para garantir a devolução dos nossos bitcoins caso o custodiante algum dia desaparecesse, ele poderia produzir uma transação com timelock para devolver as moedas para nós e, em seguida, atualizar isso de vez em quando com uma transação mais nova, no futuro. Se em algum momento ele não enviar a nova transação, as moedas retornarão para nós quando o timelock mais novo expirar. O timelock pode ser aplicado de forma semelhante a uma rede de pagamento, onde a rede mantém moedas enquanto estão sendo trocadas pelos participantes da rede. Finalmente, os timelocks oferecem um exemplo de contrato mais complexo, em que os pagamentos são enviados a várias pessoas. Esses pagamentos seriam baseados em transações com timelocks e seriam continuamente atualizados enquanto o proprietário continuasse a mostrar sinais de vida. (O fator unificador de todas essas aplicações é, obviamente, a _confiança_. Transações simples usando timelocks só funcionam se o detentor das moedas puder ser confiável para enviá-las sob as condições apropriadas).

## O que vem depois?

Vamos continuar "Expandindo Transações no Bitcoin" na seção [§8.2: Enviando uma Transação com Dados](08_2_Sending_a_Transaction_with_Data.md).