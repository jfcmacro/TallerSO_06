#pragma once

#define TUBERIA_PETICION "/tmp/tuberia_peticion"
#define TUBERIA_SOLICITUD  "/tmp/tuberia_solicitud"

void crear_tuberia(const char* nombre);
void borrar_tuberia(const char* nombre);
