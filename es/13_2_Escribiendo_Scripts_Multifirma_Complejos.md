# 13.2: Escritura de scripts complejos multifirma

Hasta la fecha, las pruebas múltiples descritas en estos documentos han sido completamente simples, de la forma m-de-n o n-de-n. Sin embargo, es posible que desee múltiples funciones más complejas, en las que los cofirmantes varían o en las que pueden estar disponibles diferentes opciones con el tiempo.

## Escriba una variable multifirma

Una firma múltiple variable requiere que firmen diferentes números de personas dependiendo de quién esté firmando.

### Escribir una firma múltiple con un solo firmante o cofirmantes

Imagine una corporación donde el presidente o dos de cada tres vicepresidentes pudieran estar de acuerdo con el uso de los fondos.

Puede escribir esto creando una declaración `IF` /` ELSE` / `ENDIF` que tenga dos bloques, uno para el presidente y su firma uno de uno y otro para los vicepresidentes y sus dos de tres firmas. Luego puede determinar qué bloque usar en función de cuántas firmas hay en el script de desbloqueo. El uso de `OP_DEPTH 1 OP_EQUAL` le dirá si hay un elemento en la pila, y luego continuará desde allí.

El script de bloqueo completo sería `OP_DEPTH 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF`

Si lo dirige el presidente, se vería así:

```
Script: <sigPres> OP_DEPTH 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Stack: [ ]

Script: OP_DEPTH 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Stack: [ <sigPres> ]

Script: 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Running: <SigPres> OP_DEPTH
Stack: [ <sigPres> 1 ]

Script: OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Stack: [ <sigPres> 1 1 ]

Script: IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Running: 1 1 OP_EQUAL
Stack: [ <sigPres> True ]
```

Debido a que el resultado es "Verdadero", la secuencia de comandos ahora se contrae en la declaración "SI":

```
Script: <pubKeyPres> OP_CHECKSIGNATURE
Running: True IF
Stack: [ <sigPres> ]

Script: OP_CHECKSIGNATURE
Stack: [ <sigPres> <pubKeyPres> ]

Script: 
Running: <sigPres> <pubKeyPres> OP_CHECKSIGNATURE
Stack: [ True ]
```

Si lo dirigen dos vicepresidentes, se vería así:

```
Script: 0 <sigVPA> <sigVPB> OP_DEPTH 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Stack: [ ]

Script: OP_DEPTH 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Stack: [ 0 <sigVPA> <sigVPB> ]

Script: 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Running: 0 <sigVPA> <sigVPB> OP_DEPTH
Stack: [ 0 <sigVPA> <sigVPB> 3 ]

Script: OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Stack: [ 0 <sigVPA> <sigVPB> 3 1 ]

Script: IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG ENDIF
Running: 3 1 OP_EQUAL
Stack: [ 0 <sigVPA> <sigVPB> False ]
```

Debido a que el resultado es "False", la secuencia de comandos ahora se contrae en la declaración "ELSE":

```
Script: 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG
Running: False IF
Stack: [ 0 <sigVPA> <sigVPB> ]

Script: OP_CHECKMULTISIG
Stack: [ 0 <sigVPA> <sigVPB> 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 ]

Script: 
Running: 0 <sigVPA> <sigVPB> 2 <pubKeyVPA> <pubKeyVPB> <pubKeyVPC> 3 OP_CHECKMULTISIG
Stack: [ ]
```

Puede notar que la firma del presidente solo usa un simple `OP_CHECKSIGNATURE` en lugar del código más complejo que generalmente se requiere para un P2PKH. 
Podrían salirse con la suya al incluir la clave pública en el script de bloqueo, obviando el galimatías habitual, porque está codificado  y no se revelará (a través del `redeemScript`) hasta que se desbloquee la transacción. 
Esto también permite que todos los posibles firmantes firmen utilizando la misma metodología.

El único problema posible es si el presidente está distraído y accidentalmente firma una transacción con uno de sus vicepresidentes, porque recuerda que se trata de una firma múltiple de 2 de 3. Una opción es decidir que es una condición de falla aceptable, porque el presidente está usando la multifirma incorrectamente. Otra opción es convertir la multifirma 2 de 3 en una 2 de 4, en caso de que el presidente no tolere la falla: `OP_DEPTH 1 OP_EQUAL IF <pubKeyPres> OP_CHECKSIGNATURE ELSE 2 <pubKeyVPA> <pubKeyVPB> < pubKeyVPC> <pubKeyPres> 4 OP_CHECKMULTISIG ENDIF`. Esto le permitiría al presidente firmar por error con cualquier vicepresidente, pero no afectaría las cosas si dos vicepresidentes quisieran firmar (correctamente).

### Escribe una firma múltiple con un firmante obligatorio

Otra posibilidad multisig implica tener una multifirma m-of-n donde se requiere uno de los firmantes. Por lo general, esto se puede gestionar dividiendo la multifirma en varios m de n-1. Por ejemplo, una firma múltiple de 2 de 3 en la que se requiere uno de los firmantes sería en realidad dos firmas múltiples de 2 de 2, cada una con el firmante requerido.

Aquí hay una forma sencilla de escribir eso:

```
OP_3DUP
2 <pubKeyRequired> <pubKeyA> 2  OP_CHECKMULTISIG
NOTIF

  2 <pubKeyRequired> <pubKeyB> 2  OP_CHECKMULTISIG

ENDIF
```
El script de desbloqueo sería `0 <pubKeyRequired> <pubKeyA>` o `0 <pubKeyRequired> <pubKeyB>`.

Primero, el script verificaría las firmas con `<pubKeyRequired> <pubKeyA>`. Si eso falla, verificaría con `<pubKeyRequired> <pubKeyB>`.

El resultado del `OP_CHECKMULTISIG` final que se ejecutó se dejará en la parte superior de la pila (aunque habrá cruft debajo si el primero tuvo éxito).

## Escribir una firma múltiple de fideicomiso

Hemos hablado mucho sobre fideicomisos. Multifirmas complejas combinadas con bloqueos de tiempo ofrecen una forma automatizada de crearlas de manera robusta.

Imagínese la compradora de vivienda Alice y el vendedor de vivienda Bob que están trabajando con un agente de depósito en garantía. La manera fácil de programar esto sería como una firma múltiple en la que dos de las tres partes podrían liberar el dinero: el vendedor y el comprador están de acuerdo o el agente de depósito en garantía se hace cargo y está de acuerdo con una de las partes: `2 <pubKeyA> <pubKeyB> <pubKeyEscrow> 3 OP_CHECKMULTISG`.

Sin embargo, esto debilita el poder del agente de custodia y permite que el vendedor y el comprador tomen accidentalmente una mala decisión entre ellos, que es una de las cosas que un sistema de custodia está diseñado para evitar. Entonces, podría ser que lo que realmente queremos es el sistema que acabamos de diseñar, donde el agente de custodia es una parte requerida en multifirma 2 de 3: `OP_3DUP 2 <pubKeyEscrow> <pubKeyA> 2 OP_CHECKMULTISIG NOTIF 2 <pubKeyEscrow > <pubKeyB> 2 OP_CHECKMULTISIG ENDIF`.

Sin embargo, esto no pasa la prueba de caminar frente a un autobús. Si el agente de depósito en garantía muere o huye a las Bahamas durante el depósito en garantía, el comprador y el vendedor pierden mucho dinero. Aquí es donde entra en juego un bloqueo de tiempo. Puede crear una prueba adicional que solo se ejecutará si hemos pasado el final de nuestro período de depósito en garantía. En esta situación, permite que el comprador y el vendedor firmen juntos:

```
OP_3DUP
2 <pubKeyEscrow> <pubKeyA> 2 OP_CHECKMULTISIG
NOTIF

  OP_3DUP
  2 <pubKeyEscrow> <pubKeyB> 2 OP_CHECKMULTISIG
  NOTIF

    <+30Days> OP_CHECKSEQUENCEVERIFY OP_DROP
    2 <pubKeyA> <pubKeyB> 2 OP_CHECKMULTISIG
    
  ENDIF
ENDIF
```

Primero, prueba una firma para el comprador y el agente de custodia, luego una firma para el vendedor y el agente de custodia. Si ambos fallan y han pasado 30 días, también permite una firma para el comprador y el vendedor.

### Escriba una firma múltiple de fideicomiso centrada en el comprador

[BIP 112](https://github.com/bitcoin/bips/blob/master/bip-0112.mediawiki#Escrow_with_Timeout) ofrece un ejemplo diferente de este tipo de depósito en garantía que no tiene las protecciones adicionales para evitar que se pierda el agente de depósito en garantía, pero que le da a Alice el control total si falla el depósito en garantía.

```
IF

    2 <pubKeyA> <pubKeyB> <pubKeyEscrow> 3 OP_CHECKMULTISIG 

ELSE

    <+30Days> OP_CHECKSEQUENCEVERIFY OP_DROP
    <pubKeyA> OP_CHECKSIGNATURE

ENDIF
```
Aquí, dos de los tres firmantes pueden liberar el dinero en cualquier momento, pero después de 30 días, Alice puede recuperar su dinero por su cuenta.

Tenga en cuenta que esta secuencia de comandos requiere que se pase un "Verdadero" o un "Falso" para identificar qué rama se está utilizando. Esta es una forma más simple y menos intensiva desde el punto de vista informático para admitir ramas en un script de Bitcoin; es bastante común.

Al principio, se permitiría el siguiente `sigScript`:` 0 <signer1> <signer2> True`. Después de 30 días, Alice podría producir un `sigScript` como este:` <sigA> False`.

## Resumen: escritura de scripts complejos multifirma

Por lo general, se pueden crear firmas múltiples más complejas combinando firmas o firmas múltiples con condicionales y pruebas. Las multifirmas resultantes pueden ser variables, requiriendo diferentes números de firmantes en función de quiénes son y cuándo están firmando.

> :fire: ***¿Cuál es el poder de los scripts multisig complejos?*** Más que todo lo que hemos visto hasta la fecha, los scripts multifirma complejos son contratos verdaderamente inteligentes. Pueden ser muy precisos sobre quién puede firmar y cuándo. Se pueden admitir corporaciones multinivel, asociaciones y depósitos en garantía. El uso de otras funciones poderosas como los bloqueos de tiempo puede proteger aún más estos fondos, lo que permite que se liberen o incluso se devuelvan en determinados momentos.

## ¿Que sigue?

Continúe "Diseñando scripts de Bitcoin reales" con [§13.3: Empoderamiento de Bitcoin con scripts](13_3_Potenciando_Bitcoin_con_Scripts.md).
