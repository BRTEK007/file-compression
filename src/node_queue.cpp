#include "node_queue.hpp"

node_queue_t* node_queue_create(size_t size){
  node_queue_t* nq = new node_queue_t();
  // cvector_vector_type(node_t*) nodes = NULL;
  // cvector_reserve(nodes, size);
  std::vector<node_t*> nodes;
  nq->nodes = nodes;
  return nq;
}

size_t node_queue_size(node_queue_t* nq){
  return nq->nodes.size();
  // return cvector_size(nq->nodes);
}

void node_queue_insert(node_queue_t* nq, node_t* node){
  // cvector_push_back(nq->nodes, node);
  nq->nodes.push_back(node); 
}

node_t* node_queue_pop(node_queue_t* nq){
  node_t* node = nq->nodes[0];
  int index = 0;
  for(size_t i = 0; i < nq->nodes.size(); i++){
    if(nq->nodes[i]->freq < node->freq){
      node = nq->nodes[i];
      index = i;
    }
  }
  // cvector_erase(nq->nodes, index);
  nq->nodes.erase(nq->nodes.begin() + index);
  return node;
}

void node_queue_free(node_queue_t* nq){
  // cvector_free(nq->nodes);
  free(nq);
}