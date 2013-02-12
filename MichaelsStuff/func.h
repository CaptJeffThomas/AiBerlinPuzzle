

#ifndef _AB_GAME_H_
#define _AB_GAME_H_


#include <stdlib.h>
#include <stdio.h>


/* node structure for each disk (with corresponding large values) 
   representing states of the game */
typedef struct d_node {
  short int lrg_val;
  short int sml_val;
} disk;

/* node structure for heap in our frontier queue */
typedef struct h_node {
  short int lrg_val;
  short int sml_val;
  struct h_node *parent;
} heap_node;



int seed_val();

void disk_setup(int input, disk arr[]);


#endif /* _AB_GAME_H_  */
