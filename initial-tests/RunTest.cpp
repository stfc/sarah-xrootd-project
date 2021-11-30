// Main file for running ceph posix library tests
// Authhor: Sarah Byrne

#include "cephposixtest.cc"

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

int main(){
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