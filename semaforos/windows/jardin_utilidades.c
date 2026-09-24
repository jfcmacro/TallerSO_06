#include <windows.h>
#include <stdlib.h>
#include "jardin_ornamental.h"

static Jardin* pJardin = NULL;

Jardin*
obtener_jardin() {
  if (pJardin == NULL) {
    pJardin = malloc(sizeof(struct Jardin));
    pJardin->contador = 0;
    pJardin->h_sem_mutex = CreateSemaphore(NULL,
					   1,
					   1,
					   NULL);
    if (pJardin->h_sem_mutex == NULL) {
      free(pJardin);
      pJardin = NULL;
    }
  }

  return pJardin;
}

void
entrar_jardin(PJardin jardin) {
  if (jardin != NULL) {
    WaitForSingleObject(jardin->h_sem_mutex, INFINITE);
    jardin->contador++;
    ReleaseSemaphore(jardin->h_sem_mutex, 1, NULL);
  }
}

void
salir_jardin(PJardin jardin) {
 if (jardin != NULL) {
    WaitForSingleObject(jardin->h_sem_mutex, INFINITE);
    jardin->contador--;
    ReleaseSemaphore(jardin->h_sem_mutex, 1, NULL);
  }
}

int
obtener_miembros_jardin(PJardin jardin) {
  int valor_retornar = -1;
  if (jardin != NULL) {
    WaitForSingleObject(jardin->h_sem_mutex, INFINITE);
    valor_retornar = jardin->contador;
    ReleaseSemaphore(jardin->h_sem_mutex, 1, NULL);
  }
  return valor_retornar;
}

void
cerrar_jardin(PJardin jardin) {
  if (jardin != NULL) {
    CloseHandle(jardin->h_sem_mutex);
    free((void *) jardin);
    pJardin = NULL;
  }
}
