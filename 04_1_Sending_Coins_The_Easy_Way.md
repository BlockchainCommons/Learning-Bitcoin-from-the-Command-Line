# 4.1: Enviando Monedas de Forma Sencilla

El comando `bitcoin-cli` ofrece tres formas principales de enviar monedas: como un simple comando; como una transacción en crudo; y como una transacción en crudo con cálculo. Cada una tiene sus propias ventajas y desventajas. El primer método para enviar monedas es también el más sencillo.

## Indica la Comisión de la Transacción

Antes de enviar dinero en la red Bitcoin, debe pensar en las comisiones por transacción que va a pagar.

> :libro: ***¿Qué es una comisión por transacción?*** No existe el almuerzo gratis. Los mineros incorporan las transacciones a los bloques porque se les paga para ello. No sólo obtienen el pago de la red por crear el bloque, sino que también les pagan los usuarios por incluir sus transacciones en el. Si no paga una comisión, su transacción puede quedarse atascada... para siempre (o, hasta que se haga con alguno de los trucos de [Capítulo Cinco](05_0_Controlling_Bitcoin_Transactions.md)).

Cuando se utilizan los métodos simples y automatizados para crear transacciones, como se indica aquí y en [§4.5: Enviando Monedas con Transacciones Automatizadas en Crudo](04_5_Sending_Coins_with_Automated_Raw_Transactions.md), Bitcoin calculará las tasas de transacción por usted. Esto se hace utilizando tasas flotantes, donde `bitcoind` observa el tiempo que tardan las transacciones en confirmarse y calcula automáticamente lo que se debe gastar.

Puede controlar esto colocando valores racionales en su fichero `~/.bitcoin/bitcoin.conf`. Los siguientes valores de bajos costes se asegurarían que haya una tasa mínima de 10.000 satoshis de comisión por kByte de data en su trasacción y solicitarían que las tasas flotantes se calcularan con una cantidad atractiva para que la transacción sea procesada en los siguientes seis bloques. 
```
mintxfee=0.0001
txconfirmtarget=6
```
Sin embargo, bajo la teoría de que usted no va esperar mientras trabaja con este tutorial, hemos adoptado los siguientes valores más altos. 
```
mintxfee=0.001
txconfirmtarget=1
```
Deberá introducir esto en `~/.bitcoin/bitcoin.conf`, en la sección principal, en la parte superior del archivo o si quiere estar seguro de que nunca se usará en otra parte, bajo la sección `[test]`.

Para llevar a cabo este tutorial, estamos dispuestos a gastar 100,00 satoshis por kB en cada transacción (Unos $10 dólares) ¡Queremos que cada transacción llegue en el siguiente bloque! (Para ponerlo en perspectiva, una transacción típica se ocupa entre 0,25 kB y 1 kB, así que en realidad estará pagando $2,50 dólares en vez $10 dólares... si esto fuera dinero real).

Despues de que haya editado su archivo bitcoin.conf, deberá reiniciar forzosamente el servicio de bitcoind.
```
$ bitcoin-cli stop
$ bitcoind -daemon
```

## Obtener una dirección

Necesita un lugar al que enviar sus monedas. Normalmente, alguien le enviará una dirección y quizás le dé una firma para demostrar que es el propietario de esa dirección. También puede darle un código QR para que lo escanee y no se equivoques al escribir la dirección. En nuestro caso, vamos a enviar monedas a `n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi`, que es una dirección de retorno de un antiguo grifo de Tesetnet.

> :libro: ***¿Qué es un código QR?*** Un código QR es sólo una codificación de una dirección Bitcoin. Muchas billeteras generarán códigos QR para usted, mientras que algunos sitios convertirán de una dirección a un código QR. Obviamente, sólo debe aceptar un código QR de un sitio en el que confíe absolutamente. Un pagador puede utilizar un escáner de código de barras para leer el código QR y luego pagar con él.

## Enviar las monedas

Ahora está listo para enviar algunas monedas. Esto es bastante sencillo a través de la línea de comandos. Sólo tiene que utilizar `bitcoin-cli sendtoaddress [address] [amount]`. Así, para enviar un pocas monedas a la dirección `n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi` sólo se requiere:
```
$ txid=$(bitcoin-cli sendtoaddress n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi 0.001)
$ echo $txid
93250d0cacb0361b8e21030ac65bc4c2159a53de1075425d800b2d7a8ab13ba8
```

> 🙏 Para ayudar a mantener vivos los grifos de testnet, intente utilizar la dirección de retorno del mismo grifo que utilizó en el capítulo anterior sobre la recepción de transacciones.

Asegurese de que la dirección que escribe es a donde quieres que vaya el dinero. Hágalo _Dos_veces. Si se equivocas en Bitcoin, no hay vuelta atrás. 

Recibirá un txid de vuelta cuando utilice este comando.

> ❕ Puede obtener un código de error si no tiene los suficientes fondos en su cartera para enviar la transacción. Dependiendo de su saldo actual `bitcoin-cli getbalance` es posible que tenga que ajustar el importe a enviar para tener en cuenta la cantidad que se envía junto a la comisión de la transacción. Si su saldo actual es de 0,001, entonces podría intentar enviar 0,0001. De mejor forma sería restar de su saldo actual la comisión prevista que aparece en el mensaje de error. Esta es una buena práctica, ya que muchos monederos esperan que calcule el monto + las comisiones a retirar, incluso entre las casas de cambio populares.

El comando `bitcoin-cli` en realidad genera comandos JSON-RPC cuando se está comunicando con bitcoind. Pueden ser muy quisquillosos. Este es un ejemplo: si enumeras la cantidad de bitcoins sin el cero inicial (es decir, ".1" en lugar de "0.1"), entonces bitcoin-cli fallará con un mensaje misterioso.

> :advertencia: **ADVERTENCIA:** El comando `bitcoin-cli` en realidad genera comandos JSON-RPC cuando se está comunicando con bitcoind. Pueden ser muy quisquillosos. Este es un ejemplo: si enumera la cantidad de bitcoins sin el cero inicial (es decir, ".1" en lugar de "0.1"), entonces bitcoin-cli fallará con un mensaje misterioso.

Incluso si eres cuidadoso con las entradas, podras ver "La estimación de la comisión falló. Fallbackfee está deshabilitado". Fundamentalmente, esto significa que su archivo `bitcoind` local no tiene suficiente información para estimar las comisiones. Realmente no debería ver este mensaje si ha esperado a que la blockchain se sincronice y se configure el sistema con Bitcoin Standup. Pero si no está completamente sincronizado, puedes ocurrir esto. También puede ser que no esté usando un `bitcoin.conf` estándar: la entrada `blocksonly=1` hará que su `bitcoind` no pueda estimar las comisiones.

> :advertencia: **ADVERTENCIA:** Incluso si eres cuidadoso con las entradas, podras ver "La estimación de la comisión falló. Fallbackfee está deshabilitado". Fundamentalmente, esto significa que su archivo `bitcoind` local no tiene suficiente información para estimar las comisiones. Realmente no debería ver este mensaje si ha esperado a que la blockchain se sincronice y se configure el sistema con Bitcoin Standup. Pero si no está completamente sincronizado, puedes ocurrir esto. También puede ser que no esté usando un `bitcoin.conf` estándar: la entrada `blocksonly=1` hará que su `bitcoind` no pueda estimar las comisiones.

## Examinando la Transacción

Puede ver la transacción utilizando el id de la transacción:
```
{
  "amount": -0.00100000,
  "fee": -0.00022200,
  "confirmations": 0,
  "trusted": true,
  "txid": "93250d0cacb0361b8e21030ac65bc4c2159a53de1075425d800b2d7a8ab13ba8",
  "walletconflicts": [
  ],
  "time": 1592604194,
  "timereceived": 1592604194,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi",
      "category": "send",
      "amount": -0.00100000,
      "vout": 1,
      "fee": -0.00022200,
      "abandoned": false
    }
  ],
  "hex": "0200000001e982921bb0189afc486e20bb05cc5825c71a0ba8868043ed04ece9ab0cb12a8e010000006a47304402200fc493a01c5c9d9574f7c321cee6880f7f1df847be71039e2d996f7f75c17b3d02203057f5baa48745ba7ab5f1d4eed11585bd8beab838b1ca03a4138516fe52b3b8012102fd5740996d853ea51a6904cf03257fc11204b0179f344c49739ec5b20b39c9bafeffffff02e8640d0000000000160014d37b6ae4a917bcc873f6395741155f565e2dc7c4a0860100000000001976a914e7c1345fc8f87c68170b3aa798a956c2fe6a9eff88ac780b1b00"
}
```
Se puede ver no sólo la cantidad transferida (.001 BTC) sino también la comisión de la transacción (.000222 BTC), que es aproximadamente una cuarta parte de la comisión mínima de 0,001 BTC/kB que se estableció, lo que sugiere que la transacción tenía un tamaño de aproximadamente un cuarto de kB.

Mientras espera a que esta transacción se complete, notará que `bitcoin-cli getbalance` muestra que todo su dinero ha desaparecido (o, al menos, todo su dinero de una única transacción entrante). Del mismo modo, `bitcoin-cli listunspent` mostrará que toda la transacción ha desaparecido, incluso si era más de lo que querías enviar. Hay una razón para esto: cada vez que ingresa dinero, tendra que enviarlo _todo_ junto, y si quieres conversar algo, tendra que hacer algunos malabares. Una vez más, `sendtoaddress` se encarga de todo esto por usted, lo que significa que no tiene que preocuparse de hacer el cambio hasta que envíe una transacción en crudo. En este caso, aparecerá una nueva transacción con el cambio cuando el gasto se incorpore a un bloque.

## Resumen: Enviando Monedas de Forma Sencilla

Para enviar monedas de forma fácil, asegúrate de que los valores predeterminados de la transacción son racionales, consigue una dirección y envía monedas allí. ¡Por eso lo llaman fácil!

> :fuego: ***¿Cuál es el poder de enviar monedas de manera fácil?***

> _Las ventajas._ Es facil. No tiene que preocuparte de cosas arcaicas como los UTXOs. No tiene que calcular las comisiones a mano, por lo que no es probable que cometa errores que le cuesten grandes cantidades de dinero. Si su único objetivo es sentarse frente a su ordenador y enviar dinero, este es el camino a seguir.

> _Las desventajas._ Es de alto nivel. Usted tiene muy poco dominio sobre lo que ocurre, y no puede hacer nada del otro mundo. Si está planeando escribir software de Bitcoin más complejo o quiere una comprensión más profunda de cómo funciona Bitcoin, entonces la forma fácil es sólo una diversión aburrida antes de llegar a lo real.

## ¿Qué sigue?

Continua "Enviando transacciones en Bitcoin" con [§4.2 Creando una Transacción en Crudo](04_2_Creating_a_Raw_Transaction.md).
