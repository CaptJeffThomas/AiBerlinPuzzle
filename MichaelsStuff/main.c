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
  int lrg_arr[r], sml_arr[r];
  memset(lrg_arr,0,(r * sizeof(int)));
  memset(sml_arr,0,(r * sizeof(int)));

  /* populate arrays with disk setup using function */
  disk_setup(r,LARGE,lrg_arr);
  disk_setup(r,SMALL,sml_arr);

  int x= 0;
  for(; x < r; x++){
    printf("%d ",lrg_arr[x]);
  }
  printf("\n");

  int y= 0;
  for(; y < r; y++){
    printf("%d ",sml_arr[y]);
  }
  printf("\n");


  return 0;
}
