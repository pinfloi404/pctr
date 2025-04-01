// Semáforos
#define MUTEX_ESPERA "mutex"
#define SILLON "sillon"
#define SILLAS "sillas"

//  Memoria compartida
#define NUMERO_CLIENTES "clientes"

//  Nombres y rutas
#define BARBERO "barbero"
#define RUTA_BARBER  "./exec/barbero"

#define CLIENTE "cliente"
#define RUTA_CLIENTE "./exec/cliente"

struct Tabla_Procesos {
    pid_t pid;
    char *nombre;
};

int n_procesos_clientes, longitud_tabla_procesos;

int instalar_señal();

void señal(int señal);

void crear_sem_mem(int n_clientes);

void crear_tabla_procesos(int n_clientes);

void crear_proceso(int i, const char *ruta, const char *nombre);

void finalizar_procesos(int n_clientes, const char *nombre);

void cerrar_procesos();

void liberar_memoria();