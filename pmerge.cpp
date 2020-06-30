/**PARALLEL MERGE
 * @AUTHOR GABBI FORSYTHE
 * @AUTHOR NICOLE SELIG
 */

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <iomanip>
#include "mpi.h" // message passing interface
#include <cmath>


using namespace std;

// New compile and run commands for MPI!
// mpicxx -o blah pmerge.cpp
// mpirun -q -np 4 blah

/*---------------------------
GLOBAL MPI VARIABLES
-----------------------------*/


int my_rank;            // my CPU number for this process
int p;                    // number of CPUs that we have


/*---------------------------
UTILITY FUNCTIONS
-----------------------------*/

//function to create an array
void createArray(int a[], int n )
{
    for (int x = 0; x < n; x++)
    {
        a[x] = x;
    }
}

// function to print array
void printArray(int a[], int n)
{
    for (int x = 0; x < n; x++){
        cout << setw(3) << a[x] << " ";
    }
}

//function to swap integers
void swap (int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

//function that generates a random permutation of a[]
//Fisher-Yates shuffle algorithm -- Time Complexity O(n)
void randomize (int a[], int n)
{
    //seed with an integer so we get the same answer every time
    srand (time(NULL));
    //start from the last element and swap one by one
    // no need to run for the first element, so i > 0
    for (int i = n - 1; i > 0; i--)
    {
        //pick a random index from 0 to i
        int j = rand() % (i+1);
        //swap a[i] with the element at the random index
        swap(&a[i], &a[j]);
    }
}

//function to fill all project arrays with zeroes
void FillRankArraysWithZeros(int srankSize, int *localendpointsA, int *localendpointsB,
                             int *endpointsA, int *endpointsB, int *localWIN, int * WIN, int n)
{
    for(int i = 0; i < srankSize*2 + 1; i++){
        localendpointsA[i] = 0;
        localendpointsB[i] = 0;
        endpointsA[i] = 0;
        endpointsB[i] = 0;
    }
    for (int i = 0; i < n; i++)
    {
        localWIN[i] = 0;
        WIN[i] = 0;
    }
}

//quicksort helper function
/* This function takes last element as pivot, places
the pivot element at its correct position in sorted
array, and places all smaller (smaller than pivot)
to left of pivot and all greater elements to right
of pivot */
int partition (int a[], int first, int last)
{
    int pivot = a[last];  // pivot
    int i = (first - 1);  // Index of smaller element

    for (int j = first; j <= last - 1; j++){
        // If current element is smaller than or
        // equal to pivot
        if (a[j] <= pivot){
            i++;    // increment index of smaller element
            swap(&a[i], &a[j]);
        }
    }
    swap(&a[i + 1], &a[last]);
    return (i + 1);
}

//quicksort function for sorting endpoints and testing
void quickSort(int a[], int first, int last)
{
    if (first < last)
    {
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partition(a, first, last);

        // Separately sort elements before
        // partition and after partition
        quickSort(a, first, pi - 1);
        quickSort(a, pi + 1, last);
    }
}



/*---------------------------
TEST FUNCTIONS
-----------------------------*/

//test that the array successfully broadcast a variable to all processors
void testBroadcastN (int n, int my_rank) {cout<<"p"<<my_rank<<": "<<n<<endl;}

//test the generation of an incremented array
void testIncrementArray (int n, int a[]){
    cout << "Here is your INCREMENTED array of size "<< n << "." << endl;
    printArray(a,n);
    cout << endl;
}

//tests the array randomizer
void testRandomizedArray(int a[], int n) {
    cout << "Here is your RANDOMIZED array of size "<< n << " with NO DUPLICATES: "<< endl;
    printArray(a,n);
    cout<<endl;
}

//test that the unsorted array successfully broadcast to all processors
void testUnsortedArrayBroadcast(int my_rank,int input[],int n){
    cout << "Process " << my_rank + 1 << " recieved array: " << endl;
    printArray(input,n);
    cout << endl;
}

void testAandB (int *a, int *b, int n) {
    if (my_rank == 0) {
        cout << "A: ";
        printArray(a, n / 2);
        cout << endl;
        cout << "B: ";
        printArray(b, n / 2);
        cout << endl;
    }
}

//test to verify a quicksorted array
void TestQuickSort(int * a, int n){
    quickSort(a,0,n-1);
    cout<< "QuickSorted Array: ";
    printArray(a,n);
    cout << endl;
}

//test that the endpoints array broadcasted correctly
void testEndpoints(int my_rank, int *endpointsA, int *endpointsB, int srankSize){
    if(my_rank == 0) {
        cout << "endpointsA: ";
        printArray(endpointsA, srankSize * 2 + 1);
        cout << endl << "endpointsB: ";
        printArray(endpointsB, srankSize * 2 + 1);
        cout << endl;
    }
}

void testWIN(int * WIN, int n){
    if (my_rank == 0) {
        cout << "WIN: ";
        printArray(WIN, n);
        cout << endl;
    }
}

/*---------------------------
PROJECT FUNCTIONS
-----------------------------*/

//rank function using binary search techniques
//valToFind is the ranks that we are searching for in an array
int rank(int* a, int first, int last, int valToFind) {

    // special case for last element in the array
    if (valToFind > a[last-1]){return last;}

    if(last == 1) {		// case of array of length 1
        if(valToFind <= a[0]) {return 0;}
        else { return 1; }
    }

    int mid = last/2;

    if(valToFind <= a[mid-1]) {
        return rank(&a[first], first, mid, valToFind);
    }
    else {
        return mid + rank(&a[mid], 0, mid, valToFind);
    }
}

//sequential merge function...used for creating our "shapes"
void smerge(int* a, int first1, int last1, int* b, int first2, int last2, int* localWIN, int first3, int last3) {

    int i = first1;
    int j = first2;
    int k = first3;
 
    while (i <= last1 && j <= last2)
        if (a[i] < b[j])
            localWIN[k++] = a[i++];
        else
            localWIN[k++] = b[j++];

    while (i <= last1)
        localWIN[k++] = a[i++];
    while (j <= last2)
        localWIN[k++] = b[j++];
}


//parallel merge function
//finds the rank of every n/logn item in each array in parallel and assigns a rank based on
//its position in the other array. These "endpoints" are used to define our small problems,
//or "shapes". The shapes are merged together sequentially and reduced in MPI to output a sorted array.
void pmerge(int* a, int *b, int n) {

    int logn2 = log2(n / 2);  // log(n/2)
    //rank array size
    int srankSize = ceil((double) (n / 2) / logn2);

    //local variables
    int localn = n;
    int *localWIN = new int[n]; //new win array
    int *localendpointsA = new int[srankSize*2+1];
    int *localendpointsB = new int[srankSize*2+1];

    //global variables
    int *WIN = new int[n];
    int * endpointsA = new int [srankSize*2+1];
    int * endpointsB = new int [srankSize*2+1];


    // initialize the array values to 0 so the MPI_SUM will work
    FillRankArraysWithZeros(srankSize, localendpointsA, localendpointsB, endpointsA, endpointsB, localWIN, WIN, n);

    //test the aliases
    //testAandB(a,b,n);


    //DEFINE RANK AND COMPUTE VALUES IN PARALLEL
    //sample n/logn items from A and m/logn items from B
    //then perform rank function
    //collect these rank positions for each array in "Endpoints"
    for (int i = my_rank; i < srankSize; i += p) {
        int first = 0;    // first will always be 0
        int last = n / 2;
        localendpointsA[i] = rank(a, first, last, b[i*logn2]); //SRANKA
        localendpointsA[i+srankSize] = i*logn2;
        localendpointsB[i] = rank(b, first, last, a[i*logn2]); //SRANKB
        localendpointsB[i+srankSize] = i*logn2;
        //cout<< my_rank <<" srank calculation" << endl; // see which processors a certain calculation
    }

    //MPI_AllReduce(void* send_data, void* recv_data, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm communicator)

    MPI_Allreduce(localendpointsA, endpointsA, srankSize*2, MPI_INT, MPI_SUM,
                  MPI_COMM_WORLD);    // collect all endpointsA and send to all processors

    MPI_Allreduce(localendpointsB, endpointsB, srankSize*2, MPI_INT, MPI_SUM,
                  MPI_COMM_WORLD);    // collect all endpointsA and send to all processors

    MPI_Allreduce(localWIN, WIN, n, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    //cout << my_rank << " Reduce complete." << endl;	// ensure all processors reach this point

    endpointsA[srankSize * 2] = n/2;
    endpointsB[srankSize * 2] = n/2; 

    //test the endpoints
   // if (my_rank==0) {
   //     cout << "SRANK + ENDPOINTS: " << endl;
   // }
   // testEndpoints(my_rank, endpointsA, endpointsB, srankSize);

    //endpoints must be sorted to define the shapes
    quickSort(endpointsA, 0, srankSize * 2 - 1);
    quickSort(endpointsB, 0, srankSize * 2 - 1);


    //test the sorted endpoints
   // if (my_rank ==0) {
    //    cout << "SORTED ENDPOINTS: " << endl;
   // }
   // testEndpoints(my_rank, endpointsA, endpointsB, srankSize);


    //CREATE THE SHAPES
    //using sequential merge
    for(int i = my_rank; i < srankSize * 2; i+=p) {
        smerge(a, endpointsA[i], endpointsA[i + 1]-1, b, endpointsB[i],
            endpointsB[i + 1]-1, localWIN, endpointsA[i] + endpointsB[i],
            endpointsB[i + 1] + endpointsB[i + 1]);
    }

    //collect all shapes into the win array
    MPI_Allreduce(localWIN, WIN, n, MPI_INT, MPI_SUM, MPI_COMM_WORLD);


    for (int i = 0; i < n; i++) {
        a[i] = WIN[i];
    }

    //test win
    testWIN(WIN,n);



}

//mergesort
void mergesort(int *input, int first, int last) {
    if (last<=4){
        quickSort(input, first, last-1);
    }
    else{
        mergesort(input, first, last/2);
        mergesort(&input[last/2],first,last/2);
        pmerge(input, &input[last/2], last);
    }
}


/*---------------------------
MAIN PROGRAM
-----------------------------*/
int main (int argc, char * argv[]) {


    int source;                // rank of the sender
    int dest;                // rank of destination
    int tag = 0;            // message number
    char message[100];        // message itself
    MPI_Status status;        // return status for receive

    // Start MPI
    MPI_Init(&argc, &argv);     // sets up processor

    // Find out my rank!
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);    // finds the rank and populates that variable

    // Find out the number of processes!
    MPI_Comm_size(MPI_COMM_WORLD, &p);


    //CREATE AN ARRAY WITH RANDOM NUMBERS BASED ON A USER INPUTTED SIZE
    //NO DUPLICATE ENTRIES IN ARRAY

    int n; //array size

    if (my_rank == 0) {
        //get the array size from user
        cout << "---------------------------------------------" << endl;
        cout << "WELCOME TO PMERGE!!" << endl;
        cout << "---------------------------------------------" << endl;
        cout << "Enter the array size: " << endl;
        cin >> n;
        cout << "Your array size is: " << n << endl;
    }

    //broadcast 'n' to all processors
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    //testBroadcastN(n,my_rank);

    int *input = new int[n];
    // SPLIT ARRAY INTO A AND B ALIASES
    int *a = &input[0];
    int *b = &input[n / 2];

    //Array from Uzi book
    // int hardarray [] = {4,6,8,9,16,17,18,19,20,21,23,25,27,29,31,32,1,2,3,5,7,10,11,12,13,14,15,22,24,26,28,30};
    // int * a = &hardarray[0];
    // int * b = &hardarray[n/2];


    if (my_rank == 0) {
        //generate an incremented array of size n
        createArray(input, n);
        //testIncrementArray(n, input);
        // randomize the array using techniques from class and generate
        // a randomized array with no duplicates
        cout << "RANDOMIZING ARRAY..." << endl;
        randomize(input, n);
        testRandomizedArray(input, n);
        // TestQuickSort(a,n);
        cout << endl;
    }

    //HAVE PROCESS 0 BROADCAST THE ARRAY TO EVERY OTHER PROCESS
    //everyone should have the same UNSORTED array
    MPI_Bcast(input, n, MPI_INT, 0, MPI_COMM_WORLD);
    // testUnsortedArrayBroadcast(my_rank,input,n);

    //ALL PROCESSORS CALL MERGESORT ON WHOLE ARRAY
    mergesort(input,0,n);

    
    // Shut down MPI
    MPI_Finalize();

    return 0;

}
