# Makefile
# Practica 1
# Javier Bejar Mendez

BIN = bin
SRC = src
OBJ = obj
LIB = lib
INCLUDE = include

CC = mpicxx 
CFLAGS = -std=c++11 -O3

TARGETS = clean $(BIN)/creaejemplo $(BIN)/main

all: $(TARGETS)


$(OBJ)/Graph.o: $(SRC)/Graph.cc $(INCLUDE)/Graph.h
	$(CC) $(CFLAGS) -c -o $(OBJ)/Graph.o $(SRC)/Graph.cc -I$(INCLUDE)

#Construccion floyd iterativo ------------------------------------------	
$(OBJ)/floyd.o: $(SRC)/floyd.cc $(INCLUDE)/Graph.h
	$(CC) $(CFLAGS) -c -o $(OBJ)/floyd.o $(SRC)/floyd.cc -I$(INCLUDE)
	
$(BIN)/floyd: $(OBJ)/Graph.o $(OBJ)/floyd.o 
	$(CC) $(CFLAGS) -o $(BIN)/floyd $(OBJ)/Graph.o $(OBJ)/floyd.o
#-----------------------------------------------------------------------

#Construccion floyd paralelo 1 dimesion --------------------------------
$(OBJ)/floyd_1d.o: $(SRC)/floyd_1d.cc $(INCLUDE)/Graph.h
	$(CC) $(CFLAGS) -c -o $(OBJ)/floyd_1d.o $(SRC)/floyd_1d.cc -I$(INCLUDE)
	
$(BIN)/floyd_1d: $(OBJ)/Graph.o $(OBJ)/floyd_1d.o 
	$(CC) $(CFLAGS) -o $(BIN)/floyd_1d $(OBJ)/Graph.o $(OBJ)/floyd_1d.o
#-----------------------------------------------------------------------

#Construccion creaejemplo ----------------------------------------------
$(BIN)/creaejemplo: $(SRC)/creaejemplo.cpp
	g++ -o $(BIN)/creaejemplo $(SRC)/creaejemplo.cpp
#-----------------------------------------------------------------------

$(BIN)/main: $(OBJ)/main.o $(OBJ)/Graph.o
	$(CC) $(CFLAGS) -o $(BIN)/main $(OBJ)/Graph.o $(OBJ)/main.o
	
$(OBJ)/main.o: $(SRC)/main.cc
	$(CC) $(CFLAGS) -c -o $(OBJ)/main.o $(SRC)/main.cc -I$(INCLUDE)

# Clean
.PHONY: clean

clean:
	-rm $(BIN)/* $(OBJ)/*.o
