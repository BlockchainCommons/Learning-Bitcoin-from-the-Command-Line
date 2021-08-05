# 13.2: Escrevendo Scripts Multisig Complexos

Até o momento, os multisigs descritos nesses documentos têm sido inteiramente simples, na forma m-de-n ou n-de-n. No entanto, podemos desejar multisigs mais complexos, onde as pessoas que assinam as transações variam ou onde diferentes opções podem se tornar disponíveis ao longo do tempo.

## Escrevendo uma Multisig Variável

Uma multisig variável requer diferentes números de pessoas para assinar, dependendo de quem está assinando.

### Escrevendo uma Multisig com um único assinante ou com vários assinantes

Imagine uma empresa em que o presidente ou dois em cada três vice-presidentes concordem com o uso dos fundos.

Podemos escrever isso criando uma instrução ```IF```/```ELSE```/```ENDIF``` que tem dois blocos, um para o presidente e sua assinatura um-de-um e um para os vice-presidentes e suas dois-de-três assinaturas. Podemos então, determinar qual bloco usar com base em quantas assinaturas estão no script de desbloqueio. Se usarmos ```OP_DEPTH 1 OP_EQUAL``` estaremos dizendo que há um item na pilha, e iremos partir daí.

O script de bloqueio completo seria ```OP_DEPTH 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF```.

Se executado pelo presidente, seria assim:
```
Script: <sigPres> OP_DEPTH 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Stack: [ ]

Script: OP_DEPTH 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Stack: [ <sigPres> ]

Script: 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Running: <SigPres> OP_DEPTH
Stack: [ <sigPres> 1 ]

Script: OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Stack: [ <sigPres> 1 1 ]

Script: IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Running: 1 1 OP_EQUAL
Stack: [ <sigPres> True ]
```
Como o resultado é `True`, o script agora se reduz para a instrução ```IF```:
```
Script: <pubKeyPres> OP_CHECKSIGNATURE
Running: True IF
Stack: [ <sigPres> ]

Script: OP_CHECKSIGNATURE
Stack: [ <sigPres> <pubKeyPres> ]

Script: 
Running: <sigPres> <pubKeyPres> OP_CHECKSIGNATURE
Stack: [ True ]
```
Se administrado por dois vice-presidentes, ficaria assim:
```
Script: 0 <sigVPA> <sigVPB> OP_DEPTH 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Stack: [ ]

Script: OP_DEPTH 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Stack: [ 0 <sigVPA> <sigVPB> ]

Script: 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Running: 0 <sigVPA> <sigVPB> OP_DEPTH
Stack: [ 0 <sigVPA> <sigVPB> 3 ]

Script: OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Stack: [ 0 <sigVPA> <sigVPB> 3 1 ]

Script: IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Running: 3 1 OP_EQUAL
Stack: [ 0 <sigVPA> <sigVPB> False ]
```
Como o resultado é `False`, o script agora se reduz à instrução ```ELSE```:
```
Script: 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG
Running: False IF
Stack: [ 0 <sigVPA> <sigVPB> ]

Script: OP_CHECKMULTISIG
Stack: [ 0 <sigVPA> <sigVPB> 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 ]

Script: 
Running: 0 <sigVPA> <sigVPB> 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG
Stack: [ ]
```
Podemos notar que a assinatura do presidente usa apenas um simples ```OP_CHECKSIGNATURE``` ao invés do código mais complexo normalmente exigido para um P2PKH. Podemos nos safar incluindo a chave pública no script de bloqueio, evitando um trabalhão padrão, porque é o hash e não será revelado (por meio do ```redeemScript```) até que a transação seja desbloqueada. Isso também permite que todos os possíveis assinantes assinem usando a mesma metodologia.

O único problema é se o presidente estiver distraído e acidentalmente assinar uma transação com um dos vice-presidentes, porque ele se lembra de ser um multisig 2 de 3. Uma opção é decidir que essa é uma condição de falha aceitável, porque o presidente está usando o multsig incorretamente. Outra opção é transformar o multisig 2 de 3 em um multisig 2 de 4, caso o presidente não tolere falhas: ```OP_DEPTH 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> < pubKeyVPC> <pubKeyPres> 4 OP_CHECKMULTISIG ENDIF```. Isso permitiria ao presidente assinar por engano com qualquer vice-presidente, mas não afetaria as coisas se dois vice-presidentes quisessem assinar (corretamente).

### Escrevendo um Multisig com um assinante obrigatório

Outra possibilidade de multisig envolve ter um multisig m-of-n onde um dos assinantes é necessário. Isso geralmente pode ser gerenciado dividindo o multisig em vários m de n-1 multisigs. Por exemplo, um multisig 2 de 3 em que um dos signatários é necessário seria, na verdade, dois multisig 2 de 2, cada um incluindo o signatário necessário.

Esta é uma maneira simples de criar um script para isso:
```
OP_3DUP
2 <pubKeyRequired> <pubKeyA> 2  OP_CHECKMULTISIG
NOTIF
  2 <pubKeyRequired> <pubKeyB> 2  OP_CHECKMULTISIG
ENDIF
```
O script de desbloqueio seria ```0 <pubKeyRequired> <pubKeyA>``` ou ```0 <pubKeyRequired> <pubKeyB>```.

Primeiro, o script verifica as assinaturas no ```<pubKeyRequired> <pubKeyA>```. Se falhar, será verificado no ```<pubKeyRequired> <pubKeyB>```.

O resultado do ```OP_CHECKMULTISIG``` final que foi executado será deixado no topo da pilha (embora haja um resto abaixo dele se o primeiro for bem-sucedido).

## Escrevendo um Escrow Multisig

Falamos muito sobre escrows. Os multisigs complexos combinados com timelocks oferecem uma maneira automatizada de criá-los de maneira robusta.

Imagine a compradora de imóveis Alice e o vendedor de imóveis Bob que estão trabalhando com um agente de garantia. A maneira fácil de fazer o script seria como um multisig onde qualquer uma das três partes poderia liberar o dinheiro: Ou o vendedor e o comprador concordam ou o agente de garantia assume o controle e concorda com uma das partes: ```2 <pubKeyA> <pubKeyB> <pubKeyEscrow> 3 OP_CHECKMULTISG```.

No entanto, isso enfraquece o poder do agente de garantia e permite que o vendedor e o comprador acidentalmente tomem uma decisão errada entre eles. O que é uma das coisas que um sistema de garantia deve evitar. Portanto, pode ser que o que realmente desejamos seja o sistema que acabamos de criar, onde o agente de custódia é uma parte necessária no multisig 2 de 3: ```OP_3DUP 2 <pubKeyEscrow> <pubKeyA> 2 OP_CHECKMULTISIG NOTIF 2 <pubKeyEscrow > <pubKeyB> 2 OP_CHECKMULTISIG ENDIF```.

No entanto, isso não passa no teste de entrar na frente de um ônibus. Se o agente de custódia morre ou foge para as Bahamas durante o processo, o comprador e o vendedor ficam sem o dinheiro. É aqui que entra um bloqueio de tempo. Podemos criar um teste adicional que só será executado se tivermos passado o final do nosso período de garantia. Nessa situação, permitimos que o comprador e o vendedor assinem juntos:
```
OP_3DUP
2 <pubKeyEscrow> <pubKeyA> 2 OP_CHECKMULTISIG
NOTIF
  OP_3DUP
  2 <pubKeyEscrow> <pubKeyB> 2 OP_CHECKMULTISIG
  NOTIF
    <+30Days> OP_CHECKSEQUENCEVERIFY OP_DROP
    2 <pubKeyA> <pubKeyB> 2 OP_CHECKMULTISIG
  ENDIF
ENDIF
```
Primeiro, testamos uma assinatura para o comprador e o agente de garantia e, em seguida, uma assinatura para o vendedor e o agente de garantia. Se ambos falharem e 30 dias se passaram, também permitimos uma assinatura para o comprador e o vendedor.

### Escrevendo um Multisig de Compromisso Centrado no Comprador 

O [BIP 112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki#Escrow_with_Timeout) oferece um exemplo diferente deste tipo de garantia que não tem proteções extras para evitar o agente de garantia, mas que dá a Alice controle total se a garantia falhar.
```
IF
    2 <pubKeyA> <pubKeyB> <pubKeyEscrow> 3 OP_CHECKMULTISIG 
ELSE
    <+30Days> OP_CHECKSEQUENCEVERIFY OP_DROP
    <pubKeyA> OP_CHECKSIGNATURE
ENDIF
```
Aqui, qualquer um dos três assinantes pode liberar o dinheiro a qualquer momento, mas depois de 30 dias, Alice pode recuperar o dinheiro por conta própria.

Observe que este script requer que um ```True``` ou ```False``` seja passado para identificar qual caminho será utilizado. Esta é uma maneira mais simples e menos computacionalmente intensiva de oferecer suporte aos caminhos usando o script de Bitcoin. É algo bem comum.

No início, o seguinte ```sigScript``` seria permitido: ```0 <signer1> <signer2> True```. Após 30 dias, Alice poderia produzir um ```sigScript``` como este: ```<sigA> False```.

## Resumo: Escrevendo Scripts Multisig Complexos

Os multisigs mais complexos podem ser normalmente criados combinando assinaturas ou multi assinaturas com condicionais e testes. Os multisigs resultantes podem ser variáveis, exigindo diferentes números de assinantes com base em quem são e quando estão assinando.

> :fire: ***Qual é o poder dos scripts complexos com multisigs?*** Mais do que qualquer coisa que vimos até agora, os scripts multisig complexos são contratos realmente inteligentes. Eles podem ser muito precisos sobre quem tem permissão para assinar e quando. Empresas com vários níveis, parcerias e escrows podem utilizar soluções semelhantes. O uso de outros recursos poderosos como os timelocks pode proteger ainda mais esses fundos, permitindo que sejam liberados ou mesmo devolvidos em determinados momentos.

## O Que Vem Depois?

Vamos continuar "Projetando Scripts Reais no Bitcoin" na seção [§13.3: Expandindo o Bitcoin com os Scripts] (13_3_Empowering_Bitcoin_with_Scripts.md).