#include "../zvec.h"

int tvec_ctor() {
  zvec_t(int) vec = zvec_new(int);
  zassert(vec != NULL, "vec should not be NULL");
  
  zvec_free(vec);
  return ZTEST_SUCCESS;
}

int tvec_dtor() {
  zvec_t(int) vec0 = zvec_new(int);
  zvec_t(int) vec1 = zvec_new(int);
  zvec_t(int) vec2 = NULL;

  zvec_push_back(vec0, 11);
  zvec_free(vec0);
  zassert(1, "non-empty");

  zvec_free(vec1);
  zassert(1, "unused");

  zvec_free(vec2);
  zassert(1, "NULL");

  return ZTEST_SUCCESS;
}
