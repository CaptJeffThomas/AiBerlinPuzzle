
#ifndef _HASH_C_
#define _HASH_C_

#include "hash.h"
#include "func.h"

extern hash_head * closed_list;

/* initialize the hash */
hash_head * init_hash(int size){
  closed_list = malloc(sizeof(hash_head));
  hash_head * temp = closed_list;

  for (int i = 1; i<size-1; i++){
    temp->head=NULL;
    temp->next = malloc(sizeof(hash_head));
    temp = temp->next;
  }

  curr_mem += sizeof(hash_head) * size * 8;
  check_mem_usage();
  return closed_list;
}

/* the hash function */
int hash(node * ptr){
  return(heuristic(ptr->state));
}

/* insert a node into the hash */
void insert_to_hash(hash_head * map, node * ptr){
  int place = hash(ptr);
  node * temp;
  hash_head * hashTemp = map;

  while (place!=0){
    hashTemp = hashTemp->next;
    place--;
  }

  if (hashTemp->head==NULL){
    hashTemp->head = malloc(sizeof(node) + (size_of_array * sizeof(short int)));
    copy_node(ptr, hashTemp->head);
  } else {
    temp = hashTemp->head;
    while(temp->next!=NULL){
      temp=temp->next;
    }
    temp->next = malloc(sizeof(node) + (size_of_array * sizeof(short int)));
    copy_node(ptr, temp);
  }
  
  curr_mem += (sizeof(node) + (size_of_array * sizeof(short int)))* 8;
  check_mem_usage();
}

/* check to see if the hash contains the given state 
   returns 0 if the state is found, 1 otherwise*/
int hash_contains(hash_head * map, node * ptr){
  int place = hash(ptr);
  node * temp = NULL;
  hash_head * hashTemp = map;
  int same_state = 0;

  while (place != 0){
    hashTemp = hashTemp->next;
    place--;
  }

  temp = hashTemp->head;
  if(temp==NULL) return 1;
  while (temp!=NULL){
    
    /* check two states, same_state 0 if the same, non 0 otherwise */
    for(int i=0; i<size_of_array; i++){
      if (ptr->state[i]!=temp->state[i]) same_state++;
    }

    if (same_state==0) return 0;
    
    temp = temp->next;
  }
  return 1;
}

#endif /* _HASH_C_ */
