# 15.1: Bitcoin Core como um Serviço I2P (Projeto Internet Invisível)

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e ainda é um rascunho inicial que pode estar aguardando revisão. Proceda com cuidado, leitor.

Ao invés de utilizarmos o serviço baseado em proxy do Tor para garantir a privacidade das nossas comunicações no Bitcoin, podemos querer utilizar o I2P, que é projetado para agir como uma rede privada dentro da internet, ao invés de simplesmente oferecer acesso privado a serviços de internet.

## Compreendendo as Diferenças

Tanto o Tor quanto o I2P oferecem acesso privado a serviços online, mas com roteamentos, bancos de dados e arquiteturas para retransmissões diferentes. Como serviços ocultos (como acesso ao Bitcoin) são a base do design do I2P, eles também foram otimizados:

|                        | Tor                                                          | I2P                                                          |
| ---------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| Roteamento             | [Onion](https://www.onion-router.net/)                       | [Garlic](https://geti2p.net/en/docs/how/garlic-routing)      |
| Banco de Dados da Rede | [Servidores de Diretório](https://blog.torproject.org/possible-upcoming-attempts-disable-tor-network) Confiáveis | [Banco de Dados de Rede Distribuído](https://geti2p.net/en/docs/how/network-database) |
| Retransmissão          | Conexões criptografadas de **mão-dupla** entre cada Retransmissor | Conexões de **mão-única** entre todo servidor em seus túneis |
| Serviços Ocultos       | Devagar                                                      | Rápido                                                       |

Podemos encontrar uma comparação mais detalhada em [geti2p.net](https://geti2p.net/pt-br/comparison/tor).

### Compreenda os Tradeoffs para Limitar as Conexões de Saída

Existem [tradeoffs](https://bitcoin.stackexchange.com/questions/107060/tor-and-i2p-tradeoffs-in-bitcoin-core) caso escolhamos usar apenas o I2P, apenas o Tor, ou ambos. Essas configurações, que limitam conexões de saída na clearnet, são feitas no Bitcoin Core usando o argumento `onlynet` no `bitcoin.conf`.

* `onlynet=onion`, que limita conexões de saída ao Tor, pode expor um node a ataques "Sybil" e criar redes separadas, devido às conexões limitadas entre o Tornet e a clearnet.
* `onlynet=onion` e `onlynet=i2p`, juntas, que executa serviço Onion com serviço I2P, é experimental por enquanto.

## Instalando o I2P

Para instalar o I2P, devemos nos certificar de que configuramos as portas de maneira correta, para então, continuar com o processo.

### Preparando as Portas

Para usar o I2P, devemos abrir as seguintes portas, necessárias para o I2P:

1. **De saída (para a internet):** uma porta aleatória entre 9000 e 31000 é selecionada. É melhor se todas essas portas estiverem abertas para conexões de saída, o que não afeta nossa segurança.
   - Podemos checar o status do nosso firewall usando `sudo ufw status verbose`, o que não deve negar conexões de saída por padrão.
2. **De entrada (olhando para a internet)**: opcional. A variedade de portas de entrada são listadas na [documentação do I2P](https://geti2p.net/pt-br/faq#ports)
   - Para maximizarmos privacidade, é preferível desabilitarmos as conexões de entrada.

### Executando o I2P

Os comandos à seguir irão executar os serviços I2P no Bitcoin Core:

1. Instalando `i2pd` no Ubuntu:

```
   sudo add-apt-repository ppa:purplei2p/i2pd
   sudo apt-get update
   sudo apt-get install i2pd
```

   Para instalarmos em outros sistemas operacionais, podemos ver [a documentação](https://i2pd.readthedocs.io/en/latest/user-guide/install/).

2. [Executando](https://i2pd.readthedocs.io/en/latest/user-guide/run/) o serviço I2P:

```
   $ sudo systemctl start i2pd.service
```

3. Para verificar se o I2P está executando, devemos observar a porta 7656:

```
   $ ss -nlt
   
   State   Recv-Q   Send-Q     Local Address:Port      Peer Address:Port  Process
   
   LISTEN  0        4096           127.0.0.1:7656           0.0.0.0:*
```

4. Devemos adicionar as seguintes linhas no `bitcoin.conf`:

```
   i2psam=127.0.0.1:7656
   debug=i2p
```

   A opção para os logs, `debug=i2p`, é utilizada para observarmos as informações adicionais no debug log sobre nossas configurações e conexões I2P. O lugar padrão para esse arquivo de debug no Linux é: `~/.bitcoin/debug.log`

5. Reiniciando o `bitcoind`

```
   $ bitcoind
```

6. Devemos verificar se o I2P foi configurado corretamente olhando o `debug.log`, ou se algum erro apareceu nos logs.

```
   2021-06-15T20:36:16Z i2paccept thread start
   2021-06-15T20:36:16Z I2P: Creating SAM session with 127.0.0.1:7656
   
   2021-06-15T20:36:56Z I2P: SAM session created: session id=3e0f35228b, my address=bmwyyuzyqdc5dcx27s4baltbu6zw7rbqfl2nmclt45i7ng3ul4pa.b32.i2p:18333
   2021-06-15T20:36:56Z AddLocal(bmwyyuzyqdc5dcx27s4baltbu6zw7rbqfl2nmclt45i7ng3ul4pa.b32.i2p:18333,4)
```

   O nosso endereço I2P é mencionado nos logs, e termina com _b32.i2p_. Por exemplo `bmwyyuzyqdc5dcx27s4baltbu6zw7rbqfl2nmclt45i7ng3ul4pa.b32.i2p:18333`.

7. Devemos confirmar que `i2p_private_key` foi criado no diretório do Bitcoin Core. A primeira vez que o Bitcoin Core conectar ao roteador I2P, o endereço I2P (e a chave privada correspondente) será automaticamente gerado e salvado em um arquivo chamado *i2p_private_key*:

```
   ~/.bitcoin/testnet3$ ls
   
   anchors.dat  chainstate         i2p_private_key  settings.json
   banlist.dat  debug.log          mempool.dat      wallets
   blocks       fee_estimates.dat  peers.dat
```

8. Devemos verificar que `bitcoin-cli -netinfo` ou `bitcoin-cli getnetworkinfo` retorna o endereço I2P:

```
   Local addresses
   bmwyyuzyqdc5dcx27s4baltbu6zw7rbqfl2nmclt45i7ng3ul4pa.b32.i2p     port  18333    score      4
```

Agora temos um servidor do Bitcoin acessível através da rede I2P no nosso novo endereço local.

## Resumo: Bitcoin Core como um Serviço I2P (Projeto Internet Invisível)

É sempre positivo termos alternativas para privacidade e não dependermos exclusivamente do Tor para executarmos o Bitcoin Core como um serviço oculto. Como I2P foi adicionado recentemente ao Bitcoin Core, poucas pessoas o utilizam. Podemos experimentá-lo e reportar bugs se encontrarmos algum problema.

> :information_source: **NOTA:** Para a implementação oficial do I2P em Java, podemos visitar a [página de download do I2P](https://geti2p.net/en/download) e seguirmos as instruções para nosso Sistema Operacional. Após instalado, podemos abrir uma janela do Terminal e escrever `i2prouter start`. Em seguida, podemos visitar o endereço `127.0.0.1:76571` em um navegador e permitirmos o SAM. Para fazermos isso, selecionamos: "Configure Homepage", então "Clients", e finalmente selecionar o "Play Button" ao lado da Bridge de aplicativo SAM. No lado esquerdo da página, devemos ver uma luz verde próximo a "Shared Clients".

Siga em frente para o "Programando com RPC" no [Capítulo Dezesseis: Conversando com o Bitcoind com C](16_0_Talking_to_Bitcoind.md).

Ou, se não formos programadores, podemos pular para o [Capítulo Dezenove: Compreendendo Sua Configuração Lightning](https://github.com/namcios/Learning-Bitcoin-from-the-Command-Line/blob/portuguese-translation/pt/19_0_Understanding_Your_Lightning_Setup.md) para continuarmos nosso aprendizado da linha de comando com a rede Lightning.
