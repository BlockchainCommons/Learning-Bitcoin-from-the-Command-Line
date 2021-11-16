# 19.3: Creación de un canal lightning

> :information_source: **NOTA:** Esta sección se agregó recientemente al curso y es un borrador inicial que aún puede estar pendiente de revisión. Lector de advertencias.

Ahora comprende los conceptos básicos de su configuración Lightning y, con suerte, ha creado o recibido información sobre un segundo nodo Lightning. Está listo para crear su primer canal Lightning Network. Por supuesto, deberá comprender qué es y cómo se crea utilizando c-lightning.

> :book: ***¿Qué es un canal de lightning?*** Simplemente, un canal de iluminación es un tubo de dinero que permite transferencias de dinero rápidas, baratas y privadas sin enviar transacciones a la cadena de bloques. Más técnicamente, un canal es una transacción de Bitcoin en cadena con firmas múltiples 2 de 2 que establece una relación financiera sin confianza entre dos personas o dos agentes. Se deposita una cierta cantidad de dinero en el canal, cuando luego se mantiene una base de datos local con saldo de bitcoins para ambas partes, haciendo un seguimiento de cuánto dinero tiene cada uno de la cantidad inicial. Los dos usuarios pueden intercambiar bitcoins a través de su canal Lightning sin tener que escribir en la cadena de bloques de Bitcoin. Solo cuando quieren cerrar su canal, liquidan sus bitcoins en la cadena de bloques, en función de la división final de monedas.

> :book: ***¿Cómo crean los canales Lightning una red Lightning?*** Aunque un canal Lightning solo permite el pago entre dos usuarios, los canales se pueden conectar entre sí para formar una red que permite pagos entre miembros que no tienen canal directo entre ellos. Esto crea una red entre varias personas construida a partir de conexiones por pares.

En esta sección, continuaremos usando nuestra configuración de c-lightning como nuestro nodo principal.

## Crear un canal

La creación de un canal Lightning requiere los siguientes pasos:

* Financie su billetera c-lightning con algunos satoshis.
* Conéctese a un nodo remoto como un par.
* Abre un canal.

### Financiar su billetera c-lightning

Para mover fondos a un canal Lightning, primero se requiere financiar su billetera c-lightning.

> :book: ***¿Qué es una billetera c-lightning?*** La implementación estándar de C-lightning viene con una billetera Bitcoin integrada que le permite enviar y recibir transacciones bitcoin en cadena. Esta billetera se utilizará para crear nuevos canales.

Lo primero que debe hacer es enviar algunos satoshis a su billetera c-lightning. Puede crear una nueva dirección usando el comando `lightning-cli newaddr`. Esto genera una nueva dirección que posteriormente se puede utilizar para financiar canales gestionados por el nodo c-lightning. Puede especificar el tipo de dirección deseada; si no se especifica, la dirección generada será un bech32.

```
$ lightning-cli --testnet newaddr
{
   "address": "tb1qefule33u7ukfuzkmxpz02kwejl8j8dt5jpgtu6",
   "bech32": "tb1qefule33u7ukfuzkmxpz02kwejl8j8dt5jpgtu6"
}
```  

Luego puede enviar fondos a esta dirección usando `bitcoin-cli sendtoaddress` (o cualquier otra metodología que prefiera). Para este ejemplo, lo hicimos en la transacción [11094bb9ac29ce5af9f1e5a0e4aac2066ae132f25b72bff90fcddf64bf2feb02](https://blockstream.info/testnet/tx/11094bb9ac29ce5af9f1e5a0e4b2ac2066ae2f25b72bff90fcddf64bf2feb02)

Esta transacción se denomina [transacción de financiación](https://github.com/lightningnetwork/lightning-rfc/blob/master/03-transactions.md#funding-transaction-output) y debe confirmarse antes de que los fondos puedan ser usado.

> :book: ***¿Qué es una transacción de financiación?*** Una transacción de financiación es una transacción de Bitcoin que coloca dinero en un canal Lightning. Puede ser de financiación única (por un participante) o de doble financiación (por ambos). A partir de ahí, las transacciones Lightning se tratan de reasignar la propiedad de la transacción de financiación, pero solo se liquidan en la cadena de bloques cuando el canal está cerrado.

Para verificar su saldo local debe usar el comando `lightning-cli listfunds`:

```       
c$ lightning-cli --testnet listfunds
{
   "outputs": [],
   "channels": []
}
```     

Dado que los fondos aún no tienen seis confirmaciones, no hay saldo disponible. Después de seis confirmaciones, debería ver un saldo:

```       
c$ lightning-cli --testnet listfunds
{
   "outputs": [
      {
         "txid": "11094bb9ac29ce5af9f1e5a0e4aac2066ae132f25b72bff90fcddf64bf2feb02",
         "output": 0,
         "value": 300000,
         "amount_msat": "300000000msat",
         "scriptpubkey": "0014ca79fcc63cf72c9e0adb3044f559d997cf23b574",
         "address": "tb1qefule33u7ukfuzkmxpz02kwejl8j8dt5jpgtu6",
         "status": "confirmed",
         "blockheight": 1780680,
         "reserved": false
      }
   ],
   "channels": []
}

```     
Tenga en cuenta que el valor aparece en satoshis o microsatoshis, ¡no en Bitcoin!

> :book: ***¿Qué son los satoshis y msat?*** Ya conoció los satoshis en [§3.4](03_4_Recibiendo_una_Transaccion.md). Un satoshi es la cien millonésima parte de un bitcoin, por lo que 300.000 satoshi = 0,003 BTC. Un satoshi es la unidad monetaria más pequeña de la red Bitcoin. Pero, la red Lightning puede ser más pequeña, por lo que 1000 msat, o milisatoshis, equivalen a un satoshi. Eso significa que 1 msat es la cien mil millonésima parte de un bitcoin y 300.000.000 msat = 0,003 BTC.

Ahora que ha financiado su billetera c-lightning, necesitará información sobre un nodo remoto para comenzar a crear el proceso de canal.

### Conectarse a un nodo remoto

Lo siguiente que debe hacer es conectar su nodo a un par. Esto se hace con el comando `lightning-cli connect`. Recuerde que si desea obtener más información sobre este comando, debe escribir `lightning-cli help connect`.

Para conectar su nodo a un par remoto, necesita su ID, que representa la clave pública del nodo de destino. Para su comodidad, `id` puede tener el formato `id@host` o `id@host:port`. Es posible que haya recuperado esto con `lightning-cli getinfo` (en c-lightning) o `lncli --network=testnet getinfo` (en LND) como se discutió en el [interludio anterior](19_2_Interludio_Accediendo_a_un_Segundo_Nodo_Lightning.md).

Hemos seleccionado el nodo LND, `032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543`, que se encuentra en la dirección IP `45.33.35.151`, a la que nos vamos a conectar desde nuestro nodo c-lightning:

```       
$ lightning-cli --network=testnet connect 032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543@45.33.35.151
{
   "id": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
   "features": "02a2a1"
}
```     

### Abrir un canal

El comando fundchannel RPC abre un canal de pago con un par al realizar una transacción de financiación en la cadena de bloques. Debe usar el comando `lightning-cli fundchannel` para hacerlo, con los siguientes parámetros:

* **id** es el retorno de la identificación del par de connect.
* **monto** es el monto en satoshis que se toma de la billetera interna para financiar el canal. El valor no puede ser menor que el límite de polvo, actualmente establecido en 546, ni más de 16.777.215 satoshi (a menos que se hayan negociado grandes canales con el par).
* **tarifa** es una tarifa opcional utilizada para la transacción de apertura y como tarifa inicial para transacciones de compromiso y HTLC.
* **anuncio** es una bandera opcional que activa si anunciar este canal o no. Por defecto es verdadero. Si desea crear un canal privado no anunciado, configúrelo en falso.
* **minconf** especifica el número mínimo de confirmaciones que deben tener las salidas utilizadas en el proceso de apertura del canal. El valor predeterminado es 1.
* **utxos** especifica los utxos que se utilizarán para financiar el canal, como una matriz de "txid: vout".

Ahora puede abrir el canal así:

```
$ lightning-cli --testnet fundchannel 032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543 100000 urgent true 1
{
   "tx": "0200000000010193dc3337837f091718f47b71f2eae8b745ec307231471f6a6aab953c3ea0e3b50100000000fdffffff02a0860100000000002200202e30365fe321a435e5f66962492163302f118c13e215ea8928de88cc46666c1d07860100000000001600142fe02e5be9283e8c5bcb93ae61421baf8cb64f9c024730440220668a7c253c9fd83fc1b45e4a52823fb6bc5fad30da36240d4604f0d6981a6f4502202aeb1da5fbbc8790791ef72b3378005fe98d485d22ffeb35e54a6fbc73178fb2012103b3efe051712e9fa6d90008186e96320491cfe1ef1922d74af5bc6d3307843327c76c1c00",
   "txid": "66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d",
   "channel_id": "1d3cf2126ae36e12be3aee893b385ed6a2e19b1da7f4e579e3ef15ca234d6966",
   "outnum": 0
}
```
Para confirmar el estado del canal, use el comando `lightning-cli listfunds`:


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
         "status": "unconfirmed",
         "reserved": false
      },
      {
         "txid": "b5e3a03e3c95ab6a6a1f47317230ec45b7e8eaf2717bf41817097f833733dc93",
         "output": 1,
         "value": 200000,
         "amount_msat": "200000000msat",
         "scriptpubkey": "0014ed54b65eae3da99b23a48bf8827c9acd78079469",
         "address": "tb1qa42tvh4w8k5ekgay30ugyly6e4uq09rfpqf9md",
         "status": "confirmed",
         "blockheight": 1862831,
         "reserved": true
      }
   ],
   "channels": [
      {
         "peer_id": "032a7572dc013b6382cde391d79f292ced27305aa4162ec3906279fc4334602543",
         "connected": true,
         "state": "CHANNELD_AWAITING_LOCKIN",
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

Si bien este nuevo canal con 100.000 satoshis no está confirmado, su estado será `CHANNELD_AWAITING_LOCKIN`. Tenga en cuenta que el cambio no confirmado de satoshis `99847` también se muestra como una nueva transacción en la billetera. Una vez completadas las seis confirmaciones, el canal cambiará al estado `CHANNELD_NORMAL`, que será su estado permanente. En este momento, también aparecerá un `short_channel_id`, como:

```
"short_channel_id": "1862856x29x0",
```

Estos valores indican dónde se puede encontrar la transacción de financiación en la cadena de bloques. Aparece en la forma `block x txid x vout`.

En este caso, "1862856x29x0" significa:

* Creado en el bloque 1862856th;
* con un `txid` de 29; y
* un `vout` de 0.

Es posible que deba usar este `short_channel_id` para ciertos comandos en Lightning.

Esta transacción de financiación también se puede encontrar en la cadena en [66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d](https://blockstream.info/testnet/tx/66694d23ca15efe379e5f4a71d9be1a2d65e383b89ee3abe126ee36a12f23c1d)

> :book: ***¿Qué es la capacidad del canal?*** En un canal Lightning, ambos lados del canal poseen una parte de su capacidad. La cantidad de su lado del canal se llama *saldo local* y la cantidad de su lado se llama *saldo remoto*. Ambos saldos se pueden actualizar muchas veces sin cerrar el canal (cuando el saldo final se envía a la cadena de bloques), pero la capacidad del canal no puede cambiar sin cerrarlo o empalmarlo. La capacidad total de un canal es la suma del saldo que tiene cada participante en el canal.

## Resumen: configuración de un canal

Necesita crear un canal con un nodo remoto para poder recibir y enviar dinero a través de Lightning Network.

## ¿Que sigue?

Usted está listo para continuar! Continúe con el [Capítulo veinte Uso de Lightning](20_0_Usando_Lightning.md).
