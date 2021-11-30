// Code to create and iterate over a vector of read requests
// Author: Sarah Byrne

#ifndef VECTORREAD_H
#define VECTORREAD_H

std::vector<XrdOucIOVec> vector_read_request();
std::vector<XrdOucIOVec> big_vector_read_request(int numsubreqs, int subreqsize, int gapsize, bool randomness);
std::vector<std::vector<XrdOucIOVec>> convert_to_vector_read_request(std::string infilename);
size_t getFilesize(const char* filename);
void iterate_vector(std::vector<XrdOucIOVec> vecIO);
void local_file_read(std::vector<XrdOucIOVec> vecIO, const char* file);
void time_ceph_posix_readV(std::string outfilename, int fd, XrdOucIOVec *ioV, int n);
void wait_20();
void wait_40();

#endif