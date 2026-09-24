#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "jardin_ornamental.h"

void usage(char const *progname, DWORD codigo_salida);
int random_siguiente(int max, int min);

DWORD WINAPI visitante(PVOID pv_arg);
DWORD WINAPI admin(PVOID pv_arg);

int
main(int argc, char *argv[]) {
  int opt;

  while ((opt = getopt(argc, argv, "h")) != -1) {
    switch(opt) {
    case 'h':
      usage(argv[0], EXIT_SUCCESS);
      break;
    }
  }

  PJardin jardin = obtener_jardin();

  if (jardin == NULL) {
    fprintf(stderr,
	    "Error: Estructura del jardin ornamental no pudo ser creada\r\n");

    ExitProcess(EXIT_FAILURE);
  }

  fprintf(stdout,
	  "Numero visitanes antes de iniciar: %d\r\n",
	  obtener_miembros_jardin(jardin));

  int visitantes = MAX_VISITANTES;
  HANDLE h_hilos[MAX_VISITANTES + 1];
  DWORD h_hilos_id[MAX_VISITANTES + 1];

  srand(time(NULL));

  for (int i = 0; i < visitantes; i++) {
    h_hilos[i] = CreateThread(NULL,
			      0,
			      visitante,
			      (PVOID) jardin,
			      0,
			      &h_hilos_id[i]);

    if (h_hilos[i] == NULL) {
      fprintf(stderr, "Error: No se pudo crear %d-esimo hilo debido a %d\r\n",
	      i, GetLastError());

      ExitProcess(EXIT_FAILURE);
    }
  }

  h_hilos[MAX_VISITANTES] = CreateThread(NULL,
					 0,
					 admin,
					 (PVOID) jardin,
					 0,
					 &h_hilos_id[MAX_VISITANTES]);

  WaitForMultipleObjects(MAX_VISITANTES + 1,
			 h_hilos,
			 TRUE,
			 INFINITE);

  for (int i = 0; i < MAX_VISITANTES + 1; i++) {

    CloseHandle(h_hilos[i]);
  }

  fprintf(stdout, "Numero visitanes antes de cerrar jardin: %d\r\n",
	  obtener_miembros_jardin(jardin));

  cerrar_jardin(jardin);

  ExitProcess(EXIT_SUCCESS);
}

void usage(char const* progname, DWORD codigo_salida) {
  fprintf(stderr,
	  "Uso: %s [-v <numero_visitantes>]\r\n     %s -h\r\n",
	  progname, progname);

  ExitProcess(codigo_salida);
}

DWORD WINAPI visitante(PVOID pv_arg) {
  PJardin jardin = (PJardin) pv_arg;
  HANDLE h_hilo = GetCurrentThread();
  DWORD hilo_id = GetThreadId(h_hilo);
  CloseHandle(h_hilo);

  for (int veces = 0; veces < MAX_VISITAS; veces++) {
    fprintf(stdout, "Visitante %d afuera\r\n", hilo_id);
    Sleep(random_siguiente(TIEMPO_AFUERA, 0));

    entrar_jardin(jardin);
    fprintf(stdout, "Visitante %d dentro del jardin\r\n", hilo_id);

    Sleep(random_siguiente(TIEMPO_VISITA, 0));
    salir_jardin(jardin);
  }

  fprintf(stdout, "Visitante %d termino su jornada\r\n", hilo_id);

  return 0;
}

DWORD WINAPI admin(PVOID pv_arg) {
  PJardin jardin = (PJardin) pv_arg;

  for (int veces = 0; veces < MAX_VISITAS; veces++) {
    Sleep(random_siguiente(TIEMPO_AFUERA * 2, 0));
    fprintf(stdout,
	    "Visitantes %d dentro del jardin\r\n",
	    obtener_miembros_jardin(jardin));
  }

  fprintf(stdout, "Administrador termino su jornada\r\n");
  return 0;
}

int random_siguiente(int max, int min) {
  double r = (double) rand();
  double u = (double) RAND_MAX;
  double d = r / u;

  return (int)(d * (max - min))  + min;
}
