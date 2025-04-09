#include <memoriaI.h>
#include <semaforoI.h>

// Semáforos
#define MUTEX "mutex"
#define BARBERO "barbero"
#define SILLON "sillon"
#define CORTE "corte"

//  Memoria compartida
#define NUMERO_CLIENTES_ESPERA "clientes_espera"

//  Nombres y rutas
#define BARBERO "barbero"
#define RUTA_BARBER  "./exec/barbero"

#define CLIENTE "cliente"
#define RUTA_CLIENTE "./exec/cliente"

//  Colores
#define ROJO "\033[31m"
#define VERDE "\033[32m"
#define AZUL "\033[34m"

//  Tabla de procesos
struct Tabla_Procesos {
    pid_t pid;
    char *nombre;
};

//  Funciones y variables Barbería
int instalar_señal();

void señal(int señal);

void crear_sem_mem();

void crear_tabla_procesos();

void crear_proceso(int i, char *ruta, char *nombre);

void finalizar_procesos_clientes();

void cerrar_procesos();

void liberar_memoria();

//  Funciones Cliente y Barbero
void obtener_sem_mem();

void incrementar_clientes_espera();

void decrementar_clientes_espera();