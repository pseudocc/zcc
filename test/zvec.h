#ifndef ZCC_VECTOR_UT_H
#define ZCC_VECTOR_UT_H

#include "./ztest.h"
#include "include/zvec.h"

#define ZVEC_UTNAME "VECTOR"

/**
 * constructor and destructor
**/

int ctor();
int dtor();

/**
 * capacity
**/

int empty();
int size();
int reserve();
int capacity();
int shrink_to_fit();

/**
 * iterators
**/

int begin();
int end();

/**
 * element access
**/

int at();
int front();
int back();
int data();

/**
 * modifiers
**/

int clear();
int insert();
int emplace();
int erase();
int push_back();
int pop_back();
int swap();

/**
 * declare UT
**/

ztes_case cases[] = {
  { "BE HAPPY EVERYDAY", &feels_good_man }
};

ztest_unit = DECL_UT(cases, ZVEC_UTNAME);

#endif
