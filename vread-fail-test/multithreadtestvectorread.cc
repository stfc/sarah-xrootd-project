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

int main(){
    //create arrays with all suggested values of no. sub requests, read sizes and gaps
    int possnumsubreqs[5] = {100, 200, 500, 1000, 2000};
    int posssubreqsizes[5] = {1000, 5000, 10000, 50000, 100000};
    int possgapsizes[2] = {1000, 150000};

    //set random seed
    srand(time(NULL));

    //create vectors of requests (4 as we have 4 available threads)
    vector<XrdOucIOVec> vecIO_1 = big_vector_read_request(possnumsubreqs[0], posssubreqsizes[(rand() % 5)], possgapsizes[(rand() % 2)], true);
    vector<XrdOucIOVec> vecIO_2 = big_vector_read_request(possnumsubreqs[2], posssubreqsizes[(rand() % 5)], possgapsizes[(rand() % 2)], true);
    vector<XrdOucIOVec> vecIO_3 = big_vector_read_request(possnumsubreqs[(rand() % 5)], posssubreqsizes[(rand() % 5)], possgapsizes[(rand() % 2)], true);
    vector<XrdOucIOVec> vecIO_4 = big_vector_read_request(possnumsubreqs[(rand() % 5)], posssubreqsizes[(rand() % 5)], possgapsizes[(rand() % 2)], true);
    XrdOucIOVec *ioV_1, *ioV_2, *ioV_3, *ioV_4;
    ioV_1 = new XrdOucIOVec[vecIO_1.size()];
    ioV_2 = new XrdOucIOVec[vecIO_2.size()];
    ioV_3 = new XrdOucIOVec[vecIO_3.size()];
    ioV_4 = new XrdOucIOVec[vecIO_4.size()];
    for (int i=0; i<(int)vecIO_1.size(); i++) ioV_1[i] = vecIO_1[i];
    for (int i=0; i<(int)vecIO_2.size(); i++) ioV_2[i] = vecIO_2[i];
    for (int i=0; i<(int)vecIO_3.size(); i++) ioV_3[i] = vecIO_3[i];
    for (int i=0; i<(int)vecIO_4.size(); i++) ioV_4[i] = vecIO_4[i];

    //iterate_vector(vecIO);
    //local_file_read(vecIO, "randomtestfile");

    //unsigned int n = thread::hardware_concurrency();
    //cout << n << endl;

    ofstream outfile;
    outfile.open("multi-logging.txt", ios_base::app);

    outfile << "Opening file for vector read." << endl;
    int fd = ceph_posix_open((XrdOucEnv *)NULL, "dteam:test/sarahbyrne/randomtestfile", 0, (mode_t)NULL);
    if(fd>=0){outfile << "File opened successfully." << endl;}
    else{outfile << "Failed to open file." << endl; return 1;}

    //thread th1(time_ceph_posix_readV, "multi-logging.txt", fd, ioV_1, vecIO_1.size());
    //thread th2(time_ceph_posix_readV, "multi-logging.txt", fd, ioV_2, vecIO_2.size());
    //thread th3(time_ceph_posix_readV, "multi-logging.txt", fd, ioV_3, vecIO_3.size());
    //thread th4(time_ceph_posix_readV, "multi-logging.txt", fd, ioV_4, vecIO_4.size());
    thread th1(wait_20);
    thread th2(wait_40);
    cout << "All threads started." << endl;
    th1.join();
    cout << "Thread 1 re-joined." << endl;
    th2.join();
    cout << "Thread 2 re-joined." << endl;
    //th3.join();
    //cout << "Thread 3 re-joined." << endl;
    //th4.join();
    //cout << "Thread 4 re-joined." << endl;

    /*ofstream timingfile;
    timingfile.open("timelog.txt", ios_base::app);
    timingfile << elapsed_seconds.count() << ", ";*/

    return 0;
}