#ifndef Z_UNIT_TEST_H
#define Z_UNIT_TEST_H

#include <stdio.h>
#include <stdlib.h>

enum {
  ZTEST_SUCCESS,
  ZTEST_PARTIAL,
  ZTEST_FAILURE
};

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"

#define STRINGIZE_DETAIL(x) #x
#define STRINGIZE(x) STRINGIZE_DETAIL(x)

#define ERR_INDENT "\t\t"
#define zerror(fmt, ...) \
  sprintf(zerrbuf, ERR_INDENT fmt "\n" __VA_OPT__(,) __VA_ARGS__)
#define zthrow(fmt, ...) \
  zerror(BLUE __FILE__ ":" STRINGIZE(__LINE__) RESET "\t" \
    fmt __VA_OPT__(,) __VA_ARGS__); \
  return ZTEST_FAILURE
#define zassert(cond, fmt, ...) \
  if (!(cond)) { zthrow(fmt __VA_OPT__(,) __VA_ARGS__); } \
  else last_assert = BLUE __FILE__ ":" STRINGIZE(__LINE__) RESET;
#define zassert_eq(actual, expected, desc, fmt, ...) \
  zassert(actual == expected, desc ": expected " fmt " but got " \
  fmt " instead" __VA_OPT__(,) __VA_ARGS__, expected, actual)

typedef int (*ztest_method)(void);
typedef struct {
  char name[64];
  ztest_method entry;
} ztest_case;

typedef struct {
  char name[64];
  ztest_case* cases;
  unsigned n_cases;
} ztest_unit;

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) sizeof(arr)/sizeof(arr[0])
#endif

#define DECL_UT(cases, name) { name, cases, ARRAY_SIZE(cases) }

#define ERR_BUF_SIZE 1024
extern char zerrbuf[ERR_BUF_SIZE];
extern char* last_assert;

extern ztest_unit zvec_tests;

int feels_good_man();

#endif
