XRootD Project

All work on my Data Services project improving XRootD data access to Echo. The work is broken down into different folders which contain the following:

Folder name        | Contents
 :---              | :---
unit-test-template | A template for using CppUnit to create a test suite with associated documentation
initial-tests      | Initial unit tests using a wrapper class around XrdCephPosix, used to understand the library
test-xrd-ceph      | The test suite for the basic methods in XrdCephPosix including readV method
vread-fail-test    | A set of tests run to check for timeouts when performing vector reads
directtest         | The tests of performance directly calling XrdCephPosix
clienttest         | The tests of performance calling vector reads via an XRootD client
plotting           | The jupyter notebook used to create plots of performance testing data