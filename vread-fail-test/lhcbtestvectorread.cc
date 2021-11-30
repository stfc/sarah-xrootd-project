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
    string outfilename = "verbose-lhcb-logging.txt";
    outfile.open(outfilename, ios_base::app);
    outfile << g_logstring << endl;
}

int main(){
    vector<vector<XrdOucIOVec>> vecIOvec = convert_to_vector_read_request("lhcb_vector_calls.py");

    ceph_posix_set_logfunc(logwrapper);

    ofstream outfile;
    string outfilename = "lhcb-long-logging.txt";
    outfile.open(outfilename, ios_base::app);
    outfile << "Beginning LHCb vector read test" << endl << endl;
    outfile.close();

    for(int ii=0; ii<vecIOvec.size(); ii++){
        XrdOucIOVec *ioV;
        ioV = new XrdOucIOVec[vecIOvec[ii].size()];
        for (int jj=0; jj<(int)vecIOvec[ii].size(); jj++) ioV[jj] = vecIOvec[ii][jj];

        ofstream outfile;
        outfile.open(outfilename, ios_base::app);

        outfile << "Opening file for vector read." << endl;
        int fd = ceph_posix_open((XrdOucEnv *)NULL, "dteam:test/sarahbyrne/biggerrandomtestfile", 0, (mode_t)NULL);
        if(fd>=0){outfile << "File opened successfully." << endl;}
        else{outfile << "Failed to open file." << endl; return 1;}

        time_ceph_posix_readV(outfilename, fd, ioV, vecIOvec[ii].size());
    }

    outfile.open(outfilename, ios_base::app);
    outfile << endl << "Completed LHCb vector read test" << endl << endl;
    outfile.close();

    return 0;
}