// CppUnit testing template
// file: unittest.cc
// Author: Sarah Byrne
#include "unittest.h"

// Registering test suite so it can be fetched in runtest.cpp
CPPUNIT_TEST_SUITE_REGISTRATION (unittest);

// Implementation of the fixture-specific methods
/*void unittest :: setUp (void)
{
    int a=1;
}

void unittest :: tearDown (void)
{
    delete a;
}*/

void unittest :: test1 (void)
{
    CPPUNIT_ASSERT_EQUAL(0, 10-10); // (expected, actual)
    CPPUNIT_ASSERT(3 < 7);
}

void unittest :: test2 (void)
{
    CPPUNIT_ASSERT_MESSAGE("This message will never print as 5 is less than 8", 5 < 8);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("These values are not equal!", 3, 10-10); //This test would fail
    CPPUNIT_ASSERT(1 < 3); //This will never be checked as the previous assert will fail
}