# 10.4: Programando um Multisig

Antes de encerrarmos esta introdução ao script P2SH, vale a pena examinar um exemplo mais realista. Desde a seção [§6.1](06_1_Sending_a_Transaction_to_a_Multisig.md), dissemos casualmente que a interface ```bitcoin-cli``` envolve nossa transação multisig em uma transação P2SH. Na verdade, esta é a metodologia padrão para a criação dos multisigs na Blockchain do Bitcoin. Veja como isso funciona, com mais detalhes.

## Compreendendo o Código Multisig

As transações multisig são criadas no Bitcoin usando o código ```OP_CHECKMULTISIG```. O ```OP_CHECKMULTISIG``` espera uma longa sequência de argumentos que se parece com isto: ```0 ... sigs ... <m> ... endereços ... <n> OP_CHECKMULTISIG```. Quando o ```OP_CHECKMULTISIG``` é executado, ele faz o seguinte:

1. Retira o primeiro valor da pilha (```<n>```);
2. Retira "n" valores da pilha como endereços Bitcoin (hash de chaves públicas);
3. Retira o próximo valor da pilha (```<m>```);
4. Retira "m" valores da pilha como assinaturas potenciais;
5. Retira um `0` da pilha devido a um erro na codificação original;
6. Compara as assinaturas com os endereços de Bitcoin;
7. Coloca ```True``` ou ```False``` dependendo do resultado.

Os operandos do ```OP_MULTISIG``` são tipicamente divididos, com o ```0``` e as assinaturas vindo do script de desbloqueio e o "m", "n" e endereços sendo detalhados pelo script de bloqueio.

O requisito para que o ```0``` seja o primeiro operando para o ```OP_CHECKMULTISIG``` é uma regra de consenso. Como a versão original do ```OP_CHECKMULTISIG``` acidentalmente retirou um item extra da pilha, o Bitcoin deve seguir esse padrão para sempre, para que os scripts de resgate complexos daquele período de tempo não sejam acidentalmente quebrados, tornando fundos antigos irrecuperáveis.

> :book: ***O que é uma regra de consenso?*** Estas são as regras que os nodes do Bitcoin seguem para trabalharem juntos. Em grande parte, eles são definidos pelo código do Bitcoin Core. Essas regras incluem muitas regras óbvias, como o limite de quantos Bitcoins são criados para cada bloco e as regras de como as transações podem ser respondidas. No entanto, eles também incluem correções para bugs que apareceram ao longo dos anos, porque uma vez que um bug foi introduzido na base de código do Bitcoin, ele deve ser continuamente suportado, para que os antigos bitcoins não se tornem impossíveis de serem gastos.

## Criando um Multisig Bruto

Conforme discutido na seção [§10.2: Construindo a Estrutura do P2SH](10_2_Building_the_Structure_of_P2SH.md), os multisigs são um dos tipos de transação padrão do Bitcoin. Uma transação pode ser criada com um script de bloqueio que usa o comando ```OP_CHECKMULTISIG``` bruto e será aceita em um bloco. Esta é a metodologia clássica para usar multisigs no Bitcoin.

Como exemplo, iremos revisitar o multisig criado na seção [§8.1](08_1_Sending_a_Transaction_with_a_Locktime.md) uma última vez e construir um novo script de bloqueio para ele usando esta metodologia. Como devemos nos lembrar, essa transação era uma multisig 2 de 2 construída a partir de ```$address1``` e ```$address2```.

Como o script de bloqueio ```OP_CHECKMULTISIG``` requer o "m" (```2```), os endereços, e o "n" (```2```), poderíamos escrever o seguinte ```scriptPubKey```:
```
2 $address1 $address2 2 OP_CHECKMULTISIG
```
Se isso parece familiar, é porque é o multisig que desserializamos no [§10.2: Construindo a Estrutura do P2SH](10_2_Building_the_Structure_of_P2SH.md).
```
2 02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191 02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3 2 OP_CHECKMULTISIG
```
> **AVISO:** Para assinaturas `OP_CHECKMULTISIG` clássicas, o "n" deve ser ≤ 3 para que a transação seja padrão.

## Desbloqueando um Multisig Bruto

O ```scriptSig``` para um endereço multisig padrão deve então enviar os operandos ausentes para o ```OP_CHECKMULTISIG```: um ```0``` seguido por "m" assinaturas. Por exemplo:
```
0 $signature1 $signature2
```

### Executando um Script Multisig Bruto

Para gastar um UTXO multisig, executamos o ```scriptSig``` e o ```scriptPubKey``` da seguinte maneira:
```
Script: 0 $signature1 $signature2 2 $address1 $address2 2 OP_CHECKMULTISIG
Stack: [ ]
```
Primeiro, colocamos todas as constantes na pilha:
```
Script: OP_CHECKMULTISIG
Stack: [ 0 $signature1 $signature2 2 $address1 $address2 2 ]
```
Então, o ```OP_CHECKMULTISIG``` começa a rodar. Primeiro, o "2" é retirado:
```
Running: OP_CHECKMULTISIG
Stack: [ 0 $signature1 $signature2 2 $address1 $address2 ]
```
Então, o "2" diz ao ```OP_CHECKMULTISIG``` para retirar dois endereços:
```
Running: OP_CHECKMULTISIG
Stack: [ 0 $signature1 $signature2 2 ]
```
Então, o próximo "2" é retirado:
```
Running: OP_CHECKMULTISIG
Stack: [ 0 $signature1 $signature2 ]
```
Então, o "2" diz ao ```OP_CHECKMULTISIG``` para retirar duas assinaturas:
```
Running: OP_CHECKMULTISIG
Stack: [ 0 ]
```
Então, mais um item é retirado por engano:
```
Running: OP_CHECKMULTISIG
Stack: [ ]
```
Depois, o ```OP_CHECKMULTISIG``` completa a operação comparando as "m" assinaturas aos "n" endereços:
```
Script:
Stack: [ True ]
```
## Compreendendo as Limitações dos Scripts Multisig Brutos

Infelizmente, a técnica de incorporar um multisig bruto em uma transação tem algumas desvantagens notáveis:

1. Como não há formato de endereço padrão para os multisigs, cada remetente deve: inserir um script multisig longo e complicado; ter um software que permite fazer isso; e ser confiável para não bagunçar nada.
2. Como os multisigs podem ser muito mais longos do que os scripts padrão de bloqueio, a blockchain aumenta o custo. Isso requer taxas de transação mais altas para o remetente e cria mais incômodo para cada node.

Geralmente, esses eram problemas com qualquer tipo de script complexo do Bitcoin, mas rapidamente se tornaram problemas muito reais quando aplicados aos multisigs, que foram alguns dos primeiros scripts complexos a serem amplamente usados ​​na rede Bitcoin. As transações P2SH foram criadas para resolver esses problemas, começando em 2012.

> :book: ***O que é um multisig P2SH?*** Os Multisigs P2SH foram a primeira implementação de transações P2SH. Eles simplesmente empacotam uma transação multisig padrão em uma transação P2SH padrão. Isso permite a padronização de endereços, reduz o armazenamento de dados e, aumenta as contagens "m" e "n".

## Criando um Multisig P2SH

Os Multisigs P2SH são a metodologia moderna para a criação de multisigs na Blockchain. Elas podem ser criadas de forma muito simples, usando o mesmo processo visto nas seções anteriores.

### Criando um Bloqueio para o Multisig P2SH

Para criar um multisig P2SH, siga as etapas padrão para criar um script de bloqueio P2SH:

1. Serializar ```2 $ address1 $ address2 2 OP_CHECKMULTISIG```;
   1. ```<serializedMultiSig>``` = "522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98dfaed8d287f76c570e98dfaed8d287f76c570123582e38ed8287fab56c570123582e38ed8287fab56.
2. Salvar o ```<serializedMultiSig>``` para referência futura como o redemScript;
   1. ```<redeemScript>``` = "522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae".
3. Fazer o hash SHA-256 e RIPEMD-160 do script serializado;
   1. `<hashedMultiSig>` = "a5d106eb8ee51b23cf60d8bd98bc285695f233f3".
4. Produzir um script de bloqueio P2SH Multisig que inclua o hash do script (```OP_HASH160 <hashedMultisig> OP_EQUAL```).
   1. ```scriptPubKey``` =" a914a5d106eb8ee51b23cf60d8bd98bc285695f233f387 ".
   

Você pode criar uma transação usando o ```scriptPubKey```.

## Desbloqueando o Multisig P2SH

Para desbloquear essa transação multisig, é necessário que o destinatário produza um scriptSig que inclua as duas assinaturas e o ```redeemScript```.

### Executando a Primeira Rodada de Validação do P2SH

Para desbloquear o multisig P2SH, primeiro vamos confirmar o script:

1. Produzir um script de desbloqueio de ```0 $ signature1 $ signature2 <serializedMultiSig>```;
2. Concatenar isso com o script de bloqueio ```OP_HASH160 <hashedMultisig> OP_EQUAL```;
3. Validar o ```0 $ assinatura1 $ assinatura2 <serializedMultiSig> OP_HASH160 <hashedMultisig> OP_EQUAL```;
4. Teremos êxito se o ```<serializedMultisig>``` corresponder ao ```<hashedMultisig>```.

### Executando a Segunda Rodada de Validação do P2SH

Em seguida, vamos executar o script multisig:


1. Desserializar o ```<serializedMultiSig>``` para ```2 $ address1 $ address2 2 OP_CHECKMULTISIG```;
2. Concatenar isso com os operandos anteriores no script de desbloqueio, ```0 $ assinatura1 $ assinatura2```;
3. Validar o ```0 $ assinatura1 $ assinatura2 2 $ endereço1 $ endereço2 2 OP_CHECKMULTISIG```;
4. Teremos sucesso se os operandos cumprirem o ```redemScript``` desserializado.

Agora sabemos como a transação multisig da seção [§6.1](06_1_Sending_a_Transaction_to_a_Multisig.md) foi realmente criada, como foi validada para ser gasta e o motivo pelo qual o ```redeemScript``` foi tão importante.

## Resumo: Programando um Multisig

Os multisigs são um tipo de transação padrão, mas são um pouco complicados de serem usados. Então, eles são regularmente incorporados em transações P2SH, como foi o caso na seção [§6.1](06_1_Sending_a_Transaction_to_a_Multisig.md) quando criamos nossos primeiros multisigs. O resultado é mais limpo, menor e mais padronizado, porém, e mais importante, é um ótimo exemplo do mundo real de como os scripts P2SH realmente funcionam.

## O Que Vem Depois?

Vamos continuar "Incorporando Scripts em Transações P2SH no Bitcoin" na seção [§10.5: Programando um Script Segwit](10_5_Scripting_a_Segwit_Script.md).