#include <stdlib.h>
#include <semaphore.h>
#include "jardin_ornamental_limitado.h"

static Jardin_Limitado* pJardin = NULL;

Jardin_Limitado*
obtener_jardin(unsigned int limite) {
  if (pJardin == NULL && limite < MAX_VISITANTES) {
    pJardin = malloc(sizeof(struct Jardin_Limitado));
    pJardin->contador = 0;
    if (sem_init(&pJardin->sem_mutex, 0, 1) == -1) {
      free(pJardin);
      pJardin = NULL;
    }

    if (sem_init(&pJardin->sem_limite, 0, limite) == -1) {
      sem_destroy(&pJardin->sem_mutex);
      free(pJardin);
      pJardin = NULL;
    }
  }

  return pJardin;
}

void
entrar_jardin(PJardin_Limitado jardin) {
  if (jardin != NULL) {
    sem_wait(&jardin->sem_limite);
    sem_wait(&jardin->sem_mutex);
    jardin->contador++;
    sem_post(&jardin->sem_mutex);
  }
}

void
salir_jardin(PJardin_Limitado jardin) {
 if (jardin != NULL) {
    sem_wait(&jardin->sem_mutex);
    jardin->contador--;
    sem_post(&jardin->sem_mutex);
    sem_post(&jardin->sem_limite);
  }
}

int
obtener_miembros_jardin(PJardin_Limitado jardin) {
  int valor_retornar = -1;
  if (jardin != NULL) {
    sem_wait(&jardin->sem_mutex);
    valor_retornar = jardin->contador;
    sem_post(&jardin->sem_mutex);
  }
  return valor_retornar;
}

void
cerrar_jardin(PJardin_Limitado jardin) {
  if (jardin != NULL) {
    sem_destroy(&jardin->sem_mutex);
    sem_destroy(&jardin->sem_limite);
    free((void *) jardin);
    pJardin = NULL;
  }
}
