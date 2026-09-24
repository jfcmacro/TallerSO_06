#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "svrwindows.h"

int
main(int argc, char* const argv[]) {

  SC_HANDLE hAdmonSCM;

  hAdmonSCM = OpenSCManagerA(NULL,  // Administrador local
			     NULL,  // abrir la base de datos de servicios por default)
			     SC_MANAGER_ALL_ACCESS);

  if (hAdmonSCM == NULL) {
    fprintf(stderr, "No pudo obtener handle del administrador SCM local debido a %d\r\n",
	    GetLastError());
    ExitProcess(EXIT_FAILURE);
  }

  SC_HANDLE hServicio;

  hServicio = OpenServiceA(hAdmonSCM, // Crear el servicio en el administrador local
			   NOMBRE_SERVICIO, // Nombre del servicio
			   SC_MANAGER_ALL_ACCESS); // Servicio
  
  if (hServicio == NULL) {
    fprintf(stderr, "No se pudo abrir el servicio debido a: %d\r\n",
	    GetLastError());
    ExitProcess(EXIT_FAILURE);
  }

  if (!DeleteService(hServicio)) {
    fprintf(stderr, "No se pudo desregistrar servicio debido a: %d\r\n",
	    GetLastError());
    ExitProcess(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
