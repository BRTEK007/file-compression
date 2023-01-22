#ifndef UNI_VEC_H
#define UNI_VEC_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//add explanation, license, how to use, memory checks, resizing, mention no bounds checking
//add pop back, front, pop at index, push front
//does not dynamically shrink, only expands

typedef struct uni_vec_t{
  void* arr;
  size_t size;
  size_t capacity;
  size_t el_size;
} uni_vec_t;

uni_vec_t* uni_vec_create(size_t n_size, size_t el_size);

void* uni_vec_get(uni_vec_t* v, size_t i);//returns pointer to element in array

void uni_vec_extract(uni_vec_t* v, size_t i, void* dest);//copies element ot destination

void uni_vec_push_back(uni_vec_t* v, void* data);

void uni_vec_push_back_array(uni_vec_t* v, void* data, size_t size_n);

void uni_vec_push_front(uni_vec_t* v, void* data);

void uni_vec_free(uni_vec_t* v);

void uni_vec_pop_back(uni_vec_t* v);

void uni_vec_pop_front(uni_vec_t* v);

void uni_vec_clear(uni_vec_t* v);

#endif

#ifdef UNI_VEC_IMPLEMENTATION

#ifndef UNI_VEC_IMPLEMENTATION_ONCE
#define UNI_VEC_IMPLEMENTATION_ONCE
void uni_vec_resize(uni_vec_t* v, size_t new_capacity);
void uni_vec_malloc_fail();

void uni_vec_resize(uni_vec_t* v, size_t new_capacity){
  void* new_arr = realloc(v->arr, new_capacity*v->el_size);
  if(new_arr == NULL)
    uni_vec_malloc_fail();
  v->arr = new_arr;
  v->capacity = new_capacity;
}

void uni_vec_malloc_fail(){
  fprintf(stderr, "ERROR: uni_vec_t, failed to allocate memory");
  exit(99);
}

//-------------------------------------------------------------

uni_vec_t* uni_vec_create(size_t n_size, size_t el_size){
  uni_vec_t* v = malloc(sizeof(uni_vec_t));
  v->arr = calloc(n_size, el_size);
  if(v->arr == NULL)
    uni_vec_malloc_fail();
  v->size = 0;
  v->el_size = el_size;
  v->capacity = n_size;
  return v;
}

void* uni_vec_get(uni_vec_t* v, size_t i){
  return v->arr + i*v->el_size;
}

void uni_vec_extract(uni_vec_t* v, size_t i, void* dest){
  memcpy(dest, v->arr + i*v->el_size, v->el_size);
}

void uni_vec_push_back(uni_vec_t* v, void* data){
  uni_vec_push_back_array(v, data, 1);
}

void uni_vec_free(uni_vec_t* v){
  free(v->arr);
  free(v);
}

void uni_vec_push_back_array(uni_vec_t* v, void* data, size_t size_n){
    if(v->size + size_n > v->capacity){//not enough space -> resize
      size_t new_capacity = (v->size+size_n) * 2;
      uni_vec_resize(v, new_capacity);
    }
    memcpy(v->arr + v->size*v->el_size, data, v->el_size * size_n);
    v->size += size_n;
}

void uni_vec_push_front(uni_vec_t* v, void* data){
  if(v->size + 1 > v->capacity){//not enough space -> resize
    size_t new_capacity = (v->size+1) * 2;
    uni_vec_resize(v, new_capacity);
  }
  void* arr_ptr = v->arr + v->size*v->el_size;//point to empty element at the end
  while(arr_ptr != v->arr){//while not pointing to the first element
    //copy previous element
    void* prev_ptr = arr_ptr - v->el_size;
    memcpy(arr_ptr, prev_ptr, v->el_size);
    arr_ptr = prev_ptr;
  }

  memcpy(v->arr, data, v->el_size);

  v->size++;
}

void uni_vec_pop_back(uni_vec_t* v){
  v->size--;
}

void uni_vec_pop_front(uni_vec_t* v){ 
  void* arr_ptr = v->arr;
  while(arr_ptr != v->arr){//while not pointing to the first element
    //copy previous element
    void* prev_ptr = arr_ptr - v->el_size;
    memcpy(arr_ptr, prev_ptr, v->el_size);
    arr_ptr;
  }

  v->size--;
  
  for(int i = 0; i < v->size; i++){
    memcpy(v->arr+i*v->el_size, v->arr+(i+1)*v->el_size, v->el_size);
  }
}

void uni_vec_clear(uni_vec_t* v){
  v->size = 0;
}


#endif
#endif
