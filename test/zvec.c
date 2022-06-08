#include "./zvec.h"

static ztest_case cases[] = {
  { "BE HAPPY EVERYDAY", &feels_good_man },
  { "CONSTRUCTOR", &tvec_ctor },
  { "DESTRUCTOR", &tvec_dtor },
  { "EMPTY", &tvec_empty },
  { "SIZE", &tvec_size },
  { "RESIZE", &tvec_resize },
  { "RESERVE", &tvec_reserve },
  { "CAPACITY", &tvec_capacity },
  { "SHRINK TO FIT", &tvec_shrink_to_fit },
  { "BEGIN ITERATOR", &tvec_begin },
  { "END ITERATOR", &tvec_end },
  { "AT ACCESS", &tvec_at },
  { "FRONT ACCESS", &tvec_front },
  { "BACK ACCESS", &tvec_back },
  { "DATA ACCESS", &tvec_data },
  { "CLEAR", &tvec_clear },
  { "INSERT", &tvec_insert },
  { "EMPLACE", &tvec_emplace },
  { "ERASE", &tvec_erase },
  { "PUSH BACK", &tvec_push_back },
  { "POP BACK", &tvec_pop_back },
  { "SWAP", &tvec_swap }
};

ztest_unit zvec_tests = DECL_UT(cases, ZVEC_UTNAME);
