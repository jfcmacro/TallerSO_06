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

  hServicio = CreateServiceA(hAdmonSCM, // Crear el servicio en el administrador local
			     NOMBRE_SERVICIO, // Nombre del servicio
			     NOMBRE_SERVICIO_PANTALLA, // Nombre del servicio en la GUI
			     SC_MANAGER_ALL_ACCESS,
			     SERVICE_WIN32_OWN_PROCESS,
			     SERVICE_DEMAND_START,
			     SERVICE_ERROR_NORMAL,
			     "C:\\msys64\\usr\\local\\bin\\svrwindows.exe",
			     NULL, // Grupo de Inicio no pertenece a servicio alguno
			     NULL, // Nueva etiqueta
			     NULL, // No tiene dependencias a otros servicios
			     NULL, // Usuario por omision
			     NULL); // No requiere password 

  if (hServicio == NULL) {
    fprintf(stderr, "No se pudo registrar el servicio debido a: %d\r\n",
	    GetLastError());
    ExitProcess(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
