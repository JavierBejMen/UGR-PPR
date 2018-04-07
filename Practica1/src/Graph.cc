//***********************************************************************
#include "Graph.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>

#include <cmath>
#include <mpi.h>

using namespace std;

//***********************************************************************
Graph::Graph ()		// Constructor
{
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); //obtenemos el identificador del proceso
    MPI_Comm_size(MPI_COMM_WORLD, &size); //obtenemos el numero de procesos
    A = nullptr;
    buffSend = nullptr;
    buffRecv = nullptr;
    raiz_P = sqrt(size);
}

Graph::~Graph()
{
	if(A!=nullptr)
		delete[] A;
		
	if(buffSend!=nullptr)	
		delete[] buffSend;
		
	if(buffRecv!=nullptr)
		delete[] buffRecv;
}

//***********************************************************************
void Graph::fija_nverts (const int nverts)
{
A=new int[nverts*nverts];
vertices=nverts;
}
//***********************************************************************
void Graph::inserta_arista(const int vertA, const int vertB, const int edge) // inserta A->B
{
  A[vertA*vertices+vertB]=edge;
}
//***********************************************************************
int Graph::arista(const int ptA,const int ptB)
{
  return A[ptA*vertices+ptB];
}
//***********************************************************************
void Graph::imprime()
{
 int i,j,vij;
 cout<<"Proceso "<<rank<<":"<<endl;
 for(i=0;i<vertices;i++)
 {cout << "A["<<i << ",*]= ";
   
  for(j=0;j<vertices;j++)
   {
      if (A[i*vertices+j]==INF) 
        cout << "INF";
      else  
        cout << A[i*vertices+j];
      if (j<vertices-1) 
        cout << ",";
      else
        cout << endl;
   }
 }
}
//***********************************************************************
void Graph::lee(char *filename)
{
#define BUF_SIZE 100
std::ifstream infile(filename);

if (!infile)
	{
	 cerr << "Nombre de archivo inválido \"" << filename << "\" !!" << endl;
	 cerr << "Saliendo........." << endl;
	 exit(-1);
	}
//Obten el numero de vertices
  char buf[BUF_SIZE];
  infile.getline(buf,BUF_SIZE,'\n');
  vertices=atoi(buf);
  A=new int[vertices*vertices];
 
  int i,j;
  for(i=0;i<vertices;i++)
     for(j=0;j<vertices;j++)
	 if (i==j) A[i*vertices+j]=0;
         else A[i*vertices+j]=INF;
    
  while (infile.getline(buf,BUF_SIZE) && infile.good() && !infile.eof())
	{
	 char *vertname2 = strpbrk(buf, " \t");
	 *vertname2++ = '\0';
	 char *buf2 = strpbrk(vertname2, " \t");
	 *buf2++ = '\0';
	 int weight = atoi(buf2);
	 i=atoi(buf);
	 j=atoi(vertname2);
         A[i*vertices+j]=weight;
	 }
}

//Practica 1 floyd 2d
void Graph::init(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
}

void Graph::finalize()
{
	MPI_Finalize();
}

int Graph::getRank() const
{
	return rank;
}

int Graph::getSize() const
{
	return size;
}

void Graph::scatter()
{
	MPI_Bcast(&vertices, 1, MPI_INT, 0, MPI_COMM_WORLD);
	N = vertices;
	int tam = N/raiz_P;
	int fila_P, columna_P, comienzo;
	if(rank == 0)
	{
		MPI_Type_vector (tam, tam, N, MPI_INT, &MPI_BLOQUE);
		MPI_Type_commit(&MPI_BLOQUE);
		
		buffSend = new int[N*N];
		
		for(int i = 0, posicion = 0; i < size; ++i)
		{
			fila_P = i/raiz_P;
			columna_P = i%raiz_P;
			comienzo = (columna_P*tam)+(fila_P*tam*tam*raiz_P);
			//cout<<"N: "<<N<<"    fila_P: "<<fila_P<<"     columna_P: "<< columna_P<<"     comienzo: "<< comienzo<<"     tam: "<<tam<<"     A[comienzo]: "<<A[comienzo]<<endl;
			MPI_Pack(&A[comienzo], 1, MPI_BLOQUE, buffSend, sizeof(int)*N*N,
					&posicion, MPI_COMM_WORLD);
		}
		
		MPI_Type_free(&MPI_BLOQUE);
		
		delete[] A;
	}
	
	
		
		
	A = new int[tam*tam];
	vertices = tam;

	MPI_Scatter(&buffSend[0], sizeof(int)*tam*tam, MPI_PACKED, A, tam*tam,
				MPI_INT, 0, MPI_COMM_WORLD);
}

void Graph::gather()
{
	if(rank == 0)
		buffRecv = new int[N*N];
		
	int tam = N/raiz_P;
	int fila_P, columna_P, comienzo;
	
	MPI_Gather(A, tam*tam, MPI_INT, buffRecv, tam*tam, MPI_INT, 0,
				MPI_COMM_WORLD);
	
	delete[] A;
	A = nullptr;
	
	if(rank == 0)
	{
		A = new int[N*N];
		
		MPI_Type_vector (tam, tam, N, MPI_INT, &MPI_BLOQUE);
		MPI_Type_commit(&MPI_BLOQUE);
		
		for(int i = 0, posicion = 0; i < size; ++i)
		{
			fila_P = i/raiz_P;
			columna_P = i%raiz_P;
			comienzo = (columna_P*tam)+(fila_P*tam*tam*raiz_P);
			
			MPI_Unpack(buffRecv, sizeof(int)*N*N, &posicion, &A[comienzo], 1, 
						MPI_BLOQUE, MPI_COMM_WORLD);
		}
		
		vertices = N;
	}
}

double Graph::compute()
{
	MPI_Comm cFila, cColum;
	int fRank, cRank, fSize, cSize, cColor, fColor;
	fColor = rank/raiz_P;
	cColor = rank%raiz_P;
	MPI_Comm_split(MPI_COMM_WORLD, fColor, rank, &cFila);
	MPI_Comm_split(MPI_COMM_WORLD, cColor, rank, &cColum);
	
	MPI_Comm_rank(cFila, &fRank); 
    MPI_Comm_rank(cColum, &cRank); 
    
    MPI_Comm_size(cFila, &fSize); 
    MPI_Comm_size(cColum, &cSize);
    
    int *kColum, *kFila;
    kColum = new int[vertices];
    kFila = new int[vertices];
    
    int kColor;
    int i_global, j_global;
    
    MPI_Barrier(MPI_COMM_WORLD);
	double t=MPI::Wtime();
    
    for(int k = 0; k < N; ++k)
	{
		kColor = k/vertices;
		if(fColor == kColor)
			memcpy(kFila, &A[(k%vertices) * vertices], sizeof(int)*vertices);
			
		if(cColor == kColor){
			for(int i = 0; i < vertices; ++i)
			{
				kColum[i] = A[k%vertices + i*vertices];
			}
		}
		
		MPI_Bcast(kFila, vertices, MPI_INT, kColor, cColum);
		MPI_Bcast(kColum, vertices, MPI_INT, kColor, cFila);
		
		//cout<<k<<"--->proceso "<<rank<<": "<<kColum[0]<<","<<kColum[1]<<","<<kColum[2]<<","<<kColum[3]<<endl;
		for(int i = 0; i < vertices; ++i)
		{
			i_global = i + fColor*vertices;
			for(int j = 0; j < vertices; ++j)
			{
				j_global = j + cColor*vertices;
				if(i_global!=j_global && i_global!=k && j_global!=k)
				{
					A[i*vertices + j] = min(A[i*vertices + j], kColum[i] + kFila[j]);
				}
			}
		}
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
	t=MPI::Wtime()-t;
	
	MPI_Allreduce(&t, &t, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
	
	return t;
}
