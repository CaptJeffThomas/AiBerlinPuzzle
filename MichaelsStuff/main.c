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
  int size = (r * r) + 1;
  disk disks[size];
  memset(disks,0,(size * sizeof(disk)));

  /* populate arrays with disk setup using function */
  disk_setup(r,disks);

  int x= 0;
  for(; x < size; x++){
    printf("%d ",disks[x].lrg_val);
  }
  printf("\n");
  for(x= 0; x < size; x++){
    printf("%d ",disks[x].sml_val);
  }
  printf("\n");


  return 0;
}
