#	Rutas
DIROBJ := obj/
DIREXE := exe/
DIRSRC := src/

CFLAGS := -c -Wall
LDLIBS := -lrt
CC := gcc

#	Crear todo
all : dirs manager filosofos

#	Crear los directorios de objetos y ejecutables
dirs :
	@mkdir -p $(DIROBJ) $(DIREXE)
	@echo "Directorios creados"

#	Compilacion de manager.c
manager : $(DIROBJ)manager.o
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)
	@echo "manager.c compilado"

# Compilacion de filosofos.c
filosofos : $(DIROBJ)filosofos.o
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)
	@echo "filosofos.c compilado"

# Compilacion de los archivos .c
$(DIROBJ)%.o : $(DIRSRC)%.c
	$(CC) $(CFLAGS) $^ -o $@

#	Limpiar los archivos
clean :
	@rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRSRC)*~
	@echo "Limpiado correctamente"