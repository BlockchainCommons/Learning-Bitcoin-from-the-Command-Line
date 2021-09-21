# 5.2: Reenviando uma Transação com RBF

Se a nossa transação Bitcoin travar e formos a parte que está enviando o saldo, podemos reenviá-la usando o RBF (Replace-By-Fee). No entanto, isso não é tudo que o RBF pode fazer: Geralmente é um recurso poderoso  que permite aos remetentes do Bitcoin recriar transações por vários motivos.

> :warning: **AVISO DE VERSÃO:** Esta é uma inovação do Bitcoin Core v0.12.0, que atingiu a maturidade total na carteira do Bitcoin Core na versão 0.14.0. Obviamente, a maioria das pessoas já deveria estar usando-a.

## Optando pelo RBF

O RBF é um recurso opcional do Bitcoin. As transações só são elegíveis para usar o RBF se tiverem sido criadas com um sinalizador RBF especial. Isso é feito configurando qualquer um dos números de sequência UTXO da transação (que normalmente são configurados automaticamente), de modo que seja maior que 0 e menor que 0xffffffff-1 (4294967294).

Isso pode ser feito simplesmente adicionando uma variável ```sequence``` às nossas entradas do UTXO:
```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "sequence": 1 } ]''' outputs='''{ "'$recipient'": 0.00007658, "'$changeaddress'": 0.00000001 }''')
```
Obviamente, devemos assinar e enviar nossa transação, como fazemos normalmente:
```
$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375
```
Agora, quando olharmos para nossa transação, devemos ver algo novo: A linha ```bip125-replaceeable```, que sempre foi marcada como ```no``` antes, agora está marcada como ```yes```:
```
$ bitcoin-cli -named gettransaction txid=5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375                                                                     
      
{
  "amount": 0.00000000,
  "fee": -0.00000141,
  "confirmations": 0,
  "trusted": true,
  "txid": "5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375",
  "walletconflicts": [
  ],
  "time": 1592954399,
  "timereceived": 1592954399,
  "bip125-replaceable": "yes",
  "details": [
  ],
  "hex": "02000000000101fa364ad3cbdb08dd0b83aac009a42a9ed00594acd6883d2a466699996cd69d8b01000000000100000002ea1d000000000000160014d591091b8074a2375ed9985a9c4b18efecfd416501000000000000001600146c45d3afa8762086c4bd76d8a71ac7c976e1919602473044022077007dff4df9ce75430e3065c82321dca9f6bdcfd5812f8dc0daeb957d3dfd1602203a624d4e9720a06def613eeea67fbf13ce1fb6188d3b7e780ce6e40e859f275d0121038a2702938e548eaec28feb92c7e4722042cfd1ea16bec9fc274640dc5be05ec500000000"
}
```
O sinalizador ```bip125-replaceeable``` permanecerá como ```yes``` até que a transação receba confirmações. Nesse ponto, ela não é mais substituível.

> :book: ***Devo confiar nas transações   que não possuem confirmações?*** Não, nunca. Isso era uma verdade antes do RBF e continua sendo depois do RBF. As transações devem receber confirmações antes de serem determinadas como confiáveis. Isto é especialmente verdadeiro se uma transação for marcada como ```bip125-replaceable```, porque ela pode muito bem ser... substituída.

> :information_source: **NOTA - SEQUÊNCIA:** Este é o primeiro uso do valor ```nSequence``` no Bitcoin. Podemos configurá-lo entre 1 e 0xffffffff-2 (4294967293) e habilitar o RBF, mas se não tivermos cuidado, poderemos bater de frente com o uso paralelo do ```nSequence``` que serve para _timelocks_ relativos. Sugerimos sempre configurá-lo como "1", que é o que o Bitcoin Core faz, mas a outra opção é configurá-lo com um valor entre 0xf0000000 (4026531840) e 0xffffffff-2 (4294967293). Configurá-lo como sendo "1" efetivamente torna os bloqueios de tempo relativos irrelevantes e configurá-lo para 0xf0000000 ou superior os desativa. Tudo isso é explicado posteriormente na seção [§11.3: Usando CSV nos Scripts](11_3_Using_CSV_in_Scripts.md). Por enquanto, basta escolhermos um dos valores não conflitantes para o ```nSequence```.

### Opcional: Sempre Habilite o RBF

Se preferirmos, podemos optar por _sempre_ habilitar o RBF. Podemos fazer isso executando nosso ```bitcoind``` com o comando ``` -walletrbf```. Depois de fazermos isso (e reiniciarmos nosso ```bitcoind```), todos os UTXOs devem ter um número de sequência inferior e as transações posteriores devem ser marcadas como ```bip125-replaceable```.

> :warning: **AVISO DE VERSÃO:** O sinalizador walletrbf requer o Bitcoin Core v.0.14.0 ou superior.

## Entendendo o Funcionamento do RBF

A funcionalidade RBF é baseada no [BIP 125](https://github.com/bitcoin/bips/blob/master/bip-0125.mediawiki), que lista as seguintes regras para usá-lo:

> 1. As transações originais sinalizam a possibilidade de serem substituídas de maneira explicita ou por herança, conforme descrito na seção anterior.

Isso significa que o número de sequência deve ser definido para menos de 0xffffffff-1 (4294967294), ou o mesmo é verdadeiro para transações pai não confirmadas.

> 2. A transação de substituição paga uma taxa absolutamente mais alta do que a soma paga pelas transações originais.
> 3. A transação de substituição não contém nenhuma nova entrada não confirmada que não apareceu anteriormente na mempool. Entradas não confirmadas são entradas e saídas de gastos de transações atualmente não confirmadas.
> 4. A transação de substituição deve pagar por seu próprio tamanho, além do valor pago pelas transações originais, ou acima da taxa definida pela configuração de taxa de retransmissão mínima do node. Por exemplo, se a taxa mínima de retransmissão for 1 satoshi/byte e a transação de substituição for de 500 bytes no total, a substituição deverá pagar uma taxa pelo menos 500 satoshis superior à soma das transações originais.
> 5. O número de transações originais a serem substituídas e as transações filhas que serão removidas do mempool não deve exceder um total de 100 transações.

> :book: ***O que é um BIP?*** Um BIP é uma proposta de melhoria de Bitcoin (Bitcoin Improvement Proposal). É uma sugestão detalhada para uma mudança no código Bitcoin Core. Frequentemente, quando um BIP foi suficientemente discutido e atualizado, ele se tornará uma parte real do código do Bitcoin Core. Por exemplo, o BIP 125 foi implementado no Bitcoin Core 0.12.0.

A outra coisa a ser entendida sobre o RBF é que, para usá-lo, devemos gastar o dobro, reutilizando um ou mais UTXOs. Apenas enviar outra transação com um UTXO diferente para o mesmo destinatário não resolverá o problema (e provavelmente resultará em perda do saldo). Ao invés disso, devemos criar um conflito proposital, onde o mesmo UTXO é usado em duas transações diferentes.

Diante desse conflito, os mineradores saberão usar aquele com a taxa mais alta e serão incentivados a isso devido a taxa ser mais alta.

> :book: ***O que é um gasto duplo?*** Um gasto duplo ocorre quando alguém envia os mesmos fundos eletrônicos para duas pessoas diferentes (ou, para a mesma pessoa duas vezes, em duas transações diferentes). Este é um problema central para qualquer sistema de dinheiro digital. É resolvido no Bitcoin devido a blockchain imutável: Uma vez que uma transação seja suficientemente confirmada, nenhum minerador irá verificar transações que reutilizam o mesmo UTXO. No entanto, é possível fazer o gasto duplo _antes_ de uma transação ser confirmada. É por isso que sempre desejamos ter uma ou mais confirmações antes de finalizar uma transação. No caso do RBF, podemos propositalmente duplicar o gasto porque uma transação inicial ficou presa, e os mineradores aceitam o nosso gasto duplo se atendermos aos critérios específicos estabelecidos pelo BIP 125.

> :warning: **ATENÇÃO:** Algumas discussões anteriores sobre esta política sugeriram que o número ```nSequence``` também fosse aumentado. Na verdade, esse era o uso pretendido do ```nSequence``` em sua forma original. Isso _não_ faz parte da política publicada no BIP 125. Na verdade, aumentar o número da sequência, pode travar acidentalmente nossa transação com um _timelock_ relativo, a menos que usemos números de sequência no intervalo de 0xf0000000 (4026531840) a 0xffffffff-2 (4294967293) .

## Substituindo uma Transação no Modo Difícil: Manualmente

Para criar uma transação RBF manual, tudo o que precisamos fazer é criar uma transação bruta que: (1) Substitua uma transação bruta anterior onde o RBF foi habilitado e que não foi confirmada; (2) Reutilizar um ou mais dos mesmos UTXOs; (3) Aumentar as taxas e; (4) Pagar a taxa mínima de ambas as transações [que já podem ser atendidas no item (3)].

O exemplo a seguir apenas reutiliza as variáveis ​​existentes, mas diminui o valor enviado para o endereço de troco, para aumentar a taxa de 0 BTC acidental da transação original para uma taxa de 0,01 BTC, excessivamente generosa, diga-se de passagem, nesta nova transação:
```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "sequence": 1 } ]''' outputs='''{ "'$recipient'": 0.000075, "'$changeaddress'": 0.00000001 }''')
```
É claro que devemos assiná-la novamente e reenviá-la:
```
$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b
```
Agora podemos olhar para a nossa transação original e ver se ela tem ```walletconflicts```:
```
$ bitcoin-cli -named gettransaction txid=5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375
{
  "amount": 0.00000000,
  "fee": -0.00000141,
  "confirmations": 0,
  "trusted": false,
  "txid": "5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375",
  "walletconflicts": [
    "c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b"
  ],
  "time": 1592954399,
  "timereceived": 1592954399,
  "bip125-replaceable": "yes",
  "details": [
  ],
  "hex": "02000000000101fa364ad3cbdb08dd0b83aac009a42a9ed00594acd6883d2a466699996cd69d8b01000000000100000002ea1d000000000000160014d591091b8074a2375ed9985a9c4b18efecfd416501000000000000001600146c45d3afa8762086c4bd76d8a71ac7c976e1919602473044022077007dff4df9ce75430e3065c82321dca9f6bdcfd5812f8dc0daeb957d3dfd1602203a624d4e9720a06def613eeea67fbf13ce1fb6188d3b7e780ce6e40e859f275d0121038a2702938e548eaec28feb92c7e4722042cfd1ea16bec9fc274640dc5be05ec500000000"
}
```
Isso representa o fato de que duas transações diferentes estão tentando usar o mesmo UTXO.

Eventualmente, a transação com a taxa maior deve ser aceita:
```
$ bitcoin-cli -named gettransaction txid=c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b
{
  "amount": 0.00000000,
  "fee": -0.00000299,
  "confirmations": 2,
  "blockhash": "0000000000000055ac4b6578d7ffb83b0eccef383ca74500b00f59ddfaa1acab",
  "blockheight": 1773266,
  "blockindex": 9,
  "blocktime": 1592955002,
  "txid": "c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b",
  "walletconflicts": [
    "5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375"
  ],
  "time": 1592954467,
  "timereceived": 1592954467,
  "bip125-replaceable": "no",
  "details": [
  ],
  "hex": "02000000000101fa364ad3cbdb08dd0b83aac009a42a9ed00594acd6883d2a466699996cd69d8b010000000001000000024c1d000000000000160014d591091b8074a2375ed9985a9c4b18efecfd416501000000000000001600146c45d3afa8762086c4bd76d8a71ac7c976e1919602473044022077dcdd98d85f6247450185c2b918a0f434d9b2e647330d741944ecae60d6ff790220424f85628cebe0ffe9fa11029b8240d08bdbfcc0c11f799483e63b437841b1cd0121038a2702938e548eaec28feb92c7e4722042cfd1ea16bec9fc274640dc5be05ec500000000"
}
```
Enquanto isso, a transação original com a taxa mais baixa começa a receber confirmações negativas, para mostrar a divergência com a blockchain:
```
$ bitcoin-cli -named gettransaction txid=5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375
{
  "amount": 0.00000000,
  "fee": -0.00000141,
  "confirmations": -2,
  "trusted": false,
  "txid": "5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375",
  "walletconflicts": [
    "c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b"
  ],
  "time": 1592954399,
  "timereceived": 1592954399,
  "bip125-replaceable": "yes",
  "details": [
  ],
  "hex": "02000000000101fa364ad3cbdb08dd0b83aac009a42a9ed00594acd6883d2a466699996cd69d8b01000000000100000002ea1d000000000000160014d591091b8074a2375ed9985a9c4b18efecfd416501000000000000001600146c45d3afa8762086c4bd76d8a71ac7c976e1919602473044022077007dff4df9ce75430e3065c82321dca9f6bdcfd5812f8dc0daeb957d3dfd1602203a624d4e9720a06def613eeea67fbf13ce1fb6188d3b7e780ce6e40e859f275d0121038a2702938e548eaec28feb92c7e4722042cfd1ea16bec9fc274640dc5be05ec500000000"
}
```
Nossos destinatários terão nosso saldo, e a transação original que foi falha acabará saindo do mempool.

## Substituindo uma Transação no Modo Fácil: Usando o bumpfee

As transações brutas são muito poderosas e podemos fazer muitas coisas interessantes combinando-as com o RBF. No entanto, às vezes _tudo_ o que desejamos é apenas liberar uma transação que está presa na mempool. Podemos fazer isso com um comando simples, o ```bumpfee```.

Por exemplo, para aumentar a taxa da transação ```4460175e8276d5a1935f6136e36868a0a3561532d44ddffb09b7cb878f76f927```, executaríamos o seguinte comando:
```
$ bitcoin-cli -named bumpfee txid=4460175e8276d5a1935f6136e36868a0a3561532d44ddffb09b7cb878f76f927
{
  "txid": "75208c5c8cbd83081a0085cd050fc7a4064d87c7d73176ad9a7e3aee5e70095f",
  "origfee": 0.00000000,
  "fee": 0.00022600,
  "errors": [
  ]
}
```
O resultado é a geração automática de uma nova transação que tem uma taxa determinada pelo arquivo ```bitcoin.conf```:
```
$ bitcoin-cli -named gettransaction txid=75208c5c8cbd83081a0085cd050fc7a4064d87c7d73176ad9a7e3aee5e70095f
{
  "amount": -0.10000000,
  "fee": -0.00022600,
  "confirmations": 0,
  "trusted": false,
  "txid": "75208c5c8cbd83081a0085cd050fc7a4064d87c7d73176ad9a7e3aee5e70095f",
  "walletconflicts": [
    "4460175e8276d5a1935f6136e36868a0a3561532d44ddffb09b7cb878f76f927"
  ],
  "time": 1491605676,
  "timereceived": 1491605676,
  "bip125-replaceable": "yes",
  "replaces_txid": "4460175e8276d5a1935f6136e36868a0a3561532d44ddffb09b7cb878f76f927",
  "details": [
    {
      "account": "",
      "address": "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi",
      "category": "send",
      "amount": -0.10000000,
      "vout": 0,
      "fee": -0.00022600,
      "abandoned": false
    }
  ],
  "hex": "02000000014e843e22cb8ee522fbf4d8a0967a733685d2ad92697e63f52ce41bec8f7c8ac0020000006b48304502210094e54afafce093008172768d205d99ee2e9681b498326c077f0b6a845d9bbef702206d90256d5a2edee3cab1017b9b1c30b302530b0dd568e4af6f2d35380bbfaa280121029f39b2a19943fadbceb6697dbc859d4a53fcd3f9a8d2c8d523df2037e7c32a71010000000280969800000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac38f25c05000000001976a914c101d8c34de7b8d83b3f8d75416ffaea871d664988ac00000000"
}
```
> :aviso: **AVISO DE VERSÃO:** O comando ```bumpfee``` no RPC requer o Bitcoin Core v.0.14.0.

## Resumo: Reenviando uma Transação com RBF

Se uma transação ficar presa na mempool e não quisermos esperar que ela expire, e se habilitamos o RBF nela, podemos fazer um gasto duplo usando o RBF para criar uma transação de substituição (ou apenas usar o comando ```bumpfee```).

> :fire: ***Qual é o poder do RBF?*** Obviamente, o RBF é muito útil se criarmos uma transação com uma taxa muito baixa e precisamos acelerá-la. No entanto, a capacidade de substituir transações não confirmadas por transações atualizadas tem mais poder do que apenas isso (e é por isso que podemos querer continuar usando o RBF com transações brutas, mesmo após sabermos que existe o ```bumpfee```).

> Por exemplo, podemos enviar uma transação e, antes de ser confirmada, combiná-la com uma segunda transação. Isso permite que possamos comprimir várias transações em uma única, diminuindo as taxas totais. Também podemos oferecer benefícios à privacidade. Existem outras razões para usarmos o RBF, como por exemplo contratos inteligentes ou transferência de transações, conforme descrito na parte referente a [Perguntas frequentes sobre RBF de opt-in](https://bitcoincore.org/en/faq/optin_rbf/).

## O Que Vem Depois?

Vamos continuar "Controlando Transações no Bitcoin" na seção [§5.3: Financiando uma Transação com CPFP](05_3_Funding_a_Transaction_with_CPFP.md).
