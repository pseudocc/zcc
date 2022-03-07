#include "./zvec.h"

zvec_h* __zvec_new(void) {
  return calloc(sizeof(zvec_h), 1);
}

void __zvec_free(zvec_h* this) {
  if (this)
    free(this->begin);
  free(this);
}

