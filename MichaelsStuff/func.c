#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<math.h>
#include"func.h"

void disk_setup(int n, disk arr[]){
  // get random values for small and large disk n position
  short int l_r = (int)((double)rand() / ((double)RAND_MAX + 1) * ((n * n) + 1));
  short int s_r = (int)((double)rand() / ((double)RAND_MAX + 1) * ((n * n) + 1));
  
  // set possible value max (n), used as temporaray to assign
  // values between 1 and n
  short int l_max = n; 
  short int s_max = n;

  int index= 0;
  for(; index < ((n * n) + 1); index++){

    /* *** LARGE DISKS ***/

    /* set random positon to palce the uncovered disk and n^2 + 1 value */
    if(index == l_r){
      arr[index].lrg_val = n + 1;
    }
    else {
      arr[index].lrg_val = (n + 1) - l_max;    
      /* decrement l_max */
      l_max--;
      /* reset l_max for next iteration of 1 through n */
      if(l_max == 0){
	l_max = n;
      }
    }

    /* *** SMALL DISKS *** */

    /* set random positon to palce the uncovered disk and n^2 + 1 value */
    if(index == s_r){
      arr[index].sml_val = 0;
    }
    else {
      arr[index].sml_val = (n + 1) - s_max;
      /* decrement s_max */
      s_max--;
      /* reset s_max for next iteration of 1 through n */
      if(s_max == 0){
	s_max = n;
      }
    }

  }
  
}
