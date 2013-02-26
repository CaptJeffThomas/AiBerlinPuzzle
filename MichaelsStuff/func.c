#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<math.h>
#include"func.h"

#define NELEMS(x) (sizeof(x)/sizeof(x[0]))

void usage()
{
  /* function produces usage message for user */
  printf(" AB program imposes a single Agent Search Method to complete a game.\n\n \
    NOTE: given no argumenta with a number to produce defined layout from \
input.txt\n\n    Options:\n        -u    display usage message.\n        -r <number> use number as n value to produce random board layout\n    Example:\n         ./AB 10\n");
}

//disk position config based off users input 
void disk_setup(int n, disk arr[]){ 

  char largeDisks[n*n + 1]; 
  char smallDisks[n*n + 1]; 

  //in any situation where input wasn't guaranteed we wouldn't use gets. however we've been assured it is godly.
  //note that below it asks for clockwise order and this is arbitrary choice that would only be relevant to some1 
  //with a physical puzzle.  Regardless it guarantees similar ordering for both small and large disks.
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

void random_disk_setup(int n, disk arr[]){

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
    new->next = NULL;
    fringe_head = new;
    new->g_val = 0;
    //new->f_val = new->g_val + heuristic(new);
  }
  else{
    fringe_head->next = new;
    new->next = NULL;
    new->g_val = new->next->g_val + 1;
    //new->f_val = new->g_val + heuristic(new);
  }
  printf("heuristic: %d\n",heuristic(new));
  size_of_fringe++;
}


void copy_node(node *org, node *dest)
{
  /* copy values of one node to another */
  dest->f_val = org->f_val;
  for(int x = 0; x < size_of_array; x++){
    dest->state[x] = org->state[x];
  }
}

void dequeue(node *current)
{
  /* function to remove node from start of queue */
  node *temp = fringe_head;
  if(size_of_fringe == 1){

    copy_node(temp,current);

    free(temp);
    temp = NULL;
    fringe_head = NULL;
  }
  else if(size_of_fringe > 1){
    
    fringe_head = temp->next;
    
    copy_node(temp,current);
    
    free(temp);
    temp = NULL;
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
  node *del = fringe_head; 
  node *temp = NULL;
  while(del != NULL){
    temp = del->next;
    free(del);
    del = temp;
  }
  del = NULL;
  temp = NULL;
}

int goal_test(node *current)
{
  
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
  for(x = start + 1, count = 0; x < size_of_array; x++, count++){
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
  if(new_index >= size_of_array){
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
  for(index = 0; index < size_of_array; index++){
    if(current->state[index] == 0){
      disk_val = arr[index].lrg_val;
      break;
    }
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
  //  int closed_idx = 0;
  disk closed[size_of_array];
  memset(closed,0,(size_of_array * sizeof(disk)));

  /* initlialize fringe with one node (initial state of board) */
  size_of_fringe = 0;
  fringe_head = NULL;

  short int temp[size_of_array];
  for(int x = 0; x < size_of_array; x++){
    temp[x] = arr[x].sml_val;
  }
  /* enqueue first node into fringe (initial state of small disks) */
  enqueue(temp);

  printf("Solution is: \n");

  int z = 0;
  while( z < 1){
    /* check to see if fringe is empty, if so return failure */
    if(size_of_fringe <= 0){
      printf("Search could not produce goal state\n");
      exit(EXIT_FAILURE);
    }

    /* remove best path node from fringe, assign to current - right now just removes head*/
    dequeue(current_node);
    
    /* get return value from goal test, if so return that its reached the goal */
    goal_state = goal_test(current_node);
    if(goal_state == 1){
      printf("Goal!!!!\n");
      for(int y = 0; y < size_of_array; y++){
	printf("%d ",current_node->state[y]);
      }
      printf("\n");
      free(current_node);
      return;
    }
    
    /* insert all possible states from current node into fringe */
    insert_all(current_node,arr);

    z++;
    //break;
  }

  /* free memory used for node assignment in SMA function */
  free(current_node);

}


//returns the number of elements adjacent to similar elements.  maximizing this value provides our best-first solution
int heuristic(node *curr){
 
  int grouped = 0; // number of similar elements grouped in the given stat
  
  //check if the first element is similar to the last
  if(curr->state[size_of_array - 1] == curr->state[0]){
    grouped+=2;
    printf("simlar items on the edge of list %d \n", grouped);
  }

  //for loop checks the state from state[1] to state[end-1] for adjacent pairs
  int full = 0;
  for(int x = 0; x < size_of_array - 1; x++){
    if(curr->state[x] == curr->state[x + 1]){
      grouped++;
      full++;
      printf("next item is similar %d \n", grouped);
    }
    
    if(  (curr->state[x] == 0) && (curr->state[x - 1] == 3) && (curr->state[x + 1] == 1) ) {
      grouped++;
      printf("3 0 1 in the middle of list %d \n", grouped);
    }
    printf("full: %d\n",full);
    if(full == 2){
      grouped++;
      full = 0;
      printf("full set of 3 together %d\n", grouped);
    } 
  }

  //affirms the 0 lies between 3 and 1
  if(  (curr->state[0] == 0) && (curr->state[size_of_array - 1] == 3) && (curr->state[1]) == 1 ) {
      grouped++;

    } 
  if(  (curr->state[0] == 1) && (curr->state[size_of_array - 1] == 0) && (curr->state[size_of_array - 2] == 3) ) {
      grouped++;

    }  
   
  return grouped;

}

