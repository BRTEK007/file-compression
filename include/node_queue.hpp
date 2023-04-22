#ifndef NODE_QUEUE_H
#define NODE_QUEUE_H

#include <stdlib.h>

#define CVECTOR_LOGARITHMIC_GROWTH

#include "tree.hpp"

typedef struct node_queue_t{
  std::vector<node_t*> nodes;
} node_queue_t;

node_queue_t* node_queue_create(size_t size);

size_t node_queue_size(node_queue_t* nq);

void node_queue_insert(node_queue_t* nq, node_t* node);

node_t* node_queue_pop(node_queue_t* nq);

void node_queue_free(node_queue_t* nq);

#endif