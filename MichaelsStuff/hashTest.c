#include "uthash.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct a_node {
  short int f_val; // sum of g() + h() calculated
  short int g_val; // path cost up till now
  UT_hash_handle hh;         /* makes this structure hashable */
  struct a_node *next; //pointer to next node
  
  short int state[]; // lrg and sml disk value corresponding to moving to some state
} node;


node *tmp, *closedList = NULL;


int main(int argc, char *argv[])
{

  short int state[10] = {1,1,1,2,2,2,3,3,0,3};
  //short int state2[10] = {2,2,2,1,1,3,1,3,3,0};

  HASH_FIND_INT(closedList, &state, tmp); //search the closed list to see if this state exists
  if (tmp == NULL){

        node *new = NULL;
        //node *new2 = NULL;
        new = malloc(sizeof(node) + (10 * sizeof(short int)));
        //new2 = malloc(sizeof(node) + (10 * sizeof(short int)));

        printf("Adding the following state to hash:  ");
        for(int x = 0; x < 10; x++){
          new->state[x] = state[x];
          printf("%d", state[x]);
        }
        HASH_ADD_INT(closedList, state, new);  /* state is the key */
       

        /* printf("\n\n Adding the following state to hash:  ");
        for(int x = 0; x < 10; x++){
          new2->state[x] = state2[x];
          printf("%d", state2[x]);
        }
        HASH_ADD_INT(closedList, state, new2);   state is the key */

  }

   HASH_FIND_INT(closedList,&state,tmp); 
   if (tmp != NULL){
      printf("\n \n The following state is found in the hash: ");
      for(int x = 0; x < 10; x++){
             printf(" %d", tmp->state[x]);
      }
   printf(" \n");
   } else {
        printf(" \n the hash did not add the state properly");
   }

  /* HASH_FIND_INT(closedList,&state2,tmp); 
   if (tmp != NULL){
      printf("\n The following state is found in the hash: ");
      for(int x = 0; x < 10; x++){
             printf(" %d", tmp->state[x]);
      }
   printf(" \n");
   } else {
        printf("\n the hash did not add the state properly");
   } */

}