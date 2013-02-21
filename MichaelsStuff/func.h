

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
// NOTE: each structure takes up 18 bytes = 144 bits as a base with memory of (n * 4)
typedef struct a_node {
  short int f_val;
  struct a_node *parent;
  short int state[]; // lrg and sml disk value corresponding to moving to some state
} node;


/* global variable for size of disk array (and what all short int arrays will be) */
int size_of_array;

/* global fringe pointer and size variable used to dynamically build fringe */
node *fringe_tail;
int size_of_fringe;

/* functions to setup the board */
int seed_val();
void disk_setup(int input, disk arr[]);

/* functions for queue structure made from a_nodes */
void enqueue(short int state[]);
void dequeue(node *current);
void clear_queue();

void heuristic(int input, disk arr[]);

/* functions for creating states and expanding current node */
void copy_node(node *org, node *dest);
void swap(int new_index, int disk_val, int current_index, short int arr[]);
void expand_node(int disk_val, int current_index, short int arr[]);
void insert_all(node *current, disk arr[]);

/* algorithm functions */
int goal_test(node *current);
void mem_bound_A(disk arr[]);


#endif /* _AB_GAME_H_  */
