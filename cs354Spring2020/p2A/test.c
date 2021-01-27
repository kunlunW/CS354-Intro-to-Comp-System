///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2019 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
//
////////////////////////////////////////////////////////////////////////////////
// Main File:        n_in_a_row.c
// This File:        n_in_a_row.c
// Other Files:      None
// Semester:         CS 354 Fall 2019
//
// Author:           Kunlun Wang
// Email:            kwang358@wisc.edu
// CS Login:         kunlun
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Bohong Chen (bchen246@wisc.edu)
// 		     Me and Bohong discussed about designing algorithms to cope with 
// 		     complex situations such as double wins or triple wins. Above situation
// 		     can happen if and only if the last move forms the intersecting point 
// 		     of two lines(must be a column and a row) or three lines (must be
// 		     a column, a row, and a diagonal).
// 		    
// 		     Such two lines must follow a strict standard designed by
// 		     us in order for it to count as a valid configuration. 
// 		     1) Double win situation: The algorithm for determining such a 
// 		     situation is as follow:
//
// 		     if two lines intersecting each other,if the first line is either 
// 		     a column or row, and it is intersecting with a diagnal, it would be fine.
// 		     But if a column is intersecting a row, then the i and j of column and row has
// 		     to match exactly 
//
// 		     2) Triple wins: the triple wins can only happen if a column, a row, and a diagnal 
// 		     intersect with each other, and they have to be the same number, the i and j of column 
// 		     and row has to be the same for right diagnol, and the i has to be the same as size-1-currentRowAllOne/twos 
// 		     for the left diagonal situation for it to be valid  
//                   
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of
//                   of any information you find.
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>  
#include <stdlib.h>
#include <string.h>     

char *COMMA = ",";  

/* COMPLETED:      
 * Retrieves from the first line of the input file,
 * the size of the board (number of rows and columns).
 * 
 * fp: file pointer for input file
 * size: pointer to size
 */
void get_dimensions(FILE *fp, int *size) {      
    char *line = NULL;
    size_t len = 0;
    if (getline(&line, &len, fp) == -1) {
        printf("Error in reading the file\n");
        exit(1);
    }

    char *token = NULL;
    token = strtok(line, COMMA);
    *size = atoi(token);
}   


/* TODO:Complete!! 
 * Returns 1 if and only if the board is in a valid state.
 * Otherwise returns 0.
 * 
 * board: heap allocated 2D board
 * size: number of rows and columns
 */
int n_in_a_row(int **board, int size) {
	int countOnes = 0;
	int countTwos = 0;
	int winnerCount =0;
	int colAllOnes =0;
	int colAllTwos =0;
	int rowAllOnes =0;
	int rowAllTwos =0;
	int leftDiagAllOnes =0;
	int leftDiagAllTwos =0;
	int rightDiagAllOnes =0;
	int rightDiagAllTwos =0;
	int currentRowWithAllOnes=0;
	int currentRowWithAllTwos=0;
	int currentColWithAllOnes=0;
	int currentColWithAllTwos=0;  
	

	// This segment of code serves to checking if the number 
	// of ones/twos is greater than the other 
	// by one. Also record the row# of any possible allOnes and
	// allTwos.	
	for (int i=0; i<size; i++) {
	for (int j=0; j<size; j++) {
		if (*(*(board+i)+j) ==1) countOnes++;
		if (*(*(board+i)+j) ==1) countTwos++;
	}
	if (countOnes == size){
		rowAllOnes++;
		currentRowWithAllOnes =i;	
	}
	if (countTwos == size) {
		rowAllTwos++;
		currentRowWithAllTwos =i;
	}
	}
	if ((rowAllOnes>0&&rowAllTwos>0)||(rowAllOnes>1) ||(rowAllTwos>1)) {
		return 0;
	}
	if (countOnes-countTwos !=1 || countTwos-countOnes !=1) {
		return 0;
	}
	countOnes=0;
	countTwos=0;

	// This segment of code checks if there's any invalid element in the tik-tak-tor
	// configuration such that only 0,1,2, is allowed as elements.
	for (int i=0; i<size; i++) {
		for (int j=0; j<size; j++) {
			if (*(*(board+i)+j) != 0 || *(*(board+i)+j) !=1 || *(*(board+i)+j) !=2) {
			return 0; 
	}
	}
	}

	// This segment of code serves to check if there's any allOnes and allTwos
	// situation in columns.
	for (int i=0; i<size; i++) {
		for (int j=0; j<size; j++) {
			if(*(*(board+i)+j)==1) countOnes++;
			else if (*(*(board+i)+j)==2) countTwos++;
	}
		if (countOnes == size ||  countTwos ==size) winnerCount++;
		if (countOnes == size) {
			rowAllOnes++;
			currentRowWithAllOnes = i;
		}
		if (countTwos == size) {
			rowAllTwos++;
			currentRowWithAllTwos =i;
		}
		countOnes=0;
		countTwos=0;
	}

	// if we have a situation such that 1) both there exists a column with 
	// all ones and a column with all twos 2) if there are more than one column 
	// with all ones or there are more than one column with all twos.
	// Then it is a violation to the configuration of the rule 
	if ((rowAllOnes>0 && rowAllTwos>0) || (rowAllOnes>1) ||(rowAllTwos>1)) {
		return 0;
	}
//	if (winnerCount>1){
//	return 0;
//	}
	countOnes =0;
	countTwos =0;

	// This segment of code serves to count the ones and twos for right diagnal
	// It also checks  
	for (int i=0; i<size; i++) {
	
		if (*(*(board+i)+i)==1) countOnes++;
		else if (*(*(board+i)+i)==2) countTwos++;
	}
	
	if (countOnes == size) {
		rightDiagAllOnes++;
	}
	if (countTwos == size) {
		rightDiagAllTwos++;
	}
	//if (countOnes == size || countTwos == size) winnerCount++;
	//if (winnerCount>1) {
	//return 0;
	//}
	countOnes =0;
	countTwos =0;
	// This segment of code serves to count ones and twos for the left diagnal
	// It also checks  
	for (int i=0; i<size; i++) {
		if (*(*(board+i)+(size-1-i))==1) countOnes++;
		else if(*(*(board+i)+(size-1-i))==2) countTwos++;
	}
	if (countOnes == size) {
		leftDiagAllOnes++;
	}
	if (countTwos == size) {
		leftDiagAllTwos++;
	}
	//if (countOnes ==size || countTwos==size) winnerCount++;
	//if (winnerCount>1) {
	//return 0;
	//}

	countOnes=0;
	countTwos=0;
	// checking the winner count for columns 
	for (int j=0; j<size; j++) {
		for (int i=0; i<size; i++) {
		if (*(*(board+i)+j)==1) countOnes++;
		else if (*(*(board+i)+j)==1) countTwos++;
	}
	if (countOnes == size) {
		colAllOnes++;
		currentColWithAllOnes = j;
	}

	if (countTwos == size) {
		colAllTwos++;
		currentColWithAllTwos =j;
	}
	//	if (countOnes == size|| countTwos == size) winnerCount++;
	countOnes=0;
	countTwos=0;
	
	}
	//if (winnerCount>1) {
	//return 0;
	//
	if ((colAllOnes>0&&colAllTwos>0) || (colAllOnes>1) ||(colAllTwos>1)) {
		return 0;
	}
	
	// in this situation, we have three lines intersecting each others, this is only
	// valid if and only if there exist only one column of same#, one row of same#,
	// and one diagnal of same#. And the number of row counting from top and number 
	// of column counting from left has to match exactly such that i=j, if not, then
	// invalid. 
	if (colAllOnes ==1 && colAllTwos ==0&& rowAllOnes ==1 && rowAllTwos==0 &&
	       	rightDiagAllOnes==1 && rightDiagAllTwos ==0 && leftDiagAllOnes==0
		&& leftDiagAllTwos==0) {
		if (currentRowWithAllOnes!= currentColWithAllOnes) {
			return 0;
	}
	}
	
	// if this situation, there are three lines intersecting, a column of 2,
	// a row of 2, and a right diagnal of 2. 
	// To deal with this situation, me and Bohong proposed that the number
	// of row counting from top and the number of number of column counting 
	// from left has to match exactly for it to be valid.  
	if (colAllOnes ==0 && colAllTwos ==1 &&rowAllOnes ==0 && rowAllTwos ==1&&
	rightDiagAllOnes==0&&rightDiagAllTwos==1&&leftDiagAllOnes==0&&leftDiagAllTwos==0) {
		if (currentRowWithAllTwos != currentColWithAllTwos) {
			return 0;
	}
	}
	
	 // In this situation, there are three lines intersecting, a column of 2,
        // a row of 2, and a right diagnal of 2.
	// Very similar to above situation, but this time we have a left diagnol
        // To deal with this situation, me and Bohong devised an algorithm such that
	// the number of row counting from top has to match the number of column counting
	// from right which can be quantified by the equation:
	// size-1-currentColAllOnes 
	if (colAllOnes ==1 && colAllTwos ==0 &&rowAllOnes ==1 && rowAllTwos ==0
	&&rightDiagAllOnes==0&&rightDiagAllTwos==0&&leftDiagAllOnes==1&&leftDiagAllTwos==0){
		if (currentRowWithAllOnes != (size-1)-currentColWithAllOnes) {
			return 0;
	}
	}

	if (colAllOnes ==0 && colAllTwos ==1 &&rowAllOnes ==0 && rowAllTwos ==1&&
	rightDiagAllOnes==0&&rightDiagAllTwos==0&&leftDiagAllOnes==0&&leftDiagAllTwos==1){ 
  		if (currentRowWithAllTwos != (size-1)-currentColWithAllTwos) {
        	return 0;
        }
        }
	
    return 1;   
}     


/* PARTIALLY COMPLETED:
 * This program prints Valid if the input file contains
 * a game board with either 1 or no winners; and where
 * there is at most 1 more X than O.
 * 
 * argc: CLA count
 * argv: CLA value
 */
int main(int argc, char *argv[]) {              

    //TODO: Check if number of command-line arguments is correct.--> COMPLETE!
	if (argc != 2) {
		printf("Missing arguments\n");
		exit(1);
	}

    //Open the file and check if it opened successfully.
    FILE *fp = fopen(*(argv + 1), "r");
    if (fp == NULL) {
        printf("Cannot open file for reading\n");
        exit(1);
    }


    //Declare local variables.
    int size;


    //TODO: Call get_dimensions to retrieve the board size.
	int *sizeOfPointer;
	sizeOfPointer = &size;
		get_dimensions(fp, sizeOfPointer);
		size = *sizeOfPointer;
		printf("The size of the matrix is %d\n", size);

    //TODO: Dynamically allocate a 2D array of dimensions retrieved above.
	int **a = malloc(sizeof(int*)*size);
	for (int i =0; i<size; i++) {
		*(a+i) =malloc (sizeof (int)*size);
	}
    //Read the file line by line.
    //Tokenize each line wrt comma to store the values in your 2D array.
    char *line = NULL;
    size_t len = 0;
    char *token = NULL;
    for (int i = 0; i < size; i++) {
        if (getline(&line, &len, fp) == -1) {
            printf("Error while reading the file\n");
            exit(1);
        }
        token = strtok(line, COMMA);
        for (int j = 0; j < size; j++) {
            //TODO: Complete the line of code below
            //to initialize your 2D array.
            *(*(a+i)+j)  = atoi(token);
            token = strtok(NULL, COMMA);
        }
    }

    //TODO: Call the function n_in_a_row and print the appropriate
    //output depending on the function's return value.
	int isValidTikTakToe =0;
	isValidTikTakToe = n_in_a_row(a, size);
	if (isValidTikTakToe ==1) {
		printf("Valid\n");
	}
	if (isValidTikTakToe ==0) {
		printf("invalid\n");
	}
    //TODO: Free all dynamically allocated memory.
	for (int i=0; i<size; i++) {
		free (*(a+i));
	}
	free(a);
    //Close the file.
    if (fclose(fp) != 0) {
        printf("Error while closing the file\n");
        exit(1);
    } 

    return 0;       
}    
