#pragma once
#include <semaphore.h>

#define MAX_NUMERO_VISITANTES 5
#define MAX_VISITANTES 20
#define MAX_VISITAS    10
#define TIEMPO_AFUERA  1    // Un segundo
#define TIEMPO_VISITA  5 // Cinco segundos

struct Jardin_Limitado {
  sem_t sem_mutex;
  sem_t sem_limite;
  int contador;
};

typedef struct Jardin_Limitado Jardin_Limitado;
typedef struct Jardin_Limitado *PJardin_Limitado;

PJardin_Limitado obtener_jardin(unsigned int limite);
void entrar_jardin(PJardin_Limitado jardin);
void salir_jardin(PJardin_Limitado jardin);
int obtener_miembros_jardin(PJardin_Limitado jardin);
void cerrar_jardin(PJardin_Limitado jardin);
