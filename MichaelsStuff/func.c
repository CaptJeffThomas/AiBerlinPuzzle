#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include"func.h"
#include"hash.h"

extern hash_head *closed_list;


void usage()
{
  /* function produces usage message for user */
  printf(" AB program imposes a single Agent Search Method to complete a game.\n\n \
    NOTE: given no argumenta with a number to produce defined layout from \
input.txt\n\n    Options:\n        -u    display usage message.\n \
        -r <number> use number as n value to produce random board layout\n \
    Example:\n         ./AB 10\n");
}

/* ensuring we are within the memory usage constraints */
void check_mem_usage(){
  printf("current memory usage is: %d/%d\n", curr_mem, MEMSIZE);
  if (curr_mem > MEMSIZE){
    printf("Ran out of memory ... pruning :(\n\n");
    prune();
  }
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

void random_disk_setup(int n, disk arr[]){

  // produce random seed with each call
  srand(0);

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
void enqueue(short int newState[],short int path_cost){

  /* function to add node at the back of the queue */
  node *new = NULL;
  new = malloc(sizeof(node) + (size_of_array * sizeof(short int)));
  
  if(new == NULL){
    printf("ERROR: malloc failed \n");
    exit(EXIT_FAILURE);
  }

  /* check mem usage */
  curr_mem += (sizeof(node) + (size_of_array * sizeof(short int))) * 8;
  check_mem_usage();

  memset(new,0,(sizeof(node) + (size_of_array * sizeof(short int))));

  

  /* run through inputted state and assign 
      values to node being added */
  for(int x = 0; x < size_of_array; x++){
      new->state[x] = newState[x];
  }
  
  /* check head if added as head, or to end of queue */
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



void copy_node(node *org, node *dest)
{
  /* copy values of one node to another */
  dest->f_val = org->f_val;
  dest->g_val = org->g_val;
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

    /* reduce mem usage */
    curr_mem -= sizeof(node) * 8;

    free(temp);
    temp = NULL;
    fringe_head = NULL;
  }
  else if(size_of_fringe > 1){
    
    fringe_head = temp->next;
    
    copy_node(temp,current);
    
    /* reduce mem usage */
    curr_mem -= sizeof(node) * 8;

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
  printf("added states to the fringe\nNOTE: prints nodes backwards cause of stack\n");
  while(del != NULL){
    int y;
    for(y = 0; y < size_of_array; y++){
      printf("%d ",del->state[y]);
    }
    printf("\n");

    temp = del->next;

    /* reduce mem usage */
    curr_mem -= sizeof(node) * 8;

    free(del);
    del = temp;
  }
  del = NULL;
  temp = NULL;
}

int goal_test(node *current)
{
  
  int x, count, val, n; 
  //x is a simple index of our current state, val is the value at index in a goal state we compare against, 
  //count is the number of similar val's which = n, n the number of distinct small disks provided on commandline

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
  
  //use temp short int array of arrays to calculate possible child states
  short int child_states[num_states][size_of_array];
  //initialize temp array with the current state values (all the same initially)
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
    printf("f value for child %d: %d\n",x,heuristic(child_states[x]));
  }

  printf("child states\n");
  for(x = 0; x < num_states; x++){
    for(y = 0; y < size_of_array; y++){
      printf("%d ",child_states[x][y]);
    }
    printf("\n");
  }

  
  /* loop through child_f_vals, determining which is the most, and add that
     to the stack (-1 designates alreayd been added), ensures least is popped
     off first and all states added once */
  /* NOTE: states witht eh same f_val are added in order as they are processed */
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
    enqueue(child_states[index],current->g_val);
    child_f_vals[index] = -1;
  }
  
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

  /* add to and check mem usage */
  curr_mem += sizeof(node);
  check_mem_usage();

  memset(current_node,0,(sizeof(node) + (size_of_array * sizeof(short int))));
  
  /* goal return value used for testing for goal state */
  int goal_state;

  /* initialize closed/extended list(hash map) */
  //closed_list = init_hash(size_of_array);

  /* initlialize fringe with one node (initial state of board) */
  size_of_fringe = 0;
  fringe_head = NULL;

  short int temp[size_of_array];
  for(int x = 0; x < size_of_array; x++){
    temp[x] = arr[x].sml_val;
  }
  /* enqueue first node into fringe (initial state of small disks) 
   NOTE: path_cost argument doesnt matter for the first node, so pass 0*/
  enqueue(temp,0);

  printf("Solution is: \n");

  int z = 0;
  while(1){
    /* check to see if fringe is empty, if so return failure */
    if(size_of_fringe <= 0){
      printf("Search could not produce goal state\n");
      exit(EXIT_FAILURE);
    }

    /* remove best path node from fringe, assign to current - right now just removes head*/
    dequeue(current_node);
    
    for(int y = 0; y < size_of_array; y++){
	printf("%d ",current_node->state[y]);
    }
    printf("\n");

    /* get return value from goal test, if so return that its reached the goal */
    goal_state = goal_test(current_node);
    if(goal_state == 1){
      printf("Goal!!!!\n");
      printf("Heuristic of goal: %d\n",heuristic(current_node->state));
      
      /* reduce mem usage */
      curr_mem -= sizeof(node);

      free(current_node);
      break;
    }
    
    /* check the closed list to see if the current state is in it */
    //int check = hash_contains(closed_list,current_node);
    //if(check == 1){
      /* if closed list doesnt contain a node, add it to the closed
	 list and expand */
      //insert_to_hash(closed_list,current_node);

      //printf("added: ");
      //for(int x = 0; x < size_of_array; x++){
    //printf("%d ",current_node->state[x]);
    //}
    //printf("to the closed list !\n");
      
      /* insert all possible states from current node into fringe */
      expand_state(current_node,arr);
      //}

    //z++;
  }

  /* free memory used for node assignment in SMA function */
  /* reduce mem usage */
  //curr_mem -= sizeof(node);
  //free(current_node);
}


//returns the number of elements adjacent to similar elements.  
//maximizing this value provides our best-first solution
int heuristic(short int curr_state[]){

 int grouped = size_of_array; 
 // number of similar elements grouped in the given stat
 //check if the first element is similar to the last
 if(curr_state[size_of_array - 1] == curr_state[0]){
    grouped -= 2;
    //printf("simlar items on the edge of list %d \n", grouped);
 }

 //for loop checks the state from state[1] to state[end-1] for adjacent pairs
 int full = 0;
 for(int x = 0; x < size_of_array - 1; x++){
   if(curr_state[x] == curr_state[x + 1]){
     grouped--;
     full++;
     //printf("next item is similar %d \n", grouped);
 }

 if(  (curr_state[x] == 0) && (curr_state[x - 1] == 3) && (curr_state[x + 1] == 1) ) {
  grouped--;
  //printf("3 0 1 in the middle of list %d \n", grouped);
 }
 //printf("full: %d\n",full);
 if(full == 2){
   grouped--;
   full = 0;
   //printf("full set of 3 together %d\n", grouped);
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

void prune(){
  node * worst_node = fringe_head;
  node * temp = fringe_head;
  int worst_val = 0;

  while(temp->next != NULL){
    if((temp->f_val - temp->g_val) > worst_val){
      worst_node = temp;
      worst_val = temp->f_val - temp->g_val;
    }
    temp = temp->next;
  }

  while (worst_node != NULL){
    temp = worst_node;
    worst_node = temp->next;
    free(temp);
    curr_mem -= sizeof(node);
    size_of_fringe--;
  }
}

void print_list(){
  
  hash_head * hashTemp = closed_list;

  node * temp;
  while (hashTemp != NULL){
    temp = hashTemp->head;
    while(temp!=NULL){
      for (int i = 0; i<size_of_array; i++){
	printf("%d ", temp->state[i]);
      }
      printf("\n");
      temp=temp->next;
    }
    hashTemp = hashTemp->next;
  }
}
