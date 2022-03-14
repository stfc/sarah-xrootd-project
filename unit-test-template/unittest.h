// CppUnit testing template
// file: unittest.h
// Author: Sarah Byrne
#ifndef UNITTEST_H
#define UNITTEST_H

//  include any files needed for test methods

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <sstream>  // included for use in an outputter

using namespace std;

class unittest : public CPPUNIT_NS :: TestFixture  // Or TestCase - test fixture inherits from test case so it's okay to leave as fixture
{
    CPPUNIT_TEST_SUITE (unittest);
    CPPUNIT_TEST (test1);
    CPPUNIT_TEST (test2);
    CPPUNIT_TEST_SUITE_END ();

    // include this is you are creating a test fixture
    /*public:
        void setUp (void);
		void tearDown (void);*/

    protected:
        void test1 (void);
        void test2 (void);

    // Declare any variables used for test fixture
    /*private:
        int *a;*/

};

#endif