// CppUnit testing for XrdCephWrapper class
// file: wrappertest.h
// Author: Sarah Byrne
#ifndef WRAPPERTEST_H
#define WRAPPERTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "XrdCephPosixWrapper.hh"
#include <sstream>

using namespace std;

class wrappertest : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (wrappertest);
    CPPUNIT_TEST (openTest);
    CPPUNIT_TEST (statTest);
    CPPUNIT_TEST (readTest);
    CPPUNIT_TEST_SUITE_END ();

    public:
        void setUp (void);
		void tearDown (void);

    protected:
        void openTest (void);
        void statTest (void);
        void readTest (void);

    private:
        XrdCephPosixWrapper *w;

};

#endif