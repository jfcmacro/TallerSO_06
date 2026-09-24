#pragma once
#include <windows.h>

#define MAX_VISITANTES 20
#define MAX_VISITAS    10
#define TIEMPO_AFUERA  1000 // Un segundo
#define TIEMPO_VISITA  5000 // Cinco segundos

struct Jardin {
  HANDLE h_sem_mutex;
  int contador;
};

typedef struct Jardin Jardin;
typedef struct Jardin *PJardin;

PJardin obtener_jardin();
void entrar_jardin(PJardin jardin);
void salir_jardin(PJardin jardin);
int obtener_miembros_jardin(PJardin jardin);
void cerrar_jardin(PJardin jardin);
