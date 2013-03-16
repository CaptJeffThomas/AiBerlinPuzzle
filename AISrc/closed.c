/*****************************************************************
 NAME: Croydon Enders, Michael Hnatiw, Jeff Thomas
 CLASS/ASSIGNMENT: CMPT355 Project #1
 Instructor: Calin Anton
*****************************************************************/

#ifndef _CLOSED_C_
#define _CLOSED_C_

#include"global.h"
#include"closed.h"

extern int curr_mem; /* variable for memory usage */

void add_closed(node * ptr){
  /* function add a passed state node to the closed list */
  if (closed_contains(ptr)==0) return;

  /* if closed list is empty, add it to the head */
  if (closed_head == NULL){
    closed_head = malloc(sizeof(closed_node) + (size_of_array * sizeof(short int)));

    /* add memory added to memory usage variable and check memory limit */
    curr_mem += (sizeof(closed_node) + (size_of_array * sizeof(short int)));
    check_mem_usage();
    
    for (int i=0; i<size_of_array; i++){
      closed_head->state[i] = ptr->state[i];
    }
  }

  /* else traverse and node at the end */
  closed_node * temp = closed_head;

  while(temp->next!=NULL){
    temp = temp->next;
  }

  temp->next = malloc(sizeof(closed_node) + (size_of_array * sizeof(short int)));

  /* add memory added to memory usage variable and check memory limit */
  curr_mem += (sizeof(closed_node) + (size_of_array * sizeof(short int)));
  check_mem_usage();
    
  for (int i=0; i<size_of_array; i++){
    temp->next->state[i] = ptr->state[i];
  }
  temp->next->next=NULL;

  return;
}

int closed_contains(node * ptr){
  /* function checks each closed_list node to see if a state node 
     is contained in the closed list, returns a 1 if found, 1 if not, 0 if found */
  closed_node * temp = closed_head;
  int same_state;

  while (temp != NULL){
    same_state=0;
    for (int i=0; i<size_of_array; i++){
      if (temp->state[i]!=ptr->state[i]) same_state++;
    }
    if (same_state==0) return 0;
    temp = temp->next;
  }
  return 1;
}

void clear_closed()
{
  /* function runs through closed list, deleting all nodes */
  closed_node *del = closed_head; 
  closed_node *temp = NULL;

  while(del != NULL){
    temp = del->next;

    /* reduce mem usage */
    curr_mem -= (sizeof(closed_node) + (size_of_array * sizeof(short int)));

    /* free node */
    free(del);
    del = temp;
  }
  del = NULL;
  temp = NULL;
}


#endif /* _CLOSED_C_ */
