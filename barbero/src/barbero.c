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

void decrementar_clientes_espera() {
    wait_sem(mutex);

    consultar_var(n_clientes_espera, &n);
    modificar_var(n_clientes_espera, --n);
    consultar_var(n_clientes_espera, &n);

    signal_sem(mutex);
}

int main(int argc, char const *argv[])
{
    pid_t pid = getpid();

    obtener_sem_mem();

    fprintf(stdout, VERDE "El [barbero / %d] está durmiendo.\n", pid);

    while (1) {
        // Espera a que un cliente lo despierte
        wait_sem(barbero);

        fprintf(stdout, VERDE "El [barbero / %d] se despierta.\n", pid);

        // Deja el sillón libre
        signal_sem(sillon);

        // Se decrementa el número de clientes en espera
        int n_a = n;
        decrementar_clientes_espera(pid);

        fprintf(stdout, VERDE "El cliente se sienta en el sillón... [%d] -> [%d] y el [barbero / %d] le hace un cortecito CR7.\n", n_a, n, pid);

        // Simula el tiempo que tarda el corte de pelo
        sleep(rand() % 10 + 5);

        // Finaliza el corte
        signal_sem(corte);

        fprintf(stdout, VERDE "El [barbero / %d] termina de cortar el pelo y se vuelve a dormir.\n", pid);
    }

    return EXIT_SUCCESS;
}