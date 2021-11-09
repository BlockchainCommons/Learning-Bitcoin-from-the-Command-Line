# 2.2: Configurando uma Máquina Bitcoin-Core de Outras Maneiras

A seção anterior, [§2.1: Configurando um Bitcoin-Core VPS com Bitcoin Standup](02_1_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md), assumimos que iríamos criar um full node em um VPS usando um StackScript Linode. Entretanto, podemos querer criar uma instância do Bitcoin Core via qualquer outra metodologia a nossa escolha e ainda assim acompanhar as próximas etapas deste tutorial.


A seguir estão outras metodologias de configuração das quais temos conhecimento:

* *[Compilar da Fonte](A2_0_Compiling_Bitcoin_from_Source.md).* Se preferirmos compilar o Bitcoin Core manualmente, a maneira como fazer isso é discutida no Apêndice 2.
* *[Usando GordianNode-macOS](https://github.com/BlockchainCommons/GordianNode-macOS).* Se possuirmos um Mac moderno, podemos usar o aplicativo *GordianNode* da Blockchain Commons, que é capacitado pelo *BitcoinStandup*, para instalar um full node nosso Mac.
* *[Usando Outros Scripts de Bitcoin Standup](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts).* A Blockchain Commons também oferece uma versão do script Linode que usamos que pode ser executada pela linha de comando em qualquer máquina Debian ou Ubuntu. Esse tende a ser o script "topo-de-linha", o que significa que terá, mais provavelmente, novas funcionalidades e funções, como a instalação da Lightning.
* *[Configurando um Node Bitcoin na AWS](https://wolfmcnally.com/115/developer-notes-setting-up-a-bitcoin-node-on-aws/).* @wolfmcnally escreveu um tutorial passo-a-passo para a configuração de um Bitcoin Core em um Amazon Web Services (AWS).
* *[Configurando um Node Bitcoin em uma Raspberry Pi 3](https://medium.com/@meeDamian/bitcoin-full-node-on-rbp3-revised-88bb7c8ef1d1).* Damian Mee explica como configurar um full node headless em um Raspberry Pi 3.

## O Que Vem Depois?

A não ser que queiramos retornar a alguma das outras metodologias de criação de um node Bitcoin Core, devemos:

   * Seguir em frente para o "bitcoin-cli" com o [Capítulo Três: Compreendendo Nossa Configuração do Bitcoin](03_0_Understanding_Your_Bitcoin_Setup.md).
