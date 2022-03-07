#include "../zvec.h"

int at() {
  zvec_t(int) vec = zvec_new(int);
  int value;

  zvec_push_back(vec, value = 19);
  zassert_eq(zvec_at(vec, 0), value, "vec[0]", "%d");

  zvec_push_back(vec, value = 23);
  zassert_eq(zvec_at(vec, 1), value, "vec[1]", "%d");

  zvec_free(vec);
  return ZTEST_SUCCESS;
}

int front() {
  zvec_t(int) vec = zvec_new(int);
  int value;

  zvec_insert(vec, zvec_begin(vec), value = 2);
  zassert_eq(zvec_front(vec), value, "first", "%d");

  zvec_insert(vec, zvec_begin(vec), value = 3);
  zassert_eq(zvec_front(vec), value, "second", "%d");

  zvec_front(vec) = value = 17;
  zassert_eq(zvec_front(vec), value, "set", "%d");

  zvec_free(vec);
  return ZTEST_SUCCESS;
}

int back() {
  zvec_t(int) vec = zvec_new(int);
  int value;

  zvec_push_back(vec, value = 5);
  zassert_eq(zvec_back(vec), value, "first", "%d");

  zvec_push_back(vec, value = 7);
  zassert_eq(zvec_back(vec), value, "second", "%d");

  zvec_back(vec) = value = 13;
  zassert_eq(zvec_back(vec), value, "set", "%d");

  zvec_free(vec);
  return ZTEST_SUCCESS;
}

int data() {
  zvec_t(int) vec = zvec_new(int);

  zassert_eq(zvec_data(vec), NULL, "new", "%p");

  zvec_push_back(vec, 11);
  zassert(zvec_data(vec) != NULL, "one item");

  free(zvec_data(vec));
  return ZTEST_SUCCESS;
}
