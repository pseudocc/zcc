#include "./zvec.h"

void __zvec_grow(zvec_h* this, size_t bytes) {
  size_t used = ztype_n_bytes(this->begin, this->end);
  this->begin = realloc(this->begin, bytes);
  this->end = this->begin + used;
  this->alloc_end = this->begin + bytes;
}

void __zvec_shrink(zvec_h* this, size_t bytes) {
  this->begin = realloc(this->begin, bytes);
  this->alloc_end = this->begin + bytes;
}
