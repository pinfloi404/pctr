#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "memoriaI.h"

int crear_var(const char *name, int valor)
{
    int shm_fd, *p;

    // Abre el objeto de memoria compartida.
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1)
    {
        fprintf(stderr, "Error al crear la variable: %s\n", strerror(errno));
        exit(1);
    }

    // Establecer el tamaño.
    if (ftruncate(shm_fd, sizeof(int)) == -1)
    {
        fprintf(stderr, "Error al truncar la variable: %s\n", strerror(errno));
        exit(1);
    }

    // Mapeo del objeto de memoria compartida.
    p = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (p == MAP_FAILED)
    {
        fprintf(stderr, "Error al mapear la variable: %s\n", strerror(errno));
        exit(1);
    }

    *p = valor;
    munmap(p, sizeof(int));

    return shm_fd;
}
int obtener_var(const char *name)
{
    int shm_fd;

    // Abre el objeto de memoria compartida.
    shm_fd = shm_open(name, O_RDWR, 0666);
    if (shm_fd == -1)
    {
        fprintf(stderr, "Error al obtener la variable: %s\n", strerror(errno));
        exit(1);
    }

    return shm_fd;
}

void destruir_var(const char *name)
{
    int shm_fd = obtener_var(name);

    if (close(shm_fd) == -1)
    {
        fprintf(stderr, "Error al destruir la variable: %s\n", strerror(errno));
        exit(1);
    }

    if (shm_unlink(name) == -1)
    {
        fprintf(stderr, "Error al destruir la variable: %s\n", strerror(errno));
        exit(1);
    }
}

void modificar_var(int shm_fd, int valor)
{
    int *p;

    // Mapeo del objeto de memoria compartida.
    p = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (p == MAP_FAILED)
    {
        fprintf(stderr, "Error al mapear la variable: %s\n", strerror(errno));
        exit(1);
    }

    *p = valor;

    munmap(p, sizeof(int));
}
void consultar_var(int shm_fd, int *valor)
{
    int *p;

    // Mapeo del objeto de memoria compartida.
    p = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (p == MAP_FAILED)
    {
        fprintf(stderr, "Error al mapear la variable: %s\n", strerror(errno));
        exit(1);
    }

    *valor = *p;

    munmap(p, sizeof(int));
}
