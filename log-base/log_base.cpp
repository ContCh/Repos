#include "log_base.h"
#include <ctime>
#include <sys/time.h>

/**********  Common parameters and functions for log  ***********/
const char* color_start[] = {
    "\033[0;32m",  // green
    "\033[0;33m",  // yellow
    "\033[0;31m",  // red
    "\033[0;34m",  // blue
    "\033[0;31m",  // red
};

const char color_end[] = "\033[0m";

const char* log_level_str[] = {
    "INFO", "WARN", "ERROR", "TEMP", "FATAL"
};

void getCurrentTime(char* time_buf) {
    time_t cur_time   = time(NULL);
    tm*    rough_time = localtime(&cur_time);
    struct timeval tv;
    gettimeofday(&tv, NULL);
    sprintf(time_buf, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
            rough_time->tm_year + 1900, rough_time->tm_mon + 1, rough_time->tm_mday,
            rough_time->tm_hour,        rough_time->tm_min,     rough_time->tm_sec,
            static_cast<int>(tv.tv_usec / 1000));
}

/**************************************************************************************/
LogStream::LogStream()
    : stream_buf_(buffer_, MaxBufferLen) {
    rdbuf(&stream_buf_);
}

void LogStream::setPrefix(uint32_t level, const char* file, int line) {
    char cur_time[40];
    getCurrentTime(cur_time);
    (*this) << color_start[level] << "[" << log_level_str[level] << "]["
            << cur_time << ']';
    if (file != nullptr) {
        (*this) << '[' << file << ':' << line << ']';
    }
    (*this) << color_end << ' ';
}

void LogStream::Flush() {
    std::cerr << buffer_ << std::flush;
}

LogMessage::LogMessage(uint32_t level) : level_(level) {
    log_stream_.setPrefix(level);
}

LogMessage::LogMessage(uint32_t level, const char* file, int line)
    : level_(level) {
    log_stream_.setPrefix(level, file, line);
}

LogStream& LogMessage::stream() {
    return this->log_stream_;
}

LogMessage::~LogMessage() {
    log_stream_.Flush();
    if (level_ >= LOG_LEVEL_FATAL) {
        abort();
    }
}
