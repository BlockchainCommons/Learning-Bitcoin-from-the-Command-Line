# 20.1: Gerando um Invoice

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho inicial que ainda pode estar aguardando revisão.

Esta seção descreve como os pagamentos funcionam na Lightning Network, como criar uma solicitação de pagamento (ou _invoice_) e, finalmente, como entendê-la. A emissão de invoices depende de termos um segundo node Lightning, conforme descrito na seção [Acessando um Segundo Node Lightning](19_2__Interlude_Accessing_a_Second_Lightning_Node.md). Esses exemplos usarão um node LND como nosso node secundário, para demonstrar ainda mais as possibilidades da Lightning Network. Para diferenciar entre os nodes nestes exemplos, os prompts serão mostrados como `c $` para o node c-lightning e `lnd $` para o node LND. Se quisermos reproduzir essas etapas, devemos [instalar nosso próprio node LND secundário](19_2__Interlude_Accessing_a_Second_Lightning_Node.md#Creating-a-new-lnd-node).

> :book: ***O que é um invoice?*** Quase todos os pagamentos feitos na Lightning Network exigem um invoice, que nada mais é do que um **pedido de pagamento** feito pelo destinatário do dinheiro e enviado por qualquer meio para o usuário que irá pagar. Todos os invoices são de uso único. Os invoices da Lightning usam a codificação bech32, que já é usada pela Segregated Witness para Bitcoin.

## Criando um Invoice

Para criar um novo invoice na c-lightning, usaríamos o comando `lightning-cli --testnet invoice`.

Vamos ver como funcionaria com o c-lightning, usando argumentos de um valor (em milisats), um rótulo e uma descrição.
```
c$ lightning-cli --testnet invoice 100000 joe-payment "The money you owe me for dinner"
{
   "payment_hash": "07a1c4bd7a38b4dea35f301c173cd8f9aac253b66bd8404d7ad829f226342490",
   "expires_at": 1603305795,
   "bolt11": "lntb1u1p0cw3krpp5q7suf0t68z6dag6lxqwpw0xclx4vy5akd0vyqnt6mq5lyf35yjgqdpj235x2grddahx27fq09hh2gr0wajjqmt9ypnx7u3qv35kumn9wgxqyjw5qcqp2sp5r3puay46tffdyzldjv39fw6tzdgu2hnlszamqhnmgjsuxqxavpgs9qy9qsqatawvx44x5qa22m7td84jau5450v7j6sl5224tlv9k5v7wdygq9qr4drz795lfnl52gklvyvnha5e5lx72lzzmgzcfnp942va5thmhsp5sx7c2",
   "warning_capacity": "No channels",
   "warning_mpp_capacity": "The total incoming capacity is still insufficient even if the payer had MPP capability."
}
```
No entanto, para este exemplo, vamos gerar um invoice em um node LND e, em seguida, pagá-lo no node c-lightning. Isso requer o comando `addinvoice` ligeiramente diferente na LND. Podemos usar o argumento `--amt` para indicar a quantia a ser paga (em milisats) e adicionar uma descrição usando o argumento `--memo`.

```
lnd$ lncli -n testnet addinvoice --amt 10000 --memo "First LN Payment - Learning Bitcoin and Lightning from the Command line."
{
    "r_hash": "6cacdedc95b89eec15e5244bd0957b88c0ab58b153eee549735b995344bc16bb",
    "payment_request": "lntb100u1p0cwnqtpp5djkdahy4hz0wc909y39ap9tm3rq2kk9320hw2jtntwv4x39uz6asdr5ge5hyum5ypxyugzsv9uk6etwwssz6gzvv4shymnfdenjqsnfw33k76twypskuepqf35kw6r5de5kueeqveex7mfqw35x2gzrdakk6ctwvssxc6twv5hqcqzpgsp5a9ryqw7t23myn9psd36ra5alzvp6lzhxua58609teslwqmdljpxs9qy9qsq9ee7h500jazef6c306psr0ncru469zgyr2m2h32c6ser28vrvh5j4q23c073xsvmjwgv9wtk2q7j6pj09fn53v2vkrdkgsjv7njh9aqqtjn3vd",
    "add_index": "1"
}
```
Observe que esses invoices não fazem referência direta ao canal que criamos: isso é necessário para o pagamento, mas não para solicitar o pagamento.

## Compreendendo um Invoice

O `bolt11 payment_request` que criamos é composto de duas partes: uma é legível por humanos e a outra são apenas dados.

> :book: **O que é um BOLT?** Os BOLTs são as [especificações individuais da Lightning Network](https://github.com/lightningnetwork/lightning-rfc).

### Lendo a Parte Legível do Invoice

A parte legível dos invoices começa com um `ln`. É `lnbc` para Bitcoin mainnet, `lntb` para Bitcoin testnet ou `lnbcrt` para Bitcoin regtest.
Em seguida, listamos os fundos solicitados no invoice.

Por exemplo, vamos olhar para a nossa fatura do node LND:
```
lntb100u1p0cwnqtpp5djkdahy4hz0wc909y39ap9tm3rq2kk9320hw2jtntwv4x39uz6asdr5ge5hyum5ypxyugzsv9uk6etwwssz6gzvv4shymnfdenjqsnfw33k76twypskuepqf35kw6r5de5kueeqveex7mfqw35x2gzrdakk6ctwvssxc6twv5hqcqzpgsp5a9ryqw7t23myn9psd36ra5alzvp6lzhxua58609teslwqmdljpxs9qy9qsq9ee7h500jazef6c306psr0ncru469zgyr2m2h32c6ser28vrvh5j4q23c073xsvmjwgv9wtk2q7j6pj09fn53v2vkrdkgsjv7njh9aqqtjn3vd
```
A parte legível por humanos é `ln` +`tb` + `100u`.

O `lntb` diz que esta é um invoice da Lightning Network para bitcoins Testnet.

O `100u` diz que é para 100 bitcoins vezes o multiplicador microsatoshi. Existem quatro multiplicadores de fundos (opcionais):

* `m` (mili): multiplique por 0,001
* `u` (micro): multiplique por 0,000001
* `n` (nano): multiplique por 0,000000001
* `p` (pico): multiplique por 0,000000000001

100 BTC * 0,000001 = 0,0001 BTC, que é o mesmo que 10.000 satoshis.

### Lendo a Parte do Invoice Referente aos Dados

O resto do invoice (`1p0cwnqtpp5djkdahy4hz0wc909y39ap9tm3rq2kk9320hw2jtntwv4x39uz6asdr5ge5hyum5ypxyugzsv9uk6etwwssz6gzvv4shymnfdenjqsnfw33k76twypskuepqf35kw6r5de5kueeqveex7mfqw35x2gzrdakk6ctwvssxc6twv5hqcqzpgsp5a9ryqw7t23myn9psd36ra5alzvp6lzhxua58609teslwqmdljpxs9qy9qsq9ee7h500jazef6c306psr0ncru469zgyr2m2h32c6ser28vrvh5j4q23c073xsvmjwgv9wtk2q7j6pj09fn53v2vkrdkgsjv7njh9aqqtjn3vd`) contém um marcador de tempo, dados especificamente marcados e uma assinatura. Obviamente, não pode ler sem decodificá-lo, mas podemos pedir ao `lightning-cli` para fazer isso com o comando `decodepay`:
```
c$ lightning-cli --testnet decodepay lntb100u1p0cwnqtpp5djkdahy4hz0wc909y39ap9tm3rq2kk9320hw2jtntwv4x39uz6asdr5ge5hyum5ypxyugzsv9uk6etwwssz6gzvv4shymnfdenjqsnfw33k76twypskuepqf35kw6r5de5kueeqveex7mfqw35x2gzrdakk6ctwvssxc6twv5hqcqzpgsp5a9ryqw7t23myn9psd36ra5alzvp6lzhxua58609teslwqmdljpxs9qy9qsq9ee7h500jazef6c306psr0ncru469zgyr2m2h32c6ser28vrvh5j4q23c073xsvmjwgv9wtk2q7j6pj09fn53v2vkrdkgsjv7njh9aqqtjn3vd
{
   "currency": "tb",
   "created_at": 1602702347,
   "expiry": 3600,
   "payee": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
   "msatoshi": 10000000,
   "amount_msat": "10000000msat",
   "description": "First LN Payment - Learning Bitcoin and Lightning from the Command line.",
   "min_final_cltv_expiry": 40,
   "payment_secret": "e946403bcb54764994306c743ed3bf1303af8ae6e7687d3cabcc3ee06dbf904d",
   "features": "028200",
   "payment_hash": "6cacdedc95b89eec15e5244bd0957b88c0ab58b153eee549735b995344bc16bb",
   "signature": "304402202e73ebd1ef974594eb117e8301be781f2ba289041ab6abc558d432351d8365e902202a8151c3fd13419b9390c2b976503d2d064f2a6748b14cb0db64424cf4e572f4"
}

```
Aqui temos o que os elementos mais relevantes significam:

1. `currency`: A moeda a ser paga;
2. `created_at`: O momento em que a fatura foi criada. O valor é dado em tempo UNIX, que é segundos desde 1970.
3. `expiry`: O momento em que nosso node marca a fatura como inválida. O padrão é 1 hora ou 3600 segundos.
4. `payee`: A chave pública da pessoa (node) que recebe o pagamento da Lightning Network;
5. `msatoshi` e `amount_msat`: O valor de satoshis a ser pago;
6. `description`: A descrição informada pelo usuário;
7. `payment_hash`: O hash da pré-imagem que é usado para bloquear o pagamento. Só podemos resgatar um pagamento bloqueado com a pré-imagem correspondente ao hash de pagamento. Isso permite o roteamento na Lightning Network sem confiar em terceiros, criando um **Pagamento Condicional** a ser preenchido;
8. `signature`: A assinatura codificada por DER.

> :book: ***O que são pagamentos condicionais?*** Embora os canais Lightning sejam criados entre dois participantes, vários canais podem ser conectados juntos, formando uma rede de pagamento que permite envio de valores entre todos os participantes da rede, mesmo aqueles sem um canal direto entre eles. Isso é feito usando um contrato inteligente denominado **Hashed Time Locked Contract**.

> :book: ***O que é um Hashed Time Locked Contract (HTLC)?*** Um HTLC é um pagamento condicional que usa hashlocks e timelocks para garantir a segurança do pagamento. O destinatário deve apresentar uma pré-imagem do pagamento ou gerar um comprovante criptográfico de pagamento antes de um determinado prazo, caso contrário o pagador pode cancelar o contrato gastando-o. Esses contratos são criados como saídas da **Transação de Compromisso**.

> :book: ***O que é uma transação de compromisso?*** Uma transação de compromisso é uma transação que gasta a transação de financiamento original. Cada par possui a assinatura do outro par, o que significa que qualquer um pode gastar sua transação do compromisso como quiser. Depois que cada nova transação de confirmação é criada, a antiga é revogada. A transação de confirmação é uma maneira pela qual a transação de financiamento pode ser desbloqueada na blockchain, conforme discutiremos na seção [§20.3](20_3_Closing_a_Channel.md).

### Verificando Nosso Invoice

Existem dois elementos cruciais para verificar o invoice. O primeiro, obviamente, é o valor do pagamento, que já examinamos na parte legível. O segundo é o dado do `payee`, que é o pubkey do destinatário (node):
```
   "payee": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
```
Precisamos verificar se ele é o destinatário esperado.

Olhando nas seções anteriores, mais precisamente na seção [§19.3](19_3_Setting_Up_a_Channel.md#opening-a-channel), podemos observar que é de fato o ID do par que usamos quando criamos nosso canal. Também podemos verificá-lo no outro node com o comando `getinfo`.
```
lnd$ lncli -n testnet getinfo
{
    "version": "0.11.0-beta.rc4 commit=v0.11.0-beta.rc4",
    "commit_hash": "fc12656a1a62e5d69430bba6e4feb8cfbaf21542",
    "identity_pubkey": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
    "alias": "StandUp",
    "color": "#3399ff",
    "num_pending_channels": 0,
    "num_active_channels": 1,
    "num_inactive_channels": 0,
    "num_peers": 3,
    "block_height": 1862983,
    "block_hash": "00000000000000c8c2f58f6da2ae2a3884d6e84f55d0e1f585a366f9dfcaa860",
    "best_header_timestamp": "1602702331",
    "synced_to_chain": true,
    "synced_to_graph": true,
    "testnet": true,
    "chains": [
        {
            "chain": "bitcoin",
            "network": "testnet"
        }
    ],
    "uris": [
    ],
    "features": {
        "0": {
            "name": "data-loss-protect",
            "is_required": true,
            "is_known": true
        },
        "5": {
            "name": "upfront-shutdown-script",
            "is_required": false,
            "is_known": true
        },
        "7": {
            "name": "gossip-queries",
            "is_required": false,
            "is_known": true
        },
        "9": {
            "name": "tlv-onion",
            "is_required": false,
            "is_known": true
        },
        "13": {
            "name": "static-remote-key",
            "is_required": false,
            "is_known": true
        },
        "15": {
            "name": "payment-addr",
            "is_required": false,
            "is_known": true
        },
        "17": {
            "name": "multi-path-payments",
            "is_required": false,
            "is_known": true
        }
    }
}
```
No entanto, o `payee` também pode ser alguém novo, caso no qual provavelmente precisaremos verificar com a pessoa que emitiu o invoice para garantir que tudo está correto.

## Resumo: Gerando um Invoice

Na maioria dos casos, precisamos receber um invoice para usar os pagamentos da Lightning Network. Neste exemplo, criamos um manualmente, mas se estivermos em um ambiente de produção, provavelmente teriam sistemas fazendo isso automaticamente sempre que alguém adquirir produtos ou serviços. Claro, depois de receber um invoice, precisamos saber como lê-lo!

## O Que Vem Depois?

Vamos continuar "Usando a Lightning" na seção [§20.2: Pagando um Invoice](20_2_Paying_a_Invoice.md).