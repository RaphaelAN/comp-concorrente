/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 3 - Laboratório: 1 */
/* Codigo: Achando Pi por uma soma de frações */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <math.h>
#include "timer.h"


typedef struct {
   int iterations, thread_num, num_threads;
} t_Args;

//funcao executada pelas threads
void *add_fractions (void *arg) {
    t_Args *args = (t_Args *) arg;
    long double thread_num = args->thread_num;
    long long int iterations = args->iterations;
    long double num_threads = args->num_threads;
    long double* sum = (long double*) malloc(sizeof(long double));
    if (sum == NULL) {printf("--ERRO: malloc()\n\n"); exit(-1);}

    for(long double i = thread_num + 1; i < iterations; i += 2 * num_threads){
        *sum += 1/i * pow(-1, i - 1);
    }
    pthread_exit((void*) sum);
}

int main(int argc, char* argv[]) {
    long long int iterations;
    int num_threads;
    long double pi_aproximation;
    long double thread_join_result;

    double start, end, elapsed, total_time;
    if(argc < 3){
        printf("Digite: %s <iterações> <numero de threads>", argv[0]);
        return 1;
    }
    printf("Inicializando estruturas!\n");
    GET_TIME(start);

    iterations = atoll(argv[1]);
    num_threads = atoi(argv[2]);
    pthread_t tid_sistema[num_threads];

    t_Args *args = (t_Args*) malloc(sizeof(t_Args) * num_threads); //receberá os argumentos para a thread
    if (args == NULL) {printf("--ERRO: malloc()\n\n"); exit(-1);}

    GET_TIME(end);
    elapsed = end - start;
    total_time += elapsed;
    printf("Inicialização levou: %lf segundos\n", elapsed);

    printf("\nExecutando a aproximação de pi com %d iterações e %d threads:\n", iterations, num_threads);

    GET_TIME(start);

    for(int i = 0; i < num_threads; i++){
        (args + i)->thread_num = i;
        (args + i)->iterations = iterations;
        (args + i)->num_threads = num_threads;
        printf("--Cria a thread %d\n\n", i);
        if (pthread_create(&tid_sistema[i], NULL, add_fractions, (void*) (args+i))) {
            printf("--ERRO: pthread_create()\n\n"); exit(-1);
        }
    }
    for (int i = 0; i < num_threads; i++) {
        if (pthread_join(tid_sistema[i], (void**) &thread_join_result)) {
            printf("--ERRO: pthread_join() \n\n"); exit(-1); 
        }
        pi_aproximation += thread_join_result;
        free(&thread_join_result);
    }

    GET_TIME(end);
    elapsed = end - start;
    total_time += elapsed;
    printf("Execução levou: %lf segundos\n", elapsed);

    printf("Finalizando o programa\n");
    GET_TIME(start);
    pi_aproximation = pi_aproximation * 4;
    printf("\nPi foi calculado em: %e \n", pi_aproximation);
    printf("\nErro para a constante da biblioteca math.h\n: ");
    printf("-- Erro absoluto: %e\n", abs(M_PI - pi_aproximation));
    printf("-- Erro absoluto: %e\n", abs(M_PI - pi_aproximation)/ M_PI);

    free(args);

    GET_TIME(end);
    elapsed = end - start;
    total_time += elapsed;
    printf("Finalização levou: %e segundos\n", elapsed);
    printf("Tempo total: %e segundos\n", total_time);

}