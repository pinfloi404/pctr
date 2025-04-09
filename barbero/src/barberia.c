#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

#include <barberia.h>

#include <memoriaI.h>
#include <semaforoI.h>

int n_clientes, n_procesos_clientes, longitud_tabla_procesos;
struct Tabla_Procesos *tabla_procesos;

int comprobar_arguentos(const char *n_clientes_arg) {
    if (n_clientes_arg == NULL || strlen(n_clientes_arg) == 0)
    {
        fprintf(stderr, ROJO "Aquí no hay nada. [%s].\n┐(´～｀)┌\n", n_clientes_arg);
        return -1;
    }

    n_clientes = atoi(n_clientes_arg);

    if (n_clientes <= 0)
    {
        fprintf(stderr, ROJO "No es bueno. [%s] -> [%d].\n┐(´～｀)┌\n", n_clientes_arg, n_clientes);
        return -1;
    }

    return 0;
}

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
        fprintf(stdout, ROJO "\n\n🚨🚨🚨 DESALOJAMOS 🚨🚨🚨\n\n");
        finalizar_procesos_clientes();
        cerrar_procesos();
        liberar_memoria();
    }

    exit(EXIT_SUCCESS);
}

void crear_sem_mem() {
    //  Semáforo Mutex
    crear_sem(MUTEX, 1);

    //  Semáforo Barbero
    crear_sem(BARBERO, 0);

    //  Semáforo Sillón
    crear_sem(SILLON, 0);

    //  Semáforo Corte
    crear_sem(CORTE, 0);

    //  Variable de número de clientes
    crear_var(NUMERO_CLIENTES_ESPERA, 0);
}

void crear_tabla_procesos() {
    tabla_procesos = malloc(n_clientes * sizeof(struct Tabla_Procesos));

    tabla_procesos[0].pid = 0;

    for (int i = 0; i < longitud_tabla_procesos; i++)
    {
        tabla_procesos[i].pid = 0;
    }
}

void crear_proceso(int i, char *ruta, char *nombre) {
    pid_t pid;

    switch (pid = fork())
    {
    case -1:
        fprintf(stderr, ROJO "No se ha podido lanzar el proceso [%s]: %s.\n", nombre, strerror(errno));
        exit(EXIT_FAILURE);
        break;

    case 0:
        if (execl(ruta, nombre, NULL) == -1)
        {
            fprintf(stderr, ROJO "No se ha podido ejecutar el proceso [%s]: %s.\n", nombre, strerror(errno));
            exit(EXIT_FAILURE);
        }
        break;
    }

    fprintf(stdout, ROJO "Proceso [%s / %d] lanzado con éxito!\n", nombre, pid);

    tabla_procesos[i].pid = pid;
    tabla_procesos[i].nombre = nombre;
}

void finalizar_procesos_clientes() {
    pid_t pid;

    while (n_procesos_clientes > 0)
    {
        pid = wait(NULL);

        for (int i = 1; i < longitud_tabla_procesos; i++)
        {
            if (pid == tabla_procesos[i].pid)
            {
                fprintf(stdout, ROJO "El proceso [%s / %d] se ha finalizado.\n", tabla_procesos[i].nombre, tabla_procesos[i].pid);

                tabla_procesos[i].pid = 0;
                n_procesos_clientes--;
                break;
            }
        }
    }

    fprintf(stdout, ROJO "\n🚨🚨🚨 Todos los procesos [%s] han finalizado. 🚨🚨🚨\n", CLIENTE);
}

void cerrar_procesos() {
    fprintf(stdout, ROJO "\n🚨🚨🚨 Comprobando que lo procesos de [%s] y [%s] han terminado. 🚨🚨🚨\n", BARBERO, CLIENTE);
    
    for (int i = 0; i < longitud_tabla_procesos; i++)
    {
        if (tabla_procesos[i].pid != 0)
        {
            fprintf(stdout, ROJO "Terminando al proceso [%s / %d]...\n", tabla_procesos[i].nombre, tabla_procesos[i].pid);

            if (kill(tabla_procesos[i].pid, SIGINT) == -1)
            {
                fprintf(stderr, ROJO "No se ha podido matar al proceso [%s / %d]: %s.\n", tabla_procesos[i].nombre, tabla_procesos[i].pid, strerror(errno));
            }
        }
    }

    fprintf(stdout, ROJO "\n🚨🚨🚨 Todos los procesos de [%s] y [%s] han terminado. 🚨🚨🚨\n\n", BARBERO, CLIENTE);
}

void liberar_memoria() {
    destruir_sem(MUTEX);
    destruir_sem(BARBERO);
    destruir_sem(SILLON);

    destruir_var(NUMERO_CLIENTES_ESPERA);

    free(tabla_procesos);
}

int main(int argc, char const *argv[])
{
    const char *n_clientes_arg = argv[1];

    if (comprobar_arguentos(n_clientes_arg) == -1)
    {
        return EXIT_FAILURE;
    }

    n_procesos_clientes = n_clientes, longitud_tabla_procesos = (n_clientes + 1);

    if (instalar_señal() == -1)
    {
        fprintf(stderr, ROJO "No se puede añadir Ctrl + C\n┐(´～｀)┌.\n%s", strerror(errno));

        return -1;
    }

    crear_sem_mem();

    crear_tabla_procesos();

    crear_proceso(0, RUTA_BARBER, BARBERO);

    for (int i = 1; i < longitud_tabla_procesos; i++)
    {
        crear_proceso(i, RUTA_CLIENTE, CLIENTE);
    }

    finalizar_procesos_clientes();

    cerrar_procesos();

    liberar_memoria();

    return EXIT_SUCCESS;
}