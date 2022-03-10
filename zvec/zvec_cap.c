#include "./zvec.h"

#define ALLOC_ALIGN_BYTES 4
void __zvec_grow(zvec_h* this, size_t bytes) {
  size_t remain = remain % ALLOC_ALIGN_BYTES;
  size_t used = ztype_n_bytes(this->begin, this->end);

  if (remain)
    bytes += ALLOC_ALIGN_BYTES - remain;

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
