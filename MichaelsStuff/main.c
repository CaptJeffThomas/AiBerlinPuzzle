#define _POSIX_C_SOURCE 2 /* defined feature macro for getopt()*/
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include"func.h"
#include"hash.h"

/* global variables for option flags */
bool rand_flag = false;
bool file_flag = false;

int main(int argc, char *argv[])
{
 
  int optn;
  /* use of getopt to determine whether to create random numbers
     for the disks or use the number specified and read disk values
     from a predeteremined file */
  
  /*NOTE: options not implemented, functions exists just need to modify
    this main file if they are desired */
  while((optn = getopt(argc,argv,"hfr")) != -1){
    switch(optn){
    case 'r':
      /* given an r option, set flag to true */
      rand_flag = true;
      break;
    case 'f':
      /* given an f option, set file flag to true */
      file_flag = true;
      break;
    case 'h':
    case '?':
    default:
      /* given an h, unknown, or default case, produce usage message */
      usage();
      return 1;
    }
  }
  
  /* initialize curr_mem */
  curr_mem = 0;

  /**** set size_of_array from input, or else
	produce usage message and exit ****/
  if(argv[1] != NULL){
    size_of_array = atoi(argv[1]);
  }
  else{
    usage();
    return 1;
  }
  
  /* disk array used to represent disk board layout */
  disk disks[size_of_array];
  
  /* chceck memory usage */
  curr_mem += sizeof(disks);  
  check_mem_usage();

  /* initialize disk array and set up the board layout */
  memset(disks,0,(size_of_array * sizeof(disk)));
  disk_setup(size_of_array, disks);


  /* print large disk and small disk layout, as well as the initial state */
  printf(" --- Large Disks --- \n");
  int x;
  for(x = 0; x < size_of_array; x++){
    printf("%d ",disks[x].lrg_val);
  }
  printf("\n\n");

  printf(" --- Small Disks --- \n");
  for(int x = 0; x < size_of_array; x++){
    printf("%d ",disks[x].sml_val);
  }
  printf("\n \n \n");

  printf("\n\n --- Initial State --- \n");
  for(x= 0; x < size_of_array; x++){
    printf("%d ",disks[x].sml_val);
  }
  printf("\n\n");

  /* call function to emunlate simple memory-bounded A* algorithm 
     on the disk board */
  mem_bound_A(disks);
  
  /* clear memory in fringe before exit */
  clear_queue();
  return 0;
}
