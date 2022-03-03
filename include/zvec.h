#ifndef ZCC_VECTOR_H
#define ZCC_VECTOR_H

#include <stdlib.h>
#include <string.h>
#include "./ztype.h"

/**
 * memory layout
 * +----+-----+----+
 * |data|alloc|user|
 * | end| end |data|
 * +----+-----+----+
 *            └─ this pointer
**/

#define ZVEC_HSIZ (sizeof(void*) * 2)

#define zvec_t(ty) (ty*)

/**
 * constructors and destructor
**/

#define zvec_new(ty)    \
  zvec_t(ty)malloc(ZVEC_HSIZ)

#define zvec_from(p, n) \
  ({\
    void* vp;           \
    vp = malloc(ZVEC_HSIZ + n * sizeof(p[0]));  \
    if (vp) {                                   \
      vp += ZVEC_HSIZ;                          \
      memcpy(vp, p, sizeof(p[0]) * n);          \
    }\
    (typeof (p))vp;     \
  })

#define zvec_free(this)         \
  do {\
    if (this)                   \
      free((void**)(this) - 2); \
  } while(0)

/**
 * capacity
**/

#define __zvec_size(this)       \
  (__zvec_end(this) - __zvec_begin(this))
#define zvec_size(this)         \
  (this ? __zvec_size(this) : (zise_t)0)

#define zvec_resize(this, n)                            \
  do {\
    void* end = __zvec_end(this);                       \
    __zvec_resize(this, n * sizeof(this[0]));           \
    memset(end, 0, (void*)__zvec_end(this) - end);      \
  } while (0)

#define __zvec_alloc_end(this)  \
  (typeof (this))((void**)this[-2])
#define __zvec_capacity(this)   \
  (__zvec_alloc_end(this) - zvec_begin(this))
#define zvec_capacity(this)     \
  (this ? __zvec_capacity(this) : (zise_t)0)

#define zvec_empty(this)        \
  (zvec_begin(this) == zvec_end(this))

#define zvec_reserve(this, n)   \
  __zvec_reserve(this, n * sizeof(this[0]));

#define zvec_shrink_to_fit(this)\
  __zvec_shrink(this, __zvec_end(this))

/**
 * iteratros
**/

#define __zvec_begin(this)      \
  (this)
#define zvec_begin(this)        \
  __zvec_begin(this)

#define __zvec_end(this)        \
  (typeof (this))((void**)this[-1])
#define zvec_end(this)          \
  (this ? __zvec_end(this) : NULL)

/**
 * modifiers
**/

#define zvec_clear(this)                        \
  do {\
    __zvec_end(this) = __zvec_begin(this);      \
  } while(0)

#define zvec_insert(this, it, v)                \
 ({\
    typeof (this) p = __zvec_emplace(this, it); \
    p ? &(*p = v) : p;                          \
  })

#define zvec_insert_range(this, it, first, last)        \
  ({\
    size_t more = (typeof (this))(last)                 \
      - (typeof (this))(first);                         \
    void* end = __zvec_end(this);                       \
    __zvec_resize(this, __zvec_size(this) + more);      \
    memmove(it + more, it, end - (void*)it)             \
  })

#define zvec_emplace(this, it, ctor, ...)       \
  ({\
    typeof (this) p = __zvec_emplace(this, it); \
    ctor && p ? ctor(p, __VA_ARGS__) : p;       \
  })

#define zvec_erase(this, it)                    \
  ({\
    void* next = NULL;                          \
    if ((it) < __zvec_end(this)) {              \
      void* new_end = --__zvec_end(this);       \
      next = (it) + 1;                          \
      memmove(it, next, new_end - next);        \
    }\
    next;                                       \
  })

#define zvec_erase_range(this, first, last)     \
  ({\
    void* next = NULL;                          \
    if ((first) < (last)                        \
        && (last) < __zvec_end(this)            \
        && __zvec_begin(this) <= (first)) {     \
      void* new_end = (__zvec_end(this) -=      \
        (void*)(last) - (void*)(first));        \
      next = last;                              \
      memmove(first, last, new_end - next);     \
    }\
    next;                                       \
  })

#define zvec_cond_grow(this, ctrl, min_cap)     \
  size_t cap, ncap;                             \
  cap = ncap = __zvec_capacity(this);           \
  ctrl (ncap <= min_cap)                        \
    ncap = __next_capacity(ncap);               \
  if (ncap != cap)                              \
    __zvec_grow(this, sizeof(this[0]) * ncap)

#define zvec_push_back(this, v) \
  do {\
    typeof (this) it = __zvec_emplace(this,     \
      __zvec_end(this));                        \
    *it = v;                                    \
  } while(0)

#define zvec_emplace_back(this, ctor, ...)      \
  zvec_emplace(this, __zvec_end(this),          \
    ctor, __VA_ARGS__)

#define zvec_pop_back(this)     \
  (*__zvec_end(this)--)

#define zvec_swap(this, other)  \

void __zvec_resize(void*, size_t);
void __zvec_reserve(void*, size_t);
void __zvec_shrink(void*, size_t);
void __zvec_grow(void*, size_t)

void __zvec_clear(void*);
void __zvec_emplace(void*, size_t, size_t, size_t);

#define __next_capacity(n)      \
  (n > 2 ? (n >> 1) : 1)

#endif
