#include "../zvec.h"

static int empty() {
  zvec_t(int) vec = zvec_new(int);
  zassert(zvec_empty(vec), "newly created vec should be empty");

  zvec_free(vec);
  return ZTEST_SUCCESS;
}

static int size() {
  const int loops = 7;
  zvec_t(int) vec = zvec_new(int);
  int count, i, j;

  count = i = j = 0;
  for (; i < loops; i++) {
    count += zrand_int(1, 5);
    for (; j < count; j++)
      zvec_push_back(vec, 11);
    zassert_eq(zvec_size(vec), count,
      "loop %d: random seed %d", "%d", i, zseed);
  }

  zvec_free(vec);
  return ZTEST_SUCCESS;
}

static int reserve() {
  size_t cap;
  zvec_t(int) vec = zvec_new(int);

  cap = 11;
  zvec_reserve(vec, 11);
  zassert(zvec_capacity(vec) >= cap, "reserve empty");
  cap = zvec_capacity(vec);

  zvec_reserve(vec, 7);
  zassert_eq(zvec_capacity(vec), cap,
    "smaller than capacity", "%ld");

  cap += 2;
  zvec_reserve(vec, cap);
  zassert(zvec_capacity(vec) >= cap, "reserve twice");

  zvec_free(vec);

  vec = zvec_new(int);
  zvec_push_back(vec, 9);
  zvec_reserve(vec, cap);
  zassert(zvec_capacity(vec) >= cap, "reserve non-empty");

  zvec_free(vec);
  return ZTEST_SUCCESS;
}

static int capacity() {
  zvec_t(int) vec = zvec_new(int);
  zvec_h* zh = (zvec_h*)vec;

  zassert_eq(zvec_capacity(vec), 0, "brand-new");

  zvec_push_back(vec, 11);
  zassert_eq(zvec_capacity(vec),
    (zh->alloc_end - zh->begin) / sizeof(int), "1 item");

  zvec_free(vec);
  return ZTEST_SUCCESS;
}

static int shrink_to_fit() {
  zvec_t(int) vec = zvec_new(int);
  int items, i;

  zassert(1, "new");
  zvec_shrink_to_fit(vec);
  zassert(1, "empty");

  items = zrand_int(1, 255);
  for (i = 0; i < items; i++)
    zvec_push_back(vec, 7);

  zvec_shrink_to_fit(vec);
  zassert_eq(zvec_capacity(zvec), zvec_size(zvec),
    "vec[%d] - random seed %d", "%d", items, zseed);

  zvec_free(vec);
  return ZTEST_SUCCESS;
}
