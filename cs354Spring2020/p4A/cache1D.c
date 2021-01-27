////////////////////////////////////////////////////////////////////////////////
// Main File:        None
// This File:        cache1D.c
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

// we need to create a a global int array of size 100000
int array[100000];

/*
 * Assign each element of the array to its corresponding index
 * Returns 0
 * 
 */
int main() {
    //update the value of each element
    for(int i = 0; i < 100000; i++) {
        array[i] = i;
    }
    return 0;
} 
