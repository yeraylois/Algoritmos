/**
* Yeray Lois Sánchez  --> yeray.lois@udc.es
* Anxo Galdo Blasco   --> anxo.galdo.blasco@udc.es
* Sofía Oubiña Falcon --> sofia.oubina.falcon@udc.es
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

#define MAX_TAM 32000 // Constante para el tamaño máximo del array
#define MAX_TAM_RAP 512000 //Constante para extender tamaño en ord_rap
// ------------------------------------------------------------------------- //
//                          Funciones de Utilidades                          //
// ------------------------------------------------------------------------- //

// Función para obtener el tiempo actual en microsegundos
double microsegundos() {
  struct timeval t;
  if (gettimeofday(&t, NULL) < 0)
    return 0.0;
  return (t.tv_usec + t.tv_sec * 1000000.0);
}

// Inicializa la semilla del generador de números aleatorios
void inicializar_semilla() {
  srand(time(NULL));
}

// Imprime un array de enteros de tamaño n
void imprimir_vector(int v[], int n) {
  int i;
  for (i = 0; i < n; i++) {
    if (i != 0)
      printf(", ");
    printf("%d", v[i]);
  }
  printf("\n");
}

// Verifica si el array está ordenado de forma ascendente
int esta_ordenado(int v[], int n) {
  int i;
  for (i = 1; i < n; i++) {
    if (v[i] < v[i - 1])
      return 0; // No ordenado
  }
  return 1; // Ordenado
}

// ------------------------------------------------------------------------- //
//                            Funciones de Arrays                            //
// ------------------------------------------------------------------------- //

// Llena el array con números aleatorios en el rango [-n, n]
void aleatorio(int v[], int n) {
  int i, m;
  m = 2 * n + 1;
  for (i = 0; i < n; i++) {
    v[i] = (rand() % m) - n;
  }
}

// Llena el array de forma ascendente, desde 0 hasta n-1
void ascendente(int v[], int n) {
  int i;
  for (i = 0; i < n; i++) {
    v[i] = i;
  }
}

// Llena el array de forma descendente, desde n-1 hasta 0
void descendente(int v[], int n) {
  int i;
  for (i = 0; i < n; i++)
    v[i] = n - i - 1;
}

// ------------------------------------------------------------------------- //
//                         Algortimos de Ordenación                          //
// ------------------------------------------------------------------------- //

void ord_ins(int v[], int n) {
  int i, j, x;
  for (i = 1; i < n; i++) {
    x = v[i];
    j = i - 1;
    while (j >= 0 && v[j] > x) {
      v[j + 1] = v[j]; 
      j--;
    }
    v[j + 1] = x; 
  }
}

// Intercambia los valores de dos posiciones en un array
void intercambiar(int v[], int i, int j) {
  int aux;
  aux = v[i];
  v[i] = v[j];
  v[j] = aux;
}

void ord_rap_aux(int v[], int iz, int dr) {
  int i, j, x, pivote;

  if (iz < dr) {
    x = (iz + dr) / 2; 
    pivote = v[x];
    intercambiar(v, iz, x); 
    i = iz + 1; 
    j = dr;     

    while (i <= j) {
      while (i <= dr && v[i] < pivote)
        i++;
      while (v[j] > pivote)
        j--;
      if (i <= j) {
        intercambiar(v, i, j);
        i++;
        j--;
      }
    }
    intercambiar(v, iz, j);
    ord_rap_aux(v, iz, j - 1);  
    ord_rap_aux(v, j + 1, dr);  
  }
}

void ord_rap(int v[], int n) {
  ord_rap_aux(v, 0, n - 1); 
}

// ------------------------------------------------------------------------- //
//                            Funciones de Test                              //
// ------------------------------------------------------------------------- //
// Función para test ord_ins
void test_ordenacion_insercion(int v[], int n, 
			       void (*inicializacion)(int[], int), 
			       char* tipo_inicializacion) {
  printf("\nOrdenacion por insercion con inicializacion %s\n", 
  	tipo_inicializacion);
  	
  inicializacion(v, n);
  imprimir_vector(v, n);
  printf("ordenado? %d\n", esta_ordenado(v, n));
  
  if(esta_ordenado(v, n) == 0){
  	printf("ordenando...\n");
  	ord_ins(v, n);
  	imprimir_vector(v, n);
  	printf("ordenado? %d\n", esta_ordenado(v, n));
  }
}

// Función para test ord_rap
void test_ordenacion_rapida(int v[], int n, void (*inicializacion)(int[], int), 
			    char* tipo_inicializacion) {
  printf("\nOrdenacion rapida con inicializacion %s\n", tipo_inicializacion);
  inicializacion(v, n);
  imprimir_vector(v, n);
  printf("ordenado? %d\n", esta_ordenado(v, n));

  if(esta_ordenado(v, n) == 0){
  	printf("ordenando...\n");
  	ord_rap(v, n);
  	imprimir_vector(v, n);
  	printf("ordenado? %d\n", esta_ordenado(v, n));
  }
}

// Función de test()
void test() {
  int n = 17;
  int v[n];

  test_ordenacion_insercion(v, n, aleatorio, "aleatoria");
  test_ordenacion_insercion(v, n, descendente, "descendente");
  test_ordenacion_insercion(v, n, ascendente, "ascendente");

  printf("------------------------------------------------------------\n");

  test_ordenacion_rapida(v, n, aleatorio, "aleatoria");
  test_ordenacion_rapida(v, n, descendente, "descendente");
  test_ordenacion_rapida(v, n, ascendente, "ascendente");
}

// ------------------------------------------------------------------------- //
//                         Función de Medir Tiempos                          //
// ------------------------------------------------------------------------- //

// Mide el tiempo de ejecución de un algoritmo de ordenación
double medir_tiempo(void (*algoritmo)(int[], int),
                    void (*inicializar)(int[], int), int v[], int n,
                    int *tiempo_pequeno) {
  double t_inicio, t_fin, t_total;
  int iteraciones, i;
  *tiempo_pequeno = 0; //Check para tiempo pequeño
  iteraciones = 1;

  // Medición inicial
  t_inicio = microsegundos();
  algoritmo(v, n); // Ejecuta el algoritmo
  t_fin = microsegundos();
  t_total = t_fin - t_inicio;

  
  if (t_total < 500) { //Tiempo Menor a 500 microsegundos
    *tiempo_pequeno = 1;
    iteraciones = 10000;  // Numero de Iteraciones
    t_inicio = microsegundos();
    for (i = 0; i < iteraciones; i++) {
      inicializar(v, n); // Reinicializa el array antes de cada ejecución
      algoritmo(v, n);
    }
    t_fin = microsegundos();
    t_total = (t_fin - t_inicio) / iteraciones; // Promedia el tiempo
  }

  return t_total;
}

// ------------------------------------------------------------------------- //
//                       Cálculo de Cotas y Resultados                       //
// ------------------------------------------------------------------------- //

// Imprime el encabezado de la tabla de resultados
void imprimir_encabezado(char *cota_sub, char *cota_ajus, char *cota_sobre) {
  printf("%-5s %10s %17s %14s %14s %16s\n",
         "", "n", "t(n)(µs)", cota_sub, cota_ajus, cota_sobre);
}

// Calcula los resultados de cotas subestimada, ajustada y sobreestimada
void calcular_cotas(double t, double n, double exponente_sub,
                    double exponente_ajus, double exponente_sobre,
                    double *cota_sub, double *cota_ajus, double *cota_sobre) {
  if (exponente_sub == -1)
    *cota_sub = t / (n * log(n));
  else
    *cota_sub = t / pow(n, exponente_sub);
  if (exponente_ajus == -1)
    *cota_ajus = t / (n * log(n));
  else
    *cota_ajus = t / pow(n, exponente_ajus);
  if (exponente_sobre == -1)
    *cota_sobre = t / (n * log(n));
  else
    *cota_sobre = t / pow(n, exponente_sobre);
}

// ------------------------------------------------------------------------- //
//                         Ejecución por Tamaño 'n'                          //
// ------------------------------------------------------------------------- //

void ejecutar_prueba_para_n(void (*algoritmo)(int[], int),
                            void (*inicializar)(int[], int),
                            int v[], int n, int *tiempo_pequeno,
                            double exponente_sub, double exponente_ajus,
                            double exponente_sobre) {
  double tiempo, n_double, t;
  double cociente_sub, cociente_ajus, cociente_sobre;

  inicializar(v, n); // Inicializa el array
  tiempo = medir_tiempo(algoritmo, inicializar, v, n, tiempo_pequeno); 

  n_double = (double)n;
  t = tiempo;
  calcular_cotas(t, n_double, exponente_sub, exponente_ajus, exponente_sobre,
                 &cociente_sub, &cociente_ajus, &cociente_sobre); 

  if (*tiempo_pequeno)
    printf("%-5s %10d %15.3f %15.7f %15.7f %15.7f\n",
           "(*)", n, tiempo, cociente_sub, cociente_ajus, cociente_sobre); 
           // Marca con (*) si el tiempo fue pequeño
  else	
    printf("%-5s %10d %15.3f %15.7f %15.7f %15.7f\n",
           "", n, tiempo, cociente_sub, cociente_ajus, cociente_sobre); 
}

// ------------------------------------------------------------------------- //
//                          Cálculo de Exponentes                            //
// ------------------------------------------------------------------------- //

void obtener_exponentes_y_cotas(void (*algoritmo)(int[], int),
                                void (*inicializar)(int[], int),
                                double *exponente_sub, double *exponente_ajus,
                                double *exponente_sobre,
                                char **cota_sub, char **cota_ajus,
                                char **cota_sobre) {
  if (algoritmo == ord_ins) { // Para 'ord_ins'
    if (inicializar == ascendente) { // Caso mejor
      *exponente_sub = 0.8;
      *exponente_ajus = 1.0;
      *exponente_sobre = 1.2;
      *cota_sub = "t(n)/n^0.8";
      *cota_ajus = "t(n)/n";
      *cota_sobre = "t(n)/n^1.2";
    } else { // Caso promedio y peor
      *exponente_sub = 1.8;
      *exponente_ajus = 2.0;
      *exponente_sobre = 2.2;
      *cota_sub = "t(n)/n^1.8";
      *cota_ajus = "t(n)/n^2";
      *cota_sobre = "t(n)/n^2.2";
    }
  } else if (algoritmo == ord_rap) { // Para 'ord_rap'
    if (inicializar == aleatorio) {
      *exponente_sub = 1.05;
      *exponente_ajus = -1;  // Use n*log(n)
      *exponente_sobre = 1.25;
      *cota_sub = "t(n)/n^1.05";
      *cota_ajus = "t(n)/n*log(n)";
      *cota_sobre = "t(n)/n^1.25";
    } else { // Inicialización ascendente o descendente
      *exponente_sub = 1.005;
      *exponente_ajus = 1.0435;
      *exponente_sobre = 1.09;
      *cota_sub = "t(n)/n^1.005";
      *cota_ajus = "t(n)/n^1.0435";
      *cota_sobre = "t(n)/n^1.09";
    }
  }
}

// ------------------------------------------------------------------------- //
//                            Ejecución Completa                             //
// ------------------------------------------------------------------------- //

void ejecutar_pruebas_inicializacion(void (*algoritmo)(int[], int),
                                     void (*inicializar)(int[], int),
                                     char *nombre_algoritmo,
                                     char *nombre_inicializacion) {
  // Definición de los tamaños de pruebas:
  int tamanos[] = {500, 1000, 2000, 4000, 8000, 16000, 32000, 
		   64000, 128000, 256000, 512000};
  int n_tamanos = 
  	(algoritmo == ord_rap) ? sizeof(tamanos) / sizeof(tamanos[0]) : 7;
  
  // Ajustar el tamaño del array dependiendo del algoritmo
  int max_size = (algoritmo == ord_rap) ? MAX_TAM_RAP : MAX_TAM;
  int v[max_size], i, tiempo_pequeno;
  double exponente_sub, exponente_ajus, exponente_sobre;
  char *cota_sub, *cota_ajus, *cota_sobre;

  printf("\n%s con %s\n", nombre_algoritmo, nombre_inicializacion);

  obtener_exponentes_y_cotas(algoritmo, inicializar,
                             &exponente_sub, &exponente_ajus,
                             &exponente_sobre, &cota_sub,
                             &cota_ajus, &cota_sobre);

  imprimir_encabezado(cota_sub, cota_ajus, cota_sobre);

  for (i = 0; i < n_tamanos; i++) {
    ejecutar_prueba_para_n(algoritmo, inicializar, v,
                           tamanos[i], &tiempo_pequeno,
                           exponente_sub, exponente_ajus, exponente_sobre);
  }
}

// Ejecuta las pruebas para todos los tipos de inicialización
void ejecutar_pruebas(void (*algoritmo)(int[], int), char *nombre_algoritmo) {
  void (*inicializaciones[])(int[], int) = {aleatorio, descendente, ascendente};
  char *nombres_inicializaciones[] = {"inicialización aleatoria",
                                      "inicialización descendente",
                                      "inicialización ascendente"};
  int k;

  for (k = 0; k < 3; k++) {
    ejecutar_pruebas_inicializacion(algoritmo, inicializaciones[k],
                                    nombre_algoritmo,
                                    nombres_inicializaciones[k]);
  }
}

// ------------------------------------------------------------------------- //
//                              Función Main                                 //
// ------------------------------------------------------------------------- //

int main() {
  inicializar_semilla(); // Inicializa la semilla 
  
  //test();
  ejecutar_pruebas(ord_ins, "Ordenación por inserción"); 
  ejecutar_pruebas(ord_rap, "Ordenación rápida"); 

  return 0;
}
