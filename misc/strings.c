#include "munit.h"
#include <stdbool.h>
#include <stdlib.h>
#include <strings.h> // SUT

// *** a single Unit Test (UT)
// UT = simple, small, fast, if it fails the reason why is VERY obvious
// want miniumum amount of assertions as possible
static MunitResult strlen_of_demitrus_is_8(const MunitParameter params[], void *user_data) {
    (void)params; // set to void so compiler won't throw 'unused parameter' signal
    (void)user_data; // set to void so compiler won't throw 'unused parameter' signal
    // arrange
    const char name[] = "demitrus";
    // act
    size_t length = strlen(name);
    // assert
    munit_assert_size(length, ==, 8); // using munit assertion macros(?)

    return MUNIT_OK;
}

static MunitResult strlen_of_david_is_5(const MunitParameter params[],
                                        void *user_data) {
  (void)params;
  (void)user_data;
  // arrange
  const char name[] = "david";
  // act
  size_t length = strlen(name);
  // assert
  munit_assert_size(length, ==, 5);

  return MUNIT_OK;
}

static MunitResult demitrus_is_greater_than_david(const MunitParameter params[],
                                                  void *user_data) {
  (void)params;
  (void)user_data;
  // arrange
  const char demitrus[] = "demitrus";
  const char david[] = "david";
  // act
  bool b = strcmp(david, demitrus);
  // assert
  munit_assert_true(b);

  return MUNIT_OK;
}


// test_suites is an array
// array of UTs
// name, UT(), setup(), tear_down(), OPTIONS, PARAMS

// *** array of UTs aka an array of structure
//.clang-format off
static MunitTest test_suite_tests[] = {
    // why cast as a char* exactly?
    // what is munit test option none exactly?
    {(char *)"strlen_of_demitrus_is_8", strlen_of_demitrus_is_8, NULL, NULL,
     MUNIT_TEST_OPTION_NONE, NULL},
    {(char *)"strlen_of_david_is_5", strlen_of_david_is_5, NULL, NULL,
     MUNIT_TEST_OPTION_NONE, NULL},
    {(char *)"demitrus_is_greater_than_david", demitrus_is_greater_than_david,
     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {NULL}};
//.clang-format on

// *** SUITE - folder/directory - test-array & optionally sub-suits
// this is not an array
static const MunitSuite test_suite = {(char *)"/string compare tests/", test_suite_tests, NULL, 1,
                                      MUNIT_SUITE_OPTION_NONE};
// *** main
int main(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
  return munit_suite_main(&test_suite, (void *)NULL, argc, argv);
}