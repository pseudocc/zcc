#ifndef ZCC_VECTOR_UT_H
#define ZCC_VECTOR_UT_H

#include "./ztest.h"
#include "include/zvec.h"

#define ZVEC_UTNAME "VECTOR"

/**
 * constructor and destructor
**/

int tvec_ctor();
int tvec_dtor();

/**
 * tvec_capacity
**/

int tvec_empty();
int tvec_size();
int tvec_resize();
int tvec_reserve();
int tvec_capacity();
int tvec_shrink_to_fit();

/**
 * iterators
**/

int tvec_begin();
int tvec_end();

/**
 * element access
**/

int tvec_at();
int tvec_front();
int tvec_back();
int tvec_data();

/**
 * modifiers
**/

int tvec_clear();
int tvec_insert();
int tvec_emplace();
int tvec_erase();
int tvec_push_back();
int tvec_pop_back();
int tvec_swap();

/**
 * define UT entry
**/

extern ztest_unit zvec_tests;

#endif
