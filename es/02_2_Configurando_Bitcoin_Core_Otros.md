# 2.2: Configuración de una máquina Bitcoin-Core a través de otros medios

La sección anterior, [§2.1: Configuración de un VPS Bitcoin-Core con Bitcoin Standup](02_1_Configurando_un_Bitcoin-Core_VPS_con_StackScript.md), presumía que 
estaría creando un nodo completo en un VPS usando un Linode Stackscript. Sin embargo, puede crear una instancia de Bitcoin-Core a través de cualquier metodología de
su elección y seguir los pasos posteriores de este tutorial.

A continuación, se muestran otras metodologías de configuración que conocemos:

* *[Compilación desde la fuente](A2_0_Compilando_Bitcoin_desde_la_Fuente.md).* Si prefiere compilar Bitcoin Core a mano, eso se trata en el Apéndice 2.
* *[Usando GordianNode-macOS](https://github.com/BlockchainCommons/GordianNode-macOS).* Si tiene una Mac moderna, puede usar la aplicación *GordianNode* de 
Blockchain Commons, con tecnología *BitcoinStandup*, para instalar un nodo completo en su Mac.
* *[Uso de otros scripts de Bitcoin Standup](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts).* Blockchain Commons también ofrece una versión del 
script de Linode que se uso anteriormente y que se puede ejecutar desde la línea de comandos en cualquier máquina Debian o Ubuntu. Este tiende a ser un script 
de vanguardia, lo que significa que es más probable que presente nuevas funciones, como la instalación de Lightning.
* *[Configuración de un nodo Bitcoin en AWS](https://wolfmcnally.com/115/developer-notes-setting-up-a-bitcoin-node-on-aws/).* @wolfmcnally ha escrito un tutorial 
paso a paso para configurar Bitcoin-Core con Amazon Web Services (AWS).
* *[Configuración de un nodo Bitcoin en una Raspberry Pi 3](https://medium.com/@meeDamian/bitcoin-full-node-on-rbp3-revised-88bb7c8ef1d1).* Damian Mee explica 
cómo configurar una Nodo completo de forma sencilla en una Raspberry Pi 3.

## ¿Que sigue?

A menos que desee volver a una de las otras metodologías para crear un nodo Bitcoin-Core, debe:

* Continúe con "bitcoin-cli" con el [Capítulo tres: Entendiendo su configuración de Bitcoin](03_0_Entendiendo_Su_Configuracion_Bitcoin.md).
