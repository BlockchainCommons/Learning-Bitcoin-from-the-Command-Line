# Capítulo 15: Usando i2p

El Proyecto Internet Invisible (I2P) es una capa de red totalmente encriptada. Esta usa una [base de datos](https://geti2p.net/en/docs/how/network-database) distribuida y túneles unidireccionales encriptados entre usted y sus pares.

Diferencias básicas entre Tor y i2p:

|  | Tor | i2p |
| :---         |     :---:      |          ---: |
| Enrutamiento   | [Onion](https://www.onion-router.net/)     | [Garlic](https://geti2p.net/en/docs/how/garlic-routing)    |
| Base de Datos de Red      |  [Servidores Directorio](https://blog.torproject.org/possible-upcoming-attempts-disable-tor-network) de confianza      | [Base de Datos de Red Distribuida](https://geti2p.net/en/docs/how/network-database)      |
| Retransmisión     | Conexiones encriptadas **bidireccionales** entre cada Retransmisor | Conexiones **unidireccionales** entre cada servidor con sus túneles |
| Servicios Ocultos | Lento       | Rápido      |

Lea más: https://geti2p.net/en/comparison/tor

Este no es instalado por [Bitcoin Standup](https://github.com/BlockchainCommons/Bitcoin-Standup-Scripts) actualmente ya que el soporte i2p fue agregado a Bitcoin Core recientemente. Sin embargo, puede probarlo manualmente siguiendo los pasos mencionados en la [Sección Uno](15_1_Servicio_i2p.md).

## Objetivos para Este Capítulo

Después de procesar este capitulo, un desarrollador sera capaz de:

  * Ejecutar Bitcoin Core como un servicio I2P (Proyecto Internet Invisible)

Los objetivos de apoyo incluyen la habilidad de:

  * Entender la red i2p
  * Entender las diferencias entre Tor e i2p

## Tabla de Contenidos

* [Sección Uno: Bitcoin Core como un Servicio I2P (Proyecto Internet Invisible)](15_1_Servicio_i2p.md)
