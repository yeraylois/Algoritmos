#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>

#define LONGITUD_CLAVE 30
#define LONGITUD_SINONIMOS 300
#define TAM_TABLA 38197
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))

typedef struct entrada_ {
    int ocupada;
    char clave[LONGITUD_CLAVE];
    char sinonimos[LONGITUD_SINONIMOS];
} entrada;

typedef struct {
    char clave[LONGITUD_CLAVE];
    char sinonimos[LONGITUD_SINONIMOS];
} item;

typedef entrada* tabla_cerrada;
typedef int pos;

/*
// Function prototypes
void inicializar_semilla();
double microsegundos();
unsigned int dispersionA(char *clave, int tamTabla);
unsigned int dispersionB(char *clave, int tamTabla);
unsigned int dispersionSecundaria(char *clave, int tamTabla);
unsigned int resol_colisiones_lineal(int pos_ini, int intento, char *clave);
unsigned int resol_colisiones_cuadratica(int pos_ini, int intento, char *clave);
unsigned int resol_colisiones_doble(int pos_ini, int intento, char *clave);
unsigned int ndispersion(char *clave, int tamTabla); // Declaración de la función ndispersion
void inicializar_cerrada(tabla_cerrada *diccionario, int tam);
void liberar_cerrada(tabla_cerrada diccionario);
int insertar_cerrada(char *clave, char *sinonimos, tabla_cerrada diccionario, int tam,
                    unsigned int (*dispersion)(char *, int),
                    unsigned int (*resol_colisiones)(int, int, char *));
pos buscar_cerrada(char *clave, tabla_cerrada diccionario, int tam, int *colisiones,
                   unsigned int (*dispersion)(char *, int),
                   unsigned int (*resol_colisiones)(int, int, char *));
void mostrar_cerrada(tabla_cerrada diccionario, int tam);
int leer_sinonimos(item datos[]);
void imprimir_resultados(int n, double t_n, double t_n_div_n_pow, double t_n_div_n, double t_n_div_n_logn, int es_promediado);
void medir_tiempos_busqueda(tabla_cerrada diccionario, int tam,
                            item datos[], int num_datos,
                            unsigned int (*dispersion)(char *, int),
                            unsigned int (*resol_colisiones)(int, int, char *),
                            char *nombre_dispersion, char *nombre_resol);

void insercion_medicion();
void test();

*/


// Inicializar semilla para generación aleatoria
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

// Funciones de dispersión
unsigned int dispersionA(char *clave, int tamTabla) {
    int i, n = MIN(8, (int)strlen(clave));
    unsigned int valor = clave[0];
    for (i = 1; i < n; i++)
        valor += clave[i];
    return valor % tamTabla;
}

unsigned int dispersionB(char *clave, int tamTabla) {
    int i, n = MIN(8, (int)strlen(clave));
    unsigned int valor = clave[0];
    for (i = 1; i < n; i++)
        valor = (valor << 5) + clave[i]; // Equivale a valor * 32 + clave[i]
    return valor % tamTabla;
}

// Función de dispersión secundaria para double hashing
unsigned int dispersionSecundaria(char *clave, int tamTabla) {
    unsigned int hash = dispersionA(clave, tamTabla);
    return 10007 - (hash % 10007);
}

// Función de dispersión de ejemplo para pruebas de clase
unsigned int ndispersion(char *clave, int tamTabla) {
    if (strcmp(clave, "ANA") == 0) return 7;
    if (strcmp(clave, "JOSE") == 0) return 7;
    if (strcmp(clave, "OLGA") == 0) return 7;
    return 6;
}

// Resolver colisiones: lineal
unsigned int resol_colisiones_lineal(int pos_ini, int intento, char *clave) {
    return (pos_ini + intento) % TAM_TABLA;
}

// Resolver colisiones: cuadratica
unsigned int resol_colisiones_cuadratica(int pos_ini, int intento, char *clave) {
    return (pos_ini + intento * intento) % TAM_TABLA;
}

// Resolver colisiones: doble hashing
unsigned int resol_colisiones_doble(int pos_ini, int intento, char *clave) {
    unsigned int h2 = dispersionSecundaria(clave, TAM_TABLA);
    if (h2 == 0) h2 = 1; // Evitar que h2 sea cero para evitar bucles infinitos
    return (pos_ini + intento * h2) % TAM_TABLA;
}

// Inicializar la tabla cerrada
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

// Liberar la tabla cerrada
void liberar_cerrada(tabla_cerrada diccionario) {
    free(diccionario);
}

int insertar_cerrada(char *clave, char *sinonimos, tabla_cerrada diccionario, int tam,
                    unsigned int (*dispersion)(char *, int),
                    unsigned int (*resol_colisiones)(int, int, char *)) {
    int colisiones = 0;
    unsigned int pos_ini = dispersion(clave, tam);
    unsigned int pos = pos_ini;
    int intento = 0;

    while (diccionario[pos].ocupada && strcmp(diccionario[pos].clave, clave) != 0 && intento < tam) {
        intento++;
        colisiones++;
        pos = resol_colisiones(pos_ini, intento, clave);
        pos %= tam; // Mantener la posición en el rango de la tabla
    }

    if (intento == tam) {
        return -1; // Tabla llena
    }

    if (!diccionario[pos].ocupada) {
        diccionario[pos].ocupada = 1;
        strcpy(diccionario[pos].clave, clave);
        strcpy(diccionario[pos].sinonimos, sinonimos);
    }

    return colisiones;
}

pos buscar_cerrada(char *clave, tabla_cerrada diccionario, int tam, 
				   int *colisiones, unsigned int (*dispersion)(char *, int),
                   unsigned int (*resol_colisiones)(int, int, char *)) {
                   
    unsigned int pos_ini = dispersion(clave, tam);
    unsigned int pos = pos_ini;
    int intento = 0;
    *colisiones = 0;

    while (diccionario[pos].ocupada 
    	   && strcmp(diccionario[pos].clave, clave) != 0 && intento < tam) {
        intento++;
        (*colisiones)++;
        pos = resol_colisiones(pos_ini, intento, clave) % tam;
    }

    if (diccionario[pos].ocupada && strcmp(diccionario[pos].clave, clave) == 0){
        return pos; // Clave encontrada
    } else {
        return -1; // Clave no encontrada
    }
}

// Mostrar la tabla cerrada
void mostrar_cerrada(tabla_cerrada diccionario, int tam) {
	printf("{\n");
    for (int i = 0; i < tam; i++) {
        if (diccionario[i].ocupada) {
        	if(i <= 9) printf(" %d- (%s)\n", i, diccionario[i].clave);
            else printf("%d- (%s)\n", i, diccionario[i].clave);
        } else {
            printf(" %d- \n", i);
        }
    }
    printf("}\n");
}


// Lectura del archivo 'sinonimos.txt'
int leer_sinonimos(item datos[]) {
    char c;
    int i, j;
    FILE *archivo;
    if ((archivo = fopen("sinonimos.txt", "r")) == NULL) {
        printf("Error al abrir 'sinonimos.txt'\n");
        return EXIT_FAILURE;
    }
    for (i = 0; fscanf(archivo, "%s", datos[i].clave) != EOF; i++) {
        if ((c = fgetc(archivo)) != '\t') {
            printf("Error al leer el tabulador en la línea %d\n", i + 1);
            fclose(archivo);
            return EXIT_FAILURE;
        }
        j = 0;
        while ((c = fgetc(archivo)) != '\n' && c != EOF) {
            if (j < LONGITUD_SINONIMOS - 1)
                datos[i].sinonimos[j++] = c;
        }
        datos[i].sinonimos[j] = '\0';
    }
    if (fclose(archivo) != 0) {
        printf("Error al cerrar el fichero\n");
        return EXIT_FAILURE;
    }
    return i;
}

// Función para imprimir los resultados en formato de tabla
void imprimir_resultados(int n, double t_n, double t_n_div_n_pow, 
						 double t_n_div_n, double t_n_div_n_logn, 
						 int es_promediado) {
						 
	if (es_promediado) {
	    printf("(*)   %10d %15.3f %15.7f %15.7f %15.7f\n", 
	    	   n, t_n, t_n_div_n_pow, t_n_div_n, t_n_div_n_logn);
	} else {
	    printf("      %10d %15.3f %15.7f %15.7f %15.7f\n", 
	    	   n, t_n, t_n_div_n_pow, t_n_div_n, t_n_div_n_logn);
	}
}
void medir_tiempos_busqueda(tabla_cerrada diccionario, int tam,
                            item datos[], int num_datos,
                            unsigned int (*dispersion)(char *, int),
                            unsigned int (*resol_colisiones)(int, int, char *),
                            char *nombre_dispersion, char *nombre_resol) {
    int n_busquedas[] = {125, 250, 500, 1000, 2000, 4000, 8000, 16000};
    int num_tests = sizeof(n_busquedas) / sizeof(n_busquedas[0]);

    printf("Buscando n elementos...\n\n");
    printf("      %10s %16s %15s %13s %19s\n", 
           "n", "t(n)(µs)", "t(n)/n^0.8", "t(n)/n", "t(n)/(n*log(n))");

    for (int i = 0; i < num_tests; i++) {
        int n = n_busquedas[i];
        if (n > num_datos) n = num_datos; // Asegurar que n no exceda el número de datos

        double t_start = microsegundos();
        for (int j = 0; j < n; j++) {
            int random_index = rand() % num_datos;
            int colisiones_busqueda = 0;
            buscar_cerrada(datos[random_index].clave, diccionario, tam, 
                           &colisiones_busqueda, dispersion, resol_colisiones);
        }
        double t_end = microsegundos();
        double t_n = t_end - t_start; // Tiempo en microsegundos
        int es_promediado = 0;

        if (t_n < 500) {
            es_promediado = 1;
            t_start = microsegundos();
            for (int rep = 0; rep < 1000; rep++) {
                for (int j = 0; j < n; j++) {
                    int random_index = rand() % num_datos;
                    int colisiones_busqueda = 0;
                    buscar_cerrada(datos[random_index].clave, diccionario, tam, 
                                   &colisiones_busqueda, dispersion, 
                                   resol_colisiones);
                }
            }
            t_end = microsegundos();
            t_n = (t_end - t_start) / 1000.0; 
        }

        double n_double = (double)n;
        double cota_sub = t_n / pow(n_double, 0.8);
        double cota_ajus = t_n / n_double;
        double cota_sobre = t_n / (n_double * log(n_double));

        imprimir_resultados(n, t_n, cota_sub, cota_ajus, cota_sobre, es_promediado);
    }
    printf("-------------------------------------------------------------"
           "-------------------\n");
}

// Función para insertar y medir colisiones y tiempos
void insercion_medicion() {
    item datos[19062];
    int num_datos = leer_sinonimos(datos);
    if (num_datos == EXIT_FAILURE) return;

    unsigned int (*funciones_dispersion[])(char *, int) = {dispersionA, dispersionB};
    char *nombres_dispersion[] = {"dispersion A", "dispersion B"};

    unsigned int (*funciones_resol[])(int, int, char *) = {resol_colisiones_lineal, resol_colisiones_cuadratica, resol_colisiones_doble};
    char *nombres_resol[] = {"lineal", "cuadratica", "doble"};

    for (int d = 0; d < 2; d++) {
        for (int r = 0; r < 3; r++) {
            tabla_cerrada diccionario;
            inicializar_cerrada(&diccionario, TAM_TABLA);

            int total_colisiones = 0;
            for (int i = 0; i < num_datos; i++) {
                int col = insertar_cerrada(datos[i].clave, datos[i].sinonimos, diccionario, TAM_TABLA,
                                           funciones_dispersion[d], funciones_resol[r]);
                if (col == -1) {
                    printf("Error: Tabla llena durante la inserción de '%s'.\n", datos[i].clave);
                    break;
                }
                total_colisiones += col;
            }

            printf("\n*** Dispersion cerrada %s con %s ***\n",nombres_resol[r], nombres_dispersion[d]);
            printf("Insertando %d elementos... Numero total de colisiones: %d\n", num_datos, total_colisiones);
            

            medir_tiempos_busqueda(diccionario, TAM_TABLA, datos, num_datos, funciones_dispersion[d], funciones_resol[r],
                                   nombres_dispersion[d], nombres_resol[r]);

            liberar_cerrada(diccionario);
        }
    }
}

// Función para realizar test con ejemplos de clase
void test() {
    tabla_cerrada diccionario;
    int tam = 11; // Tamaño de la tabla para pruebas
    int colisiones;
    char *claves[] = {"ANA", "LUIS", "JOSE", "OLGA", "ROSA", "IVAN", "CARLOS"}; 
    int num_claves = sizeof(claves) / sizeof(claves[0]);

    // Definir las funciones de resolución de colisiones y sus nombres
    unsigned int (*funciones_resol[])(int, int, char *) = {resol_colisiones_lineal, resol_colisiones_cuadratica, resol_colisiones_doble};
    char *nombres_resol[] = {"LINEAL", "CUADRATICA", "DOBLE"};

    for (int r = 0; r < 3; r++) {
        printf("\n***TABLA CERRADA %s***\n", nombres_resol[r]);
        int total_colisiones = 0;
        inicializar_cerrada(&diccionario, tam);

        // Inserción de claves
        for (int i = 0; i < num_claves - 1; i++) { // Excluye "CARLOS" de la inserción
            int col = insertar_cerrada(claves[i], "", diccionario, tam, ndispersion, funciones_resol[r]);
            if (col == -1) {
                printf("Error: Tabla llena durante la inserción de '%s'.\n", claves[i]);
                break;
            }
            total_colisiones += col;
        }
        
        mostrar_cerrada(diccionario, tam);
        printf("Número total de colisiones al insertar los elementos: %d\n", total_colisiones);
       

        // Búsqueda de todas las claves, incluyendo las no insertadas
        for (int i = 0; i < num_claves; i++) {
            pos p = buscar_cerrada(claves[i], diccionario, tam, &colisiones, ndispersion, funciones_resol[r]);
            if (p != -1) {
                printf("Al buscar: %s, encuentro: %s, colisiones: %d\n", claves[i], diccionario[p].clave, colisiones);
            } else {
                printf("No encuentro: '%s', colisiones: %d\n", claves[i], colisiones);
            }
        }

        // Liberar memoria
        liberar_cerrada(diccionario);
    }
}



int main() {
    inicializar_semilla();
    test();
    insercion_medicion();
    return 0;
}

