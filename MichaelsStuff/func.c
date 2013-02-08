#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"func.h"

void disk_setup(int n, int disk_type, int arr[]){
  /* function sets a given integer array, base don the type
     with the appropreiate values */
  int r = rand() % n; //get random position for seperator
  int vals = (n / 3);  // get possible alternating values
  int num = vals;
  int index= 0;

  for(; index < n; index++){
    if(index == r){
      /* set location of n^2 +1 in large disk array
       if array past is large, if not it remains 0*/
      if(disk_type == LARGE){
	arr[index] = (n / 3) + 1;
      }
    }
    else{
    /* loop from 1 to n/3 sequentially adding the values to 
       large array index */
      arr[index] = (vals + 1) - num;
      /* decrement vals */
      num--;
      /* reset vals for next iteration if so */
      if(num == 0){
	num = vals;
      }
    }
  }
}
