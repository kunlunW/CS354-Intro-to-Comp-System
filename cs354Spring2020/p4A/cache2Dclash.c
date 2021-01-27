////////////////////////////////////////////////////////////////////////////////
// Main File:        None
// This File:        cache2Dclash.c
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

// We need to create a 2D array with 128 rows and 8 columns
int arr2D[128][8];

/*
 * update the value of each element to be the sum of their row, column, and iteration
 * Return 0
 */
int main() {
    // 100 times interation 
    for(int iteration = 0; iteration < 100; iteration++) {
        // iterate rows 
        for(int row = 0; row < 128; row += 64) {
	        // interate cols 
	        for(int col = 0; col < 8; col++) {
                // update value 
	            arr2D[row][col] = iteration + row + col;
	        }
        }
    }
    return 0;
}
