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
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>
#include <thread>
#include <vector>

#include "vectorread.hh"

using namespace std;

char g_logstring[1024];
static void logwrapper(char *format, va_list argp) {
    vsnprintf(g_logstring, 1024, format, argp);
    ofstream outfile;
    string outfilename = "verbose-logging.txt";
    outfile.open(outfilename, ios_base::app);
    outfile << g_logstring << endl;
    //cout << g_logstring << endl;
}

int main(){
    ceph_posix_set_logfunc(logwrapper);
    //create vector of requests
    vector<XrdOucIOVec> vecIO = big_vector_read_request(200, 100000, 150000, true);
    XrdOucIOVec *ioV;
    ioV = new XrdOucIOVec[vecIO.size()];
    for (int i=0; i<(int)vecIO.size(); i++) ioV[i] = vecIO[i];

    //iterate_vector(vecIO);
    //local_file_read(vecIO, "randomtestfile");

    ofstream outfile;
    string outfilename = "logging.txt";
    outfile.open(outfilename, ios_base::app);

    outfile << "Opening file for vector read." << endl;
    int fd = ceph_posix_open((XrdOucEnv *)NULL, "dteam:test/sarahbyrne/randomtestfile", 0, (mode_t)NULL);
    if(fd>=0){outfile << "File opened successfully." << endl;}
    else{outfile << "Failed to open file." << endl; return 1;}

    time_ceph_posix_readV(outfilename, fd, ioV, vecIO.size());
    //thread th1(ceph_posix_readV, fd, ioV, vecIO.size());
    //th1.join();

    return 0;
}