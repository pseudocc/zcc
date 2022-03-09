#include "../zvec.h"

int clear() {
  zvec_t(int) vec = zvec_new(int);
  int items;

  zvec_clear(vec);
  zassert(1, "new");

  items = zrand_int(1, 255);
  while (--items)
    zvec_push_back(vec, items);
  zvec_clear(vec);
  zassert_eq((int)zvec_size(vec), 0, "all", "%d");

  zvec_free(vec);
  return ZTEST_SUCCESS;
}

int insert() {
  zvec_t(int) vec = zvec_new(int);
  int nums[] = { 7, 2, 9 }, i;

  zvec_insert(vec, zvec_begin(vec), nums[2]);
  zvec_insert(vec, zvec_begin(vec), nums[0]);
  zvec_insert(vec, &zvec_at(vec, 1), nums[1]);
  zvec_insert_range(vec, zvec_begin(vec),
    (int*)nums, nums + ARRAY_SIZE(nums));

  for (i = 0; i < zvec_size(vec); ++i)
    zassert_eq(zvec_at(vec, i), nums[i % ARRAY_SIZE(nums)],
      "vec[%d]", "%d", i);

  zvec_free(vec);
  return ZTEST_SUCCESS;
}

typedef struct {
  int hour;
  int minute;
  int second;
} day_time;

static void dt_ctor(day_time* p, int h, int m, int s) {
  p->hour = h;
  p->minute = m;
  p->second = s;
}

int emplace() {
  const day_time now = { 14, 18, 7 };
  zvec_t(day_time) vec = zvec_new(day_time);
  zvec_it(day_time) it;

  zvec_emplace_back(vec, ztor,
    now.hour, now.minute, now.second);
  zvec_emplace(vec, zvec_begin(vec) + 1, 
    dt_ctor, now.hour, now.minute, now.second);
  
  it = zvec_begin(vec);
  for (it = zvec_begin(vec); it < zvec_end(vec); ++it) {
    zassert_eq(it->hour, now.hour, "hour", "%d");
    zassert_eq(it->minute, now.minute, "minute", "%d");
    zassert_eq(it->second, now.second, "second", "%d");
  }

  zvec_free(vec);
  return ZTEST_SUCCESS;
}

int erase() {
  zvec_t(int) vec = zvec_new(int);
  int nums[] = { 2, 3, 5, 7, 11, 13, 17 }, i, n;

  n = ARRAY_SIZE(nums);
  zvec_insert_range(vec, zvec_begin(vec),
    (int*)nums, nums + ARRAY_SIZE(nums));

  zvec_erase(vec, zvec_begin(vec));
  n--;
  zassert_eq((int)zvec_size(vec), n, "begin size", "%d");
  for (i = 0; i < n; ++i)
    zassert_eq(zvec_at(vec, i),
      nums[i + 1], "begin vec[%d]", "%d", i);
  
  zvec_erase(vec, zvec_end(vec) - 1);
  n--;
  zassert_eq((int)zvec_size(vec), n, "end size", "%d");
  for (i = 0; i < ARRAY_SIZE(nums) - 2; ++i)
    zassert_eq(zvec_at(vec, i),
      nums[i + 1], "end vec[%d]", "%d", i);
  
  zvec_erase_range(vec, zvec_begin(vec) + 1,
    zvec_begin(vec) + 3);
  n -= 2;
  zassert_eq((int)zvec_size(vec), n, "end size", "%d");
  zassert_eq(zvec_front(vec), nums[1],
    "middle range vec[0]", "%d");
  for (i = 1; i < n; ++i)
    zassert_eq(zvec_at(vec, i), nums[i + 3],
      "middle range vec[%d]", "%d", i);

  zvec_free(vec);
  return ZTEST_SUCCESS;
}

static void random_integers(int* p, int n) {
  while (n--)
    *p++ = zrand_int(0, 0x7fffffff);
}

int push_back() {
  zvec_t(int) vec = zvec_new(int);
  int nums[256] = { 0 }, i, j;
  random_integers(nums, ARRAY_SIZE(nums));

  for (i = 0; i < ARRAY_SIZE(nums); ++i) {
    zassert_eq((int)zvec_size(vec), i, "size", "%d");
    zvec_push_back(vec, nums[i]);
    for (j = 0; j <= i; ++j) 
      zassert_eq(zvec_at(vec, j), nums[j],
        "loop %d: vec[%d]", "%d", i, j);
  }

  zvec_free(vec);
  return ZTEST_SUCCESS;
}

int pop_back() {
  zvec_t(int) vec = zvec_new(int);
  int nums[256] = { 0 }, i;
  random_integers(nums, ARRAY_SIZE(nums));

  zvec_insert_range(vec, zvec_begin(vec),
    (int*)nums, nums + ARRAY_SIZE(nums));
  i = ARRAY_SIZE(nums);

  while (i--) {
    zassert_eq((int)zvec_size(vec), i + 1, "size", "%d");
    zassert_eq(zvec_back(vec), nums[i],
      "vec[%d]", "%d", i);
    zvec_pop_back(vec);
  }

  zvec_free(vec);
  return ZTEST_SUCCESS;
}

int swap() {
  const int a = 10, b = 11, c = 77;
  zvec_t(int) vec1 = zvec_new(int);
  zvec_t(int) vec2 = zvec_new(int);

  zvec_push_back(vec1, a);
  zvec_push_back(vec1, b);
  zvec_push_back(vec2, c);
  zvec_swap(vec1, vec2);

  zassert_eq((int)zvec_size(vec1), 1, "vec1 size", "%d");
  zassert_eq((int)zvec_size(vec2), 2, "vec2 size", "%d");
  zassert_eq((int)zvec_front(vec1), c, "vec1[0]", "%d");
  zassert_eq((int)zvec_front(vec2), a, "vec2[0]", "%d");
  zassert_eq((int)zvec_back(vec2), b, "vec2[1]", "%d");

  zvec_free(vec1);
  zvec_free(vec2);
  return ZTEST_SUCCESS;
}