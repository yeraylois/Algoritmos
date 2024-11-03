#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>

#define LONGITUD_CLAVE 30
#define LONGITUD_SINONIMOS 300
#define TAM_TABLA 38197
#define MAX_DATOS 19062
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))

// ------------------------------------------------------------------------- //
//                            Definiciones de Tipos                          //
// ------------------------------------------------------------------------- //

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

void print_division(){
	printf("\n-------------------------------------------------------------"
           "----------------------\n");
}

// ------------------------------------------------------------------------- //
//                          Funciones de Dispersión                          //
// ------------------------------------------------------------------------- //

unsigned int dispersionA(char *clave, int tamTabla) {
    int i, n;
    unsigned int valor;
    n = MIN(8, (int)strlen(clave));
    valor = clave[0];
    for (i = 1; i < n; i++) {
        valor += clave[i];
    }
    return valor % tamTabla;
}

unsigned int dispersionB(char *clave, int tamTabla) {
    int i, n;
    unsigned int valor;
    n = MIN(8, (int)strlen(clave));
    valor = clave[0];
    for (i = 1; i < n; i++) {
        valor = (valor << 5) + clave[i]; 
    }
    return valor % tamTabla;
}

/* Función de dispersión para pruebas */
unsigned int ndispersion(char *clave, int tamTabla) {
    if (strcmp(clave, "ANA") == 0) return 7;
    if (strcmp(clave, "JOSE") == 0) return 7;
    if (strcmp(clave, "OLGA") == 0) return 7;
    return 6;
}

/* Función de dispersión secundaria para doble hashing */
unsigned int dispersionSecundaria(char *clave, int tamTabla, int pos_ini) {
   
    unsigned int h2 = 10007 - (pos_ini % 10007);
    return h2;
}

// ------------------------------------------------------------------------- //
//                 Funciones de Resolución de Colisiones                     //
// ------------------------------------------------------------------------- //

unsigned int resol_colisiones_lineal(int pos_ini, int num_intento) {
    return num_intento; // Incremento lineal simple
}

unsigned int resol_colisiones_cuadratica(int pos_ini, int num_intento) {
    return num_intento * num_intento; // Incremento cuadrático
}

unsigned int resol_colisiones_doble(int pos_ini, int num_intento) {
    return 0; 
}

unsigned int resol_colisiones_doble_con_clave(int pos_ini, int num_intento, 
		 char *clave, int tamTabla) {
		 
    unsigned int h2 = dispersionSecundaria(clave, tamTabla, pos_ini);
    return num_intento * h2;
}

// ------------------------------------------------------------------------- //
//                Funciones de Gestión de la Tabla Cerrada                   //
// ------------------------------------------------------------------------- //

void inicializar_cerrada(tabla_cerrada *diccionario, int tam) {
    int i;
    *diccionario = (entrada *)malloc(tam * sizeof(entrada));
    if (*diccionario == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para el diccionario.\n");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < tam; i++) {
        (*diccionario)[i].ocupada = 0;
        (*diccionario)[i].clave[0] = '\0';
        (*diccionario)[i].sinonimos[0] = '\0';
    }
}

void liberar_cerrada(tabla_cerrada diccionario) {
    free(diccionario);
}

pos buscar_cerrada(char *clave, tabla_cerrada diccionario, int tam, 
				   int *colisiones, unsigned int (*dispersion)(char *, int),
                   unsigned int (*resol_colisiones)(int pos_ini, int num_intento)) {
                   
    int intento = 0;
    unsigned int pos_ini = dispersion(clave, tam);
    unsigned int pos = pos_ini;
    *colisiones = 0;

    // Recorre mientras la posición esté ocupada y no encuentre la clave
    while (diccionario[pos].ocupada 
    	   && strcmp(diccionario[pos].clave, clave) != 0 && intento < tam) {
    	   
        intento++;
        (*colisiones)++;
        
        if (resol_colisiones == resol_colisiones_lineal 
        	|| resol_colisiones == resol_colisiones_cuadratica) {
        	
            pos = (pos_ini + resol_colisiones(pos_ini, intento)) % tam;
            
        } else { /* CASO: resol_colisiones_doble */
            pos = (pos_ini + resol_colisiones_doble_con_clave(pos_ini, intento, clave, tam)) % tam;
        }
    }

    if (diccionario[pos].ocupada && strcmp(diccionario[pos].clave, clave) == 0) {
        return pos; // Clave encontrada
    } else if (!diccionario[pos].ocupada) {
        return pos; // Posición vacía
    } else {
        return -1; // No encontrado y tabla llena
    }
}

int insertar_cerrada(char *clave, char *sinonimos, tabla_cerrada *diccionario, 
					 int tam, unsigned int (*dispersion)(char *, int),
                     unsigned int (*resol_colisiones)(int pos_ini, int num_intento)) {
                     
    int colisiones = 0;
    pos posicion = buscar_cerrada(clave, *diccionario, tam, &colisiones, 
    							  dispersion, resol_colisiones);

    if (posicion == -1) {
        printf("Error: No se pudo insertar la clave '%s', tabla llena.\n", clave);
        return -1;
    }

    // Verifica si la posición encontrada es buena para insertar
    if (!(*diccionario)[posicion].ocupada 
    	|| strcmp((*diccionario)[posicion].clave, clave) == 0) {
    	
        (*diccionario)[posicion].ocupada = 1;
        strcpy((*diccionario)[posicion].clave, clave);
        strcpy((*diccionario)[posicion].sinonimos, sinonimos);
        return colisiones;
        
    }

    // Si no se pudo insertar, retorna -1
    return -1;
}

void mostrar_cerrada(tabla_cerrada diccionario, int tam) {
    int i;
    printf("{\n");
    for (i = 0; i < tam; i++) {
        if (diccionario[i].ocupada) {
            if (i <= 9)
                printf(" %d- (%s)\n", i, diccionario[i].clave);
            else
                printf("%d- (%s)\n", i, diccionario[i].clave);
        } else {
            if (i <= 9)
                printf(" %d- \n", i);
            else
                printf("%d- \n", i);
        }
    }
    printf("}\n");
}

// ------------------------------------------------------------------------- //
//                      Funciones de Lectura de Datos                        //
// ------------------------------------------------------------------------- //

int leer_sinonimos(item datos[]) {
    char c;
    int i, j;
    FILE *archivo;
    archivo = fopen("sinonimos.txt", "r");
    if (archivo == NULL) {
        printf("Error al abrir 'sinonimos.txt'\n");
        return EXIT_FAILURE;
    }
    for (i = 0; fscanf(archivo, "%s", datos[i].clave) != EOF && i < MAX_DATOS; i++) {
        c = fgetc(archivo);
        if (c != '\t') {
            printf("Error al leer el tabulador en la línea %d\n", i + 1);
            fclose(archivo);
            return EXIT_FAILURE;
        }
        j = 0;
        while ((c = fgetc(archivo)) != '\n' && c != EOF) {
            if (j < LONGITUD_SINONIMOS - 1) {
                datos[i].sinonimos[j++] = c;
            }
        }
        datos[i].sinonimos[j] = '\0';
    }
    if (fclose(archivo) != 0) {
        printf("Error al cerrar el fichero\n");
        return EXIT_FAILURE;
    }
    return i;
}

// ------------------------------------------------------------------------- //
//                    Funciones de Medición de Tiempos                       //
// ------------------------------------------------------------------------- //

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
                            unsigned int (*resol_colisiones)(int pos_ini, 
                            int num_intento), char *nombre_dispersion, 
                            char *nombre_resol) {
                            
    int n_busquedas[] = {125, 250, 500, 1000, 2000, 4000, 8000, 16000};
    int num_tests = sizeof(n_busquedas) / sizeof(n_busquedas[0]);
    int i, j, n, random_index, colisiones_busqueda, es_promediado;
    double t_start, t_end, t_n, n_double, cota_sub, cota_ajus, cota_sobre;
    printf("Buscando n elementos...\n\n");
    printf("      %10s %16s %15s %13s %19s\n",
           "n", "t(n)(µs)", "t(n)/n^0.8", "t(n)/n", "t(n)/(n*log(n))");
    for (i = 0; i < num_tests; i++) {
        n = n_busquedas[i];
        if (n > num_datos) n = num_datos;
        t_start = microsegundos();
        for (j = 0; j < n; j++) {
            random_index = rand() % num_datos;
            colisiones_busqueda = 0;
            buscar_cerrada(datos[random_index].clave, diccionario, tam,
                           &colisiones_busqueda, dispersion, resol_colisiones);
        }
        t_end = microsegundos();
        t_n = t_end - t_start;
        es_promediado = 0;
        if (t_n < 500) {
            int rep;
            es_promediado = 1;
            t_start = microsegundos();
            for (rep = 0; rep < 1000; rep++) {
                for (j = 0; j < n; j++) {
                    random_index = rand() % num_datos;
                    colisiones_busqueda = 0;
                    buscar_cerrada(datos[random_index].clave, diccionario, tam,
                                   &colisiones_busqueda, dispersion, 
                                   resol_colisiones);
                }
            }
            t_end = microsegundos();
            t_n = (t_end - t_start) / 1000.0;
        }
        n_double = (double)n;
        cota_sub = t_n / pow(n_double, 0.8);
        cota_ajus = t_n / n_double;
        cota_sobre = t_n / (n_double * log(n_double));
        imprimir_resultados(n, t_n, cota_sub, cota_ajus, cota_sobre, 
        					es_promediado);
    }
    
}

// ------------------------------------------------------------------------- //
//                           Funciones a Evaluar                             //
// ------------------------------------------------------------------------- //


void insercion_medicion() {
    item datos[MAX_DATOS];
    int num_datos, d, r, i, col, total_colisiones;
    tabla_cerrada diccionario;
    unsigned int (*funciones_dispersion[])(char *, int) = {dispersionA, 
    													   dispersionB};
    													   
    char *nombres_dispersion[] = {"dispersion A", "dispersion B"};
    unsigned int (*funciones_resol[])(int, int) = {resol_colisiones_lineal,
                                                   resol_colisiones_cuadratica,
                                                   resol_colisiones_doble};
    
    char *nombres_resol[] = {"lineal", "cuadratica", "doble"};
    num_datos = leer_sinonimos(datos);
    if (num_datos == EXIT_FAILURE) return;
    
    print_division(); 
    
    for (d = 0; d < 2; d++) {
        for (r = 0; r < 3; r++) {
            inicializar_cerrada(&diccionario, TAM_TABLA);
            total_colisiones = 0;
            for (i = 0; i < num_datos; i++) {
                col = insertar_cerrada(datos[i].clave, datos[i].sinonimos, 
                					   &diccionario, TAM_TABLA, 
                					   funciones_dispersion[d], 
                					   funciones_resol[r]);
                if (col == -1) {
                    printf("Error: Tabla llena durante la inserción de '%s'.\n",
                           datos[i].clave);
                    break;
                }
                total_colisiones += col;
            }
            printf("\n*** Dispersión cerrada %s con %s ***\n",
                   nombres_resol[r], nombres_dispersion[d]);
            printf("Insertando %d elementos... Número total de colisiones: %d\n",
                   num_datos, total_colisiones);
            medir_tiempos_busqueda(diccionario, TAM_TABLA, datos, num_datos,
                                   funciones_dispersion[d], funciones_resol[r],
                                   nombres_dispersion[d], nombres_resol[r]);
            liberar_cerrada(diccionario);
            
        }
        print_division();
    }
}

void test() {
    tabla_cerrada diccionario;
    int tam, colisiones, i, total_colisiones, r;
    pos p;
    char *claves[] = {"ANA", "LUIS", "JOSE", "OLGA", "ROSA", "IVAN", "CARLOS"};
    int num_claves = sizeof(claves) / sizeof(claves[0]);
    unsigned int (*funciones_resol[])(int, int) = {resol_colisiones_lineal,
                                                   resol_colisiones_cuadratica,
                                                   resol_colisiones_doble};
    char *nombres_resol[] = {"LINEAL", "CUADRATICA", "DOBLE"};
    tam = 11; /* Tamaño de la tabla para pruebas */
    for (r = 0; r < 3; r++) {
        printf("\n*** TABLA CERRADA %s ***\n", nombres_resol[r]);
        total_colisiones = 0;
        inicializar_cerrada(&diccionario, tam);
        for (i = 0; i < num_claves - 1; i++) { /* Se excluye "CARLOS" al insertar */
            colisiones = insertar_cerrada(claves[i], "", &diccionario, tam,
                                          ndispersion, funciones_resol[r]);
            if (colisiones == -1) {
                printf("Error: Tabla llena durante la inserción de '%s'.\n", 
                	   claves[i]);
                break;
            }
            total_colisiones += colisiones;
        }
        mostrar_cerrada(diccionario, tam);
        printf("Número total de colisiones al insertar los elementos: %d\n", 
        	   total_colisiones);
        	   
        for (i = 0; i < num_claves; i++) {
            colisiones = 0;
            p = buscar_cerrada(claves[i], diccionario, tam, &colisiones,
                               ndispersion, funciones_resol[r]);
            
            if (p != -1 && diccionario[p].ocupada 
            	&& strcmp(diccionario[p].clave, claves[i]) == 0) {
            	
                printf("Al buscar: %s, encuentro: %s, colisiones: %d\n",
                       claves[i], diccionario[p].clave, colisiones);
                       
            } else {
                printf("No encuentro: '%s', colisiones: %d\n", 
                	   claves[i], colisiones);
            }
        }
        liberar_cerrada(diccionario);
    }
}

// ------------------------------------------------------------------------- //
//                             	 FUNCION MAIN                                //
// ------------------------------------------------------------------------- //

int main() {
    inicializar_semilla();
    test();
    insercion_medicion();
    return 0;
}
