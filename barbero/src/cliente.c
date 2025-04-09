#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <barberia.h>

#include <memoriaI.h>
#include <semaforoI.h>

sem_t *mutex, *barbero, *sillon, *corte;
int n_clientes_espera, n;

void obtener_sem_mem() {
    mutex = get_sem(MUTEX);
    barbero = get_sem(BARBERO);
    sillon = get_sem(SILLON);
    corte = get_sem(CORTE);

    n_clientes_espera = obtener_var(NUMERO_CLIENTES_ESPERA);
}

void incrementar_clientes_espera() {
    wait_sem(mutex);

    consultar_var(n_clientes_espera, &n);
    modificar_var(n_clientes_espera, ++n);
    consultar_var(n_clientes_espera, &n);

    signal_sem(mutex);
}

int main(int argc, char const *argv[])
{
    pid_t pid = getpid();
    srand(pid);

    obtener_sem_mem();

    fprintf(stdout, AZUL "El [cliente / %d] entra en la barbería.\n", pid);
    
    //  Se incrementa el número de clientes en espera
    int n_a = n;
    incrementar_clientes_espera(pid);

    fprintf(stdout, AZUL "El [cliente / %d] se sienta esperar... [%d] -> [%d]\n", pid, n_a, n);

    // Despierta al barbero
    signal_sem(barbero);

    fprintf(stdout, AZUL "El [cliente / %d] despierta al barbero.\n", pid);

    // Espera a que el sillón esté libre
    wait_sem(sillon);

    fprintf(stdout, AZUL "El [cliente / %d] ocupa el sillón para que le corten el pelo.\n", pid);

    // Espera a que le corten el pelo
    wait_sem(corte);

    fprintf(stdout, AZUL "El [cliente / %d] se va muy contento :)\n", pid);

    return EXIT_SUCCESS;
}