# 20.2: Pagando um Invoice

> :information_source: **NOTA:** Esta seção foi adicionada recentemente ao curso e é um rascunho inicial que ainda pode estar aguardando revisão.

Neste capítulo, aprenderemos como pagar um invoice usando o comando `lightning-cli pay`. Presume-se que já sabemos como analisar um invoice, de acordo com a seção [§20.1](20_1_Generate_a_Payment_Request.md), e se ele é válido.

## Verificando o Saldo

Obviamente, a primeira coisa que precisamos fazer é nos certificarmos de que possuímos fundos suficientes para pagar o invoice. Neste caso, o canal configurado anteriormente com `032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543` contém 100.000 satoshis. Este será o canal de pagamento do invoice.

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
         "status": "confirmed",
         "blockheight": 1862856,
         "reserved": false
      }
   ],
   "channels": [
      {
         "peer_id": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
         "connected": true,
         "state": "CHANNELD_NORMAL",
         "short_channel_id": "1862856x29x0",
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
Se não tivermos fundos suficientes, precisamos criar um novo canal.

## Pagando Nosso Invoice

Vamos usar o comando `lightning-cli pay` para pagar o invoice. Ele tentará encontrar uma rota para o destino fornecido, para posteriormente enviar os fundos solicitados. Isso é muito simples porque há um canal direto entre o pagador e o destinatário:
```
c$ lightning-cli --testnet pay lntb100u1p0cwnqtpp5djkdahy4hz0wc909y39ap9tm3rq2kk9320hw2jtntwv4x39uz6asdr5ge5hyum5ypxyugzsv9uk6etwwssz6gzvv4shymnfdenjqsnfw33k76twypskuepqf35kw6r5de5kueeqveex7mfqw35x2gzrdakk6ctwvssxc6twv5hqcqzpgsp5a9ryqw7t23myn9psd36ra5alzvp6lzhxua58609teslwqmdljpxs9qy9qsq9ee7h500jazef6c306psr0ncru469zgyr2m2h32c6ser28vrvh5j4q23c073xsvmjwgv9wtk2q7j6pj09fn53v2vkrdkgsjv7njh9aqqtjn3vd
{
   "destination": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
   "payment_hash": "6cacdedc95b89eec15e5244bd0957b88c0ab58b153eee549735b995344bc16bb",
   "created_at": 1602704828.948,
   "parts": 1,
   "msatoshi": 10000000,
   "amount_msat": "10000000msat",
   "msatoshi_sent": 10000000,
   "amount_sent_msat": "10000000msat",
   "payment_preimage": "1af4a9bb830e49b6bc8f0bef980630e189e3794ad1705f06ad1b9c71571dce0c",
   "status": "complete"
}
```
Vamos observar que todos os valores estão em `msats`, não em `sats`!

### Pagando o Invoice pela Lightning

No entanto, _não_ precisamos ter um canal com um node para pagá-lo. Só precisamos ter uma rota razoável pela Lightning Network.

Imagine que recebemos esta minúscula solicitação de pagamento de 11.111 msat:
```
c$ lightning-cli --testnet decodepay lntb111110p1p0cw43ppp5u0ngjytlw6ywec3x784jale4xd7h058g9u4mthcaf9rl2f7g8zxsdp2t9hh2gr0wajjqmt9ypnx7u3qv35kumn9wgs8gmm0yyxqyjw5qcqp2sp5kj4xhrthmfgcgyl84zaqpl9vvdjwm5x368kr09fu5nym74setw4s9qy9qsq8hxjr73ee77vat0ay603e4w9aa8ag9sa2n55xznk5lsfrjffxxdj2k0wznvcfa98l4a57s80j7dhg0cc03vwqdwehkujlzxgm0xyynqqslwhvl
{
   "currency": "tb",
   "created_at": 1602704929,
   "expiry": 604800,
   "payee": "02f3d74746934494fa378235e5bc44cfdbb5b8779d839263fb7f9218be032f6f61",
   "msatoshi": 11111,
   "amount_msat": "11111msat",
   "description": "You owe me for dinner too!",
   "min_final_cltv_expiry": 10,
   "payment_secret": "b4aa6b8d77da518413e7a8ba00fcac6364edd0d1d1ec37953ca4c9bf56195bab",
   "features": "028200",
   "payment_hash": "e3e689117f7688ece226f1eb2eff35337d77d0e82f2bb5df1d4947f527c8388d",
   "signature": "304402203dcd21fa39cfbcceadfd269f1cd5c5ef4fd4161d54e9430a76a7e091c929319b02202559ee14d984f4a7fd7b4f40ef979b743f187c58e035d9bdb92f88c8dbcc424c"
}
```
Se tentássemos pagar e não tivéssemos uma rota para o destinatário por meio da Lightning Network, poderíamos esperar um erro como este:
```
c$ lightning-cli --testnet pay lntb111110p1p0cw43ppp5u0ngjytlw6ywec3x784jale4xd7h058g9u4mthcaf9rl2f7g8zxsdp2t9hh2gr0wajjqmt9ypnx7u3qv35kumn9wgs8gmm0yyxqyjw5qcqp2sp5kj4xhrthmfgcgyl84zaqpl9vvdjwm5x368kr09fu5nym74setw4s9qy9qsq8hxjr73ee77vat0ay603e4w9aa8ag9sa2n55xznk5lsfrjffxxdj2k0wznvcfa98l4a57s80j7dhg0cc03vwqdwehkujlzxgm0xyynqqslwhvl
{
   "code": 210,
   "message": "Ran out of routes to try after 11 attempts: see `paystatus`",
   "attempts": [
      {
         "status": "failed",
         "failreason": "Error computing a route to 02f3d74746934494fa378235e5bc44cfdbb5b8779d839263fb7f9218be032f6f61: \"Could not find a route\" (205)",
         "partid": 1,
         "amount": "11111msat"
      },
...
```
Mas e se um host com o qual tínhamos um canal aberto é o destinatário pretendido?

Nesse caso, quando formos pagar a fatura, ele _automaticamente funcionará_!
```
c$ lightning-cli --testnet pay lntb111110p1p0cw43ppp5u0ngjytlw6ywec3x784jale4xd7h058g9u4mthcaf9rl2f7g8zxsdp2t9hh2gr0wajjqmt9ypnx7u3qv35kumn9wgs8gmm0yyxqyjw5qcqp2sp5kj4xhrthmfgcgyl84zaqpl9vvdjwm5x368kr09fu5nym74setw4s9qy9qsq8hxjr73ee77vat0ay603e4w9aa8ag9sa2n55xznk5lsfrjffxxdj2k0wznvcfa98l4a57s80j7dhg0cc03vwqdwehkujlzxgm0xyynqqslwhvl
{
   "destination": "02f3d74746934494fa378235e5bc44cfdbb5b8779d839263fb7f9218be032f6f61",
   "payment_hash": "e3e689117f7688ece226f1eb2eff35337d77d0e82f2bb5df1d4947f527c8388d",
   "created_at": 1602709081.324,
   "parts": 1,
   "msatoshi": 11111,
   "amount_msat": "11111msat",
   "msatoshi_sent": 12111,
   "amount_sent_msat": "12111msat",
   "payment_preimage": "ec7d1b28a7b877cd92b83be396899e8bfc3ecb0b4f944f65afb4be7d0ee72617",
   "status": "complete"
}
```
Essa é a verdadeira beleza da Lightning Network: sem nenhum esforço dos participantes ponto a ponto, nossos canais individuais se tornam uma rede!

> :book: ***Como funcionam os pagamentos na rede?*** Digamos que o node A tem um canal aberto com o node B, o node B tem um canal aberto com o node C e o node A recebe uma fatura do node C de 11.111 msat. O node A paga ao node B 11.111 msat, mais uma pequena taxa, e então o node B paga 11.111 msat ao node C. Muito fácil. Mas lembre-se de que todos os canais são, na verdade, apenas registros de quem é o proprietário e de quanto é a Transação de Financiamento. Então o que realmente acontece é 11.111 msat da Transação de Financiamento no canal AB muda de A para B e, em seguida, 11.111 msat da Transação de Financiamento no canal BC muda de B para C. Isso significa que duas coisas são necessárias para que este pagamento funcione: primeiro, cada canal deve ter capacidade suficiente para o pagamento; e segundo, o pagador em cada canal deve possuir o suficiente da capacidade para fazer o pagamento.

Observe que, neste exemplo, 12.111 msat foram enviados para pagar uma fatura de 11.111 msat: o extra sendo uma taxa fixa muito pequena (não uma porcentagem) que foi paga ao intermediário.

## Verificando Nosso Saldo

Após efetuar um pagamento com sucesso, veremos que nossos fundos foram alterados corretamente.

Aqui está a aparência dos fundos para o node pagador após o pagamento inicial de 10.000 satoshis:
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
         "status": "confirmed",
         "blockheight": 1862856,
         "reserved": false
      }
   ],
   "channels": [
      {
         "peer_id": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
         "connected": true,
         "state": "CHANNELD_NORMAL",
         "short_channel_id": "1862856x29x0",
         "channel_sat": 90000,
         "our_amount_msat": "90000000msat",
         "channel_total_sat": 100000,
         "amount_msat": "100000000msat",
         "funding_txid": "66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d",
         "funding_output": 0
      }
   ]
}
```
Observe que a capacidade do canal permanece em 100.000 satoshis (isso nunca irá mudar!), mas que o `our_amount` agora é de apenas 90.000 satoshis (ou 90.000.000 msat).

Depois de pagar a segunda fatura, de 11.111 msat, os fundos mudam novamente:
```
$ lightning-cli --testnet listfunds
{
   "outputs": [
      {
         "txid": "66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d",
         "output": 1,
         "value": 99847,
         "amount_msat": "99847000msat",
         "scriptpubkey": "00142fe02e5be9283e8c5bcb93ae61421baf8cb64f9c",
         "address": "tb1q9lszuklf9qlgck7tjwhxzssm47xtvnuu4jslf8",
         "status": "confirmed",
         "blockheight": 1862856,
         "reserved": false
      }
   ],
   "channels": [
      {
         "peer_id": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
         "connected": true,
         "state": "CHANNELD_NORMAL",
         "short_channel_id": "1862856x29x0",
         "channel_sat": 89987,
         "our_amount_msat": "89987000msat",
         "channel_total_sat": 100000,
         "amount_msat": "100000000msat",
         "funding_txid": "66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d",
         "funding_output": 0
      }
   ]
}
```
O `our_amount` agora é de apenas 89.987 satoshis, tendo pago 11.111 msat mais uma taxa de 1.000 msat.

## Resumo: Pagando um Invoice

Depois de recebermos um invoice, é fácil pagar com um único comando na Lightning. Mesmo se não tivermos um canal para o destinatário, o pagamento é simples, desde que haja uma rota entre nós e o node de destino.

## O Que Vem Depois?

Vamos continuar "Usando a Lightning" na seção [§20.3: Fechando um Canal Lightning](20_3_Closing_a_Channel.md).