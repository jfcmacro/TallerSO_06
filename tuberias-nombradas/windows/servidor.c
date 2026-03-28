#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "tuberias_nombradas.h"
#include "servidor.h"
#include "utilidad.h"

static int consecutivos[MAXIMO + 1] = { 10, 20, 30 };

int
main(int argc, char* argv[]) {

  HANDLE hPeticion = CreateNamedPipe(TUBERIA_PETICION,
				     PIPE_ACCESS_INBOUND,
				     PIPE_TYPE_MESSAGE |
				     PIPE_READMODE_MESSAGE |
				     PIPE_WAIT,
				     1,
				     0,
				     LONGITUD_MENSAJE_PETICION,
				     0,
				     NULL);

  if (hPeticion == INVALID_HANDLE_VALUE) {
    fprintf(stderr, "Error creando tuberia nombrada %s debido a %d\r\n",
	    TUBERIA_PETICION,
	    GetLastError());
    ExitProcess(EXIT_FAILURE);
  }

  HANDLE hRespuesta = CreateNamedPipe(TUBERIA_RESPUESTA,
				      PIPE_ACCESS_OUTBOUND,
				      PIPE_TYPE_MESSAGE |
				      PIPE_READMODE_MESSAGE |
				      PIPE_WAIT,
				      1,
				      0,
				      LONGITUD_MENSAJE_PETICION,
				      0,
				      NULL);

  if (hRespuesta == INVALID_HANDLE_VALUE) {
    fprintf(stderr, "Error creando tuberia nombrada %s debido a %d\r\n",
	    TUBERIA_RESPUESTA,
	    GetLastError());

    ExitProcess(EXIT_FAILURE);
  }

  for (;;) {

    char amortiguador_entrada[LONGITUD_MENSAJE_PETICION];

    if (!ConnectNamedPipe(hPeticion, NULL)) {
      fprintf(stderr, "Error en la conexion %s del cliente debido a %d\r\n",
	      TUBERIA_PETICION,
	      GetLastError());
      CloseHandle(hPeticion);

      ExitProcess(EXIT_FAILURE);
    }

    fprintf(stdout, "Preparando lectura\n");
    DWORD nBytesLeidos;

    if (!ReadFile(hPeticion, amortiguador_entrada, LONGITUD_MENSAJE_PETICION, &nBytesLeidos, NULL)) {
      fprintf(stderr, "Error leyendo en la tuberia nombrada %s debido a %d\r\n",
	      TUBERIA_PETICION,
	      GetLastError());

      CloseHandle(hPeticion);
      ExitProcess(EXIT_FAILURE);
    }

    if (nBytesLeidos > 0) {

      amortiguador_entrada[FIN_PID] = amortiguador_entrada[LONGITUD_MENSAJE_PETICION - 1] = '\0';
      pid_t proceso_cliente = atoi(amortiguador_entrada);
      Consecutivo_t consecutivo = atoi(&amortiguador_entrada[FIN_PID + 1]);

      fprintf(stdout, "Peticion: %d en la cola: %d\n", proceso_cliente, consecutivo);

      if (!ConnectNamedPipe(hRespuesta, NULL)) {
	fprintf(stderr, "Error en la conexion %s debido a %d\r\n",
		TUBERIA_RESPUESTA,
		GetLastError());
	CloseHandle(hRespuesta);

	ExitProcess(EXIT_FAILURE);
      }

      char amortiguador_salida[LONGITUD_MENSAJE_SOLICITUD];

      ZeroMemory(amortiguador_salida, LONGITUD_MENSAJE_SOLICITUD);
      sprintf(amortiguador_salida, "%06d %06d\n", proceso_cliente, consecutivos[consecutivo]++);

      DWORD nBytesEscritos;

      if (!WriteFile(hRespuesta, amortiguador_salida, LONGITUD_MENSAJE_SOLICITUD,&nBytesEscritos, NULL)) {
	fprintf(stderr, "Error escribiendo en la tuberia nombrada %s debido a %d\r\n",
		TUBERIA_RESPUESTA,
		GetLastError());

	ExitProcess(EXIT_FAILURE);
      }

      CloseHandle(hRespuesta);
    }
  }

  return EXIT_SUCCESS;
}
