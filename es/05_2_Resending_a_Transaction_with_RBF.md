# 5.2: Reenviando una Transacción con RBF

Si su transacción de Bitcoin está atascada y usted es el remitente, puede reenviarla usando RBF (replace by fee / reemplazor por tarifa). Sin embargo, eso no es todo lo que puede hacer RBF: generalmente es una característica poderosa y multipropósito que permite a los remitentes de Bitcoin recrear transacciones por una variedad de razones.

> :warning: **ADVERTENCIA DE VERSIÓN:** Esta es una innovación de Bitcoin Core v 0.12.0, que alcanzó la madurez completa en la billetera Bitcoin Core con Bitcoin Core v 0.14.0. Obviamente, la mayoría de la gente ya debería estar utilizándolo.

## Optar en RBF

RBF es una función de Bitcoin opcional. Las transacciones solo son elegibles para usar RBF si se han creado con una marca de RBF especial. Esto se hace configurando cualquiera de los números de secuencia UTXO de la transacción (que generalmente se configuran automáticamente), de modo que sea más de 0 y menos de 0xffffffff-1 (4294967294).

Esto se logra simplemente agregando una variable de `sequence` (sequencia) a sus entradas UTXO:
```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "sequence": 1 } ]''' outputs='''{ "'$recipient'": 0.00007658, "'$changeaddress'": 0.00000001 }''')
```
Por supuesto, debe firmar y enviar su transacción como de costumbre:
```
$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375
```
Ahora, cuando mire a su transacción, debería ver algo nuevo: la línea `bip125-replaceable`, que siempre ha sido marcada como `no` antes, ahora está marcada como `yes` (sí):
```
$ bitcoin-cli -named gettransaction txid=5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375                                                                     
      
{
  "amount": 0.00000000,
  "fee": -0.00000141,
  "confirmations": 0,
  "trusted": true,
  "txid": "5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375",
  "walletconflicts": [
  ],
  "time": 1592954399,
  "timereceived": 1592954399,
  "bip125-replaceable": "yes",
  "details": [
  ],
  "hex": "02000000000101fa364ad3cbdb08dd0b83aac009a42a9ed00594acd6883d2a466699996cd69d8b01000000000100000002ea1d000000000000160014d591091b8074a2375ed9985a9c4b18efecfd416501000000000000001600146c45d3afa8762086c4bd76d8a71ac7c976e1919602473044022077007dff4df9ce75430e3065c82321dca9f6bdcfd5812f8dc0daeb957d3dfd1602203a624d4e9720a06def613eeea67fbf13ce1fb6188d3b7e780ce6e40e859f275d0121038a2702938e548eaec28feb92c7e4722042cfd1ea16bec9fc274640dc5be05ec500000000"
}
```
La bandera `bip125-replaceable` permanecerá `yes` hasta que la transacción reciba confirmaciones. En ese momento, ya no es reemplazable.

> :book: ***¿Debo confiar en transacciones sin confirmaciones?*** No, nunca. Esto era cierto antes de RBF y fue cierto después de RBF. Las transacciones deben recibir confirmaciones antes de que sean confiables. Esto es especialmente cierto si una transacción está marcada como `bip125-replaceable`, porque entonces puede ser ... reemplazada.

> :information_source: **NOTA — SECUENCIA:** Este es el primer uso del valor `nSequence` en Bitcoin. Puede configurarlo entre 1 y 0xffffffff-2 (4294967293) y habilitar RBF, pero si no tiene cuidado, puede enfrentarse al uso paralelo de `nSequence` para bloqueos temporales relativos. :t Sugerimos configurarlo siempre en "1", que es lo que hace Bitcoin Core, pero la otra opción es configurarlo en un valor entre 0xf0000000 (4026531840) y 0xffffffff-2 (4294967293). Establecerlo en "1" efectivamente hace que los bloqueos temporales relativos sean irrelevantes y establecerlo en 0xf0000000 o superior los desactiva. Todo esto se explica con más detalle en [§11.3: Using CSV in Scripts](11_3_Using_CSV_in_Scripts.md). Por ahora, simplemente elija uno de los valores no conflictivos para `nSequence`.

### Opcional: Optar Siempre por RBF

Si lo prefiere, puede _siempre_ optar por RBF. Hágalo ejecutando su `bitcoind` con el comando `-walletrbf`. Cuando ya hecho esto (y reiniciado su `bitcoind`), todos los UTXOs deberían tener un número de secuencia más bajo y la transacción debería estar marcada como `bip125-replaceable`.

> :warning: **ADVERTENCIA DE VERSIÓN:** La bandera walletrbf requiere Bitcoin Core v.0.14.0.

## Comprender Cómo Funciona RBF

La funcionalidad RBF se basa en [BIP 125](https://github.com/bitcoin/bips/blob/master/bip-0125.mediawiki), que enumera las siguientes reglas para usar RBF:

> 1. Las transacciones originales señalan la reemplazabilidad explícitamente o mediante herencia, como se describe en la sección Resumen anterior.

Esto significa que el número de secuencia debe establecerse en menos de 0xffffffff-1. (4294967294), o lo mismo ocurre con las transacciones matrices no confirmada.

> 2. La transacción de reemplazo paga una tarifa absolutamente más alta que la suma pagada por las transacciones originales.
> 3. La transacción de reemplazo no contiene ninguna entrada nueva no confirmada que no apareciera previamente en el mempool. (Las entradas no confirmadas son entradas que gastan salidas de transacciones actualente no confirmadas.)
> 4. La transacción de reemplazo debe pagar por su propio ancho de banda además del monto pagado por las transacciones originales a la tasa establecida por la configuración de tarifa de retransmisión mínima del nodo por encima de ella. Por ejemplo, si la tarifa mínima de retransmisión es 1 satoshi/byte y la transacción de reemplazo es de 500 bytes en total, entonces el reemplazo debe pagar una tarifa al menos 500 satoshis más alta que la suma de los originales.
> 5. El número de transacciones originales a reemplazar y sus transacciones descendientes que serán desalojadas del mempool no debe exceder un total de 100 transacciones.

> :book: ***Que es un BIP?*** Un BIP es una Bitcoin Improvement Proposal, una propuesta de mejora de Bitcoin. Es una sugerencia en profundidad para un cambio en el código de Bitcoin Core. A menudo, cuando un BIP se ha discutido y actualizado lo suficiente se convertirá en una parte real del código de Bitcoin Core. Por ejemplo, BIP 125 se implementó en Bitcoin Core 0.12.0.

La otra cosa que hay que entender sobre RBF es que para usarlo, debe gastar dos veces, reutilizando uno o más UTXO iguales. Simplemente enviar otra transacción con un UTXO diferente al mismo destinatario no funcionará (y probablemente resultará en una pérdida de dinero). En su lugar, debe crear un conflicto a propósito, donde se usa el mismo UTXO en dos transacciones diferentes.

Ante este conflicto, los mineros sabrán usar el que tenga la tarifa más alta, y estarán incentivados a hacerlo por esa tarifa más alta.

> :book: ***¿Qué es un doble-gasto?*** Un doble-gasto ocurre cuando alguien envía los mismos fondos electrónicos a dos personas diferentes (o, a la misma persona dos veces, en dos transacciones diferentes). Este es un problema central para cualquier sistema de efectivo electrónico. Se resuelve en Bitcoin por el libro mayor inmutable: una vez que una transacción está suficientemente confirmada, ningún minero verificará transacciones que reutilicen el mismo UTXO. Sin embargo, es posible gastar dos veces _antes_ de que se confirme una transacción, por lo que siempre desea una o más confirmaciones antes de finalizar una transacción. En el caso de RBF, usted gasta el doble a propósito porque una transacción inicial se ha estancado y los mineros aceptan su doble gasto si cumple con los criterios específicos establecidos por BIP 125.

> :warning: **ADVERTENCIA:** Algunas discusiones tempranas de esta política sugirieron que el número de `nSequence` también se incrementara. De hecho, este fue el uso previsto de `nSequence` en su forma original. Esto _no_ forma parte de la política publicada en BIP 125. De hecho, aumentar su número de secuencia puede bloquear accidentalmente su transacción con un bloqueo temporal relativo, a menos que use números de secuencia en el rango de  0xf0000000 (4026531840) a 0xffffffff-2 (4294967293).

## Reemplazar una Transacción de la Manera Difícil: A Mano

Para crear una transacción RBF a mano, todo lo que tiene que hacer es crear una transacción sin procesar que: (1) reemplaza una transacción sin procesar anterior que optó por RBF y que no está confirmada; (2) reutiliza una o más de las mismas UTXOs; (3) aumenta las tarifas; y (4) paga el ancho de banda mínimo de ambas transacciones [que ya puede ser antendido por (3)].

El siguiente ejemplo solo reutiliza nuestras variables existentes, pero disminuye la cantidad enviada a la dirección de cambio, para aumentar la tarifa de los 0 BTC accidentales de la transacción original a 0.01 BTC excesivamente generosos en la nueva transacción:
```
$ rawtxhex=$(bitcoin-cli -named createrawtransaction inputs='''[ { "txid": "'$utxo_txid'", "vout": '$utxo_vout', "sequence": 1 } ]''' outputs='''{ "'$recipient'": 0.000075, "'$changeaddress'": 0.00000001 }''')
```
Por supuesto, debemos volver a firmarlo y reenviarlo:
```
$ signedtx=$(bitcoin-cli -named signrawtransactionwithwallet hexstring=$rawtxhex | jq -r '.hex')
$ bitcoin-cli -named sendrawtransaction hexstring=$signedtx
c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b
```
Ahora puede ver su transacción original y ver que tiene `walletconflicts`, o confilctos de billetera:
```
$ bitcoin-cli -named gettransaction txid=5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375
{
  "amount": 0.00000000,
  "fee": -0.00000141,
  "confirmations": 0,
  "trusted": false,
  "txid": "5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375",
  "walletconflicts": [
    "c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b"
  ],
  "time": 1592954399,
  "timereceived": 1592954399,
  "bip125-replaceable": "yes",
  "details": [
  ],
  "hex": "02000000000101fa364ad3cbdb08dd0b83aac009a42a9ed00594acd6883d2a466699996cd69d8b01000000000100000002ea1d000000000000160014d591091b8074a2375ed9985a9c4b18efecfd416501000000000000001600146c45d3afa8762086c4bd76d8a71ac7c976e1919602473044022077007dff4df9ce75430e3065c82321dca9f6bdcfd5812f8dc0daeb957d3dfd1602203a624d4e9720a06def613eeea67fbf13ce1fb6188d3b7e780ce6e40e859f275d0121038a2702938e548eaec28feb92c7e4722042cfd1ea16bec9fc274640dc5be05ec500000000"
}
```
Esto representa el hecho de que dos transacciones diferentes intentan utilizar el mismo UTXO. 

Eventualmente, se debe aceptar la transacción con la tarifa más alta:
```
$ bitcoin-cli -named gettransaction txid=c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b
{
  "amount": 0.00000000,
  "fee": -0.00000299,
  "confirmations": 2,
  "blockhash": "0000000000000055ac4b6578d7ffb83b0eccef383ca74500b00f59ddfaa1acab",
  "blockheight": 1773266,
  "blockindex": 9,
  "blocktime": 1592955002,
  "txid": "c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b",
  "walletconflicts": [
    "5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375"
  ],
  "time": 1592954467,
  "timereceived": 1592954467,
  "bip125-replaceable": "no",
  "details": [
  ],
  "hex": "02000000000101fa364ad3cbdb08dd0b83aac009a42a9ed00594acd6883d2a466699996cd69d8b010000000001000000024c1d000000000000160014d591091b8074a2375ed9985a9c4b18efecfd416501000000000000001600146c45d3afa8762086c4bd76d8a71ac7c976e1919602473044022077dcdd98d85f6247450185c2b918a0f434d9b2e647330d741944ecae60d6ff790220424f85628cebe0ffe9fa11029b8240d08bdbfcc0c11f799483e63b437841b1cd0121038a2702938e548eaec28feb92c7e4722042cfd1ea16bec9fc274640dc5be05ec500000000"
}
```
Mientras tanto, la transacción original con la tarifa más baja comienza a recoger confirmaciones negativas, para mostrar su divergencia con respecto a la cadena de bloques:
```
$ bitcoin-cli -named gettransaction txid=5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375
{
  "amount": 0.00000000,
  "fee": -0.00000141,
  "confirmations": -2,
  "trusted": false,
  "txid": "5b953a0bdfae0d11d20d195ea43ab7c31a5471d2385c258394f3bb9bb3089375",
  "walletconflicts": [
    "c6de60427b28d8ec8102e49771e5d0348fc3ef6a5bf02eb864ec745105a6951b"
  ],
  "time": 1592954399,
  "timereceived": 1592954399,
  "bip125-replaceable": "yes",
  "details": [
  ],
  "hex": "02000000000101fa364ad3cbdb08dd0b83aac009a42a9ed00594acd6883d2a466699996cd69d8b01000000000100000002ea1d000000000000160014d591091b8074a2375ed9985a9c4b18efecfd416501000000000000001600146c45d3afa8762086c4bd76d8a71ac7c976e1919602473044022077007dff4df9ce75430e3065c82321dca9f6bdcfd5812f8dc0daeb957d3dfd1602203a624d4e9720a06def613eeea67fbf13ce1fb6188d3b7e780ce6e40e859f275d0121038a2702938e548eaec28feb92c7e4722042cfd1ea16bec9fc274640dc5be05ec500000000"
}
```
Nuestros destinatarios tienen su dinero, y la transacción fallida original eventualmente se saldrá del mempool.

## Reemplazar una Transacción de la Manera Mas Fácil: Por bumpfee

Las transacciones sin procesar son muy poderosas y puede hacer muchas cosas interesantes combinándolas con RBF. Sim embargo, a veces _todo_ lo que quiere hacer es liberar una transacción que ha estado suspendida. Ahora puede hacerlo con un simple comando, `bumpfee`.

Por ejemplo, para aumentar la tarifa de la transacción `4460175e8276d5a1935f6136e36868a0a3561532d44ddffb09b7cb878f76f927` ejecutaría:
```
$ bitcoin-cli -named bumpfee txid=4460175e8276d5a1935f6136e36868a0a3561532d44ddffb09b7cb878f76f927
{
  "txid": "75208c5c8cbd83081a0085cd050fc7a4064d87c7d73176ad9a7e3aee5e70095f",
  "origfee": 0.00000000,
  "fee": 0.00022600,
  "errors": [
  ]
}
```
El resultado es la generación automática de una nueva transacción que tiene una tarifa determinada por su archivo bitcoin.conf:
```
$ bitcoin-cli -named gettransaction txid=75208c5c8cbd83081a0085cd050fc7a4064d87c7d73176ad9a7e3aee5e70095f
{
  "amount": -0.10000000,
  "fee": -0.00022600,
  "confirmations": 0,
  "trusted": false,
  "txid": "75208c5c8cbd83081a0085cd050fc7a4064d87c7d73176ad9a7e3aee5e70095f",
  "walletconflicts": [
    "4460175e8276d5a1935f6136e36868a0a3561532d44ddffb09b7cb878f76f927"
  ],
  "time": 1491605676,
  "timereceived": 1491605676,
  "bip125-replaceable": "yes",
  "replaces_txid": "4460175e8276d5a1935f6136e36868a0a3561532d44ddffb09b7cb878f76f927",
  "details": [
    {
      "account": "",
      "address": "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi",
      "category": "send",
      "amount": -0.10000000,
      "vout": 0,
      "fee": -0.00022600,
      "abandoned": false
    }
  ],
  "hex": "02000000014e843e22cb8ee522fbf4d8a0967a733685d2ad92697e63f52ce41bec8f7c8ac0020000006b48304502210094e54afafce093008172768d205d99ee2e9681b498326c077f0b6a845d9bbef702206d90256d5a2edee3cab1017b9b1c30b302530b0dd568e4af6f2d35380bbfaa280121029f39b2a19943fadbceb6697dbc859d4a53fcd3f9a8d2c8d523df2037e7c32a71010000000280969800000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac38f25c05000000001976a914c101d8c34de7b8d83b3f8d75416ffaea871d664988ac00000000"
}
```

> :warning: **ADVERTENCIA DE VERSIÓN:** El `bumpfee` por RPC requiere Bitcoin Core v.0.14.0.

## Resumen: Reenvío de una Transacción con RBF

Si una transacción está atascada y no desea esperar a que caduque por completo, si optó por RBF, puede gastarla dos veces con RBF para crear una transacción de reemplazo (o simplemente usar `bumpfee`).

> :fire: ***Cual es el poder de RBF?*** Obviamente, RBF es muy útil si creó una transacción con una tarifa demasiado baja y necesita pasar esos fondos. Sin embargo, la capacidad de reemplazar generalmente las transacciones no confirmadas con las actualizadas tiene más poder que solo eso (y es por eso que es posible que desee continuar usando RBF con transacciones sin procesar, incluso después de la llegada de `bumpfee`). 

> Por ejemplo, puede enviar una transacción y luego, antes de que se confirme, combinarla con una segunda transacción. Esto le permite comprimir múltiples transacciones en una sola, disminuyendo las tarifas generales. También podría ofrecer beneficios a la privacidad. También hay otras razones para usar RBF, para contratos inteligentes o transacciones de corte, como se describe en el [Opt-in RBF FAQ](https://bitcoincore.org/en/faq/optin_rbf/).

## Que Sigue?

Continúe "Controlando las Transacciones de Bitcoin" con [§5.3: Funding a Transaction with CPFP](05_3_Funding_a_Transaction_with_CPFP.md).  
