# 10.1: Compreendendo a Base do P2SH

Sabemos que os scripts do Bitcoin podem ser usados para controlar o resgate dos UTXOs. A próxima etapa é criar os nossos próprios scripts... mas isso requer uma técnica muito específica.

## Conhecendo os Padrões do Bitcoin

Aqui está a pegadinha sobre como usar scripts do Bitcoin: Por razões de segurança, a maioria dos nodes do Bitcoin aceitará apenas seis tipos de transações de Bitcoin, ditas como "padrão".

* __Pay to Public Key (P2PK)__ - O tipo de transação mais antiga e obsoleta (```<pubKey> OP_CHECKSIG```) que foi substituída por uma mais segura, a P2PKH.
* __Pay to Public Key Hash (P2PKH)__ - Uma transação padrão (```OP_DUP OP_HASH160 <pubKeyHash> OP_EQUALVERIFY OP_CHECKSIG```) que paga para o hash de uma chave pública.
* __Pay to Witness Public Key Hash (P2WPKH)__ - O tipo mais novo de transação de chave pública. Usa apenas (```OP_0 <pubKeyHash>```) porque depende do consenso do minerador para funcionar, conforme descrito na seção [§9.5](09_5_Scripting_a_P2WPKH.md).
* __Multisig__ - Uma transação para um grupo de chaves, conforme explicado mais detalhadamente na seção [§8.4](08_4_Scripting_a_Multisig.md).
* __Null Data__ - Uma transação que não pode ser gasta (`OP_RETURN Data`).
* __Pay to Script Hash (P2SH)__ - Uma transação que paga a um script específico, conforme explicado mais detalhadamente neste capítulo.

Então, como podemos descrever um script mais complexo do Bitcoin? A resposta está nesse último tipo de transação padrão, o P2SH. Podemos colocar qualquer tipo de script mais longo e complexo em uma transação P2SH e, contanto que sigamos as regras padrões para incorporar nosso script e resgatar os fundos, obteremos todos os benefícios do Script do Bitcoin.

> :warning: **AVISO DE VERSÃO:** Os Scripts P2SH somente se tornaram padrão à partir do Bitcoin Core 0.10.0. Antes disso, apenas os scripts do tipo P2SH Multisigs eram permitidos.

## Compreendendo o Script P2SH

Você já viu uma transação P2SH quando criou um multisig na seção [§6.1: Enviando uma Transação com Multisig](06_1_Sending_a_Transaction_to_a_Multisig.md). Embora o multisig seja um dos tipos de transação padrão, o ```bitcoin-cli``` simplifica o uso dos multisigs, incorporando-os às transações P2SH, conforme descrito mais detalhadamente na seção [§10.4: Programando um Multisig](10_4_Scripting_a_Multisig.md).

Então, vamos olhar mais uma vez para o ```scriptPubKey``` daquela transação multisig P2SH:
```
      "scriptPubKey": {
        "asm": "OP_HASH160 a5d106eb8ee51b23cf60d8bd98bc285695f233f3 OP_EQUAL",
        "hex": "a914a5d106eb8ee51b23cf60d8bd98bc285695f233f387",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr"
        ]
      }
```

O script de bloqueio tem uma aparência bastante simples: ```OP_HASH160 a5d106eb8ee51b23cf60d8bd98bc285695f233f3 OP_EQUAL```. Como de costume, há um grande bloco de dados no meio. Este é um hash de outro script de bloqueio que está oculto (```redeemScript```) que só será revelado quando os fundos forem resgatados. Em outras palavras, o script de bloqueio padrão para um endereço P2SH é: ```OP_HASH160 <redeemScriptHash> OP_EQUAL```.

> :book: ***O que é um redeemScript?*** Cada transação P2SH carrega a impressão digital de um script de bloqueio que fica oculto como um hash de 20 bytes. Quando uma transação P2SH é resgatada, o ```redeemScript``` completo (sem o hash) é incluído como parte do ```scriptSig```. O Bitcoin garantirá que o ```redeemScript``` corresponda ao hash. Então, ele realmente executa o ```redeemScript``` para ver se os fundos podem ser gastos (ou não).

Um dos elementos interessantes das transações P2SH é que nem o remetente nem o Blockchain sabem realmente o que é o ```redeemScript```! Um remetente simplesmente envia para um endereço P2SH padronizado que é marcado com um prefixo "2" e não se preocupa com a forma como o destinatário irá recuperar os fundos.

> :link: **TESTNET vs MAINNET:** Na testnet, o prefixo para endereços P2SH é ```2```, enquanto na mainnet, é ```3```.

## Compreendendo Como Construir um Script P2SH

Como o script de bloqueio está visível para uma transação P2SH e é bem simples, criar uma transação desse tipo também é um tanto quanto fácil. Em teoria. Tudo o que precisamos fazer é criar uma transação cujo script de bloqueio inclua um hash de 20 bytes do ```redeemScript```. Esse hash é feito com o padrão ```OP_HASH160``` do Bitcoin.

> :book: ***O que é OP_HASH160?*** Uma operação padrão do hash para o Bitcoin executar um hash SHA-256 e, em seguida, um hash RIPEMD-160.

No geral, quatro etapas são necessárias:

1. Criar um script de bloqueio arbitrário com Bitcoin Script;
2. Criar uma versão serializada desse script de bloqueio;
3. Executar um hash SHA-256 nesses bytes serializados;
4. Executar um hash RIPEMD-160 nos resultados desse hash SHA-256.

Cada uma dessas etapas exige algum trabalho e algumas delas podem ser bastante complexas. A boa notícia é que realmente não precisamos nos preocupar com elas, porque são complexas o suficiente para que tenhamos uma API que faz o serviço para nós, normalmente.

Por enquanto, forneceremos apenas uma visão geral, para que entendamos o contexto da metodologia. Na seção [§10.2: Construindo a Estrutura do P2SH](10_2_Building_the_Structure_of_P2SH.md), forneceremos uma visão mais aprofundada da criação do script, caso queiramos entender a essência desse processo.

## Compreendendo Como Enviar uma Transação P2SH

Então, como enviamos nossa transação P2SH? Novamente, a teoria é muito simples:

1. Incorporamos o hash em um script ```OP_HASH160 <redeemScriptHash> OP_EQUAL```;
2. Traduzimos isso para um código hexadecimal;
3. Usamos esse hex como nosso ```scriptPubKey```;
4. Criamos o resto da transação.

Infelizmente, este é outro momento em que precisaremos usar APIs, em grande parte porque o ```bitcoin-cli``` não fornece nenhum suporte para a criação de transações P2SH. Mas, podemos resgatá-las sem problema.

## Compreendendo Como Desbloquear uma Transação P2SH

O truque para resgatar uma transação P2SH é que o destinatário deve ter salvo o script secreto de bloqueio serializado que foi codificado para criar o endereço P2SH. Isso se chama ```redeemScript``` porque é o que o destinatário precisa para resgatar os fundos.

Um ```scriptSig``` de desbloqueio para uma transação P2SH tem o seguinte formato: ``` ... dados ... <redeemScript>```. Os ```dados``` devem ser _somente_ dados que são colocados na pilha, não operadores. O [BIP 16](https://github.com/bitcoin/bips/blob/master/bip-0016.mediawiki) os chama de assinaturas, mas isso não é um requisito real.

> :warning: **AVISO:** Embora assinaturas não sejam um requisito, um script P2SH na verdade não é muito seguro se não exigir pelo menos uma assinatura nas entradas. As razões para isso estão descritas na seção [§13.1: Escrevendo Scripts Quebra-Cabeças](13_1_Writing_Puzzle_Scripts.md).

Quando um UTXO é resgatado, ele é executado em duas rodadas de verificação:

1. Primeiro, o ```redeemScript``` no ```scriptSig``` está em hash e é comparado ao hash do script no ```scriptPubKey```;
2. Se eles corresponderem, uma segunda rodada de verificação será iniciada;
3. Em segundo lugar, o ```redeemScript``` é executado usando os dados anteriores que foram colocados na pilha;
4. Se a segunda rodada de verificação _também_ for bem-sucedida, o UTXO é desbloqueado.

Considerando que não podemos criar facilmente uma transação P2SH sem uma API, devemos ser capazes de resgatar facilmente uma transação P2SH com o ```bitcoin-cli```. Na verdade, já fizemos isso na seção [§6.2: Gastando uma Transação com Multisig](06_2_Spending_a_Transaction_to_a_Multisig.md). O processo exato é descrito na seção [§10.6: Gastando uma Transação P2SH](10_6_Spending_a_P2SH_Transaction.md), após terminarmos com todas as complexidades da criação de uma transação P2SH.

> :warning: **AVISO:** Podemos criar uma transação perfeitamente válida com um redeemScript com o hash correto, mas se o redeemScript não funcionar, ou não funcionar corretamente, os fundos serão perdidos para sempre. É por isso que é importantíssimo testar os scripts, conforme discutido na seção [§9.3: Testando um Script no Bitcoin](09_3_Testing_a_Bitcoin_Script.md).

## Resumo: Compreendendo a Base do P2SH

Scripts arbitrários não são o padrão no Bitcoin. No entanto, podemos incorporá-los em transações padrão usando o tipo de endereço P2SH. Precisamos apenas fazer um hash do nosso script como parte do script de bloqueio, então o revelamos e executamos como parte do script de desbloqueio. Contanto que  também possamos satisfazer o ```redeemScript```, o UTXO poderá ser gasto.

> :fire: ***Qual é o poder do P2SH?*** Já sabemos o poder do Bitcoin Script, que nos permite criar Contratos Inteligentes mais complexos de todos os tipos. O P2SH é o que realmente libera esse poder, nos permitindo que  incluamos o Script arbitrário do Bitcoin em transações padrão.

## O Que Vem Depois?

Vamos continuar "Incorporando Scripts em Transações P2SH no Bitcoin" na seção [§10.2: Construindo a Estrutura do P2SH](10_2_Building_the_Structure_of_P2SH.md).