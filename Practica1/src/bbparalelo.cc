#include "Graph.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	Graph::init(argc, argv); //Inicializamos entorno MPI
	
	Graph G;
	
	//leemos el grafo de adyacencias inicial
	if(G.getRank() == 0)
	{
		if (argc != 2) 
		{
			cerr << "Sintaxis: " << argv[0] << " <archivo de grafo>" << endl;
			return(-1);
		}
		G.lee(argv[1]);
		
		//cout<<"Grafo inicial:"<<endl;
		//G.imprime();
		
	}
	
	G.scatter();
	
	double timeCompute;
	timeCompute = G.compute();
	
	G.gather();
	/*
	if(G.getRank() == 0)
	{
		cout<<"Grafo final:"<<endl;
		G.imprime();
	}*/
	
	if(G.getRank() == 0)
	{
		timeCompute /= G.getSize();
		cout<<"time: "<<timeCompute<<endl;
	}
	
	Graph::finalize(); //Finalizamos entorno MPI
	return 0;
}
