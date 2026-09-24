## Servicio Windows

Un servicio en Windows es un proceso de consola que se ejecuta en su
propia sesion, independiente de estar conectado al sistema de sesion
actual conocido como `WinLogon`.

Para crear dichos servicios, no se puede hacer como en Linux donde un
proceso, se puede desconectar de su sesión y permanecer como un
proceso demonio. Anteriormente esto ese podía hacer pero a partir
de la versión de Windows Vista esto se cambio debido a problemas de
seguridad.

Los servicios están desconectados de las sesiones no están asociados a
una ventana o a una consola, por lo tanto no se puede obtener, ni
generar salida. Esto requiere que para comunicarse se debe utilizar
otros medios.

Para controlar el servicio utilizaremos a SCM (Service Control
Management). Este permite registrar, desregistrar, iniciar, terminar,
suspender o reasumir un servicio. Este es un programa que esta basado
en RPC (Remote Procedure Control) que permite acceder realizar todas
esas tareas.

Logicamente, se debe crear el servicio primero y después realizar su
registro, un después si el usuario así lo quiere desregistrarlo.

### Servicio

Un [servicio](./svrwindows.c) no es un programa de consola
normal. Este tiene un punto de entrada `main`, que es llamado para
registrar una tabla despacho, esta tabla contiene la información de
los servicios que pueden ser manejados. El SCM permite tener más de un
servicio corriendo al mismo tiempo dentro del espacio de un servicio.
En nuestro ejemplo tendremos solo un servicio, y la tabla señala cual
es la función que controla el servicio (`servicio_ppal`), este puede
ejecutar y controlar el servicio, en nuestro caso vamos a lanzar un
hilo para que controle el servicio.

Pero tambien requiere que se registre una función que será llamada por
el SCM, para indicar que ha recido las diferentes operaciones sobre el
servicio: `START`, `STOP`, `PAUSE`, `CONTINUE`, y `SHUTDOWN`.

### Registrar un servicio

El programa [`registrar_servicio`](./registrar_servicio.c), permite
registrar un servicio programaticamente en el register, este programa
abre primero una conexión al administrador SCM, y luego crea el
servicio.

Normalmente un usuario final no lo puede hacer, solo aquellos usarios
que este registrados como administradores lo pueden hacer. Para ello
se debe abrir una consola en modo administrador y ejecutar el programa
que registrara el servicio.

### Desregistrar un servicio

El programa [`desregistrar_servicio`](./desregistrar_servicio.c),
permite borrar el servicio previamente instalado en el register.

