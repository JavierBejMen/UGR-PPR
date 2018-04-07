#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <mpi.h>
#include "libbb.h"
#include "proceso.h"

using namespace std;

unsigned int NCIUDADES;

main (int argc, char **argv) {
	MPI::Init(argc,argv);
	
	Proceso p;
	p.inicializa(argc, argv);

	
	
	

	
	
	
	
	MPI::Finalize();
	return 0;
}
