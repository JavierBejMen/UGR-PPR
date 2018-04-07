#include <iostream>
#include <fstream>
#include <string.h>
#include "Graph.h"
#include <mpi.h>
#include <vector>
#include <cmath>

using namespace std;

int main(int argc, char *argv[])
{
	int rank, size;

    MPI_Init(&argc, &argv); //iniciamos el entorno MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //obtenemos el identificador del proceso
    MPI_Comm_size(MPI_COMM_WORLD, &size); //obtenemos el numero de procesos
	
	Graph G;
	
	int n_filas; //filas asignadas a cada proceso
	int n_vertices;
	
	if(rank == 0) //Leemos la matriz de adyacencia
	{
		if (argc != 2) 
		{
			cerr << "Sintaxis: " << argv[0] << " <archivo de grafo>" << endl;
			return(-1);
		}
		G.lee(argv[1]);
		
		//Asignamos las filas a los procesos
		n_filas = G.vertices / size;
		n_vertices = G.vertices;
		
		//G.imprime();
	}
	
	
	
	MPI_Bcast(&n_filas, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&n_vertices, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	
	//Repartimos las filas
	vector<int> g_local;
	g_local.resize(n_vertices * n_filas, 0);
	MPI_Scatter(G.get_graph(), n_vertices * n_filas, MPI_INT, &g_local[0],
				n_vertices * n_filas, MPI_INT, 0, MPI_COMM_WORLD);
				
	vector<int> fila_k; //fila k de la matriz de adyacencia
	fila_k.resize(n_vertices, 0);
	
	int i_global = 0;
	
	//Inicio del bucle
	MPI_Barrier(MPI_COMM_WORLD);
	double t=MPI::Wtime();
	
	for(int k = 0; k < n_vertices; ++k)
	{
		//Si k pertenece al proceso, copiamos la fila k para difundirla al resto de procesos
		if((int)k/n_filas == rank)
		{		
			memcpy(&fila_k[0], &g_local[k%n_filas*n_vertices], sizeof(int)*fila_k.size());
		}
		MPI_Bcast(&fila_k[0], n_vertices, MPI_INT, k/n_filas, MPI_COMM_WORLD);
		
		for(int i_local = 0; i_local < n_filas; ++i_local)
		{
			i_global = i_local + rank*n_filas;
			for(int j = 0; j < n_vertices; ++j)
			{
				if (i_global!=j && i_global!=k && j!=k) //comparacion de algoritmo de floyd
				{
					g_local[i_local*n_vertices + j] = min(g_local[i_local*n_vertices + j],
						(g_local[i_local*n_vertices + k] + fila_k[j]));
				}
			}
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);
	t=MPI::Wtime()-t;
	
	//Finalmente reconstruimos la matriz en el proceso 0
	int *B;
	if(rank == 0)
	{
		B = new int[n_vertices*n_vertices];
	}
	MPI_Gather(&g_local[0], n_vertices * n_filas, MPI_INT, B,
				n_vertices * n_filas, MPI_INT, 0, MPI_COMM_WORLD);
	
	if(rank == 0)
	{
		G.set_graph(B);
	}
	
	
	//Imprimimos resultado y tiempo
	if(rank == 0)
	{
		cout << endl<<"EL Grafo con las distancias de los caminos más cortos es:"<<endl<<endl;
		G.imprime();
		cout<< "Tiempo gastado= "<<t<<endl<<endl; 
	} 
	
	MPI::Finalize();
	return 0;
}
	
	
