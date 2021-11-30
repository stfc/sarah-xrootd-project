// Author: Sarah Byrne

#include <iostream>
#include <fstream>

using namespace std;

int main(){
    ofstream outfile;
    outfile.open("lhcb-logging.txt", ios_base::app);

    outfile << "Read timed-out." << endl << endl;;
    return 0;
}