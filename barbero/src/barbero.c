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

    int n_clientes = obtener_var(NUMERO_CLIENTES_ESPERA);
}

int main(int argc, char const *argv[])
{
    pid_t pid = getpid();

    obtener_sem_mem();

    return EXIT_SUCCESS;
}