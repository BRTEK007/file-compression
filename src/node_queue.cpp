#include "node_queue.hpp"

node_queue_t* node_queue_create(size_t size){
  node_queue_t* nq = new node_queue_t();
  std::vector<node_t*> nodes;
  nq->nodes = nodes;
  return nq;
}

size_t node_queue_size(node_queue_t* nq){
  return nq->nodes.size();
}

void node_queue_insert(node_queue_t* nq, node_t* node){
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
  nq->nodes.erase(nq->nodes.begin() + index);
  return node;
}

void node_queue_free(node_queue_t* nq){
  free(nq);
}