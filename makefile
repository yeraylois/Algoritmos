# Nombre del ejecutable
EXEC = p3

# Archivo fuente
SRC = p3.c

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
