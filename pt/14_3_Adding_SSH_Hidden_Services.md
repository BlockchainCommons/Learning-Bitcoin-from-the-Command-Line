# 14.3: Adicionando Serviços SSH Ocultos

>:information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho inicial que ainda pode estar aguardando revisão.

Até agora, usamos o Tor com os nossos serviços Bitcoin, mas também podemos usá-lo para proteger outros serviços em nossa máquina, melhorando a segurança e a privacidade. Esta seção demonstra como fazer isso, introduzindo um serviço ```ssh``` oculto para fazer o login remotamente usando o Tor.

## Criando Serviços SSH Ocultos

Novos serviços são criados adicionando-os ao arquivo ```/etc/tor/torrc```:
```
$ su 
# cat >> /etc/tor/torrc << EOF
HiddenServiceDir /var/lib/tor/hidden-service-ssh/
HiddenServicePort 22 127.0.0.1:22
EOF
# exit
```
Eis o que cada coisa significa:

* HiddenServiceDir: Indica que temos um diretório de serviço oculto com a configuração necessária para este caminho.
* HiddenServicePort: Indica a porta Tor a ser usada; no caso do SSH, geralmente é a 22.

Depois de adicionar as linhas apropriadas ao nosso arquivo ```torrc```, precisaremos reiniciar o Tor:
```
$ sudo /etc/init.d/tor restart
```

Após a reinicialização, nosso ```HiddenServiceDir``` deve ter novos arquivos da seguinte forma:
```
$ sudo ls -l /var/lib/tor/hidden-service-ssh
total 16
drwx--S--- 2 debian-tor debian-tor 4096 Jul 22 14:55 authorized_clients
-rw------- 1 debian-tor debian-tor   63 Jul 22 14:56 hostname
-rw------- 1 debian-tor debian-tor   64 Jul 22 14:55 hs_ed25519_public_key
-rw------- 1 debian-tor debian-tor   96 Jul 22 14:55 hs_ed25519_secret_key
```
O arquivo ```hostname``` neste diretório contém nosso novo ID onion:
```
$ sudo cat /var/lib/tor/hidden-service-ssh/hostname
qwkemc3vusd73glx22t3sglf7izs75hqodxsgjqgqlujemv73j73qpid.onion
```
Podemos nos conectar ao serviço oculto ```ssh``` usando o ```torify``` e esse endereço:
```
$ torify ssh standup@qwkemc3vusd73glx22t3sglf7izs75hqodxsgjqgqlujemv73j73qpid.onion
The authenticity of host 'qwkemc3vusd73glx22t3sglf7izs75hqodxsgjqgqlujemv73j73qpid.onion (127.42.42.0)' can't be established.
ECDSA key fingerprint is SHA256:LQiWMtM8qD4Nv7eYT1XwBPDq8fztQafEJ5nfpNdDtCU.
Are you sure you want to continue connecting (yes/no)? yes
Warning: Permanently added 'qwkemc3vusd73glx22t3sglf7izs75hqodxsgjqgqlujemv73j73qpid.onion' (ECDSA) to the list of known hosts.
standup@qwkemc3vusd73glx22t3sglf7izs75hqodxsgjqgqlujemv73j73qpid.onion's password: 
```
## Resumo: Adicionando Serviços SSH Ocultos

Agora que instalamos o Tor e sabemos como usá-lo, podemos adicionar outros serviços ao Tor. Apenas adicionamos as linhas ao nosso ```torrc``` (no nosso servidor) e o conectamos com o ```torify``` (no nosso cliente).

> :fire: ***Qual é o poder de utilizar outros serviços ocultos?*** Cada vez que acessamos um serviço em nosso servidor remotamente, deixamos pegadas na rede. Mesmo que os dados sejam criptografados usando SSH (ou TLS), os vigias da rede podem ver de onde estamos nos conectando, para onde estamos conectando e qual serviço estamos usando. Isso realmente importa pra nós? Esta é a pergunta que devemos fazer. Mas se a resposta for "Sim", podemos proteger a conexão com um serviço oculto.

## O Que Vem Depois?

Para um tipo diferente de privacidade, vamos seguir para "Usando o I2P" com o [Capítulo Quize: Usando o I2P](15_0_Using_i2p.md)