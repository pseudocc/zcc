#ifndef ZCC_TYPE_H
#define ZCC_TYPE_H

#include <assert.h>

#define ztor(p, ...)                    \
  do {                                  \
    *p = (typeof (*p)) { __VA_ARGS__ }; \
  } while (0)

#define ztype_n_bytes(start, end)       \
  ((void*)(end) - (void*)(start))

#define ztype_ok(actual, expected)              \
  static_assert(__builtin_types_compatible_p(   \
  typeof (actual), expected),                   \
  "expected '" #expected "' "                   \
  "but got '" #actual "' instead.")

#endif
