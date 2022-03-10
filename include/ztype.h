#ifndef ZCC_TYPE_H
#define ZCC_TYPE_H

#include <assert.h>

#define ztor(p, ...)                    \
  do {                                  \
    *p = (typeof (*p)) { __VA_ARGS__ }; \
  } while (0)

#define N_ELEMS(pty, lb, ub) ((pty)(ub) - (pty)(lb))

#define N_BYTES(lb, ub) N_ELEMS(void*, lb, ub)

#define IS_TYPE(identifier, expected)                   \
  static_assert(__builtin_types_compatible_p(           \
    typeof (identifier), expected),                     \
    #identifier " is expected to be'" #expected "'")    \

#endif
