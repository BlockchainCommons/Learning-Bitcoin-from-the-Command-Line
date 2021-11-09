# Capítulo 1: Introdução à Aprendizagem do Bitcoin Core (& Lightning) pela Linha de Comando

## Introdução

Os modos como fazemos pagamentos por bens e serviços têm mudado drasticamente nas últimas décadas. Antigamente todas as transações eram realizadas utilizando dinheiro ou cheques, e atualmente os diversos métodos de pagamento eletrônico são à regra. Porém, a maioria dos pagamentos eletrônicos ainda ocorre utilizando sistemas centralizados, onde empresas de cartão de crédito, bancos ou até instituições financeiras baseadas apenas na Internet, como o PayPal, mantêm listas de transações longas e correlacionadas individualmente, tendo o poder de censurar as transações que não gostarem.

Esses riscos de centralização foram alguns dos catalisadores primordiais para a criação de criptomoedas, sendo a primeira e mais bem sucedida, o Bitcoin. O Bitcoin oferece pseudonímia; torna difícil correlacionar as transações; e torna a censura por entidades individuais, algo próximo do impossível. Essas vantagens fizeram dele uma das moedas mais rápidas do mundo. Esse crescimento, por sua vez, despertou o interesse dos empresários e desenvolvedores, ansiosos para criar novos serviços para a comunidade do Bitcoin.

Se você é um desses empreendedores ou desenvolvedores, então este curso é para você, porque ele tem tudo o que necessita para aprender a programar no Bitcoin. É um curso introdutório que explica todas as nuances e características do Bitcoin e de tudo o que vem com ele. O curso também oferece algumas coisas mais específicas, como  aulas de como utilizar _diretamente_ o Bitcoin Core com o servidor C-Lightning usando suas interfaces RPC.

Por que não utilizar algumas das bibliotecas mais detalhadas encontradas em várias linguagens de programação? Por que não criar a sua própria do zero? Porque trabalhar com criptomoedas é perigoso. Não há redes de segurança. Se você acidentalmente pagar taxas muito altas ou perder uma chave de assinatura ou criar uma transação inválida ou fazer qualquer outros milhares de erros potenciais, você dará adeus a suas moedas para sempre. Muito dessa responsabilidade, é claro, fica com você como sendo um programador de criptomoedas, mas também pode ser minimizado, trabalhando com as interfaces de criptomoedas mais robustas e seguras que estão disponíveis, as que foram criadas pelas próprias equipes de programação dessas criptomoedas: A ``Bitcoind`` e a ``Lightningd``.

Grande parte deste livro discute como fazer um script em bitcoin (e na Lightning Network) diretamente pela linha de comando. Alguns capítulos adiantes lidam com linguagens de programação mais sofisticadas, mas novamente continuam a interagir diretamente com o `` Bitcoind`` e a  ``Lightningd``, usando o RPC ou interagindo com os arquivos que são criados por eles. Isso permite que você se mantenha no mesmo nível dos gigantes e use a tecnologia confiável para aprender como criar seus próprios sistemas confiáveis.

## Nível de Habilidade Necessária

Você não precisa ser particularmente uma pessoa da área técnica para boa parte deste curso. Tudo o que precisará é a confiança para executar comandos básicos pela linha de comando do UNIX. Se você está familiarizado com coisas como ``ssh``, ``cd``, e ``ls``, o curso irá te fornecer o resto.

Uma minoria deste curso requer conhecimento de programação, e você deve pular essas seções, se necessário, conforme iremos discutir na próxima seção.

## Resumo dos Tópicos

Este livro é dividido nas seguintes seções:

| Parte | Descrição | Habilidades |
|-------|---------|---------|
| **Parte Um: Se Preparando para o Bitcoin** | Entendendo os fundamentos do Bitcoin e configurando um servidor para uso. | Linha de Comando | 
| **Parte Dois: Usando o Bitcoin-CLI** | Usando o Bitcoin-CLI para criar transações. | Linha de Comando |
| **Parte Três: Programando no Bitcoin** | Expandindo nosso trabalho no Bitcoin com scripts. | Conceitos de Programação |
| **Parte Quatro: Privacidade** | Melhorando a segurança do nosso node com Tor ou I2P | Linha de Comando |
| **Parte Cinco: Programando com RPC** | Acessando o RPC com C e outras linguagens. | Programando em C |
| **Parte Seis: Usando a Lightning-CLI** | Usando a Lightning-CLI para criar transações. | Linha de Comando |
| **Apêndices.** | Utilizando configurações incomuns no Bitcoin. | Linha de Comando |

## Como Usar Este Curso

Então, por onde você começa? Este livro é destinado principalmente para ser lido em sequência. Basta seguir o "O que vem depois?" que estarão no final de cada seção e/ou clicar nos links individuais de cada seção em cada página do capítulo. Você conseguirá o melhor entendimento deste curso se realmente se construir um servidor Bitcoin (no capítulo 2) e, em seguida, passar por todos os exemplos que estarão disponíveis ao longo do livro: Testar os exemplos é uma excelente metodologia de aprendizado.

Se você tem diferentes níveis de habilidade ou se quiser aprender coisas diferentes, poderá pular para algumas partes diferentes do livro:

* Se você já tem um ambiente pronto do Bitcoin para ser utilizado, vá para o [Capítulo 3: Compreendendo Nossa Configuração do Bitcoin](03_0_Understanding_Your_Bitcoin_Setup.md).
* Se você só se importa com programar no Bitcoin, pule para o [Capítulo 9: Apresentando os Scripts do Bitcoin](09_0_Introducing_Bitcoin_Scripts.md).
* Se você quiser apenas ler sobre o uso das linguagens de programação, pule para o [Capítulo 16: Conversando com o Bitcoind com C](16_0_Talking_to_Bitcoind.md).
* Se não quer programar nada, definitivamente ignore os capítulos 16 ao 18 enquanto estiver lendo, e talvez seja melhor pular os capítulos 9 ao 13. O resto do curso ainda deve fazer sentido sem eles.
* Se estiver interessado apenas na Lightning Network, corra para o [Capítulo 19: Compreendendo Nossa Configuração da Lightning](19_0_Understanding_Your_Lightning_Setup.md).
* Se quiser ler o conteúdo novo adicionado na versão 2 do curso (2020), seguido da versão 1 (2017), leia [§3.5: Entendendo o Descritor](03_5_Understanding_the_Descriptor.md), [§4.6: Criando uma Transação SegWit](04_6_Creating_a_Segwit_Transaction.md), [Capítulo 7: Expandindo Transações no Bitcoin com PSBTs](07_0_Expanding_Bitcoin_Transactions_PSBTs.md), [§9.5: Programando um P2WPKH](09_5_Scripting_a_P2WPKH.md), [§10.5: Programando um Script SegWit](10_5_Scripting_a_Segwit_Script.md), [Capítulo 14: Usando o Tor](14_0_Using_Tor.md), [Capítulo 16: Conversando com o Bitcoind com C](16_0_Talking_to_Bitcoind.md), [Capítulo 17: Programando o Bitcoin com Libwally](17_0_Programming_with_Libwally.md), [Capítulo 18: Conversando com o Bitcoind com Outras Linguagens](18_0_Talking_to_Bitcoind_Other.md), [Capítulo 19: Compreendendo Nossa Configuração da Lightning](19_0_Understanding_Your_Lightning_Setup.md), e [Capítulo 20: Usando a Lightning](20_0_Using_Lightning.md).

## Por Que Utilizar Este Curso

Obviamente, você está querendo fazer este curso porque está interessado no Bitcoin. Além de transmitir o conhecimento básico, também ajudará os leitores a participar (ou criar) projetos open source e obter empregos de cargos júnior na área de programação envolvendo o Bitcoin. Um número de estagiários na Blockchain Commons aprendeu sobre o Bitcoin através deste curso, e muitos dos membros da nossa equipe de programação também.

## Como Ajudar Este Curso

* Por favor, use a área de [Issues](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/issues) para quaisquer dúvidas. A Blockchain Commons não tem uma equipe ativa de suporte, e não podemos abordar problemas ou perguntas individuais, mas iremos tratá-los com o tempo e os usaremos para melhorar nossas futuras iterações no curso.
* Por favor, use os [PRs](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/pulls) para quaisquer correções de erros de digitação ou comandos incorretos (ou que foram alterados). Para alterações técnicas ou de linha de comando, é muito útil se você usar também os comentários para explicar por que fez o que você fez, para que não precisemos gastar tempo pesquisando o motivo.
* Por favor, use nossa [Área de Discussão da Comunidade](https://github.com/BlockchainCommons/Community/discussions) para falar sobre carreiras e habilidades. A Blockchain Commons ocasionalmente oferece estágios, como falado em nosso repositório da comunidade.
* Por favor, [torne-se um patrocinador](https://github.com/sponsors/BlockchainCommons) se achar este curso útil ou se quiser ajudar a educar a próxima geração de programadores da blockchain.

## O Que Vem Depois?

Se você quiser uma introdução básica ao Bitcoin, criptografia de chave pública, ECC, blockchains e Lightning Network, leia o prefácio [Apresentando o Bitcoin](01_1_Introducing_Bitcoin.md). 

Caso contrário, se já estiver pronto para mergulhar de cabeça no curso, vá para [Configurando um Bitcoin-Core VPS](02_0_Setting_Up_a_Bitcoin-Core_VPS.md).
