### Esquema de llamadas de funciones:

```plaintext
main()
 |
 |--> inicializar_semilla()
 |
 |--> ejecutar_pruebas(ord_ins, "Ordenación por inserción")
 |--> ejecutar_pruebas(ord_rap, "Ordenación rápida")
       |
       |--> ejecutar_pruebas_inicializacion(algoritmo, inicializacion, ...)
       |     |
       |     |--> obtener_exponentes_y_cotas()
       |     |--> imprimir_encabezado()
       |     |
       |     |--> ejecutar_prueba_para_n()  (En bucle para distintos tamaños)
       |           |
       |           |--> inicializar(v, n)
       |           |--> medir_tiempo(algoritmo, inicializar, ...)
       |           |--> calcular_cotas(t, n, ...)
       |           |--> imprimir resultados (cotas subestimada, ajustada, sobreestimada)

 |
 |--> test()  [Opcional, en función de si está habilitado en el main]
       |
       |--> test_ordenacion_insercion(v, n, aleatorio, "aleatoria")
       |--> test_ordenacion_insercion(v, n, descendente, "descendente")
       |--> test_ordenacion_insercion(v, n, ascendente, "ascendente")
       |
       |--> test_ordenacion_rapida(v, n, aleatorio, "aleatoria")
       |--> test_ordenacion_rapida(v, n, descendente, "descendente")
       |--> test_ordenacion_rapida(v, n, ascendente, "ascendente")
       
       |     |--> inicializacion(v, n)  (aleatorio/descendente/ascendente)
       |     |--> imprimir_vector(v, n)
       |     |--> esta_ordenado(v, n)
       |     |--> test_ordenacion_aux(v, n, inicializacion, algoritmo, ...)
       |           |
       |           |--> algoritmo(v, n)   (ord_ins o ord_rap según corresponda)
       |           |--> imprimir_vector(v, n)
       |           |--> esta_ordenado(v, n)
       
       
