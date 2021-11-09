# Prefácio: Apresentando o Bitcoin

Antes de começar a programar em Bitcoin (e Lightning), você deve ter uma compreensão básica do que são e como funcionam. Esta seção fornece essa visão geral. Várias outras definições irão aparecer mais à frente. O objetivo deste capítulo é apenas dar o conteúdo base.

## Sobre o Bitcoin

O Bitcoin é um sistema programável que permite a transferência da moeda bitcoin. Tem como base um sistema descentralizado _peer-to-peer_ (de ponta à ponta) de nodes, que inclui full nodes, carteiras e mineradores. Trabalhando juntos, eles garantem que as transações do bitcoin sejam rápidas e irreversíveis. Graças à natureza descentralizada do sistema, essas transações também são resistentes à censura e podem fornecer outras vantagens, como pseudo-anonimato, caso sejam bem utilizadas.

Obviamente, Bitcoin é o coração deste livro, mas também é o originador de muitos outros sistemas, incluindo a blockchain e Lightning, que são detalhados neste tutorial, e muitas outras criptomoedas, como Ethereum e Litecoin, que não são.

**_Como as moedas são transferidas?_** O Bitcoin não possui moedas físicas. Elas são uma série interminável de trocas títulos de propriedade. Quando uma pessoa envia as moedas para outra, essa transferência é armazenada como uma transação. É a transação que realmente registra a propriedade do dinheiro, isso significa que não existe nenhuma moeda que sai da carteira ou da máquina do proprietário.

**_Para quem você pode enviar as moedas?_** A grande maioria das transações de bitcoin envolvem o envio de moedas para pessoas comuns (ou pelo menos para endereços Bitcoin de pessoas comuns). No entanto, metodologias mais complexas podem ser usadas para enviar bitcoins para um grupo de pessoas ou para scripts. Essas várias metodologias possuem nomes como P2PKH, multisig e P2SH.

**_Como as transações são armazenadas?_** As transações são combinadas em grandes blocos de dados, que são gravados na _ledger da blockchain_. Um bloco é construído de tal forma que não pode ser substituído ou reescrito, uma vez que vários blocos tenham sido construídos depois dele. Isso é o que torna os bitcoins irreversíveis: Um livro razão (_ledger_) global descentralizado, onde tudo é registrado, é efetivamente um banco de dados permanente e imutável.

Porém, o processo de construção desses blocos é estocástico: É um tanto aleatório, então não podemos ter certeza se uma transação será colocada em um bloco específico. Também pode haver alterações nos blocos se forem muito recentes, mas apenas se forem _muitíssimo_ recentes. Então, as coisas se tornam irreversíveis (permanentes, imutáveis) depois de um alguns minutos.

**_Como as transações são protegidas?_** Os fundos contidos em uma transação Bitcoin são assegurados por um quebra-cabeça criptográfico. Esses quebra-cabeças são projetados para que possam ser facilmente resolvidos pela pessoa para quem os fundos foram enviados. Isso é feito usando o poder da criptografia de chave pública. Tecnicamente, uma transação é protegida por uma assinatura que prova que você é o proprietário da chave pública para a qual a transação foi enviada: Essa prova de propriedade é o quebra-cabeça que está sendo resolvido.

Os fundos são protegidos pelo uso de hashes. As chaves públicas não são realmente armazenadas na blockchain até que os fundos sejam gastos: Apenas os hashes de chave pública são. Isso significa que, mesmo que um computador quântico seja criado, as transações do Bitcoin permaneceriam protegidas por esse segundo nível de criptografia.

**_Como as transações são criadas?_** O coração de cada transação do Bitcoin é uma linguagem script do tipo FORTH usada para bloquear a transação. Para reenviar o dinheiro, o destinatário fornece informações específicas ao script que prova que ele é o destinatário pretendido.

No entanto, esses scripts do Bitcoin são o nível mais baixo de funcionalidade deste protocolo. Grande parte do trabalho do Bitcoin é feito através do `bitcoind` do Bitcoin, que é controlado por meio de comandos RPC. Muitas pessoas enviam esses comandos RPC por meio do programa chamado `bitcoin-cli`, que fornece uma interface ainda mais simples. Os não programadores nem mesmo se preocupam com essas minúcias, permitindo o uso de carteiras programáveis com interfaces mais simples.

### Bitcoin –– Em Resumo

Uma maneira de pensar no Bitcoin é como _uma sequência de transações atômicas_. Cada transação é autenticada por um remetente com a solução para um quebra-cabeça criptográfico anterior que foi armazenado como um script. A nova transação é bloqueada para o destinatário com um novo quebra-cabeça criptográfico que também é armazenado como um script. Cada transação é registrada no livro razão global imutável.

## Sobre Criptografia de Chave Pública

A criptografia de chave pública é um sistema matemático para proteção de dados e comprovação de propriedade por meio de um par assimétrico de chaves que estão vinculadas entre si: a chave pública e a chave privada.

É importante para o Bitcoin (e para a maioria dos sistemas que utilizam a blockchain) porque é a base de grande parte da criptografia que protege os fundos das criptomoedas. Uma transação de Bitcoin é normalmente enviada para um endereço que é um hash da chave pública. O destinatário pode então recuperar o dinheiro utilizando a chave pública e a chave privada.

**_O que é uma chave pública?_** Uma chave pública é a chave fornecida as demais pessoas. Em um sistema de chave pública comum, um usuário gera uma chave pública e uma chave privada e, em seguida, dá a chave pública para as pessoas. Esses destinatários podem criptografar informações com a chave pública, mas não podem ser descriptografadas com a mesma chave devido à assimetria do par de chaves.

**_O que é uma chave privada?_** Uma chave privada está vinculada a uma chave pública em um par de chaves. Em um sistema de chave pública comum, um usuário mantém sua chave privada segura e a usa para descriptografar as mensagens que foram criptografadas com sua chave pública antes de serem enviadas a ele.

**_O que é uma assinatura?_** Uma mensagem (ou mais comumente, um hash de uma mensagem) pode ser assinada com uma chave privada, criando uma assinatura. Qualquer pessoa com a chave pública correspondente pode validar a assinatura, o que verifica se o assinante possui a chave privada associada à chave pública em questão. O _SegWit_ é um formato específico para armazenar uma assinatura na rede Bitcoin que falaremos mais especificamente no futuro.

**_O que é uma função hash?_** Uma função hash é um algoritmo frequentemente usado com a criptografia. É uma maneira de mapear uma grande quantidade arbitrária de dados em uma quantidade pequena e fixa de dados. As funções hash usadas na criptografia são unilaterais e resistentes a colisões, o que significa que um hash pode ser vinculado de forma confiável aos dados originais, mas os dados originais não podem ser regenerados a partir do hash. Assim, os hashes permitem a transmissão de pequenas quantidades de dados para representar grandes quantidades de dados, o que pode ser importante para a eficiência e os requisitos de armazenamento.

O Bitcoin aproveita a capacidade do hash de disfarçar os dados originais, o que permite ocultar a chave pública real do usuário, tornando as transações resistentes à computação quântica.

### Criptografia de Chave Pública –– Em Resumo

Uma maneira de pensar na criptografia de chave pública é: _um jeito de qualquer pessoa proteger os dados de forma que apenas uma pessoa autorizada possa acessá-los e também possa provar que tem esse acesso._

## Sobre CCE

CCE é um acrônimo para criptografia de curva elíptica(em inglês: ECC, ou Elliptic-Curve Cryptography). É um ramo específico da criptografia de chave pública que depende de cálculos matemáticos usando curvas elípticas definidas sobre campos finitos. É mais complexo e difícil de explicar do que a criptografia de chave pública clássica (que usava números primos), mas tem algumas vantagens interessantes.

A CCE não receberá tanta atenção neste tutorial. Isso porque falaremos mais sobre a integração com servidores Bitcoin Core e Lightning, que já cuidaram da parte criptografia para você. Na verdade, a intenção deste tutorial é que você não precise se preocupar com a criptografia, porque isso é algo que você _realmente_ deseja que os especialistas lidem.

**_O que é uma curva elíptica?_** Uma curva elíptica é uma curva geométrica que assume a forma ``y`` <sup>`` 2`` </sup> = ``x`` <sup> ``3`` </sup> `` + ax + b``. Uma curva elíptica específica é escolhida selecionando valores específicos de ``a`` e ``b``. A curva deve ser examinada cuidadosamente para determinar se funciona bem para criptografia. Por exemplo, a curva secp256k1 usada pelo Bitcoin é definida como ``a = 0`` e ``b = 7``.

Qualquer linha que cruze uma curva elíptica o fará em 1 ou 3 pontos... e essa é a base da criptografia de curva elíptica.

**_O que são campos finitos?_** Um campo finito é um conjunto finito de números, onde todas as adições, subtrações, multiplicações e divisões são definidas de forma que resultem em outros números que também estarão no mesmo campo finito. Uma maneira simples de criar um campo finito é por meio do uso da aritmética modular.

**_Como uma curva elíptica é definida sobre um campo finito?_** Uma curva elíptica definida sobre um campo finito tem todos os pontos em sua curva desenhados a partir de um campo finito específico. Ele assume a forma: ``y``<sup>``2``</sup>``% tamanho-do-campo = (x``<sup>``3``</sup>`` + ax + b) % tamanho-do-campo``. O campo finito usado para secp256k1 é ``2``<sup>``256``</sup>``-2``<sup>``32``</sup>``-2``<sup>``9``</sup>``-2``<sup>``8``</sup>``-2``<sup>``7``</sup>``-2``<sup>``6``</sup>``-2``<sup>``4``</sup>``-1``.

**_Como as curvas elípticas são usadas na criptografia?_** Na criptografia de curva elíptica, um usuário seleciona um número muito grande (256 bits) como sua chave privada. Ele então adiciona um ponto base definido na curva a si mesmo tantas vezes. (Na secp256k1, o ponto de base é ``G = 04 79BE667E F9DCBBAC 55A06295 CE870B07 029BFCDB 2DCE28D9 59F2815B 16F81798 483ADA77 26A3C465 5DA4FBFC 0E1108A8 FD17B448 A6855419 9C47D08F FB10D4B8`` que prefixa as duas partes da tupla com um ``04`` para dizer que o ponto de dados está na forma descompactada. Se você preferir uma definição geométrica direta, é o ponto "0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798,0x483ADA7726A3C4655DA4FBFC0E1108A8FD0817B07029BFCDB2DCE28D959F2815B16F81798,0x483ADA7726A3C4655DA4FBFC0E1108A8FD0817B44810A"). O número resultante é uma chave pública. Várias fórmulas matemáticas podem então ser usadas para provar a propriedade da chave pública, dada a chave privada. Como acontece com qualquer função criptográfica, esta é uma armadilha: É fácil passar de uma chave privada para uma chave pública e praticamente impossível de passar de uma chave pública para uma chave privada.

Essa metodologia específica também explica por que os campos finitos são usados ​​em curvas elípticas: Ela garante que a chave privada não ficará muito grande. Observe que o campo finito para secp256k1 é ligeiramente menor que 256 bits, o que significa que todas as chaves públicas terão 256 bits, assim como as chaves privadas.

**_Quais são as vantagens da CCE?_** A principal vantagem da CCE é que ele permite a mesma segurança da criptografia de chave pública clássica com uma chave muito menor. Uma chave pública de curva elíptica de 256 bits corresponde a uma chave pública tradicional (RSA) de 3072 bits.

### CCE –– Em Resumo

Uma maneira de pensar na CCE é: _um jeito de permitir a criptografia de chave pública usando chaves muito pequenas e uma matemática bem obscura._

## Sobre Blockchains

A blockchain é a generalização da metodologia usada pelo Bitcoin para criar um livro razão distribuído globalmente. O Bitcoin é uma blockchain, assim como qualquer outra moeda alternativa, cada um dos quais vive em sua própria rede e grava em sua própria cadeia de blocos. As sidechains como a _Liquid_ são blockchains também. As blockchains não precisam necessariamente ser relacionadas com finanças. Por exemplo, existem várias discussões para implementar blockchains em soluções para proteger identidades autossoberanas.

Embora precise entender os fundamentos de como uma blockchain funciona para entender como as transações funcionam no Bitcoin, não é necessário muito mais do que isso. Como as blockchains se tornaram uma ampla categoria de tecnologia, esses conceitos básicos provavelmente serão aplicáveis ​​a muitos outros projetos neste setor de tecnologia em crescimento. Porém, os comandos de programação específicos aprendidos neste livro não serão abrangentes a várias utilizações, já que são específicos para Bitcoin (e para a Lightning).

**_Por que é chamado de cadeia de blocos?_** Cada bloco na blockchain armazena um hash do bloco anterior. Isso liga o bloco atual de volta ao "bloco de gênese" original por meio de uma cadeia de blocos ininterrupta. É uma forma de criar uma ordem absoluta entre os dados possivelmente conflitantes. Isso também fornece a segurança da blockchain, porque cada bloco é empilhado sobre um antigo tornando mais difícil recriar o bloco antigo devido aos algoritmos de prova de trabalho utilizados ​​na criação do bloco. Depois que vários blocos foram construídos sobre uma cadeia de blocos, ele é essencialmente irreversível.

**_O que é um fork?_** Ocasionalmente, dois blocos são criados ao mesmo tempo. Isso cria, temporariamente, um fork de um bloco, onde qualquer um dos blocos atuais pode ser o "verdadeiro". De vez em quando, um fork pode se expandir para dois blocos, três blocos ou mesmo quatro blocos de comprimento, mas muito rapidamente um lado da bifurcação é determinado como o verdadeiro e o outro se torna o que chamamos de "órfão". Isso faz parte do processo estocástico de criação de bloco e demonstra por que vários blocos devem ser construídos sobre um bloco antes que ele possa ser considerado verdadeiramente confiável e não rejeitável.

### Blockchain –– Em Resumo

Uma maneira de pensar na blockchain é: _Uma série vinculada de blocos de dados imutáveis, até o seu bloco inicial._
Outra forma é: _Uma série de blocos vinculados para ordenar dados absolutos que podem ser conflitantes_.

## A Blockchain é Adequada para Mim?

Se você deseja negociar bitcoins, então obviamente a Bitcoin é adequada para você. No entanto, de forma mais ampla, a blockchain se tornou uma cultura pop da atualidade, embora não seja uma solução mágica para todos os problemas técnicos. Dito isso, existem muitas situações específicas em que a blockchain é uma tecnologia superior.

Blockchains provavelmente _serão_ úteis nos seguintes casos:

* Os usuários não confiam uns nos outros.
    * Ou: Os usuários estão em diferentes localidades.
  * Os usuários não confiam nas autoridades centrais.
    * E: Os usuários desejam controlar seus próprios destinos.
  * Os usuários desejam uma tecnologia transparente.
  * Os usuários desejam compartilhar algo.
    * E: Os usuários desejam que o que é compartilhado seja registrado permanentemente.
  * Os usuários desejam uma transação final rápida.
    * Mas: Os usuários não precisam de uma transação final instantânea.

Blockchains provavelmente _não_ serão úteis caso:

* Os usuários sejam confiáveis:
    * Por exemplo: As transações ocorrem dentro de uma empresa ou organização.
    * Por exemplo: As transações são supervisionadas por uma autoridade central.
  * O sigilo é obrigatório:
    * Por exemplo: As informações devem ser secretas.
    * Por exemplo: As transações devem ser secretas.
    * Por exemplo: As pessoas que estão transacionando devem ser secretas.
    * A menos que: uma metodologia para sigilo criptográfico seja cuidadosamente considerada, analisada e testada.
  * Os usuários precisam de transações finais instantâneas:
    * Por exemplo: Em menos de 10 minutos em uma rede semelhante a Bitcoin, em menos de 2,5 minutos em uma rede semelhante a Litecoin, em menos de 15 segundos em uma rede semelhante a Ethereum.

Observe que ainda pode haver soluções para algumas dessas situações dentro do ecossistema Bitcoin. Por exemplo, os canais de pagamento estão lidando rapidamente com questões de liquidez e finalização do pagamento.

## Sobre a Lightning

A Lightning é um protocolo de segunda camada que interage com o Bitcoin para permitir que os usuários troquem seus bitcoins "offchain" (fora da blockchain). Possui vantagens e desvantagens em relação ao uso da camada principal do Bitcoin.

A Lightning também é um dos focos deste tutorial. Embora o ponto principal seja sobre a interação direta com o Bitcoin (e o `bitcoind`), vamos falar um pouco sobre a Lightning e o porque é uma tecnologia que está prestes a se tornar uma alternativa popular ao Bitcoin, em um futuro próximo. Este livro tem a mesma abordagem para a Lightning e para o Bitcoin: Ele ensina como interagir diretamente com a Lightning de maneira confiável à partir da linha de comando.

Ao contrário do Bitcoin, existem várias variantes da Lightning. Este tutorial usa a implementação compatível do padrão [c-lightning](https://github.com/ElementsProject/lightning) como sendo seu servidor Lightning confiável.

**_O que é um protocolo de segunda camada?_** Um protocolo de segunda camada no Bitcoin funciona tendo como base o Bitcoin. Nesse caso, a Lightning trabalha em cima do Bitcoin, interagindo com ele por meio de contratos inteligentes.

**_O que é um canal Lightning?_** Um canal Lightning é uma conexão entre dois usuários utilizando a Lightning. Cada um dos usuários bloqueia uma quantidade de bitcoins na blockchain do Bitcoin usando uma assinatura multi-sig, criada e assinada por ambos. Os dois usuários podem, então, trocar bitcoins por meio do canal Lightning sem precisar gravar nada na blockchain Bitcoin. Somente quando desejam fechar o canal, eles liquidam os bitcoins, com base na divisão no saldo final das partes.

**_O que é a Lightning Network?_** A junção de todos os canais da Lightning criam a Lightning Network. Isso permite que dois usuários que não tenham um canal entre si troquem bitcoins usando Lightning: O protocolo forma uma cadeia de canais entre os dois usuários e, em seguida, troca as moedas através da cadeia usando transações que chamamos de _time-locked transactions_.

**_Quais são as vantagens da Lightning?_** A Lightning permite transações mais rápidas com taxas mais baixas. Isso cria a possibilidade real de micropagamentos usando bitcoin. Ela também oferece maior privacidade, uma vez que está fora da rede, com apenas o primeiro e o último estado da transação sendo gravados blockchain do Bitcoin.

**_Quais são as desvantagens do Lightning?_** A Lightning ainda é uma tecnologia muito nova e não foi testada tão exaustivamente quanto o Bitcoin. Isso não é apenas uma questão de implementação tecnológica, mas também se o design que pode ser manipulado de maneiras não pensadas anteriormente.

### Resumindo a Lightning

Uma maneira de pensar na Lightning é: _Um jeito de transacionar bitcoins usando canais offchain entre duas pessoas, de modo que apenas o primeiro e o último estado precisem ser gravados na blockchain_.

## Resumo: Apresentando o Bitcoin

O Bitcoin é um sistema _peer-to-peer_ que permite a transferência de fundos por meio de transações bloqueadas por quebra-cabeças. Esses quebra-cabeças dependem da criptografia de curva elíptica de chave pública. Quando você generaliza as ideias por trás do Bitcoin, encontra-se as blockchains, uma tecnologia que atualmente está crescendo e inovando muitos setores. Quando você expande as ideias por trás do Bitcoin, obtém protocolos de segunda camada, como a Lightning, que expandem o potencial da moeda.

## O Que Vem Depois?

Vamos avançar em "Se Prepararando para o Bitcoin" com o [Capítulo Dois: Configurando um Bitcoin-Core VPS](02_0_Setting_Up_a_Bitcoin-Core_VPS.md).