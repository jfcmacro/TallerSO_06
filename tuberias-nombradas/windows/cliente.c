#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
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

  HANDLE hPeticion = CreateFile(TUBERIA_PETICION,
				GENERIC_WRITE,
				0,
				NULL,
				OPEN_EXISTING,
				0,
				NULL);

  if (hPeticion == INVALID_HANDLE_VALUE) {
    fprintf(stderr, "Error creando tuberia %s %d\r\n",
	    TUBERIA_PETICION,
	    GetLastError());
    ExitProcess(EXIT_FAILURE);
  }

  char amortiguador_salida[LONGITUD_MENSAJE_PETICION];

  sprintf(amortiguador_salida, "%06d %d\n", GetProcessId(GetCurrentProcess()), consecutivo);
  fprintf(stdout, "Mensaje: %s", amortiguador_salida);

  DWORD nBytesEscritos;

  WriteFile(hPeticion, amortiguador_salida, LONGITUD_MENSAJE_PETICION, &nBytesEscritos, NULL);

  CloseHandle(hPeticion);

  HANDLE hRespuesta = CreateFile(TUBERIA_RESPUESTA,
				GENERIC_READ,
				0,
				NULL,
				OPEN_EXISTING,
				0,
				NULL);

  if (hRespuesta == INVALID_HANDLE_VALUE) {
    fprintf(stderr, "Error abriendo: %s %d\r\n",
	    TUBERIA_RESPUESTA, GetLastError());
    ExitProcess(EXIT_FAILURE);
  }


  char amortiguador_entrada[LONGITUD_MENSAJE_SOLICITUD];

  fprintf(stdout, "Abriendo tuberia de solicitud\r\n");
  DWORD nBytesLeidos;

  if (!ReadFile(hRespuesta, amortiguador_entrada, LONGITUD_MENSAJE_SOLICITUD, &nBytesLeidos, NULL)) {
    fprintf(stderr, "Error leyendo la respuesta en %s debido a %d\r\n",
	    TUBERIA_RESPUESTA, GetLastError());
    ExitProcess(EXIT_FAILURE);
  }
  
  amortiguador_entrada[FIN_PID] = amortiguador_entrada[LONGITUD_MENSAJE_SOLICITUD - 1] = '\0';
  DWORD proceso_cliente = atoi(amortiguador_entrada);
  int valor_recibido = atoi(&amortiguador_entrada[FIN_PID + 1]);

  fprintf(stdout,
	  "El proceso %d recibe consecutivo %d\n",
	  proceso_cliente, valor_recibido);

  CloseHandle(hRespuesta);

  return EXIT_SUCCESS;
}

void uso(const char* programa) {
  fprintf(stderr, "Uso: %s <consectivo>\n", programa);
  _exit(EXIT_FAILURE);
}
