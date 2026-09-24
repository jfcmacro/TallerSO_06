#pragma once

#define NOM_SEM_AGENTE  "agente"
#define NOM_SEM_CERILLA "cerilla"
#define NOM_SEM_PAPEL   "papel"
#define NOM_SEM_TABACO  "tabaco"

#define PRE_GLOBAL "/"

#define MAX_TIEMPO_PREPARANDO 4
#define MAX_TIEMPO_FUMANDO 5

char* obtener_nombre_semaforo(char const* prefijo, char const* nom_sem);

enum SEM_ID { CERILLA, PAPEL, TABACO, AGENTE };

typedef enum SEM_ID SEM_ID;
