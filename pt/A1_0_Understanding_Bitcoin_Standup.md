# Apêndice 1: Compreendendo o Bitcoin Standup

Na seção [§2.1: Configurando um Bitcoin Core VPS com StackScript](02_1_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md) explicamos o processo de criação de um node de Bitcoin usando [Bitcoin-Standup-Scripts](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts). O apêndice à seguir explica o que as principais seções do script fazem. Podemos acompanhar em [Linode Standup](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts/blob/master/Scripts/LinodeStandUp.sh) em outra janela.

## Etapa 1: Nome do Host

O nome do seu host é armazenado em `/etc/hostname` e definido com o comando `hostname`. Ele também aparece em `/etc/hosts`.

## Etapa 2: Fuso Horário

O fuso horário do nosso host é armazenado em `/etc/timezone`, então um arquivo apropriado do `/usr/share/zoneinfo/` é copiado para o `/etc/localtime`.

## Etapa 3: Atualizando o Debian

O gerenciador de pacotes `apt-get` é usado para deixar nossa máquina atualizada e instalar o `gnupg`, o gerador de números aleatórios `haveged`, e o firewall simples `ufw`.

Nossa máquina está configurada para se manter automaticamente atualizada com o `echo "unattended-upgrades unattended-upgrades / enable_auto_updates boolean true "| debconf-set-selections`.

## Etapa 4: Configurando um Usuário

Um usuário `standup` é criado, o qual será usado para nossas aplicações do Bitcoin. Ele também tem permissões `sudo`, permitindo que executemos ações privilegiadas com este usuário.

Se fornecemos uma chave SSH, ela permitirá o acesso a esta conta (caso contrário, devemos usar a senha criada na configuração).

Se fornecemos um endereço IP, o acesso `ssh` será limitado a esse endereço, de acordo com `/etc/hosts.allow`.

## Etapa 5: Configurando o Tor

O Tor é instalado para fornecer serviços protegidos (ocultos) para acessar os comandos RPC do Bitcoin através do nosso servidor.  Podemos consultar a seção [§14.1: Verificando Nossa Configuração do Tor](14_1_Verifying_Your_Tor_Setup.md) para obter mais informações sobre a configuração do Tor.

Se criarmos um cliente autorizado para os serviços ocultos, o acesso será limitado a essa chave, que está em `/var/lib/tor/standup/authorized_clients`. Caso contrário, na seção [§14.2](14_2_Changing_Your_Bitcoin_Hidden_Services.md) explicamos como podemos fazê-la depois.

## Etapa 6: Instalando o Bitcoin

Bitcoin é instalado no `~standup/.bitcoin`. Nossa configuração é armazenada no `~standup/.bitcoin/bitcoin.conf`.

Precisamos nos certificar de que as somas de verificação (checksum) foram analisadas de acordo com a seção [§2.1](02_1_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md), caso contrário, podemos estar expostos a um ataque de cadeia de produção.

## Etapa 7: Instalando o Codificador QR

Para manter tudo compatível com [GordianSystem](https://github.com/BlockchainCommons/GordianSystem), um código QR é criado e está em `/qrcode.png`. Isso pode ser lido em um cliente QuickConnect como [GordianWallet](https://github.com/BlockchainCommons/GordianWallet-iOS).

## Conclusão - Compreendendo o Bitcoin Standup

O Bitcoin Standup usa scripts para tentar combinar muitas das funcionalidades de um [GordianNode](https://github.com/BlockchainCommons/GordianNode-macOS). Ele deve fornecer a nós um ambiente Bitcoin seguro construído em uma base de Bitcoin Core com Tor para nos comunicarmos com o RPC.

## O Que Vem Depois?

Se estivermos no processo de criação de um node Bitcoin para o uso neste curso, devemos voltar para a seção [§2.1](02_1_Setting_Up_a_Bitcoin-Core_VPS_with_StackScript.md).

Se estivermos lendo os apêndices, podemos continuar no [Apêndice 2: Compilando Bitcoin da Fonte](A2_0_Compiling_Bitcoin_from_Source.md).