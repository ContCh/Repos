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

class LogStream { //: public std::ostream {
 public:
    void setPrefix(uint32_t level, const char* file = nullptr, int line = -1);

    template <class Type>
    LogStream& operator<<(Type in) {
        oss << in;
        return *this;
    }
    void Flush();

 private:
    std::ostringstream oss;
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
    void operator&(const LogStream&) {}
};

#endif //_LOG_BASE_H
