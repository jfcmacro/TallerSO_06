#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <semaphore.h>
#include "fumadores.h"

const int SEM_AGENTE=0;
const int SEM_FUMADOR=1;

void manejador(int signo);
void usage(char const *progname, int codigo_salida);
int random_siguiente(int max, int min);
void ejecutar_fumador(sem_t *sems[], char const *id_fumador);

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
      bzero(id_fumador, len);
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

  sem_t *sems[2];

  for (int i = 0; i < 2; i++) {

    char *nom_real_sem = obtener_nombre_semaforo(PRE_GLOBAL, nom_sems[i]);
    fprintf(stdout,
	    "[fumador con %s]: abriendo semaforo %s\r\n",
	    id_fumador,
	    nom_real_sem);

    sems[i] = sem_open(nom_real_sem, 0);

    if (sems[i] == SEM_FAILED) {

      fprintf(stderr, "Error: creando semaforo %s debido a %d\r\n",
	      nom_real_sem, errno);
      _exit(EXIT_FAILURE);
    }

    free(nom_real_sem);
  }

  struct sigaction sa_anterior;
  struct sigaction sa_nueva;

  sa_nueva.sa_handler = manejador;
  sigemptyset(&sa_nueva.sa_mask);
  sa_nueva.sa_flags = 0;
  sigaction(SIGINT, &sa_nueva, &sa_anterior);

  ejecutar_fumador(sems, id_fumador);
  _exit(EXIT_SUCCESS);
}

void usage(char const* progname, int codigo_salida) {
  fprintf(stderr,
	  "Uso: %s [-a <nombre>] -f <nombre>\r\n     %s -h\r\n",
	  progname, progname);

  _exit(codigo_salida);
}

void ejecutar_fumador(sem_t *sems[], char const *id_fumador) {
  for (;;) {

    fprintf(stdout, "[Fumador con %s] esperando\r\n", id_fumador);

    sem_wait(sems[SEM_FUMADOR]);
    fprintf(stdout, "[Fumador con %s] tomo ingredientes, lio cigarrillo y a fumar\r\n",
	    id_fumador);
    sleep(rand() % MAX_TIEMPO_FUMANDO + 1);
    sem_post(sems[SEM_AGENTE]);
  }
}

void manejador(int signo) {
  _exit(EXIT_SUCCESS);
}
