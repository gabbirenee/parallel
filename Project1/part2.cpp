#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h" // message passing interface
using namespace std;

// New compile and run commands for MPI!
// mpicxx -o blah part2.cpp
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
    srand(71911);

    int n = 300000;
    int * a = new int[n];
    int max, min;
    max = min = 0;
    double average, sum;
    average = sum = 0.0; 
    // fill in the array with random numbers in processor 0
    if(my_rank == 0) {
        // cout << "Array of Numbers: ";
        for(int x = 0; x < n; x++) {
            a[x] = rand() % 8;
            // cout << a[x] << " ";
        }
        // cout << endl;
    }

    // dividing the problem up here
    int localn = n/p; 
    int *locala = new int [localn];

    MPI_Scatter(a, localn, MPI_INT, locala, localn, MPI_INT, 0, MPI_COMM_WORLD);

    // doing the local work for max
    int localmax, localmin;
    double localsum;
    localsum = 0.0;
    localmax = -1; 
    localmin = 1000; 

    for(int i = 0; i < localn; i++) {
        if(locala[i] > localmax) {
            localmax = locala[i];
        }
    }

    // cout << "Max of " << my_rank << ": " << localmax << endl; 

    // combine answers back together for the maximum
    MPI_Reduce(&localmax, &max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if(my_rank == 0) {
        cout << "Maximum: " << max << endl;
    }
    
    // do the work for the minimum
    for(int i = 0; i < localn; i++) {
        if(locala[i] < localmin) {
            localmin = locala[i];
        }
    }

    // cout << "Min of " << my_rank << ": " << localmin << endl; 

    // combine all answers for the global minimum
    MPI_Reduce(&localmin, &min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    if(my_rank == 0) {
        cout << "Minimum: " << min << endl;
    }

    // do the local work to find the sum of every process
    for(int i = 0; i < localn; i++) {
        localsum += (double) locala[i];
    }

    // cout << "Local Sums" << 

    // combine all local sums together into one giant sum 
    MPI_Reduce(&localsum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // do the work to find the actual average
    if(my_rank == 0) {
        average = sum / n; 
        cout << "Average: " << average << endl; 
    }

    // avoid memory leaks at all costs
    delete [] a;
    delete [] locala;
	// Shut down MPI
	MPI_Finalize();

	return 0;
}

/* SAMPLE RUN WITH N=10:
    thomas% mpirun -q -np 5 blah
    Array of Numbers: 1 2 0 1 7 3 3 3 4 6 
    Maximum: 7
    Minimum: 0
    Average: 3
    thomas%

SAMPLE RUN WITH N=300000: 
    thomas% mpirun -q -np 10 blah
    Maximum: 7
    Minimum: 0
    Average: 3.50291
    thomas% 
*/