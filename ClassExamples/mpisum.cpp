#include <iostream>
#include <stdio.h>
#include <string.h>
#include "mpi.h" // message passing interface
using namespace std;

// New compile and run commands for MPI!
// mpicxx -o blah file.cpp
// mpirun -q -np 32 blah

int main (int argc, char * argv[]) {

	int my_rank;			// my CPU number for this process
	int p;					// number of CPUs that we have
	int source;				// rank of the sender
	int dest;				// rank of destination
	int tag = 0;			// message number
	char message[100];		// message itself
	MPI_Status status;		// return status for receive
	
	// Start MPI
	MPI_Init(&argc, &argv);     // sets up processor
	
	// Find out my rank!
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);    // finds the rank and populates that variable
	
	// Find out the number of processes!
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	
	// THE REAL PROGRAM IS HERE
        // how you would do the problem not parallely
            // int sum = 0;
            // for(int x = 0; x < 300000; x++)
            //     sum = sum + x; 
            // cout << sum << endl;
    // STEPS FOR SOLVING AN EMBARASSINGLY PARALLEL PROBLEM
        // 1. Know the problem
        // 2. Break the Problem into pieces
        // 3. Each process does its own local work
        // 4. Combine the local answers back together
    
    

	// Shut down MPI
	MPI_Finalize();

	return 0;
}