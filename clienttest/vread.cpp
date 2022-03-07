#include <iostream>
#include <XrdClient/XrdClient.hh>
#include <fstream>
#include <vector>
#include <string>
#include <sys/time.h>
#include <math.h>
#include <chrono>

#include "XrdClient/XrdClientEnv.hh"
#include "XrdSys/XrdSysHeaders.hh"

kXR_unt16 open_mode = (kXR_ur);
kXR_unt16 open_opts = (1);

/*
To compile:
Install xrootd-client-devel
g++ --std=c++11 vread.cpp -I/usr/include/xrootd -lXrdClient -o vread

To use:
get a grid/voms proxy
export the env varaiable: export XrdSecGSIUSERPROXY=[PROXYFILE]
run program, following usage instructions
e.g. ./vread root://ceph-gw1.gridpp.rl.ac.uk/dteam:test/tom_byrne/1g 10 100000 1000 logfile.txt
to do 1000, 10 byte reads, every 100KB through a file 
*/

int main(int argc, char* argv[])
{
  // check arg count and display a help message
  if (argc < 6){
    std::cout << "wrong number of arguments" << endl;
    std::cout << "usage: [file URL] [individual read length] [offset between reads] [number of reads] [time logging file]" << endl;
    return 1;
  }

  //std::cout << "parsing args" << endl;
  // parse command line args
  char *url = argv[1];
  long read_length = std::stoi(argv[2]);
  long read_offsets = std::stoi(argv[3]);
  long num_reads = std::stoi(argv[4]);
  string outfilename = argv[5];


  // calculate the last byte read to ensure the vread fits in the file
  long long max_extent = (read_offsets * num_reads) + read_length;

  std::cout << "opening file" << endl;
  // open file
  XrdClient *cli = new XrdClient(url);
  cli->Open(open_mode,open_opts);

  //std::cout << "stating file" << endl;
  // stat file
  XrdClientStatInfo *file_info = new XrdClientStatInfo();
  cli->Stat(file_info);

  // check the file is large enough for the requested vread
  if (max_extent > file_info->size){
    std::cout << "file size: " << file_info->size;
    std::cout << " B, maximum extent requested: " << max_extent << " B" << endl;
    std::cout << "vread extends past file end, exiting!" << endl;
    return 1;
  }
 
  std::cout << "generating buffer" << endl;
  // generate arrays for readV
  char* buffer = new char[num_reads * read_length];
  //std::cout << "generating offset array" << endl;
  long long* offsets = new long long[num_reads];
  //std::cout << "generating length array" << endl;
  int* lengths = new int[num_reads];
  //std::cout << "filling length and offset array" << endl;
  for (int i = 0; i < num_reads; i++){
    lengths[i] = read_length;
    offsets[i] = i * read_offsets;
  }

  std::cout << "starting readv" << endl;
  auto start = chrono::system_clock::now();
  cli->ReadV(buffer,offsets,lengths,num_reads);
  auto end = chrono::system_clock::now();
  chrono::duration<double> elapsed_seconds = end-start;
  //std::cout << buffer << endl;
  
  std::cout << "readv ended, writing time to file" << endl;
  ofstream outfile;
  outfile.open(outfilename, ios_base::app);
  outfile << read_length << "\t" << read_offsets << "\t" << num_reads << "\t" << elapsed_seconds.count() << endl;
  outfile.close();

  std::cout << "cleaning up memory" << endl;
  delete buffer;
  delete offsets;
  delete lengths;

  std::cout << "closing file" << endl;
  cli->Close();
 
  return 0;
}
