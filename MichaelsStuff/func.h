

#ifndef _AB_GAME_H_
#define _AB_GAME_H_


#include <stdlib.h>
#include <stdio.h>

#define LARGE 1
#define SMALL 0

typedef struct node {
  short int lrg_val;
  short int sml_val;
} disk;


void disk_setup(int n, int disk_type, int arr[]);





#endif /* _AB_GAME_H_  */
