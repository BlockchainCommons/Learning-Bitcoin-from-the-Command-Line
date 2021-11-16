# 16.2: Programación de Bitcoind en C con bibliotecas RPC 

> :information_source: **NOTA:** Esta sección se agregó recientemente al curso y es un borrador inicial que aún puede estar pendiente de revisión. Lector de advertencias.

[§16.1](16_1_Accediendo_a_Bitcoind_en_C_con_las_Bibliotecas_RPC.md) presentó la metodología para crear programas en C utilizando bibliotecas RPC y JSON. Ahora vamos a mostrar el potencial de esas bibliotecas C presentando un primer corte simplista de un programa Bitcoin real.

## Planifique su código

Esta sección creará una versión simplista de primer corte de `sendtoaddress`, que permitirá al usuario enviar dinero a una dirección siempre que tenga un UTXO suficientemente grande. Esto es lo que debemos hacer:

  1. Solicite una dirección y una cantidad
  2. Establecer una tarifa arbitraria  
  3. Prepare su RPC
  4. Encuentre un UTXO que sea lo suficientemente grande para la cantidad + la tarifa
  5. Cree una dirección de cambio
  6. Cree una transacción sin procesar que envíe desde la UTXO a la dirección y la dirección de cambio
  7. Firma la transacción
  8. Envía la transacción
 
### Planifique su futuro

Dado que este es su primer programa funcional en C, intentaremos mantenerlo simple (KISS). Si estuviéramos creando un programa de producción real, al menos querríamos hacer lo siguiente:

   1. Pruebe y / o revise las entradas
   2. Calcule una tarifa automáticamente
   3. Piense lógicamente sobre qué UTXO válido usar
   4. Combine múltiples UTXO si es necesario
   5. Esté atento a más errores en los comandos `libbitcoinrpc` o` jansson`
   6. Esté atento a los errores en las respuestas de RPC

Si desea continuar expandiendo este ejemplo, abordar las deficiencias de este programa de ejemplo sería un excelente lugar para comenzar.

## Escriba su software de transacciones

Ahora esta listo para emprender ese plan paso a paso.

### Paso 1: Solicite una dirección y una cantidad

Ingresar la información es bastante fácil a través de argumentos de línea de comando:

```
if (argc != 3) {

  printf("ERROR: Only %i arguments! Correct usage is '%s [recipient] [amount]'\n",argc-1,argv[0]);
  exit(-1);

}

char *tx_recipient = argv[1];
float tx_amount = atof(argv[2]);

printf("Sending %4.8f BTC to %s\n",tx_amount,tx_recipient);
```

> :warning: **ADVERTENCIA:** Un programa real necesitaría una refactorización mucho mejor de estas variables.

### Paso 2: Establecer una tarifa arbitraria

En este ejemplo, solo una tarifa arbitraria de 0.0005 BTC para garantizar que las transacciones de prueba se realicen rápidamente:

```
float tx_fee = 0.0005;
float tx_total = tx_amount + tx_fee;
```

> :warning: **ADVERTENCIA:** Un programa real calcularía una tarifa que minimiza el costo al tiempo que garantiza que la velocidad sea suficiente para el remitente.

### Paso 3: Prepare su RPC

Obviamente, necesitará tener todas sus variables listas nuevamente, como se discutió en [§16.1: Acceso a Bitcoind con C](16_1_Accediendo_a_Bitcoind_en_C_con_las_Bibliotecas_RPC.md). También necesita inicializar su biblioteca, conectar su cliente RPC y preparar su objeto de respuesta:

```
bitcoinrpc_global_init();
rpc_client = bitcoinrpc_cl_init_params ("bitcoinrpc", "YOUR-RPC-PASSWD", "127.0.0.1", 18332);
btcresponse = bitcoinrpc_resp_init();
```

### Paso 4: Encuentre un UTXO

Para encontrar un UTXO debe llamar al RPC `listunspent`:

```
rpc_method = bitcoinrpc_method_init(BITCOINRPC_METHOD_LISTUNSPENT);
bitcoinrpc_call(rpc_client, rpc_method, btcresponse, &btcerror);
```

Sin embargo, el verdadero trabajo consiste en decodificar la respuesta. La sección anterior señalaba que la biblioteca `jansson` era "algo torpe" y por eso: tiene que crear (y borrar) un conjunto muy grande de objetos `json_t` para buscar lo que quiere.

Primero, debe recuperar el campo `result` de JSON:

```
json_t *lu_response = NULL;
json_t *lu_result = NULL;

lu_response = bitcoinrpc_resp_get (btcresponse);
lu_result = json_object_get(lu_response,"result");
```

> :warning: **ADVERTENCIA:** Solo obtiene un resultado si no hubo un error. Aquí hay otro lugar para una mejor verificación de errores del código de producción.


Luego, ingresa en un ciclo, examinando cada transacción no gastada, que aparece como un elemento en su matriz de resultados JSON:

```
int i;

const char *tx_id = 0;
int tx_vout = 0;
double tx_value = 0.0;

for (i = 0 ; i < json_array_size(lu_result) ; i++) {

  json_t *lu_data = NULL;
  lu_data = json_array_get(lu_result, i);
  
  json_t *lu_value = NULL;
  lu_value = json_object_get(lu_data,"amount");
  tx_value = json_real_value(lu_value);
```

¿Es el UTXO lo suficientemente grande para pagar su transacción? Si es así, ¡selecciónelo!

> :warning: **ADVERTENCIA:** Un programa del mundo real pensaría más detenidamente sobre qué UTXO tomar, según el tamaño y otros factores. Probablemente no tomaría lo primero que vio que funcionó.

```
  if (tx_value > tx_total) {

    json_t *lu_txid = NULL;
    lu_txid = json_object_get(lu_data,"txid");
    tx_id = strdup(json_string_value(lu_txid));

    json_t *lu_vout = NULL;
    lu_vout = json_object_get(lu_data,"vout");
    tx_vout = json_integer_value(lu_vout);

    json_decref(lu_value);
    json_decref(lu_txid);
    json_decref(lu_vout);
    json_decref(lu_data);
    break;

  } 
```

También debe borrar sus elementos JSON principales:

```
}

json_decref(lu_result);
json_decref(lu_response);
```  

> :warning: **ADVERTENCIA:** Un programa del mundo real también aseguraría que los UTXO fueran "gastables".

Si no encontró ningun UTXO lo suficientemente grande, tendrá que informar ese triste hecho al usuario ... y tal vez sugerirle que use un programa mejor que combine correctamente las UTXO.

```
if (!tx_id) {

  printf("Very Sad: You don't have any UTXOs larger than %f\n",tx_total);
  exit(-1);
}
```

> **ADVERTENCIA:** Un programa real usaría subrutinas para este tipo de búsqueda, por lo que podría llamar a varias RPC de una biblioteca de funciones C. Simplemente vamos a ponerlo todo en "main" como parte de nuestra filosofía KISS de ejemplos simples.


### Paso 5: Crea una dirección de cambio

Repita la metodología de búsqueda RPC estándar para obtener una dirección de cambio:

```
rpc_method = bitcoinrpc_method_init(BITCOINRPC_METHOD_GETRAWCHANGEADDRESS);

if (!rpc_method) {

  printf("ERROR: Unable to initialize listunspent method!\n");
  exit(-1);

}

bitcoinrpc_call(rpc_client, rpc_method, btcresponse, &btcerror);

if (btcerror.code != BITCOINRPCE_OK) {

printf("Error: listunspent error code %d [%s]\n", btcerror.code,btcerror.msg);

  exit(-1);

}

lu_response = bitcoinrpc_resp_get (btcresponse);
lu_result = json_object_get(lu_response,"result");
char *changeaddress = strdup(json_string_value(lu_result));
```

La única diferencia es qué información particular se extrae del objeto JSON.

> :warning: **ADVERTENCIA:** Aquí hay un lugar en el que una subrutina sería realmente agradable: abstraer toda la inicialización y llamada del método RPC.


### Paso 6: Crea una transacción sin procesar

La creación de la transacción sin procesar real es la otra parte complicada de la programación de su reemplazo de `sendtoaddress`. Eso es porque requiere la creación de un objeto JSON complejo como parámetro.

Para crear correctamente estos parámetros, deberá revisar lo que espera el RPC `createrawtransaction`. Afortunadamente, esto es fácil de determinar usando la funcionalidad `bitcoin-cli help`:

```
$ bitcoin-cli help createrawtransaction
createrawtransaction [{"txid":"id","vout":n},...] {"address":amount,"data":"hex",...} ( locktime )
```

Para revisar, sus entradas serán una matriz JSON que contiene un objeto JSON para cada UTXO. Entonces, todas las salidas estarán en un objeto JSON. Es más fácil crear estos elementos JSON desde adentro hacia afuera, usando comandos `jansson`.

#### Paso 6.1: Crear los parámetros de entrada


Para crear el objeto de entrada para su UTXO, use `json_object`, luego rellénelo con valores-clave usando `json_object_set_new` (para referencias recién creadas) o `json_object_set` (para referencias existentes):

```
json_t *inputtxid = NULL;
inputtxid = json_object();

json_object_set_new(inputtxid,"txid",json_string(tx_id));
json_object_set_new(inputtxid,"vout",json_integer(tx_vout));
```

Notará que nuevamente tiene que traducir cada tipo de variable C en un tipo de variable JSON usando la función apropiada, como `json_string` o `json_integer`.

Para crear la matriz de entrada general para todos sus UTXO, use `json_array`, luego llénelo con objetos usando  `json_array_append`:

```
json_t *inputparams = NULL;
inputparams = json_array();
json_array_append(inputparams,inputtxid);
```

#### Paso 6.2: Crear los parámetros de salida

Para crear la matriz de salida para su transacción, siga el mismo formato, cree un objeto JSON con `json_object`, luego llénelo con `json_object_set`:

```
json_t *outputparams = NULL;
outputparams = json_object();

char tx_amount_string[32];
sprintf(tx_amount_string,"%.8f",tx_amount);
char tx_change_string[32];
sprintf(tx_change_string,"%.8f",tx_value - tx_total);

json_object_set(outputparams, tx_recipient, json_string(tx_amount_string));
json_object_set(outputparams, changeaddress, json_string(tx_change_string));
```

> :warning:**ADVERTENCIA:** Puede esperar ingresar sus valores de Bitcoin como números, usando `json_real`. Desafortunadamente, esto expone uno de los principales problemas con la integración de la biblioteca `jansson` y Bitcoin. Bitcoin solo es válido hasta ocho dígitos significativos después del punto decimal. Puede recordar que .00000001 BTC es un satoshi, y esa es la división más pequeña posible de un Bitcoin. Los dobles en C ofrecen dígitos más significativos que eso, aunque a menudo son imprecisos más allá de los ocho decimales. Si intenta convertir directamente de su valor doble en C (o un valor flotante, para el caso) a un valor de Bitcoin, la imprecisión a menudo creará un valor de Bitcoin con más de ocho dígitos significativos. Antes de Bitcoin Core 0.12, esto parece funcionar, y podría usar `json_real`. Pero a partir de Bitcoin Core 0.12, si intenta darle a `createrawtransaction` un valor de Bitcoin con demasiados dígitos significativos, obtendrá un error y la transacción no se creará. Como resultado, si el valor de Bitcoin _ever_ se ha convertido en un doble o flotante, debe reformatearlo a ocho dígitos significativos después del dígito antes de introducirlo como una cadena. Obviamente, esto es una tontería, por lo que debe asegurarse de que continúe funcionando en futuras versiones de Bitcoin Core.

#### Paso 6.3: Crear la matriz de parámetros

Para terminar de crear sus parámetros, simplemente agrúpelos todos en una matriz JSON:

```
json_t *params = NULL;
params = json_array();
json_array_append(params,inputparams);
json_array_append(params,outputparams);
```

#### Paso 6.4 Realizar la llamada RPC

Utilice el método normal para crear su llamada RPC:

```
rpc_method = bitcoinrpc_method_init(BITCOINRPC_METHOD_CREATERAWTRANSACTION);
```

Sin embargo, ahora debe alimentarlo con sus parámetros. Esto simplemente se hace con `bitcoinrpc_method_set_params`:

```
if (bitcoinrpc_method_set_params(rpc_method, params) != BITCOINRPCE_OK) {

  fprintf (stderr, "Error: Could not set params for createrawtransaction");

}
```

Después, ejecute el RPC y obtenga los resultados como de costumbre:

```
bitcoinrpc_call(rpc_client, rpc_method, btcresponse, &btcerror);

lu_response = bitcoinrpc_resp_get(btcresponse);
lu_result = json_object_get(lu_response,"result");

char *tx_rawhex = strdup(json_string_value(lu_result));
```

### Paso 7. Firma la transacción

Es mucho más fácil asignar un parámetro simple a una función. Simplemente crea una matriz JSON y luego asigna el parámetro a la matriz:

```
params = json_array();
json_array_append_new(params,json_string(tx_rawhex));
```

Firme la transacción siguiendo el galimatías típico para crear una llamada RPC:

```
rpc_method = bitcoinrpc_method_init(BITCOINRPC_METHOD_SIGNRAWTRANSACTION);
if (bitcoinrpc_method_set_params(rpc_method, params) != BITCOINRPCE_OK) {

  fprintf (stderr, "Error: Could not set params for signrawtransaction");

}

json_decref(params);

bitcoinrpc_call(rpc_client, rpc_method, btcresponse, &btcerror);
lu_response = bitcoinrpc_resp_get(btcresponse);
```

Nuevamente, usar `jansson` para acceder a la salida puede ser un poco complicado. Aquí debe recordar que `hex` es parte de un objeto JSON, no un resultado independiente, como fue el caso cuando creó la transacción sin procesar. Por supuesto, siempre puede acceder a esta información desde la ayuda de la línea de comandos: `bitcoin-cli help signrawtransaction`:

```
lu_result = json_object_get(lu_response,"result");
json_t *lu_signature = json_object_get(lu_result,"hex");
char *tx_signrawhex = strdup(json_string_value(lu_signature));
json_decref(lu_signature);
```

> :warning: ***ADVERTENCIA:*** Un programa del mundo real obviamente probaría cuidadosamente la respuesta de cada comando RPC para asegurarse de que no haya errores. Eso es especialmente cierto para `signrawtransaction`, porque podría terminar con una transacción parcialmente firmada. Peor aún, si no verifica los errores en el objeto JSON, solo verá el `hex` y no se dará cuenta de que está sin firmar o parcialmente firmado.

### Paso 8. Envíar la transacción

Ahora puede enviar su transacción, utilizando todas las técnicas anteriores:

```
params = json_array();
json_array_append_new(params,json_string(tx_signrawhex));

rpc_method = bitcoinrpc_method_init(BITCOINRPC_METHOD_SENDRAWTRANSACTION);

if (bitcoinrpc_method_set_params(rpc_method, params) != BITCOINRPCE_OK) {

  fprintf (stderr, "Error: Could not set params for sendrawtransaction");

}

json_decref(params);

bitcoinrpc_call(rpc_client, rpc_method, btcresponse, &btcerror);
lu_response = bitcoinrpc_resp_get(btcresponse);
lu_result = json_object_get(lu_response,"result");

char *tx_newid = strdup(json_string_value(lu_result));

printf("Txid: %s\n",tx_newid);
```

El código completo, con un poco más de comprobación de errores, aparece en el Apéndice.


## Pruebe su código


El código completo se puede encontrar en el [directorio src](../src/16_2_sendtoaddress.c).

Compile esto como de costumbre:

```
$ cc sendtoaddress.c -lbitcoinrpc -ljansson -o sendtoaddress
```
Luego puede usarlo para enviar fondos a una dirección:

```
./sendtoaddress tb1qynx7f8ulv4sxj3zw5gqpe56wxleh5dp9kts7ns .001
Txid: b93b19396f8baa37f5f701c7ca59d3128144c943af5294aeb48e3eb4c30fa9d2
```

Puede ver información sobre esta transacción que enviamos [aquí](https://live.blockcypher.com/btc-testnet/tx/b93b19396f8baa37f5f701c7ca59d3128144c943af5294aeb48e3eb4c30fa9d2/).

## Resumen: Programación de Bitcoind con C

Con acceso a una biblioteca C, puede crear programas con muchas más funciones de lo que era razonable hacerlo con scripts de shell. ¡Pero puede requerir mucho trabajo! Incluso con 316 líneas de código, `sendtoaddress.c` no cubre casi todas las complejidades necesarias para realizar transacciones de bitcoins de forma segura e inteligente.

## ¿Que sigue?

Obtenga más información sobre "Hablar con Bitcoind con C" en [16.3: Recibir notificaciones en C con bibliotecas ZMQ](16_3_Recibiendo_Notificaciones_de_Bitcoind_en_C_con_las_Bibliotecas_ZMQ.md).
