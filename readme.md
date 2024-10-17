  -------------------------------------------------------------------------
  Esquema de llamadas de funciones:
  
  main()
   |
   |--> inicializar_semilla()
   |
   |--> test()
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
         |     |--> ord_ins(v, n)          (Para test_ordenacion_insercion)
         |     |--> ord_rap(v, n)          (Para test_ordenacion_rapida)
         |     |--> imprimir_vector(v, n)
         |     |--> esta_ordenado(v, n)

   |
   |--> ejecutar_pruebas()
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

  -------------------------------------------------------------------------

