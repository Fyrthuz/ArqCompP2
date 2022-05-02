#include <stdio.h>
#include <stdlib.h>
#include <pmmintrin.h>
#include <time.h>
#include <unistd.h>

#define N 512
#define bsize 8

void start_counter();
double get_counter();
double mhz();

/* Initialize the cycle counter */


 static unsigned cyc_hi = 0;
 static unsigned cyc_lo = 0;


 /* Set *hi and *lo to the high and low order bits of the cycle counter.
 Implementation requires assembly code to use the rdtsc instruction. */
 void access_counter(unsigned *hi, unsigned *lo)
 {
 asm("rdtsc; movl %%edx,%0; movl %%eax,%1" /* Read cycle counter */
 : "=r" (*hi), "=r" (*lo) /* and move results to */
 : /* No input */ /* the two outputs */
 : "%edx", "%eax");
 }

 /* Record the current value of the cycle counter. */
 void start_counter()
 {
 access_counter(&cyc_hi, &cyc_lo);
 }

 /* Return the number of cycles since the last call to start_counter. */
 double get_counter()
 {
 unsigned ncyc_hi, ncyc_lo;
 unsigned hi, lo, borrow;
 double result;

 /* Get cycle counter */
 access_counter(&ncyc_hi, &ncyc_lo);

 /* Do double precision subtraction */
 lo = ncyc_lo - cyc_lo;
 borrow = lo > ncyc_lo;
 hi = ncyc_hi - cyc_hi - borrow;
 result = (double) hi * (1 << 30) * 4 + lo;
 if (result < 0) {
 fprintf(stderr, "Error: counter returns neg value: %.0f\n", result);
 }
 return result;
 }

double mhz(int verbose, int sleeptime)
 {
 double rate;

 start_counter();
 sleep(sleeptime);
 rate = get_counter() / (1e6*sleeptime);
 if (verbose)
 printf("\n-Processor clock rate = %.1f MHz\n", rate);
 return rate;
}

void desordenar(int *arr){
    srand(time(NULL));
    int i, a1, a2, aux;
    for(i = 0; i < N; i++){
        a1 = (rand() % N);
        a2 = (rand() % N);
        aux = arr[a1];
        arr[a1] = arr[a2];
        arr[a2] = aux;
    }
}
void inicializar_matriz(double **M){
    srand(time(NULL));
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            M[i][j]=0;//Valor random entre 0 y filas
        }
    }
}

void inicializar_matriz_oper1(double **M){
    srand(time(NULL));
    for(int i=0;i<N;i++){
        for(int j=0;j<8;j++){
            M[i][j]=2;//Valor random entre 0 y filas
        }
    }
}

void inicializar_matriz_oper2(double **M){
    srand(time(NULL));
    for(int i=0;i<8;i++){
        for(int j=0;j<N;j++){
            M[i][j]=3;//Valor random entre 0 y filas
        }
    }
}

void inicializar_array(double *arr){
    int valor;
    srand(time(NULL));
    for(int i=0;i<8;i++){
        arr[i]= 1;
    }
}

void inicializar_array_e(int *arr){
    int valor;
    srand(time(NULL));
    for(int i=0;i<N;i++){
        arr[i]= i;
    }
    //desordenar(arr);
}
int main(){
    double **a;
    double **b;
    double *c;
    double **d;
    double *e;
    int *ind;
    double f;
    double ck;
    //Reserva de memoria de las matrices y de los vectores
    
    a=(double **)malloc(N*sizeof(double*)); 	
	for (int i=0;i<N;i++){
		a[i] = (double*)malloc(8*sizeof(double));
    }

     
    b=(double **)malloc(8*sizeof(double*)); 
	for (int i=0;i<8;i++){
		b[i] = (double*)malloc(N*sizeof(double));
    }
    
    c=(double *)malloc(8*sizeof(double));

    d=(double **)malloc(N*sizeof(double*)); 
	for (int i=0;i<N;i++){
		d[i] = (double*)malloc(N*sizeof(double));
    }
    e=(double *)malloc(N*sizeof(double));
    ind=(int *)malloc(N*sizeof(int));
    //inicializamos todas las matrices y vectores
    inicializar_matriz(d);
    inicializar_matriz_oper2(b);
    inicializar_matriz_oper1(a);
    inicializar_array(c);
    inicializar_array_e(ind);
    start_counter();

    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            d[i][j] += 2 * a[i][0] * ( b[0][j] - c[0] );
            d[i][j] += 2 * a[i][1] * ( b[1][j] - c[1] );
            d[i][j] += 2 * a[i][2] * ( b[2][j] - c[2] );
            d[i][j] += 2 * a[i][3] * ( b[3][j] - c[3] );
            d[i][j] += 2 * a[i][4] * ( b[4][j] - c[4] );
            d[i][j] += 2 * a[i][5] * ( b[5][j] - c[5] );
            d[i][j] += 2 * a[i][6] * ( b[6][j] - c[6] );
            d[i][j] += 2 * a[i][7] * ( b[7][j] - c[7] );
        }
    }
    for (int i=0;i<N;i++){
        f+= d[ind[i]][ind[i]]/2;
    }
    ck=get_counter();

    printf("Resultado final:%lf",f);
    /* Esta rutina imprime a frecuencia de reloxo estimada coas rutinas start_counter/get_counter */
    printf("\n------------------------FRECUENCIA APROXIMADA DEL RELOJ DEL SISTEMA--------------------------------\n");
    mhz(1,1);
    printf("\nProgama ejecutado en: %lf\n",ck);
    printf("\n-----------------------------------------------------------------------------------------------------\n");

    //Liberamos la memoria de las matrices
    for(int i=0;i<N;i++){
            free(a[i]);//Cada fila va a tener reservado 64 bytes indiferentemente de cuantos valores tenga
        }
    free(a);  
    for(int i=0;i<8;i++){
            free(b[i]);//Cada fila va a tener reservado 64 bytes indiferentemente de cuantos valores tenga
        }
    free(b);
    free(c);
    free(ind);
    free(e);
    for(int i=0;i<N;i++){
            free(d[i]);//Cada fila va a tener reservado 64 bytes indiferentemente de cuantos valores tenga
        }
        free(d);
    return 0;
}