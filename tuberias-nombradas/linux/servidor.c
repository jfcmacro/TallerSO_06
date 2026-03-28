#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "tuberias_nombradas.h"
#include "servidor.h"

static int consecutivos[MAXIMO + 1] = { 0, 0, 0 };

int
main(int argc, char* argv[]) {

  int df_lectura = -2;
  
  for (;;) {

    if (df_lectura == -2 && (df_lectura = open(TUBERIA_PETICION, O_RDONLY)) == -1) {
      fprintf(stderr, "Error abriendo: %s %d %s",
	      TUBERIA_PETICION, errno, strerror(errno));
      _exit(EXIT_FAILURE);
    }

    char amortiguador_entrada[LONGITUD_MENSAJE_PETICION];
    fprintf(stdout, "Preparando lectura\n");
    ssize_t caracteres_leidos = read(df_lectura, amortiguador_entrada, LONGITUD_MENSAJE_PETICION);
    
    if (caracteres_leidos < -1) {
      close(df_lectura);
      break;
    }

    if (caracteres_leidos > 0) {

      amortiguador_entrada[FIN_PID] = amortiguador_entrada[LONGITUD_MENSAJE_PETICION - 1] = '\0';
      pid_t proceso_cliente = atoi(amortiguador_entrada);
      Consecutivo_t consecutivo = atoi(&amortiguador_entrada[FIN_PID + 1]);

      fprintf(stdout, "Peticion: %d en la cola: %d\n", proceso_cliente, consecutivo);

      int df_escritura;

      if ((df_escritura = open(TUBERIA_SOLICITUD, O_WRONLY)) == -1) {
	fprintf(stderr, "Error abriendo: %s %d %s",
		TUBERIA_SOLICITUD, errno, strerror(errno));
	_exit(EXIT_FAILURE);
      }

      char amortiguador_salida[LONGITUD_MENSAJE_SOLICITUD];

      bzero(amortiguador_salida, LONGITUD_MENSAJE_SOLICITUD);
      sprintf(amortiguador_salida, "%06d %06d\n", proceso_cliente, consecutivos[consecutivo]++);
      write(df_escritura, amortiguador_salida, LONGITUD_MENSAJE_SOLICITUD);

      close(df_escritura);
    }
    else {
      fprintf(stdout, "No se leyeron caracteres %d.\n", caracteres_leidos);
      close(df_lectura);
      df_lectura = -2;
    }
  }

  return EXIT_SUCCESS;
}
