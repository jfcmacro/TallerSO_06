#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "tuberias_nombradas.h"

void crear_tuberia(const char* nombre) {
  int retValue = mkfifo(nombre, 0666);

  if (retValue == -1) {
    fprintf(stderr, "No se pudo crear la tuberia: %s debido a %d %s\n",
	    nombre, errno, strerror(errno));
    _exit(EXIT_FAILURE);
  }
}

void borrar_tuberia(const char* nombre) {
  int retValue = unlink(nombre);

  if (retValue == -1) {
   fprintf(stderr, "No se pudo borrar la tuberia: %s debido a %d %s\n",
	    nombre, errno, strerror(errno));
    _exit(EXIT_FAILURE);
  } 
}
