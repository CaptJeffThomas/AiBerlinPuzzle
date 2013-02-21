#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"func.h"

int main(int argc, char *argv[])
{

  /* read int from command line */
  int r;
  if(argv[1] != NULL){
    r = atoi(argv[1]);
  }  
  /* initialize disk arrays */
  size_of_array = (r * r) + 1;
  disk disks[size_of_array];
  memset(disks,0,(size_of_array * sizeof(disk)));
  
  /* populate arrays with disk setup using function */
  disk_setup(r,disks);

  printf(" --- Large Disks --- \n");
  for(int x = 0; x < size_of_array; x++){
    printf("%d ",disks[x].lrg_val);
  }
  printf("\n\n");
  /*for(x= 0; x < size; x++){
    printf("%d ",disks[x].sml_val);
  }
  printf("\n");*/

  //heuristic(r, disks);

  mem_bound_A(disks);
  
  /* clear fringe before exit */
  clear_queue();
  return 0;
}
