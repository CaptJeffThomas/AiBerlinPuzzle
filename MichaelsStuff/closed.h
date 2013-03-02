/*****************************************************************
 NAME: Croydon Enders, Michael Hnatiw, Jeff Thomas
 CLASS/ASSIGNMENT: CMPT355 Project #1
 Instructor: Calin Anton
*****************************************************************/

#ifndef _CLOSED_H_
#define _CLOSED_H_

#include "global.h"

typedef struct closed_node closed_node;

/* closed list struct containing state and next pointer */
struct closed_node{
  closed_node * next;
  short int state[];
};

/* global closed list head pointer */
closed_node * closed_head;

/* functions that manipulate the closed list */
void add_closed(node * ptr);
int closed_contains(node * ptr);
void clear_closed();

#endif /* _CLOSED_H_ */
