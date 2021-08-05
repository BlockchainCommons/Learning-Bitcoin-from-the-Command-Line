# 10.6: Gastando uma Transação P2SH

Antes de encerrarmos esta visão geral das transações P2SH, vamos abordar como gastá-las. Esta seção é principalmente uma visão geral, referindo-se a uma seção anterior onde _já_ gastamos uma transação P2SH.

## Usando o Script de Resgate

Como vimos na seção [§6.2: Gastando uma Transação com Multsig](06_2_Spending_a_Transaction_to_a_Multisig.md), gastar uma transação P2SH tem tudo a ver com ter aquela versão serializada do script de bloqueio, o chamado _redeemScript_. Portanto, o primeiro passo para poder gastar uma transação P2SH é ter certeza de salvar o _redeemScript_ antes de fornecer o endereço P2SH para todos.

### Coletando as Variáveis

Como endereços P2SH além dos endereços especiais multisig e nested Segwit não estão integrados no ```bitcoin-cli```, não haverá atalhos para gastarmos o P2SH como vimos na seção [§6.3: Enviando e Gastando um Multisig de Maneira Automatizada](6_3_Sending_an_Automated_Multisig.md) . Vamos precisar coletar todas as variáveis mais complexas por conta própria!

Isso significa que precisaremos coletar:

   * O ```hex``` do ```scriptPubKey``` para a transação que estamos gastando;
   * O ```redeemScript``` serializado;
   * Quaisquer chaves privadas, já que assinaremos manualmente;
   * Todos os ```txids```,```vouts``` e ```addresses``` regulares que precisarmos.

## Criando a Transação

Como vimos na seção §6.2, a criação de uma transação é bem padrão:
```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.00005}''')
$ echo $rawtxhex
020000000121654fa95d5a268abf96427e3292baed6c9f6d16ed9e80511070f954883864b10000000000ffffffff0188130000000000001600142c48d3401f6abed74f52df3f795c644b4398844600000000
```
No entanto, a assinatura requer a inserção de informações extras para o (1) ```scriptPubKey```; (2) o ```redeemScript``` e; (3) quaisquer chaves privadas necessárias.

Aqui está o exemplo de como fazer isso para aquele multisig P2SH integrado na seção §6.2:
```
$ bitcoin-cli -named signrawtransactionwithkey hexstring=$rawtxhex prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cNPhhGjatADfhLD5gLfrR2JZKDE99Mn26NCbERsvnr24B3PcSbtR"]'
```
Com qualquer outro tipo de P2SH, incluiremos um ```redeemscript``` diferente, mas, fora isso, a prática é exatamente a mesma. A única diferença é que depois de dois capítulos de trabalho com scripts agora entendemos o que é o ```scriptPubKey``` e o que é o ```redeemScript```, então, esperançosamente, o que eram elementos misteriosos a quatro capítulos atrás, agora não são tão misteriosos assim.

## Resumo: Gastando uma Transação P2SH

Já gastamos um P2SH no Capítulo 6, quando reenviamos uma transação multsig da maneira mais difícil, que exigia alinhar as informações do ```scriptPubKey``` e do ```redeemScript```. Agora sabemos que o ```scriptPubKey``` é um script de bloqueio P2SH padronizado, enquanto o ```redeemScript``` corresponde a um hash naquele script de bloqueio e que precisamos ser capazes de executá-lo com as variáveis ​​adequadas para receber um resultado ```True```. Mas além disso, não há nada de novo ao gastar uma transação P2SH, porque já fizemos isso!

## O Que Vem Depois?

Vamos avançar com "Programando Bitcoin" no [Capítulo 11: Capacitando Timelock com Scripts no Bitcoin](11_0_Empowering_Timelock_with_Bitcoin_Scripts.md).