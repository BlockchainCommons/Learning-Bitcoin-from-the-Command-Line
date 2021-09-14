# 5.1: Vigilancia de Transacciones Atascadas

A veces, una transacción de Bitcoin puede atascarse. Por lo general, se debe a que no hubo una tarifa de transacción suficiente, pero también puede deberse a una falla única en la red o el software.

## Mire Sus Transacciones

Debe _siempre_ vigilar para asegurarse de que sus transacciones se realicen. `bitcoin-cli listtransactions` mostrará todas sus transacciones entrantes y salientes, mientras que `bitcoin-cli gettransaction` con un txid mostrará una transacción especifica.

A continuación, se muestra una transacción que no se ha incuido en un bloque. Puede decir esto porque no tiene confirmaciones.
```
$ bitcoin-cli -named gettransaction txid=fa2ddf84a4a632586d435e10880a2921db6310dfbd6f0f8f583aa0feacb74c8e
{
  "amount": -0.00020000,
  "fee": -0.00001000,
  "confirmations": 0,
  "trusted": true,
  "txid": "fa2ddf84a4a632586d435e10880a2921db6310dfbd6f0f8f583aa0feacb74c8e",
  "walletconflicts": [
  ],
  "time": 1592953220,
  "timereceived": 1592953220,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "tb1qw508d6qejxtdg4y5r3zarvary0c5xw7kxpjzsx",
      "category": "send",
      "amount": -0.00020000,
      "vout": 0,
      "fee": -0.00001000,
      "abandoned": false
    }
  ],
  "hex": "02000000014cda1f42a1bd39d8d0ff5958a804bc2bc548b71d7ceadbde53ea15aeaf1e2691000000006a473044022016a7a9f045a0f6a52129f48adb7da35c2f54a0741d6614e9d55b8a3bc3e1490a0220391e9085a3697bc790e94bb924d5310e16f23489d9c600864a32674e871f523c01210278608b54b8fb0d8379d3823d31f03a7c6ab0adffb07dd3811819fdfc34f8c132ffffffff02204e000000000000160014751e76e8199196d454941c45d1b3a323f1433bd6e8030000000000001600146c45d3afa8762086c4bd76d8a71ac7c976e1919600000000"
```
Una transacción puede considerarse bloqueada si permanece en este estado durante un período de temipo prolongado. No hace muchos años, podía estar seguro do que todas las transacciones saldrían _eventualmente_. Pero, ese ya no es el caso debido al mayor uso de Bitcoin. Ahora, si una transacción se atasca demasiado tiempo, saldrá del mempool y luego se perderá de la red Bitcoin.

> :book: ***Que es mempool?*** Mempool (o Pool de Memoria) es un grupo de todas las transacciones no confirmadas en un nodo bitcoin. Estas son las transacciones que un nodo ha recibido de la red p2p que aún no están incluidas en un bloque. Cada nodo de bitcoin puede tener un conjunto de transacctiones ligeramente diferente en su mempool: diferentes transacciones pueden haberse propagado a un nodo específico. Esto depende de cuándo se inició el nodo por última vez y también de sus límites sobre cuánto está dispuesto a almacenar. Cuando un minero hace un bloqueo, usa transacciones de su mempool. Luego cuando se verifica un bloque, todos los mineros eliminan las transacciones que contiene de sus grupos. A partir de Bitcoin 0.12, las transacciones no confirmadas también pueden vencer de la mempool si tienen la antigüedad suficiente, por lo general, 72 horas, y a partir de la versión 0.14.0, el tiempo de desalojo se incrementó a 2 semanas. Los grupos de minería pueden tener sus propios mecanismos de gestión de mempool.

Es posible que esta lista de todos las [transacciones no confirmadas](https://blockchain.info/unconfirmed-transactions) no coincida con el mempool de ninguna máquina individual, pero debería (en su mayoría) ser un superconjunto de ellas.

## Decidir Que Hacer

Si su transacción se atasca más de lo que deseas, normalmente puede hacer una de estas cuatro cosas:

**1. Espere Hasta Que Se Aclare.** Se envió su transacción con una tarifa baja o media, eventualmente debería pasar. Como se muestra en [Mempool Space](https://mempool.space), aquellos con tarifas más bajas _se retrasarán_. (Eche un vistazo a la transacción más a la izquierda y vea cuánto tiempo ha estado esperando y cuánto pagó por su tarifa)

**2. Espere Hasta Que Expire.** Se envió accidentalmente sin tarifa de transacción, o si se cumplen otras condiciones, es posible que su transacción nunca se realice. Sin embargo, sus monedas no se pierden. Siempre que no tenga una billetera que reenvíe intencionalmente las transacciones no confirmadas, debería borrarse del mempool en aproximadamente tres días, y luego puede volver a intentarlo.

**3. Utilice RBF Como Remitente.** Si usted es el remitente de la transacción y se inscribió en RBF (Replace by fee / Reemplazar Por Tarifa), puede volver a intentarlo con una tarifa más alta. Consulte [§5.2: Resending a Transaction with RBF](05_2_Resending_a_Transaction_with_RBF.md).

**4. Us CPFP Como La Receptora.** Alternativeamente, si usted es el receptor de la transacción, puede usar CPFP (Child-pays-for-parent / nino-paga-por-el-padre) para usar la transacción no confirmada como entrada para una nueva transacción. Consulte [§5.3: Funding a Transaction with CPFP](05_3_Funding_a_Transaction_with_CPFP.md).

## Resumen: Observación de Transacciones Atascadas

Esta es una introducción al poder de las transacciónes de Bitcoin. Si lo sabe que una transacción está atascada, puede decidir liberarla con funciones como RBF o CPFP.

## Que Sigue?

Continúe "Controlando las Transacciones de Bitcoin" con [§5.2: Resending a Transaction with RBF](05_2_Resending_a_Transaction_with_RBF.md).  
