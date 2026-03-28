#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "tuberias_nombradas.h"
#include "servidor.h"

void uso(const char* programa);

int
main(int argc, char* argv[]) {
  
  if (argc != 2) {
    uso(argv[0]);
  }
  
  Consecutivo_t consecutivo = atoi(argv[1]) % (MAXIMO + 1);

  fprintf(stdout, "Consecutivo: %d\n", consecutivo);

  int df_escritura;
  
  if ((df_escritura = open(TUBERIA_PETICION, O_WRONLY)) == -1) {
    fprintf(stderr, "Error abriendo: %s %d %s",
	    TUBERIA_PETICION, errno, strerror(errno));
    _exit(EXIT_FAILURE);
  }

  fprintf(stdout, "Abriendo tuberia\n");
  char amortiguador_salida[LONGITUD_MENSAJE_PETICION];

  sprintf(amortiguador_salida, "%06d %d\n", getpid(), consecutivo);
  fprintf(stdout, "Mensaje: %s", amortiguador_salida);

  write(df_escritura, amortiguador_salida, LONGITUD_MENSAJE_PETICION);

  int df_lectura;
  
  if ((df_lectura = open(TUBERIA_SOLICITUD, O_RDONLY)) == -1) {
    fprintf(stderr, "Error abriendo: %s %d %s",
	    TUBERIA_SOLICITUD, errno, strerror(errno));
    _exit(EXIT_FAILURE);
  }

  close(df_escritura);
  char amortiguador_entrada[LONGITUD_MENSAJE_SOLICITUD];

  fprintf(stdout, "Abriendo tuberia de solicitud\n");
  
  read(df_lectura, amortiguador_entrada, LONGITUD_MENSAJE_SOLICITUD);
  
  amortiguador_entrada[FIN_PID] = amortiguador_entrada[LONGITUD_MENSAJE_SOLICITUD - 1] = '\0';
  pid_t proceso_cliente = atoi(amortiguador_entrada);
  int valor_recibido = atoi(&amortiguador_entrada[FIN_PID + 1]);

  fprintf(stdout,
	  "El proceso %d recibe consecutivo %d\n",
	  proceso_cliente, valor_recibido);

  close(df_lectura);

  return EXIT_SUCCESS;
}

void uso(const char* programa) {
  fprintf(stderr, "Uso: %s <consectivo>\n", programa);
  _exit(EXIT_FAILURE);
}
