#ifndef _PROCESO_H_
#define _PROCESO_H

#include "libbb.h"
#include <mpi.h>

using namespace std;


class Proceso{
public:
	Proceso() = default;
	
	void inicializa(int argc, char **argv);
	void equilibradoCarga(tPila& pila, bool& fin);
	void difusionCotaSuperior();
	void procesa();
	void initFin();
	
	
	
private:
	tNodo	nodo,         // nodo a explorar
			lnodo,        // hijo izquierdo
			rnodo,        // hijo derecho
			solucion;     // mejor solucion
	bool fin,        // condicion de fin
		nueva_U;       // hay nuevo valor de c.s.
	int  U, recv_U;             // valor de c.s.
	int iteraciones;
	tPila pila;         // pila de nodos a explorar
	tPila pila_carga;   //pila de nodos para enviar/recibir trabajo
	int** tsp0;
	
	// Tipos de mensajes que se env�an los procesos
	enum tag_msg{
		PETICION = 0,
		NODOS = 1,
		TOKEN = 2,
		FIN = 3
	};
	/*
	const int  PETICION = 0;
	const int NODOS = 1;
	const int TOKEN = 2;
	const int FIN = 3;
	*/
	// Estados en los que se puede encontrar un proceso
	const int ACTIVO = 0;
	const int PASIVO = 1;

	// Colores que pueden tener tanto los procesos como el token
	const int BLANCO = 0;
	const int NEGRO = 1;


	// Comunicadores que usar� cada proceso
	MPI_Comm comunicadorCarga;	// Para la distribuci�n de la carga
	MPI_Comm comunicadorCota;	// Para la difusi�n de una nueva cota superior detectada
	
	
	// Variables que indican el estado de cada proceso
	int rank;	 // Identificador del proceso dentro de cada comunicador (coincide en ambos)
	int size;	// N�mero de procesos que est�n resolviendo el problema
	int estado;	// Estado del proceso {ACTIVO, PASIVO}
	int color;	// Color del proceso {BLANCO,NEGRO}
	int color_token; 	// Color del token la �ltima vez que estaba en poder del proceso
	bool token_presente;  // Indica si el proceso posee el token
	int anterior;	// Identificador del anterior proceso
	int siguiente;	// Identificador del siguiente proceso
	bool difundir_cs_local;	// Indica si el proceso puede difundir su cota inferior local
	bool pendiente_retorno_cs;	// Indica si el proceso est� esperando a recibir la cota inferior de otro proceso
	MPI_Request request;
	MPI_Status status;
	int flag;
	int origen;
	
	
	
};


#endif
