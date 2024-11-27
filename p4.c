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


#define TAM 256000

#define INTERCAMBIAR(a, b) { int temp = (a); (a) = (b); (b) = temp; }


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

/* Función para obtener el tiempo actual en microsegundos */
double microsegundos() {
    struct timeval t;
    int result = gettimeofday(&t, NULL);
    if (result < 0) {
        return 0.0;
    }
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void imprimirMonticulo(const struct monticulo *m) {
    printf("Montículo: [ ");
    for (int i = 0; i <= m->ultimo; i++) {
        printf("%d ", m->vector[i]);
    }
    printf("]\n");
}

void mostrarPromptInicio() {
    printf("\n");
    printf("===============================================================\n");
    printf("       			     TEST           \n");
    printf("===============================================================\n");

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
//                          Funciones Auxiliares                             //
// ------------------------------------------------------------------------- //

// Devuelve el índice del padre
int padre(int i) {
    return (i - 1) / 2;
}

// Devuelve el índice del hijo izquierdo
int hijoIzquierdo(int i) {
    return 2 * i + 1;
}

// Devuelve el índice del hijo derecho
int hijoDerecho(int i) {
    return 2 * i + 2;
}




// ------------------------------------------------------------------------- //
//                          Funciones de Montículos                          //
// ------------------------------------------------------------------------- //

// Inicializa un montículo vacío
void iniMonticulo(pmonticulo m) {
    m->ultimo = -1;  // Índice inicial para un montículo vacío
}

void flotar(pmonticulo m, int i) {
    while (i > 0 && m->vector[padre(i)] > m->vector[i]) { // Comparar con el padre
        // Intercambiar con el padre
        INTERCAMBIAR(m->vector[padre(i)], m->vector[i]);
        i = padre(i); // Actualizar índice para subir al nivel del padre
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
        hijoIzq = hijoIzquierdo(i); 
        hijoDer = hijoDerecho(i);  
        j = i;                    

        // Si el hijo derecho es menor que el nodo actual, actualiza el índice
        if (hijoDer <= m->ultimo && m->vector[hijoDer] < m->vector[i]) {
            i = hijoDer;
        }

        // Si el hijo izquierdo es menor que el nodo actual (o menor que el derecho), actualiza el índice
        if (hijoIzq <= m->ultimo && m->vector[hijoIzq] <= m->vector[i]) {
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

    // Ajustar desde el último padre hasta la raíz
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

    mostrarPromptInicio(); 

    struct monticulo m;

    // Valores dados para el test
    int valores[] = {15, 13, 12, 8, 9, 5, 8, 7, 5, 3, 4, 5};
    int n = sizeof(valores) / sizeof(valores[0]);

    // Inicializar el montículo
    printf("Inicializando el montículo...\n");
    iniMonticulo(&m);
    printf("Montículo inicializado.\n");
    imprimirMonticulo(&m);

    // Insertar un valor en el montículo
    printf("\nInsertando el valor 10 en el montículo...\n");
    insertarMonticulo(&m, 10);
    imprimirMonticulo(&m);

    // Consultar el menor elemento del montículo
    printf("\nConsultando el menor elemento del montículo...\n");
    int menor = consultarMenor(&m);
    printf("El menor elemento es: %d (esperado: 10)\n", menor);

    // Quitar el menor elemento del montículo
    printf("\nQuitando el menor elemento del montículo...\n");
    quitarMenor(&m);
    imprimirMonticulo(&m);

    // Insertar todos los valores del array en el montículo
    printf("\nInsertando los valores del array en el montículo...\n");
    for (int i = 0; i < n; i++) {
        insertarMonticulo(&m, valores[i]);
    }
    imprimirMonticulo(&m);

    // Crear un nuevo montículo directamente desde los valores
    printf("\nCreando un nuevo montículo directamente desde los valores dados...\n");
    crearMonticulo(&m, valores, n);
    imprimirMonticulo(&m);

    // Consultar el menor elemento del montículo creado
    printf("\nConsultando el menor elemento del nuevo montículo...\n");
    menor = consultarMenor(&m);
    printf("El menor elemento es: %d (esperado: 3)\n", menor);
}

// ------------------------------------------------------------------------- //
//                          Funciones de Tiempos                             //
// ------------------------------------------------------------------------- //
void medirInsercionUnoAUno() {
    int n, k = 5000;
    double tiempo_inicio, tiempo_fin, tiempo_total;
    double tiempo_con_insercion, tiempo_sin_insercion;
    double tiempo_ajustada;
    int v[TAM];
    struct monticulo m;

    printf("\nDemostración empírica de complejidades de montículos\n");
    printf("-----------------------------------------------------------------------\n");
    printf("                Inserción de n elementos uno a uno (O(n log n))\n\n");
    printf("%10s%15s%25s\n", "n", "t(n)", "t(n)/(n log n)");

    for (n = 500; n <= TAM; n *= 2) {
        iniMonticulo(&m);
        aleatorio(v, n);

        // Medir tiempo de inserción
        tiempo_inicio = microsegundos();
        for (int i = 0; i < n; i++) {
            insertarMonticulo(&m, v[i]);
        }
        tiempo_fin = microsegundos();
        tiempo_total = tiempo_fin - tiempo_inicio;

        if (tiempo_total < 500) {
            int repeticiones = k;
            tiempo_total = 0.0;

            // Medir tiempo total con inserciones
            tiempo_inicio = microsegundos();
            for (int rep = 0; rep < repeticiones; rep++) {
                iniMonticulo(&m);
                aleatorio(v, n);
                for (int i = 0; i < n; i++) {
                    insertarMonticulo(&m, v[i]);
                }
            }
            tiempo_fin = microsegundos();
            tiempo_con_insercion = tiempo_fin - tiempo_inicio;

            // Medir tiempo sin inserciones (solo generación)
            tiempo_inicio = microsegundos();
            for (int rep = 0; rep < repeticiones; rep++) {
                iniMonticulo(&m);
                aleatorio(v, n);
            }
            tiempo_fin = microsegundos();
            tiempo_sin_insercion = tiempo_fin - tiempo_inicio;

            // Calcular tiempo promedio ajustado
            tiempo_total = (tiempo_con_insercion - tiempo_sin_insercion) / repeticiones;
            printf("(*)");
        } else {
            printf("   ");
        }

        tiempo_ajustada = tiempo_total / (n * log(n));
        printf("%8d%15.3f%25.8f\n", n, tiempo_total, tiempo_ajustada);
    }
}
void medirCreacionDesdeVector() {
    int n, k = 10000;
    double tiempo_inicio, tiempo_fin, tiempo_total, tiempo_generacion;
    double tiempo_ajustada;
    int v[TAM];
    struct monticulo m;

    printf("\n                Creación de montículo desde un vector (O(n))\n\n");
    printf("%10s%15s%20s\n", "n", "t(n)", "t(n)/n");

    for (n = 500; n <= TAM; n *= 2) {
        // Medir tiempo de generación del vector aleatorio
        tiempo_inicio = microsegundos();
        aleatorio(v, n);
        tiempo_fin = microsegundos();
        tiempo_generacion = tiempo_fin - tiempo_inicio;

        // Medir tiempo de creación del montículo
        tiempo_inicio = microsegundos();
        aleatorio(v, n);
        crearMonticulo(&m, v, n);
        tiempo_fin = microsegundos();
        tiempo_total = tiempo_fin - tiempo_inicio;

        // Restar el tiempo de generación
        tiempo_total -= tiempo_generacion;

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
                aleatorio(v, n);
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

        tiempo_ajustada = tiempo_total / n;
        printf("%8d%15.3f%25.8f\n", n, tiempo_total, tiempo_ajustada);
    }

    printf("-----------------------------------------------------------------------\n");
}

void tCompararMonticulos() {
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
    return tiempo_fin - tiempo_inicio;
}

double ajustarTiempoOrdenacion(void (*init)(int *, int), int v[], int n, int k) {
    double tiempo_inicio, tiempo_fin;
    double tiempo_con_ordenacion = 0.0, tiempo_sin_ordenacion = 0.0;

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

void calcularComplejidadOrdenacion(void (*init)(int *, int)) {
    int n, k = 1000;
    double tiempo_total, cota_inferior, cota_ajustada, cota_superior;
    int *v;

    printf("\nDemostración empírica de complejidad del algoritmo de ordenación"
           " por montículos\n");
    printf("-----------------------------------------------------------------------\n");
    printf("%10s%15s%20s%20s%20s\n", "n", "t(n)", "t(n)/n",
           "t(n)/nlogn", "t(n)/nlog^2n");

    for (n = 500; n <= TAM && tiempo_total < 500000; n *= 2) {
        v = (int *)malloc(n * sizeof(int));
        if (v == NULL) {
            printf("Error al asignar memoria\n");
            exit(EXIT_FAILURE);
        }

        // Calcular cotas teóricas
        cota_inferior = n;
        cota_ajustada = n * log(n);
        cota_superior = n * pow(log(n), 2);

        // Medir tiempo de ordenación
        tiempo_total = medirTiempoOrdenacion(init, v, n);

        if (tiempo_total < 500) {
            tiempo_total = ajustarTiempoOrdenacion(init, v, n, k);
            printf("(*)");
        } else {
            printf("   ");
        }

        // Calcular tiempos ajustados respecto a las cotas
        printf("%10d%15.3f%20.8f%20.8f%20.8f\n", n, tiempo_total,
               tiempo_total / cota_inferior,
               tiempo_total / cota_ajustada,
               tiempo_total / cota_superior);

        free(v);
    }
}


void testComplejidades() {
    printf("\nCaso (a): Vector ordenado en orden ascendente\n");
    calcularComplejidadOrdenacion(ascendente);

    printf("\nCaso (b): Vector ordenado en orden descendente\n");
    calcularComplejidadOrdenacion(descendente);

    printf("\nCaso (c): Vector desordenado aleatoriamente\n");
    calcularComplejidadOrdenacion(aleatorio);
}

// ------------------------------------------------------------------------- //
//                                   Main                                    //
// ------------------------------------------------------------------------- //

int main() {
    inicializar_semilla();
    // test();
    tCompararMonticulos();
    // testComplejidades();
    return 0;
}




