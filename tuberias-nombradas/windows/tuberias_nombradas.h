#pragma once

#define TUBERIA_PETICION  "\\\\.\\pipe\\tub_peticion"
#define TUBERIA_RESPUESTA "\\\\.\\pipe\\tub_respuesta"

void crear_tuberia(const char* nombre);
void borrar_tuberia(const char* nombre);
