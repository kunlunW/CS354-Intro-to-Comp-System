////////////////////////////////////////////////////////////////////////////////
// Main File:        None
// This File:        division.c
// Other Files:      intdate.c sendsig.c
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
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int successCounter;  // Counter of the number of successful division

/*
 * This function serves to handle all the float point error signals. 
 * When such a signal is caught, it will print an error message. And output
 * the final count of successful operations 
 */
void SIGFPE_Handler() {
    // print out error message
    printf("Error: a division by 0 operation was attemped.\n");
    // print number of successful operations
    printf("Total number of operations completed successfully: %i\n",
           successCounter);
    printf("The program will be terminated.\n");
    exit(0);
}

/*
 * This fuction serves to handle all SIGINT signals. 
 * When such a signal is caught, it will print out the number of successful operations. 
 */
void handler_SIGINT() {
    // print number of successful operations
    printf("\nTotal number of operations successfully completed: %i\n",
           successCounter);
    printf("The program will be terminated.\n");
    exit(0);
}

/*
 * This function serves to create an infinite loop. 
 * The function will continally prompt the user for input, and it will output the quotient and
 * the remainder for each operation. And finally it will print out the number of  successful 
 * divisions. 
 * 
 * The program can only end when there is a divide by 0
 * SIGFPE will be sent when there is a divide by zero error, and the program will terminate
 * SIGINT signal will be sent when user enters Ctr+C, and then it will display the number 
 * of successful operations and then the program will exit.
 */
int main() {
    successCounter = 0;
    // handling SIGFPE signal
    struct sigaction fpe;
    memset (&fpe, 0, sizeof(fpe));
    fpe.sa_handler = SIGFPE_Handler;
    if (sigaction(SIGFPE, &fpe, NULL) < 0) {
        printf("Error: failire in handling for SIGFPE signal.\n");
        exit(0);
    }
    // handling SIGINT signal
    struct sigaction interrupt;
    memset(&interrupt, 0, sizeof(interrupt));
    interrupt.sa_handler = handler_SIGINT;
    if (sigaction(SIGINT, &interrupt, NULL) < 0) {
        printf("Error: failure in handling for SIGINT signal.\n");
        exit(0);
    }
    // This will create the infinite division loop
    while (1) {  
        // char buffer[100];
        printf("Enter first integer: ");
        // declare a large enough char array to store the first input
        char firstBuffer[100];
        char *firstNum = fgets(firstBuffer, 100, stdin);
        // Error checking for fgets();
        if (firstNum == NULL ){
            printf("Error: Failure in reading the first input\n");
            exit(0);
        }
        // now convert it to int 
        int firstInteger = atoi(firstBuffer);
        
        // getting the next input 
        printf("Enter second integer: ");
        char secondBuffer[100];
        char *secondNum = fgets(secondBuffer, 100, stdin);
        // Error checking for fgets():
        if (secondNum == NULL ){
            printf("Error: Failure in reading the second input\n");
            exit(0);
        }
        // convert to int 
        int secondInteger = atoi(secondBuffer);  
        // getting the quotient
        int quotient = firstInteger / secondInteger;
        // getting the remainder
        int remainder = firstInteger % secondInteger;  

        printf("%i / %i is %i with a remainder of %i\n",
        firstInteger, secondInteger, quotient, remainder);
        // increment the success count 
        successCounter++;  
    }
    return (0);
}
