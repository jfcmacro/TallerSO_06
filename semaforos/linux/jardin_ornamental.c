#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include "jardin_ornamental.h"

void usage(char const *progname, int codigo_salida);
int random_siguiente(int max, int min);

void* visitante(void* pv_arg);
void* admin(void* pv_arg);

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
	    "Error: Estructura del jardin ornamental no pudo ser creada\n");

    _exit(EXIT_FAILURE);
  }

  fprintf(stdout,
	  "Numero visitanes antes de iniciar: %d\n",
	  obtener_miembros_jardin(jardin));

  int visitantes = MAX_VISITANTES;
  pthread_t hilos[MAX_VISITANTES + 1];

  srand(time(NULL));

  for (int i = 0; i < visitantes; i++) {
    if (pthread_create(&hilos[i],
		       NULL,
		       visitante,
		       (void*) jardin) != 0) {
      fprintf(stderr, "Error: No se pudo crear %d-esimo hilo debido a %d\n",
	      i, errno);

      _exit(EXIT_FAILURE);
    }
  }

  pthread_create(&hilos[MAX_VISITANTES],
		 NULL,
		 admin,
		 (void*) jardin);

  for (int i = 0; i < MAX_VISITANTES + 1; i++) {
   
    pthread_join(hilos[i], NULL);
  }

  fprintf(stdout, "Numero visitantes antes de cerrar jardin: %d\n",
	  obtener_miembros_jardin(jardin));

  cerrar_jardin(jardin);

  _exit(EXIT_SUCCESS);
}

void usage(char const* progname, int codigo_salida) {
  fprintf(stderr,
	  "Uso: %s [-v <numero_visitantes>]\n     %s -h\n",
	  progname, progname);

  _exit(codigo_salida);
}

void* visitante(void* pv_arg) {
  PJardin jardin = (PJardin) pv_arg;
  pthread_t hilo = pthread_self();
  
  for (int veces = 0; veces < MAX_VISITAS; veces++) {
    fprintf(stdout, "Visitante %ld afuera\n", hilo);
    sleep(random_siguiente(TIEMPO_AFUERA, 0));

    entrar_jardin(jardin);
    fprintf(stdout, "Visitante %ld dentro del jardin\n", hilo);

    sleep(random_siguiente(TIEMPO_VISITA, 0));
    salir_jardin(jardin);
  }

  fprintf(stdout, "Visitante %ld termino su jornada\n", hilo);

  return NULL;
}

void* admin(void* pv_arg) {
  PJardin jardin = (PJardin) pv_arg;

  for (int veces = 0; veces < MAX_VISITAS; veces++) {
    sleep(random_siguiente(TIEMPO_AFUERA * 2, 0));
    fprintf(stdout,
	    "Visitantes %d dentro del jardin\n",
	    obtener_miembros_jardin(jardin));
  }

  fprintf(stdout, "Administrador termino su jornada\n");
  
  return NULL;
}

int random_siguiente(int max, int min) {
  double r = (double) rand();
  double u = (double) RAND_MAX;
  double d = r / u;

  return (int)(d * (max - min))  + min;
}
