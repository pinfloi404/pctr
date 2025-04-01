#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <barberia.h>

#include <memoriaI.h>
#include <semaforoI.h>

#define ROJO "\033[31m"
#define VERDE "\033[32m"
#define AZUL "\033[34m"

struct Tabla_Procesos *tabla_procesos;

int instalar_señal() {
    int ok = 0;

    if (signal(SIGINT, señal) == SIG_ERR)
    {
        ok = -1;
    }

    return ok;
}

void señal(int señal) {
    if (señal == SIGINT)
    {
        fprintf(stdout, ROJO "\n🚨🚨🚨 DESALOJAMOS 🚨🚨🚨\n");
    }

    exit(EXIT_SUCCESS);
}

void crear_sem_mem(int n_clientes) {
    //  Semáforo Mutex
    crear_sem(MUTEX_ESPERA, 1);

    //  Semáforo Barbero 
    crear_sem(BARBERO, 0);

    //  Semáforo Sillón
    crear_sem(SILLON, 0);

    //  Semáforo Sillas
    crear_sem(SILLAS, 0);

    //  Variable de número de clientes
    crear_var(NUMERO_CLIENTES, n_clientes);
}

void crear_tabla_procesos(int n_clientes) {
    n_procesos_clientes = n_clientes;

    tabla_procesos = malloc(longitud_tabla_procesos * sizeof(struct Tabla_Procesos));

    for (int i = 0; i < longitud_tabla_procesos; i++)
    {
        tabla_procesos[i].pid = 0;
    }
}

void crear_proceso(int i, const char *ruta, const char *nombre) {
    pid_t pid;

    switch (pid = fork())
    {
    case -1:
        fprintf(stderr, ROJO "No se ha podido lanzar el proceso %s: %s.\n", nombre, strerror(errno));
        exit(EXIT_FAILURE);
        break;

    case 0:
        if (execl(ruta, nombre, NULL) == -1)
        {
            fprintf(stderr, ROJO "No se ha podido ejecutar el proceso %s: %s.\n", nombre, strerror(errno));
            exit(EXIT_FAILURE);
        }

        tabla_procesos[i].pid = pid;
        tabla_procesos[i].nombre = nombre;
        break;
    }
}

void finalizar_procesos(int n_clientes, const char *nombre) {
    pid_t pid;

    while (n_procesos_clientes > 0)
    {
        pid = wait(NULL);

        for (int i = 1; i < n_clientes; i++)
        {
            if (pid == tabla_procesos[i].pid)
            {
                fprintf(stdout, ROJO "El %s [%d] se ha ido muy contento :)\n", tabla_procesos[i].nombre, tabla_procesos[i].pid);

                tabla_procesos[i].pid = 0;
                n_procesos_clientes--;
                break;
            }
        }
    }

    fprintf(stdout, ROJO "\n🚨🚨🚨 Todos los procesos %s han terminado. 🚨🚨🚨\n", nombre);
}

void cerrar_procesos() {
    fprintf(stdout, ROJO "\n[MANAGER] 🚨🚨🚨 Terminando todos los procesos de %s y %s 🚨🚨🚨\n\n", BARBERO, CLIENTE);
    
    for (int i = 0; i < longitud_tabla_procesos; i++)
    {
        if (tabla_procesos[i].pid != 0)
        {
            fprintf(stdout, ROJO "Terminando al %s [%d]...\n", tabla_procesos[i].nombre, tabla_procesos[i].pid);

            if (kill(tabla_procesos[i].pid, SIGINT) == -1)
            {
                fprintf(stderr, ROJO "No se ha podido matar al proceso %d: %s.\n", tabla_procesos[i].pid, strerror(errno));
            }
        }
    }
}

void liberar_memoria() {
    destruir_sem(MUTEX_ESPERA);
    destruir_sem(SILLON);
    destruir_sem(SILLAS);

    destruir_var(NUMERO_CLIENTES);

    free(tabla_procesos);
}

int main(int argc, char const *argv[])
{
    int n_clientes = 4, longitud_tabla_procesos = (n_clientes + 1);

    if (instalar_señal() == -1)
    {
        fprintf(stderr, ROJO "No se puede añadir Ctrl + C 😔😔😔.\n%s", strerror(errno));

        return -1;
    }

    crear_sem_mem(n_clientes);

    crear_tabla_procesos(n_clientes);

    crear_proceso(0, RUTA_BARBER, BARBERO);

    for (int i = 1; i < longitud_tabla_procesos; i++)
    {
        crear_proceso(i, RUTA_CLIENTE, CLIENTE);
    }

    finalizar_procesos(n_clientes, CLIENTE);

    liberar_memoria();

    return EXIT_SUCCESS;
}