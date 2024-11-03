#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>

#define LONGITUD_CLAVE 30
#define LONGITUD_SINONIMOS 300
#define TAM_TABLA 38197
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))

typedef struct entrada_ {
    int ocupada;
    char clave[LONGITUD_CLAVE];
    char sinonimos[LONGITUD_SINONIMOS];
} entrada;

typedef entrada* tabla_cerrada;
typedef int pos;

void inicializar_semilla() {
    srand(time(NULL));
}

// Función para obtener el tiempo actual en microsegundos
double microsegundos() {
  struct timeval t;
  if (gettimeofday(&t, NULL) < 0)
    return 0.0;
  return (t.tv_usec + t.tv_sec * 1000000.0);
}

unsigned int dispersionA(char *clave, int tamTabla) {
    int i, n = MIN(8, strlen(clave));
    unsigned int valor = clave[0];
    for (i = 1; i < n; i++)
        valor += clave[i];
    return valor % tamTabla;
}

unsigned int dispersionB(char *clave, int tamTabla) {
    int i, n = MIN(8, strlen(clave));
    unsigned int valor = clave[0];
    for (i = 1; i < n; i++)
        valor = (valor << 5) + clave[i]; // Equivale a valor * 32 + clave[i]
    return valor % tamTabla;
}

unsigned int ndispersion(char *clave, int tamTabla) {
    if (strcmp(clave, "ANA") == 0) return 7;
    if (strcmp(clave, "JOSE") == 0) return 7;
    if (strcmp(clave, "OLGA") == 0) return 7;
    return 6;
}

unsigned int exploracion_lineal(int pos_ini, int num_intento) {
    return pos_ini + num_intento;
}

unsigned int exploracion_cuadratica(int pos_ini, int num_intento) {
    return pos_ini + num_intento * num_intento;
}

unsigned int exploracion_doble(int pos_ini, int num_intento) {
    int h2 = 10007 - (pos_ini % 10007);
    return pos_ini + num_intento * h2;
}

void inicializar_cerrada(tabla_cerrada *diccionario, int tam) {
    *diccionario = malloc(tam * sizeof(entrada));
    if (*diccionario == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para el diccionario.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < tam; i++) {
        (*diccionario)[i].ocupada = 0;
        (*diccionario)[i].clave[0] = '\0';
        (*diccionario)[i].sinonimos[0] = '\0';
    }
}

void liberar_cerrada(tabla_cerrada diccionario) {
    free(diccionario);
}

int insertar_cerrada(char *clave, char *sinonimos, tabla_cerrada *diccionario, int tam,
                     unsigned int (*dispersion)(char *, int),
                     unsigned int (*resol_colisiones)(int, int)) {
    int colisiones = 0;
    int pos_ini = dispersion(clave, tam);
    int pos = pos_ini % tam;
    int intento = 0;
    while ((*diccionario)[pos].ocupada && intento < tam) {
        intento++;
        colisiones++;
        pos = resol_colisiones(pos_ini, intento) % tam;
    }
    if (intento == tam) {
        return -1; // Tabla llena
    }
    (*diccionario)[pos].ocupada = 1;
    strcpy((*diccionario)[pos].clave, clave);
    strcpy((*diccionario)[pos].sinonimos, sinonimos);
    return colisiones;
}

pos buscar_cerrada(char *clave, tabla_cerrada diccionario, int tam, int *colisiones,
                   unsigned int (*dispersion)(char *, int),
                   unsigned int (*resol_colisiones)(int, int)) {
    int pos_ini = dispersion(clave, tam);
    int pos = pos_ini % tam;
    int intento = 0;
    *colisiones = 0;
    while (diccionario[pos].ocupada && strcmp(diccionario[pos].clave, clave) != 0 && intento < tam) {
        intento++;
        (*colisiones)++;
        pos = resol_colisiones(pos_ini, intento) % tam;
    }
    if (diccionario[pos].ocupada && strcmp(diccionario[pos].clave, clave) == 0) {
        return pos;
    } else {
        return -1; // No encontrado
    }
}

void mostrar_cerrada(tabla_cerrada diccionario, int tam) {
    for (int i = 0; i < tam; i++) {
        if (diccionario[i].ocupada) {
            printf("%d - (%s %s)\n", i, diccionario[i].clave, diccionario[i].sinonimos);
        } else {
            printf("%d - \n", i);
        }
    }
}

int leer_sinonimos(entrada datos[]) {
    char c;
    int i, j;
    FILE *archivo;
    if ((archivo = fopen("sinonimos.txt", "r")) == NULL) {
        printf("Error al abrir 'sinonimos.txt'\n");
        return EXIT_FAILURE;
    }
    for (i = 0; fscanf(archivo, "%s", datos[i].clave) != EOF; i++) {
        if ((c = fgetc(archivo)) != '\t') {
            printf("Error al leer el tabulador\n");
            return EXIT_FAILURE;
        }
        for (j = 0; (c = fgetc(archivo)) != '\n'; j++) {
            if (j < LONGITUD_SINONIMOS - 1)
                datos[i].sinonimos[j] = c;
        }
        datos[i].sinonimos[MIN(j, LONGITUD_SINONIMOS - 1)] = '\0';
    }
    if (fclose(archivo) != 0) {
        printf("Error al cerrar el fichero\n");
        return EXIT_FAILURE;
    }
    return i;
}

void medir_tiempos_busqueda(tabla_cerrada diccionario, int tam,
                            entrada datos[], int num_datos,
                            unsigned int (*dispersion)(char *, int),
                            unsigned int (*resol_colisiones)(int, int),
                            char *nombre_dispersion, char *nombre_resol) {
    double tiempo_inicio, tiempo_fin, tiempo;
    int colisiones, n_busquedas = 1000; // Número de búsquedas para la media si es necesario.

    printf("\n*** Dispersion cerrada %s con %s***\n", nombre_resol, nombre_dispersion);
    printf("Insertando %d elementos... Numero total de colisiones: \n", num_datos);
    printf("Buscando n elementos...\n");
    printf("      %10s %17s %14s %14s %16s\n", "n", "t(n)(µs)", "t(n)/n^0.8", "t(n)/n", "t(n)/n*log(n)");

    for (int n = 125; n <= 16000; n *= 2) {
        tiempo_inicio = microsegundos();
        for (int i = 0; i < n; i++) {
            int random_index = rand() % num_datos; // Genera un índice aleatorio
            buscar_cerrada(datos[random_index].clave, diccionario, tam, &colisiones, dispersion, resol_colisiones);
        }
        tiempo_fin = microsegundos();
        tiempo = (tiempo_fin - tiempo_inicio);

        int es_promediado = 0;
                pos resultado_busqueda = 0; // Para evitar optimización

        if (tiempo < 500) {
            // Repetir la medición 1000 veces y calcular la media
            es_promediado = 1;
            tiempo_inicio = microsegundos();
            for (int rep = 0; rep < n_busquedas; rep++) {
                for (int i = 0; i < n; i++) {
                    int random_index = rand() % num_datos;
                    resultado_busqueda += buscar_cerrada(datos[random_index].clave, diccionario, tam, &colisiones, dispersion, resol_colisiones);

                }
            }
            tiempo_fin = microsegundos();
            tiempo = (tiempo_fin - tiempo_inicio) / (n_busquedas * n);
        } else {
            tiempo /= n; // Normalizar el tiempo por el número de búsquedas
        }

        double n_double = (double)n;
        double cota_sub = tiempo / pow(n_double, 0.8);
        double cota_ajus = tiempo / n_double;
        double cota_sobre = tiempo / (n_double * log(n_double));

        if (es_promediado) {
            printf("(*)   %10d %15.3f %15.7f %15.7f %15.7f\n", n, tiempo, cota_sub, cota_ajus, cota_sobre);
        } else {
            printf("      %10d %15.3f %15.7f %15.7f %15.7f\n", n, tiempo, cota_sub, cota_ajus, cota_sobre);
        }
        
        if (resultado_busqueda == -1) {
            printf("Resultado acumulado: %d\n", resultado_busqueda);
        }
    }
}

void insertar_y_medicion_completa() {
    entrada datos[19062]; 
    int num_datos = leer_sinonimos(datos);
    if (num_datos == EXIT_FAILURE) return;

    unsigned int (*funciones_dispersion[])(char *, int) = {dispersionA, dispersionB};
    unsigned int (*funciones_resol[])(int, int) = {exploracion_lineal, exploracion_cuadratica, exploracion_doble};
    char *nombres_dispersion[] = {"dispersionA", "dispersionB"};
    char *nombres_resol[] = {"lineal", "cuadratica", "doble"};

    for (int d = 0; d < 2; d++) {
        for (int r = 0; r < 3; r++) {
            int total_colisiones = 0;
            tabla_cerrada diccionario;
            inicializar_cerrada(&diccionario, TAM_TABLA);

            for (int i = 0; i < num_datos; i++) {
                total_colisiones += insertar_cerrada(datos[i].clave, datos[i].sinonimos, &diccionario, TAM_TABLA, funciones_dispersion[d], funciones_resol[r]);
            }

            printf("Total de colisiones usando %s y exploración %s: %d\n", nombres_dispersion[d], nombres_resol[r], total_colisiones);
            medir_tiempos_busqueda(diccionario, TAM_TABLA, datos, num_datos, funciones_dispersion[d], funciones_resol[r], nombres_dispersion[d], nombres_resol[r]);
            liberar_cerrada(diccionario);
        }
    }
}
void pruebas_ejemplo_clase() {
    tabla_cerrada diccionario;
    int tam = 11;
    int colisiones;
    char *claves[] = {"ANA", "LUIS", "JOSE", "OLGA", "ROSA", "IVAN"};
    int num_claves = 6;

    // Array de funciones de resolución de colisiones y sus nombres
    unsigned int (*funciones_resol[])(int, int) = {exploracion_lineal, exploracion_cuadratica, exploracion_doble};
    char *nombres_resol[] = {"LINEAL", "CUADRATICA", "DOBLE"};

    // Recorre cada función de resolución y ejecuta las pruebas
    for (int r = 0; r < 3; r++) {
        printf("\n***TABLA CERRADA %s***\n", nombres_resol[r]);
        int total_colisiones = 0;
        inicializar_cerrada(&diccionario, tam);

        // Inserción de claves
        for (int i = 0; i < num_claves; i++) {
            int col = insertar_cerrada(claves[i], "", &diccionario, tam, ndispersion, funciones_resol[r]);
            total_colisiones += col;
            printf("Insertando '%s', colisiones: %d\n", claves[i], col);
        }
        printf("Número total de colisiones al insertar los elementos: %d\n", total_colisiones);

        // Mostrar la tabla
        mostrar_cerrada(diccionario, tam);

        // Búsqueda de claves
        for (int i = 0; i < num_claves; i++) {
            pos p = buscar_cerrada(claves[i], diccionario, tam, &colisiones, ndispersion, funciones_resol[r]);
            if (p != -1) {
                printf("Al buscar: %s, encuentro: %s, colisiones: %d\n", claves[i], diccionario[p].clave, colisiones);
            } else {
                printf("Clave '%s' no encontrada.\n", claves[i]);
            }
        }

        // Búsqueda de una clave que no existe
        pos p = buscar_cerrada("CARLOS", diccionario, tam, &colisiones, ndispersion, funciones_resol[r]);
        if (p != -1) {
            printf("Al buscar: CARLOS, encuentro: %s, colisiones: %d\n", diccionario[p].clave, colisiones);
        } else {
            printf("No encuentro: CARLOS, colisiones: %d\n", colisiones);
        }

        liberar_cerrada(diccionario);
    }
}

int main() {
    inicializar_semilla();
    pruebas_ejemplo_clase();
    insertar_y_medicion_completa();
    return 0;
}
