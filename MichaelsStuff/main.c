#define _POSIX_C_SOURCE 2 /* defined feature macro for getopt()*/
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include"func.h"
#include"hash.h"

bool rand_flag = false;
bool file_flag = false;

hash_head * hash_map;

int main(int argc, char *argv[])
{
 
  int optn;
  /* use of getopt to determine whether to create random numbers
     for the disks or use the number specified and read disk values
     from a predeteremined file */
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

  /* initialize disk arrays */
  disk disks[size_of_array];
  curr_mem += sizeof(disks);
  check_mem_usage();

  memset(disks,0,(size_of_array * sizeof(disk)));
  size_of_array = atoi(argv[1]);
  disk_setup(size_of_array, disks);

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

  mem_bound_A(disks);
  
  /* clear fringe before exit */
  clear_queue();
  return 0;
}
