
#ifndef _HASH_H_
#define _HASH_H_

#include <stdio.h>
#include <stdlib.h>
#include "func.h"

#define NELEMS(x) sizeof(x)/sizeof(x[0])

typedef struct hash_head hash_head;

struct hash_head{
  node * head;
  hash_head * next;
};

hash_head * init_hash(int size);
int hash(node * ptr);
void insert_to_hash(hash_head * map, node * ptr);
void remove_from_hash(hash_head * map, node * ptr);
int hash_contains(hash_head * map, node * ptr);

#endif /* _HASH_H_ */
