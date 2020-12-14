/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 1 */
/* Codigo: "Hello World" usando threads em C passando mais de um argumento */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include "timer.h"

float *matrix_A;
float *adjancent_B;
float *result;

typedef struct {
   int matrix_size, thread_num, num_threads;
} t_Args;

//funcao executada pelas threads
void *multiply (void *arg) {
    t_Args *args = (t_Args *) arg;
    int thread_num = args->thread_num;
    int matrix_size = args->matrix_size;
    int num_threads = args->num_threads;
    int line;
    int column;
    double sum;
    for(line = thread_num; line < matrix_size; line += num_threads){
        for(column = 0; column < matrix_size; column++)
        {
            sum = 0;
            for(int i = 0; i < matrix_size; i++){
                sum +=  matrix_A[i + line*matrix_size] * adjancent_B[i + line*matrix_size];
            }
            result[column + line*matrix_size] = sum;
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    int matrix_dimension;
    int num_threads;
    double start, end, elapsed, total_time;
    if(argc < 3){
        printf("Digite: %s <dimensões da matriz> <numero de threads>", argv[0]);
        return 1;
    }
    printf("Inicializando estruturas!\n");
    GET_TIME(start);

    matrix_dimension = atoi(argv[1]);
    num_threads = atoi(argv[2]);
    pthread_t tid_sistema[num_threads];

    t_Args *args = (t_Args*) malloc(sizeof(t_Args) * num_threads); //receberá os argumentos para a thread
    if (args == NULL) {printf("--ERRO: malloc()\n\n"); exit(-1);}
    matrix_A = (float*) malloc(sizeof(float) * matrix_dimension * matrix_dimension);
    if (matrix_A == NULL) {printf("ERROR -- maloc\n"); return 2;}
    adjancent_B = (float*) malloc(sizeof(float) * matrix_dimension * matrix_dimension);
    if (adjancent_B == NULL) {printf("ERROR -- maloc\n"); return 2;}
    result = (float*) malloc(sizeof(float) * matrix_dimension * matrix_dimension);
    if (result == NULL) {printf("ERROR -- maloc\n"); return 2;}

    for(int i = 0; i < matrix_dimension; i++){
        for(int j = 0; j < matrix_dimension; j++){
            matrix_A[j + i*matrix_dimension] = 1;
            adjancent_B[j + i*matrix_dimension] = 1;
            result[j + i*matrix_dimension] = 0;
        }
    }

    GET_TIME(end);
    elapsed = end - start;
    total_time += elapsed;
    printf("Inicialização levou: %lf segundos\n", elapsed);

    printf("\nExecutando a multiplicação de matrizes AxB = R de %d dimensões com %d threads:\n", matrix_dimension, num_threads);

    GET_TIME(start);

    for(int i = 0; i < num_threads; i++){
        (args + i)->thread_num = i;
        (args + i)->matrix_size = matrix_dimension;
        (args + i)->num_threads = num_threads;
        printf("--Cria a thread %d\n\n", i);
        if (pthread_create(&tid_sistema[i], NULL, multiply, (void*) (args+i))) {
            printf("--ERRO: pthread_create()\n\n"); exit(-1);
        }
    }

    for (int i = 0; i < num_threads; i++) {
        if (pthread_join(tid_sistema[i], NULL)) {
            printf("--ERRO: pthread_join() \n\n"); exit(-1); 
        } 
    }

    GET_TIME(end);
    elapsed = end - start;
    total_time += elapsed;
    printf("Execução levou: %lf segundos\n", elapsed);

    printf("Finalizando o programa\n");
    GET_TIME(start);

    //show results - Desabilitado pois printar matriz de 2000 x 2000 é inviavel.
    /*
    printf("\nMatriz A\n");
    for(int i = 0; i < matrix_dimension; i++){
        for(int j = 0; j < matrix_dimension; j++){
            printf("%.1f ", matrix_A[j + i*matrix_dimension]);
        }
        puts("");
    }
    printf("\nMatriz B\n");
    for(int i = 0; i < matrix_dimension; i++){
        for(int j = 0; j < matrix_dimension; j++){
            printf("%.1f ", matrix_A[i + j*matrix_dimension]);
        }
        puts("");
    }
    printf("\nMatriz Resultado\n");
    for(int i = 0; i < matrix_dimension; i++){
        for(int j = 0; j < matrix_dimension; j++){
            printf("%.1f ", result[j + i*matrix_dimension]);
        }
        puts("");
    }
    */
    
    free(matrix_A);
    free(adjancent_B);
    free(result);
    free(args);

    GET_TIME(end);
    elapsed = end - start;
    total_time += elapsed;
    printf("Finalização levou: %lf segundos\n", elapsed);
    printf("Tempo total: %lf segundos\n", total_time);

}
