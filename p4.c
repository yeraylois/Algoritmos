/* 
Yeray Lois Sánchez  --> yeray.lois@udc.es                                  
Anxo Galdo Blasco   --> anxo.galdo.blasco@udc.es                            
Sofía Oubiña Falcon --> sofia.oubina.falcon@udc.es  
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>

#define TAM 256000

#define INTERCAMBIAR(a, b) { int temp = (a); (a) = (b); (b) = temp; }
#define PADRE(i)       (((i) - 1) / 2)
#define HIJO_IZQUIERDO(i)  ((2 * (i)) + 1)
#define HIJO_DERECHO(i)    ((2 * (i)) + 2)

// ------------------------------------------------------------------------- //
//                            Definiciones de Tipos                          //
// ------------------------------------------------------------------------- //

struct monticulo {
  int ultimo;
  int vector[TAM];
};


typedef struct monticulo * pmonticulo;

// ------------------------------------------------------------------------- //
//                          Funciones de Utilidades                          //
// ------------------------------------------------------------------------- //

/* Inicializar semilla para generación aleatoria */
void inicializar_semilla() {
    srand(time(NULL));
}

double microsegundos() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0)
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void imprimirMonticulo(const struct monticulo *m) {
    printf("Montículo: [ ");
    for (int i = 0; i <= m->ultimo; i++) {
        printf("%d ", m->vector[i]);
    }
    printf("]\n");
}


void mostrarPromptInicio(const char* prompt) {
    int total_width = 80;  // Número total de caracteres (incluyendo '=' y espacios)
    int border_width = total_width;
    int text_length = strlen(prompt);
    int padding = (border_width - text_length) / 2;  // Espacios a cada lado del texto
    int left_padding = padding;                     // Espacios a la izquierda
    int right_padding = border_width - text_length - left_padding;  // Espacios a la derecha

    printf("\n");
    for (int i = 0; i < border_width; i++) {
        printf("=");
    }
    printf("\n");

    printf("%*s%s%*s\n", left_padding, "", prompt, right_padding, "");

    for (int i = 0; i < border_width; i++) {
        printf("=");
    }
    printf("\n\n");
}


void ascendente(int v[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        v[i] = i;
    }
}

void descendente(int v[], int n) {
    int i, x = n;
    for (i = 0; i < n; i++) {
        v[i] = x - i;
    }
}

void aleatorio(int v[], int n) {
    int i, m = 2 * n + 1;
    for (i = 0; i < n; i++) {
        v[i] = (rand() % m) - n;
    }
}

// ------------------------------------------------------------------------- //
//                          Funciones de Montículos                          //
// ------------------------------------------------------------------------- //

// Inicializa un montículo vacío
void iniMonticulo(pmonticulo m) {
    m->ultimo = -1;  // Índice inicial para un montículo vacío
}

void flotar(pmonticulo m, int i) {
    while (i > 0 && m->vector[PADRE(i)] > m->vector[i]) { // Comparar con el PADRE
        // Intercambiar con el PADRE
        INTERCAMBIAR(m->vector[PADRE(i)], m->vector[i]);
        i = PADRE(i); // Actualizar índice para subir al nivel del PADRE
    }
}

void insertarMonticulo(pmonticulo m, int x) {
    if (m->ultimo + 1 >= TAM) {
        printf("Error: Montículo lleno.\n");
        return;
    }
    m->ultimo++;
    m->vector[m->ultimo] = x;  // Añadir al final
    flotar(m, m->ultimo);      // Ajustar el montículo
}


void hundir(pmonticulo m, int i) {
    int hijoIzq, hijoDer, j;
    do {
        hijoIzq = HIJO_IZQUIERDO(i); 
        hijoDer = HIJO_DERECHO(i);  
        j = i;                    

        // Si el hijo derecho es menor que el nodo actual, actualiza el índice
        if (hijoDer <= m->ultimo && m->vector[hijoDer] < m->vector[i]) {
            i = hijoDer;
        }

        // Si el hijo izquierdo es menor que el nodo actual (o menor que el derecho), actualiza el índice
        if (hijoIzq <= m->ultimo && m->vector[hijoIzq] < m->vector[i]) {
            i = hijoIzq;
        }

        INTERCAMBIAR(m->vector[j], m->vector[i]);
        
        
    } while (j != i); // Repetir hasta que el nodo alcance su posición final
}

// Quita el menor elemento del montículo
void quitarMenor(pmonticulo m) {
    if (m->ultimo == -1) {
        printf("Error: Montículo vacío.\n");
        return;
    }
    m->vector[0] = m->vector[m->ultimo];  // Reemplazar raíz con el último
    m->ultimo--;
    hundir(m, 0);  // Ajustar el montículo
}

// Consulta el menor elemento del montículo
int consultarMenor(const pmonticulo m) {
    if (m->ultimo == -1) {
        printf("Error: Montículo vacío.\n");
        return -1;
    }
    return m->vector[0];
}


void crearMonticulo(pmonticulo m, int v[], int n ) {
		int aux, i;
				
    // Copiar elementos del array al montículo
    for (aux = 0; aux < n; aux++) {
        m->vector[aux] = v[aux];
    }

    // Actualizar el índice del último elemento
    m->ultimo = n - 1;

    // Ajustar desde el último PADRE hasta la raíz
    for (i = (m->ultimo - 1) / 2; i >= 0; i--) {
        hundir(m, i);
    }
}

// ------------------------------------------------------------------------- //
//                          Ordenación por Montículos                        //
// ------------------------------------------------------------------------- //

// Ordenar un vector utilizando ordenación por montículos
void ordenarPorMonticulos(int v[], int n) {
    struct monticulo m;
    crearMonticulo(&m, v, n);
    for (int i = 0; i < n; i++) {
        v[i] = consultarMenor(&m);
        quitarMenor(&m);
    }
}

// ------------------------------------------------------------------------- //
//                                   Tests                                   //
// ------------------------------------------------------------------------- //

void test() {
    mostrarPromptInicio("TEST"); 
    struct monticulo m;
    int valores[] = {15, 13, 12, 8, 9, 5, 8, 7, 5, 3, 4, 5};
    int n = sizeof(valores) / sizeof(valores[0]);

    printf("Inicializando el montículo...\n");
    iniMonticulo(&m);
    printf("Montículo inicializado.\n");
    imprimirMonticulo(&m);

    printf("\nInsertando el valor 10 en el montículo...\n");
    insertarMonticulo(&m, 10);
    imprimirMonticulo(&m);

    printf("\nConsultando el menor elemento del montículo...\n");
    int menor = consultarMenor(&m);
    printf("El menor elemento es: %d (esperado: 10)\n", menor);

    printf("\nQuitando el menor elemento del montículo...\n");
    quitarMenor(&m);
    imprimirMonticulo(&m);

    printf("\nInsertando los valores del array en el montículo...\n");
    for (int i = 0; i < n; i++) {
        insertarMonticulo(&m, valores[i]);
    }
    imprimirMonticulo(&m);

    printf("\nCreando un nuevo montículo directamente desde los valores dados...\n");
    crearMonticulo(&m, valores, n);
    imprimirMonticulo(&m);

    printf("\nConsultando el menor elemento del nuevo montículo...\n");
    menor = consultarMenor(&m);
    printf("El menor elemento es: %d (esperado: 3)\n", menor);
    
    printf("\nOrdenando el vector usando ordenarPorMonticulos...\n");
    ordenarPorMonticulos(valores, n);
    
    printf("Vector ordenado: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", valores[i]);
    }
}

// ------------------------------------------------------------------------- //
//                          Funciones de Tiempos                             //
// ------------------------------------------------------------------------- //
void medirInsercionUnoAUno() {
    int n, k = 10000;
    double tiempo_inicio, tiempo_fin, tiempo_total;
    double tiempo_con_insercion, tiempo_sin_insercion;
    double tiempo_ajustada, tiempo_subestimada, tiempo_sobreestimada;
    int *v;
    struct monticulo m;
    
    printf("\nDemostración empírica de complejidades de montículos\n");
    printf("-----------------------------------------------------------------------\n");
    printf("                Inserción de n elementos uno a uno (O(n log n))\n\n");
    printf("%10s%16s%25s%22s%22s\n", "n", "[t(n)]", "[t(n)/(n log n)]", "[t(n)/(n log n)]", "[t(n)/(n log n)]");

    for (n = 500; n <= TAM; n *= 2) {
        
        v = (int *)malloc(n * sizeof(int));
        if (v == NULL) {
            printf("Error al asignar memoria\n");
            exit(EXIT_FAILURE);
        }
           
        iniMonticulo(&m);
        descendente(v, n);
        
        // Medir tiempo de inserción
        tiempo_inicio = microsegundos();
        for (int i = 0; i < n; i++) {
            insertarMonticulo(&m, v[i]);
        }
        tiempo_fin = microsegundos();
        tiempo_total = tiempo_fin - tiempo_inicio;
       

        if (tiempo_total < 500) {
            int repeticiones = k;

            // Medir tiempo sin inserciones (solo generación)
            tiempo_inicio = microsegundos();
            for (int rep = 0; rep < repeticiones; rep++) {
                iniMonticulo(&m);
            }
            tiempo_fin = microsegundos();
            tiempo_sin_insercion = tiempo_fin - tiempo_inicio;

            // Medir tiempo total con inserciones
            tiempo_inicio = microsegundos();
            for (int rep = 0; rep < repeticiones; rep++) {
                iniMonticulo(&m);

                for (int i = 0; i < n; i++) {
                    insertarMonticulo(&m, v[i]);
                }
            }
            tiempo_fin = microsegundos();
            tiempo_con_insercion = tiempo_fin - tiempo_inicio;

            // Calcular tiempo promedio ajustado
            tiempo_total = (tiempo_con_insercion - tiempo_sin_insercion) / repeticiones;
            
            printf("(*)");
            
        } else {
            printf("   ");
        }

        // Cálculos de cotas
        tiempo_subestimada = tiempo_total / pow(n,1.02);
        tiempo_ajustada = tiempo_total / (n * log(n));
        tiempo_sobreestimada = tiempo_total /  pow(n,1.25);

        // Mostrar resultados
        printf("%8d%15.4f%22.8f%22.8f%22.8f\n", n, tiempo_total, tiempo_subestimada, tiempo_ajustada, tiempo_sobreestimada);
        
        free(v);
    }
}
void medirCreacionDesdeVector() {
    int n, k = 10000;
    double tiempo_inicio, tiempo_fin, tiempo_total, tiempo_generacion;
    double tiempo_ajustada, tiempo_subestimada, tiempo_sobreestimada;
    int *v;
    struct monticulo m;

    printf("\n                Creación de montículo desde un vector (O(n))\n\n");
    printf("%10s%16s%23s%20s%24s\n", "n", "[t(n)]", "[t(n)/n^0.95]", "[t(n)/n]", "[t(n)/n^1.2]");

    for (n = 500; n <= TAM; n *= 2) {
        
        v = (int *)malloc(n * sizeof(int));
        if (v == NULL) {
            printf("Error al asignar memoria\n");
            exit(EXIT_FAILURE);
        }
        
        aleatorio(v, n);

        tiempo_inicio = microsegundos();
        crearMonticulo(&m, v, n);
        tiempo_fin = microsegundos();
        tiempo_total = tiempo_fin - tiempo_inicio;

        if (tiempo_total < 500) {
            int repeticiones = k;
            tiempo_total = 0.0;
            tiempo_generacion = 0.0;

            for (int rep = 0; rep < repeticiones; rep++) {
                // Medir tiempo de generación
                tiempo_inicio = microsegundos();
                aleatorio(v, n);
                tiempo_fin = microsegundos();
                tiempo_generacion += tiempo_fin - tiempo_inicio;

                // Medir tiempo de creación del montículo
                tiempo_inicio = microsegundos();
                aleatorio(v,n);
                crearMonticulo(&m, v, n);
                tiempo_fin = microsegundos();
                tiempo_total += tiempo_fin - tiempo_inicio;
            }

            // Calcular tiempo promedio ajustado
            tiempo_total = (tiempo_total - tiempo_generacion) / repeticiones;
            printf("(*)");
            
            
        } else {
            printf("   ");
        }

        // Cálculos de cotas
        tiempo_subestimada = tiempo_total / pow(n, 0.9);
        tiempo_ajustada = tiempo_total / n;
        tiempo_sobreestimada = tiempo_total / pow(n, 1.1);

        // Mostrar resultados
        printf("%8d%15.4f%22.8f%22.8f%22.8f\n", n, tiempo_total, tiempo_subestimada, tiempo_ajustada, tiempo_sobreestimada);
        
        free(v);
    }

    printf("-----------------------------------------------------------------------\n");
}

void tCompararMonticulos() {
    mostrarPromptInicio("COMPARACION");
    medirInsercionUnoAUno();
    medirCreacionDesdeVector();
}

// ------------------------------------------------------------------------- //
//                        Funciones de Ordenación                            //
// ------------------------------------------------------------------------- //

double medirTiempoOrdenacion(void (*init)(int *, int), int v[], int n) {
    double tiempo_inicio, tiempo_fin;
    (*init)(v, n);
    tiempo_inicio = microsegundos();
    ordenarPorMonticulos(v, n);
    tiempo_fin = microsegundos();
    return (tiempo_fin - tiempo_inicio);
}

double ajustarTiempoOrdenacion(void (*init)(int *, int), int v[], int n, int k) {
    double tiempo_inicio, tiempo_fin;
    double tiempo_con_ordenacion, tiempo_sin_ordenacion;

    // Medir tiempo total con ordenación
    tiempo_inicio = microsegundos();
    for (int rep = 0; rep < k; rep++) {
        (*init)(v, n);
        ordenarPorMonticulos(v, n);
    }
    tiempo_fin = microsegundos();
    tiempo_con_ordenacion = tiempo_fin - tiempo_inicio;

    // Medir tiempo sin ordenación (solo inicialización)
    tiempo_inicio = microsegundos();
    for (int rep = 0; rep < k; rep++) {
        (*init)(v, n);
    }
    tiempo_fin = microsegundos();
    tiempo_sin_ordenacion = tiempo_fin - tiempo_inicio;

    // Calcular tiempo promedio ajustado
    return (tiempo_con_ordenacion - tiempo_sin_ordenacion) / k;
}
void obtenerEtiquetasCotas(void (*init)(int *, int),
                           const char **cota_inf_label,
                           const char **cota_ajust_label,
                           const char **cota_sup_label) {
    if (init == ascendente) {
        *cota_inf_label = "[t(n)/(n^1.05)]";
        *cota_ajust_label = "[t(n)/(n^1.12)]";
        *cota_sup_label = "[t(n)/(n^1.21)]";
    } else if (init == aleatorio) {
        *cota_inf_label = "[t(n)/(n^1.05)]";
        *cota_ajust_label = "[t(n)/(n*log(n))]";
        *cota_sup_label = "[t(n)/(n^1.2)]";
    } else {
        *cota_inf_label = "[t(n)/(n^1.02)]";
        *cota_ajust_label = "[t(n)/(n^1.1)]";
        *cota_sup_label = "[t(n)/(n^1.3)]";
    }
}

void calcularCotas(void (*init)(int *, int), int n,
                   double *cota_inferior,
                   double *cota_ajustada,
                   double *cota_superior) {
    if (init == ascendente) {
        *cota_inferior = pow(n, 1.05);
        *cota_ajustada = pow(n, 1.13);
        *cota_superior = pow(n, 1.4);
    } else if (init == aleatorio) {
        *cota_inferior = pow(n, 1.05);
        *cota_ajustada = n * log(n);
        *cota_superior = pow(n, 1.2);
    } else {
        *cota_inferior = pow(n, 1.02);
        *cota_ajustada = pow(n, 1.1);
        *cota_superior = pow(n, 1.4);
    }
}

void calcularComplejidadOrdenacion(void (*init)(int *, int)) {
    int n, k = 10000;
    double tiempo_total, cota_inferior, cota_ajustada, cota_superior;
    int *v;
    const char *cota_inf_label, *cota_ajust_label, *cota_sup_label;
    printf("-----------------------------------------------------------------------\n");
    obtenerEtiquetasCotas(init, &cota_inf_label, &cota_ajust_label,
                          &cota_sup_label);
    printf("%13s%15s%23s%19s%20s\n", "n", "t(n)",
           cota_inf_label, cota_ajust_label, cota_sup_label);
    for (n = 2000; n <= TAM ; n *= 2) {
        v = (int *)malloc(n * sizeof(int));
        if (v == NULL) {
            printf("Error al asignar memoria\n");
            exit(EXIT_FAILURE);
        }
        calcularCotas(init, n, &cota_inferior, &cota_ajustada,
                      &cota_superior);
                      
        tiempo_total = medirTiempoOrdenacion(init, v, n);
        
        if (tiempo_total < 500) {
        
            tiempo_total = ajustarTiempoOrdenacion(init, v, n, k);
            
            printf("(*)");
            
        } else {
        
            printf("   ");
        }
        printf("%10d%15.4f%20.8f%20.8f%20.8f\n", n, tiempo_total,
               tiempo_total / cota_inferior,
               tiempo_total / cota_ajustada,
               tiempo_total / cota_superior);
        free(v);
    }
    printf("\n");
}


void testComplejidades() {
		mostrarPromptInicio("COMPLEJIDADES: ASC, DESC, ALET"); 

		printf("CASO (A): VECTOR ASCENDENTE\n");
		calcularComplejidadOrdenacion(ascendente);

		printf("CASO (B): VECTOR DESCENDENTE\n");
		calcularComplejidadOrdenacion(descendente);

		printf("CASO (C): VECTOR ALEATORIO\n");
		calcularComplejidadOrdenacion(aleatorio);
}

// ------------------------------------------------------------------------- //
//                                   Main                                    //
// ------------------------------------------------------------------------- //

int main() {
    inicializar_semilla();
    //test();
    tCompararMonticulos();
    //testComplejidades();
    return 0;
}




