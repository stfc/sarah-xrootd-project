#include <XrdCephPosix.hh>
#include <XrdOuc/XrdOucEnv.hh>
#include <fcntl.h>
#include <sys/stat.h>

#include <iostream>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

int main(int argc, char* argv[]) {



   //[[userId@]pool[,nbStripes[,stripeUnit[,objectSize]]]:]<actual path>


   const char* parms = "xrootd";

   try {
      ceph_posix_set_defaults(parms);
   } catch (exception &e) {
      cerr << "CephOss loading failed with exception " << e.what() << endl 
         <<  "Check the syntax of parameters : " <<  parms << endl;
      exit(-1);
   }  
   string filename;
   if (argc > 1) {
      filename = argv[1];
   } else {
      filename = "dteam:test/ian_johnson/file-1GiB-01";
   }
   int fd = ceph_posix_open((XrdOucEnv *)NULL, filename.c_str(), O_RDONLY, (mode_t)NULL);
   if (fd < 0) {
      cerr << "ceph_posix_open(" << filename.c_str() << " failed with code " << fd << endl;
   }
   int fd2 = ceph_posix_open((XrdOucEnv *)NULL, filename.c_str(), O_RDONLY, (mode_t)NULL);
   if (fd2 < 0) {
      cerr << "ceph_posix_open(" << filename.c_str() << " failed with code " << fd << endl;
   } else {
      cout << "fd2 is " << fd2 << endl;
   }

   struct stat statbuf;
   struct stat* stbufp = &statbuf;
 
   int rc = ceph_posix_stat((XrdOucEnv*)NULL, filename.c_str(), stbufp);

   // if (rc != 0) { // This never gets to the following line, despite rc being 0!
   //    cout << "Size = " << statbuf.st_size << endl;
   // } else {
   //    cerr << "Error code = " << rc << endl;
   //    cout << "Size = " << statbuf.st_size << endl;
   // }

   if (rc == 0) {
      cout << "Size = " << statbuf.st_size << endl;
   } else {
      cerr << "Error code = " << rc << endl;
   }  


}
