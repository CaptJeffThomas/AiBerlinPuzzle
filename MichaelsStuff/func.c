#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include"func.h"

#define NELEMS(x) (sizeof(x)/sizeof(x[0]))

int seed_val(){
  /* function returns seed value based on current
     time in seconds */

  time_t current = time(0);
  struct tm *time = localtime(&current);
  if(time == NULL){
    printf("error extracting local time\n");
    exit(EXIT_FAILURE);
  }
  
  //printf("%d\n",((time->tm_hour * 60) * 60) + (time->tm_min * 60) + time->tm_sec);
  return (((time->tm_hour * 60) * 60) + (time->tm_min * 60) + time->tm_sec);
}

void disk_setup(int n, disk arr[]){

  // call function to produce random seed with each call
  int seed = seed_val();
  srand(seed);

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


void heuristic(int input, disk arr[]){
  
  int finalVal;
  int size = (input * input) + 1;

  printf("Heuristics:\n");

  for (int i=0; i<size; i++){
    finalVal=(int)(i/input)+1;
    finalVal=arr[i].sml_val-finalVal;
    printf("arr[%d] = %d\n", i, finalVal);
  }
  printf("\n");
}
