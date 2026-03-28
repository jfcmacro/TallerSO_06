#include <windows.h>
#include "svrwindows.h"

// Variables globales para controlar el servicio
SERVICE_STATUS estado_servicio;
SERVICE_STATUS_HANDLE hServicio;
volatile BOOL corriendo = TRUE;
HANDLE ghSvcStopEvent = NULL;

void WINAPI servicio_ppal(DWORD, LPTSTR*);
void WINAPI gestion_del_scm(DWORD);
DWORD WINAPI hilo_servicio(LPVOID);

int
main(int argc, char* const argv[]) {

  SERVICE_TABLE_ENTRY DispatchTable[] = {
        { NOMBRE_SERVICIO, (LPSERVICE_MAIN_FUNCTION)servicio_ppal},
        { NULL, NULL }
  };

  if (!StartServiceCtrlDispatcher(DispatchTable)) {
    // Guardar en el log de eventos el error
    return EXIT_FAILURE;
  }

  return EXIT_FAILURE;
}

void WINAPI servicio_ppal(DWORD dwArgc, LPTSTR *lpszArgv) {

   hServicio = RegisterServiceCtrlHandler(NOMBRE_SERVICIO,
        (LPHANDLER_FUNCTION)gestion_del_scm);

    if (hServicio == 0) {
        // Log error and return
        return;
    }

    estado_servicio.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    estado_servicio.dwCurrentState = SERVICE_START_PENDING;
    estado_servicio.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    // estado_servicio.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;

    SetServiceStatus(hServicio, &estado_servicio);

    // Inicializacion. Duerme el proceso actual unos segundos
    Sleep(10000); // 10 segundos

    DWORD id_hilo_servicio;
    HANDLE h_hilo_servicio = CreateThread(NULL,
					  0,
					  hilo_servicio,
					  NULL,
					  0,
					  &id_hilo_servicio);

    if (h_hilo_servicio == NULL) {
      return;
    }
}

void WINAPI gestion_del_scm(DWORD dwControl) {
  switch(dwControl) {
  case SERVICE_CONTROL_STOP:
    estado_servicio.dwCurrentState = SERVICE_STOP_PENDING;
    SetServiceStatus(hServicio, &estado_servicio);
    corriendo = FALSE;
    break;
  case SERVICE_CONTROL_PAUSE:
    break;
  case SERVICE_CONTROL_CONTINUE:
    break;
  case SERVICE_CONTROL_SHUTDOWN:
    break;
  default:
    break;
  }
}

DWORD WINAPI hilo_servicio(LPVOID arg) {
  estado_servicio.dwCurrentState = SERVICE_RUNNING;

  SetServiceStatus(hServicio, &estado_servicio);

  while(corriendo) {
    Sleep(5000);
  }

  estado_servicio.dwCurrentState = SERVICE_STOPPED;
  estado_servicio.dwWin32ExitCode = EXIT_SUCCESS;
  estado_servicio.dwServiceSpecificExitCode = 1;
  SetServiceStatus(hServicio, &estado_servicio);

  return EXIT_SUCCESS;
}
