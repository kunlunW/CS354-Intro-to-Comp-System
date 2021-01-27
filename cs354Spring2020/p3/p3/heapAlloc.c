/////////////////////////////////////////////////////////////////////////////// 
// 
// Copyright 2020 Jim Skrentny 
// Posting or sharing this file is prohibited, including any changes/additions. 
// 
//////////////////////////////////////////////////////////////////////////////// 
// Main File:        heapAlloc.c
// This File:        heapAlloc.c
// Other Files:      None
// Semester:         CS 354 Spring 2020 
// 
// Author:           Kunlun Wang  
// Email:            kwang358@wisc.edu
// CS Login:         kunlun
// 
/////////////////////////// OTHER SOURCES OF HELP ////////////////////////////// 
//                   Fully acknowledge and credit all sources of help, 
//                   other than Instructors and TAs. 
// 
// Persons:          Identify persons by name, relationship to you, and email. 
//                   Describe in detail the the ideas and help they provided. 
// 
// Online sources:   Avoid web searches to solve your problems, but if you do 
//                   search, be sure to include Web URLs and description of 
//                   of any information you find. 
////////////////////////////////////////////////////////////////////////////////

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include "heapAlloc.h"

/*
 * This structure serves as the header for each allocated and free block.
 * It also serves as the footer for each free block but only containing size.
 */
typedef struct blockHeader {           
    int size_status;
    /*
    * Size of the block is always a multiple of 8.
    * Size is stored in all block headers and free block footers.
    *
    * Status is stored only in headers using the two least significant bits.
    *   Bit0 => least significant bit, last bit
    *   Bit0 == 0 => free block
    *   Bit0 == 1 => allocated block
    *
    *   Bit1 => second last bit 
    *   Bit1 == 0 => previous block is free
    *   Bit1 == 1 => previous block is allocated
    * 
    * End Mark: 
    *  The end of the available memory is indicated using a size_status of 1.
    * 
    * Examples:
    * 
    * 1. Allocated block of size 24 bytes:
    *    Header:
    *      If the previous block is allocated, size_status should be 27
    *      If the previous block is free, size_status should be 25
    * 
    * 2. Free block of size 24 bytes:
    *    Header:
    *      If the previous block is allocated, size_status should be 26
    *      If the previous block is free, size_status should be 24
    *    Footer:
    *      size_status should be 24
    */
} blockHeader;         

/* Global variable - DO NOT CHANGE. It should always point to the first block,
 * i.e., the block at the lowest address.
 */

blockHeader *heapStart = NULL;
blockHeader *currPtr = NULL;

/* Size of heap allocation padded to round to nearest page size.
 */
int allocsize;

/*
 * Additional global variables may be added as needed below
 */

 
/* 
 * Function for allocating 'size' bytes of heap memory.
 * Argument size: requested size for the payload
 * Returns address of allocated block on success.
 * Returns NULL on failure.
 * This function should:
 * - Check size - Return NULL if not positive or if larger than heap space.
 * - Determine block size rounding up to a multiple of 8 and possibly adding padding as a result.
 * - Use NEXT-FIT PLACEMENT POLICY to chose a free block
 * - Use SPLITTING to divide the chosen free block into two if it is too large.
 * - Update header(s) and footer as needed.
 * Tips: Be careful with pointer arithmetic and scale factors.
 */
void* allocHeap(int size) {
    // we need to set up a pointer where if we reach the end, and go to the start, and if we
    // reach this pointer --> it means that there's no fit 
    blockHeader *markerPtr = NULL;
    // this represents the size of the new block after splitting
    int newBlockSize;
    // we need to add a 4 byte header to form the actual size of the block
    int totalBlockSize = size + 4;
    // if the current pointer is null, we need to set the heapStart to it
    // this is an initialization for the first allocation
	if(currPtr == NULL) {
		currPtr = heapStart;
        // get the size of the allocation
		allocsize = heapStart -> size_status - 2;
	}
	// if the total amount of memory to be allocated is not a multiple of 8
    // then we need to calculate the size of the required block
	if((totalBlockSize) % 8 != 0) {
        int remainder = ((totalBlockSize) % 8);
	    totalBlockSize = totalBlockSize + 8 - remainder;
	}
	// Check size - Return NULL if not positive or if larger than heap space.
	if(size <= 0 || totalBlockSize > allocsize) {	
        return NULL;
    } 

    // while we haven't looped around and reached the marker 
	while(currPtr != markerPtr) {
        // get the status of the current block
        int currStatus = currPtr -> size_status % 2;
        // if the markerPtr is null, we can set it to currPtr
		if(markerPtr == NULL) {
			markerPtr = currPtr;
		}
        // if the current block is a free block
		if(currStatus == 0) {
            int currSize = currPtr -> size_status - 2; 
            // if current size equals to the total block size we want to allocate,
            // allocate it, and update the status 
			if(currSize == totalBlockSize) {
                // update the status --> allocated 
				currPtr -> size_status += 1;
                // currPtr is a header pointer, we need to return payload pointer 
				return (currPtr + 1);
			}
            // if the free block is too big for the request
            // we should split it for further use 
            
			if(currSize > totalBlockSize) {
				newBlockSize = currSize - totalBlockSize;
                 // update the currPtr p-bit and a-bit --> allocation 
				currPtr -> size_status = totalBlockSize + 2 + 1;
                // we need to update the footer
				(currPtr + (currSize) / 4 - 1) -> size_status = newBlockSize; 
                // create header 
				(currPtr + totalBlockSize / 4) -> size_status = newBlockSize + 2;
                // currPtr is header pointer, we need payload pointer 
				return (currPtr + 1); 
			}

            // if the current block is too small for the request
            // we need to move the pointer to the next block
			if(currSize < totalBlockSize) {
                int nextBlock = (currSize)/4;
				currPtr = currPtr + nextBlock; 
 				continue;
			}
		}
        // if the currPtr is already allocated 
		else {
            // if we reached the end of the heap, after the iteration of the while loop   
            // we need to go back to the start to search again 
			if(currPtr -> size_status == 1) {
				currPtr = heapStart;
			}
            // if we haven't reached the end of the heap, then go to the next block 
			else {
                int nextSkip = (currPtr -> size_status - 2 - 1) / 4;
				currPtr = currPtr + nextSkip;
			}		
		}
	}
	return NULL;
}
/* 
 * Function for freeing up a previously allocated block.
 * Argument ptr: address of the block to be freed up.
 * Returns 0 on success.
 * Returns -1 on failure.
 * This function should:
 * - Return -1 if ptr is NULL.
 * - Return -1 if ptr is not a multiple of 8.
 * - Return -1 if ptr is outside of the heap space.
 * - Return -1 if ptr block is already freed.
 * - USE IMMEDIATE COALESCING if one or both of the adjacent neighbors are free.
 * - Update header(s) and footer as needed.
 */                    
int freeHeap(void *ptr) {    
    // payload pointer 
    blockHeader *headerPtr = ptr - 4;
    blockHeader *tempStart = heapStart;
    // if the ptr is a null pointer, then it's a failure --> return -1 
	if (ptr == NULL) {
        return -1;
    }
    // if the ptr is already a free block --> undefined behavior 
    if (((headerPtr) -> size_status % 2) == 0) {
        return -1;
    }
	// if the addr of the ptr is not a multiple of 8
    // then it means that it's a failure 
    if (((unsigned int)(ptr) % 8) != 0) {
       	return -1;
    }
	
    // if ptr's addr is less than the heapStart 
    // error will occur 
	if((unsigned int)(tempStart) > (unsigned int)(ptr)) {
		return -1;	
	}
	
    // if ptr is larger than the end of the heap --> error will occur 
	if((unsigned int)(ptr) - (unsigned int)(tempStart) + 8 > allocsize) {
		return -1;
	}
	
    // while the ptr addr is larger than the heap start
    // we need to make sure that ptr is bound within the heap space 
    while ((unsigned int)(tempStart) < (unsigned int)(ptr)) {
        // if the temporary start of the heap has a size status of 1 
        // this is not allowed --> error 
        if (tempStart -> size_status == 1) {
           	return -1;
        }
        // move temporary start of the heap to the next block 
        int nextSkip = tempStart -> size_status / 4;
        tempStart = tempStart + nextSkip;
    }
	// update the status --> free it 
    (headerPtr) -> size_status--;

    int currentSize = (headerPtr) -> size_status;
    int previousSize = (headerPtr) -> size_status % 8;
	// calculate the size of the current free block     
    int freeSize = currentSize - previousSize;
    // check if last block is freed
    int lastBlockStatus = ((headerPtr) -> size_status % 8);
    // if the p bit of last block is 0 
    if (lastBlockStatus == 0) {
    	// go the footer of last block 
        (headerPtr)--;
        // total freeSize == currSize + prevSize
        freeSize += (headerPtr) -> size_status;
        // head pointer goes to the previous header
        int offsetSize = (headerPtr) -> size_status / 4; 
        (headerPtr) = (headerPtr) - offsetSize + 1;
    }
    //update previous block freeSize
    headerPtr -> size_status = freeSize + 2;

    // now we check if the next block is free or not 
    // first go to next block 
    int skip = headerPtr -> size_status / 4;
    tempStart = headerPtr + skip;
    // if next block is free
    if ((tempStart -> size_status % 2) == 0) {
    	// if the next block is free, update the total freeSize
        int nextFree = tempStart -> size_status - 2;
        freeSize = freeSize + nextFree;
        // change the header of the large free block 
        headerPtr -> size_status = freeSize + 2;
        // go to the footer
        int footerSkip = tempStart -> size_status / 4; 
        tempStart = tempStart + footerSkip - 1;
        // update the size at footer
        tempStart -> size_status = freeSize;
	    if(currPtr == (ptr - 4)) {
		    currPtr = headerPtr;
	    }
        return 0;
    }
    // if the next block is not free
    else {
        // update p-bit of next block
        tempStart -> size_status -= 2;
        // go to footer
        tempStart--;
        // update the value of footer 
        tempStart -> size_status = freeSize;
        // go to the header 
	    if(currPtr == (ptr - 4)) {
		    currPtr = headerPtr;
		}
        	return 0;
    }
    	return 0;
} 
 
/*
 * Function used to initialize the memory allocator.
 * Intended to be called ONLY once by a program.
 * Argument sizeOfRegion: the size of the heap space to be allocated.
 * Returns 0 on success.
 * Returns -1 on failure.
 */                    
int initHeap(int sizeOfRegion) {    
 
    static int allocated_once = 0; //prevent multiple initHeap calls
 
    int pagesize;  // page size
    int padsize;   // size of padding when heap size not a multiple of page size
    void* mmap_ptr; // pointer to memory mapped area
    int fd;

    blockHeader* endMark;
  
    if (0 != allocated_once) {
        fprintf(stderr, 
        "Error:mem.c: InitHeap has allocated space during a previous call\n");
        return -1;
    }
    if (sizeOfRegion <= 0) {
        fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
        return -1;
    }

    // Get the pagesize
    pagesize = getpagesize();

    // Calculate padsize as the padding required to round up sizeOfRegion 
    // to a multiple of pagesize
    padsize = sizeOfRegion % pagesize;
    padsize = (pagesize - padsize) % pagesize;

    allocsize = sizeOfRegion + padsize;

    // Using mmap to allocate memory
    fd = open("/dev/zero", O_RDWR);
    if (-1 == fd) {
        fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
        return -1;
    }
    mmap_ptr = mmap(NULL, allocsize, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (MAP_FAILED == mmap_ptr) {
        fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
        allocated_once = 0;
        return -1;
    }
  
    allocated_once = 1;

    // for double word alignment and end mark
    allocsize -= 8;

    // Initially there is only one big free block in the heap.
    // Skip first 4 bytes for double word alignment requirement.
    heapStart = (blockHeader*) mmap_ptr + 1;

    // Set the end mark
    endMark = (blockHeader*)((void*)heapStart + allocsize);
    endMark->size_status = 1;

    // Set size in header
    heapStart->size_status = allocsize;

    // Set p-bit as allocated in header
    // note a-bit left at 0 for free
    heapStart->size_status += 2;

    // Set the footer
    blockHeader *footer = (blockHeader*) ((void*)heapStart + allocsize - 4);
    footer->size_status = allocsize;
  
    return 0;
} 
                  
/* 
 * Function to be used for DEBUGGING to help you visualize your heap structure.
 * Prints out a list of all the blocks including this information:
 * No.      : serial number of the block 
 * Status   : free/used (allocated)
 * Prev     : status of previous block free/used (allocated)
 * t_Begin  : address of the first byte in the block (where the header starts) 
 * t_End    : address of the last byte in the block 
 * t_Size   : size of the block as stored in the block header
 */                     
void dumpMem() {     
 
    int counter;
    char status[5];
    char p_status[5];
    char *t_begin = NULL;
    char *t_end   = NULL;
    int t_size;

    blockHeader *current = heapStart;
    counter = 1;

    int used_size = 0;
    int free_size = 0;
    int is_used   = -1;

    fprintf(stdout, "************************************Block list***\
                    ********************************\n");
    fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
    fprintf(stdout, "-------------------------------------------------\
                    --------------------------------\n");
  
    while (current->size_status != 1) {
        t_begin = (char*)current;
        t_size = current->size_status;
    
        if (t_size & 1) {
            // LSB = 1 => used block
            strcpy(status, "used");
            is_used = 1;
            t_size = t_size - 1;
        } else {
            strcpy(status, "Free");
            is_used = 0;
        }

        if (t_size & 2) {
            strcpy(p_status, "used");
            t_size = t_size - 2;
        } else {
            strcpy(p_status, "Free");
        }

        if (is_used) 
            used_size += t_size;
        else 
            free_size += t_size;

        t_end = t_begin + t_size - 1;
    
        fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%d\n", counter, status, 
        p_status, (unsigned long int)t_begin, (unsigned long int)t_end, t_size);
    
        current = (blockHeader*)((char*)current + t_size);
        counter = counter + 1;
    }

    fprintf(stdout, "---------------------------------------------------\
                    ------------------------------\n");
    fprintf(stdout, "***************************************************\
                    ******************************\n");
    fprintf(stdout, "Total used size = %d\n", used_size);
    fprintf(stdout, "Total free size = %d\n", free_size);
    fprintf(stdout, "Total size = %d\n", used_size + free_size);
    fprintf(stdout, "***************************************************\
                    ******************************\n");
    fflush(stdout);

    return;  
} 
