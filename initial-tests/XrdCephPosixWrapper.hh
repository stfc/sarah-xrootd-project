class XrdCephPosixWrapper {
   public:

   XrdCephPosixWrapper(std::string params);

   int Open(std::string filename, int flags);

   int Stat(std::string filename, struct stat* bufp);

   ssize_t Read(int fd, void *buf, size_t count);

};

