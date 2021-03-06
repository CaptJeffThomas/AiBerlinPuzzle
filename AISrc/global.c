/*****************************************************************
 NAME: Croydon Enders, Michael Hnatiw, Jeff Thomas
 CLASS/ASSIGNMENT: CMPT355 Project #1
 Instructor: Calin Anton
*****************************************************************/

#ifndef _AB_GAME_C_
#define _AB_GAME_C_

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include"global.h"
#include"closed.h"
#include"fringe.h"

void usage()
{
  /* function produces usage message for user */
  printf(" AB program imposes a single Agent Search Method to complete a game.\n\n \
    NOTE: given no argumenta with a number to produce defined layout from \
input.txt\n\n    Options:\n        -u    display usage message.\n \
       -r <number> use number as n value to produce random board layout\n \
       -f <number> use number as n value, and read input frm file to produce board layout\n \
    Example:\n         ./AB 10\n");
}

void check_mem_usage()
{
  /* function checks memory constraints and calls function to reduce memory */
  if (curr_mem > MEMSIZE){
    printf("Reached Memory Limit ...... find worst case and drop all nodes below\n");
    printf("Before: Memory Usage: %d  --- Memory Limit: %d \n",curr_mem,MEMSIZE);
    reduce_mem_usage();
    printf("After: Memory Usage: %d  --- Memory Limit: %d \n",curr_mem,MEMSIZE);
  }
}

void disk_setup(int n, disk arr[]){ 
  /* function reads disk values from stdin into the small and large disks
     NOTE: assumes correct input */

  char largeDisks[n*n + 1]; 
  char smallDisks[n*n + 1]; 

  //in any situation where input wasn't guaranteed we wouldn't use gets. however we've been assured it is godly.
  //note that below it asks for clockwise order and this is arbitrary choice that would only be relevant to some1 
  //with a physical puzzle.  Regardless it guarantees similar ordering for both small and large disks.
  printf("Please enter the values of the large disks in clockwise order: \n"); 
  fgets(largeDisks, ((size_of_array * 2) + 5), stdin); 
  printf("\n"); 

  printf("Please enter the values of the small disks in clockwise order (use 0 to represent the empty slot): \n"); 
  fgets(smallDisks, ((size_of_array * 2) + 5), stdin); 
  printf("\n"); 

  //fills our arr with large disk values provided from users string 
  char *diskValue = strtok (largeDisks," "); 
  int i = 0; 
  while (diskValue != NULL){ 
    arr[i].lrg_val = atoi(diskValue); 
    diskValue = strtok (NULL, " "); 
    i++; 
  } 

  //fills our arr with small disk values provided from users string 
  diskValue = strtok (smallDisks," "); 
  i = 0; 
  while (diskValue != NULL){ 
    arr[i].sml_val = atoi(diskValue); 
    diskValue = strtok (NULL, " "); 
    i++; 
  } 

}


void fileDisk_setup(int input, disk arr[])
{
  /* function reads input from a file and puts values
     into disk array */

  FILE *fh = NULL;
  if((fh = fopen("input.txt","r")) == NULL){
    printf("ERROR: could not open file input.txt \n");
    exit(EXIT_FAILURE);
  }
  
  char line[input * 2];
  /* read first line, put values into large disk array */
  if(fgets(line,sizeof(line),fh) == NULL){
    printf("ERROR: input.txt is empty\n");
    exit(EXIT_FAILURE);
  }
  for(int x = 0, y = 0; x < (input * 2); x += 2, y++){
    arr[y].lrg_val = atoi(line + x);
  }

  /* read new line from file(disgard it) */
  fgets(line,sizeof(line),fh);

  /* read second line, put values into small disk array */
  if(fgets(line,sizeof(line),fh) == NULL){
    printf("ERROR: second line of input.txt is empty\n");
    exit(EXIT_FAILURE);
  }
  for(int x = 0, y = 0; x < (input * 2); x += 2, y++){
      arr[y].sml_val = atoi(line + x);
  }

  fclose(fh); /* close file pointer */
}

void random_disk_setup(int n, disk arr[]){
  /* function to produce a rnadom layout for the small and large disk read int */

  /* produce random seed with each call */
  srand(0);

  /* get random values for small and large disk n position */
  short int l_r = (int)((double)rand() / ((double)RAND_MAX + 1) * ((n * n) + 1));
  short int s_r = (int)((double)rand() / ((double)RAND_MAX + 1) * ((n * n) + 1));
  
  /* set possible value max (n), used as temporaray to assign
     values between 1 and n */
  short int l_max = n; 
  short int s_max = n;

  int index= 0;
  for(; index < ((n * n) + 1); index++){

    /* *** LARGE DISKS ***/
    /* set random positon to palce the uncovered disk and n^2 + 1 value */
    if(index == l_r){
      arr[index].lrg_val = n + 1;
    }
    else {
      arr[index].lrg_val = (n + 1) - l_max;    
      /* decrement l_max */
      l_max--;
      /* reset l_max for next iteration of 1 through n */
      if(l_max == 0){
	l_max = n;
      }
    }

    /* *** SMALL DISKS *** */
    /* set random positon to palce the uncovered disk and n^2 + 1 value */
    if(index == s_r){
      arr[index].sml_val = 0;
    }
    else {
      arr[index].sml_val = (n + 1) - s_max;
      /* decrement s_max */
      s_max--;
      /* reset s_max for next iteration of 1 through n */
      if(s_max == 0){
	s_max = n;
      }
    }

  }
  
}

int goal_test(node *current)
{
  /* function inspects each short in the the current nodes state and performs 
     checks to determine whether it is  goal state or not */

  int x, count, val, n; 
  /*x is a simple index of our current state, val is the value at index in a goal state we compare against, 
    count is the number of similar val's which = n, n the number of distinct small disks provided on commandline */

  n = sqrt(size_of_array - 1); // n value from command line
  int start; //holds the index of the uncovered disk, i.e. state's 0 value

  /*find where our uncovered disk is*/
  for(start=0; start < size_of_array; start++){
    if(current->state[start] == 0){
      break;
    }
  }
  
  /*from our start index, scan the state to see if we have a goal state. */
  val = 1;
  for(x = start + 1, count = 0; x < size_of_array; x++, count++){
    if(count == n){
      val++;
      count = 0;
    }
    if(current->state[x] != val){
      return 0;
    }
  }
  /* We've scanned from the start (0-value) index to the end and everything 
  is in order if we've arrived here.  Now we make sure everything from state[0] 
  to state[start] is also in order. */
  for(x=0; x < start; x++,count++){
    if(count == n){
      val++;
      count = 0;
    }
    if(current->state[x] != val){
      return 0;
    }
  }
 
  /* return 1 designating that the state has passed all tests and is a goal state! */
  return 1;
}

void copy_node(node *org, node *dest)
{
  /* function copy values of one node to another */
  dest->f_val = org->f_val;
  dest->g_val = org->g_val;
  for(int x = 0; x < size_of_array; x++){
    dest->state[x] = org->state[x];
  }
}

void swap(int new_index, int disk_val, int current_index, short int arr[])
{
  /* function swaps values, producing new states */

  /* calculate new index value if pass new_index > arr_size, or < 0 */
  if(new_index >= size_of_array){
    new_index = disk_val - (size_of_array - current_index);
  }
  if(new_index < 0){
    new_index = size_of_array  - (disk_val - current_index);
  }
  /* swap value within short int array */
  int temp = arr[new_index]; 
  arr[new_index] = arr[current_index];
  arr[current_index] = temp; 
}

void expand_state(node *current, disk arr[])
{
  /* function expands all possible states for a given state,
     and adds them to the fringe */

  /* find uncovered large disk value and its index */
  int disk_val, current_index;
  for(current_index = 0; current_index < size_of_array; current_index++){
    if(current->state[current_index] == 0){
      disk_val = arr[current_index].lrg_val;
      break;
    }
  }
  
  /* calculate the number of possible states based on large value (just 2 if lrg.val = 1) */
  int num_states;
  if(disk_val == 1){
    num_states = 2;
  }
  else{
    num_states = 4;
  }
  
  /*use temp short int array of arrays to calculate possible child states*/
  short int child_states[num_states][size_of_array];
  /*initialize temp array with the current state values (all the same initially)*/
  int x, y;
  for(x = 0; x < num_states; x++){
    for(y = 0; y < size_of_array; y++){
      child_states[x][y] = current->state[y];
    }
  }

  /* calculate all child states for the current state using swap function */
  /* ensure duplicate states are not added if large disk value is 1 */
  if(disk_val == 1 && num_states == 2){
      swap(current_index + 1,disk_val,current_index,child_states[0]); /* right 1*/
      swap(current_index - 1,disk_val,current_index,child_states[1]); /* left 1*/
  }
  else if(num_states == 4){
    swap(current_index + disk_val,disk_val,current_index,child_states[0]); /* right n*/
    swap(current_index - disk_val,disk_val,current_index,child_states[1]); /* left n */
    swap(current_index + 1,disk_val,current_index,child_states[2]); /* right 1*/
    swap(current_index - 1,disk_val,current_index,child_states[3]); /* left 1*/
  }

  /* calculate f values for each child for the current state */
  int child_f_vals[num_states];
  for(x = 0; x < num_states; x++){
    child_f_vals[x] = (current->g_val + 1) + heuristic(child_states[x]);
  }
  
  /* loop through child_f_vals, determining which is the most, and add that
     to the stack (-1 designates alreayd been added), ensures least is popped
     off first and all states added once */
  /* NOTE: states with the same f_val are added in order as they are processed */
  int index = 0, max;
  for(x = 0; x < num_states; x++){
    index = x;
    max = child_f_vals[x];
    /* if max == -1, reset min and index by finding first non -1 value */
    if(max == -1){
      for(y = 0; y < num_states; y++){
	if(child_f_vals[y] != -1){
	  max = child_f_vals[y];
	  index = y;
	}
      }
    }
    /* loop through child_vals, evaluateing to get the least f_val */
    for(y = 0; y < num_states; y++){
      if(child_f_vals[y] > max && child_f_vals[y] != -1){
	max = child_f_vals[y];
	index = y;
      }
    }
    /* add least f_val child to the fringe, set that childs index to -1 */
    push_to_fringe(child_states[index],current->g_val);
    child_f_vals[index] = -1;
  }
  
}

void mem_bound_A(disk arr[])
{
  /*** function implements a version of the Memory-Bounded A* Singe-Agent
       Search Algorithm, using a Stack as the Fringe, and an Array as the
       closed List ***/

  int x;

  /* node used to reference head of fringe when popped */
  node *current_node = NULL;
  current_node = malloc(sizeof(node) + (size_of_array * sizeof(short int)));
  if(current_node == NULL){
    printf("ERROR: malloc failed \n");
    exit(EXIT_FAILURE);
  }

  /* add to and check mem usage */
  curr_mem += (sizeof(node) * (size_of_array * sizeof(short int)));
  check_mem_usage();

  memset(current_node,0,(sizeof(node) + (size_of_array * sizeof(short int))));
  
  /* goal return value used for testing for goal state */
  int goal_state;

  /* initlialize fringe with one node (initial state of board) */
  size_of_fringe = 0;
  fringe_head = NULL;
  short int temp[size_of_array];
  for(x = 0; x < size_of_array; x++){
    temp[x] = arr[x].sml_val;
  }
  /* push first node into fringe (initial state of small disks) 
   NOTE: path_cost argument doesnt matter for the first node, so pass 0*/
  push_to_fringe(temp,0);

  printf("Solution is: \n");

  /* variable to count how many steps it takes to get a solution */
  int steps = 0;

  /* loop through, expanding possible states, and reducing duplicates using
     the closed list, until a goal state is produced */
  while(1){

    steps++;
    /* check to see if fringe is empty, if so return failure */
    if(size_of_fringe <= 0){
      printf("Search could not produce goal state\n");
      exit(EXIT_FAILURE);
    }

    /* remove best path node from fringe, assign to current - right now just removes head*/
    pop_from_fringe(current_node);
    
    for (x = 0; x < size_of_array; x++){
      printf("%d ", current_node->state[x]);
    }
    printf("\t Step: %d\n", steps);

    /* get return value from goal test, if so return that its reached the goal */
    goal_state = goal_test(current_node);
    if(goal_state == 1){
      printf("\nGoal State: ");
      for(int x = 0; x < size_of_array; x++){
	printf("%d ",current_node->state[x]);
      }
      printf("\nHeuristic of goal: %d\n",heuristic(current_node->state));
      
      /* reduce mem usage by subtracting current node */
      curr_mem -= (sizeof(node) * (size_of_array * sizeof(short int)));
      /* free memory and break */
      free(current_node);
      break;
    }
    
    /* expand current state insert all possible states from current node into fringe */

    /* NOTE: checking of closed list done in push_to_fringe function int fringe.c */
    expand_state(current_node,arr);
  }
  
}

int heuristic(short int curr_state[]){
  /* function calculates the heuristic value by 
   returns the number of elements adjacent to similar elements.  
   maximizing this value provides our best-first solution */

  int grouped = size_of_array; 
  /* number of similar elements grouped in the given stat
     check if the first element is similar to the last */
  if(curr_state[size_of_array - 1] == curr_state[0]){
    grouped -= 2;
  }
  
  //for loop checks the state from state[1] to state[end-1] for adjacent pairs
  int full = 0;
  for(int x = 0; x < size_of_array - 1; x++){
    if(curr_state[x] == curr_state[x + 1]){
      grouped--;
      full++;
    }
    
    if(  (curr_state[x] == 0) && (curr_state[x - 1] == 3) && (curr_state[x + 1] == 1) ) {
      grouped--;
    }
    if(full == 2){
      grouped--;
      full = 0;
    } 
  }
  
  //affirms the 0 lies between 3 and 1
  if(  (curr_state[0] == 0) && (curr_state[size_of_array - 1] == 3) && (curr_state[1]) == 1 ) {
    grouped--;
  }
  if(  (curr_state[0] == 1) && (curr_state[size_of_array - 1] == 0) && (curr_state[size_of_array - 2] == 3) ) {
    grouped--;
  }
  
  return grouped;
  
}

void reduce_mem_usage(){
  /* function deteremines the worst possible case but calculating
     the state with the worst heuristic value in the fringe (starts
     from the best 0), and then removes all the nodes below that in
     the fringe stack (removing all those nodes with worse heuristic
     values */

  /* values to keep track of current node, worst case node, and the nodes before
     the current and worst case */
  node * worst_node = fringe_head;
  node * prev_worst = NULL;
  node * temp = fringe_head;
  node * prev = NULL;
  int worst_val = 0;
  
  /* loop through whole fringe, destermining current worst case state value
     and setting node pointers properly */
  while(temp->next != NULL){
    if((temp->f_val - temp->g_val) > worst_val){
      worst_node = temp;
      prev_worst = prev;
      worst_val = temp->f_val - temp->g_val;
    }
    /* break on the first instance of the worst possible valuable */
    if(worst_val == size_of_array){
      break;
    }
    prev = temp;
    temp = temp->next;
  }
  /* ensure node before worst case breaks off and connects to null */
  prev_worst->next = NULL;

  /* loop from the worst node to the end of the stack, deleting all nodes
     till null */
  while(worst_node != NULL){
    printf("removed state: ");
    for(int x = 0; x < size_of_array; x++){
	printf("%d ",worst_node->state[x]);
    }
    printf("\n");

    temp = worst_node->next;
    free(worst_node);
    worst_node = temp;
    /* decrease memory usage and size of fringe */
    curr_mem -= (sizeof(node) + (size_of_array * sizeof(short int)));
    size_of_fringe--;
  }
  worst_node = NULL;
  temp = NULL;
}

#endif /* _AB_GAME_C_ */
