# Servicios en Linux

El registro se Linux se hace a través del `systemd`.  `systemd` es un
sistema y administrador de servicios para el sistema operativo Linux.
Son varias las unidades que controla el `systemd`, desde relojes, directorios
especiales como: `/sys/`, `/proc/` y `/dev/`, hasta servicios de usuarios.

En este taller vamos a instalar un servicio de usuario y a controlarlo
a través del `systemd`.

## Archivo de configuración `.service`

Es un fichero de configuración de [tipo
ini](https://en.wikipedia.org/wiki/INI_file). La descripción completa
de las secciones y de las claves se encuentra en [`man
systemd.service(5)`](https://man7.org/linux/man-pages/man5/systemd.service.5.html).

El fichero [`systemd-service.service`](./systemd-service.service)
contiene tres secciones `[Unit]`, `[Service]` e `[Install]`. En la
primera se describe la unidad y su propósito. Las segunda, describe el
servicio en si, con el tipo, ejecutable y forma de ejecutar el
programa, hasta el directorio de trabajo donde estará ubicado. La
tercera sección, describe la forma que será instalado en el sistema.

## El servicio

El fichero [`systemd-service.c`](./systemd-service.c) contiene un
proceso demonio, este será ejecutado como un proceso de fondo
(*background process*). Para ellos es importante manejar las señales,
y el registro del sistema.