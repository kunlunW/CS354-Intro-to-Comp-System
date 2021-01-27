////////////////////////////////////////////////////////////////////////////////
// Main File:        None
// This File:        cache2Dcols.c
// Other Files:      cache1D.c, cache2Drows.c, cache2Dcols.c, cache2Dclash.c
// Semester:         CS 354 Spring2020
//
// Author:           Kunlun Wang
// Email:            kwang358@wisc.edu
// CS Login:         kunlun
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          None
//                   
//
// Online sources:   None
//                   
//                  
////////////////////////////////////////////////////////////////////////////////

// We need to create a 2D array with 3000 rows and 500 columns
int arr2D[3000][500];

/*
 * update each element of the array to be the sum of their row and column
 * but only using a reverse loop as the cache2Drows.c
 * Return 0
 */
int main() {
   //Iterate cols first
    for(int col = 0; col < 500; col++) {
      //Iterate rows second
        for(int row = 0; row < 3000; row++) {
            // update value 
	        arr2D[row][col] = row + col;
        }
    }
    return 0;
}
