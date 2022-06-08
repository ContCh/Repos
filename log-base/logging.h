#ifndef _LOGGING_H_
#define _LOGGING_H_

#include "log_base.h"
#include <string.h>
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LogMessageHeader(level)     LogMessage(LOG_LEVEL_##level, __FILENAME__, __LINE__)

#define LOG_IF(level, condition) \
        !(condition) ? (void)0 : DummyLogDecorator() & LogMessageHeader(level).stream()

#define LOG_IF_NOT(level, condition) \
        (condition) ? (void)0 : DummyLogDecorator() & LogMessageHeader(level).stream()

#define LOG(level)       LOG_IF(level, true)
#define CHECK(condition) LOG_IF_NOT(FATAL, condition)

#define CHECK_OP_IMPL(x, y, op, description) \
        LOG_IF_NOT(FATAL, (x) op (y)) << #x << description << #y << '.'

#define CHECK_EQ(x, y) CHECK_OP_IMPL(x, y, ==, " NOT EQ ")
#define CHECK_GT(x, y) CHECK_OP_IMPL(x, y, >,  " NOT GT ")
#define CHECK_GE(x, y) CHECK_OP_IMPL(x, y, >=, " NOT GE ")
#define CHECK_LT(x, y) CHECK_OP_IMPL(x, y, <,  " NOT LT ")
#define CHECK_LE(x, y) CHECK_OP_IMPL(x, y, <=, " NOT LE ")

#ifdef NDEBUG
#define DLOG(level)        LOG_IF(level, true)
#define DCHECK(condition)  CHECK(condition)
#define DCHECK_EQ(x, y)    CHECK_EQ(x, y)
#define DCHECK_GT(x, y)    CHECK_GT(x, y)
#define DCHECK_GE(x, y)    CHECK_GE(x, y)
#define DCHECK_LT(x, y)    CHECK_LT(x, y)
#define DCHECK_LE(x, y)    CHECK_LE(x, y)

#else
#define DLOG(level)        LOG_IF(level, false)
#define DCHECK(condition)  CHECK(true)
#define DCHECK_EQ(x, y)    CHECK(true)
#define DCHECK_GT(x, y)    CHECK(true)
#define DCHECK_GE(x, y)    CHECK(true)
#define DCHECK_LT(x, y)    CHECK(true)
#define DCHECK_LE(x, y)    CHECK(true)
#endif


#endif //_LOGGING_H_