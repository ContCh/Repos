#ifndef _LOG_BASE_H
#define _LOG_BASE_H

#include <iostream>
#include <string>

enum {
    LOG_LEVEL_INFO  = 0,
    LOG_LEVEL_WARN  = 1,
    LOG_LEVEL_ERROR = 2,
    LOG_LEVEL_TEMP  = 3,
    LOG_LEVEL_FATAL = 4
};

struct LogSettings {
    static uint32_t allow_output_log_level;
    static std::string output_destination;
    static bool log_to_stderr;
};

void logSettingInitialize(uint32_t    minimum_log_level = 0,
                          std::string destination       = "");

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

    void initLogStream(uint32_t level, const char* file = nullptr, int line = -1);

    std::ostream& stream();

    ~LogMessage();

 private:
    uint32_t  level_;
    LogStream log_stream_;
};


class DummyLogDecorator {
 public:
    void operator&(const std::ostream&) {}
};

#endif //_LOG_BASE_H
