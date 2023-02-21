#include "node_queue.h"

node_queue_t* node_queue_create(size_t size){
  node_queue_t* nq = malloc(sizeof(node_queue_t));
  cvector_vector_type(node_t*) nodes = NULL;
  cvector_reserve(nodes, size);
  nq->nodes = nodes;
  return nq;
}

size_t node_queue_size(node_queue_t* nq){
  return cvector_size(nq->nodes);
}

void node_queue_insert(node_queue_t* nq, node_t* node){
  cvector_push_back(nq->nodes, node); 
}

node_t* node_queue_pop(node_queue_t* nq){
  node_t* node = nq->nodes[0];
  int index = 0;
  for(size_t i = 0; i < cvector_size(nq->nodes); i++){
    if(nq->nodes[i]->freq < node->freq){
      node = nq->nodes[i];
      index = i;
    }
  }
  cvector_erase(nq->nodes, index);
  return node;
}

void node_queue_free(node_queue_t* nq){
  cvector_free(nq->nodes);
  free(nq);
}