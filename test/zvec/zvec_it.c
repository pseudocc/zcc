#include "../zvec.h"

int tvec_begin() {
  int value;
  zvec_t(int) vec = zvec_new(int);

  zassert_eq(zvec_begin(vec), NULL, "new", "%p");

  zvec_push_back(vec, value = 11);
  zassert_eq(*zvec_begin(vec), value, "push", "%d");

  zvec_insert(vec, zvec_begin(vec), value = 31);
  zassert_eq(*zvec_begin(vec), value, "insert", "%d");

  zvec_free(vec);
  return ZTEST_SUCCESS;
}

int tvec_end() {
  int value, loops;
  zvec_t(int) vec = zvec_new(int);
  zvec_it(int) it;

  zassert_eq(zvec_end(vec), NULL, "new", "%p");

  zvec_push_back(vec, value = 11);
  zassert_eq(*(zvec_end(vec) - 1), value, "push", "%d");

  loops = zrand_int(1, 255);
  while (--loops)
    zvec_push_back(vec, 1);

  for (it = zvec_begin(vec); it < zvec_end(vec); ++it)
    loops++;
  zassert_eq((int)zvec_size(vec), loops, "loop", "%d");

  zvec_free(vec);
  return ZTEST_SUCCESS;
}
