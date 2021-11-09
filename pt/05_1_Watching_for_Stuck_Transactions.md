# 5.1: Atentando-se para Transações Presas

Às vezes, uma transação de Bitcoin pode ficar presa. Isso normalmente ocorre porque não havia taxa de transação suficiente, mas também pode ser devido a uma falha da rede ou do software.

## Observando as Transações Enviadas

Nós devemos _sempre_ observar as transações para garantir que tenham sido encerradas. O ```bitcoin-cli listtransactions``` mostrará todas as nossas transações de entrada e saída, enquanto o ```bitcoin-cli gettransaction``` juntamente com um txid, irá mostrar uma transação específica.

Abaixo temos uma transação que não foi incluída em um bloco. Podemos garantir isso porque não temos nenhuma confirmação.
```
$ bitcoin-cli -named gettransaction txid=fa2ddf84a4a632586d435e10880a2921db6310dfbd6f0f8f583aa0feacb74c8e
{
  "amount": -0.00020000,
  "fee": -0.00001000,
  "confirmations": 0,
  "trusted": true,
  "txid": "fa2ddf84a4a632586d435e10880a2921db6310dfbd6f0f8f583aa0feacb74c8e",
  "walletconflicts": [
  ],
  "time": 1592953220,
  "timereceived": 1592953220,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "tb1qw508d6qejxtdg4y5r3zarvary0c5xw7kxpjzsx",
      "category": "send",
      "amount": -0.00020000,
      "vout": 0,
      "fee": -0.00001000,
      "abandoned": false
    }
  ],
  "hex": "02000000014cda1f42a1bd39d8d0ff5958a804bc2bc548b71d7ceadbde53ea15aeaf1e2691000000006a473044022016a7a9f045a0f6a52129f48adb7da35c2f54a0741d6614e9d55b8a3bc3e1490a0220391e9085a3697bc790e94bb924d5310e16f23489d9c600864a32674e871f523c01210278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132ffffffff02204e000000000000160014751e76e8199196d454941c45d1b3a323f1433bd6e8030000000000001600146c45d3afa8762086c4bd76d8a71ac7c976e1919600000000"
```
Uma transação pode ser considerada presa se permanecer nesse estado por um longo período de tempo. A tempos atrás, podíamos ter a certeza de que todas as transações iriam ser confirmadas _eventualmente_. Mas, isso não é mais verdade devido ao aumento exponencial do uso do Bitcoin. Agora, se uma transação ficar travada por muito tempo, ela sairá da mempool e, em seguida, será esquecida para sempre da rede Bitcoin.

> :book: ***O que é a mempool?*** A Mempool (ou Piscina de Memória) é um local onde todas as transações não confirmadas em um node bitcoin ficam. Estas são as transações que um node recebeu da rede P2P que ainda não estão incluídas em um bloco. Cada node bitcoin pode ter um conjunto ligeiramente diferente de transações em sua mempool: Transações diferentes podem ter sido propagadas para um node específico. Isso depende de quando o node foi iniciado pela última vez e também os limites de tamanho. Quando um minerador faz um bloco, ele usa as transações da mempool. Então, quando um bloco é verificado, todos os mineradores removem essas transações da mempool. A partir do Bitcoin 0.12, as transações não confirmadas também podem expirar na mempool se forem antigas o suficiente, normalmente o tempo é de 72 horas. No caso das versões 0.14.0 o tempo foi aumentado para 2 semanas. As pools de mineração podem ter seus próprios mecanismos de gerenciamento da mempool.

Esta lista tem de todas as [transações não confirmadas](https://blockchain.info/unconfirmed-transactions) e pode não corresponder a nenhuma mempool de um node específico, mas pode ser considerada (principalmente) um superconjunto de todas elas.

## Decidindo o que fazer

Se sua transação ficar paralisada por mais tempo do que esperamos, normalmente podemos fazer uma dessas quatro coisas:

**1. Esperar até que seja confirmada.** Se enviamos a transação com uma taxa baixa ou média, ela deve ser processada cedo ou tarde. Conforme mostrado no site [Mempool Space](https://mempool.space), aqueles com taxas mais baixas _irão esperar mais_. (Vamos dar uma olhada na transação mais à esquerda e ver quanto tempo ela está esperando e quanto ela pagou de taxa).

**2. Esperar até que seja expirada.** Se acidentalmente enviamos uma transação sem taxa, ou se colocamos uma taxa muito baixa em um momento de congestionamento da rede, então nossa transação pode nunca ser concluída. No entanto, nossos bitcoins não serão perdidos. Contanto que não tenhamos uma carteira que fique reenviando propositalmente transações não confirmadas, ela deve ser eliminada da mempool em aproximadamente duas semanas, e então podemos tentar novamente utilizando valores mais altos nas taxas.

**3. Usar o RBF como sendo o remetente.** Se formos nós quem estamos enviando a transação e optarmos por usar o RBF (Replace-By-Fee), podemos tentar novamente com uma taxa mais alta. Podemos consultar a sessão [§5.2: Reenviando uma transação com o RBF](05_2_Resending_a_Transaction_with_RBF.md) para sabermos mais sobre isso.

**4. Use o CPFP como sendo o destinatário.** Se nós formos os recebedores do saldo, podemos usar o CPFP (Child-Pays-For-Parent) para usar a transação não confirmada como um input para uma nova transação. Podemos consultar a sessão [§5.3: Financiando uma transação com o CPFP](05_3_Funding_a_Transaction_with_CPFP.md).

## Resumo: Atentando-se para Transações Presas

Esta é uma introdução ao poder das transações do Bitcoin. Se sabemos que uma transação está presa, podemos decidir como liberá-la com recursos como o RBF ou o CPFP.

## O Que Vem Depois?

Continuemos "Controlando Transações no Bitcoin" com a sessão [§5.2: Reenviando uma Transação com RBF](05_2_Resending_a_Transaction_with_RBF.md).
