#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "fumadores.h"

void manejador(int signo);
void usage(char const *progname, int codigo_salida);
int random_siguiente(int max, int min);

enum SEM_ID { CERILLA, PAPEL, TABACO, AGENTE };

pid_t procesos[AGENTE+1];

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

  procesos[0] = fork();

  if (procesos[0] == 0) {
    execl("./agente", "agente",
	  "-a", nom_sems[AGENTE],
	  "-c", nom_sems[CERILLA],
	  "-p", nom_sems[PAPEL],
	  "-t", nom_sems[TABACO],
	  NULL);
    _exit(EXIT_FAILURE);
  }

  sleep(MAX_TIEMPO_PREPARANDO);
  
  for (int i = 0; i < AGENTE; i++) {
    sprintf(linea_comando, "./fumador -a %s -f %s",
	    nom_sems[AGENTE],
	    nom_sems[i]);

    procesos[i+1] = fork();

    if (procesos[i+1] == 0) {
      execl("./fumador", "fumador",
	    "-a", nom_sems[AGENTE],
	    "-f", nom_sems[i],
	    NULL);
      _exit(EXIT_FAILURE);
    }
  }

  struct sigaction sa_anterior;
  struct sigaction sa_nueva;

  sa_nueva.sa_handler = manejador;
  sigemptyset(&sa_nueva.sa_mask);
  sa_nueva.sa_flags = 0;
  sigaction(SIGINT, &sa_nueva, &sa_anterior);

  int estado_term_proceso;
  
  for (int i = 0; i < AGENTE + 1; i++) {
    waitpid(procesos[i], &estado_term_proceso, 0);
  }

  sigaction(SIGINT, &sa_anterior, 0);
    
  _exit(EXIT_SUCCESS);
}

void usage(char const* progname, int codigo_salida) {
  fprintf(stderr,
	  "Uso: %s [-a <nombre>] [-c <nombre>] [-p <nombre>] [-t <nombre>]\r\n     %s -h\r\n",
	  progname, progname);

  _exit(codigo_salida);
}

void manejador(int signo) {
  for (int i = 0; i < AGENTE + 1; i++) {
    kill(procesos[i], SIGINT);
  }
}
