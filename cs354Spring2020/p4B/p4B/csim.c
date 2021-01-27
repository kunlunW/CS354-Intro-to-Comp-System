////////////////////////////////////////////////////////////////////////////////
// Main File:        None
// This File:        csim.c
// Other Files:      csim.c
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
////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2013,2019-2020
// Posting or sharing this file is prohibited, including any changes/additions.
//
////////////////////////////////////////////////////////////////////////////////

/*
 * csim.c:  
 * A cache simulator that can replay traces (from Valgrind) and output
 * statistics for the number of hits, misses, and evictions.
 * The replacement policy is LRU.
 *
 * Implementation and assumptions:
 *  1. Each load/store can cause at most 1 cache miss plus a possible eviction.
 *  2. Instruction loads (I) are ignored.
 *  3. Data modify (M) is treated as a load followed by a store to the same
 *  address. Hence, an M operation can result in two cache hits, or a miss and a
 *  hit plus a possible eviction.
 */  

#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

/******************************************************************************/
/* DO NOT MODIFY THESE VARIABLES **********************************************/

//Globals set by command line args.
int b = 0; //number of block (b) bits
int s = 0; //number of set (s) bits
int E = 0; //number of lines per set

//Globals derived from command line args.
int B; //block size in bytes: B = 2^b
int S; //number of sets: S = 2^s

//Global counters to track cache statistics in access_data().
int hit_cnt = 0;
int miss_cnt = 0;
int evict_cnt = 0;

//Global to control trace output
int verbosity = 0; //print trace if set
/******************************************************************************/
  
  
//Type mem_addr_t: Use when dealing with addresses or address masks.
typedef unsigned long long int mem_addr_t;

//Type cache_line_t: Use when dealing with cache lines.
//TODO - COMPLETE THIS TYPE
typedef struct cache_line {                    
    char valid;
    mem_addr_t tag;
    //Add a data member as needed by your implementation for LRU tracking.
    int LRUTracking;
} cache_line_t;

//Type cache_set_t: Use when dealing with cache sets
//Note: Each set is a pointer to a heap array of one or more cache lines.
typedef cache_line_t* cache_set_t;
//Type cache_t: Use when dealing with the cache.
//Note: A cache is a pointer to a heap array of one or more sets.
typedef cache_set_t* cache_t;

// Create the cache (i.e., pointer var) we're simulating.
cache_t cache;  

int evictCounter = 0; 


/* TODO - COMPLETE THIS FUNCTION
 * init_cache:
 * Allocates the data structure for a cache with S sets and E lines per set.
 * Initializes all valid bits and tags with 0s.
 */                    
void init_cache() {      
    // B = 2^b, S = 2^s
    B = pow(2, b);
    S = pow(2, s);    
    // allocate cache with S sets 
    cache = malloc(S * sizeof(cache_set_t*)); 
    // error checking, check if allocation failed 
    if(cache == NULL) {
	    printf("Error: memory allocation failed. \n");
	    exit(1);
    }
    // allocate E lines for each single set 
    for(int i = 0; i < S; i++) { 
	    cache[i] = malloc(E * sizeof(cache_line_t));
        // error checking, check if allocation failed 
	    if(cache[i] == NULL) {
	        printf("Error: memory allocation failed. \n");
	        exit(1);
	    }
            // setting the default values to the structure
            for (int j = 0; j < E; j++) {
                cache[i][j].valid = 0;
                cache[i][j].tag = 0;
                cache[i][j].LRUTracking = 0;
            }
    }
}
  
/* TODO - COMPLETE THIS FUNCTION 
 * free_cache:
 * Frees all heap allocated memory used by the cache.
 */                    
void free_cache() {    
    // free every single set in the cache
    for(int i = 0; i < S; i++) {
        free(cache[i]);
    }
	// free the cache 
    free(cache);  
    // set the cache to NULL 
    cache = NULL;     
}
   
   
/* TODO - COMPLETE THIS FUNCTION 
 * access_data:
 * Simulates data access at given "addr" memory address in the cache.
 *
 * If already in cache, increment hit_cnt
 * If not in cache, cache it (set tag), increment miss_cnt
 * If a line is evicted, increment evict_cnt
 */                    
void access_data(mem_addr_t addr) {   
    // int t = 64 - s - b;
    // we can skip bitwisely b+s bits to get to the t bit 
    int tagBits = addr >> (s + b); 
    // skip b bits and eliminate t bits to get to the s bit 
    int setBits = (addr >> b) & (S - 1); 
    // this is the line number of the particular to be evicted 
    int evictedLine = 0; 
    // for every set 
    for(int i=0; i<E; i++) { 
	    // if the target is present in cache 
	    if((cache[setBits][i].tag == tagBits) && (cache[setBits][i].valid == 1)) {
            // we have hit the target, and need to increment the hit count 
	        hit_cnt++;
	        cache[setBits][i].LRUTracking = ++ evictCounter;
	        return; // so that it doesn't go through code below
	    }
    }
    // increment miss count when failed to hit 
    miss_cnt++;
    // for every single set 
    for(int i = 0; i < E; i++) { 
	    // checking for potential cold miss
	    if(cache[setBits][i].valid == 0) {
	        // update the structure of the line
	        cache[setBits][i].valid = 1;
	        cache[setBits][i].tag = tagBits;
	        cache[setBits][i].LRUTracking = ++evictCounter;
	        return;
	    }
    }
    // for each line
    for(int i = 0; i < E; i++) { 
        // checking for potential capacity miss
	    if(cache[setBits][i].LRUTracking < cache[setBits][evictedLine].LRUTracking) {
            // recording the line to be evicted
	        evictedLine = i;
	    }
    }
    // update the structure
    cache[setBits][evictedLine].tag = tagBits;
    cache[setBits][evictedLine].LRUTracking = ++evictCounter;
    evict_cnt++;
    return;   
}
  
  
/* TODO - FILL IN THE MISSING CODE
 * replay_trace:
 * Replays the given trace file against the cache.
 *
 * Reads the input trace file line by line.
 * Extracts the type of each memory access : L/S/M
 * TRANSLATE each "L" as a load i.e. 1 memory access
 * TRANSLATE each "S" as a store i.e. 1 memory access
 * TRANSLATE each "M" as a load followed by a store i.e. 2 memory accesses 
 */              

void replay_trace(char* trace_fn) {           
    char buf[1000];  
    mem_addr_t addr = 0;
    unsigned int len = 0;
    FILE* trace_fp = fopen(trace_fn, "r"); 
    // FOR GRADER: added error checking 
    if (trace_fp == NULL) {
        printf("File Open Failure.\n");
		exit(1);
    }
    if (!trace_fp) { 
        fprintf(stderr, "%s: %s\n", trace_fn, strerror(errno));
        exit(1);   
    }

    while (fgets(buf, 1000, trace_fp) != NULL) {
        if (buf[1] == 'S' || buf[1] == 'L' || buf[1] == 'M') {
            sscanf(buf+3, "%llx,%u", &addr, &len);

            if (verbosity)
                printf("%c %llx,%u ", buf[1], addr, len);

            // TODO - MISSING CODE
            // GIVEN: 1. addr has the address to be accessed
            //        2. buf[1] has type of acccess(S/L/M)
            // call access_data function here depending on type of access

        // we need to check what is the value of buf[1] 
        // and need to conduct the corresponding access
        // variable access store either S, L, or M
            char access = buf[1]; 
            // S only need one access
            if (access == 'S') {
                access_data(addr);
            }
            // L only need one access
            if (access == 'L') {
                access_data(addr);
            }
            // M need two accesses
            if (access == 'M') {
                access_data(addr);
                access_data(addr);
            }
            if (verbosity){ 
            printf("\n");
            }
        }
    }
    // error checking for fclose() 
    if (fclose(trace_fp) != 0) {
        printf("Error while closing the file\n");
        exit(1);
    } 
}  
  
  
/*
 * print_usage:
 * Print information on how to use csim to standard output.
 */                    
void print_usage(char* argv[]) {                 
    printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", argv[0]);
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of s bits for set index.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of b bits for block offsets.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\nExamples:\n");
    printf("  linux>  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n", argv[0]);
    printf("  linux>  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n", argv[0]);
    exit(0);
}  
  
  
/*
 * print_summary:
 * Prints a summary of the cache simulation statistics to a file.
 */                    
void print_summary(int hits, int misses, int evictions) {                
    printf("hits:%d misses:%d evictions:%d\n", hits, misses, evictions);
    FILE* output_fp = fopen(".csim_results", "w");
    assert(output_fp);
    fprintf(output_fp, "%d %d %d\n", hits, misses, evictions);
    fclose(output_fp);
}  
  
  
/*
 * main:
 * Main parses command line args, makes the cache, replays the memory accesses
 * free the cache and print the summary statistics.  
 */                    
int main(int argc, char* argv[]) {                      
    char* trace_file = NULL;
    char c;
    
    // Parse the command line arguments: -h, -v, -s, -E, -b, -t 
    while ((c = getopt(argc, argv, "s:E:b:t:vh")) != -1) {
        switch (c) {
            case 'b':
                b = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'h':
                print_usage(argv);
                exit(0);
            case 's':
                s = atoi(optarg);
                break;
            case 't':
                trace_file = optarg;
                break;
            case 'v':
                verbosity = 1;
                break;
            default:
                print_usage(argv);
                exit(1);
        }
    }

    //Make sure that all required command line args were specified.
    if (s == 0 || E == 0 || b == 0 || trace_file == NULL) {
        printf("%s: Missing required command line argument\n", argv[0]);
        print_usage(argv);
        exit(1);
    }

    //Initialize cache.
    init_cache();

    //Replay the memory access trace.
    replay_trace(trace_file);

    //Free memory allocated for cache.
    free_cache();

    //Print the statistics to a file.
    //DO NOT REMOVE: This function must be called for test_csim to work.
    print_summary(hit_cnt, miss_cnt, evict_cnt);
    return 0;   
}  
