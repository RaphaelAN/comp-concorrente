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
   int iterations, thread_num, num_threads, multiplier;
} t_Args;

//funcao executada pelas threads
void *add_fractions (void *arg) {
    t_Args *args = (t_Args *) arg;
    double thread_num = args->thread_num;
    long long int iterations = args->iterations;
    double num_threads = args->num_threads;
    double multiplier = args->multiplier;
    double* sum = (double*) malloc(sizeof(double));
    *sum = 0;
    if (sum == NULL) {printf("--ERRO: malloc()\n\n"); exit(-1);}
    
    for(double i = thread_num + 1; i < iterations * 2; i += 2 * num_threads){
        *sum += (1.0/i) * multiplier;
    }
    pthread_exit((void*) sum);
}

int main(int argc, char* argv[]) {
    long long int iterations;
    int num_threads;
    double pi_aproximation;
    double* thread_join_result;

    double start, end, elapsed, total_time = 0;
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

    printf("\nExecutando a aproximação de pi com %lld iterações e %d threads:\n", iterations, num_threads);

    GET_TIME(start);
    int multiplier = 1;
    for(int i = 0; i < num_threads; i++){
        (args + i)->thread_num = i;
        (args + i)->iterations = iterations;
        (args + i)->num_threads = num_threads;
        (args + i)->multiplier = multiplier;
        printf("--Cria a thread %d\n\n", i);
        if (pthread_create(&tid_sistema[i], NULL, add_fractions, (void*) (args+i))) {
            printf("--ERRO: pthread_create()\n\n"); exit(-1);
        }
        multiplier = multiplier * -1;
    }
    for (int i = 0; i < num_threads; i++) {
        if (pthread_join(tid_sistema[i], (void**) &thread_join_result)) {
            printf("--ERRO: pthread_join() \n\n"); exit(-1); 
        }
        pi_aproximation += *thread_join_result;
        printf("thread: %d  - %e\n", i,*thread_join_result);
        free(thread_join_result);
    }

    GET_TIME(end);
    elapsed = end - start;
    total_time += elapsed;
    printf("Execução levou: %lf segundos\n", elapsed);

    printf("Finalizando o programa\n");
    GET_TIME(start);
    pi_aproximation = pi_aproximation * 4;
    printf("\nPi foi calculado em: %e \n", pi_aproximation);
    printf("\nErro para a constante da biblioteca math.h: \n");
    printf("-- Erro absoluto: %e\n", M_PI - pi_aproximation);
    printf("-- Erro relativo: %e\n", M_PI - pi_aproximation/ M_PI);

    free(args);

    GET_TIME(end);
    elapsed = end - start;
    total_time += elapsed;
    printf("\nFinalização levou: %e segundos\n", elapsed);
    printf("Tempo total: %e segundos\n", total_time);

}