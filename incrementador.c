/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 1 */
/* Codigo: "incrementador" usando threads em C para incrementar todos os indices de um vetor */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define VECTOR_SIZE 100 //total de threads a serem criadas
#define NTHREADS 2

int vector[VECTOR_SIZE];

//cria a estrutura de dados para armazenar os argumentos da thread
typedef struct {
   int start, stop;
} t_Args;

//funcao executada pelas threads
void *increment (void *arg) {
  t_Args *args = (t_Args *) arg;

  printf("Incrementando os indexes de %d a %d\n\n", args->start, args->stop);
  int counter;
  for(counter = args->start; counter < args->stop; counter++){
      vector[counter] = 1;
  }
  free(arg); //aqui pode liberar a alocacao feita na main

  pthread_exit(NULL);
}

//funcao principal do programa
int main() {
  pthread_t tid_sistema[NTHREADS]; //identificadores das threads no sistema
  int thread; //variavel auxiliar
  t_Args *arg; //receberá os argumentos para a thread

  int thread_partition_size = VECTOR_SIZE / NTHREADS; // determines the number of indexes each thread will fill
  int current_index = 0;
  printf("--Imprimindo valores no vetor\n\n");
  for(int i = 0; i < VECTOR_SIZE; i++){
      printf("%d, ", vector[i]);
    }

  for(thread=0; thread<NTHREADS; thread++) {
    printf("\n--Aloca e preenche argumentos para thread %d\n", thread);
    arg = malloc(sizeof(t_Args));
    if (arg == NULL) {
      printf("--ERRO: malloc()\n\n"); exit(-1);
    }
    arg->start = current_index;
    if(thread + 1 == NTHREADS) {
        arg->stop = VECTOR_SIZE;
    }
    else {
        arg->stop = current_index + thread_partition_size;
        current_index += thread_partition_size;
    }
    printf("--Cria a thread %d\n\n", thread);
    if (pthread_create(&tid_sistema[thread], NULL, increment, (void*) arg)) {
      printf("--ERRO: pthread_create()\n\n"); exit(-1);
    }
  }

  for (thread=0; thread<NTHREADS; thread++) {
    if (pthread_join(tid_sistema[thread], NULL)) {
         printf("--ERRO: pthread_join() \n\n"); exit(-1); 
    } 
  }

  for(int i = 0; i < VECTOR_SIZE; i++){
      printf("%d, ", vector[i]);
  }

  printf("\n\n--Thread principal terminou\n\n");
  pthread_exit(NULL);
}
