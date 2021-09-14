# 5.3: Financiación de una Transación con CPFP

Si su transacción de Bitocin está atascada y usted es el _receptor_, puede borrarla usando CPFP (child-pays-for-parent o el niño paga por el padre). Esta es una alternativa a la capacidad del _remitente_ para hacerlo con RBF.

> :warning: **ADVERTENCIA DE VERSIÓN:** Esta es una innovación de Bitcoin Core v 0.13.0, lo que nuevamente significa que la mayoría de la gente ya debería estar usándolo.

## Comprenda Cómo Funciona CPFP

RBF se trataba del remitente. Se equivocó y necesitaba aumentar la tarifa, o quería ser inteligente y combinar transacciones por una variedad de razones. Era una potente función orientada al remitente. De alguna manera, CPFP es lo opuesto a RBF, porque le da poder al destinatario que sabe que su dinero aún no ha llegado y quiere acelerarlo. Sin embargo, también es una característica mucho más simple, con una aplicabilidad menos amplia.

Básicamente, la idea de CPFP es que un destinatario tiene una transacción que no ha sido confirmada en un bloque que quiere gastar. Entonces, incluye esa transacción no confirmada en una nueva transacción y paga una tarifa lo suficientemente alta como para alentar a un minero a incluir tanto la transacción original (principal) como la nueva transacción (secundaria) en un bloque. Como resultado, las transacciones principales y secundarias se borran simultáneamnete.

Cabe señalar que CPFP no es una función de protocolo nueva como RBF.  Es solo un nuevo esquema de incentivos que los mineros pueden usar para la selección de transacciones. Esto también significa que no es tan confiable como un cambio de protocolo como RBF: puede haber razones por las que la secundaria no sea seleccionado para ser colocado en un bloque, y eso evitará que el principal sea colocado en un bloque.

## Gastar UTXOs No Confirmadas
Financiar una transacción con CPFP es un proceso muy simple que utiliza los métodos con los que ya está familiarizado:

   1. Busque el `txid` y `vout` de la transacción no confirmada. Esta puede ser la parte más complicada, ya que `bitcoin-cli` generalmente intenta protegerlo de transaciones no confirmadas. Es posible que el remitente pueda enviarle esta información; incluso con solo el `txid`, debería poder averigular el `vout` en un explorador de blockchain.
   
   Tiene otra opción: use `bitcoin-cli getrawmempool`, que se puede usar para enumerar el contenido de su mempool completo, donde se encontrarán las transacciones no confirmadas. Es posible que tenga que buscar en varios se el mempool está particularmente ocupado. A continuación, puede obtener más información sobre una transacción especifica con `bitcoin-cli getrawtransaction` con el indicador detallado en `true`:
   ```
$ bitcoin-cli getrawmempool
[
  "95d51e813daeb9a861b2dcdddf1da8c198d06452bbbecfd827447881ff79e061"
]

$ bitcoin-cli getrawtransaction 95d51e813daeb9a861b2dcdddf1da8c198d06452bbbecfd827447881ff79e061 true
{
  "txid": "95d51e813daeb9a861b2dcdddf1da8c198d06452bbbecfd827447881ff79e061",
  "hash": "9729e47b8aee776112a82cec46df7638d112ca51856c53e238a9b1f7af3be4ce",
  "version": 2,
  "size": 247,
  "vsize": 166,
  "weight": 661,
  "locktime": 1773277,
  "vin": [
    {
      "txid": "7a0178472300247d423ac4a04ff9165fa5b944104f6d6f9ebc557c6d207e7524",
      "vout": 0,
      "scriptSig": {
        "asm": "0014334f3a112df0f22e743ad97eec8195a00faa59a0",
        "hex": "160014334f3a112df0f22e743ad97eec8195a00faa59a0"
      },
      "txinwitness": [
        "304402207966aa87db340841d76d3c3596d8b4858e02aed1c02d87098dcedbc60721d8940220218aac9d728c9a485820b074804a8c5936fa3145ce68e24dcf477024b19e88ae01",
        "03574b1328a5dc2d648498fc12523cdf708efd091c28722a422d122f8a0db8daa9"
      ],
      "sequence": 4294967294
    }
  ],
  "vout": [
    {
      "value": 0.01000000,
      "n": 0,
      "scriptPubKey": {
        "asm": "OP_HASH160 f079f77f2ef0ef1187093379d128ec28d0b4bf76 OP_EQUAL",
        "hex": "a914f079f77f2ef0ef1187093379d128ec28d0b4bf7687",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2NFAkGiwnp8wvCodRBx3smJwxncuG3hndn5"
        ]
      }
    },
    {
      "value": 0.02598722,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_HASH160 8799be12fb9eae6644659d95b9602ddfbb4b2aff OP_EQUAL",
        "hex": "a9148799be12fb9eae6644659d95b9602ddfbb4b2aff87",
        "reqSigs": 1,
        "type": "scripthash",
        "addresses": [
          "2N5cDPPuCTtYq13oXw8RfpY9dHJW8sL64U2"
        ]
      }
    }
  ],
  "hex": "0200000000010124757e206d7c55bc9e6f6d4f1044b9a55f16f94fa0c43a427d2400234778017a0000000017160014334f3a112df0f22e743ad97eec8195a00faa59a0feffffff0240420f000000000017a914f079f77f2ef0ef1187093379d128ec28d0b4bf768742a727000000000017a9148799be12fb9eae6644659d95b9602ddfbb4b2aff870247304402207966aa87db340841d76d3c3596d8b4858e02aed1c02d87098dcedbc60721d8940220218aac9d728c9a485820b074804a8c5936fa3145ce68e24dcf477024b19e88ae012103574b1328a5dc2d648498fc12523cdf708efd091c28722a422d122f8a0db8daa9dd0e1b00"
}
```
Busque a través de la matriz `vout`. Encuentra el objecto que coincida con su dirección. (Aqui, es el único.) El `n` valor es su `vout`. Ahora tiene todo lo que necesita para crear una nueva transacción de CPFP.
```
$ utxo_txid=2NFAkGiwnp8wvCodRBx3smJwxncuG3hndn5
$ utxo_vout=0
$ recipient2=$(bitcoin-cli getrawchangeaddress)
```

   2. Cree una transacción sin procesar utilizando su transacción no confirmada como entrada.
   3. Duplique las tarifas de transacción (o mas).
   
Cuando siga estos pasos, todo debería verse totalmente normal, a pesar de que está trabajando con una transacción no confirmada. Para verificar que todo estaba bien, incluso miramos los resultados de nuestra firma antes de guardar la información en una variable:
```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout' } ]''' outputs='''{ "'$recipient2'": 0.03597 }''')

$ bitcoin-cli -named signrawtransaction hexstring=$rawtxhex | jq -r '.hex'
02000000012b137ef780666ba214842ff6ea2c3a0b36711bcaba839c3710f763e3d9687fed000000006a473044022003ca1f6797d781ef121ba7c2d1d41d763a815e9dad52aa8bc5ea61e4d521f68e022036b992e8e6bf2c44748219ca6e0056a88e8250f6fd0794dc69f79a2e8993671601210317b163ab8c8862e09c71767112b828abd3852e315441893fa0f535de4fa39b8dffffffff01905abd07000000001976a91450b1d90a130c4f3f1e5fbfa7320fd36b7265db0488ac00000000

$ signedtx=$(bitcoin-cli -named signrawtransaction hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
6a184a2f07fa30189f4831d6f041d52653a103b3883d2bec2f79187331fd7f0e
```

   4. No es necesario cruzar los dedos. Ha verificado que sus datos son correctos. A partir de este momento, las cosas están fuera de sus manos.
   
Es posible que sus transacciones se realicen rápidamente. Puede que no. Todo depende de si los mineros que están generando aleatoriamente los bloques actuales tienen el parche CPFP o no. Pero le ha dado a sus transacciones la mejor oportunidad.

Eso es realmente todo lo que hay que hacer.

### Tenga Cuidado con los Matices

Aunque CPFP generalmente se describe como un destinatario que usa una nueva transacción para pagar una anterior que no ha sido confirmada, esto tiene sus matices.

Un _remitente_ podría usar CPFP para liberar una transacción si recibiera un cambio. Simplemente usaría ese cambio como su entrada, y el uso resultante de CPFP liberaría toda la transacción. Eso sí, sería mejor que usara RBF siempre que estuviera habilitado, ya que las tarifas totales serían más bajas.

Un _receptor_ podría usar CPFP uncluso si no planeaba gastar el dinero de inmediato, por ejemplo, si le preocupa que los fondos no se reenvíen si la transacción expira. En este caso, simplemente crea una transacción secundaria que envía todo el dinero (menos una tarifa de transacción) a una dirección de cambio. Eso es lo que hicimos en nuestro ejemplo arriba.

## Resumen: Financiación de una Transacción con CPFP

Puede aprovechar los incentivos de CPFP para liberar fondos que le han sido enviados pero que no han sido confirmados. Simplemente use la transacción no confirmada como UTXO y pague una tarifa de transacción superior al promedio.

> :fire: ***¿Cuál es el Poder de CPFP?*** Principalmente, CPFP es útil para que los fondos se despeguen cuando usted es el destinatario y el remitente no está siendo útil por cualquier motivo. No tiene las posibilidades más poderosas de RBF, pero es una forma alternativa de ejercer control sobre una transacción después de que se haya colocado en el mempool, pero antes de que se confirme en un bloque.

## Que Sigue?

Avance a través de "bitcoin-cli" con [Chapter Six: Expanding Bitcoin Transactions with Multisigs](06_0_Expanding_Bitcoin_Transactions_Multisigs.md).
