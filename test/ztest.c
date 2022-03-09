#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <setjmp.h>
#include <signal.h>
#include "./ztest.h"

typedef struct timeval timev;

static char* diff_time(timev t1, timev t2) {
  static char buf[16];
  timev t0 = {
    .tv_sec = t2.tv_sec - t1.tv_sec,
    .tv_usec = t2.tv_usec - t1.tv_usec
  };

  if (t0.tv_sec) {
    if (t0.tv_sec < 10)
      sprintf(buf, "%ld.%ld s",
        t0.tv_sec, t0.tv_usec / 10000);
    else if (t0.tv_sec < 60)
      sprintf(buf, "%ld s", t0.tv_sec);
    else if (t0.tv_sec < 60 * 60) {
      if (t0.tv_usec < 60 * 2)
        sprintf(buf, "1 min");
      else
        sprintf(buf, "%ld mins", t0.tv_sec / 60);
    }
    else
      sprintf(buf, ">1 hrs");
  }
  else {
    if (t0.tv_usec > 1000)
      sprintf(buf, "%ld ms", t0.tv_usec / 1000);
    else if (t0.tv_usec)
      sprintf(buf, "%ld us", t0.tv_usec);
    else
      sprintf(buf, "<1 us");
  }

  return buf;
}

char zerrbuf[ERR_BUF_SIZE];
char zastbuf[ERR_BUF_SIZE];
int catched_signal;

jmp_buf point;
static void sig_handler(int sig, siginfo_t* si, void* vp) {
  catched_signal = sig;
  longjmp(point, 1);
}

static int run_case(ztest_case* p) {
  char* sig_str;
  char* assert_msg;
  if (!setjmp(point))
    return p->entry();
  // may handle other core dumps in the future
  switch (catched_signal) {
  case SIGABRT:
    sig_str = "SIGABRT";
    break;
  case SIGSEGV:
    sig_str = "SIGSEGV";
    break;
  }
  assert_msg = zastbuf;
  if (strlen(zastbuf))
    zerror("[%s] last successful assert:\n"
      ERR_INDENT "%s", sig_str, assert_msg);
  else
    zerror("[%s]\tcore dumps "
      "before the 1st assertion", sig_str);
  return ZTEST_FAILURE;
}

static int run_ut(ztest_unit unit) {
  int success = 0, ztest_state;
  timev tv0, tv1, tv2;
  ztest_case zc;

  printf("Running tests for %s\n", unit.name);
  gettimeofday(&tv0, NULL);

  for (int i = 0; i < unit.n_cases; i++) {
    zc = unit.cases[i];
    memset(zerrbuf, 0, ERR_BUF_SIZE);
    memset(zastbuf, 0, ERR_BUF_SIZE);
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
    printf(" - [%s]\n", diff_time(tv1, tv2));
    if (*zerrbuf)
      printf("%s", zerrbuf);
  }

  printf("\t%d/%d of the tests are passed.\n",
    success, unit.n_cases);
  printf("Total time elasped %s.\n", diff_time(tv0, tv2));

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
  sact.sa_sigaction = &sig_handler;

  sigaction(SIGSEGV, &sact, NULL);
  sigaction(SIGABRT, &sact, NULL);

  zseed = argc > 1
    ? (unsigned int)atol(argv[1])
    : (unsigned int)time(NULL);
  srand(zseed);
  printf("random seed: %u\n", zseed);

  for (failed = 0, i = 0; i < ARRAY_SIZE(cats); i++)
    failed += run_ut(cats[i]);
  return failed != 0; 
}

