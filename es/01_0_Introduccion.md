# Capítulo uno: Introducción al aprendizaje de Bitcoin Core (y Lightning) desde la línea de comandos

## Introducción

La forma en que realizamos pagos por bienes y servicios ha cambiado drásticamente en las últimas décadas. Donde antes todas las transacciones se realizaban a través de efectivo o cheques, ahora distintos métodos de pago electrónico son la norma. Sin embargo, la mayoría de estos pagos electrónicos aún se realizan a través de sistemas centralizados, donde las compañías de tarjetas de crédito, los bancos o incluso las instituciones financieras basadas en Internet como Paypal mantienen listas de transacciones largas e individualmente correlacionadas lo que les da el poder de censurar las transacciones que no les gustan.

Estos riesgos de centralización fueron algunos de los principales catalizadores detrás de la creación de las criptomonedas, el primero y más exitoso proyecto es Bitcoin. Bitcoin ofrece seudoanonimato; dificulta la correlación de transacciones y hace que la censura por parte de entidades individuales sea casi imposible. Estas ventajas la han convertido en una de las monedas de más rápido crecimiento en el mundo. Ese crecimiento, a su vez, ha convertido a Bitcoin en una empresa en marcha entre empresarios y desarrolladores, ansiosos por crear nuevos servicios para la comunidad de Bitcoin.

Si eres uno de esos emprendedores o desarrolladores, este curso es para ti, porque se trata de aprender a programar Bitcoin. Es un curso introductorio que explica todos los matices y características de Bitcoin a medida que avanza. También toma un rumbo muy específico, al ofrecer lecciones sobre cómo trabajar directamente con Bitcoin Core y con el servidor c-lightning usando sus interfaces RPC.

¿Por qué no utilizar algunas de las bibliotecas más completas que se encuentran en varios lenguajes de programación? ¿Por qué no crear el tuyo propio desde cero? Porque empezar a trabajar con criptomonedas puede ser peligroso. No hay redes de seguridad. Si accidentalmente paga de más en sus tarifas o pierde una llave de firma o crea una transacción no válida o comete una cantidad de errores potenciales, entonces sus monedas desaparecerán para siempre. Gran parte de esa responsabilidad, por supuesto, recaerá en usted como programador de criptomonedas, aunque puede minimizarse el riesgo trabajando con las interfaces de criptomonedas más sólidas, seguras y probadas, las creadas por los propios equipos de programación de criptomonedas: bitcoind y lightningd.

Por lo tanto, gran parte de este libro analiza cómo crear un script de Bitcoin (y Lightning) directamente desde la línea de comandos. Algunos capítulos posteriores tratan sobre lenguajes de programación más sofisticados, pero nuevamente continúan interactuando directamente con los demonios bitcoind y lightningd usando RPC o interactuando con los archivos que crean. Esto le permite pararse sobre los hombros de gigantes y utilizar la tecnología de forma confiable para aprender a crear sus propios sistemas confiables.

# Nivel de habilidad requerido

No es necesario ser técnico para avanzar en la mayor parte de este curso. Todo lo que necesita es la confianza para ejecutar comandos básicos en la línea de comandos de UNIX. Si está familiarizado con cosas como ssh, cd y ls, el curso le proporcionará el resto.

Una parte menor de este curso requiere conocimientos de programación, y debe omitir esas secciones si es necesario como se explica en la siguiente sección.

## Resumen de temas

Este libro se divide a grandes rasgos en las siguientes secciones:

| Parte | Descripción | Habilidades |
|-------|---------|---------|
| **Primera parte: Primera parte: prepararse para Bitcoin** | Comprender los conceptos básicos de Bitcoin y configurar un servidor para su uso. | Línea de comandos | 
| **Segunda parte: Uso de Bitcoin-CLI** | Usando el Bitcoin-CLI para crear transacciones. | Línea de comandos |
| **Tercera parte: Secuencias de comandos de Bitcoin** | Ampliando su trabajo de Bitcoin con scripts | Conceptos de programación |
| **Parte Cuatro: Uso de Tor** | Mejorando la seguridad de su nodo con Tor | Línea de comandos |
| **Parte Cinco: Programación con RPC** | Accediendo a RPC desde C y otros lenguajes | Programación en lenguaje C |
| **Parte Seis: Uso de Lightning-CLI** | Uso de Lightning-CLI para crear transacciones. | Línea de comandos |
| **Apendices** | Utilizando configuraciones de Bitcoin menos comunes | Linea de comandos |

# Cómo utilizar este curso

Así que ¿Por dónde empiezas? Este libro está destinado principalmente para leerse de forma secuencial. Simplemente se debe seguir el enlace llamado "¿Qué sigue?"  al final de cada sección y / o hacer clic en los enlaces de las secciones individuales en cada página de capítulo. Obtendrá una mejor comprensión de este curso si realmente configura un servidor Bitcoin (siguiendo el Capítulo 2) y luego repasa todos los ejemplos a lo largo del libro: probar ejemplos es una excelente metodología de aprendizaje.

Si tiene diferentes niveles de habilidad o desea aprender cosas diferentes, puede pasar directamente a diferentes partes del libro:

* Si ya tiene un entorno de Bitcoin listo para ser utilizado, vaya al [Capítulo Tres: Comprensión de la configuración de Bitcoin.](03_2_Conociendo_su_Configuracion_Bitcoin.md)
* Si solo le interesan las secuencias de comandos de Bitcoin, vaya al [Capítulo nueve: Introducción a las secuencias de comandos de Bitcoin.](09_0_Introducing_Bitcoin_Scripts.md)
* Si solo desea leer sobre el uso de lenguajes de programación, vaya al Capítulo dieciséis: Comunicarse con el servidor Bitcoin.
* Si, por el contrario, no quiere hacer nada relativo a programación, definitivamente omita los capítulos 15-17 mientras lee, y tal vez omita los capítulos 9-13. El resto del curso aún debería tener sentido sin ellos.
* Si solo está interesado en Lightning, pase al Capítulo Dieciocho: Comprensión de la configuración de Lightning .
* Si desea leer el contenido nuevo más importante agregado para la versión 2 del curso (2020), a continuación de la versión 1 (2017), lea [§3.5: Comprensión de los descriptores](03_5_Entendiendo_El_Descriptor.md) , §4.6: Creación de una transacción SegWit , Capítulo 7: Expansión de Bitcoin con PSBT , [§9.5: Scripting con P2WPKH](09_5_Codificando_una_P2WPKH.md) , §10.5: Scripting con SegWit Script , Capítulo 14: Usando Tor , Capítulo 15: Interactuando con Bitcoind con C , Capítulo 16: Programando con Libwally , Capítulo Diecisiete: Interactuando con Bitcoind con otros lenguajes , Capítulo dieciocho: Comprensión y Capítulo Diecinueve: Uso de Lightning.

# Por qué utilizar este curso

Obviamente está trabajando en este curso porque está interesado en Bitcoin. Además de impartir conocimientos básicos, este curso también ha ayudado a los lectores a unirse (o crear) proyectos de código abierto y a obtener trabajos a nivel inicial en la programación de Bitcoin. Varios pasantes de Blockchain Commons aprendieron sobre Bitcoin en este curso, al igual que algunos miembros de nuestro equipo de programación.

# Cómo apoyar este curso

* Utilice la sección de [Problemas](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/issues) para cualquier pregunta. Blockchain Commons no tiene un equipo de soporte activo, por lo que no podemos abordar problemas o consultas individuales, pero los revisaremos a tiempo y los usaremos para mejorar las iteraciones futuras del curso.
* Utilice la sección de [PR's](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/pulls) para corregir errores tipográficos o comandos incorrectos (o modificados). Para cambios técnicos o de línea de comandos, es muy útil si también usa los comentarios en los PR's para explicar las razones por las cuales hizo sus cambios y facilitar o evitar la investigación.
* Utilice el [área de discusión](https://github.com/BlockchainCommons/Community/discussions) de nuestra comunidad para hablar sobre carreras y habilidades. Blockchain Commons ocasionalmente ofrece pasantías, como se explica en nuestro repositorio de la comunidad.
* Conviértase en un [patrocinador](https://github.com/sponsors/BlockchainCommons) si encuentra útil este curso o si desea ayudar a educar a la próxima generación de programadores de blockchain.

# ¿Que Sigue?

Si desea una introducción básica a Bitcoin, criptografía de clave pública, ECC, blockchains y Lightning, lea el interludio [Introduciendo Bitcoin.](01_1_Introduciendo_Bitcoin.md)

De lo contrario, si está listo para sumergirse en el curso, vaya a [Configuración de un Servidor privado Virtual de Bitcoin-Core.](02_0_Configurando_un_VPS_Bitcoin-Core.md)
