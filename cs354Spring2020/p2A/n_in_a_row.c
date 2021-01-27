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


/* TODO:
 * Returns 1 if and only if the board is in a valid state.
 * Otherwise returns 0.
 * 
 * board: heap allocated 2D board
 * size: number of rows and columns
 */

int n_in_a_row(int **board, int size) {
    int counterForOnes = 0;
    int counterForTwos = 0;

    int colAllOnes = 0; 
    int colAllTwos = 0; 
    int rowAllOnes = 0; 
    int rowAllTwos = 0; 

    int leftDiagAllOnes = 0; 
    int leftDiagAllTwos = 0; 
    int rightDiagAllOnes = 0; 
    int rightDiagAllTwos = 0; 


    int colAllOnesPosition = 0; 
    int rowAllOnesPosition = 0; 
    int colAllTwosPosition = 0; 
    int rowAllTwosPosition = 0; 
    
    // first of all, we need to check if there only exists 0,1,2 in the
    // chess board
    for(int i=0; i<size; i++) {
    	for(int j=0; j<size; j++) {
		int number = *(*(board+i)+j);
		if (number != 0 && number != 1 && number != 2) {
		}
        }
    }

    // we also need to check if the number of 1s > number of 2s
    for(int i=0; i<size; i++) {
    	for(int j=0; j<size; j++) {
		    if(*(*(board+i)+j) == 1){
			    counterForOnes++;
		    }
		    if(*(*(board+i)+j) == 2) {
			    counterForTwos++;
		    }
	    }
    }

    if( counterForOnes < counterForTwos || counterForOnes - counterForTwos >1 ) {
	return 0;
    }

    
    // now we need to check row that is all 1 or 2 
    counterForOnes = 0;
    counterForTwos = 0;
    
    for(int i=0; i<size; i++) {
    	for(int j=0; j<size; j++) {
    		if(*(*(board+i)+j) == 1) {
			    counterForOnes++;
		    }
		    if(*(*(board+i)+j) == 2) {
			    counterForTwos++;
            }
        }
	if(counterForOnes == size) {
		rowAllOnes++;
		rowAllOnesPosition = i;
	}
	if(counterForTwos == size) {
		rowAllTwos++;
		rowAllTwosPosition = i;
	}
    }
    
    if((rowAllOnes>0 && rowAllTwos>0) || (rowAllOnes>1) || (rowAllTwos>1)) {
    	return 0;
    }

    
	// now we need to check column that is all 1 or 2 
    counterForOnes = 0;
    counterForTwos = 0;
    for(int i=0; i<size; i++) {
    	for(int j=0; j<size; j++) {
    		if(*(*(board+j)+i) == 1) {
			    counterForOnes++;
		    }
		    if(*(*(board+j)+i) == 2) {
			    counterForTwos++;
		    }
        }
	if(counterForOnes == size) {
		colAllOnes++;
		colAllOnesPosition = i;
	}
	if(counterForTwos == size) {
		colAllTwos++;
		colAllTwosPosition = i;
	}
    }
    if((colAllOnes>0 && colAllTwos>0) || (colAllOnes>1) || (colAllTwos>1)) {
    	return 0;
    }
    

    // now we can start checking the diagnals
    // 
    //              \ 0 0 0
    //              0 \ 0 0
    //              0 0 \ 0
    //              0 0 0 \ 
    // 
    // we can define the above diagnal as right diagnal
        counterForOnes = 0;
        counterForTwos = 0;
    for(int i=0; i<size; i++) {
	    if(*(*(board+i)+i) == 1) {
		    counterForOnes++;
	    }
	    if(*(*(board+i)+i) == 2) {
		    counterForTwos++;
	    }
    }

    if(counterForOnes == size) {
		rightDiagAllOnes = 1;
	}
	if(counterForTwos == size) {
		rightDiagAllTwos = 1;
	}
	
    // now we need to check the left diagnal
    // 
    //          0 0 0 /
    //          0 0 / 0
    //          0 / 0 0
    //          / 0 0 0
    //
    // we can define the above diagnal as left diagnal 
    counterForOnes = 0;
    counterForTwos = 0;
    for(int i=0; i<size; i++) {
	if(*(*(board+i)+(size-1-i)) == 1) {
		counterForOnes++;
	}
	if(*(*(board+i)+(size-1-i)) == 2) {
		counterForTwos++;
	}
    }

    if(counterForOnes == size) {
		leftDiagAllOnes = 1;
	}
	if(counterForTwos == size) {
		leftDiagAllTwos = 1;
	}
	counterForOnes = 0;
    counterForTwos = 0;


    //cannot have a diag of 1 and a diag of 2
    if((leftDiagAllTwos==1 && rightDiagAllOnes==1) || (leftDiagAllOnes==1 && rightDiagAllTwos==1)) {
    	return 0;
    }
   


    // now we need to list out some invalid configurations
    if(colAllOnes == 0 && colAllTwos == 0 && rowAllOnes == 0 && rowAllTwos == 0 
    && rightDiagAllOnes == 0 && rightDiagAllTwos == 0 && leftDiagAllOnes == 0 && leftDiagAllTwos == 0) {
	return 0;
    }


    if(colAllOnes == 1 && colAllTwos == 0 && rowAllOnes == 1 && rowAllTwos == 0 
    && rightDiagAllOnes == 1 && rightDiagAllTwos == 0 && leftDiagAllOnes == 0 && leftDiagAllTwos == 0) {
	    if(rowAllOnesPosition != colAllOnesPosition) {
		    return 0;
	    }
    }

    if(colAllOnes == 0 && colAllTwos == 1 && rowAllOnes == 0 && rowAllTwos == 1 
    && rightDiagAllOnes == 0 && rightDiagAllTwos == 1 && leftDiagAllOnes == 0 && leftDiagAllTwos == 0) {
	    if(rowAllTwosPosition != colAllTwosPosition) {
		    return 0;
	    }
    }



    if(colAllOnes == 1 && colAllTwos == 0 && rowAllOnes == 1 && rowAllTwos == 0 
    && rightDiagAllOnes == 0 && rightDiagAllTwos == 0 && leftDiagAllOnes == 1 && leftDiagAllTwos == 0) {
	    if(rowAllOnesPosition != (size-1)-colAllOnesPosition) {
		    return 0;
	    }
    }

    if(colAllOnes == 0 && colAllTwos == 1 && rowAllOnes == 0 && rowAllTwos == 1 
    && rightDiagAllOnes == 0 && rightDiagAllTwos == 0 && leftDiagAllOnes == 0 && leftDiagAllTwos == 1) {
	    if(rowAllTwosPosition != (size-1)-colAllTwosPosition) {
		    return 0;
	    }
    }
    return 1;   
}     



int main(int argc, char *argv[]) {              

    //TODO: Check if number of command-line arguments is correct.
    if (argc != 2) {
            printf("Wrong number of arguments\n");
            exit(1);
        }

    //Open the file and check if it opened successfully.
    FILE *fp = fopen(*(argv + 1), "r");
    if (fp == NULL) {
        printf("Cannot open file for reading\n");
        exit(1);
    }


    //Declare local variables.
    int size = 0;
    //Declare pointer to size
    int *sizep;
    sizep = &size;


    //TODO: Call get_dimensions to retrieve the board size.
    get_dimensions(fp, sizep);
    size = *sizep;



   

    //TODO: Dynamically allocate a 2D array of dimensions retrieved above.
    int **a = malloc(sizeof(int*)*size);
    for(int i=0; i<size; i++)
    {
    	*(a+i) = malloc(sizeof(int)*size);
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
		

            *(*(a+i)+j) = atoi(token);
            token = strtok(NULL, COMMA);
        }
    }

    //TODO: Call the function n_in_a_row and print the appropriate
    //output depending on the function's return value.
    int valid = 0;
    valid = n_in_a_row(a, size);
    if(valid==1)
    	printf("valid\n");
    if(valid==0)
	printf("invalid\n");

    //TODO: Free all dynamically allocated memory.
    for(int i=0; i<size; i++)
    {
	free(*(a+i));
    }
    free(a);
    
    //Close the file.
    if (fclose(fp) != 0) {
        printf("Error while closing the file\n");
        exit(1);
    } 

    return 0;       
} 
