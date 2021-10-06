# 20.4: Expandiendo la Red Lightning

> :information_source: **NOTA:** Esta sección se ha agregado recientemente al curso y es un borrador inicial que aún puede estar pendiente de revisión. Lector de advertencias.

Estos dos capítulos han cubierto solo algunas de las actividades más importantes con Lightning. Se puede hacer mucho más y es posible una gran variedad. Lo que sigue son algunos consejos a seguir.

## Usar Complementos de c-lightning

c-lightning es una implementación ligera, altamente pseronalizable y compatible con el estándar del protocolo de la Red Lightning. Extiende su funcionalidad usando complementos. Principalmente, estos son subprocesos que son inciados por el demonio `lightningd` y pueden interactuar con `lightningd` en una variedad de formas:

* Las opciones de la línea de comandos permiten que los complementos registren sus propios argumentos de línea de comandos, que luego se exponen a través de `lightningd`.
* El paso de comandos JSON-RPC permite que los complementos agreguen sus propios comandos a la interfaz JSON-RPC.
* Las suscripciones de flujo de eventos proporcionan complementos con un mecanismo de notificación basado en push para `lightnind`.
* Los hooks son una opción primitiva que permite que los complementos sean notificados sobre eventos en el demonio `lightningd` y modifiquen su comportamiento o transmitan comportamientos personalizados.

Un complemento puede estar escrito en cualquier lenguaje y puede comunicarse con `lightningd` a través de stdin y stdout del complemento. JSON-RPCv2 se utiliza como protocolo en la parte superior de los dos flujos, con el complemento actuando como servidor y `lightningd` actuando como cliente. 

El repositorio de GitHub `lightningd` mantiene una lista actualizada de [complementos](https://github.com/lightningd/plugins) disponibles.

## Usar Carteras Móviles

Actualmente conocemos dos billeteras Lightning móviles que admiten la implementación de c-lightning.

Para dispositivos iOS, FullyNoded es una billetera de Bitcoin para iOS de código abierto que se conecta a través del servicio autenticado Tor V3 a su propio nodo completo. La funcionalidad FullyNoded se encuentra actualmente en desarrolo activo y en la fase de prueba beta inicial.

*  [FullyNoded](https://github.com/Fonta1n3/FullyNoded/blob/master/Docs/Lightning.md)

SparkWallet es una GUI de billetera minimalista para c-lightning, accesible a través de la web o mediante aplicaciones móviles y de escritorio para Android.

*  [SparkWallet](https://github.com/shesek/spark-wallet)

## Usar Differentes Implementaciones Lightning

c-lightning no es su única opción. Hoy en día existen tres implementaciones ampliamente utilizadas de la Red Network. Todos siguen los [documentos Basis of Lightning Technology (BOLT)](https://github.com/lightningnetwork/lightning-rfc), (la base de la tecnología lightning) que describen un protocolo de layer-2 para transferencias de bitcoins fuera de la cadena. Las especificaciones son actualmente un trabajo en progreso que aún se está redactando.

| Nombre  | Descripción | BitcoinStandup | Lenguaje | Repositorio |
| ------------- | ------------- | :---: | ------------- | ------------- |
| C-lighting  | Blockstream  | X | C | [Descargar](https://github.com/ElementsProject/lightning) |
| LND  | Lightning Labs  | X | Go | [Descargar](https://github.com/lightningnetwork/lnd) |
| Eclair  | ACINQ  | - | Scala | [Descargar](https://github.com/ACINQ/eclair) |

## Mantener Copias de Seguridad

Su nodo Lightning debe estar en línea todo el tiempo; de lo contrario, su contraparte podría enviar un estado de canal anterior y robar sus fondos. Sin embargo, existe otro escenario en el que se pueden perder fondos, y es cuando ocurre una falla de hardware que impide que el nodo establezca un cierre cooperativo con la contraparte. Esto probablemente significará que si no tiene una copia exacta del estado del canal antes de la falla, tendrá un estado no válido que podría hacer que el otro nodo lo asuma como un intento de fraude y use la transacción de penalización. En este caso, se perderán todos los fondos. Para evitar esta situación indeseable existe una solución basada en la alta disponibilidad de [la base de postgresQL](https://github.com/gabridome/docs/blob/master/c-lightning_with_postgresql_reliability.md).

No hemos probado esta solución.

## Resumen: Expandiendo la Red Lightning

Puede usar diferentes implementaciones complementos billeteras móviles o copias de seguridad para expandir su experiencia Lightning. 

## ¿Que Sigue?

Ha completado Aprender Bitcoin Desde la Línea de Comandos, aunque si nunca visitó los [Apéndices](A0_Apendices.md) de configuraciones alternativas, puede hacerlo ahora.

De lo contrario, le recomendamos que se una a las comunidades de desarrolladores, programe y ponga en práctica sus nuevos conocimientos.

También puede ayudarnos aquí en Blockchain Commons con problemas o relaciones públicas para aprender Bitcoin o para cualquiera de nuestros otros repositorios, o inlcuso puede convertirse en [Patrocinador](https://github.com/sponsors/BlockchainCommons). También puede ayudar haciendo correr la voz: informe a las personas en las redes sociales sobre el curso y lo que aprendió de él!

Ahora salga y haga de la comunidad Blockchain un lugar mejor!

