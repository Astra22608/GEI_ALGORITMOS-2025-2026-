#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

double microsegundos() { /* obtiene la hora del sistema en microsegundos */
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

int sumaSubMax1 (int vector[],int tamano){
    int sumaMax=0;
    int estasuma=0;
    for(int i=0;i<tamano;i++){
        estasuma=0;
        for(int j=i;j<tamano;j++){
            estasuma=estasuma+vector[j];
            if(estasuma>sumaMax){
                sumaMax=estasuma;
            }
        }
    }
    return sumaMax;
}

int sumaSubMax2 (int vector[],int tamano){
    int estasuma=0;
    int sumaMax=0;
    for(int j=0;j<tamano;j++){
        estasuma=estasuma+vector[j];
        if(estasuma>sumaMax){sumaMax=estasuma;}
        else if(estasuma <0){estasuma=0;}
    }
    return sumaMax;
}

void inicializar_semilla() {
    srand(time(NULL));
    /* se establece la semilla de una nueva serie de enteros pseudo-aleatorios*/
}

void aleatorio(int v [], int n) {
    int i, m=2*n+1;
    for (i=0; i < n; i++)
    v[i] = (rand() % m) - n;
    /* se generan números pseudoaleatorio entre -n y +n*/
}

void imprimirVector(int v[],int filas){
    printf ("\n");
    printf("[");
    for (int i=0;i<filas;i++){
        printf("%3d,",v[i]);
    }
    printf("]");
}

void test1(){
    int i,a,b, n =5;
  
       int v[6][5] = {
        {-9,  2, -5, -4,  6},
        { 4,  0,  9,  2,  5},
        {-2, -1, -9, -7, -1},
        { 9, -2,  1, -7, -8},
        {15, -2, -5, -4, 16},
        { 7, -5,  6,  7, -7}
        };

    printf("\ntest 1\n");
    printf("%28s%33s%17s\n", "", "sumaSubMax1", "sumaSubMax2");

    for (i=0; i<6; i++) {
        imprimirVector(v[i], n);
        a = sumaSubMax1(v[i], n);
        b = sumaSubMax2(v[i], n);
        printf("%33d%18d\n", a, b);
    }
}

void test2() {
    int i, a, b;
    int v[9];
    printf("\n\n");
    printf("test 2\n");
    printf("%28s%33s%17s\n", "", "sumaSubMax1", "sumaSubMax2");
    
    for (i=0; i<10; i++) {
        aleatorio(v, 9);
        imprimirVector(v, 9);
        a = sumaSubMax1(v, 9);
        b = sumaSubMax2(v, 9);
        printf("%20d%16d\n", a, b);
    }
}

void tiemposejecucion1(){

  double tiempoantes, tiempodespues, t_n1,t_n_18,t_n_2,t_n_22, t1, t2, t_total;
  
    printf("\nMedición de tiempo sumaSubMax1\n\n");
    printf("%10s %16s %16s %16s %16s\n\n", "n",
        "t(n)", "t(n)/n^1.8","t(n)/n^2","t(n)/n^2.2");
  
    for (int n=500; n<=32000;n*=2){
        int v[n], k = 100;
        aleatorio(v,n);
  
        tiempoantes = microsegundos();
        sumaSubMax1(v,n);
        tiempodespues = microsegundos();
        t_n1 = tiempodespues-tiempoantes;

        if (t_n1 < 500){
            tiempoantes = microsegundos();
            for (int i = 0; i < k; i++){
                sumaSubMax1(v,n);
            }
            tiempodespues = microsegundos();
            t1 = tiempodespues - tiempoantes;

            tiempoantes = microsegundos();
            for (int i = 0; i < k; i++){}
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
        t_n_2 = t_n1/ (pow (n,2));
        t_n_22 = t_n1/ (pow (n, 2.2));

        printf("%10d%16f%17f%17f%17f\n",n,t_n1,t_n_18,t_n_2,t_n_22);
    }
}

void tiemposejecucion2 (){
      double tiempoantes, tiempodespues, t_n2,t_n_18,t_n_2,t_n_log, t1, t2, t_total;
  
    printf("\nMedición de tiempo sumaSubMax2\n\n");
    printf("%10s %16s %16s %16s %16s\n\n", "n",
        "t(n)", "t(n)/n^0.8","t(n)/n","t(n)/n*log2(n)");
  
    for (int n=500; n<=32000;n*=2){
        int v[n], k = 100;
        aleatorio(v,n);
  
        tiempoantes = microsegundos();
        sumaSubMax2(v,n);
        tiempodespues = microsegundos();
        t_n2 = tiempodespues-tiempoantes;

        if (t_n2 < 500){
            tiempoantes = microsegundos();
            for (int i = 0; i < k; i++){
                sumaSubMax2(v,n);
            }
            tiempodespues = microsegundos();
            t1 = tiempodespues - tiempoantes;

            tiempoantes = microsegundos();
            for (int i = 0; i < k; i++){}
            tiempodespues = microsegundos();
            t2 = tiempodespues - tiempoantes;
            t_total = (t1-t2)/k;
            t_n2=t_total;
            printf("*");
        }
        else{
            printf(" ");
        }

        t_n_18 = t_n2/ (pow (n, 0.8));
        t_n_2 = t_n2/ (pow (n,1));
        t_n_log = t_n2/ (n*log(n));

        printf("%10d %15f %16f %16f %16f\n",n,t_n2,t_n_18,t_n_2,t_n_log);
    }
}

int main(void) {
    inicializar_semilla ();
    test1();
    test2();
    tiemposejecucion1();
    tiemposejecucion2();
    return 0;
}