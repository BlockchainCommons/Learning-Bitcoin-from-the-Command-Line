# 13.3: Capacitando o Bitcoin com Scripts

Os scripts do Bitcoin podem ir muito além dos instrumentos financeiros relativamente simples detalhados até o momento. Eles também são a base dos usos mais complexos da rede Bitcoin, conforme demonstrado por esses exemplos do mundo real de funcionalidade fora da blockchain (off-chain), extraídos dos exemplos da Lightning Network no [BIP 112](https://github.com/bitcoin /bips/blob/master/bip-0112.mediawiki).

## Bloqueio para a Lightning Network

A [Lightning Network](https://rusty.ozlabs.org/?p=450) é um canal de pagamento que permite aos usuários retirar fundos off-chain e se envolver em várias micro-transações antes de finalizar o canal de pagamento e trazer os fundos de volta para o Bitcoin. Os benefícios incluem taxas mais baixas e velocidades de transação mais rápidas. Iremos discutir em mais detalhes, com exemplos de como utilizá-la usando a linha de comando, começando no [Capítulo 19](19_0_Understanding_Your_Lightning_Setup.md).

O [BIP 112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki) contém alguns exemplos de como essas transações off-chain poderiam ser geradas, usando scripts de bloqueio do Bitcoin.

### Bloqueio com Transações de Compromisso Revogáveis

O truque com a Lightning é o fato de ela estar off-chain. Para usar a Lightning, os participantes bloqueiam fundos em conjunto na blockchain do Bitcoin com uma transação multisig n-de-n. Em seguida, eles se envolvem em uma série de transações entre si. Cada nova "transação de compromisso" (commitment transaction) divide esses fundos de uma maneira diferente. Essas transações são parcialmente assinadas, mas _não são colocadas na blockchain_.

Se temos uma massa de transações não publicadas, qualquer uma delas _pode_ ser colocada na Blockchain, portanto, como evitamos que um dos participantes reverta a uma transação antiga que é mais benéfica para eles? A resposta é _revogação_. Um exemplo simplificado no BIP 112, que oferece um dos trampolins para a Lightning, mostra o processo. Nós damos ao participante que seria prejudicado pela reversão de uma transação revogada a capacidade de retirar os fundos ele mesmo se o outro participante tentar usar ilegitimamente a transação revogada.

Por exemplo, suponha que Alice e Bob atualizem a transação de compromisso para dar mais fundos a Bob (ou seja, Alice enviou fundos a Bob por meio da rede Lightning). Eles assinam parcialmente novas transações, mas  cada um também oferece seu próprio ```revokeCode``` para as transações anteriores. Isso efetivamente garante que nenhum deles irão publicar as transações anteriores, porque isso permitiria que a contraparte reivindicasse os fundos anteriores.

Então, como se parece a transação antiga? Foi uma transação de compromisso mostrando fundos destinados a Alice, antes que ela os desse a Bob. Ele tinha um script de bloqueio da seguinte maneira:
```
OP_HASH160 
<revokeHash> 
OP_EQUAL
IF 
    <pubKeyBob>
ELSE
    <+24Hours> 
    OP_CHECKSEQUENCEVERIFY 
    OP_DROP
    <pubKeyAlice>
ENDIF
OP_CHECKSIG
```
O bloco ```ELSE``` é onde Alice conseguiu os fundos, após um atraso de 24 horas. No entanto, agora foi substituído. Afinal, esse é o ponto principal de um canal de pagamento na Lightning. Nesta situação, esta transação nunca deve ser publicada. Bob não tem incentivo para isso porque ele tem uma transação mais recente, que o beneficia mais porque ele recebeu parte dos fundos de Alice. Alice também não tem incentivo, porque ela perde o dinheiro se tentar retirar mais do que deve, por conta do ```revokeCode```. Assim, ninguém coloca a transação na blockchain e as transações off-chain continuam.

Vale a pena explorar como esse script funcionaria em uma variedade de situações, a maioria das quais envolve Alice tentando trapacear, revertendo para a transação mais antiga, que descreve os fundos _antes_ da Alice enviar os satoshis para Bob.

#### Executando o Script de Bloqueio para a Alice Trapaceira, com Código de Revogação

Alice poderia tentar usar o código de revogação que ela deu a Bob para reivindicar imediatamente os fundos. Ela escreve um script de bloqueio de ```<sigAlice> <revokeCode>```:
```
Script: <sigAlice> <revokeCode> OP_HASH160 <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ ]

Script: OP_HASH160 <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ <sigAlice> <revokeCode> ]

Script: <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Running: <revokeCode> OP_HASH160 
Stack: [ <sigAlice> <revokeHash> ]

Script: OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ <sigAlice> <revokeHash> <revokeHash> ]

Script: IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Running: <revokeHash> <revokeHash> OP_EQUAL
Stack: [ <sigAlice> True ]
```
O ```OP_EQUAL``` alimenta a instrução ```IF```. Como Alice usa o ```revokeCode```, ela entra no caminho que permite gatar os fundos imediatamente, reduzindo o resto do script para ```<pubKeyBob>``` (dentro do condicional) e com o ```OP_CHECKSIG``` (depois).
```
Script: <pubKeyBob> OP_CHECKSIG
Running: True IF
Stack: [ <sigAlice> ]
```
Maldição! Apenas Bob pode assinar imediatamente usando o ```redeemCode```!
```
Script:  OP_CHECKSIG
Stack: [ <sigAlice> <pubKeyBob> ]

Script:
Running: <sigAlice> <pubKeyBob> OP_CHECKSIG
Stack: [ False ]
```
#### Executando o Script de Bloqueio para a Alice Trapaceira, sem Código de Revogação

E se Alice tentar usar sua própria assinatura, sem o ```revokeCode```? Ela usa um script de desbloqueio de ```<sigAlice> <notRevokeCode>```.
```
Script: <sigAlice> 0 OP_HASH160 <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ ]

Script: OP_HASH160 <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ <sigAlice> 0 ]

Script: <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Running: 0 OP_HASH160 
Stack: [ <sigAlice> <0Hash> ]

Script: OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ <sigAlice> <0Hash> <revokeHash> ]

Script: IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Running: <0Hash> <revokeHash> OP_EQUAL
Stack: [ <sigAlice> False ]
```
Agora reduzimos para a instrução ```ELSE``` e o que vem depois do condicional:
```
Script: <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> OP_CHECKSIG
Running: False IF
Stack: [ <sigAlice> ]

Script: OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> OP_CHECKSIG
Stack: [ <sigAlice> <+24Hours> ]
```
E então Alice é frustrada de novo porque 24 horas não se passaram!
```
Script: OP_DROP <pubKeyAlice> OP_CHECKSIG
Running: <+24Hours> OP_CHECKSEQUENCEVERIFY 
Stack: [ <sigAlice> <+24Hours> ] — Script EXITS
```
#### Executando o Script de Bloqueio para o Bob Vitimizado

O que isso significa é que Bob tem 24 horas para recuperar os fundos se Alice tentar trapacear, usando o ```<revokeCode>``` e a assinatura como script de desbloqueio:
```
Script: <SigBob> <revokeCode> OP_HASH160 <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ ]

Script: OP_HASH160 <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ <SigBob> <revokeCode> ]

Script: <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Running: <revokeCode> OP_HASH160 
Stack: [ <SigBob> <revokeHash> ]

Script: OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ <SigBob> <revokeHash> <revokeHash> ]

Script: IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Running: <revokeHash> <revokeHash> OP_EQUAL
Stack: [ <SigBob> True ]

Script: <pubKeyBob> OP_CHECKSIG
Running: True IF
Stack: [ <SigBob> ]

Script:  OP_CHECKSIG
Stack: [ <SigBob> <pubKeyBob> ]

Script:
Running: <SigBob> <pubKeyBob> OP_CHECKSIG
Stack: [ True ]
```
#### Executando o Script de Bloqueio para a Alice Virtuosa

Todas as transações de confirmação da Alice são bloqueadas com este mesmo script de bloqueio, quer tenham sido revogadas ou não. Isso significa que a transação de confirmação mais recente, que é a válida atualmente, também está bloqueada com ela. Alice nunca enviou uma transação mais recente para Bob e, portanto, nunca enviou a ele o ```revokeCode``` anterior.

Nessa situação, ela poderia publicar virtuosamente a transação, fechando o canal. Ela coloca a transação na blockchain e espera 24 horas. Bob não pode fazer nada a respeito porque não possui o código de recuperação. Então, após a espera, Alice recupera os fundos. Bob pode fazer a mesma coisa com sua própria transação final.

### Bloqueando com Contratos de Bloqueio de Tempo em Hash

As Transações de Compromisso Revogável foram apenas um trampolim para a Lightning. A Lightning Network real usa um mecanismo mais complexo chamado [Contrato de Bloqueio de Tempo em Hash (ou no inglês, Hashed TimeLock Contract)](https://en.bitcoin.it/wiki/Hashed_Timelock_Contracts), mais conhecido como HTLC.

O principal objetivo dos HTLCs é criar uma rede abrangente de participantes. As transações não são mais apenas entre um par de participantes que entraram na rede juntos, mas agora podem ser várias pessoas que não estão associadas diretamente. Quando os fundos são enviados, uma string de transações é criada, cada uma delas bloqueada com um ```secretHash```. Quando o ```secretCode``` correspondente é revelado, toda a sequência de transações pode ser gasta. Isso é o que permite que transações singulares realmente se tornem uma rede.

Também há um pouco mais de complexidade nos scripts de bloqueio da Lightning Network. Existem bloqueios separados para o remetente e o destinatário de cada transação que são mais amplamente divergentes do que as diferentes transações de compromisso mencionadas na seção anterior. Vamos mostrar os dois, para demonstrar o poder desses scripts de bloqueio, mas não vamos explicar como eles interagem entre si.

#### Bloqueando a Transação do Destinatário

Mais uma vez, vamos começar a olhar para a transação de compromisso da Alice, que mostra os fundos que ela recebeu:
```
OP_HASH160 
OP_DUP 
<secretHash> 
OP_EQUAL
IF
    <+24Hours>
    OP_CHECKSEQUENCEVERIFY
    OP_2DROP
    <pubKeyAlice>  
ELSE
    <revokeHash> 
    OP_EQUAL
    OP_NOTIF 
        <Date> 
        OP_CHECKLOCKTIMEVERIFY 
        OP_DROP
    ENDIF 
    <pubKeyBob>
ENDIF
OP_CHECKSIG
```
A chave para esses novos HTLCs é o ```secretHash```, que dissemos que é o que permite que uma transação se estenda pela rede. Quando a transação vai do remetente ao destinatário pretendido, o ```secretCode``` é revelado, o que permite que todos os participantes criem um ```secretHash``` e desbloqueiem toda a rede de pagamentos.

Após o ```secretCode``` ter sido revelado, o caminho do ```IF``` é utilizado. Alice pode reivindicar os fundos 24 horas após a transação ser colocada na rede Bitcoin.

No entanto, também há a oportunidade de Bob recuperar os fundos, que aparecem no caminho do ```ELSE```. Ele pode fazer isso se a transação foi revogada (mas Alice a coloca na blockchain de qualquer maneira), _ou se_ um tempo absoluto se esgotar (timeout).

#### Bloqueando a Transação do Remetente

Este é o script alternativo de bloqueio da transação de confirmação usado pelo remetente:
```
OP_HASH160 
OP_DUP 
<secretHash>
OP_EQUAL
OP_SWAP 
<revokeHash> 
OP_EQUAL 
OP_ADD
IF
    <pubKeyAlice>
ELSE
    <Date>
    OP_CHECKLOCKTIMEVERIFY
    <+24Hours>
    OP_CHECKSEQUENCEVERIFY
    OP_2DROP
    <pubKeyBob>
ENDIF
OP_CHECKSIG
```
A parte inicial do script é bastante inteligente e vale a pena executá-la:
```
Initial Script: <suppliedCode> OP_HASH160 OP_DUP <secretHash> OP_EQUAL OP_SWAP <revokeHash> OP_EQUAL OP_ADD
Stack: [ ]

Initial Script: OP_HASH160 OP_DUP <secretHash> OP_EQUAL OP_SWAP <revokeHash> OP_EQUAL OP_ADD
Stack: [ <suppliedCode> ]

Initial Script: OP_DUP <secretHash> OP_EQUAL OP_SWAP <revokeHash> OP_EQUAL OP_ADD
Running: <suppliedCode> OP_HASH160 
Stack: [ <suppliedHash> ]

Initial Script: <secretHash> OP_EQUAL OP_SWAP <revokeHash> OP_EQUAL OP_ADD
Running: <suppliedHash> OP_DUP 
Stack: [ <suppliedHash> <suppliedHash> ]

Initial Script: OP_EQUAL OP_SWAP <revokeHash> OP_EQUAL OP_ADD
Stack: [ <suppliedHash> <suppliedHash> <secretHash> ]

Initial Script: OP_SWAP <revokeHash> OP_EQUAL OP_ADD
Running: <suppliedHash> <secretHash> OP_EQUAL
Stack: [ <suppliedHash> <wasItSecretHash?> ]

Initial Script: <revokeHash> OP_EQUAL OP_ADD
Running: <suppliedHash> <wasItSecretHash?> OP_SWAP
Stack: [ <wasItSecretHash?> <suppliedHash> ]

Initial Script: OP_EQUAL OP_ADD
Stack: [ <wasItSecretHash?> <suppliedHash> <revokeHash> ]

Initial Script: OP_ADD
Running: <suppliedHash> <revokeHash> OP_EQUAL
Stack: [ <wasItSecretHash?> <wasItRevokeHash?> ]

Initial Script: 
Running: <wasItSecretHash?> <wasItRevokeHash?> OP_ADD
Stack: [ <wasItSecretOrRevokeHash?> ]
```
A execução do script revela que as verificações iniciais, acima do ```IF```/```ELSE```/```ENDIF```, determinam se o hash era _tanto_ o ```secretCode``` _ou_ o ```revokeCode```. Nesse caso, Alice pode pegar os fundos no primeiro bloco. Do contrário, Bob pode pegar os fundos, mas somente depois que Alice tiver sua chance e depois que o tempo limite de 24 horas e o tempo limite absoluto tiverem se esgotado.

#### Compreendendo os HTLCs

Os HTLCs são bastante complexos e esta seção não tenta explicar todas as suas complexidades. A [visão geral](https://rusty.ozlabs.org/?p=462) escrita por Rusty Russell explica bem mais, e há ainda mais detalhes no seu artigo [Deployable-lightning](https://github.com/ElementsProject/lightning/blob/master/doc/deployable-lightning.pdf). Mas não se preocupe se algumas das complexidades ainda não ficaram claras para você, especialmente sobre o funcionamento das inter-relações dos dois scripts.

Para os fins deste tutorial, existem duas lições importantes em relação aos HTLCs:

   * Entender que uma estrutura muito complexa como um HTLC pode ser criada usando o Bitcoin Script;
   * Analisar como executar cada um dos scripts HTLC.
   
Vale a pena executar cada um dos dois scripts HTLC em cada uma de permutações, um item da pilha de cada vez.

## Resumo: Capacitando o Bitcoin com Scripts

Estamos encerrando o exame dos scripts do Bitcoin com uma visão de como eles podem ser realmente poderosos. Com 20 opcodes ou menos, um script Bitcoin pode formar a base de todo um canal de pagamento fora da blockchain. Da mesma forma, sidechains com conexões bidirecionais são o produto de menos de vinte opcodes, como também observado brevemente no [BIP 112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki).

Se você já viu funcionalidades complexas do Bitcoin ou sistemas adjacentes ao Bitcoin, eles provavelmente foram construídos em Bitcoin Script. E agora temos todas as ferramentas para fazer o mesmo.

## O Que Vem Depois?

Vamos ficar "Usando o Tor" no [Capítulo Quatorze: Usando o Tor](14_0_Using_Tor.md).

Ou, se preferir, existem dois caminhos alternativos:

Se quiser manter o foco no Bitcoin, siga em frente para "Programando com RPC" no [Capítulo Dezesseis: Conversando com Bitcoind com C](16_0_Talking_to_Bitcoind.md).

Ou, se quiser manter o foco na linha de comando porque não é um programador, pode pular para o [Capítulo Dezenove: Compreendendo Nossa Configuração da Lightning](19_0_Understanding_Your_Lightning_Setup.md) para continuar seus estudos da linha de comando com a Lightning Network.