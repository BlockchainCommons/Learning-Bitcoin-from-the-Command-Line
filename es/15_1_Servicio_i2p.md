# 15.1: Bitcoin Core como un Servicio I2P (Proyecto Internet Invisible)

Los usuarios deben considerar diferentes ventajas y desventajas involucradas en el uso de i2p con otras redes en Bitcoin Core para una mejor privacidad:

- Ataques de tipo Sybil y Particionamiento de Red son posibles con `onlynet=i2p`
- Ejecutar el servicio Onion con el servicio i2p por ahora es experimental
- Para una máxima privacidad, es preferible deshabilitar las conexiones entrantes

Lea más: https://bitcoin.stackexchange.com/questions/107060/tor-and-i2p-tradeoffs-in-bitcoin-core

Siga los siguientes pasos para ejecutar el servicio i2p de Bitcoin Core:

1. Puede instalar `i2pd` en Ubuntu con el siguiente comando:

   ```
   sudo add-apt-repository ppa:purplei2p/i2pd
   sudo apt-get update
   sudo apt-get install i2pd
   ```

   Para instalarlo en otro sistema operativo: https://i2pd.readthedocs.io/en/latest/user-guide/install/

2. Luego [ejecute](https://i2pd.readthedocs.io/en/latest/user-guide/run/) y verifique si esta ejecutándose:

   ```
   $ sudo systemctl start i2pd.service
   ```

   Usted debería verlo ejecutándose en el puerto 7656:

   ```
   $ ss -nlt

   State   Recv-Q   Send-Q     Local Address:Port      Peer Address:Port  Process

   LISTEN  0        4096           127.0.0.1:7656           0.0.0.0:*
   ```

3. Agregue las siguientes lineas en `bitcoin.conf`. La opción de configuración de registros `debug=i2p` es usada para tener información adicional en el registro de depuración sobre la configuración I2P y sus conexiones (ubicación predeterminada: ~/.bitcoin/bitcoin.conf):

   ```
   i2psam=127.0.0.1:7656
   debug=i2p
   ```
4. Reinicie `bitcoind`

   ```
   $ bitcoind
   ```

6. Verifique en `debug.log` si i2p fue configurado correctamente o cualquier error fue escrito en los registros. La dirección I2P es mencionada en los registros y termina con _b32.i2p_:
   ```
   2021-06-15T20:36:16Z i2paccept thread start
   2021-06-15T20:36:16Z I2P: Creating SAM session with 127.0.0.1:7656

   2021-06-15T20:36:56Z I2P: SAM session created: session id=3e0f35228b, my address=bmwyyuzyqdc5dcx27s4baltbu6zw7rbqfl2nmclt45i7ng3ul4pa.b32.i2p:18333
   2021-06-15T20:36:56Z AddLocal(bmwyyuzyqdc5dcx27s4baltbu6zw7rbqfl2nmclt45i7ng3ul4pa.b32.i2p:18333,4)
   ```

7. Confirme que `i2p_private_key` fue creada en el directorio de datos de Bitcoin Core. La primera vez que Bitcoin Core se conecte con el enrutador I2P, su dirección I2P (y correspondiente llave privada) serán automáticamente generadas y guardadas en un archivo llamado *i2p_private_key*:
   ```
   ~/.bitcoin/testnet3$ ls

   anchors.dat  chainstate         i2p_private_key  settings.json
   banlist.dat  debug.log          mempool.dat      wallets
   blocks       fee_estimates.dat  peers.dat
   ```

8. `bitcoin-cli -netinfo` o `bitcoin-cli getnetworkinfo` retornan una dirección i2p:

   ```
   Local addresses
   bmwyyuzyqdc5dcx27s4baltbu6zw7rbqfl2nmclt45i7ng3ul4pa.b32.i2p     port  18333    score      4
   ```

## Resumen: Bitcoin Core como un Servicio I2P (Proyecto Internet Invisible)

Siempre es bueno tener alternativas para privacidad y no depender solamente de Tor para correr Bitcoin Core como un servicio oculto. Dado que i2p fue recientemente agregado a Bitcoin Core, poca gente lo usa. Experimente con este y reporte errores si encuentra algún problema.

> :information_source: **NOTA:** _i2pd_ (C++) es diferente de _i2prouter_ (Java), usted necesitara `i2pd` para Bitcoin Core.

Continué a "Programando con RPC" con [16.0: Hablando a Bitcoind con C](16_0_Hablando_a_Bitcoind_con_C.md)

O, si usted no es un programador, puede omitirlo y continuar a [19.0: Entendiendo Su Configuración Lightning](19_0_Entendiendo_Su_Configuracion_Lightning.md) para continuar su educación en la línea de comandos con la red Lightning.
