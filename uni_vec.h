#ifndef UNI_VEC_H
#define UNI_VEC_H

#include <stdlib.h>
#include <string.h>

//add explanation, license, how to use, memory checks, resizing

typedef struct uni_vec_t{
  void* arr;
  size_t size;
  size_t capacity;
  size_t el_size;
} uni_vec_t;

uni_vec_t* uni_vec_create(size_t n_size, size_t el_size);

void* uni_vec_get(uni_vec_t* v, size_t i);

void uni_vec_push(uni_vec_t* v, void* data);

void uni_vec_push_array(uni_vec_t* v, void* data, size_t size_n);

void uni_vec_free(uni_vec_t* v);

#endif

#ifdef UNI_VEC_IMPLEMENTATION

#ifndef UNI_VEC_IMPLEMENTATION_ONCE
#define UNI_VEC_IMPLEMENTATION_ONCE

uni_vec_t* uni_vec_create(size_t n_size, size_t el_size){
  uni_vec_t* v = malloc(sizeof(uni_vec_t));
  v->arr = calloc(n_size, el_size);
  v->size = 0;
  v->el_size = el_size;
  v->capacity = n_size;
  return v;
}

void* uni_vec_get(uni_vec_t* v, size_t i){
  return v->arr + i*v->el_size;
}

void uni_vec_push(uni_vec_t* v, void* data){
  uni_vec_push_array(v, data, 1);
}

void uni_vec_free(uni_vec_t* v){
  free(v->arr);
  free(v);
}

void uni_vec_push_array(uni_vec_t* v, void* data, size_t size_n){
    memcpy(v->arr + v->size*v->el_size, data, v->el_size * size_n);
    v->size += size_n;
}

#endif
#endif