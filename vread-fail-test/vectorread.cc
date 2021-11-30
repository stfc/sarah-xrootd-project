// Supplementary functions for creating and iterating over vectors of read requests, and timing vector reads
// Author: Sarah Byrne

#include <XrdCephPosix.hh>
#include <XrdOuc/XrdOucEnv.hh>
#include <XrdOuc/XrdOucIOVec.hh>

#include <vector>
#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <cstring>
#include <time.h>
#include <chrono>
#include <ctime>
#include <thread>

#include "vectorread.hh"

using namespace std;

vector<XrdOucIOVec> vector_read_request(){
    // Function to create a small manual vector of read requests for testing
    vector<XrdOucIOVec> vecIO;
    XrdOucIOVec *ioV, ioItem;
    //set info as zero for all requests as this value will not be used
    ioItem.info = 0;

    ioItem.offset = 5; ioItem.size = 10;
    char *readdata1 = (char *)malloc(100); ioItem.data = readdata1;
    vecIO.push_back(ioItem);

    ioItem.offset = 38; ioItem.size = 12;
    char *readdata2 = (char *)malloc(100); ioItem.data = readdata2;
    vecIO.push_back(ioItem);

    ioItem.offset = 207; ioItem.size = 43;
    char *readdata3 = (char *)malloc(100); ioItem.data = readdata3;
    vecIO.push_back(ioItem);

    ioItem.offset = 805; ioItem.size = 25;
    char *readdata4 = (char *)malloc(100); ioItem.data = readdata4;
    vecIO.push_back(ioItem);

    ioItem.offset = 505; ioItem.size = 71;
    char *readdata5 = (char *)malloc(100); ioItem.data = readdata5;
    vecIO.push_back(ioItem);

    ioItem.offset = 972; ioItem.size = 17;
    char *readdata6 = (char *)malloc(100); ioItem.data = readdata6;
    vecIO.push_back(ioItem);

    return vecIO;
}

vector<XrdOucIOVec> big_vector_read_request(int numsubreqs, int subreqsize, int gapsize, bool randomness){
    // Functon to create a vector of read requests from input with option of adding 10% randomness to all values
    // Input: number of sub requests in vector, size of the sub requests, gap between each request and randomness bool

    vector<XrdOucIOVec> vecIO;
    XrdOucIOVec ioItem;
    //set info as zero for all requests as this value will not be used
    ioItem.info = 0;

    //set random seed
    srand(time(NULL));

    int totaloffset = 0;
    if(randomness){
        totaloffset += rand() % 3000000; //randomness will have starting offset somewhere within fist section of the file
        int a = numsubreqs/10;
        int b = rand() % (2*a);
        numsubreqs -= a; numsubreqs += b; //adds +/- 10% to number of sub requests
    }
    for(int ii=0; ii<numsubreqs; ii++){
        int reqsize = subreqsize;
        int gap = gapsize;
        if(randomness){
            int c = subreqsize/10;
            int d = rand() % (2*c);
            reqsize -= c; reqsize += d; //adds +/- 10% to request size per sub request - not all same
            int e = gapsize/10;
            int f = rand() % (2*e);
            gap -= e; gap += f; //adds +/- 10% to gap between requests per gap - not all same
        }
        //allocate memory - we are not doing anything with data so nothing more is done with this
        char *readdata = (char *)malloc(reqsize);
        ioItem.data = readdata;
        ioItem.offset = totaloffset;
        ioItem.size = reqsize;
        //add our requests to vector of requests
        vecIO.push_back(ioItem);
        totaloffset += gap+reqsize;
    }
    //return completed vector of requests
    return vecIO;
}

vector<vector<XrdOucIOVec>> convert_to_vector_read_request(string infilename){
    // Function to return a vector of vector read requests converted from a pyroot program
    // This assumes a specific format of the pyroot program as in lhcb_vector_calls.py - unsure how standard this is, hopefully very

    vector<vector<XrdOucIOVec>> vecIOvec;

    //read in pyroot file line by line
    ifstream infile(infilename);
    string line;
    while(getline(infile, line)){
        vector<XrdOucIOVec> vecIO;
        XrdOucIOVec ioItem;
        ioItem.info = 0;

        if(line[0] == 'v'){
            //find the vread values from multi reads
            while(line.length() > 20){
                //extract each pair of values from vector of (offset,size) pairs
                line = line.substr(line.find('(') + 1);
                int comma = line.find(',');
                long offset = stol(line.substr(0, comma));
                line = line.substr(comma+2);
                int bracket = line.find(')');
                int size = stol(line.substr(0, bracket));

                char *readdata = (char *)malloc(size);
                ioItem.data = readdata;
                ioItem.offset = offset;
                ioItem.size = size;
                vecIO.push_back(ioItem);
            }
            vecIOvec.push_back(vecIO);
            vecIO.clear();
        }

        if(line.rfind("f.read", 0) == 0){
            //find the vread values from singular reads
            size_t offsetstart = line.find('=') + 1;
            size_t offsetlen = line.find(',') - offsetstart;
            long offset = stol(line.substr(offsetstart, offsetlen));
            size_t sizestart = line.find(',') + 7;
            size_t sizelen = line.find(')') - sizestart;
            int size = stoi(line.substr(sizestart, sizelen));

            char *readdata = (char *)malloc(size);
            ioItem.data = readdata;
            ioItem.offset = offset;
            ioItem.size = size;
            vecIO.push_back(ioItem);

            vecIOvec.push_back(vecIO);
            vecIO.clear();
        }
    }

    return vecIOvec;
}

void time_ceph_posix_readV(string outfilename, int fd, XrdOucIOVec *ioV, int n){
    // Function to time eaach call of ceph-posix_readV

    //output start time to outfile
    auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());

    //open logging file and print start time
    ofstream outfile;
    outfile.open(outfilename, ios_base::app);
    outfile << "Beginning vector read " << " at " << ctime(&now);
    outfile.close();

    //time our vector read
    auto start = chrono::system_clock::now();

    //do read
    int a = ceph_posix_readV(fd, ioV, n);

    //end timer and output to outfile
    auto end = chrono::system_clock::now();
    outfile.open(outfilename, ios_base::app);
    outfile << "Vector read " << " successful" << endl;
    chrono::duration<double> elapsed_seconds = end-start;
    outfile << "Elapsed time: " << elapsed_seconds.count() << "s\n" << endl;
    outfile.close();
}

size_t getFilesize(const char* filename) {
    // Function to get the size of a local file
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}

void iterate_vector(vector<XrdOucIOVec> vecIO){
    // Function to iterate over a vector of read requests and print info on each request

    //loop though all read requests in vector and print values
    for(int i=0; i<vecIO.size(); i++){
        int datasize = vecIO[i].size;
        int dataoffset = vecIO[i].offset;

        //output of read request information
        std::cout << "Request " << i+1 << ":" << std::endl;
        std::cout << "Offset: " << dataoffset << std::endl;
        std::cout << "Size: " << datasize << std::endl;
    }
}

void local_file_read(vector<XrdOucIOVec> vecIO, const char* file){
    // Function to locally mimic the readV function and check returned data is correct

    //open the file - in testing with ceph this gets us the fd
    ifstream myfile; myfile.open(file);

    //let's try this memory mapping stuff
    size_t filesize = getFilesize(file);
    //open file and read into memory
    int fd = open(file, O_RDONLY, 0);
    char *mmappedfile = (char *)mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0);

    //check returned data for each sub request
    for(int i=0; i<vecIO.size(); i++){
        int datasize = vecIO[i].size;
        int dataoffset = vecIO[i].offset;

        //equivalent to the readV method where we will then inspect the buffer
        myfile.seekg(dataoffset, ios_base::beg);
        char *data = (char *)malloc(datasize+1);
        myfile.read(data, datasize);
        data[datasize] = 0;

        //read correct range from mmappeddata for comparison
        char *extracted = mmappedfile + dataoffset; //new buffer with data starting at offset for comparison

        //check data is same for range of requested bytes
        int match = memcmp(extracted, data, datasize);
        if(match != 0){cout << "Data did not match - incorrect read for request " << i << endl;}
    }
}

void wait_20(){
    this_thread::sleep_for(20s);
}

void wait_40(){
    this_thread::sleep_for(40s);
}