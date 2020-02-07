// Gabbi Forsythe
// Parallel Project 1 - Part 1
// 2/10/2020

#include <iostream>
#include <stdio.h>
#include <string.h>
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
    char * alias = message;
    sprintf(alias, "Sending List: ");
    sprintf(alias+strlen(alias), " %d ", my_rank);

    // Ring 1
    if(my_rank == 0) {
        cout << alias << endl;
        MPI_Send(alias, strlen(alias)+1, MPI_CHAR, 2, tag, MPI_COMM_WORLD);
        if(p%2 == 0) {
            MPI_Recv(alias, 100, MPI_CHAR, p-2, tag, MPI_COMM_WORLD, &status);
        }
        else {
            MPI_Recv(alias, 100, MPI_CHAR, p-1, tag, MPI_COMM_WORLD, &status);
        }
        sprintf(alias + strlen(alias), " %d ", my_rank);
        cout << alias << endl;
    }
    else if(my_rank%2 == 0) {    // if my_rank is even
        MPI_Recv(alias, 100, MPI_CHAR, my_rank - 2, tag, MPI_COMM_WORLD, &status);
        sprintf(alias + strlen(alias), " %d ", my_rank);
        if(p-1 == my_rank || p-2 == my_rank) {   // if my_rank is going to be the last one in the ring
            MPI_Send(alias, strlen(alias)+1, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
        }
        else {
            MPI_Send(alias, strlen(alias)+1, MPI_CHAR, my_rank + 2, tag, MPI_COMM_WORLD);
        }
        cout << alias << endl; 
    }

    // Ring 2
    else if (my_rank == 1) {
        cout << alias << endl;
        if(p%2 == 0) {
            MPI_Send(alias, strlen(alias)+1, MPI_CHAR, p-1, tag, MPI_COMM_WORLD);
        }
        else {
            MPI_Send(alias, strlen(alias)+1, MPI_CHAR, p-2, tag, MPI_COMM_WORLD);
        }
        MPI_Recv(alias, 100, MPI_CHAR, 3, tag, MPI_COMM_WORLD, &status);
        sprintf(alias + strlen(alias), " %d ", my_rank);
        cout << alias << endl;
    }
    else {
        if(p-1 == my_rank || p-2 == my_rank) {   // if my_rank should be receiving from p1
            MPI_Recv(alias, 100, MPI_CHAR, 1, tag, MPI_COMM_WORLD, &status);
            sprintf(alias + strlen(alias), " %d ", my_rank);
        }
        else {
            MPI_Recv(alias, 100, MPI_CHAR, my_rank+2, tag, MPI_COMM_WORLD, &status);
            sprintf(alias + strlen(alias), " %d ", my_rank);
        }
        MPI_Send(alias, strlen(alias)+1, MPI_CHAR, my_rank - 2, tag, MPI_COMM_WORLD);
        cout << alias << endl; 
    }



	// Shut down MPI
	MPI_Finalize();

	return 0;
}

/* SAMPLE RUNS: 
thomas% mpirun -q -np 5 blah
Sending List:  0 
Sending List:  0  2 
Sending List:  0  2  4  0 
Sending List:  0  2  4 
Sending List:  1 
Sending List:  1  3  1 
Sending List:  1  3 
thomas% 



thomas% mpirun -q -np 32 blah
Sending List:  0 
Sending List:  1 
Sending List:  0  2 
Sending List:  0  2  4 
Sending List:  0  2  4  6 
Sending List:  0  2  4  6  8 
Sending List:  0  2  4  6  8  10 
Sending List:  0  2  4  6  8  10  12 
Sending List:  0  2  4  6  8  10  12  14 
Sending List:  0  2  4  6  8  10  12  14  16 
Sending List:  0  2  4  6  8  10  12  14  16  18 
Sending List:  0  2  4  6  8  10  12  14  16  18  20 
Sending List:  0  2  4  6  8  10  12  14  16  18  20  22 
Sending List:  0  2  4  6  8  10  12  14  16  18  20  22  24 
Sending List:  0  2  4  6  8  10  12  14  16  18  20  22  24  26  28  30  0 
Sending List:  0  2  4  6  8  10  12  14  16  18  20  22  24  26 
Sending List:  0  2  4  6  8  10  12  14  16  18  20  22  24  26  28 
Sending List:  0  2  4  6  8  10  12  14  16  18  20  22  24  26  28  30 
Sending List:  1  31  29  27  25  23 
Sending List:  1  31  29  27  25 
Sending List:  1  31  29  27 
Sending List:  1  31  29 
Sending List:  1  31 
Sending List:  1  31  29  27  25  23  21  19  17  15  13 
Sending List:  1  31  29  27  25  23  21  19  17  15 
Sending List:  1  31  29  27  25  23  21  19  17 
Sending List:  1  31  29  27  25  23  21  19 
Sending List:  1  31  29  27  25  23  21 
Sending List:  1  31  29  27  25  23  21  19  17  15  13  11  9  7  5  3  1 
Sending List:  1  31  29  27  25  23  21  19  17  15  13  11  9  7  5  3 
Sending List:  1  31  29  27  25  23  21  19  17  15  13  11  9  7  5 
Sending List:  1  31  29  27  25  23  21  19  17  15  13  11  9  7 
Sending List:  1  31  29  27  25  23  21  19  17  15  13  11  9 
Sending List:  1  31  29  27  25  23  21  19  17  15  13  11 
thomas% 
*/