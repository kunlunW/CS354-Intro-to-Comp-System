////////////////////////////////////////////////////////////////////////////////
// Main File:        None
// This File:        intdate.c
// Other Files:      sendsig.c division.c
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
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

// first, we need to declare some global vars
// 1. set the alarm timer = 3 second
// 2. set a user signal counter
int timer = 3;  
int userSigCounter = 0; 

/* 
 * This function serves to be the handler for SIGALRM
 * The function will print out PID and current time. 
 */
void SIGALRM_Handler() {
    time_t currentTime;
    currentTime = time(NULL);
    // Error checking for time() :
    // we need to check the return value of the time() function
    // if the return value is -1, that means time() fails
    if (currentTime == -1) { 
        printf("Error: failure to access current time.\n");
        //exit(0);
        exit(1);
    }
    // Error checking for ctime(): 
    // if ctime() returns NULL, then it fails
    char *string = ctime(&currentTime);
    if (string == NULL) { 
        printf("Error: failure to convert current time to string. \n");
        exit(1);
    }

    // getpid() function is always successful, so there's no need to check it
    printf("PID: %d", getpid());
    printf(" | Current time: %s", string);

    // Error checking for alarm() 
    if (alarm(timer) != 0) {  
        printf("Error: failure in alarm().\n");
        exit(1);
    }
    // send a SIGALRM signal, print time message every 3 secs
    alarm(timer);  
}

/* 
 * This function serves to be the handler for SIGUSR1. 
 * It will print a message when a SIGUSR1 signal is caught
 * and it will increase the counter 
 */
void SIGUSR1_Handler() {
    printf("SIGUSR1 caught!\n");
    userSigCounter++;  
}

/* 
 * This function serves to be the handler for SIGINT. 
 * It will catch the INT signal and print out corresponding
 * messages
 */
void  SIGINT_Handler() {
    printf("\nSIGINT received.\n");
    printf("SIGUSR1 was received %i times. Exiting now.\n", userSigCounter);
    exit(0);
}

/*
 * The main function serves to create an infinite loop. 
 * An alarm will be triggered every 3 seconds. 
 * The function will respond to the various signals. 
 */
int main(int argc, char *argv[]) {
    // print start message 
    printf("Pid and time will be printed every 3 seconds.\n");
    printf("Enter ^C to end the program.\n");
    // handling SIGALARM
    struct sigaction alrm;
    memset (&alrm, 0, sizeof(alrm));
    alrm.sa_handler = SIGALRM_Handler;
    // error checking for sigaction 
    if (sigaction(SIGALRM, &alrm, NULL) < 0) {
        printf("Error: failure in handling for SIGALARM");
        exit(0);
    }
    if (alarm(timer) != 0) { 
        printf("Error: Failure in Alarm.\n");
        exit(1);
    }
    // handing SIGUSR1
    struct sigaction user1;
    memset (&user1, 0, sizeof(user1));
    user1.sa_handler = SIGUSR1_Handler;
    // error checking for sigaction() :
    if (sigaction(SIGUSR1, &user1, NULL) < 0) {
        printf("Error: failure in handling for SIGUSR1");
        exit(1);
    }
    // Signal handler for SIGINT
    struct sigaction interrupt;
    memset(&interrupt, 0, sizeof(interrupt));
    interrupt.sa_handler = SIGINT_Handler;
    // error checking for sigaction ();
    if (sigaction(SIGINT, &interrupt, NULL) < 0) {
        printf("Error: failure in handling for SIGINT");
        exit(1);
    }
    // this will create an infinite loop
    while (1) {
    }

    return (0);
}
