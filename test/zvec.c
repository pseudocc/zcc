#include "./zvec.h"

static ztest_case cases[] = {
  { "BE HAPPY EVERYDAY", &feels_good_man },
  { "CONSTRUCTOR", &ctor },
  { "DESTRUCTOR", &dtor },
  { "EMPTY", &empty },
  { "SIZE", &size },
  { "RESERVE", &reserve },
  { "CAPACITY", &capacity },
  { "SHRINK TO FIT", &shrink_to_fit }
};

ztest_unit zvec_tests = DECL_UT(cases, ZVEC_UTNAME);
