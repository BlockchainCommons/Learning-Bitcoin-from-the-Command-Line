# 15.1: Bitcoin Core como servicio I2P (Proyecto de Internet invisible)

> :information_source: **NOTE:** Esta sección se ha agregado recientemente al curso y es un borrador inicial que aún puede estar pendiente de revisión. Lectura con advertencias.

En lugar de utilizar el servicio Tor basado en proxy para garantizar la privacidad de sus comunicaciones de Bitcoin, es posible que desee utilizar I2P, que está diseñado para actuar como una red privada dentro de Internet, en lugar de simplemente ofrecer acceso privado a los servicios de Internet.

## Entender las diferencias

Tanto Tor como I2P ofrecen acceso privado a servicios en línea, pero con diferentes enrutamiento y bases de datos, y con diferentes arquitecturas para retransmisiones. Dado que los servicios ocultos (como el acceso a Bitcoin) son fundamentales para el diseño de I2P, también se han optimizado mejor:

|  | Tor | i2p |
| :---         |     :---:      |          ---: |
| Enrutamiento   | [Onion](https://www.onion-router.net/)     | [Garlic](https://geti2p.net/en/docs/how/garlic-routing)    |
| Base de Datos de Red      |  [Servidores Directorio](https://blog.torproject.org/possible-upcoming-attempts-disable-tor-network) de confianza      | [Base de Datos de Red Distribuida](https://geti2p.net/en/docs/how/network-database)      |
| Retransmisión     | Conexiones encriptadas **bidireccionales** entre cada Retransmisor | Conexiones **unidireccionales** entre cada servidor con sus túneles |
| Servicios Ocultos | Lento       | Rápido      |

Puede encontrar una comparación más detallada en [geti2p.net](https://geti2p.net/en/comparison/tor).

### Comprender las ventajas y desventajas para limitar las conexiones salientes

Hay [compensaciones](https://bitcoin.stackexchange.com/questions/107060/tor-and-i2p-tradeoffs-in-bitcoin-core) si elige admitir solo I2P, solo Tor o ambos. Estas configuraciones, que limitan las conexiones clearnet salientes, se realizan en Bitcoin Core usando el argumento `onlynet` en su `bitcoin.conf`.

* `onlynet=onion`, que limita las conexiones salientes a Tor, puede exponer un nodo a los ataques de Sybil y puede crear particiones de red debido a las conexiones limitadas entre Tornet y clearnet.
* `onlynet=onion` y `onlynet=i2p` en conjunto, que ejecuta el servicio Onion con el servicio I2P, es experimental por ahora.

## Instalar I2P

Para instalar I2P, debe asegurarse de que sus puertos estén configurados correctamente y luego puede continuar con su proceso de configuración.

### Preparar puertos

Para usar I2P, deberá abrir los siguientes puertos, que son requeridos por I2P:

1. **Saliente (Hacia Internet):** Saliente (frente a Internet): se selecciona un puerto aleatorio entre 9000 y 31000. Es mejor si todos estos puertos están abiertos para conexiones salientes, lo que no afecta su seguridad.
- Puede verificar el estado del firewall usando `sudo ufw status verbose`, que no debería negar las conexiones salientes de forma predeterminada.
2. **Entrante (Desde Internet):** Opcional. Una variedad de puertos entrantes se enumeran en los [documentos I2P](https://geti2p.net/en/faq#ports).
- Para obtener la máxima privacidad, es preferible desactivar la aceptación de conexiones entrantes.

### Ejecute I2P

Lo siguiente ejecutará los servicios de Bitcoin Core I2P:

1. Instale `i2pd` en Ubuntu:

   ```
   sudo add-apt-repository ppa:purplei2p/i2pd
   sudo apt-get update
   sudo apt-get install i2pd
   ```

   Para instalar en otros sistemas operativos, consulte [estos documentos](https://i2pd.readthedocs.io/en/latest/user-guide/install/)

2. [Ejecute](https://i2pd.readthedocs.io/en/latest/user-guide/run/) el servicio I2P:

   ```
   $ sudo systemctl start i2pd.service
   ```

3. Compruebe que I2P se esté ejecutando. Debería verlo en el puerto 7656:

   ```
   $ ss -nlt

   State   Recv-Q   Send-Q     Local Address:Port      Peer Address:Port  Process

   LISTEN  0        4096           127.0.0.1:7656           0.0.0.0:*
   ```

4. Agregue las siguientes líneas en `bitcoin.conf`: 

   ```
   i2psam=127.0.0.1:7656
   debug=i2p
   ```
   La opción de registro, `debug=i2p`, se utiliza para registrar información adicional en el registro de depuración sobre la configuración y las conexiones de I2P. La ubicación predeterminada para este archivo de depuración en Linux es `~/.bitcoin/debug.log`:

5. Reinicie `bitcoind`

   ```
   $ bitcoind
   ```

6. Revise el archivo `debug.log` para ver si I2P se configuró correctamente o si apareció algún error en los registros.  
   ```
   2021-06-15T20:36:16Z i2paccept thread start
   2021-06-15T20:36:16Z I2P: Creating SAM session with 127.0.0.1:7656

   2021-06-15T20:36:56Z I2P: SAM session created: session id=3e0f35228b, my address=bmwyyuzyqdc5dcx27s4baltbu6zw7rbqfl2nmclt45i7ng3ul4pa.b32.i2p:18333
   2021-06-15T20:36:56Z AddLocal(bmwyyuzyqdc5dcx27s4baltbu6zw7rbqfl2nmclt45i7ng3ul4pa.b32.i2p:18333,4)
   ```
   
   La dirección I2P se menciona en los registros y termina en b32.i2p . Por ejemplo `bmwyyuzyqdc5dcx27s4baltbu6zw7rbqfl2nmclt45i7ng3ul4pa.b32.i2p:18333`.

7. Confirme que `i2p_private_key` se creó en el directorio de datos de Bitcoin Core. La primera vez que Bitcoin Core se conecta al enrutador I2P, su dirección I2P (y la clave privada correspondiente) se generará automáticamente y se guardará en un archivo llamado *i2p_private_key* :
   ```
   ~/.bitcoin/testnet3$ ls

   anchors.dat  chainstate         i2p_private_key  settings.json
   banlist.dat  debug.log          mempool.dat      wallets
   blocks       fee_estimates.dat  peers.dat
   ```

8. Compruebe que `bitcoin-cli -netinfo` o `bitcoin-cli getnetworkinfo` retornan la dirección I2P:

   ```
   Local addresses
   bmwyyuzyqdc5dcx27s4baltbu6zw7rbqfl2nmclt45i7ng3ul4pa.b32.i2p     port  18333    score      4
   ```
  
Ahora tiene su servidor Bitcoin accesible a través de la red I2P en su nueva dirección local.

## Resumen: Bitcoin Core como servicio I2P (Proyecto de Internet invisible)

Siempre es bueno tener alternativas para la privacidad y no depender únicamente de Tor para ejecutar Bitcoin Core como un servicio oculto. Dado que I2P se agregó recientemente en Bitcoin Core, no muchas personas lo usan. Experimente con él e informe de errores si encuentra algún problema.

> :information_source: **NOTE:** Para la implementación oficial de i2prouter en Java, visite [la página de descarga](https://geti2p.net/en/download) de I2P y siga las instrucciones para su sistema operativo. Una vez instalado, abra una ventana de terminal y escriba `i2prouter start`. Luego, visite `127.0.0.1:7657` en su navegador para habilitar SAM. Para hacerlo, seleccione: "Configurar página de inicio", luego "Clientes", y finalmente seleccione el "Botón Reproducir" junto a la aplicación SAM Bridge. En el lado izquierdo de la página, debería haber una luz verde junto a "Clientes compartidos".

Continué a "Programando con RPC" con [16.0: Hablando a Bitcoind con C](16_0_Hablando_a_Bitcoind_con_C.md)

O, si usted no es un programador, puede omitirlo y continuar a [19.0: Entendiendo Su Configuración Lightning](19_0_Entendiendo_Su_Configuracion_Lightning.md) para continuar su educación en la línea de comandos con la red Lightning.
