# 6.2: Gastando una Transacción con una Multifirma

La clásica, y compleja, forma de gastar fondos enviados a una dirección multifirma usando `bitcoin-cli` requiere mucho trabajo.

## Encontrar Sus Fondos

Para empezar, necesita encontrar sus fondos; su computadora no sabe buscarlos, porque no están asociados con ninguna dirección en su billetera. Puede alertar a `bitcoind` para hacerlo usando el comando `importaddress`:
```
$ bitcoin-cli -named importaddress address=2NAGfA4nW6nrZkD5je8tSiAcYB9xL2xYMCz
```
Si tiene un nodo podado (y probablemente lo tenga), tendrá que decirle que no vuelva a escanear:
```
$ bitcoin-cli -named importaddress address=2NAGfA4nW6nrZkD5je8tSiAcYB9xL2xYMCz rescan="false"
```
Si prefiere, puede importar la dirección usando su descriptor (y esta es generalmente la mejor, más estandarizada respuesta):
```
$ bitcoin-cli importmulti '[{"desc": "sh(multi(2,02da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d191,02bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa3))#0pazcr4y", "timestamp": "now", "watchonly": true}]'
[
  {
    "success": true
  }
]
```
Posteriormente, los fondos deberían aparecer cuando haga `listunspent` ... pero aún así no son fáciles de gastar. (De hecho, su billetera puede afirmar que no se pueden gastar `spendable` en absoluto!)

Si por alguna razón no le es posible incorporar la dirección a su billetera, puede usar `gettransaction` para obtener la info (o mirar en un explorador de bloques).
```
$ bitcoin-cli -named gettransaction txid=b164388854f9701051809eed166d9f6cedba92327e4296bf8a265a5da94f6521 verbose=true
{
  "amount": -0.00006500,
  "fee": -0.00001000,
  "confirmations": 3,
  "blockhash": "0000000000000165b5f602920088a7e36b11214161d6aaebf5229e3ed4f10adc",
  "blockheight": 1773282,
  "blockindex": 9,
  "blocktime": 1592959320,
  "txid": "b164388854f9701051809eed166d9f6cedba92327e4296bf8a265a5da94f6521",
  "walletconflicts": [
  ],
  "time": 1592958753,
  "timereceived": 1592958753,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr",
      "category": "send",
      "amount": -0.00006500,
      "vout": 0,
      "fee": -0.00001000,
      "abandoned": false
    }
  ],
  "hex": "020000000001011b95a6055174ec64b82ef05b6aefc38f34d0e57197e40281ecd8287b4260dec60000000000ffffffff01641900000000000017a914a5d106eb8ee51b23cf60d8bd98bc285695f233f38702473044022070275f81ac4129e1d167ef7e700739f2899ea4c7f1adef3a4da29436f14fb97e02207310d4ec449eba49f0fa404ae45b9c82431d883490c7a0ed882ad0b5d7a623d0012102883bb5463e37d55252d8b3d5c2141b007b37c8a7db6211f75c955acc5ea325eb00000000",
  "decoded": {
    "txid": "b164388854f9701051809eed166d9f6cedba92327e4296bf8a265a5da94f6521",
    "hash": "bdf4e3bc5d354a5dfa5528f172480976321d989d7e5806ac14f1fe9b0b1c093a",
    "version": 2,
    "size": 192,
    "vsize": 111,
    "weight": 441,
    "locktime": 0,
    "vin": [
      {
        "txid": "c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b",
        "vout": 0,
        "scriptSig": {
          "asm": "",
          "hex": ""
        },
        "txinwitness": [
          "3044022070275f81ac4129e1d167ef7e700739f2899ea4c7f1adef3a4da29436f14fb97e02207310d4ec449eba49f0fa404ae45b9c82431d883490c7a0ed882ad0b5d7a623d001",
          "02883bb5463e37d55252d8b3d5c2141b007b37c8a7db6211f75c955acc5ea325eb"
        ],
        "sequence": 4294967295
      }
    ],
    "vout": [
      {
        "value": 0.00006500,
        "n": 0,
        "scriptPubKey": {
          "asm": "OP_HASH160 a5d106eb8ee51b23cf60d8bd98bc285695f233f3 OP_EQUAL",
          "hex": "a914a5d106eb8ee51b23cf60d8bd98bc285695f233f387",
          "reqSigs": 1,
          "type": "scripthash",
          "addresses": [
            "2N8MytPW2ih27LctLjn6LfLFZZb1PFSsqBr"
          ]
        }
      }
    ]
  }
}
```

## Configurar Sus Variables

Cuando esté listo para gastar los fondos recibidos por una dirección de multifirma, necesitará recopilar _muchos_ datos: mucho más de lo que necesita cuando gasta un P2PKH normal o un UTXO SegWit. Esto se debe en parte a que la información de la dirección multifirma no está en su billetera, y en parte a que está gastando dinero que se envió a una dirección P2SH (pago a script-hash), y eso es mucho más exigente.

En total, necesitará recopilar tres cosas: información ampliada sobre el UTXO; el redeemScript; y todas las claves privadas involucradas. Por supuesto, también necesitará una nueva dirección de destinatario. Las claves privadas deben esperar el paso de firma, pero todo lo demás se puede hacer ahora.

### Acceder a la información del UTXO

Para empezar, tome la `txid` y la `vout` para la transacción que desea gastar, como es habitual. En este caso, se recuperó de la información de `gettransaction`, arriba:
```
$ utxo_txid=b164388854f9701051809eed166d9f6cedba92327e4296bf8a265a5da94f6521
$ utxo_vout=0
```
Sin embargo, necesita acceder también a una tercera parte de la información acerca del UTXO, es `scriptPubKey`/`hex`, que es el script que bloqueó la transacción. Nuevamente, probablemente está haciendo esto mirando los detalles de la transacción:
```
$ utxo_spk=a914a5d106eb8ee51b23cf60d8bd98bc285695f233f387
```

### Registrar el Script de Redención

Con suerte, ha guardado el `redeemScript`. Ahora debería registrarlo en una variable.

Esto se extrajo de nuestra creación de la dirección en la sección anterior.
```
redeem_script="522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae"
```
### Decidir Su Destinatario

Nos enviaremos el dinero a nosotros mismos. Esto es útil porque libera los fondos de la multifirma, convirtiéndolos en una transacción P2PKH normal que luego puede ser confirmada por una sola clave privada:
```
$ recipient=$(bitcoin-cli getrawchangeaddress)
```
## Crear Su Transacción

Ahora puede crear su transacción. Esto no es diferente de lo habitual.
```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.00005}''')
$ echo $rawtxhex
020000000121654fa95d5a268abf96427e3292baed6c9f6d16ed9e80511070f954883864b10000000000ffffffff0188130000000000001600142c48d3401f6abed74f52df3f795c644b4398844600000000
```

## Firmar Su Transacción

Ahora está listo para firma su transacción. Este es un proceso de varios pasos porque deberá hacerlo en varias máquinas, cada una de las cuales contribuirá con sus propias claves privadas.

### Descargar Su Primera Clave Privada

Debido a que esta transacción no está haciendo un uso completo de su billetera, necesitará acceder directamente a sus claves privadas. Comience en `machine1`, donde debe recuperar cualquiera de las claves privadas de ese usuario que estuvieron involucradas en la multifirma:
```
machine1$ bitcoin-cli -named dumpprivkey address=$address1
cNPhhGjatADfhLD5gLfrR2JZKDE99Mn26NCbERsvnr24B3PcSbtR
```
> :warning: **ATENCIÓN:** Acceder directamente a sus claves privadas desde el shell es un comportamiento muy peligroso y debe hacerse con sumo cuidado si está usando dinero real. Al menos, no guarde la información en una variable a la que se pueda acceder desde su máquina. Eliminar el historial de su shell es otro gran paso. A lo sumo, no lo haga.

### Hacer Su Primer Firma

Ahora puede hacer su primer firma con el comando `signrawtransactionwithkey`. Aquí es donde las cosas son diferentes: necesitará instruir al comando sobre cómo firmar. Puede hacer esto agregando la siguiente información nueva:

* Incluya un argumento `prevtxs` que incluya la `txid`, la `vout`, la `scriptPubKey`, y la `redeemScript` que registró, cada una de ellas un par key-value en el objeto JSON. 
* Incluya un argumento `privkeys` que liste las claves privadas que descargó en esta máquina.

```
machine1$ bitcoin-cli -named signrawtransactionwithkey hexstring=$rawtxhex prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cNPhhGjatADfhLD5gLfrR2JZKDE99Mn26NCbERsvnr24B3PcSbtR"]'
{
  "hex": "020000000121654fa95d5a268abf96427e3292baed6c9f6d16ed9e80511070f954883864b100000000920047304402201c97b48215f261055e41b765ab025e77a849b349698ed742b305f2c845c69b3f022013a5142ef61db1ff425fbdcdeb3ea370aaff5265eee0956cff9aa97ad9a357e3010047522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352aeffffffff0188130000000000001600142c48d3401f6abed74f52df3f795c644b4398844600000000",
  "complete": false,
  "errors": [
    {
      "txid": "b164388854f9701051809eed166d9f6cedba92327e4296bf8a265a5da94f6521",
      "vout": 0,
      "witness": [
      ],
      "scriptSig": "0047304402201c97b48215f261055e41b765ab025e77a849b349698ed742b305f2c845c69b3f022013a5142ef61db1ff425fbdcdeb3ea370aaff5265eee0956cff9aa97ad9a357e3010047522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352ae",
      "sequence": 4294967295,
      "error": "CHECK(MULTI)SIG failing with non-zero signature (possibly need more signatures)"
    }
  ]
}

```
Eso produce errores aterradores y dice que está `fallando`. Esto está bien. Puede ver que la firma se ha realizado parcialmente correcta porque el `hex` se ha alargado. Aunque la transacción se ha firmado parcialmente, no se realiza porque necesita más firmas. 

### Repetir para Otros Firmantes

Ahora puede transferir la transacción, para que la vuelva a firmar cualquier otra persona requerida para la multifirma. Hacen esto corriendo el mismo comando de firma que hizo pero: (1) con el `hex` más largo que sacó de (`bitcoin-cli -named signrawtransactionwithkey hexstring=$rawtxhex prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cMgb3KM8hPATCtgMKarKMiFesLft6eEw3DY6BB8d97fkeXeqQagw"]' | jq -r '.hex'`); y (2) con su propia clave privada.

> :information_source: **NOTA — M-DE-N VS N-DE-N:** Obviamente, si tiene una firma n-de-n (como la multifirma 2-de-2 en este ejemplo), entonces todos deben firmar, pero si tiene una multifirma m-de-n donde "m < n", entonces la firma estará completa cuando sólo alguno ("m") de los firmantes haya firmado.

Para hacerlo primero acceden a sus claves privadas:
```
machine2$ bitcoin-cli -named dumpprivkey address=$address2
cVhqpKhx2jgfLUWmyR22JnichoctJCHPtPERm11a2yxnVFKWEKyz
```
Segundo, firman el nuevo `hex` usando los mismos valores `prevtxs`:
```
machine1$  bitcoin-cli -named signrawtransactionwithkey hexstring=020000000121654fa95d5a268abf96427e3292baed6c9f6d16ed9e80511070f954883864b100000000920047304402201c97b48215f261055e41b765ab025e77a849b349698ed742b305f2c845c69b3f022013a5142ef61db1ff425fbdcdeb3ea370aaff5265eee0956cff9aa97ad9a357e3010047522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352aeffffffff0188130000000000001600142c48d3401f6abed74f52df3f795c644b4398844600000000 prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cVhqpKhx2jgfLUWmyR22JnichoctJCHPtPERm11a2yxnVFKWEKyz"]'
{
  "hex": "020000000121654fa95d5a268abf96427e3292baed6c9f6d16ed9e80511070f954883864b100000000d90047304402201c97b48215f261055e41b765ab025e77a849b349698ed742b305f2c845c69b3f022013a5142ef61db1ff425fbdcdeb3ea370aaff5265eee0956cff9aa97ad9a357e301473044022000a402ec4549a65799688dd531d7b18b03c6379416cc8c29b92011987084e9f402205470e24781509c70e2410aaa6d827aa133d6df2c578e96a496b885584fb039200147522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352aeffffffff0188130000000000001600142c48d3401f6abed74f52df3f795c644b4398844600000000",
  "complete": true
}
```
Tercero, puede que necesiten enviar en la aún más larga `hexstring` que producen para firmantes adicionales.

Pero en este caso, vemos ahora que la firma está `completa`!

## Enviar Su Transacción

Una vez terminado, debería volver a la metodología JQ estándar para guardar su `hexstring` y luego enviarla:
```
$ signedtx=$(bitcoin-cli -named signrawtransactionwithkey hexstring=020000000121654fa95d5a268abf96427e3292baed6c9f6d16ed9e80511070f954883864b100000000920047304402201c97b48215f261055e41b765ab025e77a849b349698ed742b305f2c845c69b3f022013a5142ef61db1ff425fbdcdeb3ea370aaff5265eee0956cff9aa97ad9a357e3010047522102da2f10746e9778dd57bd0276a4f84101c4e0a711f9cfd9f09cde55acbdd2d1912102bfde48be4aa8f4bf76c570e98a8d287f9be5638412ab38dede8e78df82f33fa352aeffffffff0188130000000000001600142c48d3401f6abed74f52df3f795c644b4398844600000000 prevtxs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "scriptPubKey": "'$utxo_spk'", "redeemScript": "'$redeem_script'" } ]''' privkeys='["cVhqpKhx2jgfLUWmyR22JnichoctJCHPtPERm11a2yxnVFKWEKyz"]' | jq -r .hex)
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
99d2b5717fed8875a1ed3b2827dd60ae3089f9caa7c7c23d47635f6f5b397c04
```

## Comprendiendo la Importancia de Esta Metodología de Firmado Expandida

Esto requirió algo de trabajo, y como aprenderá pronto, la tontería con las claves privadas, el script de redención y el scriptpubkey en realidad no es necesario para redimir desde direcciones de firmas múltiples utilizando versiones más nuevas de Bitcoin Core. ¿De qué se trata, entonces?

Esta metodología de redención muestra una forma estándar de firmar y reutilizar transacciones P2SH. En síntesis, para redimir fondos P2SH, una `signrawtransactionwithkey` necesita:

1. Incluir la `scriptPubKey`, lo que explica el rompecabezas criptográfico P2SH.
2. Incluir el `redeemScript`, que resuelve el rompecabezas criptográfico P2SH, e introduce un nuevo rompecabezas propio.
3. Ejecutarse en cada máquina que tenga las claves privadas requeridas.
4. Incluir las firmas relevantes, que resuelve el rompecabezas redeemScript.

Aquí, vimos esta metodología utilizada para redimir fondos multifirma. En el futuro también puede usarlo para redimir fondos que estaban bloqueados con otros, scripts P2SH más complejos, como se explica a partir del Capítulo 9.

## Resumen: Gastando una Transacción con una Multifirma

Resulta que gastar dinero enviado a una dirección multifirma puede requerir bastante trabajo. Pero siempre que tenga sus direcciones originales y su redeemScript, puede hacerlo firmando una transacción sin procesar con cada dirección diferente y proporcionando más información en el camino.

## ¿Qué sigue?

Continuar "Expandiendo Transacciones Bitcoin" con [§6.3: Enviando & Gastando una Multifirma Automatizada](06_3_Enviando_una_Multifirma_Automatizada.md).
