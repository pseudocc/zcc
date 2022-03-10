#include "./zvec.h"

void* __zvec_emplace(zvec_h* this, void* it, size_t bytes) {
  size_t allocated = ztype_n_bytes(this->begin, this->alloc_end);
  size_t used = ztype_n_bytes(this->begin, this->end);
  size_t it_bias = ztype_n_bytes(this->begin, it);
  size_t needed = used + bytes;

  if (allocated < needed) {
    do
      allocated += allocated > 2 ? (allocated >> 1) : 1;
    while (allocated < needed);
    __zvec_grow(this, allocated);
  }

  it = this->begin + it_bias;
  this->end = this->begin + needed;
  memmove(it + bytes, it, used - it_bias);

  return it;
}

void __zvec_swap(zvec_h* this, zvec_h* other) {
  zvec_h tmp = *this;

  this->begin = other->begin;
  this->end = other->end;
  this->alloc_end = other->alloc_end;

  other->begin = tmp.begin;
  other->end = tmp.end;
  other->alloc_end = tmp.alloc_end;
}
