#include <windows.h>
#include <stdlib.h>
#include "jardin_ornamental_limitado.h"

static Jardin_Limitado* pJardin = NULL;

Jardin_Limitado*
obtener_jardin(unsigned int limite) {
  if (pJardin == NULL && limite < MAX_VISITANTES) {
    pJardin = malloc(sizeof(struct Jardin_Limitado));
    pJardin->contador = 0;
    pJardin->h_sem_mutex = CreateSemaphore(NULL,
					   1,
					   1,
					   NULL);
    if (pJardin->h_sem_mutex == NULL) {
      free(pJardin);
      pJardin = NULL;
    }

    pJardin->h_sem_limite = CreateSemaphore(NULL,
					    limite,
					    limite,
					    NULL);

    if (pJardin->h_sem_limite == NULL) {
      CloseHandle(pJardin->h_sem_mutex);
      free(pJardin);
      pJardin = NULL;
    }
  }

  return pJardin;
}

void
entrar_jardin(PJardin_Limitado jardin) {
  if (jardin != NULL) {
    WaitForSingleObject(jardin->h_sem_limite, INFINITE);
    WaitForSingleObject(jardin->h_sem_mutex, INFINITE);
    jardin->contador++;
    ReleaseSemaphore(jardin->h_sem_mutex, 1, NULL);
  }
}

void
salir_jardin(PJardin_Limitado jardin) {
 if (jardin != NULL) {
    WaitForSingleObject(jardin->h_sem_mutex, INFINITE);
    jardin->contador--;
    ReleaseSemaphore(jardin->h_sem_mutex, 1, NULL);
    ReleaseSemaphore(jardin->h_sem_limite, 1, NULL);
  }
}

int
obtener_miembros_jardin(PJardin_Limitado jardin) {
  int valor_retornar = -1;
  if (jardin != NULL) {
    WaitForSingleObject(jardin->h_sem_mutex, INFINITE);
    valor_retornar = jardin->contador;
    ReleaseSemaphore(jardin->h_sem_mutex, 1, NULL);
  }
  return valor_retornar;
}

void
cerrar_jardin(PJardin_Limitado jardin) {
  if (jardin != NULL) {
    CloseHandle(jardin->h_sem_mutex);
    CloseHandle(jardin->h_sem_limite);
    free((void *) jardin);
    pJardin = NULL;
  }
}
