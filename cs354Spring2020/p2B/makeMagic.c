/////////////////////////////////////////////////////////////////////////////// 
// 
// Copyright 2020 Jim Skrentny 
// Posting or sharing this file is prohibited, including any changes/additions. 
// 
//////////////////////////////////////////////////////////////////////////////// 
// Main File:        makeMagic.c
// This File:        makeMagic.c
// Other Files:      None
// Semester:         CS 354 Spring 2020 
// 
// Author:           Kunlun Wang  
// Email:            kwang358@wisc.edu
// CS Login:         kunlun
// 
/////////////////////////// OTHER SOURCES OF HELP ////////////////////////////// 
//                   Fully acknowledge and credit all sources of help, 
//                   other than Instructors and TAs. 
// 
// Persons:          Identify persons by name, relationship to you, and email. 
//                   Describe in detail the the ideas and help they provided. 
// 
// Online sources:   Avoid web searches to solve your problems, but if you do 
//                   search, be sure to include Web URLs and description of 
//                   of any information you find. 
//////////////////////////////////////////////////////////////////////////////// 
    
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
 
// Structure that represents a magic square 
typedef struct { 
    int size;     // dimension of the square 
    int **msquare; // pointer to heap allocated magic square 
} MSquare; 
 
/* TODO: 
 * Prompts the user for the magic square's size, reads it, 
 * checks if it's an odd number >= 3 (if not display the required 
 * error message and exit), and returns the valid number. 
 */ 
int getSize() { 
    int a;
    printf("Enter magic square's size (odd integer >= 3)\n");
    scanf("%d", &a);
    // printf("%d", a);

    // first need to check if the size is proper
    if (a < 3) {
        if (a % 2 == 0){
            printf("Size must be odd.\n");
            exit(1);
        }
        printf("Size must be >= 3.\n");
        exit(1);
    }
    if (a % 2 == 0) {
        printf("Size must be odd.\n");
        exit(1);
    }
    return a;    
}  
    
/* TODO: 
 * Makes a magic square of size n using the Siamese algorithm 
 * and returns a pointer to the completed MSquare struct. 
 * 
 * n the number of rosw and columns 
 */ 
MSquare *generateMSquare(int n) { 
    MSquare * magicSquare = malloc (sizeof(MSquare));
    // the size of the square
    magicSquare -> size = n;
    // allocate the rows
    magicSquare -> msquare = malloc(n * sizeof(int*));
    // we need to check the return value of malloc(),
    // if it is NULL -> then some error happened 
    if (magicSquare -> msquare == NULL) {
        printf("Allocation Failure.\n");
		exit(1);
    }
    // allocate the cols, here using calloc would be easier
    // since we don't have to add another loop to initiate 
    // all the elements in the heap to 0 
    for (int i = 0; i < n; i++) {
        *(magicSquare -> msquare + i) = calloc(n, sizeof(int));
        // we need to check if calloc() functions properly 
        if (*(magicSquare -> msquare + i) == NULL) {
            printf("Allocation Failure.\n");
			exit(1);
        }
    }
    // we start at the first row, middle col
    int currentRowIndex = 0;
    int currentColIndex = (n - 1)/ 2;

    for (int i =1; i <= n * n; i++) {
        *(*(magicSquare -> msquare+currentRowIndex) + currentColIndex) = i;
        // we need to move the cursor to the upper-right position
        // so we need to decrement the row count
        int successorRowIndex = currentRowIndex - 1;
        // we need to move the cursor to the upper-right position
        // so we need to increment the col count
        int successorColIndex = currentColIndex + 1;

        // if our cursor surpassed the upper boundary, then we need to 
        // move it to the bottom row -> (n-1)row 
        if (successorRowIndex < 0) {
            successorRowIndex = n -1;
        }
        // if our cursor surpassed the right boundary, we need to move it to
        // the left-most col
        if (successorColIndex >= n) {
            successorColIndex = 0;
        }
        if (*(*(magicSquare -> msquare + successorRowIndex)+successorColIndex) == 0){
            currentRowIndex = successorRowIndex;
            currentColIndex = successorColIndex;
        }
        // if the successor position has already been filled, we need to move to 
        // the position under it -> row++
        else{
            currentRowIndex++;
            if (currentRowIndex >= n){
                currentRowIndex = 0;
            }
        }
    }
    return magicSquare;
    }
         

/* TODO:   
 * Opens a new file (or overwrites the existing file) 
 * and writes the square in the specified format. 
 * 
 * msquare the magic square to output 
 * filename the name of the output file 
 */ 
void outputMSquare(MSquare *msquare, char *filename) { 
    // declare a file pointer 
    FILE* fileptr;
    // open file
    fileptr = fopen(filename, "w");
    // we need to check if fopen() functions properly
    if (fileptr == NULL) {
        printf("File Open Failure.\n");
		exit(1);
    }
    // save size and array to a local copy -> make it easier to 
    // use later 
    int size = msquare -> size;
    int **array = msquare -> msquare;
    // print the size 
    fprintf(fileptr,"%d", size); 
    // fill up the square 
    for (int i = 0; i < size; i++) {
        fprintf(fileptr, "\n");
        for (int j = 0; j < size; j++) {  
            if (j < size - 1){
                fprintf(fileptr, "%d,", *(*(array + i) + j));
            }
            else {
                fprintf(fileptr, "%d", *(*(array + i) + j));
            }
        }
    }
    fprintf(fileptr, "\n");
    if (fclose(fileptr) != 0) {
        printf("Error while closing the file\n");
        exit(1);
    } 
} 
 
/* TODO: 
 * Generates a magic square of the user specified size and 
 * output the quare to the output filename 
 */ 
int main(int argc, char *argv[]) { 
    // TODO: Check input arguments to get output filename 
    // check if there are only two arguments 
    if (argc != 2){
        printf("Usage: ./makeMagic <output_filename>\n");
        exit(1);
    }
    // get the size 
    int sizeOfSquare = getSize();
    // save the square 
    MSquare* magicSqr = generateMSquare( sizeOfSquare );
    // display the sqaure 
    outputMSquare(magicSqr, argv[1]);
    // free the pointers 
    for (int i = 0; i < magicSqr -> size; i++){
        free(*(magicSqr -> msquare + i));
    }
    // free the pointers 
    free(magicSqr -> msquare);
    free(magicSqr);
    magicSqr = NULL;
    return 0; 
}  
