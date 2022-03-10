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

/**
 * internal aliases
**/

#define __atb3x(v) typeof(*v)
#define __dtpI5(v) typeof(**v)
#define __lbqF6(v) (((zvec_h*)(v))->begin)
#define __dphPM(v) (((zvec_h*)(v))->end)
#define __ubbPJ(v) (((zvec_h*)(v))->alloc_end)
#define __szd77(v) N_ELEMS(__atb3x(v), __lbqF6(v), __dphPM(v))
#define __cpqnp(v) N_ELEMS(__atb3x(v), __lbqF6(v), __ubbPJ(v))
#define __essiA(v) sizeof(**v)

/**
 * generic types
**/

#define zvec_t(ty) ty**
#define zvec_it(ty) ty*

/**
 * constructor and destructor
**/

#define zvec_new(ty) ((zvec_t(ty)) __zvec_new())

#define zvec_free(v) __zvec_free((zvec_h*)(v))

/**
 * iterators
**/

#define zvec_begin(v) (v ? (__atb3x(v)) __lbqF6(v) : NULL)

#define zvec_end(v) (v ? (__atb3x(v)) __dphPM(v) : NULL)

/**
 * capacity
**/

#define zvec_size(v) (v ? __szd77(v) : (size_t)0)

#define zvec_resize(v, n)               \
  do {                                  \
    size_t bytes = (n) * __essiA(v);    \
    __zvec_resize((zvec_h*)(v), bytes); \
  } while (0)

#define zvec_capacity(v) (v ? __cpqnp(v) : (size_t)0)

#define zvec_empty(v) (zvec_begin(v) == zvec_end(v))

#define zvec_reserve(v, n)                              \
  do {                                                  \
    size_t bytes = (n) * __essiA(v);                    \
    size_t allocated = N_BYTES(__lbqF6(v), __ubbPJ(v)); \
    if (bytes > allocated)                              \
      __zvec_grow((zvec_h*)(v), bytes);                 \
  } while (0)

#define zvec_shrink_to_fit(v)                           \
  do {                                                  \
    size_t used = N_BYTES(__lbqF6(v), __dphPM(v));      \
    __zvec_shrink((zvec_h*)(v), used);                  \
  } while (0)

/**
 * ele(zvec_h*)(v)nt access
**/

#define zvec_at(v, i)   \
  (*v)[i]

#define zvec_front(v)   \
  zvec_at(v, 0)

#define zvec_back(v)    \
  ((__atb3x(v)) __dphPM(v))[-1]

#define zvec_data(v)    \
  zvec_begin(v)

/**
 * modifiers
**/

#define zvec_clear(v)                           \
  do {                                          \
    size_t used = N_BYTES(                      \
      __lbqF6(v), __dphPM(v));                  \
    __dphPM(v) = __lbqF6(v);                    \
    __zvec_shrink((zvec_h*)(v), used >> 2);     \
  } while (0)

#define zvec_insert(v, it, d)                           \
  ({                                                    \
    __auto_type _it = it;                               \
    IS_TYPE(_it, __atb3x(v));                           \
    __atb3x(v) p;                                       \
    p = __zvec_emplace((zvec_h*)(v), _it, __essiA(v));  \
    if (p) *p = d;                                      \
    p;                                                  \
  })

#define zvec_insert_range(v, it, lb, ub)        \
  ({                                            \
    __auto_type _it = it;                       \
    __auto_type _lb = lb;                       \
    __auto_type _ub = ub;                       \
    IS_TYPE(_it, __atb3x(v));                   \
    IS_TYPE(_lb, __atb3x(v));                   \
    IS_TYPE(_ub, __atb3x(v));                   \
    __atb3x(v) p;                               \
    size_t bytes = N_BYTES(_lb, _ub);           \
    p = __zvec_emplace((zvec_h*)(v), it, bytes);\
    if (p) memcpy(p, lb, bytes);                \
    p;                                          \
  })

#define zvec_emplace(v, it, ctor, ...)  \
  ({                                    \
    __auto_type _it = it;               \
    IS_TYPE(_it, __atb3x(v));           \
    __atb3x(v) p;                       \
    p = __zvec_emplace((zvec_h*)(v),    \
      _it, __essiA(v));                 \
    if (p) ctor(p, __VA_ARGS__);        \
    p;                                  \
  })

#define zvec_erase(v, it)                               \
  ({                                                    \
    __auto_type _it = it;                               \
    IS_TYPE(_it, __atb3x(v));                           \
    void* next = NULL;                                  \
    void* vit = _it;                                    \
    if (vit != NULL && vit < __dphPM(v)                 \
        && vit >= __lbqF6(v)) {                         \
      next = vit + __essiA(v);                          \
      memmove(vit, next, N_BYTES(next, __dphPM(v)));    \
      __dphPM(v) -= __essiA(v);                         \
    }                                                   \
    next;                                               \
  })

#define zvec_erase_range(v, lb, ub)                     \
  ({                                                    \
    __auto_type _lb = lb;                               \
    __auto_type _ub = ub;                               \
    IS_TYPE(_lb, __atb3x(v));                           \
    IS_TYPE(_ub, __atb3x(v));                           \
    void* next = NULL;                                  \
    if (_lb < _ub                                       \
        && (void*)(_ub) < __dphPM(v)                    \
        && __lbqF6(v) <= (void*)(_lb)) {                \
      next = _ub;                                       \
      memmove(_lb, _ub, N_BYTES(next, __dphPM(v)));     \
      __dphPM(v) -= N_BYTES(lb, ub);                    \
    }                                                   \
    next;                                               \
  })

#define zvec_push_back(v, d)            \
  do {                                  \
    __atb3x(v) it;                      \
    it = __zvec_emplace((zvec_h*)(v),   \
      __dphPM(v), __essiA(v));          \
    *it = d;                            \
  } while (0)

#define zvec_emplace_back(v, ctor, ...) \
  zvec_emplace(v, (__atb3x(v))__dphPM(v), ctor, __VA_ARGS__)

#define zvec_pop_back(v)        \
  do {                          \
    __dphPM(v) -= __essiA(v);   \
  } while (0)

#define zvec_swap(v1, v2)                       \
  do {                                          \
    IS_TYPE(v2, typeof(v1));                    \
    __zvec_swap((zvec_h*)(v1), (zvec_h*)(v2));  \
  } while (0)

zvec_h* __zvec_new(void);
void __zvec_free(zvec_h*);
void __zvec_grow(zvec_h*, size_t);
void __zvec_shrink(zvec_h*, size_t);
void __zvec_resize(zvec_h*, size_t);
void* __zvec_emplace(zvec_h*, void*, size_t);
void __zvec_swap(zvec_h*, zvec_h*);

#endif
