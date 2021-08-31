# 14.2: Mudando Nossos Serviços Ocultos do Bitcoin

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho inicial que ainda pode estar aguardando revisão.

Temos um serviço Tor funcionando, mas com o tempo talvez queiramos reiniciá-lo ou ajustá-lo de outra forma.

## Protejendo Nossos Serviços Ocultos

O Tor permite que limitemos quais clientes conversam com nossos serviços ocultos. Se ainda não autorizamos nosso cliente durante a configuração do servidor, na primeira oportunidade devemos fazer o seguinte:

1. Solicitar uma chave pública de autenticação Tor V3 do nosso cliente. (Na [GordianWallet](https://github.com/BlockchainCommons/GordianWallet-iOS), está disponível no menu de configurações).
2. Vamos para o subdiretório apropriado para nosso serviço oculto do Bitcoin. Se usamos o Bitcoin Standup é ```/var/lib/tor/standup/```.
3. Vamos para o subdiretório ```authorized_clients```.
4. Vamos adicionar um arquivo chamado ```[qualquer coisa].auth```. O ```[qualquer coisa]``` pode ser realmente qualquer coisa.
5. Colocamos a chave pública (e nada mais) no arquivo.

Depois de adicionar um arquivo ```.auth``` ao subdiretório ```authorized_client```, somente os clientes autorizados serão capazes de se comunicar com esse serviço oculto. Podemos adicionar mais ou menos 330 chaves públicas diferentes para habilitar clientes diferentes.

## Redefinindo Nosso Endereço Onion do ```bitcoind```

Se quisermos redefinir nosso endereço onion para o ```bitcoind```, podemos apenas remover o ```onion_private_key``` do nosso diretório de dados, no `~/.bitcoin/testnet`:
```
$ cd ~/.bitcoin/testnet
$ rm onion_private_key 
```
Quando reiniciarmos, um novo endereço onion será gerado:
```
2020-07-22T23:52:27Z tor: Got service ID pyrtqyiqbwb3rhe7, advertising service pyrtqyiqbwb3rhe7.onion:18333
2020-07-22T23:52:27Z tor: Cached service private key to /home/standup/.bitcoin/testnet3/onion_private_key
```

## Redefinindo Nosso Endereço Onion do RPC

Se quisermos redefinir nosso endereço onion para o acesso RPC, podemos excluir da mesma forma o ```HiddenServiceDirectory``` apropriado e reiniciar o Tor:
```
$ sudo rm -rf /var/lib/tor/standup/
$ sudo /etc/init.d/tor restart
```

> :warning: **AVISO:** Redefinir o endereço onion do RPC desconectará quaisquer carteiras mobile ou outros serviços que contamos usando a API Quicklink. Por isso, temos que tomar bastante cuidado.

## Forçando o ```bitcoind``` a Usar o Tor

Finalmente, podemos forçar o ```bitcoind``` a usar o onion adicionando o seguinte ao nosso arquivo ```bitcoin.conf```:
```
proxy=127.0.0.1:9050
listen=1
bind=127.0.0.1
onlynet=onion
```
Precisaremos adicionar seed nodes baseados no onion ou outros nodes à nossa configuração, mais uma vez editando o ```bitcoin.conf```:
```
seednode=address.onion
seednode=address.onion
seednode=address.onion
seednode=address.onion
addnode=address.onion
addnode=address.onion
addnode=address.onion
addnode=address.onion
```
Depois, basta reiniciar o ```tor``` e o ```bitcoind```.

Agora devemos estar nos comunicando exclusivamente com o Tor. Mas, a menos que estejamos em um local hostil, esse nível de anonimato provavelmente não é necessário. Também não é particularmente recomendado, pois podemos diminuir muito o número de potenciais peers, criando problemas de censura ou mesmo de correlação. Também podemos começar a sofrer com alta latência. E essa configuração pode nos dar uma falsa sensação de anonimato que realmente não existe na rede Bitcoin.

> :warning: **AVISO:** Esta configuração não foi testada! Use por sua conta e risco!

## Resumo: Mudando Nossos Serviços Ocultos do Bitcoin

Provavelmente nós não precisaremos brincar com os nossos serviços onion depois de verificá-los, mas caso precisemos, aqui está como redefinir um endereço Tor que foi comprometido ou passar a usar exclusivamente o Tor com nosso ```bitcoind``` .

## O Que Vem Depois?

Vamos continuar "Usando o Tor" na seção [14.3: Adicionando Serviços SSH Ocultos](14_3_Adding_SSH_Hidden_Services.md).