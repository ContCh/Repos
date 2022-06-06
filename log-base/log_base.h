#ifndef _LOG_BASE_H
#define _LOG_BASE_H

#include <iostream>
#include <sstream>

enum {
    LOG_LEVEL_INFO  = 0,
    LOG_LEVEL_WARN  = 1,
    LOG_LEVEL_ERROR = 2,
    LOG_LEVEL_TEMP  = 3,
    LOG_LEVEL_FATAL = 4
};

class LogStreamBuffer : public std::streambuf {
public:
    LogStreamBuffer(char* buf_begin, int len) {
        setp(buf_begin, buf_begin + len - 2);
    }

    size_t pcount() const { return pptr() - pbase(); }
    char* pbase() const { return std::streambuf::pbase(); }
};

constexpr int MaxBufferLen = 1024;

class LogStream : public std::ostream { //: public std::ostream {
 public:
    LogStream();
    void setPrefix(uint32_t level, const char* file = nullptr, int line = -1);

    void Flush();

 private:
    char buffer_[MaxBufferLen];
    LogStreamBuffer stream_buf_;
    // Avoid copy construct.
    LogStream& operator=(const LogStream&);
    LogStream(const LogStream&);
};

class LogMessage {
 public:
    LogMessage(uint32_t level);

    LogMessage(uint32_t level, const char* file, int line);

    LogStream& stream();

    ~LogMessage();

 private:
    uint32_t  level_;
    LogStream log_stream_;
};


class DummyLogDecorator {
 public:
    // Why I cannot use LogStream& ?
    void operator&(const std::ostream&) {}
};

#endif //_LOG_BASE_H
