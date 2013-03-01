
#ifndef _HASH_C_
#define _HASH_C_

#include "hash.h"
#include "func.h"

extern hash_head * hash_map;

/* initialize the hash */
hash_head * init_hash(int size){
  hash_map = malloc(sizeof(hash_head));
  hash_head * temp = hash_map;

  for (int i = 1; i<size ; i++){
    temp->next = malloc(sizeof(hash_head));
    temp = temp->next;
  }

  curr_mem += sizeof(hash_head) * size * 8;
  check_mem_usage();
  return hash_map;
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

  if (hashTemp==NULL){
    hashTemp = malloc(sizeof(node));
    copy_node(ptr, hashTemp->head);
  } else {
    temp = hashTemp->head;
    while(temp->next!=NULL){
      temp=temp->next;
    }
    temp->next = malloc(sizeof(node));
    copy_node(ptr, temp);
  }
  
  curr_mem += sizeof(node) * 8;
  check_mem_usage();
}

/* remove element from the hash */
void remove_from_hash(hash_head * map, node * ptr){
  int place = hash(ptr);
  node * prev = NULL;
  node * temp = NULL;
  hash_head * hashTemp = NULL;
  int same_state = 0;

  hashTemp = map;

  while(place!=0){
    hashTemp = hashTemp->next;
    place--;
  }

  temp = hashTemp->head;
  if (temp==NULL){
    return;
  } else {
    while (temp->next!=NULL){
      
      /* check the two states. same_state will be 0 if they are the same, non 0 otherwise */
      for (int i=0; i<size_of_array; i++){
	if (ptr->state[i]!=temp->state[i]) same_state++;
      }

      /* remove element if it is what we are looking for */
      if(same_state==0){
	if(prev==NULL){
	  hashTemp->head = temp->next;
	} else {
	  prev->next = temp->next;
	}
	curr_mem -= sizeof(node) * 8;
	free(temp);
	break;
      }

      same_state=0;
      prev = temp;
      temp = temp->next;
    }
  }
  
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
  while (temp->next!=NULL){
    
    /* check two states, same_state 0 if the same, non 0 otherwise */
    for(int i=0; i<size_of_array; i++){
      if (ptr->state[i]!=temp->state[i]) same_state++;
    }

    if (same_state==0) return 0;

  }
  return 1;
}

#endif /* _HASH_C_ */
