# 13.3: Empoderar a Bitcoin con scripts

Los scripts de Bitcoin pueden ir mucho más allá de los instrumentos financieros relativamente simples detallados hasta la fecha. También son la base de los usos más complejos de la red Bitcoin, como lo demuestran estos ejemplos del mundo real de funcionalidad fuera de la cadena, extraídos de los ejemplos de Lightning Network en [BIP 112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki).

## Bloqueo para Lightning Network

[Lightning Network](https://rusty.ozlabs.org/?p=450) es un canal de pago que permite a los usuarios retirar fondos de la cadena y participar en numerosas microtransacciones antes de finalizar el canal de pago y devolver los fondos a Bitcoin. Los beneficios incluyen tarifas más bajas y velocidades de transacción más rápidas. Se discute con más detalle, con ejemplos de cómo usarlo desde la línea de comando, comenzando el [Capítulo 18](18_0_Understanding_Your_Lightning_Setup.md).

[BIP 112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki) contiene algunos ejemplos de cómo se podrían generar estas transacciones fuera de la cadena, utilizando scripts de bloqueo de Bitcoin.

### Bloqueo con transacciones de compromiso revocable

El truco con Lightning es el hecho de que está fuera de la cadena. Para usar Lightning, los participantes bloquean los fondos de forma conjunta en la cadena de bloques de Bitcoin con una firma múltiple n-de-n. Luego, se involucran en una serie de transacciones entre ellos. Cada nueva "transacción de compromiso" divide esos fondos conjuntos de una manera diferente; estas transacciones están parcialmente firmadas pero _no se colocan en la cadena de bloques_.

Si tiene una gran cantidad de transacciones no publicadas, cualquiera de las cuales _podría_ publicarse a la cadena de bloques, ¿cómo puede evitar que uno de los participantes vuelva a una transacción anterior que sea más beneficiosa para ellos? La respuesta es la _revocación_. Un ejemplo simplificado en BIP 112, que ofrece uno de los escalones base hacia Lightning, muestra cómo: le da al participante que se vería perjudicado por la reversión a una transacción revocada la capacidad de reclamar los fondos él mismo si el otro participante intenta usar ilegítimamente la transacción revocada.

Por ejemplo, suponga que Alice y Bob actualizan la transacción de compromiso para darle más fondos a Bob (efectivamente: Alice envió fondos a Bob a través de la red Lightning). Firman parcialmente nuevas transacciones, pero también ofrecen su propio "revokeCode" para transacciones anteriores. Esto garantiza efectivamente que no publicarán transacciones anteriores, porque hacerlo permitiría a su contraparte reclamar esos fondos anteriores.

Entonces, ¿cómo se ve la transacción anterior? Era una transacción de compromiso que mostraba fondos destinados a Alice, antes de que se los diera a Bob. Tenía un script de bloqueo de la siguiente manera:

```
OP_HASH160 
<revokeHash> 
OP_EQUAL
    
IF
        
    <pubKeyBob>
    
ELSE

    <+24Hours> 
    OP_CHECKSEQUENCEVERIFY 
    OP_DROP
    <pubKeyAlice>
    
ENDIF
OP_CHECKSIG
```

El bloque `ELSE` es donde Alice obtuvo sus fondos, después de un retraso de 24 horas. Sin embargo, ahora ha sido reemplazado; ese es el objetivo de un canal de pago al estilo Lightning, después de todo. En esta situación, esta transacción nunca debe publicarse. Bob no tiene ningún incentivo para hacerlo porque tiene una transacción más nueva, lo que lo beneficia más porque le han enviado algunos de los fondos de Alice. Alice tampoco tiene ningún incentivo, porque pierde los fondos si lo intenta debido a ese `revokeCode`. Por lo tanto, nadie coloca la transacción en la cadena de bloques y las transacciones fuera de la cadena continúan.

Vale la pena explorar cómo funcionaría este script en una variedad de situaciones, la mayoría de las cuales involucran a Alice tratando de hacer trampa volviendo a esta transacción anterior, que describe los fondos _antes_ que Alice enviara algunos de ellos a Bob.

#### Ejecuta el script de bloqueo para engañar a Alice, con código de revocación

Alice podría intentar usar el código de revocación que le dio a Bob para reclamar los fondos de inmediato. Ella escribe un script de bloqueo de `<sigAlice> <revokeCode>`:

```
Script: <sigAlice> <revokeCode> OP_HASH160 <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ ]

Script: OP_HASH160 <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ <sigAlice> <revokeCode> ]

Script: <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Running: <revokeCode> OP_HASH160 
Stack: [ <sigAlice> <revokeHash> ]

Script: OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ <sigAlice> <revokeHash> <revokeHash> ]

Script: IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Running: <revokeHash> <revokeHash> OP_EQUAL
Stack: [ <sigAlice> True ]
```

El `OP_EQUAL` alimenta la declaración` IF`. Debido a que Alice usa el `revokeCode`, ingresa a la sucursal que le permite canjear los fondos inmediatamente, colapsando el resto del script a` <pubKeyBob> `(dentro del condicional) y`OP_CHECKSIG` (después).

```
Script: <pubKeyBob> OP_CHECKSIG
Running: True IF
Stack: [ <sigAlice> ]
```
Maldiciones! ¡Solo Bob puede firmar inmediatamente usando el `redeemCode`!

```
Script:  OP_CHECKSIG
Stack: [ <sigAlice> <pubKeyBob> ]

Script:
Running: <sigAlice> <pubKeyBob> OP_CHECKSIG
Stack: [ False ]
```

#### Ejecuta el script de bloqueo para engañar a Alice, sin código de revocación

Entonces, ¿qué pasa si Alice intenta usar su propia firma, sin el `revokeCode`? Ella usa un script de desbloqueo de `<sigAlice> <notRevokeCode>`.

```
Script: <sigAlice> 0 OP_HASH160 <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ ]

Script: OP_HASH160 <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ <sigAlice> 0 ]

Script: <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Running: 0 OP_HASH160 
Stack: [ <sigAlice> <0Hash> ]

Script: OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ <sigAlice> <0Hash> <revokeHash> ]

Script: IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Running: <0Hash> <revokeHash> OP_EQUAL
Stack: [ <sigAlice> False ]
```

Ahora colapsamos hasta la declaración `ELSE` y lo que viene después del condicional:

```
Script: <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> OP_CHECKSIG
Running: False IF
Stack: [ <sigAlice> ]

Script: OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> OP_CHECKSIG
Stack: [ <sigAlice> <+24Hours> ]
```

¡Y luego Alice está frustrada nuevamente porque no han pasado 24 horas!

```
Script: OP_DROP <pubKeyAlice> OP_CHECKSIG
Running: <+24Hours> OP_CHECKSEQUENCEVERIFY 
Stack: [ <sigAlice> <+24Hours> ] — Script EXITS
```

#### Ejecuta el script de bloqueo para un Bob victimizado

Lo que esto significa es que Bob tiene 24 horas para reclamar sus fondos si Alice alguna vez intenta hacer trampa, usando el `<revokeCode>` y su firma como su script de desbloqueo:

```
Script: <SigBob> <revokeCode> OP_HASH160 <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ ]

Script: OP_HASH160 <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ <SigBob> <revokeCode> ]

Script: <revokeHash> OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Running: <revokeCode> OP_HASH160 
Stack: [ <SigBob> <revokeHash> ]

Script: OP_EQUAL IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Stack: [ <SigBob> <revokeHash> <revokeHash> ]

Script: IF <pubKeyBob> ELSE <+24Hours> OP_CHECKSEQUENCEVERIFY OP_DROP <pubKeyAlice> ENDIF OP_CHECKSIG
Running: <revokeHash> <revokeHash> OP_EQUAL
Stack: [ <SigBob> True ]

Script: <pubKeyBob> OP_CHECKSIG
Running: True IF
Stack: [ <SigBob> ]

Script:  OP_CHECKSIG
Stack: [ <SigBob> <pubKeyBob> ]

Script:
Running: <SigBob> <pubKeyBob> OP_CHECKSIG
Stack: [ True ]
```

#### Ejecuta el script de bloqueo para una Alice virtuosa

Todas las transacciones de compromiso de Alice están bloqueadas con este mismo script de bloqueo, ya sea que hayan sido revocadas o no. Eso significa que la transacción de compromiso más reciente, que es la válida actualmente, también está bloqueada. Alice nunca ha enviado una nueva transacción a Bob y, por lo tanto, nunca le envió el `revokeCode` anterior.

En esta situación, podría publicar virtuosamente la transacción, cerrando el canal proto-Lightning. Ella pone la transacción en la cadena y espera 24 horas. Bob no puede hacer nada al respecto porque no tiene el código de recuperación. Luego, después de la espera, Alice reclama sus fondos. (Bob hace lo mismo con su propia transacción de compromiso final).

### Bloqueo con contratos codificados con bloqueo de tiempo

Las Transacciones de Compromiso Revocable fueron solo un trampolín hacia Lightning. La Lightning Network real utiliza un mecanismo más complejo llamado [contrato de bloqueo de tiempo hash] (https://en.bitcoin.it/wiki/Hashed_Timelock_Contracts), o HTLC.

El objetivo principal de los HTLC es crear una red integral de participantes. Las transacciones ya no son solo entre un par de participantes que han ingresado a la red juntos, sino que ahora pueden ser entre personas previamente no asociadas. Cuando se envían fondos, se crea una serie de transacciones, cada una de ellas bloqueada con un "secretHash". Cuando se revela el "secretCode" correspondiente, se puede gastar toda la cadena de transacciones. Esto es lo que permite que las transacciones singulares se conviertan realmente en una red.

También hay un poco más de complejidad en los scripts de bloqueo de Lightning Network. Hay bloqueos separados para el remitente y el destinatario de cada transacción que son más divergentes que las diferentes transacciones de compromiso mencionadas en la sección anterior. Vamos a mostrarlos a ambos para demostrar el poder de estos scripts de bloqueo, pero no vamos a detenernos en cómo interactúan entre sí.

#### Bloquear la transacción del destinatario

Una vez más, comenzaremos a ver la transacción de compromiso de Alice, que muestra los fondos que recibió:

```
OP_HASH160 
OP_DUP 
<secretHash> 
OP_EQUAL
    
IF

    <+24Hours>
    OP_CHECKSEQUENCEVERIFY
    OP_2DROP
    <pubKeyAlice>
    
ELSE

    <revokeHash> 
    OP_EQUAL
        
    OP_NOTIF
            
        <Date> 
        OP_CHECKLOCKTIMEVERIFY 
        OP_DROP
        
    ENDIF
        
    <pubKeyBob>
    
ENDIF
    
OP_CHECKSIG
```

La clave de estos nuevos HTLC es el `secretHash`, que dijimos es lo que permite que una transacción abarque la red. Cuando la transacción se ha extendido desde su originador hasta su destinatario previsto, se revela el "secretCode", que permite a todos los participantes crear un "secretHash" y desbloquear toda la red de pagos.

Una vez que se ha revelado el `secretCode`, se abre la sucursal` IF`: Alice puede reclamar los fondos 24 horas después de que la transacción se haya realizado en la red Bitcoin.

Sin embargo, también existe la oportunidad de que Bob reclame sus fondos, que aparecen en la rama "ELSE". Puede hacerlo si la transacción ha sido revocada (pero Alice la pone en la cadena de bloques de todos modos), _o si_ ha ocurrido un tiempo de espera absoluto.

#### Bloquear la transacción del remitente

Aquí está el script de bloqueo de transacción de compromiso alternativo utilizado por el remitente:

```
OP_HASH160 
OP_DUP 
<secretHash>
OP_EQUAL
OP_SWAP 
<revokeHash> 
OP_EQUAL 
OP_ADD
   
IF

    <pubKeyAlice>

ELSE

    <Date>
    OP_CHECKLOCKTIMEVERIFY
    <+24Hours>
    OP_CHECKSEQUENCEVERIFY
    OP_2DROP
    <pubKeyBob>

ENDIF
OP_CHECKSIG
```

La parte inicial de su script es bastante inteligente y, por lo tanto, vale la pena ejecutarla:

```
Initial Script: <suppliedCode> OP_HASH160 OP_DUP <secretHash> OP_EQUAL OP_SWAP <revokeHash> OP_EQUAL OP_ADD
Stack: [ ]

Initial Script: OP_HASH160 OP_DUP <secretHash> OP_EQUAL OP_SWAP <revokeHash> OP_EQUAL OP_ADD
Stack: [ <suppliedCode> ]

Initial Script: OP_DUP <secretHash> OP_EQUAL OP_SWAP <revokeHash> OP_EQUAL OP_ADD
Running: <suppliedCode> OP_HASH160 
Stack: [ <suppliedHash> ]

Initial Script: <secretHash> OP_EQUAL OP_SWAP <revokeHash> OP_EQUAL OP_ADD
Running: <suppliedHash> OP_DUP 
Stack: [ <suppliedHash> <suppliedHash> ]

Initial Script: OP_EQUAL OP_SWAP <revokeHash> OP_EQUAL OP_ADD
Stack: [ <suppliedHash> <suppliedHash> <secretHash> ]

Initial Script: OP_SWAP <revokeHash> OP_EQUAL OP_ADD
Running: <suppliedHash> <secretHash> OP_EQUAL
Stack: [ <suppliedHash> <wasItSecretHash?> ]

Initial Script: <revokeHash> OP_EQUAL OP_ADD
Running: <suppliedHash> <wasItSecretHash?> OP_SWAP
Stack: [ <wasItSecretHash?> <suppliedHash> ]

Initial Script: OP_EQUAL OP_ADD
Stack: [ <wasItSecretHash?> <suppliedHash> <revokeHash> ]

Initial Script: OP_ADD
Running: <suppliedHash> <revokeHash> OP_EQUAL
Stack: [ <wasItSecretHash?> <wasItRevokeHash?> ]

Initial Script: 
Running: <wasItSecretHash?> <wasItRevokeHash?> OP_ADD
Stack: [ <wasItSecretOrRevokeHash?> ]
```

La ejecución del script revela que las comprobaciones iniciales, por encima de `IF` /` ELSE` / `ENDIF`, determinan si el hash era _o_ el` secretCode` _o_ el `revokeCode`. Si es así, Alice puede tomar los fondos en el primer bloque. De lo contrario, Bob puede tomar los fondos, pero solo después de que Alice haya tenido su oportunidad y después de que hayan transcurrido el tiempo de espera de 24 horas y el tiempo de espera absoluto.

#### Comprender los HTLC

Los HTLC son bastante complejos y esta descripción general no intenta explicar todas sus complejidades. La [descripción general](https://rusty.ozlabs.org/?p=462) de Rusty Russell explica más y hay incluso más detalles en su artículo [Deployable Lightning](https://github.com/ElementsProject/lightning/blob/master/doc/deployable-lightning.pdf). Pero no se preocupe si algunas de las complejidades aún se le escapan, particularmente las interrelaciones de los dos guiones.

A los efectos de este tutorial, hay dos lecciones importantes para los HTLC:

* Comprenda que se puede crear una estructura muy compleja como un HTLC con Bitcoin Script.
   * Analizar cómo ejecutar cada uno de los scripts HTLC.

Vale la pena dedicar tiempo a ejecutar cada uno de los dos scripts HTLC a través de cada una de sus permutaciones, un elemento de pila a la vez.

## Resumen: Empoderamiento de Bitcoin con scripts

Cerramos nuestro examen de los scripts de Bitcoin con un vistazo a lo realmente poderosos que pueden ser. En 20 códigos de operación o menos, un script de Bitcoin puede formar la base de todo un canal de pago fuera de la cadena. De manera similar, las cadenas laterales vinculadas de dos vías son el producto de menos de veinte códigos de operación, como también se indica brevemente en [BIP 112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki).

Si alguna vez ha visto funciones complejas de Bitcoin o sistemas adyacentes a Bitcoin, probablemente se hayan construido sobre scripts de Bitcoin. Y ahora tiene todas las herramientas para hacer lo mismo por usted mismo.

## ¿Que sigue?

Continúe con "Uso de Tor" con el [Capítulo catorce: Uso de Tor](14_0_Usando_Tor.md).

O, si lo prefiere, hay dos caminos alternativos:

Si desea mantenerse enfocado en Bitcoin, pase a "Programación con RPC" con el [Capítulo Quince: Hablando con Bitcoind con C](15_0_Hablando_a_Bitcoind_con_C.md).

O, si desea concentrarse en la línea de comandos porque no es un programador, puede pasar al [Capítulo dieciocho: Comprensión de su Configuración Lightning](18_0_Entendiendo_Su_Configuracion_Lightning.md) para continuar su educación en línea de comandos con Lightning Network.
