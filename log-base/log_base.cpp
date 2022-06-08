#include "log_base.h"
#include <ctime>
#include <sys/time.h>

uint32_t LogSettings::allow_output_log_level = 0;
std::string LogSettings::output_destination = "";
bool LogSettings::log_to_stderr = true;

void logSettingInitialize(uint32_t minimum_log_level, std::string destination) {
    minimum_log_level = minimum_log_level < static_cast<uint32_t>(LOG_LEVEL_FATAL) ?
                        minimum_log_level : static_cast<uint32_t>(LOG_LEVEL_FATAL);
    LogSettings::allow_output_log_level = minimum_log_level;
    LogSettings::output_destination     = destination;
    if (FILE* fp = fopen(LogSettings::output_destination.c_str(), "w")) {
        LogSettings::log_to_stderr = false;
        fclose(fp);
    } else {
        printf("The file path not initialize, target path not exist. \033[0;33mWARNING\033[0m : %s",
               "Use default output path [stderr].\n");
    }
}

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

void LogStream::Flush() {
    FILE* fp = LogSettings::log_to_stderr ? stderr :
                                            fopen(LogSettings::output_destination.c_str(), "a");
    size_t num_chars = stream_buf_.pcount();
    char* last_char = stream_buf_.pbase() + num_chars;
    if (*(last_char - 1) != '\n') {
        *last_char = '\n';
        num_chars++;
    }
    fwrite(stream_buf_.pbase(), 1, num_chars, fp);
    if (fp == stderr) {
        std::fflush(stderr);
    } else {
        fclose(fp);
    }
}

void LogMessage::initLogStream(uint32_t level, const char* file, int line) {
    log_stream_.flush();
    char cur_time[32];
    getCurrentTime(cur_time);
    if (LogSettings::log_to_stderr) {
        log_stream_ << color_start[level] << '[' << log_level_str[level] << ']'
                    << color_end;
    } else {
        log_stream_ << '[' << log_level_str[level] << ']';
    }
    log_stream_ << '[' << cur_time << "][" << file << ':' << line << "] ";
}

LogMessage::LogMessage(uint32_t level) : level_(level) {
    initLogStream(level);
}

LogMessage::LogMessage(uint32_t level, const char* file, int line)
    : level_(level) {
    initLogStream(level, file, line);
}

std::ostream& LogMessage::stream() {
    return this->log_stream_;
}

LogMessage::~LogMessage() {
    if (level_ >= LogSettings::allow_output_log_level) {
        log_stream_.Flush();
    }
    if (level_ >= LOG_LEVEL_FATAL) {
        printf("...exec failed...\n");
        abort();
    }
}
