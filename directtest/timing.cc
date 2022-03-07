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

int main(int argc, char* argv[]){
    //command line input processing
    long numreqs = stol(argv[2]);
    long reqsize = stol(argv[3]);
    long gapsize = stol(argv[4]);

    //create vector of requests
    vector<XrdOucIOVec> vecIO = big_vector_read_request(numreqs, reqsize, gapsize, false);
    XrdOucIOVec *ioV;
    ioV = new XrdOucIOVec[vecIO.size()];
    for (int i=0; i<(int)vecIO.size(); i++) ioV[i] = vecIO[i];

    ofstream outfile;
    string outfilename = "logging.txt";
    string timelog = argv[1];
    outfile.open(outfilename, ios_base::app);

    outfile << "Opening file for vector read." << endl;
    int fd = ceph_posix_open((XrdOucEnv *)NULL, "dteam:test/ian_johnson/file-10GiB", 0, (mode_t)NULL);
    if(fd>=0){outfile << "File opened successfully." << endl;}
    else{outfile << "Failed to open file." << endl; return 1;}
    outfile.close();

    outfile.open(timelog, ios_base::app);
    outfile << numreqs << ", " << reqsize << ", " << gapsize << " : ";
    outfile.close();

    time_ceph_posix_readV(outfilename, timelog, fd, ioV, vecIO.size());

    return 0;
}

