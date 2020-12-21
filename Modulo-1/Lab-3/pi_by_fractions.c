/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 3 - Laboratório: 1 */
/* Codigo: Achando Pi por uma soma de frações */
/* Alunos: Sidney Outeiro & Raphael Novello*/

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <math.h>
#include "timer.h"

double* vetor; //vetor de termos
double* vetor_resultados_parciais;
int dim; // dimensão do vetor de termos
int n_threads; // numero de threads a ser usadas

typedef struct {
   long int id, dim_bloco, comeco_bloco, fim_bloco;
} t_Args;

void* soma_con(void* arg){
    t_Args* args = (t_Args*) arg;

    for(register int i=args->comeco_bloco; i<(args->fim_bloco);i++){
       vetor_resultados_parciais[args->id]+=vetor[i];
    }
    pthread_exit(NULL);
}


void preenche_vetor(){
    for(register int i=0;i<dim;i++){
        vetor[i]= 1.0/(double)(i*2+1) * pow(-1.0,(double) i); // preenche o vetor com o inverso dos numeros impares e alternando o sinal
    }
}

int main(int argc, char* argv[]) {
    pthread_t *tid;
    double sum=0;
    double start_seq, finish_seq;
    double start_con, finish_con;
    
    if(argc < 3){
        fprintf(stderr,"Digite: %s <quantidade de termos> <numero de threads>", argv[0]);
        return 1;
    }
     printf("Inicializando estruturas!\n");

    dim = atoi(argv[1]);//quantidade de termos a serem usados
    n_threads = atoi(argv[2]);// quantidades de threads a serem usadas

    vetor = (double *) malloc(sizeof(double) * dim);
    if(vetor==NULL){fprintf(stderr,"Erro no malloc\n");return 2;}
    
    preenche_vetor();//função para preencher o vetor com os termos que serão usados


    if (n_threads <= 1){// caso o usuario não queria usar mais threads além da principal
        GET_TIME(start_seq);
        for(register int i=0;i<dim;i++){
            sum+=vetor[i];
        }
        GET_TIME(finish_seq);
    }else //caso o usuario queria usar mais threads além da principal
    {
        t_Args* args =(t_Args*)malloc(sizeof(t_Args)*n_threads);
        if(args==NULL){fprintf(stderr,"Erro no malloc\n");return 2;}


        vetor_resultados_parciais = (double*)malloc(sizeof(double)*n_threads); // vetor onde cada thread irá inserir seu resultado parcial
        if(vetor_resultados_parciais==NULL){fprintf(stderr,"Erro no malloc\n");return 2;}

        //inicializando o vetor de resultados parciais
        for(register int i=0; i<n_threads;i++)
            vetor_resultados_parciais[i]=0;

        tid = (pthread_t *) malloc(sizeof(pthread_t)* n_threads);
        if(tid==NULL){fprintf(stderr,"Erro no malloc\n");return 2;}

        GET_TIME(start_con);
        

        for(register int i=0;i<n_threads;i++){
            (args+i)->id = i;
            (args+i)->dim_bloco=dim/n_threads;
            (args+i)->comeco_bloco=i* (args+i)->dim_bloco;
            
            if(i == n_threads-1) (args+i)->fim_bloco=dim;//caso seja a ultima thread 
            else (args+i)->fim_bloco=(args+i)->comeco_bloco+(args+i)->dim_bloco;//caso não seja a ultima thread 

            if(pthread_create(tid+i,NULL,soma_con,(void *) (args+i))){
                fprintf(stderr, "Erro no pthread create");
                return 3;
            }
        }

        for(register int i=0;i<n_threads;i++){
            if(pthread_join(*(tid+i),NULL)){
                fprintf(stderr, "Erro no pthread join");
                return 4;
            }
            sum+=vetor_resultados_parciais[i];//junta as somas parciais
        }
        free(vetor_resultados_parciais);
        free(args);
        GET_TIME(finish_con);
    }

    
    printf("aproximação pi: %lf\n",sum*4);
    if(n_threads <= 1){printf("Tempo seq: %.lf\n",finish_seq-start_seq);}
    else{printf("Tempo con: %lf\n",finish_con-start_con);}

    free(vetor);
    return 0;
}