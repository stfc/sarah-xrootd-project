// CppUnit testing for XrdCephPosix library
// file: wrappertest.cc
// Author: Sarah Byrne
#include <XrdCephPosix.hh>
#include <XrdOuc/XrdOucEnv.hh>
#include <XrdOuc/XrdOucIOVec.hh>
#include <fcntl.h>
#include <sys/stat.h>
#include <sstream>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <cstring>

#include "cephposixtest.hh"
#include "vectorread.hh"

CPPUNIT_TEST_SUITE_REGISTRATION (cephposixtest);

void cephposixtest :: setUp (void)
{
    file1 = "dteam:test/sarahbyrne/testfile";
    longerfile = "dteam:test/sarahbyrne/longertestfile";
    file2 = "dteam:test/ian_johnson/etc/motd";
    file3 = "dteam:test/sarahbyrne/thisisnotafile";
    ioV = new XrdOucIOVec;
}

void cephposixtest :: tearDown (void)
{
    delete ioV;
    TestFixture::tearDown();
}

void cephposixtest :: openTest (void)
{
    int fd1 = ceph_posix_open((XrdOucEnv *)NULL, file1, 0, (mode_t)NULL);
    CPPUNIT_ASSERT_EQUAL(0, fd1);
    int fd2 = ceph_posix_open((XrdOucEnv *)NULL, file2, 0, (mode_t)NULL);
    CPPUNIT_ASSERT_EQUAL(1, fd2);
    int fd3 = ceph_posix_open((XrdOucEnv *)NULL, file3, 0, (mode_t)NULL);
    CPPUNIT_ASSERT(fd3 < 0);
}

void cephposixtest :: statTest (void)
{
    struct stat buf, *bufp = &buf;
    int rc1 = ceph_posix_stat((XrdOucEnv*)NULL, file1, bufp);
    CPPUNIT_ASSERT_EQUAL(23l, buf.st_size);
    int rc2 = ceph_posix_stat((XrdOucEnv*)NULL, file2, bufp);
    CPPUNIT_ASSERT_EQUAL(287l, buf.st_size);

}

void cephposixtest :: readTest (void)
{
    int fd1 = ceph_posix_open((XrdOucEnv *)NULL, file1, 0, (mode_t)NULL);
    int fd2 = ceph_posix_open((XrdOucEnv *)NULL, file2, 0, (mode_t)NULL);
    void *bufferp = (void *)malloc(1024);
    ssize_t a = ceph_posix_read(fd1, bufferp, 1024);
    stringstream ss((char *)bufferp);
    string firstline;
    getline(ss, firstline, '\n');
    string expectedline = "Hello my name is Sarah";
    string message = "First line of file incorrect";
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, expectedline, firstline);
}

void cephposixtest :: readVTest (void)
{
    // Potentially read file into memory (with ceph_posix_read) to test what is read by readV
    int fd1 = ceph_posix_open((XrdOucEnv *)NULL, longerfile, 0, (mode_t)NULL);
    //char *mmappedfile = (char *)mmap(NULL, 1000, PROT_READ, MAP_PRIVATE, fd1, 0);
    void *bufferp = (void *)malloc(1000);
    ssize_t b = ceph_posix_read(fd1, bufferp, 1024);
    char *rawdata = (char *)bufferp;

    // Create vector of read requests (potentailly add new function here to call to do this?)
    vector<XrdOucIOVec> vecIO = vector_read_request();
    ioV = new XrdOucIOVec[vecIO.size()];
    for (int i = 0; i < (int)vecIO.size(); i++) ioV[i] = vecIO[i];

    // Call the ceph_posix_readV function with our vector of read requests
    size_t a = ceph_posix_readV(fd1, ioV, vecIO.size());

    // Make assertions based on what is returned/what we have stored in memory as our test?
    // Do we also want to assert based on values of offset/size?
    for(int i=0; i<vecIO.size(); i++){
        char *expected = rawdata + ioV[i].offset;
        //cout << expected << endl;
        //cout << ioV[i].data << endl;
        int match = memcmp(expected, ioV[i].data, ioV[i].size);
        CPPUNIT_ASSERT_EQUAL(0, match);
    }
}