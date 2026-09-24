#include <string.h>
#include <stdlib.h>
#include "fumadores.h"

char* obtener_nombre_semaforo(char const* prefijo, char const* nom_sem) {
  size_t tam = strlen(prefijo) + strlen(nom_sem) + 1;
  char *valor_ret = malloc(tam);
  valor_ret[0] = '\0';
  strcpy(valor_ret, prefijo);
  strcat(valor_ret, nom_sem);

  return valor_ret;
}
