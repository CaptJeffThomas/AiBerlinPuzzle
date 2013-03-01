
#ifndef _CLOSED_C_
#define _CLOSED_C_

#include "closed.h"

void add_closed(node * ptr){
  if (closed_contains(ptr)==0) return;

  if (closed_head == NULL){
    closed_head = malloc(sizeof(closed_node) + (size_of_array * sizeof(short int)));

    for (int i=0; i<size_of_array; i++){
      closed_head->state[i] = ptr->state[i];
    }
  }

  closed_node * temp = closed_head;

  while(temp->next!=NULL){
    temp = temp->next;
  }

  temp->next = malloc(sizeof(closed_node) + (size_of_array * sizeof(short int)));
  
  for (int i=0; i<size_of_array; i++){
    temp->next->state[i] = ptr->state[i];
  }
  temp->next->next=NULL;

  return;
}

int closed_contains(node * ptr){
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

#endif /* _CLOSED_C_ */
