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
    int j, x, k, h, i;
    for (k = 0; k < tamano_inc; k++) {
        h = inc[k];
        for (i = h; i < tamano_v; i++) {
            x = v[i];
            j = i;
            while (j >= h && v[j - h] > x) {
                v[j] = v[j - h];
                j = j - h;
            }
            v[j] = x;
        }
    }
    return 0;
}

void inicializar_semilla() {
    srand(time(NULL));
}

void aleatorio(int v [], int n) {
    int i, m = 2 * n + 1;
    for (i = 0; i < n; i++)
        v[i] = (rand() % m) - n;
}

void inicializar_ascendente(int v[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        v[i] = i;
    }
}

void inicializar_descendente(int v[], int n) {
    int i;
    /* Genera n-1, n-2, ..., 0 (evita asignar v[0]=n) */
    for (i = 0; i < n; i++) {
        v[i] = n - i - 1;
    }
}

void imprimirVector(int v[], int filas){
    int i;
    printf ("\n");
    for (i = 0; i < filas; i++){
        printf("%2d, ", v[i]);
    }
    printf("\n");
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

void copiarVector (int original [], int final[], int n){
    int i;
    for (i = 0; i < n; i++){
        final[i] = original[i];
    }
}

void invertir (int inc[], int tamano){
    int i, temp;
    for (i = 0; i < tamano / 2; i++){
        temp = inc[i];
        inc[i] = inc[tamano - i - 1];
        inc[tamano - i - 1] = temp;
    }
}

void hibbard (int inc[], int tamano){
    int i;
    for (i = 0; i < tamano; i++){
        inc[i] = (int) (pow(2, i+1) - 1);
    }
}

void knuth (int inc[], int tamano){
    int i;
    for (i = 0; i < tamano; i++){
        if (i == 0) inc[i] = 1;
        else inc[i] = (int) ((pow(3, i+1) - 1) / 2);
    }
}

void sedgewick (int inc[], int tamano){
    int i;
    for (i = 0; i < tamano; i++){
        if (i == 0) inc[i] = 1;
        else inc[i] = (int)(pow(4, i) + 3 * pow(2, i-1) + 1);
    }
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
}


void comprobar_hibbard(int inc[], int tamano){
    int i,incremento_esperado;
    for (i = 0; i < tamano; i++){  
        incremento_esperado = (int)(pow(2, i+1) - 1);
        if(inc[i] != incremento_esperado){
            printf ("Incremento inesperado\n");
            inc[i] = incremento_esperado; 
        }
    }
    printf("Incrementos de Hibbard:\n");
    imprimirVector(inc, tamano);
}

void comprobar_knuth(int inc[], int tamano){
    int i,incremento_esperado;
    for (i = 0; i < tamano; i++){  
        incremento_esperado = (int) ((pow(3, i+1) - 1) / 2);
        if(inc[i] != incremento_esperado){
            printf ("Incremento inesperado\n");
            inc[i] = incremento_esperado; 

        }
    }
    printf("Incrementos de Knuth:\n");
    imprimirVector(inc, tamano);
}

void comprobar_sedgewick(int inc[], int tamano) {
    int i, incremento_esperado;
    for (i = 0; i < tamano; i++) {
        incremento_esperado = (i == 0) ? 1 : (int)(pow(4, i) + 3 * pow(2, i-1) + 1);
        if (inc[i] != incremento_esperado) {
            printf ("Incremento inesperado\n");
            inc[i] = incremento_esperado;
        }
    }
    printf("Incrementos de Sedgewcik:\n");
    imprimirVector(inc, tamano);
}

void comprobar_ciura(int inc[], int tamano) {
    int ciura_base[] = {1, 4, 10, 23, 57, 132, 301, 701, 1750};
    int i , incremento_esperado, base_size = 9;
    for (i = 0; i < tamano; i++) {
        if (i < base_size)
            incremento_esperado = ciura_base[i];
        else
            incremento_esperado = (int)(inc[i-1] * 2.25 + 0.5);
        if (inc[i] != incremento_esperado) {
            printf ("Incremento inesperado\n");            
            inc[i] = incremento_esperado;
        }
    }
    printf("Incrementos de Ciura:\n");
    imprimirVector(inc, tamano);
}


void test_ins(){
    int v[17];
    printf ("\nTest Ordenación por Inserción\n");
    printf("Inicializacion aleatoria\n");
    aleatorio(v,17);
    imprimirVector(v,17);
    printf ("\nordenado? %d\n", comprobacion_ordenacion(v,17));
    printf("Ordenacion por Inserccion\n");
    ord_ins (v,17);
    imprimirVector(v,17);
    printf ("\nordenado? %d\n", comprobacion_ordenacion(v,17));
}

void test_shell_hibbard(){
    int v[17], vcopia[17], inc[15];
    printf("\nTest Shell\nInicialización aleatoria\n");
    aleatorio(v,17);
    imprimirVector(v,17);

    // Hibbard
    printf("\nOrdenación shell con Hibbard\n");
    hibbard(inc, 10);
    comprobar_hibbard(inc, 10);
    invertir(inc, 10);
    copiarVector(v, vcopia, 17);
    ord_shell(vcopia, 17, inc, 10);
    printf("Vector ordenado con Hibbard:\n");
    imprimirVector(vcopia, 17);
}

void test_shell_knuth(){
    int v[17], vcopia[17], inc[15];
    printf("\nTest Shell\nInicialización aleatoria\n");
    aleatorio(v,17);
    imprimirVector(v,17);

    // Knuth
    printf("\nOrdenación shell con Knuth\n");
    knuth(inc, 8);
    comprobar_knuth(inc, 8);
    invertir(inc, 8);
    copiarVector(v, vcopia, 17);
    ord_shell(vcopia, 17, inc, 8);
    printf("Vector ordenado con Knuth:\n");
    imprimirVector(vcopia, 17);
}

void test_shell_sedgewick(){
    int v[17], vcopia[17], inc[15];
    printf("\nTest Shell\nInicialización aleatoria\n");
    aleatorio(v,17);
    imprimirVector(v,17);

    // Sedgewick
    printf("\nOrdenación shell con Sedgewick\n");
    sedgewick(inc, 7);
    comprobar_sedgewick(inc, 7);
    invertir(inc, 7);
    copiarVector(v, vcopia, 17);
    ord_shell(vcopia, 17, inc, 7);
    printf("Vector ordenado con Sedgewick:\n");
    imprimirVector(vcopia, 17);
}

void test_shell_ciura(){
    int v[17], vcopia[17], inc[15];
    printf("\nTest Shell\nInicialización aleatoria\n");
    aleatorio(v,17);
    imprimirVector(v,17);

    // Ciura
    printf("\nOrdenación shell con Ciura\n");
    ciura(inc, 10);
    comprobar_ciura(inc, 10);
    invertir(inc, 10);
    copiarVector(v, vcopia, 17);
    ord_shell(vcopia, 17, inc, 10);
    printf("Vector ordenado con Ciura:\n");
    imprimirVector(vcopia, 17);
}

void tiemposejecucioninserciondescendete () {
    double tiempoantes, tiempodespues, t_n1, t_n_18, t_n_2, t_n_22, t1, t2, t_total;
    int n, m, k = 1000, i, v[MAX_V];

    for (m = 1; m <= 3; m++){
        printf("\nOrdenación por inserción con inicialización descendente\n\n");
        printf("%10s %16s %16s %16s %16s\n\n", "n",
            "t(n)", "t(n)/n^1.8","t(n)/n^2","t(n)/n^2.2");

        for (n = 500; n <= 32000; n *= 2) {
            inicializar_descendente(v, n);

            tiempoantes = microsegundos();
            ord_ins(v, n);
            tiempodespues = microsegundos();
            t_n1 = tiempodespues - tiempoantes;

            if (t_n1 < 500) {
                tiempoantes = microsegundos();
                for (i = 0; i < k; i++){
                    inicializar_descendente(v, n);
                    ord_ins(v, n);
                }
                tiempodespues = microsegundos();
                t1 = tiempodespues - tiempoantes;

                tiempoantes = microsegundos();
                for (i = 0; i < k; i++) {
                    inicializar_descendente(v, n);
                }
                tiempodespues = microsegundos();
                t2 = tiempodespues - tiempoantes;

                t_total = (t1 - t2) / k;
                t_n1 = t_total;
                printf("*");
            } else {
                printf(" ");
            }

            t_n_18 = t_n1 / pow(n, 1.8);
            t_n_2 = t_n1 / pow(n, 2);
            t_n_22 = t_n1 / pow(n, 2.2);

            printf("%10d%16f%17f%17f%17f\n", n, t_n1, t_n_18, t_n_2, t_n_22);
        }
    }
}

void tiemposejecucioninsercionascendente () {
    double tiempoantes, tiempodespues, t_n1, t_n_18, t_n_2, t_n_22, t1, t2, t_total;
    int n, m, k = 1000, i, v[MAX_V];

    for (m = 1; m <= 3; m++){
        printf("\nOrdenación por inserción con inicialización ascendente\n\n");
        printf("%10s %16s %16s %16s %16s\n\n", "n",
            "t(n)", "t(n)/n^0.8","t(n)/n","t(n)/n*log(n)");

        for (n = 500; n <= 32000; n *= 2){

            tiempoantes = microsegundos();
            ord_ins(v, n);
            tiempodespues = microsegundos();
            t_n1 = tiempodespues - tiempoantes;

            if (t_n1 < 500){
                tiempoantes = microsegundos();
                for (i = 0; i < k; i++){
                    ord_ins(v, n);
                }
                tiempodespues = microsegundos();
                t1 = tiempodespues - tiempoantes;

                tiempoantes = microsegundos();
                for (i = 0; i < k; i++){}
                tiempodespues = microsegundos();
                t2 = tiempodespues - tiempoantes;
                t_total = (t1 - t2) / k;
                t_n1 = t_total;
                printf("*");
            } else {
                printf(" ");
            }

            t_n_18 = t_n1 / pow(n, 0.8);
            t_n_2 = t_n1 / n;
            t_n_22 = t_n1 / (n * log(n));

            printf("%10d%16f%17f%17f%17f\n", n, t_n1, t_n_18, t_n_2, t_n_22);
        }
    }
}

void tiemposejecucioninserciondesordenada(){
    double tiempoantes, tiempodespues, t_n1, t_n_18, t_n_2, t_n_22, t1, t2, t_total;
    int n, m, k = 1000, i, v[MAX_V];

    for (m = 1; m <= 3; m++){
        printf("\nOrdenación por inserción con inicialización desordenada\n\n");
        printf("%10s %16s %16s %16s %16s\n\n", "n",
            "t(n)", "t(n)/n^1.8","t(n)/n^2","t(n)/n^2.2");

        for (n = 500; n <= 32000; n *= 2){
            aleatorio(v, n);

            tiempoantes = microsegundos();
            ord_ins(v, n);
            tiempodespues = microsegundos();
            t_n1 = tiempodespues - tiempoantes;

            if (t_n1 < 500){
                aleatorio(v, n);
                tiempoantes = microsegundos();
                for (i = 0; i < k; i++){
                    aleatorio(v, n);
                    ord_ins(v, n);
                }
                tiempodespues = microsegundos();
                t1 = tiempodespues - tiempoantes;

                tiempoantes = microsegundos();
                for (i = 0; i < k; i++){
                    aleatorio(v, n);
                }
                tiempodespues = microsegundos();
                t2 = tiempodespues - tiempoantes;
                t_total = (t1 - t2) / k;
                t_n1 = t_total;
                printf("*");
            } else {
                printf(" ");
            }

            t_n_18 = t_n1 / pow(n, 1.8);
            t_n_2 = t_n1 / pow(n, 2);
            t_n_22 = t_n1 / pow(n, 2.2);

            printf("%10d%16f%17f%17f%17f\n", n, t_n1, t_n_18, t_n_2, t_n_22);
        }
    }
}

void tiemposejecucionshellHibbard () {
    double tiempoantes, tiempodespues, t_n1, t_n_18, t_n_2, t_n_22, t1, t2, t_total;
    int n, m, k = 1000, i, v[MAX_V], inc[15];

    for (m = 1; m <= 3; m++){
        printf("\nOrdenación por shell con Hibbard e inicialización desordenada\n\n");
        printf("%10s %16s %16s %16s %16s\n\n", "n",
            "t(n)", "t(n)/n","t(n)/n^1.1","t(n)/n^1.25");

        for (n = 500; n <= 32000; n *= 2){
            hibbard(inc, 10);
            invertir(inc, 10);
            aleatorio(v, n);

            tiempoantes = microsegundos();
            ord_shell(v, n, inc, 10);
            tiempodespues = microsegundos();
            t_n1 = tiempodespues - tiempoantes;

            if (t_n1 < 500){
                aleatorio(v, n);
                tiempoantes = microsegundos();
                for (i = 0; i < k; i++){
                    aleatorio(v, n);
                    ord_shell(v, n, inc, 10);
                }
                tiempodespues = microsegundos();
                t1 = tiempodespues - tiempoantes;

                tiempoantes = microsegundos();
                for (i = 0; i < k; i++){
                    aleatorio(v, n);
                }
                tiempodespues = microsegundos();
                t2 = tiempodespues - tiempoantes;
                t_total = (t1 - t2) / k;
                t_n1 = t_total;
                printf("*");
            } else {
                printf(" ");
            }

            t_n_18 = t_n1 / n;
            t_n_2 = t_n1 / pow(n, 1.1);
            t_n_22 = t_n1 / pow(n, 1.25);
            printf("%10d%16f%17f%17f%17f\n", n, t_n1, t_n_18, t_n_2, t_n_22);
        }
    }
}

void tiempoejecucionshellKnuth(){
    double tiempoantes, tiempodespues, t_n1, t_n_18, t_n_2, t_n_22, t1, t2, t_total;
    int n, m, k = 1000, i, v[MAX_V], inc[15];

    for (m =1; m <= 3; m++){
        printf("\nOrdenación por shell con Knuth e inicialización desordenada\n\n");
        printf("%10s %16s %16s %16s %16s\n\n", "n",
            "t(n)", "t(n)/n","t(n)/n^1.1","t(n)/n^1.25");

        for (n = 500; n <= 32000; n *= 2){
            knuth(inc, 8);
            invertir(inc, 8);
            aleatorio(v, n);

            tiempoantes = microsegundos();
            ord_shell(v, n, inc, 8);
            tiempodespues = microsegundos();
            t_n1 = tiempodespues - tiempoantes;

            if (t_n1 < 500){
                aleatorio(v, n);
                tiempoantes = microsegundos();
                for (i = 0; i < k; i++){
                    aleatorio(v, n);
                    ord_shell(v, n, inc, 8);
                }
                tiempodespues = microsegundos();
                t1 = tiempodespues - tiempoantes;

                tiempoantes = microsegundos();
                for (i = 0; i < k; i++){
                    aleatorio(v, n);
                }
                tiempodespues = microsegundos();
                t2 = tiempodespues - tiempoantes;
                t_total = (t1 - t2) / k;
                t_n1 = t_total;
                printf("*");
            } else {
                printf(" ");
            }
            t_n_18 = t_n1 / n;
            t_n_2 = t_n1 / pow(n, 1.1);
            t_n_22 = t_n1 / pow(n, 1.25);
            printf("%10d%16f%17f%17f%17f\n", n, t_n1, t_n_18, t_n_2, t_n_22);
        }
    }
}

void tiempoejecucionshellSedgewick(){
    double tiempoantes, tiempodespues, t_n1, t_n_18, t_n_2, t_n_22, t1, t2, t_total;
    int n, m, k = 1000, i, v[MAX_V], inc[15];

    for (m =1; m <= 3; m++){
        printf("\nOrdenación por shell con Sedgewick e inicialización desordenada\n\n");
        printf("%10s %16s %16s %16s %16s\n\n", "n",
            "t(n)", "t(n)/n","t(n)/n^1.1","t(n)/n^1.25");

        for (n = 500; n <= 32000; n *= 2){
            sedgewick(inc, 7);
            invertir(inc, 7);
            aleatorio(v, n);

            tiempoantes = microsegundos();
            ord_shell(v, n, inc, 7);
            tiempodespues = microsegundos();
            t_n1 = tiempodespues - tiempoantes;

            if (t_n1 < 500){
                aleatorio(v, n);
                tiempoantes = microsegundos();
                for (i = 0; i < k; i++){
                    aleatorio(v, n);
                    ord_shell(v, n, inc, 7);
                }
                tiempodespues = microsegundos();
                t1 = tiempodespues - tiempoantes;

                tiempoantes = microsegundos();
                for (i = 0; i < k; i++){
                    aleatorio(v, n);
                }
                tiempodespues = microsegundos();
                t2 = tiempodespues - tiempoantes;
                t_total = (t1 - t2) / k;
                t_n1 = t_total;
                printf("*");
            } else {
                printf(" ");
            }
            t_n_18 = t_n1 / n;
            t_n_2 = t_n1 / pow(n, 1.1);
            t_n_22 = t_n1 / pow(n, 1.25);
            printf("%10d%16f%17f%17f%17f\n", n, t_n1, t_n_18, t_n_2, t_n_22);
        }
    }
}

void tiempoejecucionshellCiura(){
    double tiempoantes, tiempodespues, t_n1, t_n_18, t_n_2, t_n_22, t1, t2, t_total;
    int n, m, k = 1000, i, v[MAX_V], inc[15];

    for (m = 1; m <= 3; m++){
        printf("\nOrdenación por shell con Ciura e inicialización desordenada\n\n");
        printf("%10s %16s %16s %16s %16s\n\n", "n",
            "t(n)", "t(n)/n","t(n)/n^1.1","t(n)/n^1.25");

        for (n = 500; n <= 32000; n *= 2){
            ciura(inc, 10);
            invertir(inc, 10);
            aleatorio(v, n);

            tiempoantes = microsegundos();
            ord_shell(v, n, inc, 10);
            tiempodespues = microsegundos();
            t_n1 = tiempodespues - tiempoantes;

            if (t_n1 < 500){
                aleatorio(v, n);
                tiempoantes = microsegundos();
                for (i = 0; i < k; i++){
                    aleatorio(v, n);
                    ord_shell(v, n, inc, 10);
                }
                tiempodespues = microsegundos();
                t1 = tiempodespues - tiempoantes;

                tiempoantes = microsegundos();
                for (i = 0; i < k; i++){
                    aleatorio(v, n);
                }
                tiempodespues = microsegundos();
                t2 = tiempodespues - tiempoantes;
                t_total = (t1 - t2) / k;
                t_n1 = t_total;
                printf("*");
            } else {
                printf(" ");
            }
            t_n_18 = t_n1 / n;
            t_n_2 = t_n1 / pow(n, 1.1);
            t_n_22 = t_n1 / pow(n, 1.25);
            printf("%10d%16f%17f%17f%17f\n", n, t_n1, t_n_18, t_n_2, t_n_22);
        }
    }
}

int main(void) {
    inicializar_semilla();

    test_ins();
    test_shell_hibbard();
    test_shell_knuth();
    test_shell_sedgewick();
    test_shell_ciura();

    tiemposejecucioninserciondescendete ();
    tiemposejecucioninsercionascendente ();
    tiemposejecucioninserciondesordenada ();

    tiemposejecucionshellHibbard();
    tiempoejecucionshellKnuth();
    tiempoejecucionshellSedgewick();
    tiempoejecucionshellCiura();
    return 0;
}
