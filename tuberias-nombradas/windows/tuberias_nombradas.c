#include <windows.h>
#include "tuberias_nombradas.h"

void crear_tuberia(const char* nombre) {
  /* CreateNamedPipeA(nombre, */
		   
  /* int retValue = mkfifo(nombre, 0666); */

  /* if (retValue == -1) { */
  /*   fprintf(stderr, "No se pudo crear la tuberia: %s debido a %d %s\n", */
  /* 	    nombre, errno, strerror(errno)); */
  /*   _exit(EXIT_FAILURE); */
  /* } */
}

void borrar_tuberia(const char* nombre) {
  /* int retValue = unlink(nombre); */

  /* if (retValue == -1) { */
  /*  fprintf(stderr, "No se pudo borrar la tuberia: %s debido a %d %s\n", */
  /* 	    nombre, errno, strerror(errno)); */
  /*   _exit(EXIT_FAILURE); */
  /* }  */
}
