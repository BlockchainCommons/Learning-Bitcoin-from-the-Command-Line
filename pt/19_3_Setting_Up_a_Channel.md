# 19.3: Criando um Canal Lightning

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho inicial que ainda pode estar aguardando revisão.

Agora que entendemos o básico da configuração da Lightning e, com sorte, já criamos ou recebemos informações sobre um segundo node Lightning, estamos prontos para criar nosso primeiro canal na Lightning Network. Claro, precisaremos entender o que ele é e como é criado usando a c-lightning.

> :book: ***O que é um canal Lightning?*** De maneira simples, um canal Lightning é um tubo de dinheiro que permite transferências rápidas, baratas e privadas sem enviar transações para a blockchain. Mais tecnicamente, um canal é uma transação multisig 2-de-2 no Bitcoin que estabelece um relacionamento financeiro sem confiança entre duas pessoas ou dois agentes. Uma certa quantia de dinheiro é depositada no canal, quando então se mantém um banco de dados local com saldo em bitcoins para ambas as partes, mantendo o registro de qual é o saldo de cada parte. Os dois usuários podem então trocar bitcoins por meio do canal Lightning sem nunca escrever na blockchain do Bitcoin. Somente quando desejam fechar o canal é que eles dividem os bitcoins na blockchain, com base na divisão final das moedas para cada um.

> :book: ***Como os canais Lightning criam uma rede Lightning?*** Embora um canal Lightning só permita o pagamento entre dois usuários, os canais podem ser conectados para formar uma rede que permite pagamentos entre membros que não têm um canal direto entre eles. Isso cria uma rede entre várias pessoas, construída a partir de conexões em pares.

Nesta seção, continuaremos usando nossa configuraçãa c-lightning como nosso node principal.

## Criando um Canal

A criação de um canal Lightning requer as seguintes etapas:

* Financiar nossa carteira c-lightning com alguns satoshis;
* Conectar-se a um node remoto como um par;
* Abrir um canal.

### Financiando Nossa Carteira c-lightning

Para mover fundos para um canal Lightning, primeiro é necessário financiar nossa carteira c-lightning.

> :book: ***O que é uma carteira c-lightning?*** A implementação padrão da c-lightning vem com uma carteira Bitcoin integrada que permite enviar e receber transações de bitcoin na blockchain. Esta carteira será usada para criar novos canais.

A primeira coisa que precisamos fazer é enviar alguns satoshis para nossa carteira c-lightning. Podemos criar um novo endereço usando o comando `lightning-cli newaddr`. Isto gera um novo endereço que pode ser subsequentemente usado para financiar canais gerenciados pelo node c-lightning. Podemos especificar o tipo de endereço desejado; se não for especificado, o endereço gerado será um bech32.

```
$ lightning-cli --testnet newaddr
{
   "address": "tb1qefule33u7ukfuzkmxpz02kwejl8j8dt5jpgtu6",
   "bech32": "tb1qefule33u7ukfuzkmxpz02kwejl8j8dt5jpgtu6"
}
``` 
Podemos então enviar fundos para este endereço usando `bitcoin-cli sendtoaddress` (ou qualquer outro método de preferência). Para este exemplo, fizemos o envio que pode ser observado na transação [11094bb9ac29ce5af9f1e5a0e4aac2066ae132f25b72bff90fcddf64bf2feb02](https://mempool.space/pt/testnet/tx/11094bb9ac29ce5af9f1e5a0e4aac2066ae132f25b72bff90fcddf64bf2feb02).

Esta transação é chamada de [transação de financiamento](https://github.com/lightningnetwork/lightning-rfc/blob/master/03-transactions.md#funding-transaction-output) e precisa ser confirmada antes que os fundos possam ser utilizados.

> :book: ***O que é uma transação de financiamento?*** Uma transação de financiamento é uma transação de bitcoin que coloca dinheiro em um canal Lightning. Pode ser de financiamento único (por um participante) ou de financiamento duplo (por ambos). A partir daí, as transações da Lightning tratam de realocar a propriedade da transação de financiamento, mas só se ajustam na blockchain quando o canal é fechado.

Para verificar nosso saldo local, devemos usar o comando `lightning-cli listfunds`:

```       
c$ lightning-cli --testnet listfunds
{
   "outputs": [],
   "channels": []
}
```

Como os fundos ainda não têm seis confirmações, não há saldo disponível. Após seis confirmações, devemos ver o saldo alterado:
```       
c$ lightning-cli --testnet listfunds
{
   "outputs": [
      {
         "txid": "11094bb9ac29ce5af9f1e5a0e4aac2066ae132f25b72bff90fcddf64bf2feb02",
         "output": 0,
         "value": 300000,
         "amount_msat": "300000000msat",
         "scriptpubkey": "0014ca79fcc63cf72c9e0adb3044f559d997cf23b574",
         "address": "tb1qefule33u7ukfuzkmxpz02kwejl8j8dt5jpgtu6",
         "status": "confirmed",
         "blockheight": 1780680,
         "reserved": false
      }
   ],
   "channels": []
}

``` 
Observe que o valor está listado em satoshis ou microsatoshis, não em Bitcoin!

> :book: ***O que são satoshis e msats?*** Já conhecemos os satoshis na seção [§3.4](03_4_Receiving_a_Transaction.md). Um satoshi é o centésimo milionésimo de um bitcoin, então 300.000 satoshis equivalem a 0,003 BTC. Um satoshi é a menor unidade monetária na rede Bitcoin. Mas, a rede Lightning pode ser menor, então 1.000 msat, ou milisatoshis, equivalem a um satoshi. Isso significa que 1 msat é o centésimo bilionésimo de um bitcoin e 300.000.000 msat equivalem a 0,003 BTC.

Agora que financiamos nossa carteira c-lightning, precisaremos de informações sobre um node remoto para começar a criar o processo do canal.

### Conectando a um Node Remoto

A próxima coisa que precisaremos fazer é conectar nosso node a um par. Isso é feito com o comando `lightning-cli connect`. Lembre-se que se quisermos mais informações sobre este comando, devemos digitar `lightning-cli help connect`.

Para conectar nosso node a um par remoto, precisaremos do nosso id, que representa a chave pública do node de destino. Por conveniência, o `ID` pode ter a forma `id@host` ou `id@host:port`. Podemos já ter pego esta informação com o `lightning-cli getinfo` (na c-lightning) ou `lncli --network=testnet getinfo` (no LND) conforme discutido no [adendo anterior](19_2__Interlude_Accessing_a_Second_Lightning_Node.md).

Selecionamos o node LND, `032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543`, que está localizado no endereço IP `45.33.35.151`, ao qual vamos nos conectar a partir de nosso node c-lightning:

```       
$ lightning-cli --network=testnet connect 032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543@45.33.35.151
{
   "id": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
   "features": "02a2a1"
}
```     

### Abrindo um Canal

O comando fundchannel do RPC abre um canal de pagamento com um par ao comprometer uma transação de financiamento para a blockchain. Devemos usar o comando `lightning-cli fundchannel` para fazer isso, com os seguintes parâmetros:

* **id** é o retorno do id do peer da conexão;
* **amount** é o valor em satoshis retirado da carteira interna para financiar o canal. O valor não pode ser inferior ao limite mínimo, atualmente definido como 546 satoshis, nem superior a 16.777.215 satoshis (a menos que grandes canais tenham sido negociados com o par).
* **feerate** é o feerate opcional usado para a transação de abertura e como feerate inicial para transações de confirmação e HTLC.
* **announce** é um sinalizador opcional que aciona o anúncio deste canal ou não. O padrão é verdadeiro. Se desejarmos criar um canal privado, precisamos definí-lo como falso.
* **minconf** especifica o número mínimo de confirmações que saídas usadas no processo de abertura do canal devem ter. O padrão é 1.
* **utxos** especifica os utxos a serem usados ​​para financiar o canal, como uma matriz de “txid:vout”.

Agora podemos abrir o canal assim:

```
$ lightning-cli --testnet fundchannel 032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543 100000 urgent true 1
{
   "tx": "0200000000010193dc3337837f091718f47b71f2eae8b745ec307231471f6a6aab953c3ea0e3b50100000000fdffffff02a0860100000000002200202e30365fe321a435e5f66962492163302f118c13e215ea8928de88cc46666c1d07860100000000001600142fe02e5be9283e8c5bcb93ae61421baf8cb64f9c024730440220668a7c253c9fd83fc1b45e4a52823fb6bc5fad30da36240d4604f0d6981a6f4502202aeb1da5fbbc8790791ef72b3378005fe98d485d22ffeb35e54a6fbc73178fb2012103b3efe051712e9fa6d90008186e96320491cfe1ef1922d74af5bc6d3307843327c76c1c00",
   "txid": "66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d",
   "channel_id": "1d3cf2126ae36e12be3aee893b385ed6a2e19b1da7f4e579e3ef15ca234d6966",
   "outnum": 0
}
```
Para confirmar o status do canal, vamos usar o comando `lightning-cli listfunds`:

```
c$ lightning-cli --testnet listfunds
{
   "outputs": [
      {
         "txid": "66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d",
         "output": 1,
         "value": 99847,
         "amount_msat": "99847000msat",
         "scriptpubkey": "00142fe02e5be9283e8c5bcb93ae61421baf8cb64f9c",
         "address": "tb1q9lszuklf9qlgck7tjwhxzssm47xtvnuu4jslf8",
         "status": "unconfirmed",
         "reserved": false
      },
      {
         "txid": "b5e3a03e3c95ab6a6a1f47317230ec45b7e8eaf2717bf41817097f833733dc93",
         "output": 1,
         "value": 200000,
         "amount_msat": "200000000msat",
         "scriptpubkey": "0014ed54b65eae3da99b23a48bf8827c9acd78079469",
         "address": "tb1qa42tvh4w8k5ekgay30ugyly6e4uq09rfpqf9md",
         "status": "confirmed",
         "blockheight": 1862831,
         "reserved": true
      }
   ],
   "channels": [
      {
         "peer_id": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
         "connected": true,
         "state": "CHANNELD_AWAITING_LOCKIN",
         "channel_sat": 100000,
         "our_amount_msat": "100000000msat",
         "channel_total_sat": 100000,
         "amount_msat": "100000000msat",
         "funding_txid": "66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d",
         "funding_output": 0
      }
   ]
}

```
Embora este novo canal com 100.000 satoshis não esteja confirmado, nosso estado será `CHANNELD_AWAITING_LOCKIN`. Observe que a alteração não confirmada de `99847` satoshis também está aparecendo como uma nova transação na carteira. Depois que todas as seis confirmações forem concluídas, o canal mudará para o estado `CHANNELD_NORMAL`, que será o estado permanente. Nesse momento, um `short_channel_id` também aparecerá, por exemplo:
```
"short_channel_id": "1862856x29x0",
```
Esses valores indicam onde a transação de financiamento pode ser encontrada na blockchain. Ela aparece na forma `bloco x txid x vout`.

Neste caso, `1862856x29x0` significa:

* Criado no bloco 1862856;
* Com um `txid` de 29 e;
* Um `vout` de 0.

Podemos precisar usar este `short_channel_id` para certos comandos na Lightning.

Esta transação de financiamento também pode ser encontrada onchain pelo TXID [66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d] (https://mempool.space/pt/testnet/tx/66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d)

> :book: ***O que é a capacidade do canal?*** Em um canal Lightning, ambos os lados do canal possuem uma parte da capacidade. O valor do seu lado do canal é chamado de *saldo local (local balance)* e o valor do outro lado é chamado de *saldo remoto (remote balance)*. Ambos os saldos podem ser atualizados muitas vezes sem fechar o canal (quando o saldo final é enviado para a blockchain), mas a capacidade do canal não pode mudar sem fechá-lo. A capacidade total de um canal é a soma do saldo de cada participante do canal.

## Resumo: Criando um Canal Lightning

Precisaremos criar um canal com um node remoto para poder receber e enviar dinheiro pela Lightning Network.

## O Que Vem Depois?

Você está pronto para passar para o [Capítulo 20: Usando a Lightning](20_0_Using_Lightning.md).