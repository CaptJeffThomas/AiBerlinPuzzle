#define _POSIX_C_SOURCE 2 /* defined feature macro for getopt()*/
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include"func.h"

bool flag = false;

int main(int argc, char *argv[])
{
 
  int n, optn;
  /* use of getopt to determine whether to create random numbers
     for the disks or use the number specified and read disk values
     from a predeteremined file */
  while((optn = getopt(argc,argv,"hr")) != -1){
    switch(optn){
    case 'r':
      /* given an r option, set flag to true */
      flag = true;
      break;
    case 'h':
    case '?':
    default:
      /* given an h, unknown, or default case, produce usage message */
      usage();
      return 1;
    }
  }
  

  /* initialize disk arrays */
  disk disks[size_of_array];
  memset(disks,0,(size_of_array * sizeof(disk)));


  /* given an r option that sets the flag, use inputted numver as 'n' value 
  and set the size_of_array global */
  if(flag){
      if(argv[2] != NULL){
	n = atoi(argv[2]);
	size_of_array = (n * n) + 1;
      }
      else{
	printf("ERROR: no 'n' inputted to generate random layout of disks\n");
	exit(EXIT_FAILURE);
      }
      /* call function to randomize board layout */
      random_disk_setup(n,disks);
  }
  /* if command line given no options, use given value as n^2+1, read input.txt */
  else{
    /* default case, use given value as n^2+1, read input.txt */
      if(argv[1] != NULL){
	size_of_array = atoi(argv[1]);
      }
      else{
	printf("ERROR: no 'n' inputted to generate random layout of disks\n");
	exit(EXIT_FAILURE);
      }
      /* call function to define board layout */
      disk_setup(size_of_array,disks);
  }


  printf(" --- Large Disks --- \n");
  int x;
  for(x = 0; x < size_of_array; x++){
    printf("%d ",disks[x].lrg_val);
  }
  printf("\n\n --- Initial State --- \n");
  for(x= 0; x < size_of_array; x++){
    printf("%d ",disks[x].sml_val);
  }
  printf("\n\n");

  //heuristic(n,disks);

  mem_bound_A(disks);
  
  /* clear fringe before exit */
  clear_queue();
  return 0;
}
