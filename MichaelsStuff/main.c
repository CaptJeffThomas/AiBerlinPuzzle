#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "func.h"

int main(int argc, char *argv[])
{

  /* read int from command line */
  int n;
  if(argv[1] != NULL){
    n = atoi(argv[1]);
  }  
  /* initialize disk arrays */
  size_of_array = (n * n) + 1;
  disk disks[size_of_array];
  memset(disks,0,(size_of_array * sizeof(disk)));
  
  /* populate arrays with disk setup using function */
  disk_setup(n,disks);

  printf(" --- Large Disks --- \n");
  for(int x = 0; x < size_of_array; x++){
    printf("%d ",disks[x].lrg_val);
  }
  printf("\n\n");

  printf(" --- Small Disks --- \n");
  for(int x = 0; x < size_of_array; x++){
    printf("%d ",disks[x].sml_val);
  }
  printf("\n \n \n");

  //heuristic(n, disks);

  mem_bound_A(disks);
  
  /* clear fringe before exit */
  clear_queue();
  return 0;
}
