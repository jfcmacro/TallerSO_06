#include <stdlib.h>
#include <semaphore.h>
#include "jardin_ornamental.h"

static Jardin* pJardin = NULL;

Jardin*
obtener_jardin() {
  if (pJardin == NULL) {
    pJardin = malloc(sizeof(struct Jardin));
    pJardin->contador = 0;
    
    if (sem_init(&pJardin->sem_mutex, 0, 1) == -1) {
      free(pJardin);
      pJardin = NULL;
    }
  }

  return pJardin;
}

void
entrar_jardin(PJardin jardin) {
  if (jardin != NULL) {
    sem_wait(&jardin->sem_mutex);
    jardin->contador++;
    sem_post(&jardin->sem_mutex);
  }
}

void
salir_jardin(PJardin jardin) {
 if (jardin != NULL) {
    sem_wait(&jardin->sem_mutex);
    jardin->contador--;
    sem_post(&jardin->sem_mutex);
  }
}

int
obtener_miembros_jardin(PJardin jardin) {
  int valor_retornar = -1;
  if (jardin != NULL) {
    sem_wait(&jardin->sem_mutex);
    valor_retornar = jardin->contador;
    sem_post(&jardin->sem_mutex);
  }
  return valor_retornar;
}

void
cerrar_jardin(PJardin jardin) {
  if (jardin != NULL) {
    sem_destroy(&jardin->sem_mutex);
    free((void *) jardin);
    pJardin = NULL;
  }
}
