// CppUnit testing for XrdCephPosix library
// file: wrappertest.hh
// Author: Sarah Byrne
#ifndef CEPHPOSIXTEST_H
#define CEPHPOSIXTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <XrdOuc/XrdOucIOVec.hh>

using namespace std;

class cephposixtest : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (cephposixtest);
    CPPUNIT_TEST (openTest);
    CPPUNIT_TEST (statTest);
    CPPUNIT_TEST (readTest);
    CPPUNIT_TEST (readVTest);
    CPPUNIT_TEST_SUITE_END ();

    public:
        void setUp (void);
		void tearDown (void);

    protected:
        void openTest (void);
        void statTest (void);
        void readTest (void);
        void readVTest(void);

    private:
        const char *file1, *file2, *file3, *longerfile;
        XrdOucIOVec *ioV;
};

#endif