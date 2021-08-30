# 2.1: Configurando um Bitcoin-Core VPS com Bitcoin Standup

Este documento explica como configurar um VPS (Servidor Privado Virtual, ou no inglês, Virtual Private Server) para rodar um node do Bitcoin em Linode.com, instalado usando um StackScript automatizado do [projeto Bitcoin Standup](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts). Você apenas precisa copiar e colar alguns comandos e inicializar o seu VPS. Quase que imediatamente após a inicialização, você encontrará o seu novo node do Bitcoin alegremente baixando blocos.


> :warning: **AVISO:** Não use o VPS para uma carteira de bitcoin com fundos reais significativos; veja http://blog.thestateofme.com/2012/03/03/lessons-to-be-learned-from-the-linode-bitcoin-incident/ . Ele é muito bom para poder experimentar com transações de bitcoin reais em um node online sem atar um servidor auto-hospedado na sua rede local. Ele também é útil para poder usar um iPhone ou iPad para comunicar via SSH com o seu VPS para fazer algumas tarefas Bitcoin simples. Mas um nível mais alto de segurança é preciso para fundos significantes.

* Se você quiser saber o que essa configuração faz, leia o [Apêndice I: Compreendendo o Bitcoin Standup](A1_0_Understanding_Bitcoin_Standup.md) enquanto o instala.
* Se, ao invés disso, você quiser configurá-lo em outra máquina que não seja um VPS na Linode, como uma máquina na AWS ou um Mac, vá para [§2.2: Configurando um Bitcoin-Core de Outras Maneiras](02_2_Setting_Up_Bitcoin_Core_Other.md)
* Se você já tem um node do Bitcoin rodando, vá para [Capítulo Três: Compreendendo a Sua Configuração do Bitcoin](03_0_Understanding_Your_Bitcoin_Setup.md).

## Começando com a Linode

A Linode é um serviço de hospedagem na nuvem que oferece servidores rápidos e baratos com armazenamento SSD. Nós a utilizamos neste tutorial principalmente porque os seus StackScripts feitos para BASH oferecem uma maneira fácil de automaticamente configurar um node Bitcoin sem confusão e bagunça.

### Configure uma Conta na Linode

Você pode criar uma conta na Linode por aqui:

```
https://www.linode.com
```

Se preferir, o seguinte código de indicação te dará dois meses de uso grátis (até U$100), ótimo para aprender sobre Bitcoin:

[https://www.linode.com/?r=3c7fa15a78407c9a3d4aefb027539db2557b3765](https://www.linode.com/?r=3c7fa15a78407c9a3d4aefb027539db2557b3765)

Você precisará prover um endereço de email e depois pré-carregar dinheiro de um cartão de crédito ou do PayPal para eventuais custos futuros.

Quando estiver terminado, você deve cair em [https://cloud.linode.com/dashboard](https://cloud.linode.com/dashboard).

### Considere Autenticação de Dois Fatores

A segurança do seu servidor não estará completa se pessoas possam invadir a sua conta da Linode, então considere ativar a Autenticação de Dois Fatores para ele. Você pode encontrar essa configuração na [página Minha Conta: Senha e Autenticação](https://manager.linode.com/profile/auth). Se você não fizer isso agora, anote como tarefa para voltar e fazê-lo depois.

## Criando a Imagem Linode usando um StackScript

### Load the StackScript

Faça o download do [Linode Standup Script](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts/blob/master/Scripts/LinodeStandUp.sh) pelo [repositório do Bitcoin Standup Scripts](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts). Este script basicamente automatiza todas as instruções de configuração do VPS Bitcoin. Se você quiser ser particularmente prudente, leia-o com atenção. Se estiver satisfeito, pode copiar o StackScript para sua conta Linode e selecionar [Criar Novo StackScript](https://cloud.linode.com/stackscripts/create). Dê a ele um bom nome (nós usamos `Bitcoin Standup`), então copie e cole o script. Escolha Debian 10 para sua imagem alvo e clique em "Salvar".

### Faça a Configuração Inicial

Agora você está pronto para criar um node baseado no StackScript.

1. Na [página de StackScripts](https://cloud.linode.com/stackscripts?type=account), clique em "..." à esquerda do seu novo script e escolha "Deploy New Linode".
2. Coloque um hostname curto e qualificado por completo
   * **Hostname Curto.** Escolha um nome para o seu VPS. Por exemplo, "meutestebtc".
   * **Hostname Qualificado por Completo.** Se você for incluir este VPS como parte de uma rede com recordes DNS completos, escreva o hostname com seu domínio. Por exemplo, "meutestebtc.meudominio.com". Caso contrário, apenas repita o hostname curto e adicione ".local", por exemplo "meutestebtc.local".
3. Coloque a senha do usuário "standup".
4. Escolha um Tipo de Instalação nas opções avançadas.
   * **Tipo de Instalação.** Provavelmente isto será "Mainnet" ou "Mainnet Prunado" se você estiver configurando um node para uso e "Testnet" ou "Testnet Prunado" se você estiver apenas testando. A maior parte deste tutorial irá assumir que você tenha escolhido "Testnet Prunado", mas você deve conseguir segui-lo mesmo com os outros tipos. Veja a [Sinopse](#synopsis-bitcoin-installation-types) para maiores informações sobre essas opções. (Perceba que se você estiver planejando a tentar os capítulos de Lightning, você provavelmente se dará melhor com um node não prunado, pois trabalhar com nodes prunados na Lightning é experimental. Refira à seção [§19.1](19_1_Verifying_Your_Lightning_Setup.md#compiling-the-source-code) para maiores informações.)
5. Preencha quaisquer outras opções avançadas aplicáveis.
   * **Chave Pública X25519.** Esta é a chave pública para adicionar à lista de clientes autorizados do Tor. Se você não a utilizar, qualquer pessoa que obtiver acesso ao QR code do seu node poderá acessá-lo. Você receberá esta chave pública do cliente que você estiver utilizando para acessar o seu node. Por exemplo, se você estiver utilizando a [FullyNoded 2](https://github.com/BlockchainCommons/FullyNoded-2), você pode ir às configurações e clicar em "Export Tor V3 Authentication Public Key" para usá-la aqui.
  * **Chave SSH.** Copie a chave SSH do seu computador local para este campo; isto permite que você possa fazer login automaticamente à conta standup via SSH. Se você ainda não configurou uma chave SSH no seu computador local, o [Github](https://help.github.com/articles/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent/) contém boas instruções para isto. Você provavelmente também irá querer adicionar sua chave SSH ao seu LISH (Linode Interactive Shell) através da sua "Linode Home Page / My Preferences / LISH Settings / LISH Keys". Usar SSH te proporcionará uma maneira mais fácil e simples de fazer login no seu servidor.
  * **IPs Permitidos para SSH.** Esta é uma lista de endereços IP separados por vírgula que serão autorizados a se conectarem via SSH ao seu VPS. Por exemplo "192.168.1.15,192.168.1.16". Se você não adicionar nenhum IP, _o seu VPS não estará muito seguro_. Ele será bombardeado constantemente por hackers tentando encontrar uma forma de adentrá-lo, e podem conseguir.
4. Selecione uma Imagem
   * **Imagem Alvo.** Se você seguiu as instruções, isto apenas irá deixá-lo selecionar "Debian 10" (apesar de que "Debian 9" tem funcionado com versões posteriores deste StackScript e pode ser que ainda funcione).
5. Escolha uma região para onde o Linode ficará hospedado.

*Todas as questões remanescentes têm a ver com as mecânicas de implantar o VPS e devem ser deixadas como estão, com uma exceção: aumente o Swap Disk de 256MB para 512MB, para se assegurar de que você terá memória suficiente para baixar a blockchain._

Finalmente, você deverâ preencher a senha root, que será a senha usada para a conta root.

### Escolha Outras Opções Standup

A Blockchain Commons ainda está no processo de expandir seus Bitcoin Standup Scripts com opções para a instalação da Lightning e outras aplicações notáveis de Bitcoin. Dê uma olhada nas opções extras e veja se elas são coisas com as quais você gostaria de brincar. Em particular, se Lightning for uma opção, nós sugerimos que a instale, porque facilitará bastante os capítulos [19](19_0_Understanding_Your_Lightning_Setup.md) e [20](20_0_Using_Lightning.md).

### Escolha um Plano da Linode

Em seguida, você escolherá um plano da Linode.

Um Linode de 4GB será suficiente para a maioria das configurações, incluindo: Mainnet Prunado, Testnet Prunado, e até Testnet não prunado. Todos estes usam menos de 50GB de armazenamento e 4GB é uma quantidade confortável de memória. Este é o plano que recomendamos. Ele gasta 20 dólares por mês.

Se, ao invés disso, você quiser ter um node Mainnet não prunado em um VPS, você precisará instalar um Linode em um disco com mais de 280GB(!), que no momento é o Linode 16GB, que tem 320GB de armazenamento e 16GB de memória, custando aproximadamente 80 dólares por mês. Nós _não_ recomendamos este plano.

A tabela à seguir mostra os requisitos mínimos

| Configuração | Memória | Armazenamento | Linode |
|-------|--------|---------|---------|
| Mainnet | 2G | 280G | Linode 16GB |
| Mainnet Prunado | 2G | ~5G | Linode 4GB |
| Testnet | 2G | ~15G | Linode 4GB |
| Testnet Prunado | 2G | ~5G | Linode 4GB |
| Regtest | 2G | ~ | Linode 4GB |

Mas saiba que possam existir maneiras de reduzir custos.

* Para as máquinas que sugerimos o **Linode 4GB**, você pode conseguir reduzí-las para um Linode 2GB. Algumas versões do Bitcoin Core têm funcionado bem com este tamanho, outras têm esgotado a memória de vez em quando e depois recuperado, e outras têm esgotado a memória continuamente. Lembre-se de aumentar o Swap Disk para maximizar as chances disto funcionar. Use por sua conta própria e risco.
* Para o Mainnet Não Prunado, que suregimos como um **Linode 16GB**, você provavelmente conseguirá utilizar um Linode 4GB, mas adicione [Block Storage](https://cloud.linode.com/volumes) suficiente para armazenar a blockchain. Esta é com certeza uma solução melhor para o longo prazo porque os requisitos de armazenamento da blockchain do Bitcoin aumentam continuamente se você não pruná-la, enquanto os requisitos de CPU não aumentam (ou pelo menos não nas mesmas proporções). Um armazenamento GibiByte de 320GB custaria U$32 por mês, que combinado com um Linode 4GB custa U$52 por mês, ao invés de U$80, e mais importante, você pode continuar crescendo-o. Nós não documentamos esta configuração por completo por duas razões: (1) nós não sugerimos a configuração mainnet não prunada, então suspeitamos ser uma configuração incomum; e (2) nós não testamos como volumes Linode comparam com seus SSDs intrínsecos em performance e uso. Mas existe uma documentação completa na página Block Storage. Você precisaria configurar o Linode, executar seu stackscript, e então interrompê-lo para mover o armazenamento da blockchain para um volume comissionado antes de continuar.

### Faça a Configuração Final

A última coisa que você precisa fazer é colocar a senha root. (Se você tiver se esquecido de algo, você será informado agora!)

Clique em "Deploy" para iniciallizar seus discos e preparar seu VPS. A fila inteira deve executar em menos de um minuto. Quando estiver pronto você verá, na "Host Job Queue", botões verdes de "Success" dizendo "Disk Create from StackScript - Setting password for root... done." e "Create Filesystem - 256MB Swap Image".

Agora você pode querer alterar o nome do seu VPS Linode do padrão `linodexxxxxxxx`. Vá para a aba de configurações e altere o rótulo para algo mais útil, como o hostname curto do seu VPS. Por exemplo, você pode chamá-lo de `bitcoin-testnet-prunado` para diferenciá-lo de outros VPSs em sua conta.

## Faça Login no Seu VPS

Se você olhar o painel de controle do seu Linode, você deve ver o novo computador se iniciar. Quando o serviço atingir 100%, você poderá fazer login.

Primeiramente, você precisará do endereço de IP. Clique na aba "Linodes" e uma lista dos seus VPSs deve aparecer, além do fato de que ele está rodando, o seu "plano", endereço de IP, e outras informações.

Vá para o seu console local e faça login na conta `standup` usando aquele endereço:

```
ssh standup@[IP-ADDRESS]
```

Por exemplo:

```
ssh standup@192.168.33.11
```

Se você configurou o seu VPS para usar uma chave SSH, o login deve ser automático (possivelmente requisitando sua senha SSH para desbloquear sua chave). Se você não tiver configurado uma chave SSH, então precisará escrever a senha do user1.

### Espere Alguns Instantes

Aqui vai uma pequena sacada: _o seu StackScript está executando agora_. O script BASH é executado na primeira vex que o VPS é ligado. O que significa que o seu VPS ainda não está pronto.

O tempo total de execução é por volta de 10 minutos. Então, tire um intervalo, vá buscar um café, ou apenas relaxe por alguns instantes. Há duas partes do script que demoram um pouco: a atualização de todos os pacotes do Debian e o download do código do Bitcoin. Eles devem demorar não mais do que 5 minutos cada, o que significa que se você voltar em uns 10 minutos, provavelmente tudo estará pronto.

Se você estiver impaciente, pode pular e fazer `sudo tail -f /standup.log`, que mostrará o progresso da instalação, como descrito na próxima seção.

## Verifique Sua Instalação

Você saberâ que o stackscript terminou quando a `tail` do `standup.log` disser algo como o seguinte:
```
/root/StackScript - Bitcoin is setup as a service and will automatically start if your VPS reboots and so is Tor
/root/StackScript - You can manually stop Bitcoin with: sudo systemctl stop bitcoind.service
/root/StackScript - You can manually start Bitcoin with: sudo systemctl start bitcoind.service
```
Naquele momento, seu diretório home se parecerá com isso:

```
$ ls
bitcoin-0.20.0-x86_64-linux-gnu.tar.gz  laanwj-releases.asc  SHA256SUMS.asc
```

Essas são os vários arquivos que foram usados para instalar o Bitcoin no seu VPS. _Nenhum_ deles é necessário. Nós apenas os deixamos lá caso você queira fazer qualquer verificacão adicional. Caso contrário, você pode deletá-los:

```
$ rm *
```

### Verifique a Configuração do Bitcoin

Para ter certeza de que o Bitcoin baixado é válido, o StackScript checa a assinatura e o checksum SHA. Você deve verificar que ambos estes testes foram bem sucedidos:

```
$ sudo grep VERIFICATION /standup.log
```

Se você ver algo como o seguinte, tudo deve estar OK:

```
/root/StackScript - VERIFICATION SUCCESS / SIG: gpg: Good signature from "Wladimir J. van der Laan (Bitcoin Core binary release signing key) <laanwj@gmail.com>" [unknown]
/root/StackScript - VERIFICATION SUCCESS / SHA: 35ec10f87b6bc1e44fd9cd1157e5dfa4```
```
However, if either of those two checks instead reads "VERIFICATION ERROR", then there's a problem. Since this is all scripted, it's possible that there's just been a minor change that has caused the script's checks not to work right. (This has happened a few times over the existence of the script that became Standup.) But, it's also possible that someone is trying to encourage you to run a fake copy of the Bitcoin daemon. So, _be very sure you know what happened before you make use of Bitcoin!_

Entretanto, se qualquer um desses dois cheques disser "VERIFICATION ERROR", então temos um problema. Como tudo isso é programado, é possível que tenha ocorrido apenas uma mudança pequena que causou erros nos cheques do script. (Isto aconteceu poucas vezes durante a existência do script que virou o Standup.) Mas, também é possível que alguém esteja tentando te empurrar uma cópia falsa do daemon do Bitcoin. Então, _tenha absoluta que você saiba o que de fato aconteceu antes de usar o Bitcoin!_

### Leia os Logs

Você também pode querer ler todos os logs dos arquivos de configuração para se assegurar de que nada imprevisível aconteceu durante a instalação.

É melhor olhar o arquivo de log padrão do StackScript, que tem todas as saídas, incluindo erros:

`$ sudo more /standup.log`

Observe que é totalmente normal ver _alguns_ erros, particularmente quando executando o software gpg e quando várias coisas tentam accessar o dispositivo `/dev/tty` inexistente.

Se, ao invés disso, você quiser olhar para um conjunto menor de informação, todos os erros devem estar em:

`$ sudo more /standup.err`

Ele ainda contém uma certa quantidade de informação além de erros, mas é uma leitura mais rápida.

Se tudo estiver OK, parabéns, você tem um node Bitcoin funcionando usando a Linode!

## O Que Nós Temos Feito

Apesar de a imagem padrão do Debian 10 que estamos usando para o VPS ter sido modificada pela Linode para ser razoavelmente segura, o seu node Bitcoin instalado pelo StackScript Linode é configurado com um nível de segurança ainda maior. Isto pode ser limitante, ou pode te impedir de fazer algumas coisas que você esperava conseguir fazer. O que segue são algumas notas à respeito disso:

### Serviços Protegidos

A instalação do seu Bitcoin VPS é mïnima e permite quase nenhum tipo de comunicação. Isto é feito através do "uncomplicated firewall" (`ufw`), que bloqueia tudo exceto conexões SSH. Também existe alguma segurança adicional possível para suas portas RFC, graças aos serviços ocultos instalados pelo Tor.

**Ajustando UFW.** Você provavelmente deve deixar o UFW no estágio de super-protegido! Você não deve usar uma máquina Bitcoin para outros serviços, porque cada um aumenta sua vulnerabilidade! Se você decidir o oposto, existem vários [guias para UFW](https://www.digitalocean.com/community/tutorials/ufw-essentials-common-firewall-rules-and-commands) que te ajudarão a adicionar serviços. Como o nome diz, é _uncomplicated_. Por exemplo, adicionar serviços de email requeriria apenas abrir a porta de mail: `sudo ufw allow 25`. Mas não faça isso.

**Ajustando Tor.** Você pode querer proteger seus serviços como SSH de uma maneira melhor. Referencie o [Capítulo 12: Usando o Tor](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/12_0_Using_Tor.md) para maiores informações sobre o Tor.

### Shells Protegidas

Se você definiu "SSH-allowed IPs", acesso SSH (e SCP) ao servidor é bastante restrito. `/etc/hosts.deny` impede qualquer um de logar. _Nós não recomendamos que altere isso_. `/etc/hosts.allow` então autoriza endereços de IP específicos. Apenas adicione mais endereços de IP em uma lista separada por vírgulas se você precisar oferecer mais acesso.

Por exemplo:
```
sshd: 127.0.0.1, 192.128.23.1
```

### Atualizações Automáticas

Debian is also set up to automatically upgrade itself, to ensure that it remains abreast of the newest security patches. O Debian também é configurado para se atualizar automaticamente para garantir de que ele mantenha lado a lado aos patches de segurança mais recentes.

Se, por alguma razão, você desejasse alterar isso (_algo que não recomendamos_), você poderia fazer o seguinte:

```
echo "unattended-upgrades unattended-upgrades/enable_auto_updates boolean false" | debconf-set-selections
```

_Se você quiser saber mais sobre o que o Bitcoin Standup StackScript faz, veja o [Apêndice I: Compreendendo o Bitcoin Standup](A1_0_Understanding_Bitcoin_Standup.md)_.

## Brincando com o Bitcoin

Então agora você provavelmente deseja brincar com o Bitcoin!

Mas calma, provavelmente o seu daemon do Bitcoin ainda está baixando blocos. O `bitcoin-cli getblockcount` irá dizer como está indo:
```
$ bitcoin-cli getblockcount
1771352
```
Se o resultado for diferente a cada vez que você digitar o comando, você precisará esperar antes de começar a trabalhar com o Bitcoin. Isto levará, atualmente, de 1 a 6 horas para um node prunado, dependendo da sua máquina específica.

Mas quando estabilizar em um número, você estará pronto para continuar!

Mesmo assim, pode ser uma boa hora para mais alguns cafézinhos. Mas logo logo o seu sistema estará pronto, e você também!

## Resumo: Configurando um Bitcoin-Core VPS Manualmente

Criar um Bitcoin-Core VPS com os scripts Standup fizeram com que todo o processo fosse rápido, simples, e (esperamos) indolor.

## O Que Vem A Seguir?

Você tem algumas opções para o que se seguirá:

   * Leia o [StackScript](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts/blob/master/Scripts/LinodeStandUp.sh) para entender a sua configuração.
   * Leia o que o StackScript faz no [Apêndice I: Compreendendo o Bitcoin Standup](A1_0_Understanding_Bitcoin_Standup.md).
   * Escolha uma metodologia inteiramente alternativa em [§2.2: Configurando uma Máquina Bitcoin-Core de Outras Maneiras](02_2_Setting_Up_Bitcoin_Core_Other.md).
   * Siga em frente para "bitcoin-cli" com o [Capítulo Três: Compreendendo a Sua Configuração do Bitcoin](03_0_Understanding_Your_Bitcoin_Setup.md).

## Sinopse: Tipos de Instalação do Bitcoin

**Mainnet.** Isto irá fazer o download da blockchain inteira do Bitcoin. São 280GB de dados (e aumentando a cada dia).

**Mainnet Prunada.** Isto irá reduzir a blockchain que você irá armazenar para apenas os últimos 550 blocos. Se você não estiver minerando ou executando outro serviço do Bitcoin, isso deve ser o suficiente para validação.

**Testnet.** Isto te dará acesso a uma blockchain alternativa do Bitcoin onde os bitcoins não têm valor algum. É feita para experimentos e testes.

**Testnet Prunada.** Isto são apenas os últimos 550 blocos da Testnet ... porque a blockchain da Testnet está meio grande agora também.

**Regtest Privada.** Este é o modo Regression Testing, que te permite rodar um servidor de Bitcoin totalmente local. Te permite também realizar testes mais detalhados. Não há um modo prunado aqui, porque você estará começando do zero. Esta é uma configuração bem diferente, então é discutida no [Apêndice 3](A3_0_Using_Bitcoin_Regtest.md).
