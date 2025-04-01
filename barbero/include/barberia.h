// Semáforos
#define MUTEX_ESPERA "mutex"
#define SILLON "sillon"
#define SILLAS "sillas"

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
int n_procesos_clientes, longitud_tabla_procesos;

int instalar_señal();

void señal(int señal);

void crear_sem_mem(int n_clientes);

void crear_tabla_procesos(int n_clientes);

void crear_proceso(int i, const char *ruta, const char *nombre);

void finalizar_procesos(int n_clientes, const char *nombre);

void cerrar_procesos();

void liberar_memoria();

//  Funciones y variables Cliente y Barbero
static sem_t *mutex, *sillon, *sillas;
int clientes_espera;

void obtener_sem_mem();

void incrementar_clientes_espera(int n);