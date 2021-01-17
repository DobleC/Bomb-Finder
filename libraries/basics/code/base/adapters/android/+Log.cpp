/*
 * LOG
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712172130
 */

#include <basics/Log>
#include <basics/macros>
#include <android/log.h>

#if defined(BASICS_ANDROID_OS)

    namespace basics
    {

        static const android_LogPriority android_log_priorities[] =
        {
            ANDROID_LOG_VERBOSE,
            ANDROID_LOG_DEBUG,
            ANDROID_LOG_INFO,
            ANDROID_LOG_WARN,
            ANDROID_LOG_ERROR,
            ANDROID_LOG_FATAL,
        };

        void Log::dump (Level level, const char * tag, const char * cstring)
        {
            __android_log_write (android_log_priorities[level], tag ? tag : "*", cstring);
        }

        Log log;

    }

#endif
