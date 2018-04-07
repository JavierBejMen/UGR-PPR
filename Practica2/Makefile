# Makefile
# Practica 1
# Javier Bejar Mendez

BIN = bin
SRC = src
OBJ = obj
LIB = lib
INCLUDE = include

CC = mpicxx 
CCFLAGS = -std=c++11 -O3

TARGETS = clean $(BIN)/bbseq $(BIN)/bbparalelo

all: $(TARGETS)

$(BIN)/bbseq: $(OBJ)/bbseq.o $(OBJ)/libbb.o
	$(CC) $(CCFLAGS) -o $@ $^
	
$(OBJ)/bbseq.o: $(SRC)/bbseq.cc
	$(CC) $(CCFLAGS) -c -o $@ $^ -I$(INCLUDE)


$(OBJ)/libbb.o: $(SRC)/libbb.cc
	$(CC) $(CCFLAGS) -c -o $@ $^ -I$(INCLUDE)
	
$(BIN)/bbparalelo: $(OBJ)/proceso.o $(OBJ)/libbb.o $(OBJ)/bbparalelo.o
	$(CC) $(CCFLAGS) -o $@ $^
	
$(OBJ)/bbparalelo.o: $(SRC)/bbparalelo.cc
	$(CC) $(CCFLAGS) -c -o $@ $^ -I$(INCLUDE)
	
$(OBJ)/proceso.o:$(SRC)/proceso.cc
	$(CC) $(CCFLAGS) -c -o $@ $^ -I$(INCLUDE)

# Clean
.PHONY: clean

clean:
	-rm $(BIN)/* $(OBJ)/*.o


