#include "./zvec.h"

static ztest_case cases[] = {
  { "BE HAPPY EVERYDAY", &feels_good_man },
  { "CONSTRUCTOR", &ctor },
  { "DESTRUCTOR", &dtor },
  { "EMPTY", &empty },
  { "SIZE", &size },
  { "RESERVE", &reserve },
  { "CAPACITY", &capacity },
  { "SHRINK TO FIT", &shrink_to_fit },
  { "BEGIN ITERATOR", &begin },
  { "END ITERATOR", &end },
  { "ELEMENT AT", &at },
  { "FIRST ELEMENT", &front },
  { "LAST ELEMENT", &back },
  { "DATA", &data },
  { "CLEAR", &clear },
  { "INSERT", &insert },
  { "EMPLACE", &emplace },
  { "ERASE", &erase },
  { "PUSH BACK", &push_back },
  { "POP BACK", &pop_back },
  { "SWAP", &swap }
};

ztest_unit zvec_tests = DECL_UT(cases, ZVEC_UTNAME);
