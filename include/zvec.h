#ifndef ZCC_VECTOR_H
#define ZCC_VECTOR_H

#include <stdlib.h>
#include <string.h>
#include "./ztype.h"

typedef struct zvec_header zvec_h;

struct zvec_header {
  void* begin;
  void* end;
  void* alloc_end;
};

#define zvec_t(ty) ty**
#define zvec_it(ty) ty*

#define __zvec_alloc_type(this) \
  typeof (*this)

#define __zvec_value_type(this) \
  typeof (**this)

/**
 * constructor and destructor
**/

#define zvec_new(ty)    \
  ((zvec_t(ty)) __zvec_new())

#define zvec_free(this) \
  __zvec_free((zvec_h*)this)

/**
 * capacity
**/

#define __zvec_size(this)                               \
  ((__zvec_alloc_type(this))__zvec_end(this)            \
    - (__zvec_alloc_type(this))__zvec_begin(this))
#define zvec_size(this)         \
  (this ? __zvec_size(this) : (size_t)0)

#define zvec_resize(this, n)                            \
  do {                                                  \
    void* curr_end;                                     \
    size_t size = __zvec_size(this);                    \
    if ((n) > size) {                                   \
      __zvec_grow((zvec_h*)this, (n) * sizeof(this[0]));\
      curr_end = __zvec_end(begin) + size;              \
      memset(curr_end, 0, ztype_n_bytes(                \
        curr_end, __zvec_end(this)));                   \
    }                                                   \
  } while (0)

#define __zvec_alloc_end(this)  \
  (((zvec_h*)this)->alloc_end)
#define __zvec_capacity(this)                           \
  ((__zvec_alloc_type(this))__zvec_alloc_end(this)      \
    - (__zvec_alloc_type(this))__zvec_begin(this))
#define zvec_capacity(this)     \
  (this ? __zvec_capacity(this) : (size_t)0)

#define zvec_empty(this)        \
  (zvec_begin(this) == zvec_end(this))

#define zvec_reserve(this, n)                           \
  do {                                                  \
    if ((n) > zvec_capacity(this))                      \
      __zvec_grow((zvec_h*)this, (n) * sizeof(**this)); \
  } while (0)

#define zvec_shrink_to_fit(this)                        \
  do {                                                  \
    size_t bytes = ztype_n_bytes(                       \
      __zvec_begin(this), __zvec_end(this));            \
    __zvec_shrink((zvec_h*)this, bytes);                \
  } while (0)

/**
 * iterators
**/

#define __zvec_begin(this)      \
  (((zvec_h*)(this))->begin)
#define zvec_begin(this)        \
  (this ? (__zvec_alloc_type(this)) __zvec_begin(this) : NULL)

#define __zvec_end(this)        \
  (((zvec_h*)(this))->end)
#define zvec_end(this)          \
  (this ? (__zvec_alloc_type(this)) __zvec_end(this) : NULL)

/**
 * element access
**/

#define zvec_at(this, i)        \
  (*this)[i]

#define zvec_front(this)        \
  zvec_at(this, 0)

#define zvec_back(this)         \
  ((__zvec_alloc_type(this)) __zvec_end(this))[-1]

#define zvec_data(this)         \
  zvec_begin(this)

/**
 * modifiers
**/

#define zvec_clear(this)                        \
  do {                                          \
    size_t used = ztype_n_bytes(                \
      __zvec_begin(this), __zvec_end(this));    \
    __zvec_end(this) = __zvec_begin(this);      \
    __zvec_shrink((zvec_h*)this, bytes >> 2);   \
  } while (0)

#define zvec_insert(this, it, v)                \
  ({                                            \
    ztype_ok(it, __zvec_alloc_type(this));      \
    __zvec_alloc_type(this) p;                  \
    p = __zvec_emplace((zvec_h*)this,           \
      it, sizeof(*(it)));                       \
    if (p) *p = v;                              \
    p;                                          \
  })

#define zvec_insert_range(this, it, first, last)        \
  ({                                                    \
    ztype_ok(it, __zvec_alloc_type(this));              \
    ztype_ok(first, __zvec_alloc_type(this));           \
    ztype_ok(last, __zvec_alloc_type(this));            \
    __zvec_alloc_type(this) p;                          \
    size_t bytes = ztype_n_bytes(first, last);          \
    p = __zvec_emplace((zvec_h*)this, it, bytes);       \
    if (p) memcpy(p, first, bytes);                     \
    p;                                                  \
  })

#define zvec_emplace(this, it, ctor, ...)       \
  ({                                            \
    ztype_ok(it, __zvec_alloc_type(this));      \
    __zvec_alloc_type(this) p;                  \
    p = __zvec_emplace((zvec_h*)this,           \
      it, sizeof(*(it)));                       \
    if (ctor && p)                              \
      ctor(p, __VA_ARGS__);                     \
    p;                                          \
  })

#define zvec_erase(this, it)                    \
  ({                                            \
    void* next = NULL;                          \
    if ((it) < __zvec_end(this)) {              \
      void* new_end = --__zvec_end(this);       \
      next = (it) + 1;                          \
      memmove(it, next, new_end - next);        \
    }                                           \
    next;                                       \
  })

#define zvec_erase_range(this, first, last)             \
  ({                                                    \
    void* next = NULL;                                  \
    if ((first) < (last)                                \
        && (last) < __zvec_end(this)                    \
        && __zvec_begin(this) <= (first)) {             \
      __zvec_end(this) -= ztype_n_bytes(first, last);   \
      next = last;                                      \
      memmove(first, last, __zvec_end(this) - next);    \
    }                                                   \
    next;                                               \
  })

#define zvec_push_back(this, v)         \
  do {                                  \
    __zvec_alloc_type(this) it;         \
    it = __zvec_emplace((zvec_h*)this,  \
      __zvec_end(this), sizeof(*it));   \
    *it = v;                            \
  } while (0)

#define zvec_emplace_back(this, ctor, ...)      \
  zvec_emplace(this, __zvec_end(this),          \
    ctor, __VA_ARGS__)

#define zvec_pop_back(this)             \
  do {                                  \
    __zvec_end(this) -= sizeof(**this); \
  } while (0)

#define zvec_swap(this, other)                  \
  do {                                          \
    ztype_ok(other, typeof (this));             \
    __zvec_swap((zvec_h*)this, (zvec_h*)other); \
  } while (0)

zvec_h* __zvec_new(void);
void __zvec_free(zvec_h*);
void __zvec_grow(zvec_h*, size_t);
void __zvec_shrink(zvec_h*, size_t);
void* __zvec_emplace(zvec_h*, void*, size_t);
void __zvec_swap(zvec_h*, zvec_h*);

#endif
