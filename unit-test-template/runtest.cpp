// Main file for running ceph posix library tests
// Authhor: Sarah Byrne

#include "unittest.cc"

#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
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
    // This will give us overall results from the test suite
    CPPUNIT_NS :: TestResultCollector collectedresults;
	testresult.addListener (&collectedresults);

	// register listener for per-test progress output
    // This will give us results for each individual test
	CPPUNIT_NS :: BriefTestProgressListener progress;
	testresult.addListener (&progress);

	// insert test-suite at test-runner by registry
	CPPUNIT_NS :: TestRunner testrunner;
	testrunner.addTest (CPPUNIT_NS :: TestFactoryRegistry :: getRegistry ().makeTest ());
    // Run the tests
	testrunner.run (testresult);

	// output results in compiler-format
	CPPUNIT_NS :: CompilerOutputter compileroutputter (&collectedresults, std::cerr);
	compileroutputter.write ();

    // output results in xml-format
    std::ofstream myfile;
    myfile.open ("xmltestresults.xml");
    CPPUNIT_NS :: XmlOutputter xmloutputter (&collectedresults, myfile, std::string("ISO-8859-1"));
    xmloutputter.write ();
    myfile.close ();

    // return 0 if tests were successful
	return collectedresults.wasSuccessful () ? 0 : 1;
}

// Use the following command to compile and run this template test
// g++ -DMAIN runtest.cpp -lcppunit -o Test
// ./Test