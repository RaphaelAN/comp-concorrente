/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 6 */
/* Codigo: Atividade 5 */
/* Alunos: Sidney Outeiro & Raphael Novello */

import java.util.Arrays;


//--PASSO 1: cria uma classe que guarda e maneja os recursos compartilhados
class Vetor {
    private int size;
    private int vetor[];
    
    public Vetor(int size, int valorInicial){
        this.size = size;
        this.vetor = new int[size];
        Arrays.fill(this.vetor, valorInicial);
    }
    
    public void imprimir(){
        System.out.println(Arrays.toString(this.vetor));
    }
    
    public void editar(int index, int valor) {
        this.vetor[index] = valor;
    }
    
    public int get(int index) {
        return this.vetor[index];
    }
}


//--PASSO 2: cria uma classe que estende a classe Thread 
class Somador extends Thread {
   private int tid;
   private int vectorSize;
   private int numThreads;
   Vetor a;
   Vetor b; 
   Vetor c;
   
   //--construtor
   public Somador(int tid, int vectorSize, int numThreads, Vetor a, Vetor b, Vetor c) { 
      this.tid = tid;
      this.vectorSize = vectorSize;
      this.numThreads = numThreads;
      this.a = a;
      this.b = b;
      this.c = c;
   }

   //--metodo executado pela thread
   public void run() {
      int chunk = (vectorSize/numThreads);
      int start = tid * chunk;
      int end = start + chunk;
      if(end > vectorSize) {
          end = vectorSize;
      }
      for(int i = start; i < end; i++)
      this.c.editar(i, this.a.get(i) + this.b.get(i));
   }
}

//--classe do metodo main
class Main {
   static final int numThreads = 10;
   static final int vectorSize = 100;

   public static void main (String[] args) {
      //--reserva espaço para um vetor de threads
      Thread[] threads = new Thread[numThreads];
      Vetor a = new Vetor(vectorSize, 2);
      Vetor b = new Vetor(vectorSize, 4); 
      Vetor c = new Vetor(vectorSize, 0);
    
      //--PASSO 3: cria threads da classe que estende Thread
      for (int i=0; i<threads.length; i++) {
         threads[i] = new Somador(i, vectorSize, numThreads, a, b, c);
      }

      //--PASSO 4: iniciar as threads
      for (int i=0; i<threads.length; i++) {
         threads[i].start();
      }

      //--PASSO 5: esperar pelo termino de todas as threads
      for (int i=0; i<threads.length; i++) {
         try { threads[i].join(); } 
         catch (InterruptedException e) { return; }
      }
      System.out.println("Vetor A: ");
      a.imprimir();
      
      System.out.println("Vetor B: ");
      b.imprimir();
      
      System.out.println("Vetor C: ");
      c.imprimir();

      System.out.println("Terminou"); 
   }
}