#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <setjmp.h>
#include <signal.h>
#include "./ztest.h"

typedef long long ms_t;

static ms_t diff_ms(struct timeval tv1, struct timeval tv2) {
  ms_t sec, usec;
  sec = tv2.tv_sec - tv1.tv_sec;
  usec = tv2.tv_usec - tv1.tv_usec;
  return sec * 1000 + usec / 1000;
}

char zerrbuf[ERR_BUF_SIZE];
char* last_assert;

jmp_buf point;
static void segfault_handler(int sig, siginfo_t* si, void* vp) {
  longjmp(point, 1);
}

static int run_case(ztest_case* p) {
  if (!setjmp(point))
    return p->entry();
  // may handle other core dumps in the future
  if (last_assert)
    zerror("%s\tsegfault after", last_assert);
  else
    zerror("segfault before the 1st assertion");
  return ZTEST_FAILURE;
}

static int run_ut(ztest_unit unit) {
  int success = 0, ztest_state;
  struct timeval tv0, tv1, tv2;
  ztest_case zc;

  printf("Running tests for %s\n", unit.name);
  gettimeofday(&tv0, NULL);

  for (int i = 0; i < unit.n_cases; i++) {
    zc = unit.cases[i];
    memset(zerrbuf, 0, ERR_BUF_SIZE);
    last_assert = NULL;
    gettimeofday(&tv1, NULL);

    switch (run_case(&zc)) {
    case ZTEST_SUCCESS:
      printf("\t" GREEN "SUCCESS" RESET);
      success++;
      break;
    case ZTEST_PARTIAL:
      printf("\t" YELLOW "PARTIAL" RESET);
      success++;
      break;
    case ZTEST_FAILURE:
      printf("\t" RED "FAILURE" RESET);
      break;
    }

    gettimeofday(&tv2, NULL);
    printf(" - %s - %s", unit.name, zc.name);
    printf(" - [%lld ms]\n", diff_ms(tv1, tv2));
    if (*zerrbuf)
      printf("%s", zerrbuf);
  }

  printf("\t%d/%d of the tests are passed.\n",
    success, unit.n_cases);
  printf("Total time elasped %lld ms.\n", diff_ms(tv0, tv2));

  return unit.n_cases - success;
}

int feels_good_man() {
  return ZTEST_SUCCESS;
}

unsigned int zseed;
int zrand_int(int min, int max) {
  int v = rand();
  int l = max - min;
  return v % l + min;
}

int main(int argc, const char* argv[]) {
  // all test categories to be included in the unit tests
  ztest_unit cats[] = { zvec_tests };
  struct sigaction sact;
  int failed, i;

  memset(&sact, 0, sizeof(struct sigaction));
  sigemptyset(&sact.sa_mask);
  sact.sa_flags = SA_NODEFER;
  sact.sa_sigaction = &segfault_handler;

  sigaction(SIGSEGV, &sact, NULL);

  zseed = argc > 1
    ? (unsigned int)atol(argv[1])
    : (unsigned int)time(NULL);
  srand(zseed);
  printf("random seed: %u\n", zseed);

  for (failed = 0, i = 0; i < ARRAY_SIZE(cats); i++)
    failed += run_ut(cats[i]);
  return failed != 0; 
}

