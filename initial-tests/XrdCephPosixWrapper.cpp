#include <XrdCephPosix.hh>
#include <XrdOuc/XrdOucEnv.hh>
#include <fcntl.h>
#include <sys/stat.h>
#include "wrappertest.cc"

#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/AdditionalMessage.h>
#include <cppunit/Message.h>

#include <iostream>
#include <string>

using namespace std;

//#pragma once

XrdCephPosixWrapper::XrdCephPosixWrapper(string params) {
    ceph_posix_set_defaults(params.c_str());
}

int XrdCephPosixWrapper::Open(string filename, int flags) {
    return ceph_posix_open((XrdOucEnv *)NULL, filename.c_str(), flags, (mode_t)NULL);
}

int XrdCephPosixWrapper::Stat(string filename, struct stat* bufp) {
   return ceph_posix_stat((XrdOucEnv*)NULL, filename.c_str(), bufp);
}

ssize_t XrdCephPosixWrapper::Read(int fd, void *buf, size_t count) {
    return ceph_posix_read(fd, buf, count);
}

#ifdef MAIN
int main(int argc, char* argv[]) {

    /*XrdCephPosixWrapper w("xrootd");

    int fd = w.Open(argc > 1 ? argv[1] : "dteam:test/sarahbyrne/testfile", O_RDONLY);

    (fd < 0 ? cerr : cout) << "open " << (fd < 0 ? "failed " : "succeeded ") << fd << endl;

    struct stat buf, *bufp = &buf;

    int rc = w.Stat(argc > 1 ? argv[1] : "dteam:test/ian_johnson/file-1GiB-01", bufp);
    if (rc == 0) {
        cout << "Size = " << buf.st_size << endl;
    }

    void *bufferp = (void *)malloc(10000);
    ssize_t something = w.Read(fd, bufferp, 1024);
    cout << "Number of bytes read from file: " << something << "\n";
    stringstream ss((char *)bufferp);
    string firstline;
    getline(ss, firstline, '\n');
    cout << firstline << endl;
    cout << string((char *)bufferp) << endl;

    cout << "My file fd: " << fd << endl;*/


    // informs listener about test results
    CPPUNIT_NS :: TestResult testresult;

    // register listener for collecting test-results
    CPPUNIT_NS :: TestResultCollector collectedresults;
	testresult.addListener (&collectedresults);

	// register listener for per-test progress output
	CPPUNIT_NS :: BriefTestProgressListener progress;
	testresult.addListener (&progress);

	// insert test-suite at test-runner by registry
	CPPUNIT_NS :: TestRunner testrunner;
	testrunner.addTest (CPPUNIT_NS :: TestFactoryRegistry :: getRegistry ().makeTest ());
	testrunner.run (testresult);

	// output results in compiler-format
	CPPUNIT_NS :: CompilerOutputter compileroutputter (&collectedresults, std::cerr);
	compileroutputter.write ();

    // return 0 if tests were successful
	return collectedresults.wasSuccessful () ? 0 : 1;

}
#endif
