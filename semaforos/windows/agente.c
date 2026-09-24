#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "fumadores.h"

void usage(char const *progname, DWORD codigo_salida);
void ejecutar_fumadores(HANDLE hSems[]);

int
main(int argc, char *argv[]) {
  int opt;

  char const *nom_sems[] = {
    NOM_SEM_CERILLA,
    NOM_SEM_PAPEL,
    NOM_SEM_TABACO,
    NOM_SEM_AGENTE
  };

  while ((opt = getopt(argc, argv, "a:c:hp:t:v")) != -1) {
    switch(opt) {
    case 'a':
      nom_sems[AGENTE] = optarg;
      break;

    case 'c':
      nom_sems[CERILLA] = optarg;
      break;

    case 'h':
      usage(argv[0], EXIT_SUCCESS);
      break;

    case 'p':
      nom_sems[PAPEL] = optarg;
      break;

    case 't':
      nom_sems[TABACO] = optarg;
      break;
    }
  }

  srand(time(NULL));

  HANDLE hSems[AGENTE + 1];

  for (int i = 0; i < AGENTE + 1; i++) {

    char *nom_sem = obtener_nombre_semaforo(PRE_GLOBAL, nom_sems[i]);

    fprintf(stdout, "[Agente] Creando semaforo: %s\r\n", nom_sem);

    hSems[i] = CreateSemaphore(NULL,
			       0,
			       1,
			       nom_sem
			       );
    if (hSems[i] == NULL) {
      fprintf(stderr, "Error: creando semaforo %d\r\n", GetLastError());
      ExitProcess(EXIT_FAILURE);
    }

    free(nom_sem);
  }

  ejecutar_fumadores(hSems);

  ExitProcess(EXIT_SUCCESS);
}

void usage(char const* progname, DWORD codigo_salida) {
  fprintf(stderr,
	  "Uso: %s [-a <nombre>] [-c <nombre>] [-p <nombre>] [-t <nombre>]\r\n",
	  progname);
  fprintf(stderr,
	 "     %s -h\r\n",
	 progname);

  ExitProcess(codigo_salida);
}

void ejecutar_fumadores(HANDLE hSems[]) {
  for (;;) {
    SEM_ID sem = rand() % AGENTE;

    Sleep(rand() % MAX_TIEMPO_PREPARANDO + 1);
    fprintf(stdout, "[Agente] ");

    switch(sem) {
    case CERILLA:
      fprintf(stdout, "poniendo papel y tabaco\r\n");
      break;

    case PAPEL:
      fprintf(stdout, "poniendo cerilla y tabaco\r\n");
      break;

    case TABACO:
      fprintf(stdout, "poniendo cerilla y papel\r\n");
      break;
    }

    ReleaseSemaphore(hSems[sem], 1, NULL);
    fprintf(stdout, "[Agente] Esperando continuar\r\n");
    WaitForSingleObject(hSems[AGENTE], INFINITE);
  }
}
