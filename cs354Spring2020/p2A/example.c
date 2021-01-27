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
    int cnt1 = 0;
    int cnt2 = 0;

    //To check if the alphabet is {0,1,2}
    for(int i=0; i<size; i++)
    {
    	for(int j=0; j<size; j++)
	{
		int temp = *(*(board+i)+j);
		if(temp != 0 && temp != 1 && temp != 2)
		{
//printf(" cnt %d \n", temp);
			//return 0;
		}
        }

    }

    //To check if one is greater than the other by 1
    for(int i=0; i<size; i++)
    {
    	for(int j=0; j<size; j++)
        {
		if(*(*(board+i)+j) == 1)
		{
			cnt1++;
		}
		if(*(*(board+i)+j) == 2)
		{
			cnt2++;
		}
	}
    }
//printf(" cnt %d %d\n", cnt1,cnt2);	
    if( cnt1<cnt2 || cnt1-cnt2>1 )
    {
	return 0;
    }

    int c1 = 0; //number of cols with all 1's
    int c2 = 0; //number of cols with all 2's
    int r1 = 0; //number of rows with all 1's
    int r2 = 0; //number of rows with all 2's
    int ld1 = 0; //it is 1 when a left bot to right top diagonal of all 1's is detected
    int ld2 = 0; //it is 1 when a left bot to right top diagonal of all 2's is detected
    int rd1 = 0; //it is 1 when a right bot to right top diagonal of all 1's is detected
    int rd2 = 0; //it is 1 when a left bot to right top diagonal of all 2's is detected
    int curc1 = 0; //when only one c1, record the col number
    int curr1 = 0; //when only one r1, record the row number
    int curc2 = 0; //when only one c2, record the col number
    int curr2 = 0; //when only one r2, record the row number

    //check the row of all 1 or all 2
    cnt1 = 0;
    cnt2 = 0;
    
    for(int i=0; i<size; i++)
    {
    	for(int j=0; j<size; j++)
    	{
    		if(*(*(board+i)+j) == 1)
		{
			cnt1++;
		}
		if(*(*(board+i)+j) == 2)
		{
			cnt2++;
		}
        }

	if(cnt1 == size)
	{
		r1++;
		curr1 = i;
	}
	if(cnt2 == size)
	{
		r2++;
		curr2 = i;
	}
	cnt1 = 0;
        cnt2 = 0;
    }
    
    if((r1>0 && r2>0) || (r1>1) || (r2>1)) //if there are more than one r1 or r2, or both r1 and r2 are detected, invalid
    {
    	return 0;
    }

    //Here the sample should be: r1=0 r2=0, r1=1 r2=0 , or r1=0 r2=1	

    //check the column of all 1 or all 2		
		
    for(int i=0; i<size; i++)
    {
    	for(int j=0; j<size; j++)
    	{
    		if(*(*(board+j)+i) == 1)
		{
			cnt1++;
		}
		if(*(*(board+j)+i) == 2)
		{
			cnt2++;
		}
        }

	if(cnt1 == size)
	{
		c1++;
		curc1 = i;
	}
	if(cnt2 == size)
	{
		c2++;
		curc2 = i;
	}
	cnt1 = 0;
        cnt2 = 0;
    }
    
    if((c1>0 && c2>0) || (c1>1) || (c2>1)) //if there are more than one c1 or c2, or both c1 and c2 are detected, invalid
    {
    	return 0;
    }
    //Here the sample should be: c1=0 c2=0, c1=1 c2=0, or c1=0 c2=1

    //Since if you have a c1(c2) you can't have a r2(r1), now the sample should be: c1=c2=r1=r2=0, c1=r1=1 c2=r2=0, or c1=r1=0 c2=r2=1

    //check right bot to left top diag - rd

    for(int i=0; i<size; i++)
    {
	if(*(*(board+i)+i) == 1)
	{
		cnt1++;
	}
	if(*(*(board+i)+i) == 2)
	{
		cnt2++;
	}
    }

    if(cnt1 == size)
	{
		rd1 = 1;
	}
	if(cnt2 == size)
	{
		rd2 = 1;
	}
	cnt1 = 0;
        cnt2 = 0;

    //check left bot to right top diag - ld

    for(int i=0; i<size; i++)
    {
	if(*(*(board+i)+(size-1-i)) == 1)
	{
		cnt1++;
	}
	if(*(*(board+i)+(size-1-i)) == 2)
	{
		cnt2++;
	}
    }

    if(cnt1 == size)
	{
		ld1 = 1;
	}
	if(cnt2 == size)
	{
		ld2 = 1;
	}
	cnt1 = 0;
        cnt2 = 0;
//printf("%d %d\n", c1,r1);
//printf("%d %d\n", c2,r2);
//printf("%d %d\n", ld1,ld2);
//printf("%d %d\n", rd1,rd2);

    //cannot have a diag of 1 and a diag of 2
    if((ld2==1 && rd1==1) || (ld1==1 && rd2==1))
    {
    	return 0;
    }
    //Now the sample should be: no win, one column or row win, a col and a row win of the same kind, and a col, a row, a ld(rd) win of the same kind


    //check the relation of the row, col and diag, they can only have one intersection
    if(c1 == 0 && c2 == 0 && r1 == 0 && r2 == 0 && rd1 == 0 && rd2 == 0 && ld1 == 0 && ld2 == 0)
    {
	return 0;
    }



    if(c1 == 1 && c2 == 0 && r1 == 1 && r2 == 0 && rd1 == 1 && rd2 == 0 && ld1 == 0 && ld2 == 0)
    {
	if(curr1 != curc1)
	{
		return 0;
	}
    }

    if(c1 == 0 && c2 == 1 && r1 == 0 && r2 == 1 && rd1 == 0 && rd2 == 1 && ld1 == 0 && ld2 == 0)
    {
	if(curr2 != curc2)
	{
		return 0;
	}
    }



    if(c1 == 1 && c2 == 0 && r1 == 1 && r2 == 0 && rd1 == 0 && rd2 == 0 && ld1 == 1 && ld2 == 0)
    {
	if(curr1 != (size-1)-curc1)
	{
		return 0;
	}
    }

    if(c1 == 0 && c2 == 1 && r1 == 0 && r2 == 1 && rd1 == 0 && rd2 == 0 && ld1 == 0 && ld2 == 1)
    {
	if(curr2 != (size-1)-curc2)
	{
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

    //TODO: Check if number of command-line arguments is correct.
    if (argc != 2) {
            printf("Error while passing the argument\n");
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

