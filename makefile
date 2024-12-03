# Nombre del ejecutable
EXEC = p4

# Archivo fuente
SRC = p4.c

# Compilador y opciones
CC = gcc
CFLAGS = -Wall 

# Regla para compilar y ejecutar
all: $(EXEC)
	./$(EXEC)

# Regla para compilar el ejecutable
$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC)

# Regla para limpiar los archivos generados
clean:
	rm -f $(EXEC)

