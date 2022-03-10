#include "./zvec.h"

void __zvec_grow(zvec_h* this, size_t bytes) {
  size_t used = ztype_n_bytes(this->begin, this->end);
  this->begin = realloc(this->begin, bytes);
  this->end = this->begin + used;
  this->alloc_end = this->begin + bytes;
}

void __zvec_shrink(zvec_h* this, size_t bytes) {
  if (this->begin == NULL)
    return;

  this->begin = realloc(this->begin, bytes);
  this->alloc_end = this->begin + bytes;
}

void __zvec_resize(zvec_h* this, size_t bytes) {
  size_t used = ztype_n_bytes(this->begin, this->end);
  size_t diff;
  void* it;

  if (bytes > used) {
    diff = bytes - used;
    it = __zvec_emplace(this, this->end, diff);
    memset(it, 0, diff);
  }

  this->end = this->begin + bytes;
}
