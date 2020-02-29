#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
using namespace std;

void merge(int * a, int * b, int first, int last, int middle) {
    int i, j, k;
    i = first;
    j = middle+1;
    k = first;

    // int n = middle-first;
    // int m = last-middle+1;

    while(i <= middle && j <= last) {
        if (a[i] < a[j]) 
            b[k++] = a[i++]; 
        else
            b[k++] = a[j++]; 
    }

    while (i <= middle) 
        b[k++] = a[i++]; 
    while (j <= last) 
        b[k++] = a[j++]; 
    
    for(int x = first; x < k; x++) {
        a[x] = b[x];
    }
}

void mergesort (int * a, int first, int last) {
    int mid = (first + last)/2; 

    int * temp = new int [last+1];

    if(first < last) {
        mergesort(a, first, mid);
        mergesort(a, mid+1, last);
        merge(a, temp, first, last, mid);
    }
    
    delete temp;
}


int main (int argc, char * argv[]) {
    int size; 
    cout<<"Input amount of numbers to sort: ";
    cin>>size;  // get size from user
    
    int * arr = new int [size];
    for(int i = 0; i < size; i++) {     // fill the array with the random numbers
        arr[i] = rand() % 100;
        // cout<<arr[i]<<endl;
    }

    cout<<"Unsorted Array = [";
    for (int i = 0; i < size; i ++) {
        cout<<arr[i]<< " "; 
    }
    cout<<"]"<<endl;

    // sort the arrays here
    mergesort(arr, 0, size-1);

    cout<<"Sorted Array = [";
    for (int i = 0; i < size; i ++) {
        cout<<arr[i]<< " "; 
    }
    cout<<"]"<<endl;

    delete arr;
    return 0; 
}

/* SAMPLE RUNS: 

thomas% a.out
Input amount of numbers to sort: 20
Unsorted Array = [83 86 77 15 93 35 86 92 49 21 62 27 90 59 63 26 40 26 72 36 ]
Sorted Array = [15 21 26 26 27 35 36 40 49 59 62 63 72 77 83 86 86 90 92 93 ]
thomas% a.out
Input amount of numbers to sort: 10
Unsorted Array = [83 86 77 15 93 35 86 92 49 21 ]
Sorted Array = [15 21 35 49 77 83 86 86 92 93 ]
thomas% 

*/ 