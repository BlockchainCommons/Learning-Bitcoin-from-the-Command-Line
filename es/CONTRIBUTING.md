# Contribuyendo

¡Nos encanta su aporte! Queremos hacer que contribuir a este proyecto sea tan
fácil y transparente como sea posible, si se trata de:

- Reportar un bug
- Discutir el estado actual del código.
- Enviar una solución
- Proponer nuevas características.
- Convertirse un mantenedor

## Nosotros desarrollamos con Github

Usamos Github para alojar código, rastrear problemas y pedidos de
características, y para aceptar "pull request".

## Reporte bugs usando los
[issues](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/issues)
de Github

Si usted encuentra bugs, errores, o inconsistencias en el código o documentos
de este proyecto, por favor, háganos lo saber 
[abriendo un nuevo issue](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/issues/new),
pero considere buscar a través de los issues existentes primero para comprobar
y ver si el problema ya ha sido reportado. Si es así, nunca hace daño agregar
un rápido "+1" o "Yo tengo el mismo problema también". Esto ayuda a priorizar
los problemas y solicitudes mas comunes.

### Escriba reportes de bugs con detalle, contexto y código de ejemplo

[Este es un ejemplo](http://stackoverflow.com/q/12488905/180626) de un buen
reporte de un bug por parte de @briandk. Aquí hay 
[otro ejemplo de craig.hockenberry](http://www.openradar.me/11905408).

**Buenos reportes de bugs** suelen tener:

- Un rápido resumen o contexto
- Pasos para reproducirlo
    - ¡Sea especifico!
    - Facilite código de muestra si puede. [El reporte de bugs de stackoverflow](http://stackoverflow.com/q/12488905/180626) 
      incluye muestras de codigo que *cualquiera* con una configuracion basica
      de R puede correr para reproducir lo que estoy viendo.
- Lo que esperaba que pasara
- Lo que realmente sucedió
- Notas (posiblemente incluyendo porque usted cree que esto pueda estar
  pasando, o cosas que intento y no funcionaron)

La gente *ama* los reportes de bugs exhaustivos. No es broma.

## Envíe cambios en el código a través de "pull requests"

"pull requests" simples para arreglar errores de tipeo, para documentar, o para corregir
bugs pequeños son siempre bienvenidos.

Nosotros pedimos que mejoras mas significativas al proyecto sean primero
propuestas antes de empezar a programarlas como un 
[issue](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/issues)
o como un 
[borrador de "pull request"](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/pulls), 
el cual es una 
[linda característica nueva](https://github.blog/2019-02-14-introducing-draft-pull-requests/) 
que le da a otros contribuidores la oportunidad de dirigirte en la dirección
correcta, darte devoluciones en el diseño, e incluso quizás discutir si ya hay
trabajo relacionado en camino.

### Use un estilo de código consistente

* Nosotros aplicamos sangría usando dos espacios ("soft tabs")
* Nosotros SIEMPRE ponemos espacios luego de los elementos de listas y los
  parámetros de los métodos ([1, 2, 3], no [1,2,3]), alrededor de operadores 
  (x += 1, no x+=1), y alrededor de las "hash arrows".
* Este es código abierto. Considere que la gente leerá su código, y
  hágalo ver lindo para ellos. Es como manejar un auto: Por ahí usted ama hacer
  trompos cuando esta solo, pero con pasajeros el objetivo es hacer el paseo lo
  mas suave posible.

### Use [Github Flow](https://guides.github.com/introduction/flow/index.html)
para las "pull requests"

Usamos [Github Flow](https://guides.github.com/introduction/flow/index.html).
Cuando envíe una "pull request", por favor:

1. Haga un "fork" del repositorio y cree su rama con base en `master`.
2. Si usted ha agregado código que debe ser probado, agregue test.
3. Si usted ha cambiado APIs, actualice la documentación.
4. Asegúrese de que su código pasa su conjunto de pruebas.
5. Asegúrese que su código no tenga errores.
6. ¡Envíe su "pull request"!

### Envíe bajo la licencia "BSD-2-Clause Plus Patent"

En resumen, cuando envía sus cambios en el código, se entiende que lo enviado
estará disponible bajo la misma licencia [CC-BY](../LICENSE-CC-BY-4.0.md) que
cubre este proyecto. Nosotros también solicitamos a todos los contribuyentes al
código que firmen mediante la herramienta GPG el 
[Acuerdo de Licencia del Contribuidor (CLA.md)](../CLA.md) para proteger a
futuros usuarios de este proyecto. Siéntase libre de contactar a los
mantenedores si eso fuera un problema.

## Referencias

Porciones de este documento, CONTRIBUYENDO.md, fueron adoptadas de las mejores
practicas de un numero de proyectos de código abierto, incluyendo:
* [Facebook's Draft](https://github.com/facebook/draft-js/blob/a9316a723f9e918afde44dea68b5f9f39b7d9b00/CONTRIBUTING.md)
* [IPFS Contributing](https://github.com/ipfs/community/blob/master/CONTRIBUTING.md)
