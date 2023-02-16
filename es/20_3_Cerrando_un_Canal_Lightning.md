# 20.3: Cerrando un Canal

> :information_source: **NOTA:** Esta sección se ha agregado recientemente al curso y es un borrador inicial que aún puede estar pendiente de revisión. Lector de advertencias.

En este capítulo aprenderá a cerrar un canal usando la interfaz de línea de comandos `lightning-cli close`. Cerrar un canal significa que usted y su contraparte enviarán el saldo del canal acordado a la blockchain, por lo que debe pagar las tarifas de transacción de la blockchain y esperar a que se mine la transacción. Un cierre puede ser cooperativo o no cooperativo, pero funciona de cualquier manera.

Para cerrar un canal, primero debe conocer el ID del nodo remoto; puede recuperarlo de dos formas.

## Encuentra sus Canales por Fondos

Puede usar el comando `lightning-cli listfunds` para ver sus canales. Este comando RPC muestra todos los fondos disponibles, ya sea en `salidas` (UTXOs) no gastadas en la billetera interna o bloqueados en `canales` abiertos actualmente.
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
         "channel_sat": 89987,
         "our_amount_msat": "89987000msat",
         "channel_total_sat": 100000,
         "amount_msat": "100000000msat",
         "funding_txid": "66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d",
         "funding_output": 0
      }
   ]
}

  "message_flags": 1,
  "channel_flags": 2,
  "active": true,
  "last_update": 1595508075,
  "base_fee_millisatoshi": 1000,
  "fee_per_millionth": 1,
  "delay": 40,
  "htlc_minimum_msat": "1000msat",
  "htlc_maximum_msat": "280000000msat",
  "features": ""
}
```

Puede recuperar el ID del canal 0 en una variable como esta:
```
c$ nodeidremote=$(lightning-cli --testnet listfunds | jq '.channels[0] | .peer_id')
```

## Encuentra sus Canales con JQ

La otra forma de encontrar canales para cerrar es usando el comando `listchannels`. Devuelve datos sobre canales que son conocidos por el nodo. Debido a que los canales pueden ser bidireccionales, se devolverán hasta dos nodos para cada canal (uno para cada dirección). 

Sin embargo, la red de chismes de Lightning es muy efectiva, por lo que en poco tiempo conocerá miles de canales. Eso es excelente para enviar pagos a través de Lightning Network, pero menos útil para descubrir sus propios canales. Hacerlo requiere un poco trabajo `jq`.

Primero, necesita saber su propia ID de nodo, que puede ser recuperada con `getinfo`:
```
c$ nodeid=$(lightning-cli --testnet getinfo | jq .id)
c$ echo $nodeid
"03240a4878a9a64aea6c3921a434e573845267b86e89ab19003b0c910a86d17687"
c$
```
Luego puede usar eso para buscar a través de `listchannels` por cualquier canal donde su nodo sea el origen o el destino:
```
c$ lightning-cli --testnet listchannels | jq '.channels[] | select(.source == '$nodeid' or .destination == '$nodeid')'
{
  "source": "03240a4878a9a64aea6c3921a434e573845267b86e89ab19003b0c910a86d17687",
  "destination": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
  "short_channel_id": "1862856x29x0",
  "public": true,
  "satoshis": 100000,
  "amount_msat": "100000000msat",
  "message_flags": 1,
  "channel_flags": 0,
  "active": true,
  "last_update": 1602639570,
  "base_fee_millisatoshi": 1,
  "fee_per_millionth": 10,
  "delay": 6,
  "htlc_minimum_msat": "1msat",
  "htlc_maximum_msat": "99000000msat",
  "features": ""
}
```
Ahí está nuestro viejo favorito `032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543` nuevamente, como destino.

Una vez que sepa lo que tiene, puede almacenarlo en una variable:
```
c$ nodeidremote=$(lightning-cli --testnet listchannels | jq '.channels[] | select(.source == '$nodeid' or .destination == '$nodeid') | .destination')
```

## Cerrar un Canal

Ahora que tiene un ID de nodo remoto, está listo para usar el comando `lightning-cli close` para cerrar un canal. De forma predeterminada, intentará cerrar el canal de forma cooperativa con el par; si desea cerrarlo unilateralmente, establezca el argumento `unilateraltimeout` con el número de segundos de espera. (Si lo establece en 0 y el par está en línea, aún se intenta un cierre mutuo.) Para este ejemplo, intentará un cierre mutuo.

```
c$ lightning-cli --testnet close $nodeidremote 0
{
   "tx": "02000000011d3cf2126ae36e12be3aee893b385ed6a2e19b1da7f4e579e3ef15ca234d69660000000000ffffffff021c27000000000000160014d39feb57a663803da116402d6cb0ac050bf051d9cc5e01000000000016001451c88b44420940c52a384bd8a03888e3676c150900000000",
   "txid": "f68de52d80a1076e36c677ef640539c50e3d03f77f9f9db4f13048519489593f",
   "type": "mutual"
}
```
La transacción de cierre en cadena es [f68de52d80a1076e36c677ef640539c50e3d03f77f9f9db4f13048519489593f](https://blockstream.info/testnet/tx/f68de52d80a1076e36c677ef640539c50e3d03f77f9f9db4f13048519489593f).

Es esta transacción de cierre la que realmente desembolsa los fondos que se intercambiaron de un lado a otro a través de transacciones Lightning. Esto se puede ver examinando la transacción:
```
$ bitcoin-cli --named getrawtransaction txid=f68de52d80a1076e36c677ef640539c50e3d03f77f9f9db4f13048519489593f verbose=1
{
  "txid": "f68de52d80a1076e36c677ef640539c50e3d03f77f9f9db4f13048519489593f",
  "hash": "3a6b3994932ae781bab80e159314bad06fc55d3d33453a1d663f9f9415c9719c",
  "version": 2,
  "size": 334,
  "vsize": 169,
  "weight": 673,
  "locktime": 0,
  "vin": [
    {
      "txid": "66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d",
      "vout": 0,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "txinwitness": [
        "",
        "304402207f8048e29192ec86019bc83be8b4cac5d1fc682374538bed0707f58192d41c390220512ebcde122d53747feedd70c09153a40c56d09a5fec02e47642afdbb20aa2ac01",
        "3045022100d686a16084b60800fa0f6b14c25dca1c13d10a55c5fb7c6a3eb1c5f4a2fb20360220555f5b6e672cf9ef82941f7d46ee03dd52e0e848b9f094a41ff299deb8207cab01",
        "522102f7589fd8366252cdbb37827dff65e3304abd5d17bbab57460eff71a9e32bc00b210343b980dff4f2723e0db99ac72d0841aad934b51cbe556ce3a1b257b34059a17052ae"
      ],
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 0.00010012,
      "n": 0,
      "scriptPubKey": {
        "asm": "0 d39feb57a663803da116402d6cb0ac050bf051d9",
        "hex": "0014d39feb57a663803da116402d6cb0ac050bf051d9",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1q6w07k4axvwqrmggkgqkkev9vq59lq5we5fcrzn"
        ]
      }
    },
    {
      "value": 0.00089804,
      "n": 1,
      "scriptPubKey": {
        "asm": "0 51c88b44420940c52a384bd8a03888e3676c1509",
        "hex": "001451c88b44420940c52a384bd8a03888e3676c1509",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1q28ygk3zzp9qv223cf0v2qwygudnkc9gfp30ud4"
        ]
      }
    }
  ],
  "hex": "020000000001011d3cf2126ae36e12be3aee893b385ed6a2e19b1da7f4e579e3ef15ca234d69660000000000ffffffff021c27000000000000160014d39feb57a663803da116402d6cb0ac050bf051d9cc5e01000000000016001451c88b44420940c52a384bd8a03888e3676c1509040047304402207f8048e29192ec86019bc83be8b4cac5d1fc682374538bed0707f58192d41c390220512ebcde122d53747feedd70c09153a40c56d09a5fec02e47642afdbb20aa2ac01483045022100d686a16084b60800fa0f6b14c25dca1c13d10a55c5fb7c6a3eb1c5f4a2fb20360220555f5b6e672cf9ef82941f7d46ee03dd52e0e848b9f094a41ff299deb8207cab0147522102f7589fd8366252cdbb37827dff65e3304abd5d17bbab57460eff71a9e32bc00b210343b980dff4f2723e0db99ac72d0841aad934b51cbe556ce3a1b257b34059a17052ae00000000",
  "blockhash": "000000000000002a214b1ffc3a67c64deda838dd24d12154c15d3a6f1137e94d",
  "confirmations": 1,
  "time": 1602713519,
  "blocktime": 1602713519
}
```
La entrada de la transacción es `66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d`, que era la transacción de financiación en [§19.3](19_3_Creando_un_Canal_en_Lightning.md). La transacción tiene dos salidas, una para el nodo remoto y la otra para la billetera c-lightning local. La salida en el índice 0 corresponde al nodo remoto con un valor de 0.00010012 BTC; y la salida en el índice 1 corresponde al nodo local con un valor de 0.00089804 BTC.

Lightning mostrará de manera similar 89804 satoshis devueltos como un nuevo UTXO en su billetera:
```
$ lightning-cli --network=testnet listfunds
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
      },
      {
         "txid": "f68de52d80a1076e36c677ef640539c50e3d03f77f9f9db4f13048519489593f",
         "output": 1,
         "value": 89804,
         "amount_msat": "89804000msat",
         "scriptpubkey": "001451c88b44420940c52a384bd8a03888e3676c1509",
         "address": "tb1q28ygk3zzp9qv223cf0v2qwygudnkc9gfp30ud4",
         "status": "confirmed",
         "blockheight": 1863006,
         "reserved": false
      }
   ],
   "channels": [
      {
         "peer_id": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
         "connected": false,
         "state": "ONCHAIN",
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

### Entender los Tipos de Canales de Cierre

El comando `close` RPC intenta cerrar un canal de manera cooperativa con su par o unilateralmente después de que expire al argumento `unilateraltimeout`. Esto conlleva una discusión adicional, ya que va al corazón del diseño sin confianza de Lightning:

Cada participante de un canal puede crear tantos pagos Lightning a su contraparte como lo permitan sus fondos. La mayoría de las veces no habrá desacuerdos entre los participantes, por lo que solo habrá dos transacciones en cadena, una abriendo y la otra cerrando el canal. Sin embargo, puede haber escenarios en los que un par no esté en línea o no esté de acuerdo con el estado final del canal o cuando alguien intente robar fondos de la otra parte. Es por eso que existen cierres tanto cooperativos como forzosos.

#### Cierre Cooperativo

En el caso de un cierre cooperativo, ambos participantes del canal acuerdan cerrar el canal y establecer el estado final de la blockchain. Ambos participantes deben estar en línea; El cierre se realiza mediante la transmisíon de un gasto incondicional de la transacción de financiación con una salida a cada par. 

#### Forzar Cierre

En el caso de un cierre forzado, solo un participante está en línea o los participantes no están de acuerdo con el estado final del canal. En esta situación, un par puede realizar un cierre unilateral del canal sin la cooperación del otro nodo. Se realiza mediante la transmisión de una transacción de compromiso que se compromete con un estado de canal anterior que ambas partes han acordado. Esta transacción de compromiso contiene el estado del canal dividido en dos partes: el saldo de cada participante y todos los pagos pendientes (HTLCs).

Para realizar este tipo de cierre, debe especificar un argumento `unilateraltimeout`. Si este valor no es cero, el comando de cierre cerrará unilateralmente el canal cuando se alcance ese número de segundos:
```
c$ lightning-cli --network=testnet close $newidremote 60
{
   "tx": "0200000001a1091f727e6041cc93fead2ea46b8402133f53e6ab89ab106b49638c11f27cba00000000006a40aa8001df85010000000000160014d22818913daf3b4f86e0bcb302a5a812d1ef6b91c6772d20",
   "txid": "02cc4c647eb3e06f37fcbde39871ebae4333b7581954ea86b27b85ced6a5c4f7",
   "type": "unilateral"
}

```
## Resumen: Cerrando un Canal

Cuando cierra un canal, realiza una transacción en cadena que pone fin a su relación financiera con el nodo remoto. Para cerrar un canal, debe tener en cuenta su estado y el tipo de cierre que quiere ejecutar.

## ¿Que Sigue?

Continúe "Usando Lightning" con [§20.4: Expandiendo la Red Lightning](20_4_Expandiendo_la_Red_Lightning.md).
