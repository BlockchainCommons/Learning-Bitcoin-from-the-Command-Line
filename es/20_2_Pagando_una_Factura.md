# 20.2: Pagando una Factura

> :information_source: **NOTA:** Esta sección se ha agregado recientemente al curso y es un borrador inicial que aún puede estar pendiente de revisión. Lector de advertencias.

En este capitulo aprenderá cómo pagar una factura usando el comando `lightning-cli pay`. Se asume que ya revisó la factura, según [§20.1](20_1_Generando_una_Solicitud_de_Pago.md) y determinó que era válida.

## Consulta su Saldo

Obviamente, lo primero que debe hacer es asegurarse de tener fondos suficientes para pagar una factura. En este caso, el canal configurado previamente con `032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543` contiene 100,000 satoshis. Este será el canal utilizado para pagar la factura.

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
Si no tiene fondos suficientes, debe crear un canal nuevo.

## Paga su Factura

Utiliza el comando `lightning-cli pay` para pagar una factura. Intentará encontrar una ruta al destino dado y enviar los fondos solicitados. Aquí eso es muy simple porque hay un canal directo entre el pagador y el destinatario:
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
¡Tenga en cuenta que aquí todas las cantidades están en `msats`, no en `sats`!

### Pagar su Factura en Toda la Red

Sin embargo, _no_ necesita tener un canal con un nodo para poder pagarles. Solo debe haber una ruta razonable a través de Lightning Network.

Imagine que recibió esta pequeña solicitud de pago por 11,111 msat:
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
Si intentó pagarlo y no tenía una ruta hacia el destinatario a través de Lightning Network, podría esperar un error como este:
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
Pero, ¿qué pasa si un anfitrión con el que tiene un canal abre un canal con el destinatario previsto?

En ese caso, cuando vaya a pagar la factura, _funcionará automáticamente_!
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
Esa es la verdadera belleza de Lightning Network allí: sin esfuerzo de los participantes igual a igual, sus canales individuales se convierten en una red!

> :book: ***¿Cómo Funcionan los Pagos en la Red?*** Digamos que el nodo A tiene un canal abierto con el nodo B, el nodo B tiene un canal abierto con el nodo C y el nodo A recibe una factura del nodo C por 11,111 msat. El nodo A paga al nodo B los 11,111 msat, más una pequeña tarifa, y luego el nodo B paga los 11,111 msat al nodo C. Suficientemente fácil. Excepto que recuerde que todos los canales en realidad son solo registros de quién es el propietario de la cantidad de la Transacción de pago. Entonces, lo que realmente sucede es 11,111 msat de la Transacción de pago en el canal A-B cambia de A a B, y luego 11,111 msat de la Transacción de pago en el canal B-C cambia de B a C. Esto significa que se requieren dos cosas para que este pago funcione: primero, cada canal debe tener suficiente capacidad para el pago; y segundo, el pagador en cada canal debe poseer suficiente capacidad para realizar el pago.  

Tenga en cuenta que en este ejemplo, se enviaron 12,111 msat para pagar una factura de 11,111 msat: el extra es una tarifa plana muy pequeña (no un porcentaje) que se pagó al intermediario.

## Consulta su Saldo

Después de haber realizado un pago con éxito, debería ver que sus fondos han cambiado en consecuencia.

Así es como se veían los fondos para el nodo de pago después del pago inicial de 10,000 satoshis:
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
Tenga en cuenta que la capacidad del canal permanece en 100,000 satoshis (¡nunca cambia!), pero que `our_amount` (nuestro cantidad) ahora es de solo 90,000 satoshis (o 90,000,000 msat).

Después de pagar la segunda factura, por 11,111 msat, los fondos cambian nuevamente en consecuencia:
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
`our_amount` es ahora solo 89,987 satoshis, habiendo pagado 11,111 msat más una tarifa de 1,000 msat.

## Resumen: Pagando de una Factura

Una vez que tenga una factura, es bastante fácil pagar con un solo comando en Lightning. Incluso si no tiene un canal para un destinatario, el pago es así de simple, siempre que haya una ruta entre usted y el nodo de destino.

## ¿Que Sigue?

Continúe "Usando Lighting" con [§20.3: Cerrando un Canal Lightning](20_3_Cerrando_un_Canal_Lightning.md).
