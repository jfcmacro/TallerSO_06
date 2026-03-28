#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

void registar_log(HANDLE, LPCSTR);

int
main(int argc, char const *argv[]) {

  LPCSTR szFuente = "reg_eventos";
  HANDLE hRegistar_LogRegistro = RegisterEventSourceA(NULL, szFuente);

  if (hRegistar_LogRegistro == NULL) {
    fprintf(stderr, "Error obteniendo handle al Registro de registar_logs: %d\r\n",
	    GetLastError());
    ExitProcess(EXIT_FAILURE);
  }

  registar_log(hRegistar_LogRegistro, "Iniciar");
  registar_log(hRegistar_LogRegistro, "Dormir");
  Sleep(5000);
  registar_log(hRegistar_LogRegistro, "Terminado");

  return EXIT_SUCCESS;
}

void registar_log(HANDLE hRegistar_LogRegistro, LPCSTR mensaje) {
  LPCSTR lpszMensajeArray[1];

  if (hRegistar_LogRegistro != NULL) {
    lpszMensajeArray[0] = mensaje;

    ReportEventA(hRegistar_LogRegistro,      // Registar_Log de eventos
		 EVENTLOG_INFORMATION_TYPE,  // Tipo de evento
		 0,                          // Categoria
		 0,                          // Event ID
		 NULL,                       // ID del usuario
		 1,                          // Numero de strings en el arreglo
		 0,                          // Tamaño del dato
		 lpszMensajeArray,           // Arreglo de cadenas
		 NULL                        // Amortiguador de datos crudos
		 );
  }
}
