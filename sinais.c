#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Inicializa Mutex e variavel
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;
int sinal = 0; // Variável de controle para o sinal

// Função executada pela thread sinalizadora
void* thread_sinalizadora(void* arg) {
    printf("Thread sinalizadora: iniciando tarefa...\n");
    sleep(2); 

    // Bloqueia o mutex antes de modificar o valor do sinal
    pthread_mutex_lock(&mutex);
    sinal = 1; // Sinal para indicar que a tarefa foi concluída
    printf("Thread sinalizadora: tarefa concluída. Enviando sinal...\n");

    // Sinaliza a thread ouvinte para que ela possa continuar
    pthread_cond_signal(&cond_var);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

// Função executada pela thread ouvinte
void* thread_ouvinte(void* arg) {
    printf("Thread ouvinte: aguardando sinal...\n");

    // Bloqueia o mutex para verificar a condição do sinal
    pthread_mutex_lock(&mutex);

    // Aguarda o sinal da thread sinalizadora
    while (sinal == 0) {
        pthread_cond_wait(&cond_var, &mutex); // Libera o mutex e espera pelo sinal
    }

    // Quando recebe o sinal, prossegue com a execução
    printf("Thread ouvinte: sinal recebido! Continuando execução...\n");
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    pthread_t t1, t2;

    // Cria as threads ouvinte e sinalizadora
    pthread_create(&t1, NULL, thread_ouvinte, NULL);
    pthread_create(&t2, NULL, thread_sinalizadora, NULL);

    // Espera as threads terminarem
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Destrói o mutex e a variável de condição
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_var);

    printf("Programa concluído.\n");
    return 0;
}
