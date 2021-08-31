# 2.2: Configurando uma Máquina Bitcoin-Core de Outras Maneiras

A seção anterior, [§2.1: Configurando um Bitcoin-Core VPS com Bitcoin Standup](02_1_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md), assumia que você criaria um full node em um VPS usando um StackScript Linode. Entretanto, você pode, na verdade, criar uma instância do Bitcoin-Core via qualquer outra metodologia de sua escolha e ainda assim acompanhar as próximas etapas deste tutorial.


A seguir estão outras metodologias de configuração das quais temos conhecimento:

* *[Compilar da Fonte](A2_0_Compiling_Bitcoin_from_Source.md).* Se você preferir compilar o Bitcoin Core manualmente, a maneira como fazê-lo é discutida no Apêndice 2.
* *[Usando GordianNode-macOS](https://github.com/BlockchainCommons/GordianNode-macOS).* Se você possuir um Mac moderno, você pode usar o aplicativo *GordianNode* da Blockchain Commons, que é capacitado pelo *BitcoinStandup*, para instalar um full node no seu Mac.
* *[Usando Outros Scripts de Bitcoin Standup](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts).* A Blockchain Commons também oferece uma versão do script Linode que você usou que pode ser executado pela linha de comando em qualquer máquina Debian ou Ubuntu. Esse tende a ser o script "topo-de-linha", o que significa que terá, mais provavelmente, novas funcionalidades e funções, como a instalação da Lightning.
* *[Configurando um Node Bitcoin na AWS](https://wolfmcnally.com/115/developer-notes-setting-up-a-bitcoin-node-on-aws/).* @wolfmcnally escreveu um tutorial passo-a-passo para a configuração de um Bitcoin-Core com o Amazon Web Services (AWS).
* *[Configurando um Node Bitcoin em uma Raspberry Pi 3](https://medium.com/@meeDamian/bitcoin-full-node-on-rbp3-revised-88bb7c8ef1d1).* Damian Mee explica como configurar um headless full node em uma Raspberry Pi 3.

## O Que Vem A Seguir?

A não ser que você queira retornar a alguma das outras metodologias de criação de um node Bitcoin-Core, você deve:

   * Seguir em frente para "bitcoin-cli" com o [Capítulo Três: Compreendendo Sua Configuração do Bitcoin](03_0_Understanding_Your_Bitcoin_Setup.md).
