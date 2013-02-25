//AI 355 Project 1 
//Feb 2013 

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<math.h>
#include"solve.h"

#define NELEMS(x) (sizeof(x)/sizeof(x[0]))

int seed_val(){
  /* function returns seed value based on current
     time in seconds */
  time_t current = time(0);
  struct tm *time = localtime(&current);
  if(time == NULL){
    printf("error extracting local time\n");
    exit(EXIT_FAILURE);
  }
  return (((time->tm_hour * 60) * 60) + (time->tm_min * 60) + time->tm_sec);
}

//disk position config based off users input 
void disk_setup(int n, disk arr[]){ 

    char largeDisks[n*n + 1]; 
    char smallDisks[n*n + 1]; 

    //in any situation where input wasn't guaranteed we wouldn't use gets. however we've been assured it is godly.
    //note that below it asks for clockwise order and this is arbitrary choice that would only be relevant to some1 with a physical puzzle.  Regardless it guarantees similar ordering for both small and large disks.
    printf("Please enter the values of the large disks in clockwise order: \n"); 
    gets(largeDisks); 
    printf("\n"); 

    printf("Please enter the values of the small disks in clockwise order (use 0 to represent the empty slot): \n"); 
    gets(smallDisks); 
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




//random assignment of disk placement.  not entirely necessary but fun for the whole family
void randDisk_setup(int n, disk arr[]){

  // call function to produce random seed with each call
  int seed = seed_val();
  srand(seed);

  // get random values for small and large disk n position
  short int l_r = (int)((double)rand() / ((double)RAND_MAX + 1) * ((n * n) + 1));
  short int s_r = (int)((double)rand() / ((double)RAND_MAX + 1) * ((n * n) + 1));
  
  // set possible value max (n), used as temporaray to assign
  // values between 1 and n
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




/*** FUNCTIONS TO MAKE FRINGE (PRIOIRT QUEUE) ***/
void enqueue(short int state[])
{
  /* function to add node at the back of the queue */
  node *new = NULL;
  new = malloc(sizeof(node) + (size_of_array * sizeof(short int)));
  if(new == NULL){
    printf("ERROR: malloc failed \n");
    exit(EXIT_FAILURE);
  }
  memset(new,0,(sizeof(node) + (size_of_array * sizeof(short int))));

  /* run through inputted state and assign 
     values to node being added */
  for(int x = 0; x < size_of_array; x++){
    new->state[x] = state[x];
  }
  
  /* check head if added as head, or to end of queue */
  if(size_of_fringe == 0){
    new->parent = NULL;
    new->pathCost = 0;
    fringe_tail = new;
  }
  else{
    new->parent = fringe_tail;
    new->pathCost = new->parent->pathCost + 1;
    fringe_tail = new;
  }

  size_of_fringe++;
}



void copy_node(node *org, node *dest)
{
  /* copy values of one node to another */
  dest->pathCost = org->pathCost;
  for(int x = 0; x < size_of_array; x++){
    dest->state[x] = org->state[x];
  }

}




void dequeue(node *current)
{
  /* function to remove node from start of queue */
  node *del = fringe_tail;
  if(size_of_fringe == 1){

    copy_node(del,current);

    free(del);
    del = NULL;
    fringe_tail = NULL;
    //*current = fringe_tail;
  }
  else if(size_of_fringe > 1){
    for(; del->parent->parent != NULL; del = del->parent);

    copy_node(del->parent,current);

    /* reset start of the queue */
    free(del->parent);
    del->parent = NULL;
    //*current = del->parent;
  }
  else{
    printf("ERROR: fringe is empty can't dequeue\n");
    exit(EXIT_FAILURE);
  }

  size_of_fringe--;
}



void clear_queue()
{
  /* function runs through queue, deleting nodes */
  node *del = fringe_tail; 
  node *temp = NULL;
  while(del != NULL){
    temp = del->parent;
    free(del);
    del = temp;
  }
  del = NULL;
  temp = NULL;
}



/* function returns 1 if goal state reached, 0 if not */
int goal_test(node *current){

  
  int x, count, val, n; //x is a simple index of our current state, val is the value at index in a goal state we compare against, count is the number of similar val's which = n, n the number of distinct small disks provided on commandline

  n = sqrt(size_of_array - 1); // n value from command line
  int start; //holds the index of the uncovered disk, i.e. state's 0 value

  //find where our uncovered disk is
  for(start=0; start < size_of_array; start++){
	if(current->state[start] == 0){
		break;
	}
	
  }
  
  //from our start index, scan the state to see if we have a goal state.
  val = 1;
  for(x = start + 1, count = 0; x < size_of_array - start; x++, count++){
      if(count == n){
          val++;
          count = 0;
      }
      if(current->state[x] != val){
	  return 0;
      }
  }
   /* We've scanned from the start (0-value) index to the end and everything is in order if we've arrived here.  Now we make sure everything from state[0] to state[start] is also in order. */
  for(x=0; x < start; x++,count++){
      if(count == n){
          val++;
          count = 0;
      }
      if(current->state[x] != val){
	  return 0;
      }
  }
  
  // Victory!  The ewoks dance tonight!  
  return 1;

}




void swap(int new_index, int disk_val, int current_index, short int arr[])
{
  /* calculate new index value if pass new_index > arr_size, or < 0
   (Treats array as circular array) */
  if(new_index > size_of_array){
    new_index = disk_val - (size_of_array - current_index);
  }
  if(new_index < 0){
    new_index = size_of_array  - (disk_val - current_index);
  }
  /* swap value within short int array */
  int temp = arr[new_index]; 
  arr[new_index] = arr[current_index]; // 0
  arr[current_index] = temp; // some value
}



void expand_node(int disk_val, int current_index, short int state[])
{
  /*** right now, function just picks random option between the 4 possible
       next states from the current state to add to the queue ***/
  int seed = seed_val();
  srand(seed);

  int rstate = (int)((double)rand() / ((double)RAND_MAX + 1) * 4);
  switch(rstate){
      case 0:
	/* right n */
	swap(current_index + disk_val,disk_val,current_index,state);
	break;
      case 1:
	/* left n */
	swap(current_index - disk_val,disk_val,current_index,state);
	break;
      case 2:
	/* right 1 */
	swap(current_index + 1,disk_val,current_index,state);
        break;
      case 3:
	/* left 1 */
	swap(current_index - 1,disk_val,current_index,state);
  }
  
  /* add random next state to fringe */
  enqueue(state);
  
}




void insert_all(node *current, disk arr[])
{
  /* function expands all possible states for a given state,
     and adds them to the fringe */

  /* find uncovered large disk value and its index */
  int disk_val, index;
  index = 0;
  while(index < size_of_array){
    if(current->state[index] == 0){
      disk_val = arr[index].lrg_val;
      break;
    }
    index++;
  }
  
  /* initialize temp short int array to used as base for swapping*/
  short int temp[size_of_array];
  for(int x = 0; x < size_of_array; x++){
    temp[x] = current->state[x];
  }
  
  /* go through each 4 (or 2) possible states to add to the fringe */
  expand_node(disk_val,index,temp);

}



void mem_bound_A(disk arr[])
{
  int y;
  /* node used to reference head of fringe when dequeued */
  node *current_node = NULL;
  current_node = malloc(sizeof(node) + (size_of_array * sizeof(short int)));
  if(current_node == NULL){
    printf("ERROR: malloc failed \n");
    exit(EXIT_FAILURE);
  }
  memset(current_node,0,(sizeof(node) + (size_of_array * sizeof(short int))));


  /* goal return value used for testing for goal state */
  int goal_state;

  /* initialize closed/extended list */
  int closed_idx = 0;
  disk closed[size_of_array];
  memset(closed,0,(size_of_array * sizeof(disk)));

  /* initlialize fringe with one node (initial state of board) */
  size_of_fringe = 0;
  fringe_tail = NULL;

  short int temp[size_of_array];
  for(int x = 0; x < size_of_array; x++){
    temp[x] = arr[x].sml_val;
  }
  /* enqueue first node into fringe (initial state of small disks) */
  enqueue(temp);

  printf(" --- Initial State ---\n");

  int z = 0;
  while( z < 10){
    /* check to see if fringe is empty, if so return failure */
    if(size_of_fringe <= 0){
      printf("Search could not produce goal state\n");
      exit(EXIT_FAILURE);
    }

    /* remove best path node from fringe, assign to current - right now just removes head*/
    dequeue(current_node);

    /* get return value from goal test, if so return that its reached the goal */
    goal_state = goal_test(current_node);
    if(goal_state){
      printf("Goal!!!!: %d\n",goal_state);
      free(current_node);
      return;
    }
    
    /* insert all possible states from current node into fringe */
    insert_all(current_node,arr);

    for(y = 0; y < size_of_array; y++){
      printf("%d ",current_node->state[y]);
    }
    printf("\n");
    

    z++;
    //break;
  }

  /* free memory used for node assignment in SMA function */
  free(current_node);

}




//returns the node 
int heuristic(int input, disk arr[]){
  
  int finalVal;
  int size = (input * input) + 1;

  printf("Heuristics:\n");

  for (int i=0; i<size; i++){
    finalVal=(int)(i/input)+1;
    finalVal=arr[i].sml_val-finalVal;
    printf("arr[%d] = %d\n", i, finalVal);
  }
  printf("\n");
}





