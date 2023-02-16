# Aprendendo Bitcoin pela Linha de Comando 2.0.1
### _por Christopher Allen e Shannon Appelcline_
### _tradução por Namcios, Korea, Luke Pavsky e hgrams_

![](https://www.blockchaincommons.com/images/projects/lbtc-screen.png)

"Aprendendo Bitcoin pela Linha de Comando" é a versão em português de [Learning Bitcoin from the Command Line](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line), um tutorial para trabalhar com Bitcoin (e Lightning) que ensina a interação direta com os próprios servidores como a maneira mais robusta e segura para começar a trabalhar com criptomoedas.

> NOTA: Este é um rascunho em progresso, para que eu possa receber algum feedback de revisores iniciais. Ainda não está pronto para uso.

_Este tutorial assume que você tenha um mínimo de background em como utilizar a linha de comando. Caso contrário, a vários tutoriais que estão disponíveis na internet, e eu possui um curso para usuários de Mac em: https://github.com/ChristopherA/intro-mac-command-line._

## Traduções

* [Espanhol](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/tree/spanish-translation/es) - em progresso

Se você gostaria de fazer a sua própria tradução, por favor veja [Contribuindo](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/tree/master/pt#contribuindo) abaixo.

## Índice

### PARTE UM: SE PREPARANDO PARA O BITCOIN

**Estado**: Finalizado. Atualizado para 0.20.

* [1.0: Introdução à Programação com Bitcoin Core e Lightning](01_0_Introduction.md)
    * [Prefácio: Apresentando o Bitcoin](01_1_Introducing_Bitcoin.md)
* [2.0: Configurando um Bitcoin-Core VPS](02_0_Setting_Up_a_Bitcoin-Core_VPS.md)
  * [2.1: Configurando um Bitcoin-Core VPS com Bitcoin Standup](02_1_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md)
  * [2.2: Configurando uma Máquina Bitcoin-Core de Outras Maneiras](02_2_Setting_Up_Bitcoin_Core_Other.md)

### PARTE DOIS: USANDO O BITCOIN-CLI

**Estado:** Finalizado. Atualizado para 0.20.

* [3.0: Compreendendo Nossa Configuração do Bitcoin](03_0_Understanding_Your_Bitcoin_Setup.md)
  * [3.1: Verificando Nossa Configuração do Bitcoin](03_1_Verifying_Your_Bitcoin_Setup.md)
  * [3.2: Conhecendo Nossa Configuração do Bitcoin](03_2_Knowing_Your_Bitcoin_Setup.md)
  * [3.3: Configurando Nossa Carteira](03_3_Setting_Up_Your_Wallet.md)
    * [Prefácio: Usando Variáveis de Linha de Comando](03_3__Interlude_Using_Command-Line_Variables.md)
  * [3.4: Recebendo uma Transação](03_4_Receiving_a_Transaction.md)
  * [3.5: Compreendendo o Descritor](03_5_Understanding_the_Descriptor.md)
* [4.0: Enviando Transações no Bitcoin](04_0_Sending_Bitcoin_Transactions.md)
  * [4.1: Enviando Moedas da Maneira Fácil](04_1_Sending_Coins_The_Easy_Way.md)
  * [4.2: Criando uma Transação Bruta](04_2_Creating_a_Raw_Transaction.md)
     * [Prefácio: Usando JQ](04_2__Interlude_Using_JQ.md)
  * [4.3: Criando uma Transação Bruta com Argumentos Nomeados](04_3_Creating_a_Raw_Transaction_with_Named_Arguments.md)
  * [4.4: Enviando Moedas com Transações Brutas](04_4_Sending_Coins_with_a_Raw_Transaction.md)
     * [Prefácio: Usando Curl](04_4__Interlude_Using_Curl.md)
  * [4.5: Enviando Moedas com Transações Brutas Automatizadas](04_5_Sending_Coins_with_Automated_Raw_Transactions.md)
  * [4.6: Criando uma Transação SegWit](04_6_Creating_a_Segwit_Transaction.md)
* [5.0: Controlando Transações no Bitcoin](05_0_Controlling_Bitcoin_Transactions.md)
  * [5.1 Atentando-se para Transações Presas](05_1_Watching_for_Stuck_Transactions.md)
  * [5.2: Reenviando uma Transação com RBF](05_2_Resending_a_Transaction_with_RBF.md)
  * [5.3: Financiando uma Transação com CPFP](05_3_Funding_a_Transaction_with_CPFP.md)
* [6.0: Expandindo Transações no Bitcoin com Multisigs](06_0_Expanding_Bitcoin_Transactions_Multisigs.md)
  * [6.1: Enviando uma Transação com Multisig](06_1_Sending_a_Transaction_to_a_Multisig.md)
  * [6.2: Gastando uma Transação com Multisig](06_2_Spending_a_Transaction_to_a_Multisig.md)
  * [6.3: Enviando & Gastando um Multisig Automatizado](06_3_Sending_an_Automated_Multisig.md)
* [7.0: Expandindo Transações no Bitcoin com PSBTs](07_0_Expanding_Bitcoin_Transactions_PSBTs.md)
  * [7.1: Criando uma Transação Parcialmente Assinada no Bitcoin (PSBT)](07_1_Creating_a_Partially_Signed_Bitcoin_Transaction.md)
  * [7.2: Usando uma Transação Parcialmente Assinada no Bitcoin (PSBT)](07_2_Using_a_Partially_Signed_Bitcoin_Transaction.md)
  * [7.3: Integrando com Hardware Wallets](07_3_Integrating_with_Hardware_Wallets.md)
* [8.0: Expandindo Transações no Bitcoin de Outras Maneiras](08_0_Expanding_Bitcoin_Transactions_Other.md)  
  * [8.1: Enviando uma Transação com um Locktime](08_1_Sending_a_Transaction_with_a_Locktime.md)
  * [8.2: Enviando uma Transação com Dados](08_2_Sending_a_Transaction_with_Data.md)

### PARTE TRÊS: PROGRAMANDO NO BITCOIN

**Estado:** Finalizado. Atualizado para 0.20 e btcdeb.

* [9.0: Apresentando os Scripts do Bitcoin](09_0_Introducing_Bitcoin_Scripts.md)
  * [9.1: Compreendendo a Base das Transações](09_1_Understanding_the_Foundation_of_Transactions.md)
  * [9.2: Executando um Script do Bitcoin](09_2_Running_a_Bitcoin_Script.md)
  * [9.3: Testando um Script do Bitcoin](09_3_Testing_a_Bitcoin_Script.md)
  * [9.4: Programando um P2PKH](09_4_Scripting_a_P2PKH.md)
  * [9.5: Programando um P2WPKH](09_5_Scripting_a_P2WPKH.md)
* [10.0: Incorporando Scripts em Transações P2SH no Bitcoin](10_0_Embedding_Bitcoin_Scripts_in_P2SH_Transactions.md)
  * [10.1: Compreendendo a Base do P2SH](10_1_Understanding_the_Foundation_of_P2SH.md)
  * [10.2: Construindo a Estrutura do P2SH](10_2_Building_the_Structure_of_P2SH.md)
  * [10.3: Executando um Script do Bitcoin com P2SH](10_3_Running_a_Bitcoin_Script_with_P2SH.md)
  * [10.4: Programando um Multisig](10_4_Scripting_a_Multisig.md)
  * [10.5: Programando um Script Segwit](10_5_Scripting_a_Segwit_Script.md)
  * [10.6: Gastando uma Transação P2SH](10_6_Spending_a_P2SH_Transaction.md)
* [11.0: Capacitando Timelock com Scripts do Bitcoin](11_0_Empowering_Timelock_with_Bitcoin_Scripts.md)
  * [11.1: Compreendendo As Opções de Timelock](11_1_Understanding_Timelock_Options.md)
  * [11.2: Usando CLTV em Scripts](11_2_Using_CLTV_in_Scripts.md)
  * [11.3: Usando CSV em Scripts](11_3_Using_CSV_in_Scripts.md)
* [12.0: Expandindo os Scripts do Bitcoin](12_0_Expanding_Bitcoin_Scripts.md)
  * [12.1: Usando Condicionais no Script](12_1_Using_Script_Conditionals.md)
  * [12.2: Usando Outros Comandos no Script](12_2_Using_Other_Script_Commands.md)
* [13.0: Projetando Scripts Reais no Bitcoin](13_0_Designing_Real_Bitcoin_Scripts.md)
  * [13.1: Escrevendo Scripts de Quebra-Cabeças](13_1_Writing_Puzzle_Scripts.md)
  * [13.2: Escrevendo Scripts Complexos de Multisig](13_2_Writing_Complex_Multisig_Scripts.md)
  * [13.3: Capacitando o Bitcoin com Scripts](13_3_Empowering_Bitcoin_with_Scripts.md)

### PARTE QUATRO: PRIVACIDADE

**Estado:** Finalizado.

* [14.0: Usando o Tor](14_0_Using_Tor.md)
  * [14.1: Verificando Nossa Configuração do Tor](14_1_Verifying_Your_Tor_Setup.md)
  * [14.2: Mudando Nossos Serviços Ocultos do Bitcoin](14_2_Changing_Your_Bitcoin_Hidden_Services.md)
  * [14.3: Adicionando Serviços SSH Ocultos](14_3_Adding_SSH_Hidden_Services.md)

* [15.0: Usando o I2P](15_0_Using_i2p.md)
  * [15.1: Bitcoin Core como um Serviço I2P (Projeto Internet Invisível)](15_1_i2p_service.md)

### PARTE CINCO: PROGRAMANDO COM RPC

**Estado:** Finalizado.

* [16.0: Conversando com o Bitcoind com C](16_0_Talking_to_Bitcoind.md)
  * [16.1: Acessando o Bitcoind em C com Bibliotecas RPC](16_1_Accessing_Bitcoind_with_C.md)
  * [16.2: Programando o Bitcoind em C com Bibliotecas RPC](16_2_Programming_Bitcoind_with_C.md)
  * [16.3: Recebendo Notificações em C com Bibliotecas ZMQ](16_3_Receiving_Bitcoind_Notifications_with_C.md)
* [17.0: Programando o Bitcoin com Libwally](17_0_Programming_with_Libwally.md)
   * [17.1: Configurando a Libwally](17_1_Setting_Up_Libwally.md)
   * [17.2: Usando BIP39 na Libwally](17_2_Using_BIP39_in_Libwally.md)
   * [17.3: Usando BIP32 na Libwally](17_3_Using_BIP32_in_Libwally.md) 
   * [17.4: Usando PSBTs na Libwally](17_4_Using_PSBTs_in_Libwally.md)
   * [17.5: Usando Scripts na Libwally](17_5_Using_Scripts_in_Libwally.md)
   * [17.6: Usando Outras Funções na Libwally](17_6_Using_Other_Functions_in_Libwally.md)
   * [17.7: Integrando Libwally e Bitcoin-CLI](17_7_Integrating_Libwally_and_Bitcoin-CLI.md)
* [18.0: Conversando com o Bitcoind com Outras Linguagens](18_0_Talking_to_Bitcoind_Other.md)
  * [18.1: Acessando o Bitcoind com Go](18_1_Accessing_Bitcoind_with_Go.md)
  * [18.2: Acessando o Bitcoind com Java](18_2_Accessing_Bitcoind_with_Java.md)
  * [18.3: Acessando o Bitcoind com Node JS](18_3_Accessing_Bitcoind_with_NodeJS.md)
  * [18.4: Acessando o Bitcoind com Python](18_4_Accessing_Bitcoind_with_Python.md)
  * [18.5: Acessando o Bitcoind com Rust](18_5_Accessing_Bitcoind_with_Rust.md)
  * [18.6: Acessando o Bitcoind com Swift](18_6_Accessing_Bitcoind_with_Swift.md)

### PARTE SEIS: USANDO A LIGHTNING-CLI

**Estado:** Finalizado.

* [19.0: Compreendendo Nossa Configuração da Lightning](19_0_Understanding_Your_Lightning_Setup.md)
  * [19.1: Verificando Nossa Configuração da c-lightning](19_1_Verifying_Your_Lightning_Setup.md)
  * [19.2: Conhecendo Nossa Configuração da c-lightning](19_2_Knowing_Your_lightning_Setup.md)
     * [Prefácio: Acessando um Segundo Node Lightning](19_2__Interlude_Accessing_a_Second_Lightning_Node.md)
  * [19.3: Criando um Canal Lightning](19_3_Setting_Up_a_Channel.md)
* [20.0: Usando a Lightning](20_0_Using_Lightning.md)
  * [20.1: Gerando um Invoice](20_1_Generate_a_Payment_Request.md)
  * [20.2: Pagando um Invoice](20_2_Paying_a_Invoice.md)
  * [20.3: Fechando um Canal Lighnting]((20_3_Closing_a_Channel.md))
  * [20.4: Expandindo a Lightning Network](20_4_Lightning_Network_Review.md)

### APÊNDICES

**Estado:** Finalizado.

* [Apêndices](A0_Appendices.md)
  * [Apêndice I: Compreendendo o Bitcoin Standup](A1_0_Understanding_Bitcoin_Standup.md)
  * [Apêndice II: Compilando Bitcoin da Fonte](A2_0_Compiling_Bitcoin_from_Source.md)
  * [Apêndice III: Usando o Regtest do Bitcoin](A3_0_Using_Bitcoin_Regtest.md)
  
## Estado - Beta

v2.0.1 do **Aprendendo Bitcoin pela Linha de Comando** é uma beta. Pode ainda estar sendo sujeito a revisões ou edições de terceiros, mas já pode ser utilizado para o aprendizado.

Nós também estamos considerando o que poderíamos incluir em uma [v3.0](../TODO-30.md) do curso. Se você gostaria de apoiar um trabalho desse tipo, torne-se um [Patrocinador no GitHub](https://github.com/sponsors/BlockchainCommons) ou nos apoie pelo nosso [Servidor BTCPay](https://btcpay.blockchaincommons.com/), e nos informe que **Aprendendo Bitcoin** foi o motivo.

## Origem, Autores, Copyright & Licenças

A não ser que indicado o contrário (ou neste [/README.md](./README.md) ou nos comentários nos cabeçalhos dos aquivos), os conteúdos deste repositório são Copyright © 2020 por Blockchain Commons, LLC, e licenciados sob [CC-BY](../LICENSE-CC-BY-4.0.md).

## Apoio Financeiro

*Aprendendo Bitcoin pela Linha de Comando* é um projeto da [Blockchain Commons](https://www.blockchaincommons.com/). Nós somos, com orgulho, uma organização de benefício social sem fins lucrativos comprometida ao desenvolvimento aberto e open source. O nosso trabalho é financiado inteiramente por doações e parcerias colaborativas com pessoas como você. Toda contribuição será gasta na construção de ferramenta abertas, tecnologias e técnicas que sustentam e avançam segurança do blockchain e da internet e promovem uma rede aberta.

Para apoiar financeiramente o desenvolvimento futuro de `$projectname` e outros projetos, por favor considere se tornar um Patrocinador da Blockchain Commons por meio de patrocínios mensais contínuos como um [Patrocinador no GitHub](https://github.com/sponsors/BlockchainCommons). Você também pode apoiar a Blockchain Commons com bitcoins através do nosso [Servidor BTCPay](https://btcpay.blockchaincommons.com/).

## Contribuindo

Nós encorajamos contribuições públicas por meio de issues e pull requests! Por favor revise [CONTRIBUTING.md](./CONTRIBUTING.md) para detalhes sobre o nosso processo de desenvolvimento. Todas as contribuições para este repositório requerem um [Contrato de Licença do Contribuidor](../CLA.md) assinado por GPG.

Se você gostaria de providenciar uma tradução de Aprendendo Bitcoin para outra língua, por favor veja também [TRANSLATING.md](../TRANSLATING.md).

### Discussões

O melhor lugar para conversar sobre a Blockchain Commons e seus projetos é na nossa área de Discussões no GitHub.

[**Discussões Sobre Blockchain Commons**](https://github.com/BlockchainCommons/Community/discussions). Para desenvolvedores, estagiários a patrocinadores da Blockchain Commons, por favor utilize a área de discussões do [repositório da Comunidade](https://github.com/BlockchainCommons/Community) para falar sobre questões gerais da Blockchain Commons, o programa de estágio ou outros tópicos que não sejam o [Gordian System](https://github.com/BlockchainCommons/Gordian/discussions) ou os [padrões de carteira](https://github.com/BlockchainCommons/AirgappedSigning/discussions), pois cada um destes tem sua área de discussão própria.

### Outras Questões & Problemas

Como uma comunidade open-source e de desenvolvimento aberto, a Blockchain Commons não tem os recursos para fornecer apoio direto dos nossos projetos. Por favor considere a área de discussões como um local onde você pode receber respostas a perguntas. Alternativamente, por favor use o campo de [issues](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/issues) deste repositório. Infelizmente, não podemos fazer promessas em relação ao tempo de resposta.

Se a sua empresa requer apoio para usar os nossos projetos, por favor nos contacte diretamente para opcões. Nós podemos ser capazes de te oferecer um contrato para apoio por um de nossos contribuidores, ou poderemos te indicar outra entidade que possa oferecer o apoio contratual que você precisa.

### Créditos

As pessoas a seguir contribuíram diretamente para este repositório. Você pode adicionar o seu nome aqui ao se envolver. O primeiro passo é aprender como contribuir por meio da nossa documentação em [CONTRIBUTING.md](./CONTRIBUTING.md).


| Nome              | Função                | Github                                            | Email                                 | GPG Fingerprint                                    |
| ----------------- | ------------------- | ------------------------------------------------- | ------------------------------------- | -------------------------------------------------- |
| Christopher Allen | Autor Líder | [@ChristopherA](https://github.com/ChristopherA) | \<ChristopherA@LifeWithAlacrity.com\> | FDFE 14A5 4ECB 30FC 5D22  74EF F8D3 6C91 3574 05ED |
| Shannon Appelcline |  Autor Líder | [@shannona](https://github.com/shannona) | \<shannon.appelcline@gmail.com\> | 7EC6 B928 606F 27AD |


Contribuições adicionais estão listadas abaixo:

| Função                | Nomes                                    |
| ------------------- | ---------------------------------------- |
| ***Contribuidores:*** | [gg2001](https://github.com/gg2001) (seções Go, Node.js), [gorazdko](https://github.com/gorazdko) (seção Rust), [Javier Vargas](https://github.com/javiervargas) (seções C, Java, Lightning, Tor), [jodobear](https://github.com/jodobear) (Apêndice: Compilando Bitcoin, seção Python), [Prayank](    https://github.com/prayank23) (seções i2p)                            |
| ***Revisores:***    | Glen Willem [@gwillem](https://github.com/gwillem) |
| ***Patrocinadores:***     | Blockstream Corporation                  |


## Revelação Responsável

Nós queremos manter todo o nosso software seguro para todos. Se você descobriu uma vulnerabilidade de segurança, nós agradeceríamos a sua ajuda em nos revelá-la de forma responsável. Infelizmente, nós não conseguimos oferecer programas de recompensa (bug bounty) no momento.

Nós pedimos que você seja honesto e que, ao melhor de sua capacidade, não vaze informações ou prejudique qualquer usuário, seus dados, ou a comunidade de desenvolvedores. Por favor nos dê uma quantidade de tempo razoável para consertar o problema antes de publicá-lo. Não defraude nossos usuários ou nós mesmos no processo de descoberta. Nós prometemos não entrar com um processo contra pesquisadores que apontem um problema, dado que dêem o seu melhor para seguir estas diretrizes.

### Reportando uma Vulnerabilidade

Por favor reporte suspeitas de vulnerabilidades de segurança em um email privado para ChristopherA@BlockchainCommons.com (não utilize esse email para suporte). Por favor NÃO crie issues públicos para suspeitas de vulnerabilidades de segurança.

As seguintes chaves podem ser utilizadas para comunicar informação confidencial para desenvolvedores:

| Nome              | Fingerprint                                        |
| ----------------- | -------------------------------------------------- |
| Christopher Allen | FDFE 14A5 4ECB 30FC 5D22  74EF F8D3 6C91 3574 05ED |

Você pode importar uma chave executando o seguinte comando com a fingerprint daquele indivíduo: `gpg --recv-keys "<fingerprint>"` Se certifique de colocar o fingerprint entre aspas caso o mesmo contenha espaços.
