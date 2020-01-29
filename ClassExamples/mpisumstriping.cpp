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
    // STEPS FOR SOLVING AN EMBARRASSINGLY PARALLEL PROBLEM

    /* SAME AS MPISUM BUT THIS IS THE STIPING METHOD */

	// 1. Know the problem
	int n = 300000; 
	long int sum = 0; 

	// 2. Break the Problem into pieces
	int local_start = my_rank; 	// assume that the size of the problem is easily divided by the number of problems
	// know why this is the number because tbh i don't know why this math works

	// 3. Each process does its own local work
	long int local_sum; 
	for(int x = local_start; x < n; x += p) {
		local_sum += x;
	}
	
	// 4. Combine the local answers back together
	// The processor that is "in charge" os called the overseer
	// By convention, the overseer is usually processor 0

	// Do stuff based upon what processor you are
	if(my_rank != 0) {
		MPI_Send(&local_sum, 1, MPI_LONG, 0, tag, MPI_COMM_WORLD);
	}
	else {
		sum = local_sum;
		long temp; 
		
		for(int x = 1; x < p; x++) {
			MPI_Recv(&temp, 1, MPI_LONG, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
			sum += temp;
		}
		cout << sum << endl; 
	}
	
	// Shut down MPI
	MPI_Finalize();

	return 0;
}