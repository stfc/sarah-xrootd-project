#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sys/time.h>
#include <math.h>
#include <chrono>
#include <ctime>

/*
To compile:
gcc recordtime.cpp -o time

*/

using namespace std;

int main(int argc, char* argv[])
{
  // check arg count and display a help message
  if (argc < 3){
    std::cout << "wrong number of arguments" << endl;
    std::cout << "usage: [time logging file] [start/end]" << endl;
    return 1;
  }

  // parse command line args
  string outfilename = argv[1];
  string startend = argv[2];

  auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
  
  ofstream outfile;
  outfile.open(outfilename, ios_base::app);
  outfile << startend << " time " << ctime(&now) << endl;
  outfile.close();

  return 0;
}
