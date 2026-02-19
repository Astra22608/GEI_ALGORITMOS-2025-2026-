#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>

#define MAX_V 32000

double microsegundos() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

int ord_ins(int v[], int tamano) {
    int j, x, i;
    for (i = 1; i < tamano; i++) {
        x = v[i];
        j = i - 1;
        while (j >= 0 && v[j] > x) {
            v[j + 1] = v[j];
            j = j - 1;
        }
        v[j + 1] = x;
    }
    return 0;
}

int ord_shell (int v[], int tamano_v, int inc [], int tamano_inc) {
    // v es el vector a ordenar de tamaño n
    // inic es el vector de incremento de tamaño m (el ultimo debe ser 1)
    int j, x, k, h,  i;
    for (k = 0; k < tamano_inc; k++) {
        h = inc [k];
        // se realiza una ordenación por inserción con salto h
        for (i = h; i < tamano_v; i++) {
            x = v[i];
            j = i;
            while (j >= h && v[j-h] > x) {
                v[j] = v[j-h];
                j = j - h;
            }
            v[j] = x;
        }
    }
    return 0;
}

void inicializar_semilla() {
    srand(time(NULL));
    /* se establece la semilla de una nueva serie de enteros pseudo-aleatorios */
}

void aleatorio(int v [], int n) {
    int i, m = 2 * n + 1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
    /* se generan números pseudoaleatorio entre -n y +n */
}

void imprimirVector(int v[], int filas){
    int i;
    printf ("\n");
    for (i = 0; i < filas; i++){
        printf("%2d, ",v[i]);
    }
}

int comprobacion_ordenacion (int v[], int tamano){
    int i;
    for (i = 0; i < tamano - 1; i++){
       if (v[i] > v[i+1]){
        return 0;
       }
    }
    return 1;
}

void vueltavector(int v[],int size){
    int i, temp[size];
    for (i = 0; i < size; i++) {
        temp[size-i-1] = v[i];
    }
    for (i = 0; i < size; i++) {v[i] = temp[i];}
}

void hibbard (int inc[], int tamano){
    int i;
    for (i = 0; i < tamano; i++){
        inc[i] = (int) pow (2, i+1) -1;
    }
    vueltavector(inc,tamano);
}

void knuth (int inc[], int tamano){
    int i;
    inc[0] = 1;
    for (i = 1; i < tamano; i++){
        inc[i] = (int) ((pow (3,i+1)-1)/2);
    }
    vueltavector(inc,tamano);
}

void sedgewick (int inc[], int tamano){
    int i;
    inc[0] = 1;
    for (i = 1; i < tamano; i++){
        inc[i] = (int)(pow(4,i) + 3 * pow(2, i-1)+1);
    }
    vueltavector(inc,tamano);
}

void ciura (int inc[], int tamano){
    int ciura_base[] = {1, 4, 10, 23, 57, 132, 301, 701, 1750};
    int base_size = 9;
    int i;
    
    for (i = 0; i < base_size && i < tamano; i++) {
        inc[i] = ciura_base[i];
    }
    
    for (i = base_size; i < tamano; i++) {
        inc[i] = (int)(inc[i-1] * 2.25 + 0.5);  
    }
    vueltavector(inc,tamano);
}

void test_ins(){
    int v[17];
    printf ("\nTest Ordenación por Insercción\n");
    printf("Inicializacion aleatoria\n");
    aleatorio(v,17);
    imprimirVector(v,17);
    printf ("\nordenado? %d\n", comprobacion_ordenacion(v,17));
    printf("Ordenacion por Inserccion\n");
    ord_ins (v,17);
    imprimirVector(v,17);
    printf ("\nordenado? %d\n", comprobacion_ordenacion(v,17));

}

void inialeatoria(int v[]){
    printf ("\nInicialización Aleatoria");
    aleatorio(v,17);
    imprimirVector(v,17);
}

void test_incrementos(){
    int inc [15];
    printf("Comprobación de incrementos:\n");
    printf("Hibbard:");
    hibbard(inc,10);
    imprimirVector(inc,10);

    printf("\nKnuth:");
    knuth(inc,8);
    imprimirVector(inc,8);

    printf("\nSedgewick:");
    sedgewick(inc,7);
    imprimirVector(inc,7);

    printf("\nCiura:");
    ciura(inc,10);
    imprimirVector(inc,10);
    printf("\n");
}

void test_shell(){
    int v[17], inc [15];

     // Hibbard
    inialeatoria(v);
    printf("\nOrdenación shell con Hibbard");
    hibbard(inc, 10);  // Generar secuencia de Hibbard con 10 elementos
    ord_shell(v, 17, inc, 10);
    imprimirVector(v, 17);

    // Knuth
    inialeatoria(v);
    printf("\nOrdenación shell con Knuth");
    knuth(inc, 8);  // Generar secuencia de Knuth con 8 elementos
    ord_shell(v, 17, inc, 8);
    imprimirVector(v, 17);

    // Sedgewick
    inialeatoria(v);
    printf("\nOrdenación shell con Sedgewick");
    sedgewick(inc, 7); // Generar secuencia de Sedgewick con 7 elementos
    ord_shell(v, 17, inc, 7);
    imprimirVector(v, 17);

    // Ciura
    inialeatoria(v);
    printf("\nOrdenación shell con Ciura");
    ciura(inc, 10);  // Generar secuencia de Ciura con 10 elementos
    ord_shell(v, 17, inc, 10);
    imprimirVector(v, 17);
}

void tiemposejecucioninserciondescendete () { 
  double tiempoantes, tiempodespues, t_n1,t_n_18,t_n_2,t_n_22, t1, t2, t_total;
  int n, m ,k = 100, i, j ,v[MAX_V];
  
  for (m = 1; m <= 3; m++){
        printf("\nOrdenación por inserción con inicialización descendente\n\n");
        printf("%10s %16s %16s %16s %16s\n\n", "n",
            "t(n)", "t(n)/n^1.8","t(n)/n^2","t(n)/n^2.2");
    
        for (n=500; n<=32000;n*=2){
            for(i=0;i<n;i++){
                v[i] = n - i;
            }
  
            tiempoantes = microsegundos();
            ord_ins(v,n);
            tiempodespues = microsegundos();
            t_n1 = tiempodespues-tiempoantes;

            if (t_n1 < 500){
                tiempoantes = microsegundos();
                for (i = 0; i < k; i++){
                    for(j=0;j<n;j++){
                        v[j] = n - j;
                    }
                    ord_ins(v,n);
                }
                tiempodespues = microsegundos();
                t1 = tiempodespues - tiempoantes;

                tiempoantes = microsegundos();
                for (i = 0; i < k; i++){
                    for(j=0;j<n;j++){
                        v[j] = n - j;
                    }
                }
                tiempodespues = microsegundos();
                t2 = tiempodespues - tiempoantes;
                t_total = (t1-t2)/k;
                t_n1=t_total;
                printf("*");
            }
            else{
                printf(" ");
            }
            t_n_18 = t_n1/ (pow (n, 1.8));
            t_n_2 = t_n1/ pow(n,2);
            t_n_22 = t_n1/ pow(n,2.2);

            printf("%10d%16f%17f%17f%17f\n",n,t_n1,t_n_18,t_n_2,t_n_22);
        }
    }
}

void tiemposejecucioninsercionascendente () { 
    double tiempoantes, tiempodespues, t_n1,t_n_18,t_n_2,t_n_22, t1, t2, t_total;
    int n, m, k = 1000, i ,v[MAX_V];

    for (m = 1; m <= 3; m++){
        printf("\nOrdenación por inserción con inicialización ascendente\n\n");
        printf("%10s %16s %16s %16s %16s\n\n", "n",
            "t(n)", "t(n)/n^0.8","t(n)/n","t(n)/n*log(n)");
    
        for (n=500; n<=32000;n*=2){
            for(i=0;i<n;i++){
                v[i]=i;
            }
  
            tiempoantes = microsegundos();
            ord_ins(v,n);
            tiempodespues = microsegundos();
            t_n1 = tiempodespues-tiempoantes;

            if (t_n1 < 500){
                tiempoantes = microsegundos();
                for (i = 0; i < k; i++){
                    ord_ins(v,n);
                }
                tiempodespues = microsegundos();
                t1 = tiempodespues - tiempoantes;

                tiempoantes = microsegundos();
                for (i = 0; i < k; i++){}
                tiempodespues = microsegundos();
                t2 = tiempodespues - tiempoantes;
                t_total = (t1-t2)/k;
                t_n1=t_total;
                printf("*");
            }
            else{
                printf(" ");
            }
            t_n_18 = t_n1/ (pow (n, 0.8));
            t_n_2 = t_n1/ n;
            t_n_22 = t_n1/ (n*log(n));

            printf("%10d%16f%17f%17f%17f\n",n,t_n1,t_n_18,t_n_2,t_n_22);
        }
    }
}

void tiemposejecucioninserciondesordenada(){ 
    double tiempoantes, tiempodespues, t_n1,t_n_18,t_n_2,t_n_22, t1, t2, t_total;
    int n, m ,k = 1000, i ,v[MAX_V];


    for (m = 1; m <= 3; m++){
        printf("\nOrdenación por inserción con inicialización desordenada\n\n");
        printf("%10s %16s %16s %16s %16s\n\n", "n",
            "t(n)", "t(n)/n^1.8","t(n)/n^2","t(n)/n^2.1");
    
        for (n=500; n<=32000;n*=2){
            aleatorio(v,n);

            tiempoantes = microsegundos();
            ord_ins(v,n);
            tiempodespues = microsegundos();
            t_n1 = tiempodespues-tiempoantes;

            if (t_n1 < 500){
                tiempoantes = microsegundos();
                for (i = 0; i < k; i++){
                    aleatorio(v,n);
                    ord_ins(v,n);
                }
                tiempodespues = microsegundos();
                t1 = tiempodespues - tiempoantes;

                tiempoantes = microsegundos();
                for (i = 0; i < k; i++){
                  aleatorio(v,n);
                }
                tiempodespues = microsegundos();
                t2 = tiempodespues - tiempoantes;
                t_total = (t1-t2)/k;
                t_n1=t_total;
                printf("*");
            }
            else{
                printf(" ");
            }
            t_n_18 = t_n1/ (pow (n, 1.8));
            t_n_2 = t_n1/ pow(n,2);
            t_n_22 = t_n1/ pow(n,2.1);

            printf("%10d%16f%17f%17f%17f\n",n,t_n1,t_n_18,t_n_2,t_n_22);
        }
    }
}

void tiemposejecucionshellHibbard () { 
    double tiempoantes, tiempodespues, t_n1,t_n_18,t_n_2,t_n_22, t1, t2, t_total;
    int n, m ,k = 1000, i ,v[MAX_V], inc[15];
    //int Hibbard[10]={1,3,7,15,31,63,127,255,511,1023,1};

    for (m =1; m <= 3; m++){
        printf("\nOrdenación por shell con Hibbard e inicialización desordenada\n\n");
          printf("%10s %16s %16s %16s %16s\n\n", "n",
            "t(n)", "t(n)/n^0.98","t(n)/n^1.18","t(n)/n^1.28");

        for (n=500; n<=32000;n*=2){
            hibbard(inc,10);
            aleatorio(v,n);

            tiempoantes = microsegundos();
            ord_shell(v,n,inc,10);
            tiempodespues = microsegundos();
            t_n1 = tiempodespues-tiempoantes;

            if (t_n1 < 500){
                tiempoantes = microsegundos();
                for (i = 0; i < k; i++){
                    aleatorio(v,n);
                    ord_shell(v,n,inc,10);
                }
                tiempodespues = microsegundos();
                t1 = tiempodespues - tiempoantes;

                tiempoantes = microsegundos();
                for (i = 0; i < k; i++){
                  aleatorio(v,n);
                }
                tiempodespues = microsegundos();
                t2 = tiempodespues - tiempoantes;
                t_total = (t1-t2)/k;
                t_n1=t_total;
                printf("*");
            }
            else{
                printf(" ");
            }
            t_n_18 = t_n1/ (pow (n, 0.98));
            t_n_2 = t_n1/ (pow (n,1.18));
            t_n_22 = t_n1/ (pow (n, 1.28));
            printf("%10d%16f%17f%17f%17f\n",n,t_n1,t_n_18,t_n_2,t_n_22);
        }
    }
}

void tiempoejecucionshellKnuth(){
    double tiempoantes, tiempodespues, t_n1,t_n_18,t_n_2,t_n_22, t1, t2, t_total;
    int n, m ,k = 1000, i ,v[MAX_V], inc[15];

    //int Knuth[8]={1,4,13,40,121,364,1093,3280,1};
    for (m =1; m <= 3; m++){
        printf("\nOrdenación por shell con Knuth e inicialización desordenada\n\n");
          printf("%10s %16s %16s %16s %16s\n\n", "n",
            "t(n)", "t(n)/n^1.05","t(n)/n^1.15","t(n)/n^1.25");

        for (n=500; n<=32000;n*=2){
            knuth (inc, 8);
            aleatorio(v,n);
            tiempoantes = microsegundos();
            ord_shell(v,n,inc,8);
            tiempodespues = microsegundos();
            t_n1 = tiempodespues-tiempoantes;

            if (t_n1 < 500){
                tiempoantes = microsegundos();
                for (i = 0; i < k; i++){
                    aleatorio(v,n);
                    ord_shell(v,n,inc,8);
                }
                tiempodespues = microsegundos();
                t1 = tiempodespues - tiempoantes;

                tiempoantes = microsegundos();
                for (i = 0; i < k; i++){
                  aleatorio(v,n);
                }
                tiempodespues = microsegundos();
                t2 = tiempodespues - tiempoantes;
                t_total = (t1-t2)/k;
                t_n1=t_total;
                printf("*");
            }
            else{
                printf(" ");
            }
            t_n_18 = t_n1/ (pow (n, 1.05));
            t_n_2 = t_n1/ (pow (n,1.15));
            t_n_22 = t_n1/ (pow (n, 1.25));
            printf("%10d%16f%17f%17f%17f\n",n,t_n1,t_n_18,t_n_2,t_n_22);
        }
    }
}

void tiempoejecucionshellSedgewick(){
    double tiempoantes, tiempodespues, t_n1,t_n_18,t_n_2,t_n_22, t1, t2, t_total;
    int n, m,k = 1000, i ,v[MAX_V], inc[15];
    //int Sedgewick[7]={1,8,23,77,281,1073,4193,1};

    for (m =1; m <= 3; m++){
        printf("\nOrdenación por shell con Sedgewick e inicialización desordenada\n\n");
          printf("%10s %16s %16s %16s %16s\n\n", "n",
            "t(n)", "t(n)/n^1.05","t(n)/n^1.15","t(n)/n^1.25");

        for (n=500; n<=32000;n*=2){
            sedgewick (inc, 7);
            aleatorio(v,n);
            tiempoantes = microsegundos();
            ord_shell(v,n,inc,7);
            tiempodespues = microsegundos();
            t_n1 = tiempodespues-tiempoantes;

            if (t_n1 < 500){
                tiempoantes = microsegundos();
                for (i = 0; i < k; i++){
                    aleatorio(v,n);
                    ord_shell(v,n,inc,7);
                }
                tiempodespues = microsegundos();
                t1 = tiempodespues - tiempoantes;

                tiempoantes = microsegundos();
                for (i = 0; i < k; i++){
                  aleatorio(v,n);
                }
                tiempodespues = microsegundos();
                t2 = tiempodespues - tiempoantes;
                t_total = (t1-t2)/k;
                t_n1=t_total;
                printf("*");
            }
            else{
                printf(" ");
            }
            t_n_18 = t_n1/ (pow (n, 1.05));
            t_n_2 = t_n1/ (pow (n,1.15));
            t_n_22 = t_n1/ (pow (n, 1.25));
            printf("%10d%16f%17f%17f%17f\n",n,t_n1,t_n_18,t_n_2,t_n_22);
        }
    }
}

void tiempoejecucionshellCiura(){
    double tiempoantes, tiempodespues, t_n1,t_n_18,t_n_2,t_n_22, t1, t2, t_total;
    int n,m,k = 1000 , i ,v[MAX_V], inc[15];
    
    //int Ciura[10]={1,4,10,23,57,132,301,701,1750,4170,1};
    for (m =1; m <= 3; m++){
        printf("\nOrdenación por shell con Ciura e inicialización desordenada\n\n");
          printf("%10s %16s %16s %16s %16s\n\n", "n",
            "t(n)", "t(n)/n^1.03","t(n)/n^1.13","t(n)/n^1.23");

        for (n=500; n<=32000;n*=2){
            ciura (inc, 10);
            aleatorio(v,n);
            tiempoantes = microsegundos();
            ord_shell(v,n,inc,10);
            tiempodespues = microsegundos();
            t_n1 = tiempodespues-tiempoantes;

            if (t_n1 < 500){
                tiempoantes = microsegundos();
                for (i = 0; i < k; i++){
                    aleatorio(v,n);
                    ord_shell(v,n,inc,10);
                }
                tiempodespues = microsegundos();
                t1 = tiempodespues - tiempoantes;

                tiempoantes = microsegundos();
                for (i = 0; i < k; i++){
                    aleatorio(v,n);
                }
                tiempodespues = microsegundos();
                t2 = tiempodespues - tiempoantes;
                t_total = (t1-t2)/k;
                t_n1=t_total;
                printf("*");
            }
            else{
                printf(" ");
            }
            t_n_18 = t_n1/ (pow (n, 1.03));
            t_n_2 = t_n1/ (pow (n,1.13));
            t_n_22 = t_n1/ (pow (n, 1.23));
            printf("%10d%16f%17f%17f%17f\n",n,t_n1,t_n_18,t_n_2,t_n_22);
        }
    }
}

int main(void) {
    inicializar_semilla();
    printf("Test Shell\n");
    test_incrementos();
    test_shell();
    printf("\n");
    test_ins();
    tiemposejecucioninserciondescendete ();
    tiemposejecucioninsercionascendente ();
    tiemposejecucioninserciondesordenada ();
    tiemposejecucionshellHibbard();
    tiempoejecucionshellKnuth();
    tiempoejecucionshellSedgewick();
    tiempoejecucionshellCiura();
    return 0;
}
