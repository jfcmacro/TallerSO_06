#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "fumadores.h"

BOOL WINAPI handle_signals(DWORD dwCtrlType);

void usage(char const *progname, DWORD codigo_salida);
int random_siguiente(int max, int min);

enum SEM_ID { CERILLA, PAPEL, TABACO, AGENTE };

STARTUPINFO si[AGENTE+1];
PROCESS_INFORMATION pi[AGENTE+1];

int
main(int argc, char *argv[]) {
  int opt;

  char const *nom_sems[] = {
    NOM_SEM_CERILLA,
    NOM_SEM_PAPEL,
    NOM_SEM_TABACO,
    NOM_SEM_AGENTE
  };

  while ((opt = getopt(argc, argv, "a:c:hp:t:")) != -1) {
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

  char linea_comando[1024];

  sprintf(linea_comando, "./agente -a %s -c %s -p %s -t %s",
	  nom_sems[AGENTE],
	  nom_sems[CERILLA],
	  nom_sems[PAPEL],
	  nom_sems[TABACO]);

  ZeroMemory(&si[0], sizeof(si[0]));
  si[0].cb = sizeof(si[0]);
  ZeroMemory(&pi[0], sizeof(pi[0]));

  CreateProcess(NULL,
		linea_comando,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si[0],
		&pi[0]);

  // SetConsoleCtrlHandler(handle_signals, TRUE);

  for (int i = 0; i < AGENTE; i++) {
    sprintf(linea_comando, "./fumador -a %s -f %s",
	    nom_sems[AGENTE],
	    nom_sems[i]);
    ZeroMemory(&si[i+1], sizeof(si[i+1]));
    si[i+1].cb = sizeof(si[i+1]);
    ZeroMemory(&pi[i+1], sizeof(pi[i+1]));
    CreateProcess(NULL,
		  linea_comando,
		  NULL,
		  NULL,
		  FALSE,
		  0,
		  NULL,
		  NULL,
		  &si[i+1],
		  &pi[i+1]);
  }

  // WaitForSingleObject(pi[0].hProcess, INFINITE);
  ExitProcess(EXIT_SUCCESS);
}

void usage(char const* progname, DWORD codigo_salida) {
  fprintf(stderr,
	  "Uso: %s [-a <nombre>] [-c <nombre>] [-p <nombre>] [-t <nombre>]\r\n     %s -h\r\n",
	  progname, progname);

  ExitProcess(codigo_salida);
}

BOOL WINAPI handle_signals(DWORD dwCtrlType) {
  fprintf(stdout, "[ctrl_fumadores] Terminado los procesos\r\n");
  for (int i = 0; i < AGENTE + 1; i++) {
    TerminateProcess(pi[i].hProcess, EXIT_SUCCESS);
  }
}
