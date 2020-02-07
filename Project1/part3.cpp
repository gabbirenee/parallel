// Gabbi Forsythe
// Parallel Project 1 - Part 3
// 2/10/2020

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include "mpi.h" // message passing interface
using namespace std;

// New compile and run commands for MPI!
// mpicxx -o blah part3.cpp
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
	int n = 1000;	// number of characters in the text file
    char * txt = new char [n];	// where the characters from the text file will go
	int * counts = new int [26];
	
	if(my_rank == 0) {
		ifstream infile; 
		infile.open("mordorsecode.txt");

		char temp; 
		temp = ' '; 
		if(!infile) {
			cout << "Error reading file" << endl; 
			return 0;
		}

		// read numbers from text file and put them in the array
		for(int i = 0; i < n; i++) {
			infile >> temp;
			txt[i] = temp; 
			cout << txt[i];
		}
		cout << endl; 
		infile.close();
	}

	// DIVIDE THE PROBLEM UP
	char * localtxt = new char [n/p];
	int * localCounts = new int [26];
	for(int i = 0; i < 26; i++) {	// set elements of local count to 0 for all processes
		localCounts[i] = 0;
	}
	
	MPI_Scatter(txt, n/p, MPI_CHAR, localtxt, n/p, MPI_CHAR, 0, MPI_COMM_WORLD);

	// DO THE LOCAL WORK
	int ascii = 0;
	for(int i = 0; i < n/p; i++) {	// use ascii numbers to count all the letters in the array; lowercase a = 97 to lowercase = 122
		ascii = int(localtxt[i]);
		if(ascii >= 97 && ascii <= 122) {
			localCounts[ascii-97]++; 	// minus 97 because that is where the lowercase numbers start in ascii world; increment the array position for each letter of the alphabet (a=0; z=26 )
		}
	}

	// bring it back now y'all 
	for(int i = 0; i < 26; i++) {
		MPI_Reduce(&localCounts[i], &counts[i], 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);	// adding the numbers in the array positions together to get the 
	}

	// print out all the letters and how many of them were in the text file
	if(my_rank == 0) {
		for(int i = 0; i < 26; i++) {
			cout << char(i+97) << ": " << counts[i] << endl;
		}
		cout << endl;
	}

	delete [] txt; 
	delete [] counts; 
	delete [] localtxt; 
	delete [] localCounts; 
	// Shut down MPI
	MPI_Finalize();

	return 0;
}

/* SAMPLE RUN:
thomas% mpirun -q -np 10 blah
aiufhweliuhiufioqiqeoipeqiuhvnznxzcvmnwfoijfawokopwekfiherugkzdzjknvdvcnjewfjkloakwefiouaheruhiertjknfqweorituyalskdjhfgmzxncbvalskdjfhgpqoweirutyaiufhweliuhiufioqiqeoipeqiuhvnznxzcvmnwfoijfawokopwekfiherugkzdzjknvdvcnjewfjkloakwefiouaheruhiertjknfqweorituyalskdjhfgmzxncbvalskdjfhgpqoweirutyaiufhweliuhiufioqiqeoipeqiuhvnznxzcvmnwfoijfawokopwekfiherugkzdzjknvdvcnjewfjkloakwefiouaheruhiertjknfqweorituyalskdjhfgmzxncbvalskdjfhgpqoweirutyaiufhweliuhiufioqiqeoipeqiuhvnznxzcvmnwfoijfawokopwekfiherugkzdzjknvdvcnjewfjkloakwefiouaheruhiertjknfqweorituyalskdjhfgmzxncbvalskdjfhgpqoweirutyaiufhweliuhiufioqiqeoipeqiuhvnznxzcvmnwfoijfawokopwekfiherugkzdzjknvdvcnjewfjkloakwefiouaheruhiertjknfqweorituyalskdjhfgmzxncbvalskdjfhgpqoweirutyaiufhweliuhiufioqiqeoipeqiuhvnznxzcvmnwfoijfawokopwekfiherugkzdzjknvdvcnjewfjkloakwefiouaheruhiertjknfqweorituyalskdjhfgmzxncbvalskdjfhgpqoweirutyaiufhweliuhiufioqiqeoipeqiuhvnznxzcvmnwfoijfawokopwekfiherugkzdzjknvdvcnjewfjkloakwefiouaheruhiertjknfqoqwieruakjsdhncvbiqwo
a: 41
b: 7
c: 21
d: 27
e: 76
f: 68
g: 19
h: 55
i: 91
j: 48
k: 62
l: 26
m: 13
n: 49
o: 63
p: 20
q: 36
r: 34
s: 13
t: 19
u: 62
v: 35
w: 56
x: 13
y: 12
z: 34

*/