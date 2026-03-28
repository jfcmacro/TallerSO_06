# Taller 6

## Agenda

1. Preliminares
2. `tmux`
3. Tuberías nombradas
4. Proceso demonios
5. Registro de eventos (log)

## Preliminares

Abra una terminal. Abra el directorio de repositorio y de los talleres

```
cd <repositorio-enlace>
cd talleres
```

Descargue el taller en formato zip, descomprimalo.

```
wget https://github.com/jfcmacro/TallerSO_06/archive/refs/heads/master.zip
unzip master.zip
rm master.zip
```

Mire la estructura actual

```
tree .
```

Entre al directorio del taller

```
cd TallerSO_06-master
```

Adicione los ficheros

```bash
git add README.md .gitignore
```

Adicione los ficheros del proyecto, esto adiciona todos ficheros del taller.

```bash
find . -name *.c -exec git add {} \;
find . -name *.h -exec git add {} \;
find . -name .keep -exec git add {} \;
find . -name makefile -exec git add {} \;
find . -name *.mk -exec git add {} \;
```

Acometa (*commit*) el proyecto.

```
git commit -m "Iniciando el Taller 06"
git push
```

## `tmux`

* [A Beginner's Guide to `tmux`](https://medium.com/pragmatic-programmers/a-beginners-guide-to-tmux-7e6daa5c0154)
* [Introduction `tmux` linux](https://www.redhat.com/en/blog/introduction-tmux-linux) 

Abra una sesión en `tmux`

```bash
tmux
```

> Explicación del profesor

## Tuberías nombradas

### Linux

* [`mkfifo`(3)](https://man7.org/linux/man-pages/man3/mkfifo.3.html)
* [`unlink`(2)](https://man7.org/linux/man-pages/man2/unlink.2.html)
* [`open`(2)](https://man7.org/linux/man-pages/man2/open.2.html)
* [`read`(2)](https://man7.org/linux/man-pages/man2/read.2.html)
* [`write`(2)](https://man7.org/linux/man-pages/man2/read.2.html)

Abrir una terminal e ir al directorio donde esta guardado el taller:

```bash 
cd tuberias-nombradas/linux
makefile
```

> Explicación. Profesor tuberías nombradas linux

Crear las tuberias

```bash
./crear_tuberia
```

Observar las tuberías creadas.

```bash
ls -l /tmp/tuberia*
```

Divida la terminal de `tmux`  en dos:

Y ejecute en la primera el servidor:

```bash
./servidor
```

En la segunda ejecute el cliente

```bash
./cliente 1
```

**[Ejercicio 1].** (Carpeta: `./tuberias-nombradas/linux/` Nombre: `servidor2.c`)

En primer lugar copie el servidor original (`servidor.c`)

```bash
# En la carpeta ./tuberia-nombradas/linux/
cp servidor.c servidor2.c
```

* Formato: `servidor2 [-c] [-p <peticion-nombre>] [-s <solicitud-nombre>]`
* Descripción: El `servidor2` utiliza los nombres de los tuberías preestablecidas (`/tmp/tuberia_peticion`  y `/tmp/tuberia_solicitud`) si no se le pasa argumento alguno. Si al `servidor2` se le pasa la opción `-c`  este se encarga de crear las tuberías, las opciones `-p`  y `-s` le permiten nombrar las tuberías. El servidor también debe aceptar la señal de terminación  `SIGQUIT` que termina el `servidor2` y borrar las tuberías nombradas.
* **Nota:** No olvide modificar el fichero `makefile`

**[Ejercicio 2].** (Carpeta: `./tuberias-nombradas/linux/` Nombre: `cliente2.c`)

En primer lugar copie el cliente original (`cliente.c`)

```bash
# En la carpeta ./tuberia-nombrada/linux/
cp cliente.c cliente2.c
```

* Formato: `cliente2 [-c] [-p <peticion-nombre>] [-s <solicitud-nombre>]`
* Descripción: El `cliente2` utiliza los nombres de tuberías preestablecidas (`/tmp/tuberia_peticion`  y `/tmp/tuberia_solicitud`) si no se le pasa argumento alguno. Si al `servidor2` las opciones `-p`  y `-s` le permiten utilizar otras nombres de tuberías nombradas.
* **Nota**: No olvidar modificar el fichero `makefile`

### Windows

* [`CreateNamedPipeA`](https://learn.microsoft.com/es-es/windows/win32/api/winbase/nf-winbase-createnamedpipea)
* [`ConnectNamedPipe`](https://learn.microsoft.com/es-es/windows/win32/api/namedpipeapi/nf-namedpipeapi-connectnamedpipe)
* [`ReadFile`]()
* [`WriteFile`]()
* [`CreateFile`]()
* [`WaitNamedPipe`]()
* [`CloseHandle`]()

Abrir una terminal e ir al directorio donde esta guardado el taller:

```bash 
cd tuberias-nombradas/linux
makefile servidor_example cliente_example
```

> Explicación. Tuberías nombradas

## Procesos demonios - `daemon` (procesos en sesión propia)

Un proceso demonio (o servicio) es un proceso que no está asociado a una sesión de control. Generalmente, todos los proceso que abrimos en consola esta bajo una misma sesión de control. Si esta sesión termina, todos los procesos perteneciente a la sesión son terminados. Un proceso dentro de un sesión se puede escapar, creando su propia sesión, en este caso el proceso se convierte en un proceso demonio (*daemon*).
Este proceso esta en su propia sesión y se convierte en un proceso de segundo plano.

### Linux

* [`fork`(2)](https://man7.org/linux/man-pages/man2/fork.2.html)
* [`setsid`(2)](https://man7-org.translate.goog/linux/man-pages/man2/setsid.2.html)
* [`chdir`(2)](https://man7-org.translate.goog/linux/man-pages/man2/chdir.2.html)
* [`close`(2)](https://man7.org/linux/man-pages/man2/close.2.html)

* [Conectar procesos](./conectar-procesos/linux/conectar-procesos.c)

> Explicación procesos demonios en linux

**[Ejercicio 3]**. (Carpeta: `./proceso-demonio/linux` Fichero: `servidor3.c`)

Copie la versión anterior:

```bash
# En la carpeta ./proceso-demonio/linux
cp ../../../tuberias-nombradas/linux/servidor2.c servidor3.c
```

* Formato: `servidor3 [-d] [-c] [-p <peticion-nombre>] [-s <solicitud-nombre>]`
* Descripción: El `servidor3` utiliza los nombres de los tuberías preestablecidas (`/tmp/tuberia_peticion`  y `/tmp/tuberia_solicitud`) si no se le pasa argumento alguno. Si al `servidor3` se le pasa la opción `-c`  este se encarga de crear las tuberías, las opciones `-p`  y `-s` le permiten nombrar las tuberías. El servidor también debe aceptar la señal de terminación  `SIGQUIT` que termina el `servidor2` y borrar las tuberías nombradas. El `servidor3`acepta la opción `-d` que permite que el proceso se convierta en un proceso demonio, que permite que el `cliente` del ejercicio anterior se pueda conectar con este servidor, mientras aparece desconectado.
* **Nota**: No olvidar modificar el fichero `makefile`

### Windows

Windows en sus versiones originales permitia que los proceso consola pudieran desconectarse de su sesión gráficada dominada por el programa [`WinLogon`](https://learn.microsoft.com/es-es/windows/win32/secauthn/winlogon).  A partir de la versión Windows Vista, se encontró que este mecanismo podría conducir a riesgos puesto que un proceso de servicio en Windows podría acceder a funciones del kernel. Ya no es posible hacerlo directamente.

> Explicación procesos demonios (servicios) en windows

## Registro de Eventos

### Linux

* [`openlog`(3p)](https://man7.org/linux/man-pages/man3/openlog.3p.html)
* [`syslog(3)`](https://man7.org/linux/man-pages/man3/syslog.3.html)
* [`closelog`(3p)](https://man7.org/linux/man-pages/man3/syslog.3.html)

> Explicación de registro de eventos en linux

**[Ejercicio 4]**. (Carpeta: `./registro-servicio/linux` Fichero: `servidor4.c`)

Copie la versión anterior:

```bash
# En la carpeta ./registro-servicio
cp ../../proceso-demonio/linux/servidor3.c servidor4.c
```

* Formato: `servidor4 [-d] [-c] [-p <peticion-nombre>] [-s <solicitud-nombre>]`
* Descripción: El `servidor4` utiliza los nombres de los tuberías preestablecidas (`/tmp/tuberia_peticion`  y `/tmp/tuberia_solicitud`) si no se le pasa argumento alguno. Si al `servidor3` se le pasa la opción `-c`  este se encarga de crear las tuberías, las opciones `-p`  y `-s` le permiten nombrar las tuberías. El servidor también debe aceptar la señal de terminación  `SIGQUIT` que termina el `servidor2` y borrar las tuberías nombradas. El `servidor3`acepta la opción `-d` que permite que el proceso se convierta en un proceso demonio, que permite que el `cliente` del ejercicio anterior se pueda conectar con este servidor, mientras aparece desconectado. Este servidor imprime los mensajes de inicio, de petición de cada cliente indicado que cliente se ha conectado y la petición, como también el mensaje de terminación todo ello escrito a través del `syslog`.
* **Nota**: No olvidar modificar el fichero `makefile`

### Windows

* [`RegisterEventSourceA`](https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-registereventsourcea)
* [`ReportEventA`](https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-registereventsourcea)
* [`DeregisterEventSource`](https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-deregistereventsource)



​	
