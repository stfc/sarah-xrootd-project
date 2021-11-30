// CppUnit testing for XrdCephPosix library
// file: wrappertest.cc
// Author: Sarah Byrne
#include "cephposixtest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (cephposixtest);

void cephposixtest :: openTest (void)
{
    int fd1 = ceph_posix_open((XrdOucEnv *)NULL, "dteam:test/sarahbyrne/testfile", 0, (mode_t)NULL);
    CPPUNIT_ASSERT_EQUAL(0, fd1);
    int fd2 = ceph_posix_open((XrdOucEnv *)NULL, "dteam:test/ian_johnson/etc/motd", 0, (mode_t)NULL);
    CPPUNIT_ASSERT_EQUAL(1, fd2);
    int fd3 = ceph_posix_open((XrdOucEnv *)NULL, "dteam:test/sarahbyrne/thisisnotafile", 0, (mode_t)NULL);
    CPPUNIT_ASSERT(fd3 < 0);
}

void cephposixtest :: statTest (void)
{
    struct stat buf, *bufp = &buf;
    int rc1 = ceph_posix_stat((XrdOucEnv*)NULL, "dteam:test/sarahbyrne/testfile", bufp);
    CPPUNIT_ASSERT_EQUAL(23l, buf.st_size);
    int rc2 = ceph_posix_stat((XrdOucEnv*)NULL, "dteam:test/ian_johnson/etc/motd", bufp);
    CPPUNIT_ASSERT_EQUAL(287l, buf.st_size);

}

void cephposixtest :: readTest (void)
{
    int fd1 = ceph_posix_open((XrdOucEnv *)NULL, "dteam:test/sarahbyrne/testfile", 0, (mode_t)NULL);
    int fd2 = ceph_posix_open((XrdOucEnv *)NULL, "dteam:test/ian_johnson/etc/motd", 0, (mode_t)NULL);
    void *bufferp = (void *)malloc(1024);
    ssize_t a = ceph_posix_read(fd1, bufferp, 1024);
    stringstream ss((char *)bufferp);
    string firstline;
    getline(ss, firstline, '\n');
    string expectedline = "Hello my name is Sarah";
    string message = "First line of file incorrect";
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, expectedline, firstline);
}