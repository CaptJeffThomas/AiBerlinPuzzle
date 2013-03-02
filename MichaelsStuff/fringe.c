/*****************************************************************
 NAME: Croydon Enders, Michael Hnatiw, Jeff Thomas
 CLASS/ASSIGNMENT: CMPT355 Project #1
 Instructor: Calin Anton
*****************************************************************/

#ifndef _FRINGE_C_
#define _FRINGE_C_

#include<string.h>
#include"global.h"
#include"closed.h"
#include"fringe.h"

/*extern variables for the fringe head node and the size of the input */
extern node *fringe_head;
extern int size_of_array;

void push_to_fringe(short int newState[],short int path_cost)
{
  /* function to add node at the top of the stack */

  node *new = NULL;
  new = malloc(sizeof(node) + (size_of_array * sizeof(short int)));  
  if(new == NULL){
    printf("ERROR: malloc failed \n");
    exit(EXIT_FAILURE);
  }

  /* check mem usage */
  curr_mem += (sizeof(node) + (size_of_array * sizeof(short int)));
  check_mem_usage();

  memset(new,0,(sizeof(node) + (size_of_array * sizeof(short int))));

  /* run through inputted state and assign 
      values to node being added */
  for(int x = 0; x < size_of_array; x++){
      new->state[x] = newState[x];
  }
  
  /* check whether the node to be added is in the closed list
   *** ENSURES NO DUPLICATE STATES ARE ADDED TO THE FRINGE ****/
  int check = closed_contains(new);
  if(check == 1){
    /* if state node not in the closed list, add it to the closed list */
    add_closed(new);
      
    /* add node to the fringe as the head or push node onto the head */
    if(size_of_fringe == 0){
      new->next = NULL;
      fringe_head = new;
      new->g_val = 0;
      new->f_val = new->g_val + heuristic(new->state);
    }
    else{
      new->next = fringe_head;
      fringe_head = new;
      new->g_val = path_cost + 1;
      new->f_val = new->g_val + heuristic(new->state);
    }

    size_of_fringe++;
  }
  else{
    /* else if current node is in the closed list, free
       its memory, reduce memory usage, and dont add it*/
    free(new);
    curr_mem -= (sizeof(node) + (size_of_array * sizeof(short int)));
  }

}

void pop_from_fringe(node *current)
{
  /* function to remove node from start of queue */

  node *temp = fringe_head;
  if(size_of_fringe == 1){
    /* call function to copy values from not to current node */
    copy_node(temp,current);
    /* reduce mem usage */
    curr_mem -= (sizeof(node) + (size_of_array * sizeof(short int)));
    /* free memory */
    free(temp);
    temp = NULL;
    fringe_head = NULL;
  }
  else if(size_of_fringe > 1){
    
    fringe_head = temp->next;

    /* call function to copy values from not to current node */
    copy_node(temp,current);    
    /* reduce mem usage */
    curr_mem -= (sizeof(node) + (size_of_array * sizeof(short int)));
    /* free memory */
    free(temp);
    temp = NULL;
  }
  else{
    printf("ERROR: fringe is empty can't dequeue\n");
    exit(EXIT_FAILURE);
  }

  size_of_fringe--;
}

void clear_fringe()
{
  /* function runs through fringe, deleting all nodes */
  node *del = fringe_head; 
  node *temp = NULL;

  while(del != NULL){
    temp = del->next;

    /* reduce mem usage */
    curr_mem -= (sizeof(node) + (size_of_array * sizeof(short int)));

    /* free node */
    free(del);
    del = temp;
  }
  del = NULL;
  temp = NULL;
}

#endif /* _FRINGE_C_ */
