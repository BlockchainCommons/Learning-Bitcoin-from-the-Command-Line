# 4.5: Enviando Monedas con Transacciones Crudas Automatizadas

Este capítulo expone tres formas de enviar fondos a través de la interfaz cli de Bitcoin. [§4.1](04_1_Enviando_Monedas_de_la_Forma_Facil.md) describe cómo hacerlo con un simple comando, y [§4.4](04_4_Enviando_Monedas_con_una_Transaccion_Cruda.md) detalla cómo usar una transacción en crudo más peligrosa. Esta sección final divide la diferencia mostrando cómo hacer que las transacciones crudas sean más simples y seguras. 

## Deje que Bitcoin calcule por usted

La metodología para automatizar las transacciones en crudo es sencilla: usted crea una transacción en crudo, pero utiliza el comando `fundrawtransaction` para pedir a bitcoind que realice los cálculos por usted.

Para utilizar este comando, tendrá que asegurarse de que tu archivo ~/.bitcoin/bitcoin.conf contiene variables racionales para calcular las tasas de las transacciones. Por favor, consulta [§4.1: Enviando Monedas de la Forma Fácil](04_1_Enviando_Monedas_de_la_Forma_Facil.md) para más información al respecto.

Para números muy conservadores, sugerimos añadir lo siguiente al `bitcoin.conf`:
```
mintxfee=0.0001
txconfirmtarget=6
```
Para que el tutorial siga avanzando (y, en general, para que el dinero se mueva con rapidez) sugerimos lo siguiente:
```
mintxfee=0.001
txconfirmtarget=1
```

## Crear una transacción básica en crudo

Para utilizar `fundrawtransaction`, primero debe crear una transacción básica que no contenga ninguna entrada ni ninguna dirección de cambio. Sólo debe indicar el destinatario y la cantidad que desea enviarle, en este caso "destinatario" y "0.0002" BTC.
```
$ recipient=n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi
$ unfinishedtx=$(bitcoin-cli -named createrawtransaction inputs='''[]''' outputs='''{ "'$recipient'": 0.0002 }''')
```

## Financiar su transacción básica

A continuación, se le indica a `bitcoin-cli` que financie esa transacción básica:
```
$ bitcoin-cli -named fundrawtransaction hexstring=$unfinishedtx
{
  "hex": "02000000012db87641c6a21e5a68b20c226428544978e6ac44964d5d8060d7388000c584eb0100000000feffffff02204e0000000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac781e0000000000001600140cc9cdcf45d4ea17f5227a7ead52367aad10a88400000000",
  "fee": 0.00022200,
  "changepos": 1
}
```
Esto proporciona mucha información útil, pero una vez que esté seguro de cómo funciona, querrá usar JQ para guardar su hex en una variable, como de costumbre:
```
$ rawtxhex3=$(bitcoin-cli -named fundrawtransaction hexstring=$unfinishedtx | jq -r '.hex')
```
## Verifique la transacción financiada

Parece magia, así que las primeras veces que use `fundrawtransaction`, probablemente querrá verificarla.

Ejecutar `decoderawtransaction` mostrará que la transacción en crudo está ahora dispuesta correctamente, utilizando uno o más de sus UTXOs y enviando los fondos sobrantes a una dirección de cambio:
```
$ bitcoin-cli -named decoderawtransaction hexstring=$rawtxhex3
{
  "txid": "b3b4c2057dbfbef6690e975ede92fde805ddea13c730f58401939a52c9ac1b99",
  "hash": "b3b4c2057dbfbef6690e975ede92fde805ddea13c730f58401939a52c9ac1b99",
  "version": 2,
  "size": 116,
  "vsize": 116,
  "weight": 464,
  "locktime": 0,
  "vin": [
    {
      "txid": "eb84c5008038d760805d4d9644ace67849542864220cb2685a1ea2c64176b82d",
      "vout": 1,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967294
    }
  ],
  "vout": [
    {
      "value": 0.00020000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 e7c1345fc8f87c68170b3aa798a956c2fe6a9eff OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi"
        ]
      }
    },
    {
      "value": 0.00007800,
      "n": 1,
      "scriptPubKey": {
        "asm": "0 a782f4c6e1e75a5b24f3d675d6f11b5ebf3b2142",
        "hex": "0014a782f4c6e1e75a5b24f3d675d6f11b5ebf3b2142",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1q57p0f3hpuad9kf8n6e6adugmt6lnkg2zzr592r"
        ]
      }
    }
  ]
}
```
Una cosa de interés aquí es la dirección de cambio, que es la segunda `vout`. Observe que es una dirección `tb1`, lo que significa que es Bech32; cuando le dimos a Bitcoin Core la capacidad total de gestionar nuestro cambio, lo hizo usando su tipo de dirección por defecto, Bech32, y funcionó bien. Por eso nuestro cambio a direcciones SegWit en [§4.6](04_6_Creando_una_Transaccion_Segwit.md) realmente no es gran cosa, pero hay algunos inconvenientes para un uso más amplio, de los que hablaremos allí.

Aunque vimos la tasa en la salida de `fundrawtransaction`, no es visible aquí. Sin embargo, puede verificarlo con el script JQ `txfee-calc.sh` creado en el [Interludio: Usando JQ](04_2_Interludio_Usando_JQ.md):
```
$ ~/txfee-calc.sh $rawtxhex3
.000222
```
Por último, puede utilizar `getaddressinfo` para comprobar que la dirección de cambio generada le pertenece realmente:
```
$ bitcoin-cli -named getaddressinfo address=tb1q57p0f3hpuad9kf8n6e6adugmt6lnkg2zzr592r
{
  "address": "tb1q57p0f3hpuad9kf8n6e6adugmt6lnkg2zzr592r",
  "scriptPubKey": "0014a782f4c6e1e75a5b24f3d675d6f11b5ebf3b2142",
  "ismine": true,
  "solvable": true,
  "desc": "wpkh([d6043800/0'/1'/10']038a2702938e548eaec28feb92c7e4722042cfd1ea16bec9fc274640dc5be05ec5)#zpv26nar",
  "iswatchonly": false,
  "isscript": false,
  "iswitness": true,
  "witness_version": 0,
  "witness_program": "a782f4c6e1e75a5b24f3d675d6f11b5ebf3b2142",
  "pubkey": "038a2702938e548eaec28feb92c7e4722042cfd1ea16bec9fc274640dc5be05ec5",
  "ischange": true,
  "timestamp": 1592335137,
  "hdkeypath": "m/0'/1'/10'",
  "hdseedid": "fdea8e2630f00d29a9d6ff2af7bf5b358d061078",
  "hdmasterfingerprint": "d6043800",
  "labels": [
  ]
}
```
Observe los resultados del `ismine`.

## Envíe la transacción financiada

En este punto puede firmar y enviar la transacción como de costumbre.
```
$ signedtx3=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex3 | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx3
8b9dd66c999966462a3d88d6ac9405d09e2aa409c0aa830bdd08dbcbd34a36fa
```
En varios minutos, tendrá su cambio de vuelta:
```
$ bitcoin-cli listunspent
[
  {
    "txid": "8b9dd66c999966462a3d88d6ac9405d09e2aa409c0aa830bdd08dbcbd34a36fa",
    "vout": 1,
    "address": "tb1q57p0f3hpuad9kf8n6e6adugmt6lnkg2zzr592r",
    "scriptPubKey": "0014a782f4c6e1e75a5b24f3d675d6f11b5ebf3b2142",
    "amount": 0.00007800,
    "confirmations": 1,
    "spendable": true,
    "solvable": true,
    "desc": "wpkh([d6043800/0'/1'/10']038a2702938e548eaec28feb92c7e4722042cfd1ea16bec9fc274640dc5be05ec5)#zpv26nar",
    "safe": true
  }
]
```

## Resumen: Envío de monedas con transacciones crudas automatizadas

Si tiene que enviar fondos con transacciones crudas, entonces `fundrawtransaction` le da una buena alternativa donde las comisiones, entradas y salidas se calculan por usted, para que no pierda accidentalmente un montón de dinero.

> :fire: ___¿Cuál es el poder de enviar monedas con transacciones crudas automatizadas?___

> _Las ventajas._ Proporciona un buen equilibrio. Si está enviando fondos a mano y `sendtoaddress` no ofrece suficiente control por cualquier razón, puede obtener algunas de las ventajas de las transacciones en crudo sin los peligros asociados. Esta metodología debería usarse siempre que sea posible si está enviando transacciones en crudo a mano.

> _Las desventajas._ Es una mezcolanza. Aunque hay algunas opciones adicionales para el comando `fundrawtransaction` que no se han mencionado aquí, el control sigue siendo limitado. Probablemente nunca querrá usar este método si estuviera escribiendo un programa donde el objetivo es saber exactamente lo que está pasando.

## ¿Qué sigue?

Complemente "Enviando transacciones de Bitcoin" con [§4.6: Creando una Transacción Segwit](04_6_Creando_una_Transaccion_Segwit.md).
