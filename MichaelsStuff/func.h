

#ifndef _AB_GAME_H_
#define _AB_GAME_H_


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/* maximum amount of memory allowable for search structures */
#define MEMSIZE 1000000

/* node structure for each disk (with corresponding large values) 
   representing states of the game */
typedef struct d_node {
  short int lrg_val;
  short int sml_val;
} disk;

/* node structure for heap in our frontier queue */
// NOTE: each structure takes up 19 bytes = 152 bits as a base with memory of (n * 4)
typedef struct a_node {
  short int f_val; // sum of g() + h() calculated
  short int g_val; // path cost up till now
  struct a_node *next; //pointer to next node
  short int state[]; // lrg and sml disk value corresponding to moving to some state
} node;


/* global variable for size of disk array (and what all short int arrays will be) */
int size_of_array;

/* global fringe pointer and size variable used to dynamically build fringe */
node *fringe_head;
int size_of_fringe;

/* global variable to track memory usage and associated function for checking mem usage */
int curr_mem;
void check_mem_usage();

/* function for usage message */
void usage();

/* functions to setup the board */
void random_disk_setup(int input, disk arr[]);
void disk_setup(int input, disk arr[]);
void fileDisk_setup(int input, disk arr[]);

/* functions for queue structure made from a_nodes */
void enqueue(short int state[], short int path_cost);
void dequeue(node *current);
void clear_queue();

int heuristic(short int curr_state[]);

/* functions for creating states and expanding current node */
void copy_node(node *org, node *dest);
void swap(int new_index, int disk_val, int current_index, short int arr[]);
void expand_state(node *current,disk arr[]);

/* algorithm functions */
int evaluate_cost(int current_index, node *current);
int goal_test(node *current);
void mem_bound_A(disk arr[]);


#endif /* _AB_GAME_H_  */
