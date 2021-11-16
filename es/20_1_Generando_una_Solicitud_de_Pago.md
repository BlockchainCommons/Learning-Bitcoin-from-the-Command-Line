
# 20.1: Generación de una Solicitud de Pago

> :information_source: **NOTA:** Esta sección se ha agregado recientemente al curso y es un borrador inicial que aún puede estar pendiente de revisión. Lector de advertencias.

Esta sección describe cómo funcionan los pagos en Lightning Network, cómo crear una solicitud de pago (o _factura_), y, finalmente, cómo entenderla. Le emisión de facturas depende de que tenga un segundo nodo Lightning, como se describe en [Acceso a un Segundo Nodo Lightning](19_2_Interludio_Accediendo_a_un_Segundo_Nodo_Lightning.md). Estos ejemplos usarán un nodo LND como su nodo secundario, para demostrar aún más las posibilidades de Lightning Network. Para diferenciar entre los nodos en estos ejemplos, las solicitudes se mostrarán como `c$` para el nodo c-lightning y `lnd$` como el nodo LND. Si desea reproducir estos pasos, debe [instalar su propio nodo LND secundario](19_2_Interludio_Accediendo_a_un_Segundo_Nodo_Lightning.md#Crear-un-nuevo-nodo-LND). 

> :book: ***¿Qué es una Factura?** Casi todos los pagos realizados en Lightning Network requieren una factura, que no es más que una **solicitud de pago** realizada por el destinatario del dinero y enviada por una variedad de medios al usario que paga. Todas las solicitudes de pago son de un solo uso. Las facturas Lightning utilizan la codificación bech32, que ya utiliza Segregated Witness para Bitcoin.

## Crear una Factura

Para crear una nueva factura en c-lightning, usaría el comando `lightning-cli --testnet invoice`.

Así es como funcionaría con c-lightning, usando argumentos de una cantidad (en millisatoshis), una etiqueta y una descripción.
```
c$ lightning-cli --testnet invoice 100000 joe-payment "The money you owe me for dinner"
{
   "payment_hash": "07a1c4bd7a38b4dea35f301c173cd8f9aac253b66bd8404d7ad829f226342490",
   "expires_at": 1603305795,
   "bolt11": "lntb1u1p0cw3krpp5q7suf0t68z6dag6lxqwpw0xclx4vy5akd0vyqnt6mq5lyf35yjgqdpj235x2grddahx27fq09hh2gr0wajjqmt9ypnx7u3qv35kumn9wgxqyjw5qcqp2sp5r3puay46tffdyzldjv39fw6tzdgu2hnlszamqhnmgjsuxqxavpgs9qy9qsqatawvx44x5qa22m7td84jau5450v7j6sl5224tlv9k5v7wdygq9qr4drz795lfnl52gklvyvnha5e5lx72lzzmgzcfnp942va5thmhsp5sx7c2",
   "warning_capacity": "No channels",
   "warning_mpp_capacity": "The total incoming capacity is still insufficient even if the payer had MPP capability."
}
```
Sin embargo, para este ejemplo, vamos a generar una factura en un nodo LND y luego pagarla en el nodo c-lightning. Esto requiere el comando `addinvoice` ligeramente diferente de LND.  Puede usar el argumento `--amt` para indicar la cantidad a pagar (en millisatoshis) y agregar una descripción usando el argumento `--memo`.

```
lnd$ lncli -n testnet addinvoice --amt 10000 --memo "First LN Payment - Learning Bitcoin and Lightning from the Command line."
{
    "r_hash": "6cacdedc95b89eec15e5244bd0957b88c0ab58b153eee549735b995344bc16bb",
    "payment_request": "lntb100u1p0cwnqtpp5djkdahy4hz0wc909y39ap9tm3rq2kk9320hw2jtntwv4x39uz6asdr5ge5hyum5ypxyugzsv9uk6etwwssz6gzvv4shymnfdenjqsnfw33k76twypskuepqf35kw6r5de5kueeqveex7mfqw35x2gzrdakk6ctwvssxc6twv5hqcqzpgsp5a9ryqw7t23myn9psd36ra5alzvp6lzhxua58609teslwqmdljpxs9qy9qsq9ee7h500jazef6c306psr0ncru469zgyr2m2h32c6ser28vrvh5j4q23c073xsvmjwgv9wtk2q7j6pj09fn53v2vkrdkgsjv7njh9aqqtjn3vd",
    "add_index": "1"
}
```
Tenga en cuenta que estas facturas no hacen referencia directamente al canal que creó: es necesario para el pago, pero no para solicitar el pago.

## Entender una Factura

El `bolt11` `payment_request` que ha creado se compone de dos partes: una es legible por humanos y la otra son datos.

> :book: **Que es un BOLT?** Los BOLTs son las individuales [especificaciones de Lightning Network](https://github.com/lightningnetwork/lightning-rfc).

### Lea la Parte de la Factura Legible por Humanos

La parte legible por humanos de sus facturas comienza con un `ln`. Es `lnbc` para Bitcoin mainnet, `lntb` para Bitcoin testnet, o `lnbcrt` para Bitcoin regtest.
Luego enumera los fondos solicitados en la factura.

Por ejemplo, mire su factura desde su nodo LND:
```
lntb100u1p0cwnqtpp5djkdahy4hz0wc909y39ap9tm3rq2kk9320hw2jtntwv4x39uz6asdr5ge5hyum5ypxyugzsv9uk6etwwssz6gzvv4shymnfdenjqsnfw33k76twypskuepqf35kw6r5de5kueeqveex7mfqw35x2gzrdakk6ctwvssxc6twv5hqcqzpgsp5a9ryqw7t23myn9psd36ra5alzvp6lzhxua58609teslwqmdljpxs9qy9qsq9ee7h500jazef6c306psr0ncru469zgyr2m2h32c6ser28vrvh5j4q23c073xsvmjwgv9wtk2q7j6pj09fn53v2vkrdkgsjv7njh9aqqtjn3vd
```
La parte legible por humanos `ln` + `tb` + `100u`.

`lntb` dice que esta es una factura de Lightning Network para los bitcoins de Testnet.

`100u` dice que es por 100 bitcoins multiplicado por el multiplicador de microsatoshi. Hay cuatro multiplicadores de fondos (opcionales):

* `m` (milli): multiplicar por 0.001
* `u` (micro): multiplicar por 0.000001
* `n` (nano): multiplicar por 0.000000001
* `p` (pico): multiplicar por 0.000000000001

100 BTC * .000001 = .0001 BTC, que es lo mismo que 10,000 satoshis.

### Leer la Parte de la Factura de Datos

El resto de la factura (`1p0cwnqtpp5djkdahy4hz0wc909y39ap9tm3rq2kk9320hw2jtntwv4x39uz6asdr5ge5hyum5ypxyugzsv9uk6etwwssz6gzvv4shymnfdenjqsnfw33k76twypskuepqf35kw6r5de5kueeqveex7mfqw35x2gzrdakk6ctwvssxc6twv5hqcqzpgsp5a9ryqw7t23myn9psd36ra5alzvp6lzhxua58609teslwqmdljpxs9qy9qsq9ee7h500jazef6c306psr0ncru469zgyr2m2h32c6ser28vrvh5j4q23c073xsvmjwgv9wtk2q7j6pj09fn53v2vkrdkgsjv7njh9aqqtjn3vd`) contiene una marca de tiempo, datos etiquetados específicamente y una firma. Obviamente, no puede leerlo usted mismo, pero puede pedirle a `lightning-cli` de c-lightning que lo haga con el comando `decodepay`:
```
c$ lightning-cli --testnet decodepay lntb100u1p0cwnqtpp5djkdahy4hz0wc909y39ap9tm3rq2kk9320hw2jtntwv4x39uz6asdr5ge5hyum5ypxyugzsv9uk6etwwssz6gzvv4shymnfdenjqsnfw33k76twypskuepqf35kw6r5de5kueeqveex7mfqw35x2gzrdakk6ctwvssxc6twv5hqcqzpgsp5a9ryqw7t23myn9psd36ra5alzvp6lzhxua58609teslwqmdljpxs9qy9qsq9ee7h500jazef6c306psr0ncru469zgyr2m2h32c6ser28vrvh5j4q23c073xsvmjwgv9wtk2q7j6pj09fn53v2vkrdkgsjv7njh9aqqtjn3vd
{
   "currency": "tb",
   "created_at": 1602702347,
   "expiry": 3600,
   "payee": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
   "msatoshi": 10000000,
   "amount_msat": "10000000msat",
   "description": "First LN Payment - Learning Bitcoin and Lightning from the Command line.",
   "min_final_cltv_expiry": 40,
   "payment_secret": "e946403bcb54764994306c743ed3bf1303af8ae6e7687d3cabcc3ee06dbf904d",
   "features": "028200",
   "payment_hash": "6cacdedc95b89eec15e5244bd0957b88c0ab58b153eee549735b995344bc16bb",
   "signature": "304402202e73ebd1ef974594eb117e8301be781f2ba289041ab6abc558d432351d8365e902202a8151c3fd13419b9390c2b976503d2d064f2a6748b14cb0db64424cf4e572f4"
}

```
Esto es lo que significan los elementos más relevantes:

1. `currency`: La moneda que se paga.
2. `created_at`: Hora en que se creó la factura. Esto se mide en tiempo UNIX, que son segundos desde 1970.
3. `expiry`: El momento en que su nodo marca la factura como inválida. El valor predeterminado es 1 hora o 3600 segundos.
4. `payee`: La clave pública de la persona (nodo) que recibe el pago de Lightning Network.
5. `msatoshi` y `amount_msat`: la cantidad de satoshis a pagar.
6. `description`: La descripción de entrada del usuario.
7. `payment_hash`: El hash de la preimagen que se usa para bloquear el pago. Solo puede canjear un pago bloqueado con la preimagen correspondiente al hash de pago. Esto permite el enrutamiento en Lightning Network, sin confiar en terceros, al crear un **Pago Condicional** para completar.
8. `signature`: La firma codificada en DER.

> :book: ****¿Qué son los Pagos Condicionales?*** Aunque los Canales Lightning se crean entre dos participantes, se pueden conectar múltiples canales entre sí, formando una red de pago que permite pagos entre todos los participantes de la red, incluso aquellos sin un canal directo entre ellos. Esto se hace mediante un contrato intelligente llamado **Hashed Time Locked Contract** o contrato bloqueado por tiempo codificado.

> :book: ***¿Qué es un Contrato Bloqueado por Tiempo Codificado / Hashed Time Locked Contract (HTLC)?*** Un HTLC es un pago condicional que utiliza bloqueos de codificado y bloqueos de tiempo para garantizar la seguridad del pago. El receptor debe presentar una preimagen de pago o generar un comprobante de pago criptográfico antes de un tiempo determinado, de lo contrario el pagador puede cancelar el contrato gastándolo. Estos contratos se crean como resultados de la **Transacción de Compromiso**.

> :book: ***¿Qué es una Transacción de Compromiso?*** Una transacción de compromiso es una transacción que gasta la transacción de financiacción original. Cada par tiene la firma del otro par, lo que significa que cualquiera puede gastar su transacción de compromiso cuando quiera. Después de que se crea cada nueva transacción de compromiso, se revoca la anterior. La transacción de compromiso es una forma en que la transacción de financiación se puede desbloquear en la cadena de bloques, como se analiza en [§20.3](20_3_Cerrando_un_Canal_Lightning.md).

### Verifique su Factura

Hay dos elementos cruciales para verificar la factura. El primero, obviamente, es el monto del pago, que ya examinó en la parte legible por humanos. El segundo es el valor `payee` (beneficiario) que es la clave pública del destinatario (nodo):
```
   "payee": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
```
Debe comprobar que ese es el destinatario esperado.

Mirando hacia atrás en [§19.3](19_3_Creando_un_Canal_en_Lightning.md#opening-a-channel), puede ver que ese es efectivamente el ID de par que usó cuando creó su canal. También puede verificarlo en el nodo opuesto con el comando `getinfo`.
```
lnd$ lncli -n testnet getinfo
{
    "version": "0.11.0-beta.rc4 commit=v0.11.0-beta.rc4",
    "commit_hash": "fc12656a1a62e5d69430bba6e4feb8cfbaf21542",
    "identity_pubkey": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
    "alias": "StandUp",
    "color": "#3399ff",
    "num_pending_channels": 0,
    "num_active_channels": 1,
    "num_inactive_channels": 0,
    "num_peers": 3,
    "block_height": 1862983,
    "block_hash": "00000000000000c8c2f58f6da2ae2a3884d6e84f55d0e1f585a366f9dfcaa860",
    "best_header_timestamp": "1602702331",
    "synced_to_chain": true,
    "synced_to_graph": true,
    "testnet": true,
    "chains": [
        {
            "chain": "bitcoin",
            "network": "testnet"
        }
    ],
    "uris": [
    ],
    "features": {
        "0": {
            "name": "data-loss-protect",
            "is_required": true,
            "is_known": true
        },
        "5": {
            "name": "upfront-shutdown-script",
            "is_required": false,
            "is_known": true
        },
        "7": {
            "name": "gossip-queries",
            "is_required": false,
            "is_known": true
        },
        "9": {
            "name": "tlv-onion",
            "is_required": false,
            "is_known": true
        },
        "13": {
            "name": "static-remote-key",
            "is_required": false,
            "is_known": true
        },
        "15": {
            "name": "payment-addr",
            "is_required": false,
            "is_known": true
        },
        "17": {
            "name": "multi-path-payments",
            "is_required": false,
            "is_known": true
        }
    }
}
```
Sin embargo, el `payee` (beneficario) también puede ser alguien nuevo, en cuyo caso probablemente deberá consultar con el sitio web o la persona que emitió la factura para asegurarse de que sea correcta.

## Resumen: Generación de una Solicitud de Pago

En la mayoría de los casos, debe recibir una factura para utilizar los pagos de Lightning Network. En este ejemplo, creamos uno manualmente, pero si tuviera un entorno de producción, es probable que los sistemas lo hagan automáticamente cada vez que alguien compra productos o servicios. Por supuesto, una vez que haya recibido una factura, debe comprender cómo leerla!

## Que Sigue?

Continúe "Usando Lightning" con [§20.2: Pagando una Factura](20_2_Pagando_una_Factura.md).
