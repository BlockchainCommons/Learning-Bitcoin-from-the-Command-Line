# 4.3 Creación de una transacción en cruda con argumentos ingresado con nombre

A veces puede ser desalentador averiguar el orden correcto de los argumentos de un comando bitcoin-cli. Afortunadamente, puede utilizar _argumentos con nombre_ como alternativa.

> ⚠️ **ADVERTENCIA DE VERSIÓN:** Esta es una innovación de Bitcoin Core v 0.14.0. Si ha utilizado nuestros scripts de instalación, es lo que debería tener, pero vuelva a comprobar su versión si tiene algún problema. También hay un error en el uso del comando `createrawtransaction` de los argumentos con nombre que presumiblemente será corregido en 0.14.1.

## Crear un Alias para el argumento con nombre 

Para utilizar un argumento con nombre debe ejecutar `bitcoin-cli` con el argumento `-named`. Si planea hacer esto regularmente, probablemente querrá crear un alias:
```
alias bitcoin-cli="bitcoin-cli -named"
```
Como siempre, esto es para facilitar su uso, pero seguiremos usando los comandos completos para mantener la claridad.

## Probar un argumento con nombre

Para conocer los nombres de los argumentos de un comando, consulta la ayuda de `bitcoin-cli`. Enumerará los argumentos en su orden correcto, pero ahora también dará nombres para cada uno de ellos.

Por ejemplo, `bitcoin-cli help getbalance` lista estos argumentos:

   1. dummy [used to be account]
   2. minconf
   3. include_watchonly
   4. avoid_reuse
   
A continuación se muestra un uso tradicional y poco intuitivo de `getbalance` utilizando el argumento de confirmación mínima:
```
$ bitcoin-cli getbalance "*" 1
```
Con los argumentos con nombre, puede aclarar lo que está haciendo, lo que también minimiza los errores:
```
$ bitcoin-cli -named getbalance minconf=1
```

## Probar una transacción en crudo

A continuación se muestran los comandos para enviar una transacción en crudo con argumentos nombrados:
```
$ utxo_txid=$(bitcoin-cli listunspent | jq -r '.[0] | .txid') 
$ utxo_vout=$(bitcoin-cli listunspent | jq -r '.[0] | .vout')
$ recipient="n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi"

$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.00001 }''')
$ bitcoin-cli -named decoderawtransaction hexstring=$rawtxhex 
{
  "txid": "2b59c31bc232c0399acee4c2a381b564b6fec295c21044fbcbb899ffa56c3da5",
  "hash": "2b59c31bc232c0399acee4c2a381b564b6fec295c21044fbcbb899ffa56c3da5",
  "version": 2,
  "size": 85,
  "vsize": 85,
  "weight": 340,
  "locktime": 0,
  "vin": [
    {
      "txid": "ca4898d8f950df03d6bfaa00578bd0305d041d24788b630d0c4a32debcac9f36",
      "vout": 0,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 0.00001000,
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
    }
  ]
}

$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
e70dd2aa13422d12c222481c17ca21a57071f92ff86bdcffd7eaca71772ba172
```
¡Voilà! Ha enviado otra transacción en crudo, pero esta vez utilizando argumentos con nombre para mayor claridad y para reducir los errores.

> ⚠️ **ADVERTENCIA DE VERSIÓN:** Aquí es donde aparece el error en Bitcoin Core 0.14: el argumento 'inputs' para 'createrawtransaction' se llama erróneamente 'transactions'. Así que, si está en Bitcoin Core 0.14.0, sustituye el argumento 'inputs' por 'transactions' para este y futuros ejemplos. Sin embargo, a partir de Bitcoin Core 0.14.1, este código debería funcionar como se muestra.

## Resumen: Creación de una transacción en crudo con argumentos nombrados

Ejecutando `bitcoin-cli` con la bandera `-named`, puede utilizar argumentos con nombre en lugar de depender de argumentos ordenados. La ayuda de `bitcoin-cli` le mostrará siempre el nombre correcto de cada argumento. Esto puede resultar en un código más robusto, más fácil de leer y menos propenso a errores.

_Esta documentación utilizará argumentos con nombre para todos los ejemplos futuros, por claridad y para establecer las mejores prácticas. Sin embargo, también mostrará todos los argumentos en el orden correcto. Por lo tanto, si prefiere no utilizar argumentos con nombre, simplemente elimine la bandera '-named' y todos los "name="s y los ejemplos deberían seguir funcionando correctamente._

## ¿Qué sigue?

Continuar "Enviando transacciones de Bitcoin" con [§4.4: Enviando Monedas con una Transacción Cruda](04_4_Enviando_Monedas_con_una_Transaccion_Cruda.md).
