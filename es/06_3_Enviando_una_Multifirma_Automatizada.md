# 6.3: Enviando & Gastando una Multifirma Automatizada

La técnica estándar para crear direcciones de multifirma y para gastar sus fondos es compleja, pero es un ejercicio que vale la pena para comprender un poco más sobre cómo funcionan y cómo puede manipularlas a un nivel relativamente bajo. Sin embargo, Bitcoin Core ha facilitado un poco las multifirmas en los nuevos lanzamientos. 

> :warning: **AVISO DE VERSION:** El comando `addmultisigaddress` está disponible en Bitcoin Core v 0.10 o superior.

## Crear una Dirección Multifirma en Su Billetera

Para que los fondos enviados a direcciones multifirma sean más fáciles de gastar, sólo necesita hacer algunos preparativos usando el comando `addmultisigaddress`. Probablemente no sea lo que le gustaría hacer si estuviera escribiendo programas de billetera multifirma, pero si sólo estuviera tratando de recibir algunos fondos a mano, podría ahorrarse algunos tirones de pelo.

### Recolectar las Claves
Empieza creando direcciones P2PKH y recuperando claves públicas, como es habitual, para cada usuario que formará parte de la multifirma:
```
machine1$ address3=$(bitcoin-cli getnewaddress)
machine1$ echo $address3
tb1q4ep2vmakpkkj6mflu94x5f94q662m0u5ad0t4w
machine1$ bitcoin-cli -named getaddressinfo address=$address3 | jq -r '.pubkey'
0297e681bff16cd4600138449e2527db4b2f83955c691a1b84254ecffddb9bfbfc

machine2$ address4=$(bitcoin-cli getnewaddress)
machine2$ echo $address4
tb1qa9v5h6zkhq8wh0etnv3ae9cdurkh085xufl3de
machine2$ bitcoin-cli -named getaddressinfo address=$address4 | jq -r '.pubkey'
02a0d96e16458ff0c90db4826f86408f2cfa0e960514c0db547ff152d3e567738f
```

### Crear la Dirección Multifirma en Cualquier Lugar

A continuación, crea la multifirma en _cada máquina que aporta firmas_ usando un nuevo comando, `addmultisigaddress`, en lugar de `createmultisig`. Este nuevo comando guarda parte de la información en su billetera, lo que hace que sea mucho más fácil gastar el dinero después.
```
machine1$ bitcoin-cli -named addmultisigaddress nrequired=2 keys='''["'$address3'","02a0d96e16458ff0c90db4826f86408f2cfa0e960514c0db547ff152d3e567738f"]'''
{
  "address": "tb1q9as46kupwcxancdx82gw65365svlzdwmjal4uxs23t3zz3rgg3wqpqlhex",
  "redeemScript": "52210297e681bff16cd4600138449e2527db4b2f83955c691a1b84254ecffddb9bfbfc2102a0d96e16458ff0c90db4826f86408f2cfa0e960514c0db547ff152d3e567738f52ae",
  "descriptor": "wsh(multi(2,[d6043800/0'/0'/15']0297e681bff16cd4600138449e2527db4b2f83955c691a1b84254ecffddb9bfbfc,[e9594be8]02a0d96e16458ff0c90db4826f86408f2cfa0e960514c0db547ff152d3e567738f))#wxn4tdju"
}

machine2$ bitcoin-cli -named addmultisigaddress nrequired=2 keys='''["0297e681bff16cd4600138449e2527db4b2f83955c691a1b84254ecffddb9bfbfc","'$address4'"]'''
{
  "address": "tb1q9as46kupwcxancdx82gw65365svlzdwmjal4uxs23t3zz3rgg3wqpqlhex",
  "redeemScript": "52210297e681bff16cd4600138449e2527db4b2f83955c691a1b84254ecffddb9bfbfc2102a0d96e16458ff0c90db4826f86408f2cfa0e960514c0db547ff152d3e567738f52ae",
  "descriptor": "wsh(multi(2,[ae42a66f]0297e681bff16cd4600138449e2527db4b2f83955c691a1b84254ecffddb9bfbfc,[fe6f2292/0'/0'/2']02a0d96e16458ff0c90db4826f86408f2cfa0e960514c0db547ff152d3e567738f))#cc96c5n6"
}
```
Como se señaló en la sección anterior, actualmente no importa si usa direcciones o claves públicas, por lo que mostramos el otro mecanismo aquí, mezclando los dos. Obtendrá la misma dirección multifirma de cualquier manera. Sin embargo, _debe utilizar el mismo orden_. Por lo tanto, es mejor que los miembros de la multifirma se comprueben entre ellos para asegurarse de que todos obtuvieron el mismo resultado.

### Estar Atento a los Fondos

Posteriormente, los miembros de la multifirma aún necesitarán ejecutar `importaddress` para estar atentos a los fondos recibidos en la dirección multifirma:
```
machine1$ bitcoin-cli -named importaddress address=2Mzw7WBvh9RAQ4ssKqxyNyP7L9NAojLqSW8 rescan="false"

machine2$ bitcoin-cli -named importaddress address=2Mzw7WBvh9RAQ4ssKqxyNyP7L9NAojLqSW8 rescan="false"
```

## Volver a Gastar con una Transacción Automatizada

Posteriormente, podrá recibir fondos en la dirección multifirma como de costumbre. El uso de `addmultisigaddress` es simplemente una cuestión burocrática por parte de los destinatarios: un poco de contabilidad para facilitarles la vida cuando quieran gastar sus fondos.

Pero hace la vida mucho más fácil. Debido a que la información se guardó en la billetera, los firmantes podrán volver a gastar los fondos enviados a la dirección multifirma exactamente igual que cualquier otra dirección ... aparte de la necesidad de firmar en varias máquinas.

Comienza recolectando sus variables, pero ya no tiene que preocuparse por `scriptPubKey` o `redeemScript`.

Aquí hay una nueva transacción enviada a nuestra nueva dirección multifirma:
```
machine1$ utxo_txid=b9f3c4756ef8159d6a66414a4317f865882ee04beb57a0f8349dafcc98f5acbc
machine1$ utxo_vout=0
machine1$ recipient=$(bitcoin-cli getrawchangeaddress)
```
Crea una transacción sin procesar:
```
machine1$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient'": 0.00005}''')
```
Luego la firma:
```
machine1$ bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex
{
  "hex": "02000000000101bcacf598ccaf9d34f8a057eb4be02e8865f817434a41666a9d15f86e75c4f3b90000000000ffffffff0188130000000000001600144f93c831ec739166ea425984170f4dc6bac75829040047304402205f84d40ba16ff49e60a7fc9228ef5917473aae1ab667dad01e113ca0fef3008b02201a50da2c65f38798aea94bcbd5bbf065bc1e38de44bacee69d525dcddcc11bba01004752210297e681bff16cd4600138449e2527db4b2f83955c691a1b84254ecffddb9bfbfc2102a0d96e16458ff0c90db4826f86408f2cfa0e960514c0db547ff152d3e567738f52ae00000000",
  "complete": false,
  "errors": [
    {
      "txid": "b9f3c4756ef8159d6a66414a4317f865882ee04beb57a0f8349dafcc98f5acbc",
      "vout": 0,
      "witness": [
        "",
        "304402205f84d40ba16ff49e60a7fc9228ef5917473aae1ab667dad01e113ca0fef3008b02201a50da2c65f38798aea94bcbd5bbf065bc1e38de44bacee69d525dcddcc11bba01",
        "",
        "52210297e681bff16cd4600138449e2527db4b2f83955c691a1b84254ecffddb9bfbfc2102a0d96e16458ff0c90db4826f86408f2cfa0e960514c0db547ff152d3e567738f52ae"
      ],
      "scriptSig": "",
      "sequence": 4294967295,
      "error": "CHECK(MULTI)SIG failing with non-zero signature (possibly need more signatures)"
    }
  ]
}

```
Tenga en cuenta que ya no tiene que darle ayuda extra a `signrawtransactionwithkey`, porque toda esa información adicional ya estaba en su billetera. Lo más importante es que no ha hecho vulnerables sus claves privadas manipulándolas directamente. En su lugar el proceso fue _exactamente_ el mismo que volver a gastar una UTXO normal, excepto que la transacción no fue firmada completamente al final.

### Fírmela en Otras Máquinas

El paso final es exportar el `hex` parcialmente firmado a cualquier otra máquina y volver a firmarlo:
```
machine2$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=02000000014ecda61c45f488e35c613a7c4ae26335a8d7bfd0a942f026d0fb1050e744a67d000000009100473044022025decef887fe2e3eb1c4b3edaa155e5755102d1570716f1467bb0b518b777ddf022017e97f8853af8acab4853ccf502213b7ff4cc3bd9502941369905371545de28d0147522102e7356952f4bb1daf475c04b95a2f7e0d9a12cf5b5c48a25b2303783d91849ba421030186d2b55de166389aefe209f508ce1fbd79966d9ac417adef74b7c1b5e0777652aeffffffff0130e1be07000000001976a9148dfbf103e48df7d1993448aa387dc31a2ebd522d88ac00000000 | jq -r '.hex')
```
Cuando todos los que sean necesarios hayan firmado, se irá a las carreras:
```
machine2$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
3ce88839ac6165aeadcfb188c490e1b850468eff571b4ca78fac64342751510d
```
Al igual que con el shortcut discutido en [§4.5: Enviando Monedas con Transacciones Crudas Automatizadas](04_5_Enviando_Monedas_con_Transacciones_Crudas_Automatizadas.md), el resultado es mucho más sencillo, pero pierde algo de control en el proceso.

## Resúmen: Enviando & Gastando una Multifirma Automatizada

Existe una manera más fácil de volver a gastar los fondos enviados a direcciones multifirma que simplemente requiere el uso del comando `addmultisigaddress` cuando crea su dirección. No demuestra las complejidades de volver a gastar de P2SH y no le brinda un control amplio, pero si solo desea obtener su dinero, este es el camino a seguir.

## ¿Qué sigue?

Aprende más acerca de "Expandiendo Transacciones Bitcoin" con [Capítulo Siete: Expandiendo las Transacciones Bitcoin con PSBTs](07_0_Expandiendo_las_Transacciones_Bitcoin_con_PSBTs.md).
