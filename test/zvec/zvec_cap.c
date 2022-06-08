#include "../zvec.h"

int tvec_empty() {
  zvec_t(int) vec = zvec_new(int);
  zassert(zvec_empty(vec), "new");

  zvec_free(vec);
  return ZTEST_SUCCESS;
}

int tvec_size() {
  const int loops = 7;
  zvec_t(int) vec = zvec_new(int);
  int count, i, j;

  count = i = j = 0;
  for (; i < loops; i++) {
    count += zrand_int(1, 5);
    for (; j < count; j++)
      zvec_push_back(vec, 11);
    zassert_eq((int)zvec_size(vec), count, "loop %d", "%d", i);
  }

  zvec_free(vec);
  return ZTEST_SUCCESS;
}

int tvec_resize() {
  int size;
  zvec_t(int) vec = zvec_new(int);
  
  zvec_resize(vec, size = 10);
  zassert_eq((int)zvec_size(vec), size, "empty increase", "%d");
  
  zvec_resize(vec, size = 7);
  zassert_eq((int)zvec_size(vec), size, "decrease", "%d");

  zvec_free(vec);
  return ZTEST_SUCCESS;
}

int tvec_reserve() {
  int cap;
  zvec_t(int) vec = zvec_new(int);

  cap = 11;
  zvec_reserve(vec, 11);
  zassert(zvec_capacity(vec) >= cap, "reserve empty");
  cap = zvec_capacity(vec);

  zvec_reserve(vec, 7);
  zassert_eq((int)zvec_capacity(vec), cap,
    "smaller than capacity", "%d");

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

int tvec_capacity() {
  zvec_t(int) vec = zvec_new(int);
  zvec_h* zh = (zvec_h*)vec;
  int cap;

  zassert_eq((int)zvec_capacity(vec), 0, "brand-new", "%d");

  zvec_push_back(vec, 11);
  cap = (zh->alloc_end - zh->begin) / sizeof(int);
  zassert_eq((int)zvec_capacity(vec), cap, "1 item", "%d");

  zvec_free(vec);
  return ZTEST_SUCCESS;
}

int tvec_shrink_to_fit() {
  zvec_t(int) vec = zvec_new(int);
  int items, i;

  zvec_shrink_to_fit(vec);
  zassert(1, "empty");

  items = zrand_int(1, 255);
  for (i = 0; i < items; i++)
    zvec_push_back(vec, 7);

  zvec_shrink_to_fit(vec);
  zassert_eq((int)zvec_capacity(vec),
    (int)zvec_size(vec), "vec[%d]", "%d", items);

  zvec_free(vec);
  return ZTEST_SUCCESS;
}
