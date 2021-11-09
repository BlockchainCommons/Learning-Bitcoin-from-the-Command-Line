# 2.1: Configurando um Bitcoin-Core VPS com Bitcoin Standup

Este documento explica como configurar um VPS (Virtual Private Server, ou no português, Servidor Privado Virtual) para rodar um node do Bitcoin usando o Linode.com, instalado usando um StackScript automatizado do [projeto Bitcoin Standup](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts). Apenas precisamos copiar e colar alguns comandos e inicializar o VPS. Quase que imediatamente após a inicialização, encontraremos o nosso novo node do Bitcoin baixando os blocos.

> :warning: **AVISO:** Não podemos usar o VPS para uma carteira de bitcoin com saldos significativos e reais; vamos ler o http://blog.thestateofme.com/2012/03/03/lessons-to-be-learned-from-the-linode-bitcoin-incident/. Ele é muito bom para poder experimentar com transações de bitcoin reais em um node online sem usar um servidor auto-hospedado na nossa rede local. Ele também é útil para poder usar em um iPhone ou iPad para se comunicar via SSH com o nosso VPS para fazer algumas tarefas Bitcoin simples. Mas é necessário um nível mais alto de segurança para utilizarmos saldos grandes e reais.

* Se quisermos saber o que essa configuração faz, podemos ler o [Apêndice I: Compreendendo o Bitcoin Standup](A1_0_Understanding_Bitcoin_Standup.md) enquanto o instalamos;
* Se, ao invés disso, quisermos configurá-lo em outra máquina que não seja um VPS na Linode, como uma máquina na AWS ou um Mac, podemos ir para a seção [§2.2: Configurando um Bitcoin-Core de Outras Maneiras](02_2_Setting_Up_Bitcoin_Core_Other.md)
* Se já temos um node do Bitcoin rodando, podemos ir para o [Capítulo 03: Compreendendo a configuração do node Bitcoin](03_0_Understanding_Your_Bitcoin_Setup.md).

## Começando com o Linode

O Linode é um serviço de hospedagem na nuvem que oferece servidores rápidos e baratos com armazenamento SSD. Nós a utilizamos neste tutorial principalmente porque os StackScripts feitos para BASH oferecem uma maneira fácil de automaticamente configurar um node Bitcoin sem confusão e bagunça.

### Configurando uma Conta no Linode

Podemos criar uma conta na Linode por aqui:

```
https://www.linode.com
```

Se preferirmos, o seguinte código de indicação nos dará dois meses de uso grátis (até U$100), ótimo para aprender sobre o Bitcoin:

[https://www.linode.com/?r=3c7fa15a78407c9a3d4aefb027539db2557b3765](https://www.linode.com/?r=3c7fa15a78407c9a3d4aefb027539db2557b3765)

Precisaremos informar um endereço de email e depois pré-carregar o dinheiro usando um cartão de crédito ou conta do PayPal para eventuais custos futuros.

Quando estiver terminado, devemos cair na seguinte página [https://cloud.linode.com/dashboard](https://cloud.linode.com/dashboard).

### Considerando a Autenticação de Dois Fatores

A segurança do seu servidor não estará completa se pessoas possam invadir a sua conta da Linode, então considere ativar a Autenticação de Dois Fatores para ele. Você pode encontrar essa configuração na [página Minha Conta: Senha e Autenticação](https://manager.linode.com/profile/auth). Se você não fizer isso agora, anote como tarefa para voltar e fazê-lo depois.

## Criando a Imagem Linode usando um StackScript

### Carregando o StackScript

Vamos fazer o download do [Linode Standup Script](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts/blob/master/Scripts/LinodeStandUp.sh) pelo [repositório do Bitcoin Standup Scripts](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts). Este script basicamente automatiza todas as instruções de configuração do VPS Bitcoin. Se quisermos ser mais prudentes, podemos lê-lo com atenção. Se estivermos satisfeito, pode copiar o StackScript para nossa conta do Linode e selecionar [Criar Novo StackScript](https://cloud.linode.com/stackscripts/create). Vamos dar a ele um nome (usamos `Bitcoin Standup`), então vamos copiar e colar o script. Vamos escolher o Debian 10 para nossa imagem alvo e clicar em "Salvar".

### Fazendo a Configuração Inicial

Agora estamos prontos para criar um node baseado no StackScript.

1. Na [página de StackScripts](https://cloud.linode.com/stackscripts?type=account), cliquemos em "..." à esquerda do novo script e vamos escolher o "Deploy New Linode";
2. Coloquemos um Short Hostname e Fully Qualified Hostname;
   * **Short Hostname.** Vamos escolher um nome para o VPS. Por exemplo, "meutestebtc";
   * **Fully Qualified Hostname.** Se formos incluir este VPS como parte de uma rede com informações de DNS completos, vamos escrever o hostname com nosso domínio. Por exemplo, "meutestebtc.meudominio.com". Caso contrário, vamos apenas repitir o hostname curto e adicione ".local", por exemplo "meutestebtc.local";
3. Coloquemos a senha do usuário "standup";
4. Vamos escolher um Installation Type nas opções avançadas.
   * **Installation Type.** Provavelmente será "Mainnet" ou "Pruned Mainnet" se estivermos configurando um node para usar e "Testnet" ou "Pruned Testnet" se estivermos apenas testando. A maior parte deste tutorial irá assumir que tenhamos escolhido o "Pruned Testnet", mas devemos conseguir segui-lo mesmo com os outros tipos. Veja a [Sinopse](#synopsis-bitcoin-installation-types) para maiores informações sobre essas opções. (Percebamos que se estivermos planejando tentar colocar em prática os capítulos da Lightning, provavelmente nos daremos melhor com um node não prunado, pois trabalhar com nodes prunados na Lightning é algo bem experimental. Vamos até a seção [§19.1](19_1_Verifying_Your_Lightning_Setup.md#compiling-the-source-code) para maiores informações);
5. Vamos preencher quaisquer outras opções avançadas aplicáveis;
   * **X25519 Public Key.** Esta é a chave pública para adicionar à lista de clientes autorizados do Tor. Se não a utilizarmos, qualquer pessoa que obtiver acesso ao QR code do nosso node podendo assim acessá-lo. Receberemos esta chave pública do cliente que estivermos utilizando para acessar o node. Por exemplo, se  estivermos utilizando a [FullyNoded 2](https://github.com/BlockchainCommons/FullyNoded-2), podemos ir às configurações e clicar em "Export Tor V3 Authentication Public Key" para usá-la aqui;
  * **SSH Key.** Vamos copiar a chave SSH do computador local para este campo; isto permite que possamos fazer login automaticamente usando a conta standup via SSH. Se ainda não configuramos uma chave SSH no nosso computador local, o [Github](https://help.github.com/articles/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent/) contém boas instruções para fazermos isso. Provavelmente também iremos querer adicionar nossa chave SSH ao nosso LISH (Linode Interactive Shell) através do nosso "Linode Home Page/My Preferences/LISH Settings/LISH Keys". Usar o SSH nos proporcionará uma maneira mais fácil e simples de fazer login no nosso servidor;
  * **SSH-Allowed IPs.** Esta é uma lista de endereços IP separados por vírgula que serão autorizados a se conectarem via SSH ao nosso VPS. Por exemplo "192.168.1.15,192.168.1.16". Se não adicionarmos nenhum IP, _o nosso VPS não estará muito seguro_. Ele será bombardeado constantemente por hackers tentando encontrar uma forma de adentrá-lo, e podem conseguir;
4. Selecione uma Imagem;
   * **Target Image.** Se seguimos as instruções, as configurações irão nos deixar selecionar apenas o "Debian 10" (apesar de que "Debian 9" tem funcionado com versões posteriores deste StackScript e pode ser que ainda funcione);
5. Escolha uma região para onde o Linode ficará hospedado.

*_Todas as questões remanescentes têm a ver com as mecânicas de implantar o VPS e devem ser deixadas como estão, com uma exceção: aumentar o Swap Disk de 256MB para 512MB, para assegurarmos de que teremos memória suficiente para baixar a blockchain._

Finalmente, deveremos preencher a senha root, que será a senha usada para a conta root.

### Escolhendo Outras Opções do Standup

A Blockchain Commons ainda está no processo de expandir os Bitcoin Standup Scripts com opções para a instalação da Lightning e outras aplicações notáveis do Bitcoin. Podemo dar uma olhada nas opções extras para ver se elas são coisas com as quais gostaríamos de brincar. Em particular, se a Lightning for uma opção, nós sugerimos que instalá-la, porque facilitará bastante os capítulos [19](19_0_Understanding_Your_Lightning_Setup.md) e [20](20_0_Using_Lightning.md).

### Escolhendo um Plano da Linode

Em seguida, escolheremos um plano da Linode.

Um Linode de 4GB será suficiente para a maioria das configurações, incluindo: Mainnet Prunada, Testnet Prunada, e até Testnet não prunada. Todos estes usam menos de 50GB de armazenamento e 4GB é uma quantidade confortável de memória. Este é o plano que recomendamos. Ele consome 20 dólares por mês.

Se, ao invés disso, quisermos ter um node Mainnet não prunada em um VPS, precisaremos instalar um Linode em um disco com mais de 280GB(!), que no momento é o Linode 16GB, que tem 320GB de armazenamento e 16GB de memória, custando aproximadamente 80 dólares por mês. Nós _não_ recomendamos este plano.

A tabela à seguir mostra os requisitos mínimos:

| Configuração | Memória | Armazenamento | Linode |
|-------|--------|---------|---------|
| Mainnet | 2G | 280G | Linode 16GB |
| Mainnet Prunada | 2G | ~5G | Linode 4GB |
| Testnet | 2G | ~15G | Linode 4GB |
| Testnet Prunada | 2G | ~5G | Linode 4GB |
| Regtest | 2G | ~ | Linode 4GB |

Mas podem existir maneiras de reduzir os custos.

* Para as máquinas que sugerimos o **Linode 4GB**, podemos conseguir reduzí-las para um Linode 2GB. Algumas versões do Bitcoin Core têm funcionado bem com esta quantidade, outras têm esgotado a memória de vez em quando e depois recuperado, e outras têm esgotado a memória continuamente. Lembre-se de aumentar o Swap Disk para maximizar as chances de funcionar. Use por sua conta e risco;
* Para a Mainnet Não Prunada, surgerimos um **Linode 16GB**, porém provavelmente conseguiremos utilizar um Linode 4GB, mas precisaremos adicionar um [Block Storage](https://cloud.linode.com/volumes) suficiente para armazenar a blockchain. Esta é com certeza uma solução melhor para o longo prazo porque os requisitos de armazenamento da blockchain do Bitcoin aumentam continuamente se não a prunarmos, enquanto os requisitos de CPU não aumentam (ou pelo menos não nas mesmas proporções). Um armazenamento GibiByte de 320GB custaria U$32 por mês, que combinado com um Linode 4GB custa U$52 por mês, ao invés de U$80, e mais importante, podemos continuar aumentando-o. Nós não documentamos esta configuração por completo por duas razões: (1) nós não sugerimos a configuração mainnet não prunada, então suspeitamos ser uma configuração incomum; e (2) nós não testamos como volumes Linode comparam com nossos SSDs intrínsecos em performance e uso. Mas existe uma documentação completa na página do Block Storage. Precisaríamos configurar o Linode, executar o stackscript, e então interrompê-lo para mover o armazenamento da blockchain para um volume comissionado antes de continuarmos.

### Fazendo a Configuração Final

A última coisa que precisamos fazer é colocar a senha root. (Se tivermos nos esquecido de algo, seremos informados agora).

Clique em "Deploy" para inicializar os discos e preparar o VPS. Tudo deve ser executado em menos de um minuto. Quando estiver pronto veremos, na "Host Job Queue", botões verdes de "Success" dizendo "Disk Create from StackScript - Setting password for root... done." e "Create Filesystem - 256MB Swap Image".

Agora podemos querer alterar o nome do nosso VPS Linode do padrão `linodexxxxxxxx`. Vamos na aba de configurações e vamos alterar o rótulo para algo mais útil, como o hostname curto do nosso VPS. Por exemplo, podemos chamá-lo de `bitcoin-testnet-prunada` para diferenciá-lo de outros VPSs em nossa conta.

## Fazendo Login no Nosso VPS

Se olharmos o painel de controle do nosso Linode, devemos ver o novo computador se iniciar. Quando o serviço atingir 100%, poderemos fazer o login.

Primeiramente, precisaremos do endereço de IP. Clique na aba "Linodes" e uma lista dos nossos VPSs deve aparecer, além do fato de que ele está online, o seu "plano", endereço de IP, e outras informações.

Vamos para o console local e vamos fazer login na conta `standup` usando aquele endereço:

```
ssh standup@[IP-ADDRESS]
```

Por exemplo:

```
ssh standup@192.168.33.11
```

Se configuramos o nosso VPS para usar uma chave SSH, o login deve ser automático (possivelmente requisitando a senha SSH para desbloquear a chave). Se não tivermos configurado a chave SSH, então precisaremos escrever a senha do user1.

### Esperando Alguns Instantes

Aqui vai uma pequena sacada: _o nosso StackScript está executando agora_. O script BASH é executado na primeira vez que o VPS é ligado. O que significa que o nosso VPS ainda não está pronto.

O tempo total de execução é por volta de 10 minutos. Então, vamos usar este intervalo para fazer um café, ou apenas relaxar por alguns instantes. Há duas partes do script que demoram um pouco: a atualização de todos os pacotes do Debian e o download do código do Bitcoin. Eles devem demorar não mais que 5 minutos cada, o que significa que se voltarmos em uns 10 minutos, provavelmente tudo estará pronto.

Se estivermos impaciente, podemos pular e usar o `sudo tail -f /standup.log`, que mostrará o progresso da instalação, como descrito na próxima seção.

## Verificando Nossa Instalação

Saberemos que o stackscript terminou quando a `tail` do `standup.log` disser algo como o seguinte:
```
/root/StackScript - Bitcoin is setup as a service and will automatically start if your VPS reboots and so is Tor
/root/StackScript - You can manually stop Bitcoin with: sudo systemctl stop bitcoind.service
/root/StackScript - You can manually start Bitcoin with: sudo systemctl start bitcoind.service
```
Nesse momento, nosso diretório home se parecerá com isso:

```
$ ls
bitcoin-0.20.0-x86_64-linux-gnu.tar.gz  laanwj-releases.asc  SHA256SUMS.asc
```

Esses são os vários arquivos que foram usados para instalar o Bitcoin no nosso VPS. _Nenhum_ deles é necessário. Nós apenas os deixamos lá caso queiramos fazer qualquer verificacão adicional. Caso contrário, podemos deletá-los:

```
$ rm *
```

### Verificando a Configuração do Bitcoin

Para ter certeza de que o Bitcoin baixado é válido, o StackScript checa a assinatura e o checksum SHA. Devemos verificar se ambos os testes foram bem sucedidos:

```
$ sudo grep VERIFICATION /standup.log
```

Se observarmos algo como o descrito abaixo, tudo estará OK:

```
/root/StackScript - VERIFICATION SUCCESS / SIG: gpg: Good signature from "Wladimir J. van der Laan (Bitcoin Core binary release signing key) <laanwj@gmail.com>" [unknown]
/root/StackScript - VERIFICATION SUCCESS / SHA: 35ec10f87b6bc1e44fd9cd1157e5dfa4```
```
Entretanto, se qualquer uma dessas verificações retornar "VERIFICATION ERROR", então temos um problema. Como tudo isso é programado, é possível que tenha ocorrido apenas uma mudança pequena que causou erros nas verificações do script. Isto aconteceu poucas vezes durante a existência do script que virou o Standup. Mas, também é possível que alguém esteja tentando te empurrar uma cópia falsa do daemon do Bitcoin. Então, _precisamos ter a certeza absoluta que sabemos o que de fato aconteceu antes de usarmos o Bitcoin!_

### Lendo os Logs

Também podemos querer ler todos os logs dos arquivos de configuração para se assegurar de que nada imprevisível aconteceu durante a instalação.

É melhor olhar o arquivo de log padrão do StackScript, que tem todas as saídas, incluindo os erros:

`$ sudo more /standup.log`

Podemos observar que é totalmente normal ver _alguns_ erros, particularmente quando executamos o software gpg e quando várias coisas tentam acessar o dispositivo `/dev/tty` inexistente.

Se, ao invés disso, quisermos olhar para um conjunto menor de informação, todos os erros devem estar em:

`$ sudo more /standup.err`

Ele ainda contém uma certa quantidade de informação além de erros, mas é uma leitura mais rápida.

Se tudo estiver OK, parabéns, temos um node Bitcoin funcionando usando o Linode!

## O Que Nós Fizemos

Apesar de a imagem padrão do Debian 10 que estamos usando para o VPS ter sido modificada pelo Linode para ser razoavelmente seguro, o nosso node Bitcoin instalado pelo StackScript Linode é configurado com um nível de segurança ainda maior. Isto pode ser limitante, ou pode nos impedir de fazer algumas coisas que esperávamos conseguir fazer. O que segue são algumas dicas à respeito disso:

### Serviços Protegidos

A instalação do Bitcoin VPS é mínima e permite pouco tipo de comunicação. Isto é feito através do "uncomplicated firewall" (`ufw`), que bloqueia tudo, exceto conexões SSH. Também existe alguma segurança adicional possível para as portas RFC, graças aos serviços ocultos instalados pelo Tor.

**Ajustando o UFW.** Provavelmente deveremos deixar o UFW no estágio de super-protegido! Não devemos usar uma máquina Bitcoin para outros serviços, porque cada um aumenta a vulnerabilidade! Se decidirmos o oposto, existem vários [guias para o UFW](https://www.digitalocean.com/community/tutorials/ufw-essentials-common-firewall-rules-and-commands) que podem nos ajudar a adicionar outros serviços. Como o nome diz, é _uncomplicated_. Por exemplo, adicionar serviços de email requeriria apenas abrir a porta de mail: `sudo ufw allow 25`. Mas não vamos fazer isso, não é mesmo.

**Ajustando o Tor.** Podemos querer proteger os serviços como SSH de uma maneira melhor. Podemos ler sobre isso no [Capítulo 12: Usando o Tor](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/12_0_Using_Tor.md), para maiores informações sobre o Tor.

### Shells Protegidos

Se definimos o "SSH-allowed IPs", o acesso SSH (e SCP) ao servidor é bastante restrito. `/etc/hosts.deny` impede qualquer um de se logar. _Nós não recomendamos que isso seja alterado_. `/etc/hosts.allow` então autoriza endereços de IP específicos. Podemos apenas adicionar mais endereços de IP em uma lista separada por vírgulas se precisarmos oferecer mais acesso.

Por exemplo:
```
sshd: 127.0.0.1, 192.128.23.1
```

### Atualizações Automáticas

O Debian também é configurado para se atualizar automaticamente para garantir de que mantenha atualizado aos patches de segurança mais recentes.

Se, por alguma razão, desejarmos alterar isso (_algo que não recomendamos_), podemos fazer o seguinte:

```
echo "unattended-upgrades unattended-upgrades/enable_auto_updates boolean false" | debconf-set-selections
```

_Se quisermos saber mais sobre o que o Bitcoin Standup StackScript faz, podemos ler o [Apêndice I: Compreendendo o Bitcoin Standup](A1_0_Understanding_Bitcoin_Standup.md)_.

## Brincando com o Bitcoin

Agora, provavelmente desejaremos brincar com o Bitcoin!

Mas calma, provavelmente o nosso daemon do Bitcoin ainda está baixando os blocos. O `bitcoin-cli getblockcount` irá dizer como está indo:
```
$ bitcoin-cli getblockcount
1771352
```
Se o resultado for diferente a cada vez que digitarmos o comando, precisaremos esperar antes de começar a trabalhar com o Bitcoin. Isto levará, atualmente, de 1 a 6 horas para um node prunado, dependendo das especificações da nossa máquina.

Mas quando estabilizar em um número, estaremos prontos para continuar!

Mesmo assim, pode ser uma boa hora para mais alguns cafézinhos. Mas logo logo o nosso sistema estará pronto, e nós também!

## Resumo: Configurando um Bitcoin-Core VPS Manualmente

Criar um Bitcoin Core VPS com os scripts Standup fizeram com que todo o processo fique rápido, simples, e (esperamos), indolor.

## O Que Vem Depois?

Temos algumas opções para o que podemos seguir:

   * Ler o [StackScript](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts/blob/master/Scripts/LinodeStandUp.sh) para entender a configuração;
   * Ler o que o StackScript faz no [Apêndice I: Compreendendo o Bitcoin Standup](A1_0_Understanding_Bitcoin_Standup.md);
   * Escolher uma metodologia inteiramente alternativa, como descrito na seção [§2.2: Configurando uma Máquina Bitcoin-Core de Outras Maneiras](02_2_Setting_Up_Bitcoin_Core_Other.md);
   * Usar o "bitcoin-cli" com o [Capítulo Três: Compreendendo Nossa Configuração do Bitcoin](03_0_Understanding_Your_Bitcoin_Setup.md).

## Sinopse: Tipos de Instalação do Bitcoin

**Mainnet.** Isto irá fazer o download da blockchain inteira do Bitcoin. São 280GB de dados (e está aumentando a cada dia).

**Mainnet Prunada.** Isto irá reduzir a blockchain que iremos armazenar para apenas os últimos 550 blocos. Se não estivermos minerando ou executando outro serviço do Bitcoin, isso deve ser o suficiente para validação.

**Testnet.** Isto nos dará acesso a uma blockchain alternativa do Bitcoin onde os bitcoins não têm valor algum. É feita para experimentos e testes.

**Testnet Prunada.** Isto são apenas os últimos 550 blocos da Testnet ... porque a blockchain da Testnet está meio grande agora também.

**Regtest Privada.** Este é o modo Regression Testing, que nos permite rodar um servidor de Bitcoin totalmente local. Nos permite também realizar testes mais detalhados. Não há um modo prunado aqui, porque estaremos começando a blockchain do zero. Esta é uma configuração bem diferente, então é discutida no [Apêndice 3](A3_0_Using_Bitcoin_Regtest.md).