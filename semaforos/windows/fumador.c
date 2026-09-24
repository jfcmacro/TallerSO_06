#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "fumadores.h"

const int SEM_AGENTE=0;
const int SEM_FUMADOR=1;

void usage(char const *progname, DWORD codigo_salida);
int random_siguiente(int max, int min);
void ejecutar_fumador(HANDLE hSems[], char const *id_fumador);

int
main(int argc, char *argv[]) {
  int opt;

  char const *nom_sems[] =
    { NOM_SEM_AGENTE,
      NULL
    };

  char *id_fumador = NULL;

  while ((opt = getopt(argc, argv, "a:f:h")) != -1) {
    switch(opt) {
    case 'a':
      nom_sems[SEM_AGENTE] = optarg;
      break;

    case 'f':
      nom_sems[SEM_FUMADOR] = optarg;
      size_t len = strlen(optarg) + 1;
      id_fumador = malloc(len);
      ZeroMemory(id_fumador, len);
      strcpy(id_fumador, optarg);
      break;

    case 'h':
      usage(argv[0], EXIT_SUCCESS);
      break;
    }
  }

  if (nom_sems[SEM_FUMADOR] == NULL || id_fumador == NULL) {
    usage(argv[0], EXIT_FAILURE);
  }

  srand(time(NULL));

  HANDLE hSems[2];

  for (int i = 0; i < 2; i++) {

    char *nom_sem = obtener_nombre_semaforo(PRE_GLOBAL, nom_sems[i]);
    fprintf(stdout,
	    "[fumador con %s]: abriendo semaforo %s\r\n",
	    id_fumador,
	    nom_sem);

    hSems[i] = OpenSemaphore(SYNCHRONIZE | SEMAPHORE_MODIFY_STATE, FALSE, nom_sem);

    if (hSems[i] == NULL) {

      fprintf(stderr, "Error: creando semaforo %d\r\n", GetLastError());
      ExitProcess(EXIT_FAILURE);
    }

    free(nom_sem);
  }

  ejecutar_fumador(hSems, id_fumador);
  ExitProcess(EXIT_SUCCESS);
}

void usage(char const* progname, DWORD codigo_salida) {
  fprintf(stderr,
	  "Uso: %s [-a <nombre>] -f <nombre>\r\n     %s -h\r\n",
	  progname, progname);

  ExitProcess(codigo_salida);
}

void ejecutar_fumador(HANDLE hSems[], char const *id_fumador) {
  for (;;) {

    fprintf(stdout, "[Fumador con %s] esperando\r\n", id_fumador);

    WaitForSingleObject(hSems[SEM_FUMADOR], INFINITE);
    fprintf(stdout, "[Fumador con %s] tomo ingredientes, lio cigarrillo y a fumar\r\n",
	    id_fumador);
    Sleep(rand() % MAX_TIEMPO_FUMANDO + 1);
    ReleaseSemaphore(hSems[SEM_AGENTE], 1, NULL);
  }
}
