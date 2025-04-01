#ifndef __VARIABLEI_H__
#define __VARIABLEI_H__

// Crea un objeto de memoria compartida y devuelve el descriptor de archivo.
int crear_var (const char *name, int valor);

// Obtiene el descriptor asociado a la variable.
int obtener_var (const char *name);

// Destruye el objeto de memoria compartida.
void destruir_var (const char *name);

// Modifica el valor del objeto de memoria compartida.
void modificar_var (int shm_fd, int valor);

// Devuelve el valor del objeto de memoria compartida.
void consultar_var (int shm_fd, int *valor);

#endif

