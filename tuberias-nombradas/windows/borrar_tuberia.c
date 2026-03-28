#include <stdio.h>
#include <stdlib.h>
#include "tuberias_nombradas.h"

int
main(int argc, char* argv[]) {

  borrar_tuberia(TUBERIA_PETICION);
  borrar_tuberia(TUBERIA_RESPUESTA);

  fprintf(stdout, "Tuberia borradas: %s %s\n",
	  TUBERIA_PETICION,
	  TUBERIA_RESPUESTA);

  return EXIT_SUCCESS;
}
