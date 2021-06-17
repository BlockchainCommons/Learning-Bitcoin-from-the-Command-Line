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
