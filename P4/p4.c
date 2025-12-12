#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define LONGITUD_CLAVE 30
#define LONGITUD_SINONIMOS 300
#define TAM_ABIERTA 19069
#define TAM_CERRADA 38197

struct nodo {
char clave [LONGITUD_CLAVE];
char sinonimos [LONGITUD_SINONIMOS];
struct nodo *siguiente;
};

typedef struct nodo *pnodo;
typedef pnodo lista;
typedef lista *tabla_abierta;
typedef struct entrada_ {
int ocupada;
char clave [LONGITUD_CLAVE];
char sinonimos [LONGITUD_SINONIMOS];
} entrada;
typedef int pos;
typedef entrada *tabla_cerrada;
typedef struct {
char clave [LONGITUD_CLAVE];
char sinonimos [LONGITUD_SINONIMOS];
} item;

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

unsigned int dispersionA(char *clave, int tamTabla) {
    int i, n = MIN(8, strlen(clave));
    unsigned int valor = clave[0];
    for (i = 1; i < n; i++){
        valor += clave[i];
    }
    return valor % tamTabla; //asi se calcula el hash,  osea le pasamos clave(que seria el nombre), hace distintas operaciones
                             //y entonces nos devuelve un numero que sera el indice de la tabla. este numero, al ser %tamtabla siempre
                            //caera en un indice posible, y no superara el tamaño de la tabla. al ser dispersionAbierta 
                            //nos piden que el factor
                            //de carga sea 1, osea que el tamaño de la tabla debe ser igual al numero de entradas.
                            //(19069)
}

unsigned int dispersionB(char *clave, int tamTabla) {
    int i, n = MIN(8, strlen(clave));
    unsigned int valor = clave[0];
    for (i = 1; i < n; i++){
        valor = (valor<<5) + clave[i]; /* el desplazamiento de 5 bits equivale a */
    }
    return valor % tamTabla; /* multipicar por 32 */
}

void inicializar_abierta(tabla_abierta *diccionario, int tam) {
    int i;
    *diccionario = malloc(tam * sizeof(lista));
    if (*diccionario == NULL) {
        printf("Error al reservar memoria\n");
        exit(1);
    }
    for (i = 0; i < tam; i++)
        (*diccionario)[i] = NULL;
}

pnodo buscar_abierta(char *clave, tabla_abierta diccionario, int tam, int *colisiones, 
    unsigned int (*dispersion)(char *, int)){
    unsigned int posicion = dispersion(clave,tam);
    *colisiones = 0;
    pnodo actual = diccionario[posicion];
    while(actual!=NULL){
        if(strcmp(actual->clave,clave)==0){
            return actual;
        }
        (*colisiones)++;
        actual=actual->siguiente;
    }
    return NULL;
}

int insertar_abierta(char *clave, char *sinonimos, tabla_abierta *diccionario, int tam, 
    unsigned int (*dispersion)(char *, int)){
    unsigned int posicion=dispersion(clave,tam);
    int colisiones = 0;
    pnodo actual = (*diccionario)[posicion];
    while(actual!=NULL){
        colisiones++;
        actual=actual->siguiente;
    }
    pnodo nuevo = malloc(sizeof(struct nodo));
    if (!nuevo) {
        printf("Error al reservar memoria\n");
        return -1;
    }
    strcpy(nuevo->clave,clave);
    if(sinonimos!=NULL){strcpy(nuevo->sinonimos,sinonimos);}
    nuevo->siguiente = (*diccionario)[posicion];
    (*diccionario)[posicion] = nuevo;


    return colisiones;
}


void mostrar_abierta(tabla_abierta diccionario, int tam) {
    printf("{\n");
    for (int i = 0; i < tam; i++) {
        printf("%3d- ", i);
        pnodo actual = diccionario[i];
        printf("[");
        while (actual != NULL) {
            if(actual->sinonimos[0]=='\0'){
                printf(" (%s ) ", actual->clave);
            }
            else{(printf("(%s : %s)",actual->clave,actual->sinonimos));}
            actual = actual->siguiente;
        }
        printf("]");
        printf("\n");
    }
    printf("}\n");
}

void inicializar_cerrada(tabla_cerrada *diccionario, int tam){
    int i=0;
    *diccionario = malloc(tam * sizeof(entrada));
    if (*diccionario == NULL) {
        printf("Error al reservar memoria\n");
        exit(1);
    }
    while(i<tam){
        (*diccionario)[i].ocupada=0;
        (*diccionario)[i].clave[0]='\0';
        (*diccionario)[i].sinonimos[0]='\0';
        i++;
    }
}

pos buscarcerrada(char *clave, tabla_cerrada diccionario, int tam,int *colisiones, 
    unsigned int (*dispersion)(char *, int), unsigned int (*resol_colisiones)(int pos_ini, int num_intento)){
    int i = 0;
    unsigned int posicioninicial = dispersion(clave, tam);
    unsigned int posicion = posicioninicial;
    *colisiones = 0;
    while(i<tam){
        if(strcmp(diccionario[posicion].clave,clave)==0){
            return posicion;
        }
        if(!diccionario[posicion].ocupada){
            return -1;
        }
        (*colisiones)++;
        i++;
        posicion = resol_colisiones(posicioninicial,i) % tam;
    }
    return -1;//no se encontró y estaba llena la tabla
}

int insertar_cerrada(char *clave, char *sinonimos,tabla_cerrada *diccionario, int tam,
    unsigned int (*dispersion)(char *, int),unsigned int (*resol_colisiones)(int pos_ini, int num_intento)){
    int i = 0;
    unsigned int posicioninicial = dispersion(clave, tam);
    unsigned int posicion = posicioninicial;
    int colisiones = 0;
    while(i<tam){
        if(!(*diccionario)[posicion].ocupada){
            strcpy((*diccionario)[posicion].clave,clave);
            if(sinonimos!=NULL){strcpy((*diccionario)[posicion].sinonimos,sinonimos);}
            (*diccionario)[posicion].ocupada=1;
            return colisiones;
        }
        if (strcmp((*diccionario)[posicion].clave, clave) == 0) {
            if(sinonimos!=NULL){
                strcpy((*diccionario)[posicion].sinonimos, sinonimos);
            }
            return colisiones;
        }
        colisiones++;
        i++;
        posicion = resol_colisiones(posicioninicial,i) % tam;
    }
    return -1;//la tabla está llena
}

void mostrar_cerrada(tabla_cerrada diccionario, int tam){
    int i = 0;
    printf("{\n");
    for(i=0;i<tam;i++){
        printf("%3d- ",i);
        if(diccionario[i].ocupada){
            if(diccionario[i].sinonimos[0]!='\0'){printf("(%s : %s)",diccionario[i].clave,diccionario[i].sinonimos);}
            else{printf("(%s )",diccionario[i].clave);}
        }
        printf("\n");
    }
    printf("}\n");
}

unsigned int exploracion_lineal(int pos_ini, int intento) {
    return pos_ini + intento;
}

unsigned int exploracion_cuadratica(int pos_ini, int intento) {
    return pos_ini + intento * intento;
}

unsigned int exploracion_doble(int pos_ini, int intento) {
    unsigned int h2 = 10007 - (pos_ini % 10007);
    return pos_ini + intento * h2;
}


unsigned int ndispersion(char *clave, int tamTabla) {
    (void)tamTabla;
    if (strcmp(clave, "ANA") == 0) return 7;
    if (strcmp(clave, "JOSE") == 0) return 7;
    if (strcmp(clave, "OLGA") == 0) return 7;
    return 6;
}

int leer_sinonimos(item datos[]) {
    char c;
    int i, j;
    FILE *archivo;
    if ((archivo = fopen("sinonimos.txt", "r")) == NULL) {
        printf("Error al abrir 'sinonimos.txt'\n");
        return(EXIT_FAILURE);
    }
    for (i = 0; fscanf(archivo, "%s", datos[i].clave) != EOF; i++) {
        if ((c = fgetc(archivo)) != '\t') {
            printf("Error al leer el tabulador\n");
            return(EXIT_FAILURE);
        }
        for (j = 0; (c = fgetc(archivo)) != '\n'; j++) {
            if (j < LONGITUD_SINONIMOS - 1)
                datos[i].sinonimos[j] = c;
        }
        datos[i].sinonimos[MIN(j, LONGITUD_SINONIMOS -1)] = '\0';
    }
    if (fclose(archivo) != 0) {
        printf("Error al cerrar el fichero\n");
        return(EXIT_FAILURE);
    }
    return(i);
}

void liberar_abierta(tabla_abierta *diccionario, int tam) {
    int i;
    for (i = 0; i < tam; i++) {
        pnodo actual = (*diccionario)[i];
        while (actual != NULL) {
            pnodo siguiente = actual->siguiente;
            free(actual);
            actual = siguiente;
        }
    }
    free(*diccionario);
    *diccionario = NULL;
}

void liberar_cerrada(tabla_cerrada *diccionario) {
    free(*diccionario);
    *diccionario = NULL;
}

void testabierta(){
    tabla_abierta dicc;
    char* elementos[6] = {"ANA","LUIS","JOSE","OLGA","ROSA","IVAN"};
    char* busqueda[7] = {"ANA","LUIS","JOSE","OLGA","ROSA","IVAN","CARLOS"};
    int tam = 11;
    int i = 0, j = 0, colisiones = 0;
    inicializar_abierta(&dicc,tam);
    while(i<6){
        colisiones += insertar_abierta(elementos[i],"",&dicc,tam,ndispersion);
        i++;
    }
    mostrar_abierta(dicc,tam);
    printf("Número total de colisiones: %d\n",colisiones);
    while(j<7){
        if(!buscar_abierta(busqueda[j],dicc,tam,&colisiones,ndispersion)){
            printf("No encuentro: %s\n",busqueda[j]);
        }
        else{
            printf("Al buscar: %s, encuentro: %s, colisiones: %d\n",busqueda[j],busqueda[j],colisiones);
        }
        j++;
    }
}

void testcerrada(unsigned int (*resol_colisiones)(int pos_ini, int num_intento)){
    tabla_cerrada dicc;
    int tam = 11, i = 0, colisiones = 0, j = 0,pos;
    char* elementos[6] = {"ANA","LUIS","JOSE","OLGA","ROSA","IVAN"};
    char* busqueda[7] = {"ANA","LUIS","JOSE","OLGA","ROSA","IVAN","CARLOS"};
    inicializar_cerrada(&dicc,tam);
    while(i<6){
        colisiones += insertar_cerrada(elementos[i],"",&dicc,tam,ndispersion,resol_colisiones);
        i++;
    }
    mostrar_cerrada(dicc,tam);
    printf("Número total de colisiones al insertar los elementos: %d\n",colisiones);
    while(j<7){
        pos = buscarcerrada(busqueda[j], dicc, tam, &colisiones, ndispersion, resol_colisiones);
        if(pos==-1){
            printf("No encuentro: %s, colisiones: %d\n",busqueda[j],colisiones);
        }
        else{
            printf("Al buscar: %s, encuentro: %s, colisiones: %d\n",busqueda[j],busqueda[j],colisiones);
        }
        j++;
    }
}

void sinonimos_abierta(item datos[], int tam) {
    tabla_abierta ta;
    int i, d, colisiones;
    unsigned int (*dispersiones[])(char *, int) = {dispersionA, dispersionB};
    char *nombres_disp[] = {"A", "B"};

    printf("\nTABLA ABIERTA (colisiones sinonimos)\n");
    for (d = 0; d < 2; d++){
        colisiones = 0;
        inicializar_abierta(&ta, TAM_ABIERTA);
        
        printf("\nDispersion %s:\n", nombres_disp[d]);
        for (i = 0; i < tam; i++){
            colisiones += insertar_abierta(datos[i].clave, datos[i].sinonimos, 
            &ta, TAM_ABIERTA, dispersiones[d]);
        }
        printf("Colisiones totales: %d\n", colisiones);
        liberar_abierta(&ta, TAM_ABIERTA);
    }
}

void sinonimos_cerrada(item datos[], int tam) {
    tabla_cerrada tc;
    int i,d, e, colisiones;
    unsigned int (*dispersiones[])(char *, int) = {dispersionA, dispersionB};
    unsigned int (*exploraciones[])(int, int) = {
        exploracion_lineal, 
        exploracion_cuadratica, 
        exploracion_doble
    };
    char *nombres_disp[] = {"A", "B"};
    char *nombres_exp[] = {"Lineal", "Cuadratica", "Doble"};

    printf("\nTABLA CERRADA (colisiones sinonimos)\n");
    
    for (d = 0; d < 2; d++) {
        printf("\nDispersion %s\n", nombres_disp[d]);
        
        for (e = 0; e < 3; e++) {
            colisiones = 0;
            inicializar_cerrada(&tc, TAM_CERRADA);
            
            printf("Exploracion %s: ", nombres_exp[e]);
            for (i = 0; i < tam; i++) {
                colisiones += insertar_cerrada(datos[i].clave, datos[i].sinonimos,
                                &tc, TAM_CERRADA, dispersiones[d], exploraciones[e]);
            }
            printf("%d colisiones\n", colisiones);
            liberar_cerrada(&tc);
        }
    }
}

double medir_tiempo_busqueda_abierta(int n, item datos[], int tam, unsigned int (*dispersion)(char *, int),
                                     tabla_abierta ta, int k, int *colisiones_tmp) {
    double tiempoantes, tiempodespues, t_n1, t_n2, treal, t1, t2, t_total;
    int i, j, posicionrandom;
    tiempoantes = microsegundos();
    for (i = 0; i < n; i++) {
        posicionrandom = rand() % tam;
        buscar_abierta(datos[posicionrandom].clave, ta, tam, colisiones_tmp, dispersion);
    }
    tiempodespues = microsegundos();
    t_n1 = tiempodespues - tiempoantes;
    tiempoantes = microsegundos();
    for (i = 0; i < n; i++){
        posicionrandom = rand() % tam;
    }
    tiempodespues = microsegundos();
    t_n2 = tiempodespues - tiempoantes;
    treal = t_n1 - t_n2;
    if (treal < 500){
        tiempoantes = microsegundos();
        for (i = 0; i < k; i++){
            for (j = 0; j < n; j++) {
                posicionrandom = rand() % tam;
                buscar_abierta(datos[posicionrandom].clave, ta, tam, colisiones_tmp, dispersion);
            }
        }
        tiempodespues = microsegundos();
        t1 = tiempodespues - tiempoantes;
        tiempoantes = microsegundos();
        for (i = 0; i < k; i++){
            for (j = 0; j < n; j++) {
                posicionrandom = rand() % tam;
            }
        }
        tiempodespues = microsegundos();
        t2 = tiempodespues - tiempoantes;
        t_total = (t1-t2)/k;
        treal = t_total;
        printf("*");
    }
    else{
        printf(" ");
    }
    return treal;
}

void tiemposejecucionabierta(item datos[], int tam){
    tabla_abierta ta;
    double treal, t_sub, t_medio, t_sobre;
    int n, colisiones, m, k = 100, i, d, colisiones_tmp = 0;
    unsigned int (*dispersiones[])(char *, int) = {dispersionA, dispersionB};
    char *nombres_disp[] = {"A", "B"};

    for (d = 0; d < 2; d++) {
        for (m = 1; m <= 3; m++){
            colisiones = 0;
            inicializar_abierta(&ta, TAM_ABIERTA);
            for (i = 0; i < tam; i++){
                colisiones += insertar_abierta(datos[i].clave, datos[i].sinonimos, 
                &ta, TAM_ABIERTA, dispersiones[d]);
            }

            printf("\n***Dispersión abierta con dispersión %s\nInsertando %d elementos...Numero total de colisiones: %d\nBuscando n elementos...\n",
                nombres_disp[d],tam,colisiones);
            printf("%10s %16s %16s %16s %16s\n\n", "n","t(n )", "t(n)/n^0.8","t(n)/n","t(n)/n*log(n)");

            for (n=125; n<=16000;n*=2){
                treal = medir_tiempo_busqueda_abierta(n, datos, tam, dispersiones[d], ta, k, &colisiones_tmp);

                t_sub = treal/ pow(n,0.8);
                t_medio = treal/ n;
                t_sobre = treal/ (n*log(n));

                printf("%10d%16f%17f%17f%17f\n",n,treal,t_sub,t_medio,t_sobre);

            }
            liberar_abierta(&ta, TAM_ABIERTA);
        }
    }
}

void impresiontabla(int e, int d){
    switch(e){
            case 0:
                switch(d){
                    case 0:
                        printf("%10s %16s %16s %16s %16s\n\n", "n","t(n)", "t(n)/n^0.8","t(n)/n","t(n)/n*log(n)");    
                        break;
                    case 1:
                        printf("%10s %16s %16s %16s %16s\n\n", "n","t(n)", "t(n)/n^0.8","t(n)/n","t(n)/n*log(n)");
                        break;
                }
                break;
            case 1:
                switch(d){
                    case 0:
                        printf("%10s %16s %16s %16s %16s\n\n", "n","t(n)", "t(n)/n^0.8","t(n)/n","t(n)/n*log(n)");    
                        break;
                    case 1:
                        printf("%10s %16s %16s %16s %16s\n\n", "n","t(n)", "t(n)/n^0.8","t(n)/n","t(n)/n*log(n)");
                        break;
                }
                break;
            case 2:
                switch(d){
                    case 0:
                        printf("%10s %16s %16s %16s %16s\n\n", "n","t(n)", "t(n)/n^0.8","t(n)/n","t(n)/n*log(n)");    
                        break;
                    case 1:
                        printf("%10s %16s %16s %16s %16s\n\n", "n","t(n)", "t(n)/n^0.8","t(n)/n","t(n)/n*log(n)");
                        break;
                }
                break;
        }
    }

void impresioncotas(int e, int d,int n,double treal){
    double t_sub,t_medio,t_sobre;
    switch(e){
        case 0:
        switch(d){
            case 0:
                t_sub = treal/ pow(n,0.8);
                t_medio = treal/ n;
                t_sobre = treal/ (n*log(n));
                printf("%10d%16f%17f%17f%17f\n",n,treal,t_sub,t_medio,t_sobre);    
            break;
            case 1:
                t_sub = treal/ pow(n,0.8);
                t_medio = treal/ n;
                t_sobre = treal/ (n*log(n));
                printf("%10d%16f%17f%17f%17f\n",n,treal,t_sub,t_medio,t_sobre);
            break;
        }
        break;
        case 1:
            switch(d){
                case 0:
                    t_sub = treal/ pow(n,0.8);
                    t_medio = treal/ n;
                    t_sobre = treal/ (n*log(n));
                    printf("%10d%16f%17f%17f%17f\n",n,treal,t_sub,t_medio,t_sobre);    
                break;
                case 1:
                    t_sub = treal/ pow(n,0.8);
                    t_medio = treal/ n;
                    t_sobre = treal/ (n*log(n));
                    printf("%10d%16f%17f%17f%17f\n",n,treal,t_sub,t_medio,t_sobre);
                break;
            }
            break;
        case 2:
            switch(d){
                case 0:
                    t_sub = treal/ pow(n,0.8);
                    t_medio = treal/ n;
                    t_sobre = treal/ (n*log(n));
                    printf("%10d%16f%17f%17f%17f\n",n,treal,t_sub,t_medio,t_sobre);    
                break;
                case 1:
                    t_sub = treal/ pow(n,0.8);
                    t_medio = treal/ n;
                    t_sobre = treal/ (n*log(n));
                    printf("%10d%16f%17f%17f%17f\n",n,treal,t_sub,t_medio,t_sobre);
                break;
                }
        break;
    }
}

double medir_tiempo_busqueda_cerrada(int n, item datos[], int tam, unsigned int (*dispersion)(char *, int),
                     unsigned int (*exploracion)(int, int), tabla_cerrada tc, int k, int *colisiones_tmp) {
    double tiempoantes, tiempodespues, t_n1, t_n2, treal, t1, t2, t_total;
    int i, j, posicionrandom;
    tiempoantes = microsegundos();
    for (i = 0; i < n; i++) {
        posicionrandom = rand() % tam;
        buscarcerrada(datos[posicionrandom].clave, tc, tam, colisiones_tmp, dispersion, exploracion);
    }
    tiempodespues = microsegundos();
    t_n1 = tiempodespues-tiempoantes;
    tiempoantes = microsegundos();
    for (i = 0; i < n; i++){
        posicionrandom = rand() % tam;
    }
    tiempodespues = microsegundos();
    t_n2 = tiempodespues-tiempoantes;
    treal = t_n1 - t_n2;
    if (treal < 500){
        tiempoantes = microsegundos();
        for (i = 0; i < k; i++){
            for (j = 0; j < n; j++) {
                posicionrandom = rand() % tam;
                buscarcerrada(datos[posicionrandom].clave, tc, tam, colisiones_tmp, dispersion, exploracion);
            }
        }
        tiempodespues = microsegundos();
        t1 = tiempodespues - tiempoantes;
        tiempoantes = microsegundos();
        for (i = 0; i < k; i++){
            for (j = 0; j < n; j++) {
                posicionrandom = rand() % tam;
            }
        }
        tiempodespues = microsegundos();
        t2 = tiempodespues - tiempoantes;
        t_total = (t1-t2)/k;
        treal=t_total;
        printf("*");
    }
    else{
        printf(" ");
    }
    return treal;
}

void tiemposejecucioncerrada(item datos[], int tam){
    tabla_cerrada tc;
    double treal;
    int n,colisiones,m,k = 100,i,d,e,colisiones_tmp = 0;
    unsigned int (*dispersiones[])(char *, int) = {dispersionA, dispersionB};
    unsigned int (*exploraciones[])(int, int) = {
        exploracion_lineal, 
        exploracion_cuadratica, 
        exploracion_doble
    };
    char *nombres_disp[] = {"A", "B"};
    char *nombres_exp[] = {"lineal", "cuadratica", "doble"};

    for(e=0;e<3;e++){
        for (d = 0; d < 2; d++) {
            for (m = 1; m <= 3; m++){
                colisiones = 0;
                inicializar_cerrada(&tc, TAM_CERRADA);
                for (i = 0; i < tam; i++){
                    colisiones += insertar_cerrada(datos[i].clave, datos[i].sinonimos, 
                    &tc, TAM_CERRADA, dispersiones[d],exploraciones[e]);
                }

                printf("\n***Dispersión cerrada %s con dispersión %s\nInsertando %d elementos...Numero total de colisiones: %d\nBuscando n elementos...\n",
                    nombres_exp[e],nombres_disp[d],tam,colisiones);
                impresiontabla(e,d);
                for (n=125; n<=16000;n*=2){
                    treal = medir_tiempo_busqueda_cerrada(n, datos, tam, dispersiones[d], exploraciones[e], tc, k, &colisiones_tmp);
                    
                    impresioncotas(e,d,n,treal);
                }
                liberar_cerrada(&tc);
            }
        }
    }
}

int main(void) {
    item datos[TAM_ABIERTA];
    inicializar_semilla();

    printf("***TABLA ABIERTA\n");
    testabierta();
    printf("\n***TABLA CERRADA LINEAL\n");
    testcerrada(exploracion_lineal); //cerrada con expl. lineal
    printf("\n***TABLA CERRADA CUADRÁTICA\n");
    testcerrada(exploracion_cuadratica); //cerrada con expl.
    printf("\n***TABLA CERRADA DOBLE\n");
    testcerrada(exploracion_doble); //cerrada con expl. doble

    int num_datos = leer_sinonimos(datos);
     if (num_datos == -1) {
        printf("\nError al leer los datos\n");
        return 1;
    }
    printf("\nLeidos %d sinonimos del archivo\n", num_datos);

    sinonimos_abierta(datos,num_datos);
    sinonimos_cerrada(datos, num_datos);
    tiemposejecucionabierta(datos,num_datos);
    tiemposejecucioncerrada(datos,num_datos);
    return 0;
}