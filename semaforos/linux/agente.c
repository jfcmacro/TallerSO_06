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

void manejador(int signo);
void usage(char const *progname, int codigo_salida);
void ejecutar_fumadores(sem_t *sems[]);

sem_t *sems[AGENTE + 1];
char* nom_real_sem[AGENTE + 1];

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
  
  for (int i = 0; i < AGENTE + 1; i++) {

    nom_real_sem[i] = obtener_nombre_semaforo(PRE_GLOBAL, nom_sems[i]);

    fprintf(stdout, "[Agente] Creando semaforo: %s\n", nom_real_sem[i]);

    sems[i] = sem_open(nom_real_sem[i],
		       O_CREAT | O_EXCL,
		       S_IRUSR | S_IWUSR,
		       0);
    
    if (sems[i] == SEM_FAILED) {
      fprintf(stderr, "Error: creando semaforo %d\n", errno);
      
      for (int j = 0; j < i; j++) {
	sem_close(sems[j]);
	sem_unlink(nom_real_sem[j]);
      }
      
      _exit(EXIT_FAILURE);
    }
  }

  struct sigaction sa_anterior;
  struct sigaction sa_nueva;

  sa_nueva.sa_handler = manejador;
  sigemptyset(&sa_nueva.sa_mask);
  sa_nueva.sa_flags = 0;
  sigaction(SIGINT, &sa_nueva, &sa_anterior);

  ejecutar_fumadores(sems);

  _exit(EXIT_SUCCESS);
}

void usage(char const* progname, int codigo_salida) {
  fprintf(stderr,
	  "Uso: %s [-a <nombre>] [-c <nombre>] [-p <nombre>] [-t <nombre>]\n",
	  progname);
  fprintf(stderr,
	 "     %s -h\n",
	 progname);

  _exit(codigo_salida);
}

void ejecutar_fumadores(sem_t *sems[]) {
  for (;;) {
    SEM_ID sem = rand() % AGENTE;

    sleep(rand() % MAX_TIEMPO_PREPARANDO + 1);
    fprintf(stdout, "[Agente] ");

    switch(sem) {
    case CERILLA:
      fprintf(stdout, "poniendo papel y tabaco\n");
      break;

    case PAPEL:
      fprintf(stdout, "poniendo cerilla y tabaco\n");
      break;

    case TABACO:
      fprintf(stdout, "poniendo cerilla y papel\n");
      break;
    }

    sem_post(sems[sem]);
    fprintf(stdout, "[Agente] Esperando continuar\n");
    sem_wait(sems[AGENTE]);
  }
}

void manejador(int signo) {
  for (int i = 0; i < AGENTE +1; i++) {
    sem_close(sems[i]);
    sem_unlink(nom_real_sem[i]);
    free(nom_real_sem[i]);
  }
  
  _exit(EXIT_SUCCESS);
}
