/*****************************************************************
 NAME: Croydon Enders, Michael Hnatiw, Jeff Thomas
 CLASS/ASSIGNMENT: CMPT355 Project #1
 Instructor: Calin Anton
*****************************************************************/

#ifndef _FRINGE_H_
#define _FRINGE_H_

#include "global.h"

/* global fringe pointer and size variable used to dynamically build fringe */
node *fringe_head;
int size_of_fringe;

/* functions for queue structure made from a_nodes */
void push_to_fringe(short int state[], short int path_cost);
void pop_from_fringe(node *current);
void clear_fringe();


#endif /* _FRINGE_H_ */
