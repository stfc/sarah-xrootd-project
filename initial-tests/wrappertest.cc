// CppUnit testing for XrdCephWrapper class
// file: wrappertest.cc
// Author: Sarah Byrne
#include "wrappertest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (wrappertest);

void wrappertest :: setUp (void)
{
    w = new XrdCephPosixWrapper ("xrootd");
}

void wrappertest :: tearDown (void)
{
    delete w;
}

void wrappertest :: openTest (void)
{
    //XrdCephPosixWrapper w("xrootd");
    int fd1 = w->Open("dteam:test/sarahbyrne/testfile", 0);
    CPPUNIT_ASSERT_EQUAL(0, fd1);
    int fd2 = w->Open("dteam:test/sarahbyrne/testfile", 0);
    CPPUNIT_ASSERT_EQUAL(1, fd2);
}

void wrappertest :: statTest (void)
{
    struct stat buf, *bufp = &buf;
    int rc1 = w->Stat("dteam:test/sarahbyrne/testfile", bufp);
    CPPUNIT_ASSERT_EQUAL(23l, buf.st_size);
    int rc2 = w->Stat("dteam:test/ian_johnson/etc/motd", bufp);
    CPPUNIT_ASSERT_EQUAL(287l, buf.st_size);

}

void wrappertest :: readTest (void)
{
    void *bufferp = (void *)malloc(10000);
    int fd1 = w->Open("dteam:test/sarahbyrne/testfile", 0);
    ssize_t a = w->Read(fd1, bufferp, 1024);
    stringstream ss((char *)bufferp);
    string firstline;
    getline(ss, firstline, '\n');
    string expectedline = "Hello my name is Satan";
    string message = "First line of file incorrect";
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, expectedline, firstline);
}