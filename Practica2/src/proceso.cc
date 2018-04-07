#include "proceso.h"
#include "libbb.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <mpi.h>

using namespace std;

void Proceso::inicializa(int argc, char **argv){
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); //obtenemos el identificador del proceso
    MPI_Comm_size(MPI_COMM_WORLD, &size); //obtenemos el numero de procesos
    
    if(rank == 0){
		switch (argc) {
			case 3:		NCIUDADES = atoi(argv[1]);
						break;
			default:	cerr << "La sintaxis es: bbseq <tama�o> <archivo>" << endl;
						exit(1);
						break;
		}
	}
    MPI_Bcast(&NCIUDADES, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
	iteraciones = 0;
	U = INFINITO;  
	nodo.inicializa();                // inicializa cota superior
	InicNodo (&nodo);              // inicializa estructura nodo
	
	if(rank==0){
		int** tsp0 = reservarMatrizCuadrada(NCIUDADES);
		LeerMatriz (argv[2], tsp0);    // lee matriz de fichero
		fin = !Inconsistente(tsp0);
		
		
	}else{
		equilibradoCarga(pila, fin);
		if(!fin) pila.pop(nodo);
	}
	
	MPI_Comm_split(MPI_COMM_WORLD, 0, rank, &comunicadorCarga);
	MPI_Comm_split(MPI_COMM_WORLD, 0, rank, &comunicadorCota);
	siguiente=(rank+1)%size;
	anterior=(rank-1+size)%size;
	//cout<<"Soy "<<rank<<" me precede "<<anterior<<" y me prosigue "<<siguiente<<endl;
}

void Proceso::procesa(){
	while (!fin) {       // ciclo del Branch&Bound
		Ramifica (&nodo, &lnodo, &rnodo, tsp0);		
		nueva_U = false;
		if (Solucion(&rnodo)) {
			if (rnodo.ci() < U) {    // se ha encontrado una solucion mejor
				U = rnodo.ci();
				nueva_U = true;
				difundir_cs_local = true;
				CopiaNodo (&rnodo, &solucion);
			}
		}
		else {                    //  no es un nodo solucion
			if (rnodo.ci() < U) {     //  cota inferior menor que cota superior
				if (!pila.push(rnodo)) {
					printf ("Error: pila agotada\n");
					liberarMatriz(tsp0);
					exit (1);
				}
			}
		}
		if (Solucion(&lnodo)) {
			if (lnodo.ci() < U) {    // se ha encontrado una solucion mejor
				U = lnodo.ci();
				nueva_U = true;
				difundir_cs_local = true;
				CopiaNodo (&lnodo,&solucion);
			}
		}
		else {                     // no es nodo solucion
			if (lnodo.ci() < U) {      // cota inferior menor que cota superior
				if (!pila.push(lnodo)) {
					printf ("Error: pila agotada\n");
					liberarMatriz(tsp0);
					exit (1);
				}
			}
		}
		
		difusionCotaSuperior();
		if (nueva_U) pila.acotar(U);
		
		equilibradoCarga(pila, fin);
		
		if(!fin) pila.pop(nodo);
		iteraciones++;
	}
}

void Proceso::equilibradoCarga(tPila& pila, bool& fin){
	if(pila.vacia()){
		MPI_Isend(&rank, 1, MPI_INT, siguiente, PETICION, comunicadorCarga, &request);
		
		while(pila.vacia() && !fin){
			MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, comunicadorCarga, &status);
			
			switch(status.MPI_TAG){
			case PETICION:
				MPI_Recv(&origen, 1, MPI_INT, anterior, PETICION, comunicadorCarga, &status);
				if(origen == rank){
					MPI_Isend(&rank, 1, MPI_INT, siguiente, PETICION, comunicadorCarga, &request);
					initFin();
				}
				else
					MPI_Isend(&origen, 1, MPI_INT, siguiente, PETICION, comunicadorCarga, &request);
				break;
				
			case NODOS:
					MPI_Recv(pila.nodos, MAXPILA, MPI_INT, status.MPI_SOURCE, NODOS, comunicadorCarga, MPI_STATUS_IGNORE);
					MPI_Get_count(&status, MPI_INT, &pila.tope);
				break;
				
			default:
				cout<<"Error: mensaje desconocido en equilibrar carga"<<endl;
			}
		}
	}
	if(!fin){
		MPI_Iprobe(anterior, PETICION, comunicadorCarga, &flag, &status);
		while(flag){
			MPI_Recv(&origen, 1, MPI_INT, anterior, PETICION, comunicadorCarga, &status);
			if(pila.divide(pila_carga))
				MPI_Isend(pila_carga.nodos, pila_carga.numInts(), MPI_INT, origen, NODOS, comunicadorCarga, &request);
			else
				MPI_Isend(&origen, 1, MPI_INT, siguiente, PETICION, comunicadorCarga, &request);
				
			MPI_Iprobe(anterior, PETICION, comunicadorCarga, &flag, &status);	
		}
	}
}

void Proceso::difusionCotaSuperior(){
	if(difundir_cs_local && !pendiente_retorno_cs){
		MPI_Isend(&U, 1, MPI_INT, siguiente, rank, comunicadorCota, &request);
		pendiente_retorno_cs=true;
		difundir_cs_local=false;
	}
	MPI_Iprobe(anterior, MPI_ANY_TAG, comunicadorCota, &flag, &status);
	while(flag){
		MPI_Recv(&recv_U, 1, MPI_INT, anterior, origen, comunicadorCota, &status);
		if(recv_U < U){
			U = recv_U;
			nueva_U = true;
		}
		
		if(origen == rank && difundir_cs_local){
			MPI_Isend(&U, 1, MPI_INT, siguiente, rank, comunicadorCota, &request);
			pendiente_retorno_cs=true;
			difundir_cs_local=false;
		}
		else if(origen == rank && !difundir_cs_local)
			pendiente_retorno_cs=false;
		else
			MPI_Isend(&recv_U, 1, MPI_INT, siguiente, origen, comunicadorCota, &request);
		
		MPI_Iprobe(anterior, MPI_ANY_TAG, comunicadorCota, &flag, &status);
	}
}

void Proceso::initFin(){
	
}











