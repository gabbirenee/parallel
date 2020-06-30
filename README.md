# Parallel Algorithms
My solutions to projects assigned in CS452: Parallel Algorithms at Butler University. Languages Used: C++ and MPI

## Class Examples 
This folder contains all the files we used while learning about MPI in class. My notes are in the comments of the code. The files in this folder are very simple programs that are used to teach the basic MPI API calls. 

## Project 1 - Get to Know MPI
This project was focused on getting acquainted with MPI. The outputs of these programs can be found at the bottom of the program files in the comments.
* `tworings.cpp` - The first ring starts with process 0 sending one message to the next even process, which sends to the next even process, and so on until the last process sends its message back to process 0. THe second wring starts with process 1 and sends its message to the previous even process, which continues until the last process sends the message back to process 1. It does not matter what order the messages are sent in as long as they are sent. 
* `part2.cpp` - Use the MPI scatter and reduce API calls to find the maximum, minimum, and average values in an array. 
* `part3.cpp` - REad in a text file full of letters and figure out how many of each letter there are in the file using the scatter and gather (or reduce) MPI API calls. The text file that my program reads from is `mordorsecode.txt` 

## Project 2 - Sequential Mergesort
This program generates an array of random numbers and then uses sequential mergesort to sort this array. 

## `pmerge.cpp` - Parallel Mergesort
Affectionately called *pmerge*. This program generates a random array and sorts it using the Parallel Mergesort Algortihm. 
