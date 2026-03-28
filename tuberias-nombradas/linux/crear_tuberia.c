#include <stdio.h>
#include <stdlib.h>
#include "tuberias_nombradas.h"

int
main(int argc, char* argv[]) {

  crear_tuberia(TUBERIA_PETICION);
  crear_tuberia(TUBERIA_SOLICITUD);

  fprintf(stdout, "Creada tuberia: %s %s\n",
	  TUBERIA_PETICION,
	  TUBERIA_SOLICITUD);
  
  return EXIT_SUCCESS;
}
