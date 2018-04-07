//**************************************************************************
#ifndef GRAPH_H
#define GRAPH_H

#include <mpi.h>

//**************************************************************************
const int INF= 100000;

//**************************************************************************
class Graph //Adjacency List clas
{
private:
	int *A;
	int vertices;
	int N;
	
	int rank, size, raiz_P;
	int *buffSend;
	int *buffRecv;
	
	MPI_Datatype MPI_BLOQUE;
	
	
public:
	Graph();
	~Graph();
	
	void fija_nverts(const int verts);
	void inserta_arista(const int ptA,const int ptB, const int edge);
	int arista(const int ptA,const int ptB);
	void imprime();
	void lee(char *filename);
	
	
	static void init(int argc, char* argv[]);
	static void finalize();
	
	int getRank() const;
	int getSize() const;
	
	void scatter();
	void gather();
	
	double compute();
};

//**************************************************************************
#endif
//**************************************************************************
