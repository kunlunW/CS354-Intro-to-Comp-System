////////////////////////////////////////////////////////////////////////////////
// Main File:        None
// This File:        sendsig.c
// Other Files:      intdate.c division.c
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
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

/*
 * This fuction serves to send signals to some process PID. 
 * It will respond differently according to the user's input.
 * The function can respond to two types of signals 
 * 1) SIGUSR1 (-u) 
 * 2) SIGINT (-i)
 */
int main(int argc, char* argv[]) {
    // we are expecting 3 command line arguments 
    // 1) sendsig 2) -u/-i 3)pid
    if (argc != 3) {  
        printf("Usage: <signal type> <pid>\n");
        exit(0);
    }
    
    // if the signal type is -u --> user defined signal 
    if (strcmp(argv[1], "-u") == 0) {  
        if (kill(atoi(argv[2]), SIGUSR1) == -1) {
            printf("Error: failure to send SIGUSR1 signal.\n");
            exit(0);
        }
    } 
    // if the signal type is -i 
    else if (strcmp(argv[1], "-i") == 0) { 
        if (kill(atoi(argv[2]), SIGINT) == -1) {
            printf("Error: failure to send SIGINT signal.\n");
            exit(0);
        }
    } 
    // if the type is not recognized
    else {  
        printf("Error: parameter not recognized.\n");
        exit(0);
    }
    return 0;
}
