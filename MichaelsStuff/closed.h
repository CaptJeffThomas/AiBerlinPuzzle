
#ifndef _CLOSED_H_
#define _CLOSED_H_

#include "func.h"

typedef struct closed_node closed_node;

struct closed_node{
  closed_node * next;
  short int state[];

};

closed_node * closed_head;

void add_closed(node * ptr);
int closed_contains(node * ptr);

#endif /* _CLOSED_H_ */
