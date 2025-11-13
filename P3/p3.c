#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>

#define MAX_V 512000

double microsegundos() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
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

void insercionSort (int v[], int tamano) {
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
}

void intercambiar (int v[], int i, int j){
    int temp;
    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

void median3 (int v[], int i, int j){
    // PRECONDICION = i < j
    int k;
    k = (i+j)/2;
    if (v[k] > v[j]){
        intercambiar (v, k, j);
    }
    if (v[k] > v[i]){
        intercambiar (v, k, i);
    }
    if (v[i] > v[j]){
        intercambiar (v, i, j);
    }
}

void sortAux (int v[], int izquierda, int derecha, int umbral){
    /*
    * UMBRAL >= 1.
    * el pivote está en ’izq’ y en ’der’ habrá
    * un valor mayor o igual que el pivote.
    */
    int pivote, i, j;
    
    if ((izquierda + umbral) <= derecha){
        median3(v,izquierda, derecha);
        pivote = v[izquierda];
        i = izquierda;
        j = derecha;
        do {
            do {
                i++;
            } while (v[i] < pivote);
            
            do {
                j--;
            } while (v[j] > pivote);
            
            if(i<j){intercambiar (v, i, j);
            }
        } while (j > i);
        intercambiar (v, izquierda, j);
        sortAux (v, izquierda, j - 1, umbral);
        sortAux(v, j + 1, derecha, umbral);
    }
}

void quicksort (int v[], int n, int umbral){
    sortAux(v, 0, n-1, umbral);
    if (umbral > 1){
        insercionSort (v, n);
    }
}

//Funciones auxiliares
void inicializar_ascendente(int v[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        v[i] = i;
    }
}

void inicializar_descendente(int v[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        v[i] = n - i;
    }
}

void imprimirVector(int v[], int filas){
    int i;
    printf ("\n");
    for (i = 0; i < filas; i++){
        printf("%2d, ",v[i]);
    }
    printf("\n");
}

int esta_ordenado(int v[], int n) {
    int i;
    for (i = 0; i < n - 1; i++) {
        if (v[i] > v[i + 1]) {
            return 0;
        }
    }
    return 1;
}

void test_quicksortaleatorio(){
    int v[17];
    aleatorio(v,17);
    printf("Test Quicksort:\n\nInicialización aleatoria:\n");
    imprimirVector(v,17);
    printf ("\nordenado? %d\n", esta_ordenado(v,17));
    printf("\nComprobación de la Ordenacion Rápida con umbral 1:\n");
    quicksort(v,17,1);//funcionamiento con umbral=1
    imprimirVector(v,17);
    printf ("\nordenado? %d\n", esta_ordenado(v,17));
}

void test_quicksortdescendente(){
    int v[17];
    inicializar_descendente(v,17);
    printf("\n\nInicialización descendente:\n");
    imprimirVector(v,17);
    printf ("\nordenado? %d\n", esta_ordenado(v,17));
    printf("\nComprobación de la Ordenacion Rápida con umbral 1:\n");
    quicksort(v,17,1);//funcionamiento con umbral=1
    imprimirVector(v,17);
    printf ("\nordenado? %d\n", esta_ordenado(v,17));
}

void test_quicksortascendente(){
    int v[17];
    inicializar_ascendente(v,17);
    printf("\n\nInicialización ascendente:\n");
    imprimirVector(v,17);
    printf ("\nordenado? %d\n", esta_ordenado(v,17));
    printf("\nComprobación de la Ordenacion Rápida con umbral 1:\n");
    quicksort(v,17,1);//funcionamiento con umbral=1
    imprimirVector(v,17);
    printf ("\nordenado? %d\n", esta_ordenado(v,17));
}

int calcular_tiempo(int v[], int n, int umbral){
    double tiempoantes, tiempodespues;
    tiempoantes = microsegundos();
    quicksort(v,n,umbral);
    tiempodespues = microsegundos();
    return tiempodespues-tiempoantes;
}

void menores_desordenada(int v[], int n, int umbral, double t_n1){
    double tiempoantes, tiempodespues, t1, t2, t_total;
    int i, k = 1000;;
    if (t_n1 < 1000){
        aleatorio(v,n);
        tiempoantes = microsegundos();
        for (i = 0; i < k; i++){
            aleatorio(v,n);
            quicksort(v,n,umbral);
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
}

void menores_ascendente(int v[], int n, int umbral, double t_n1){
    double tiempoantes, tiempodespues, t1, t2, t_total;
    int i, k = 1000;;
    if (t_n1 < 1000){
        tiempoantes = microsegundos();
        for (i = 0; i < k; i++){
            quicksort(v,n,umbral);
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
}

void menores_decendente(int v[], int n, int umbral, double t_n1){
    double tiempoantes, tiempodespues, t1, t2, t_total;
    int i, k = 1000;;
    if (t_n1 < 1000){
        tiempoantes = microsegundos();
        for (i = 0; i < k; i++){
            inicializar_descendente(v,n);
            quicksort(v,n,umbral);
        }
        tiempodespues = microsegundos();
        t1 = tiempodespues - tiempoantes;

        tiempoantes = microsegundos();
        for (i = 0; i < k; i++){
            inicializar_descendente(v,n);
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
}

void tiemposejecuciondesordenada(int umbral){
    double t_n1,t_sub,t_medio,t_sobre;
    int n,m,v[MAX_V];
    for (m = 1; m <= 3; m++){
        printf("\nOrdenación rápida con inicialización desordenada y umbral %d:\n\n",umbral);
        if(umbral==1){
            printf("%10s %16s %16s %16s %16s\n\n", 
                "n","t(n)", "t(n)/(n*log(n))^0.89","t(n)/(n*log(n))","t(n)/(n*log(n))^1.06");
        }
        else if(umbral==10){
            printf("%10s %16s %16s %16s %16s\n\n", 
                "n","t(n)", "t(n)/(n*log(n))^0.95","t(n)/(n*log(n))^1.005","(n*log(n))^1.05");
        }
        else if(umbral==100){
            printf("%10s %16s %16s %16s %16s\n\n", 
                "n","t(n)", "t(n)/(n*log(n))^0.86","t(n)/(n*log(n))^0.985","t(n)/(n*log(n))^1.3");
        }
        for (n=500; n<=512000;n*=2){
            aleatorio(v,n);
            t_n1 = calcular_tiempo(v,n,umbral);
            menores_desordenada(v,n,umbral,t_n1);
            if(umbral==1){
                t_sub = t_n1/ pow(n*log(n),0.89);    
                t_medio = t_n1/ (n*log(n));
                t_sobre = t_n1/ pow(n*log(n),1.06);
            }
            else if(umbral==10){
                t_sub = t_n1/ pow(n*log(n),0.95);    
                t_medio = t_n1/ pow(n*log(n),1.005);
                t_sobre = t_n1/ pow(n*log(n),1.05);
            }
            if(umbral==100){
                t_sub = t_n1/ pow(n*log(n),0.86);    
                t_medio = t_n1/ pow(n*log(n),0.985);
                t_sobre = t_n1/ pow(n*log(n),1.3);
            }
            printf("%10d%16f%17f%17f%17f\n",n,t_n1,t_sub,t_medio,t_sobre);
        }
    }
}

void tiemposejecucionoascendente(int umbral){
    double t_n1,t_sub,t_medio,t_sobre;
    int n,m,v[MAX_V];
    for (m = 1; m <= 3; m++){
        printf("\nOrdenación rápida con inicialización ascendente y umbral %d:\n\n",umbral);
        if(umbral==1){
            printf("%10s %16s %16s %16s %16s\n\n", 
                "n","t(n)", "t(n)/(n*log(n))^0.87","t(n)/(n*log(n))^0.925","t(n)/(n*log(n))^1.03");
       }
        else if(umbral==10){
            printf("%10s %16s %16s %16s %16s\n\n", 
                "n","t(n)", "t(n)/(n*log(n))^0.885","t(n)/(n*log(n))^0.965","t(n)/(n*log(n))^1.056");
        }
        else if(umbral==100){
            printf("%10s %16s %16s %16s %16s\n\n", 
                "n","t(n)", "t(n)/(n*log(n))^0.91","t(n)/(n*log(n))^0.971","t(n)/(n*log(n))^1.09");
        }
        for (n=500; n<=512000;n*=2){
            inicializar_ascendente(v,n);
            t_n1 = calcular_tiempo(v,n,umbral);
            menores_ascendente(v,n,umbral,t_n1);
            if(umbral==1){
                t_sub = t_n1/ pow(n*log(n),0.87);
                t_medio = t_n1/ pow(n*log(n),0.925);
                t_sobre = t_n1/ pow(n*log(n),1.03);
            }
            else if(umbral==10){
                t_sub = t_n1/ pow(n*log(n),0.885);
                t_medio = t_n1/ pow(n*log(n),0.965);
                t_sobre = t_n1/ pow(n*log(n),1.056);
            }
            if(umbral==100){
                t_sub = t_n1/ pow(n*log(n),0.91);
                t_medio = t_n1/ pow(n*log(n),0.971);
                t_sobre = t_n1/ pow(n*log(n),1.09);
            }
            printf("%10d%16f%17f%17f%17f\n",n,t_n1,t_sub,t_medio,t_sobre);
        }
    }
}

void tiemposejecuciondescendente(int umbral){
    double t_n1,t_sub,t_medio,t_sobre;
    int n,m,v[MAX_V];
    for (m = 1; m <= 3; m++){
        printf("\nOrdenación rápida con inicialización descendente y umbral %d:\n\n",umbral);
        if(umbral==1){
            printf("%10s %16s %16s %16s %16s\n\n", 
                "n","t(n)", "t(n)/(n*log(n))^0.87","t(n)/(n*log(n))^0.945","t(n)/(n*log(n))^1.055");
        }
        else if(umbral==10){
            printf("%10s %16s %16s %16s %16s\n\n", 
                "n","t(n)", "t(n)/(n*log(n))^0.89","t(n)/(n*log(n))^0.973","t(n)/(n*log(n))^1.076");
        }
        else if(umbral==100){
            printf("%10s %16s %16s %16s %16s\n\n", 
                "n","t(n)", "t(n)/(n*log(n))^0.94","t(n)/(n*log(n))^0.995","t(n)/(n*log(n))^1.09");
        }
        for (n=500; n<=512000;n*=2){
            inicializar_descendente(v,n);
            t_n1 = calcular_tiempo(v,n,umbral);
            menores_decendente(v,n,umbral,t_n1);
            if(umbral==1){
                t_sub = t_n1/ pow(n*log(n),0.87);
                t_medio = t_n1/ pow(n*log(n),0.945);
                t_sobre = t_n1/ pow(n*log(n),1.055);
            }
            else if(umbral==10){
                t_sub = t_n1/ pow(n*log(n),0.89);
                t_medio = t_n1/ pow(n*log(n),0.973);
                t_sobre = t_n1/ pow(n*log(n),1.076);
            }
            if(umbral==100){
                t_sub = t_n1/ pow(n*log(n),0.94);
                t_medio = t_n1/ pow(n*log(n),0.995);
                t_sobre = t_n1/ pow(n*log(n),1.09);
            }
            printf("%10d%16f%17f%17f%17f\n",n,t_n1,t_sub,t_medio,t_sobre);
        }
    }
}

int main(void) {
    inicializar_semilla();

    test_quicksortaleatorio();
    test_quicksortascendente();
    test_quicksortdescendente();

    tiemposejecuciondesordenada(1);
    tiemposejecuciondesordenada(10);
    tiemposejecuciondesordenada(100);

    tiemposejecucionoascendente(1);
    tiemposejecucionoascendente(10);
    tiemposejecucionoascendente(100);
    
    tiemposejecuciondescendente(1);
    tiemposejecuciondescendente(10);
    tiemposejecuciondescendente(100);
    
    return 0;
}