#include <windows.h>
#include "svrwindows.h"

// Variables globales para controlar el servicio
SERVICE_STATUS estado_servicio;
SERVICE_STATUS_HANDLE hServicio;
// volatile BOOL corriendo = TRUE;
HANDLE ghSvcStopEvent = NULL;

void WINAPI servicio_ppal(DWORD, LPTSTR*);
void WINAPI gestion_del_scm(DWORD);
void registrar_evento_log(HANDLE, WORD, LPCSTR);
DWORD WINAPI hilo_servicio(LPVOID);
HANDLE hRegistrar_LogRegistro = NULL;
volatile ESTADOS_SERVICIO estado = NO_INICIADO;

int
main(int argc, char* const argv[]) {
  LPCSTR szFuente = "svrwindows";
  hRegistrar_LogRegistro = RegisterEventSourceA(NULL, szFuente);

  SERVICE_TABLE_ENTRY DispatchTable[] = {
        { NOMBRE_SERVICIO, (LPSERVICE_MAIN_FUNCTION)servicio_ppal},
        { NULL, NULL }
  };

  if (!StartServiceCtrlDispatcher(DispatchTable)) {
    registrar_evento_log(hRegistrar_LogRegistro,
			 EVENTLOG_ERROR_TYPE,
			 "No pudo registrar la tabla de servicios");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

void WINAPI servicio_ppal(DWORD dwArgc, LPTSTR *lpszArgv) {

   hServicio = RegisterServiceCtrlHandler(NOMBRE_SERVICIO,
					  (LPHANDLER_FUNCTION) gestion_del_scm);

    if (hServicio == 0) {
      registrar_evento_log(hRegistrar_LogRegistro,
			   EVENTLOG_ERROR_TYPE,
			   "No pudo obtener el handle del registro de servicio");
      return;
    }

    estado_servicio.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    estado_servicio.dwCurrentState = SERVICE_START_PENDING;
    // estado_servicio.dwControlsAccepted = SERVICE_ACCEPT_STOP | ;
    estado_servicio.dwControlsAccepted = SERVICE_ACCEPT_STOP |
      SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_SHUTDOWN;
    estado = INICIANDO;

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
    if (estado != TERMINADO) {
      estado_servicio.dwCurrentState = SERVICE_STOP_PENDING;
      SetServiceStatus(hServicio, &estado_servicio);
      registrar_evento_log(hRegistrar_LogRegistro,
			   EVENTLOG_INFORMATION_TYPE,
			   "Terminado el servicio");
      estado = TERMINANDO;
    }
    break;
  case SERVICE_CONTROL_PAUSE:
    if (estado == CORRIENDO) {
      estado_servicio.dwCurrentState = SERVICE_PAUSE_PENDING;
      SetServiceStatus(hServicio, &estado_servicio);
      registrar_evento_log(hRegistrar_LogRegistro,
			   EVENTLOG_INFORMATION_TYPE,
			   "Pausando el proceso pendiente");
      estado = PAUSANDO;
    }
    break;
  case SERVICE_CONTROL_CONTINUE:
    if (estado == PAUSADO) {
      estado_servicio.dwCurrentState = SERVICE_CONTINUE_PENDING;
      SetServiceStatus(hServicio, &estado_servicio);
      registrar_evento_log(hRegistrar_LogRegistro,
			   EVENTLOG_INFORMATION_TYPE,
			   "Continuando el proceso pendiente");
      estado = INICIANDO;
    }
    break;
  case SERVICE_CONTROL_SHUTDOWN:
    if (estado != TERMINADO) {
      estado_servicio.dwCurrentState = SERVICE_STOP_PENDING;
      SetServiceStatus(hServicio, &estado_servicio);
      registrar_evento_log(hRegistrar_LogRegistro,
			   EVENTLOG_INFORMATION_TYPE,
			   "Terminado el servicio");
      estado = TERMINANDO;
    }
    break;
  default:
    break;
  }
}

DWORD WINAPI hilo_servicio(LPVOID arg) {
  estado_servicio.dwCurrentState = SERVICE_RUNNING;
  SetServiceStatus(hServicio, &estado_servicio);
  estado = CORRIENDO;

  while(estado != TERMINADO) {
    switch(estado) {
    case NO_INICIADO:
      registrar_evento_log(hRegistrar_LogRegistro,
			 EVENTLOG_WARNING_TYPE,
			 "Sistema en un estado imposible");
      estado = CORRIENDO;
      break;

    case INICIANDO:
      registrar_evento_log(hRegistrar_LogRegistro,
			   EVENTLOG_INFORMATION_TYPE,
			   "Proceso proximo a correr");
      estado_servicio.dwCurrentState = SERVICE_RUNNING;
      SetServiceStatus(hServicio, &estado_servicio);
      estado = CORRIENDO;
      break;

    case CORRIENDO:
      registrar_evento_log(hRegistrar_LogRegistro,
			   EVENTLOG_INFORMATION_TYPE,
			   "Sistema corriendo");
      break;

    case PAUSANDO:
      registrar_evento_log(hRegistrar_LogRegistro,
			   EVENTLOG_INFORMATION_TYPE,
			   "Sistema ha sido pausado");
      estado_servicio.dwCurrentState = SERVICE_PAUSED;
      SetServiceStatus(hServicio, &estado_servicio);
      estado = PAUSADO;
      break;

    case PAUSADO:
      registrar_evento_log(hRegistrar_LogRegistro,
			   EVENTLOG_INFORMATION_TYPE,
			   "Sistema en pausa");
      break;

    case TERMINANDO:
      registrar_evento_log(hRegistrar_LogRegistro,
			   EVENTLOG_INFORMATION_TYPE,
			   "Sistema en pausa");
      estado_servicio.dwCurrentState = SERVICE_STOPPED;
      SetServiceStatus(hServicio, &estado_servicio);
      estado_servicio.dwWin32ExitCode = EXIT_SUCCESS;
      estado_servicio.dwServiceSpecificExitCode = 1;
      estado = TERMINADO;
      break;
    }

  }
  
  return EXIT_SUCCESS;
}

void registrar_evento_log(HANDLE hRegistrar_LogServicio,
			  WORD wTipo,
			  LPCSTR mensaje) {
  LPCSTR lpszArregloMensajes[1];

  if (hRegistrar_LogServicio != NULL) {
        lpszArregloMensajes[0] = mensaje;

    ReportEventA(hRegistrar_LogRegistro,     // Registar_Log de eventos
		 wTipo,                      // Tipo de evento
		 0,                          // Categoria
		 0,                          // Event ID
		 NULL,                       // ID del usuario
		 1,                          // Numero de strings en el arreglo
		 0,                          // Tamaño del dato
		 lpszArregloMensajes,        // Arreglo de cadenas
		 NULL                        // Amortiguador de datos crudos
		 );
  }
}
