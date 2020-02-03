#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h" // message passing interface
using namespace std;

// New compile and run commands for MPI!
// mpicxx -o blah file.cpp
// mpirun -q -np 10 blah

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
    // srand(time(0));      // uses the clock time of the system as the seed for the random numbers
    srand(1241);
    int n = 300000;
    int * a = new int[n];
    int * b = new int[n];
    long int dotproduct = 0; 

    // fill in the array with random numbers in processor 0
    if(my_rank == 0) {
        for(int x = 0; x < n; x++) {
            a[x] = rand() % 8;      // % 8 keeps the numbers in the array small
            b[x] = rand() % 8; 
        }
    }

    // DIVIDE THE PROBLEM INTO PIECES
    int localn = n/p; 
    int *locala = new int [localn];
    int *localb = new int [localn];

    // After executing the lines of code below, locala and localb will have the "right" info in them
    // In other words,each array will have the block of array a (or b) that corresponds to the local information that they should hav ein their rank
    // Only process 0 has this information before the below lines are run
    // MPI_SCATTER(toScatter, chunkSize, type, whereToPutChunks, sizeOfLoc, locType, something, MPI_COMM_WORLD)
    MPI_Scatter(&a[0], localn, MPI_INT, locala, localn, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(&b[0], localn, MPI_INT, localb, localn, MPI_INT, 0, MPI_COMM_WORLD);

    // do the local work
    long int localdot = 0;
    for(int x = 0; x < localn; x++) {
        localdot += (locala[x] * localb[x]);
    }

    // COLLECT THE RESULTS
        // if (my_rank != 0) {
        //     MPI_Send(&localdot, 1, MPI_LONG, 0, tag, MPI_COMM_WORLD); 
        // }
        // else {
        //     dotproduct = localdot; 
        //     long int temp = 0; 
        //     for (int x = 1; x < p; x++) {
        //         MPI_Recv(&temp, 1, MPI_LONG , MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status); 
        //         dotproduct += temp;
        //     }
        //     cout << dotproduct << endl; 
        //     dotproduct = 0; 
        //     for(int x = 0; x < n; x++) {
        //         dotproduct += (a[x]*b[x]);
        //     }
        //     cout << dotproduct << endl;
        // }

    // MPI_Reduce(toCombine, answerVariable, length, type, operation, whereToCombine, MPI_COMM_WORLD)
        // MPI_Reduce(&localdot, &dotproduct, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);     // sends answers to process 0

    // Way to combine using for loop and reduce
        // for(int x = 0; x < p; x++) {
        //     MPI_Reduce(&localdot, &dotproduct, 1, MPI_LONG, MPI_SUM, x, MPI_COMM_WORLD);
        // }

    MPI_Allreduce(&localdot, &dotproduct, 1, MPI_LONG, MPI_SUM, MPI_COMM_WORLD);

    cout << dotproduct << endl;

    // no memory leaks here
    delete [] a; 
    delete [] b;
    delete [] locala;
    delete [] localb; 
	// Shut down MPI
	MPI_Finalize();

	return 0;
}