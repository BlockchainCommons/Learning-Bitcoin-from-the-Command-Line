# 4.1: Enviando Moedas da Maneira Fácil

O ```bitcoin-cli``` oferece três principais maneiras de enviar bitcoins: Utilizando um simples comando; Utilizando uma transação bruta e; Utilizando uma transação bruta com cálculos. Cada uma possui seus prós e contras. Este primeiro método de envio será o mais simples.

## Definindo Nossa Taxa de Transação

Antes de enviar qualquer bitcoin pela rede, devemos pensar sobre as taxas de transação que iremos pagar.

> :book: ***O que é uma taxa de transação?*** Não existe almoço grátis. Os mineradores adicionam as transações nos blocos porque são pagos para fazer isso. Eles não apenas são pagos pela rede para criar o bloco, mas também são pagos pelas pessoas que realizam as transações para incluí-las na blockchain. Se não pagarmos a taxa, nossa transação pode ficar travada... para sempre (ou, até que seja salva por alguns dos truques que falaremos no [capítulo cinco](05_0_Controlling_Bitcoin_Transactions.md)).

Ao usar métodos simples e automatizados para criar transações, conforme descrito aqui e na sessão [§4.5: Enviando Moedas com Transações Brutas Automatizadas](04_5_Sending_Coins_with_Automated_Raw_Transactions.md), o Bitcoin calculará as taxas de transação para nós. Isso é feito usando taxas flutuantes, onde o ```bitcoind``` observa quanto tempo as transações estão demorando para confirmar e calcula automaticamente o que devemos gastar.

Podemos ter um controle dessas informações colocando os valores racionais no nosso arquivo ```~/.bitcoin/bitcoin.conf```. Os valores de baixo custo a seguir garantiríam que houvesse uma taxa de transação mínima de 10.000 satoshis por kByte de dados em nossa transação e solicitaríam que as taxas flutuantes calculassem uma boa quantia para colocar a nossa transação nos próximos seis blocos.
```
mintxfee=0.0001
txconfirmtarget=6
```
No entanto, como iremos partir do pressuposto que ninguém que esteja fazendo este tutorial queira esperar para as transações serem confirmadas, vamos adotar os seguintes valores:
```
mintxfee=0.001
txconfirmtarget=1
```
Devemos inserí-los no arquivo ```~/.bitcoin/bitcoin.conf```, na seção principal, no início do arquivo ou se quisermos ter a certeza que nunca iremos utilizá-lo em outro lugar, podemos colocar na seção ```[test]```.

Para trabalharmos neste tutorial, estamos dispostos a gastar 100.000 satoshis por kB em cada transação, e queremos colocar cada transação no próximo bloco! Para colocar isso em uma perspectiva para melhor entendimento, uma transação simples é executada com um tamanho de 0,25 KB a 1 KB, então estaremos pagando algo em torno de 25 mil a 100 mil satoshis, sendo que atualmente, taxas acima de 10 mil são consideradas altíssimas para transações de quantidade média.

Depois de editar o arquivo ```bitcoin.conf```, vamos reiniciar o bitcoind usando dois comandos:
```
$ bitcoin-cli stop
$ bitcoind -daemon
```

## Obtendo um Endereço

Precisamos encontrar algum endereço para qual enviar nossas moedas. Normalmente, alguém nos envia um endereço e talvez nos dê uma assinatura para provar que é o proprietário desse endereço. Como alternativa, podemos fornecer um QR code para que possamos digitalizar, evitando assim possíveis erros de digitação na hora de colocar o endereço no local do destinatário. Em nosso caso, vamos enviar os bitcoins para `n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi`, que é o endereço de retorno de um antigo faucet da rede Testenet.

> :book: ***O que é um QR code?*** Um QR code é apenas um jeito diferente de passar o endereço Bitcoin. Muitas carteiras geram os QR codes para nós, enquanto alguns sites tentam convertê-los em um endereço usando o QR code. Obviamente, só podemos aceitar um QR code de um site no qual confiamos. Um pagador pode usar um leitor de código de barras para ler o código QR e, em seguida, pagá-lo.

## Enviando os Bitcoins

Agora estamos prontos para enviar alguns bitcoins. Na verdade, isso é bastante simples por meio da linha de comando. Basta usar ```bitcoin-cli sendtoaddress [endereço] [quantidade]```. Portanto, para enviar uns satoshinhos para o endereço `n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi`, basta fazer o seguinte:
```
$ txid=$(bitcoin-cli sendtoaddress n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi 0.001)
$ echo $txid
93250d0cacb0361b8e21030ac65bc4c2159a53de1075425d800b2d7a8ab13ba8
```

> 🙏 Para ajudar a manter os faucets da rede de testes vivos, tente usar o endereço que nos foi enviado os bitcoins no capítulo anterior, onde falamos sobre os recebimentos de transações.

Precisamos nos certificar de que o endereço digitado é o lugar para onde desejamos enviá-lo. Por isso, certifique-se _duas vezes_. Se cometermos algum erro no Bitcoin, não há como voltar atrás.

Iremos receber um txid de retorno quando usarmos este comando.

> ❕ Você pode acabar com um código de erro se não tivermos bitcoins suficientes na carteira para fazer a transação. Dependendo do nosso saldo atual, que podemos acessar usando o ```bitcoin-cli getbalance```, pode ser necessário ajustar o valor que iremos enviar para que bata com o valor que está sendo enviado, não se esquecendo da taxa de transação no meio deste processo. Se o nosso saldo atual for 0,001, podemos tentar enviar 0,0001. Como alternativa, seria melhor tirar a taxa que esperamos pagar que foi enviada na mensagem de erro do nosso saldo atual. Esta é uma boa prática, pois muitas carteiras esperam que calculemos nosso próprio valor + taxas ao fazermos as transações, mesmo dentre as corretoras mais populares.

> :warning: **ATENÇÃO:** O comando ```bitcoin-cli``` realmente gera comandos usando o JSON-RPC quando está se comunicando com o bitcoind. Eles podem ser muito exigentes. Este é um exemplo: Se listarmos a quantidade de bitcoin sem o zero à esquerda (ou seja usando ".1" em vez de "0.1"), o ```bitcoin-cli``` irá acusar um erro com uma mensagem misteriosa.

> :warning: **ATENÇÃO:** Mesmo se formos cuidadosos com nossos dados, é possível que haja este erro: _"Fee estimation failed. Fallbackfee is disabled"_. De maneira geral, isso significa que nosso ```bitcoind``` local não tem informações suficientes para estimar as taxas. Não é para vermos isso se tivermos esperado que nossa blockchain sincronize e configure nosso sistema com o Bitcoin Standup. Mas se não estivermos totalmente sincronizados, podemos nos deparar com este erro. Também pode ser que não estejamos usando um ```bitcoin.conf``` padrão: A informação ```blocksonly = 1``` fará com que nosso ```bitcoind``` não consiga estimar as taxas.

## Examinando Nossa Transação

Podemos ver nossa transação usando o ID de transação:
```
{
  "amount": -0.00100000,
  "fee": -0.00022200,
  "confirmations": 0,
  "trusted": true,
  "txid": "93250d0cacb0361b8e21030ac65bc4c2159a53de1075425d800b2d7a8ab13ba8",
  "walletconflicts": [
  ],
  "time": 1592604194,
  "timereceived": 1592604194,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi",
      "category": "send",
      "amount": -0.00100000,
      "vout": 1,
      "fee": -0.00022200,
      "abandoned": false
    }
  ],
  "hex": "0200000001e982921bb0189afc486e20bb05cc5825c71a0ba8868043ed04ece9ab0cb12a8e010000006a47304402200fc493a01c5c9d9574f7c321cee6880f7f1df847be71039e2d996f7f75c17b3d02203057f5baa48745ba7ab5f1d4eed11585bd8beab838b1ca03a4138516fe52b3b8012102fd5740996d853ea51a6904cf03257fc11204b0179f344c49739ec5b20b39c9bafeffffff02e8640d0000000000160014d37b6ae4a917bcc873f6395741155f565e2dc7c4a0860100000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac780b1b00"
}
```
Você pode ver não apenas o valor transferido (0,001 BTC), mas também uma taxa de transação (0,000222 BTC), que é cerca de um quarto da taxa mínima de 0,001 BTC/kB que foi definida, o que sugere que a transação foi de cerca de um quarto de kB de tamanho.

Enquanto esperamos que ela seja confirmada, podemos notar que o ```bitcoin-cli getbalance``` mostra que nosso dinheiro já foi debitado. Da mesma forma, o ```bitcoin-cli listunspent``` mostrará que uma transação inteira foi perdida, mesmo que fosse mais do que o que queríamos enviar. Há uma razão para isso: Sempre que temos moedas entrando, precisamos enviar _tudo_ junto, e temos que fazer um pouco de malabarismos se quisermos realmente ficar com parte dele! Mais uma vez, o ```sendtoaddress``` cuida de tudo isso para nós, o que significa que não precisamos nos preocupar em fazer qualquer alteração até enviar uma transação bruta. Nesse caso, uma nova transação aparecerá com nossa alteração quando nosso envio for incorporado a um bloco.

##  Resumo: Enviando Moedas da Maneira Fácil

Para enviar moedas facilmente, precisamos nos certificar de que nossos padrões de transação sejam racionais, obtendo um endereço e enviando as nossas moedas para lá. É por isso que esse modo é o mais fácil deles!

> :fire: ***Qual é o poder de enviar moedas da maneira fácil?***
> _As vantagens._ É fácil. Não precisamos nos preocupar com coisas misteriosas como UTXOs. Não precisamos calcular as taxas de transação manualmente, então, é provável que não iremos cometer erros que custem grandes valores. Se o nosso único objetivo é sentar na frente do computador e enviar alguns bitcoins, este é o caminho certo para fazer isso.
> _As desvantagens._ É alto nível. Temos muito pouco controle sobre o que está acontecendo e não podemos fazer nada demais. Se estamos planejando desenvolver um software mais complexo utilizando o Bitcoin ou se desejamos um entendimento mais profundo de como o Bitcoin funciona, então a maneira fácil é apenas um carrossel sem graça antes de chegarmos nas montanhas russas.

##  O Que Vem Depois?

Vamos continuar "Enviando Transações no Bitcoin" na sessão [§4.2 Criando Uma Transação Bruta](04_2_Creating_a_Raw_Transaction.md).