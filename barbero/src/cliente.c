#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <barberia.h>

#include <memoriaI.h>
#include <semaforoI.h>

void obtener_sem_mem() {
    mutex = get_sem(MUTEX_ESPERA);
    sillon = get_sem(SILLON);
    sillas = get_sem(SILLAS);

    clientes_espera = obtener_var(NUMERO_CLIENTES_ESPERA);
}

void incrementar_clientes_espera(int n) {
    consultar_var(clientes_espera, &n);
    modificar_var(clientes_espera, ++n);
    consultar_var(clientes_espera, &n);
}

int main(int argc, char const *argv[])
{
    int n;

    pid_t pid = getpid();
    srand(pid);

    obtener_sem_mem();

    fprintf(stdout, AZUL "El cliente [%d] entra en la barbería.", pid);

    sleep(rand() % 31 + 30);

    wait_sem(mutex);

    incrementar_clientes_espera(n);

    return EXIT_SUCCESS;
}