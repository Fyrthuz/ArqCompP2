#include <stdio.h>
#include <stdlib.h>
#include <pmmintrin.h>
#include <time.h>
#include <unistd.h>
#include <immintrin.h>

#define N 512
#define bsize 8
#define ALIGN __attribute__ ((aligned (32)))

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
void inicializar_matriz(__m128d **M){
    srand(time(NULL));
    double ALIGN x=0.0;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            M[i][j]=_mm_loaddup_pd(&x);
        }
    }
}

void inicializar_matriz_oper1(__m128d **M){
    srand(time(NULL));
    double ALIGN x=2.0;

    for(int i=0;i<N;i++){
        for(int j=0;j<8;j++){
            M[i][j]=_mm_loaddup_pd(&x);//Valor random entre 0 y filas
        }
    }
}

void inicializar_matriz_oper2(__m128d **M){
    srand(time(NULL));
    double ALIGN x=3.0;
    for(int i=0;i<8;i++){
        for(int j=0;j<N;j++){
            M[i][j]=_mm_loaddup_pd(&x);//Valor random entre 0 y filas
        }
    }
}

void inicializar_array(__m128d *arr){
    int valor;
    srand(time(NULL));
    double ALIGN x=1.0;
    for(int i=0;i<8;i++){
        arr[i]=_mm_loaddup_pd(&x);
    }
}

void inicializar_array_e(int *arr){
    int valor;
    srand(time(NULL));
    for(int i=0;i<N;i++){
        arr[i]= i;
    }
    desordenar(arr);
}
int main(){
    __m128d **a;
    __m128d **b;
    __m128d *c;
    __m128d **d;
    __m128d *e;
    __m128d aux,aux2,aux3,aux4;
    int *ind;
    double ALIGN h=2.0;
    double __attribute__ ((aligned (32)))f,rest=0.0;
    __attribute__ ((aligned (32)))double resul[2];
    __attribute__ ((aligned (32))) double ck;

    a=_mm_malloc(N*sizeof(__m128d *),32);
    for(int i=0;i<N;i++){
        a[i]=_mm_malloc(8*sizeof(__m256),32);
    }

    b=_mm_malloc(8*sizeof(__m128d*),32);
    for(int i=0;i<8;i++){
        b[i]=_mm_malloc(N*sizeof(__m128d),32);
    }

    c=_mm_malloc(8*sizeof(__m128d),32);

    d=_mm_malloc(N*sizeof(__m128d*),32);
    for(int i=0;i<N;i++){
        d[i]=_mm_malloc(N*sizeof(__m128d),32);
    }

    e=_mm_malloc(N*sizeof(__m128d),32);
    ind=(int *)malloc(N*sizeof(int));

    inicializar_matriz(d);
    inicializar_matriz_oper1(a);
    inicializar_matriz_oper2(b);
    inicializar_array(c);
    inicializar_array_e(ind);

    start_counter();
    __m128d p=_mm_loaddup_pd(&h);
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            
            aux=_mm_sub_pd(b[0][j],c[0]);
            aux=_mm_mul_pd(a[i][0],aux);
            aux=_mm_mul_pd(p,aux);

            aux2=_mm_sub_pd(b[2][j],c[2]);
            aux2=_mm_mul_pd(a[i][2],aux2);
            aux2=_mm_mul_pd(p,aux2);

            aux3=_mm_sub_pd(b[4][j],c[4]);
            aux3=_mm_mul_pd(a[i][4],aux3);
            aux3=_mm_mul_pd(p,aux3);

            aux4=_mm_sub_pd(b[6][j],c[6]);
            aux4=_mm_mul_pd(a[i][6],aux4);
            aux4=_mm_mul_pd(p,aux4);
            
            
            aux=_mm_add_pd(aux,aux2);//Sumamos los vetores
            aux3=_mm_add_pd(aux3,aux4);

            aux=_mm_add_pd(aux,aux3);

            _mm_store_pd(resul,aux);

            for(int z=0;z<2;z++){
                rest+=resul[z];
            }
            //CARGAR EN D RESULT
            d[i][j]=_mm_load_pd(&rest);
            //printf("%lf\n",rest);
            rest=0.0;
        }
    }
    for(int i=0;i<N;i++){
         __attribute__ ((aligned (32)))double *k;
        e[i]=_mm_div_pd(d[ind[i]][ind[i]],p);
        k=(double *)&e[i];
        f+=*k;
    }
    ck=get_counter();
    printf("\nResultado final %lf\n",f);
    printf("\n------------------------FRECUENCIA APROXIMADA DEL RELOJ DEL SISTEMA--------------------------------\n");
    mhz(1,1);
    printf("\nProgama ejecutado en: %lf\n",ck);
    printf("\n-----------------------------------------------------------------------------------------------------\n");

    for(int i=0;i<N;i++){
        _mm_free(a[i]);
    }
    _mm_free(a);
    for(int i=0;i<8;i++){
        _mm_free(b[i]);
    }
    _mm_free(b);

    _mm_free(c);
    free(ind);
    _mm_free(e);
    for(int i=0;i<N;i++){
            _mm_free(d[i]);//Cada fila va a tener reservado 64 bytes indiferentemente de cuantos valores tenga
        }
        _mm_free(d);
    return 0;
}


