#pragma once

#define NOMBRE_SERVICIO TEXT("svrwindows")
#define NOMBRE_SERVICIO_PANTALLA TEXT("Servicio Windows Creado de Prueba")

typedef enum Estados_Servicio {
  NO_INICIADO,
  INICIANDO,
  CORRIENDO,
  PAUSANDO,
  PAUSADO,
  TERMINANDO,
  TERMINADO } ESTADOS_SERVICIO;
