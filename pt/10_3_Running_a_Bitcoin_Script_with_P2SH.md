# 10.3: Executando um Script no Bitcoin com P2SH

Agora que conhecemos a teoria e a prática por trás dos endereços P2SH, estamos prontos para transformar um script de Bitcoin não-padrão em uma transação real. Vamos utilizar o script de bloqueio simples da seção [§9.2: Executando um Script no Bitcoin](09_2_Running_a_Bitcoin_Script.md), `OP_ADD 99 OP_EQUAL`.

## Criando uma Transação P2SH

Para bloquear uma transação com este script, precisamos fazer o seguinte:

1. Serializar o `OP_ADD 99 OP_EQUAL`:
   1. OP_ADD = 0x93 - uma tradução simples do opcode;
   2. 99 = 0x01, 0x63 - este opcode coloca um byte na pilha, 99 (hex: 0x63);
      * Não se preocupe com a conversão porque é apenas um byte.
   3. OP_EQUAL = 0x87 - uma tradução simples do opcode;
   4. ```<serialized99Equal>``` = "93016387".

```
$ btcc OP_ADD 99 OP_EQUAL
93016387
```
2. Vamos salvar o ```<serialized99Equal>``` para referência futura como ```redeemScript```;
   1. `<redeemScript>` = "93016387".
3. SHA-256 e RIPEMD-160 do hash do script serializado;
   1. ```<hashed99Equal>``` = "3f58b4f7b14847a9083694b9b3b52a4cea2569ed".
4. Produzir um script de bloqueio P2SH que inclua o ```<hashed99Equal>```;
   1. ```scriptPubKey``` ="a9143f58b4f7b14847a9083694b9b3b52a4cea2569ed87".

Podemos então criar uma transação usando o ```scriptPubKey```, provavelmente através de uma API.

## Desbloqueando a Transação P2SH

Para desbloquear essa transação, é necessário que o destinatário produza um ```scriptSig``` que acrescente duas constantes, totalizando noventa e nove, ao script serializado:```1 98 <serialized99Equal>```.

### Executando a Primeira Rodada de Validação

O processo de desbloqueio da transação P2SH começa com uma primeira rodada de validação, que nada mais é que a verificação se o script de resgate corresponde ao valor hash no script de bloqueio.

Vamos concatenar o ```scriptSig``` e o ```scriptPubKey``` e executá-los, normalmente:
```
Script: 1 98 <serialized99Equal> OP_HASH160 <hashed99Equal> OP_EQUAL
Stack: []

Script: 98 <serialized99Equal> OP_HASH160 <hashed99Equal> OP_EQUAL
Stack: [ 1 ]

Script: <serialized99Equal> OP_HASH160 <hashed99Equal> OP_EQUAL
Stack: [ 1 98 ]

Script: OP_HASH160 <hashed99Equal> OP_EQUAL
Stack: [ 1 98 <serialized99Equal> ]

Script: <hashed99Equal> OP_EQUAL
Running: <serialized99Equal> OP_HASH160
Stack: [ 1 98 <hashed99Equal> ]

Script: OP_EQUAL
Stack: [ 1 98 <hashed99Equal> <hashed99Equal> ]

Script: 
Running: <hashed99Equal> <hashed99Equal> OP_EQUAL
Stack: [ 1 98 True ]
```
O script termina com um ```True``` no topo da pilha e, portanto, foi bem-sucedido... embora haja outro fragmento abaixo dele.

Porém, por se tratar de um script P2SH, a execução não está concluída.

### Executando a Segunda Rodada de Validação

Para a segunda rodada de validação, vamos verificar se os valores no script de desbloqueio satisfazem o ```redeemScript```: desserializando o ```redeemScript``` ("93016387" = "OP_ADD 99 OP_EQUAL") e executando-o usando os itens no ```scriptSig``` anterior para o script serializado:

```
Script: 1 98 OP_ADD 99 OP_EQUAL
Stack: [ ]

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
Com essa segunda validação _também_ verdadeira, o UTXO agora pode ser gasto!

## Resumo: Executando um Script no Bitcoin com P2SH

Depois de conhecer a técnica de construção dos P2SH, qualquer script pode ser embutido em uma transação Bitcoin, e depois de entender a técnica de validação do P2SH, é fácil executar os scripts em duas fases.

## O Que Vem Depois?

Vamos continuar "Incorporando Scripts em Transações P2SH no Bitcoin" na seção [§10.4: Programando um Multisig](10_4_Scripting_a_Multisig.md).